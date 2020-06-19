
#include "EnvioMensajes.h"

static void agregarPosicionSiLoNecesita(char* nombrePokemon, t_posicion posicion, t_list* pokemonesGlobales, t_list* pokemonesLibres);
static void agregarComoIdCorrelativoLocalized(int idCorrelativo);
static void agregarComoIdCorrelativoCaught(int idCorrelativo);
static void recibirIdCatch(int socketIdCatch, Entrenador* entrenador);
static int tieneComoIdCorrelativoLocalized(int idBuscado);
static int tieneComoIdCorrelativoCaught(int idBuscado);
static Entrenador* entrenadorQueTieneId(int idCatchQueResponde);
static void procesarEspera(Entrenador*  entrenador, uint32_t atrapo);
t_list* idsCorrelativosCaught;
t_list* idsCorrelativosLocalized;

typedef bool(*erasedTypeFilter)(void*);

//////////GET-LOCALIZED//////////////
void enviarGetDesde(t_list* objetivosGlobales, int socketGet){
	t_log* logger = iniciar_logger();
	quickLog("Esta enviando los get por cada pokemon objetivo necesario");
	for(int index=0; index<list_size(objetivosGlobales);index++){
		PokemonEnElMapa* poke = list_get(objetivosGlobales, index);
		t_get_pokemon* getPoke = crearEstructuraGetDesde(poke);
		enviar_get_pokemon(getPoke, socketGet, -1, -1);
		log_info(logger, "Se envio el get para el pokemon %s de tamaño %d", getPoke->pokemon, getPoke->lengthOfPokemon);
		//recibirIdGet(socketGet);
	}
}

//TODO: verificar que si no lo recibe por un tiempo es que el broker no funciona
void recibirIdGet(int socketGet) {
	t_paquete* paqueteIdRecibido = recibir_mensaje(socketGet);
	t_respuesta_id* idGet = paqueteIdRecibido->buffer->stream;
	agregarComoIdCorrelativoLocalized(idGet->idCorrelativo);
}

void agregarComoIdCorrelativoLocalized(int idCorrelativo){
	//puntero? 	o se podria inicializar en el main e ir pasando como parametro
	//lista de ids correlativos globales que se mandaron
	//recien se agregan cuando recibo la respuesta del broker
	list_add(idsCorrelativosLocalized, &idCorrelativo);
}

t_paquete* recibirLocalizedYGuardalos(int socketLocalized, t_list* pokemonesGlobales, t_list* pokemonesLibres) {
	t_paquete* paqueteLocalized = recibir_mensaje(socketLocalized);
	t_localized_pokemon* localized = paqueteLocalized->buffer->stream;
	//si el id correlativo del localized recibido coincide con algunos de los que tengo en mi lista de correlativos mandados
	//if(tieneComoIdCorrelativoLocalized(paqueteLocalized->ID_CORRELATIVO) == 1) {
		for(int index=0; index<localized->cantidadPosiciones; index++){
			//cada posicion recibida en el localized del poke que necesito cazar la agrego en la lista de pokemonesLibres
			t_posicion* posicion = list_get(localized->listaPosiciones,index);
			agregarPosicionSiLoNecesita(localized->pokemon, *posicion, pokemonesGlobales, pokemonesLibres);
		}

	return paqueteLocalized;
}

int tieneComoIdCorrelativoLocalized(int idBuscado) {
	typedef bool(*erasedTypeFind)(void*);

	int existe(int idExistente) {
		return idExistente == idBuscado;
	}
	if(list_is_empty(idsCorrelativosLocalized) == 1) {
		return 0;
	}
	//me fijo de la lista de idsCorrelativos que mande como get, si coincide con el id del que recien llego
	return list_find(idsCorrelativosLocalized, (erasedTypeFind)existe) != NULL;
}

//////////APPEARED//////////////
t_paquete* recibirAppearedYGuardarlos(int socketAppeared, t_list* pokemonesGlobales, t_list* pokemonesLibres) {

	t_log* logger = iniciar_logger();
	t_paquete* paqueteAppeared = recibir_mensaje(socketAppeared);
	t_appeared_pokemon* appeared = paqueteAppeared->buffer->stream;

	agregarPosicionSiLoNecesita(appeared->pokemon, *(appeared->posicion), pokemonesGlobales, pokemonesLibres);

//	free(appeared->pokemon);
//	free(appeared->posicion);
//	free(appeared);
//	free(paqueteAppeared->buffer);
//	free(paqueteAppeared);
	log_info(logger, "Se recibio el appeared | Pokemon: %s - Posicion X: %d - Posicion Y: %d", appeared->pokemon, appeared->posicion->posicionX, appeared->posicion->posicionY);;
	log_info(logger, "Ahora la cantidad de pokemones libres es: %d", list_size(pokemonesLibres));
	if(buscarPorNombre(appeared->pokemon, pokemonesGlobales) != NULL){
		log_info(logger, "La cantidad de pokemones %s en esa posicion es: %d", appeared->pokemon, (buscarPorNombre(appeared->pokemon, pokemonesLibres))->cantidad);
	}
	return paqueteAppeared;
}

//si tengo ese pokemon como objetivo lo agregego en la lista de pokemones libres
void agregarPosicionSiLoNecesita(char* nombreNuevoPoke, t_posicion posicionNuevoPoke, t_list* pokemonesGlobales, t_list* pokemonesLibres){
	t_log* logger = iniciar_logger();
	//si ese pokemon lo tengo como objetivo
	if(buscarPorNombre(nombreNuevoPoke, pokemonesGlobales) != NULL) {
		//ya tengo uno de esos pokes libres en el mapa y esta en la misma posicion
		if(buscarPorNombre(nombreNuevoPoke, pokemonesLibres) != NULL && sonLaMismaPosicion(buscarPorNombre(nombreNuevoPoke, pokemonesLibres)->posicion, posicionNuevoPoke)) {
			PokemonEnElMapa* pokeExistente = buscarPorNombre(nombreNuevoPoke, pokemonesLibres);
			pokeExistente->cantidad ++;
		} else {
			//solo lo agrego a la lista
			PokemonEnElMapa* pokemonNuevo = newPokemon();
			setPosicionTo(pokemonNuevo, posicionNuevoPoke);
			setNombreTo(pokemonNuevo, nombreNuevoPoke);
			setCantidadTo(pokemonNuevo, 1);
			list_add(pokemonesLibres, pokemonNuevo);


			Entrenador* entrenadorMasCercano = entrenadorMasCercanoA(pokemonNuevo, entrenadores);
			log_info(logger, "El entranador mas cercano a este nuevo pokemon esta en la posicion (%d, %d)", entrenadorMasCercano->posicion->posicionX, entrenadorMasCercano->posicion->posicionY);
		}
	}
}

//////////CATCH-CAUGHT/////////////
// cuando un entrenador manda este mensaje, ese poke ya no deberia estar en la lista de pokes libres
void enviarCatchDesde(Entrenador* entrenadorEsperando, int socketGet){
	quickLog("Esta enviando los catch por cada pokemon que esta por ser atrapado");
	PokemonEnElMapa* pokemonPorAtrapar = entrenadorEsperando->movimientoEnExec->pokemonNecesitado;
	t_catch_pokemon* catchPoke = crearEstructuraCatchDesde(pokemonPorAtrapar);
	enviar_catch_pokemon(catchPoke, socketCatch, -1, -1);
	//el entrenador que mando el catch de ese pokemon necesita guardarse el id de ese que mando
	//para saber saber que respuesta de caught es de el
	quickLog("Esta esperando recibir el id de su catch enviado");
	recibirIdCatch(socketIdCatch, entrenadorEsperando);
}

void recibirIdCatch(int socketIdCatch, Entrenador* entrenador) {
	t_paquete* paqueteIdRecibido = recibir_mensaje(socketIdCatch);
	t_respuesta_id* idCatch = paqueteIdRecibido->buffer->stream;
	agregarComoIdCorrelativoCaught(idCatch->idCorrelativo);
	entrenador->idCorrelativoDeEspera = idCatch->idCorrelativo;
}

void agregarComoIdCorrelativoCaught(int idCorrelativo){
	//lista de ids correlativos globales que se mandaron
	//recien se agregan cuando recibo la respuesta del broker
	list_add(idsCorrelativosCaught, &idCorrelativo);
}

t_paquete* recibirCaught(int socketCaught){
	t_paquete* paqueteCaught = recibir_mensaje(socketCaught);

	if(tieneComoIdCorrelativoCaught(paqueteCaught->ID_CORRELATIVO) == 1) {
		pthread_mutex_lock(&mutexEntrenadores);
		//el entrenador que hizo el catch del caught respondido cambia de estado de acuerdo a la respuesta
		ejecutarRespuestaCaught(paqueteCaught->ID_CORRELATIVO, paqueteCaught);
		pthread_mutex_unlock(&mutexEntrenadores);
		return paqueteCaught;
	}
	return NULL;
}

int tieneComoIdCorrelativoCaught(int idBuscado) {
	typedef bool(*erasedTypeFind)(void*);

	int existe(uint32_t idExistente) {
		return idExistente == idBuscado;
	}

	if(list_is_empty(idsCorrelativosCaught) == 1) {
		return 0;
	}
	//me fijo de la lista de idsCorrelativos que mande como catch, si coincide con el id del que recien llego
	return list_find(idsCorrelativosCaught, (erasedTypeFind)existe) != NULL;
}

void ejecutarRespuestaCaught(int idCatchQueResponde, t_paquete* paqueteCaught){
	t_caught_pokemon* caught = paqueteCaught->buffer->stream;
	Entrenador* entrenador = entrenadorQueTieneId(idCatchQueResponde);
	procesarEspera(entrenador, caught->ok);
}

Entrenador* entrenadorQueTieneId(int idCatchQueResponde) {
	int tieneIdCorrelativo(Entrenador* entrenador) {
		return entrenador->idCorrelativoDeEspera = idCatchQueResponde;
	}
	//ya estaba el mutex al llamar esta funcion o mejor aca??? TODO
	return list_find(entrenadores, (erasedTypeFilter)tieneIdCorrelativo);
}

void procesarEspera(Entrenador*  entrenador, uint32_t atrapo){
	PokemonEnElMapa* pokemonAtrapado = entrenador->movimientoEnExec->pokemonNecesitado;
	//si lo atrapo
	if(atrapo){
		agregarAtrapado(entrenador, pokemonAtrapado);
		estadoSiAtrapo(entrenador);
	}
	//no lo atrapo
	else {
		pthread_mutex_lock(&mutexObjetivosGlobales);
		//lo agrego devuelta a la lista de objetivos globales para que otro entrenador lo atrape
		setPokemonA(objetivosGlobales, pokemonAtrapado);
		pthread_mutex_lock(&mutexObjetivosGlobales);

		pasarADormido(entrenador);
	}
}


