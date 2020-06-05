
#ifndef SRC_MODEL_TRAINER_H_
#define SRC_MODEL_TRAINER_H_

#include "utils.h"
#include "../AppInit/LogFunctions.h"
#include "Pokemon.h"

typedef enum{
	MOVER = 1,
	INTERCAMBIAR = 2,
	ATRAPAR = 3
}ObjetivoEnExec;

typedef struct {
	t_posicion* posicion;
	t_list* pokemonesObjetivos;
	t_list* pokemonesAtrapados;
	ObjetivoEnExec objetivo;
	pthread_t hiloEntrenador;
	//pthread_mutex o contador, despues vemo
}Entrenador;

Entrenador* newEntrenador();
void setPosicionA(Entrenador* entrenador, t_posicion* posicion);
void setPokemonObjetivoA(Entrenador* entrenador, PokemonEnElMapa* nuevoObjetivo);
void setPokemonAtrapadoA(Entrenador* entrenador, PokemonEnElMapa* pokemon);
t_posicion* newPosicion();

#endif /* SRC_MODEL_TRAINER_H_ */
