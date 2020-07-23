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
PokemonEnElMapa* buscarPorNombre(char* nombrePokemon, t_list* pokemones){

	bool tieneNombre(PokemonEnElMapa* pokemon){
		return (strcmp(pokemon->nombre, nombrePokemon) == 0);
		//return pokemon->nombre == nombrePokemon;
	}

	if(list_is_empty(pokemones)) {
		return NULL;
	}

	PokemonEnElMapa* pokemonBuscado = list_find(pokemones, (void*)tieneNombre);
	return pokemonBuscado;
}

int estaElPokemon(PokemonEnElMapa* pokemon, t_list* listaPokes){
	return buscarPorNombre(pokemon->nombre, listaPokes) != NULL;
}

int sonLaMismaPosicion(t_posicion posicion1, t_posicion posicion2) {
	return (posicion1.posicionX == posicion2.posicionX) && (posicion1.posicionY == posicion2.posicionY);
}

//Setters
void setNombreTo(PokemonEnElMapa* pokemon, char* nombre) {
	pokemon->nombre = nombre;
}

void setPosicionTo(PokemonEnElMapa* pokemon, t_posicion posicion) {
	pokemon->posicion = posicion;
}

void setCantidadTo(PokemonEnElMapa* pokemon, int cantidad){
	pokemon->cantidad = cantidad;
}

PokemonEnElMapa* asignarPokemonCopia(PokemonEnElMapa* pokemonACopiar) {
	PokemonEnElMapa* pokemonCopia = malloc(sizeof(PokemonEnElMapa));


	//memcpy(pokemonCopia, pokemonACopiar, sizeof(PokemonEnElMapa));

	memcpy(&pokemonCopia->cantidad, &(pokemonACopiar->cantidad), sizeof(int));
	pokemonCopia->nombre = malloc(strlen(pokemonACopiar->nombre) + 1);
	memcpy(pokemonCopia->nombre, pokemonACopiar->nombre, strlen(pokemonACopiar->nombre) + 1);
	//posicion no es un puntero
//	memcpy(pokemonCopia + offset, &pokemonACopiar->posicion.posicionX, sizeof(uint32_t));
//	offset += sizeof(uint32_t);
//	memcpy(pokemonCopia + offset, &pokemonACopiar->posicion.posicionY, sizeof(uint32_t));
//	offset += sizeof(uint32_t);

	pokemonCopia->posicion.posicionX = pokemonACopiar->posicion.posicionX;
	pokemonCopia->posicion.posicionY = pokemonACopiar->posicion.posicionY;

	return pokemonCopia;
}

