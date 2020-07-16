/*
 * t_buddy.h
 *
 *  Created on: Jul 13, 2020
 *      Author: utnso
 */

#ifndef SRC_BUDDY_H_
#define SRC_BUDDY_H_

#include "variables.h"
#include <sys/mman.h>
#include <math.h>

struct t_buddy;
struct t_buddy* buddy;

struct t_buddy *buddy_init(unsigned num_of_fragments);
int buddy_pedir_memoria(struct t_buddy *self, size_t size);
void buddy_liberar_memoria(struct t_buddy *self, int offset);
int tamanioNodo(struct t_buddy *self, int offset);

int buddy_pedir_mem(size_t size);
void buddy_liberar_mem(int offset);

int MIN;
int NIVELES;
int MAX;
uint32_t CANTIDADBLOQUES;

void iniciarBuddySystem(void);

int potenciaDeDosProxima(uint32_t size);
int esPotenciaDeDos(int numero);

#endif /* SRC_BUDDY_H_ */
