/*
 * conexion.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "Broker.h"

void * esperarClientes() {

	int conexionCliente = crearSocket();
	if (escuchaEn(conexionCliente, PUERTO_BROKER)) {
		log_info(logger, "Escuchando conexiones al Broker");
	}

	while (1) {
		log_debug(logger, "Esperando conexiones...");
		int socketCliente = aceptarConexion(conexionCliente);

		/*prueba
		 */
		argumentos* sockets = malloc(sizeof(argumentos));
		sockets->socketNuevo = &socketCliente;
		sockets->socketOG = &conexionCliente;

		pthread_t thread;
//			pthread_create(&thread, NULL, (void*) atenderCliente, &socketCliente);
		pthread_create(&thread, NULL, (void*) atenderCliente, sockets);
		pthread_detach(thread);

		sleep(2);
	}
}

void atenderCliente(argumentos* sockets) {
	log_debug(logger, "START: atenderCliente");

	t_handshake * broker = malloc(sizeof(t_handshake));
	broker->id = BROKER;
	broker->idUnico = 100;
	t_handshake * idProcesoConectado = iniciarHandshake(*(sockets->socketNuevo), broker);
	log_info(logger, "El ID del proceso con el que me conecte es: %s",ID_PROCESO[idProcesoConectado->id]);
	free(broker);

	int* cliente = sockets->socketOG;

	switch (idProcesoConectado->id) {
	case TEAM:;
		t_suscriptor_team* team = malloc(sizeof(t_suscriptor_team));
		team->id = idProcesoConectado->idUnico;
		team->socketGet = aceptarConexion(*cliente);
		team->socketIdGet = aceptarConexion(*cliente);
		team->socketLocalized = aceptarConexion(*cliente);
		team->socketACKLocalized = aceptarConexion(*cliente);
		team->socketAppeared = aceptarConexion(*cliente);
		team->socketACKAppeared = aceptarConexion(*cliente);
		team->socketCatch = aceptarConexion(*cliente);
		team->socketIdCatch = aceptarConexion(*cliente);
		team->socketCaught = aceptarConexion(*cliente);
		team->socketACKCaught = aceptarConexion(*cliente);

		int existeTeam = check_si_existe_team(team->id);

		if(existeTeam == 1){
			reemplazar_suscriptor_team(team);
		}
		else{
			agregar_suscriptor_team(team);
		}

		pthread_mutex_lock(&mutexColas);
		agregarSuscriptorACola(team->id, LOCALIZED_POKEMON);
		t_list* mensajesLocalized = mensajesAEnviar(team->id, LOCALIZED_POKEMON);
		agregarSuscriptorACola(team->id, APPEARED_POKEMON);
		t_list* mensajesAppeared = mensajesAEnviar(team->id, APPEARED_POKEMON);
		agregarSuscriptorACola(team->id, CAUGHT_POKEMON);
		t_list* mensajesCaught = mensajesAEnviar(team->id, CAUGHT_POKEMON);
		pthread_mutex_unlock(&mutexColas);

		lanzarHiloEscucha(team->id, &team->socketGet);
		lanzarHiloEscucha(team->id, &team->socketCatch);
		lanzarHiloEscuchaACK(team->id, &team->socketACKLocalized, LOCALIZED_POKEMON);
		lanzarHiloEscuchaACK(team->id, &team->socketACKCaught, CAUGHT_POKEMON);
		lanzarHiloEscuchaACK(team->id, &team->socketACKAppeared, APPEARED_POKEMON);

		pthread_mutex_lock(&mutexEnvio);
		enviar_mensajes_cacheados(mensajesLocalized, LOCALIZED_POKEMON, &team->socketLocalized);
		enviar_mensajes_cacheados(mensajesAppeared, APPEARED_POKEMON, &team->socketAppeared);
		enviar_mensajes_cacheados(mensajesCaught, CAUGHT_POKEMON, &team->socketCaught);
		pthread_mutex_unlock(&mutexEnvio);

		log_info(logger, "Team se suscribio a 3 colas");
		close(*cliente);
		break;

	case GAMECARD:;
		t_suscriptor_gamecard* gamecard = malloc(sizeof(t_suscriptor_gamecard));
		gamecard->id = idProcesoConectado->idUnico;
		gamecard->socketNew = aceptarConexion(*cliente);
		log_debug(logger, "socketNew: %d", gamecard->socketNew);
		gamecard->socketACKNew = aceptarConexion(*cliente);
		log_debug(logger, "socketACKNew: %d", gamecard->socketACKNew);
		gamecard->socketCatch = aceptarConexion(*cliente);
		log_debug(logger, "socketCatch: %d", gamecard->socketCatch);
		gamecard->socketACKCatch = aceptarConexion(*cliente);
		log_debug(logger, "socketACKCatch: %d", gamecard->socketACKCatch);
		gamecard->socketGet = aceptarConexion(*cliente);
		log_debug(logger, "socketGet: %d", gamecard->socketGet);
		gamecard->socketACKGet = aceptarConexion(*cliente);
		log_debug(logger, "socketACKGet: %d", gamecard->socketACKGet);
		gamecard->socketAppeared = aceptarConexion(*cliente);
		log_debug(logger, "socketAppeared: %d", gamecard->socketAppeared);
		gamecard->socketCaught = aceptarConexion(*cliente);
		log_debug(logger, "socketCaught: %d", gamecard->socketCaught);
		gamecard->socketLocalized = aceptarConexion(*cliente);
		log_debug(logger, "socketLocalized: %d", gamecard->socketLocalized);

		if(check_si_existe_gamecard(gamecard->id)){
			reemplazar_suscriptor_gamecard(gamecard);
		}

		else{
			agregar_suscriptor_gamecard(gamecard);
		}

		pthread_mutex_lock(&mutexColas);
		agregarSuscriptorACola(gamecard->id, GET_POKEMON);
		t_list* mensajesGet = mensajesAEnviar(gamecard->id, GET_POKEMON);
		agregarSuscriptorACola(gamecard->id, CATCH_POKEMON);
		t_list* mensajesCatch = mensajesAEnviar(gamecard->id, CATCH_POKEMON);
		agregarSuscriptorACola(gamecard->id, NEW_POKEMON);
		t_list* mensajesNew = mensajesAEnviar(gamecard->id, NEW_POKEMON);
		pthread_mutex_unlock(&mutexColas);

		lanzarHiloEscucha(gamecard->id, &(gamecard->socketAppeared));
		lanzarHiloEscucha(gamecard->id, &gamecard->socketLocalized);
		lanzarHiloEscucha(gamecard->id, &gamecard->socketCaught);
		lanzarHiloEscuchaACK(gamecard->id, &gamecard->socketACKNew, NEW_POKEMON);
		lanzarHiloEscuchaACK(gamecard->id, &gamecard->socketACKGet, GET_POKEMON);
		lanzarHiloEscuchaACK(gamecard->id, &gamecard->socketACKCatch, CATCH_POKEMON);

		pthread_mutex_lock(&mutexEnvio);
		enviar_mensajes_cacheados(mensajesGet, GET_POKEMON, &gamecard->socketGet);
		enviar_mensajes_cacheados(mensajesCatch, CATCH_POKEMON, &gamecard->socketCatch);
		enviar_mensajes_cacheados(mensajesNew, NEW_POKEMON, &gamecard->socketNew);
		pthread_mutex_unlock(&mutexEnvio);

		log_info(logger, "GameCard se suscribio a 3 colas");
//			close(*cliente);
		break;

	case GAMEBOY:;
		t_paquete* mensajeRecibido = recibir_mensaje(*(sockets->socketNuevo));

		if(mensajeRecibido == NULL){
			free(idProcesoConectado);
			return;
		}

		/* El gameboy se suscribe
		 * a alguna cola
		 */
		if(mensajeRecibido->codigo_operacion == GAMEBOYSUSCRIBE){
			log_debug(logger, "Recibimos un GAMEBOYSUSCRIBE");
			t_gameboy_suscribe* suscribe = mensajeRecibido->buffer->stream;
			t_suscriptor_gameboy* gameboy;

			// Si existe gameboy con ese ID le cambio el socket a enviar
			if(check_si_existe_gameboy(idProcesoConectado->idUnico) == 1){
				log_debug(logger, "GameBoy existe en nuestro repositorio. Se procede a modificarlo.");
				gameboy = buscar_suscriptor_gameboy(idProcesoConectado->idUnico);
				gameboy->socketDondeEscucha = *cliente;

				//Lo elimino de la cola donde estaba antes
				pthread_mutex_lock(&mutexColas);
				for(int i = 0; i < 6; i++) {
					if(cola[i].nombreCola == gameboy->colaEscuchando){

						int index = -1;
						int cantidadSuscriptores = list_size(cola[i].suscriptores);

						for(int j = 0; j < cantidadSuscriptores; j++){
							int ID = (int) list_get(cola[i].suscriptores, j);
							if(ID == idProcesoConectado->idUnico){
								index = j;
							}
						}

						if (index != -1){
							list_remove_and_destroy_element(cola[i].suscriptores, index, (void*) liberoPuntero);
						}
					}
				}
				pthread_mutex_unlock(&mutexColas);

				gameboy->colaEscuchando = suscribe->codigoCola;
			}

			else{ // Si no existe gameboy con ese ID lo creo
				log_debug(logger, "GameBoy no existe en nuestro repositorio. Se procede a agregarlo.");
				gameboy = malloc(sizeof(t_suscriptor_gameboy));
				gameboy->id = idProcesoConectado->idUnico;
				gameboy->socketDondeEscucha = aceptarConexion(*cliente);
				gameboy->colaEscuchando = suscribe->codigoCola;
				agregar_suscriptor_gameboy(gameboy);
			}

			pthread_mutex_lock(&mutexColas);
			agregarSuscriptorACola(gameboy->id, suscribe->codigoCola);
			t_list* mensajes = mensajesAEnviar(gameboy->id, suscribe->codigoCola);
			pthread_mutex_unlock(&mutexColas);

			pthread_mutex_lock(&mutexEnvio);
			enviar_mensajes_cacheados(mensajes, suscribe->codigoCola, &gameboy->socketDondeEscucha);
			pthread_mutex_unlock(&mutexEnvio);

			free(mensajeRecibido->buffer->stream);
			free(mensajeRecibido->buffer);
			free(mensajeRecibido);
		}

		/* El gameboy envia un mensaje
		 * que debemos agregar a la cola
		 */
		else{
			pthread_mutex_lock(&mutexColas);
			log_info(logger, "Recibimos un mensaje de tipo %d", mensajeRecibido->codigo_operacion);
			agregarMensajeACola(mensajeRecibido);
			log_info(logger, "MensajeAgregado");
			pthread_mutex_unlock(&mutexColas);

			pthread_mutex_lock(&mutexEnvio);
			enviar_mensaje_a_suscriptores(mensajeRecibido);
			pthread_mutex_unlock(&mutexEnvio);
		}
		break;

	default:
		break;
	}

	free(idProcesoConectado);
}

void enviar_mensajes_cacheados(t_list* mensajes, op_code tipoDeMensaje, int* socket){
	log_debug(logger, "START: enviar_mensajes_cacheados");

	int cantidadMensajes = list_size(mensajes);
	log_debug(logger, "Cantidad de mensajes cacheados a enviar: %d", cantidadMensajes);

	switch(tipoDeMensaje){
		case NEW_POKEMON:
			for(int i = 0; i < cantidadMensajes; i+=3){
				t_new_pokemon* new_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				int resultadoEnvio = enviar_new_pokemon(new_pokemon, *socket, *id, *idCorrelativo);
				if (resultadoEnvio == -1){
					log_info(logger, "Fallo en envio de mensaje.");
				}
			}
			break;

		case APPEARED_POKEMON:
			for(int i = 0; i < cantidadMensajes; i+=3){
				t_appeared_pokemon* appeared_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				enviar_appeared_pokemon(appeared_pokemon, *socket, *id, *idCorrelativo);
			}
			break;

		case CATCH_POKEMON:
			for(int i = 0; i < cantidadMensajes; i+=3){
				t_catch_pokemon* catch_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				enviar_catch_pokemon(catch_pokemon, *socket, *id, *idCorrelativo);
			}
			break;

		case CAUGHT_POKEMON:
			for (int i = 0; i < cantidadMensajes; i += 3) {
				t_caught_pokemon* caught_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				enviar_caught_pokemon(caught_pokemon, *socket, *id, *idCorrelativo);
			}
			break;

		case GET_POKEMON:
			for (int i = 0; i < cantidadMensajes; i += 3) {
				t_get_pokemon* get_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				enviar_get_pokemon(get_pokemon, *socket, *id, *idCorrelativo);
			}
			break;

		case LOCALIZED_POKEMON:
			for (int i = 0; i < cantidadMensajes; i += 3) {
				t_localized_pokemon* localized_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				enviar_localized_pokemon(localized_pokemon, *socket, *id, *idCorrelativo);
			}
			break;

		default:
			break;
	}

	list_clean(mensajes);
	list_destroy(mensajes);
	log_debug(logger, "END: enviar_mensajes_cacheados");
}

void lanzarHiloEscucha(int id, int* socket){
	t_args_socket_escucha* argumentos = malloc(sizeof(t_args_socket_escucha));
	argumentos->id = id;
	argumentos->socket = socket;

	pthread_t hilo;
	pthread_create(&hilo, NULL, (void*) escucharSocketMensajesACachear, argumentos);
	pthread_detach(hilo);
}

void lanzarHiloEscuchaACK(int id, int* socket, op_code cola){
	t_args_socket_ACK* argumentos = malloc(sizeof(t_args_socket_ACK));
	argumentos->id = id;
	argumentos->socket = socket;
	argumentos->cola = cola;

	pthread_t hilo;
	pthread_create(&hilo, NULL, (void*) escucharSocketACK, argumentos);
	pthread_detach(hilo);
}

void escucharSocketMensajesACachear(t_args_socket_escucha* args){

	log_debug(logger, "START: escucharSocketMensajesACachear");

	log_debug(logger, "Socket escuchado: %d", *(args->socket));

	while(1){
		log_debug(logger, "Esperando mensajes...");
		t_paquete* paquete = recibir_mensaje(*(args->socket));

		/* Si se rompe el socket me da NULL
		 */
		if(paquete == NULL){
			log_debug(logger, "Conexión caída, se procede a cerrar el socket.");
			close(*args->socket);
			free(args);
			break;
		}

		log_debug(logger, "Se recibió un mensaje de tipo %d", paquete->codigo_operacion);

		pthread_mutex_lock(&mutexColas);
		agregarMensajeACola(paquete);
		pthread_mutex_unlock(&mutexColas);

		if(paquete->codigo_operacion == CATCH_POKEMON){
			t_respuesta_id* respuesta_id = malloc(sizeof(t_respuesta_id));
			respuesta_id->idCorrelativo = paquete->ID;

			if(check_si_existe_team(args->id) == 1){
				t_suscriptor_team* suscriptor = buscar_suscriptor_team(args->id);
				pthread_mutex_lock(&mutexEnvio);
				enviar_respuesta_id(respuesta_id, suscriptor->socketIdCatch, -1, -1);
				pthread_mutex_unlock(&mutexEnvio);
			}
		}

		else if(paquete->codigo_operacion == GET_POKEMON){
			t_respuesta_id* respuesta_id = malloc(sizeof(t_respuesta_id));
			respuesta_id->idCorrelativo = paquete->ID;

			if(check_si_existe_team(args->id) == 1){
				t_suscriptor_team* suscriptor = buscar_suscriptor_team(args->id);
				pthread_mutex_lock(&mutexEnvio);
				enviar_respuesta_id(respuesta_id, suscriptor->socketIdGet, -1, -1);
				pthread_mutex_unlock(&mutexEnvio);
			}
		}

		enviar_mensaje_a_suscriptores(paquete);
	}

	log_debug(logger, "END: escucharSocketMensajesACachear");
}

void escucharSocketACK(void* socketArgs){

	log_debug(logger, "START: escucharSocketACK");
	t_args_socket_ACK* args = socketArgs;

	while(1){
		t_paquete* paquete = recibir_mensaje(*(args->socket));

		if(paquete == NULL){
			log_debug(logger, "Conexión caída, se procede a cerrar el socket.");
			close(*args->socket);
			free(args);
			break;
		}

		pthread_mutex_lock(&mutexColas);

		/* Agrego el ID del proceso a la lista de ACK del mensaje
		 */
		for(int i = 0; i < 6; i++) {
			if(cola[i].nombreCola == args->cola){
				int cantidadMensajes = list_size(cola[i].mensajes);
				/* Recorro la lista de mensajes hasta dar con el indicado
				 */
				for(int j = 0; j < cantidadMensajes; j++){
					t_metadata* mensaje = list_get(cola[i].mensajes, j);
					/* Agrego el ID del proceso a la lista de ACK del mensaje
					 */
					if(mensaje->ID == paquete->ID_CORRELATIVO){
						list_add(mensaje->ACKSuscriptores, &args->id);
					}

					/* Chequeo si se llego a la cantidad necesaria de ACK
					 * Si tengo los suficientes, elimino el mensaje
					 * Para eso tengo que removerlo de la lista de mensajes
					 * Luego liberar los elementos de la lista ACK de ese mensaje,
					 * Destruir la lista ACK y finalmente liberar el mensaje
					 */

					if(list_size(mensaje->ACKSuscriptores) == list_size(cola[i].suscriptores)){
						list_remove_and_destroy_element(cola[i].mensajes, j, (void*) liberoMensaje);
					}
				}
			}
		}
		pthread_mutex_unlock(&mutexColas);
	}

	log_debug(logger, "START: escucharSocketACK");
}

void liberoMensaje(t_metadata* mensaje){
	list_destroy_and_destroy_elements(mensaje->ACKSuscriptores, (void*) liberoPuntero);
	free(mensaje);
}

void liberoPuntero(int* puntero){
	free(puntero);
}

void enviar_mensaje_a_suscriptores(t_paquete* paquete){

	pthread_t threadEnvio;

	switch(paquete->codigo_operacion){
		case NEW_POKEMON:
			pthread_create(&threadEnvio, NULL, (void*) enviar_mensaje_NEW_a_suscriptores, (void*) paquete);
			break;
		case APPEARED_POKEMON:
			pthread_create(&threadEnvio, NULL, (void*) enviar_mensaje_APPEARED_a_suscriptores, (void*) paquete);
			break;
		case CATCH_POKEMON:
			pthread_create(&threadEnvio, NULL, (void*) enviar_mensaje_CATCH_a_suscriptores, (void*) paquete);
			break;
		case CAUGHT_POKEMON:
			pthread_create(&threadEnvio, NULL, (void*) enviar_mensaje_CAUGHT_a_suscriptores, (void*) paquete);
			break;
		case GET_POKEMON:
			pthread_create(&threadEnvio, NULL, (void*) enviar_mensaje_GET_a_suscriptores, (void*) paquete);
			break;
		case LOCALIZED_POKEMON:
			pthread_create(&threadEnvio, NULL, (void*) enviar_mensaje_LOCALIZED_a_suscriptores, (void*) paquete);
			break;
		default:
			break;
	}
	pthread_detach(threadEnvio);
}

void enviar_mensaje_NEW_a_suscriptores(void* paqueteVoid){
	t_paquete* paquete = (t_paquete*) paqueteVoid;
	t_new_pokemon* new_pokemon = paquete->buffer->stream;

	for(int i = 0; i < 6; i++) {
		if(cola[i].nombreCola == paquete->codigo_operacion){

			pthread_mutex_lock(&mutexColas);
			t_list* listaDeSuscriptores = list_duplicate(cola[i].suscriptores);
			pthread_mutex_unlock(&mutexColas);

			int socketAUsar = 0;
			int listaSize = list_size(listaDeSuscriptores);
			log_debug(logger, "Se procede a mandar el mensaje a los %d suscriptores de la cola %d.", listaSize, paquete->codigo_operacion);

			for(int j = 0; j < listaSize; j++){
				int* idSuscriptor = list_get(listaDeSuscriptores, j);
				if(check_si_existe_gamecard(*idSuscriptor)){
					t_suscriptor_gamecard* gamecard = buscar_suscriptor_gamecard(*idSuscriptor);
					socketAUsar = gamecard->socketNew;
				}
				else if(check_si_existe_gameboy(*idSuscriptor)){
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(*idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
				}

				else{
					continue;
				}

				/* Duplico la estructura
				 * para no borrar la original
				 */
				t_new_pokemon* new_a_enviar = malloc(sizeof(t_new_pokemon));
				new_a_enviar->lengthOfPokemon = new_pokemon->lengthOfPokemon;
				new_a_enviar->pokemon = string_duplicate(new_pokemon->pokemon);
				new_a_enviar->posicion = malloc(sizeof(t_posicion));
				new_a_enviar->posicion->posicionX = new_pokemon->posicion->posicionX;
				new_a_enviar->posicion->posicionY = new_pokemon->posicion->posicionY;
				new_a_enviar->cantidad = new_pokemon->cantidad;

				enviar_new_pokemon(new_a_enviar, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);
			}

			list_destroy(listaDeSuscriptores);
		}
	}

	free(new_pokemon->pokemon);
	free(new_pokemon->posicion);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje_APPEARED_a_suscriptores(void* paqueteVoid){
	t_paquete* paquete = (t_paquete*) paqueteVoid;
	t_appeared_pokemon* appeared_pokemon = paquete->buffer->stream;

	for(int i = 0; i < 6; i++) {
		if(cola[i].nombreCola == paquete->codigo_operacion){

			pthread_mutex_lock(&mutexColas);
			t_list* listaDeSuscriptores = list_duplicate(cola[i].suscriptores);
			pthread_mutex_unlock(&mutexColas);

			int socketAUsar = 0;
			int listaSize = list_size(listaDeSuscriptores);

			for(int j = 0; j < listaSize; j++){
				int* idSuscriptor = list_get(listaDeSuscriptores, j);
				if(check_si_existe_team(*idSuscriptor)){
					t_suscriptor_team* team = buscar_suscriptor_team(*idSuscriptor);
					socketAUsar = team->socketAppeared;
				}
				else if(check_si_existe_gameboy(*idSuscriptor)){
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(*idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
				}

				else{
					continue;
				}

				/* Duplico la estructura
				 * para no borrar la original
				 */
				t_appeared_pokemon* appeared_a_enviar = malloc(sizeof(t_appeared_pokemon));
				appeared_a_enviar->lengthOfPokemon = appeared_pokemon->lengthOfPokemon;
				appeared_a_enviar->pokemon = string_duplicate(appeared_pokemon->pokemon);
				appeared_a_enviar->posicion = malloc(sizeof(t_posicion));
				appeared_a_enviar->posicion->posicionX = appeared_pokemon->posicion->posicionX;
				appeared_a_enviar->posicion->posicionY = appeared_pokemon->posicion->posicionY;

				enviar_appeared_pokemon(appeared_a_enviar, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);
			}

			list_destroy(listaDeSuscriptores);
		}
	}

	free(appeared_pokemon->pokemon);
	free(appeared_pokemon->posicion);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje_CATCH_a_suscriptores(void* paqueteVoid){
	t_paquete* paquete = (t_paquete*) paqueteVoid;
	t_catch_pokemon* catch_pokemon = paquete->buffer->stream;

	for(int i = 0; i < 6; i++) {
		if(cola[i].nombreCola == paquete->codigo_operacion){

			pthread_mutex_lock(&mutexColas);
			t_list* listaDeSuscriptores = list_duplicate(cola[i].suscriptores);
			pthread_mutex_unlock(&mutexColas);

			int socketAUsar = 0;
			int listaSize = list_size(listaDeSuscriptores);

			for(int j = 0; j < listaSize; j++){
				int* idSuscriptor = list_get(listaDeSuscriptores, j);
				if(check_si_existe_gamecard(*idSuscriptor)){
					t_suscriptor_gamecard* gamecard = buscar_suscriptor_gamecard(*idSuscriptor);
					socketAUsar = gamecard->socketCatch;
				}
				else if(check_si_existe_gameboy(*idSuscriptor)){
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(*idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
				}

				else{
					continue;
				}

				/* Duplico la estructura
				 * para no borrar la original
				 */
				t_catch_pokemon* catch_a_enviar = malloc(sizeof(t_catch_pokemon));
				catch_a_enviar->lengthOfPokemon = catch_pokemon->lengthOfPokemon;
				catch_a_enviar->pokemon = string_duplicate(catch_pokemon->pokemon);
				catch_a_enviar->posicion = malloc(sizeof(t_posicion));
				catch_a_enviar->posicion->posicionX = catch_pokemon->posicion->posicionX;
				catch_a_enviar->posicion->posicionY = catch_pokemon->posicion->posicionY;

				enviar_catch_pokemon(catch_a_enviar, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);
			}

			list_destroy(listaDeSuscriptores);
		}
	}

	free(catch_pokemon->pokemon);
	free(catch_pokemon->posicion);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje_CAUGHT_a_suscriptores(void* paqueteVoid){
	t_paquete* paquete = (t_paquete*) paqueteVoid;
	t_caught_pokemon* caught_pokemon = paquete->buffer->stream;

	for(int i = 0; i < 6; i++) {
		if(cola[i].nombreCola == paquete->codigo_operacion){

			pthread_mutex_lock(&mutexColas);
			t_list* listaDeSuscriptores = list_duplicate(cola[i].suscriptores);
			pthread_mutex_unlock(&mutexColas);

			int socketAUsar = 0;
			int listaSize = list_size(listaDeSuscriptores);

			for(int j = 0; j < listaSize; j++){
				int* idSuscriptor = list_get(listaDeSuscriptores, j);
				if(check_si_existe_team(*idSuscriptor)){
					t_suscriptor_team* team = buscar_suscriptor_team(*idSuscriptor);
					socketAUsar = team->socketCaught;
				}
				else if(check_si_existe_gameboy(*idSuscriptor)){
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(*idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
				}

				else{
					continue;
				}

				/* Duplico la estructura
				 * para no borrar la original
				 */
				t_caught_pokemon* caught_a_enviar = malloc(sizeof(t_caught_pokemon));
				caught_a_enviar->ok = caught_pokemon->ok;
				enviar_caught_pokemon(caught_a_enviar, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);
			}

			list_destroy(listaDeSuscriptores);
		}
	}

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje_GET_a_suscriptores(void* paqueteVoid){
	t_paquete* paquete = (t_paquete*) paqueteVoid;
	t_get_pokemon* get_pokemon = paquete->buffer->stream;

	for(int i = 0; i < 6; i++) {
		if(cola[i].nombreCola == paquete->codigo_operacion){

			pthread_mutex_lock(&mutexColas);
			t_list* listaDeSuscriptores = list_duplicate(cola[i].suscriptores);
			pthread_mutex_unlock(&mutexColas);

			int socketAUsar = 0;
			int listaSize = list_size(listaDeSuscriptores);

			for(int j = 0; j < listaSize; j++){
				int* idSuscriptor = list_get(listaDeSuscriptores, j);
				if(check_si_existe_gamecard(*idSuscriptor)){
					t_suscriptor_gamecard* gamecard = buscar_suscriptor_gamecard(*idSuscriptor);
					socketAUsar = gamecard->socketGet;
				}
				else if(check_si_existe_gameboy(*idSuscriptor)){
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(*idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
				}

				else{
					continue;
				}

				/* Duplico la estructura
				 * para no borrar la original
				 */
				t_get_pokemon* get_a_enviar = malloc(sizeof(t_get_pokemon));
				get_a_enviar->lengthOfPokemon = get_pokemon->lengthOfPokemon;
				get_a_enviar->pokemon = string_duplicate(get_pokemon->pokemon);
				enviar_get_pokemon(get_a_enviar, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);
			}

			list_destroy(listaDeSuscriptores);
		}
	}

	free(get_pokemon->pokemon);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje_LOCALIZED_a_suscriptores(void* paqueteVoid){
	t_paquete* paquete = (t_paquete*) paqueteVoid;
	t_localized_pokemon* localized_pokemon = paquete->buffer->stream;

	for(int i = 0; i < 6; i++) {
		if(cola[i].nombreCola == paquete->codigo_operacion){

			pthread_mutex_lock(&mutexColas);
			t_list* listaDeSuscriptores = list_duplicate(cola[i].suscriptores);
			pthread_mutex_unlock(&mutexColas);

			int socketAUsar = 0;
			int listaSize = list_size(listaDeSuscriptores);

			for(int j = 0; j < listaSize; j++){
				int* idSuscriptor = list_get(listaDeSuscriptores, j);
				if(check_si_existe_team(*idSuscriptor)){
					t_suscriptor_team* team = buscar_suscriptor_team(*idSuscriptor);
					socketAUsar = team->socketLocalized;
				}
				else if(check_si_existe_gameboy(*idSuscriptor)){
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(*idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
				}

				else{
					continue;
				}

				/* Duplico la estructura
				 * para no borrar la original
				 */
				t_localized_pokemon* localized_a_enviar = malloc(sizeof(t_localized_pokemon));
				localized_a_enviar->lengthOfPokemon = localized_pokemon->lengthOfPokemon;
				localized_a_enviar->pokemon = string_duplicate(localized_pokemon->pokemon);
				localized_a_enviar->cantidadPosiciones = localized_pokemon->cantidadPosiciones;
				localized_a_enviar->listaPosiciones = list_create();

				for(int h = 0; h < localized_pokemon->cantidadPosiciones; h++){
					t_posicion* posicionACopiar = list_get(localized_pokemon->listaPosiciones, h);
					t_posicion* posicion = malloc(sizeof(t_posicion));
					posicion->posicionX = posicionACopiar->posicionX;
					posicion->posicionY = posicionACopiar->posicionY;

					list_add(localized_a_enviar->listaPosiciones, posicion);
				}

				enviar_localized_pokemon(localized_pokemon, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);
			}

			list_destroy(listaDeSuscriptores);
		}
	}

	for(int i = 0; i < localized_pokemon->cantidadPosiciones; i++){
		t_posicion* posicionALiberar = list_get(localized_pokemon->listaPosiciones, i);
		free(posicionALiberar);
	}
	list_destroy(localized_pokemon->listaPosiciones);
	free(localized_pokemon->pokemon);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}
