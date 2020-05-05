
#include <stdio.h>
#include <stdlib.h>
#include "Team.h"

t_list* inicializarEntrenadoresHasta(int cantidad);
t_list* getEntrenadoresDesde(String nombreDeArchivo);
void setPosicionesEnEntrenadoresDesde(t_config* config, t_list* entrenadores);

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

//	terminar_programa(conexion, logger, config);

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

t_config* leerConfigDesde(char* nombreDeArchivo) {
	t_config* config = config_create(nombreDeArchivo);
	t_log* logger = iniciar_logger();

	if(config == NULL){
		log_error(logger, "No se pudo abrir el archivo de configuracion '%s'", nombreDeArchivo);
		exit(2);
	}
	log_info(logger,"Archivo de configuracion '%s' leido correctamente", nombreDeArchivo);
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

//aca es donde delego el comportamiento para crear una lista de entrenadores, a partir del archivo de configuracion donde estan
t_list* getEntrenadoresDesde(String nombreDeArchivo) {
	t_config* configEntrenador = leerConfigDesde(nombreDeArchivo);
	t_log* log = iniciar_logger();
	t_list* entrenadores;

	//esto lo agregamos al archivo de configuracion, ya que podiamos agregar lo que necesitabamos. Sirve para saber cuantos entrenadores hay
	int cantidadEntrenadores = config_get_int_value(configEntrenador, "CANTIDAD_ENTRENADORES");
	log_info(log, "Cantidad de entrenadores: %d", cantidadEntrenadores);
	entrenadores = inicializarEntrenadoresHasta(cantidadEntrenadores); //aca inicializamos tantos entrenadores como haya

	//aca delego lo de setear posiciones dentro de los entrenadores
	setPosicionesEnEntrenadoresDesde(configEntrenador, entrenadores);
	log_info(log, "Cargadas las posiciones de los entrenadores");

	String* pokemones = config_get_array_value(configEntrenador, "POKEMON_ENTRENADORES");
	String* objetivos = config_get_array_value(configEntrenador, "OBJETIVOS_ENTRENADORES");

	log_destroy(log);

	return entrenadores;
}

//esto crea una lista con entrenadores vacios, hasta la cantidad que le indiques
t_list* inicializarEntrenadoresHasta(int cantidad) {
	t_list* entrenadores = list_create();

	for(int contador=0; contador<cantidad; contador++) {
		t_entrenador* entrenador = newTrainer();

		list_add(entrenadores, entrenador);
	}

	return entrenadores;
}

void setPosicionesEnEntrenadoresDesde(t_config* config, t_list* entrenadores) {
	String* stringPositions = config_get_array_value(config, "POSICIONES_ENTRENADORES");
	//esto es un map clasico. Recibe una lista de tipo t_list*, y una funcion que transforma. Fijate que le paso la funcion con un & antes
	//crearListaCon te devuelve una t_list* a partir del array de strings que sacas del archivo de configuracion
	t_list* posiciones = list_map(crearListaCon(stringPositions, list_size(entrenadores)), posicionDesde);

	//aca es simple, para cada entrenador, le seteo su correspondiente posicion
	for(int index = 0; index < list_size(entrenadores); index++) {
		t_entrenador* entrenador = list_get(entrenadores, index);
		t_posicion* posicion = list_get(posiciones, index);

		setPosicion(entrenador, posicion);
	}
}
