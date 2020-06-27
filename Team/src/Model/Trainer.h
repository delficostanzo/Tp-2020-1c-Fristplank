
#ifndef SRC_MODEL_TRAINER_H_
#define SRC_MODEL_TRAINER_H_

#include "utils.h"
#include "../AppInit/LogFunctions.h"
#include "Pokemon.h"


typedef enum{
	MOVERyATRAPAR = 1,
	MOVEReINTERCAMBIAR = 2,
}ObjetivoEnExec;

typedef enum{
	ESPERANDORESPUESTA = 1,
	DORMIDO = 2,
	DEADLOCK = 3
}MotivoDeBloqueo;

typedef enum{
	NEW = 1,
	READY = 2,
	EXEC = 3,
	BLOCK = 4,
	EXIT = 5
}Estado;

typedef struct {
	PokemonEnElMapa* pokemonNecesitado;
	PokemonEnElMapa* pokemonAIntercambiar; // cuando el objetivo sea atrapar, este estaria en NULL. Se va a usar cuando es mover e intercambiar(DEADLOCK)
	ObjetivoEnExec objetivo;
}MovimientoEnExec;

typedef struct {
	t_posicion* posicion;
	t_list* pokemonesObjetivos;
	t_list* pokemonesAtrapados;
	MovimientoEnExec* movimientoEnExec;
	MotivoDeBloqueo motivo;
	Estado estado;
	pthread_t hiloEntrenador;
	int ciclosCPUConsumido;
	int idCorrelativoDeEspera;
	pthread_mutex_t mutexEntrenador;
}Entrenador;

Entrenador* newEntrenador();
void setPosicionA(Entrenador* entrenador, t_posicion* posicion);
void setPokemonA(t_list* listaPokemones, PokemonEnElMapa* nuevoPokemon);
t_posicion* newPosicion();
Entrenador* entrenadorMasCercanoA(PokemonEnElMapa* pokemon, t_list* entrenadores);
Entrenador* buscarEntrenadorParaIntercambiar(PokemonEnElMapa* pokemonInnecesario, PokemonEnElMapa* pokemonNecesitado);
//void atrapar(Entrenador* entrenador, PokemonEnElMapa* pokemon);
void agregarAtrapado(Entrenador* entrenador, PokemonEnElMapa* pokemonAtrapado);
void pasarADormido(Entrenador* entrenador);
void estadoSiAtrapo(Entrenador* entrenador);
void pasarABlockEsperando(Entrenador* entrenador);
int distanciaEntre(t_posicion* posicion1, t_posicion* posicion2);

#endif /* SRC_MODEL_TRAINER_H_ */
