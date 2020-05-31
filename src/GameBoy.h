
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
#include"Library/utils.h"

char* IP_BROKER;
char* IP_TEAM;
char* IP_GAMECARD;
char* PUERTO_BROKER;
char* PUERTO_TEAM;
char* PUERTO_GAMECARD;

char* IP_GAMEBOY;
char* PUERTO_GAMEBOY;

t_log* logger;
t_log* loggerObligatorio;

t_log* iniciarLogger(void);
t_log* iniciarLoggerObligatorio(void);
void leer_config(t_log* logger);
void terminar_programa(int, t_log*, t_config*);

void procesarBrokerGetPokemon(int socket, char* argv[]);
void procesarBrokerNewPokemon(int socket, char* argv[]);
void procesarBrokerAppearedPokemon(int socket, char* argv[]);
void procesarBrokerCatchPokemon(int socket, char* argv[]);
void procesarBrokerCaughtPokemon(int socket, char* argv[]);

void procesarTeamAppearedPokemon(int socket, char* argv[]);

void procesarGameCardCatchPokemon(int socket, char* argv[]);
void procesarGameCardNewPokemon(int socket, char* argv[]);
void procesarGameCardGetPokemon(int socket, char* argv[]);

void procesarSubscribe(int socket, char* argv[]);

void* escucharCola(void* colaAEscuchar);

#endif /* GAMEBOY_H_ */
