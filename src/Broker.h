
#ifndef BROKER_H_
#define BROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<commons/collections/list.h>
#include<utils.h>


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

pthread_t thread;


t_log* iniciar_logger(void);
t_config* leer_config(void);
void terminar_programa(int, t_log*, t_config*);
void iniciar_servidor(void);

#endif /* BROKER_H_ */
