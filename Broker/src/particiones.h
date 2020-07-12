/*
 * particiones.h
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_PARTICIONES_H_
#define SRC_PARTICIONES_H_
#include "Broker.h"

typedef struct{
	int posicion;
	int tamanio;
}t_particion_libre;


bool ordenarId(t_metadata * unaParticion, t_metadata * otraParticion);
bool ordenarTamanio(t_particion_libre * unaParticion, t_particion_libre * otraParticion);
bool ordenarPosicion(t_metadata * unaParticion, t_metadata * otraParticion);
bool ordenarFlagLRU(t_metadata * unaParticion, t_metadata * otraParticion);
void crearParticion(t_metadata * meta);
void eliminarParticion(t_metadata *particionAEliminar);
void particion_destroy(t_metadata *self);
bool ordenarPosicion(t_metadata * unaParticion, t_metadata * otraParticion);
int particionLibre(int sizeDato);
int tamanioParticionMinima(int unTamanio);
void compactarMemoria();

#endif /* SRC_PARTICIONES_H_ */
