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
	int blockQuantity = getBlockQuantity(arrayDeBlocks);
	char* contenidoDeArchivo = string_new();
	int sizeRestante = size;

	for(int i = 0; i < blockQuantity; i++){

		FILE* blockActual = fopen(string_from_format("%s%s.bin", PATH_BLOCKS, arrayDeBlocks[i]), "r+w");

		if(sizeRestante < BLOCK_SIZE){
			char* contenidoDeBloqueActual = malloc(sizeRestante);
			fread(&contenidoDeBloqueActual, 1, size, blockActual);
			string_append(contenidoDeArchivo, contenidoDeBloqueActual);
			free(contenidoDeBloqueActual);
			fclose(blockActual);
		}
		else{
			char* contenidoDeBloqueActual = malloc(BLOCK_SIZE);
			fread(&contenidoDeBloqueActual, 1, BLOCK_SIZE, blockActual);
			string_append(contenidoDeArchivo, contenidoDeBloqueActual);
			sizeRestante -= BLOCK_SIZE;
			free(contenidoDeBloqueActual);
			fclose(blockActual);
		}
	}
	return contenidoDeArchivo;
}
