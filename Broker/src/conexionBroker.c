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

		if(socketCliente != -1){
			t_handshake * broker = malloc(sizeof(t_handshake));
			broker->id = BROKER;
			broker->idUnico = 100;
			t_handshake * idProcesoConectado = iniciarHandshake(socketCliente, broker);
			free(broker);

			switch (idProcesoConectado->id) {
				case TEAM:;
					t_suscriptor_team* team = malloc(sizeof(t_suscriptor_team));
					team->id = idProcesoConectado->idUnico;
					team->socketGet = aceptarConexion(conexionCliente);
					team->socketIdGet = aceptarConexion(conexionCliente);
					team->socketAppeared = aceptarConexion(conexionCliente);
					team->socketLocalized = aceptarConexion(conexionCliente);
					team->socketCatch = aceptarConexion(conexionCliente);
					team->socketIdCatch = aceptarConexion(conexionCliente);
					team->socketCaught = aceptarConexion(conexionCliente);

					if(team->socketCaught != -1){
						atenderTeam(team);
					}
					free(idProcesoConectado);
					break;

				case GAMECARD:;
					t_suscriptor_gamecard* gamecard = malloc(sizeof(t_suscriptor_gamecard));
					gamecard->id = idProcesoConectado->idUnico;
					gamecard->socketNew = aceptarConexion(conexionCliente);
					gamecard->socketCatch = aceptarConexion(conexionCliente);
					gamecard->socketGet = aceptarConexion(conexionCliente);
					gamecard->socketAppeared = aceptarConexion(conexionCliente);
					gamecard->socketCaught = aceptarConexion(conexionCliente);
					gamecard->socketLocalized = aceptarConexion(conexionCliente);

					if(gamecard->socketLocalized != -1){
						atenderGamecard(gamecard);
					}
					free(idProcesoConectado);
					break;

				case GAMEBOY:;
					atenderGameboy(&socketCliente, idProcesoConectado->idUnico);
					free(idProcesoConectado);
					break;

				default:
					break;
			}
		}
	}
}

void atenderGameboy(int* socket, int idUnico){
	t_paquete* mensajeRecibido = recibir_mensaje(*(socket));

	if(mensajeRecibido == NULL){
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
		pthread_mutex_lock(&mutexRepoGameBoy);
		if(check_si_existe_gameboy(idUnico) == 1){
			log_debug(logger, "GameBoy existe en nuestro repositorio. Se procede a modificarlo.");
			gameboy = buscar_suscriptor_gameboy(idUnico);
			gameboy->socketDondeEscucha = *(socket);
			pthread_mutex_unlock(&mutexRepoGameBoy);

			//Lo elimino de la cola donde estaba antes
			pthread_mutex_lock(&mutexColas);
			for(int i = 0; i < 6; i++) {
				if(cola[i].nombreCola == gameboy->colaEscuchando){

					int index = -1;
					int cantidadSuscriptores = list_size(cola[i].suscriptores);

					for(int j = 0; j < cantidadSuscriptores; j++){
						int ID = (int) list_get(cola[i].suscriptores, j);
						if(ID == idUnico){
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
			pthread_mutex_unlock(&mutexRepoGameBoy);
			log_debug(logger, "GameBoy no existe en nuestro repositorio. Se procede a agregarlo.");
			gameboy = malloc(sizeof(t_suscriptor_gameboy));
			gameboy->id = idUnico;
			gameboy->socketDondeEscucha = *(socket);
			gameboy->colaEscuchando = suscribe->codigoCola;
			pthread_mutex_lock(&mutexRepoGameBoy);
			agregar_suscriptor_gameboy(gameboy);
			pthread_mutex_unlock(&mutexRepoGameBoy);
		}

		pthread_mutex_lock(&mutexColas);
		agregarSuscriptorACola(gameboy->id, suscribe->codigoCola);
		t_list* mensajes = mensajesAEnviar(gameboy->id, suscribe->codigoCola);
		pthread_mutex_unlock(&mutexColas);

		log_info(logger, "GameBoy se suscribió a la cola %s", ID_COLA[suscribe->codigoCola]);

		pthread_mutex_lock(&mutexEnvio);
		enviar_mensajes_cacheados(mensajes, suscribe->codigoCola, gameboy->socketDondeEscucha, gameboy->id);
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
		log_info(logger, "Mensaje recibido: %s", ID_COLA[mensajeRecibido->codigo_operacion]);
		agregarMensajeACola(mensajeRecibido);
		log_info(logger, "Mensaje agregado a memoria.");
		pthread_mutex_unlock(&mutexColas);

		enviar_mensaje_a_suscriptores(mensajeRecibido);
		log_info(logger, "Mensaje enviado a suscriptores.");
	}
}

void atenderTeam(t_suscriptor_team* team){

	pthread_mutex_lock(&mutexRepoTeam);
	if(check_si_existe_team(team->id) == 1){
		log_debug(logger, "TEAM ID [%d] YA EXISTIA EN BROKER", team->id);
		reemplazar_suscriptor_team(team);
	}
	else{
		log_debug(logger, "TEAM ID [%d] SE AGREGA", team->id);
		agregar_suscriptor_team(team);
	}
	pthread_mutex_unlock(&mutexRepoTeam);

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

	pthread_mutex_lock(&mutexEnvio);
	enviar_mensajes_cacheados(mensajesLocalized, LOCALIZED_POKEMON, team->socketLocalized, team->id);
	enviar_mensajes_cacheados(mensajesAppeared, APPEARED_POKEMON, team->socketAppeared, team->id);
	enviar_mensajes_cacheados(mensajesCaught, CAUGHT_POKEMON, team->socketCaught, team->id);
	pthread_mutex_unlock(&mutexEnvio);

	log_info(logger, "TEAM se suscribió a tres colas");
}

void atenderGamecard(t_suscriptor_gamecard* gamecard){
	pthread_mutex_lock(&mutexRepoGameCard);
	if(check_si_existe_gamecard(gamecard->id)){
		log_debug(logger, "GAMECARD ID [%d] YA EXISTIA EN BROKER", gamecard->id);
		reemplazar_suscriptor_gamecard(gamecard);
	}

	else{
		log_debug(logger, "GAMECARD ID [%d] SE AGREGA", gamecard->id);
		agregar_suscriptor_gamecard(gamecard);
	}
	pthread_mutex_unlock(&mutexRepoGameCard);

	pthread_mutex_lock(&mutexColas);
	agregarSuscriptorACola(gamecard->id, GET_POKEMON);
	t_list* mensajesGet = mensajesAEnviar(gamecard->id, GET_POKEMON);
	agregarSuscriptorACola(gamecard->id, CATCH_POKEMON);
	t_list* mensajesCatch = mensajesAEnviar(gamecard->id, CATCH_POKEMON);
	agregarSuscriptorACola(gamecard->id, NEW_POKEMON);
	t_list* mensajesNew = mensajesAEnviar(gamecard->id, NEW_POKEMON);
	pthread_mutex_unlock(&mutexColas);

	lanzarHiloEscucha(gamecard->id, &gamecard->socketAppeared);
	lanzarHiloEscucha(gamecard->id, &gamecard->socketLocalized);
	lanzarHiloEscucha(gamecard->id, &gamecard->socketCaught);

	pthread_mutex_lock(&mutexEnvio);
	enviar_mensajes_cacheados(mensajesGet, GET_POKEMON, gamecard->socketGet, gamecard->id);
	enviar_mensajes_cacheados(mensajesCatch, CATCH_POKEMON, gamecard->socketCatch, gamecard->id);
	enviar_mensajes_cacheados(mensajesNew, NEW_POKEMON, gamecard->socketNew, gamecard->id);
	pthread_mutex_unlock(&mutexEnvio);

	log_info(logger, "GAMECARD se suscribió a 3 colas.");
}

void enviar_mensajes_cacheados(t_list* mensajes, op_code tipoDeMensaje, int socket, int idProceso){
	log_debug(logger, "START: enviar_mensajes_cacheados");

	int cantidadMensajes = list_size(mensajes);
	log_info(logger, "Se procede a enviar los %d mensajes cacheados", cantidadMensajes / 4);

	switch(tipoDeMensaje){
		case NEW_POKEMON:
			for(int i = 0; i < cantidadMensajes; i+=3){
				t_new_pokemon* new_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				if (enviar_new_pokemon(new_pokemon, socket, *id, *idCorrelativo) == -1){
					log_error(logger, "Fallo en envio de mensaje.");
				}
				else{
					log_info(logger, "Mensaje cacheado %s enviado | ID Mensaje: %d", ID_COLA[tipoDeMensaje] , *id);
					pthread_mutex_lock(&mutexColas);
					agregarIdAEnviados(idProceso, *id, tipoDeMensaje);
					pthread_mutex_unlock(&mutexColas);

					t_paquete* ack = recibir_mensaje(socket);

					if(ack != NULL){
						pthread_mutex_lock(&mutexColas);
						procesarACK(ack, tipoDeMensaje, idProceso);
						pthread_mutex_unlock(&mutexColas);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", idProceso);
					}

				}
			}
			break;

		case APPEARED_POKEMON:
			for(int i = 0; i < cantidadMensajes; i+=3){
				t_appeared_pokemon* appeared_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				if(enviar_appeared_pokemon(appeared_pokemon, socket, *id, *idCorrelativo) == -1){
					log_error(logger, "Fallo en envio de mensaje.");
				}
				else{
					log_info(logger, "Mensaje cacheado %s enviado | ID Mensaje: %d", ID_COLA[tipoDeMensaje] , *id);
					pthread_mutex_lock(&mutexColas);
					agregarIdAEnviados(idProceso, *id, tipoDeMensaje);
					pthread_mutex_unlock(&mutexColas);

					t_paquete* ack = recibir_mensaje(socket);

					if(ack != NULL){
						pthread_mutex_lock(&mutexColas);
						procesarACK(ack, tipoDeMensaje, idProceso);
						pthread_mutex_unlock(&mutexColas);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", idProceso);
					}
				}
			}
			break;

		case CATCH_POKEMON:
			for(int i = 0; i < cantidadMensajes; i+=3){
				t_catch_pokemon* catch_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				if(enviar_catch_pokemon(catch_pokemon, socket, *id, *idCorrelativo) == -1){
					log_error(logger, "Fallo en envio de mensaje.");
				}
				else{
					log_info(logger, "Mensaje cacheado %s enviado | ID Mensaje: %d", ID_COLA[tipoDeMensaje] , *id);
					pthread_mutex_lock(&mutexColas);
					agregarIdAEnviados(idProceso, *id, tipoDeMensaje);
					pthread_mutex_unlock(&mutexColas);

					t_paquete* ack = recibir_mensaje(socket);

					if(ack != NULL){
						pthread_mutex_lock(&mutexColas);
						procesarACK(ack, tipoDeMensaje, idProceso);
						pthread_mutex_unlock(&mutexColas);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", idProceso);
					}
				}
			}
			break;

		case CAUGHT_POKEMON:
			for (int i = 0; i < cantidadMensajes; i += 3) {
				t_caught_pokemon* caught_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				if(enviar_caught_pokemon(caught_pokemon, socket, *id, *idCorrelativo) == -1){
					log_error(logger, "Fallo en envio de mensaje.");
				}
				else{
					log_info(logger, "Mensaje cacheado %s enviado | ID Mensaje: %d", ID_COLA[tipoDeMensaje] , *id);
					pthread_mutex_lock(&mutexColas);
					agregarIdAEnviados(idProceso, *id, tipoDeMensaje);
					pthread_mutex_unlock(&mutexColas);

					t_paquete* ack = recibir_mensaje(socket);

					if(ack != NULL){
						pthread_mutex_lock(&mutexColas);
						procesarACK(ack, tipoDeMensaje, idProceso);
						pthread_mutex_unlock(&mutexColas);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", idProceso);
					}
				}
			}
			break;

		case GET_POKEMON:
			for (int i = 0; i < cantidadMensajes; i += 3) {
				t_get_pokemon* get_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				if(enviar_get_pokemon(get_pokemon, socket, *id, *idCorrelativo) == -1){
					log_error(logger, "Fallo en envio de mensaje.");
				}
				else{
					log_info(logger, "Mensaje cacheado %s enviado | ID Mensaje: %d", ID_COLA[tipoDeMensaje] , *id);
					pthread_mutex_lock(&mutexColas);
					agregarIdAEnviados(idProceso, *id, tipoDeMensaje);
					pthread_mutex_unlock(&mutexColas);

					t_paquete* ack = recibir_mensaje(socket);

					if(ack != NULL){
						pthread_mutex_lock(&mutexColas);
						procesarACK(ack, tipoDeMensaje, idProceso);
						pthread_mutex_unlock(&mutexColas);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", idProceso);
					}

				}
			}
			break;

		case LOCALIZED_POKEMON:
			for (int i = 0; i < cantidadMensajes; i += 3) {
				t_localized_pokemon* localized_pokemon = list_get(mensajes, i);
				int* id = list_get(mensajes, i + 1);
				int* idCorrelativo = list_get(mensajes, i + 2);

				if(enviar_localized_pokemon(localized_pokemon, socket, *id, *idCorrelativo) == -1){
					log_error(logger, "Fallo en envio de mensaje.");
				}
				else{
					log_info(logger, "Mensaje cacheado %s enviado | ID Mensaje: %d", ID_COLA[tipoDeMensaje] , *id);
					pthread_mutex_lock(&mutexColas);
					agregarIdAEnviados(idProceso, *id, tipoDeMensaje);
					pthread_mutex_unlock(&mutexColas);

					t_paquete* ack = recibir_mensaje(socket);

					if(ack != NULL){
						pthread_mutex_lock(&mutexColas);
						procesarACK(ack, tipoDeMensaje, idProceso);
						pthread_mutex_unlock(&mutexColas);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", idProceso);
					}
				}
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

//void lanzarHiloEscuchaACK(int id, int* socket, op_code cola){
//	t_args_socket_ACK* argumentos = malloc(sizeof(t_args_socket_ACK));
//	argumentos->id = id;
//	argumentos->socket = socket;
//	argumentos->cola = cola;
//
//	pthread_t hilo;
//	pthread_create(&hilo, NULL, (void*) escucharSocketACK, argumentos);
//	pthread_detach(hilo);
//}

void escucharSocketMensajesACachear(t_args_socket_escucha* args){

	log_debug(logger, "START: escucharSocketMensajesACachear");

	while(1){
		log_debug(logger, "Esperando mensajes...");
		t_paquete* paquete = recibir_mensaje(*(args->socket));

		/* Si se rompe el socket me da NULL
		 */
		if(paquete == NULL){
			log_info(logger, "Se cerró socket de escucha a proceso ID [%d] por pérdida de conexión", args->id);
			close(*args->socket);
			free(args);
			break;
		}

		log_info(logger, "Mensaje recibido: %s | ID Único proceso: %d", ID_COLA[paquete->codigo_operacion], args->id);

		pthread_mutex_lock(&mutexColas);
		agregarMensajeACola(paquete);
		pthread_mutex_unlock(&mutexColas);

		if(paquete->codigo_operacion == CATCH_POKEMON){
			t_respuesta_id* respuesta_id = malloc(sizeof(t_respuesta_id));
			respuesta_id->idCorrelativo = paquete->ID;

			pthread_mutex_lock(&mutexRepoTeam);
			int existeTeam = check_si_existe_team(args->id);
			pthread_mutex_unlock(&mutexRepoTeam);

			if(existeTeam == 1){
				pthread_mutex_lock(&mutexRepoTeam);
				t_suscriptor_team* suscriptor = buscar_suscriptor_team(args->id);
				pthread_mutex_unlock(&mutexRepoTeam);

				enviar_respuesta_id(respuesta_id, suscriptor->socketIdCatch, -1, -1);
			}
		}

		else if(paquete->codigo_operacion == GET_POKEMON){
			t_respuesta_id* respuesta_id = malloc(sizeof(t_respuesta_id));
			respuesta_id->idCorrelativo = paquete->ID;

			pthread_mutex_lock(&mutexRepoTeam);
			int existeTeam = check_si_existe_team(args->id);
			pthread_mutex_unlock(&mutexRepoTeam);

			if(existeTeam == 1){
				pthread_mutex_lock(&mutexRepoTeam);
				t_suscriptor_team* suscriptor = buscar_suscriptor_team(args->id);
				pthread_mutex_unlock(&mutexRepoTeam);
				enviar_respuesta_id(respuesta_id, suscriptor->socketIdGet, -1, -1);
			}
		}

		/* Solamente para mostrar en debug que llega
		 *
		 */
		else if(paquete->codigo_operacion == LOCALIZED_POKEMON){
			t_localized_pokemon* localized = paquete->buffer->stream;
			log_debug(logger, "Me llegan %d cantidad", localized->cantidadPosiciones);


			char* posicionesImpresas = string_new();
			string_append(&posicionesImpresas, "[");
			for(int i = 0; i < localized->cantidadPosiciones; i++){
				t_posicion* posicion = list_get(localized->listaPosiciones, i);
				char* posicionAppend = string_from_format(" (%d,%d) ", posicion->posicionX, posicion->posicionY);
				string_append(&posicionesImpresas, posicionAppend);
				free(posicionAppend);
			} string_append(&posicionesImpresas, "]");

			log_debug(logger, "Lista de posiciones: %s", posicionesImpresas);
			free(posicionesImpresas);
		} else if(paquete->codigo_operacion == CAUGHT_POKEMON){
			t_caught_pokemon* caught = paquete->buffer->stream;

			if(caught->ok == 1){
				log_debug(logger, "CAUGHT = [OK]");
			} else{
				log_debug(logger, "CAUGHT = [FALSE]");
			}
		}

		enviar_mensaje_a_suscriptores(paquete);
		log_info(logger, "Mensaje enviado a suscriptores.");
	}

	log_debug(logger, "END: escucharSocketMensajesACachear");
}

void procesarACK(t_paquete* paquete, op_code tipoCola, int idSuscriptor){
	//todo agregar esto al esperarlo
//	if(paquete == NULL){
//		log_info(logger, "Se perdió conexión ACK con proceso ID [%d]", args->id);
//		close(*args->socket);
//		free(args);
//		break;
//	}

	log_info(logger, "Mensaje recibido: ACK");

	/* Agrego el ID del proceso a la lista de ACK del mensaje
	 */
	for(int i = 0; i < 6; i++) {
		if(cola[i].nombreCola == tipoCola){
			int cantidadMensajes = list_size(cola[i].mensajes);
			/* Recorro la lista de mensajes hasta dar con el indicado
			 */
			for(int j = 0; j < cantidadMensajes; j++){
				t_metadata* mensaje = list_get(cola[i].mensajes, j);
				/* Agrego el ID del proceso a la lista de ACK del mensaje
				 */
				if(mensaje->ID == paquete->ID_CORRELATIVO){
					list_add(mensaje->ACKSuscriptores, &idSuscriptor);

					if(list_size(mensaje->ACKSuscriptores) == list_size(cola[i].suscriptores)){
						log_info(logger, "Todos mis suscriptores enviaron ACK. Elimino mensaje ID [%d]", paquete->ID_CORRELATIVO);
						list_remove_and_destroy_element(cola[i].mensajes, j, (void*) liberoMensaje);
					}
					break;
				}

			}
			break;
		}
	}
	free(paquete->buffer);
	free(paquete);
}

void liberoMensaje(t_metadata* mensaje){
	list_destroy(mensaje->ACKSuscriptores);
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

			int socketAUsar = 0;
			int listaSize = list_size(cola[i].suscriptores);
			log_debug(logger, "Se procede a mandar el mensaje a los %d suscriptores de la cola %d", listaSize, paquete->codigo_operacion);

			for(int j = 0; j < listaSize; j++){
				suscriptorEnCola* suscriptor = list_get(cola[i].suscriptores, j);
				int idSuscriptor = suscriptor->ID;
				log_debug(logger, "El ID del proceso al que le mando mensaje es %d", idSuscriptor);

				pthread_mutex_lock(&mutexRepoGameCard);
				int existeGameCard = check_si_existe_gamecard(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoGameCard);

				pthread_mutex_lock(&mutexRepoGameBoy);
				int existeGameBoy = check_si_existe_gameboy(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoGameBoy);

				if(existeGameCard == 1){
					pthread_mutex_lock(&mutexRepoGameCard);
					log_debug(logger, "Existe gamecard");
					t_suscriptor_gamecard* gamecard = buscar_suscriptor_gamecard(idSuscriptor);
					socketAUsar = gamecard->socketNew;
					pthread_mutex_unlock(&mutexRepoGameCard);
				}

				else if(existeGameBoy == 1){
					log_debug(logger, "Existe gameboy");
					pthread_mutex_lock(&mutexRepoGameBoy);
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
					pthread_mutex_unlock(&mutexRepoGameBoy);
				}

				else{
					log_debug(logger, "no existe sub");
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

				int enviado = enviar_new_pokemon(new_a_enviar, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);

				if (enviado != -1){
					log_info(logger, "Mensaje NEW_POKEMON enviado | ID Mensaje: %d", paquete->ID);

					agregarIdAEnviados(idSuscriptor, paquete->ID, paquete->codigo_operacion);

					t_paquete* ack = recibir_mensaje(socketAUsar);

					if(ack != NULL){
						procesarACK(ack, paquete->codigo_operacion, suscriptor->ID);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", suscriptor->ID);
						break;
					}
				}
				else{
					log_info(logger, "Mensaje NEW_POKEMON no ha podido ser enviado.");
				}
			}
			pthread_mutex_unlock(&mutexColas);
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

			int socketAUsar = 0;
			int listaSize = list_size(cola[i].suscriptores);

			for(int j = 0; j < listaSize; j++){
				suscriptorEnCola* suscriptor = list_get(cola[i].suscriptores, j);
				int idSuscriptor = suscriptor->ID;
				log_debug(logger, "El ID del proceso al que le mando mensaje es %d", idSuscriptor);

				pthread_mutex_lock(&mutexRepoTeam);
				int existeTeam = check_si_existe_team(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoTeam);

				pthread_mutex_lock(&mutexRepoGameBoy);
				int existeGameBoy = check_si_existe_gameboy(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoGameBoy);

				if(existeTeam == 1){
					pthread_mutex_lock(&mutexRepoTeam);
					t_suscriptor_team* team = buscar_suscriptor_team(idSuscriptor);
					socketAUsar = team->socketAppeared;
					pthread_mutex_unlock(&mutexRepoTeam);
				}
				else if(existeGameBoy == 1){
					pthread_mutex_lock(&mutexRepoGameBoy);
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
					pthread_mutex_unlock(&mutexRepoGameBoy);
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

				int enviado = enviar_appeared_pokemon(appeared_a_enviar, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);

				if (enviado != -1){
					log_info(logger, "Mensaje APPEARED_POKEMON enviado | ID Mensaje: %d", paquete->ID);

					agregarIdAEnviados(idSuscriptor, paquete->ID, paquete->codigo_operacion);

					t_paquete* ack = recibir_mensaje(socketAUsar);

					if(ack != NULL){
						procesarACK(ack, paquete->codigo_operacion, suscriptor->ID);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", suscriptor->ID);
						break;
					}
				}
				else{
					log_info(logger, "Mensaje APPEARED_POKEMON no ha podido ser enviado.");
				}
			}
			pthread_mutex_unlock(&mutexColas);
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

			int socketAUsar = 0;
			int listaSize = list_size(cola[i].suscriptores);

			for(int j = 0; j < listaSize; j++){
				suscriptorEnCola* suscriptor = list_get(cola[i].suscriptores, j);
				int idSuscriptor = suscriptor->ID;
				log_debug(logger, "El ID del proceso al que le mando mensaje es %d", idSuscriptor);

				pthread_mutex_lock(&mutexRepoGameCard);
				int existeGameCard = check_si_existe_gamecard(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoGameCard);

				pthread_mutex_lock(&mutexRepoGameBoy);
				int existeGameBoy = check_si_existe_gameboy(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoGameBoy);

				if(existeGameCard == 1){
					pthread_mutex_lock(&mutexRepoGameCard);
					t_suscriptor_gamecard* gamecard = buscar_suscriptor_gamecard(idSuscriptor);
					socketAUsar = gamecard->socketCatch;
					pthread_mutex_unlock(&mutexRepoGameCard);
				}
				else if(existeGameBoy == 1){
					pthread_mutex_lock(&mutexRepoGameBoy);
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
					pthread_mutex_unlock(&mutexRepoGameBoy);
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

				int enviado = enviar_catch_pokemon(catch_a_enviar, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);

				if (enviado != -1){
					log_info(logger, "Mensaje CATCH_POKEMON enviado | ID Mensaje: %d", paquete->ID);

					agregarIdAEnviados(idSuscriptor, paquete->ID, paquete->codigo_operacion);

					t_paquete* ack = recibir_mensaje(socketAUsar);

					if(ack != NULL){
						procesarACK(ack, paquete->codigo_operacion, suscriptor->ID);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", suscriptor->ID);
						break;
					}
				}
				else{
					log_info(logger, "Mensaje CATCH_POKEMON no ha podido ser enviado.");
				}
			}

			pthread_mutex_unlock(&mutexColas);
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

			int socketAUsar = 0;
			int listaSize = list_size(cola[i].suscriptores);

			for(int j = 0; j < listaSize; j++){
				suscriptorEnCola* suscriptor = list_get(cola[i].suscriptores, j);
				int idSuscriptor = suscriptor->ID;
				log_debug(logger, "El ID del proceso al que le mando mensaje es %d", idSuscriptor);

				pthread_mutex_lock(&mutexRepoTeam);
				int existeTeam = check_si_existe_team(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoTeam);

				pthread_mutex_lock(&mutexRepoGameBoy);
				int existeGameBoy = check_si_existe_gameboy(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoGameBoy);

				if(existeTeam == 1){
					pthread_mutex_lock(&mutexRepoTeam);
					t_suscriptor_team* team = buscar_suscriptor_team(idSuscriptor);
					socketAUsar = team->socketCaught;
					pthread_mutex_unlock(&mutexRepoTeam);
				}
				else if(existeGameBoy == 1){
					pthread_mutex_lock(&mutexRepoGameBoy);
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
					pthread_mutex_unlock(&mutexRepoGameBoy);
				}

				else{
					continue;
				}

				/* Duplico la estructura
				 * para no borrar la original
				 */
				t_caught_pokemon* caught_a_enviar = malloc(sizeof(t_caught_pokemon));
				caught_a_enviar->ok = caught_pokemon->ok;
				int enviado = enviar_caught_pokemon(caught_a_enviar, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);

				if (enviado != -1){
					log_info(logger, "Mensaje CAUGHT_POKEMON enviado | ID Mensaje: %d", paquete->ID);

					agregarIdAEnviados(idSuscriptor, paquete->ID, paquete->codigo_operacion);

					t_paquete* ack = recibir_mensaje(socketAUsar);

					if(ack != NULL){
						procesarACK(ack, paquete->codigo_operacion, suscriptor->ID);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", suscriptor->ID);
						break;
					}
				}
				else{
					log_info(logger, "Mensaje CAUGHT_POKEMON no ha podido ser enviado.");
				}

			}

			pthread_mutex_unlock(&mutexColas);
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

			int socketAUsar = 0;
			int listaSize = list_size(cola[i].suscriptores);

			for(int j = 0; j < listaSize; j++){
				suscriptorEnCola* suscriptor = list_get(cola[i].suscriptores, j);
				int idSuscriptor = suscriptor->ID;
				log_debug(logger, "El ID del proceso al que le mando mensaje es %d", idSuscriptor);

				pthread_mutex_lock(&mutexRepoGameCard);
				int existeGameCard = check_si_existe_gamecard(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoGameCard);

				pthread_mutex_lock(&mutexRepoGameBoy);
				int existeGameBoy = check_si_existe_gameboy(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoGameBoy);

				if(existeGameCard == 1){
					pthread_mutex_lock(&mutexRepoGameCard);
					t_suscriptor_gamecard* gamecard = buscar_suscriptor_gamecard(idSuscriptor);
					socketAUsar = gamecard->socketGet;
					pthread_mutex_unlock(&mutexRepoGameCard);
				}
				else if(existeGameBoy == 1){
					pthread_mutex_lock(&mutexRepoGameBoy);
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
					pthread_mutex_unlock(&mutexRepoGameBoy);
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
				int enviado = enviar_get_pokemon(get_a_enviar, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);

				if (enviado != -1){
					log_info(logger, "Mensaje GET_POKEMON enviado | ID Mensaje: %d", paquete->ID);

					agregarIdAEnviados(idSuscriptor, paquete->ID, paquete->codigo_operacion);

					t_paquete* ack = recibir_mensaje(socketAUsar);

					if(ack != NULL){
						procesarACK(ack, paquete->codigo_operacion, suscriptor->ID);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", suscriptor->ID);
						break;
					}
				}
				else{
					log_info(logger, "Mensaje GET_POKEMON no ha podido ser enviado.");
				}
			}
			pthread_mutex_unlock(&mutexColas);
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

			int socketAUsar = 0;
			int listaSize = list_size(cola[i].suscriptores);

			for(int j = 0; j < listaSize; j++){
				suscriptorEnCola* suscriptor = list_get(cola[i].suscriptores, j);
				int idSuscriptor = suscriptor->ID;
				log_debug(logger, "El ID del proceso al que le mando mensaje es %d", idSuscriptor);

				pthread_mutex_lock(&mutexRepoTeam);
				int existeTeam = check_si_existe_team(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoTeam);

				pthread_mutex_lock(&mutexRepoGameBoy);
				int existeGameBoy = check_si_existe_gameboy(idSuscriptor);
				pthread_mutex_unlock(&mutexRepoGameBoy);

				if(existeTeam == 1){
					pthread_mutex_lock(&mutexRepoTeam);
					t_suscriptor_team* team = buscar_suscriptor_team(idSuscriptor);
					socketAUsar = team->socketLocalized;
					pthread_mutex_unlock(&mutexRepoTeam);
				}
				else if(existeGameBoy == 1){
					pthread_mutex_lock(&mutexRepoGameBoy);
					t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(idSuscriptor);
					socketAUsar = gameboy->socketDondeEscucha;
					pthread_mutex_unlock(&mutexRepoGameBoy);
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

				log_debug(logger, "LOCALIZED ENVIADO: CANTIDAD POSICIONES: [%d]", localized_a_enviar->cantidadPosiciones);
				int enviado = enviar_localized_pokemon(localized_a_enviar, socketAUsar, paquete->ID, paquete->ID_CORRELATIVO);

				if (enviado != -1){
					log_info(logger, "Mensaje LOCALIZED_POKEMON enviado | ID Mensaje: %d", paquete->ID);

					agregarIdAEnviados(idSuscriptor, paquete->ID, paquete->codigo_operacion);

					t_paquete* ack = recibir_mensaje(socketAUsar);

					if(ack != NULL){
						procesarACK(ack, paquete->codigo_operacion, suscriptor->ID);
					}

					else{
						log_info(logger, "No se pudo recibir ACK de ID [%d]", suscriptor->ID);
						break;
					}
				}
				else{
					log_info(logger, "Mensaje LOCALIZED_POKEMON no ha podido ser enviado.");
				}

			}
			pthread_mutex_unlock(&mutexColas);
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
