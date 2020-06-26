/*
 * blocks.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include"blocks.h"


int getBlockQuantity(char** arrayDeBlocks){
	int i;
	for(i = 0; arrayDeBlocks[i] != NULL; i++);
	return i;
}

char* getDatosDeBlocks(char** arrayDeBlocks, int size){
	log_debug(logger, "<> START: getDatosDeBlocks | Size del archivo: %d <>", size);
	int blockQuantity = getBlockQuantity(arrayDeBlocks);
	log_debug(logger, "Cantidad de bloques a concatenar: %d", blockQuantity);
	char* contenidoDeArchivo = string_new();
	int sizeRestante = size;

	for(int i = 0; i < blockQuantity; i++){

		char* path = getPathDeBlock(atoi(arrayDeBlocks[i]));
		log_debug(logger, "Abriendo el block %s", path);
		FILE* blockActual = fopen(path, "rb+");

		if(sizeRestante < BLOCK_SIZE){
			char* contenidoDeBloqueActual = malloc(sizeRestante);
			fread(contenidoDeBloqueActual, sizeRestante, 1, blockActual);
			log_debug(logger, "Contenido del bloque actual: %s", contenidoDeBloqueActual);

			string_append(&contenidoDeArchivo, contenidoDeBloqueActual);

			free(contenidoDeBloqueActual);
			fclose(blockActual);
		}
		else{
			log_debug(logger, "sizeRestante > BLOCK_SIZE");
			char* contenidoDeBloqueActual = malloc(BLOCK_SIZE);
			fread(contenidoDeBloqueActual, BLOCK_SIZE, 1, blockActual);
			string_append(&contenidoDeArchivo, contenidoDeBloqueActual);
			sizeRestante -= BLOCK_SIZE;
			free(contenidoDeBloqueActual);
			fclose(blockActual);
		}

		free(path);
	}
	log_debug(logger, "<> END: getDatosDeBlocks <>");

	return contenidoDeArchivo;
}

void guardarDatosEnBlocks(char** arrayDeBlocks, int size){

}

char* getPathDeBlock(int bloque){
	char* path = string_new();
	string_append(&path, PATH_BLOCKS);
	char* numeroEnString = string_itoa(bloque);
	string_append(&path, numeroEnString);
	string_append(&path, ".bin");
	return path;
}
