
#include "HilosEntrenadores.h"
#include "../Model/Trainer.h"
#include "../TeamUtils/LogManager.h"
#include "ConfigFunctions.h"

void funcionesDelEntrenador(Data entrenador);
void crearHiloParaEntrenador(Data entrenador, pthread_t* hilo);

/////// CREACION DE LOS HILOS DE CADA ENTRENADOR /////////////////

//esta funcion agarra una lista de entrenadores y devuelve una lista de hilos
//hilos[list_size(entrenadores)] cuando lo llame inicializar los hilos asi
void crearHilosDeEntrenadores(t_list* entrenadores, pthread_t* hilos[]){
	for(int index=0; index < list_size(entrenadores); index ++) {
		crearHiloParaEntrenador(list_get(entrenadores, index), hilos[index]);
	}
}

// esta funcion agarra un entrenador del tipo Entrenador y lo convierte en un hilo (este seria el estado NEW)
void crearHiloParaEntrenador(Data entrenador, pthread_t* hilo){ // ESTADO NEW -TODO
	typedef void*(*erasedType)(void*);

	// pthread_create(el hilo creado, por ahora NULL, la funcion (micromain) donde el hilo hace todas sus tareas, los parametros que usa esa funcion)
	pthread_create(&hilo, NULL, (erasedType)funcionesDelEntrenador, entrenador);
}

//la funcion funcionesDelEntrenador tendria que estar en el Team.c, lo dejo aca por ahora
void funcionesDelEntrenador(Data entrenador){
	// por ahora usamos esto como ejemplo, donde de cada hilo te devuelve la posicion en x del entrenador
	Entrenador* unEntrenador = entrenador;

	char* posicionX = string_from_format("La posicion del entrenador esta sig: %d", unEntrenador->posicion->x);

	while(true) {
		quickLog(posicionX);
		sleep(3);
	}
}
