
#include "utils.c"


#ifndef SRC_APPINIT_PLANIFICADOR_H_
#define SRC_APPINIT_PLANIFICADOR_H_

typedef struct{
	t_list* hilosEntrenadoresNew;
}EstadoNew;

typedef struct{
	t_list* hilosEntrenadoresReady;
}EstadoReady;

typedef struct{
	t_list* hilosEntrenadoresBlock;
}EstadoBlock;

typedef struct{
	t_list* hilosEntrenadoresExec;
}EstadoExec;

typedef struct{
	t_list* hilosEntrenadoresExit;
}EstadoExit;
#endif /* SRC_APPINIT_PLANIFICADOR_H_ */
