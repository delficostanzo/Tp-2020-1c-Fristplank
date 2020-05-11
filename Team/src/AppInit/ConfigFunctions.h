/*
 * ConfigParser.h
 *
 *  Created on: 5 may. 2020
 *      Author: utnso
 */

#ifndef SRC_APPINIT_CONFIGFUNCTIONS_H_
#define SRC_APPINIT_CONFIGFUNCTIONS_H_

#include "utils.h"
#include<commons/string.h>
#include<commons/config.h>
#include"../Model/Pokemon.h"

t_config* leerConfigDesde(String nombreDeArchivo);
t_list* crearListaConStringsDeConfig(String* strings);
t_posicion* posicionDesde(String stringDePosicion);
t_list* pokemonesDesdeString(String nombrePokemons);
t_pokemon* pokemonDesde(String nombrePokemon);


#endif /* SRC_APPINIT_CONFIGFUNCTIONS_H_ */