#include "GameBoy.h"

int main(int argc, char* argv[]) {

	//Inicio el logger para debuggear nosotros y también el que piden en la consigna.
	/* Las acciones a loguear en este archivo son:
		* Conexión a cualquier proceso.
		* Suscripción a una cola de mensajes.
		* Llegada de un nuevo mensaje a una cola de mensajes.
		* Envío de un mensaje a un suscriptor específico.
	 */

	logger = iniciar_logger();
	log_info(logger, "Logger creado.");
	loggerObligatorio = iniciarLoggerObligatorio();
	log_info(logger, "Logger obligatorio creado.");
	leer_config(logger);

//  ./gameboy [PROCESO] [TIPO_MENSAJE] [ARGUMENTOS]*

//	DONE ./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]
//	./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]
//	./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]
//	./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE] [OK/FAIL]
//	./gameboy BROKER GET_POKEMON [POKEMON]
//	./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]
//  ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]
//	./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]
//	./gameboy GAMECARD GET_POKEMON [POKEMON]
//	./gameboy SUSCRIPTOR [COLA_DE_MENSAJES] [TIEMPO]
//

	if (strcmp(argv[1], "BROKER") == 0){ //MENSAJES AL TEAM

		int socket = crear_conexion(IP_BROKER, PUERTO_BROKER);
		log_debug(loggerObligatorio, "Conexión hecha a IP %s | Puerto %s", IP_BROKER, PUERTO_BROKER);

		if (strcmp(argv[2], "NEW_POKEMON")){
			procesarBrokerNewPokemon(socket, argv);
		}
		else if (strcmp(argv[2], "APPEARED_POKEMON")){

		}
		else if (strcmp(argv[2], "CATCH_POKEMON")){

		}
		else if (strcmp(argv[2], "CAUGHT_POKEMON")){

		}
		else if (strcmp(argv[2], "GET_POKEMON")){

		}
		else{
			log_info(logger, "Tipo de mensaje incorrecto.");
		}
	}
	else if (strcmp(argv[1], "TEAM") == 0){ //MENSAJES AL TEAM

	}
	else if (strcmp(argv[1], "GAMECARD") == 0){ //MENSAJES AL GAMECARD

	}
	else if (strcmp(argv[1], "SUSCRIPTOR") == 0){ //MENSAJES PARA SUSCBRIBIRSE AL BROKER POR X SEGUNDOS

	}

	else{
		log_info(logger, "Proceso incorrecto.");
	}
}

void procesarBrokerNewPokemon(int socket, char* argv[]){
	char* pokemon = (char*)argv[3];
	int posX = (int) argv[4];
	int posY = (int) argv[5];
	int cant = (int) argv[6];

	t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
	new_pokemon->pokemon = pokemon;
	new_pokemon->cantidad = cant;
	new_pokemon->posicionX = posX;
	new_pokemon->posicionY = posY;

	enviar_new_pokemon(new_pokemon, socket);
	log_debug(loggerObligatorio, "Mensaje enviado a Broker | Pokemon: %s - Posicion X: %d - Posicion Y: %d - Cantidad: %d", new_pokemon->pokemon, new_pokemon->posicionX, new_pokemon->posicionY, new_pokemon->cantidad);

	free(new_pokemon);
}

void leer_config(t_log* logger)
{
	t_config* config = config_create("./src/gameboy.config");

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

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger != NULL){
		log_destroy(logger);
	}

	if(config != NULL){
		config_destroy(config); //destruye la esctructura de config en memoria, no lo esta eliminando el archivo de config
	}


	liberar_conexion(conexion);

}

t_log* iniciarLoggerObligatorio(void){

	t_log* logger;
	if((logger = log_create("./gameboyObligatorio.log", "GAMEBOY", 1, log_level_from_string("INFO"))) == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}
	return logger;
}

t_log* iniciar_logger(void){

	t_log* logger;
	if((logger = log_create("./gameboy.log", "GAMEBOY", 1, log_level_from_string("INFO"))) == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}
	return logger;
}

