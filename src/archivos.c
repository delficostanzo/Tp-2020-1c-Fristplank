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
	char* open = "OPEN=Y";

	fwrite (&directory, sizeof (directory), 1, metadata);
	fwrite (&open, sizeof (open), 1, metadata);

	free(directory);
	free(open);
	fclose(metadata);
}
