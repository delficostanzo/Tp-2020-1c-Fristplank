
#include <stdio.h>
#include <stdlib.h>
#include "Team.h"

t_list* inicializarEntrenadoresHasta(int cantidad);
t_list* getEntrenadoresDesde(String nombreDeArchivo);

int main(int argc, char *argv[]) {
	int conexion;

	t_log* logger = iniciar_logger();

	t_config* configConnection = leerConfigDesde("src/connection.config");
	t_list* entrenadores = getEntrenadoresDesde("src/team.config");

	//ejemplo
	t_caught_pokemon* caughtStruct = malloc(sizeof(t_caught_pokemon));
	caughtStruct->id_correlativo=1;
	caughtStruct->ok=true;

	char** POSICONES_ENTRENADORES = config_get_array_value(configConnection, "POSICONES_ENTRENADORES");
	char** POKEMON_ENTRENADORES = config_get_array_value(configConnection, "POKEMON_ENTRENADORES");
	char** OBJETIVOS_ENTRENADORES = config_get_array_value(configConnection, "OBJETIVOS_ENTRENADORES");
	int TIEMPO_RECONEXION = config_get_int_value(configConnection, "TIEMPO_RECONEXION");
	int RETARDO_CICLO_CPU = config_get_int_value(configConnection, "RETARDO_CICLO_CPU");
	char* ALGORITMO_PLANIFICACION = config_get_string_value(configConnection, "ALGORITMO_PLANIFICACION");
	int QUANTUM = config_get_int_value(configConnection, "QUANTUM");
	char* IP_BROKER = config_get_string_value(configConnection, "IP_BROKER");
	int ESTIMACION_INICIAL = config_get_int_value(configConnection, "ESTIMACION_INICIAL");
	char* PUERTO_BROKER = config_get_string_value(configConnection, "PUERTO_BROKER");
	char* LOG_FILE =  config_get_string_value(configConnection,"LOG_FILE");

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

t_config* leerConfigDesde(char* nombreDeArchivo) {
	t_config* config = config_create(nombreDeArchivo);
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

t_list* getEntrenadoresDesde(String nombreDeArchivo) {
	t_config* configEntrenador = leerConfigDesde(nombreDeArchivo);
	t_list* entrenadores;

	int cantidadEntrenadores = config_get_int_value(configEntrenador, "CANTIDAD_ENTRENADORES");
	entrenadores = inicializarEntrenadoresHasta(cantidadEntrenadores);

	String* posiciones = config_get_array_value(configEntrenador, "POSICIONES_ENTRENADORES");
	String* pokemones = config_get_array_value(configEntrenador, "POKEMON_ENTRENADORES");
	String* objetivos = config_get_array_value(configEntrenador, "OBJETIVOS_ENTRENADORES");


	return entrenadores;
}

t_list* inicializarEntrenadoresHasta(int cantidad) {
	t_list* entrenadores = list_create();

	for(int contador=0; contador<cantidad; contador++) {
		t_entrenador* entrenador = newTrainer();

		list_add(entrenadores, entrenador);
	}

	return entrenadores;
}
