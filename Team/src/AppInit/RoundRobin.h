
#include "utils.h"
#include "../Model/EntrenadoresPorEstado.h"
#include "../Model/Trainer.h"

#ifndef SRC_APPINIT_ROUNDROBIN_H_
#define SRC_APPINIT_ROUNDROBIN_H_

void moverseSegunElQuantum(Entrenador* entrenador, PokemonEnElMapa* pokemon, int distanciaHastaPokemon);
void moverSiDistanciaMayorAQ(Entrenador* entrenador, int posicionXPoke, int posicionYPoke, int distanciaHastaPokemon);
void pasarAReadyPorQuantum(Entrenador* entrenador);
#endif /* SRC_APPINIT_ROUNDROBIN_H_ */
