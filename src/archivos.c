/*
 * archivos.c
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#include"archivos.h"

int checkingOpenFile(char* filePath){

	t_config* configFile = config_create (filePath);
	char* openFile = config_get_string_value(configFile, "OPEN");
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
	char* size = "SIZE=0";
	char* blocks = "BLOCKS=";
	char* open = "OPEN=Y";

	fwrite (&directory, sizeof (directory), 1, metadata);
	fwrite (&size, sizeof (size), 1, metadata);
	fwrite (&blocks, sizeof (blocks), 1, metadata);
	fwrite (&open, sizeof (open), 1, metadata);

	free(directory);
	free(open);
	fclose(metadata);
}

void pedirArchivoParaUso(char* filePath){

	if(access(filePath, F_OK) != -1 ) { // El archivo existe. Se prosigue a verificar si está en uso. DONE
		int abierto = 1;

		while (abierto){
			pthread_mutex_lock(&semaforoOpen); // Impido que dos hilos accedan. DONE
			abierto = checkingOpenFile(filePath);
			if (!abierto){ // Está cerrado -> abro y le cambio el OPEN = Y. DONE
				cambiarAAbierto(filePath);
			}
			pthread_mutex_unlock(&semaforoOpen);
			if (abierto){ //Si está abierto que el hilo duerma. DONE
				sleep(TIEMPO_DE_REINTENTO_OPERACION);
			}
		}
	}

	else { //	En caso de no existir se deberá crear. DONE

		crearArchivo(filePath);
		/*Ejemplo archivo:
			DIRECTORY=N -> YA SETEADO
			SIZE=250
			BLOCKS=[40,21,82,3]
			OPEN=Y -> YA SETEADO
		 *
		 */
	}
}
