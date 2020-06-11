/*
 * utils.c
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#include "utils.h"

t_log* iniciar_logger(void){

	t_log* logger;
	if((logger = log_create("./gamecard.log", "GAMECARD", 1, log_level_from_string("INFO"))) == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}
	return logger;
}

void leer_config(void){

	config = config_create("./src/gamecard.config");

	TIEMPO_DE_REINTENTO_CONEXION = config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION");
	TIEMPO_DE_REINTENTO_OPERACION = config_get_int_value(config, "TIEMPO_DE_REINTENTO_OPERACION");
	PUNTO_MONTAJE_TALLGRASS = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	PUERTO_BROKER = config_get_string_value(config, "PUERTO_BROKER");

	// faltaria loggear la info de todo el archivo de configuracion, ademas de ip y puerto
	log_debug(logger, "Config file | IP del broker: %s", IP_BROKER);
	log_debug(logger, "Config file | Puerto del broker: %s",PUERTO_BROKER);
	log_debug(logger, "Config file | Punto montaje de TallGrass: %s", PUNTO_MONTAJE_TALLGRASS);
	log_debug(logger, "Config file | Tiempo de reintento de conexión al broker: %d", TIEMPO_DE_REINTENTO_CONEXION);
	log_debug(logger, "Config file | Tiempo de reintento de operación: %d", TIEMPO_DE_REINTENTO_OPERACION);

	config_destroy(config);
}
