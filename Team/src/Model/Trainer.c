/*
 * Trainer.c
 *
 *  Created on: 4 may. 2020
 *      Author: utnso
 */

#include "Trainer.h"

//inicializa y devuelve un trainer nuevo. Es el new, al estilo Java
t_entrenador* newTrainer() {
	return malloc(sizeof(t_entrenador));
}

//imaginate esto como un setter
void setPosicion(t_entrenador* entrenador, t_posicion* posicion) {
	entrenador->posicion = posicion;
}
