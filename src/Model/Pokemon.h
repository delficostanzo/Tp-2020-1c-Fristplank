/*
 * t_pokemon.h
 *
 *  Created on: 5 may. 2020
 *      Author: utnso
 */

#ifndef SRC_MODEL_POKEMON_H_
#define SRC_MODEL_POKEMON_H_

#include "utils.h"

typedef struct {
	String nombre;
	t_posicion* posicion;
}t_pokemon;

t_pokemon* newPokemon();
void setNombreTo(t_pokemon* pokemon, String nombre);
void setPositionTo(t_pokemon* pokemon, t_posicion* posicion);

#endif /* SRC_MODEL_POKEMON_H_ */
