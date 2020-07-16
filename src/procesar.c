/*
 * procesar.c
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#include"procesar.h"

void procesarNewPokemon(void* args) {
	log_debug(logger, "<> START: procesarNewPokemon <>");

	t_new_pokemon* new_pokemon = args;
	log_info(logger, "Comenzamos a procesar NEW_POKEMON: Agregamos una cantidad %d en la posicion (%d,%d) del Pokemon %s", new_pokemon->cantidad, new_pokemon->posicion->posicionX, new_pokemon->posicion->posicionY, new_pokemon->pokemon);

	char* filePath = string_new();
	char* finFilePath = string_from_format("%s%s/Metadata.bin", PATH_FILES_POKEMONES, new_pokemon->pokemon);
	string_append(&filePath, finFilePath);

	free(finFilePath);

	log_debug(logger, "Pido archivo para el uso. Ruta: %s", filePath);
	pedirArchivoParaUso(filePath);
	log_debug(logger, "Obtengo archivo para el uso.");

	t_config* metadata = config_create(filePath);
	char** arrayDeBlocks = config_get_array_value(metadata, "BLOCKS");
	int sizeArchivo = config_get_int_value(metadata, "SIZE");
	log_info("El size actual es %d bytes", sizeArchivo);


	char* posicionPlana = string_from_format("%d-%d=",
			new_pokemon->posicion->posicionX, new_pokemon->posicion->posicionY);
	char* posicionPlanaYCantidad = string_from_format("%s%d\n", posicionPlana,
			new_pokemon->cantidad);

	/* HAY BLOQUES
	 */
	if (arrayDeBlocks[0] != NULL) {
		log_debug(logger, "Existen bloques del pokemon.");

		pthread_mutex_lock(&semaforoGetDatos);
		char* contenidoActual = getDatosDeBlocks(arrayDeBlocks, sizeArchivo);
		pthread_mutex_unlock(&semaforoGetDatos);

		char* contenidoNuevo = string_new();

		char** posicionesActuales = string_split(contenidoActual, "\n");

		bool posicionEncontrada = false;
		for (int i = 0; posicionesActuales[i] != NULL; i++) {

			/* ESTA LA POSICION
			 */
			if (string_contains(posicionesActuales[i], posicionPlana)) {
				posicionEncontrada = true;

				char** posicionYCantidad = string_split(posicionesActuales[i],
						"=");

				log_debug(logger, "Se ha encontrado la posicion");
				log_debug(logger, "Cantidad vieja de Pokemons: %d",
						atoi(posicionYCantidad[1]));
				log_debug(logger, "Cantidad nueva de Pokemons: %d",
						atoi(posicionYCantidad[1]) + new_pokemon->cantidad);

				char* punteroACantidadVieja = posicionYCantidad[1];
				char* punteroAViejo = posicionesActuales[i];

				posicionYCantidad[1] = string_itoa(
						atoi(posicionYCantidad[1]) + new_pokemon->cantidad);
				posicionesActuales[i] = string_from_format("%s=%s\n",
						posicionYCantidad[0], posicionYCantidad[1]);

				free(punteroAViejo); //FREE LINEA VIEJA
				free(posicionYCantidad[0]); //FREE POSICION VIEJA
				free(posicionYCantidad[1]);
				free(posicionYCantidad);
				free(punteroACantidadVieja); //FREE CANTIDAD VIEJA

				log_debug(logger, "Linea a guardar: %s", posicionesActuales[i]);
				string_append(&contenidoNuevo, posicionesActuales[i]);
			} else {
				if((string_contains(posicionesActuales[i], "=") && string_contains(posicionesActuales[i], "-"))){
					log_debug(logger, "No encuentro la posicion. Vuelvo a agregar linea como está.");
					char* posicionAAgregar = string_from_format("%s\n", posicionesActuales[i]);
					string_append(&contenidoNuevo, posicionAAgregar);
					free(posicionAAgregar);
				}
				else{
					log_debug(logger, "Linea basura: %s", posicionesActuales[i]);
				}
			}
			free(posicionesActuales[i]);
		}

		free(posicionesActuales);

		/* NO ESTA LA POSICION
		 */
		if (!posicionEncontrada) {
			log_debug(logger, "Posicion no encontrada. Se procede a agregarla.");
			string_append(&contenidoNuevo, posicionPlanaYCantidad);
		}

		int sizeNuevo = strlen(contenidoNuevo);

		log_debug(logger, "Comenzamos a guardar los %d bytes en los bloques.", sizeNuevo);

		/* PASO ARRAY DE BLOQUES A STRING
		 * PARA PODER AGREGAR NUEVOS
		 */
		int cantidadDeBloques = getBlockQuantity(arrayDeBlocks);
		log_debug(logger, "Cantidad de bloques actuales: %d", cantidadDeBloques);

		char* listaDeBloques = string_new();
		string_append(&listaDeBloques, "[");

		for(int i = 0; arrayDeBlocks[i] != NULL; i++) {
			string_append(&listaDeBloques, arrayDeBlocks[i]);
			free(arrayDeBlocks[i]);

			if(arrayDeBlocks[i + 1] != NULL){
				string_append(&listaDeBloques, ",");
			}
		}
		free(arrayDeBlocks);

		log_info(logger, "La lista actual de bloques es %s]", listaDeBloques);

		/* PIDO BLOQUES SI ES NECESARIO
		 */
		int noHayBloquesDisponibles = -1;
		t_list* listaDeBloquesNuevos = list_create();

		while ( (sizeNuevo > cantidadDeBloques * BLOCK_SIZE) && (noHayBloquesDisponibles == -1) ) {
			log_info(logger, "Solicitando un bloque más.");
			int bloque = solicitarBloque();

			if(bloque < 0){
				log_info(logger, "No hay bloques disponibles para guardar la información.");
				noHayBloquesDisponibles = 1;
			}

			else{
				list_add(listaDeBloquesNuevos, bloque);
				char* bloqueNuevo = string_itoa(bloque);
				cantidadDeBloques++;

				string_append(&listaDeBloques, ",");
				string_append(&listaDeBloques, bloqueNuevo);
				free(bloqueNuevo);
			}
		}
		string_append(&listaDeBloques, "]");
		char* bloqueEnConfig = listaDeBloques;

		/* NO TENGO BLOQUES DISPONIBLES
		 * LIBERO LOS QUE YA AGARRE
		 */
		if (noHayBloquesDisponibles == 1){
			int cantBloquesADevolver = list_size(listaDeBloquesNuevos);

			for (int i = 0; i < cantBloquesADevolver; i++){
				int bloqueALiberar = (int) list_get(listaDeBloquesNuevos, i);
				liberarBloque(bloqueALiberar);
				list_remove(listaDeBloquesNuevos, i);
			}
		}

		/* COMPORTAMIENTO NORMAL
		 * HAY BLOQUES DISPONIBLES
		 */
		else{

			int cantBloquesNuevos = list_size(listaDeBloquesNuevos);
			for (int i = 0; i < cantBloquesNuevos; i++){
				list_remove(listaDeBloquesNuevos, i);
			}

			log_info(logger, "La nueva lista de bloques es %s", listaDeBloques);
			/* SETEO LA NUEVA LISTA DE BLOQUES EN LA CONFIG
			 */

			char* sizeEnConfig = string_itoa(sizeNuevo);
			config_set_value(metadata, "SIZE", sizeEnConfig);
			config_set_value(metadata, "BLOCKS", listaDeBloques);
			log_info(logger, "El size nuevo es %s bytes", sizeEnConfig);

			/* TRABAJO CON EL NUEVO ARRAY
			 * PARA GUARDAR LOS DATOS
			 */
			char** nuevoArrayDeBloques = string_get_string_as_array(listaDeBloques);

			pthread_mutex_lock(&semaforoGuardarDatos);
			guardarDatosEnBlocks(contenidoNuevo, nuevoArrayDeBloques);
			pthread_mutex_unlock(&semaforoGuardarDatos);

			log_info(logger, "Los datos se han guardado correctamente.");

			//LIBERO ARRAY DE BLOQUES
			for(int i = 0; nuevoArrayDeBloques[i] != NULL; i++){
				free(nuevoArrayDeBloques[i]);
			}
			free(nuevoArrayDeBloques);
			free(sizeEnConfig);
		}


		list_destroy(listaDeBloquesNuevos);
		free(bloqueEnConfig);
		free(contenidoActual);
		free(contenidoNuevo);
	}

	/* NO HAY BLOQUES
	 */
	else {
		log_info(logger, "No existen bloques del pokemon.");
		log_debug(logger, "Se procede a guardar la linea: %s", posicionPlanaYCantidad);
		int bloqueAEscribir = solicitarBloque();

		if(bloqueAEscribir < 0){
			log_info(logger, "No hay bloques disponibles para guardar la información.");
		}

		else{
			char* path = getPathDeBlock(bloqueAEscribir);

			FILE* bloqueACrear = fopen(path, "wb+");
			fwrite(posicionPlanaYCantidad, strlen(posicionPlanaYCantidad), 1, bloqueACrear);
			fclose(bloqueACrear);
			log_debug(logger, "Se guarda el contenido en el bloque %d",
					bloqueAEscribir);

			char* bloqueEnConfig = string_from_format("[%d]", bloqueAEscribir);
			log_info(logger, "La nueva lista de bloques es %s", bloqueEnConfig);
			config_set_value(metadata, "BLOCKS", bloqueEnConfig);
			log_info(logger, "El size nuevo es %d bytes", strlen(posicionPlanaYCantidad));
			char* sizeEnConfig = string_itoa(strlen(posicionPlanaYCantidad));
			config_set_value(metadata, "SIZE", sizeEnConfig);

			free(bloqueEnConfig);
			free(sizeEnConfig);
			free(path);
		}
	}

	config_save(metadata);
	config_destroy(metadata);
	cambiarACerrado(filePath);
	free(filePath);
	free(posicionPlana);
	free(posicionPlanaYCantidad);

	log_debug(logger, "<> END: procesarNewPokemon <>");
}

void* procesarCatchPokemon(void* args) {

	log_debug(logger, "<> START: procesarCatchPokemon <>");

	t_catch_pokemon* catch_pokemon = args;
	log_info(logger, "Comenzamos a procesar CATCH_POKEMON: Buscamos la posicion (%d,%d) del Pokemon %s",catch_pokemon->posicion->posicionX, catch_pokemon->posicion->posicionY, catch_pokemon->pokemon);

	/* Si el metadata no existe significa que el Pokemon no existe
	 * devolvemos 0 para que se informe que no se pudo atrapar
	 */
	char* filePath = string_new();
	char* finFilePath = string_from_format("%s%s/Metadata.bin", PATH_FILES_POKEMONES, catch_pokemon->pokemon);
	string_append(&filePath, finFilePath);
	free(finFilePath);

	if (!checkArchivoExiste(filePath)) {
		free(filePath);
		return 0;
	}

	log_debug(logger, "Pido archivo para el uso. Ruta: %s", filePath);
	pedirArchivoParaUso(filePath);
	log_debug(logger, "Obtengo archivo para el uso.");

	t_config* metadata = config_create(filePath);
	char** arrayDeBlocks = config_get_array_value(metadata, "BLOCKS");
	int sizeArchivo = config_get_int_value(metadata, "SIZE");
	log_info("El size actual es %d bytes", sizeArchivo);

	//	Verificar si las posiciones ya existen dentro del archivo. En caso de no existir se debe informar un error.
	//	En caso que la cantidad del Pokémon sea “1”, se debe eliminar la línea. En caso contrario se debe decrementar la cantidad en uno.

	char* posicionPlana = string_from_format("%d-%d=",
			catch_pokemon->posicion->posicionX,
			catch_pokemon->posicion->posicionY);

	/* HAY BLOQUES
	 */
	if (arrayDeBlocks[0] != NULL) {
		log_debug(logger, "Existen bloques del pokemon.");

		pthread_mutex_lock(&semaforoGetDatos);
		char* contenidoActual = getDatosDeBlocks(arrayDeBlocks, sizeArchivo);
		pthread_mutex_unlock(&semaforoGetDatos);

		char* contenidoNuevo = string_new();

		char** posicionesActuales = string_split(contenidoActual, "\n");

		bool posicionEncontrada = false;
		for (int i = 0; posicionesActuales[i] != NULL; i++) {

			/* ESTA LA POSICION
			 */
			if (string_contains(posicionesActuales[i], posicionPlana)) {
				posicionEncontrada = true;

				char** posicionYCantidad = string_split(posicionesActuales[i],
						"=");

				log_info(logger, "Se ha encontrado la posicion");
				log_info(logger, "Cantidad vieja de Pokemons: %d",
						atoi(posicionYCantidad[1]));
				log_info(logger, "Cantidad nueva de Pokemons: %d",
						atoi(posicionYCantidad[1]) - 1);

				if (atoi(posicionYCantidad[1]) != 1) {
					//la disminuyo
					char* punteroACantidadVieja = posicionYCantidad[1];
					char* punteroAViejo = posicionesActuales[i];

					posicionYCantidad[1] = string_itoa(
							atoi(posicionYCantidad[1]) - 1);
					posicionesActuales[i] = string_from_format("%s=%s\n",
							posicionYCantidad[0], posicionYCantidad[1]);

					free(punteroAViejo); //FREE LINEA VIEJA
					free(posicionYCantidad[0]); //FREE POSICION VIEJA
					free(punteroACantidadVieja); //FREE CANTIDAD VIEJA

					log_debug(logger, "Linea a guardar: %s",
							posicionesActuales[i]);
					string_append(&contenidoNuevo, posicionesActuales[i]);
				}

				else {
					log_debug(logger,
							"No quedan pokemons en esta posición. Se procede a eliminarla.");
					free(posicionYCantidad[0]);
					free(posicionYCantidad[1]);
					free(posicionesActuales[i]);
				}

				//free(posicionesActuales[i]
			} else {
				if ((string_contains(posicionesActuales[i], "=")
						&& string_contains(posicionesActuales[i], "-"))) {
					log_debug(logger,
							"No encuentro la posicion. Vuelvo a agregar linea como está.");
					char* posicionAAgregar = string_from_format("%s\n",
							posicionesActuales[i]);
					string_append(&contenidoNuevo, posicionAAgregar);
				} else {
					log_debug(logger, "Linea basura: %s",
							posicionesActuales[i]);
				}
				//free(posicionAAgregar);
			}
		}

		/* NO ESTA LA POSICION
		 */
		if (!posicionEncontrada) {
			log_info(logger, "Posicion no encontrada.");
		}

		int sizeNuevo = strlen(contenidoNuevo);
		char* sizeEnConfig = string_itoa(sizeNuevo);
		config_set_value(metadata, "SIZE", sizeEnConfig);
		log_info("El size nuevo es %d bytes", sizeNuevo);

		log_debug(logger, "Comenzamos a guardar los %d bytes en los bloques.", sizeNuevo);

		/* PASO ARRAY DE BLOQUES A STRING
		 * PARA PODER AGREGAR NUEVOS
		 */
		int cantidadDeBloques = getBlockQuantity(arrayDeBlocks);
		log_debug(logger, "Cantidad de bloques actuales: %d",
				cantidadDeBloques);

		/* LIBERO BLOQUES
		 * IF NEEDED
		 */
		int bloquesALiberar = 0;
		while (sizeNuevo < (cantidadDeBloques - 1) * BLOCK_SIZE) {
					log_debug(logger, "Hay bloques sin usar. Libero un bloque.");
					bloquesALiberar++;
					cantidadDeBloques--;
				}

		char* listaDeBloques = string_new();
		string_append(&listaDeBloques, "[");

		for (int i = 0; arrayDeBlocks[i] != NULL; i++) {

			if(bloquesALiberar != 0){ //Si tengo bloques a liberar -> Los libero y no los agrego

				pthread_mutex_lock(&semaforoBitarray);
				liberarBloque(atoi(arrayDeBlocks[i]));
				pthread_mutex_unlock(&semaforoBitarray);

				bloquesALiberar--;
				free(arrayDeBlocks[i]);
			}

			else{ // Si ya no tengo bloques a liberar -> Los agrego
				string_append(&listaDeBloques, arrayDeBlocks[i]);
				if (arrayDeBlocks[i + 1] != NULL) {
					string_append(&listaDeBloques, ",");
					free(arrayDeBlocks[i]);
				}
			}
		}
		free(arrayDeBlocks);

		string_append(&listaDeBloques, "]");

		log_info(logger, "La nueva lista de bloques es %s", listaDeBloques);

		/* SETEO LA NUEVA LISTA DE BLOQUES EN LA CONFIG
		 */
		char* bloqueEnConfig = listaDeBloques;
		config_set_value(metadata, "BLOCKS", bloqueEnConfig);


		/* TRABAJO CON EL NUEVO ARRAY
		 * PARA GUARDAR LOS DATOS
		 */
		char** nuevoArrayDeBloques = string_get_string_as_array(listaDeBloques);

		pthread_mutex_lock(&semaforoGuardarDatos);
		guardarDatosEnBlocks(contenidoNuevo, nuevoArrayDeBloques);
		pthread_mutex_unlock(&semaforoGuardarDatos);

		log_info(logger, "Los datos se han guardado correctamente.");

		//LIBERO ARRAY DE BLOQUES
		for(int i = 0; nuevoArrayDeBloques[i] != NULL; i++){
			free(nuevoArrayDeBloques[i]);
		}

		free(contenidoActual);
		free(contenidoNuevo);

		free(bloqueEnConfig);
		free(sizeEnConfig);

		config_save(metadata);
		config_destroy(metadata);
		cambiarACerrado(filePath);

		free(filePath);

		log_debug(logger, "<> END: procesarCatchPokemon <>");

		if (posicionEncontrada) {
			return 1;
		} else {
			return 0;
		}
	}


	else{
		config_destroy(metadata);
		cambiarACerrado(filePath);
		return 0;
	}


}

void procesarGetPokemon(t_argumentos_procesar_get* args) {

	log_debug(logger, "<> START: procesarGetPokemon <>");

	t_get_pokemon* get_pokemon = args->get_pokemon;

	/* Si el metadata no existe significa que el Pokemon no existe
	 * devolvemos 0 para que se informe que no se pudo atrapar
	 */
	char* filePath = string_new();
	string_append(&filePath,
			string_from_format("%s%s/Metadata.bin", PATH_FILES_POKEMONES,
					get_pokemon->pokemon));

	if (!checkArchivoExiste(filePath)) {
		t_localized_pokemon* localized_pokemon = malloc(sizeof(t_localized_pokemon));
		localized_pokemon->lengthOfPokemon = get_pokemon->lengthOfPokemon;
		localized_pokemon->pokemon = get_pokemon->pokemon;
		localized_pokemon->listaPosiciones = list_create();
		localized_pokemon->cantidadPosiciones = 0;
		args->puntero_a_localized_pokemon = localized_pokemon;

		log_info(logger, "No se encontro el pokemon en el filesystem. Se retorna un localized vacío.");

		free(filePath);
	}

	else{
		log_debug(logger, "Pido archivo para el uso. Ruta: %s", filePath);
		pedirArchivoParaUso(filePath);
		log_debug(logger, "Obtengo archivo para el uso.");

		t_config* metadata = config_create(filePath);
		char** arrayDeBlocks = config_get_array_value(metadata, "BLOCKS");
		int sizeArchivo = config_get_int_value(metadata, "SIZE");

		/* HAY BLOQUES
		 */
		if (arrayDeBlocks[0] != NULL) {
			log_debug(logger, "Existen bloques del pokemon.");

			pthread_mutex_lock(&semaforoGetDatos);
			char* contenidoActual = getDatosDeBlocks(arrayDeBlocks, sizeArchivo);
			pthread_mutex_unlock(&semaforoGetDatos);

			char** posicionesActuales = string_split(contenidoActual, "\n");

			/*INSTANCIO RESPUESTA
			 */
			t_localized_pokemon* localized_pokemon = malloc(sizeof(t_localized_pokemon));
			localized_pokemon->lengthOfPokemon = get_pokemon->lengthOfPokemon;
			localized_pokemon->pokemon = get_pokemon->pokemon;
			localized_pokemon->listaPosiciones = list_create();

			int posicionesEncontradas = 0;

			for (int i = 0; posicionesActuales[i] != NULL; i++) {
				if(string_contains(posicionesActuales[i], "=") && string_contains(posicionesActuales[i], "-")){
					char** posicionesSeparadasDeCantidad = string_split(posicionesActuales[i], "=");
					char** posXposY = string_split(posicionesSeparadasDeCantidad[0], "-");
					log_debug(logger, "Posicion N°%d: (%s,%s)", i + 1, posXposY[0], posXposY[1]);

					t_posicion* posicion = malloc(sizeof(posicion));

					posicion->posicionX = atoi(posXposY[0]);
					posicion->posicionY = atoi(posXposY[1]);

					list_add(localized_pokemon->listaPosiciones, posicion);
					posicionesEncontradas++;

					free(posXposY[0]);
					free(posXposY[1]);
					free(posicionesSeparadasDeCantidad[0]);
					free(posicionesSeparadasDeCantidad[1]);
				}

				free(posicionesActuales[i]);
			}

			localized_pokemon->cantidadPosiciones = posicionesEncontradas;
			log_info(logger, "Se encontraron %d pares de posiciones para el Pokemon %s", localized_pokemon->cantidadPosiciones, localized_pokemon->pokemon);

			for(int i = 0; arrayDeBlocks[i] != NULL; i++){
				free(arrayDeBlocks[i]);
			}
			free(arrayDeBlocks);
			free(contenidoActual);
			config_save(metadata);
			config_destroy(metadata);
			cambiarACerrado(filePath);
			free(filePath);

			args->puntero_a_localized_pokemon = localized_pokemon;
		}

		else{
			t_localized_pokemon* localized_pokemon = malloc(sizeof(t_localized_pokemon));
			localized_pokemon->lengthOfPokemon = get_pokemon->lengthOfPokemon;
			localized_pokemon->pokemon = get_pokemon->pokemon;
			localized_pokemon->listaPosiciones = list_create();
			localized_pokemon->cantidadPosiciones = 0;
			log_info(logger, "No se encontraron bloques asignados al pokemon. Se retorna un localized vacío.");

			free(arrayDeBlocks);

			config_save(metadata);
			config_destroy(metadata);
			cambiarACerrado(filePath);
			free(filePath);

			args->puntero_a_localized_pokemon = localized_pokemon;
		}
	}
	log_debug(logger, "<> END: procesarGetPokemon <>");
}
