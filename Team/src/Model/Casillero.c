/*
 * Casillero.c
 *
 *  Created on: 25 may. 2020
 *      Author: fritsplank
 */


#include "Casillero.h"

Casillero* newCasillero() {
	Casillero* casillero = malloc(sizeof(Casillero));

	casillero->pokemon = newPokemon();
	casillero->entrenadorPrincipal = newEntrenador();
	casillero->entrenadorIntercambio = newEntrenador();

	return casillero;
}

void agregarPokemonAlCasillero(Casillero* casillero, t_pokemon* pokemon) {
	casillero->pokemon = pokemon;
}

void agregarEntrenadorPrincipalAlCasillero(Casillero* casillero, Entrenador* entrenador) {
	casillero->entrenadorPrincipal = entrenador;
}

void agregarEntrenadorIntercambioAlCasillero(Casillero* casillero, Entrenador* entrenador) {
	casillero->entrenadorIntercambio = entrenador;
}

void liberarCasillero(Casillero* casillero) {
	free(casillero->pokemon);
	free(casillero->entrenadorPrincipal);
	free(casillero->entrenadorIntercambio);
	free(casillero);
}
