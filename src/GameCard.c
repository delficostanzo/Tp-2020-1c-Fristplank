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

	static pthread_mutex_t semaforoBitarray = PTHREAD_MUTEX_INITIALIZER;

	logger = iniciar_logger();
	log_info(logger, "Logger iniciado.");

	iniciar_filesystem();

	//Lanzar hilo para escuchar a GameBoy
	pthread_t hiloEscuchaGameBoy;
	pthread_create(&hiloEscuchaGameBoy, NULL, (void*) escucharGameBoy, NULL);

	//Lanzar hilo para concetarme a Broker
	pthread_t hiloConexionBroker;
	pthread_create(&hiloConexionBroker, NULL, (void*) generarSocketsConBroker, NULL);
	pthread_detach(hiloConexionBroker);

	//
	/* SUSCRIBIRSE A LAS COLAS NEW_POKEMON | CATCH_POKEMON | GET_POKEMON */

	/* Al suscribirse a cada una de las colas deberá quedarse a la espera de recibir un mensaje del Broker. Al recibir un mensaje de cualquier hilo se deberá:
		- Informar al Broker la recepción del mismo (ACK).
		- Crear un hilo que atienda dicha solicitud.
		- Volver a estar a la escucha de nuevos mensajes de la cola de mensajes en cuestión. */

	//terminar_programa(conexion, logger, config);
	config_destroy(config);
}

void iniciar_filesystem(){
	leer_configuracionGameCard();
	leer_metadata();
	init_bitmap();
	//TODO: Crear carpetas de Files
	//TODO: Crear careptas de Blocks
}


void init_bitmap() {
	log_debug(logger, "<> START: Creacion bitmap <>");

	FILE* bitmapFile = fopen(PATH_BITMAP, "wb");

	int cantidadDeBits = BLOCKS / 8;

	for(int i = 0; i < cantidadDeBits; i++){
		fwrite("0", 1, 1, bitmapFile);
	}

	fclose(bitmapFile);

	void* buffer = malloc(cantidadDeBits);

	bitarray = bitarray_create_with_mode(buffer, cantidadDeBits, LSB_FIRST);

	log_debug(logger, "<> END: Creacion bitmap <>");
}
