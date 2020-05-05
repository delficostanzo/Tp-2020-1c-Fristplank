/*
 * Parser.c
 *
 *  Created on: 5 may. 2020
 *      Author: utnso
 */

//esto va a servir como auxiliar para parsear los archivos de configuracion, asi te queda todo mas prolijo

#include "ConfigParser.h"

//con esto, transformas el array de strings que te viene del config, en una t_list*
t_list* crearListaCon(String* strings, int cantidad) {
	t_list* lista = list_create();

	for(int index=0; index<cantidad; index++) {
		String unString = strings[index];

		list_add(lista, unString);
	}

	return lista;
}

//esto convierte un string de tipo "1|2" de posicion, en una t_posicion.
t_posicion* posicionDesde(String stringDePosicion) {
	t_posicion* posicion = newPosicion();

	posicion->x = atoi(string_split(stringDePosicion,"|")[0]);
	posicion->y = atoi(string_split(stringDePosicion,"|")[1]);

	return posicion;
}
