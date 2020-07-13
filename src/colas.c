/*
 * colas.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */
#include "colas.h"


void agregarMensajeACola(t_paquete * nuevoPaquete) {
	log_debug(logger, "START: agregarMensajeACola");

	t_metadata * nuevaMeta = malloc(sizeof(t_metadata));
	nuevaMeta->tipoMensaje = nuevoPaquete->codigo_operacion;

	pthread_mutex_lock(&mutexIdMensaje);
	log_debug(logger, "Entro al mutex de ID para generarlo");
	nuevaMeta->ID = crearID();
	nuevoPaquete->ID = nuevaMeta->ID;
	pthread_mutex_unlock(&mutexIdMensaje);

	nuevaMeta->IDCorrelativo = nuevoPaquete->ID_CORRELATIVO;
	nuevaMeta->tamanioMensaje = nuevoPaquete->buffer->size;
	nuevaMeta->flagLRU = 0;
	nuevaMeta->posicion = SINPARTICION;

	log_debug(logger, "Procedo a buscar la cola %d", nuevoPaquete->codigo_operacion);

	for (int i = 0; i < 6; i++) {
		if (cola[i].nombreCola == nuevoPaquete->codigo_operacion) {
			pthread_mutex_lock(&mutexColas);
			pthread_mutex_lock(&mutexMemoria);
			//EL TAMAÑO Y POSICION DE UN MENSAJE EN MEMORIA  SE CALCULAN EN LA SIGUIENTE FUNCION
			cachearMensaje(nuevaMeta, nuevoPaquete->buffer->stream);
			list_add(cola[i].mensajes, nuevaMeta);

			pthread_mutex_unlock(&mutexMemoria);
			pthread_mutex_unlock(&mutexColas);

			log_info(logger, "Se agregó un nuevo mensaje a la cola %s.",ID_COLA[i+1]); //LOG OBLIGATORIO (3)
		}
	}
}

void agregarSuscriptorACola(int idSuscriptor, op_code tipoCola) {

	for (int i = 0; i < 6; i++) {
		if (cola[i].nombreCola == tipoCola) {
			if (!existeSuscriptor(cola[i].suscriptores, idSuscriptor) != 0) {
				list_add(cola[i].suscriptores, (int*) idSuscriptor);
				log_info(logger, "Se suscribió con éxito el proceso a la cola %s", ID_COLA[tipoCola]);
			}
		}
	}
}

t_list* mensajesAEnviar(int idProceso, op_code codigoCola){

	t_list* mensajes = list_create();

	for (int i = 0; i < 6; i++) {
		if (cola[i].nombreCola == codigoCola) {
			for(int j = 0; j < list_size(cola[i].mensajes); i++){

				t_metadata* metadata = list_get(cola[i].mensajes, i);

				/* Chequeo que el ID no este entre los ACK
				 */
				int estaEntreACK = 0;
				int cantACK = list_size(metadata->ACKSuscriptores);
				for(int h = 0; h < cantACK; h++){
					int* ID = list_get(metadata->ACKSuscriptores, h);

					if(*ID == idProceso){
						estaEntreACK = 1;
						break;
					}
				}

				if(estaEntreACK == 0){
					void* estructura = leerMemoria(metadata);
					list_add(mensajes, estructura);
					list_add(mensajes, &metadata->ID);
					list_add(mensajes, &metadata->IDCorrelativo);
				}
			}
		}
	}
	return mensajes;
}

/*void administrarColas(){
	pthread_t threadColaNew;
	pthread_t threadColaAppeared;
	pthread_t threadColaCatch;
	pthread_t threadColaCaught;
	pthread_t threadColaGet;
	pthread_t threadColaLocalized;
		//NEW_POKEMON
	pthread_create(&threadColaNew,NULL, (void *)enviar_mensajes_new, &cola[0]);
		//APPEARED_POKEMON:
	//pthread_create(&threadColaNew,NULL, (void *)enviar_mensaje_suscriptores, cola[1]);
		//CATCH_POKEMON;
	//pthread_create(&threadColaNew,NULL, (void *)enviar_mensaje_suscriptores, cola[2]);
		//CAUGHT_POKEMON;
	//pthread_create(&threadColaNew,NULL, (void *)enviar_mensaje_suscriptores, cola[3]);
		//GET_POKEMON;
	//pthread_create(&threadColaNew,NULL, (void *)enviar_mensaje_suscriptores, cola[4]);
		//LOCALIZED_POKEMON;
	//pthread_create(&threadColaNew,NULL, (void *)enviar_mensaje_suscriptores, cola[5]);

	pthread_join(threadColaNew,NULL);
	pthread_join(threadColaAppeared,NULL);
	pthread_join(threadColaCatch,NULL);
	pthread_join(threadColaCaught,NULL);
	pthread_join(threadColaLocalized,NULL);


	void enviar_mensajes_new(t_cola * cola) {
		t_metadata * metaMensajeAEnviar = malloc(sizeof(t_metadata));
		int i,j,unSuscriptor;
		while(1){
		sem_wait(colaNew);
		for(i=0;i<list_size(cola->mensajes);i++){
		metaMensajeAEnviar = list_get(cola->mensajes,i);
		while(list_size(cola->suscriptores)>list_size(metaMensajeAEnviar->EnvioSuscriptores)){
			unSuscriptor = list_get(cola->suscriptores,j);
			if(){

			}
		}
		}
		}
	}
	*/
//SEMAFORO CONSUMIDOR


