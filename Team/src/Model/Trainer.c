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

void setPokemonA(t_list* listaPokemones, PokemonEnElMapa* nuevoPokemon) {

	if(list_is_empty(listaPokemones) == 1){
		list_add(listaPokemones, nuevoPokemon);
	}
	//YA HAY UNO DE ESA ESPECIE EN LA LISTA DE POKEMONES PASADA POR PARAMETRO
	else if(buscarPorNombre(nuevoPokemon->nombre, listaPokemones) != NULL){
		//le aumento la cantidad
		PokemonEnElMapa* pokemonASumar = buscarPorNombre(nuevoPokemon->nombre, listaPokemones);
		pokemonASumar->cantidad += nuevoPokemon->cantidad;
	}
		else {
		list_add(listaPokemones, nuevoPokemon);
		}
}

//void setPokemonesAtrapadosA(Entrenador* entrenador, t_list* pokemones) {
//	entrenador->pokemonesAtrapados = pokemones;
//}

t_posicion* newPosicion() {
	return malloc(sizeof(t_posicion));
}
