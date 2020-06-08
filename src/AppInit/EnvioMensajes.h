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
#include "../Model/Pokemon.h"

typedef struct{
	t_list* idsCorrelativos;
}idsCorrelativosLocalized;

typedef struct{
	t_list* idCorrelativos;
}idsCorrelativosCaught;

void enviarGetDesde(t_list* objetivosGlobales, int socketGet);
void recibirIdGet(int socketGet);
void recibirLocalizedYGuardalos(int socketLocalized, t_list* pokemonesGlobales, t_list* pokemonesLibres);
void recibirAppearedYGuardarlos(int socketAppeared, t_list* pokemonesGlobales, t_list* pokemonesLibres);

#endif /* SRC_APPINIT_ENVIOMENSAJES_H_ */
