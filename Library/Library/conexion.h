/*
 * conexion.h
 *
 *
 *      Author: fritsplank
 */

#ifndef CONEXION_H_
#define CONEXION_H_

#include "utils.h"


typedef enum {
	IDERROR = -1,
	BROKER = 1,
	TEAM = 2,
	GAMEBOY = 3,
	GAMECARD = 4
}id_proceso;

int crearSocket();
bool escuchaEn(int socketListener, int puerto);
bool conectarA(int socketServidor, char* ip, int puerto);
int aceptarConexion(int socketListener);
id_proceso iniciarHandshake(int socketDestino, id_proceso idProcesoOrigen);
id_proceso responderHandshake(int socketDestino, id_proceso idProcesoOrigen);
void* recibirDatos(int socket, int sizeStream);


#endif /* CONEXION_H_ */
