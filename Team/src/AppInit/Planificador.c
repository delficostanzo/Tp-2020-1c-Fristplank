
#include "Planificador.h"

static bool noHayEntrenadoresEnExec(ListaEstados* entrenadores);

//pasa el entrenador que esta mas cerca de los pokes libres a ready
void pasarDeNewAReady(ListaEstados* estados, Entrenador* entrenador){
	t_list* pokemonesLibres;
	for(int index=0; index < list_size(pokemonesLibres); index++){
		PokemonEnElMapa* pokemonLibre = list_get(pokemonesLibres, index);

		//si es el entrenador mas cerca a un pokemon
		if(entrenadorMasCercanoA(pokemonLibre, estados->entrenadores) == entrenador){
			entrenador->estado = 2;
		} else{
			//su estado no cambia
		}
	}

}
//segun la lista de entrenadores, paso a un entrenador de READY a EXEC segun la condicion
//TODO: falta la condicion de que se quieren mover y atrapar o mover e intercambiar
void pasarDeReadyAExec(ListaEstados* estados, Entrenador* entrenador){
	//si no hay ninguno entrenador en estado exec
	if(noHayEntrenadoresEnExec(estados) == 1){
		entrenador->estado = 3;
	} else{
		// su estado no cambia
	}
}

//chequeo que no hay entrenadores en exec
bool noHayEntrenadoresEnExec(ListaEstados* estados){
	typedef bool(*erasedType)(void*);

	bool noEstaEnEstadoExec(Entrenador* entrenador){
		return entrenador->estado != 3;
	}

	//determino si todos los entrenadores cumplen que no haya ninguno en estado exec
	return list_all_satisfy(estados->entrenadores, (erasedType)noEstaEnEstadoExec);
}

