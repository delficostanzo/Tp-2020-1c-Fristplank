/*
 * procesar.c
 *
 *  Created on: 4 jun. 2020
 *      Author: utnso
 */

#include "procesar.h"

void procesarBrokerNewPokemon(int socket, char* argv[]){

	if(argv[6] == NULL){
		log_error(logger, "Faltan ingresar parámetros. Se aborta la ejecución.");
		exit(1);
	}

	t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
	new_pokemon->lengthOfPokemon = strlen(argv[3]);
	new_pokemon->pokemon = string_from_format(argv[3]);
	new_pokemon->cantidad = atoi(argv[6]);
	new_pokemon->posicion = malloc(sizeof(t_posicion));
	new_pokemon->posicion->posicionX = atoi(argv[4]);
	new_pokemon->posicion->posicionY = atoi(argv[5]);

	enviar_new_pokemon(new_pokemon, socket, -1, -1);
	log_info(logger, "Mensaje enviado a Broker | Pokemon: %s - Posicion (X,Y) = (%d,%d) - Cantidad: %d", argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
}

void procesarGameCardNewPokemon(int socket, char* argv[]){

	if(argv[7] == NULL){
		log_error(logger, "Faltan ingresar parámetros. Se aborta la ejecución.");
		exit(1);
	}

	t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
	new_pokemon->lengthOfPokemon = strlen(argv[3]);
	new_pokemon->pokemon = string_from_format(argv[3]);
	new_pokemon->cantidad = atoi(argv[6]);
	new_pokemon->posicion = malloc(sizeof(t_posicion));
	new_pokemon->posicion->posicionX = atoi(argv[4]);
	new_pokemon->posicion->posicionY = atoi(argv[5]);

	enviar_new_pokemon(new_pokemon, socket, atoi(argv[7]), -1);
	log_info(logger, "Mensaje enviado a GameCard | ID del Mensaje: %d | Pokemon: %s - Posicion (X,Y) = (%d,%d) - Cantidad: %d", atoi(argv[7]), argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
}


void procesarGameCardGetPokemon(int socket, char* argv[]){

	if(argv[4] == NULL){
		log_error(logger, "Faltan ingresar parámetros. Se aborta la ejecución.");
		exit(1);
	}

	t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));
	get_pokemon->lengthOfPokemon = strlen(argv[3]);
	get_pokemon->pokemon = string_from_format(argv[3]);

	enviar_get_pokemon(get_pokemon, socket, atoi(argv[4]), -1);
	log_info(logger, "Mensaje enviado a GameCard | ID del mensaje: %d | Pokemon: %s", atoi(argv[4]), argv[3]);
}

void procesarBrokerGetPokemon(int socket, char* argv[]){

	if(argv[3] == NULL){
		log_error(logger, "Faltan ingresar parámetros. Se aborta la ejecución.");
		exit(1);
	}

	t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));
	get_pokemon->lengthOfPokemon = strlen(argv[3]);
	get_pokemon->pokemon = string_from_format(argv[3]);

	enviar_get_pokemon(get_pokemon, socket, -1, -1);
	log_info(logger, "Mensaje enviado a GameCard | Pokemon: %s", argv[3]);
}

void procesarBrokerAppearedPokemon(int socket, char* argv[]){

	if(argv[6] == NULL){
		log_error(logger, "Faltan ingresar parámetros. Se aborta la ejecución.");
		exit(1);
	}

	t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
	appeared_pokemon->lengthOfPokemon = strlen(argv[3]);
	appeared_pokemon->pokemon = string_from_format(argv[3]);
	appeared_pokemon->posicion = malloc(sizeof(t_posicion));
	appeared_pokemon->posicion->posicionX = atoi(argv[4]);
	appeared_pokemon->posicion->posicionY = atoi(argv[5]);

	enviar_appeared_pokemon(appeared_pokemon, socket, -1, atoi(argv[6]));
	log_info(logger, "Mensaje enviado a Broker | Pokemon: %s - Posicion (X,Y) = (%d,%d) | ID Correlativo del Mensaje: %d", argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
}

void procesarBrokerCatchPokemon(int socket, char* argv[]){

	if(argv[5] == NULL){
		log_error(logger, "Faltan ingresar parámetros. Se aborta la ejecución.");
		exit(1);
	}

	t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
	catch_pokemon->lengthOfPokemon = strlen(argv[3]);
	catch_pokemon->pokemon = string_from_format(argv[3]);
	catch_pokemon->posicion = malloc(sizeof(t_posicion));
	catch_pokemon->posicion->posicionX = atoi(argv[4]);
	catch_pokemon->posicion->posicionY = atoi(argv[5]);

	enviar_catch_pokemon(catch_pokemon, socket, -1, -1);
	log_info(logger, "Mensaje enviado a Broker | Pokemon: %s - Posicion (X,Y) = (%d,%d)", argv[3], atoi(argv[4]), atoi(argv[5]));
}

void procesarBrokerCaughtPokemon(int socket, char* argv[]){

	if(argv[4] == NULL){
		log_error(logger, "Faltan ingresar parámetros. Se aborta la ejecución.");
		exit(1);
	}

	t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));

	if(string_equals_ignore_case("FAIL", argv[4])){
		caught_pokemon->ok = 0;
	}
	else if(string_equals_ignore_case("OK", argv[4])){
		caught_pokemon->ok = 1;
	}

	else{

	}

	enviar_caught_pokemon(caught_pokemon, socket, -1, atoi(argv[3]));
	log_info(logger, "Mensaje enviado a Broker | ID Correlativo del Mensaje: %d - Bool enviado: %d", atoi(argv[3]), atoi(argv[4]));
}

void procesarTeamAppearedPokemon(int socket, char* argv[]){

	if(argv[5] == NULL){
		log_error(logger, "Faltan ingresar parámetros. Se aborta la ejecución.");
		exit(1);
	}

	t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
	appeared_pokemon->lengthOfPokemon = strlen(argv[3]);
	appeared_pokemon->pokemon = string_from_format(argv[3]);
	appeared_pokemon->posicion = malloc(sizeof(t_posicion));
	appeared_pokemon->posicion->posicionX = atoi(argv[4]);
	appeared_pokemon->posicion->posicionY = atoi(argv[5]);

	enviar_appeared_pokemon(appeared_pokemon, socket, -1, -1);
	log_info(logger, "Mensaje enviado a Team | Pokemon: %s - Posicion (X,Y) = (%d,%d)", argv[3], atoi(argv[4]), atoi(argv[5]));
}

void procesarGameCardCatchPokemon(int socket, char* argv[]){

	if(argv[6] == NULL){
		log_error(logger, "Faltan ingresar parámetros. Se aborta la ejecución.");
		exit(1);
	}


	t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
	catch_pokemon->lengthOfPokemon = strlen(argv[3]);
	catch_pokemon->pokemon = string_from_format(argv[3]);
	catch_pokemon->posicion = malloc(sizeof(t_posicion));
	catch_pokemon->posicion->posicionX = atoi(argv[4]);
	catch_pokemon->posicion->posicionY = atoi(argv[5]);

	enviar_catch_pokemon(catch_pokemon, socket, atoi(argv[6]), -1);
	log_info(logger, "Mensaje enviado a GameCard | Pokemon: %s - Posicion (X,Y) = (%d,%d) | ID del Mensaje: %d", argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
}
