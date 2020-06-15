
#include "HilosEntrenadores.h"


void crearHiloParaEntrenador(Entrenador* entrenador);
void funcionesDelEntrenador(Data entrenador);

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
}

//la funcion funcionesDelEntrenador tendria que estar en el Team.c, lo dejo aca por ahora
void funcionesDelEntrenador(void* entrenador){

	while(1){
		//cuando quiero cambiar de estado, dentro de la lista de entrenadores debo fijarme cual es el que tiene enum en ese estado
		Entrenador* unEntrenador = entrenador;
		switch(unEntrenador->estado){
		case NEW:
			//se odena por distancia mas corta (el entrenador mas cerca de ese poke) y se pasa a ready
			break;
		case READY:
			// para que se pase a estado EXEC, se hace por fifo, primero se verifica que ningun entrenador este en EXEC
			break;
		case EXEC:
			cumplirObjetivo(entrenador);
			// aca hay 3 codiciones: moverse y atrapar en el mapa, intercambiar y mover
			break;
		case BLOCK:
			//el entrenador que va a pasar a estado READY es el que tenga la distancia mas cerca al poke
			//condiciones: esperando respuesta del broker o esperando otro en BLOCK por intercambio de pokes
			break;
		case EXIT:
			//cada vez que entrea un entrenador, se verifica que se cumple el objetivo global
			break;
		}
	}

void cumplirObjetivo(Entrenador* entrenador){
	MovimientoEnExec* movimientoEnExec = entrenador->movimientoEnExec;
	ObjetivoEnExec mision = movimientoEnExec->objetivo;

	switch(mision){
		case MOVERyATRAPAR:
			atrapar(movimientoEnExec->pokemonNecesitado);
			break;
		case MOVEReINTERCAMBIAR:
			Entrenador* entrenadorDeIntercambio = buscarEntrenadorParaIntercambiar(movimientoEnExec->pokemonNecesitado, movimientoEnExec->pokemonAIntercambiar);
			intercambiarPokemonesCon(entrenadorDeIntercambio);

	//cambiar de estado  => planificador
}

void intercambiarPokemonesCon(Entrenador* entrenadorDeIntercambio){
	//TODO
}

//
//	if(no hay ninguno ejecutando){
//		lock();
//		entrenador pasa de READY a EXEC
//		hace lo que tenga en su objetivo (enum de objetivos)
//		entrenador pasa a estado BLOCK / EXIT / READY
//		unlock();
//	}

}

