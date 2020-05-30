
#include "HilosEntrenadores.h"


void crearHiloParaEntrenador(Entrenador* entrenador);
void funcionesDelEntrenador(Data entrenador);

/////// CREACION DE LOS HILOS DE CADA ENTRENADOR /////////////////

//esta funcion agarra una lista de entrenadores y devuelve una lista de hilos
//hilos[list_size(entrenadores)] cuando lo llame inicializar los hilos asi
void crearHilosDeEntrenadores(t_list* entrenadores){
	for(int index=0; index < list_size(entrenadores); index ++) {
		crearHiloParaEntrenador(list_get(entrenadores, index));
	}
}

// esta funcion agarra un entrenador del tipo Entrenador y lo convierte en un hilo (este seria el estado NEW)
void crearHiloParaEntrenador(Entrenador* entrenador){ // ESTADO NEW
	typedef void*(*erasedType)(void*);
	pthread_t* hilo = entrenador->hiloEntrenador;

	// pthread_create(el hilo creado, por ahora NULL, la funcion (micromain) donde el hilo hace todas sus tareas, los parametros que usa esa funcion)
	pthread_create(&hilo, NULL, (erasedType)funcionesDelEntrenador, entrenador);
}

//la funcion funcionesDelEntrenador tendria que estar en el Team.c, lo dejo aca por ahora
void funcionesDelEntrenador(Data entrenador){

	// por ahora usamos esto como ejemplo, donde de cada hilo te devuelve la posicion en x del entrenador
	Entrenador* unEntrenador = entrenador;
	EstadoNew* estadoNew = malloc(sizeof(EstadoNew));

	//Agrego entrenador a la lista del estado NEW
	list_add(estadoNew->hilosEntrenadoresNew, unEntrenador);

//
//	if(no hay ninguno ejecutando){
//		lock();
//		entrenador pasa de READY a EXEC
//		hace lo que tenga en su objetivo (enum de objetivos)
//		entrenador pasa a estado BLOCK / EXIT / READY
//		unlock();
//	}

	//free(estadoNew->hilosEntrenadoresNew);
	//free(estadoNew);
}
