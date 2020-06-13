#ifndef GAMECARD_H_
#define GAMECARD_H_

#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<netdb.h>

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/bitarray.h>
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
#include"bitmap.h"

int BLOCK_SIZE;
int BLOCKS;

int TIEMPO_DE_REINTENTO_CONEXION;
int TIEMPO_DE_REINTENTO_OPERACION;
char* PUNTO_MONTAJE_TALLGRASS;
char* PATH_BITMAP;
char* PATH_FILES;
char* PATH_BLOCKS;

char* IP_BROKER;
char* PUERTO_BROKER;
t_log* logger;
t_config* config;
t_bitarray* bitarray;

pthread_mutex_t semaforoOpen;
pthread_mutex_t semaforoBitarray;

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
void init_semaforos(void);
void init_bitmap(void);
void init_estructura_files_blocks(void);
void init_bloques(void);

#endif /* GAMECARD_H_ */
