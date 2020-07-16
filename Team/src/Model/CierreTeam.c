/*
 * CierreTeam.c
 *
 *  Created on:
 *      Author: fritsplank
 */

#include "CierreTeam.h"

void freeEntrenadores();
void liberarConexion(int socket_cliente);


void terminarTeam() {

	if (config != NULL) {
		config_destroy(config); //destruye la esctructura de config en memoria, no elimina el archivo de config
	}
	log_info(LO, "Se destruye la estructura de config sin eliminar el archivo config");
	//pthread_exit(escucharAppearedPokemon);

	liberarConexion(socketGet);
	//liberarConexion(socketIdGet);
	liberarConexion(suscripcionAppeared);
	//liberarConexion(socketACKAppeared);
	liberarConexion(socketCatch);
	liberarConexion(socketIdCatch);
	liberarConexion(suscripcionCaught);
	liberarConexion(socketACKCaught);

	log_info(LO,"Se libera la conexion de los sockets");

	freeEntrenadores();

	log_info(LO,"Se liberan los entrenadores");

	pthread_mutex_destroy(&mutexEntrenadores);
	pthread_mutex_destroy(&mutexObjetivosGlobales);
	pthread_mutex_destroy(&mutexPokemonesLibres);
	pthread_mutex_destroy(&mutexListaEntrenadoresReady);
	sem_destroy(&semaforoCatch);
	sem_destroy(&semaforoCorrelativos);

	log_info(LO,"Se liberan los semaforos");

	//apuntaban a los mismos entrenadores ya destruidos
	list_destroy(entrenadores);
	list_destroy(listaEntrenadoresReady);

	//TODO: apuntaba a los mismos pokemones que apuntaban los entrenadores
	//ya fueron liberados por cada entrenador
	list_destroy(objetivosTotales);
	list_destroy(objetivosAtrapados);
	//TODO los globales son copias
	list_destroy(objetivosGlobales);
	list_destroy(pokemonesLibres);

	log_info(LO, "Se liberan las variables globales");

	//freePaquetes();
	destruirLog(logger);
	destruirLog(LO);

}

void freeEntrenadores() {
	for(int index = 0; index < list_size(entrenadores); index++) {
		Entrenador* entrenador = list_get(entrenadores, index);
		free(entrenador->posicion);
		t_list* atrapados = entrenador->pokemonesAtrapados;
		for(int i = 0; i <list_size(atrapados); i++) {
			PokemonEnElMapa* poke = list_get(atrapados, i);
			free(poke->nombre);
			free(poke);
		}
		list_destroy(entrenador->pokemonesAtrapados);
		t_list* objetivos = entrenador->pokemonesObjetivos;
		for(int i = 0; i <list_size(objetivos); i++) {
			PokemonEnElMapa* poke = list_get(objetivos, i);
			free(poke->nombre);
			free(poke);
		}
		list_destroy(entrenador->pokemonesObjetivos);
		list_destroy(entrenador->pokemonesAtrapados);
		//son copias de los atrapados asi que tambien hay que liberarlos
		free(entrenador->movimientoEnExec->pokemonAIntercambiar->nombre);
		free(entrenador->movimientoEnExec->pokemonAIntercambiar);
		free(entrenador->movimientoEnExec->pokemonNecesitado->nombre);
		free(entrenador->movimientoEnExec->pokemonNecesitado);
		free(entrenador->movimientoEnExec);
		//pthread_mutex_destroy(&entrenador->mutexEntrenador);
		pthread_exit(&entrenador->hiloEntrenador);
		free(entrenador);
	}
}
void liberarConexion(int socket_cliente) {
	if (close(socket_cliente) == -1) {
		log_error(logger, "Error al cerrar la conexion");
	}
}
