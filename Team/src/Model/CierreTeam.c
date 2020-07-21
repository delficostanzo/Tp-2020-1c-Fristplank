/*
 * CierreTeam.c
 *
 *  Created on:
 *      Author: fritsplank
 */

#include "CierreTeam.h"

void freeEntrenadores();
void liberarPokesGlobales();
int tieneQueLiberarMovimiento(Entrenador* entrenador);
void destruirPokemon(PokemonEnElMapa* poke);

void terminarTeam() {

	noHayQueFinalizar = 0;

	if (config != NULL) {
		config_destroy(config); //destruye la esctructura de config en memoria, no elimina el archivo de config
	}
	liberarConexion(socketGameBoy);
	liberarConexion(conexionBroker);
	liberarConexion(socketGet);
	liberarConexion(socketIdGet);
	liberarConexion(suscripcionAppeared);
	liberarConexion(socketACKAppeared);
	liberarConexion(socketCatch);
	liberarConexion(socketIdCatch);
	liberarConexion(suscripcionCaught);
	liberarConexion(socketACKCaught);

	pthread_cancel(escucharAppearedPokemon);
	pthread_cancel(escucharCaughtPokemon);

	quickLog("Se libera la conexion de los sockets");

	pthread_mutex_destroy(&mutexEntrenadores);
	pthread_mutex_destroy(&mutexObjetivosGlobales);
	pthread_mutex_destroy(&mutexPokemonesLibres);
	pthread_mutex_destroy(&mutexListaEntrenadoresReady);
	sem_destroy(&semaforoCatch);
	sem_destroy(&semaforoCorrelativos);

	quickLog("Se liberan los semaforos");

	freeEntrenadores();

	quickLog("Se liberan los entrenadores");

	list_destroy(objetivosTotales);
	list_destroy(objetivosAtrapados);
	liberarPokesGlobales();

	list_destroy(pokemonesLibres);

	//apuntaban a los mismos entrenadores ya destruidos
	list_destroy(entrenadores);
	list_destroy(listaEntrenadoresReady);

	quickLog("Se liberan las variables globales");

	destruirLog(logger);
	destruirLog(LO);

}

void freeEntrenadores() {
	for(int index = 0; index < list_size(entrenadores); index++) {
		Entrenador* entrenador = list_get(entrenadores, index);
//		t_list* atrapados = entrenador->pokemonesAtrapados;
//		for(int i = 0; i <list_size(atrapados); i++) {
//			PokemonEnElMapa* poke = list_get(atrapados, i);
//			free(poke->nombre);
//			free(poke);
//		}

//		if(list_is_empty(entrenador->pokemonesAtrapados) != 1){
//			list_destroy_and_destroy_elements(entrenador->pokemonesAtrapados, destruirPokemon);
//		}
//		t_list* objetivos = entrenador->pokemonesObjetivos;
//		for(int y = 0; y <list_size(objetivos); y++) {
//			PokemonEnElMapa* poke = list_get(objetivos, y);
//			free(poke->nombre);
//			free(poke);
//		}
		list_destroy_and_destroy_elements(entrenador->pokemonesObjetivos, destruirPokemon);
//		list_destroy(entrenador->pokemonesObjetivos);
		//if(tieneQueLiberarMovimiento(entrenador)) {
			free(entrenador->movimientoEnExec->pokemonNecesitado->nombre);
			free(entrenador->movimientoEnExec->pokemonNecesitado);
			//son copias de los atrapados asi que tambien hay que liberarlos
			if(entrenador->movimientoEnExec->pokemonAIntercambiar->nombre == NULL) {
				free(entrenador->movimientoEnExec->pokemonAIntercambiar->nombre);
			}

			free(entrenador->movimientoEnExec->pokemonAIntercambiar);
			//pthread_cancel(entrenador->hiloEntrenador);
		//}
		free(entrenador->movimientoEnExec);
		pthread_mutex_destroy(&entrenador->mutexEstado);
		pthread_mutex_destroy(&entrenador->mutexCorrelativo);
		sem_destroy(&entrenador->semaforoExecEntrenador);
		//pthread_mutex_destroy(&entrenador->mutexEntrenador);
//		pthread_cancel(entrenador->hiloEntrenador);
		free(entrenador);
	}
}

void liberarPokesGlobales() {
	t_list* objetivos = objetivosGlobales;
	for(int i = 0; i <list_size(objetivos); i++) {
		PokemonEnElMapa* poke = list_get(objetivos, i);
		free(poke->nombre);
		free(poke);
	}
	list_destroy(objetivosGlobales);
}

void liberarConexion(int socket_cliente) {
	if (close(socket_cliente) == -1) {
		log_error(logger, "Error al cerrar la conexion");
	}
}

int tieneQueLiberarMovimiento(Entrenador* entrenador) {
	return entrenador->numeroEntrenador == 'A';

}

void destruirPokemon(PokemonEnElMapa* poke) {
	//free(poke->nombre);
	free(poke);
}
