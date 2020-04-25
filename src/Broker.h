#ifndef BROKER_H_
#define BROKER_H_

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
int TAMANO_MEMORIA;
int TAMANO_MINIMO_PARTICION;
char* ALGORITMO_MEMORIA;
char* ALGORITMO_REEMPLAZO;
char* ALGORITMO_PARTICION_LIBRE;
char* IP_BROKER;
char* PUERTO_BROKER;
int FRECUENCIA_COMPACTACION;
char* LOG_FILE;

t_log* logger;
t_config* config;


t_log* iniciar_logger(void);
t_config* leer_config(void);
void terminar_programa(int, t_log*, t_config*);

#endif /* BROKER_H_ */
