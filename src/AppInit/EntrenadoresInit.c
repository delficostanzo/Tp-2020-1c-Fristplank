
#include "EntrenadoresInit.h"


t_config* leerConfigDesde(char* nombreDeArchivo);
// el "static" es para que las funciones queden privadas
static t_list* inicializarEntrenadoresHasta(int cantidad);
static void setPosicionesEnEntrenadoresDesde(t_config* config, t_list* entrenadores);
static void setPokemonesObjetivosDesde(t_config* config, t_list* entrenadores);
static void setPokemonesAtrapadosDesde(t_config* config, t_list* entrenadores);
static t_list* getObjetivosDesde(Entrenador* entrenador);
static t_list* getAtrapadosDesde(Entrenador* entrenador);
static t_list* unirPokemones(t_list* primerosPokes, t_list* segundosPokes);
static void agregarObjetivosA(Entrenador* entrenador, t_list* objetivosDelEntrenador);
static void agregarAtrapadosA(Entrenador* entrenador, t_list* pokemonesAAgregar);
static bool noTieneCantidadCero(PokemonEnElMapa* pokemon);

//si declaras aca arriba las funciones con un 'static' adelante, es la manera de hacerlas privadas. No alcanza solo con omitirlas en el ".h"

//delego el comportamiento para crear una lista de entrenadores a partir del archivo de configuracion de donde estan
t_list* getEntrenadoresDesde(char* nombreDeArchivo) {
	//t_log* logger = iniciar_log("team");
	t_log* logger = iniciar_logger();

	t_config* configEntrenador = leerConfigDesde(nombreDeArchivo);

	//esto lo agregamos al archivo de configuracion, ya que podiamos agregar lo que necesitabamos. Sirve para saber cuantos entrenadores hay
	int cantidadEntrenadores = config_get_int_value(configEntrenador, "CANTIDAD_ENTRENADORES");
	log_info(logger, string_from_format("Cantidad de entrenadores: %d", cantidadEntrenadores));
	//lista de estructuras entrenador segun la cantidad que haya en config
	t_list* entrenadores = inicializarEntrenadoresHasta(cantidadEntrenadores);

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

	char** stringPosiciones = config_get_array_value(config, "POSICIONES_ENTRENADORES");
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

void setPokemonesObjetivosDesde(t_config* config, t_list* entrenadores) {
	typedef void*(*erasedType)(void*);

	//lee todos los pokemones objetivos como 1 string*
	char** stringPokemonesObjetivos = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");

	//lista de listas de estructuras de pokes objetivos de cada entrenador
	//llegan 3 valores OK
	t_list* objetivos = list_map(crearListaConStringsDeConfig(stringPokemonesObjetivos), (erasedType)pokemonesDesdeString);

	for(int index = 0; index < list_size(entrenadores); index++) {
		Entrenador* entrenador = list_get(entrenadores, index);

		//lista de pokemones objetivos de cada entrenador
		t_list* objetivosDelEntrenador = list_get(objetivos, index);

		agregarObjetivosA(entrenador, objetivosDelEntrenador);

		quickLog(string_from_format("El entrenador %d tiene %d objetivos",index,entrenador->pokemonesObjetivos->elements_count));
	}

	list_destroy(objetivos);
	quickLog("Cargados los objetivos de los entrenadores");

}

void setPokemonesAtrapadosDesde(t_config* config, t_list* entrenadores) {
	typedef void*(*erasedType)(void*);

	//lee todos los pokemones objetivos como 1 string*
	char** stringPokemonesAtrapados = config_get_array_value(config, "POKEMON_ENTRENADORES");

	//lista de listas de estructuras de pokes objetivos de cada entrenador
	//llegan 3 valores OK
	t_list* atrapados = list_map(crearListaConStringsDeConfig(stringPokemonesAtrapados), (erasedType)pokemonesDesdeString);

	for(int index = 0; index < list_size(entrenadores); index++) {
		Entrenador* entrenador = list_get(entrenadores, index);

		//lista de pokemones atrapados de cada entrenador
		t_list* atrapadosDelEntrenador = list_get(atrapados, index);

		agregarAtrapadosA(entrenador, atrapadosDelEntrenador);

		quickLog(string_from_format("El entrenador %d tiene %d atrapados",index,entrenador->pokemonesAtrapados->elements_count));
	}

	list_destroy(atrapados);
	quickLog("Cargados los atrapados de los entrenadores");
}


///////POKEMONES////////

void agregarObjetivosA(Entrenador* entrenador, t_list* pokemonesAAgregar) {
	t_list* objetivosDelEntrenador = list_create();
	entrenador->pokemonesObjetivos = objetivosDelEntrenador;
	for(int index = 0; index < list_size(pokemonesAAgregar); index++){
		PokemonEnElMapa* objetivoDelEntrenador =  list_get(pokemonesAAgregar, index);
		setPokemonA(objetivosDelEntrenador, objetivoDelEntrenador);
	}
}

void agregarAtrapadosA(Entrenador* entrenador, t_list* pokemonesAAgregar) {
	t_list* atrapadosDelEntrenador = list_create();
	entrenador->pokemonesAtrapados = atrapadosDelEntrenador;
	for(int index = 0; index < list_size(pokemonesAAgregar); index++){
		PokemonEnElMapa* atrapadoDelEntrenador =  list_get(pokemonesAAgregar, index);
		setPokemonA(atrapadosDelEntrenador, atrapadoDelEntrenador);
	}
}

t_list* getObjetivosTotalesDesde(t_list* entrenadores) {
	typedef void*(*erasedTypeMap)(void*);
	typedef void*(*erasedTypeFold)(void*, void*);

	t_list* objetivosTotales;

	//me devuelve una lista de todos los pokemones (los que atrapo y los que no)
	//es una lista de listas
	objetivosTotales = list_map(entrenadores, (erasedTypeMap)getObjetivosDesde);

	// objetivosTotales agarra la primer lista y la une con la lista del segundo parametro (que es una lista con otros pokes) y te devuelve
	// una lista
	t_list* objetivosAgrupados = list_create();
	list_fold(objetivosTotales, objetivosAgrupados, (erasedTypeFold)unirPokemones);

	list_destroy(objetivosTotales);
	return objetivosAgrupados;
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

// devuelve una lista de los pokemones objetivos con su cantidad que falta atrapar
t_list* getObjetivosGlobalesDesde(t_list* entrenadores) {
	typedef void*(*erasedTypeMap)(void*);

	PokemonEnElMapa* restarCantidadQueFalta(PokemonEnElMapa* pokemonRestado) {
		typedef bool(*erasedTypeMap)(void*);

		bool esLaMismaEspecie(PokemonEnElMapa* pokemonARestar){
			return pokemonRestado->nombre == pokemonARestar->nombre;
		}
		//si todavia no hay ninguno atrapado o se atraparon pero ninguno coincide con la especie a restar
		if(list_is_empty(getTotalAtrapadosDesde(entrenadores)) == 1 || list_find(getTotalAtrapadosDesde(entrenadores), (erasedTypeMap)esLaMismaEspecie) == NULL){
			//no le cambia la cantidad, entonces todavia falta por atrapar (queda como objetivo global)
			return pokemonRestado;

		} else{
			// si coincide con alguno de la lista de pokes atrapados, se resta
			PokemonEnElMapa* pokemonAtrapado = list_find(getTotalAtrapadosDesde(entrenadores), (erasedTypeMap)esLaMismaEspecie);
			pokemonRestado->cantidad -= pokemonAtrapado->cantidad;
			return pokemonRestado;

		}

	}
	//diferencia entre la lista de objetivos totales y los atrapados
	t_list* listaConObjetivosRestados = list_map(getObjetivosTotalesDesde(entrenadores), (erasedTypeMap)restarCantidadQueFalta);
	return list_filter(listaConObjetivosRestados, noTieneCantidadCero);

}

bool noTieneCantidadCero(PokemonEnElMapa* pokemon){
	return pokemon->cantidad != 0;
}



t_list* getObjetivosDesde(Entrenador* entrenador) {
	return entrenador->pokemonesObjetivos;
}

t_list* getAtrapadosDesde(Entrenador* entrenador) {
	return entrenador->pokemonesAtrapados;
}

//
t_list* unirPokemones(t_list* primerosPokes, t_list* segundosPokes){
	for(int index=0;index < sizeof(segundosPokes);index++){
		PokemonEnElMapa* pokemon = list_get(segundosPokes, index);

		if(list_is_empty(primerosPokes) == 1){

			list_add(primerosPokes, pokemon);

		} else if(buscarPorNombre(pokemon->nombre, primerosPokes) != NULL){

				PokemonEnElMapa* pokemonPrimerLista = buscarPorNombre(pokemon->nombre, primerosPokes);
				//PokemonEnElMapa* pokemonSegundaLista = buscarPorNombre(pokemon->nombre, segundosPokes);
				pokemonPrimerLista->cantidad += pokemon->cantidad;

				} else{
					list_add(primerosPokes, pokemon);
				}

	}

	return primerosPokes;
}
