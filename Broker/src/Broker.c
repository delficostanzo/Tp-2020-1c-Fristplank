

#include <stdio.h>
#include <stdlib.h>
#include "Broker.h"

static void envioAppearedPrueba1(int socketSuscripcionAppeared);
static void envioAppearedPrueba2(int socketSuscripcionAppeared);
static void envioLocalizedPrueba1(int socketSuscripcionLocalized);
static int recibirCatchDesde(int socketCatch);
static void enviarIdCatchA(int socketIdCatch);
static void enviarCaughtA(int suscripcionCaught);
static void recibirGetDesde(int socketGet);
static void recibirACK(int socketACKAppeared);
static void destruirLog(t_log* logger);

int main(void) {
	t_log* logger;
	t_config* config;

	// inicializo el log del Broker

	logger = iniciar_logger();
	// creo y devuelvo un puntero a la estructura t_config
	config = leer_config();

	char* IP_BROKER = config_get_string_value(config, "IP_BROKER");
	int PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");
	log_info(logger, "Lei la IP %s y PUERTO %d", IP_BROKER, PUERTO_BROKER);



	//crear conexion, un socket conectado
	int conexion = crearSocket();

	if(escuchaEn(conexion, PUERTO_BROKER)){
		log_info(logger, "Escuchando conexiones al Broker");
	}

	//se queda bloqueado esperando que los procesos se conecten
	int teamSocket = aceptarConexion(conexion);

	t_handshake* handshakePropio = malloc(sizeof(t_handshake));
	handshakePropio->id = BROKER;
	handshakePropio->idUnico = 1;


	t_handshake* handshakeResponse;
	handshakeResponse = iniciarHandshake(teamSocket, handshakePropio);

	id_proceso idProcesoConectado = handshakeResponse->id;
	log_info(logger, "El id del proceso con el que me conecte es: %d", handshakeResponse->id);

	free(handshakePropio);
	free(handshakeResponse);

	int suscripcionAppeared, suscripcionCaught, suscripcionLocalized, socketGet, socketCatch, socketIdCatch;
	//int socketIdGet, socketACKAppeared, socketACKLocalized;
	int socketACKCaught;

	switch(idProcesoConectado) {
	case TEAM:
		log_info(logger, "Llego a ver que es team");

		socketGet = aceptarConexion(conexion);
		//socketIdGet = aceptarConexion(conexion);

		suscripcionAppeared = aceptarConexion(conexion);
		//socketACKAppeared = aceptarConexion(conexion);

		suscripcionLocalized = aceptarConexion(conexion);
		//socketACKLocalized = aceptarConexion(conexion);

		socketCatch = aceptarConexion(conexion);
		socketIdCatch = aceptarConexion(conexion);

		suscripcionCaught = aceptarConexion(conexion);
		socketACKCaught = aceptarConexion(conexion);


		liberar_conexion(teamSocket);
		log_info(logger, "Team se suscribio a 3 colas");
		break;
	default:
		break;
	}

	recibirGetDesde(socketGet);
	recibirGetDesde(socketGet);
	recibirGetDesde(socketGet);

	envioAppearedPrueba1(suscripcionAppeared);
	//recibirACK(socketACKAppeared);

	//envioLocalizedPrueba1(suscripcionLocalized);
	//recibirACK(socketACKLocalized);

	if(recibirCatchDesde(socketCatch)){
		enviarIdCatchA(socketIdCatch);

		sleep(5);
		enviarCaughtA(suscripcionCaught);
		recibirACK(socketACKCaught);
	}



	//recibirCatchDesde(socketCatch);
	//envioAppearedPrueba2(suscripcionAppeared);

	destruirLog(logger);

	//loguear mensaje recibido
	//log_info(logger, "El mensaje recibido es: %s\n", mensaje);

	//terminar_programa(conexion, logger, config);

}

void recibirGetDesde(int socketGet){
	t_log* logger = iniciar_logger();
	t_paquete* paqueteGetRecibido = recibir_mensaje(socketGet);
	t_get_pokemon* getPoke = paqueteGetRecibido->buffer->stream;

	log_info(logger, "Se recibio el get del Pokemon: %s", getPoke->pokemon);
	destruirLog(logger);
}

void recibirACK(int socketACK){
	t_log* logger = iniciar_logger();
	t_paquete* paqueteACK = recibir_mensaje(socketACK);

	log_info(logger, "Se recibio el ACK con ID: %d", paqueteACK->ID_CORRELATIVO);
	destruirLog(logger);
}

void envioAppearedPrueba1(int socketSuscripcionAppeared) {
	t_log* logger = iniciar_logger();
	//char* nombrePoke = "Pikachu";

	t_appeared_pokemon* appearedPrueba = malloc(sizeof(t_appeared_pokemon));
	appearedPrueba->pokemon = string_from_format("Pikachu");
	appearedPrueba->lengthOfPokemon = strlen(appearedPrueba->pokemon);

	appearedPrueba->posicion = malloc(sizeof(t_posicion));
	appearedPrueba->posicion->posicionX = 2;
	appearedPrueba->posicion->posicionY = 2;

	enviar_appeared_pokemon(appearedPrueba, socketSuscripcionAppeared, 1, -1);
	log_info(logger, "Envio del appeared del Pikachu");
	destruirLog(logger);
}

void envioAppearedPrueba2(int socketSuscripcionAppeared) {
	t_log* logger = iniciar_logger();

	t_appeared_pokemon* appearedPrueba = malloc(sizeof(t_appeared_pokemon));
	appearedPrueba->pokemon = string_from_format("Charmander");
	appearedPrueba->lengthOfPokemon = strlen(appearedPrueba->pokemon);

	appearedPrueba->posicion = malloc(sizeof(t_posicion));
	appearedPrueba->posicion->posicionX = 6;
	appearedPrueba->posicion->posicionY = 4;

	enviar_appeared_pokemon(appearedPrueba, socketSuscripcionAppeared, 8, -1);
	log_info(logger, "Envio del appeared del Charmander");
	destruirLog(logger);
}


void envioLocalizedPrueba1(int socketSuscripcionLocalized) {
	t_log* logger = iniciar_logger();
	t_localized_pokemon* localizedPrueba = malloc(sizeof(t_localized_pokemon));
	localizedPrueba->pokemon = string_from_format("Ganger");
	localizedPrueba->lengthOfPokemon = strlen(localizedPrueba->pokemon);

	t_posicion* posicion1 = malloc(sizeof(t_posicion));
	posicion1->posicionX = 6;
	posicion1->posicionY = 3;

	t_posicion* posicion2 = malloc(sizeof(t_posicion));
	posicion2->posicionX = 4;
	posicion2->posicionY = 1;

	t_list* posicionesPrueba = list_create();
	list_add(posicionesPrueba, posicion1);
	list_add(posicionesPrueba, posicion2);

	localizedPrueba->cantidadPosiciones = 2;
	localizedPrueba->listaPosiciones = posicionesPrueba;

	enviar_localized_pokemon(localizedPrueba, socketSuscripcionLocalized, 4, -1);
	log_info(logger, "Envio del localized del Ganger");
	destruirLog(logger);
}

//devuelvo 1 si se recibio el catch de un pokemon del team
int recibirCatchDesde(int socketCatch) {
	t_log* logger = iniciar_logger();
	t_paquete* paqueteCatchRecibido = recibir_mensaje(socketCatch);
	t_catch_pokemon* catchPoke = paqueteCatchRecibido->buffer->stream;

	log_info(logger, "Se recibio el catch | Pokemon: %s - Posicion X: %d - Posicion Y: %d", catchPoke->pokemon, catchPoke->posicion->posicionX, catchPoke->posicion->posicionY);
	destruirLog(logger);
	return catchPoke != NULL;
}

void enviarIdCatchA(int socketIdCatch) {
	t_log* logger = iniciar_logger();
	t_respuesta_id* idCatch = malloc(sizeof(t_respuesta_id));
	int id= 55; //el id del mensaje catch que mando el entrenador
	idCatch->idCorrelativo = id;
	//se podria pasar por id correlativo ?
	enviar_respuesta_id(idCatch, socketIdCatch, 1, 55);
	log_info(logger, "Se envio el id del catch");
	destruirLog(logger);
}

void enviarCaughtA(int suscripcionCaught) {
	t_log* logger = iniciar_logger();
	t_caught_pokemon* caughtPoke = malloc(sizeof(t_caught_pokemon));
	uint32_t loAtrapo = 1;
	caughtPoke->ok = loAtrapo;
	enviar_caught_pokemon(caughtPoke, suscripcionCaught, 3, 55);
	log_info(logger, "Se envio el caught del catch 55");
	destruirLog(logger);
}







t_log* iniciar_logger(void){
	t_log * log = malloc(sizeof(t_log));
	log = log_create("broker.log", "BROKER", 1,0);
	if(log == NULL){
			printf("No pude crear el logger \n");
			exit(1);
	}
	log_info(log,"Logger Iniciado");
	return log;
}

t_config* leer_config()
{
	t_config* config = config_create("src/broker.config");
	t_log* logger = iniciar_logger();

	if(config == NULL){
		printf("No pude leer la config \n");
		exit(2);
	}
	log_info(logger,"Archivo de configuracion seteado");
	destruirLog(logger);
	return config;
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

void destruirLog(t_log* logger) {
	if (logger != NULL) {
		log_destroy(logger);
	}
}
