/*
 * Pokemon.h
 *
 *  Created on: 5 may. 2020
 *      Author: fritsplank
 */

#ifndef SRC_MODEL_POKEMON_H_
#define SRC_MODEL_POKEMON_H_

#include "utils.h"



typedef struct {
	String nombre;
	t_posicion* posicion;
	int cantidad;
}PokemonEnElMapa;

PokemonEnElMapa* newPokemon();
void setNombreTo(PokemonEnElMapa* pokemon, String nombre);
void setPositionTo(PokemonEnElMapa* pokemon, t_posicion* posicion);
void setCantidadTo(PokemonEnElMapa* pokemon, int cantidad);
t_posicion* newPosicion();

#endif /* SRC_MODEL_POKEMON_H_ */
