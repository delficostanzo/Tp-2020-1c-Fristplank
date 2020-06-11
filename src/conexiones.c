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

	if (conectarA(socketNewPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,
				"Suscripto a cola New Pokemon. Lanzando socket de escucha..");
		// TODO Lanzar hilo donde la escucho
	}

	if (conectarA(socketCatchPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,
				"Suscripto a cola Catch Pokemon. Lanzando socket de escucha..");
		// TODO Lanzar hilo donde la escucho
	}

	if (conectarA(socketGetPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger,
				"Suscripto a cola Get Pokemon. Lanzando socket de escucha..");
		// TODO Lanzar hilo donde la escucho
	}

	if (conectarA(socketAppearedPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger, "Socket de Appeared Pockemon guardado.");
	}

	if (conectarA(socketCaughtPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger, "Socket de Caught Pockemon guardado.");
	}

	if (conectarA(socketLocalizedPokemon, IP_BROKER, puertoBrokerInt)) {
		log_info(logger, "Socket de Localized Pockemon guardado.");
	}

	return 0;
}

void* escucharGameBoy(){
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
