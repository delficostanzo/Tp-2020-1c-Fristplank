
#include "HilosEntrenadores.h"


void crearHiloParaEntrenador(Entrenador* entrenador);
void funcionesDelEntrenador(Entrenador* entrenador);

/////// CREACION DE LOS HILOS DE CADA ENTRENADOR /////////////////

//esta funcion agarra una lista de entrenadores e inicializa cada hilo de las estructuras de los entrenadores
void crearHilosDeEntrenadores(t_list* entrenadores){
	for(int index=0; index < list_size(entrenadores); index ++) {
		crearHiloParaEntrenador(list_get(entrenadores, index));
	}
}

// esta funcion agarra un entrenador del tipo Entrenador y lo convierte en un hilo (este seria el estado NEW)
void crearHiloParaEntrenador(Entrenador* entrenador){ // ESTADO NEW
	typedef void*(*erasedType)(void*);
	pthread_t hilo = entrenador->hiloEntrenador;

	// pthread_create(el hilo creado, por ahora NULL, la funcion (micromain) donde el hilo hace todas sus tareas, los parametros que usa esa funcion)
	pthread_create(&hilo, NULL, (erasedType)funcionesDelEntrenador, entrenador);
	pthread_detach(hilo);
}

//la funcion funcionesDelEntrenador tendria que estar en el Team.c, lo dejo aca por ahora
//ENTRENADOR LLEGA VACIO
void funcionesDelEntrenador(Entrenador* unEntrenador){
	t_log* logger = iniciar_logger();

	pthread_mutex_lock(&(unEntrenador->mutexEntrenador));

	int noEsteEnExit() {
		return unEntrenador->estado != 5;
	}
	//bloqueo esperando que otro me active y me da el objetivo
	while(noEsteEnExit) {
		//el unlock de este mutex lo va a hacer el planificador cuando este en exec
		//el entrenador no se entera
		cumplirObjetivo(unEntrenador);
		pthread_mutex_lock(&(unEntrenador->mutexEntrenador));

	}

	destruirLog(logger);
}

