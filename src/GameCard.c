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

	logger = iniciar_log();
	log_info(logger, "Logger iniciado.");

	leer_config();

	int socketBroker = crear_conexion(IP_BROKER, PUERTO_BROKER);

	/* SUSCRIBIRSE A LAS COLAS NEW_POKEMON | CATCH_POKEMON | GET_POKEMON */

	/* Al suscribirse a cada una de las colas deberá quedarse a la espera de recibir un mensaje del Broker. Al recibir un mensaje de cualquier hilo se deberá:
		- Informar al Broker la recepción del mismo (ACK).
		- Crear un hilo que atienda dicha solicitud.
		- Volver a estar a la escucha de nuevos mensajes de la cola de mensajes en cuestión. */

	//aca corro hilo con socket de escucha para el gameboy

//	while (1){
//		// recibir mensaje
//

		int codeOp;
		recv(socketBroker, &codeOp, sizeof(int), MSG_WAITALL);
		log_info(logger, "Op code procesado: %d.", codeOp);

		switch(codeOp){

		case 1:; //new_pokemon
			t_new_pokemon_args *arguments = malloc(sizeof(t_new_pokemon_args));

			//recv(socketBroker, arguments->ID, sizeof(int), MSG_WAITALL) TODAVIA NO ESTA EN LA LIBRERIA

			recv(socketBroker, &arguments->size, sizeof(int), MSG_WAITALL);
			arguments->pokemon = malloc(arguments->size - (2 * sizeof(int)));
			recv(socketBroker, &arguments->pokemon, arguments->size - (2 * sizeof(int)), MSG_WAITALL);
			recv(socketBroker, &arguments->posX, sizeof(int), MSG_WAITALL);
			recv(socketBroker, &arguments->posY, sizeof(int), MSG_WAITALL);
			recv(socketBroker, &arguments->cantidad, sizeof(int), MSG_WAITALL);

			pthread_t nuevoHilo;
			pthread_create(&nuevoHilo, NULL, procesarNewPokemon, &arguments);

			break;

		case 3: //catch_pokemon
			break;

		case 5: //get_pokemon
			break;
		}
//	}

	//terminar_programa(conexion, logger, config);
}

void* procesarNewPokemon(t_new_pokemon_args *arguments){

	char* filePath = PUNTO_MONTAJE_TALLGRASS;
	sprintf(filePath,"/Files/%s/Metadata.bin", arguments->pokemon);

	//	Verificar si el Pokémon existe dentro de nuestro Filesystem. DONE
	if(access(filePath, F_OK) != -1 ) {

	    // El archivo existe. Se prosigue a verificar si está en uso.

		int openFile = 1;

		//	Verificar si se puede abrir el archivo (si no hay otro proceso que lo esté abriendo). DONE
		//	En caso que el archivo se encuentre abierto se deberá finalizar el hilo y reintentar la operación luego de un tiempo definido por configuración. DONE
		while (openFile == 1){

			pthread_t checkOpenFile;
			pthread_create(&checkOpenFile, NULL, checkOpenFile, &filePath);
			pthread_join(&checkOpenFile, &openFile);

			if (openFile == 1){
				sleep(TIEMPO_DE_REINTENTO_OPERACION);
			}
		}

		//	Verificar si las posiciones ya existen dentro del archivo.
		//	En caso de existir se deben agregar la cantidad pasada por parámetro a la actual.
		//	En caso de no existir se debe agregar al final del archivo una nueva línea indicando la cantidad de pokémon pasadas.
		//	Cerrar el archivo.

	} else { //	En caso de no existir se deberá crear.

	}


	//	Conectarse al Broker y enviar el mensaje a la Cola de Mensajes APPEARED_POKEMON con los los datos:
	//	ID del mensaje recibido.
	//	Pokemon.
	//	Posición del mapa.
	//	En caso que no se pueda realizar la conexión con el Broker se debe informar por logs y continuar la ejecución.

	return NULL;
}

void* procesarCatchPokemon(void){
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

void* procesarGetPokemon(void){
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

void leer_config(void){

	config = config_create("./src/gamecard.config");

	TIEMPO_DE_REINTENTO_CONEXION = config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION");
	TIEMPO_DE_REINTENTO_OPERACION = config_get_int_value(config, "TIEMPO_DE_REINTENTO_OPERACION");
	PUNTO_MONTAJE_TALLGRASS = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	PUERTO_BROKER = config_get_string_value(config, "PUERTO_BROKER");

	// faltaria loggear la info de todo el archivo de configuracion, ademas de ip y puerto
	log_info(logger, "Config file | IP del broker: %s", IP_BROKER);
	log_info(logger, "Config file | Puerto del broker: %s",PUERTO_BROKER);
	log_info(logger, "Config file | Punto montaje de TallGrass: %s", PUNTO_MONTAJE_TALLGRASS);
	log_info(logger, "Config file | Tiempo de reintento de conexión al broker: %d", TIEMPO_DE_REINTENTO_CONEXION);
	log_info(logger, "Config file | Tiempo de reintento de operación: %d", TIEMPO_DE_REINTENTO_OPERACION);

}

void* checkOpenFile (char* filePath){

	char* openFile;

	t_config* configFile;
	config_create (filePath);
	IP_BROKER = config_get_string_value(config, "OPEN");
	config_destroy(configFile);

	if (strcmp("Y",openFile) == 0){
		//ARCHIVO ABIERTO
		return 0;
	}
	else{
		//ARCHIVO CERRADO
		return 1;
	}

}
