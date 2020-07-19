/*
 * particiones.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "particiones.h"

int particionLibre(int sizeDato) { //PARTICIONES OK | FALTA BUDDY SYSTEM

	int offset = 0;
	int posicionEncontrada = -1;
	t_list * particiones = list_create();

	/* Meto todas las metadatas de las colas en una sola lista
	 */
	for (int j = 0; j < 6; j++) {
		list_add_all(particiones, cola[j].mensajes);
	}

	list_sort(particiones, (void*) ordenarPosicion);
	int cantParticionesActuales = list_size(particiones);
	t_list * particionesLibres = list_create();

	if (string_equals_ignore_case(ALGORITMO_MEMORIA, "PARTICIONES")) {

		if (cantParticionesActuales == 0){
			return 0; //si no hay particiones, se crea al inicio
		}

		if(cantidadParticionesEliminadas == FRECUENCIA_COMPACTACION){
			compactarMemoria(particiones);
			cantParticionesActuales = list_size(particiones);
		}

		if (string_equals_ignore_case(ALGORITMO_PARTICION_LIBRE, "BF")) {

			for (int i = 0; i < cantParticionesActuales; i++) {
				t_metadata* auxParticion = list_get(particiones, i);

				/* La particion actual empieza donde termina la anterior
				 */
				if (offset == auxParticion->posicion) {
					offset += tamanioParticionMinima(auxParticion->tamanioMensajeEnMemoria);
				}

				/* Si la particion actual no empieza donde termina la anterior
				 */
				else {
					int diferencia = auxParticion->posicion - offset;
					if (diferencia >= tamanioParticionMinima(sizeDato)) {
						t_particion* particion = malloc(sizeof(t_particion));
						particion->posicion = offset;
						particion->tamanio = diferencia;
						list_add(particionesLibres, particion);
					}

					offset += tamanioParticionMinima(auxParticion->tamanioMensajeEnMemoria);
				}
			}

			/* Si hay particiones libres
			 * Asigno la primera
			 */
			if (!list_is_empty(particionesLibres)) {
				list_sort(particionesLibres, (void*) ordenarTamanio);
				t_particion* particion = list_get(particionesLibres, 0);
				posicionEncontrada = particion->posicion;
			}

			else if ((TAMANO_MEMORIA - offset) >= tamanioParticionMinima(sizeDato)){
				posicionEncontrada = offset;
			}

		}

		else if (string_equals_ignore_case(ALGORITMO_PARTICION_LIBRE, "FF")) { //siempre comienza desde el principio

			for (int i = 0; i < cantParticionesActuales; i++) {
				t_metadata* auxParticion = list_get(particiones, i);

				log_debug(logger, "offset es %d", offset);
				if (offset == auxParticion->posicion) {
					log_debug(logger, "mensajeEncontrado en pos %d", auxParticion->posicion);
					offset += tamanioParticionMinima(auxParticion->tamanioMensajeEnMemoria);
					log_debug(logger, "tamanioMensaje %d", tamanioParticionMinima(auxParticion->tamanioMensajeEnMemoria));
				}

				else { //asumo que si no es offset, entonces es mas grande
					int diferencia = auxParticion->posicion - offset; //NO ESTA COMPACTADO

					if (diferencia >= tamanioParticionMinima(sizeDato)){
						posicionEncontrada = offset; //hay espacio entre dos particiones?? diferencia >0
						break;
					}
					offset += tamanioParticionMinima(auxParticion->tamanioMensajeEnMemoria);
					log_debug(logger, "Offset: %d", offset);
				}

			}
			if ((TAMANO_MEMORIA - offset) >= tamanioParticionMinima(sizeDato)){
				posicionEncontrada = offset; //SI aun puedo seguir agregando, retorno la ultima posicion
			}
		}

		/* No encontro posicion
		 * procedo a eliminar particiones
		 */
		if(posicionEncontrada == -1){
			//si no retorno nada en BF y FF --> hay que el eliminar 1 particion
			//FIFO ESCOGE EL MENSAJE MAS VIEJO
			if (string_equals_ignore_case(ALGORITMO_REEMPLAZO, "FIFO")) { //considero que el primer nodo en particiones es la que entro primero
				log_debug(logger, "Entro en if de FIFO");
				list_sort(particiones, (void*) ordenarId);
				log_debug(logger, "Ordeno");
				t_metadata* auxParticion = list_get(particiones, 0); //aca no tomo las auxTabla ya que lo voy a sacar
				log_debug(logger, "list_get(particiones,0)");
				eliminarParticion(auxParticion);
				log_debug(logger, "termino de eliminar");
			}

			else if (string_equals_ignore_case(ALGORITMO_REEMPLAZO, "LRU")) {
				list_sort(particiones, (void *) ordenarFlagLRU);
				t_metadata* auxParticion = list_get(particiones, 0); //aca no tomo las copias ya que lo voy a sacar
				eliminarParticion(auxParticion);
			}
		}
	}

	else if(string_equals_ignore_case(ALGORITMO_MEMORIA, "BS")){
		/* Definimos que el tamaño a buscar sea potencia de dos
		 */
		log_debug(logger, "Entro a if de BS");

		sizeDato = tamanioParticionMinima(sizeDato);
		posicionEncontrada = buddy_pedir_mem(sizeDato);

		log_debug(logger, "Posición encontrada %d", posicionEncontrada);

		if (posicionEncontrada == -1){

			if (string_equals_ignore_case(ALGORITMO_REEMPLAZO, "FIFO")) {
				list_sort(particiones, (void*) ordenarId);
			}

			else if(string_equals_ignore_case(ALGORITMO_REEMPLAZO, "LRU")){
				list_sort(particiones, (void *) ordenarFlagLRU);
			}

			for(int i = 0; posicionEncontrada == -1; i++){
				t_metadata* auxParticion = list_get(particiones, i);
				buddy_liberar_mem(auxParticion->posicion);
				eliminarParticion(auxParticion);
				posicionEncontrada = buddy_pedir_mem(sizeDato);
			}
		}
	}

	list_destroy_and_destroy_elements(particionesLibres, (void*) liberarPuntero);
	list_destroy(particiones);
	return posicionEncontrada;
}

void liberarPuntero(t_particion* self){
	free(self);
}

bool ordenarTamanio(t_particion * unaParticion,
		t_particion * otraParticion) {
	return unaParticion->tamanio <= otraParticion->tamanio;
}
bool ordenarPosicion(t_metadata * unaParticion, t_metadata * otraParticion) {
	return unaParticion->posicion <= otraParticion->posicion;
}
bool ordenarFlagLRU(t_metadata * unaParticion, t_metadata * otraParticion) {
	return unaParticion->flagLRU < otraParticion->flagLRU;
}
bool ordenarId(t_metadata * unaParticion, t_metadata * otraParticion) {
	return unaParticion->ID < otraParticion->ID;
}
int tamanioParticionMinima(int unTamanio){ //LO UTILIZO EN EL OFFSET PARA MARCAR QUE HAY UN TAMAÑO DE PARTICION MINIMA
	if(unTamanio>=TAMANO_MINIMO_PARTICION)return unTamanio;
	else return TAMANO_MINIMO_PARTICION;
}

void eliminarParticion(t_metadata * particionAEliminar) { //OK

	int posicion = particionAEliminar->posicion;
	int colaABuscar = particionAEliminar->tipoMensaje - 1;
	log_debug(logger, "%s", ID_COLA[colaABuscar]);

	int cantidadMensajes = list_size(cola[colaABuscar].mensajes);

	for(int i = 0; i < cantidadMensajes; i++){
		t_metadata* auxMeta = list_get(cola[colaABuscar].mensajes, i);

		if(auxMeta->posicion == posicion){
			list_remove_and_destroy_element(cola[colaABuscar].mensajes, i, (void*) particion_destroy);
			log_info(logger, "Se eliminó la partición de la posición [%d]", auxMeta->posicion); //OBLIGATORIO (7)
			cantidadParticionesEliminadas++;
			break;
		}
	}
}

void particion_destroy(t_metadata *self) { //OK
	list_destroy_and_destroy_elements(self->ACKSuscriptores, (void*) puntero_destroy);
	list_destroy_and_destroy_elements(self->enviadosSuscriptores, (void*) puntero_destroy);
	free(self);
}

void puntero_destroy(int* idACK){
	free(idACK);
}
