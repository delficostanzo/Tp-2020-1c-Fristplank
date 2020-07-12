/*
 * particiones.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "particiones.h"

int particionLibre(int sizeDato) { //PARTICIONES OK | FALTA BUDDY SYSTEM

	int offset, j, i, diferencia;
	t_metadata * auxParticion = malloc(sizeof(t_metadata));
	t_list * particiones = malloc(sizeof(t_list));

	for (j = 0; j < 6; j++) {
		list_add_all(particiones, cola[j].mensajes);
	}
	list_sort(particiones, (void*) ordenarPosicion);
	int cantParticionesActuales = list_size(particiones);
	t_particion_libre * particion = malloc(sizeof(t_particion_libre));
	t_list * particionesLibres = malloc(sizeof(t_list*));

	if (!particiones)
		return 0; //si no hay particiones, se crea al inicio

	if (string_equals_ignore_case(ALGORITMO_MEMORIA, "PARTICIONES")) {

		if (string_equals_ignore_case(ALGORITMO_PARTICION_LIBRE, "BF")) {

			for (i = 0; i < cantParticionesActuales; i++) {
				auxParticion = list_get(particiones, i);
				if (offset == auxParticion->posicion) {
						offset += tamanioParticionMinima(auxParticion->tamanioMensaje);
					continue;
				}

				if (offset != auxParticion->posicion) {
					diferencia = auxParticion->posicion - offset;
					if (diferencia >= sizeDato) {
						particion->posicion = offset;
						particion->tamanio = diferencia;
						list_add(particionesLibres, particion);
						offset += tamanioParticionMinima(auxParticion->tamanioMensaje);
						continue;
					}
				}
			}
			if (particionesLibres) {
				list_sort(particionesLibres, (void*) ordenarTamanio);
				particion = list_get(particionesLibres, 0);
				return particion->posicion;
			} else {
				if ((TAMANO_MEMORIA - offset) >= sizeDato)
					return offset;
			}
		} //FIN BF

		if (string_equals_ignore_case(ALGORITMO_PARTICION_LIBRE, "FF")) { //siempre comienza desde el principio

			for (i = 0; i < cantParticionesActuales; i++) {
				auxParticion = list_get(particiones, i);

				if (offset == auxParticion->posicion) {
					offset += tamanioParticionMinima(auxParticion->tamanioMensaje);
					continue;
				}

				if (offset != auxParticion->posicion) { //asumo que si no es offset, entonces es mas grande
					diferencia = auxParticion->posicion - offset; //NO ESTA COMPACTADO
					if (diferencia >= sizeDato)
						return offset; //hay espacio entre dos particiones?? diferencia >0
					offset += tamanioParticionMinima(auxParticion->tamanioMensaje);
				}

			}
			if ((TAMANO_MEMORIA - offset) >= sizeDato)
				return offset; //SI aun puedo seguir agregando, retorno la ultima posicion
		//si no retorno nada en BF y FF --> hay que el eliminar 1 particion
		//FIFO ESCOGE EL MENSAJE MAS VIEJO
		if (string_equals_ignore_case(ALGORITMO_REEMPLAZO, "FIFO")) { //considero que el primer nodo en particiones es la que entro primero
			list_sort(particiones, (void*) ordenarId);
			auxParticion = list_get(particiones, i); //aca no tomo las auxTabla ya que lo voy a sacar
			eliminarParticion(auxParticion);
			}

		}
		if (string_equals_ignore_case(ALGORITMO_REEMPLAZO, "LRU")) {

			list_sort(particiones, (void *) ordenarFlagLRU);
				auxParticion = list_get(particiones, i); //aca no tomo las copias ya que lo voy a sacar
				offset = auxParticion->posicion;
				eliminarParticion(auxParticion);
			}

	}
	free(particion);
	free(auxParticion);
	free(particiones);
	return -1;
}

bool ordenarTamanio(t_particion_libre * unaParticion,
		t_particion_libre * otraParticion) {
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
	int i, j, cantidadMensajes;
	t_metadata * auxMeta = malloc(sizeof(t_metadata));

	for (i = 0; i < 6; i++) {
		cantidadMensajes = list_size(cola[i].mensajes);
		for (j = 0; j < cantidadMensajes; j++) {
			auxMeta = list_get(cola[i].mensajes, j);
			if (auxMeta->posicion == posicion) {
				list_remove_and_destroy_element(cola[i].mensajes, j,
						(void*) particion_destroy);
				log_info(logger, "Se elimino la particion de la posicion %d",
						posicion); //OBLIGATORIO (7)
			}
		}
	}

	free(auxMeta);
}

void particion_destroy(t_metadata *self) { //OK
	free(self);
}


