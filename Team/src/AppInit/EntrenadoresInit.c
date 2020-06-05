
#include "EntrenadoresInit.h"


t_config* leerConfigDesde(String nombreDeArchivo);
// el "static" es para que las funciones queden privadas
static t_list* inicializarEntrenadoresHasta(int cantidad);
static void setPosicionesEnEntrenadoresDesde(t_config* config, t_list* entrenadores);
static void setPokemonesObjetivosDesde(t_config* config, t_list* entrenadores);
static void setPokemonesAtrapadosDesde(t_config* config, t_list* entrenadores);
static t_list* getObjetivosDesde(Entrenador* entrenador);
static t_list* getAtrapadosDesde(Entrenador* entrenador);
static t_list* unirPokemones(t_list* primerosPokes, t_list* segundosPokes);

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

	setPosicionesEnEntrenadoresDesde(configEntrenador, entrenadores);
	//
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
	//lista de listas de estructuras de pokes objetivos (solo tienen el nombre)
	t_list* objetivos = list_map(crearListaConStringsDeConfig(stringPokemonesObjetivos), (erasedType)pokemonesDesdeString);

	for(int index = 0; index < list_size(entrenadores); index++) {
		Entrenador* entrenador = list_get(entrenadores, index);

		//lista de pokemones objetivos de cada entrenador

		t_list* objetivosDelEntrenador = list_get(objetivos, index);

		for(int index2 = 0; index2 < list_size(objetivosDelEntrenador); index2++){
			PokemonEnElMapa* objetivoDelEntrenador =  list_get(objetivosDelEntrenador, index2);
			quickLog("Llega hasta aca"); //ROMPE EN LIST_FIND_ELEMENT??
			setPokemonObjetivoA(entrenador, objetivoDelEntrenador);
			quickLog("Aca no llega");
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

t_list* getObjetivosTotalesDesde(t_list* entrenadores) {
	typedef void*(*erasedTypeMap)(void*);
	typedef void*(*erasedTypeFold)(void*, void*);

	t_list* objetivosTotales;

	//me devuelve una lista de listas de objetivos globales
	objetivosTotales = list_map(entrenadores, (erasedTypeMap)getObjetivosDesde);

	//listaFinal es la lista con todos los pokemons que se deben atrapar en total
	t_list* listaObjetivos = list_create();
	list_fold(objetivosTotales, listaObjetivos, (erasedTypeFold)unirPokemones);

	list_destroy(objetivosTotales);
	return listaObjetivos;
}

t_list* getTotalAtrapadosDesde(t_list* entrenadores) {
	typedef void*(*erasedTypeMap)(void*);
	typedef void*(*erasedTypeFold)(void*, void*);

	t_list* objetivosAtrapados;

	//me devuelve una lista de listas de objetivos globales
	objetivosAtrapados = list_map(entrenadores, (erasedTypeMap)getAtrapadosDesde);

	//listaFinal es la lista con todos los pokemons que se deben atrapar en total
	t_list* listaAtrapados = list_create();
	list_fold(objetivosAtrapados, listaAtrapados, (erasedTypeFold)unirPokemones);

	list_destroy(objetivosAtrapados);
	return listaAtrapados;

}

// devuelve una lista de los pokemones objetivos disminuyendo la cantidad, si esta atrapado
// Si hay un pokemon que ya se atrapo, te va a devolver que ese objetivo es cantidad 0
t_list* getObjetivosGlobalesDesde(t_list* entrenadores) {
	typedef void*(*erasedTypeMap)(void*);

	PokemonEnElMapa* restarCantidadQueFalta(PokemonEnElMapa* pokemonRestado) {
		typedef bool(*erasedTypeMap)(void*);

		bool esLaMismaEspecie(PokemonEnElMapa* pokemonARestar){
			return pokemonRestado->nombre == pokemonARestar->nombre;
		}
		PokemonEnElMapa* pokemonAtrapado = list_find(getTotalAtrapadosDesde(entrenadores), (erasedTypeMap)esLaMismaEspecie);
		pokemonRestado->cantidad -= pokemonAtrapado->cantidad;
		return pokemonRestado;

	}
	//diferencia entre la lista de objetivos totales y los atrapados
	return list_map(getObjetivosTotalesDesde(entrenadores), (erasedTypeMap)restarCantidadQueFalta);


}



t_list* getObjetivosDesde(Entrenador* entrenador) {
	return entrenador->pokemonesObjetivos;
}

t_list* getAtrapadosDesde(Entrenador* entrenador) {
	return entrenador->pokemonesAtrapados;
}

t_list* unirPokemones(t_list* primerosPokes, t_list* segundosPokes){
	for(int index=0;index < sizeof(segundosPokes);index++){
		PokemonEnElMapa* pokemon = list_get(segundosPokes, index);

		if(buscarPorNombre(pokemon->nombre, primerosPokes) != NULL){
			PokemonEnElMapa* pokemonPrimerLista = buscarPorNombre(pokemon->nombre, primerosPokes);
			PokemonEnElMapa* pokemonSegundaLista = buscarPorNombre(pokemon->nombre, segundosPokes);
			pokemonPrimerLista->cantidad += pokemonSegundaLista->cantidad;
		}else{
			list_add(primerosPokes, pokemon);
		}

	}

	return primerosPokes;
}
