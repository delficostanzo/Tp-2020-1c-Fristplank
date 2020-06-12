
#include "EnvioMensajes.h"

static void agregarPosicionSiLoNecesita(char* nombrePokemon, t_posicion posicion, t_list* pokemonesGlobales, t_list* pokemonesLibres);
static void agregarComoIdCorrelativoLocalized(uint32_t idCorrelativo);
static int tieneComoIdCorrelativoLocalized(uint32_t idBuscado);

//////////GET-LOCALIZED//////////////
void enviarGetDesde(t_list* objetivosGlobales, int socketGet){
	quickLog("Esta enviando los get por cada pokemon objetivo necesario");
	for(int index=0; index<list_size(objetivosGlobales);index++){
		t_get_pokemon* getPoke = crearEstructuraGetDesde(list_get(objetivosGlobales, index));
		enviar_get_pokemon(getPoke, socketGet, -1, -1);
		//recibirIdGet(socketGet);
	}
}

//TODO: verificar que si no lo recibe por un tiempo es que el broker no funciona
void recibirIdGet(int socketGet) {
	t_paquete* paqueteIdRecibido = recibir_mensaje(socketGet);
	t_respuesta_id* idGet = paqueteIdRecibido->buffer->stream;
	agregarComoIdCorrelativoLocalized(idGet->idCorrelativo);
}

void agregarComoIdCorrelativoLocalized(uint32_t idCorrelativo){
	idsCorrelativosLocalized* idsCorrelativos = malloc(sizeof(idsCorrelativosLocalized));
	//puntero? 	o se podria inicializar en el main e ir pasando como parametro
	//lista de ids correlativos globales que se mandaron
	//recien se agregan cuando recibo la respuesta del broker
	list_add(idsCorrelativos->idsCorrelativos, &idCorrelativo);
}

void recibirLocalizedYGuardalos(int socketLocalized, t_list* pokemonesGlobales, t_list* pokemonesLibres) {
	t_paquete* paqueteLocalized = recibir_mensaje(socketLocalized);
	t_localized_pokemon* localized = paqueteLocalized->buffer->stream;
	//si el id correlativo del localized recibido coincide con algunos de los que tengo en mi lista de correlativos mandados
	//if(tieneComoIdCorrelativoLocalized(paqueteLocalized->ID_CORRELATIVO) == 1) {
		for(int index=0; index<localized->cantidadPosiciones; index++){
			//cada posicion recibida en el localized del poke que necesito cazar la agrego en la lista de pokemonesLibres
			t_posicion* posicion = list_get(localized->listaPosiciones,index);
			agregarPosicionSiLoNecesita(localized->pokemon, *posicion, pokemonesGlobales, pokemonesLibres);
		}
}

int tieneComoIdCorrelativoLocalized(uint32_t idBuscado) {
	typedef bool(*erasedTypeFind)(void*);
	idsCorrelativosLocalized idsCorrelativos;
	int existe(uint32_t idExistente) {
		return idExistente == idBuscado;
	}
	if(list_is_empty(idsCorrelativos.idsCorrelativos) == 1) {
		return 0;
	}
	//me fijo de la lista de idsCorrelativos que mande como get, si coincide con el id del que recien llego
	return list_find(idsCorrelativos.idsCorrelativos, (erasedTypeFind)existe) != NULL;
}

//////////APPEARED//////////////
void recibirAppearedYGuardarlos(int socketAppeared, t_list* pokemonesGlobales, t_list* pokemonesLibres) {
	t_paquete* paqueteAppeared = recibir_mensaje(socketAppeared);
	t_appeared_pokemon* appeared = paqueteAppeared->buffer->stream;

	agregarPosicionSiLoNecesita(appeared->pokemon, *(appeared->posicion), pokemonesGlobales, pokemonesLibres);

	free(appeared->pokemon);
	free(appeared->posicion);
	free(appeared);
	free(paqueteAppeared->buffer);
	free(paqueteAppeared);
	quickLog("Aparecieron nuevos pokemones libres");
}

//si tengo ese pokemon como objetivo lo agregego en la lista de pokemones libres
void agregarPosicionSiLoNecesita(char* nombreNuevoPoke, t_posicion posicionNuevoPoke, t_list* pokemonesGlobales, t_list* pokemonesLibres){
	//si ese pokemon lo tengo como objetivo
	if(buscarPorNombre(nombreNuevoPoke, pokemonesGlobales) != NULL) {
		//ya tengo uno de esos pokes libres en el mapa
		if(buscarPorNombre(nombreNuevoPoke, pokemonesLibres) != NULL) {
			PokemonEnElMapa* pokeExistente = buscarPorNombre(nombreNuevoPoke, pokemonesLibres);
			//si esta en la misma posicion
			if(sonLaMismaPosicion(pokeExistente->posicion, posicionNuevoPoke)){
				pokeExistente->cantidad ++;
			} else {
				//solo lo agrego a la lista
				PokemonEnElMapa* pokemonNuevo = newPokemon();
				setPosicionTo(pokemonNuevo, posicionNuevoPoke);
				setNombreTo(pokemonNuevo, nombreNuevoPoke);
				setCantidadTo(pokemonNuevo, 1);
				list_add(pokemonesLibres, pokemonNuevo);
			}
		}
	}
}

//////////CAGHT-CAUGHT//////////////

