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
	log_debug(logger, "Logger creado.");
	config = leer_config();

/*	DONE ./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]
 * ejemplo: ./GameBoy BROKER NEW_POKEMON "pikachu" 3 7 2
 *
 * DONE ./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]
 * ejemplo:
 *
 * DONE ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]
 * ejemplo:
 *
 * DONE ./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE] [OK/FAIL]
 * ejemplo:
 *
 * DONE ./gameboy BROKER GET_POKEMON [POKEMON]
 * ejemplo:
 *
 * DONE ./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]
 * ejemplo:
 *
 * DONE ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]
 * ejemplo:
 *
 * DONE ./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]
 * ejemplo:
 *
 * DONE ./gameboy GAMECARD GET_POKEMON [POKEMON]
 * ejemplo:
 *
 * DONE ./gameboy SUSCRIPTOR [COLA_DE_MENSAJES] [TIEMPO]
 * ejemplo:	./GameBoy SUSCRIPTOR CATCH_POKEMON 10
*/

	int socket;

	//MENSAJES AL BROKER
	if (strcmp(argv[1], "BROKER") == 0){

		socket = conectarAModulo(PUERTO_BROKER, IP_BROKER);
		log_info(logger, "Conexión hecha a IP %s | Puerto %s", IP_BROKER, PUERTO_BROKER);

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

	//MENSAJES AL TEAM
	else if (strcmp(argv[1], "TEAM") == 0){

		socket = conectarAModulo(PUERTO_TEAM, IP_TEAM);
		log_info(logger, "Conexión hecha a IP %s | Puerto %s", IP_TEAM, PUERTO_TEAM);

		if (strcmp(argv[2], "APPEARED_POKEMON") == 0){
			procesarTeamAppearedPokemon(socket, argv);
		}

		else{
			log_info(logger, "Tipo de mensaje incorrecto.");
		}
	}

	//MENSAJES AL GAMECARD
	else if (strcmp(argv[1], "GAMECARD") == 0){

		socket = conectarAModulo(PUERTO_GAMECARD, IP_GAMECARD);
		log_info(logger, "Conexión hecha a IP %s | Puerto %s", IP_GAMECARD, PUERTO_GAMECARD);

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

	//MENSAJES PARA SUSCBRIBIRSE AL BROKER POR X SEGUNDOS
	else if (strcmp(argv[1], "SUSCRIPTOR") == 0){

		procesarSubscribe(argv);
	}

	else{
		log_info(logger, "Proceso incorrecto.");
	}


	log_debug(logger, "Preparando para terminar programa...");

	log_debug(logger, "Liberando conexion...");
	close(socket);

	log_debug(logger, "Destruyendo config...");
	config_destroy(config);

	log_debug(logger, "Destruyendo logger...");
	log_destroy(logger);

	return 0;
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

	int segundosAEscuchar = atoi(argv[3]);

	pthread_t threadEscucha;
	pthread_create(&threadEscucha, NULL, escucharCola, (void*) cola);
	sleep(segundosAEscuchar);
	pthread_cancel(threadEscucha);
}

void* escucharCola(void* colaAEscuchar){
	op_code cola = (op_code) colaAEscuchar;

	int conexionBroker = conectarAModulo(PUERTO_BROKER, IP_BROKER);

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
	liberar_conexion(conexionBroker);
	return NULL;
}


/* OBJETIVO Conecta al módulo que sea y realiza el Handshake
 * PARAMETROS Precisa del Puerto y el IP en formato String
 * RETORNA socket
 */
int conectarAModulo(String PUERTO, String IP){
	int conexion = crearSocket();

	int puertoInt = atoi(PUERTO);
	if(conectarA(conexion, IP, puertoInt)){
		log_debug(logger, "Conectando al módulo...");
	}

	id_proceso idProceso;
	idProceso = responderHandshake(conexion, GAMEBOY);
	log_debug(logger, "El id del proceso con el que me conecte es: %d", idProceso);

	return conexion;
}

t_config* leer_config(void)
{
	t_config* config = config_create("./gameboy.config");

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

	log_debug(logger, "CONFIG FILE -> IP Broker: %s", IP_BROKER);
	log_debug(logger, "CONFIG FILE -> Puerto Broker: %s", PUERTO_BROKER);
	log_debug(logger, "CONFIG FILE -> IP Team: %s", IP_TEAM);
	log_debug(logger, "CONFIG FILE -> Puerto Team: %s", PUERTO_TEAM);
	log_debug(logger, "CONFIG FILE -> IP GameCard: %s", IP_GAMECARD);
	log_debug(logger, "CONFIG FILE -> Puerto GameCard: %s", PUERTO_GAMECARD);

	return config;
}

t_log* iniciarLogger(void){

	t_log* logger;
	if((logger = log_create("./gameboy.log", "GAMEBOY", 1, log_level_from_string("DEBUG"))) == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}
	return logger;
}

