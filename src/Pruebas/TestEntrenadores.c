/*
 * TestEntrenadores.c
 *
 *  Created on: 18 jun. 2020
 *      Author: utnso
 */

#include "TestEntrenadores.h"

Entrenador* crearEntrenadorEnExec(){
	Entrenador* entrenadorNuevo = newEntrenador();

	t_posicion* posicionEntrenador = newPosicion();
	posicionEntrenador->posicionX = 8;
	posicionEntrenador->posicionY = 7;
	entrenadorNuevo->posicion = posicionEntrenador;

	PokemonEnElMapa* pokeObje = newPokemon();
	pokeObje->cantidad = 1;
	pokeObje->nombre = "Ganger";

	t_list* pokesObje = list_create();
	list_add(pokesObje,pokeObje);

	t_posicion posicionLibre;
	posicionLibre.posicionX = 8;
	posicionLibre.posicionY = 8;

	PokemonEnElMapa* pokeLibre = newPokemon();
	pokeLibre->cantidad = 1;
	pokeLibre->nombre = "Pikachu";
	pokeLibre->posicion = posicionLibre;

	MovimientoEnExec* movimiento = malloc(sizeof(MovimientoEnExec));
	movimiento->objetivo = MOVERyATRAPAR;
	movimiento->pokemonNecesitado = pokeLibre;

	entrenadorNuevo->posicion = posicionEntrenador;
	entrenadorNuevo->pokemonesObjetivos = pokesObje;
	entrenadorNuevo->movimientoEnExec = movimiento;
	entrenadorNuevo->estado = 3;

	return entrenadorNuevo;
}

void pasarAExecEntrenador(Entrenador* entrenador) {

	t_posicion posicionLibre;
	posicionLibre.posicionX = 8;
	posicionLibre.posicionY = 8;

	PokemonEnElMapa* pokeLibre = newPokemon();
	pokeLibre->cantidad = 1;
	pokeLibre->nombre = "Pikachu";
	pokeLibre->posicion = posicionLibre;

	MovimientoEnExec* movimiento = malloc(sizeof(MovimientoEnExec));
	movimiento->objetivo = MOVERyATRAPAR;
	movimiento->pokemonNecesitado = pokeLibre;

	entrenador->movimientoEnExec = movimiento;
	entrenador->estado = 3;
}
