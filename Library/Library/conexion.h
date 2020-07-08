
/*
 * conexion.h
 *
 *
 *      Author: fritsplank
 */

#ifndef CONEXION_H_
#define CONEXION_H_

#include "utils.h"

typedef char* String;

typedef enum {
	IDERROR = -1,
	BROKER = 1,
	TEAM = 2,
	GAMEBOY = 3,
	GAMECARD = 4
}id_proceso;

typedef struct{
	id_proceso id;
	int idUnico;
}t_handshake;

int crearSocket();
bool escuchaEn(int socketListener, int puerto);
bool conectarA(int socketServidor, String ip, int puerto);
int aceptarConexion(int socketListener);
t_handshake* iniciarHandshake(int socketDestino, t_handshake* idProcesoOrigen);
t_handshake* responderHandshake(int socketDestino, t_handshake* idProcesoOrigen);
void* recibirDatos(int socket, int sizeStream);


#endif /* CONEXION_H_ */
