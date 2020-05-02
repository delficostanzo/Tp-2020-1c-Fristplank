

#include <stdio.h>
#include <stdlib.h>
#include "GameBoy.h"

int main(void) {
	int conexion;
	int conexionTeam;
	int conexionGameCard;

	//ejemplo
	t_get_pokemon* getStruct = malloc(sizeof(t_get_pokemon));
	getStruct->pokemon = malloc(sizeof(t_get_pokemon));
	getStruct->pokemon = "Pikachu.txt";
	// inicializo el log del Broker
	t_log* logger = iniciar_logger();

	// creo y devuelvo un puntero a la estructura t_config
	t_config* config = leer_config();

	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	IP_TEAM = config_get_string_value(config, "IP_TEAM");
	IP_GAMECARD = config_get_string_value(config, "IP_GAMECARD");
	PUERTO_BROKER = config_get_string_value(config, "IP_BROKER");
	PUERTO_TEAM = config_get_string_value(config, "PUERTO_TEAM");
	PUERTO_GAMECARD = config_get_string_value(config, "PUERTO_GAMECARD");


	log_info(logger, "Lei la IP %s y PUERTO %s\n del broker", IP_BROKER, PUERTO_BROKER);
	log_info(logger, "Lei la IP %s y PUERTO %s\n del broker", IP_TEAM, PUERTO_TEAM);
	log_info(logger, "Lei la IP %s y PUERTO %s\n del broker", IP_GAMECARD, PUERTO_GAMECARD);

	//crear conexion de cada proceso, un socket conectado

	conexion = crear_conexion(IP_BROKER, PUERTO_BROKER);
	conexionTeam = crear_conexion(IP_TEAM, PUERTO_TEAM);
	conexionGameCard = crear_conexion(IP_GAMECARD, PUERTO_GAMECARD);

	enviar_get_pokemon(getStruct,conexion);
	enviar_get_pokemon(getStruct,conexionTeam);
	enviar_get_pokemon(getStruct,conexionGameCard);

	// recibir mensaje
	//t_paquete* mensaje = recibir_mensaje(conexion); //lo recibimos y la funcion recibir mensaje lo mete en un paquete

	//loguear mensaje recibido
	//log_info(logger, "El mensaje recibido es: %s\n", mensaje);

	terminar_programa(conexion, logger, config);

	free(getStruct->pokemon);
	free(getStruct);
}

t_log* iniciar_logger(void){
	t_log * log = malloc(sizeof(t_log));
	log = log_create("gameboy.log", "GAMEBOY", 1,0);
	if(log == NULL){
		printf("No pude crear el logger \n");
		exit(1);
	}
	log_info(log,"Logger Iniciado");
	return log;
}

t_config* leer_config(void)
{
	t_config* config = config_create("src/gameboy.config");
	t_log* logger = iniciar_logger();

	if(config == NULL){
		printf("No pude leer la config \n");
		exit(2);
	}
	log_info(logger,"Archivo de configuracion seteado");
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


	liberar_conexion(conexion);

}


