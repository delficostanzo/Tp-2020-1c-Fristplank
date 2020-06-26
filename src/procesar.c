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

	char* filePath = string_new();
	string_append(&filePath,
			string_from_format("%s%s/Metadata.bin", PATH_FILES_POKEMONES,
					new_pokemon->pokemon));

	log_debug(logger, "Pido archivo para el uso. Ruta: %s", filePath);
	pedirArchivoParaUso(filePath);
	log_debug(logger, "Obtengo archivo para el uso."); //TODO QUEDE ACA

	t_config* metadata = config_create(filePath);
	char** arrayDeBlocks = config_get_array_value(metadata, "BLOCKS");
	int sizeArchivo = config_get_int_value(metadata, "SIZE");

	char* posicionPlana = string_from_format("%d-%d=",
			new_pokemon->posicion->posicionX, new_pokemon->posicion->posicionY);
	char* posicionPlanaYCantidad = string_from_format("%s%d\n", posicionPlana,
			new_pokemon->cantidad);

	/* HAY BLOQUES
	 */
	if (arrayDeBlocks[0] != NULL) {
		log_debug(logger, "Existen bloques del pokemon.");
		char* contenidoActual = getDatosDeBlocks(arrayDeBlocks, sizeArchivo);
		char* contenidoNuevo = string_new();

		char** posicionesActuales = string_split(contenidoActual, "\n");

		bool posicionEncontrada;
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
				free(punteroACantidadVieja); //FREE CANTIDAD VIEJA

				log_debug(logger, "Linea a guardar: %s", posicionesActuales[i]);
				string_append(&contenidoNuevo, posicionesActuales[i]);
				//free(posicionesActuales[i]
			} else {
				char* posicionAAgregar = string_from_format("%s\n",
						posicionesActuales[i]);
				string_append(&contenidoNuevo, posicionAAgregar);
				//free(posicionAAgregar);
			}
		}

		/* NO ESTA LA POSICION
		 */
		if (!posicionEncontrada) {
			string_append(&contenidoNuevo, posicionPlanaYCantidad);
		}

		int sizeNuevo = strlen(contenidoNuevo);
		char* sizeEnConfig = string_itoa(sizeNuevo);
		config_set_value(metadata, "SIZE", sizeEnConfig);

		////////////////////// COMIENZA GUARDADO DE DATOS EN BLOQUES

		log_debug(logger, "Comenzamos a guardar los %d bytes en los bloques.", sizeNuevo);
		//TODO LLEGA HASTA ACA CUANDO CARGO DOS COSAS DEL MISMO POKEMON 26/06

		/* PASO ARRAY DE BLOQUES A STRING
		 * PARA PODER AGREGAR NUEVOS
		 */
		int cantidadDeBloques = getBlockQuantity(arrayDeBlocks);
		char* listaDeBloques = string_new();
		string_append(&listaDeBloques, "[");
		int y = 0;
		while (arrayDeBlocks[y] != NULL) {
			string_append(&listaDeBloques, arrayDeBlocks[y]);
//			free(arrayDeBlocks[y]);
		}
		log_debug(logger, "La lista actual de bloques es %s]", listaDeBloques);

		/* PIDO BLOQUES SI ES NECESARIO
		 */
		while (sizeNuevo > cantidadDeBloques * BLOCK_SIZE) {
			log_debug(logger, "Solicitando un bloque más.");
			int bloque = solicitarBloque();
			char* bloqueNuevo = string_itoa(bloque);
			cantidadDeBloques++;

			string_append(&listaDeBloques, bloqueNuevo);
			free(bloqueNuevo);
		}
		string_append(&listaDeBloques, "]");
		log_debug(logger, "La nueva lista de bloques es %s", listaDeBloques);

		/* SETEO LA NUEVA LISTA DE BLOQUES EN LA CONFIG
		 */
		char* bloqueEnConfig = listaDeBloques;
		config_set_value(metadata, "BLOCKS", bloqueEnConfig);

		/* TRABAJO CON EL NUEVO ARRAY
		 * PARA GUARDAR LOS DATOS
		 */
		char** nuevoArrayDeBloques = string_get_string_as_array(listaDeBloques);
		int bytesGuardados = 0;
		for(int i = 0; i < cantidadDeBloques; i++){
			char* datosAGuardar = string_substring(contenidoNuevo, bytesGuardados, bytesGuardados + BLOCK_SIZE - 1);
			char* path = getPathDeBlock(nuevoArrayDeBloques[i]);

			FILE* bloqueACrear = fopen(path, "rb+");
			fwrite(datosAGuardar, strlen(datosAGuardar), 1, bloqueACrear);
			fclose(bloqueACrear);

			free(path);

			bytesGuardados += BLOCK_SIZE;
		}
		log_debug(logger, "Los datos se han guardado correctamente.");

		//LIBERO ARRAY DE BLOQUES
		for(int i = 0; nuevoArrayDeBloques[i] != NULL; i++){
			free(nuevoArrayDeBloques[i]);
		}

		//TODO FREE POSICIONESACTUALES
		free(contenidoActual);
		free(contenidoNuevo);

		config_set_value(metadata, "OPEN", "N");
	}

	/* NO HAY BLOQUES
	 */
	else {
		log_debug(logger, "No existen bloques del pokemon.");
		int bloqueAEscribir = solicitarBloque();
		char* path = getPathDeBlock(bloqueAEscribir);

		FILE* bloqueACrear = fopen(path, "rb+");
		fwrite(posicionPlanaYCantidad, strlen(posicionPlanaYCantidad), 1,
				bloqueACrear);
		fclose(bloqueACrear);
		log_debug(logger, "Se guarda el contenido en el bloque %d",
				bloqueAEscribir);

		char* bloqueEnConfig = string_from_format("[%d]", bloqueAEscribir);
		config_set_value(metadata, "BLOCKS", bloqueEnConfig);
		char* sizeEnConfig = string_itoa(strlen(posicionPlanaYCantidad));
		config_set_value(metadata, "SIZE", sizeEnConfig);
		config_set_value(metadata, "OPEN", "N");

		free(bloqueEnConfig);
		free(sizeEnConfig);
		free(path);
	}

	config_save(metadata);
	config_destroy(metadata);
	cambiarACerrado(filePath);
	free(filePath);
	free(posicionPlana);
	free(posicionPlanaYCantidad);

	log_debug(logger, "<> END: procesarNewPokemon <>");
}

void procesarCatchPokemon(void) {
//	Este mensaje cumplirá la función de indicar si es posible capturar un Pokemon y capturarlo en tal caso. Para esto se recibirán los siguientes parámetros:
//	ID del mensaje recibido.
//	Pokemon a atrapar.
//	Posición del mapa.
//	Al recibir este mensaje se deberán realizar las siguientes operaciones:
//	Verificar si el Pokémon existe dentro de nuestro Filesystem. Para esto se deberá buscar dentro del directorio Pokemon si existe el archivo con el nombre de nuestro pokémon. En caso de no existir se deberá informar un error.
//	Verificar si se puede abrir el archivo (si no hay otro proceso que lo esté abriendo). En caso que el archivo se encuentre abierto se deberá finalizar el hilo y reintentar la operación luego de un tiempo definido por configuración.
//	Verificar si las posiciones ya existen dentro del archivo. En caso de no existir se debe informar un error.
//	En caso que la cantidad del Pokémon sea “1”, se debe eliminar la línea. En caso contrario se debe decrementar la cantidad en uno.
//	Cerrar el archivo.
//	Conectarse al Broker y enviar el mensaje indicando el resultado correcto.
//	Todo resultado, sea correcto o no, deberá realizarse conectandose al Broker y enviando un mensaje a la Cola de Mensajes CAUGHT_POKEMON indicando:
//	ID del mensaje recibido originalmente.
//	Resultado.
//	En caso que no se pueda realizar la conexión con el Broker se debe informar por logs y continuar la ejecución.
}

void procesarGetPokemon(void) {
//	Este mensaje cumplirá la función de obtener todas las posiciones y su cantidad de un Pokémon específico. Para esto recibirá:
//	El identificador del mensaje recibido.
//	Pokémon a devolver.
//	Al recibir este mensaje se deberán realizar las siguientes operaciones:
//	Verificar si el Pokémon existe dentro de nuestro Filesystem. Para esto se deberá buscar dentro del directorio Pokemon si existe el archivo con el nombre de nuestro pokémon. En caso de no existir se deberá informar el mensaje sin posiciones ni cantidades.
//	Verificar si se puede abrir el archivo (si no hay otro proceso que lo esté abriendo). En caso que el archivo se encuentre abierto se deberá finalizar el hilo y reintentar la operación luego de un tiempo definido por configuración.
//	Obtener todas las posiciones y cantidades de Pokemon requerido.
//	Cerrar el archivo.
//	Conectarse al Broker y enviar el mensaje con todas las posiciones y su cantidad.
//	En caso que se encuentre por lo menos una posición para el Pokémon solicitado se deberá enviar un mensaje al Broker a la Cola de Mensajes LOCALIZED_POKEMON indicando:
//	ID del mensaje recibido originalmente.
//	El Pokémon solicitado.
//	La lista de posiciones y la cantidad de cada una de ellas en el mapa.
//	En caso que no se pueda realizar la conexión con el Broker se debe informar por logs y continuar la ejecución.
}
