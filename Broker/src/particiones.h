/*
 * particiones.h
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_PARTICIONES_H_
#define SRC_PARTICIONES_H_

#include "memoriaCache.h"
#include "buddy.h"

typedef struct{
	int posicion;
	int tamanio;
}t_particion;



bool ordenarId(t_metadata * unaParticion, t_metadata * otraParticion);
bool ordenarTamanio(t_particion * unaParticion, t_particion * otraParticion);
bool ordenarPosicion(t_metadata * unaParticion, t_metadata * otraParticion);
bool ordenarFlagLRU(t_metadata * unaParticion, t_metadata * otraParticion);

void crearParticion(t_metadata * meta);
void eliminarParticion(t_metadata *particionAEliminar);
void particion_destroy(t_metadata *self);
bool ordenarPosicion(t_metadata * unaParticion, t_metadata * otraParticion);

int particionLibre(int sizeDato);
int tamanioParticionMinima(int unTamanio);

/*Funciones para list_destroy_and_destroy_elements
 */
void liberarPuntero(t_particion* self);
void puntero_destroy(int* id);

#endif /* SRC_PARTICIONES_H_ */
