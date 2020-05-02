
#include <stdio.h>
#include <stdlib.h>
#include "Team.h"


int main(void) {
	int conexion;

	t_log* logger = iniciar_logger();
	t_config* config = leer_config();

	//ejemplo
	t_caught_pokemon* caughtStruct = malloc(sizeof(t_caught_pokemon));
	caughtStruct->id_correlativo=1;
	caughtStruct->ok=true;

	POSICONES_ENTRENADORES = config_get_array_value(config, "POSICONES_ENTRENADORES");
	POKEMON_ENTRENADORES = config_get_array_value(config, "POKEMON_ENTRENADORES");
	OBJETIVOS_ENTRENADORES = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
	TIEMPO_RECONEXION = config_get_int_value(config, "TIEMPO_RECONEXION");
	RETARDO_CICLO_CPU = config_get_int_value(config, "RETARDO_CICLO_CPU");
	ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	QUANTUM = config_get_int_value(config, "QUANTUM");
	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");
	PUERTO_BROKER = config_get_string_value(config, "PUERTO_BROKER");
	LOG_FILE =  config_get_string_value(config,"LOG_FILE");

	// faltaria loggear la info de todo el archivo de configuracion, ademas de ip y puerto
	log_info(logger, "Lei la IP %s y PUERTO %s\n", IP_BROKER, PUERTO_BROKER);

	conexion = crear_conexion(IP_BROKER, PUERTO_BROKER);

	enviar_caught_pokemon(caughtStruct,conexion);

	// recibir mensaje
	//t_paquete* mensaje = recibir_mensaje(conexion); //lo recibimos y la funcion recibir mensaje lo mete en un paquete

	//loguear mensaje recibido

	//log_info(logger, "El mensaje recibido es: %s\n", mensaje);

	//terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void){
	t_log * log = malloc(sizeof(t_log));
	log = log_create("team.log", "TEAM", 1,0);
	if(log == NULL){
		printf("No pude crear el logger \n");
		exit(1);
	}
	log_info(log,"Logger Iniciado");
	return log;
}

t_config* leer_config(void)
{
	t_config* config = config_create("src/team.config");
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

	//liberar_conexion(conexion); // esta funcion esta en utils.c

}

