#ifndef TEAM_H_
#define TEAM_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
//>>>>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<commons/collections/list.h>

typedef enum
{
	NEW_POKEMON = 1,
	APPEARED_POKEMON = 2,
	CATCH_POKEMON = 3,
	CAUGHT_POKEMON = 4,
	GET_POKEMON = 5,
	LOCALIZED_POKEMON = 6,
	MENSAJE = 7
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;
//<<<<<<

//#include "utils.h"

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

t_log* iniciar_logger(void);
t_config* leer_config(void);
void terminar_programa(int, t_log*, t_config*);

#endif /* TEAM_H_ */
