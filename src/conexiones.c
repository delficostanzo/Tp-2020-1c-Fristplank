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

void* escucharGameBoy(){
	log_debug(logger, "<> START: Comienzo a escuchar GameBoy <>");
	socketListenerGameBoy = crearSocket();
	int puertoBrokerInt = atoi(PUERTO_BROKER);
	if(escuchaEn(socketListenerGameBoy, puertoBrokerInt)){
				log_debug(logger, "Escuchando conexiones del GameBoy");
	}

	while(1){
		int socketGameBoy = aceptarConexion(socketListenerGameBoy);

		id_proceso idProcesoConectado;
		idProcesoConectado = iniciarHandshake(socket, GAMECARD);
		log_info(logger, "Me conecté con GameBoy");

		t_paquete* paqueteNuevo = recibir_mensaje(socketGameBoy);

		switch(paqueteNuevo->codigo_operacion){
			case NEW_POKEMON:
				//TODO iniciar hilo para procesarlo
				break;
			case CATCH_POKEMON:
				//TODO iniciar hilo para procesarlo
				break;
			case GET_POKEMON:
				//TODO iniciar hilo para procesarlo
				break;
			default:
				log_info(logger, "Tipo de mensaje invalido.");
		}
	}

	return 0;
}

void* escucharColaNewPokemon(){

	while(1){
		log_info(logger, "Esperando mensajes NEW_POKEMON...");
		t_paquete* paqueteNuevo = recibir_mensaje(socketNewPokemon);

		if(paqueteNuevo->codigo_operacion == NEW_POKEMON){
			//TODO iniciar hilo para procesarlo

		}
		else{
			log_info(logger, "Tipo de mensaje invalido.");
		}

		enviar_ACK(socketACKNewPokemon, -1, paqueteNuevo->ID);
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
