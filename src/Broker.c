/*
 ============================================================================
 Name        : Broker.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Broker.h"
//#include "utils.h" // esto lo agrego igual aunque la biblio no este lista todavia


int main(void) {
	// inicializo el log del Broker
	logger = iniciar_logger();

	// creo y devuelvo un puntero a la estructura t_config
	config = leer_config();

	TAMANO_MEMORIA = config_get_int_value(config, "TAMANO_MEMORIA");
	TAMANO_MINIMO_PARTICION = config_get_int_value(config, "TAMANO_MINIMO_PARTICION");
	ALGORITMO_MEMORIA = config_get_string_value(config, "ALGORITMO_MEMORIA");
	ALGORITMO_REEMPLAZO = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
	ALGORITMO_PARTICION_LIBRE = config_get_string_value(config,"ALGORITMO_PARTICION_LIBRE");
	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	PUERTO_BROKER = config_get_string_value(config, "PUERTO_BROKER");
	FRECUENCIA_COMPACTACION = config_get_int_value(config, "FRECUENCIA_COMPACTACION");
	LOG_FILE =  config_get_string_value(config,"LOG_FILE");

	// faltaria loggear la info de todo el archivo de configuracion, ademas de ip y puerto
	log_info(logger, "Lei la IP %s y PUERTO %s\n", IP_BROKER, PUERTO_BROKER);

	//crear conexion, un socket conectado
	int socket_cliente = crear_conexion(IP_BROKER, PUERTO_BROKER);

	// envio mensaje (el primer parametro esta vacio porque depende de cada estructura de mensaje, va un valor distinto). No lo completo porque vicky no commiteo al master
	//enviar(pack,socket_cliente);


	// recibir mensaje
	//t_paquete* mensaje = recibir_mensaje(conexion); //lo recibimos y la funcion recibir mensaje lo mete en un paquete

	//loguear mensaje recibido

	//log_info(logger, "El mensaje recibido es: %s\n", mensaje);

	//terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void){
	t_log * log = malloc(sizeof(t_log));
	log = log_create("broker.log", "BROKER", 1,0);
	if(log == NULL){
			printf("No pude crear el logger \n");
			exit(1);
	}
	log_info(log,"Logger Iniciado");
	return log;
}
t_config* leer_config()
{
	t_config* config = config_create("src/broker.config");

	if(config == NULL){
		printf("No pude leer la config \n");
		exit(2);
	}
	log_info(logger,"Archivo de configuracion seteado");
	return config;
}


void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger != NULL){
		log_destroy(logger);
	}

	if(config != NULL){
		config_destroy(config); //destruye la esctructura de config en memoria, no lo esta eliminando el archivo de config
	}

	//liberar_conexion(conexion); // esta funcion esta en utils.c

}

//>>>>>>>>>>>>>>>>>>>>>>>>>NO VA

void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	int size_serializado = sizeof(op_code) + sizeof(int) + (paquete->buffer->size); //----

	t_log* logger = iniciar_logger();
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
	log_info(logger,"Creo un socket para conectarme a un servidor");

	return socket_cliente;
}

void enviar(t_paquete* paquete, int socket_cliente)
{
	t_log* logger = iniciar_logger();

	int size_serializado;

	void* mensajeAEnviar = malloc(sizeof(paquete->buffer->size) + sizeof(int) + sizeof(op_code));

	mensajeAEnviar = serializar_paquete(paquete, &size_serializado);
	log_info(logger, "Serializacion del paquete");

	send(socket_cliente, mensajeAEnviar, size_serializado, 0);

	free(mensajeAEnviar);
	//free(paquete->buffer);
	//free(paquete);

}

t_paquete * crearPaqueteCon(void*datos, int* sizeOfStream, op_code op_code) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->stream = datos;
	paquete->buffer->size = *sizeOfStream;
	paquete->codigo_operacion = op_code;

	return paquete;
}

/*t_paquete * recibir_mensaje(int socket_cliente)
{
	t_log* logger = iniciar_logger();
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

	//Ahora en funcion del codigo recibido procedemos a deserealizar el resto
//	switch(paquete->codigo_operacion) {
//		case NEW_POKEMON:
//			t_new_pokemon* new_pokemon = deserializar_new_pokemon(paquete->buffer);
//
//	}

	return paquete;
}

//t_new_pokemon* desempaquetar_new_pokemon(t_buffer* paquete->buffer) {
//
//}

void liberar_conexion(int socket_cliente)
{
	t_log* logger = iniciar_logger();
	if (close(socket_cliente) == -1) {
		log_error(logger, "Error al cerrar la conexion");
	}
}

//<<<<<<<<<<*/
