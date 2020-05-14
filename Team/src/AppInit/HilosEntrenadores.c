#include "HilosEntrenadores.h"
#include "../Model/Trainer.h"
#include "../TeamUtils/LogManager.h"
#include "ConfigFunctions.h"

/////// CREACION DE LOS HILOS DE CADA ENTRENADOR /////////////////
// para cada entrenador que hay, creo un hilo. Primero tengo que contar la cantidad de entrenadores que hay en ese team,
//en base a eso, crear los hilos (hacer un for donde su index sea < la cantidad de entrenadores que tiene el team. dentro del for se crean los hilos

// esta funcion agarra una lista de entrenadores y devuelve una lista de hilos
t_list* crearHilosDeEntrenadores(t_list* entrenadores){
	return list_map(entrenadores, (void*)crearHiloParaEntrenador);
}

// esta funcion agarra un entrenador del tipo t_entrenador y lo conbierte en un hilo (este seria el estado NEW)
pthread_t crearHiloParaEntrenador(void* entrenador){
	pthread_t hilo;

	// pthread_create(el hilo creado, por ahora NULL, la funcion (micromain) donde el hilo hace todas sus tareas, los parametros que usa esa funcion)
	pthread_create(&hilo, NULL, (void*)funcionesDelEntrenador, entrenador);

	return hilo;
}

//la funcion funcionesDelEntrenador tendria que estar en el Team.c, lo dejo aca por ahora
void funcionesDelEntrenador(void* entrenador){
	// por ahora usamos esto como ejemplo, donde de cada hilo te devuelve la posicion en x del entrenador
	t_entrenador* unEntrenador = entrenador;

	char* posicionX = string_from_format("La posicion del entrenador esta sig: %d", unEntrenador->posicion->x);

	quickLog(posicionX);
}
