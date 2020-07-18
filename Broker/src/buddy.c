/*
 * t_buddy.c
 *
 *  Created on: Jul 13, 2020
 *      Author: utnso
 */

#include "buddy.h"

struct t_buddy {
    size_t size;
    size_t longest[1];
};

void iniciarBuddySystem(void){
	buddy = NULL;
	buddy = buddy_init(TAMANO_MEMORIA);
}

/* el operando >> mueve los bits del numero en la izquierda hacia la izquierda
 * utilizandolo con los valores de abajo logramos que el valor se incremente en potencias de dos
 * sacando la minima potencia de 2 que sea mayor al numero enviado por parametro
 */
int potenciaDeDosProxima(uint32_t tamanio) {
    tamanio -= 1;
    tamanio = tamanio | (tamanio >> 1);
    tamanio = tamanio | (tamanio >> 2);
    tamanio = tamanio | (tamanio >> 4);
    tamanio = tamanio | (tamanio >> 8);
    tamanio = tamanio | (tamanio >> 16);
    return tamanio + 1;
}

int esPotenciaDeDos(int numero){
	if(!(numero & (numero - 1))){
		return 1;
	}
	else{
		return 0;
	}
}

static inline int hijeIzquierdo(int index)
{
    /* index * 2 + 1 */
    return ((index << 1) + 1);
}

static inline int hijeDerecho(int index)
{
    /* index * 2 + 2 */
    return ((index << 1) + 2);
}

static inline int xadre(int index)
{
    /* (index+1)/2 - 1 */
    return (((index+1)>>1) - 1);
}

#define max(a, b) (((a)>(b))?(a):(b))
#define min(a, b) (((a)<(b))?(a):(b))

struct t_buddy *buddy_init(unsigned sizeMemoria)
{
    struct t_buddy *self = NULL;
    size_t tamanioNodo;

    int i;

    self = (struct t_buddy *) malloc(sizeof(struct t_buddy)
                                     + 2 * sizeMemoria * sizeof(size_t));
    self->size = sizeMemoria;
    tamanioNodo = sizeMemoria * 2;


    int iter_end = sizeMemoria * 2 - 1;
    for (i = 0; i < iter_end; i++) {
        if (esPotenciaDeDos(i+1)) {
            tamanioNodo >>= 1;
        }
        self->longest[i] = tamanioNodo;
    }

    return self;
}

void buddy_destruir(struct t_buddy *self)
{
    free(self);
}

unsigned elegir_hije(struct t_buddy *self, unsigned index, size_t size)
{
    struct compound {
        size_t size;
        unsigned index;
    } hijes[2];
    hijes[0].index = hijeIzquierdo(index);
    hijes[0].size = self->longest[hijes[0].index];
    hijes[1].index = hijeDerecho(index);
    hijes[1].size = self->longest[hijes[1].index];

    int min_idx = (hijes[0].size <= hijes[1].size) ? 0: 1;

    if (size > hijes[min_idx].size) {
        min_idx = 1 - min_idx;
    }

    return hijes[min_idx].index;
}

int buddy_pedir_mem(size_t size){
	return buddy_pedir_memoria(buddy, size);
}
void buddy_liberar_mem(int offset){
	buddy_liberar_memoria(buddy, offset);
}


int buddy_pedir_memoria(struct t_buddy *self, size_t size)
{
    size = potenciaDeDosProxima(size);

    unsigned index = 0;
    if (self->longest[index] < size) {
        return -1;
    }

    unsigned tamanioNodo = 0;
    for (tamanioNodo = self->size; tamanioNodo != size; tamanioNodo >>= 1) {
        index = elegir_hije(self, index, size);
    }

    /* update the *longest* value back */
    self->longest[index] = 0;
    int offset = (index + 1)*tamanioNodo - self->size;

    while (index) {
        index = xadre(index);
        self->longest[index] =
            max(self->longest[hijeIzquierdo(index)],
                self->longest[hijeDerecho(index)]);
    }

    return offset;
}

void buddy_liberar_memoria(struct t_buddy *self, int offset)
{
    if (self == NULL || offset < 0 || offset > self->size) {
        return;
    }

    size_t tamanioNodo;
    unsigned index;

    tamanioNodo = 1;
    index = offset + self->size - 1;

    for (; self->longest[index] != 0; index = xadre(index)) {
        tamanioNodo <<= 1;    /* tamanioNodo *= 2; */
        if (index == 0) {
            break;
        }
    }

    self->longest[index] = tamanioNodo;

    while (index) {
        index = xadre(index);
        tamanioNodo <<= 1;

        size_t left_longest = self->longest[hijeIzquierdo(index)];
        size_t right_longest = self->longest[hijeDerecho(index)];

        if (left_longest + right_longest == tamanioNodo) {
            self->longest[index] = tamanioNodo;
        } else {
            self->longest[index] = max(left_longest, right_longest);
        }
    }
}

int tamanioNodo(struct t_buddy *self, int offset)
{
    unsigned tamanioNodo = 1;
    unsigned index = offset + self->size - 1;

    for (; self->longest[index]; index = xadre(index)) {
        tamanioNodo >>= 1;
    }

    return tamanioNodo;
}
