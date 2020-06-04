/*
 * utils.c
 *
 *
 *      Author: fritsplank
 */

#include "utils.h"

void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	int size_serializado = sizeof(op_code) + sizeof(uint32_t)*2 + sizeof(int) + (paquete->buffer->size);

	t_log* logger = iniciar_log();
	//t_log* logger = iniciar_log("serializarpaquete");
	log_info(logger, "Inicio de Serializacion");

	void* streamFinal = malloc(size_serializado);
	log_info(logger, "Asignacion de espacio al stream");

	int offset = 0;

	memcpy(streamFinal + offset, &(paquete->codigo_operacion), sizeof(op_code));
	log_info(logger, "Codigo de operacion (protocolo) copiado al stream");
	offset += sizeof(op_code);

	memcpy(streamFinal + offset, &(paquete->ID), sizeof(uint32_t));
	log_info(logger, "Id copiado al stream");
	offset += sizeof(uint32_t);

	memcpy(streamFinal + offset, &(paquete->ID_CORRELATIVO), sizeof(uint32_t));
	log_info(logger, "Id correlativo copiado al stream");
	offset += sizeof(uint32_t);

	memcpy(streamFinal + offset, &paquete->buffer->size, sizeof(paquete->buffer->size));
	log_info(logger, "Tamaño del mensaje copiado al stream");
	offset += sizeof(paquete->buffer->size);

	memcpy(streamFinal + offset, paquete->buffer->stream, paquete->buffer->size);
	log_info(logger, "Mensaje copiado al stream");

	*bytes = size_serializado;
	log_info(logger, "Insercion del tamaño en bytes al int apuntado");


	return streamFinal;

}

//int crear_conexion(char *ip, char* puerto)
//{
//	struct addrinfo hints;
//	struct addrinfo *server_info;
//
//	memset(&hints, 0, sizeof(hints));
//	hints.ai_family = AF_UNSPEC;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_flags = AI_PASSIVE;
//
//	getaddrinfo(ip, puerto, &hints, &server_info);
//
//	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
//
//	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
//		printf("error");
//
//	freeaddrinfo(server_info);
//
//	return socket_cliente;
//}

void enviar(t_paquete* paquete, int socket_cliente)
{
	t_log* logger = iniciar_log();
	//t_log* logger = iniciar_log("enviarpaquete");

	int size_serializado;

	void* mensajeAEnviar = malloc(sizeof(paquete->buffer->size) + sizeof(uint32_t)*2 + sizeof(op_code));

	mensajeAEnviar = serializar_paquete(paquete, &size_serializado);
	log_info(logger, "Serializacion del paquete");

	send(socket_cliente, mensajeAEnviar, size_serializado, 0);
	log_info(logger, "Se envio el mensaje");


	free(mensajeAEnviar);
	free(paquete->buffer);
	free(paquete);

}

t_paquete* crearPaqueteCon(void* datos, int sizeOfStream, uint32_t Id, uint32_t IdCorrelativo, op_code op_code) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->ID = Id;
	paquete->ID_CORRELATIVO = IdCorrelativo;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->stream = datos;
	paquete->buffer->size = sizeOfStream;
	paquete->codigo_operacion = op_code;

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


t_paquete* recibir_mensaje(int socket_cliente) {
	t_log* logger = iniciar_log();
	//t_log* logger = iniciar_log("recibirmensaje");
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	//primero recibimos el codigo de operacion
	//op_code operacion;
	recv(socket_cliente, &(paquete->codigo_operacion), sizeof(op_code), 0);
	log_info(logger, "Recibimos el codigo de operacion");

	recv(socket_cliente, &(paquete->ID), sizeof(uint32_t), 0);
	log_info(logger, "Recibimos el id del mensaje");

	recv(socket_cliente, &(paquete->ID_CORRELATIVO), sizeof(uint32_t), 0);
	log_info(logger, "Recibimos el id del mensaje correlativo");

	//despues recibimos el buffer. Primero su tamaño y despues el contenido
	//uint32_t size_datos;
	recv(socket_cliente, &(paquete->buffer->size), sizeof(int), 0);

	//el recv de un stream muy grande puede cortarse
	//void* datos = recibirDatos(socket_cliente, paquete->buffer->size);
	//paquete->buffer->stream = datos;
	log_info(logger, "Recibimos el mensaje correspondiente");

	return paquete;
}

void liberar_conexion(int socket_cliente) {
	t_log* logger = iniciar_log();
	//t_log* logger = iniciar_log("liberarconexion");
	if (close(socket_cliente) == -1) {
		log_error(logger, "Error al cerrar la conexion");
	}
}

t_log* iniciar_log(void) {
	t_log* logger;
	if((logger = log_create("broker.log", "BROKER", 1, log_level_from_string("INFO"))) == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}
	return logger;
}

//t_log* iniciar_log(char* nombreModulo) {
//	t_log* logger;
//
//	char* nombreArchivoLog = malloc(strlen(nombreModulo) + 1);
//	strcpy(nombreArchivoLog, nombreModulo);
//	strcat(nombreArchivoLog, ".log");
//	string_to_upper(nombreModulo);
//
//	if((logger = log_create(nombreArchivoLog, nombreModulo, 1, log_level_from_string("INFO"))) == NULL){
//		printf("No pude crear el logger\n");
//
//		free(nombreArchivoLog);
//		free(nombreModulo);
//		exit(1);
//	}
//
//	free(nombreArchivoLog);
//	free(nombreModulo);
//	return logger;
//}


