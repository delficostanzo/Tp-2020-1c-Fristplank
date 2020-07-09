
#include "Planificador.h"

//static bool noHayEntrenadoresEnExec(t_list* entrenadores);
static void intercambiarPokemonesCon(Entrenador* entrenadorDeIntercambio);
static void atrapar(Entrenador* entrenador, PokemonEnElMapa* pokemon);
static Entrenador* asignarObjetivoA(t_list* entrenadoresAMover, PokemonEnElMapa* pokemonLibre);
static Entrenador* buscarEntrenadorSegun(char* algoritmo);
static int noEstanTodosEnExit();

typedef bool(*erasedTypeFilter)(void*);


void planificarEntrenadores(){


	while(noEstanTodosEnExit()){ // lista de entrenadores que no estan en exit
		// se pasan entrenadores a READY segun su condicion
		pasarAReady();
		pasarAExec();

	}

}

int noEstanTodosEnExit(){
	typedef bool(*erasedType)(void*);
	pthread_mutex_lock(&mutexEntrenadores);
	int noTodosExit = list_any_satisfy(entrenadores, (erasedType)noEstaEnExit);
	pthread_mutex_unlock(&mutexEntrenadores);
	return noTodosExit;
}

//paso a ready los entrenadores que esten mas cerca
void pasarAReady(){
	typedef bool(*erasedTypeFilter)(void*);
	t_log* logger = iniciar_logger();

	int tieneEstadoNewODormido(Entrenador* entrenador) {
		return entrenador->estado==1 || (entrenador->estado==4 && entrenador->motivo==2);
	}
	pthread_mutex_lock(&mutexEntrenadores);
	t_list* entrenadoresPosibles = list_filter(entrenadores, (erasedTypeFilter)tieneEstadoNewODormido);
	pthread_mutex_unlock(&mutexEntrenadores);

	if(list_is_empty(entrenadoresPosibles) != 1) {
		pthread_mutex_lock(&mutexPokemonesLibres);
		//asignamos objetivo al entrenador mas cercano
		for(int index=0; index < list_size(pokemonesLibres); index++){


			PokemonEnElMapa* pokemonLibre = list_get(pokemonesLibres, index);
			Entrenador* entrenadorAReady =  asignarObjetivoA(entrenadoresPosibles, pokemonLibre);
			// cambio de estado al entrenador, pasa a ready
			entrenadorAReady->estado = 2;
			// ese poke se saca de la lista de pokes libres porque ya fue asginado
			disminuirCantidadPokemones(pokemonLibre, pokemonesLibres);

			pthread_mutex_lock(&mutexObjetivosGlobales);
			// disminuyo la cantidad de ese poke libre en los obj globales (lo saco si cant = 0)
			disminuirCantidadPokemones(pokemonLibre, objetivosGlobales);
			log_info(logger, "Se paso a ready el entrenador");
			pthread_mutex_unlock(&mutexObjetivosGlobales);

		}
		pthread_mutex_unlock(&mutexPokemonesLibres);

	}
	destruirLog(logger);
}

Entrenador* asignarObjetivoA(t_list* entrenadoresAMover, PokemonEnElMapa* pokemonLibre){
	Entrenador* entrenadorAAsignar = entrenadorMasCercanoA(pokemonLibre, entrenadoresAMover);
	MovimientoEnExec* movimiento = malloc(sizeof(MovimientoEnExec));
	movimiento->pokemonNecesitado = asignarPokemonCopia(pokemonLibre);
	entrenadorAAsignar->movimientoEnExec = movimiento;
	entrenadorAAsignar->movimientoEnExec->objetivo = 1;

	return entrenadorAAsignar;
}

void disminuirCantidadPokemones(PokemonEnElMapa* pokemonLibre, t_list* listaPokes){
	typedef bool(*erasedTypeRemove)(void*);
	PokemonEnElMapa* pokeComoObj = buscarPorNombre(pokemonLibre->nombre, listaPokes);
	//siempre va a encontrar a ese objetivo porque si estaba libre estaba como obje

	bool sacarSiCantidadEsCero(PokemonEnElMapa* pokeComoObj){
		return pokeComoObj->cantidad == 0;
	}


	// disminuir la cantidad de ese poke libre en los objetivos globales
	//TODO
	pokeComoObj->cantidad -= 1;

	// (sacarlo si cant = 0)
	list_remove_by_condition(listaPokes, (erasedTypeRemove)sacarSiCantidadEsCero);

}



/////////


//solo se puede pasar un entrenador a estado EXEC si no hay ninguno en estado EXEC
void pasarAExec(){

	Entrenador* entrenador = buscarEntrenadorSegun(ALGORITMO);

	if(entrenadorExec() == NULL){
		entrenador->estado = 3;
		pthread_mutex_unlock(&(entrenador->mutexEntrenador));
	}

}

Entrenador* buscarEntrenadorSegun(char* algoritmo) {
	if(strcmp(algoritmo, "FIFO") == 0){
		//el primer entrenador de los ready es el que esta en indice 0
		Entrenador* entrenador = list_get(entrenadoresReady(), 0);
		return entrenador;
	} else {//ROUND ROBIN
		//TODO
		return NULL;
	}
}


////chequeo que no hay entrenadores en exec
//bool noHayEntrenadoresEnExec(t_list* entrenadores){
//	typedef bool(*erasedType)(void*);
//	t_list* entrenadoresEnReady = entrenadoresReady();
//
//	bool noEstaEnExec(Entrenador* entrenador){
//		return entrenador->estado != 3;
//	}
//	//determino si todos los entrenadores cumplen que no haya ninguno en estado exec
//	return list_all_satisfy(entrenadoresEnReady, (erasedType)noEstaEnExec);
//}


///////EXEC///////////

void cumplirObjetivo(Entrenador* entrenador){
	quickLog("Se esta cumpliendo un objetivo");
	MovimientoEnExec* movimientoEnExec = entrenador->movimientoEnExec;
	ObjetivoEnExec mision = movimientoEnExec->objetivo;
	Entrenador* entrenadorDeIntercambio;
	quickLog("Llega a cargar los datos del objetivo del entrenador en exec");

	switch(mision){
		case MOVERyATRAPAR:
			//se mueve hasta ese pokemon, manda el catch de ese pokemon,
			//se guarda el id del catch que va a esperar como id correlativo en el caught y se cambia de estado
			atrapar(entrenador, movimientoEnExec->pokemonNecesitado);
			quickLog("Se mando el catch de un nuevo pokemon");
			break;
		case MOVEReINTERCAMBIAR:
			//se pasan invertidos los pokemones porque este pokemon necesitado es de un entrenador que pasaria como innecesario de OTRO entrenador
			entrenadorDeIntercambio = buscarEntrenadorParaIntercambiar(movimientoEnExec->pokemonNecesitado, movimientoEnExec->pokemonAIntercambiar);
			intercambiarPokemonesCon(entrenadorDeIntercambio);
			break;
	}
}

void intercambiarPokemonesCon(Entrenador* entrenadorDeIntercambio){
	//TODO
}

//se busca al entrenador que necesite el que yo tengo de mas y tenga el que yo necesito
//si no lo encuentra devuelve NULL
Entrenador* buscarEntrenadorParaIntercambiar(PokemonEnElMapa* pokemonInnecesario, PokemonEnElMapa* pokemonNecesitado) {
	t_list* entrenadoresPosibles = entrenadoresBloqueadosPorDeadlock();

	int entrenadorCumpleCondicion(Entrenador* entrenador) {
		return puedeIntercambiar(entrenador, pokemonInnecesario, pokemonNecesitado);
	}
	//si hay otro entrenador en deadlock
	if(list_is_empty(entrenadoresPosibles) != 1){
		t_list* entrenadoresQueCumplen = list_filter(entrenadoresPosibles, (erasedTypeFilter)entrenadorCumpleCondicion);
		if(list_is_empty(entrenadoresQueCumplen) != 1) {
			//agarra el primero que cumpla si al menos hay 1
			return list_get(entrenadoresQueCumplen, 0);
		}
	}
	return NULL;
}



//mientras este en ready va a hacer que se fije entre todos los demas que estan ready y pasar a ready el mas cercano
//void moverAReady() {
//	pthread_mutex_lock(&mutexEntrenadores);
//	pthread_mutex_lock(&mutexPokemonesLibres);
//	pasarDeNewAReady(entrenadores, pokemonesLibres);
//	pthread_mutex_unlock(&mutexEntrenadores);
//	pthread_mutex_unlock(&mutexPokemonesLibres);
//}

void verificarSiTodosExit() {
	int estaEnExec(Entrenador* entrenador) {
		return entrenador->estado == 5;
	}
	pthread_mutex_lock(&mutexEntrenadores);
	int cumple = list_all_satisfy(entrenadores, (erasedTypeFilter)estaEnExec);
	pthread_mutex_unlock(&mutexEntrenadores);

	if(cumple) {
		//TERMINAR TODO;
	}
}

void atrapar(Entrenador* entrenador, PokemonEnElMapa* pokemon) {
	quickLog("Llega al metodo de atrapar");
	int distanciaHastaPokemon = distanciaEntre(&(pokemon->posicion), entrenador->posicion);
	quickLog("Calcula la distancia al pokemon");
	entrenador->ciclosCPUConsumido += distanciaHastaPokemon;
	entrenador->posicion = &(pokemon->posicion);

	//el socket ya esta conectado con el broker en Conexion
	sem_wait(&semaforoCatch);
	enviarCatchDesde(entrenador);
	sem_post(&semaforoCatch);

	pasarABlockEsperando(entrenador);
	quickLog("Se paso a estado bloqueado esperando respuesta");

}
