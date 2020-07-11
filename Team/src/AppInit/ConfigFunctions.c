/*
 * ConfigFunctions.c
 *
 *  Created on:
 *      Author: fritsplank
 */

//funciones para extraer la info desde el archivo de configuracion
#include "ConfigFunctions.h"


PokemonEnElMapa* pokemonDesde(char* nombrePokemon);

//crea un config a partir del nombre de un archivo de configuracion y lo devuelve
t_config* leerConfigDesde(char* nombreDeArchivo) {
	t_config* config = config_create(nombreDeArchivo);
	//t_log* logger = iniciar_log("team");
	t_log* logger = iniciar_logger();

	if(config == NULL){
		log_error(logger, "$-No se pudo abrir el archivo de configuracion '%s'", nombreDeArchivo);
		exit(1);
	}

	log_info(logger,"$-Archivo de configuracion '%s' leido correctamente", nombreDeArchivo);
	log_destroy(logger);

	return config;
}

//con esto transformas la lista de strings que te viene del config en una t_list*
t_list* crearListaConStringsDeConfig(char** strings) {
	t_list* lista = list_create();

	for(int index=0; index >= 0; index++) {
		if(strings[index] != NULL){
			char* unString = malloc(strlen(strings[index])+1);
			memcpy(unString, strings[index],strlen(strings[index])+1);

			list_add(lista, unString);
		} else{
			break;
		}
	}

	return lista;
}

//esto convierte un string de tipo "1|2" de posicion, en una t_posicion.
t_posicion* posicionDesde(char* stringDePosicion) {
	t_posicion* posicion = newPosicion();

	char** posicionString = string_split(stringDePosicion,"|");

	posicion->posicionX = atoi(posicionString[0]);
	posicion->posicionY = atoi(posicionString[1]);

	free(posicionString[0]);
	free(posicionString[1]);
	free(posicionString);

	return posicion;
}

//recibe cada conjunto de pokemones de cada entrenador separados por |
//DEVUELVE UNA LISTA DE ESTRUCTURAS DE POKEMONES
t_list* pokemonesDesdeString(char* stringDePokemones) {
	typedef void*(*erasedType)(void*);


	char** stringDeNombresPokemones = string_split(stringDePokemones, "|");

	//pasarlo de String* a t_list* para usarlo en el map
	t_list* listaDeNombres = crearListaConStringsDeConfig(stringDeNombresPokemones);

	t_list* pokemones = list_map(listaDeNombres, (erasedType)pokemonDesde);

	return pokemones;
}

// recibiendo un string con el nombre del pokemon te retorna una estructura de pokemon en el mapa
PokemonEnElMapa* pokemonDesde(char* nombrePokemon){
	PokemonEnElMapa* pokemon = newPokemon();

	pokemon->nombre = nombrePokemon;
	pokemon->posicion.posicionX = 0;
	pokemon->posicion.posicionY = 0;
	pokemon->cantidad = 1;
	return pokemon;
}


