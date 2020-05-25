/*
 * HilosEntrenadores.c
 *
 *  Created on:
 *      Author: fritsplank
 */

#include "HilosEntrenadores.h"
#include "../Model/Trainer.h"
#include "../TeamUtils/LogManager.h"
#include "ConfigFunctions.h"

void funcionesDelEntrenador(Data entrenador);
pthread_t crearHiloParaEntrenador(Data entrenador);

/////// CREACION DE LOS HILOS DE CADA ENTRENADOR /////////////////

// esta funcion agarra una lista de entrenadores y devuelve una lista de hilos
t_list* crearHilosDeEntrenadores(t_list* entrenadores){
	return list_map(entrenadores, (Data)crearHiloParaEntrenador);
}

// esta funcion agarra un entrenador del tipo t_entrenador y lo conbierte en un hilo (este seria el estado NEW)
pthread_t crearHiloParaEntrenador(Data entrenador){
	pthread_t hilo;

	// pthread_create(el hilo creado, por ahora NULL, la funcion (micromain) donde el hilo hace todas sus tareas, los parametros que usa esa funcion)
	//pthread_create(&hilo, NULL, (Data)funcionesDelEntrenador, entrenador);

	return hilo;
}

//la funcion funcionesDelEntrenador tendria que estar en el Team.c, lo dejo aca por ahora
void funcionesDelEntrenador(Data entrenador){
	// por ahora usamos esto como ejemplo, donde de cada hilo te devuelve la posicion en x del entrenador
	Entrenador* unEntrenador = entrenador;

	char* posicionX = string_from_format("La posicion del entrenador esta sig: %d", unEntrenador->posicion->x);

	quickLog(posicionX);
}
