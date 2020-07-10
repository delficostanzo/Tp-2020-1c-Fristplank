
#include "Planificador.h"

//static bool noHayEntrenadoresEnExec(t_list* entrenadores);
void intercambiarPokemonesCon(Entrenador* entrenadorMovido, Entrenador* entrenadorBloqueado);
static void atrapar(Entrenador* entrenador, PokemonEnElMapa* pokemon);
static Entrenador* asignarObjetivoA(t_list* entrenadoresAMover, PokemonEnElMapa* pokemonLibre);
static void cambiarCantidadEnPokesLibres(PokemonEnElMapa* pokeLibre);
static void cambiarCantidadEnPokesObj(PokemonEnElMapa* pokeLibre);
static Entrenador* buscarEntrenadorSegun(char* algoritmo);
static int noEstanTodosEnExit();
static bool sacarSiCantidadEsCero(PokemonEnElMapa* pokeComoObj);

typedef bool(*erasedTypeFilter)(void*);


void planificarEntrenadores(){

	while(noEstanTodosEnExit()){ // lista de entrenadores que no estan en exit
		// se pasan entrenadores a READY segun su condicion
		pasarAReadyParaAtrapar();
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
void pasarAReadyParaAtrapar(){
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
		int cantidadDePokesLibres = list_size(pokemonesLibres);
		pthread_mutex_unlock(&mutexPokemonesLibres);

		//asignamos objetivo al entrenador mas cercano
		for(int index=0; index < cantidadDePokesLibres; index++){
			PokemonEnElMapa* pokemonLibre = list_get(pokemonesLibres, index);
			Entrenador* entrenadorAReady =  asignarObjetivoA(entrenadoresPosibles, pokemonLibre);

			// cambio de estado al entrenador, pasa a ready
			entrenadorAReady->estado = 2;
			// ese poke se saca de la lista de pokes libres porque ya fue asginado
			cambiarCantidadEnPokesLibres(pokemonLibre);
			// disminuyo la cantidad de ese poke libre en los obj globales (lo saco si cant = 0)
			cambiarCantidadEnPokesObj(pokemonLibre);

			log_info(logger, "Se paso a ready el entrenador");

		}
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

void cambiarCantidadEnPokesLibres(PokemonEnElMapa* pokeLibre){
	pthread_mutex_lock(&mutexPokemonesLibres);
	disminuirCantidadPokemones(pokeLibre, pokemonesLibres);
	pthread_mutex_unlock(&mutexPokemonesLibres);
}

void cambiarCantidadEnPokesObj(PokemonEnElMapa* pokeLibre){
	pthread_mutex_lock(&mutexObjetivosGlobales);
	disminuirCantidadPokemones(pokeLibre, objetivosGlobales);
	pthread_mutex_unlock(&mutexObjetivosGlobales);
}

void disminuirCantidadPokemones(PokemonEnElMapa* pokemonLibre, t_list* listaPokes){
	typedef bool(*erasedTypeRemove)(void*);
	PokemonEnElMapa* pokeComoObj = buscarPorNombre(pokemonLibre->nombre, listaPokes);
	//siempre va a encontrar a ese objetivo porque si estaba libre estaba como obje

	// disminuir la cantidad de ese poke libre en los objetivos globales
	pokeComoObj->cantidad -= 1;

	// (sacarlo si cant = 0)
	list_remove_by_condition(listaPokes, (erasedTypeRemove)sacarSiCantidadEsCero);

}

bool sacarSiCantidadEsCero(PokemonEnElMapa* pokeComoObj){
	return pokeComoObj->cantidad == 0;
}


void pasarAReadyParaIntercambiar(){
	t_list* entrenadoresDeadlock = entrenadoresBloqueadosPorDeadlock();
	for(int index = 0; index < list_size(entrenadoresDeadlock); index++) {
		Entrenador* bloqueado = list_get(entrenadoresDeadlock, index);
		//se pasan invertidos los pokemones porque este pokemon necesitado es de un entrenador que pasaria como innecesario de OTRO entrenador
		Entrenador* entrenadorDeIntercambio = buscarEntrenadorParaIntercambiar(bloqueado->movimientoEnExec->pokemonNecesitado, bloqueado->movimientoEnExec->pokemonAIntercambiar);
		if(entrenadorDeIntercambio != NULL) {
			//pasa a ready, sus movimientos ya estan definidos
			entrenadorDeIntercambio->estado = 3;
		}
	}

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
		Entrenador* entrenadorQueCumplen = list_find(entrenadoresPosibles, (erasedTypeFilter)entrenadorCumpleCondicion);
		return entrenadorQueCumplen;
	}
	return NULL;
}




///////EXEC///////////


//solo se puede pasar un entrenador a estado EXEC si no hay ninguno en estado EXEC
void pasarAExec(){

	Entrenador* entrenador = buscarEntrenadorSegun(ALGORITMO);

	if(entrenadorExec() == NULL){
		//sem_wait(&semaforoEstados);
		entrenador->estado = 3;
		//sem_post(&semaforoEstados);
		pthread_mutex_unlock(&(entrenador->mutexEntrenador));
	}

}

Entrenador* buscarEntrenadorSegun(char* algoritmo) {
	if(strcmp(algoritmo, "FIFO") == 0){
		//el primer entrenador de los ready es el que esta en indice 0
		Entrenador* entrenador = list_get(entrenadoresReady(), 0);
		return entrenador;
	} else {//ROUND ROBIN -> se atiende por orden de llegada dejando que ejecute hasta maximo de QUANTUM
		Entrenador* entrenador = list_get(entrenadoresReady(), 0);
		return NULL;
	}
}



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
			intercambiarPokemonesCon(entrenador, entrenadorDeIntercambio);
			break;
	}
}

void intercambiarPokemonesCon(Entrenador* entrenadorMovido, Entrenador* entrenadorBloqueado){
	int distanciaHastaBloqueado = distanciaEntre(entrenadorMovido->posicion, entrenadorBloqueado->posicion);

	//si es por fifo o es round robin pero le alcanza el quantum para moverse e intercambiar
	if((strcmp(ALGORITMO, "FIFO") == 0) || (QUANTUM >= distanciaHastaBloqueado +5)) {
		entrenadorMovido->posicion = entrenadorBloqueado->posicion;
		entrenadorMovido->ciclosCPUConsumido += distanciaHastaBloqueado;
		entrenadorMovido->ciclosCPUConsumido += 5;
		PokemonEnElMapa* nuevoAtrapadoDelMovido = entrenadorMovido->movimientoEnExec->pokemonNecesitado;
		PokemonEnElMapa* nuevoAtrapadoDelBloqueado = entrenadorMovido->movimientoEnExec->pokemonAIntercambiar;

		//agrego en el que se mueve el pokemon que tenia el que se queda quieto
		setPokemonA(entrenadorMovido->pokemonesAtrapados, nuevoAtrapadoDelBloqueado);
		//y le saco el qe el tenia de mas (el que se quedo quieto)
		disminuirCantidadPokemones(nuevoAtrapadoDelBloqueado, entrenadorMovido->pokemonesAtrapados);

		//agrego en el que se quedo quieto el pokemon que tenia el otro
		setPokemonA(entrenadorBloqueado->pokemonesAtrapados, nuevoAtrapadoDelBloqueado);
		//saco el que tenia de mas
		disminuirCantidadPokemones(nuevoAtrapadoDelMovido, entrenadorBloqueado->pokemonesAtrapados);

		estadoSiAtrapo(entrenadorMovido);
		estadoSiAtrapo(entrenadorBloqueado);

	} else if(QUANTUM == distanciaHastaBloqueado) { //llega a moverse pero no a hacer los intercambios
			entrenadorMovido->posicion = entrenadorBloqueado->posicion;
			entrenadorMovido->ciclosCPUConsumido += distanciaHastaBloqueado;

			pasarADeadlock(entrenadorMovido);

		} else if(QUANTUM > distanciaHastaBloqueado) { //llega a hacer la distancia y consume parte de ciclo de intercambio
					entrenadorMovido->posicion = entrenadorBloqueado->posicion;
					//lo que sobro del quantum
					int sobrante = QUANTUM - distanciaHastaBloqueado;
					entrenadorMovido->ciclosCPUConsumido += distanciaHastaBloqueado + (QUANTUM - sobrante);

					entrenadorMovido->ciclosCPUFaltantesIntercambio = sobrante;
					pasarADeadlock(entrenadorMovido);

				} else { //
					//moverseDistanciaMenorAQ(entrenadorMovido, entrenadorBloqueado->posicion->posicionX, entrenadorBloqueado->posicion->posicionY, distanciaHastaBloqueado);

					pasarADeadlock(entrenadorMovido);
				}
}


void atrapar(Entrenador* entrenador, PokemonEnElMapa* pokemon) {
	quickLog("Llega al metodo de atrapar");
	int distanciaHastaPokemon = distanciaEntre(&(pokemon->posicion), entrenador->posicion);
	quickLog("Calcula la distancia al pokemon");

	//CONDICIONES SEGUN EL ALGORITMO
	if(strcmp(ALGORITMO, "FIFO") == 0 || QUANTUM >= distanciaHastaPokemon){
		entrenador->ciclosCPUConsumido += distanciaHastaPokemon;
		entrenador->posicion = &(pokemon->posicion);
	} else{// distancia hasta el poke es menor que el quantum
		// va a entrar a esta condicion hasta que la distancia se reduzca y sea menor que el quantum
		moverseSegunElQuantum(entrenador, pokemon, distanciaHastaPokemon);
	}

	//el socket ya esta conectado con el broker en Conexion
	sem_wait(&semaforoCatch);
	enviarCatchDesde(entrenador);
	sem_post(&semaforoCatch);

	pasarABlockEsperando(entrenador);
	quickLog("Se paso a estado bloqueado esperando respuesta");

}

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
