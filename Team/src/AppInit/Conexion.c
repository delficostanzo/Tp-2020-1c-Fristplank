
#include "Conexion.h"


void* escucharGameBoy(){
	conexionGameboy = crearSocket();
	if(escuchaEn(conexionGameboy, puertoTeam)){
		quickLog("$-Escuchando conexiones del GameBoy");
		log_info(LO, "Escuchando conexiones del GameBoy");
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


		quickLog("$-Me conecté con GameBoy");

		t_paquete* paqueteNuevo = recibirAppearedYGuardarlos(socketGameBoy);
		free(paqueteNuevo);

	}
}

int generarSocketsConBroker() {
	quickLog("INICIO GENERAR SOCKETS");

	conexionBroker = crearSocket();

	while ((conectarA(conexionBroker, IP_BROKER, PUERTO_BROKER)) != 1) {
		quickLog("$-Intentando conexión a Broker...");
		log_info(LO, "Se corto la conexion con el Broker, se reintentara en %d segundos", TIEMPO_RECONEXION);
		sleep(TIEMPO_RECONEXION);
	}

	t_handshake* handshakePropio = malloc(sizeof(t_handshake));
	handshakePropio->id = TEAM;
	handshakePropio->idUnico = ID_UNICO;

	t_handshake* handshakeResponse;
	handshakeResponse = responderHandshake(conexionBroker, handshakePropio);
	//log_info(logger, "$-El id del proceso con el que me conecte es: %d", handshakeResponse->id);
	free(handshakePropio);
	free(handshakeResponse);

	socketGet = crearSocket();
	socketIdGet = crearSocket();

	suscripcionAppeared = crearSocket();
	socketACKAppeared = crearSocket();

	suscripcionLocalized = crearSocket();
	socketACKLocalized = crearSocket();

	socketCatch = crearSocket();
	socketIdCatch = crearSocket();

	suscripcionCaught = crearSocket();
	socketACKCaught = crearSocket();

	int conexionCorrecta = 1;

	//ENVIA GET Y ESCUCHA EL ID GET
	if (conectarA(socketGet, IP_BROKER, PUERTO_BROKER)) {
		quickLog("$-Ya se conecto a la cola de get para poder enviarle mensajes");
		if (conectarA(socketIdGet, IP_BROKER, PUERTO_BROKER)) {
			quickLog("$-Socket de recepcion de ids Get guardado.");

		} else{
			conexionCorrecta = -1;
		}
	} else{
		conexionCorrecta = -1;
	 }


	//ESCUCHA APPEARED Y ENVIA EL ACK
	if (conectarA(suscripcionAppeared, IP_BROKER, PUERTO_BROKER)) {
		quickLog("$-Suscripto a la cola de appeared_pokemon");
		if (conectarA(socketACKAppeared, IP_BROKER, PUERTO_BROKER)) {
			quickLog("$-Socket de ACK Appeared Pokemon guardado.");
		} else{
			conexionCorrecta = -1;
		 }
	} else{
		conexionCorrecta = -1;
	 }

	//ESCUCHA LOCALIZED Y ENVIA EL ACK
	if (conectarA(suscripcionLocalized, IP_BROKER, PUERTO_BROKER)) {
		quickLog("$-Suscripto a la cola de localized_pokemon");
		if (conectarA(socketACKLocalized, IP_BROKER, PUERTO_BROKER)) {
			quickLog("$-Socket de ACK Localized Pokemon guardado.");

		} else{
			conexionCorrecta = -1;
		 }

	} else{
		conexionCorrecta = -1;
	 }

	//ENVIA CATCH Y ESCUCHA EL ID CATCH
	if (conectarA(socketCatch, IP_BROKER, PUERTO_BROKER)) {
		quickLog("$-Ya se conecto a la cola de catch para poder enviarle mensajes");
		if (conectarA(socketIdCatch, IP_BROKER, PUERTO_BROKER)) {
			quickLog("$-Socket de recepcion de ids Catch guardado.");
		//el envio lo hace cada entrenador!
		} else{
			conexionCorrecta = -1;
		 }
	} else{
		conexionCorrecta = -1;
	 }

	//ESCUCHA CAUGHT Y ENVIA EL ACK
	if (conectarA(suscripcionCaught, IP_BROKER, PUERTO_BROKER)) {
		quickLog("$-Suscripto a la cola de caught_pokemon");
		if (conectarA(socketACKCaught, IP_BROKER, PUERTO_BROKER)) {
			quickLog("$-Socket de ACK Caught Pokemon guardado.");
		} else{
			conexionCorrecta = -1;
		 }
	} else{
		conexionCorrecta = -1;
	 }

	sleep(2);

	return conexionCorrecta;
}

void crearHilosDeEscucha() {

	if(generarSocketsConBroker() == -1){
		quickLog("$-No se pudo conectar con Broker, se reintenta conexión");
		sleep(TIEMPO_RECONEXION);
		generarSocketsConBroker();
	}



	pthread_create(&escucharAppearedPokemon, NULL, (void*)escucharColaAppearedPokemon, NULL);
	pthread_detach(escucharAppearedPokemon);


	pthread_create(&escucharLocalizedPokemon, NULL, (void*)escucharColaLocalizedPokemon, NULL);
	pthread_detach(escucharLocalizedPokemon);


	pthread_create(&escucharCaughtPokemon, NULL, (void*)escucharColaCaughtPokemon, NULL);
	pthread_detach(escucharCaughtPokemon);

	enviarGetDesde(socketGet);
	quickLog("$-Se envian correctamente los get");
}

////////FUNCIONES DE LOS HILOS DE COLAS A LAS QUE ME SUSCRIBO//////////
void* escucharColaAppearedPokemonGameBoy(){
		quickLog("$-Recibiendo appeared del gameboy");

		t_paquete* paqueteNuevo = recibirAppearedYGuardarlos(socketGameBoy);

		return paqueteNuevo;
}


void* escucharColaAppearedPokemon(){

	while(1){
		quickLog("$-Esperando mensajes de Appeared");

		t_paquete* paqueteNuevo = recibirAppearedYGuardarlos(suscripcionAppeared);

		if(paqueteNuevo != NULL){
			enviar_ACK(socketACKAppeared, -1, paqueteNuevo->ID);
			free(paqueteNuevo);
			quickLog("$-Pudo enviar el ACK de los appeared");
		} else {
			log_debug(logger, "Se desconecto el socket de Appeared Pokemon");
			desconexion = 1;
			pthread_mutex_lock(&semaforoDesconexion);
			reconectarBroker();
			pthread_mutex_unlock(&semaforoDesconexion);
		}

	}
}

void* escucharColaIdsCatchPokemon(){

	while(1){
		quickLog("$-Esperando mensajes de ids catch");


	}
}

void* escucharColaCaughtPokemon(){

	while(1){
		//quickLog("$-Esperando mensajes de Caught");
		//el entrenador que estaba esperando esa respuesta es ejecutado y pasa al estado segun corresponda
		t_paquete* paqueteNuevo = recibirCaught(suscripcionCaught);
		if(paqueteNuevo != NULL){
			enviar_ACK(socketACKCaught, -1, paqueteNuevo->ID);
			quickLog("$-Pudo enviar el ACK del caught");
			free(paqueteNuevo->buffer->stream);
			free(paqueteNuevo->buffer);
			free(paqueteNuevo);

		} else {
			log_debug(logger, "Se desconecto el socket de Caught Pokemon");
			desconexion = 1;
			pthread_mutex_lock(&semaforoDesconexion);
			reconectarBroker();
			pthread_mutex_unlock(&semaforoDesconexion);
		}


	}
}

void* escucharColaLocalizedPokemon(){

	while(1){
		quickLog("$-Esperando mensajes de Localized");

		t_paquete* paqueteNuevo = recibirLocalizedYGuardalos(suscripcionLocalized);

		//si es -10 es que no se corto la conexion con broker pero tiene que dejar de escuchar localized
		if(paqueteNuevo->ID == -10) {
			enviar_ACK(socketACKLocalized, -1, paqueteNuevo->ID);
			free(paqueteNuevo);
			liberarConexion(suscripcionLocalized);
			liberarConexion(socketACKLocalized);
			pthread_cancel(escucharLocalizedPokemon);
		}
		//si tiene que seguir recibiendo localized
		else if(paqueteNuevo != NULL) {
			enviar_ACK(socketACKLocalized, -1, paqueteNuevo->ID);
			quickLog("$-Pudo enviar el ACK del localized");
			free(paqueteNuevo->buffer->stream);
			free(paqueteNuevo->buffer);
			free(paqueteNuevo);
		//si es null es que se desconecto del broker y tiene que reconectarse
		} else {
			log_debug(logger, "Se desconecto el socket de Localized Pokemon");
			desconexion = 1;
			pthread_mutex_lock(&semaforoDesconexion);
			reconectarBroker();
			pthread_mutex_unlock(&semaforoDesconexion);
		}
	}
}

void reconectarBroker(){
	if (desconexion == 1) {
		int conectado = generarSocketsConBroker();

		while(conectado != 1){
			conectado = generarSocketsConBroker();
		}
		desconexion = 0;
	}
}
