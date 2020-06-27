
#include "Conexion.h"


void* escucharGameBoy(){
	conexionGameboy = crearSocket();
	if(escuchaEn(conexionGameboy, puertoTeam)){
		quickLog("Escuchando conexiones del GameBoy");
	}

	while(1){
		socketGameBoy = aceptarConexion(conexionGameboy);

		id_proceso idProcesoConectado;
		idProcesoConectado = iniciarHandshake(socketGameBoy, TEAM);
		quickLog("Me conecté con GameBoy");

		pthread_t escucharAppearedPokemon;
		pthread_create(&escucharAppearedPokemon, NULL, (void*)escucharColaAppearedPokemonGameBoy, NULL);
		pthread_detach(escucharAppearedPokemon);
	}
}

void* generarSocketsConBroker() {

	conexionBroker = crearSocket();

	while ((conectarA(conexionBroker, IP_BROKER, PUERTO_BROKER)) != 1) {
		quickLog("Intentando conexión a Broker...");
		//TODO: reconectando cada cierto tiempo
	}

	id_proceso idProceso;
	idProceso = responderHandshake(conexionBroker, TEAM);

	socketGet = crearSocket();
	//socketIdGet = crearSocket();

	suscripcionAppeared = crearSocket();
	//socketACKAppeared = crearSocket();

	suscripcionLocalized = crearSocket();
	socketACKLocalized = crearSocket();

	socketCatch = crearSocket();
	socketIdCatch = crearSocket();

	suscripcionCaught = crearSocket();
	socketACKCaught = crearSocket();


	if (conectarA(socketGet, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Ya se conecto a la cola de get para poder enviarle mensajes");
//		if (conectarA(socketIdGet, IP_BROKER, PUERTO_BROKER)) {
//			quickLog("Socket de recepcion de ids Get guardado.");

			enviarGetDesde(socketGet);

			quickLog("Se envia correctamente el get");
	//			pthread_t enviarGet;
	//			pthread_create(&enviarGet, NULL, enviarAColaGet, NULL);
	//			pthread_detach(enviarGet);
		//}
	}

	if (conectarA(suscripcionAppeared, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de appeared_pokemon");
//		if (conectarA(socketACKAppeared, IP_BROKER, PUERTO_BROKER)) {
//			quickLog("Socket de ACK Appeared Pokemon guardado.");

			pthread_t escucharAppearedPokemon;
			pthread_create(&escucharAppearedPokemon, NULL, (void*)escucharColaAppearedPokemon, NULL);
			//pthread_detach(escucharAppearedPokemon);
		//}
	}

	if (conectarA(suscripcionLocalized, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de localized_pokemon");
		if (conectarA(socketACKLocalized, IP_BROKER, PUERTO_BROKER)) {
			quickLog("Socket de ACK Localized Pokemon guardado.");

			pthread_t escucharLocalizedPokemon;
			pthread_create(&escucharLocalizedPokemon, NULL, (void*)escucharColaLocalizedPokemon, NULL);
			//pthread_detach(escucharLocalizedPokemon);
		}
	}

	if (conectarA(socketCatch, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Ya se conecto a la cola de catch para poder enviarle mensajes");
		if (conectarA(socketIdCatch, IP_BROKER, PUERTO_BROKER)) {
			quickLog("Socket de recepcion de ids Catch guardado.");
			sem_post(&semaforoCatch);
		//el envio lo hace cada entrenador!
		}
	}

	if (conectarA(suscripcionCaught, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de caught_pokemon");
		if (conectarA(socketACKCaught, IP_BROKER, PUERTO_BROKER)) {
			quickLog("Socket de ACK Caught Pokemon guardado.");

			pthread_t escucharCaughtPokemon;
			quickLog("BOOM");
			pthread_create(&escucharCaughtPokemon, NULL, (void*)escucharColaCaughtPokemon, NULL);
			//pthread_detach(escucharCaughtPokemon);
		}
	}


	return 0;
}

////////FUNCIONES DE LOS HILOS DE COLAS A LAS QUE ME SUSCRIBO//////////
void* escucharColaAppearedPokemonGameBoy(){
		quickLog("Recibiendo appeared del gameboy");


		t_paquete* paqueteNuevo = recibirAppearedYGuardarlos(socketGameBoy);


		return paqueteNuevo;
}

void* escucharColaAppearedPokemon(){

	while(1){
		quickLog("Esperando mensajes de Appeared");

		t_paquete* paqueteNuevo = recibirAppearedYGuardarlos(suscripcionAppeared);

		//enviar_ACK(socketACKAppeared, -1, paqueteNuevo->ID);
		quickLog("Pudo enviar el ACK de los appeared");
	}
}

void* escucharColaCaughtPokemon(){

	while(1){
		//quickLog("Esperando mensajes de Caught");
		//el entrenador que estaba esperando esa respuesta es ejecutado y pasa al estado segun corresponda
		t_paquete* paqueteNuevo = recibirCaught(suscripcionCaught);
		if(paqueteNuevo != NULL){
			enviar_ACK(socketACKCaught, -1, paqueteNuevo->ID);
			quickLog("Pudo enviar el ACK del caught");
		}

	}
}

void* escucharColaLocalizedPokemon(){

	while(1){
		quickLog("Esperando mensajes de Localized");

		t_paquete* paqueteNuevo = recibirLocalizedYGuardalos(suscripcionLocalized);

		enviar_ACK(socketACKLocalized, -1, paqueteNuevo->ID);
		quickLog("Pudo enviar el ACK del localized");
	}
}
