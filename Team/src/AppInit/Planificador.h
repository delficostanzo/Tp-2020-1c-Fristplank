
#include "utils.h"
#include "ConfigFunctions.h"
#include "../Model/Mapa.h"


#ifndef SRC_APPINIT_PLANIFICADOR_H_
#define SRC_APPINIT_PLANIFICADOR_H_

typedef struct{
	t_list* entrenadores;
}ListaEstados;

void pasarDeNewAReady(t_list* entrenadores, t_list* pokemonesLibres);
void pasarDeReadyAExec(t_list* entrenadores);
void pasarDeBlockAReady(t_list* entrenadores, t_list* pokemonesLibres);
#endif /* SRC_APPINIT_PLANIFICADOR_H_ */
