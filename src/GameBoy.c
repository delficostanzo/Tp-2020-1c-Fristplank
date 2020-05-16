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
//	DONE ./gameboy SUSCRIPTOR [COLA_DE_MENSAJES] [TIEMPO]
//

int socket;

	if (strcmp(argv[1], "BROKER") == 0){ //MENSAJES AL BROKER

		socket = crear_conexion(IP_BROKER, PUERTO_BROKER);
		log_debug(loggerObligatorio, "Conexión hecha a IP %s | Puerto %s", IP_BROKER, PUERTO_BROKER);

		if (strcmp(argv[2], "NEW_POKEMON")){
			procesarNewPokemon(socket, argv, "Broker");
		}
		else if (strcmp(argv[2], "APPEARED_POKEMON")){
			procesarBrokerAppearedPokemon(socket, argv);
		}
		else if (strcmp(argv[2], "CATCH_POKEMON")){
			procesarBrokerCatchPokemon(socket, argv);
		}
		else if (strcmp(argv[2], "CAUGHT_POKEMON")){
			procesarBrokerCaughtPokemon(socket, argv);
		}
		else if (strcmp(argv[2], "GET_POKEMON")){
			procesarGetPokemon(socket, argv, "Broker");
		}
		else{
			log_info(logger, "Tipo de mensaje incorrecto.");
		}

	}
	else if (strcmp(argv[1], "TEAM") == 0){ //MENSAJES AL TEAM
		socket = crear_conexion(IP_TEAM, PUERTO_TEAM);
		log_debug(loggerObligatorio, "Conexión hecha a IP %s | Puerto %s", IP_TEAM, PUERTO_TEAM);

		if (strcmp(argv[2], "APPEARED_POKEMON")){
			procesarTeamAppearedPokemon(socket, argv);
		}

		else{
			log_info(logger, "Tipo de mensaje incorrecto.");
		}
	}
	else if (strcmp(argv[1], "GAMECARD") == 0){ //MENSAJES AL GAMECARD

		socket = crear_conexion(IP_GAMECARD, PUERTO_GAMECARD);
		log_debug(loggerObligatorio, "Conexión hecha a IP %s | Puerto %s", IP_GAMECARD, PUERTO_GAMECARD);

		if (strcmp(argv[2], "NEW_POKEMON")){
			procesarNewPokemon(socket, argv, "GameCard");
		}
		else if (strcmp(argv[2], "CATCH_POKEMON")){
			procesarGamecardCatchPokemon(socket, argv);
		}
		else if (strcmp(argv[2], "GET_POKEMON")){
			procesarGetPokemon(socket, argv, "GameCard");
		}
		else{
			log_info(logger, "Tipo de mensaje incorrecto.");
		}

	}
	else if (strcmp(argv[1], "SUSCRIPTOR") == 0){ //MENSAJES PARA SUSCBRIBIRSE AL BROKER POR X SEGUNDOS
		socket = crear_conexion(IP_BROKER, PUERTO_BROKER);
		log_debug(loggerObligatorio, "Conexión hecha a IP %s | Puerto %s", IP_BROKER, PUERTO_BROKER);

		procesarSubscribe(socket, argv);
	}

	else{
		log_info(logger, "Proceso incorrecto.");
	}

	liberar_conexion(socket);
	log_destroy(logger);
	log_destroy(loggerObligatorio);
}

void procesarSubscribe(int socket, char* argv[]){ //TODO -> HAY QUE VER COMO HACER CON EL TIEMPO (argv[3])

	op_code cola;

	if (strcmp(argv[2], "NEW_POKEMON")){
		cola = NEW_POKEMON;
	}
	else if (strcmp(argv[2], "APPEARED_POKEMON")){
		cola = APPEARED_POKEMON;
	}
	else if (strcmp(argv[2], "CATCH_POKEMON")){
		cola = CATCH_POKEMON;
	}
	else if (strcmp(argv[2], "CAUGHT_POKEMON")){
		cola = CAUGHT_POKEMON;
	}
	else if (strcmp(argv[2], "GET_POKEMON")){
		cola = GET_POKEMON;
	}
	else if (strcmp(argv[2], "LOCALIZED_POKEMON")){
		cola = LOCALIZED_POKEMON;
	}

	t_subscribe* subscribe = malloc(sizeof(subscribe));
	subscribe->cola = cola;

	enviar_subscribe(subscribe, socket, -1);
	log_debug(loggerObligatorio, "Mensaje enviado a Broker | Cola a suscribirnos: %s", argv[3]);

	free(subscribe);
}

void procesarNewPokemon(int socket, char* argv[], char* nombreModulo){
	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];
	int cant = (int) argv[6];

	t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
	new_pokemon->pokemon = pokemon;
	new_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	new_pokemon->cantidad = cant;
	new_pokemon->posicionX = posX;
	new_pokemon->posicionY = posY;

	enviar_new_pokemon(new_pokemon, socket, -1);
	log_debug(loggerObligatorio, "Mensaje enviado a %s | Pokemon: %s - Posicion X: %d - Posicion Y: %d - Cantidad: %d", nombreModulo, new_pokemon->pokemon, new_pokemon->posicionX, new_pokemon->posicionY, new_pokemon->cantidad);

	free(new_pokemon);
}

void procesarGetPokemon(int socket, char* argv[], char* nombreModulo){
	char* pokemon = (char*) argv[3];

	t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));
	get_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	get_pokemon->pokemon = pokemon;

	enviar_get_pokemon(get_pokemon, socket, -1);
	log_debug(loggerObligatorio, "Mensaje enviado a %s | Pokemon: %s", nombreModulo, get_pokemon->pokemon);

	free(get_pokemon);
}

void procesarBrokerAppearedPokemon(int socket, char* argv[]){
	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];
	int ID = (int) argv[6];

	t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
	appeared_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	appeared_pokemon->pokemon = pokemon;
	appeared_pokemon->posicionX = posX;
	appeared_pokemon->posicionY = posY;

	enviar_appeared_pokemon(appeared_pokemon, socket, ID);
	log_debug(loggerObligatorio, "Mensaje enviado a Broker | Pokemon: %s - Posicion X: %d - Posicion Y: %d | ID del Mensaje: %d", appeared_pokemon->pokemon, appeared_pokemon->posicionX, appeared_pokemon->posicionY, ID);

	free(appeared_pokemon);
}

void procesarBrokerCatchPokemon(int socket, char* argv[]){
	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];

	t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
	catch_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	catch_pokemon->pokemon = pokemon;
	catch_pokemon->posicionX = posX;
	catch_pokemon->posicionY = posY;

	enviar_catch_pokemon(catch_pokemon, socket, -1);
	log_debug(loggerObligatorio, "Mensaje enviado a Broker | Pokemon: %s - Posicion X: %d - Posicion Y: %d", catch_pokemon->pokemon, catch_pokemon->posicionX, catch_pokemon->posicionY);

	free(catch_pokemon);
}

void procesarBrokerCaughtPokemon(int socket, char* argv[]){
	int ID = (int) argv[3];
	bool ok = (bool) argv[4];

	t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));
	caught_pokemon->ok = ok;

	enviar_caught_pokemon(caught_pokemon, socket, -1);
	log_debug(loggerObligatorio, "Mensaje enviado a Broker | ID del Mensaje: %d - Bool enviado: %d", ID, caught_pokemon->ok);

	free(caught_pokemon);
}

void procesarTeamAppearedPokemon(int socket, char* argv[]){
	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];

	t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
	appeared_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	appeared_pokemon->pokemon = pokemon;
	appeared_pokemon->posicionX = posX;
	appeared_pokemon->posicionY = posY;

	enviar_appeared_pokemon(appeared_pokemon, socket, -1);
	log_debug(loggerObligatorio, "Mensaje enviado a Team | Pokemon: %s - Posicion X: %d - Posicion Y: %d", appeared_pokemon->pokemon, appeared_pokemon->posicionX, appeared_pokemon->posicionY);

	free(appeared_pokemon);
}

void procesarGamecardCatchPokemon(int socket, char* argv[]){
	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];
	int ID = (int) argv[6];

	t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
	catch_pokemon->lengthOfPokemon = strlen(pokemon + 1);
	catch_pokemon->pokemon = pokemon;
	catch_pokemon->posicionX = posX;
	catch_pokemon->posicionY = posY;

	enviar_catch_pokemon(catch_pokemon, socket, ID);
	log_debug(loggerObligatorio, "Mensaje enviado a GameCard | Pokemon: %s - Posicion X: %d - Posicion Y: %d | ID del Mensaje: %d", catch_pokemon->pokemon, catch_pokemon->posicionX, catch_pokemon->posicionY, ID);

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

