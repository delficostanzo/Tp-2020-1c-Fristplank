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
void* generarSocketsConBroker() {

	socketBroker = crearSocket();
	int puertoBrokerInt = atoi(PUERTO_BROKER);
	while (conectarA(socketBroker, IP_BROKER, puertoBrokerInt) != 1) {
		log_debug(logger, "Intentando conexión a Broker...");
		sleep(TIEMPO_DE_REINTENTO_CONEXION);
	}

	id_proceso idProceso;
	idProceso = responderHandshake(socketBroker, GAMEBOY);

	socketNewPokemon = crearSocket();
	socketCatchPokemon = crearSocket();
	socketGetPokemon = crearSocket();

	socketAppearedPokemon = crearSocket();
	socketCaughtPokemon = crearSocket();
	socketLocalizedPokemon = crearSocket();

	socketACKNewPokemon = crearSocket();
	socketACKCatchPokemon = crearSocket();
	socketACKGetPokemon = crearSocket();

	if (conectarA(socketNewPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,"Suscripto a cola New Pokemon. Lanzando socket de escucha..");

		if (conectarA(socketACKNewPokemon, IP_BROKER, puertoBrokerInt)) {
			log_debug(logger, "Socket de ACK New Pokemon guardado.");

			pthread_t escucharNewPokemon;
			pthread_create(&escucharNewPokemon, NULL, escucharColaNewPokemon, NULL);
			pthread_detach(escucharNewPokemon);
		}
	}

	if (conectarA(socketCatchPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,"Suscripto a cola Catch Pokemon. Lanzando socket de escucha..");

		if (conectarA(socketACKCatchPokemon, IP_BROKER, puertoBrokerInt)) {
			log_debug(logger, "Socket de ACK Catch Pokemon guardado.");

			pthread_t escucharCatchPokemon;
			pthread_create(&escucharCatchPokemon, NULL, escucharColaCatchPokemon, NULL);
			pthread_detach(escucharCatchPokemon);
		}
	}

	if (conectarA(socketGetPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,"Suscripto a cola Get Pokemon. Lanzando socket de escucha..");

		if (conectarA(socketACKGetPokemon, IP_BROKER, puertoBrokerInt)) {
			log_debug(logger, "Socket de ACK Catch Pokemon guardado.");

			pthread_t escucharGetPokemon;
			pthread_create(&escucharGetPokemon, NULL, escucharColaGetPokemon, NULL);
			pthread_detach(escucharGetPokemon);
		}
	}

	if (conectarA(socketAppearedPokemon, IP_BROKER, puertoBrokerInt)) { log_debug(logger, "Socket de Appeared Pokemon guardado.");}
	if (conectarA(socketCaughtPokemon, IP_BROKER, puertoBrokerInt)) { log_debug(logger, "Socket de Caught Pokemon guardado.");}
	if (conectarA(socketLocalizedPokemon, IP_BROKER, puertoBrokerInt)) { log_debug(logger, "Socket de Localized Pokemon guardado.");}

	return 0;
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

		id_proceso idProcesoConectado;
		idProcesoConectado = iniciarHandshake(socketGameBoy, GAMECARD);
		log_info(logger, "Me conecté con %s", ID_PROCESO[idProcesoConectado]);

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
				t_new_pokemon* new_pokemon = paqueteNuevo->buffer->stream;
//					log_info(logger, "Mensaje NEW_POKEMON recibido.");
////					pthread_t hiloProcesarNewPokemon;
////					pthread_create(&hiloProcesarNewPokemon, NULL, procesarNewPokemon, (void*) paqueteNuevo->buffer->stream);
////					pthread_detach(hiloProcesarNewPokemon);

				//TODO: AHORA QUE NO ME ROMPE, SEGUIR CON LA LOGICA
				log_info(logger, new_pokemon->pokemon);

				free(new_pokemon->posicion);
				free(new_pokemon->pokemon);
				free(new_pokemon);
				break;
//			case CATCH_POKEMON:
//				log_info(logger, "Mensaje CATCH_POKEMON recibido.");
//				//TODO iniciar hilo para procesarlo
//				break;
//			case GET_POKEMON:
//				log_info(logger, "Mensaje GET_POKEMON recibido.");
//				//TODO iniciar hilo para procesarlo
//				break;
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

		if(paqueteNuevo->codigo_operacion == NEW_POKEMON){

			enviar_ACK(socketACKNewPokemon, -1, paqueteNuevo->ID);

			pthread_t hiloProcesarNewPokemon;
			pthread_create(&hiloProcesarNewPokemon, NULL, procesarNewPokemon, (void*) paqueteNuevo->buffer->stream);
			pthread_detach(hiloProcesarNewPokemon);
		}
		else{
			log_info(logger, "Tipo de mensaje invalido.");
		}
	}
}

void* escucharColaCatchPokemon(){

	while(1){
		log_info(logger, "Esperando mensajes CATCH_POKEMON...");
		t_paquete* paqueteNuevo = recibir_mensaje(socketCatchPokemon);

		if(paqueteNuevo->codigo_operacion == CATCH_POKEMON){
			//TODO iniciar hilo para procesarlo

		}
		else{
			log_info(logger, "Tipo de mensaje invalido.");
		}

		enviar_ACK(socketACKCatchPokemon, -1, paqueteNuevo->ID);
	}
}

void* escucharColaGetPokemon(){

	while(1){
		log_info(logger, "Esperando mensajes GET_POKEMON...");
		t_paquete* paqueteNuevo = recibir_mensaje(socketGetPokemon);

		if(paqueteNuevo->codigo_operacion == GET_POKEMON){
			//TODO iniciar hilo para procesarlo
		}
		else{
			log_info(logger, "Tipo de mensaje invalido.");
		}

		enviar_ACK(socketACKGetPokemon, -1, paqueteNuevo->ID);
	}
}
