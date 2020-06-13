/*
 * procesar.c
 *
 *  Created on: 4 jun. 2020
 *      Author: utnso
 */

#include "procesar.h"

void procesarBrokerNewPokemon(int socket, char* argv[]){

	log_debug(logger, "Comienza procesarBrokerNewPokemon");
	log_debug(logger, "Mensaje a enviar a Broker | Pokemon: %s - Posicion X: %d - Posicion Y: %d - Cantidad: %d", (char*) argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

	t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
	new_pokemon->pokemon = malloc(strlen(argv[3] + 1));
	new_pokemon->pokemon = argv[3];
	new_pokemon->lengthOfPokemon = strlen(argv[3]) + 1;
	new_pokemon->cantidad = atoi(argv[6]);
	new_pokemon->posicion = malloc(sizeof(t_posicion));
	new_pokemon->posicion->posicionX = atoi(argv[4]);
	new_pokemon->posicion->posicionY = atoi(argv[5]);

	enviar_new_pokemon(new_pokemon, socket, -1, -1);
	log_info(logger, "Mensaje enviado a Broker | Pokemon: %s - Posicion X: %d - Posicion Y: %d - Cantidad: %d", new_pokemon->pokemon, new_pokemon->posicion->posicionX, new_pokemon->posicion->posicionY, new_pokemon->cantidad);

	log_debug(logger, "Termina procesarBrokerNewPokemon");

}

void procesarGameCardNewPokemon(int socket, char* argv[]){
	log_debug(logger, "Mensaje a enviar a GameCard | Pokemon: %s - Posicion X: %d - Posicion Y: %d - Cantidad: %d", (char*) argv[3], (int) argv[4], (int) argv[5], (int) argv[6]);

	t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
	new_pokemon->pokemon = argv[3];
	new_pokemon->lengthOfPokemon = strlen(argv[3]);
	new_pokemon->cantidad = atoi(argv[6]);
	new_pokemon->posicion = malloc(sizeof(t_posicion));
	new_pokemon->posicion->posicionX = atoi(argv[4]);
	new_pokemon->posicion->posicionY = atoi(argv[5]);

	enviar_new_pokemon(new_pokemon, socket, atoi(argv[7]), -1);
	log_info(logger, "Mensaje enviado a GameCard | ID del Mensaje: %d | Pokemon: %s - Posicion X: %d - Posicion Y: %d - Cantidad: %d", argv[7], new_pokemon->pokemon, new_pokemon->posicion->posicionX, new_pokemon->posicion->posicionY, new_pokemon->cantidad);

	free(new_pokemon->posicion);
	free(new_pokemon);
}


void procesarGameCardGetPokemon(int socket, char* argv[]){
	t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));
	get_pokemon->lengthOfPokemon = strlen(argv[3]) + 1;
	get_pokemon->pokemon = argv[3];


	enviar_get_pokemon(get_pokemon, socket, -1, -1);
	log_info(logger, "Mensaje enviado a GameCard | Pokemon: %s", get_pokemon->pokemon);

	free(get_pokemon);
}

void procesarBrokerGetPokemon(int socket, char* argv[]){
	t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));
	get_pokemon->lengthOfPokemon = strlen(argv[3]) + 1;
	get_pokemon->pokemon = argv[3];


	enviar_get_pokemon(get_pokemon, socket, -1, -1);
	log_info(logger, "Mensaje enviado a GameCard | Pokemon: %s", get_pokemon->pokemon);

	free(get_pokemon);
}

void procesarBrokerAppearedPokemon(int socket, char* argv[]){
	t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
	appeared_pokemon->lengthOfPokemon = strlen(argv[3]) + 1;
	appeared_pokemon->pokemon = argv[3];
	appeared_pokemon->posicion->posicionX = atoi(argv[4]);
	appeared_pokemon->posicion->posicionX = atoi(argv[5]);

	enviar_appeared_pokemon(appeared_pokemon, socket, -1, atoi(argv[6]));
	log_info(logger, "Mensaje enviado a Broker | Pokemon: %s - Posicion X: %d - Posicion Y: %d | ID Correlativo del Mensaje: %d", appeared_pokemon->pokemon, appeared_pokemon->posicion->posicionX, appeared_pokemon->posicion->posicionY, atoi(argv[6]));

	free(appeared_pokemon->posicion);
	free(appeared_pokemon);
}

void procesarBrokerCatchPokemon(int socket, char* argv[]){
	t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
	catch_pokemon->lengthOfPokemon = strlen(argv[3]) + 1;
	catch_pokemon->pokemon = argv[3];
	catch_pokemon->posicion->posicionX = atoi(argv[4]);
	catch_pokemon->posicion->posicionX = atoi(argv[5]);

	enviar_catch_pokemon(catch_pokemon, socket, -1, -1);
	log_info(logger, "Mensaje enviado a Broker | Pokemon: %s - Posicion X: %d - Posicion Y: %d", catch_pokemon->pokemon, catch_pokemon->posicion->posicionX, catch_pokemon->posicion->posicionY);

	free(catch_pokemon->posicion);
	free(catch_pokemon);
}

void procesarBrokerCaughtPokemon(int socket, char* argv[]){
	t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));
	caught_pokemon->ok = atoi(argv[4]);

	enviar_caught_pokemon(caught_pokemon, socket, -1, atoi(argv[3]));
	log_info(logger, "Mensaje enviado a Broker | ID Correlativo del Mensaje: %d - Bool enviado: %d", atoi(argv[3]), caught_pokemon->ok);

	free(caught_pokemon);
}

void procesarTeamAppearedPokemon(int socket, char* argv[]){
	t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
	appeared_pokemon->lengthOfPokemon = strlen(argv[3]);
	appeared_pokemon->pokemon = malloc(strlen(argv[3]) + 1);
	appeared_pokemon->pokemon = argv[3];
	appeared_pokemon->posicion = malloc(sizeof(t_posicion));
	appeared_pokemon->posicion->posicionX = atoi(argv[4]);
	appeared_pokemon->posicion->posicionY = atoi(argv[5]);
	enviar_appeared_pokemon(appeared_pokemon, socket, -1, -1);
	log_info(logger, "Mensaje enviado a Team | Pokemon: %s - Posicion X: %d - Posicion Y: %d", appeared_pokemon->pokemon, appeared_pokemon->posicion->posicionX, appeared_pokemon->posicion->posicionY);

	free(appeared_pokemon->posicion);
	free(appeared_pokemon);
}

void procesarGameCardCatchPokemon(int socket, char* argv[]){
	t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
	catch_pokemon->lengthOfPokemon = strlen(argv[3]) + 1;
	catch_pokemon->pokemon = argv[3];
	catch_pokemon->posicion->posicionX = atoi(argv[4]);
	catch_pokemon->posicion->posicionX = atoi(argv[5]);

	enviar_catch_pokemon(catch_pokemon, socket, atoi(argv[6]), -1);
	log_info(logger, "Mensaje enviado a GameCard | Pokemon: %s - Posicion X: %d - Posicion Y: %d | ID del Mensaje: %d", catch_pokemon->pokemon, catch_pokemon->posicion->posicionX, catch_pokemon->posicion->posicionY, atoi(argv[6]));

	free(catch_pokemon->posicion);
	free(catch_pokemon);
}
