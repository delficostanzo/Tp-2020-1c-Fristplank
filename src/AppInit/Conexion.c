
#include "Conexion.h"


void* escucharGameBoy(){
	int conexionGameboy = crearSocket();
	if(escuchaEn(conexionGameboy, puertoTeam)){
		quickLog("Escuchando conexiones del GameBoy");
	}

	while(1){
		int socketGameBoy = aceptarConexion(conexionGameboy);

		id_proceso idProcesoConectado;
		idProcesoConectado = iniciarHandshake(socketGameBoy, TEAM);
		quickLog("Me conecté con GameBoy");

		t_paquete* paqueteNuevo = recibir_mensaje(socketGameBoy);

		switch(paqueteNuevo->codigo_operacion){
			case APPEARED_POKEMON:
				//TODO iniciar hilo para procesarlo
				break;
			case CAUGHT_POKEMON:
				//TODO iniciar hilo para procesarlo
				break;
			case LOCALIZED_POKEMON:
				//TODO iniciar hilo para procesarlo
				break;
			default:
				quickLog("Tipo de mensaje invalido.");
		}
	}
	return 0;
}

void* generarSocketsConBroker() {

	conexionBroker = crearSocket();

	while ((conectarA(conexionBroker, IP_BROKER, PUERTO_BROKER)) != 1) {
		quickLog("Intentando conexión a Broker...");
		//TODO: reconectando cada cierto tiempo
	}

	id_proceso idProceso;
	idProceso = responderHandshake(conexionBroker, TEAM);


	suscripcionAppeared = crearSocket();
	suscripcionCaught = crearSocket();
	suscripcionLocalized = crearSocket();
	socketGet = crearSocket();
	socketCatch = crearSocket();

	socketACKAppeared = crearSocket();
	socketACKCaught = crearSocket();
	socketACKLocalized = crearSocket();

	if (conectarA(suscripcionAppeared, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de appeared_pokemon");
		if (conectarA(socketACKAppeared, IP_BROKER, PUERTO_BROKER)) {
			quickLog("Socket de ACK Get Pokemon guardado.");

			pthread_t escucharAppearedPokemon;
			pthread_create(&escucharAppearedPokemon, NULL, escucharColaAppearedPokemon, NULL);
			pthread_detach(escucharAppearedPokemon);
		}
	}

	if (conectarA(suscripcionCaught, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de caught_pokemon");
		if (conectarA(socketACKCaught, IP_BROKER, PUERTO_BROKER)) {
			quickLog("Socket de ACK Caught Pokemon guardado.");

			pthread_t escucharCaughtPokemon;
			pthread_create(&escucharCaughtPokemon, NULL, escucharColaCaughtPokemon, NULL);
			pthread_detach(escucharCaughtPokemon);
		}
	}


	if (conectarA(suscripcionLocalized, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de localized_pokemon");
		if (conectarA(socketACKLocalized, IP_BROKER, PUERTO_BROKER)) {
			quickLog("Socket de ACK Localized Pokemon guardado.");

			pthread_t escucharLocalizedPokemon;
			pthread_create(&escucharLocalizedPokemon, NULL, escucharColaLocalizedPokemon, NULL);
			pthread_detach(escucharLocalizedPokemon);
		}
	}

	if (conectarA(socketGet, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Ya se conecto a la cola de get para poder enviarle mensajes");

		pthread_mutex_lock(&mutexObjetivosGlobales);
		enviarGetDesde(objetivosGlobales, socketGet);
		pthread_mutex_unlock(&mutexObjetivosGlobales);

		quickLog("Se envia correctamente el get");
//			pthread_t enviarGet;
//			pthread_create(&enviarGet, NULL, enviarAColaGet, NULL);
//			pthread_detach(enviarGet);

	}
	if (conectarA(socketCatch, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Ya se conecto a la cola de catch para poder enviarle mensajes");
	}

	return 0;
}

////////FUNCIONES DE LOS HILOS DE COLAS A LAS QUE ME SUSCRIBO//////////
void* escucharColaAppearedPokemon(){

	while(1){
		quickLog("Esperando mensajes");
//		t_list* objetivosTotales = getObjetivosTotalesDesde(entrenadores);
//		t_list* objetivosAtrapados = getTotalAtrapadosDesde(entrenadores);
//		t_list* objetivosGlobales = getObjetivosGlobalesDesde(objetivosTotales, objetivosAtrapados);

		t_paquete* paqueteNuevo = recibirAppearedYGuardarlos(suscripcionAppeared, objetivosGlobales, pokemonesLibres);

		enviar_ACK(socketACKAppeared, -1, paqueteNuevo->ID);
	}
}

void* escucharColaCaughtPokemon(){

	while(1){
		quickLog("Esperando mensajes");
		t_paquete* paqueteNuevo = recibirCaught(suscripcionCaught);
		if(paqueteNuevo != NULL){
			enviar_ACK(socketACKCaught, -1, paqueteNuevo->ID);
			// avisarle al entrenador si atrapo al poke o no
		}
	}
}

void* escucharColaLocalizedPokemon(){

	while(1){
		quickLog("Esperando mensajes");
		t_paquete* paqueteNuevo = recibirLocalizedYGuardalos(suscripcionLocalized, objetivosGlobales, pokemonesLibres);

		enviar_ACK(socketACKLocalized, -1, paqueteNuevo->ID);
	}
}
