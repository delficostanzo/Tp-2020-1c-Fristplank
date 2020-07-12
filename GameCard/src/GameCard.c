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

	pthread_t hiloEscuchaGameBoy;
	pthread_create(&hiloEscuchaGameBoy, NULL, (void*) escucharGameBoy, NULL);

	int conectado = generarSocketsConBroker();
	while(conectado == -1){
		conectado = generarSocketsConBroker();
	}

	log_info(logger, "Se logró conectar con Broker. Comenzando a lanzar hilos de escucha.");
	pthread_t hiloEscuchaSockets;
	pthread_create(&hiloEscuchaSockets, NULL, (void*) lanzarHilosDeEscucha, NULL);

	pthread_join(hiloEscuchaGameBoy, NULL);
	pthread_join(hiloEscuchaSockets, NULL);

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
	pthread_mutex_init(&semaforoGetDatos, NULL);
	pthread_mutex_init(&semaforoGuardarDatos, NULL);
	pthread_mutex_init(&semaforoDesconexion, NULL);
	log_debug(logger, "<> END: Inicializacion semaforos <>");
}

void init_bitmap() {
	log_debug(logger, "<> START: Creacion bitmap <>");

	FILE* bitmapFile = fopen(PATH_BITMAP, "w+");
	int cantidadDeBits = BLOCKS / 8;
	log_debug(logger, "Archivo Bitmap.bin creado.");

	for (int i = 0; i < BLOCKS; i++) {
		fwrite("0", 1, 1, bitmapFile);
	}
	fclose(bitmapFile);
	log_debug(logger, "Archivo Bitmap.bin escrito.");

	bufferBitarray = malloc(cantidadDeBits);
	bitarray = bitarray_create_with_mode(bufferBitarray, cantidadDeBits, LSB_FIRST);
	log_debug(logger, "El bitmap creado contiene %d bits",
			bitarray_get_max_bit(bitarray));

	for(int i = 0; i < BLOCKS; i++){
		bitarray_clean_bit(bitarray, i);
	}

	log_debug(logger, "<> END: Creacion bitmap <>");
}

void init_estructura_files_blocks() {
	log_debug(logger, "<> START: Creacion de carpetas blocks y files <>");
	mkdir(PATH_FILES, S_IRWXU);
	mkdir(PATH_BLOCKS, S_IRWXU);
	mkdir(PATH_FILES_POKEMONES, S_IRWXU);
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

		FILE* bloqueACrear = fopen(path, "wb");
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
	pthread_mutex_destroy(&semaforoGetDatos);
	pthread_mutex_destroy(&semaforoGuardarDatos);
	pthread_mutex_destroy(&semaforoDesconexion);

	free(PUNTO_MONTAJE_TALLGRASS);
	free(IP_BROKER);
	free(PUERTO_BROKER);
	free(IP_GAMECARD);
	free(PUERTO_GAMECARD);
	free(PATH_BITMAP);
	free(PATH_FILES);
	free(PATH_FILES_POKEMONES);
	free(PATH_BLOCKS);

	free(configPath);
	config_destroy(config);

	free(bufferBitarray);
	bitarray_destroy(bitarray);
}

