
#include "utils.h"
#include "ConfigFunctions.h"
#include "../Model/Mapa.h"


#ifndef SRC_APPINIT_PLANIFICADOR_H_
#define SRC_APPINIT_PLANIFICADOR_H_

typedef struct{
	t_list* entrenadores;
}ListaEstados;

void pasarDeNewAReady(ListaEstados* entrenadores, Entrenador* entrenador);
void pasarDeReadyAExec(ListaEstados* entrenadores, Entrenador* entrenador);
#endif /* SRC_APPINIT_PLANIFICADOR_H_ */
