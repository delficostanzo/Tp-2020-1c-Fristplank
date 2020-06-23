/*
 * Mapa.h
 *
 *  Created on: 25 may. 2020
 *      Author: fritsplank
 */

#ifndef SRC_MODEL_MAPA_H_
#define SRC_MODEL_MAPA_H_

#include "utils.h"
#include "../AppInit/ConfigFunctions.h"



char* IP_BROKER;
int PUERTO_BROKER;
int puertoTeam;

int conexionGameboy;
int socketGameBoy;

int conexionBroker;

int suscripcionAppeared;
int suscripcionCaught;
int suscripcionLocalized;

int socketGet;
//int socketIdGet;

int socketCatch;
int socketIdCatch;

int socketACKAppeared;
int socketACKCaught;
int socketACKLocalized;


//pthread_mutex_t mutexConexionGameboy;
//pthread_mutex_t mutexSocketGameBoy;
//pthread_mutex_t mutexConexionBroker;
//pthread_mutex_t mutexSuscripcionAppeared;
//pthread_mutex_t mutexSuscripcionCaught;
//pthread_mutex_t mutexSuscripcionLocalized;
//pthread_mutex_t mutexSocketGet;
//pthread_mutex_t mutexSocketCatch;
//pthread_mutex_t mutexSocketACKAppeared;
//pthread_mutex_t mutexSocketACKCaught;
//pthread_mutex_t mutexSocketACKLocalized;


pthread_mutex_t mutexEntrenadores;
pthread_mutex_t mutexObjetivosTotales;
pthread_mutex_t mutexObjetivosAtrapados;
pthread_mutex_t mutexObjetivosGlobales;
pthread_mutex_t mutexPokemonesLibres;
sem_t semaforoCatch;

t_list* entrenadores;
t_list* objetivosTotales;
t_list* objetivosAtrapados;
t_list* objetivosGlobales;
t_list* pokemonesLibres;

t_list* idsCorrelativosCaught;
t_list* idsCorrelativosLocalized;

int inicializarIdsCatch;
int inicializarIdsGet;
sem_t semaforoCatch;



void iniciarVariables();

#endif /* SRC_MODEL_MAPA_H_ */
