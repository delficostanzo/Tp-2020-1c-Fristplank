
#include "Mapa.h"





void iniciarVariables(){
//	pthread_mutex_init(&mutexConexionGameboy, NULL);
//	pthread_mutex_init(&mutexSocketGameBoy, NULL);
//	pthread_mutex_init(&mutexConexionBroker, NULL);
//	pthread_mutex_init(&mutexSuscripcionAppeared, NULL);
//	pthread_mutex_init(&mutexSuscripcionCaught, NULL);
//	pthread_mutex_init(&mutexSuscripcionLocalized, NULL);
//	pthread_mutex_init(&mutexSocketGet, NULL);
//	pthread_mutex_init(&mutexSocketCatch, NULL);
//	pthread_mutex_init(&mutexSocketACKAppeared, NULL);
//	pthread_mutex_init(&mutexSocketACKCaught, NULL);
//	pthread_mutex_init(&mutexSocketACKLocalized, NULL);


	pthread_mutex_init(&mutexEntrenadores, NULL);
	pthread_mutex_init(&mutexObjetivosTotales, NULL);
	pthread_mutex_init(&mutexObjetivosAtrapados, NULL);
	pthread_mutex_init(&mutexObjetivosGlobales, NULL);
	pthread_mutex_init(&mutexPokemonesLibres, NULL);

	sem_init(&semaforoCatch, 1, 0);

	idsCorrelativosCaught = list_create();
	idsCorrelativosLocalized = list_create();
}
