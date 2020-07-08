
#include "Planificador.h"

static bool noHayEntrenadoresEnExec(t_list* entrenadores);
static void intercambiarPokemonesCon(Entrenador* entrenadorDeIntercambio);
static void atrapar(Entrenador* entrenador, PokemonEnElMapa* pokemon);
static Entrenador* asignarObjetivoA(t_list* entrenadoresAMover, PokemonEnElMapa* pokemonLibre);

typedef bool(*erasedTypeFilter)(void*);


void planificarEntrenadores(){

	typedef void*(*erasedTypeMap)(void*);
	t_log* logger = iniciar_logger();

	int noEstanEnExit(){
		int noEstaEnExit(Entrenador* entrenador){
			return entrenador->estado != 5;
		}
		pthread_mutex_lock(&mutexEntrenadores);
		return list_all_satisfy(entrenadores, (erasedTypeFilter)noEstaEnExit);
		pthread_mutex_unlock(&mutexEntrenadores);
	}

	while(noEstanEnExit){ // lista de entrenadores que no estan en exit
		quickLog("Se pasan todos los entrenadores a Ready");
		// se pasan entrenadores a READY segun su condicion
		pasarAReady();
		pasarAExec();
		log_info(logger, "Ya se cambiaron a ready todos los entrenadores segun condicion");

	}



	//while(siNoEstanTodosEnExit)

	//pasarAReady(entrenadoresPosibles);
	Entrenador* entrenadorAEjecutar = entrenadorExec();
}

//paso a ready los entrenadores que esten mas cerca
void pasarAReady(){
	typedef bool(*erasedTypeFilter)(void*);
	t_log* logger = iniciar_logger();

	int tieneEstadoNewODormido(Entrenador* entrenador) {
		return entrenador->estado==1 || (entrenador->estado==4 && entrenador->motivo==2);
	}
	pthread_mutex_lock(&mutexEntrenadores);
	log_info(logger, "Verifico si hay entrenadores posibles para pasar a ready");
	t_list* entrenadoresPosibles = list_filter(entrenadores, (erasedTypeFilter)tieneEstadoNewODormido);
	pthread_mutex_unlock(&mutexEntrenadores);

	pthread_mutex_lock(&mutexPokemonesLibres);
	//asignamos objetivo al entrenador mas cercano
	for(int index=0; index < list_size(pokemonesLibres); index++){


		PokemonEnElMapa* pokemonLibre = list_get(pokemonesLibres, index);
		Entrenador* entrenadorAReady =  asignarObjetivoA(entrenadoresPosibles, pokemonLibre);
		// cambio de estado al entrenador, pasa a ready
		entrenadorAReady->estado = 2;
		// ese poke se saca de la lista de pokes libres porque ya fue asginado
		// TODO: aca se supone que solo cambia la cantidad en la lista, en el entrenador tiene que quedar como cant 1
		disminuirCantidadPokemones(pokemonLibre, pokemonesLibres);

		pthread_mutex_lock(&mutexObjetivosGlobales);
		// disminuyo la cantidad de ese poke libre en los obj globales (lo saco si cant = 0)
		disminuirCantidadPokemones(pokemonLibre, objetivosGlobales);
		pthread_mutex_unlock(&mutexObjetivosGlobales);

	}
	pthread_mutex_unlock(&mutexPokemonesLibres);

	log_info(logger, "Se paso a ready el entrenador");
	destruirLog(logger);
}

Entrenador* asignarObjetivoA(t_list* entrenadoresAMover, PokemonEnElMapa* pokemonLibre){
	Entrenador* entrenadorAAsignar = entrenadorMasCercanoA(pokemonLibre, entrenadoresAMover);
	MovimientoEnExec* movimiento = malloc(sizeof(MovimientoEnExec));
	movimiento->pokemonNecesitado = pokemonLibre;
	//TODO: me tira error aca y no se porque, sera por el malloc?
	entrenadorAAsignar->movimientoEnExec = movimiento;
	entrenadorAAsignar->movimientoEnExec->objetivo = 1;

	return entrenadorAAsignar;
}

void disminuirCantidadPokemones(PokemonEnElMapa* pokemonLibre, t_list* listaPokes){
	typedef bool(*erasedTypeRemove)(void*);
	PokemonEnElMapa* pokeComoObj = buscarPorNombre(pokemonLibre->nombre, listaPokes);

	bool sacarSiCantidadEsCero(PokemonEnElMapa* pokeComoObj){
		return pokeComoObj->cantidad == 0;
	}

	// disminuir la cantidad de ese poke libre en los objetivos globales
	pokeComoObj->cantidad -= 1;

	// (sacarlo si cant = 0)
	list_remove_by_condition(listaPokes, (erasedTypeRemove)sacarSiCantidadEsCero);

}



/////////


//solo se puede pasar un entrenador a estado EXEC si no hay ninguno en estado EXEC
void pasarAExec(){
	pthread_mutex_lock(&mutexEntrenadores);
	//FIFO
	Entrenador* entrenador = list_get(entrenadores, 0);
	if(noHayEntrenadoresEnExec(entrenadores) == 1){
			entrenador->estado = 3;
			pthread_mutex_unlock(&entrenador->mutexEntrenador);
	}

	pthread_mutex_unlock(&mutexEntrenadores);
}


//chequeo que no hay entrenadores en exec
bool noHayEntrenadoresEnExec(t_list* entrenadores){
	typedef bool(*erasedType)(void*);
	t_list* entrenadoresEnReady = entrenadoresReady();

	bool noEstaEnExec(Entrenador* entrenador){
		return entrenador->estado != 3;
	}
	//determino si todos los entrenadores cumplen que no haya ninguno en estado exec
	return list_all_satisfy(entrenadoresEnReady, (erasedType)noEstaEnExec);
}
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

	pasarABlockEsperando(entrenador);

	//el socket ya esta conectado con el broker en Conexion
	sem_wait(&semaforoCatch);
	enviarCatchDesde(entrenador);
	sem_post(&semaforoCatch);
}
