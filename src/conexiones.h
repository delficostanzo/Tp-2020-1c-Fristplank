/*
 * conexiones.h
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_CONEXIONES_H_
#define SRC_CONEXIONES_H_

#include"GameCard.h"
#include"procesar.h"

typedef struct{
	int socketEscucha;
	int socketACK;
}t_par_de_sockets;

void* generarSocketsConBroker(void);
void* escucharGameBoy(void);

void* escucharColaNewPokemon(void);
void* escucharColaCatchPokemon(void);
void* escucharColaGetPokemon(void);

#endif /* SRC_CONEXIONES_H_ */
