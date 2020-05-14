/*
 * Trainer.h
 *
 *  Created on: 4 may. 2020
 *      Author: utnso
 */

#ifndef SRC_MODEL_TRAINER_H_
#define SRC_MODEL_TRAINER_H_

#include "utils.h"

typedef struct {
	t_posicion* posicion;
	t_list* pokemonesObjetivos;
	t_list* pokemonesAtrapados;
}Entrenador;

Entrenador* newTrainer();
void setPosicion(Entrenador* entrenador, t_posicion* posicion);
void setPokemonesObjetivosTo(Entrenador* entrenador, t_list* pokemones);
void setPokemonesAtrapadosTo(Entrenador* entrenador, t_list* pokemones);

#endif /* SRC_MODEL_TRAINER_H_ */
