/*
 * procesar.h
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_PROCESAR_H_
#define SRC_PROCESAR_H_

#include"GameCard.h"

typedef struct{
	t_get_pokemon* get_pokemon;
	t_localized_pokemon* puntero_a_localized_pokemon;
}t_argumentos_procesar_get;

void procesarGetPokemon(t_argumentos_procesar_get* args);
int* procesarCatchPokemon(void* args);
void procesarNewPokemon(void* args);

#endif /* SRC_PROCESAR_H_ */
