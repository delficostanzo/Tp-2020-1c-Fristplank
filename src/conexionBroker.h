/*
 * conexion.h
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_CONEXION_H_
#define SRC_CONEXION_H_

#include "Broker.h"
#include "colas.h"

typedef struct {

	op_code tipoMensaje;
	int ID;
	int IDCorrelativo;
	int posicion;
	int tamanioMensaje; // con /0
	int tamanioMensajeEnMemoria; // sin /0
	t_list * ACKSuscriptores;
	int flagLRU;
	char ultimaReferencia;
//int flagFIFO; id mensaje
} t_metadata;

typedef struct{
	int id;
	int* socket;
}t_args_socket_escucha;

typedef struct{
	int id;
	int* socket;
	op_code cola;
}t_args_socket_ACK;


typedef struct {
	int* socketNuevo;
	int* socketOG;
}argumentos;

//BORRAR
int socketNew;
int socketACKNew;
int socketCatch;
int socketACKCatch;
int socketGet;
int socketACKGet;
int socketAppeared;
int socketCaught;
int socketLocalized;


int conexionCliente;
int socketCliente;
void * esperarClientes();
//void atenderCliente(int* cliente);
void atenderCliente(argumentos* sockets);

void lanzarHiloEscucha(int id, int* socket);
void lanzarHiloEscuchaACK(int id, int* socket, op_code cola);

void escucharSocketMensajesACachear(t_args_socket_escucha* args);
void escucharSocketACK(void* socketArgs);

void enviar_mensajes_cacheados(t_list* mensajes, op_code tipoDeMensaje, int* socket);
void enviar_mensaje_a_suscriptores(t_paquete* paquete);
void enviar_mensaje_NEW_a_suscriptores(void* paquete);
void enviar_mensaje_APPEARED_a_suscriptores(void* paquete);
void enviar_mensaje_CATCH_a_suscriptores(void* paquete);
void enviar_mensaje_CAUGHT_a_suscriptores(void* paquete);
void enviar_mensaje_GET_a_suscriptores(void* paquete);
void enviar_mensaje_LOCALIZED_a_suscriptores(void* paquete);

void liberoMensaje(t_metadata* mensaje);
void liberoPuntero(int* puntero); // Para liberar los punteros a Entero cuando tengo los ACK suficientes

#endif /* SRC_CONEXION_H_ */
