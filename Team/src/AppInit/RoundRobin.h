
#include "utils.h"
#include "../Model/EntrenadoresPorEstado.h"
#include "../Model/Trainer.h"

#ifndef SRC_APPINIT_ROUNDROBIN_H_
#define SRC_APPINIT_ROUNDROBIN_H_

void moverseSegunElQuantum(Entrenador* entrenador, PokemonEnElMapa* pokemon, int distanciaHastaPokemon);
void moverSiDistanciaMenorAQ(Entrenador* entrenador, int posicionXPoke, int posicionYPoke, int distanciaHastaPokemon);


#endif /* SRC_APPINIT_ROUNDROBIN_H_ */
