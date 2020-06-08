/*
 * EnvioMensajes.h
 *
 *  Created on: 7 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_APPINIT_ENVIOMENSAJES_H_
#define SRC_APPINIT_ENVIOMENSAJES_H_

#include "utils.h"
#include "CreacionEstructurasMsj.h"
#include "Pokemon.h"

typedef struct{
	t_list* idsCorrelativos;
}idCorrelativosLocalized;

typedef struct{
	t_list* idCorrelativos;
}idCorrelativosCaught;

void enviarGetDesde(t_list* objetivosGlobales, int socket);

#endif /* SRC_APPINIT_ENVIOMENSAJES_H_ */
