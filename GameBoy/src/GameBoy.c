/*
 ============================================================================
 Name        : GameBoy.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "GameBoy.h"
#include "utils.h" // esto lo agrego igual aunque la biblio no este lista todavia

int main(void) {
	int conexion;
	int conexionTeam;
	int conexionGameCard;

	//agrego las variables que se agregan en el archivo de conf ademas del ip y puerto
	char* IP_BROKER;
	char* IP_TEAM;
	char* IP_GAMECARD;
	int* PUERTO_BROKER;
	int* PUERTO_TEAM;
	int* PUERTO_GAMECARD;

	t_log* logger;
	t_config* config;

	// inicializo el log del Broker
	logger = iniciar_logger();

	// creo y devuelvo un puntero a la estructura t_config
	config = leer_config();

	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	IP_TEAM = config_get_string_value(config, "IP_TEAM");
	IP_GAMECARD = config_get_string_value(config, "IP_GAMECARD");
	IP_BROKER = config_get_int_value(config, "IP_BROKER");
	PUERTO_TEAM = config_get_int_value(config, "PUERTO_TEAM");
	PUERTO_GAMECARD = config_get_int_value(config, "PUERTO_GAMECARD");


	log_info(logger, "Lei la IP %s y PUERTO %s\n del broker", IP_BROKER, PUERTO_BROKER);
	log_info(logger, "Lei la IP %s y PUERTO %s\n del broker", IP_TEAM, PUERTO_TEAM);
	log_info(logger, "Lei la IP %s y PUERTO %s\n del broker", IP_GAMECARD, PUERTO_GAMECARD);

	//crear conexion de cada proceso, un socket conectado

	conexion = crear_conexion(IP_BROKER, PUERTO_BROKER);
	conexionTeam = crear_conexion(IP_TEAM, PUERTO_TEAM);
	conexionGameCard = crear_conexion(IP_GAMECARD, PUERTO_GAMECARD);

	//TODO: envio mensaje (el primer parametro esta vacio porque depende de cada estructura de mensaje, va un valor distinto). No lo completo porque vicky no commiteo al master
	enviar_mensaje(, conexion);

	// recibir mensaje
	char* mensaje = recibir_mensaje(conexion);

	//loguear mensaje recibido

	log_info(logger, "El mensaje recibido es: %s\n", mensaje);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void){
	t_log* logger = log_create("gameboy.log", "GAMEBOY", 1,LOG_LEVEL_INFO);
	if(logger == NULL){
			printf("No pude crear el logger \n");
			exit(1);
	}
	return logger;
}

t_config* leer_config(void)
{
	t_config* config = config_create("./gameboy.config");

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


