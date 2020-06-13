#ifndef GAMECARD_H_
#define GAMECARD_H_

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
//UTILS
#include<utils.h>
#include<conexion.h>
//THREADS
#include<pthread.h>
//OTROS FUENTES DEL PROYECTO
#include"procesar.h"
#include"conexiones.h"
#include"utils.h"
#include"archivos.h"

int BLOCK_SIZE;
int BLOCKS;

int TIEMPO_DE_REINTENTO_CONEXION;
int TIEMPO_DE_REINTENTO_OPERACION;
char* PUNTO_MONTAJE_TALLGRASS;

char* IP_BROKER;
char* PUERTO_BROKER;
t_log* logger;
t_config* config;

pthread_mutex_t semaforoOpen;

int socketBroker;
int socketNewPokemon;
int socketCatchPokemon;
int socketGetPokemon;
int socketAppearedPokemon;
int socketCaughtPokemon;
int socketLocalizedPokemon;
int socketListenerGameBoy;
int socketACKNewPokemon;
int socketACKCatchPokemon;
int socketACKGetPokemon;

void iniciar_filesystem(void);
void init_bitmap(void);

#endif /* GAMECARD_H_ */
