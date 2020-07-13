/*
 * repositorioSuscriptores.h
 *
 *  Created on: 9 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_REPOSITORIOSUSCRIPTORES_H_
#define SRC_REPOSITORIOSUSCRIPTORES_H_

#include "variables.h"

typedef struct{
	int id;

	// FLUJO GET - LOCALIZED
	int socketGet; // RECIBIMOS GET
	int socketIdGet; // ENVIAMOS IDGET
	int socketLocalized; // ENVIAMOS LOCALIZED
	int socketACKLocalized; // RECIBIMOS ACK DE LOCALIZED

	// FLUJO CATCH - CAUGHT
	int socketCatch; // RECIBIMOS CATCH
	int socketIdCatch; // ENVIAMOS IDCATCH
	int socketCaught; // ENVIAMOS CAUGHT
	int socketACKCaught; // RECIBIMOS ACK DE CAUGHT

	// FLUJO APPEARED
	int socketAppeared; // ENVIAMOS APPEARED
	int socketACKAppeared; // RECIBIMOS ACK DE APPEARED
}t_suscriptor_team;

typedef struct{
	int id;

	// FLUJO NEW - APPEARED
	int socketNew; // ENVIAMOS NEW
	int socketACKNew; // RECIBIMOS ACK DE NEW
	int socketAppeared; // RECIBIMOS APPEARED

	// FLUJO GET - LOCALIZED
	int socketGet; // ENVIAMOS GET
	int socketACKGet; // RECIBIMOS ACK DE GET
	int socketLocalized; // RECIBIMOS LOCALIZED

	// FLUJO CATCH - CAUGHT
	int socketCatch; // ENVIAMOS CATCH
	int socketACKCatch; // RECIBIMOS ACK DE CATCH
	int socketCaught; // RECIBIMOS CAUGHT
}t_suscriptor_gamecard;

typedef struct{
	int id;
	int socketDondeEscucha;
	op_code colaEscuchando;
}t_suscriptor_gameboy;

t_list* suscriptoresGameBoy;
t_list* suscriptoresTeam;
t_list* suscriptoresGameCard;
pthread_mutex_t mutexRepoGameBoy;
pthread_mutex_t mutexRepoTeam;
pthread_mutex_t mutexRepoGameCard;

void init_repositorio_suscriptores(void);
void finish_repositorio_suscriptores(void);

void agregar_suscriptor_gameboy(t_suscriptor_gameboy* gameboy);
void agregar_suscriptor_team(t_suscriptor_team* team);
void agregar_suscriptor_gamecard(t_suscriptor_gamecard* gamecard);
void reemplazar_suscriptor_team(t_suscriptor_team* team);
void reemplazar_suscriptor_gamecard(t_suscriptor_gamecard* gamecard);

int check_si_existe_gameboy(int ID);
int check_si_existe_team(int ID);
int check_si_existe_gamecard(int ID);

t_suscriptor_gameboy* buscar_suscriptor_gameboy(int ID);
t_suscriptor_team* buscar_suscriptor_team(int ID);
t_suscriptor_gamecard* buscar_suscriptor_gamecard(int ID);

int getSocketParaEnvio(int idSuscriptor, op_code tipoDeMensaje);

#endif /* SRC_REPOSITORIOSUSCRIPTORES_H_ */
