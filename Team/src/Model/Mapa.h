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
#include "Trainer.h"


char* IP_BROKER;
int PUERTO_BROKER;
int puertoTeam;

int conexionBroker;
int suscripcionAppeared;
int suscripcionCaught;
int suscripcionLocalized;
int socketGet;
int socketCatch;

int socketACKAppeared;
int socketACKCaught;
int socketACKLocalized;


pthread_mutex_t mutexEntrenadores;
pthread_mutex_t mutexObjetivosTotales;
pthread_mutex_t mutexObjetivosAtrapados;
pthread_mutex_t mutexObjetivosGlobales;
pthread_mutex_t mutexPokemonesLibres;

t_list* entrenadores;
t_list* objetivosTotales;
t_list* objetivosAtrapados;
t_list* objetivosGlobales;
t_list* pokemonesLibres;


Entrenador* entrenadorMasCercanoA(PokemonEnElMapa* pokemon, t_list* entrenadores);

#endif /* SRC_MODEL_MAPA_H_ */
