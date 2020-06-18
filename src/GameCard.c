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

	// EMPIEZO A TESTEAR
//	int caca = solicitarBloque();
//	log_debug(logger, "Utilizo el bloque %d", caca);
//	int caca2 = solicitarBloque();
//	log_debug(logger, "Utilizo el bloque %d", caca2);
//	int caac3 = solicitarBloque();
//	log_debug(logger, "Utilizo el bloque %d", caac3);
//	int caca4 = solicitarBloque();
//	log_debug(logger, "Utilizo el bloque %d", caca4);
//	liberarBloque(1);
	// TERMINO DE TESTEAR

	//Lanzar hilo para escuchar a GameBoy
	pthread_t hiloEscuchaGameBoy;
	pthread_create(&hiloEscuchaGameBoy, NULL, (void*) escucharGameBoy, NULL);

	//Lanzar hilo para concetarme a Broker
//	pthread_t hiloConexionBroker;
//	pthread_create(&hiloConexionBroker, NULL, (void*) generarSocketsConBroker, NULL);

	//
	/* SUSCRIBIRSE A LAS COLAS NEW_POKEMON | CATCH_POKEMON | GET_POKEMON */

	/* Al suscribirse a cada una de las colas deberá quedarse a la espera de recibir un mensaje del Broker. Al recibir un mensaje de cualquier hilo se deberá:
	 - Informar al Broker la recepción del mismo (ACK).
	 - Crear un hilo que atienda dicha solicitud.
	 - Volver a estar a la escucha de nuevos mensajes de la cola de mensajes en cuestión. */

	//terminar_programa(conexion, logger, config);

	pthread_join(hiloEscuchaGameBoy, NULL);
//	pthread_join(hiloConexionBroker, NULL);

	finalizar_gamecard();
}

void iniciar_filesystem() {
	leer_configuracionGameCard();
	leer_metadata();
	init_semaforos();
	init_bitmap();
	init_estructura_files_blocks();
	init_bloques();
}

void init_semaforos() {
	log_debug(logger, "<> START: Inicializacion semaforos <>");
	pthread_mutex_init(&semaforoOpen, NULL);
	pthread_mutex_init(&semaforoBitarray, NULL);
	log_debug(logger, "<> END: Inicializacion semaforos <>");
}

void init_bitmap() {
	log_debug(logger, "<> START: Creacion bitmap <>");

	FILE* bitmapFile = fopen(PATH_BITMAP, "wb");
	int cantidadDeBits = BLOCKS / 8;

	for (int i = 0; i < cantidadDeBits; i++) {
		fwrite("0", 1, 1, bitmapFile);
	}
	fclose(bitmapFile);

	void* buffer = malloc(cantidadDeBits);
	bitarray = bitarray_create_with_mode(buffer, cantidadDeBits, LSB_FIRST);
	log_debug(logger, "El bitmap creado contiene %d bits",
			bitarray_get_max_bit(bitarray));

	log_debug(logger, "<> END: Creacion bitmap <>");
}

void init_estructura_files_blocks() {
	log_debug(logger, "<> START: Creacion de carpetas blocks y files <>");
	mkdir(PATH_FILES, S_IRWXU);
	mkdir(PATH_BLOCKS, S_IRWXU);
	log_debug(logger, "<> END: Creacion de carpetas blocks y files <>");
}

void init_bloques() {
	log_debug(logger, "<> START: Creacion de archivos de bloques <>");
	for (int i = 0; i < BLOCKS; i++) {

		char* path = string_new();
		string_append(&path, PATH_BLOCKS);

		char* numeroEnString = string_itoa(i);

		string_append(&path, numeroEnString);
		string_append(&path, ".bin");

		FILE* bloqueACrear = fopen(path, "w");
		fclose(bloqueACrear);

		free(numeroEnString);
		free(path);
	}
	log_debug(logger, "<> END: Creacion de archivos de bloques <>");
}

void finalizar_gamecard() {
	log_destroy(logger);
	pthread_mutex_destroy(&semaforoOpen);
	pthread_mutex_destroy(&semaforoBitarray);
	free(PUNTO_MONTAJE_TALLGRASS);
	free(IP_BROKER);
	free(PUERTO_BROKER);
	free(IP_GAMECARD);
	free(PUERTO_GAMECARD);
	free(PATH_BITMAP);
	free(PATH_FILES);
	free(PATH_BLOCKS);

	free(configPath);
	config_destroy(config);
}


