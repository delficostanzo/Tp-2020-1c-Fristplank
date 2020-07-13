/*
 * colas.h
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_COLAS_H_
#define SRC_COLAS_H_

#include "variables.h"
#include "particiones.h"
#include "memoriaCache.h"

#define SINPARTICION -1;

void agregarSuscriptorACola(int suscriptor, op_code tipoCola);
void agregarMensajeACola(t_paquete * nuevoPaquete);
t_list* mensajesAEnviar(int idProceso, op_code codigoCola);

#endif /* SRC_COLAS_H_ */
