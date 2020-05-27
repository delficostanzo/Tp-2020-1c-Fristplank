
#ifndef SRC_MODEL_CASILLERO_H_
#define SRC_MODEL_CASILLERO_H_

#include "Trainer.h"
#include "Pokemon.h"

//un casillero esta en una posicion
//situciones posibles:
//1. que tenga 1 pokemon sin atrapar => tiene pokemon, no tiene ni entrenadorPrincipal ni entrenadorIntercambio
//2. que un pokemon sea atrapado por un entrenador principal que tenia a ese pokemon como objetivo => no hay entrenadorIntercambio ni pokemon
//3. que un pokemon sea atrapado por un entrenador principal que no tenia a ese pokemon como objetivo => termina bloqueado => no hay entrenadorIntercambio ni pokemon
//4. que otro pokemon sea atrapado por un entrenador principal que no tenia a ese pokemon como objetivo => termina bloqueado => no hay entrenadorIntercambio ni pokemon
//5. Alguno de los 2 entrenadores bloqueados se mueve hasta el casillero donde esta el otro entrenador
	//el casillero del que se movio queda vacio, y el casillero del pokemon queda con el entrenadorPrincipal (que no se movio) y el entrenadorIntercambio (el que se movio), no tiene pokemon
	//intercambian entre si los pokemones que tienen en atrapados pero no en objetivos
typedef struct {
	t_pokemon* pokemon; //pokemon sin atrapar
	Entrenador* entrenadorPrincipal; //el que tiene el entrenador que no se movio en un DEADLOCK
	Entrenador* entrenadorIntercambio;
}Casillero;

Casillero* newCasillero();
void agregarPokemonAlCasillero(Casillero* casillero, t_pokemon* pokemon);
void agregarEntrenadorPrincipalAlCasillero(Casillero* casillero, Entrenador* entrenador);
void agregarEntrenadorIntercambioAlCasillero(Casillero* casillero, Entrenador* entrenador);
void borrarEntrenadorPrincipalDelCasillero(Casillero* casillero);
void borrarEntrenadorIntercambioDelCasillero(Casillero* casillero);
void liberarCasillero(Casillero* casillero);


#endif /* SRC_MODEL_CASILLERO_H_ */
