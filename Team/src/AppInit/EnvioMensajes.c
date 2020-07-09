
#include "EnvioMensajes.h"

static void agregarPokemonSiLoNecesita(char* nombrePokemon, t_posicion posicion);
static void agregarComoIdCorrelativoLocalized(int idCorrelativo);
static void agregarComoIdCorrelativoCaught(int idCorrelativo, Entrenador* entrenador);
static void recibirIdCatch(int socketIdCatch, Entrenador* entrenador);
//static int tieneComoIdCorrelativoLocalized(int idBuscado);
static int tieneComoIdCorrelativoCaught(int idBuscado);
static Entrenador* entrenadorQueTieneId(int idCatchQueResponde);
static void procesarEspera(Entrenador*  entrenador, uint32_t atrapo);

typedef bool(*erasedTypeFilter)(void*);

//////////GET-LOCALIZED//////////////
void enviarGetDesde(int socketGet){
	quickLog("Esta enviando los get por cada pokemon objetivo necesario");
	pthread_mutex_lock(&mutexObjetivosGlobales);
	for(int index=0; index<list_size(objetivosGlobales);index++){
		PokemonEnElMapa* poke = list_get(objetivosGlobales, index);
		t_get_pokemon* getPoke = crearEstructuraGetDesde(poke);
		enviar_get_pokemon(getPoke, socketGet, -1, -1);
		//recibirIdGet(socketGet);

		//free(getPoke); ->DOUBLE FREE
	}

	pthread_mutex_unlock(&mutexObjetivosGlobales);
}

//TODO: verificar que si no lo recibe por un tiempo es que el broker no funciona
void recibirIdGet(int socketGet) {
	t_paquete* paqueteIdRecibido = recibir_mensaje(socketGet);
	t_respuesta_id* idGet = paqueteIdRecibido->buffer->stream;
	agregarComoIdCorrelativoLocalized(idGet->idCorrelativo);
}

void agregarComoIdCorrelativoLocalized(int idCorrelativo){
	//lista de ids correlativos globales que se mandaron
	//recien se agregan cuando recibo la respuesta del broker

	list_add(idsCorrelativosLocalized, &idCorrelativo);
}

t_paquete* recibirLocalizedYGuardalos(int socketLocalized) {
	t_log* logger = iniciar_logger();
	t_paquete* paqueteLocalized = recibir_mensaje(socketLocalized);
	t_localized_pokemon* localized = paqueteLocalized->buffer->stream;
	//si el id correlativo del localized recibido coincide con algunos de los que tengo en mi lista de correlativos mandados
	//if(tieneComoIdCorrelativoLocalized(paqueteLocalized->ID_CORRELATIVO) == 1) {
	int cantidad = (int) (localized->cantidadPosiciones);
	for(int index=0; index<cantidad; index++){
		//cada posicion recibida en el localized del poke que necesito cazar la agrego en la lista de pokemonesLibres
		t_posicion* posicion = list_get(localized->listaPosiciones,index);

		agregarPokemonSiLoNecesita(localized->pokemon, *posicion);

	}

	log_info(logger, "Se recibieron el localized | Pokemon: %s | Cantidad de posiciones: %d ", localized->pokemon, localized->cantidadPosiciones);

	destruirLog(logger);
	return paqueteLocalized;
}

//int tieneComoIdCorrelativoLocalized(int idBuscado) {
//	typedef bool(*erasedTypeFind)(void*);
//
//	int existe(int idExistente) {
//		return idExistente == idBuscado;
//	}
//	if(list_is_empty(idsCorrelativosLocalized) == 1) {
//		return 0;
//	}
//	//me fijo de la lista de idsCorrelativos que mande como get, si coincide con el id del que recien llego
//	return list_find(idsCorrelativosLocalized, (erasedTypeFind)existe) != NULL;
//}

//////////APPEARED//////////////
t_paquete* recibirAppearedYGuardarlos(int socketAppeared) {

	quickLog("Esta por recibir el appeared");

	t_log* logger = iniciar_logger();
	t_paquete* paqueteAppeared = recibir_mensaje(socketAppeared);
	quickLog("Recibe el appeared");
	t_appeared_pokemon* appeared = paqueteAppeared->buffer->stream;


	agregarPokemonSiLoNecesita(appeared->pokemon, *(appeared->posicion));


//	free(appeared->pokemon);
//	free(appeared->posicion);
//	free(appeared);
//	free(paqueteAppeared->buffer);
//	free(paqueteAppeared);
	log_info(logger, "Se recibio el appeared | Pokemon: %s - Posicion X: %d - Posicion Y: %d", appeared->pokemon, appeared->posicion->posicionX, appeared->posicion->posicionY);

	destruirLog(logger);
	return paqueteAppeared;
}

//si tengo ese pokemon como objetivo lo agregego en la lista de pokemones libres
void agregarPokemonSiLoNecesita(char* nombreNuevoPoke, t_posicion posicionNuevoPoke){

	t_log* logger = iniciar_logger();
	//si ese pokemon lo tengo como objetivo
	pthread_mutex_lock(&mutexObjetivosGlobales);
	t_list* pokemonesAAtrapar = objetivosGlobales;
	//en el nombre de los globales hay cualquier cosa
	if(buscarPorNombre(nombreNuevoPoke, pokemonesAAtrapar) != NULL) {

		//ya tengo uno de esos pokes libres en el mapa y esta en la misma posicion
		pthread_mutex_lock(&mutexPokemonesLibres);
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
		}
		pthread_mutex_unlock(&mutexPokemonesLibres);
	}
	pthread_mutex_unlock(&mutexObjetivosGlobales);

	pthread_mutex_lock(&mutexPokemonesLibres);
	log_info(logger, "Ahora la cantidad de pokemones libres es: %d", list_size(pokemonesLibres));
	pthread_mutex_unlock(&mutexPokemonesLibres);

	destruirLog(logger);
}




//////////CATCH-CAUGHT/////////////
// cuando un entrenador manda este mensaje, ese poke ya no deberia estar en la lista de pokes libres
//funcion llamada con un semaforo -> solo 1 puede atrapar a la vez y agregar un id correlativo a la lista a la vez
void enviarCatchDesde(Entrenador* entrenadorEsperando){

	quickLog("Esta enviando los catch por cada pokemon que esta por ser atrapado");
	PokemonEnElMapa* pokemonPorAtrapar = entrenadorEsperando->movimientoEnExec->pokemonNecesitado;
	t_catch_pokemon* catchPoke = crearEstructuraCatchDesde(pokemonPorAtrapar);
	enviar_catch_pokemon(catchPoke, socketCatch, -1, -1);
	//el entrenador que mando el catch de ese pokemon necesita guardarse el id de ese que mando
	//para saber saber que respuesta de caught es de el
	quickLog("Esta esperando recibir el id de su catch enviado");
	recibirIdCatch(socketIdCatch, entrenadorEsperando);
	free(catchPoke);

}

void recibirIdCatch(int socketIdCatch, Entrenador* entrenador) {
	t_paquete* paqueteIdRecibido = recibir_mensaje(socketIdCatch);
	t_respuesta_id* idCatch = paqueteIdRecibido->buffer->stream;
	agregarComoIdCorrelativoCaught(idCatch->idCorrelativo, entrenador);
	quickLog("Se agrego como id en el entrenador necesario");



}

//no tengo como chequear que ese id sea el mio
void agregarComoIdCorrelativoCaught(int idCorrelativo, Entrenador* entrenador){
	t_log* logger = iniciar_logger();
	//lista de ids correlativos globales que se mandaron
	//recien se agregan cuando recibo la respuesta del broker

	entrenador->idCorrelativoDeEspera = idCorrelativo;

	list_add(idsCorrelativosCaught, idCorrelativo);
	log_info(logger, "Se registro el id del catch que mando el entrenador como id: %d", idCorrelativo);
	log_info(logger, "Ahora la cantidad de ids correlativos esperando respuestas caught es: %d", list_size(idsCorrelativosCaught));
	destruirLog(logger);
}

t_paquete* recibirCaught(int socketCaught){
	t_paquete* paqueteCaught = recibir_mensaje(socketCaught);
	quickLog("Se recibio un caught");

	if(tieneComoIdCorrelativoCaught(paqueteCaught->ID_CORRELATIVO) == 1) {
		//el entrenador que hizo el catch del caught respondido cambia de estado de acuerdo a la respuesta

		ejecutarRespuestaCaught(paqueteCaught->ID_CORRELATIVO, paqueteCaught);
		quickLog("El entrenador que esperaba el caught fue procesado");
		return paqueteCaught;
	}

	return NULL;
}

int tieneComoIdCorrelativoCaught(int idBuscado) {
	typedef bool(*erasedType)(void*);

	quickLog("Procesando respuesta caught util");
	int existe(int idExistente) {
		return idExistente == idBuscado;
	}

	//si la lista no esta vacia
	if(list_is_empty(idsCorrelativosCaught) != 1) {
		//me fijo de la lista de idsCorrelativos que mande como catch, si coincide con el id del que recien llego
		return list_any_satisfy(idsCorrelativosCaught, (erasedType)existe);
	}

	return 0;
}

void ejecutarRespuestaCaught(int idCatchQueResponde, t_paquete* paqueteCaught){
	t_caught_pokemon* caught = paqueteCaught->buffer->stream;
	//ya sabemos que algun entrenador tiene ese id como correlativo de espera
	Entrenador* entrenador = entrenadorQueTieneId(idCatchQueResponde);
	procesarEspera(entrenador, caught->ok);
	//el remove recibe el indice
	//list_remove(idsCorrelativosCaught, &idCatchQueResponde);
}

Entrenador* entrenadorQueTieneId(int idCatchQueResponde) {
	int tieneIdCorrelativo(Entrenador* entrenador) {
		return entrenador->idCorrelativoDeEspera == idCatchQueResponde;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	Entrenador* entrenadorRespondido = list_find(entrenadores, (erasedTypeFilter)tieneIdCorrelativo);
	pthread_mutex_unlock(&mutexEntrenadores);
	quickLog("Se encontro el entrenador que esperaba el caught");
	return entrenadorRespondido;
}

void procesarEspera(Entrenador*  entrenador, uint32_t atrapo){
	t_log* logger = iniciar_logger();
	PokemonEnElMapa* pokemonAtrapado = entrenador->movimientoEnExec->pokemonNecesitado;

	log_info(logger, "Se esta procesando el entrenador en espera de la respuesta");
	//si lo atrapo
	if(atrapo){
		agregarAtrapado(entrenador, pokemonAtrapado);
		quickLog("Se atrapo el pokemon");
		estadoSiAtrapo(entrenador);

	}
	//no lo atrapo
	else {
		pthread_mutex_lock(&mutexObjetivosGlobales);
		//lo agrego devuelta a la lista de objetivos globales para que otro entrenador lo atrape
		setPokemonA(objetivosGlobales, pokemonAtrapado);
		pthread_mutex_unlock(&mutexObjetivosGlobales);

		pasarADormido(entrenador);
	}

	log_info(logger, "Luego de recibir la respuesta del caught el entrenador quedo en estado %d", entrenador->estado);
	destruirLog(logger);
}


