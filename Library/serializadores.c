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
	memcpy(*streamAEnviar + offset, pokemon->pokemon, pokemon->lengthOfPokemon + 1);
	offset += pokemon->lengthOfPokemon + 1;
	memcpy(*streamAEnviar + offset, &pokemon->posicion->posicionX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*streamAEnviar + offset, &pokemon->posicion->posicionY, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*streamAEnviar + offset, &pokemon->cantidad, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	*bytes = offset;

	free(pokemon->pokemon);
	free(pokemon->posicion);
}

void serializar_appeared_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	t_appeared_pokemon* pokemon = streamPayload;

	memcpy(*streamAEnviar + offset, &pokemon->lengthOfPokemon, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*streamAEnviar + offset, pokemon->pokemon, pokemon->lengthOfPokemon + 1);
	offset += pokemon->lengthOfPokemon + 1;
	memcpy(*streamAEnviar + offset, &pokemon->posicion->posicionX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*streamAEnviar + offset, &pokemon->posicion->posicionY, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	*bytes = offset;

	free(pokemon->pokemon);
	free(pokemon->posicion);
}

void serializar_catch_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	t_catch_pokemon* pokemon = streamPayload;

	memcpy(*streamAEnviar + offset, &pokemon->lengthOfPokemon, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*streamAEnviar + offset, pokemon->pokemon, pokemon->lengthOfPokemon + 1);
	offset += pokemon->lengthOfPokemon + 1;
	memcpy(*streamAEnviar + offset, &pokemon->posicion->posicionX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*streamAEnviar + offset, &pokemon->posicion->posicionY, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	*bytes = offset;

	free(pokemon->pokemon);
	free(pokemon->posicion);
}

void serializar_caught_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	t_caught_pokemon* pokemon = streamPayload;

	memcpy(*streamAEnviar + offset, &pokemon->ok, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	*bytes = offset;
}

void serializar_get_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	t_get_pokemon* pokemon = streamPayload;

	memcpy(*streamAEnviar + offset, &pokemon->lengthOfPokemon, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*streamAEnviar + offset, pokemon->pokemon, pokemon->lengthOfPokemon + 1);
	offset += pokemon->lengthOfPokemon + 1;

	*bytes = offset;

	free(pokemon->pokemon);
}

void serializar_localized_pokemon(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	t_localized_pokemon* pokemon = streamPayload;

	memcpy(*streamAEnviar + offset, &pokemon->lengthOfPokemon, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*streamAEnviar + offset, pokemon->pokemon, pokemon->lengthOfPokemon + 1);
	offset += pokemon->lengthOfPokemon + 1;
	memcpy(*streamAEnviar + offset, &pokemon->cantidadPosiciones, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	for(int i = 0; i < pokemon->cantidadPosiciones; i++){
		t_posicion* posicion = list_get(pokemon->listaPosiciones, i);
		memcpy(*streamAEnviar + offset, &posicion->posicionX, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(*streamAEnviar + offset, &posicion->posicionY, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		free(posicion);
	}

	*bytes = offset;

	free(pokemon->pokemon);
	list_destroy(pokemon->listaPosiciones);
}

void serializar_respuesta_id(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	t_respuesta_id* pokemon = streamPayload;

	memcpy(*streamAEnviar + offset, &pokemon->idCorrelativo, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	*bytes = offset;
}

void serializar_gameboy_suscribe(void** streamAEnviar, int offset, void* streamPayload, int *bytes){
	t_gameboy_suscribe* pokemon = streamPayload;

	memcpy(*streamAEnviar + offset, &pokemon->codigoCola, sizeof(op_code));
	offset += sizeof(op_code);

	*bytes = offset;
}

