
#include "CreacionEstructurasMsj.h"

t_get_pokemon* crearEstructuraGetDesde(PokemonEnElMapa* pokemon){
	t_get_pokemon* getPokemon= malloc(sizeof(t_get_pokemon));
	char* nombrePoke = pokemon->nombre;
	getPokemon->pokemon = nombrePoke;
	getPokemon->lengthOfPokemon = strlen(nombrePoke);
	return getPokemon;
}
