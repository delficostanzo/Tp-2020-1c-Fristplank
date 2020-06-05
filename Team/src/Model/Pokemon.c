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
	return malloc(sizeof(PokemonEnElMapa));
}

// retorna la estructura poke que tiene ese nombre si esta en la lista pasada por parametro
// si no cumple la condicion, DEVUELVE NULL
PokemonEnElMapa* buscarPorNombre(String nombrePokemon, t_list* pokemones){
	typedef bool(*erasedType)(void*);

	bool tieneNombre(PokemonEnElMapa* pokemon){
		return strcmp(pokemon->nombre, nombrePokemon) == 0;
		//return pokemon->nombre == nombrePokemon;
	}
	return list_find(pokemones, (erasedType)tieneNombre);
}

//Setters
void setNombreTo(PokemonEnElMapa* pokemon, String nombre) {
	strcpy(pokemon->nombre, nombre);
}

void setPosicionTo(PokemonEnElMapa* pokemon, t_posicion posicion) {
	pokemon->posicion = posicion;
}

void setCantidadTo(PokemonEnElMapa* pokemon, int cantidad){
	pokemon->cantidad = cantidad;
}


