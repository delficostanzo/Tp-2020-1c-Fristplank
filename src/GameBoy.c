#include "GameBoy.h"

int main(int argc, char* argv[]) {

	//Inicio el logger para debuggear nosotros y también el que piden en la consigna.
	/* Las acciones a loguear en este archivo son:
		* Conexión a cualquier proceso.
		* Suscripción a una cola de mensajes.
		* Llegada de un nuevo mensaje a una cola de mensajes.
		* Envío de un mensaje a un suscriptor específico.
	 */

	logger = iniciarLogger();
	log_info(logger, "Logger creado.");
	loggerObligatorio = iniciarLoggerObligatorio();
	log_info(logger, "Logger obligatorio creado.");
	leer_config(logger);

//	DONE ./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]
//	DONE ./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]
//	DONE ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]
//	DONE ./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE] [OK/FAIL]
//	DONE ./gameboy BROKER GET_POKEMON [POKEMON]
//	DONE ./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]
//  DONE ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]
//	DONE ./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]
//	DONE ./gameboy GAMECARD GET_POKEMON [POKEMON]
//	./gameboy SUSCRIPTOR [COLA_DE_MENSAJES] [TIEMPO]
//

	int socket;

	if (strcmp(argv[1], "BROKER") == 0){ //MENSAJES AL BROKER

		socket = crear_conexion(IP_BROKER, PUERTO_BROKER);
		log_debug(loggerObligatorio, "Conexión hecha a IP %s | Puerto %s", IP_BROKER, PUERTO_BROKER);

		if (strcmp(argv[2], "NEW_POKEMON") == 0){

			procesarBrokerNewPokemon(socket, argv);
		}
		else if (strcmp(argv[2], "APPEARED_POKEMON") == 0){

			procesarBrokerAppearedPokemon(socket, argv);
		}
		else if (strcmp(argv[2], "CATCH_POKEMON") == 0){

			procesarBrokerCatchPokemon(socket, argv);
		}
		else if (strcmp(argv[2], "CAUGHT_POKEMON") == 0){

			procesarBrokerCaughtPokemon(socket, argv);
		}
		else if (strcmp(argv[2], "GET_POKEMON") == 0){

			procesarBrokerGetPokemon(socket, argv);
		}
		else{
			puts(argv[2]);
			log_info(logger, "Tipo de mensaje incorrecto.");
		}

	}
	else if (strcmp(argv[1], "TEAM") == 0){ //MENSAJES AL TEAM
		socket = crear_conexion(IP_TEAM, PUERTO_TEAM);
		log_debug(loggerObligatorio, "Conexión hecha a IP %s | Puerto %s", IP_TEAM, PUERTO_TEAM);

		if (strcmp(argv[2], "APPEARED_POKEMON") == 0){
			procesarTeamAppearedPokemon(socket, argv);
		}

		else{
			log_info(logger, "Tipo de mensaje incorrecto.");
		}
	}
	else if (strcmp(argv[1], "GAMECARD") == 0){ //MENSAJES AL GAMECARD

		socket = crear_conexion(IP_GAMECARD, PUERTO_GAMECARD);
		log_debug(loggerObligatorio, "Conexión hecha a IP %s | Puerto %s", IP_GAMECARD, PUERTO_GAMECARD);

		if (strcmp(argv[2], "NEW_POKEMON") == 0){
			procesarGameCardNewPokemon(socket, argv);
		}
		else if (strcmp(argv[2], "CATCH_POKEMON") == 0){
			procesarGameCardCatchPokemon(socket, argv);
		}
		else if (strcmp(argv[2], "GET_POKEMON") == 0){
			procesarGameCardGetPokemon(socket, argv);
		}
		else{
			log_info(logger, "Tipo de mensaje incorrecto.");
		}

	}
	else if (strcmp(argv[1], "SUSCRIPTOR") == 0){ //MENSAJES PARA SUSCBRIBIRSE AL BROKER POR X SEGUNDOS
//		socket = crear_conexion(IP_BROKER, PUERTO_BROKER);
		log_debug(loggerObligatorio, "Conexión hecha a IP %s | Puerto %s", IP_BROKER, PUERTO_BROKER);

		procesarSubscribe(argv);
	}

	else{
		log_info(logger, "Proceso incorrecto.");
	}

	liberar_conexion(socket);
	log_destroy(logger);
	log_destroy(loggerObligatorio);
}

void procesarSubscribe(char* argv[]){

	op_code cola;

	if (strcmp(argv[2], "NEW_POKEMON") == 0){
		cola = NEW_POKEMON;
	}
	else if (strcmp(argv[2], "APPEARED_POKEMON") == 0){
		cola = APPEARED_POKEMON;
	}
	else if (strcmp(argv[2], "CATCH_POKEMON") == 0){
		cola = CATCH_POKEMON;
	}
	else if (strcmp(argv[2], "CAUGHT_POKEMON") == 0){
		cola = CAUGHT_POKEMON;
	}
	else if (strcmp(argv[2], "GET_POKEMON") == 0){
		cola = GET_POKEMON;
	}
	else if (strcmp(argv[2], "LOCALIZED_POKEMON") == 0){
		cola = LOCALIZED_POKEMON;
	}

	int segundosAEscuchar = (int) argv[3];


	//TODO ver como hacer para que termine despues de segundosAEscuchar

	//Iniciar thread de escucha
	pthread_t threadEscucha;
	pthread_create(&threadEscucha, NULL, escucharCola, (void*) cola);
	sleep(segundosAEscuchar);
	exit(0);
}

void* escucharCola(void* colaAEscuchar){
	op_code cola = (op_code) colaAEscuchar;

	// CONEXION A BROKER
	int conexionBroker = crearSocket();

	int puertoBrokerInt = atoi(PUERTO_BROKER);
	if(conectarA(conexionBroker, IP_BROKER, puertoBrokerInt)){
		log_info(logger, "Conectando al Broker");
	}

	id_proceso idProcesoBroker;
	idProcesoBroker = responderHandshake(conexionBroker, GAMEBOY);
	log_info(logger, "El id del proceso con el que me conecte es: %d", idProcesoBroker);

	t_gameboy_suscribe* gameboysuscribe = malloc(sizeof(t_gameboy_suscribe));
	gameboysuscribe->codigoCola = cola;

	log_info(logger, "Se envía un mensaje a Broker con la cola a la cual nos queremos suscribir: %s", COLAS_STRING[cola]);
	enviar_gameboy_suscribe(gameboysuscribe, conexionBroker, -1, -1);

	log_info(logger, "Suscripto a la cola de %s", COLAS_STRING[cola]);

	int escucharCola = crearSocket();
	int puertoPropio = atoi(PUERTO_GAMEBOY);
	if(escuchaEn(escucharCola, puertoPropio)){
		log_info(logger, "Escuchando a Broker");

		//TODO ACA HAY QUE LEER LO QUE NOS LLEGA
	}

	// END CONEXION A BROKER

	return NULL;
}

void procesarBrokerNewPokemon(int socket, char* argv[]){
	log_debug(logger, "Mensaje a enviar a Broker | Pokemon: %s - Posicion X: %d - Posicion Y: %d - Cantidad: %d", (char*) argv[3], (int) argv[4], (int) argv[5], (int) argv[6]);

	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];
	int cant = (int) argv[6];

	t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
	new_pokemon->pokemon = pokemon;
	new_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	new_pokemon->cantidad = cant;
	new_pokemon->posicion = malloc(sizeof(t_posicion));
	new_pokemon->posicion->posicionX = posX;
	new_pokemon->posicion->posicionX = posY;

	//TODO ENVIAR MENSAJE CON NUEVA LIBRARY
//	enviar_new_pokemon(new_pokemon, socket, -1, -1);
//	log_debug(loggerObligatorio, "Mensaje enviado a Broker | Pokemon: %s - Posicion X: %d - Posicion Y: %d - Cantidad: %d", new_pokemon->pokemon, new_pokemon->posicion->posicionX, new_pokemon->posicion->posicionY, new_pokemon->cantidad);

	free(new_pokemon->posicion);
	free(new_pokemon);
}

void procesarGameCardNewPokemon(int socket, char* argv[]){
	log_debug(logger, "Mensaje a enviar a GameCard | Pokemon: %s - Posicion X: %d - Posicion Y: %d - Cantidad: %d", (char*) argv[3], (int) argv[4], (int) argv[5], (int) argv[6]);

	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];
	int cant = (int) argv[6];
	int id = (int) argv[7];

	t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
	new_pokemon->pokemon = pokemon;
	new_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	new_pokemon->cantidad = cant;
	new_pokemon->posicion = malloc(sizeof(t_posicion));
	new_pokemon->posicion->posicionX = posX;
	new_pokemon->posicion->posicionX = posY;

	//TODO ENVIAR MENSAJE CON NUEVA LIBRARY
//	enviar_new_pokemon(new_pokemon, socket, id, -1);
//	log_debug(loggerObligatorio, "Mensaje enviado a GameCard | ID del Mensaje: %d | Pokemon: %s - Posicion X: %d - Posicion Y: %d - Cantidad: %d", id, new_pokemon->pokemon, new_pokemon->posicion->posicionX, new_pokemon->posicion->posicionY, new_pokemon->cantidad);

	free(new_pokemon->posicion);
	free(new_pokemon);
}


void procesarGameCardGetPokemon(int socket, char* argv[]){
	char* pokemon = (char*) argv[3];
	int id = (int) argv[4];

	t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));
	get_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	get_pokemon->pokemon = pokemon;

	//TODO ENVIAR MENSAJE CON NUEVA LIBRARY
//	enviar_get_pokemon(get_pokemon, socket, id, -1);
//	log_debug(loggerObligatorio, "Mensaje enviado a GameCard | ID del Mensaje: %d | Pokemon: %s", id, get_pokemon->pokemon);

	free(get_pokemon);
}

void procesarBrokerGetPokemon(int socket, char* argv[]){
	char* pokemon = (char*) argv[3];

	t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));
	get_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	get_pokemon->pokemon = pokemon;

	//TODO ENVIAR MENSAJE CON NUEVA LIBRARY
//	enviar_get_pokemon(get_pokemon, socket, -1, -1);
//	log_debug(loggerObligatorio, "Mensaje enviado a GameCard | Pokemon: %s", get_pokemon->pokemon);

	free(get_pokemon);
}

void procesarBrokerAppearedPokemon(int socket, char* argv[]){
	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];
	int idCorrelativo = (int) argv[6];

	t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
	appeared_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	appeared_pokemon->pokemon = pokemon;
	appeared_pokemon->posicion->posicionX = posX;
	appeared_pokemon->posicion->posicionX = posY;

	//TODO ENVIAR MENSAJE CON NUEVA LIBRARY
//	enviar_appeared_pokemon(appeared_pokemon, socket, -1, idCorrelativo);
//	log_debug(loggerObligatorio, "Mensaje enviado a Broker | Pokemon: %s - Posicion X: %d - Posicion Y: %d | ID Correlativo del Mensaje: %d", appeared_pokemon->pokemon, appeared_pokemon->posicion->posicionX, appeared_pokemon->posicion->posicionY, idCorrelativo);

	free(appeared_pokemon->posicion);
	free(appeared_pokemon);
}

void procesarBrokerCatchPokemon(int socket, char* argv[]){
	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];

	t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
	catch_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	catch_pokemon->pokemon = pokemon;
	catch_pokemon->posicion->posicionX = posX;
	catch_pokemon->posicion->posicionX = posY;

	//TODO ENVIAR MENSAJE CON NUEVA LIBRARY
//	enviar_catch_pokemon(catch_pokemon, socket, -1, -1);
//	log_debug(loggerObligatorio, "Mensaje enviado a Broker | Pokemon: %s - Posicion X: %d - Posicion Y: %d", catch_pokemon->pokemon, catch_pokemon->posicion->posicionX, catch_pokemon->posicion->posicionY);

	free(catch_pokemon->posicion);
	free(catch_pokemon);
}

void procesarBrokerCaughtPokemon(int socket, char* argv[]){
	int idCorrelativo = (int) argv[3];
	bool ok = (bool) argv[4];

	t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));
	caught_pokemon->ok = ok;

	//TODO ENVIAR MENSAJE CON NUEVA LIBRARY
//	enviar_caught_pokemon(caught_pokemon, socket, -1, idCorrelativo);
//	log_debug(loggerObligatorio, "Mensaje enviado a Broker | ID Correlativo del Mensaje: %d - Bool enviado: %d", idCorrelativo, caught_pokemon->ok);

	free(caught_pokemon);
}

void procesarTeamAppearedPokemon(int socket, char* argv[]){
	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];

	t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
	appeared_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	appeared_pokemon->pokemon = pokemon;
	appeared_pokemon->posicion->posicionX = posX;
	appeared_pokemon->posicion->posicionX = posY;

	//TODO ENVIAR MENSAJE CON NUEVA LIBRARY
//	enviar_appeared_pokemon(appeared_pokemon, socket, -1, -1);
//	log_debug(loggerObligatorio, "Mensaje enviado a Team | Pokemon: %s - Posicion X: %d - Posicion Y: %d", appeared_pokemon->pokemon, appeared_pokemon->posicion->posicionX, appeared_pokemon->posicion->posicionY);

	free(appeared_pokemon->posicion);
	free(appeared_pokemon);
}

void procesarGameCardCatchPokemon(int socket, char* argv[]){
	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];
	int ID = (int) argv[6];

	t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
	catch_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	catch_pokemon->pokemon = pokemon;
	catch_pokemon->posicion->posicionX = posX;
	catch_pokemon->posicion->posicionX = posY;

	//TODO ENVIAR MENSAJE CON NUEVA LIBRARY
//	enviar_catch_pokemon(catch_pokemon, socket, ID, -1);
//	log_debug(loggerObligatorio, "Mensaje enviado a GameCard | Pokemon: %s - Posicion X: %d - Posicion Y: %d | ID del Mensaje: %d", catch_pokemon->pokemon, catch_pokemon->posicion->posicionX, catch_pokemon->posicion->posicionY, ID);

	free(catch_pokemon->posicion);
	free(catch_pokemon);
}


void leer_config(t_log* logger)
{
	t_config* config = config_create("./gameboy.config"); //

	if(config == NULL){
		printf("No pude leer la config \n");
		exit(2);
	}

	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	IP_TEAM = config_get_string_value(config, "IP_TEAM");
	IP_GAMECARD = config_get_string_value(config, "IP_GAMECARD");
	PUERTO_BROKER = config_get_string_value(config, "PUERTO_BROKER");
	PUERTO_TEAM = config_get_string_value(config, "PUERTO_TEAM");
	PUERTO_GAMECARD = config_get_string_value(config, "PUERTO_GAMECARD");

	IP_GAMEBOY = config_get_string_value(config, "IP_GAMEBOY");
	PUERTO_GAMEBOY = config_get_string_value(config, "PUERTO_GAMEBOY");

	log_info(logger, "CONFIG FILE -> IP Broker: %s", IP_BROKER);
	log_info(logger, "CONFIG FILE -> Puerto Broker: %s", PUERTO_BROKER);
	log_info(logger, "CONFIG FILE -> IP Team: %s", IP_TEAM);
	log_info(logger, "CONFIG FILE -> Puerto Team: %s", PUERTO_TEAM);
	log_info(logger, "CONFIG FILE -> IP GameCard: %s", IP_GAMECARD);
	log_info(logger, "CONFIG FILE -> Puerto GameCard: %s", PUERTO_GAMECARD);
}

t_log* iniciarLoggerObligatorio(void){

	t_log* logger;
	if((logger = log_create("./gameboyObligatorio.log", "GAMEBOY", 1, log_level_from_string("INFO"))) == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}
	return logger;
}

t_log* iniciarLogger(void){

	t_log* logger;
	if((logger = log_create("./gameboy.log", "GAMEBOY", 1, log_level_from_string("INFO"))) == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}
	return logger;
}

