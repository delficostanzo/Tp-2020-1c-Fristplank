/*
 * Mapa.h
 *
 *  Created on: 25 may. 2020
 *      Author: fritsplank
 */

#ifndef SRC_MODEL_MAPA_H_
#define SRC_MODEL_MAPA_H_

#include "Casillero.h"

typedef struct {
	int cantidadFilas;
	int cantidadColumnas;
	Casillero*** matriz; //plano de filas y columnas -> matriz[fila][columna] = cada casillero
}Mapa;

Mapa* nuevoMapaConLimites(t_posicion* limitePosicion);
void agregarPokemonAlMapa(Mapa* mapa, t_pokemon* pokemon);
void agregarEntrenadorPrincipalAlMapa(Mapa* mapa, Entrenador* entrenador);
void agregarEntrenadorIntercambioAlMapa(Mapa* mapa, Entrenador* entrenador);
//para atrapar un pokemon
void moverEntrenadorPrincipalALaPosicion(Mapa* mapa, Entrenador* entrenador, t_posicion* nuevaPosicion);
//para hacer el intercambio de pokemones en un DEADLOCK
void moverEntrenadorIntercambioALaPosicion(Mapa* mapa, Entrenador* entrenador, t_posicion* nuevaPosicion);

#endif /* SRC_MODEL_MAPA_H_ */
