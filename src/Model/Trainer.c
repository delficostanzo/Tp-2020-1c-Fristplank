#include "Trainer.h"

//inicializa y devuelve un trainer nuevo. Es el new, al estilo Java. ESTADO NEW
Entrenador* newEntrenador() {
	return malloc(sizeof(Entrenador));
}

//imaginate esto como setters
void setPosicionA(Entrenador* entrenador, t_posicion* posicion) {
	entrenador->posicion = posicion;
}

void setPokemonesObjetivosA(Entrenador* entrenador, t_list* pokemones) {
	entrenador->pokemonesObjetivos = pokemones;
}

void setPokemonesAtrapadosA(Entrenador* entrenador, t_list* pokemones) {
	entrenador->pokemonesAtrapados = pokemones;
}
