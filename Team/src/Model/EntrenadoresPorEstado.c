/*
 * EntrenadoresPorEstado.c
 *
 *  Created on:
 *      Author: fritsplank
 */

#include "EntrenadoresPorEstado.h"


t_list* entrenadoresBloqueados();
typedef bool(*erasedTypeFilter)(void*);

//si no encuentra devuelve una lista vacia
t_list* entrenadoresBloqueadosPorDeadlock(void) {
	int suMotivoDeBloqueoEsDeadlock(Entrenador* entrenador) {
		return entrenador->motivo == 3;
	}
	t_list* entrenadoresBlock = entrenadoresBloqueados();
	//si la lista entrenadoresBlock esta vacia devuelve otra lista vacia
	return list_filter(entrenadoresBlock, (erasedTypeFilter)suMotivoDeBloqueoEsDeadlock);
}

t_list* entrenadoresDormidos(void) {
	int suMotivoDeBloqueoEsDormido(Entrenador* entrenador) {
		return entrenador->motivo == 2;
	}

	t_list* entrenadoresBlock = entrenadoresBloqueados();
	//si la lista entrenadoresBlock esta vacia devuelve otra lista vacia
	return list_filter(entrenadoresBlock, (erasedTypeFilter)suMotivoDeBloqueoEsDormido);
}

t_list* entrenadoresEsperandoRespuesta(void) {
	int suMotivoDeBloqueoEsEspera(Entrenador* entrenador) {
		return entrenador->motivo == 1;
	}

	t_list* entrenadoresBlock = entrenadoresBloqueados();
	//si la lista entrenadoresBlock esta vacia devuelve otra lista vacia
	return list_filter(entrenadoresBlock, (erasedTypeFilter)suMotivoDeBloqueoEsEspera);
}

//si no encuentra devuelve una lista vacia
t_list* entrenadoresBloqueados() {
	int estaBloqueado(Entrenador* entrenador) {
		return entrenador->estado == 4;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	//si no encuentra ninguno devuelve una lista vacia
	t_list* entrenadoresBlock = list_filter(entrenadores, (erasedTypeFilter)estaBloqueado);
	pthread_mutex_unlock(&mutexEntrenadores);

	return entrenadoresBlock;
}

t_list* entrenadoresNew(void) {
	int esNew(Entrenador* entrenador) {
		return entrenador->estado == 1;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	//si no encuentra ninguno devuelve una lista vacia
	t_list* entrenadoresNuevos = list_filter(entrenadores, (erasedTypeFilter)esNew);
	pthread_mutex_unlock(&mutexEntrenadores);

	return entrenadoresNuevos;
}

t_list* entrenadoresReady(void) {
	int estaReady(Entrenador* entrenador) {
		return entrenador->estado == 2;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	//si no encuentra ninguno devuelve una lista vacia
	t_list* entrenadoresReady = list_filter(entrenadores, (erasedTypeFilter)estaReady);
	pthread_mutex_unlock(&mutexEntrenadores);

	return entrenadoresReady;
}

Entrenador* entrenadorExec(void) {
	int estaReady(Entrenador* entrenador) {
		return entrenador->estado == 3;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	//si no encuentra ninguno devuelve una lista vacia
	Entrenador* entrenadorExec = list_find(entrenadores, (erasedTypeFilter)estaReady);
	pthread_mutex_unlock(&mutexEntrenadores);

	return entrenadorExec;
}


