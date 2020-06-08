
#include "Planificador.h"

static bool noHayEntrenadoresEnExec(t_list* entrenadores);

//pasa el entrenador que esta mas cerca de los pokes libres a ready
//TODO:aca no estoy segura si esta bien pasarle por parametro la lista de los pokes libres que le manda el Broker?
void pasarDeNewAReady(t_list* entrenadores, t_list* pokemonesLibres){
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

