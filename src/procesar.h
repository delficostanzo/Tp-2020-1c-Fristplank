/*
 * procesar.h
 *
 *  Created on: 4 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_PROCESAR_H_
#define SRC_PROCESAR_H_

#include "GameBoy.h"

extern t_log* logger;

void procesarBrokerGetPokemon(int socket, char* argv[]);
void procesarBrokerNewPokemon(int socket, char* argv[]);
void procesarBrokerAppearedPokemon(int socket, char* argv[]);
void procesarBrokerCatchPokemon(int socket, char* argv[]);
void procesarBrokerCaughtPokemon(int socket, char* argv[]);
void procesarTeamAppearedPokemon(int socket, char* argv[]);
void procesarGameCardCatchPokemon(int socket, char* argv[]);
void procesarGameCardNewPokemon(int socket, char* argv[]);
void procesarGameCardGetPokemon(int socket, char* argv[]);


#endif /* SRC_PROCESAR_H_ */
