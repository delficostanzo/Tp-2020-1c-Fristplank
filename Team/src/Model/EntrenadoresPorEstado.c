/*
 * EntrenadoresPorEstado.c
 *
 *  Created on:
 *      Author: fritsplank
 */

//si no encuentra devuelve una lista vacia
t_list* entrenadoresBloqueadosPorDeadlock(void) {
	int suMotivoDeBloqueoEsDeadlock(Entrenador* entrenador) {
		return entrenador->motivo == 3;
	}

	t_list* entrenadoresBlock = entrenadoresBloqueados();
	//si la lista entrenadoresBlock esta vacia devuelve otra lista vacia
	return list_filter(entrenadoresBlock, suMotivoDeBloqueoEsDeadlock);
}

t_list* entrenadoresDormidos(void) {

}

//si no encuentra devuelve una lista vacia
t_list* entrenadoresBloqueados(void) {
	int estaBloqueado(Entrenador* entrenador) {
		return entrenador->estado == 4;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	//si no encuentra ninguno devuelve una lista vacia
	t_list* entrenadoresBlock = list_filter(entrenadores, estaBloqueado);
	pthread_mutex_unlock(&mutexEntrenadores);

	return entrenadoresBlock;
}
