
#include "Mapa.h"


void iniciarVariables(){
	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");

	puertoTeam = config_get_int_value(config, "PUERTO_TEAM");

	ID_UNICO = config_get_int_value(config, "ID_UNICO");

	TIEMPO_RECONEXION = config_get_int_value(config, "TIEMPO_RECONEXION");

	ALGORITMO = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	QUANTUM = config_get_string_value(config, "QUANTUM");

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
	pthread_mutex_init(&mutexObjetivosGlobales, NULL);
	pthread_mutex_init(&mutexPokemonesLibres, NULL);
	pthread_mutex_init(&mutexListaEntrenadoresReady, NULL);
	sem_init(&semaforoCatch, 1, 0);

	idsCorrelativosCaught = list_create();
	sem_init(&semaforoCorrelativos, 1, 1);
	idsCorrelativosLocalized = list_create();


	sem_init(&semaforoEstados, 1, 1);

	pokemonesLibres = list_create();
	listaEntrenadoresReady = list_create();

	//llegan bien sus nombres
}
