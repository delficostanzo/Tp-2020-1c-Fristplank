/*
 * Pokemon.c
 *
 *  Created on: 5 may. 2020
 *      Author: utnso
 */

#include "Pokemon.h"

//esto es como pdep, podes declararte alias de tipos para hacerte la vida mas facil. Fijate que si los declaras en el ".c", son privados
typedef t_pokemon* Pokemon;
typedef t_posicion* Posicion;

//inicializa y devuelve un pokemon nuevo. Es el new, al estilo Java
Pokemon newPokemon() {
	return malloc(sizeof(Pokemon));
}

Posicion newPosicion() {
	return malloc(sizeof(t_posicion));
}// esto no tendria que estar aca, lo dejamos por ahora (ACORDARSE DE CAMBIARLO)

//Setters
void setNombreTo(Pokemon pokemon, String nombre) {
	strcpy(pokemon->nombre, nombre);
}

void setPositionTo(Pokemon pokemon, Posicion posicion) {
	pokemon->posicion = posicion;
}


