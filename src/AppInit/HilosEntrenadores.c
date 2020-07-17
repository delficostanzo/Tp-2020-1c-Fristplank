
#include "HilosEntrenadores.h"


void crearHiloParaEntrenador(Entrenador* entrenador);
void funcionesDelEntrenador(void* entrenador);

/////// CREACION DE LOS HILOS DE CADA ENTRENADOR /////////////////

//esta funcion agarra una lista de entrenadores e inicializa cada hilo de las estructuras de los entrenadores
void crearHilosDeEntrenadores(){

	pthread_mutex_lock(&mutexEntrenadores);
	if(list_is_empty(entrenadores) != 1){
	for(int index=0; index < list_size(entrenadores); index ++) {
		crearHiloParaEntrenador(list_get(entrenadores, index));
	}
	}
	pthread_mutex_unlock(&mutexEntrenadores);
}

// esta funcion agarra un entrenador del tipo Entrenador y lo convierte en un hilo (este seria el estado NEW)
void crearHiloParaEntrenador(Entrenador* entrenador){
	typedef void*(*erasedType)(void*);
	pthread_t hilo = entrenador->hiloEntrenador;

	// pthread_create(el hilo creado, por ahora NULL, la funcion (micromain) donde el hilo hace todas sus tareas, los parametros que usa esa funcion)
	pthread_create(&hilo, NULL, (erasedType)funcionesDelEntrenador, (void*)entrenador);
	pthread_detach(hilo);
}

//la funcion funcionesDelEntrenador tendria que estar en el Team.c, lo dejo aca por ahora
void funcionesDelEntrenador(void* unEntrenador){

	Entrenador* entrenador = (Entrenador*) unEntrenador;

	quickLog("$-LLEGA ACA");

	pthread_mutex_lock(&entrenador->mutexEstado);
	int cumple = entrenador->estado != 5;
	pthread_mutex_unlock(&entrenador->mutexEstado);
	//bloqueo esperando que otro me active y me da el objetivo
	while(cumple) {
		//el unlock de este mutex lo va a hacer el planificador cuando este en exec
		//el entrenador no se entera
		//pthread_mutex_lock(&(entrenador->mutexEntrenador));
		sem_wait(&entrenador->semaforoExecEntrenador);
		cumplirObjetivo(entrenador);
		pthread_mutex_lock(&entrenador->mutexEstado);
		cumple = entrenador->estado != 5;
		pthread_mutex_unlock(&entrenador->mutexEstado);
	}

	log_info(LO, "--El entrenador %c ya esta en exit", entrenador->numeroEntrenador);

	quickLog("$-un entrenador ya esta en exit");
}

