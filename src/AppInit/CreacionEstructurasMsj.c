
#include "CreacionEstructurasMsj.h"

t_get_pokemon* crearEstructuraGetDesde(PokemonEnElMapa* pokemon){
	t_get_pokemon* getPokemon= malloc(sizeof(t_get_pokemon));
//	char* nombrePoke = string_new();
//	string_append(&nombrePoke, getPokemon->pokemon);
	char* nombrePoke = pokemon->nombre;
	int tamanio = strlen(nombrePoke);
	getPokemon->pokemon = nombrePoke;
	getPokemon->lengthOfPokemon = tamanio;
	return getPokemon;
}

t_catch_pokemon* crearEstructuraCatchDesde(PokemonEnElMapa* pokemon){
	t_catch_pokemon* catchPokemon = malloc(sizeof(t_catch_pokemon));
	char* nombrePoke = pokemon->nombre;
	catchPokemon->pokemon = nombrePoke;
	catchPokemon->lengthOfPokemon = strlen(nombrePoke);
	catchPokemon->posicion = &(pokemon->posicion);
	return catchPokemon;
}
