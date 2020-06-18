/*
 * utils.c
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#include "utils.h"

t_log* iniciar_logger(void){

	t_log* logger;
	if((logger = log_create("./gamecard.log", "GAMECARD", 1, log_level_from_string("DEBUG"))) == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}
	return logger;
}

void leer_configuracionGameCard(void){
	log_debug(logger, "<> START: Leyendo archivo config <>");

	//TODO root hardcoded para correrlo desde terminal, CAMBIAR
	char* rootPath = string_from_format("/home/utnso/Escritorio/Workspaces/tp-2020-1c-Fritsplank/GameCard");
//	configPath = string_from_format ("%s/src/gamecard.config", rootPath);
//	config = config_create(configPath);

	config = config_create("/home/utnso/Escritorio/Workspaces/tp-2020-1c-Fritsplank/GameCard/src/gamecard.config");
	TIEMPO_DE_REINTENTO_CONEXION = config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION");
	TIEMPO_DE_REINTENTO_OPERACION = config_get_int_value(config, "TIEMPO_DE_REINTENTO_OPERACION");
	TIEMPO_RETARDO_OPERACION = config_get_int_value(config, "TIEMPO_RETARDO_OPERACION");
	// con el root hardcoded
//	PUNTO_MONTAJE_TALLGRASS = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
	char* PUNTO_MONTAJE_TALLGRASS_RELATIVO = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
	PUNTO_MONTAJE_TALLGRASS = string_from_format("%s%s", rootPath, PUNTO_MONTAJE_TALLGRASS_RELATIVO);

	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	PUERTO_BROKER = config_get_string_value(config, "PUERTO_BROKER");
	IP_GAMECARD = config_get_string_value(config, "IP_GAMECARD");
	PUERTO_GAMECARD = config_get_string_value(config, "PUERTO_GAMECARD");
	PATH_BITMAP = string_from_format("%s/Metadata/Bitmap.bin", PUNTO_MONTAJE_TALLGRASS);
	PATH_FILES = string_from_format("%s/Files/", PUNTO_MONTAJE_TALLGRASS);
	PATH_BLOCKS = string_from_format("%s/Blocks/", PUNTO_MONTAJE_TALLGRASS);

	log_debug(logger, "Config file | IP del Broker: %s", IP_BROKER);
	log_debug(logger, "Config file | Puerto del Broker: %s",PUERTO_BROKER);
	log_debug(logger, "Config file | IP del GameCard: %s", IP_GAMECARD);
	log_debug(logger, "Config file | Puerto del GameCard: %s",PUERTO_GAMECARD);
	log_debug(logger, "Config file | Punto montaje de TallGrass: %s", PUNTO_MONTAJE_TALLGRASS);
	log_debug(logger, "Config file | Tiempo de reintento de conexión al broker: %d", TIEMPO_DE_REINTENTO_CONEXION);
	log_debug(logger, "Config file | Tiempo de reintento de operación: %d", TIEMPO_DE_REINTENTO_OPERACION);
	log_debug(logger, "Config file | Tiempo de retardo de operación: %d", TIEMPO_RETARDO_OPERACION);
	log_debug(logger, "Config file | Path de Bitmap calculado: %s", PATH_BITMAP);
	log_debug(logger, "Config file | Path de Files calculado: %s", PATH_FILES);
	log_debug(logger, "Config file | Path de Blocks calculado: %s", PATH_BLOCKS);
	log_debug(logger, "<> END: Leyendo archivo config <>");

	free(PUNTO_MONTAJE_TALLGRASS_RELATIVO);
	free(rootPath);
}

void leer_metadata(){
	//TODO Si lo corro en Ubuntu no voy a poder tener escritorio
	char* metadataPath = string_from_format("%s/Metadata/Metadata.bin", PUNTO_MONTAJE_TALLGRASS);
	t_config* configMetadata = config_create(metadataPath);
	BLOCK_SIZE = config_get_int_value(configMetadata, "BLOCK_SIZE");
	BLOCKS = config_get_int_value(configMetadata, "BLOCKS");

	config_destroy(configMetadata);
	free(metadataPath);

	log_debug(logger, "<> START: Leyendo Metadata del FileSystem <>");
	log_debug(logger, "Tamaño de bloques expresado en bytes: %d",BLOCK_SIZE);
	log_debug(logger, "Cantidad de bloques: %d", BLOCKS);
	log_debug(logger, "<> END: Leyendo Metadata del FileSystem <>");
}
