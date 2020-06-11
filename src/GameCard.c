/*
 ============================================================================
 Name        : GameCard.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : FileSystem de nuestro sistema
 ============================================================================
 */

#include "GameCard.h"

int main(void) {

	logger = iniciar_logger();
	log_info(logger, "Logger iniciado.");

	leer_config();

	//iniciar filesystem

	//Lanzar hilo para concetarme a Broker
	pthread_t hiloConexionBroker;
	pthread_create(&hiloConexionBroker, NULL, (void*) generarSocketsConBroker, NULL);
	pthread_detach(hiloConexionBroker);

	//Lanzar hilo para escuchar a GameBoy
	pthread_t hiloEscuchaGameBoy;
	pthread_create(&hiloEscuchaGameBoy, NULL, (void*) escucharGameBoy, NULL);

	//
	/* SUSCRIBIRSE A LAS COLAS NEW_POKEMON | CATCH_POKEMON | GET_POKEMON */

	/* Al suscribirse a cada una de las colas deberá quedarse a la espera de recibir un mensaje del Broker. Al recibir un mensaje de cualquier hilo se deberá:
		- Informar al Broker la recepción del mismo (ACK).
		- Crear un hilo que atienda dicha solicitud.
		- Volver a estar a la escucha de nuevos mensajes de la cola de mensajes en cuestión. */

	//terminar_programa(conexion, logger, config);
}

int checkingOpenFile(char* filePath){

	t_config* configFile = config_create (filePath);
	char* openFile = config_get_string_value(config, "OPEN");
	config_destroy(configFile);

	if (strcmp("Y",openFile) == 0){
		//ARCHIVO ABIERTO
		free(openFile);
		return 1;
	}
	else{
		//ARCHIVO CERRADO
		free(openFile);
		return 0;
	}
}

void cambiarAAbierto(char* filePath){

	t_config* metadata = config_create(filePath);
	config_set_value(metadata, "OPEN", "Y");
	config_save(metadata);
	config_destroy(metadata);

}

void cambiarACerrado(char* filePath){
	t_config* metadata = config_create(filePath);
	config_set_value(metadata, "OPEN", "N");
	config_save(metadata);
	config_destroy(metadata);
}

void crearArchivo(char* filePath){

	FILE* metadata = fopen (filePath, "wb");

	char* directory = "DIRECTORY=N";
	char* open = "OPEN=Y";

	fwrite (&directory, sizeof (directory), 1, metadata);
	fwrite (&open, sizeof (open), 1, metadata);

	free(directory);
	free(open);
	fclose(metadata);
}
