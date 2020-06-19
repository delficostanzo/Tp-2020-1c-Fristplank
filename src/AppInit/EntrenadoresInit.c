
#include "EntrenadoresInit.h"


t_config* leerConfigDesde(char* nombreDeArchivo);
// el "static" es para que las funciones queden privadas
static t_list* inicializarEntrenadoresHasta(int cantidad);
static void setPosicionesEnEntrenadoresDesde(t_config* config, t_list* entrenadores);
static void setPokemonesObjetivosDesde(t_config* config, t_list* entrenadores);
static void setPokemonesAtrapadosDesde(t_config* config, t_list* entrenadores);
static void setEstadoNew(t_list* entrenadores);
static t_list* getObjetivosDesde(Entrenador* entrenador);
static t_list* getAtrapadosDesde(Entrenador* entrenador);
static void agregarObjetivosA(Entrenador* entrenador, t_list* objetivosDelEntrenador);
static void agregarAtrapadosA(Entrenador* entrenador, t_list* pokemonesAAgregar);
static int noTieneCantidadNegativa(PokemonEnElMapa* pokemon);

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
	setEstadoNew(entrenadores);

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

void setEstadoNew(t_list* entrenadores){
	for(int index=0; index < list_size(entrenadores); index++){
		Entrenador* entrenador = list_get(entrenadores, index);
		entrenador->estado = 1;
	}
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


//objetivos de TODOS los entrenadores esten atrapados o no
t_list* getObjetivosTotalesDesde(t_list* entrenadores) {
	typedef void*(*erasedTypeMap)(void*);

	t_list* objetivosTotales;

	//me devuelve una lista de todos los pokemones (los que atrapo y los que no)
	//es una lista de listas
	objetivosTotales = list_map(entrenadores, (erasedTypeMap)getObjetivosDesde);

	t_list* objetivosAgrupados = list_create();

	void agregarAUnaNuevaListaConcat(t_list* objetivosDeUnEntrenador) {
		for(int index=0; index<list_size(objetivosDeUnEntrenador); index++){
			PokemonEnElMapa* pokemonEnLaLista = list_get(objetivosDeUnEntrenador,index);
			setPokemonA(objetivosAgrupados,pokemonEnLaLista);
		}
	}

	t_list* voids = list_map(objetivosTotales, (erasedTypeMap)agregarAUnaNuevaListaConcat);

	quickLog("Se cargaron los objetivos totales de los entrenadores (atrapados y sin atrapar)");

	list_destroy(objetivosTotales);
	list_destroy(voids);

	return objetivosAgrupados;
}

t_list* getTotalAtrapadosDesde(t_list* entrenadores) {
	typedef void*(*erasedTypeMap)(void*);

	t_list* pokemonesAtrapados;

	//me devuelve una lista de listas de objetivos atrapados (1 lista por cada entrenador)
	pokemonesAtrapados = list_map(entrenadores, (erasedTypeMap)getAtrapadosDesde);

	t_list* atrapadosAgrupados = list_create();

	//listaAgrupados es la lista con todos los pokemons que se deben atrapar en total
	void agregarAUnaNuevaListaConcat(t_list* atrapadosDeUnEntrenador) {
		for(int index=0; index<list_size(atrapadosDeUnEntrenador); index++){
			PokemonEnElMapa* pokemonEnLaLista = list_get(atrapadosDeUnEntrenador,index);
			setPokemonA(atrapadosAgrupados,pokemonEnLaLista);
		}
	}

	t_list* voids = list_map(pokemonesAtrapados, (erasedTypeMap)agregarAUnaNuevaListaConcat);

	list_destroy(pokemonesAtrapados);
	list_destroy(voids);
	return atrapadosAgrupados;
}

// devuelve una lista de los pokemones objetivos con su cantidad que falta atrapar
t_list* getObjetivosGlobalesDesde(t_list* pokemonesObjetivos, t_list* pokemonesAtrapados) {
	typedef void*(*erasedTypeMap)(void*);
	typedef bool(*erasedTypeFilter)(void*);


	quickLog("Llega aca");
	//el pokemon restado serian los pokemones objetivos, que se va a agregar con la cantidad restada si ya se atrapo
	PokemonEnElMapa* restarCantidadQueFalta(PokemonEnElMapa* pokemonRestado) {

		//si todavia no hay ninguno atrapado o se atraparon pero ninguno coincide con la especie a restar
		if(list_is_empty(pokemonesAtrapados) == 1 || buscarPorNombre(pokemonRestado->nombre,pokemonesAtrapados) == NULL){
			//no le cambia la cantidad, entonces todavia falta por atrapar (queda como objetivo global)
			return pokemonRestado;
		}else {
			// si coincide con alguno de la lista de pokes atrapados, se resta
			PokemonEnElMapa* pokemonAtrapado = buscarPorNombre(pokemonRestado->nombre,pokemonesAtrapados);
			pokemonRestado->cantidad = pokemonRestado->cantidad - pokemonAtrapado->cantidad;
			return pokemonRestado;
		}
	}

	//diferencia entre la lista de objetivos totales y los atrapados
	t_list* listaConObjetivosRestados = list_create();
	listaConObjetivosRestados = list_map(pokemonesObjetivos, (erasedTypeMap)restarCantidadQueFalta);
	quickLog("Se cargaron todos los objetivos globales");
	return list_filter(listaConObjetivosRestados, (erasedTypeFilter)noTieneCantidadNegativa);
}

int noTieneCantidadNegativa(PokemonEnElMapa* pokemon){
	return pokemon->cantidad != 0;
}


t_list* getObjetivosDesde(Entrenador* entrenador) {
	return entrenador->pokemonesObjetivos;
}

t_list* getAtrapadosDesde(Entrenador* entrenador) {
	return entrenador->pokemonesAtrapados;
}

