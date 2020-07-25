/*
 * conexiones.c
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#include"conexiones.h"

/* Me conecto a Broker
 * Creo sockets de escucha de NEW_POKEMON, CATCH_POKEMON, GET_POKEMON
 * Guardo sockets envío de APPEARED_POKEMON, CAUGHT_POKEMON, LOCALIZED_POKEMON
 */
int generarSocketsConBroker() {
	socketBroker = crearSocket();
	int puertoBrokerInt = atoi(PUERTO_BROKER);

	log_info(logger, "Intentando conectarme a Broker...");

	while (conectarA(socketBroker, IP_BROKER, puertoBrokerInt) != 1) {
		log_info(logger, "No se logro conexión a Broker, se reintentará conexión en %d segundos...", TIEMPO_DE_REINTENTO_CONEXION);
		sleep(TIEMPO_DE_REINTENTO_CONEXION);
	}

	t_handshake* handshakePropio = malloc(sizeof(t_handshake));
	handshakePropio->id = GAMECARD;
	handshakePropio->idUnico = ID_UNICO;

	t_handshake* handshakeResponse;
	handshakeResponse = responderHandshake(socketBroker, handshakePropio);
	free(handshakePropio);
	free(handshakeResponse);

	socketNewPokemon = crearSocket();
	socketCatchPokemon = crearSocket();
	socketGetPokemon = crearSocket();

	socketAppearedPokemon = crearSocket();
	socketCaughtPokemon = crearSocket();
	socketLocalizedPokemon = crearSocket();

	if (conectarA(socketNewPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,"Suscripto a cola New Pokemon. Lanzando socket de escucha..");
	}
	else{
		return -1;
	}

	if (conectarA(socketCatchPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,"Suscripto a cola Catch Pokemon. Lanzando socket de escucha..");
	}
	else{
		return -1;
	}

	if (conectarA(socketGetPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,"Suscripto a cola Get Pokemon. Lanzando socket de escucha..");

	}
	else{
		return -1;
	}

	if (conectarA(socketAppearedPokemon, IP_BROKER, puertoBrokerInt)) { log_debug(logger, "Socket de Appeared Pokemon guardado.");}
	else{return -1;}
	if (conectarA(socketCaughtPokemon, IP_BROKER, puertoBrokerInt)) { log_debug(logger, "Socket de Caught Pokemon guardado.");}
	else{return -1;}
	if (conectarA(socketLocalizedPokemon, IP_BROKER, puertoBrokerInt)) { log_debug(logger, "Socket de Localized Pokemon guardado.");}
	else{return -1;}

	return 1;
}

void lanzarHilosDeEscucha(){

	log_debug(logger, "START: lanzarHilosEscucha");
	pthread_t escucharNewPokemon, escucharCatchPokemon, escucharGetPokemon;
	pthread_create(&escucharNewPokemon, NULL, (void*) escucharColaNewPokemon, NULL);
	pthread_create(&escucharCatchPokemon, NULL, (void*) escucharColaCatchPokemon, NULL);
	pthread_create(&escucharGetPokemon, NULL, (void*) escucharColaGetPokemon, NULL);

	pthread_join(escucharNewPokemon, NULL);
	pthread_join(escucharCatchPokemon, NULL);
	pthread_join(escucharGetPokemon, NULL);

	log_debug(logger, "END: lanzarHilosEscucha");
}

void escucharGameBoy(){
	log_debug(logger, "<> START: Comienzo a escuchar GameBoy <>");
	socketListenerGameBoy = crearSocket();
	int puertoGamecardInt = atoi(PUERTO_GAMECARD);
	if(escuchaEn(socketListenerGameBoy, puertoGamecardInt)){
				log_debug(logger, "Escuchando conexiones del GameBoy...");
	}

	while(1){
		log_info(logger, "Esperando conexion de GameBoy...");
		int socketGameBoy = aceptarConexion(socketListenerGameBoy);

		t_handshake* handshakePropio = malloc(sizeof(t_handshake));
		handshakePropio->id = GAMECARD;
		handshakePropio->idUnico = ID_UNICO;

		t_handshake* handshakeGameboy = iniciarHandshake(socketGameBoy, handshakePropio);
		log_info(logger, "Me conecté con %s", ID_PROCESO[handshakeGameboy->id]);

		free(handshakePropio);
		free(handshakeGameboy);

		t_paquete* paqueteNuevo = recibir_mensaje(socketGameBoy);

		if(paqueteNuevo == NULL){
			log_info(logger, "Se ha roto la conexión con GameBoy. Continuando ejecución.");
			close(socketGameBoy);
			continue;
		}

		log_info(logger, "<><> Nuevo mensaje de GAMEBOY recibido <><>");
		log_info(logger, "Tipo de mensaje: %s", COLAS_STRING[paqueteNuevo->codigo_operacion]);
		log_info(logger, "ID del mensaje: %d", paqueteNuevo->ID);
		log_info(logger, "ID correlativo del mensaje: %d", paqueteNuevo->ID_CORRELATIVO);
		log_debug(logger,"Tamaño de payload: %d", paqueteNuevo->buffer->size);

		switch(paqueteNuevo->codigo_operacion){
			case NEW_POKEMON:
			log_debug(logger, "<><> Mensaje NEW_POKEMON recibido <><>");

				t_new_pokemon* new_pokemon = paqueteNuevo->buffer->stream;
				pthread_t hiloProcesarNewPokemon;
				pthread_create(&hiloProcesarNewPokemon, NULL, (void*) procesarNewPokemon, (void *) new_pokemon);
				pthread_join(hiloProcesarNewPokemon, NULL);

				t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
				appeared_pokemon->lengthOfPokemon = new_pokemon->lengthOfPokemon;
				appeared_pokemon->pokemon = new_pokemon->pokemon;
				appeared_pokemon->posicion = new_pokemon->posicion;
				log_debug(logger, "Por enviar APPEARED_POKEMON | Pokemon: %s, Posicion: (%d, %d)", appeared_pokemon->pokemon, appeared_pokemon->posicion->posicionX, appeared_pokemon->posicion->posicionY);

				if (enviar_appeared_pokemon(appeared_pokemon, socketAppearedPokemon, -1, paqueteNuevo->ID) == -1){
					log_info(logger, "No se pudo enviar el mensaje a Broker.");
				}

				free(new_pokemon);
				break;

			case CATCH_POKEMON:
				log_debug(logger, "<><> Mensaje CATCH_POKEMON recibido <><>");

				t_resultado_catch* argumentos_localized = malloc(sizeof(t_resultado_catch*));
				argumentos_localized->catch_pokemon = paqueteNuevo->buffer->stream;
				argumentos_localized->resultado = 0;

				pthread_t hiloProcesarCatchPokemon;
				pthread_create(&hiloProcesarCatchPokemon, NULL, (void*) procesarCatchPokemon, argumentos_localized);

				pthread_join(hiloProcesarCatchPokemon, NULL);

				log_debug(logger, "Sale de procesar catch");

				t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));
				caught_pokemon->ok = argumentos_localized->resultado;

				if(argumentos_localized->resultado){
					log_info(logger, "Resultado CAUGHT_POKEMON %s: [ATRAPADO]", argumentos_localized->catch_pokemon->pokemon);
				} else{
					log_info(logger, "Resultado CAUGHT_POKEMON %s: [NO ATRAPADO]", argumentos_localized->catch_pokemon->pokemon);
				}

				if (enviar_caught_pokemon(caught_pokemon, socketCaughtPokemon, -1, paqueteNuevo->ID) == -1){
					log_info(logger, "No se pudo enviar el mensaje a Broker.");
				}else{
					log_info(logger, "<> Mensaje CAUGHT_POKEMON enviado <>");
				}

				free(argumentos_localized->catch_pokemon->posicion);
				free(argumentos_localized->catch_pokemon->pokemon);
				free(argumentos_localized->catch_pokemon);
				free(argumentos_localized);
				break;

			case GET_POKEMON:
				log_debug(logger, "<><> Mensaje GET_POKEMON recibido <><>");


				t_argumentos_procesar_get* argumentos = malloc(sizeof(t_argumentos_procesar_get));
				argumentos->get_pokemon = paqueteNuevo->buffer->stream;

				pthread_t hiloProcesarGetPokemon;
				pthread_create(&hiloProcesarGetPokemon, NULL, (void*) procesarGetPokemon, argumentos);

				log_debug(logger, "Value de socket %d", socketLocalizedPokemon);

				pthread_join(hiloProcesarGetPokemon, NULL);
				t_localized_pokemon* localized_pokemon = argumentos->puntero_a_localized_pokemon;

				if (enviar_localized_pokemon(localized_pokemon, socketLocalizedPokemon, -1, paqueteNuevo->ID) == -1){
					log_info(logger, "No se pudo enviar el mensaje a Broker.");
				}


				free(paqueteNuevo->buffer->stream);
				free(argumentos);

				break;
			default:
				log_info(logger, "Tipo de mensaje invalido.");
		}

		free(paqueteNuevo->buffer);
		free(paqueteNuevo);
		close(socketGameBoy);
	}

	close(socketListenerGameBoy);
}

void* escucharColaNewPokemon(){

	while(1){
		log_info(logger, "Esperando mensajes NEW_POKEMON...");

		t_paquete* paqueteNuevo = recibir_mensaje(socketNewPokemon);

		if(paqueteNuevo == NULL){
			log_debug(logger, "Se desconecto el socket de NEW_POKEMON");
			desconexion = 1;
			pthread_mutex_lock(&semaforoDesconexion);
			reconectarABroker();
			pthread_mutex_unlock(&semaforoDesconexion);
		}

		else{

			log_info(logger, "<><> Mensaje NEW_POKEMON recibido <><>");

			if (enviar_ACK(socketNewPokemon, -1, paqueteNuevo->ID) == -1){
				log_info(logger, "No se pudo enviar el mensaje a Broker.");
			}else{
				log_info(logger, "Se envió el ACK de NEW_POKEMON");
			}

			t_new_pokemon* new_pokemon = paqueteNuevo->buffer->stream;
			pthread_t hiloProcesarNewPokemon;
			pthread_create(&hiloProcesarNewPokemon, NULL, (void*) procesarNewPokemon, (void *) new_pokemon);
			pthread_join(hiloProcesarNewPokemon, NULL);

			t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
			appeared_pokemon->lengthOfPokemon = new_pokemon->lengthOfPokemon;
			appeared_pokemon->pokemon = new_pokemon->pokemon;
			appeared_pokemon->posicion = new_pokemon->posicion;

			char* nombreDuplicado = string_duplicate(appeared_pokemon->pokemon);

			if(enviar_appeared_pokemon(appeared_pokemon, socketAppearedPokemon, -1, paqueteNuevo->ID) == -1){
				log_info(logger, "No se pudo enviar el mensaje a Broker.");
			}
			else{
				log_info(logger, "<> Mensaje APPEARED_POKEMON %s enviado <>", nombreDuplicado);
			}

			free(nombreDuplicado);
			free(new_pokemon);
			free(paqueteNuevo->buffer);
			free(paqueteNuevo);
		}
	}
}

void* escucharColaCatchPokemon(){

	while(1){
		log_info(logger, "Esperando mensajes CATCH_POKEMON...");
		t_paquete* paqueteNuevo = recibir_mensaje(socketCatchPokemon);

		if(paqueteNuevo == NULL){
			log_debug(logger, "Se desconecto el socket de CATCH_POKEMON");
			desconexion = 1;
			pthread_mutex_lock(&semaforoDesconexion);
			reconectarABroker();
			pthread_mutex_unlock(&semaforoDesconexion);
		}

		else{

			log_info(logger, "<><> Mensaje CATCH_POKEMON recibido <><>");

			if(enviar_ACK(socketCatchPokemon, -1, paqueteNuevo->ID) == -1){
				log_info(logger, "No se pudo enviar el mensaje a Broker.");
			}else{
				log_info(logger, "Se envió el ACK de CATCH_POKEMON");
			}

			t_resultado_catch* argumentos_localized = malloc(sizeof(t_resultado_catch*));
			argumentos_localized->catch_pokemon = paqueteNuevo->buffer->stream;
			argumentos_localized->resultado = 0;

			pthread_t hiloProcesarCatchPokemon;
			pthread_create(&hiloProcesarCatchPokemon, NULL, (void*) procesarCatchPokemon, (void *) argumentos_localized);

			pthread_join(hiloProcesarCatchPokemon, NULL);

			t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));
			caught_pokemon->ok = argumentos_localized->resultado;

			if(argumentos_localized->resultado){
				log_info(logger, "Resultado CAUGHT_POKEMON %s: [ATRAPADO]", argumentos_localized->catch_pokemon->pokemon);
			} else{
				log_info(logger, "Resultado CAUGHT_POKEMON %s: [NO ATRAPADO]", argumentos_localized->catch_pokemon->pokemon);
			}

			if(enviar_caught_pokemon(caught_pokemon, socketCaughtPokemon, -1, paqueteNuevo->ID) == -1){
				log_info(logger, "No se pudo enviar el mensaje a Broker.");
			}
			else{
				log_info(logger, "<> Mensaje CAUGHT_POKEMON enviado <>");
			}

			free(argumentos_localized->catch_pokemon->posicion);
			free(argumentos_localized->catch_pokemon->pokemon);
			free(argumentos_localized->catch_pokemon);
			free(argumentos_localized);
			free(paqueteNuevo->buffer);
			free(paqueteNuevo);
		}
	}
}

void* escucharColaGetPokemon(){

	while(1){
		log_info(logger, "Esperando mensajes GET_POKEMON...");
		t_paquete* paqueteNuevo = recibir_mensaje(socketGetPokemon);

		if (paqueteNuevo == NULL) {
			log_debug(logger, "Se desconecto el socket de GET_POKEMON");
			desconexion = 1;
			pthread_mutex_lock(&semaforoDesconexion);
			reconectarABroker();
			pthread_mutex_unlock(&semaforoDesconexion);
		}

		else {

			log_info(logger, "<><> Mensaje GET_POKEMON recibido <><>");

			if(enviar_ACK(socketGetPokemon, -1, paqueteNuevo->ID) == -1){
				log_info(logger, "No se pudo enviar el mensaje a Broker.");
			}else{
				log_info(logger, "Se envió el ACK de GET_POKEMON");
			}

			t_argumentos_procesar_get* argumentos = malloc(sizeof(t_argumentos_procesar_get));
			argumentos->get_pokemon = paqueteNuevo->buffer->stream;

			pthread_t hiloProcesarGetPokemon;
			pthread_create(&hiloProcesarGetPokemon, NULL, (void*) procesarGetPokemon, argumentos);

			pthread_join(hiloProcesarGetPokemon, NULL);
			t_localized_pokemon* localized_pokemon = argumentos->puntero_a_localized_pokemon;

			char* nombreDuplicado = string_duplicate(localized_pokemon->pokemon);

			if(enviar_localized_pokemon(localized_pokemon, socketLocalizedPokemon, -1, paqueteNuevo->ID) == -1){
				log_info(logger, "No se pudo enviar el mensaje a Broker.");
			}
			else{
				log_info(logger, "<> Mensaje LOCALIZED_POKEMON %s enviado <>", nombreDuplicado);
			}

			free(nombreDuplicado);
			free(paqueteNuevo->buffer->stream);
			free(paqueteNuevo->buffer);
			free(paqueteNuevo);
		}
	}
}

void reconectarABroker(){
	if (desconexion == 1) {
		int conectado = generarSocketsConBroker();

		while(conectado != 1){
			conectado = generarSocketsConBroker();
		}
		desconexion = 0;
	}
}
