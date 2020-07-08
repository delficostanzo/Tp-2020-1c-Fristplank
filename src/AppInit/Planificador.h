
#include "utils.h"
#include "ConfigFunctions.h"
#include "../Model/Mapa.h"
#include "EnvioMensajes.h"
#include "../Model/EntrenadoresPorEstado.h"

#ifndef SRC_APPINIT_PLANIFICADOR_H_
#define SRC_APPINIT_PLANIFICADOR_H_



void verificarSiTodosExit();
void cumplirObjetivo(Entrenador* entrenador);
//void moverAReady();
void pasarAReady();
void pasarAExec();
void planificarEntrenadores();
void disminuirCantidadPokemones(PokemonEnElMapa* pokemonLibre, t_list* listaPokes);
#endif /* SRC_APPINIT_PLANIFICADOR_H_ */
