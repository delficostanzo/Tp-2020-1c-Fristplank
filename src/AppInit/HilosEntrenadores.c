
#include "HilosEntrenadores.h"
#include "../Model/Trainer.h"
#include "../TeamUtils/LogManager.h"
#include "ConfigFunctions.h"

void funcionesDelEntrenador(Data entrenador);
void crearHiloParaEntrenador(Data entrenador);

/////// CREACION DE LOS HILOS DE CADA ENTRENADOR /////////////////

// esta funcion agarra una lista de entrenadores y devuelve una lista de hilos
t_list* crearHilosDeEntrenadores(t_list* entrenadores){
	typedef void*(*erasedType)(void*);
	return list_map(entrenadores, (erasedType)crearHiloParaEntrenador);
}

// esta funcion agarra un entrenador del tipo Entrenador y lo convierte en un hilo (este seria el estado NEW)
void crearHiloParaEntrenador(Data entrenador, pthread_t* hilo){
	typedef void*(*erasedType)(void*);

	// pthread_create(el hilo creado, por ahora NULL, la funcion (micromain) donde el hilo hace todas sus tareas, los parametros que usa esa funcion)
	pthread_create(&hilo, NULL, (erasedType) funcionesDelEntrenador, entrenador);
}

//la funcion funcionesDelEntrenador tendria que estar en el Team.c, lo dejo aca por ahora
void funcionesDelEntrenador(Data entrenador){
	// por ahora usamos esto como ejemplo, donde de cada hilo te devuelve la posicion en x del entrenador
	Entrenador* unEntrenador = entrenador;

	char* posicionX = string_from_format("La posicion del entrenador esta sig: %d", unEntrenador->posicion->x);

	quickLog(posicionX);
}
