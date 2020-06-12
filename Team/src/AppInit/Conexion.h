/*
 * Conexion.h
 *
 *  Created on: 11 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_APPINIT_CONEXION_H_
#define SRC_APPINIT_CONEXION_H_

#include "utils.h"
#include "ConfigFunctions.h"
#include "../Team.h"

//typedef struct{
//	int socketEscucha;
//	int socketACK;
//}t_par_de_sockets;

void* escucharGameBoy(void);
void* generarSocketsConBroker(void);

void* escucharColaAppearedPokemon(void);
void* escucharColaCaughtPokemon(void);
void* escucharColaLocalizedPokemon(void);

#endif /* SRC_APPINIT_CONEXION_H_ */
