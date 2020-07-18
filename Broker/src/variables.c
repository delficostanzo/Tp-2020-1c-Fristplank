/*
 * variables.c
 *
 *  Created on: Jul 13, 2020
 *      Author: utnso
 */


#include "variables.h"

int crearID() {
	IDmsg++;
	return IDmsg;

}

int existeSuscriptor(t_list * suscriptores, int suscriptor) {
	int encontrado = 0;

	int cantSuscriptoresTotales = list_size(suscriptores);
	for (int i = 0; i < cantSuscriptoresTotales; i++) {
		suscriptorEnCola* auxSuscriptor = list_get(suscriptores, i);

		if (auxSuscriptor->ID == suscriptor){
			encontrado = 1;
		}
	}
	log_debug(logger, "Encontrado : %d", encontrado);
	return encontrado;
}

