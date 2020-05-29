/*
 * Pokemon.c
 *
 *  Created on: 5 may. 2020
 *      Author: fritsplank
 */

#include "Pokemon.h"

//esto es como pdep, podes declararte alias de tipos para hacerte la vida mas facil. Fijate que si los declaras en el ".c", son privados


//inicializa y devuelve un pokemon nuevo. Es el new, al estilo Java
PokemonEnElMapa* newPokemon() {
	return malloc(sizeof(PokemonEnElMapa*));
}

t_posicion* newPosicion() {
	return malloc(sizeof(t_posicion));
}// esto no tendria que estar aca, lo dejamos por ahora (ACORDARSE DE CAMBIARLO)

//Setters
void setNombreTo(PokemonEnElMapa* pokemon, String nombre) {
	strcpy(pokemon->nombre, nombre);
}

void setPositionTo(PokemonEnElMapa* pokemon, t_posicion* posicion) {
	pokemon->posicion = posicion;
}

void setCantidadTo(PokemonEnElMapa* pokemon, int cantidad){
	pokemon->cantidad = cantidad;
}


