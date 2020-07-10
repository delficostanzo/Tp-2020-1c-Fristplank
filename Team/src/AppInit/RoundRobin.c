#include "RoundRobin.h"

static void pasarAReadyPorQuantum(Entrenador* entrenador);

void moverseSegunElQuantum(Entrenador* entrenador, PokemonEnElMapa* pokemon, int distanciaHastaPokemon){
	int posicionXPoke = pokemon->posicion.posicionX;
	int posicionYPoke = pokemon->posicion.posicionY;

	// si la distancia hasta el poke es < que el quantum
	moverSiDistanciaMenorAQ(entrenador,posicionXPoke,posicionYPoke,distanciaHastaPokemon);
}

void moverSiDistanciaMenorAQ(Entrenador* entrenador, int posicionXPoke, int posicionYPoke, int distanciaHastaPokemon){

	int posicionXEntrenador = entrenador->posicion->posicionX;
	int posicionYEntrenador = entrenador->posicion->posicionY;

	// calculos de distancias -> cuanto tiene que moverse el entrenador
	int distanciaEnX = posicionXPoke - posicionXEntrenador;
	// si la distancia total es menor al quantum
	if(distanciaHastaPokemon > QUANTUM){
		if(distanciaEnX > QUANTUM || distanciaEnX == QUANTUM){
			entrenador->posicion->posicionX += QUANTUM;
			entrenador->ciclosCPUConsumido += QUANTUM;
		}else{//distancia a X es menor que Quantum
			entrenador->posicion->posicionX += distanciaEnX;
			entrenador->ciclosCPUConsumido += distanciaEnX;
			int quantumQueMeSobra = QUANTUM - abs(distanciaEnX);

			if(quantumQueMeSobra != 0){//si sobro quantum se lo asigno a Y
				if(posicionYEntrenador > posicionYPoke){// si la posY entrenador es > a la del poke, se resta
					entrenador->posicion->posicionY -= quantumQueMeSobra;
					entrenador->ciclosCPUConsumido += quantumQueMeSobra;
				}else{// caso contrario se suma
					entrenador->posicion->posicionY += quantumQueMeSobra;
					entrenador->ciclosCPUConsumido += quantumQueMeSobra;
				}
			} else{
					pasarAReadyPorQuantum(entrenador);
			}
		}
	}
}

void pasarAReadyPorQuantum(Entrenador* entrenador){
	entrenador->estado = 2;
}

