/*
 * desserializadores.h
 *
 *  Created on: 8 jun. 2020
 *      Author: utnso
 */

#ifndef LIBRARY_DESSERIALIZADORES_H_
#define LIBRARY_DESSERIALIZADORES_H_

#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/string.h>
#include"utils.h"
#include"conexion.h"

void* des_serializar_new_pokemon(int socket_cliente, int size);
void* des_serializar_appeared_pokemon(int socket_cliente, int size);
void* des_serializar_catch_pokemon(int socket_cliente, int size);
void* des_serializar_caught_pokemon(int socket_cliente, int size);
void* des_serializar_get_pokemon(int socket_cliente, int size);
void* des_serializar_localized_pokemon(int socket_cliente, int size);
void* des_serializar_respuesta_id(int socket_cliente, int size);
void* des_serializar_gameboy_suscribe(int socket_cliente, int size);

#endif /* LIBRARY_DESSERIALIZADORES_H_ */
