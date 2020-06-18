/*
 * desserializadores.c
 *
 *  Created on: 8 jun. 2020
 *      Author: utnso
 */

#include "desserializadores.h"
#include <stdlib.h>

void* des_serializar_new_pokemon(int socket_cliente, int size){

	void* stream = recibirDatos(socket_cliente, size);
	void* recorrerStream = stream;
	t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));

	memcpy(&(new_pokemon->lengthOfPokemon), recorrerStream, sizeof(uint32_t));
	recorrerStream += sizeof(uint32_t);


	new_pokemon->pokemon = malloc(new_pokemon->lengthOfPokemon + 1);
	memcpy(new_pokemon->pokemon, recorrerStream, new_pokemon->lengthOfPokemon + 1);
	recorrerStream += new_pokemon->lengthOfPokemon + 1;

	new_pokemon->posicion = malloc(sizeof(t_posicion));
	memcpy(&(new_pokemon->posicion->posicionX), recorrerStream, sizeof(uint32_t));
	recorrerStream += sizeof(uint32_t);
	memcpy(&(new_pokemon->posicion->posicionY), recorrerStream, sizeof(uint32_t));
	recorrerStream += sizeof(uint32_t);
	memcpy(&(new_pokemon->cantidad), recorrerStream, sizeof(uint32_t));

	free(stream);
	return new_pokemon;
}

void* des_serializar_appeared_pokemon(int socket_cliente, int size){
	void* stream = recibirDatos(socket_cliente, size);
	void* recorrerStream = stream;
	t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));

	memcpy(&(appeared_pokemon->lengthOfPokemon), recorrerStream, sizeof(uint32_t));
	recorrerStream += sizeof(uint32_t);
	appeared_pokemon->pokemon = malloc(appeared_pokemon->lengthOfPokemon + 1);
	memcpy(appeared_pokemon->pokemon, recorrerStream, appeared_pokemon->lengthOfPokemon + 1);
	recorrerStream += appeared_pokemon->lengthOfPokemon + 1;
	appeared_pokemon->posicion = malloc(sizeof(t_posicion));
	memcpy(&(appeared_pokemon->posicion->posicionX), recorrerStream, sizeof(uint32_t));
	recorrerStream += sizeof(uint32_t);
	memcpy(&(appeared_pokemon->posicion->posicionY), recorrerStream, sizeof(uint32_t));

	free(stream);
	return appeared_pokemon;
}

void* des_serializar_catch_pokemon(int socket_cliente, int size){
	void* stream = recibirDatos(socket_cliente, size);
	void* recorrerStream = stream;
	t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));

	memcpy(&(catch_pokemon->lengthOfPokemon), recorrerStream, sizeof(uint32_t));
	recorrerStream += sizeof(uint32_t);
	catch_pokemon->pokemon = malloc(catch_pokemon->lengthOfPokemon + 1);
	memcpy(catch_pokemon->pokemon, recorrerStream, catch_pokemon->lengthOfPokemon + 1);
	recorrerStream += catch_pokemon->lengthOfPokemon + 1;
	catch_pokemon->posicion = malloc(sizeof(t_posicion));
	memcpy(&(catch_pokemon->posicion->posicionX), recorrerStream, sizeof(uint32_t));
	recorrerStream += sizeof(uint32_t);
	memcpy(&(catch_pokemon->posicion->posicionY), recorrerStream, sizeof(uint32_t));

	free(stream);
	return catch_pokemon;
}

void* des_serializar_caught_pokemon(int socket_cliente, int size){
	void* stream = recibirDatos(socket_cliente, size);
	void* recorrerStream = stream;
	t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));

	memcpy(&(caught_pokemon->ok), recorrerStream, sizeof(uint32_t));

	free(stream);
	return caught_pokemon;
}

void* des_serializar_get_pokemon(int socket_cliente, int size){
	void* stream = recibirDatos(socket_cliente, size);
	void* recorrerStream = stream;
	t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));

	memcpy(&(get_pokemon->lengthOfPokemon), recorrerStream, sizeof(uint32_t));
	recorrerStream += sizeof(uint32_t);
	get_pokemon->pokemon = malloc(get_pokemon->lengthOfPokemon + 1);
	memcpy(get_pokemon->pokemon, recorrerStream, get_pokemon->lengthOfPokemon + 1);

	free(stream);
	return get_pokemon;
}

void* des_serializar_localized_pokemon(int socket_cliente, int size){
	void* stream = recibirDatos(socket_cliente, size);
	void* recorrerStream = stream;
	t_localized_pokemon* localized_pokemon = malloc(sizeof(t_localized_pokemon));

	memcpy(&(localized_pokemon->lengthOfPokemon), recorrerStream, sizeof(uint32_t));
	recorrerStream += sizeof(uint32_t);
	localized_pokemon->pokemon = malloc(localized_pokemon->lengthOfPokemon + 1);
	memcpy(localized_pokemon->pokemon, recorrerStream, localized_pokemon->lengthOfPokemon + 1);
	recorrerStream += localized_pokemon->lengthOfPokemon + 1;
	memcpy(&(localized_pokemon->cantidadPosiciones), recorrerStream, sizeof(uint32_t));
	recorrerStream += sizeof(uint32_t);

	localized_pokemon->listaPosiciones = list_create();

	for(int i = 0; i < localized_pokemon->cantidadPosiciones; i++){
		t_posicion* posicion = malloc(sizeof(t_posicion));

		memcpy(&(posicion->posicionX), recorrerStream, sizeof(uint32_t));
		recorrerStream += sizeof(uint32_t);
		memcpy(&(posicion->posicionY), recorrerStream, sizeof(uint32_t));
		recorrerStream += sizeof(uint32_t);

		list_add(localized_pokemon->listaPosiciones, posicion);
	}
	free(stream);
	return localized_pokemon;
}

void* des_serializar_respuesta_id(int socket_cliente, int size){
	void* stream = recibirDatos(socket_cliente, size);
	void* recorrerStream = stream;
	t_respuesta_id* respuesta_id = malloc(sizeof(t_respuesta_id));

	memcpy(&(respuesta_id->idCorrelativo), recorrerStream, sizeof(uint32_t));
	recorrerStream += sizeof(uint32_t);

	free(stream);
	return respuesta_id;
}

void* des_serializar_gameboy_suscribe(int socket_cliente, int size){
	void* stream = recibirDatos(socket_cliente, size);
	void* recorrerStream = stream;
	t_gameboy_suscribe* gameboy_suscribe = malloc(sizeof(t_gameboy_suscribe));

	memcpy(&(gameboy_suscribe->codigoCola), recorrerStream, sizeof(op_code));
	recorrerStream += sizeof(op_code);

	free(stream);
	return gameboy_suscribe;
}
