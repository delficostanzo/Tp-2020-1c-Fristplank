

#include <stdio.h>
#include <stdlib.h>
#include "Broker.h"

static void envioAppearedPrueba1(int socketSuscripcionAppeared);
static void envioAppearedPrueba2(int socketSuscripcionAppeared);
static void envioLocalizedPrueba1(int socketSuscripcionLocalized);
static int recibirCatchDesde(int socketCatch);
static void enviarIdCatchA(socketIdCatch);

int main(void) {
	t_log* logger;
	t_config* config;

	// inicializo el log del Broker

	logger = iniciar_logger();
	// creo y devuelvo un puntero a la estructura t_config
	config = leer_config();

	char* IP_BROKER = config_get_string_value(config, "IP_BROKER");
	int PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");

	log_info(logger, "Lei la IP %s y PUERTO %d\n", IP_BROKER, PUERTO_BROKER);

	//crear conexion, un socket conectado
	int conexion = crearSocket();

	if(escuchaEn(conexion, PUERTO_BROKER)){
		log_info(logger, "Escuchando conexiones al Broker");
	}

	//se queda bloqueado esperando que los procesos se conecten
	int teamSocket = aceptarConexion(conexion);

	id_proceso idProcesoConectado;
	idProcesoConectado = iniciarHandshake(teamSocket, BROKER);
	log_info(logger, "El id del proceso con el que me conecte es: %d", idProcesoConectado);

	int suscripcionAppeared, suscripcionCaught, suscripcionLocalized, socketGet, socketIdGet, socketCatch, socketIdCatch;
	switch(idProcesoConectado) {
	case TEAM:
		suscripcionAppeared = aceptarConexion(conexion);
		suscripcionCaught = aceptarConexion(conexion);
		suscripcionLocalized = aceptarConexion(conexion);
		socketGet = aceptarConexion(conexion);
		socketIdGet = aceptarConexion(conexion);
		socketCatch = aceptarConexion(conexion);
		socketIdCatch = aceptarConexion(conexion);

		liberar_conexion(teamSocket);
		log_info(logger, "Team se suscribio a 3 colas");
		break;
	default:
		break;
	}

//	envioAppearedPrueba1(suscripcionAppeared);
//	envioAppearedPrueba2(suscripcionAppeared);
//	envioLocalizedPrueba1(suscripcionLocalized);


	if(recibirCatchDesde(socketCatch)){
		enviarIdCatchA(socketIdCatch);
	}

	//loguear mensaje recibido
	//log_info(logger, "El mensaje recibido es: %s\n", mensaje);

	//terminar_programa(conexion, logger, config);

}

void envioAppearedPrueba1(int socketSuscripcionAppeared) {
	char* nombrePoke = "Pikachu";

	t_appeared_pokemon* appearedPrueba = malloc(sizeof(t_appeared_pokemon));
	appearedPrueba->pokemon = nombrePoke;
	appearedPrueba->lengthOfPokemon = strlen(appearedPrueba->pokemon);

	appearedPrueba->posicion = malloc(sizeof(t_posicion));
	appearedPrueba->posicion->posicionX = 2;
	appearedPrueba->posicion->posicionY = 2;

	enviar_appeared_pokemon(appearedPrueba, socketSuscripcionAppeared, 1, -1);
}

void envioAppearedPrueba2(int socketSuscripcionAppeared) {
	char* nombrePoke = "Charmander";

	t_appeared_pokemon* appearedPrueba = malloc(sizeof(t_appeared_pokemon));
	appearedPrueba->pokemon = nombrePoke;
	appearedPrueba->lengthOfPokemon = strlen(appearedPrueba->pokemon);

	appearedPrueba->posicion = malloc(sizeof(t_posicion));
	appearedPrueba->posicion->posicionX = 8;
	appearedPrueba->posicion->posicionY = 8;

	enviar_appeared_pokemon(appearedPrueba, socketSuscripcionAppeared, 1, -1);
}


void envioLocalizedPrueba1(int socketSuscripcionLocalized) {
	char* nombrePoke = "Delfi";

	t_localized_pokemon* localizedPrueba = malloc(sizeof(t_localized_pokemon));
	localizedPrueba->pokemon = nombrePoke;
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

	enviar_localized_pokemon(localizedPrueba, socketSuscripcionLocalized, 1, -1);
}

//devuelvo 1 si se recibio el catch de un pokemon del team
int recibirCatchDesde(int socketCatch) {
	t_log* logger = iniciar_logger();
	t_paquete* paqueteCatchRecibido = recibir_mensaje(socketCatch);
	t_catch_pokemon* catchPoke = paqueteCatchRecibido->buffer->stream;

	log_info(logger, "Se recibio el mensaje catch");
	return catchPoke != NULL;
}

void enviarIdCatchA(int socketIdCatch) {
	t_respuesta_id* idCatch = malloc(sizeof(t_respuesta_id));
	int id= 1; //el id del mensaje catch que mando el entrenador es 1
	idCatch->idCorrelativo = id;
	//se podria pasar por id correlativo ?
	enviar_respuesta_id(idCatch, socketIdCatch, 1, -1);
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
