
#include "EnvioMensajes.h"

//////////GET-LOCALIZED//////////////
void enviarGetDesde(t_list* objetivosGlobales, int socketGet){
	for(int index=0; index<list_size(objetivosGlobales);index++){
		t_get_pokemon* getPoke = crearEstructuraGetDesde(list_get(objetivosGlobales, index));
		enviar_get_pokemon(getPoke, socketGet, -1, -1);
		recibirIdGet(socketGet);
	}
}

void recibirIdGet(int socketGet) {
	t_paquete* paqueteIdRecibido = recibir_mensaje(socketGet);
	t_respuesta_id* idGet = paqueteIdRecibido->buffer->stream;
//	t_respuesta_id* idGet = malloc(sizeof(t_respuesta_id));
//	if(recv(socketGet, &(idGet), sizeof(uint32_t), 0) != -1) {
//		quickLog("Se pudo recibir el id del get enviado");
//	}
	agregarComoIdCorrelativoLocalized(idGet->idCorrelativo);
}

void recibirLocalizedYGuardalos(int socketLocalized, t_list* pokemonesGlobales, t_list* pokemonesLibres) {
	t_paquete* paqueteLocalized = recibir_mensaje(socketLocalized);
	//si el id correlativo del localized recibido coincide con algunos de los que tengo en mi lista de correlativos mandados
	if(tieneComoIdCorrelativoLocalized(paqueteLocalized->ID_CORRELATIVO) == 1) {
		t_localized_pokemon* localized = paqueteLocalized->buffer->stream;
		for(int index=0; index<localized->cantidadPosiciones; index++){
			//cada posicion recibida en el localized del poke que necesito cazar la agrego en la lista de pokemonesLibres
			agregarSiLoNecesita(list_get(localized->listaPosiciones,index), pokemonesGlobales, pokemonesLibres);
		}
	}

}

void agregarComoIdCorrelativoLocalized(uint32_t idCorrelativo){
	//puntero? 	o se podria inicializar en el main e ir pasando como parametro
	//lista de ids correlativos globales que se mandaron
	//recien se agregan cuando recibo la respuesta del broker
	idCorrelativosLocalized ids;
	list_add(ids.idsCorrelativos, idCorrelativo);
}

int tieneComoIdCorrelativoLocalized(uint32_t idBuscado) {
	idCorrelativosLocalized idsCorrelativos;
	int existe(uint32_t idExistente) {
		return idExistente == idBuscado;
	}
	if(list_is_empty(idsCorrelativos) == 1) {
		return 0;
	}
	//me fijo de la lista de idsCorrelativos que mande como get, si coincide con el id del que recien llego
	return list_find(idsCorrelativos, existe) != NULL;
}
//////////APPEARED//////////////
void recibirAppeared(int socketAppeared, t_list* pokemonesGlobales, t_list* pokemonesLibres) {
	t_paquete* paqueteAppeared = recibir_mensaje(socketAppeared);
	t_appeared_pokemon* appeared = paqueteAppeared->buffer->stream;

	agregarPosicionSiLoNecesita(appeared->pokemon, pokemonesGlobales);


}

//si tengo ese pokemon como objetivo lo agregego en la lista de pokemones libres
void agregarPosicionSiLoNecesita(char* nombrePokemon, t_list* pokemonesGlobales, t_list* pokemonesLibres){
	//si ese pokemon lo tengo como objetivo
	if(buscarPorNombre(nombrePokemon, pokemonesGlobales) != NULL) {
		PokemonEnElMapa* pokeNuevo = buscarPorNombre(nombrePokemon, pokemonesGlobales);
		//ya tengo uno de esos pokes libres en el mapa
		if(buscarPorNombre(nombrePokemon, pokemonesLibres) != NULL) {
			PokemonEnElMapa* pokeExistente = buscarPorNombre(nombrePokemon, pokemonesLibres)
			//si esta en la misma posicion
			if(pokeExistente->posicion == pokeNuevo->posicion){
				pokeExistente->cantidad ++;
			} else {
				//solo lo agrego a la lista
				list_add(pokemonesLibres, pokeNuevo);
			}
		}
	}
}

//////////CAGHT-CAUGHT//////////////

