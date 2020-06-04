
#include "EntrenadoresInit.h"
#include "../Model/Trainer.h"
#include "ConfigFunctions.h"


t_config* leerConfigDesde(String nombreDeArchivo);
// el "static" es para que las funciones queden privadas
static t_list* inicializarEntrenadoresHasta(int cantidad);
static void setPosicionesEnEntrenadoresDesde(t_config* config, t_list* entrenadores);
static void setPokemonesObjetivosDesde(t_config* config, t_list* entrenadores);
static void setPokemonesAtrapadosDesde(t_config* config, t_list* entrenadores);
static t_list* getObjetivosDesde(Entrenador* entrenador);
static t_list* list_adding_all(t_list* firstList, t_list* secondList);

//si declaras aca arriba las funciones con un 'static' adelante, es la manera de hacerlas privadas. No alcanza solo con omitirlas en el ".h"

//delego el comportamiento para crear una lista de entrenadores a partir del archivo de configuracion de donde estan
t_list* getEntrenadoresDesde(String nombreDeArchivo) {
	//t_log* logger = iniciar_log("team");
	t_log* logger = iniciar_logger();

	t_config* configEntrenador = leerConfigDesde(nombreDeArchivo);
	t_list* entrenadores;

	//esto lo agregamos al archivo de configuracion, ya que podiamos agregar lo que necesitabamos. Sirve para saber cuantos entrenadores hay
	int cantidadEntrenadores = config_get_int_value(configEntrenador, "CANTIDAD_ENTRENADORES");
	log_info(logger, string_from_format("Cantidad de entrenadores: %d", cantidadEntrenadores));
	entrenadores = inicializarEntrenadoresHasta(cantidadEntrenadores); //aca inicializamos tantos entrenadores como haya

	//aca delego lo de setear posiciones dentro de los entrenadores
	setPosicionesEnEntrenadoresDesde(configEntrenador, entrenadores);
	setPokemonesObjetivosDesde(configEntrenador, entrenadores);
	setPokemonesAtrapadosDesde(configEntrenador, entrenadores);

	return entrenadores;
}

//esto crea una lista con entrenadores vacios, hasta la cantidad que le indiques
t_list* inicializarEntrenadoresHasta(int cantidad) {
	t_list* entrenadores = list_create();

	for(int contador=0; contador<cantidad; contador++) {
		Entrenador* entrenador = newEntrenador();

		list_add(entrenadores, entrenador);
	}

	return entrenadores;
}

void setPosicionesEnEntrenadoresDesde(t_config* config, t_list* entrenadores) {
	//podes hacer estos typedef locales, para mutear los invalidPointers warnings, como en el map de mas abajo.
	//Solo tenes que emular lo que hago con esto, y ponerle el nombre que vos quieras al type
	typedef void*(*erasedType)(void*);

	String* stringPosiciones = config_get_array_value(config, "POSICIONES_ENTRENADORES");
	//el map recibe una lista de tipo t_list*, y una funcion que transforma
	//crearListaConStringsDeConfig te devuelve una t_list* a partir del array de strings que sacas del archivo de configuracion
	t_list* posiciones = list_map(crearListaConStringsDeConfig(stringPosiciones), (erasedType)posicionDesde);

	//para cada entrenador, le seteo su correspondiente posicion
	for(int index = 0; index < list_size(entrenadores); index++) {
		Entrenador* entrenador = list_get(entrenadores, index);
		t_posicion* posicion = list_get(posiciones, index);

		setPosicionA(entrenador, posicion);
	}

	list_destroy(posiciones);
	quickLog("Cargadas las posiciones de los entrenadores");
}

void setPokemonesAtrapadosDesde(t_config* config, t_list* entrenadores) {
	typedef void*(*erasedType)(void*);

	String* stringPokemonesAtrapados = config_get_array_value(config, "POKEMON_ENTRENADORES");
	t_list* atrapados = list_map(crearListaConStringsDeConfig(stringPokemonesAtrapados), (erasedType)pokemonesDesdeString);

	for(int index = 0; index < list_size(entrenadores); index++) {
		Entrenador* entrenador = list_get(entrenadores, index);

		t_list* atrapadosDelEntrenador = list_get(atrapados, index);

		for(int index2 = 0; index2 < list_size(atrapadosDelEntrenador); index2 ++){
			setPokemonAtrapadoA(entrenador, list_get(atrapadosDelEntrenador, index2));
		}

		//setPokemonesAtrapadosA(entrenador, objetivosDelEntrenador);

	}

	list_destroy(atrapados);
	quickLog("Cargados los pokemones atrapados de los entrenadores");
}

void setPokemonesObjetivosDesde(t_config* config, t_list* entrenadores) {
	typedef void*(*erasedType)(void*);

	String* stringPokemonesObjetivos = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
	//lista de estructuras de pokes objetivos
	t_list* objetivos = list_map(crearListaConStringsDeConfig(stringPokemonesObjetivos), (erasedType)pokemonesDesdeString);

	for(int index = 0; index < list_size(entrenadores); index++) {
		Entrenador* entrenador = list_get(entrenadores, index);

		t_list* objetivosDelEntrenador = list_get(objetivos, index);

		for(int index2 = 0; index2 < list_size(objetivosDelEntrenador); index2 ++){
			setPokemonObjetivoA(entrenador, list_get(objetivosDelEntrenador, index2));
		}

		//setPokemonesObjetivosA(entrenador, objetivosDelEntrenador);

		quickLog(string_from_format("Un entrenador tiene posX: %d", (((Entrenador*)list_get(entrenadores, 0))->posicion->posicionX)));
		quickLog(string_from_format("Un entrenador tiene posY: %d", (((Entrenador*)list_get(entrenadores, 0))->posicion->posicionY)));

	}

	list_destroy(objetivos);
	quickLog("Cargados los objetivos de los entrenadores");

}


///////OBJETIVOS////////
//Esas listas son listas de pokes en el mapa
t_list* unirPokemones(t_list* firstList, t_list* secondList){
	for(int index=0;index < sizeof(secondList);index++){
		PokemonEnElMapa* pokemon = list_get(secondList, index);

		if(buscarPorNombre(pokemon->nombre, firstList) != NULL){
			PokemonEnElMapa* pokemonPrimerLista = buscarPorNombre(pokemon->nombre, firstList);
			PokemonEnElMapa* pokemonSegundaLista = buscarPorNombre(pokemon->nombre, secondList);
			pokemonPrimerLista->cantidad += pokemonSegundaLista->cantidad;
		}else{
			list_add(firstList, pokemon);
		}

	}

	return firstList;
}

t_list* getObjetivosTotalesDesde(t_list* entrenadores) {
	typedef void*(*erasedTypeMap)(void*);
	typedef void*(*erasedTypeFold)(void*, void*);

	t_list* objetivosTotales;

	//me devuelve una lista de listas de objetivos globales
	objetivosTotales = list_map(entrenadores, (erasedTypeMap)getObjetivosDesde);

	//listaFinal es la lista con todos los pokemons que se deben atrapar en total
	t_list* listaObjetivos = list_create();
	list_fold(objetivosTotales, listaObjetivos, (erasedTypeFold)list_adding_all);

	list_destroy(objetivosTotales);
	return listaObjetivos;
}

t_list* getObjetivosAtrapadosDesde(t_list* entrenadores) {
	typedef void*(*erasedTypeMap)(void*);
	typedef void*(*erasedTypeFold)(void*, void*);

	t_list* objetivosAtrapados;

	//me devuelve una lista de listas de objetivos globales
	objetivosAtrapados = list_map(entrenadores, (erasedTypeMap)getAtrapadosDesde);

	//listaFinal es la lista con todos los pokemons que se deben atrapar en total
	t_list* listaAtrapados = list_create();
	list_fold(objetivosAtrapados, listaAtrapados, (erasedTypeFold)list_adding_all);

	list_destroy(objetivosAtrapados);
	return listaAtrapados;

}

t_list* getObjetivosGlobalesDesde(t_list* entrenadores) {
	bool noEstaAtrapado(PokemonEnElMapa* pokemon) {

	}
	//diferencia entre la lista de objetivos totales y los atrapados
	return list_filter(getObjetivosTotalesDesde(entrenadores), noEstaAtrapado);

}


t_list* getObjetivosDesde(Entrenador* entrenador) {
	return entrenador->pokemonesObjetivos;
}

t_list* getAtrapadosDesde(Entrenador* entrenador) {
	return entrenador->pokemonesAtrapados;
}

//TODO: el proceso Team debera conocer cuales (esto ya esta) y que cantidad de Pokemones de cada especie requiere en total para cumplir su objetivo global

// necesita saber el proceso Team que especies y la cantidad de cada especie que necesita en total
// Ej: Pikachu necesita 4, Squirtle necesita 2, Delfina necesita 9, Victoria necesita 8

// ordeno la lista de objetivos por especie, asi te queda ej: [Pikachu, Pikachu, Delfina, Delfina, Delfina, Victoria]
//t_list* especiesDePokemonsQueHayQueAtraparOrdenados(t_list* objetivosGlobales){
//	typedef bool(*erasedTypeSort)(void*, void*);
//
//	bool tipoDeEspecieSegunElNombre(PokemonEnElMapa* pokemon1, PokemonEnElMapa* pokemon2){
//		String nombrePokemon1 = pokemon1->nombre;
//		String nombrePokemon2 = pokemon2->nombre;
//
//		return nombrePokemon1 == nombrePokemon2;
//	}
//	// ordeno la lista de objetivos segun la especie
//	t_list* objetivosOrdenadosPorEspecie = list_sorted(objetivosGlobales, (erasedTypeSort)tipoDeEspecieSegunElNombre);
//
//	return objetivosOrdenadosPorEspecie;
//}
//
//

