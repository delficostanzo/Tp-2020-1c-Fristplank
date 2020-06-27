
#include "utils.h"
#include "ConfigFunctions.h"
#include "../Model/Mapa.h"
#include "EnvioMensajes.h"

#ifndef SRC_APPINIT_PLANIFICADOR_H_
#define SRC_APPINIT_PLANIFICADOR_H_


void pasarDeNewAReady(t_list* entrenadores, t_list* pokemonesLibres);
void pasarDeReadyAExec(t_list* entrenadores);
void pasarDeBlockAReady(t_list* entrenadores, t_list* pokemonesLibres);
void verificarSiTodosExit();
void cumplirObjetivo(Entrenador* entrenador);
void moverAReady();
#endif /* SRC_APPINIT_PLANIFICADOR_H_ */
