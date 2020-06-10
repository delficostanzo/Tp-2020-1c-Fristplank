
#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<readline/readline.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<time.h>
#include"Library/utils.h"
#include"procesar.h"

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

static const char* BOOL_CAUGHT[] = {
		"No se atrapo al Pokemon",
		"Se logró atrapar al Pokemon",
};

char* IP_BROKER;
char* IP_TEAM;
char* IP_GAMECARD;
char* PUERTO_BROKER;
char* PUERTO_TEAM;
char* PUERTO_GAMECARD;
char* IP_GAMEBOY;
char* PUERTO_GAMEBOY;

t_log* logger;
t_config* config;

t_log* iniciarLogger(void);
t_log* iniciarLoggerObligatorio(void);
t_config* leer_config(void);
void terminar_programa(int, t_log*, t_config*);

int conectarAModulo(String PUERTO, String IP);

void procesarSubscribe(char* argv[]);
void escucharCola(void* colaAEscuchar);
void recibirEImprimirMensaje(int socketBroker, op_code cola);

#endif /* GAMEBOY_H_ */
