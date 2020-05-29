/*
 * Mapa.h
 *
 *  Created on: 25 may. 2020
 *      Author: fritsplank
 */

#ifndef SRC_MODEL_MAPA_H_
#define SRC_MODEL_MAPA_H_

#include "utils.h"
#include "Trainer.h"
#include "Pokemon.h"

//
//typedef struct {
//	t_list* entrenadores;
//	t_list* pokemonesEnElMapa;
//}Mapa;

Entrenador* entrenadorMasCercanoA(PokemonEnElMapa* pokemon, t_list* entrenadores);

#endif /* SRC_MODEL_MAPA_H_ */
