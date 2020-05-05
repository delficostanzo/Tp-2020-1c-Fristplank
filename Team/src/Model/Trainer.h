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
}t_entrenador;

t_entrenador* newTrainer();
void setPosicion(t_entrenador* entrenador, t_posicion* posicion);

#endif /* SRC_MODEL_TRAINER_H_ */
