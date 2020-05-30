
#include "utils.c"


#ifndef SRC_APPINIT_PLANIFICADOR_H_
#define SRC_APPINIT_PLANIFICADOR_H_

typedef struct{
	t_list* hilosEntrenadores;
}EstadoNew;

typedef struct{
	t_list* hilosEntrenadores;
}EstadoReady;

typedef struct{
	t_list* hilosEntrenadores;
}EstadoBlock;

typedef struct{
	t_list* hilosEntrenadores;
}EstadoExec;

typedef struct{
	t_list* hilosEntrenadores;
}EstadoExit;
#endif /* SRC_APPINIT_PLANIFICADOR_H_ */
