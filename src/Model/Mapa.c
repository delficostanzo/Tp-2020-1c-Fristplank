
#include "Mapa.h"
#include "utils.h"
#include "Trainer.h"
#include "Pokemon.h"

static Casillero* casilleroParaLaPosicion(Mapa* mapa, t_posicion* posicion);

//link: https://stackoverflow.com/questions/14450123/c-pointer-to-a-matrix
Mapa* nuevoMapaConLimites(t_posicion* limitePosicion) {

	Mapa* mapa = malloc(sizeof(Mapa));

	mapa->cantidadFilas = limitePosicion->x;
	mapa->cantidadColumnas = limitePosicion->y;
	mapa->matriz = malloc(sizeof(Casillero) * mapa->cantidadFilas * mapa->cantidadColumnas);

	for(int fila = 0; fila < mapa->cantidadFilas; fila++) {
		for(int columna = 0; columna < mapa->cantidadColumnas; columna++) {
			//mapa->matriz[fila * mapa->cantidadColumnas + columna] = newCasillero();
			mapa->matriz[fila][columna] = newCasillero();
		}
	}

	return mapa;
}

Casillero* casilleroParaLaPosicion(Mapa* mapa, t_posicion* posicion) {
	return mapa->matriz[posicion->x][posicion->y];
}

void agregarPokemonAlMapa(Mapa* mapa, t_pokemon* pokemon) {
	Casillero* casillero = casilleroParaLaPosicion(mapa, pokemon->posicion);
	agregarPokemonAlCasillero(casillero, pokemon);
}

void agregarEntrenadorPrincipalAlMapa(Mapa* mapa, Entrenador* entrenador) {
	Casillero* casillero = casilleroParaLaPosicion(mapa, entrenador->posicion);
	agregarEntrenadorPrincipalAlCasillero(casillero, entrenador);
}

//solo cuando hay DEADLOCK
void agregarEntrenadorIntercambioAlMapa(Mapa* mapa, Entrenador* entrenador) {
	Casillero* casillero = casilleroParaLaPosicion(mapa, entrenador->posicion);
	agregarEntrenadorIntercambioAlCasillero(casillero, entrenador);
}

void borrarEntrenadorPrincipalDe(Mapa* mapa, t_posicion* posicion) {
	Casillero* casillero = casilleroParaLaPosicion(mapa, posicion);
	borrarEntrenadorPrincipalDelCasillero(casillero);
}

void borrarEntrenadorIntercambioDe(Mapa* mapa, t_posicion* posicion) {
	Casillero* casillero = casilleroParaLaPosicion(mapa, posicion);
	borrarEntrenadorIntercambioDelCasillero(casillero);
}

void moverEntrenadorPrincipalALaPosicion(Mapa* mapa, Entrenador* entrenador, t_posicion* nuevaPosicion) {
	borrarEntrenadorPrincipalDe(mapa, entrenador->posicion);
	setPosicionA(entrenador, nuevaPosicion);
	agregarEntrenadorPrincipalAlMapa(mapa, entrenador);
}

//esa nueva posicion seria en donde esta el entrenadorPrincipal
//el entrenadorIntercambio se acerca a la posicion del entrenadorPrincipal, con el que hace el cambio
void moverEntrenadorIntercambioALaPosicion(Mapa* mapa, Entrenador* entrenador, t_posicion* nuevaPosicion) {
	borrarEntrenadorIntercambioDe(mapa, entrenador->posicion);
	setPosicionA(entrenador, nuevaPosicion);
	agregarEntrenadorIntercambioAlMapa(mapa, entrenador);
}
