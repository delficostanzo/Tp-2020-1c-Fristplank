/*
 * CierreTeam.c
 *
 *  Created on:
 *      Author: fritsplank
 */

#include "CierreTeam.h"

void freeEntrenadores();


void terminarTeam() {

	if (config != NULL) {
		config_destroy(config); //destruye la esctructura de config en memoria, no elimina el archivo de config
	}

	liberar_conexion(socketGet);
	//liberar_conexion(socketIdGet);
	liberar_conexion(suscripcionAppeared);
	//liberar_conexion(socketACKAppeared);
	liberar_conexion(suscripcionLocalized);
	//liberar_conexion(socketACKLocalized);
	liberar_conexion(socketCatch);
	liberar_conexion(socketIdCatch);
	liberar_conexion(suscripcionCaught);
	liberar_conexion(socketACKCaught);

	freeEntrenadores();

	//freePaquetes();

}

void freeEntrenadores() {
	for(int index = 0; index < list_size(entrenadores); index++) {
		Entrenador* entrenador = list_get(entrenadores, index);
		free(entrenador->posicion);
		t_list* atrapados = entrenador->pokemonesAtrapados;
		//solo tiene pokemones atrapados porque ya todos los entrenadores estan en exit
		for(int i = 0; i <list_size(atrapados); i++) {
			PokemonEnElMapa* poke = list_get(atrapados, i);
			free(poke->nombre);
			free(poke);
		}
		list_destroy(entrenador->pokemonesObjetivos);
		list_destroy(entrenador->pokemonesAtrapados);
		free(entrenador->movimientoEnExec->pokemonAIntercambiar);
		free(entrenador->movimientoEnExec->pokemonNecesitado);
		free(entrenador->movimientoEnExec);
	}
}
