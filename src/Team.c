
#include <stdio.h>
#include <stdlib.h>
#include "Team.h"
#include "./Model/Pokemon.h"
#include "./AppInit/EntrenadoresInit.h"

int main(int argc, char *argv[]) {
	int conexion;

	t_log* logger = newLoggerFrom("team.log");

	t_config* configConnection = leerConfigDesde("src/connection.config");
	t_list* entrenadores = getEntrenadoresDesde("src/team.config");

	//t_list* objetivosGlobales = getObjetivosGlobalesDesde("src/team.config");
	t_list* objetivosGlobales = getObjetivosGlobalesDesde(entrenadores);
	quickLog("primer objetivo de pokemones");
	quickLog(((t_pokemon*)list_get(objetivosGlobales, 0))->nombre);

	int TIEMPO_RECONEXION = config_get_int_value(configConnection, "TIEMPO_RECONEXION");
	int RETARDO_CICLO_CPU = config_get_int_value(configConnection, "RETARDO_CICLO_CPU");
	char* ALGORITMO_PLANIFICACION = config_get_string_value(configConnection, "ALGORITMO_PLANIFICACION");
	int QUANTUM = config_get_int_value(configConnection, "QUANTUM");
	char* IP_BROKER = config_get_string_value(configConnection, "IP_BROKER");
	int ESTIMACION_INICIAL = config_get_int_value(configConnection, "ESTIMACION_INICIAL");
	char* PUERTO_BROKER = config_get_string_value(configConnection, "PUERTO_BROKER");
	char* LOG_FILE =  config_get_string_value(configConnection,"LOG_FILE");

	// faltaria loggear la info de todo el archivo de configuracion, ademas de ip y puerto
	// -----> NO, solo se van loggeando las acciones que va haciendo el sistema, como que se cargaron bien los entrenadores, o cosas asi
	log_info(logger, "Lei la IP %s y PUERTO %s\n", IP_BROKER, PUERTO_BROKER);

	conexion = crear_conexion(IP_BROKER, PUERTO_BROKER);

	// recibir mensaje
	//t_paquete* mensaje = recibir_mensaje(conexion); //lo recibimos y la funcion recibir mensaje lo mete en un paquete

	//loguear mensaje recibido

	//log_info(logger, "El mensaje recibido es: %s\n", mensaje);

	//terminar_programa(conexion, logger, config);

	list_destroy_and_destroy_elements(entrenadores, free);

	return 0;
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


void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger != NULL){
		log_destroy(logger);
	}

	if(config != NULL){
		config_destroy(config); //destruye la esctructura de config en memoria, no elimina el archivo de config
	}

	liberar_conexion(conexion);

}
