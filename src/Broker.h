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
#include "conexionBroker.h"
#include "repositorioSuscriptores.h"

/*typedef struct{
	int idSuscriptor;
	 int socketNew;
	 int socketAppeard;
	 int socketGet;
	 int socketCatch;
	 int socketCaught;
	 int socketLocalized;
	 int socket
}t_suscriptor_team;
*/

int IDmsg;
void * memoriaCache;
t_log* logger;

int TAMANO_MEMORIA;
int TAMANO_MINIMO_PARTICION;
char * ALGORITMO_MEMORIA;
char * ALGORITMO_PARTICION_LIBRE;
char* ALGORITMO_REEMPLAZO;
char* IP_BROKER;
int PUERTO_BROKER;
int FRECUENCIA_COMPACTACION;

typedef struct {
	op_code nombreCola;
	t_list * mensajes; //toda la metadata
	t_list * suscriptores;
} t_cola;

static const char* ID_COLA[] = { "COLAERROR", "NEW", "APPEAR", "CATCH",
		"CAUGHT", "GET", "LOCALIZED" };
static const char* ID_PROCESO[] = {
		"IDERROR",
		"BROKER",
		"TEAM",
		"GAMEBOY",
		"GAMECARD"
};
t_cola cola[6];

pthread_mutex_t mutexMemoria;
pthread_mutex_t mutexColas;
pthread_mutex_t mutexIdMensaje;
sem_t * colaNew;
sem_t compactMemoria;

t_config* leer_config(void);
void terminar_programa(int, t_log*, t_config*);
int crearID();
void iniciarMemoria();
void iniciarColas();
t_log* iniciar_logger(void);
//int existeSuscriptor(t_list * suscriptores, int suscriptor);

#endif /* BROKER_H_ */
