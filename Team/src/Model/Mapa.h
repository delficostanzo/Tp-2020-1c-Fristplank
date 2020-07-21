

#ifndef SRC_MODEL_MAPA_H_
#define SRC_MODEL_MAPA_H_

#include "utils.h"
#include<commons/config.h>
#include "../AppInit/LogFunctions.h"

char* IP_BROKER;
int PUERTO_BROKER;
int puertoTeam;

int ID_UNICO;

int TIEMPO_RECONEXION;
int QUANTUM;

char* ALGORITMO;

int conexionGameboy;
int socketGameBoy;

int conexionBroker;

char* LogOficial;
t_log* LO;
t_log* logger;

int suscripcionAppeared;
int suscripcionCaught;
int suscripcionLocalized;

int socketGet;
int socketIdGet;

int socketCatch;
int socketIdCatch;

int socketACKAppeared;
int socketACKCaught;
int socketACKLocalized;

pthread_t escucharLocalizedPokemon;
pthread_t escucharAppearedPokemon;
pthread_t escucharCaughtPokemon;

int cantidadDeEspeciesTotales;

int CC;
int cantidadDeadlocks;
int cantidadIntercambios;

int noHayQueFinalizar;


int desconexion;

pthread_mutex_t semaforoDesconexion;
t_config* config;



pthread_mutex_t mutexEntrenadores;
pthread_mutex_t mutexObjetivosGlobales;
pthread_mutex_t mutexPokemonesLibres;
pthread_mutex_t mutexPokemonesRecibidos;
pthread_mutex_t mutexListaEntrenadoresReady;
sem_t semaforoCatch;
sem_t esperandoPasarAlgunoAExec;
sem_t arrancarPlan;

t_list* entrenadores;
t_list* objetivosTotales;
t_list* objetivosAtrapados;
t_list* objetivosGlobales;
t_list* pokemonesLibres;
t_list* pokemonesRecibidos;
t_list* listaEntrenadoresReady;

t_list* idsCorrelativosCaught;
sem_t semaforoCorrelativos;
t_list* idsCorrelativosLocalized;


sem_t semaforoEstados;


sem_t semaforoEntrenadorEsperando;


void iniciarVariables();

#endif /* SRC_MODEL_MAPA_H_ */
