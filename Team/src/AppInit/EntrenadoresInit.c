

#include "EntrenadoresInit.h"
#include "../Model/Trainer.h"
#include "../TeamUtils/LogManager.h"
#include "ConfigFunctions.h"


t_config* leerConfigDesde(String nombreDeArchivo);
// el "static" es para que las funciones queden privadas
static t_list* inicializarEntrenadoresHasta(int cantidad);
static void setPosicionesEnEntrenadoresDesde(t_config* config, t_list* entrenadores);
static void setPokemonesObjetivosDesde(t_config* config, t_list* entrenadores);
static void setPokemonesAtrapadosDesde(t_config* config, t_list* entrenadores);
static t_list* getObjetivoDesde(Entrenador* entrenador);

//si declaras aca arriba las funciones con un 'static' adelante, es la manera de hacerlas privadas. No alcanza solo con omitirlas en el ".h"

//delego el comportamiento para crear una lista de entrenadores a partir del archivo de configuracion de donde estan
t_list* getEntrenadoresDesde(String nombreDeArchivo) {
	t_config* configEntrenador = leerConfigDesde(nombreDeArchivo);
	t_list* entrenadores;

	//esto lo agregamos al archivo de configuracion, ya que podiamos agregar lo que necesitabamos. Sirve para saber cuantos entrenadores hay
	int cantidadEntrenadores = config_get_int_value(configEntrenador, "CANTIDAD_ENTRENADORES");
	quickLog(string_from_format("Cantidad de entrenadores: %d", cantidadEntrenadores));
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
		Entrenador* entrenador = newTrainer();

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

		setPosicion(entrenador, posicion);
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

			setPokemonesAtrapadosTo(entrenador, atrapadosDelEntrenador);
		}

	list_destroy(atrapados);
	quickLog("Cargados los pokemones atrapados de los entrenadores");
}

void setPokemonesObjetivosDesde(t_config* config, t_list* entrenadores) {
	typedef void*(*erasedType)(void*);

	String* stringPokemonesObjetivos = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
	t_list* objetivos = list_map(crearListaConStringsDeConfig(stringPokemonesObjetivos), (erasedType)pokemonesDesdeString);

	for(int index = 0; index < list_size(entrenadores); index++) {
			Entrenador* entrenador = list_get(entrenadores, index);
			t_list* objetivosDelEntrenador = list_get(objetivos, index);

			setPokemonesObjetivosTo(entrenador, objetivosDelEntrenador);
		}

	quickLog(string_from_format("%d", (((Entrenador*)list_get(entrenadores, 0))->posicion->x)));
	quickLog(string_from_format("%d", (((Entrenador*)list_get(entrenadores, 0))->posicion->y)));

	list_destroy(objetivos);
	quickLog("Cargados los objetivos de los entrenadores");

}


///////OBJETIVOS////////
t_list* list_adding_all(t_list* firstList, t_list* secondList){
	list_add_all(firstList, secondList);

	return firstList;
}

t_list* getObjetivosGlobalesDesde(t_list* entrenadores) {
	typedef void*(*erasedTypeMap)(void*);
	typedef void*(*erasedTypeFold)(void*, void*);

	t_list* objetivosGlobales;

	//me devuelve una lista de listas de objetivos globales
	objetivosGlobales = list_map(entrenadores, (erasedTypeMap)getObjetivoDesde);

	t_list* listaFinal = list_create();
	list_fold(objetivosGlobales, listaFinal, (erasedTypeFold)list_adding_all);

	list_destroy(objetivosGlobales);
	return listaFinal;

}

t_list* getObjetivoDesde(Entrenador* entrenador) {
	return entrenador->pokemonesObjetivos;
}

