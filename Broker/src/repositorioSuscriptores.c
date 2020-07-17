/*
 * repositorioSuscriptores.c
 *
 *  Created on: 9 jul. 2020
 *      Author: utnso
 */

#include "repositorioSuscriptores.h"

void init_repositorio_suscriptores(){
	suscriptoresGameBoy = list_create();
	suscriptoresTeam = list_create();
	suscriptoresGameCard = list_create();
	pthread_mutex_init(&mutexRepoGameBoy, NULL);
	pthread_mutex_init(&mutexRepoTeam, NULL);
	pthread_mutex_init(&mutexRepoGameCard, NULL);
}

void finish_repositorio_suscriptores(){

	int sizeListaTeam = list_size(suscriptoresTeam);
	for(int i = 0; i < sizeListaTeam; i++){
		t_suscriptor_team* suscriptor = list_get(suscriptoresTeam, i);
		free(suscriptor);
	}

	int sizeListaGameCard = list_size(suscriptoresGameCard);
	for(int i = 0; i < sizeListaGameCard; i++){
		t_suscriptor_gamecard* suscriptor = list_get(suscriptoresGameCard, i);
		free(suscriptor);
	}

	list_destroy(suscriptoresGameBoy);
	list_destroy(suscriptoresTeam);
	list_destroy(suscriptoresGameCard);
	pthread_mutex_destroy(&mutexRepoGameBoy);
	pthread_mutex_destroy(&mutexRepoTeam);
	pthread_mutex_destroy(&mutexRepoGameCard);
}

void agregar_suscriptor_gameboy(t_suscriptor_gameboy* gameboy){
	list_add(suscriptoresGameBoy, gameboy);
}

void agregar_suscriptor_team(t_suscriptor_team* team){
	list_add(suscriptoresTeam, team);
}

void agregar_suscriptor_gamecard(t_suscriptor_gamecard* gamecard){
	list_add(suscriptoresGameCard, gamecard);
}

void reemplazar_suscriptor_team(t_suscriptor_team* team){

	int sizeLista = list_size(suscriptoresTeam);
	int posicion = -1;
	for(int i = 0; i < sizeLista; i++){
		t_suscriptor_team* suscriptor = list_get(suscriptoresTeam, i);
		if(suscriptor->id == team->id){
			posicion = i;
		}
	}

	t_suscriptor_team* teamViejo = list_replace(suscriptoresTeam, posicion, team);
	free(teamViejo);
}

void reemplazar_suscriptor_gamecard(t_suscriptor_gamecard* gamecard){

	int sizeLista = list_size(suscriptoresGameCard);
	int posicion = -1;
	for(int i = 0; i < sizeLista; i++){
		t_suscriptor_gamecard* suscriptor = list_get(suscriptoresGameCard, i);
		if(suscriptor->id == gamecard->id){
			posicion = i;
		}
	}

	t_suscriptor_gamecard* gamecardViejo = list_replace(suscriptoresGameCard, posicion, gamecard);
	free(gamecardViejo);
}

int check_si_existe_gameboy(int ID){

	int encontrado = 0;
	int sizeLista = list_size(suscriptoresGameBoy);

	for(int i = 0; i < sizeLista; i++){
		t_suscriptor_gameboy* suscriptor = list_get(suscriptoresGameBoy, i);
		if(suscriptor->id == ID){ encontrado = 1; }
	}

	return encontrado;
}

int check_si_existe_team(int ID){

	int encontrado = 0;
	int sizeLista = list_size(suscriptoresTeam);

	for(int i = 0; i < sizeLista; i++){
		t_suscriptor_team* suscriptor = list_get(suscriptoresTeam, i);
		if(suscriptor->id == ID){ encontrado = 1; }
	}

	return encontrado;
}

int check_si_existe_gamecard(int ID){

	int encontrado = 0;
	int sizeLista = list_size(suscriptoresGameCard);

	for(int i = 0; i < sizeLista; i++){
		t_suscriptor_gamecard* suscriptor = list_get(suscriptoresGameCard, i);
		if(suscriptor->id == ID){ encontrado = 1; }
	}

	return encontrado;
}

t_suscriptor_gameboy* buscar_suscriptor_gameboy(int ID){

	t_suscriptor_gameboy* gameboy;
	int sizeLista = list_size(suscriptoresGameBoy);

	for(int i = 0; i < sizeLista; i++){
		t_suscriptor_gameboy* suscriptor = list_get(suscriptoresGameBoy, i);
		if(suscriptor->id == ID){
			gameboy = suscriptor;
		}
	}

	return gameboy;
}

t_suscriptor_team* buscar_suscriptor_team(int ID){

	t_suscriptor_team* team;
	int sizeLista = list_size(suscriptoresTeam);

	for(int i = 0; i < sizeLista; i++){
		t_suscriptor_team* suscriptor = list_get(suscriptoresTeam, i);
		if(suscriptor->id == ID){
			team = suscriptor;
		}
	}

	return team;
}

t_suscriptor_gamecard* buscar_suscriptor_gamecard(int ID){

		t_suscriptor_gamecard* gamecard;
		int sizeLista = list_size(suscriptoresGameCard);

		for(int i = 0; i < sizeLista; i++){
			t_suscriptor_gamecard* suscriptor = list_get(suscriptoresGameCard, i);
			if(suscriptor->id == ID){
				gamecard = suscriptor;
			}
		}

		return gamecard;
}

int getSocketParaEnvio(int idSuscriptor, op_code tipoDeMensaje){

	pthread_mutex_lock(&mutexRepoTeam);
	int existeTeam = check_si_existe_team(idSuscriptor);
	pthread_mutex_unlock(&mutexRepoTeam);

	if(existeTeam == 1){
		pthread_mutex_lock(&mutexRepoTeam);
		t_suscriptor_team* team = buscar_suscriptor_team(idSuscriptor);
		pthread_mutex_unlock(&mutexRepoTeam);
		switch(tipoDeMensaje){
			case LOCALIZED_POKEMON:
				return team->socketLocalized;
				break;
			case CAUGHT_POKEMON:
				return team->socketCaught;
				break;
			case APPEARED_POKEMON:
				return team->socketAppeared;
				break;
			default:
				return -1;
				break;
		}
	}

	pthread_mutex_lock(&mutexRepoGameCard);
	int existeGameCard = check_si_existe_gamecard(idSuscriptor);
	pthread_mutex_unlock(&mutexRepoGameCard);

	if(existeGameCard == 1){
		pthread_mutex_lock(&mutexRepoGameCard);
		t_suscriptor_gamecard* gamecard = buscar_suscriptor_gamecard(idSuscriptor);
		pthread_mutex_unlock(&mutexRepoGameCard);
		switch(tipoDeMensaje){
			case NEW_POKEMON:
				return gamecard->socketNew;
				break;
			case GET_POKEMON:
				return gamecard->socketGet;
				break;
			case CATCH_POKEMON:
				return gamecard->socketCatch;
				break;
			default:
				return -1;
				break;
		}
	}

	pthread_mutex_lock(&mutexRepoGameBoy);
	int existeGameBoy = check_si_existe_gameboy(idSuscriptor);
	pthread_mutex_unlock(&mutexRepoGameBoy);

	if(check_si_existe_gameboy(idSuscriptor) == 1){
		pthread_mutex_lock(&mutexRepoGameBoy);
		t_suscriptor_gameboy* gameboy = buscar_suscriptor_gameboy(idSuscriptor);
		pthread_mutex_unlock(&mutexRepoGameBoy);

		if(tipoDeMensaje == gameboy->colaEscuchando){
			return gameboy->socketDondeEscucha;
		}
		else{
			return -1;
		}
	}
	else{
		return -1;
	}
}
