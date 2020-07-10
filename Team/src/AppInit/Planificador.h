
#include "utils.h"
#include "ConfigFunctions.h"
#include "EnvioMensajes.h"
#include "../Model/EntrenadoresPorEstado.h"
#include "../Model/Trainer.h"
#include "RoundRobin.h"
#include "../Model/CierreTeam.h"

#ifndef SRC_APPINIT_PLANIFICADOR_H_
#define SRC_APPINIT_PLANIFICADOR_H_


void cumplirObjetivo(Entrenador* entrenador);
void pasarAReadyParaAtrapar();
void pasarAReadyParaIntercambiar();
void pasarAExec();
void planificarEntrenadores();
void disminuirCantidadPokemones(PokemonEnElMapa* pokemonLibre, t_list* listaPokes);

#endif /* SRC_APPINIT_PLANIFICADOR_H_ */
