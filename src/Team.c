/*
 ============================================================================
 Name        : Team.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Team.h"
#include "utils.h" // esto lo agrego igual aunque la biblio no este lista todavia


int main(void) {
int conexion;

	//agrego las variables que se agregan en el archivo de conf ademas del ip y puerto
	// los primeros 3 no estoy segura se esta bien asi. Se supone que son una lista de listas. Son bienvenidos a revisar si eso esta bien :)
	char** POSICONES_ENTRENADORES;
	char** POKEMON_ENTRENADORES;
	char** OBJETIVOS_ENTRENADORES;
	int* TIEMPO_RECONEXION;
	int* RETARDO_CICLO_CPU;
	char* ALGORITMO_PLANIFICACION;
	int* QUANTUM;
	char* IP_BROKER;
	int* ESTIMACION_INICIAL;
	int* PUERTO_BROKER;
	char* LOG_FILE;

	t_log* logger;
	t_config* config;

	// inicializo el log del Broker
	logger = iniciar_logger();

	// creo y devuelvo un puntero a la estructura t_config
	config = leer_config();

	POSICONES_ENTRENADORES = config_get_array_value(config, "POSICONES_ENTRENADORES");
	POKEMON_ENTRENADORES = config_get_array_value(config, "POKEMON_ENTRENADORES");
	OBJETIVOS_ENTRENADORES = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
	TIEMPO_RECONEXION = config_get_int_value(config, "TIEMPO_RECONEXION");
	RETARDO_CICLO_CPU = config_get_int_value(config, "RETARDO_CICLO_CPU");
	ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	QUANTUM = config_get_int_value(config, "QUANTUM");
	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");
	PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");
	LOG_FILE =  config_get_string_value(config,"LOG_FILE");

	// faltaria loggear la info de todo el archivo de configuracion, ademas de ip y puerto
	log_info(logger, "Lei la IP %s y PUERTO %s\n", IP_BROKER, PUERTO_BROKER);

	//crear conexion, un socket conectado
	conexion = crear_conexion(IP_BROKER, PUERTO_BROKER);

	//TODO: envio mensaje (el primer parametro esta vacio porque depende de cada estructura de mensaje, va un valor distinto). No lo completo porque vicky no commiteo al master
	enviar_mensaje(, conexion);

	// recibir mensaje
	char* mensaje = recibir_mensaje(conexion);

	//loguear mensaje recibido

	log_info(logger, "El mensaje recibido es: %s\n", mensaje);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void){
	t_log* logger = log_create("team.log", "TEAM", 1,LOG_LEVEL_INFO);
	if(logger == NULL){
			printf("No pude crear el logger \n");
			exit(1);
	}
	return logger;
}

t_config* leer_config(void)
{
	t_config* config = config_create("./team.config");

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

