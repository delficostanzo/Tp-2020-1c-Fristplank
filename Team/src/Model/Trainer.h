
#ifndef SRC_MODEL_TRAINER_H_
#define SRC_MODEL_TRAINER_H_

#include "utils.h"

typedef struct {
	t_posicion* posicion;
	t_list* pokemonesObjetivos;
	t_list* pokemonesAtrapados;
	//enum objetivo -> movimiento, intercambio, atrapar
	//pthread
	//pthread_mutex o contador
}Entrenador;

Entrenador* newEntrenador();
void setPosicionA(Entrenador* entrenador, t_posicion* posicion);
void setPokemonesObjetivosA(Entrenador* entrenador, t_list* pokemones);
void setPokemonesAtrapadosA(Entrenador* entrenador, t_list* pokemones);
t_posicion* newPosicion();

#endif /* SRC_MODEL_TRAINER_H_ */
