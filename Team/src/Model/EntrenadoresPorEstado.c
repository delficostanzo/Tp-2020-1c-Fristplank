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
		pthread_mutex_lock(&entrenador->mutexEstado);
		int cumple = entrenador->motivo == 3;
		pthread_mutex_unlock(&entrenador->mutexEstado);
		return cumple;
	}
	t_list* entrenadoresBlock = entrenadoresBloqueados();
	//si la lista entrenadoresBlock esta vacia devuelve otra lista vacia
	return list_filter(entrenadoresBlock, (erasedTypeFilter)suMotivoDeBloqueoEsDeadlock);
}

t_list* entrenadoresDormidos(void) {
	int suMotivoDeBloqueoEsDormido(Entrenador* entrenador) {
		pthread_mutex_lock(&entrenador->mutexEstado);
		int cumple = entrenador->motivo == 2;
		pthread_mutex_unlock(&entrenador->mutexEstado);
		return cumple;
	}

	t_list* entrenadoresBlock = entrenadoresBloqueados();
	//si la lista entrenadoresBlock esta vacia devuelve otra lista vacia
	return list_filter(entrenadoresBlock, (erasedTypeFilter)suMotivoDeBloqueoEsDormido);
}

t_list* entrenadoresEsperandoRespuesta(void) {
	int suMotivoDeBloqueoEsEspera(Entrenador* entrenador) {
		pthread_mutex_lock(&entrenador->mutexEstado);
		int cumple = entrenador->motivo == 1;
		pthread_mutex_unlock(&entrenador->mutexEstado);
		return cumple;
	}

	t_list* entrenadoresBlock = entrenadoresBloqueados();
	//si la lista entrenadoresBlock esta vacia devuelve otra lista vacia
	return list_filter(entrenadoresBlock, (erasedTypeFilter)suMotivoDeBloqueoEsEspera);
}

//si no encuentra devuelve una lista vacia
t_list* entrenadoresBloqueados() {
	int estaBloqueado(Entrenador* entrenador) {
		pthread_mutex_lock(&entrenador->mutexEstado);
		int cumple = entrenador->estado == 4;
		pthread_mutex_unlock(&entrenador->mutexEstado);
		return cumple;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	//si no encuentra ninguno devuelve una lista vacia
	t_list* entrenadoresBlock = list_filter(entrenadores, (erasedTypeFilter)estaBloqueado);
	pthread_mutex_unlock(&mutexEntrenadores);

	return entrenadoresBlock;
}

t_list* entrenadoresNew(void) {
	int esNew(Entrenador* entrenador) {
		pthread_mutex_lock(&entrenador->mutexEstado);
		int cumple = entrenador->estado == 1;
		pthread_mutex_lock(&entrenador->mutexEstado);
		return cumple;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	//si no encuentra ninguno devuelve una lista vacia
	t_list* entrenadoresNuevos = list_filter(entrenadores, (erasedTypeFilter)esNew);
	pthread_mutex_unlock(&mutexEntrenadores);

	return entrenadoresNuevos;
}

t_list* entrenadoresReady(void) {
	int estaReady(Entrenador* entrenador) {
		pthread_mutex_lock(&entrenador->mutexEstado);
		int cumple = entrenador->estado == 2;
		pthread_mutex_lock(&entrenador->mutexEstado);
		return cumple;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	//si no encuentra ninguno devuelve una lista vacia
	t_list* entrenadoresReady = list_filter(entrenadores, (erasedTypeFilter)estaReady);
	pthread_mutex_unlock(&mutexEntrenadores);

	return entrenadoresReady;
}

Entrenador* entrenadorExec(void) {
	int estaExec(Entrenador* entrenador) {
		pthread_mutex_lock(&entrenador->mutexEstado);
		int cumple = entrenador->estado == 3;
		pthread_mutex_unlock(&entrenador->mutexEstado);
		return cumple;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	//si no encuentra ninguno devuelve una lista vacia
	Entrenador* entrenadorExec = list_find(entrenadores, (erasedTypeFilter)estaExec);
	pthread_mutex_unlock(&mutexEntrenadores);

	return entrenadorExec;
}

int hayEntrenadoresEnNewODormido() {
	pthread_mutex_lock(&mutexEntrenadores);
	log_info(LO, "Aca si llega");
	int cumple = list_any_satisfy(entrenadores, (erasedTypeFilter) tieneEstadoNewODormido);
	pthread_mutex_unlock(&mutexEntrenadores);
	return cumple;
}

int tieneEstadoNew(Entrenador* entrenador) {
	pthread_mutex_lock(&entrenador->mutexEstado);
	int cumple = entrenador->estado == 1;
	pthread_mutex_unlock(&entrenador->mutexEstado);
	return cumple;
}

int hayEntrenadoresEnNew(){
	pthread_mutex_lock(&mutexEntrenadores);
	log_info(LO, "Aca si llega");
	int cumple = list_any_satisfy(entrenadores, (erasedTypeFilter) tieneEstadoNew);
	pthread_mutex_unlock(&mutexEntrenadores);
	return cumple;
}
