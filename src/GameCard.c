/*
 ============================================================================
 Name        : GameCard.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Broker.h"
#include "utils.h"

int main(void) {
	int conexion;

	//agrego las variables que se agregan en el archivo de conf ademas del ip y puerto
		int* TIEMPO_DE_REINTENTO_CONEXION;
		int* TIEMPO_DE_REINTENTO_OPERACION;
		char* PUNTO_MONTAJE_TALLGRASS;
		char* IP_BROKER;
		int* PUERTO_BROKER;

		t_log* logger;
		t_config* config;

		// inicializo el log del Broker
		logger = iniciar_logger();

		// creo y devuelvo un puntero a la estructura t_config
		config = leer_config();

		TIEMPO_DE_REINTENTO_CONEXION = config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION");
		TIEMPO_DE_REINTENTO_OPERACION = config_get_int_value(config, "TIEMPO_DE_REINTENTO_OPERACION");
		PUNTO_MONTAJE_TALLGRASS = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
		IP_BROKER = config_get_string_value(config, "IP_BROKER");
		PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");

		// faltaria loggear la info de todo el archivo de configuracion, ademas de ip y puerto
		log_info(logger, "Lei la IP %s y PUERTO %s\n", IP_BROKER, PUERTO_BROKER);

		//crear conexion, un socket conectado

		conexion = crear_conexion(IP_BROKER, PUERTO_BROKER);

		// envio mensaje (el primer parametro esta vacio porque depende de cada estructura de mensaje, va un valor distinto). No lo completo porque vicky no commiteo al master
		enviar_mensaje(, conexion);

		// recibir mensaje
		char* mensaje = recibir_mensaje(conexion);

		//loguear mensaje recibido

		log_info(logger, "El mensaje recibido es: %s\n", mensaje);

		terminar_programa(conexion, logger, config);


	}

	t_log* iniciar_logger(void){
		t_log* logger = log_create("gamecard.log", "GAMECARD", 1,LOG_LEVEL_INFO);
		if(logger == NULL){
				printf("No pude crear el logger \n");
				exit(1);
		}
		return logger;
	}

	t_config* leer_config(void)
	{
		t_config* config = config_create("./gamecard.config");

		if(config == NULL){
			printf("No pude leer la config \n");
			exit(2);
		}
		return config;
	}


	void terminar_programa(int conexion, t_log* logger, t_config* config)
	{
		if(logger != NULL){
			log_destroy(logger);
		}

		if(config != NULL){
			config_destroy(config); //destruye la esctructura de config en memoria, no lo esta eliminando el archivo de config
		}

		liberar_conexion(conexion); // esta funcion esta en utils.c

	}

