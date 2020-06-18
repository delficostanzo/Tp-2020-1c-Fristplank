#include "Trainer.h"

static int puedeIntercambiar(Entrenador* entrenador, PokemonEnElMapa* pokemonInnecesario, PokemonEnElMapa* pokemonDado);


typedef bool(*erasedTypeFilter)(void*);

//inicializa y devuelve un trainer nuevo. Es el new, al estilo Java. ESTADO NEW
Entrenador* newEntrenador() {
	return malloc(sizeof(Entrenador));
}

//imaginate esto como setters
void setPosicionA(Entrenador* entrenador, t_posicion* posicion) {
	entrenador->posicion = posicion;
}

//void setPokemonesObjetivosA(Entrenador* entrenador, t_list* pokemones) {
//	entrenador->pokemonesObjetivos = pokemones;
//}

void setPokemonA(t_list* listaPokemones, PokemonEnElMapa* nuevoPokemon) {

	if(list_is_empty(listaPokemones) == 1){
		list_add(listaPokemones, nuevoPokemon);
	}
	//YA HAY UNO DE ESA ESPECIE EN LA LISTA DE POKEMONES PASADA POR PARAMETRO
	else if(buscarPorNombre(nuevoPokemon->nombre, listaPokemones) != NULL){
		//le aumento la cantidad
		PokemonEnElMapa* pokemonASumar = buscarPorNombre(nuevoPokemon->nombre, listaPokemones);
		pokemonASumar->cantidad += nuevoPokemon->cantidad;
	}
		else {
		list_add(listaPokemones, nuevoPokemon);
		}
}

//void setPokemonesAtrapadosA(Entrenador* entrenador, t_list* pokemones) {
//	entrenador->pokemonesAtrapados = pokemones;
//}

t_posicion* newPosicion() {
	return malloc(sizeof(t_posicion));
}



void atrapar(Entrenador* entrenador, PokemonEnElMapa* pokemon) {
	int distanciaHastaPokemon = distanciaEntre(&(pokemon->posicion), entrenador->posicion);
	entrenador->ciclosCPUConsumido += distanciaHastaPokemon;
	entrenador->posicion = &(pokemon->posicion);
	//el socket ya esta conectado con el broker en Conexion
	enviarCatchDesde(entrenador, socketCatch);
	pasarDeExecABlockEsperando(entrenador);
}


/////////////INTERCAMBIO////////////////
//se busca al entrenador que necesite el que yo tengo de mas y tenga el que yo necesito
//si no lo encuentra devuelve NULL
Entrenador* buscarEntrenadorParaIntercambiar(PokemonEnElMapa* pokemonInnecesario, PokemonEnElMapa* pokemonNecesitado) {
	t_list* entrenadoresPosibles = entrenadoresBloqueadosPorDeadlock();

	int entrenadorCumpleCondicion(Entrenador* entrenador) {
		return puedeIntercambiar(entrenador, pokemonInnecesario, pokemonNecesitado);
	}
	//si hay otro entrenador en deadlock
	if(list_is_empty(entrenadoresPosibles) != 1){
		t_list* entrenadoresQueCumplen = list_filter(entrenadoresPosibles, (erasedTypeFilter) entrenadorCumpleCondicion);
		if(list_is_empty(entrenadoresQueCumplen) != 1) {
			//agarra el primero que cumpla si al menos hay 1
			return list_get(entrenadoresQueCumplen, 0);
		}
	}

	return NULL;

}

int puedeIntercambiar(Entrenador* entrenador, PokemonEnElMapa* pokemonInnecesario, PokemonEnElMapa* pokemonDado){
	//condicione del entrenador preguntado
	MovimientoEnExec* movimientoEnExec = entrenador->movimientoEnExec;
	char* nombrePokemonAIntercambiar = movimientoEnExec->pokemonAIntercambiar->nombre;
	char* nombrePokemonNecesitado = movimientoEnExec->pokemonNecesitado->nombre;

	//condiciones del entrenador que cumple la condicion
	char* nombrePokemonInnecesario = pokemonInnecesario->nombre;
	char* nombrePokemonDado = pokemonDado->nombre;

	return nombrePokemonAIntercambiar == nombrePokemonInnecesario && nombrePokemonNecesitado == nombrePokemonDado;
}


Entrenador* entrenadorMasCercanoA(PokemonEnElMapa* pokemon, t_list* entrenadores) {
	typedef bool(*erasedTypeSort)(void*, void*);

	bool estaMasCerca(Entrenador* entrenador1, Entrenador* entrenador2) {
		int distanciaEntrenador1 = distanciaEntre(entrenador1->posicion, &(pokemon->posicion));
		int distanciaEntrenador2 = distanciaEntre(entrenador2->posicion, &(pokemon->posicion));
		return distanciaEntrenador1 <= distanciaEntrenador2;

	}

	list_sort(entrenadores, (erasedTypeSort)estaMasCerca);
	return list_get(entrenadores, 0);
}





