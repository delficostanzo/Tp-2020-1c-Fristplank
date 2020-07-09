
#include "Conexion.h"


void* escucharGameBoy(){
	conexionGameboy = crearSocket();
	if(escuchaEn(conexionGameboy, puertoTeam)){
		quickLog("Escuchando conexiones del GameBoy");
	}

	while(1){
		socketGameBoy = aceptarConexion(conexionGameboy);

		t_handshake* handshakePropio = malloc(sizeof(t_handshake));
		handshakePropio->id = TEAM;
		handshakePropio->idUnico = ID_UNICO;

		t_handshake* handshakeResponse;
		handshakeResponse = iniciarHandshake(socketGameBoy, handshakePropio);
		free(handshakePropio);
		free(handshakeResponse);

		quickLog("Me conecté con GameBoy");

		pthread_t escucharAppearedPokemon;
		pthread_create(&escucharAppearedPokemon, NULL, (void*)escucharColaAppearedPokemonGameBoy, NULL);
		pthread_detach(escucharAppearedPokemon);
	}
}

int generarSocketsConBroker() {

	t_log* logger = iniciar_logger();

	conexionBroker = crearSocket();

	while ((conectarA(conexionBroker, IP_BROKER, PUERTO_BROKER)) != 1) {
		quickLog("Intentando conexión a Broker...");
		sleep(TIEMPO_RECONEXION);
	}

	t_handshake* handshakePropio = malloc(sizeof(t_handshake));
	handshakePropio->id = TEAM;
	handshakePropio->idUnico = ID_UNICO;

	t_handshake* handshakeResponse;
	handshakeResponse = responderHandshake(conexionBroker, handshakePropio);
	log_info(logger, "El id del proceso con el que me conecte es: %d", handshakeResponse->id);
	free(handshakePropio);
	free(handshakeResponse);

	socketGet = crearSocket();
	//socketIdGet = crearSocket();

	suscripcionAppeared = crearSocket();
	//socketACKAppeared = crearSocket();

	suscripcionLocalized = crearSocket();
	//socketACKLocalized = crearSocket();

	socketCatch = crearSocket();
	socketIdCatch = crearSocket();

	suscripcionCaught = crearSocket();
	socketACKCaught = crearSocket();

	int conexionCorrecta = 1;


	//ENVIA GET Y ESCUCHA EL ID GET
	if (conectarA(socketGet, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Ya se conecto a la cola de get para poder enviarle mensajes");
//		if (conectarA(socketIdGet, IP_BROKER, PUERTO_BROKER)) {
//			quickLog("Socket de recepcion de ids Get guardado.");

			enviarGetDesde(socketGet);

			quickLog("Se envian correctamente los get");
		//} else{
//				conexionCorrecta = -1;
//			}
	} else{
		conexionCorrecta = -1;
	 }


	//ESCUCHA APPEARED Y ENVIA EL ACK
	if (conectarA(suscripcionAppeared, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de appeared_pokemon");
//		if (conectarA(socketACKAppeared, IP_BROKER, PUERTO_BROKER)) {
//			quickLog("Socket de ACK Appeared Pokemon guardado.");
		//} else{
//			conexionCorrecta = -1;
//		 }
	} else{
		conexionCorrecta = -1;
	 }

	//ESCUCHA LOCALIZED Y ENVIA EL ACK
	if (conectarA(suscripcionLocalized, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de localized_pokemon");
//		if (conectarA(socketACKLocalized, IP_BROKER, PUERTO_BROKER)) {
//			quickLog("Socket de ACK Localized Pokemon guardado.");
//
//		} else{
//			conexionCorrecta = -1;
//		 }

	} else{
		conexionCorrecta = -1;
	 }

	//ENVIA CATCH Y ESCUCHA EL ID CATCH
	if (conectarA(socketCatch, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Ya se conecto a la cola de catch para poder enviarle mensajes");
		if (conectarA(socketIdCatch, IP_BROKER, PUERTO_BROKER)) {
			quickLog("Socket de recepcion de ids Catch guardado.");
			sem_post(&semaforoCatch);
		//el envio lo hace cada entrenador!
		} else{
			conexionCorrecta = -1;
		 }
	} else{
		conexionCorrecta = -1;
	 }

	//ESCUCHA CAUGHT Y ENVIA EL ACK
	if (conectarA(suscripcionCaught, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de caught_pokemon");
		if (conectarA(socketACKCaught, IP_BROKER, PUERTO_BROKER)) {
			quickLog("Socket de ACK Caught Pokemon guardado.");
		} else{
			conexionCorrecta = -1;
		 }
	} else{
		conexionCorrecta = -1;
	 }


	return conexionCorrecta;
}

void crearHilosDeEscucha() {

	pthread_t escucharAppearedPokemon;
	pthread_create(&escucharAppearedPokemon, NULL, (void*)escucharColaAppearedPokemon, NULL);
	//pthread_detach(escucharLocalizedPokemon);

	pthread_t escucharLocalizedPokemon;
	pthread_create(&escucharLocalizedPokemon, NULL, (void*)escucharColaLocalizedPokemon, NULL);
	//pthread_detach(escucharLocalizedPokemon);

	pthread_t escucharCaughtPokemon;
	pthread_create(&escucharCaughtPokemon, NULL, (void*)escucharColaCaughtPokemon, NULL);
	//pthread_detach(escucharCaughtPokemon);
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
		//quickLog("Pudo enviar el ACK de los appeared");
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

//		enviar_ACK(socketACKLocalized, -1, paqueteNuevo->ID);
//		quickLog("Pudo enviar el ACK del localized");
	}
}
