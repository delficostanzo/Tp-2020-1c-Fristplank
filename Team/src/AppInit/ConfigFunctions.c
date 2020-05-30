/*
 * ConfigFunctions.c
 *
 *  Created on:
 *      Author: fritsplank
 */

//funciones para extraer la info desde el archivo de configuracion
#include "ConfigFunctions.h"
#include "../TeamUtils/LogManager.h"

//crea un config a partir del nombre de un archivo de configuracion y lo devuelve
t_config* leerConfigDesde(String nombreDeArchivo) {
	t_config* config = config_create(nombreDeArchivo);
	t_log* logger = newLoggerFrom("team.log");

	if(config == NULL){
		log_error(logger, "No se pudo abrir el archivo de configuracion '%s'", nombreDeArchivo);
		exit(1);
	}

	log_info(logger,"Archivo de configuracion '%s' leido correctamente", nombreDeArchivo);
	destroyLogger(logger);

	return config;
}

//con esto transformas la lista de strings que te viene del config en una t_list*
t_list* crearListaConStringsDeConfig(String* strings) {
	t_list* lista = list_create();

	for(int index=0; index >= 0; index++) {
		if(strings[index] != NULL){
			String unString = strings[index];

			list_add(lista, unString);
		} else{
			break;
		}
	}

	return lista;
}

//esto convierte un string de tipo "1|2" de posicion, en una t_posicion.
t_posicion* posicionDesde(String stringDePosicion) {
	t_posicion* posicion = newPosicion();

	String* posicionString = string_split(stringDePosicion,"|");

	posicion->x = atoi(posicionString[0]);
	posicion->y = atoi(posicionString[1]);

	free(posicionString[0]);
	free(posicionString[1]);
	free(posicionString);

	return posicion;
}

//recibe cada conjunto de pokemones de cada entrenador separados por |
t_list* pokemonesDesdeString(String stringDePokemones) {
	typedef void*(*erasedType)(void*);


	String* stringDeNombresPokemones = string_split(stringDePokemones, "|");

	//pasarlo de String* a t_list* para usarlo en el map
	t_list* listaDeNombres = crearListaConStringsDeConfig(stringDeNombresPokemones);

	t_list* pokemones = list_map(listaDeNombres, (erasedType)pokemonDesde);

	return pokemones;
}

PokemonEnElMapa* pokemonDesde(String nombrePokemon){
	PokemonEnElMapa* pokemon = newPokemon();

	pokemon->nombre = nombrePokemon;

	return pokemon;
}
