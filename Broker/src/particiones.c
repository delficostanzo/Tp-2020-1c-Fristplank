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

		if(cantidadParticionesEliminadas == FRECUENCIA_COMPACTACION){
			compactarMemoria();
		}

		if (cantParticionesActuales == 0){
			return 0; //si no hay particiones, se crea al inicio
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

					offset += tamanioParticionMinima(auxParticion->tamanioMensaje);
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

			else if ((TAMANO_MEMORIA - offset - 1) >= tamanioParticionMinima(sizeDato)){
				posicionEncontrada = offset;
			}

		}

		else if (string_equals_ignore_case(ALGORITMO_PARTICION_LIBRE, "FF")) { //siempre comienza desde el principio

			for (int i = 0; i < cantParticionesActuales; i++) {
				t_metadata* auxParticion = list_get(particiones, i);

				if (offset == auxParticion->posicion) {
					offset += tamanioParticionMinima(auxParticion->tamanioMensaje);
				}

				else { //asumo que si no es offset, entonces es mas grande
					int diferencia = auxParticion->posicion - offset; //NO ESTA COMPACTADO

					if (diferencia >= tamanioParticionMinima(sizeDato)){
						posicionEncontrada = offset; //hay espacio entre dos particiones?? diferencia >0
					}
					offset += tamanioParticionMinima(auxParticion->tamanioMensaje);
				}

			}
			if ((TAMANO_MEMORIA - offset - 1) >= sizeDato){
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
				list_sort(particiones, (void*) ordenarId);
				t_metadata* auxParticion = list_get(particiones, 0); //aca no tomo las auxTabla ya que lo voy a sacar
				eliminarParticion(auxParticion);
				cantidadParticionesEliminadas++;
			}

			else if (string_equals_ignore_case(ALGORITMO_REEMPLAZO, "LRU")) {
				list_sort(particiones, (void *) ordenarFlagLRU);
				t_metadata* auxParticion = list_get(particiones, 0); //aca no tomo las copias ya que lo voy a sacar
				eliminarParticion(auxParticion);
				cantidadParticionesEliminadas++;
			}
		}
	}

	else if(string_equals_ignore_case(ALGORITMO_MEMORIA, "BS")){
		/* Definimos que el tamaño a buscar sea potencia de dos
		 */
		sizeDato = tamanioParticionMinima(sizeDato);
		posicionEncontrada = buddy_pedir_mem(sizeDato);

		if (posicionEncontrada == -1){

			if (string_equals_ignore_case(ALGORITMO_REEMPLAZO, "FIFO")) {
				list_sort(particiones, (void*) ordenarId);
			}

			else if(string_equals_ignore_case(ALGORITMO_REEMPLAZO, "LRU")){
				list_sort(particiones, (void *) ordenarFlagLRU);
			}

			int i = 0;
			while(posicionEncontrada == -1){
				t_metadata* auxParticion = list_get(particiones, i);
				buddy_liberar_mem(auxParticion->posicion);
				eliminarParticion(auxParticion);
				posicionEncontrada = buddy_pedir_mem(sizeDato);
				i++;
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
	return unaParticion->flagLRU <= unaParticion->flagLRU;
}
bool ordenarId(t_metadata * unaParticion, t_metadata * otraParticion) {
	return unaParticion->ID < otraParticion->ID;
}
int tamanioParticionMinima(int unTamanio){ //LO UTILIZO EN EL OFFSET PARA MARCAR QUE HAY UN TAMAÑO DE PARTICION MINIMA
	if(unTamanio>=TAMANO_MINIMO_PARTICION)return unTamanio;
	else return TAMANO_MINIMO_PARTICION;
}
int existeSuscriptor(t_list * suscriptores, int suscriptor) {
	int auxSuscriptor;
	int encontrado = 0;

	int cantSuscriptoresTotales = list_size(suscriptores);
	for (int i = 0; i < cantSuscriptoresTotales; i++) {
		auxSuscriptor = (int) list_get(suscriptores, i);
		if (auxSuscriptor == suscriptor){
			encontrado = 1;
		}
	}
	return encontrado;
}

void eliminarParticion(t_metadata * particionAEliminar) { //OK

	int posicion = particionAEliminar->posicion;

	for (int i = 0; i < 6; i++) {
		int cantidadMensajes = list_size(cola[i].mensajes);

		for (int j = 0; j < cantidadMensajes; j++) {
			t_metadata* auxMeta = list_get(cola[i].mensajes, j);

			if (auxMeta->posicion == posicion) {
				list_remove_and_destroy_element(cola[i].mensajes, j, (void*) particion_destroy);
				log_info(logger, "Se elimino la particion de la posicion %d", posicion); //OBLIGATORIO (7)
				break;
			}
		}
	}
}

void particion_destroy(t_metadata *self) { //OK
	list_destroy_and_destroy_elements(self->ACKSuscriptores, (void*) ACK_destroy);
	free(self);
}

void ACK_destroy(int* idACK){
	free(idACK);
}
