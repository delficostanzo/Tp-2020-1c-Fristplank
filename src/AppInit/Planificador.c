
#include "Planificador.h"

void newAReady(t_list* estadoNew, t_list* estadoReady){
	for(int index = 0; index < list_size(estadoNew); index++){
	pthread_t* hiloEntrenador = list_remove(estadoNew, index);
	list_add(estadoReady, hiloEntrenador);
	}
}
//memmove(estadoNew, estadoNew + 1, list_size(estadoNew) - list_size(*estadoNew));

void readyAExec(t_list* estadoReady, t_list* estadoExec){
	pthread_t* hiloEntrenador = list_remove(estadoReady, 0);
	// muevo los elementos del estado READY para que no queden espacios en blanco
	//memmove(estadoNew, estadoNew + 1, list_size(estadoNew) - list_size(*estadoNew));
	list_add(estadoExec, hiloEntrenador);
}

void execAReady(t_list* estadoExec, t_list* estadoReady){
	pthread_t* hiloEntrenador = list_remove(estadoExec, 0);
	// muevo los elementos del estado READY para que no queden espacios en blanco
	//memmove(estadoNew, estadoNew + 1, list_size(estadoNew) - list_size(*estadoNew));
	list_add(estadoReady, hiloEntrenador);
}

void execABlock(t_list* estadoExec, t_list* estadoBlock){
	pthread_t* hiloEntrenador = list_remove(estadoExec, 0);
	// muevo los elementos del estado READY para que no queden espacios en blanco
	//memmove(estadoNew, estadoNew + 1, list_size(estadoNew) - list_size(*estadoNew));
	list_add(estadoBlock, hiloEntrenador);
}

void blockAReady(t_list* estadoBlock, t_list* estadoReady){
	pthread_t* hiloEntrenador = list_remove(estadoBlock, 0);
	// muevo los elementos del estado READY para que no queden espacios en blanco
	//memmove(estadoNew, estadoNew + 1, list_size(estadoNew) - list_size(*estadoNew));
	list_add(estadoReady, hiloEntrenador);
}

void blockAExit(t_list* estadoBlock, t_list* estadoExit){
	pthread_t* hiloEntrenador = list_remove(estadoBlock, 0);
	// muevo los elementos del estado READY para que no queden espacios en blanco
	//memmove(estadoNew, estadoNew + 1, list_size(estadoNew) - list_size(*estadoNew));
	list_add(estadoExit, hiloEntrenador);
}

void execAExit(t_list* estadoExec, t_list* estadoExit){
	pthread_t* hiloEntrenador = list_remove(estadoExec, 0);
	// muevo los elementos del estado READY para que no queden espacios en blanco
	//memmove(estadoNew, estadoNew + 1, list_size(estadoNew) - list_size(*estadoNew));
	list_add(estadoExit, hiloEntrenador);
}
