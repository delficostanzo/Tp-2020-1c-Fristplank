#include "utils.h"


void serializarSubscribe(t_subscribe* subscribe, void* stream){
	int offset = 0;

	memcpy(stream + offset, &(subscribe->cola), sizeof(op_code));
}

void serializarNewPokemon(t_new_pokemon* new_pokemon, void* stream){
	int offset = 0;

	memcpy(stream + offset, &(new_pokemon->lengthOfPokemon), sizeof(int));
	offset += sizeof(int);

	memcpy(stream + offset, &(new_pokemon->pokemon), new_pokemon->lengthOfPokemon);
	offset += new_pokemon->lengthOfPokemon;

	memcpy(stream + offset, &(new_pokemon->posicionX), sizeof(int));
	offset += sizeof(int);

	memcpy(stream + offset, &(new_pokemon->posicionY), sizeof(int));
	offset += sizeof(int);

	memcpy(stream + offset, &(new_pokemon->cantidad), sizeof(int));
	offset += sizeof(int);
}

void serializarAppearedPokemon(t_appeared_pokemon* appeared_pokemon, void* stream){
	int offset = 0;

	memcpy(stream + offset, &(appeared_pokemon->lengthOfPokemon), sizeof(int));
	offset += sizeof(int);

	memcpy(stream + offset, &(appeared_pokemon->pokemon), appeared_pokemon->lengthOfPokemon);
	offset += appeared_pokemon->lengthOfPokemon;

	memcpy(stream + offset, &(appeared_pokemon->posicionX), sizeof(int));
	offset += sizeof(int);

	memcpy(stream + offset, &(appeared_pokemon->posicionY), sizeof(int));
	offset += sizeof(int);
}

void serializarCatchPokemon(t_catch_pokemon* catch_pokemon, void* stream){
	int offset = 0;

	memcpy(stream + offset, &(catch_pokemon->lengthOfPokemon), sizeof(int));
	offset += sizeof(int);

	memcpy(stream + offset, &(catch_pokemon->pokemon), catch_pokemon->lengthOfPokemon);
	offset += catch_pokemon->lengthOfPokemon;

	memcpy(stream + offset, &(catch_pokemon->posicionX), sizeof(int));
	offset += sizeof(int);

	memcpy(stream + offset, &(catch_pokemon->posicionY), sizeof(int));
	offset += sizeof(int);
}

void serializarCaughtPokemon(t_caught_pokemon* caught_pokemon, void* stream){
	int offset = 0;

	memcpy(stream + offset, &(caught_pokemon->ok), sizeof(bool));
	offset += sizeof(bool);
}

void serializarGetPokemon(t_get_pokemon* get_pokemon, void* stream){
	int offset = 0;

	memcpy(stream + offset, &(get_pokemon->lengthOfPokemon), sizeof(int));
	offset += sizeof(int);

	memcpy(stream + offset, &(get_pokemon->pokemon), get_pokemon->lengthOfPokemon);
	offset += get_pokemon->lengthOfPokemon;
}

// TODO
void* serializarLocalizedPokemon(t_localized_pokemon* localized_pokemon, void* stream){
	return NULL;
}

void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	t_log* logger = iniciar_log();
	log_info(logger, "Inicio de Serializacion");

	//El size sería el opcode + el id + variable size + lo que tenga el mensaje
	int size_serializado = sizeof(op_code) + sizeof(int)*2 + (paquete->buffer->size);
	*bytes = size_serializado;
	void* streamFinal = malloc(size_serializado);

	int offset = 0;

	memcpy(streamFinal + offset, &(paquete->codigo_operacion), sizeof(op_code));
	log_info(logger, "Codigo de operacion (protocolo) copiado al stream");
	offset += sizeof(op_code);

	memcpy(streamFinal + offset, &(paquete->ID), sizeof(int));
	log_info(logger, "ID (protocolo) copiado al stream");
	offset += sizeof(int);

	memcpy(streamFinal + offset, &paquete->buffer->size, sizeof(paquete->buffer->size));
	log_info(logger, "Tamaño del mensaje copiado al stream");
	offset += sizeof(paquete->buffer->size);

	void* mensajeSerializado = malloc(paquete->buffer->size);

	switch (paquete->codigo_operacion){
		case SUBSCRIBE:
			serializarSubscribe(paquete->buffer->stream, &mensajeSerializado);
			break;
		case NEW_POKEMON:
			serializarNewPokemon(paquete->buffer->stream, &mensajeSerializado);
			break;
		case APPEARED_POKEMON:
			serializarAppearedPokemon(paquete->buffer->stream, &mensajeSerializado);
			break;
		case CATCH_POKEMON:
			serializarCatchPokemon(paquete->buffer->stream, &mensajeSerializado);
			break;
		case CAUGHT_POKEMON:
			serializarCaughtPokemon(paquete->buffer->stream, &mensajeSerializado);
			break;
		case GET_POKEMON:
			serializarGetPokemon(paquete->buffer->stream, &mensajeSerializado);
			break;
		case LOCALIZED_POKEMON: //TODO
			serializarLocalizedPokemon(paquete->buffer->stream, &mensajeSerializado); //falta definir struct
			break;
	}

	memcpy(streamFinal + offset, mensajeSerializado, paquete->buffer->size);
	log_info(logger, "Mensaje copiado al stream");

	free(mensajeSerializado);
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
	t_log* logger = iniciar_log();

	void* mensajeAEnviar = serializar_paquete(paquete, &size_serializado);
	log_info(logger, "Serializacion del paquete");

	send(socket_cliente, mensajeAEnviar, size_serializado, 0);

	free(mensajeAEnviar);
	free(paquete->buffer);
	free(paquete);

}

t_paquete* crearPaqueteCon(void* datos, int sizeOfStream, int ID, op_code op_code) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->ID = ID;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->stream = datos;
	paquete->buffer->size = sizeOfStream;
	paquete->codigo_operacion = op_code;

	return paquete;
}

void enviar_subscribe(t_subscribe* subscribe, int socket_cliente, int ID) {
	int size = sizeof(op_code);
	t_paquete* paquete = crearPaqueteCon(subscribe, size, ID, SUBSCRIBE);
	enviar(paquete, socket_cliente);
}

void enviar_new_pokemon(t_new_pokemon* new_pokemon, int socket_cliente, int ID) {
	int size = sizeof(int)*4 + new_pokemon->lengthOfPokemon;
	t_paquete* paquete = crearPaqueteCon(new_pokemon, size, ID, NEW_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_appeared_pokemon(t_appeared_pokemon* appeared_pokemon, int socket_cliente, int ID) {
	int size = sizeof(int)*3 + appeared_pokemon->lengthOfPokemon;
	t_paquete* paquete = crearPaqueteCon(appeared_pokemon, size, ID, APPEARED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_catch_pokemon(t_catch_pokemon* catch_pokemon, int socket_cliente, int ID) {
	int size = sizeof(int)*3 + catch_pokemon->lengthOfPokemon;
	t_paquete* paquete = crearPaqueteCon(catch_pokemon, size, ID, CATCH_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_caught_pokemon(t_caught_pokemon* caught_pokemon, int socket_cliente, int ID) {
	int size = sizeof(bool);
	t_paquete* paquete = crearPaqueteCon(caught_pokemon, size, ID, APPEARED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_get_pokemon(t_get_pokemon* get_pokemon, int socket_cliente, int ID) {
	int size = sizeof(int) + get_pokemon->lengthOfPokemon;
	t_paquete* paquete = crearPaqueteCon(get_pokemon, size, ID, GET_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_localized_pokemon(t_localized_pokemon* localized_pokemon, int socket_cliente, int ID) {
	// no se bien como resolver esto, por ahora lo dejo como estaba
//	int size = sizeof(int)*2 + localized_pokemon->lengthOfPokemon + );
	t_paquete* paquete = crearPaqueteCon(localized_pokemon, sizeof(int) + strlen(localized_pokemon->pokemon)+1 + sizeof(localized_pokemon->listaPosiciones), ID, LOCALIZED_POKEMON);
	enviar(paquete, socket_cliente);
}


void des_serializarSubscribe(int socket, void* stream){
	t_subscribe* subscribe = stream;
	recv(socket, &(subscribe->cola), sizeof(op_code),0);
}

void des_serializarNewPokemon(int socket, void* stream){
	t_new_pokemon* new_pokemon = stream;
	recv(socket, &(new_pokemon->lengthOfPokemon), sizeof(int), 0);
	recv(socket, &(new_pokemon->pokemon), new_pokemon->lengthOfPokemon, 0);
	recv(socket, &(new_pokemon->posicionX), sizeof(int), 0);
	recv(socket, &(new_pokemon->posicionY), sizeof(int), 0);
	recv(socket, &(new_pokemon->cantidad), sizeof(int), 0);
}

void des_serializarAppearedPokemon(int socket, void* stream){
	t_appeared_pokemon* appeared_pokemon = stream;
	recv(socket, &(appeared_pokemon->lengthOfPokemon), sizeof(int), 0);
	recv(socket, &(appeared_pokemon->pokemon), appeared_pokemon->lengthOfPokemon, 0);
	recv(socket, &(appeared_pokemon->posicionX), sizeof(int), 0);
	recv(socket, &(appeared_pokemon->posicionY), sizeof(int), 0);
}

void des_serializarCatchPokemon(int socket, void* stream){
	t_catch_pokemon* catch_pokemon = stream;
	recv(socket, &(catch_pokemon->lengthOfPokemon), sizeof(int), 0);
	recv(socket, &(catch_pokemon->pokemon), catch_pokemon->lengthOfPokemon, 0);
	recv(socket, &(catch_pokemon->posicionX), sizeof(int), 0);
	recv(socket, &(catch_pokemon->posicionY), sizeof(int), 0);
}

void des_serializarCaughtPokemon(int socket, void* stream){
	t_caught_pokemon* caught_pokemon = stream;
	recv(socket, &(caught_pokemon->ok), sizeof(bool), 0);
}

void des_serializarGetPokemon(int socket, void* stream){
	t_get_pokemon* get_pokemon = stream;
	recv(socket, &(get_pokemon->lengthOfPokemon), sizeof(int), 0);
	recv(socket, &(get_pokemon->pokemon), get_pokemon->lengthOfPokemon, 0);
}

void des_serializarLocalizedPokemon(int socket, void* stream){
	//TODO
}

t_paquete* recibir_mensaje(int socket)
{
	t_log* logger = iniciar_log();
	t_paquete* paquete = malloc(sizeof(t_paquete));

	//primero recibimos el codigo de operacion
	//op_code operacion;
	recv(socket, &(paquete->codigo_operacion), sizeof(op_code), 0);
	log_info(logger, "Recibimos el codigo de operacion: %d", paquete->codigo_operacion);

	recv(socket, &(paquete->ID), sizeof(int), 0);
	log_info(logger, "Recibimos el ID del mensaje: %d", paquete->ID);

	//despues recibimos el buffer. Primero su tamaño y despues el contenido
	recv(socket, &(paquete->buffer->size), sizeof(int), 0);

	paquete->buffer->stream = malloc(paquete->buffer->size);

	switch(paquete->codigo_operacion){

		case SUBSCRIBE:
			des_serializarSubscribe(socket, &(paquete->buffer->stream));
			break;
		case NEW_POKEMON:
			des_serializarNewPokemon(socket, &(paquete->buffer->stream));
			break;
		case APPEARED_POKEMON:
			des_serializarAppearedPokemon(socket, &(paquete->buffer->stream));
			break;
		case CATCH_POKEMON:
			des_serializarCatchPokemon(socket, &(paquete->buffer->stream));
			break;
		case CAUGHT_POKEMON:
			des_serializarCaughtPokemon(socket, &(paquete->buffer->stream));
			break;
		case GET_POKEMON:
			des_serializarGetPokemon(socket, &(paquete->buffer->stream));
			break;
		case LOCALIZED_POKEMON:
			// TODO
			des_serializarLocalizedPokemon(socket, &(paquete->buffer->stream));
			break;

	}

	log_info(logger, "Recibimos el mensaje correspondiente");
	return paquete;
}

void liberar_conexion(int socket_cliente)
{
	t_log* logger = iniciar_log();
	if (close(socket_cliente) == -1) {
		log_error(logger, "Error al cerrar la conexion");
	}
}

t_log* iniciar_log(void)
{
	t_log* logger;
	if((logger = log_create("broker.log", "BROKER", 1, log_level_from_string("INFO"))) == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}
	return logger;
}
