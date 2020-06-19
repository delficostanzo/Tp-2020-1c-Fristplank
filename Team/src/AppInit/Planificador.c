
#include "Planificador.h"

static bool noHayEntrenadoresEnExec(t_list* entrenadores);
static void intercambiarPokemonesCon(Entrenador* entrenadorDeIntercambio);

typedef bool(*erasedTypeFilter)(void*);


//pasa el entrenador que esta mas cerca de los pokes libres a ready
void pasarDeNewAReady(t_list* entrenadores, t_list* pokemonesLibres){
	int tieneEstadoNewODormido(Entrenador* entrenador) {
		return entrenador->estado==1 || (entrenador->estado==4 && entrenador->motivo==2);
	}
	t_list* entrenadoresPosibles = list_filter(entrenadores, (erasedTypeFilter)tieneEstadoNewODormido);
	for(int index=0; index < list_size(pokemonesLibres); index++){
		PokemonEnElMapa* pokemonLibre = list_get(pokemonesLibres, index);
		Entrenador* entrenador = entrenadorMasCercanoA(pokemonLibre, entrenadoresPosibles);
		entrenador->estado = 2;
		entrenador->movimientoEnExec->pokemonNecesitado = pokemonLibre;
		list_remove(pokemonesLibres, pokemonLibre);
	}
}

//solo se puede pasar un entrenador a estado EXEC si no hay ninguno en estado EXEC
//TODO: falta la condicion de que se quieren mover y atrapar (1) o mover e intercambiar (2)
// el Team deberia settear los objetivos de los entrenadores segun sus pokemones, si de la lista con los pokes libres
// algun entrenador debe atrapar, su objetivo es (1)? En el caso de el intercambio todavia no se
void pasarDeReadyAExec(t_list* entrenadores){
	for(int index=0; index < list_size(entrenadores); index++){
		Entrenador* entrenador = list_get(entrenadores, index);
		//si no hay ninguno entrenador en estado exec
		if(noHayEntrenadoresEnExec(entrenadores) == 1){
			entrenador->estado = 3;
		} else{
			// su estado no cambia
		}
	}
}

//chequeo que no hay entrenadores en exec
bool noHayEntrenadoresEnExec(t_list* entrenadores){
	typedef bool(*erasedType)(void*);

	bool noEstaEnEstadoExec(Entrenador* entrenador){
		return entrenador->estado != 3;
	}

	//determino si todos los entrenadores cumplen que no haya ninguno en estado exec
	return list_all_satisfy(entrenadores, (erasedType)noEstaEnEstadoExec);
}


void pasarDeBlockAReady(t_list* entrenadores, t_list* pokemonesLibres){
	for(int index=0; index < list_size(entrenadores); index++){
		Entrenador* entrenador = list_get(entrenadores, index);
		for(int index=0; index < list_size(pokemonesLibres); index++){
			PokemonEnElMapa* pokemonLibre = list_get(pokemonesLibres, index);
				//si es el entrenador mas cerca a un pokemon
				if(entrenadorMasCercanoA(pokemonLibre, entrenadores) == entrenador){
					entrenador->estado = 2;
				} else{
					//su estado no cambia
				}
		}
	}
}

void pasarDeExecABlockEsperando(Entrenador* entrenador) {
	entrenador->estado = 4;
	entrenador->motivo = 1;
}



void cumplirObjetivo(Entrenador* entrenador){
	quickLog("Se esta cumpliendo un objetivo");
	MovimientoEnExec* movimientoEnExec = entrenador->movimientoEnExec;
	ObjetivoEnExec mision = movimientoEnExec->objetivo;
	Entrenador* entrenadorDeIntercambio;

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

void moverAReady(Entrenador* entrenador) {
	pthread_mutex_lock(&mutexEntrenadores);
	pthread_mutex_lock(&mutexPokemonesLibres);
	pasarDeNewAReady(entrenadores, pokemonesLibres);
	pthread_mutex_unlock(&mutexEntrenadores);
	pthread_mutex_unlock(&mutexPokemonesLibres);
}

