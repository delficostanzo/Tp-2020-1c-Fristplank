#include "RoundRobin.h"


void moverseSegunElQuantum(Entrenador* entrenador, PokemonEnElMapa* pokemon, int distanciaHastaPokemon){
	int posicionXPoke = pokemon->posicion.posicionX;
	int posicionYPoke = pokemon->posicion.posicionY;

	// si la distancia hasta el poke es > que el quantum
	moverSiDistanciaMayorAQ(entrenador,posicionXPoke,posicionYPoke,distanciaHastaPokemon);
}

void moverSiDistanciaMayorAQ(Entrenador* entrenador, int posicionXPoke, int posicionYPoke, int distanciaHastaPokemon){

	int posicionXEntrenador = entrenador->posicion->posicionX;
	int posicionYEntrenador = entrenador->posicion->posicionY;

	// calculos de distancias -> cuanto tiene que moverse el entrenador
	int distanciaEnXAbs = abs(posicionXPoke - posicionXEntrenador);

	// si la distancia total es mayor al quantum
	if(distanciaHastaPokemon > QUANTUM){
		//primero lo muevo en x

		if(distanciaEnXAbs == QUANTUM){ //si con el quantum solo llega a moverse hasta la x del poke
			//la x del poke y del entrenador son iguales
			entrenador->posicion->posicionX = posicionXPoke;
			entrenador->ciclosCPUConsumido += QUANTUM;
			log_info(LO, "El entrenador %d se movio a la posicion (%d, %d)", entrenador->numeroEntrenador, entrenador->posicion->posicionX, entrenador->posicion->posicionY);

			pasarAReadyPorQuantum(entrenador);

		} else if(distanciaEnXAbs > QUANTUM) { //no le alcanza el quantum para moverse todo el x
			if(posicionXEntrenador > posicionXPoke) { //si el entrenador esta a la derecha
				entrenador->posicion->posicionX -= QUANTUM; //va hacia la izquierda => disminuye
				entrenador->ciclosCPUConsumido += QUANTUM;
			} else { //si el entrenador esta a la izquierda
				entrenador->posicion->posicionX += QUANTUM; //va hacia la izquierda => disminuye
				entrenador->ciclosCPUConsumido += QUANTUM;
			}
			log_info(LO, "El entrenador %d se movio a la posicion (%d, %d)", entrenador->numeroEntrenador, entrenador->posicion->posicionX, entrenador->posicion->posicionY);

			pasarAReadyPorQuantum(entrenador);

		} else{//distanciaXAbs < Q => se mueve lo que llegue en x y lo que le sobre en y
			   if((posicionXPoke - posicionXEntrenador) < 0) { //si el entrenador esta a la derecha
					entrenador->posicion->posicionX -= distanciaEnXAbs; //va hacia la izquierda => disminuye
					entrenador->ciclosCPUConsumido += distanciaEnXAbs;
				} else { //si el entrenador esta a la izquierda => >0 (nunca = 0 porque ya la distanciaX > Q)
					entrenador->posicion->posicionX += distanciaEnXAbs; //va hacia la izquierda => disminuye
					entrenador->ciclosCPUConsumido += distanciaEnXAbs;
				}

			int quantumQueMeSobra = QUANTUM - distanciaEnXAbs; //siempre va a sobrar porque distanciaXAbs < Q
			if(posicionYEntrenador > posicionYPoke){//si el entrenador esta arriba
				entrenador->posicion->posicionY -= quantumQueMeSobra;
				entrenador->ciclosCPUConsumido += quantumQueMeSobra;
			}else{// caso contrario se suma
				entrenador->posicion->posicionY += quantumQueMeSobra;
				entrenador->ciclosCPUConsumido += quantumQueMeSobra;
			}
			log_info(LO, "El entrenador %d se movio a la posicion (%d, %d)", entrenador->numeroEntrenador, entrenador->posicion->posicionX, entrenador->posicion->posicionY);

			pasarAReadyPorQuantum(entrenador);
		}
	}
}

void pasarAReadyPorQuantum(Entrenador* entrenador){
	agregarAListaReady(entrenador);
	// se pasa a ready con el mismo objetivo de atrapar el mismo poke
	pthread_mutex_lock(&entrenador->mutexEstado);
	entrenador->estado = 2;
	pthread_mutex_unlock(&entrenador->mutexEstado);

}



