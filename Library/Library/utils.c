#include "utils.h"

void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	int size_serializado = sizeof(op_code) + sizeof(int) + (paquete->buffer->size); //----

	t_log* logger = iniciar_log();
	log_info(logger, "Inicio de Serializacion");

	void* streamFinal = malloc(size_serializado);
	log_info(logger, "Asignacion de espacio al stream");

	int offset = 0;

	memcpy(streamFinal + offset, &(paquete->codigo_operacion), sizeof(op_code));
	log_info(logger, "Codigo de operacion (protocolo) copiado al stream");
	offset += sizeof(op_code);

	memcpy(streamFinal + offset, &paquete->buffer->size, sizeof(paquete->buffer->size));
	log_info(logger, "Tamaño del mensaje copiado al stream");
	offset += sizeof(paquete->buffer->size);

	memcpy(streamFinal + offset, paquete->buffer->stream, paquete->buffer->size);
	log_info(logger, "Mensaje copiado al stream");

	*bytes = size_serializado;
	log_info(logger, "Insercion del tamaño en bytes al int apuntado");


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
	t_log* logger = iniciar_log();

	int size_serializado;

	void* mensajeAEnviar = malloc(sizeof(paquete->buffer->size) + sizeof(int) + sizeof(op_code));

	mensajeAEnviar = serializar_paquete(paquete, &size_serializado);
	log_info(logger, "Serializacion del paquete");

	send(socket_cliente, mensajeAEnviar, size_serializado, 0);

	free(mensajeAEnviar);
	//free(paquete->buffer);
	//free(paquete);

}

t_paquete* crearPaqueteCon(void* datos, int sizeOfStream, op_code op_code) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->stream = datos;
	paquete->buffer->size = sizeOfStream;
	paquete->codigo_operacion = op_code;

	return paquete;
}

void enviar_new_pokemon(t_new_pokemon* new_pokemon, int socket_cliente) {
	t_paquete* paquete = crearPaqueteCon(new_pokemon, strlen(new_pokemon->pokemon)+1 + sizeof(int)*3, NEW_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_appeared_pokemon(t_appeared_pokemon* appeared_pokemon, int socket_cliente) {
	t_paquete* paquete = crearPaqueteCon(appeared_pokemon, strlen(appeared_pokemon->pokemon)+1 + sizeof(int)*3, APPEARED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_catch_pokemon(t_catch_pokemon* catch_pokemon, int socket_cliente) {
	t_paquete* paquete = crearPaqueteCon((void*) catch_pokemon, strlen(catch_pokemon->pokemon)+1 + sizeof(int)*2, CATCH_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_caught_pokemon(t_caught_pokemon* caught_pokemon, int socket_cliente) {
	t_paquete* paquete = crearPaqueteCon((void*) caught_pokemon, sizeof(int) + sizeof(bool), APPEARED_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_get_pokemon(t_get_pokemon* get_pokemon, int socket_cliente) {
	t_paquete* paquete = crearPaqueteCon((void*) get_pokemon, strlen(get_pokemon->pokemon)+1, GET_POKEMON);
	enviar(paquete, socket_cliente);
}

void enviar_localized_pokemon(t_localized_pokemon* localized_pokemon, int socket_cliente) {
	t_paquete* paquete = crearPaqueteCon((void*) localized_pokemon, sizeof(int) + strlen(localized_pokemon->pokemon)+1 + sizeof(localized_pokemon->listaPosiciones), LOCALIZED_POKEMON);
	enviar(paquete, socket_cliente);
}


t_paquete* recibir_mensaje(int socket_cliente)
{
	t_log* logger = iniciar_log();
	t_paquete* paquete = malloc(sizeof(t_paquete));

	//primero recibimos el codigo de operacion
	//op_code operacion;
	recv(socket_cliente, &(paquete->codigo_operacion), sizeof(op_code), 0);
	log_info(logger, "Recibimos el codigo de operacion");

	//despues recibimos el buffer. Primero su tamaño y despues el contenido
	//int size_datos;
	recv(socket_cliente, &(paquete->buffer->size), sizeof(int), 0);

	//void* datos = malloc(size_datos);
	recv(socket_cliente, paquete->buffer->stream, paquete->buffer->size, 0);
	log_info(logger, "Recibimos el mensaje correspondiente");

	return paquete;
}

//t_new_pokemon* desempaquetar_new_pokemon(t_buffer* paquete->buffer) {
//
//}

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

t_posicion* newPosicion() {
	return malloc(sizeof(t_posicion));
}
