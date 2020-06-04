/*
 * serializadores.h
 *
 *  Created on: 3 jun. 2020
 *      Author: utnso
 */

#ifndef SERIALIZADORES_H_
#define SERIALIZADORES_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>

#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/string.h>

#include"utils.h"

void serializar_new_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes);
void serializar_appeared_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes);
void serializar_catch_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes);
void serializar_caught_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes);
void serializar_get_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes);
void serializar_localized_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes);
void serializar_respuesta_id(void** streamAEnviar, int offset, void* streamPayload, int *bytes);
void serializar_gameboysuscribe(void** streamAEnviar, int offset, void* streamPayload, int *bytes);

#endif /* SERIALIZADORES_H_ */
