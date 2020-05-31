/*
 * utils.c
 *
 *
 *      Author: fritsplank
 */

#include "utils.h"

void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	int size_serializado = sizeof(op_code) + 2*sizeof(int) + (paquete->buffer->size); //----

	void* streamFinal = malloc(size_serializado);
	int offset = 0;

	memcpy(streamFinal + offset, &(paquete->codigo_operacion), sizeof(op_code));
	offset += sizeof(op_code);
	memcpy(streamFinal + offset, &paquete->buffer->size, sizeof(paquete->buffer->size));
	offset += sizeof(paquete->buffer->size);
	memcpy(streamFinal + offset, paquete->buffer->stream, paquete->buffer->size);

	*bytes = size_serializado;

	return streamFinal;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar(t_paquete* paquete, int socket_cliente)
{

	int size_serializado;

	void* mensajeAEnviar = malloc(sizeof(paquete->buffer->size) + sizeof(int) + sizeof(op_code));

	mensajeAEnviar = serializar_paquete(paquete, &size_serializado);

	send(socket_cliente, mensajeAEnviar, size_serializado, 0);

	free(mensajeAEnviar);
	free(paquete->buffer);
	free(paquete);
}

t_paquete* crearPaqueteCon(void* datos, int sizeOfStream, int Id, int IdCorrelativo, op_code op_code) {

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->ID = Id;
	paquete->ID_CORRELATIVO = IdCorrelativo;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->stream = datos;
	paquete->buffer->size = sizeOfStream;
	paquete->codigo_operacion = op_code;

	return paquete;
}

void enviar_new_pokemon(t_new_pokemon* new_pokemon, int socket_cliente, int Id, int IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon(new_pokemon, new_pokemon->lengthOfPokemon + sizeof(int)*4, Id, IdCorrelativo, NEW_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_appeared_pokemon(t_appeared_pokemon* appeared_pokemon, int socket_cliente, int Id, int IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon(appeared_pokemon, appeared_pokemon->lengthOfPokemon + sizeof(int)*4, Id, IdCorrelativo, APPEARED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_catch_pokemon(t_catch_pokemon* catch_pokemon, int socket_cliente, int Id, int IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon((void*) catch_pokemon, catch_pokemon->lengthOfPokemon + sizeof(int)*3, Id, IdCorrelativo, CATCH_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_caught_pokemon(t_caught_pokemon* caught_pokemon, int socket_cliente, int Id, int IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon((void*) caught_pokemon, sizeof(int) + sizeof(bool), Id, IdCorrelativo, APPEARED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_get_pokemon(t_get_pokemon* get_pokemon, int socket_cliente, int Id, int IdCorrelativo) {
	t_paquete* paquete = crearPaqueteCon((void*) get_pokemon, get_pokemon->lengthOfPokemon + sizeof(int), Id, IdCorrelativo, GET_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_localized_pokemon(t_localized_pokemon* localized_pokemon, int socket_cliente, int Id, int IdCorrelativo) {
	int sizeListaPosiciones = list_size(localized_pokemon->listaPosiciones)*sizeof(t_posicion);
	t_paquete* paquete = crearPaqueteCon((void*) localized_pokemon, localized_pokemon->lengthOfPokemon + sizeof(int)*2 + sizeListaPosiciones, Id, IdCorrelativo, LOCALIZED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_respuesta_id(t_respuesta_id* respuesta_id, int socket_cliente, int Id, int IdCorrelativo){
	t_paquete* paquete = crearPaqueteCon((void*) respuesta_id, sizeof(int), Id, IdCorrelativo, RESPUESTA_ID);
	enviar(paquete, socket_cliente);
}

void enviar_ACK(int socket_cliente, int Id, int IdCorrelativo){
	t_paquete* paquete = crearPaqueteCon(NULL, 0, Id, IdCorrelativo, ACK);
	enviar(paquete, socket_cliente);
}

void enviar_gameboy_suscribe(t_gameboy_suscribe* gameboy_suscribe, int socket_cliente, int Id, int IdCorrelativo){
	t_paquete* paquete = crearPaqueteCon((void*) gameboy_suscribe, sizeof(op_code), Id, IdCorrelativo, GAMEBOYSUSCRIBE);
	enviar(paquete, socket_cliente);
}


t_paquete* recibir_mensaje(int socket_cliente) {

	t_paquete* paquete = malloc(sizeof(t_paquete));

	recv(socket_cliente, &(paquete->codigo_operacion), sizeof(op_code), 0);
	recv(socket_cliente, &(paquete->ID), sizeof(int), 0);
	recv(socket_cliente, &(paquete->ID_CORRELATIVO), sizeof(int), 0);

	paquete->buffer = malloc(sizeof(t_buffer));
	recv(socket_cliente, &(paquete->buffer->size), sizeof(int), 0);

	paquete->buffer->stream = malloc(paquete->buffer->size);
	recv(socket_cliente, &(paquete->buffer->stream), paquete->buffer->size, 0);
//	void* datos = recibirDatos(socket_cliente, paquete->buffer->size);
//	paquete->buffer->stream = datos;
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
