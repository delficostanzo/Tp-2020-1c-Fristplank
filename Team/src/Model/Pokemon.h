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
	char* nombre;
	t_posicion posicion; //no es puntero porque el pokemon no se mueve
	int cantidad;
}PokemonEnElMapa;

PokemonEnElMapa* newPokemon();
void setNombreTo(PokemonEnElMapa* pokemon, char* nombre);
void setPosicionTo(PokemonEnElMapa* pokemon, t_posicion posicion);
void setCantidadTo(PokemonEnElMapa* pokemon, int cantidad);
PokemonEnElMapa* buscarPorNombre(char* nombrePokemon, t_list* pokemones);
int sonLaMismaPosicion(t_posicion posicion1, t_posicion posicion2);
PokemonEnElMapa* asignarPokemonCopia(PokemonEnElMapa* pokemonACopiar);


#endif /* SRC_MODEL_POKEMON_H_ */
