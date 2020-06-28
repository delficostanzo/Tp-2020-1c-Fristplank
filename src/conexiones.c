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
	while (conectarA(socketBroker, IP_BROKER, puertoBrokerInt) != 1) {
		log_debug(logger, "Intentando conexión a Broker...");
		sleep(TIEMPO_DE_REINTENTO_CONEXION);
	}

	t_handshake* handshakePropio = malloc(sizeof(t_handshake));
	handshakePropio->id = GAMEBOY;
	handshakePropio->idUnico = ID_UNICO;

	t_handshake* handshakeResponse;
	handshakeResponse = responderHandshake(socketBroker, handshakePropio);
	free(handshakeResponse);

	socketNewPokemon = crearSocket();
	socketCatchPokemon = crearSocket();
	socketGetPokemon = crearSocket();

	socketAppearedPokemon = crearSocket();
	socketCaughtPokemon = crearSocket();
	socketLocalizedPokemon = crearSocket();

	socketACKNewPokemon = crearSocket();
	socketACKCatchPokemon = crearSocket();
	socketACKGetPokemon = crearSocket();

	int conexionCorrecta = 1;

	if (conectarA(socketNewPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,"Suscripto a cola New Pokemon. Lanzando socket de escucha..");

		if (conectarA(socketACKNewPokemon, IP_BROKER, puertoBrokerInt)) {
			log_debug(logger, "Socket de ACK New Pokemon guardado.");
		}
		else{
			conexionCorrecta = -1;
		}
	}
	else{
		conexionCorrecta = -1;
	}

	if (conectarA(socketCatchPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,"Suscripto a cola Catch Pokemon. Lanzando socket de escucha..");

		if (conectarA(socketACKCatchPokemon, IP_BROKER, puertoBrokerInt)) {
			log_debug(logger, "Socket de ACK Catch Pokemon guardado.");
		}
		else{
			conexionCorrecta = -1;
		}
	}
	else{
		conexionCorrecta = -1;
	}

	if (conectarA(socketGetPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,"Suscripto a cola Get Pokemon. Lanzando socket de escucha..");

		if (conectarA(socketACKGetPokemon, IP_BROKER, puertoBrokerInt)) {
			log_debug(logger, "Socket de ACK Catch Pokemon guardado.");
		}
		else{
			conexionCorrecta = -1;
		}
	}
	else{
		conexionCorrecta = -1;
	}

	if (conectarA(socketAppearedPokemon, IP_BROKER, puertoBrokerInt)) { log_debug(logger, "Socket de Appeared Pokemon guardado.");}
	else{conexionCorrecta = -1;}
	if (conectarA(socketCaughtPokemon, IP_BROKER, puertoBrokerInt)) { log_debug(logger, "Socket de Caught Pokemon guardado.");}
	else{conexionCorrecta = -1;}
	if (conectarA(socketLocalizedPokemon, IP_BROKER, puertoBrokerInt)) { log_debug(logger, "Socket de Localized Pokemon guardado.");}
	else{conexionCorrecta = -1;}

	return conexionCorrecta;
}

void lanzarHilosDeEscucha(){

	pthread_t escucharNewPokemon;
	pthread_create(&escucharNewPokemon, NULL, (void*) escucharColaNewPokemon, NULL);
	pthread_detach(escucharNewPokemon);

	pthread_t escucharCatchPokemon;
	pthread_create(&escucharCatchPokemon, NULL, (void*) escucharColaCatchPokemon, NULL);
	pthread_detach(escucharCatchPokemon);

	pthread_t escucharGetPokemon;
	pthread_create(&escucharGetPokemon, NULL, (void*) escucharColaGetPokemon, NULL);
	pthread_detach(escucharGetPokemon);
}

void escucharGameBoy(){
	log_debug(logger, "<> START: Comienzo a escuchar GameBoy <>");
	socketListenerGameBoy = crearSocket();
	int puertoGamecardInt = atoi(PUERTO_GAMECARD);
	if(escuchaEn(socketListenerGameBoy, puertoGamecardInt)){
				log_debug(logger, "Escuchando conexiones del GameBoy");
	}

	while(1){
		int socketGameBoy = aceptarConexion(socketListenerGameBoy);

		t_handshake* handshakePropio = malloc(sizeof(t_handshake));
		handshakePropio->id = GAMECARD;
		handshakePropio->idUnico = ID_UNICO;

		t_handshake* handshakeGameboy = iniciarHandshake(socketGameBoy, handshakePropio);
		log_info(logger, "Me conecté con %s", ID_PROCESO[handshakeGameboy->id]);

		t_paquete* paqueteNuevo = recibir_mensaje(socketGameBoy);


//		puts(new_pokemon->pokemon);
//		log_debug(logger, new_pokemon->pokemon);

		log_debug(logger, "======= Nuevo mensaje recibido =======");
		log_info(logger, "Tipo de mensaje: %s", COLAS_STRING[paqueteNuevo->codigo_operacion]);
		log_info(logger, "ID del mensaje: %d", paqueteNuevo->ID);
		log_info(logger, "ID correlativo del mensaje: %d", paqueteNuevo->ID_CORRELATIVO);
		log_debug(logger,"Tamaño de payload: %d", paqueteNuevo->buffer->size);
//
		switch(paqueteNuevo->codigo_operacion){
			case NEW_POKEMON:;
				log_info(logger, "Mensaje NEW_POKEMON recibido.");

				t_new_pokemon* new_pokemon = paqueteNuevo->buffer->stream;
				pthread_t hiloProcesarNewPokemon;
				pthread_create(&hiloProcesarNewPokemon, NULL, (void*) procesarNewPokemon, (void *) new_pokemon);
				pthread_join(hiloProcesarNewPokemon, NULL);

				/* faltan conexiones a broker
				t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
				appeared_pokemon->lengthOfPokemon = new_pokemon->lengthOfPokemon;
				appeared_pokemon->pokemon = new_pokemon->pokemon;
				appeared_pokemon->posicion = new_pokemon->posicion;

				enviar_appeared_pokemon(appeared_pokemon, socketAppearedPokemon, -1, paqueteNuevo->ID);
				*/
				free(new_pokemon->posicion);
				free(new_pokemon->pokemon);
				free(new_pokemon);
				break;

			case CATCH_POKEMON:
				log_info(logger, "Mensaje CATCH_POKEMON recibido.");
//				//TODO iniciar hilo para procesarlo
				t_catch_pokemon* catch_pokemon = paqueteNuevo->buffer->stream;
				pthread_t hiloProcesarCatchPokemon;
				pthread_create(&hiloProcesarCatchPokemon, NULL, (void*) procesarCatchPokemon, (void *) catch_pokemon);

				void* encontrado;
				pthread_join(hiloProcesarCatchPokemon, encontrado);

				if((int) encontrado){
					log_debug(logger, "Se encontro el pokemon en esa posicion.");
				}
				else{
					log_debug(logger, "NO se encontro el pokemon en esa posicion.");
				}

//				free(encontrado);
				//	Todo resultado, sea correcto o no, deberá realizarse conectandose al Broker y enviando un mensaje a la Cola de Mensajes CAUGHT_POKEMON indicando:
				//	ID del mensaje recibido originalmente.
				//	Resultado.
				//	En caso que no se pueda realizar la conexión con el Broker se debe informar por logs y continuar la ejecución.

				free(catch_pokemon->posicion);
				free(catch_pokemon->pokemon);
				free(catch_pokemon);
				break;

			case GET_POKEMON:
				log_info(logger, "Mensaje GET_POKEMON recibido.");

				t_get_pokemon* get_pokemon = paqueteNuevo->buffer->stream;
				pthread_t hiloProcesarGetPokemon;
				pthread_create(&hiloProcesarGetPokemon, NULL, (void*) procesarGetPokemon, (void *) get_pokemon);

				void* localized;
				pthread_join(hiloProcesarGetPokemon, &localized);
				t_localized_pokemon* localized_pokemon = localized;

				log_debug(logger, "TEST: Pokemon a enviar: %s", localized_pokemon->pokemon);
				log_debug(logger, "TEST: Cantidad a enviar: %d", localized_pokemon->cantidadPosiciones);
				//enviar_localized_pokemon(localized_pokemon, socketLocalizedPokemon, -1, paqueteNuevo->ID);
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
			desconexion = 1;
			pthread_mutex_lock(&semaforoDesconexion);
			reconectarABroker();
			pthread_mutex_unlock(&semaforoDesconexion);
		}

		else{
			enviar_ACK(socketACKNewPokemon, -1, paqueteNuevo->ID);

			if (paqueteNuevo->codigo_operacion == NEW_POKEMON) {
				//TODO iniciar hilo para procesarlo
			} else {
				log_info(logger, "Tipo de mensaje invalido.");
			}
		}
	}
}

void* escucharColaCatchPokemon(){

	while(1){
		log_info(logger, "Esperando mensajes CATCH_POKEMON...");
		t_paquete* paqueteNuevo = recibir_mensaje(socketCatchPokemon);

		if(paqueteNuevo == NULL){
			desconexion = 1;
			pthread_mutex_lock(&semaforoDesconexion);
			reconectarABroker();
			pthread_mutex_unlock(&semaforoDesconexion);
		}

		else{
			enviar_ACK(socketACKCatchPokemon, -1, paqueteNuevo->ID);
			if(paqueteNuevo->codigo_operacion == CATCH_POKEMON){
				//TODO iniciar hilo para procesarlo

			} else {
				log_info(logger, "Tipo de mensaje invalido.");
			}
		}
	}
}

void* escucharColaGetPokemon(){

	while(1){
		log_info(logger, "Esperando mensajes GET_POKEMON...");
		t_paquete* paqueteNuevo = recibir_mensaje(socketGetPokemon);

		if (paqueteNuevo == NULL) {
			desconexion = 1;
			pthread_mutex_lock(&semaforoDesconexion);
			reconectarABroker();
			pthread_mutex_unlock(&semaforoDesconexion);
		}

		else {
			enviar_ACK(socketACKGetPokemon, -1, paqueteNuevo->ID);
			if (paqueteNuevo->codigo_operacion == GET_POKEMON) {
				//TODO iniciar hilo para procesarlo
			} else {
				log_info(logger, "Tipo de mensaje invalido.");
			}
		}
	}
}

void reconectarABroker(){
	if (desconexion) {
		while (!generarSocketsConBroker()) {
			sleep(TIEMPO_DE_REINTENTO_CONEXION);
		}
		desconexion = 0;
	}
}
