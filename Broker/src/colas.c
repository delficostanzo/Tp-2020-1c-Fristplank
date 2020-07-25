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

	pthread_mutex_lock(&mutexLRUcounter);
	LRUcounter++;
	nuevaMeta->flagLRU = LRUcounter;
	pthread_mutex_unlock(&mutexLRUcounter);

	nuevaMeta->posicion = SINPARTICION;
	nuevaMeta->ACKSuscriptores = list_create();
	nuevaMeta->enviadosSuscriptores = list_create();

	log_debug(logger, "Procedo a buscar la cola %d", nuevoPaquete->codigo_operacion);

	for (int i = 0; i < 6; i++) {
		if (cola[i].nombreCola == nuevoPaquete->codigo_operacion) {
			pthread_mutex_lock(&mutexMemoria);
			//EL TAMAÑO Y POSICION DE UN MENSAJE EN MEMORIA  SE CALCULAN EN LA SIGUIENTE FUNCION
			cachearMensaje(nuevaMeta, nuevoPaquete->buffer->stream);
			list_add(cola[i].mensajes, nuevaMeta);
			log_info(logger, "Se agregó un nuevo mensaje a la cola %s.", ID_COLA[i + 1]); //LOG OBLIGATORIO (3)
			pthread_mutex_unlock(&mutexMemoria);
			break;
		}
	}
}

void agregarSuscriptorACola(int idSuscriptor, op_code tipoCola) {

	log_debug(logger, "Entro a agregarSuscriptorACola");

	for (int i = 0; i < 6; i++) {
		if (cola[i].nombreCola == tipoCola) {

			int existeSuscriptor1 = existeSuscriptor(cola[i].suscriptores, idSuscriptor);
			if (existeSuscriptor1 == 0) {
				suscriptorEnCola* suscriptorEnCola = malloc(sizeof(suscriptorEnCola));
				suscriptorEnCola->ID = idSuscriptor;
				list_add(cola[i].suscriptores, suscriptorEnCola);
				log_info(logger, "Se suscribió con éxito el proceso a la cola %s", ID_COLA[tipoCola]);
				break;
			}
		}
	}
}

t_list* mensajesAEnviar(int idProceso, op_code codigoCola){

	log_debug(logger, "entro a mensajesAEnviar");
	t_list* mensajes = list_create();

	for (int i = 0; i < 6; i++) {
		if (cola[i].nombreCola == codigoCola) {
			for(int j = 0; j < list_size(cola[i].mensajes); j++){

				t_metadata* metadata = list_get(cola[i].mensajes, j);

				/* Chequeo que el ID no este entre los ACK
				 */
				int estaEnEnviados = 0;
				int cantEnviados = list_size(metadata->enviadosSuscriptores);
				for(int h = 0; h < estaEnEnviados; h++){
					int* ID = list_get(metadata->enviadosSuscriptores, h);

					if(*ID == idProceso){
						estaEnEnviados = 1;
						break;
					}
				}

				if(estaEnEnviados == 0){
					log_debug(logger, "antes de leerMemoria");
					log_debug(logger, "Tipo de mensaje %d", metadata->tipoMensaje);
					log_debug(logger, "tamaño en memoria %d", metadata->tamanioMensajeEnMemoria);
					pthread_mutex_lock(&mutexMemoria);
					void* estructura = leerMemoria(metadata);
					pthread_mutex_unlock(&mutexMemoria);
					log_debug(logger, "despues de leerMemoria");
					list_add(mensajes, estructura);
					list_add(mensajes, &(metadata->ID));
					list_add(mensajes, &(metadata->IDCorrelativo));
				}
			}
		}
	}
	log_debug(logger,"Salgo de mensajesAEnviar");
	return mensajes;
}

void agregarIdAEnviados(int idSuscriptor, int idMensaje, op_code colaMensaje){

	for(int i = 0; i < 6; i++){
		if(cola[i].nombreCola == cola){

			int cantMensajes = list_size(cola[i].mensajes);

			for(int j = 0; j < cantMensajes; j++){
				t_metadata* mensaje = list_get(cola[i].mensajes, j);

				if(mensaje->ID == idMensaje){
					list_add(mensaje->enviadosSuscriptores, idSuscriptor);
					log_debug(logger, "Se ha agregado el suscriptor %d a los enviados del mensaje %d", idSuscriptor, mensaje->ID);
					break;
				}
			}
			break;
		}
	}
}

