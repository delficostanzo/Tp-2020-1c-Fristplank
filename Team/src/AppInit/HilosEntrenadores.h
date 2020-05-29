

#ifndef SRC_APPINIT_HILOSENTRENADORES_H_
#define SRC_APPINIT_HILOSENTRENADORES_H_

#include "utils.h"

typedef void* Data;

void crearHilosDeEntrenadores(t_list* entrenadores, pthread_t* hilos[]);

#endif /* SRC_APPINIT_HILOSENTRENADORES_H_ */
