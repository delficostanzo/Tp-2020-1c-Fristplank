
#include <stdio.h>
#include <stdlib.h>
#include "Team.h"


int main(void) {
	int conexionBroker; //cliente del broker
	int conexionGameBoy; //servidor del gameboy

	t_log* logger = iniciar_logger();
	t_config* config = leer_config();


//	char** POSICONES_ENTRENADORES = config_get_array_value(config, "POSICONES_ENTRENADORES");
//	char** POKEMON_ENTRENADORES = config_get_array_value(config, "POKEMON_ENTRENADORES");
//	char** OBJETIVOS_ENTRENADORES = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
//	int TIEMPO_RECONEXION = config_get_int_value(config, "TIEMPO_RECONEXION");
//	int RETARDO_CICLO_CPU = config_get_int_value(config, "RETARDO_CICLO_CPU");
//	char* ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
//	int QUANTUM = config_get_int_value(config, "QUANTUM");
//	char* IP_BROKER = config_get_string_value(config, "IP_BROKER");
//	int ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");
//	int PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");
//	char* LOG_FILE =  config_get_string_value(config,"LOG_FILE");

	int puertoTeam = config_get_int_value(config,"PUERTO_TEAM");
	// faltaria loggear la info de todo el archivo de configuracion, ademas de ip y puerto
	//log_info(logger, "Lei la IP %s y PUERTO %s\n", IP_BROKER, PUERTO_BROKER);

	//conexionBroker = crear_conexion(IP_BROKER, PUERTO_BROKER);
	conexionGameBoy = crearSocket();

	if(escuchaEn(conexionGameBoy,puertoTeam)){
		log_info(logger, "Escuchando conexiones del GameBoy");
	}

	//se queda bloqueado esperando que el gameboy se conecte
	int socketGameBoy = aceptarConexion(conexionGameBoy);

	id_proceso idProcesoConectado;
	idProcesoConectado = iniciarHandshake(socketGameBoy, TEAM);
	log_info(logger, "El id del proceso con el que me conecte es: %d", idProcesoConectado);


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

