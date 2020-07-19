/*
 * variables.h
 *
 *  Created on: Jul 13, 2020
 *      Author: utnso
 */

#ifndef SRC_VARIABLES_H_
#define SRC_VARIABLES_H_

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

typedef struct {
	op_code tipoMensaje;
	int ID;
	int IDCorrelativo;
	int posicion;
	int tamanioMensaje; // con /0
	int tamanioMensajeEnMemoria; // sin /0
	t_list* ACKSuscriptores;
	t_list* enviadosSuscriptores;
	int flagLRU;
	char ultimaReferencia;
} t_metadata;

typedef struct {
	op_code nombreCola;
	t_list * mensajes; //toda la metadata
	t_list * suscriptores;
} t_cola;

static const char* ID_COLA[] = {
		"COLAERROR",
		"NEW_POKEMON",
		"APPEARED_POKEMON",
		"CATCH_POKEMON",
		"CAUGHT_POKEMON",
		"GET_POKEMON",
		"LOCALIZED_POKEMON"};

static const char* ID_PROCESO[] = {
		"IDERROR",
		"BROKER",
		"TEAM",
		"GAMEBOY",
		"GAMECARD"
};
t_cola cola[6];

int existeSuscriptor(t_list * suscriptores, int suscriptor);

typedef struct{
	int ID;
}suscriptorEnCola;

int LRUcounter;
int IDmsg;

int TAMANO_MEMORIA;
int TAMANO_MINIMO_PARTICION;
char * ALGORITMO_MEMORIA;
char * ALGORITMO_PARTICION_LIBRE;
char* ALGORITMO_REEMPLAZO;
char* IP_BROKER;
int PUERTO_BROKER;
int FRECUENCIA_COMPACTACION;
char* LOG_FILE;

int cantidadParticionesEliminadas;

pthread_mutex_t mutexMemoria;
pthread_mutex_t mutexColas;
pthread_mutex_t mutexIdMensaje;
pthread_mutex_t mutexLRUcounter;
pthread_mutex_t mutexEnvio;
sem_t * colaNew;
sem_t compactMemoria;

t_log* logger;
void * memoriaCache;

int crearID();

#endif /* SRC_VARIABLES_H_ */
