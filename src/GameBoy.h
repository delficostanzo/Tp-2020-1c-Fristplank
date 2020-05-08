
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
#include<utils.h>

//agrego las variables que se agregan en el archivo de conf ademas del ip y puerto
char* IP_BROKER;
char* IP_TEAM;
char* IP_GAMECARD;
char* PUERTO_BROKER;
char* PUERTO_TEAM;
char* PUERTO_GAMECARD;

t_log* logger;
t_log* loggerObligatorio;

t_log* iniciarLogger(void);
t_log* iniciarLoggerObligatorio(void);
void leer_config(t_log* logger);
void terminar_programa(int, t_log*, t_config*);

void procesarBrokerNewPokemon(int socket, char* argv[]);

#endif /* GAMEBOY_H_ */
