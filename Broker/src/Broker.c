

#include <stdio.h>
#include <stdlib.h>
#include "Broker.h"

int main(void) {
	t_log* logger;
	t_config* config;

	//ejemplo
	t_get_pokemon* getStruct = malloc(sizeof(t_get_pokemon));
	getStruct->pokemon = malloc(sizeof(t_get_pokemon));
	getStruct->pokemon = "Pikachu.txt";

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
	int conexion = crear_conexion(IP_BROKER, PUERTO_BROKER);

	// envio mensaje (el primer parametro esta vacio porque depende de cada estructura de mensaje, va un valor distinto).
	enviar_get_pokemon(getStruct,conexion);
	log_info(logger, "Mensaje serializado enviado"); //TODO: no llega


	// recibir mensaje
	t_paquete* mensaje = recibir_mensaje(conexion); //lo recibimos y la funcion recibir mensaje lo mete en un paquete

	//loguear mensaje recibido
	//log_info(logger, "El mensaje recibido es: %s\n", mensaje);

	terminar_programa(conexion, logger, config);

	free(getStruct->pokemon);
	free(getStruct);
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
	t_log* logger = iniciar_logger();

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

	liberar_conexion(conexion);

}

////////SERVIDOR//////////
//void iniciar_servidor(void)
//{
//	int socket_servidor;
//
//    struct addrinfo hints, *servinfo, *p;
//
//    memset(&hints, 0, sizeof(hints));
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_flags = AI_PASSIVE;
//
//    getaddrinfo(IP_BROKER, PUERTO_BROKER, &hints, &servinfo);
//
//    for (p=servinfo; p != NULL; p = p->ai_next)
//    {
//        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
//            continue;
//
//        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
//            close(socket_servidor);
//            continue;
//        }
//        break;
//    }
//
//	listen(socket_servidor, SOMAXCONN);
//
//    freeaddrinfo(servinfo);
//
//    while(1)
//    	esperar_cliente(socket_servidor);
//}
//
//void esperar_cliente(int socket_servidor)
//{
//	struct sockaddr_in dir_cliente;
//
//	int tam_direccion = sizeof(struct sockaddr_in);
//
//	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);
//
//	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
//	pthread_detach(thread);
//
//}
//
//void serve_client(int* socket)
//{
//	int cod_op;
//	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
//		cod_op = -1;
//	process_request(cod_op, *socket);
//}
//
//void process_request(int cod_op, int cliente_fd) {
//	int size;
//	void* msg;
//		switch (cod_op) {
//		case GET_POKEMON:
//			msg = recibir_mensaje_(cliente_fd, &size);
//			devolver_mensaje(msg, size, cliente_fd);
//			free(msg);
//			break;
//		case 0:
//			pthread_exit(NULL);
//		case -1:
//			pthread_exit(NULL);
//		}
//}
//
//void* recibir_mensaje_(int socket_cliente, int* size)
//{
//	void * buffer;
//
//	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
//	buffer = malloc(*size);
//	recv(socket_cliente, buffer, *size, MSG_WAITALL);
//
//	return buffer;
//}
//
//void* serializar_paquete_(t_paquete* paquete, int bytes)
//{
//	void * magic = malloc(bytes);
//	int desplazamiento = 0;
//
//	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
//	desplazamiento+= sizeof(int);
//	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
//	desplazamiento+= sizeof(int);
//	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
//	desplazamiento+= paquete->buffer->size;
//
//	return magic;
//}
//
//void devolver_mensaje(void* payload, int size, int socket_cliente)
//{
//	t_paquete* paquete = malloc(sizeof(t_paquete));
//
//	paquete->codigo_operacion = GET_POKEMON;
//	paquete->buffer = malloc(sizeof(t_buffer));
//	paquete->buffer->size = size;
//	paquete->buffer->stream = malloc(paquete->buffer->size);
//	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);
//
//	int bytes = paquete->buffer->size + 2*sizeof(int);
//
//	void* a_enviar = serializar_paquete_(paquete, bytes);
//
//	send(socket_cliente, a_enviar, bytes, 0);
//
//	free(a_enviar);
//	free(paquete->buffer->stream);
//	free(paquete->buffer);
//	free(paquete);
//}



