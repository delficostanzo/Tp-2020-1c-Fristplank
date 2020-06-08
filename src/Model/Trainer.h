
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

typedef enum{
	NEW = 1,
	READY = 2,
	EXEC = 3,
	BLOCK = 4,
	EXIT = 5
}Estado;

typedef struct {
	t_posicion* posicion;
	t_list* pokemonesObjetivos;
	t_list* pokemonesAtrapados;
	ObjetivoEnExec objetivo;
	Estado estado;
	pthread_t hiloEntrenador;
	//pthread_mutex o contador, despues vemo
}Entrenador;

Entrenador* newEntrenador();
void setPosicionA(Entrenador* entrenador, t_posicion* posicion);
void setPokemonA(t_list* listaPokemones, PokemonEnElMapa* nuevoPokemon);
t_posicion* newPosicion();

#endif /* SRC_MODEL_TRAINER_H_ */
