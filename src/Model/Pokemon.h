/*
 * Pokemon.h
 *
 *  Created on: 5 may. 2020
 *      Author: fritsplank
 */

#ifndef SRC_MODEL_POKEMON_H_
#define SRC_MODEL_POKEMON_H_

#include "utils.h"
#include "../AppInit/LogFunctions.h"

typedef struct {
	String nombre;
	t_posicion posicion; //no es puntero porque el pokemon no se mueve
	int cantidad;
}PokemonEnElMapa;

PokemonEnElMapa* newPokemon();
void setNombreTo(PokemonEnElMapa* pokemon, String nombre);
void setPosicionTo(PokemonEnElMapa* pokemon, t_posicion posicion);
void setCantidadTo(PokemonEnElMapa* pokemon, int cantidad);
PokemonEnElMapa* buscarPorNombre(String nombrePokemon, t_list* pokemones);

#endif /* SRC_MODEL_POKEMON_H_ */
