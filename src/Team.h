/*
 * Team.h
 *
 *  Created on: 25 may. 2020
 *      Author: fritsplank
 */

#ifndef TEAM_H_
#define TEAM_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<utils.h>
#include "AppInit/EntrenadoresInit.h"
#include "AppInit/ConfigFunctions.h"
#include "AppInit/Planificador.h"
#include "Model/Trainer.h"
#include "AppInit/EnvioMensajes.h"
#include "AppInit/Conexion.h"

char* IP_BROKER;
int PUERTO_BROKER;
int puertoTeam;

int conexionBroker;
int suscripcionAppeared;
int suscripcionCaught;
int suscripcionLocalized;
int socketGet;
int socketCatch;

int socketACKAppeared;
int socketACKCaught;
int socketACKLocalized;



#endif /* TEAM_H_ */
