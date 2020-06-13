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
	init_bitmap(); //TODO
	//TODO: Crear carpetas de Files
	//TODO: Crear careptas de Blocks
}


void init_bitmap() {
//	log_debug(logger, "init_bitmap: void");
//
//	FILE* bitmap_f = fopen(string_from_format("%s/Metadata/Bitmap.bin", PUNTO_MONTAJE_TALLGRASS), "r");
//
//	int size;
//	char* buffer;
//	fseek(bitmap_f, 0L, SEEK_END);
//	size = ftell(bitmap_f);
//	fseek(bitmap_f, 0L, SEEK_SET);
//
//	buffer = malloc(size);
//	fread(buffer, size, 1, bitmap_f);
//	buffer = string_substring_until(buffer, size);
//
//	//string_append(&buffer, string_repeat('\0', block_quantity / 8 - size));
//	bitmap = bitarray_create(buffer, block_quantity / 8);
//
//	//free(buffer);
//	fclose(bitmap_f);
//
//	log_debug(logger, "init_bitmap: void");
}
