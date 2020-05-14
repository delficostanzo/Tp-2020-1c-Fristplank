
#include "Trainer.h"

//inicializa y devuelve un trainer nuevo. Es el new, al estilo Java
Entrenador* newTrainer() {
	return malloc(sizeof(Entrenador));
}

//imaginate esto como setters
void setPosicion(Entrenador* entrenador, t_posicion* posicion) {
	entrenador->posicion = posicion;
}

void setPokemonesObjetivosTo(Entrenador* entrenador, t_list* pokemones) {
	entrenador->pokemonesObjetivos = pokemones;
}

void setPokemonesAtrapadosTo(Entrenador* entrenador, t_list* pokemones) {
	entrenador->pokemonesAtrapados = pokemones;
}
