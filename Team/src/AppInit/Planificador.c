
#include "Planificador.h"

//static bool noHayEntrenadoresEnExec(t_list* entrenadores);
void intercambiarPokemonesCon(Entrenador* entrenadorMovido, Entrenador* entrenadorBloqueado);
static void atrapar(Entrenador* entrenador, PokemonEnElMapa* pokemon);
static Entrenador* asignarObjetivoA(t_list* entrenadoresAMover, PokemonEnElMapa* pokemonLibre);
static void cambiarCantidadEnPokesLibres(PokemonEnElMapa* pokeLibre);
static void cambiarCantidadEnPokesObj(PokemonEnElMapa* pokeLibre);
static Entrenador* buscarPrimerEntrenador();
static bool noEstanTodosEnExit();
static bool sacarSiCantidadEsCero(PokemonEnElMapa* pokeComoObj);
static int terminarSiTodosExit();
static void logearResultadosEntrenadores();
static int ciclosTotales();
static int cantidadPokesLibres();

typedef bool(*erasedTypeFilter)(void*);


void planificarEntrenadores(){
	int cumple = noEstanTodosEnExit();

	//sem_wait(&arrancarPlan);
	while(cumple){ // lista de entrenadores que no estan en exit
		// se pasan entrenadores a READY segun su condicion
		if(cantidadPokesLibres() != 0) {
			pasarAReadyParaAtrapar();
		}
//

//		pasarAReadyParaIntercambiar();
//
//		log_info(LO, "Me quede qui");
		sem_wait(&esperandoPasarAlgunoAExec);
//		log_info(LO, "Tome el lock");
		pasarAExec();
		if(cantidadPokesLibres() != 0) {
			pasarAReadyParaAtrapar();
		}
		t_list* entrenadoresDeadlock = entrenadoresBloqueadosPorDeadlock();

		if(list_size(entrenadoresDeadlock) >= 2) {
			Entrenador* entrenador1 = list_get(entrenadoresDeadlock,0);
			Entrenador* entrenador2 = list_get(entrenadoresDeadlock,1);
			log_info(LO,"Los entrenadores ue estan en deadlock son %c y %c", entrenador1->numeroEntrenador, entrenador2->numeroEntrenador);
			pasarAReadyParaIntercambiar(entrenadoresDeadlock);
			sem_post(&esperandoPasarAlgunoAExec);
		}



		if(terminarSiTodosExit()) {
			break;
		}

		//cumple = noEstanTodosEnExit();
	}
}

bool noEstanTodosEnExit(){
	typedef bool(*erasedType)(void*);
	pthread_mutex_lock(&mutexEntrenadores);
	bool noTodosExit = list_any_satisfy(entrenadores, (erasedType)noEstaEnExit);
	pthread_mutex_unlock(&mutexEntrenadores);
	return noTodosExit;
}


///////////READY/////////////////

int cantidadPokesLibres() {
	pthread_mutex_lock(&mutexPokemonesLibres);
	int cantidadDePokesLibres = list_size(pokemonesLibres);
	pthread_mutex_unlock(&mutexPokemonesLibres);
	return cantidadDePokesLibres;
}

//paso a ready los entrenadores que esten mas cerca
void pasarAReadyParaAtrapar(){
	typedef bool(*erasedTypeFilter)(void*);


	int cantidad = cantidadPokesLibres();
	//asignamos objetivo al entrenador mas cercano
	for(int index=0; index < cantidad; index++){

		pthread_mutex_lock(&mutexEntrenadores);
		t_list* entrenadoresPosibles = list_filter(entrenadores, (erasedTypeFilter)tieneEstadoNewODormido);
		int cantidadPosibles = list_size(entrenadoresPosibles);
		pthread_mutex_unlock(&mutexEntrenadores);

		if(cantidadPosibles != 0) {

			pthread_mutex_lock(&mutexPokemonesLibres);
			PokemonEnElMapa* pokemonLibre = list_get(pokemonesLibres, index);
			pthread_mutex_unlock(&mutexPokemonesLibres);

			if(pokemonLibre != NULL){
				pthread_mutex_lock(&mutexEntrenadores);
				//apuntan a los mismos entrenadores globales
				Entrenador* entrenadorAsignado =  asignarObjetivoA(entrenadoresPosibles, pokemonLibre);
				pthread_mutex_unlock(&mutexEntrenadores);

				agregarAListaReady(entrenadorAsignado);

				// cambio de estado al entrenador, pasa a ready
				pthread_mutex_lock(&entrenadorAsignado->mutexEstado);
				entrenadorAsignado->estado = 2;
				pthread_mutex_unlock(&entrenadorAsignado->mutexEstado);


				// ese poke se saca de la lista de pokes libres porque ya fue asginado
				cambiarCantidadEnPokesLibres(pokemonLibre);
				// disminuyo la cantidad de ese poke libre en los obj globales (lo saco si cant = 0)
				cambiarCantidadEnPokesObj(pokemonLibre);
				log_info(LO, "El entrenador %c paso a estado ready para atrapar al pokemon %s", entrenadorAsignado->numeroEntrenador, pokemonLibre->nombre);

				sem_post(&esperandoPasarAlgunoAExec);
			}

		}
	}
	//destruirLog(logger);
}



Entrenador* asignarObjetivoA(t_list* entrenadoresAMover, PokemonEnElMapa* pokemonLibre){
	Entrenador* entrenadorAsignado = entrenadorMasCercanoA(pokemonLibre, entrenadoresAMover);
	log_info(LO, "--El entrenador mas cercano al poke %s es el %c", pokemonLibre->nombre, entrenadorAsignado->numeroEntrenador);

	MovimientoEnExec* movimiento = malloc(sizeof(MovimientoEnExec));
	movimiento->pokemonNecesitado = asignarPokemonCopia(pokemonLibre);
	entrenadorAsignado->movimientoEnExec = movimiento;
	//entrenadorAAsignar->movimientoEnExec->pokemonNecesitado = asignarPokemonCopia(pokemonLibre);
	entrenadorAsignado->movimientoEnExec->objetivo = 1;


	return entrenadorAsignado;
}

void cambiarCantidadEnPokesLibres(PokemonEnElMapa* pokeLibre){
	pthread_mutex_lock(&mutexPokemonesLibres);
	disminuirCantidadPokemones(pokeLibre, pokemonesLibres);
	pthread_mutex_unlock(&mutexPokemonesLibres);
}

void cambiarCantidadEnPokesObj(PokemonEnElMapa* pokeLibre){
	pthread_mutex_lock(&mutexObjetivosGlobales);
	//saco el mutex porque esta funcion es llamada dentro de dicho mutex
	disminuirCantidadPokemones(pokeLibre, objetivosGlobales);
	pthread_mutex_unlock(&mutexObjetivosGlobales);
}

void disminuirCantidadPokemones(PokemonEnElMapa* pokemonLibre, t_list* listaPokes){
	typedef bool(*erasedTypeRemove)(void*);
	PokemonEnElMapa* pokeComoObj = buscarPorNombre(pokemonLibre->nombre, listaPokes);
	//siempre va a encontrar a ese objetivo porque si estaba libre estaba como obje

	// disminuir la cantidad de ese poke libre en los objetivos globales
	if(pokeComoObj != NULL) {
		pokeComoObj->cantidad -= 1;
	}


	// (sacarlo si cant = 0)
	list_remove_by_condition(listaPokes, (erasedTypeRemove)sacarSiCantidadEsCero);

}

bool sacarSiCantidadEsCero(PokemonEnElMapa* pokeComoObj){
	return pokeComoObj->cantidad == 0;
}


void pasarAReadyParaIntercambiar(t_list* entrenadoresDeadlock){
//	t_list* entrenadoresDeadlock = entrenadoresBloqueadosPorDeadlock();
//	if(list_size(entrenadoresDeadlock) >= 2) {
		log_info(LO, "Inicio del algoritmo de deteccion de deadlock (comienza a buscar con quien intercambiar)");

		for(int index = 0; index < list_size(entrenadoresDeadlock); index++) {
			Entrenador* bloqueado = list_get(entrenadoresDeadlock, index);

			//se pasan invertidos los pokemones porque este pokemon necesitado es de un entrenador que pasaria como innecesario de OTRO entrenador
			Entrenador* entrenadorDeIntercambio = buscarEntrenadorParaIntercambiar(bloqueado->movimientoEnExec->pokemonNecesitado);
			t_list* entrenadoresPosibles = entrenadoresBloqueadosPorDeadlock();
			//si existe un entrenador que tenga para intercambiar el que el bloqueado necesita y mientras no este por ser intercambiado por otro entrenador
			if(entrenadorDeIntercambio != NULL && esteComoIntercambioEntre(entrenadoresPosibles, entrenadorDeIntercambio) != 1) {
				//pasa a ready, sus movimientos ya estan definidos
				pthread_mutex_lock(&entrenadorDeIntercambio->mutexEstado);
				entrenadorDeIntercambio->estado = 2;
				pthread_mutex_unlock(&entrenadorDeIntercambio->mutexEstado);

				log_info(LO, "Lo que te guste");

				// TODO: No cuenta la cantidad de deadlocks, no se si es porque no le gusta hacer el != con un char
				if(entrenadorDeIntercambio->movimientoEnExec->numeroDelEntrenadorIntercambio != 0 && bloqueado->movimientoEnExec->numeroDelEntrenadorIntercambio != 0) {
					cantidadDeadlocks ++;
				}

				//me guardo con que entrenador intercambiar cuando pase a exec y el que esta bloqueado a cual esta esperando
				entrenadorDeIntercambio->movimientoEnExec->numeroDelEntrenadorIntercambio = bloqueado->numeroEntrenador;
				bloqueado->movimientoEnExec->numeroDelEntrenadorIntercambio = entrenadorDeIntercambio->numeroEntrenador;
				agregarAListaReady(entrenadorDeIntercambio);
				log_info(LO, "El entrenador %c paso a estado ready para intercambiar con el entrenador %c", entrenadorDeIntercambio->numeroEntrenador, bloqueado->numeroEntrenador);

//				sem_post(&esperandoPasarAlgunoAExec);
				return;
			}
		}
//	}

}

//se busca al entrenador que necesite el que yo tengo de mas y tenga el que yo necesito
//si no lo encuentra devuelve NULL
Entrenador* buscarEntrenadorParaIntercambiar(PokemonEnElMapa* pokemonNecesitado) {
	t_list* entrenadoresPosibles = entrenadoresBloqueadosPorDeadlock();

	int entrenadorCumpleCondicion(Entrenador* entrenador) {
		char* nombrePokemonIntercambiado = entrenador->movimientoEnExec->pokemonAIntercambiar->nombre;

		return strcmp(nombrePokemonIntercambiado, pokemonNecesitado->nombre) == 0;
	}

	//si hay otro entrenador en deadlock
	pthread_mutex_lock(&mutexEntrenadores);
	Entrenador* entrenadorQueCumplen = list_find(entrenadoresPosibles, (erasedTypeFilter)entrenadorCumpleCondicion);
	//no lo encuentra
	pthread_mutex_unlock(&mutexEntrenadores);
	return entrenadorQueCumplen;

}




///////EXEC///////////


//solo se puede pasar un entrenador a estado EXEC si no hay ninguno en estado EXEC
void pasarAExec(){
	pthread_mutex_lock(&mutexListaEntrenadoresReady);
	Entrenador* entrenador = buscarPrimerEntrenador();
	pthread_mutex_unlock(&mutexListaEntrenadoresReady);

	//si no hay ningun entrenador en exec y quedan entrenadores en ready
	if(entrenadorExec() == NULL && entrenador != NULL){
		CC ++;
		char* charDelMovimiento = obtenerCharDeMov(entrenador->movimientoEnExec->objetivo);
		//sem_wait(&semaforoEstados);
		sacarDeListaReady(entrenador);

		pthread_mutex_lock(&entrenador->mutexEstado);
		entrenador->estado = 3;
		pthread_mutex_unlock(&entrenador->mutexEstado);

		log_info(LO, "El entrenador %c paso a estado exec para %s", entrenador->numeroEntrenador, charDelMovimiento);
		//sem_post(&semaforoEstados);
		//pthread_mutex_unlock(&(entrenador->mutexEntrenador));
		sem_post(&entrenador->semaforoExecEntrenador);
	}

}

Entrenador* buscarPrimerEntrenador() {

	//el primer entrenador de los ready es el que esta en indice 0
	//pthread_mutex_lock(&mutexListaEntrenadoresReady);
	if(list_is_empty(listaEntrenadoresReady) != 1) {
		Entrenador* entrenador = list_get(listaEntrenadoresReady, 0);
		//pthread_mutex_unlock(&mutexListaEntrenadoresReady);
		return entrenador;
	}
	//pthread_mutex_unlock(&mutexListaEntrenadoresReady);
	return NULL;
}


//se supone que este no esta como posible para pasar a ready, pero ni idea
void cumplirObjetivo(Entrenador* entrenador){
	pthread_mutex_lock(&entrenador->mutexEstado);
	int cumple = entrenador->estado != 5;
	pthread_mutex_unlock(&entrenador->mutexEstado);
	if(cumple){
	quickLog("$-Se esta cumpliendo un objetivo");
	MovimientoEnExec* movimientoEnExec = entrenador->movimientoEnExec;
	ObjetivoEnExec mision = movimientoEnExec->objetivo;
	Entrenador* entrenadorDeIntercambioBloqueado;
	quickLog("$-Llega a cargar los datos del objetivo del entrenador en exec");

	switch(mision){
		case MOVERyATRAPAR:
			//se mueve hasta ese pokemon, manda el catch de ese pokemon,
			//se guarda el id del catch que va a esperar como id correlativo en el caught y se cambia de estado
			log_info(LO, "El entrenador %c va a atrapar al pokemon %s en la posicion (%d, %d)", entrenador->numeroEntrenador, movimientoEnExec->pokemonNecesitado->nombre, movimientoEnExec->pokemonNecesitado->posicion.posicionX, movimientoEnExec->pokemonNecesitado->posicion.posicionY);

			atrapar(entrenador, movimientoEnExec->pokemonNecesitado);
			quickLog("$-Se mando el catch de un nuevo pokemon");
			break;
		case MOVEReINTERCAMBIAR:
			entrenadorDeIntercambioBloqueado = buscarPorNumero(entrenador->movimientoEnExec->numeroDelEntrenadorIntercambio);
			log_info(LO, "El entrenador %c va a intercambiar con el entrenador %c", entrenador->numeroEntrenador, entrenadorDeIntercambioBloqueado->numeroEntrenador);

			intercambiarPokemonesCon(entrenador, entrenadorDeIntercambioBloqueado);
			break;
	}
	}
}

void intercambiarPokemonesCon(Entrenador* entrenadorMovido, Entrenador* entrenadorBloqueado){
	int distanciaHastaBloqueado = distanciaEntre(entrenadorMovido->posicion, entrenadorBloqueado->posicion);

	//si es por fifo o es round robin pero le alcanza el quantum para moverse e intercambiar
	if((strcmp(ALGORITMO, "FIFO") == 0) || ((strcmp(ALGORITMO, "RR") == 0) && QUANTUM >= (distanciaHastaBloqueado + entrenadorMovido->ciclosCPUFaltantesIntercambio))) {
		entrenadorMovido->posicion = entrenadorBloqueado->posicion;
		log_info(LO, "El entrenador %c se movio a la posicion (%d, %d)", entrenadorMovido->numeroEntrenador, entrenadorMovido->posicion->posicionX, entrenadorMovido->posicion->posicionY);
		cantidadIntercambios ++;
		//si ninguno de los 2 intercambio antes

		entrenadorMovido->ciclosCPUConsumido += distanciaHastaBloqueado;
		entrenadorMovido->ciclosCPUConsumido += entrenadorMovido->ciclosCPUFaltantesIntercambio;
		PokemonEnElMapa* nuevoAtrapadoDelMovido = entrenadorBloqueado->movimientoEnExec->pokemonAIntercambiar;
		PokemonEnElMapa* nuevoAtrapadoDelBloqueado = entrenadorMovido->movimientoEnExec->pokemonAIntercambiar;

		//agrego en el que se mueve el pokemon que tenia el que se queda quieto
		setPokemonA(entrenadorMovido->pokemonesAtrapados, nuevoAtrapadoDelMovido);
		//y le saco el que el tenia de mas (el que se quedo quieto)
		disminuirCantidadPokemones(nuevoAtrapadoDelBloqueado, entrenadorMovido->pokemonesAtrapados);

		//agrego en el que se quedo quieto el pokemon que tenia el otro
		setPokemonA(entrenadorBloqueado->pokemonesAtrapados, nuevoAtrapadoDelBloqueado);
		//saco el que tenia de mas
		disminuirCantidadPokemones(nuevoAtrapadoDelMovido, entrenadorBloqueado->pokemonesAtrapados);

		log_info(LO, "Se resolvio el deadlock entre los entrenadores %c y %c", entrenadorMovido->numeroEntrenador, entrenadorBloqueado->numeroEntrenador);
		//para que pueda volver a deadlock y no este en el intercambio viejo
		entrenadorMovido->movimientoEnExec->numeroDelEntrenadorIntercambio = 'Z';
		entrenadorBloqueado->movimientoEnExec->numeroDelEntrenadorIntercambio = 'Z';
		estadoSiAtrapo(entrenadorMovido);
		sem_post(&esperandoPasarAlgunoAExec);
		estadoSiAtrapo(entrenadorBloqueado);


	} else if(QUANTUM == distanciaHastaBloqueado) { //llega a moverse pero no a hacer los intercambios
			entrenadorMovido->posicion = entrenadorBloqueado->posicion;
			entrenadorMovido->ciclosCPUConsumido += distanciaHastaBloqueado;
			log_info(LO, "El entrenador %c se movio a la posicion (%d, %d)", entrenadorMovido->numeroEntrenador, entrenadorMovido->posicion->posicionX, entrenadorMovido->posicion->posicionY);

			pasarAReadyPorQuantum(entrenadorMovido);
			log_info(LO, "El entrenador %c paso devuelta a ready porque no le alcanzo el Quantum para intercambiar", entrenadorMovido->numeroEntrenador);

		} else if(QUANTUM > distanciaHastaBloqueado) { //llega a hacer la distancia y consume parte de ciclo de intercambio
					entrenadorMovido->posicion = entrenadorBloqueado->posicion;
					//lo que sobro del quantum
//					int sobrante = QUANTUM - distanciaHastaBloqueado;
//
//					entrenadorMovido->ciclosCPUConsumido += sobrante;
//
//					entrenadorMovido->ciclosCPUFaltantesIntercambio -= sobrante;
//
//					//por si en el proximo ready se planifica al bloqueado para que se mueva
//					entrenadorBloqueado->ciclosCPUFaltantesIntercambio = sobrante;

					log_info(LO, "El entrenador %c se movio a la posicion (%d, %d)", entrenadorMovido->numeroEntrenador, entrenadorMovido->posicion->posicionX, entrenadorMovido->posicion->posicionY);

					//pasarAReadyPorQuantum(entrenadorMovido);
					log_info(LO, "El entrenador %c paso devuelta a ready porque no le alcanzo el Quantum para terminar de hacer el intercambio", entrenadorMovido->numeroEntrenador);

					//para el intercambio
					int quatumSobrante = QUANTUM - distanciaHastaBloqueado;
					//para el intercambio
					int cpuIntercambio = entrenadorMovido->ciclosCPUFaltantesIntercambio;
					//nunca va a ser menor o igual, si no entraria en el primer if
					if(quatumSobrante < cpuIntercambio){
						entrenadorMovido->ciclosCPUConsumido += QUANTUM;
						int loQueLeFalta = cpuIntercambio - QUANTUM;
						entrenadorMovido->ciclosCPUFaltantesIntercambio = loQueLeFalta;
						pasarAReadyPorQuantum(entrenadorMovido);
					}// si no es este caso, hace el if de arriba de todo

					} else { //no llego a moverse hasta el entrenador para hacer el intercambio
					moverSiDistanciaMayorAQ(entrenadorMovido, entrenadorBloqueado->posicion->posicionX, entrenadorBloqueado->posicion->posicionY, distanciaHastaBloqueado);

					log_info(LO, "El entrenador %c paso devuelta a ready porque no le alcanzo el Quantum para moverse y hacer el intercambio", entrenadorMovido->numeroEntrenador);
				}
}


void atrapar(Entrenador* entrenador, PokemonEnElMapa* pokemon) {
	quickLog("$-Llega al metodo de atrapar");
	int distanciaHastaPokemon = distanciaEntre(&(pokemon->posicion), entrenador->posicion);
	quickLog("$-Calcula la distancia al pokemon");

	//CONDICIONES SEGUN EL ALGORITMO
	if(strcmp(ALGORITMO, "FIFO") == 0 || QUANTUM > distanciaHastaPokemon){
		entrenador->ciclosCPUConsumido += distanciaHastaPokemon;
		entrenador->posicion = &(pokemon->posicion);
		log_info(LO, "El entrenador %c se movio a la posicion (%d, %d)", entrenador->numeroEntrenador, entrenador->posicion->posicionX, entrenador->posicion->posicionY);


		//el socket ya esta conectado con el broker en Conexion
		//sem_wait(&semaforoCatch);
		enviarCatchDesde(entrenador);
		//sem_post(&semaforoCatch);

	} else if (QUANTUM == distanciaHastaPokemon){ // se llega a mover hasta el poke pero no le alcanzo el quantum para mandar el catch
			entrenador->ciclosCPUConsumido += distanciaHastaPokemon;
			entrenador->posicion = &(pokemon->posicion);
			log_info(LO, "El entrenador %c se movio a la posicion (%d, %d)", entrenador->numeroEntrenador, entrenador->posicion->posicionX, entrenador->posicion->posicionY);

			pasarAReadyPorQuantum(entrenador);
			log_info(LO, "El entrenador %c paso devuelta a ready porque no le alcanzo el Quantum para mandar el catch", entrenador->numeroEntrenador);
	} else { // distancia hasta el poke es mayor que el quantum

			// va a entrar a esta condicion y va avanzar hasta lo que le alcance con el quantum
			moverseSegunElQuantum(entrenador, pokemon, distanciaHastaPokemon);
			log_info(LO, "El entrenador %c paso devuelta a ready porque no le alcanzo el Quantum para moverse hasta el pokemon y mandar el catch", entrenador->numeroEntrenador);
		}
}

int terminarSiTodosExit() {


	pthread_mutex_lock(&mutexEntrenadores);
	int todosCumplen = list_all_satisfy(entrenadores, (erasedTypeFilter)estaEnExit);
	log_info(LO, "Termine de consultar el estado");
	pthread_mutex_unlock(&mutexEntrenadores);


	if(todosCumplen) {
		int cantidadCPUTotal = ciclosTotales();
		log_info(LO, "Todos lo entrenadores estan en exit, el Team cumplio su objetivo");
		log_info(LO, "El total de ciclos de CPU  consumidos por el team fue de: %d", cantidadCPUTotal);
		log_info(LO, "La cantidad de cambios de contexto fue: %d", CC);
		log_info(LO, "La cantidad de intercambios fue: %d", cantidadIntercambios);
		log_info(LO, "La cantidad de deadlocks fue: %d", cantidadDeadlocks);
		logearResultadosEntrenadores();
		terminarTeam();
		return 1;
	}

	log_info(LO, "No todos estan en exit");
	return 0;
}

void logearResultadosEntrenadores(){
	pthread_mutex_lock(&mutexEntrenadores);
	for(int index = 0; index < list_size(entrenadores); index++) {
		Entrenador* entrenador = list_get(entrenadores, index);
		log_info(LO, "El entrenador %c consumio %d ciclos de CPU", entrenador->numeroEntrenador, entrenador->ciclosCPUConsumido);
	}
	pthread_mutex_unlock(&mutexEntrenadores);
}

int ciclosTotales() {
	int total = 0;
	pthread_mutex_lock(&mutexEntrenadores);
	for(int index = 0; index < list_size(entrenadores); index++) {
		Entrenador* entrenador = list_get(entrenadores, index);
		total += entrenador->ciclosCPUConsumido;
	}
	pthread_mutex_unlock(&mutexEntrenadores);
	return total;
}
