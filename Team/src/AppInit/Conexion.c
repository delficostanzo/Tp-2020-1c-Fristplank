/*
 * Conexion.c
 *
 *  Created on: 11 jun. 2020
 *      Author: utnso
 */

#include "Conexion.h"

int conectarABroker( char* IP_BROKER, int PUERTO_BROKER){
	int conexionBroker = crearSocket();
	t_log* logger = iniciar_logger();

	while(conectarA(conexionBroker, IP_BROKER, PUERTO_BROKER) != 1){
		quickLog("Conectando al módulo...");
	}

	id_proceso idProcesoBroker;
	idProcesoBroker = responderHandshake(conexionBroker, TEAM);
	log_info(logger, "El id del proceso con el que me conecte es: %d", idProcesoBroker);

	return conexionBroker;
}
