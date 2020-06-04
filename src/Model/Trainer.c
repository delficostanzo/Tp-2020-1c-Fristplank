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

void setPokemonObjetivoA(Entrenador* entrenador, PokemonEnElMapa* pokemon) {
	bool tieneElMismoNombre(PokemonEnElMapa* pokemonEntrenador){
		return pokemonEntrenador == pokemon;
	}

	t_list* objetivosDelEntrenador = entrenador->pokemonesObjetivos;
	//YA HAY UNO DE ESOS POKES EN SU LISTA DE OBJETIVOS
	if(buscarPorNombre(pokemon->nombre, objetivosDelEntrenador) != NULL){
		//le aumento la cantidad
		PokemonEnElMapa* pokemonASumar = list_find(objetivosDelEntrenador, tieneElMismoNombre);
		pokemonASumar->cantidad += 1;
	}
	else {
		//lo agrego con cantidad 1
		pokemon->cantidad = 1;
		list_add(objetivosDelEntrenador, pokemon);
	}
}

//void setPokemonesAtrapadosA(Entrenador* entrenador, t_list* pokemones) {
//	entrenador->pokemonesAtrapados = pokemones;
//}

void setPokemonAtrapadoA(Entrenador* entrenador, PokemonEnElMapa* pokemon) {
	bool tieneElMismoNombre(PokemonEnElMapa* pokemonEntrenador){
		return pokemonEntrenador == pokemon;
	}

	t_list* atrapadosDelEntrenador = entrenador->pokemonesObjetivos;
	//YA HAY UNO DE ESOS POKES EN SU LISTA DE ATRAPADOS
	if(buscarPorNombre(pokemon->nombre, atrapadosDelEntrenador) != NULL){
		//le aumento la cantidad
		PokemonEnElMapa* pokemonASumar = list_find(atrapadosDelEntrenador, tieneElMismoNombre);
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
