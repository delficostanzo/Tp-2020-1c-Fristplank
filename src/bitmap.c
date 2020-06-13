/*
 * bitmap.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include"bitmap.h"

int solicitarBloque() {
	log_debug(logger, "<> START: Solicitar bloque <>");
	pthread_mutex_lock(&semaforoBitarray);

	int bloqueEncontrado = -1;
	int cantidadBloques = bitarray_get_max_bit(bitarray);

	for (int i = 0; i < cantidadBloques; i++) {
		if (!bitarray_test_bit(bitarray, i)) {
			bitarray_set_bit(bitarray, i);
			bloqueEncontrado = i;
			break;
		}
	}

	log_debug(logger, "Bloque libre encontrado: %d", bloqueEncontrado);

	FILE* bitmap = fopen(PATH_BITMAP, "r+w");
	fseek(bitmap, bloqueEncontrado, SEEK_SET);
	fwrite("1", 1, 1, bitmap);
	fclose(bitmap);

	pthread_mutex_unlock(&semaforoBitarray);
	log_debug(logger, "<> END: Solicitar bloque <>");
	return bloqueEncontrado;
}

void liberarBloque(int bloqueALiberar){
	log_debug(logger, "<> START: Liberar bloque <>");
	pthread_mutex_lock(&semaforoBitarray);

	bitarray_clean_bit(bitarray, bloqueALiberar);

	FILE* bitmap = fopen(PATH_BITMAP, "r+w");
	fseek(bitmap, bloqueALiberar, SEEK_SET);
	fwrite("0", 1, 1, bitmap);
	fclose(bitmap);

	pthread_mutex_unlock(&semaforoBitarray);
	log_debug(logger, "<> END: Liberar bloque <>");
}
