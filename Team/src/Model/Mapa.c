
#include "Mapa.h"


int distanciaEntre(t_posicion* posicion1, t_posicion* posicion2);


Entrenador* entrenadorMasCercanoA(PokemonEnElMapa* pokemon, t_list* entrenadores) {
	//Mapa mapa;
	typedef bool(*erasedTypeSort)(void*, void*);

	bool estaMasCerca(Entrenador* entrenador1, Entrenador* entrenador2) {
		int distanciaEntrenador1 = distanciaEntre(entrenador1->posicion, &(pokemon->posicion));
		int distanciaEntrenador2 = distanciaEntre(entrenador2->posicion, &(pokemon->posicion));
		return distanciaEntrenador1 <= distanciaEntrenador2;

	}

	list_sort(entrenadores, (erasedTypeSort)estaMasCerca);
	return list_get(entrenadores, 0);
}

int distanciaEntre(t_posicion* posicion1, t_posicion* posicion2) {
	int distanciaEnX = posicion1->x - posicion2->x;
	int distanciaEnY = posicion1->y - posicion2->y;
	return abs(distanciaEnX) + abs(distanciaEnY);
}

