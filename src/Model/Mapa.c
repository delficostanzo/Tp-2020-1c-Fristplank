
#include "Mapa.h"


int distanciaEntre(t_posicion* posicion1, t_posicion* posicion2) {
	int distanciaEnX = posicion1->posicionX - posicion2->posicionX;
	int distanciaEnY = posicion1->posicionY - posicion2->posicionY;
	return abs(distanciaEnX) + abs(distanciaEnY);
}

