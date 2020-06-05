#include "Trainer.h"

//inicializa y devuelve un trainer nuevo. Es el new, al estilo Java. ESTADO NEW
Entrenador* newEntrenador() {
	return malloc(sizeof(Entrenador));
}

//imaginate esto como setters
void setPosicionA(Entrenador* entrenador, t_posicion* posicion) {
	entrenador->posicion = posicion;
}

//void setPokemonesObjetivosA(Entrenador* entrenador, t_list* pokemones) {
//	entrenador->pokemonesObjetivos = pokemones;
//}

void setPokemonObjetivoA(Entrenador* entrenador, PokemonEnElMapa* nuevoObjetivo) {
	//typedef bool(*erasedType)(void*);

//	bool tieneElMismoNombre(PokemonEnElMapa* pokemonEntrenador){
//		//return pokemonEntrenador->nombre == nuevoObjetivo->nombre;
//		return (strcmp(pokemonEntrenador->nombre, nuevoObjetivo->nombre) == 0);
//	}

	t_list* objetivosDelEntrenador = entrenador->pokemonesObjetivos;
	//YA HAY UNO DE ESA ESPECIE EN SU LISTA DE OBJETIVOS
	if(buscarPorNombre(nuevoObjetivo->nombre, objetivosDelEntrenador) != NULL){
		//le aumento la cantidad
		//PokemonEnElMapa* pokemonASumar = list_find(objetivosDelEntrenador, (erasedType)tieneElMismoNombre);
		PokemonEnElMapa* pokemonASumar = buscarPorNombre(nuevoObjetivo->nombre, objetivosDelEntrenador);
		pokemonASumar->cantidad += 1;
	}
	else {
		//lo agrego con cantidad 1
		nuevoObjetivo->cantidad = 1;
		list_add(objetivosDelEntrenador, nuevoObjetivo);
	}
}

//void setPokemonesAtrapadosA(Entrenador* entrenador, t_list* pokemones) {
//	entrenador->pokemonesAtrapados = pokemones;
//}

void setPokemonAtrapadoA(Entrenador* entrenador, PokemonEnElMapa* pokemon) {
	typedef bool(*erasedType)(void*);

	bool tieneElMismoNombre(PokemonEnElMapa* pokemonEntrenador){
		return pokemonEntrenador->nombre == pokemon->nombre;
	}

	t_list* atrapadosDelEntrenador = entrenador->pokemonesObjetivos;
	//YA HAY UNO DE ESOS POKES EN SU LISTA DE ATRAPADOS
	if(buscarPorNombre(pokemon->nombre, atrapadosDelEntrenador) != NULL){
		//le aumento la cantidad
		PokemonEnElMapa* pokemonASumar = list_find(atrapadosDelEntrenador, (erasedType)tieneElMismoNombre);
		pokemonASumar->cantidad += 1;
	}
	else {
		//lo agrego con cantidad 1
		pokemon->cantidad = 1;
		list_add(atrapadosDelEntrenador, pokemon);
	}
}

t_posicion* newPosicion() {
	return malloc(sizeof(t_posicion));
}
