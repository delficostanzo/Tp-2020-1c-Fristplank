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
		serializar_appeared_pokemon(&streamFinal, offset, paquete->buffer->stream, bytes);
		break;
	case CATCH_POKEMON:
		serializar_catch_pokemon(&streamFinal, offset, paquete->buffer->stream, bytes);
		break;
	case CAUGHT_POKEMON:
		serializar_caught_pokemon(&streamFinal, offset, paquete->buffer->stream, bytes);
		break;
	case GET_POKEMON:
		serializar_get_pokemon(&streamFinal, offset, paquete->buffer->stream, bytes);
		break;
	case LOCALIZED_POKEMON:
		//TODO IMPLEMENTACION
		break;
	case RESPUESTA_ID:
		serializar_respuesta_id(&streamFinal, offset, paquete->buffer->stream, bytes);
		break;
	case ACK:
		// No se hace nada porque el ACK no tiene payload
		break;
	case GAMEBOYSUSCRIBE:
		serializar_gameboy_suscribe(&streamFinal, offset, paquete->buffer->stream, bytes);
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
	paquete->buffer->stream = datos;
	paquete->buffer->size = sizeOfStream;
	return paquete;
}

void enviar_new_pokemon(t_new_pokemon* new_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon(new_pokemon, new_pokemon->lengthOfPokemon + sizeof(uint32_t)*4, Id, IdCorrelativo, NEW_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_appeared_pokemon(t_appeared_pokemon* appeared_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon(appeared_pokemon, appeared_pokemon->lengthOfPokemon + sizeof(uint32_t)*3, Id, IdCorrelativo, APPEARED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_catch_pokemon(t_catch_pokemon* catch_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon((void*) catch_pokemon, catch_pokemon->lengthOfPokemon + sizeof(uint32_t)*3, Id, IdCorrelativo, CATCH_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_caught_pokemon(t_caught_pokemon* caught_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon((void*) caught_pokemon, sizeof(uint32_t), Id, IdCorrelativo, APPEARED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_get_pokemon(t_get_pokemon* get_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon((void*) get_pokemon, get_pokemon->lengthOfPokemon + sizeof(uint32_t), Id, IdCorrelativo, GET_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_localized_pokemon(t_localized_pokemon* localized_pokemon, int socket_cliente, uint32_t Id, uint32_t IdCorrelativo) {
	uint32_t sizeListaPosiciones = (localized_pokemon->cantidadPosiciones)*sizeof(t_posicion);
	t_paquete* paquete = crearPaqueteCon((void*) localized_pokemon, localized_pokemon->lengthOfPokemon + sizeof(uint32_t)*2 + sizeListaPosiciones, Id, IdCorrelativo, LOCALIZED_POKEMON);
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

/* Retorna el mensaje completo: Header y Payload ya serializado
 *
 * No me deja por alguna razón llevar esas funciones des_serializar a otro fuente
 * porque me tira que no reconoce t_paquete (idk, no intenté mucho para ser honesto)
 *
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
		t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
		recv(socket_cliente, &(new_pokemon->lengthOfPokemon), sizeof(uint32_t), 0);
		new_pokemon->pokemon = malloc(new_pokemon->lengthOfPokemon);
		recv(socket_cliente, &(new_pokemon->pokemon), new_pokemon->lengthOfPokemon, 0);
		new_pokemon->posicion = malloc(sizeof(t_posicion));
		recv(socket_cliente, &(new_pokemon->posicion->posicionX), sizeof(uint32_t), 0);
		recv(socket_cliente, &(new_pokemon->posicion->posicionY), sizeof(uint32_t), 0);
		recv(socket_cliente, &(new_pokemon->cantidad), sizeof(uint32_t), 0);

		paquete->buffer->stream = new_pokemon;
		break;

	case APPEARED_POKEMON:;
		t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
		recv(socket_cliente, &(appeared_pokemon->lengthOfPokemon), sizeof(uint32_t), 0);
		appeared_pokemon->pokemon = malloc(appeared_pokemon->lengthOfPokemon);
		recv(socket_cliente, &(appeared_pokemon->pokemon), appeared_pokemon->lengthOfPokemon, 0);
		appeared_pokemon->posicion = malloc(sizeof(t_posicion));
		recv(socket_cliente, &(appeared_pokemon->posicion->posicionX), sizeof(uint32_t), 0);
		recv(socket_cliente, &(appeared_pokemon->posicion->posicionY), sizeof(uint32_t), 0);

		paquete->buffer->stream = appeared_pokemon;
		break;

	case CATCH_POKEMON:;
		t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
		recv(socket_cliente, &(catch_pokemon->lengthOfPokemon), sizeof(uint32_t), 0);
		catch_pokemon->pokemon = malloc(catch_pokemon->lengthOfPokemon);
		recv(socket_cliente, &(catch_pokemon->pokemon), catch_pokemon->lengthOfPokemon, 0);
		catch_pokemon->posicion = malloc(sizeof(t_posicion));
		recv(socket_cliente, &(catch_pokemon->posicion->posicionX), sizeof(uint32_t), 0);
		recv(socket_cliente, &(catch_pokemon->posicion->posicionY), sizeof(uint32_t), 0);

		paquete->buffer->stream = catch_pokemon;
		break;

	case CAUGHT_POKEMON:;
		t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));
		recv(socket_cliente, &(caught_pokemon->ok), sizeof(uint32_t), 0);

		paquete->buffer->stream = caught_pokemon;
		break;

	case GET_POKEMON:;
		t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));
		recv(socket_cliente, &(get_pokemon->lengthOfPokemon), sizeof(uint32_t), 0);
		get_pokemon->pokemon = malloc(get_pokemon->lengthOfPokemon);
		recv(socket_cliente, &(get_pokemon->pokemon), get_pokemon->lengthOfPokemon, 0);

		paquete->buffer->stream = get_pokemon;
		break;

	case LOCALIZED_POKEMON:
		//TODO IMPLEMENTACION
		break;

	case RESPUESTA_ID:;
		t_respuesta_id* respuesta_id = malloc(sizeof(t_respuesta_id));
		recv(socket_cliente, &(respuesta_id->idCorrelativo), sizeof(uint32_t), 0);

		paquete->buffer->stream = respuesta_id;
		break;

	case ACK:
		// No se hace nada porque no tiene payload
		break;

	case GAMEBOYSUSCRIBE:;
		t_gameboy_suscribe* gameboy_suscribe = malloc(sizeof(t_gameboy_suscribe));
		recv(socket_cliente, &(gameboy_suscribe->codigoCola), sizeof(op_code), 0);

		paquete->buffer->stream = gameboy_suscribe;
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
//		pruint32_tf("No pude crear el logger\n");
//		exit(1);
//	}
//	return logger;
//}

t_log* iniciar_logger_modulo(char* nombreModulo) {
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
