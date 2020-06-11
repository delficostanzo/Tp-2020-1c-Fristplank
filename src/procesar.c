/*
 * procesar.c
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#include"procesar.h"

void* procesarNewPokemon(void*args){

//	t_new_pokemon_args* arguments = (t_new_pokemon_args *) args;

	char* filePath = PUNTO_MONTAJE_TALLGRASS;
//	sprintf(filePath,"/Files/Pokemones/%s/Metadata.bin", arguments->pokemon);

	//	Verificar si el Pokémon existe dentro de nuestro Filesystem. DONE
	if(access(filePath, F_OK) != -1 ) { // El archivo existe. Se prosigue a verificar si está en uso. DONE

		//	Verificar si se puede abrir el archivo (si no hay otro proceso que lo esté abriendo). DONE
		//	En caso que el archivo se encuentre abierto se deberá finalizar el hilo y reintentar la operación luego de un tiempo definido por configuración. DONE

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
	}

	//	Verificar si las posiciones ya existen dentro del archivo.
	//	En caso de existir se deben agregar la cantidad pasada por parámetro a la actual.
	//	En caso de no existir se debe agregar al final del archivo una nueva línea indicando la cantidad de pokémon pasadas.

	//	Cerrar el archivo -> OPEN=N. DONE
	cambiarACerrado(filePath);

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
	return NULL;
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
	return NULL;
}
