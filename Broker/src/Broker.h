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
#include <string.h>
#include<commons/collections/list.h>
#include "conexion.h"
#include "utils.h"
#include <stdbool.h>
#include <semaphore.h>

#include "variables.h"
#include "conexionBroker.h"
#include "colas.h"
#include "memoriaCache.h"
#include "particiones.h"
#include "repositorioSuscriptores.h"
#include "buddy.h"

t_config* leer_config(void);
void terminar_programa(int, t_log*, t_config*);
void iniciarMemoria();
void iniciarColas();
t_log* iniciar_logger(void);

//int existeSuscriptor(t_list * suscriptores, int suscriptor);

#endif /* BROKER_H_ */
