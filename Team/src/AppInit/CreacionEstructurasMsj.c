
#include "CreacionEstructurasMsj.h"

t_get_pokemon* crearEstructuraGetDesde(PokemonEnElMapa* pokemon){
	t_get_pokemon* getPokemon= malloc(sizeof(t_get_pokemon));
	char* nombrePoke = pokemon->nombre;
	int tamanio = strlen(nombrePoke);
	getPokemon->pokemon = nombrePoke;
	getPokemon->lengthOfPokemon = tamanio;
	quickLog("Llega aca");
	return getPokemon;
}

t_catch_pokemon* crearEstructuraCatchDesde(PokemonEnElMapa* pokemon){
	t_catch_pokemon* catchPokemon = malloc(sizeof(t_get_pokemon));
	char* nombrePoke = pokemon->nombre;
	catchPokemon->pokemon = nombrePoke;
	catchPokemon->lengthOfPokemon = strlen(nombrePoke);
	catchPokemon->posicion = &(pokemon->posicion);
	return catchPokemon;
}
