/*
 * serializadores.c
 *
 *  Created on: 3 jun. 2020
 *      Author: utnso
 */

#include "serializadores.h"


void serializar_new_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	t_new_pokemon* pokemon = streamPayload;

	memcpy(*streamAEnviar + offset, &pokemon->lengthOfPokemon, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*streamAEnviar + offset, &pokemon->pokemon, pokemon->lengthOfPokemon);
	offset += pokemon->lengthOfPokemon;
	memcpy(*streamAEnviar + offset, &pokemon->posicion->posicionX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*streamAEnviar + offset, &pokemon->posicion->posicionY, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*streamAEnviar + offset, &pokemon->cantidad, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	*bytes = offset;
}

void serializar_appeared_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	//TODO
}

void serializar_catch_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	//TODO
}

void serializar_caught_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	//TODO
}

void serializar_get_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	//TODO
}

void serializar_localized_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	//TODO
}

void serializar_respuesta_id(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	//TODO
}

void serializar_gameboysuscribe(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	//TODO
}

