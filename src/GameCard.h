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
#include"blocks.h"

static const char* COLAS_STRING[] = {
		"ERROR",
		"NEW_POKEMON",
		"APPEARED_POKEMON",
		"CATCH_POKEMON",
		"CAUGHT_POKEMON",
		"GET_POKEMON",
		"LOCALIZED_POKEMON",
		"RESPUESTA_ID",
		"ACK",
		"GAMEBOYSUSCRIBE"
};

static const char* ID_PROCESO[] = {
		"IDERROR",
		"BROKER",
		"TEAM",
		"GAMEBOY",
		"GAMECARD"
};

int BLOCK_SIZE;
int BLOCKS;

int TIEMPO_DE_REINTENTO_CONEXION;
int TIEMPO_DE_REINTENTO_OPERACION;
int TIEMPO_RETARDO_OPERACION;
char* PUNTO_MONTAJE_TALLGRASS;
char* PATH_BITMAP;
char* PATH_FILES;
char* PATH_FILES_POKEMONES;
char* PATH_BLOCKS;

char* IP_BROKER;
char* PUERTO_BROKER;
char* IP_GAMECARD;
char* PUERTO_GAMECARD;

char* configPath;

t_log* logger;
t_config* config;
t_bitarray* bitarray;
void* bufferBitarray;

pthread_mutex_t semaforoOpen;
pthread_mutex_t semaforoBitarray;
pthread_mutex_t semaforoGetDatos;

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
void finalizar_gamecard(void);

#endif /* GAMECARD_H_ */
