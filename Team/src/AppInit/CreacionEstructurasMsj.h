/*
 * CreacionEstructurasMsj.h
 *
 *  Created on: 7 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_APPINIT_CREACIONESTRUCTURASMSJ_H_
#define SRC_APPINIT_CREACIONESTRUCTURASMSJ_H_

#include "utils.h"
#include "../Model/Pokemon.h"

t_get_pokemon* crearEstructuraGetDesde(PokemonEnElMapa* pokemones);
t_catch_pokemon* crearEstructuraCatchDesde(PokemonEnElMapa* pokemon);

#endif /* SRC_APPINIT_CREACIONESTRUCTURASMSJ_H_ */
