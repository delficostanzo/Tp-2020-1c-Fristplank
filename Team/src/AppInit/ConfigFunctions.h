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

t_config* leerConfigDesde(String nombreDeArchivo);
t_list* crearListaCon(String* strings, int cantidad);
t_posicion* posicionDesde(String stringDePosicion);
t_pokemon* pokemonDesde(String nombrePokemons);


#endif /* SRC_APPINIT_CONFIGFUNCTIONS_H_ */
