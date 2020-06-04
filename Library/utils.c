/*
 * utils.c
 *
 *
 *      Author: fritsplank
 */

#include "utils.h"

void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	int size_serializado = sizeof(op_code) + 3*sizeof(uint32_t) + (paquete->buffer->size);

	void* streamFinal = malloc(size_serializado);
	int offset = 0;

	memcpy(streamFinal + offset, &paquete->codigo_operacion, sizeof(op_code));
	offset += sizeof(op_code);
	memcpy(streamFinal + offset, &paquete->ID, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(streamFinal + offset, &paquete->ID_CORRELATIVO, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(streamFinal + offset, &paquete->buffer->size, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	/* Serializo dependiendo de tipo de mensaje */
	switch(paquete->codigo_operacion){
	case NEW_POKEMON:
		serializar_new_pokemon(&streamFinal, offset, paquete->buffer->stream, bytes);
		break;
	case APPEARED_POKEMON:
		//TODO IMPLEMENTACION
		break;
	case CATCH_POKEMON:
		//TODO IMPLEMENTACION
		break;
	case CAUGHT_POKEMON:
		//TODO IMPLEMENTACION
		break;
	case GET_POKEMON:
		//TODO IMPLEMENTACION
		break;
	case LOCALIZED_POKEMON:
		//TODO IMPLEMENTACION
		break;
	case RESPUESTA_ID:
		//TODO IMPLEMENTACION
		break;
	case ACK:
		//TODO IMPLEMENTACION
		break;
	case GAMEBOYSUSCRIBE:
		//TODO IMPLEMENTACION
		break;
	}
	return streamFinal;
}

void enviar(t_paquete* paquete, int socket_cliente)
{
	int size_serializado;

	void* mensajeAEnviar = serializar_paquete(paquete, &size_serializado);

	send(socket_cliente, mensajeAEnviar, size_serializado, 0);

	free(mensajeAEnviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

t_paquete* crearPaqueteCon(void* datos, int sizeOfStream, uint32_t Id, uint32_t IdCorrelativo, op_code op_code) {

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = op_code;
	paquete->ID = Id;
	paquete->ID_CORRELATIVO = IdCorrelativo;

	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = sizeOfStream;
	paquete->buffer->stream = datos;

	return paquete;
}

void enviar_new_pokemon(t_new_pokemon* new_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon(new_pokemon, new_pokemon->lengthOfPokemon + sizeof(uint32_t)*4, Id, IdCorrelativo, NEW_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_appeared_pokemon(t_appeared_pokemon* appeared_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon(appeared_pokemon, appeared_pokemon->lengthOfPokemon + sizeof(uint32_t)*4, Id, IdCorrelativo, APPEARED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_catch_pokemon(t_catch_pokemon* catch_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon((void*) catch_pokemon, catch_pokemon->lengthOfPokemon + sizeof(uint32_t)*3, Id, IdCorrelativo, CATCH_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_caught_pokemon(t_caught_pokemon* caught_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon((void*) caught_pokemon, sizeof(uint32_t)*2, Id, IdCorrelativo, APPEARED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_get_pokemon(t_get_pokemon* get_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon((void*) get_pokemon, get_pokemon->lengthOfPokemon + sizeof(uint32_t), Id, IdCorrelativo, GET_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_localized_pokemon(t_localized_pokemon* localized_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	int sizeListaPosiciones = list_size(localized_pokemon->listaPosiciones)*sizeof(t_posicion);
	t_paquete* paquete = crearPaqueteCon((void*) localized_pokemon, localized_pokemon->lengthOfPokemon + sizeof(int)*2 + sizeListaPosiciones, Id, IdCorrelativo, LOCALIZED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_respuesta_id(t_respuesta_id* respuesta_id, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo){
	t_paquete* paquete = crearPaqueteCon((void*) respuesta_id, sizeof(uint32_t), Id, IdCorrelativo, RESPUESTA_ID);
	enviar(paquete, socket_cliente);
}

void enviar_ACK(int socket_cliente, uint32_t Id, uint32_t IdCorrelativo){
	t_paquete* paquete = crearPaqueteCon(NULL, 0, Id, IdCorrelativo, ACK);
	enviar(paquete, socket_cliente);
}

void enviar_gameboy_suscribe(t_gameboy_suscribe* gameboy_suscribe, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo){
	t_paquete* paquete = crearPaqueteCon((void*) gameboy_suscribe, sizeof(op_code), Id, IdCorrelativo, GAMEBOYSUSCRIBE);
	enviar(paquete, socket_cliente);
}


/* Retorna el mensaje
 */
t_paquete* recibir_mensaje(int socket_cliente) {

	t_paquete* paquete = malloc(sizeof(t_paquete));

	recv(socket_cliente, &(paquete->codigo_operacion), sizeof(op_code), 0);
	recv(socket_cliente, &(paquete->ID), sizeof(uint32_t), 0);
	recv(socket_cliente, &(paquete->ID_CORRELATIVO), sizeof(uint32_t), 0);
	paquete->buffer = malloc(sizeof(t_buffer));
	recv(socket_cliente, &(paquete->buffer->size), sizeof(uint32_t), 0);

	switch(paquete->codigo_operacion){
	case NEW_POKEMON:;

		t_new_pokemon* new_pokemon = malloc(sizeof(new_pokemon));
		recv(socket_cliente, &(new_pokemon->lengthOfPokemon), sizeof(uint32_t), 0);
		new_pokemon->pokemon = malloc(new_pokemon->lengthOfPokemon);

		recv(socket_cliente, &(new_pokemon->pokemon), new_pokemon->lengthOfPokemon, 0);
		new_pokemon->posicion = malloc(sizeof(t_posicion));

		recv(socket_cliente, &(new_pokemon->posicion->posicionX), sizeof(uint32_t), 0);
		recv(socket_cliente, &(new_pokemon->posicion->posicionY), sizeof(uint32_t), 0);
		recv(socket_cliente, &(new_pokemon->cantidad), sizeof(uint32_t), 0);

		paquete->buffer->stream = new_pokemon;
		break;

	case APPEARED_POKEMON:
		//TODO IMPLEMENTACION
		break;
	case CATCH_POKEMON:
		//TODO IMPLEMENTACION
		break;
	case CAUGHT_POKEMON:
		//TODO IMPLEMENTACION
		break;
	case GET_POKEMON:
		//TODO IMPLEMENTACION
		break;
	case LOCALIZED_POKEMON:
		//TODO IMPLEMENTACION
		break;
	case RESPUESTA_ID:
		//TODO IMPLEMENTACION
		break;
	case ACK:
		//TODO IMPLEMENTACION
		break;
	case GAMEBOYSUSCRIBE:
		//TODO IMPLEMENTACION
		break;
	}
	return paquete;
}

void liberar_conexion(int socket_cliente) {
//	t_log* logger = iniciar_log();
	t_log* logger = iniciar_log("liberarconexion");
	if (close(socket_cliente) == -1) {
		log_error(logger, "Error al cerrar la conexion");
	}
}

//t_log* iniciar_log(void) {
//	t_log* logger;
//	if((logger = log_create("broker.log", "BROKER", 1, log_level_from_string("INFO"))) == NULL){
//		printf("No pude crear el logger\n");
//		exit(1);
//	}
//	return logger;
//}

t_log* iniciar_log(char* nombreModulo) {
	t_log* logger;

	char* nombreArchivoLog = malloc(strlen(nombreModulo) + 1);
	strcpy(nombreArchivoLog, nombreModulo);
	strcat(nombreArchivoLog, ".log");
	string_to_upper(nombreModulo);

	if((logger = log_create(nombreArchivoLog, nombreModulo, 1, log_level_from_string("INFO"))) == NULL){
		printf("No pude crear el logger\n");

		free(nombreArchivoLog);
		free(nombreModulo);
		exit(1);
	}

	free(nombreArchivoLog);
	free(nombreModulo);
	return logger;
}
