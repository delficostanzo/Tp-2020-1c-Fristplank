
#ifndef TEAM_H_
#define TEAM_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include <utils.h>

//agrego las variables que se agregan en el archivo de conf ademas del ip y puerto
// los primeros 3 no estoy segura se esta bien asi. Se supone que son una lista de listas. Son bienvenidos a revisar si eso esta bien :)
//char** POSICONES_ENTRENADORES;
//char** POKEMON_ENTRENADORES;
//char** OBJETIVOS_ENTRENADORES;
//int TIEMPO_RECONEXION;
//int RETARDO_CICLO_CPU;
//char* ALGORITMO_PLANIFICACION;
//int QUANTUM;
//char* IP_BROKER;
//int ESTIMACION_INICIAL;
//char* PUERTO_BROKER;
//char* LOG_FILE;

t_log* iniciar_logger(void);
t_config* leer_config(void);
void terminar_programa(int, t_log*, t_config*);

#endif /* TEAM_H_ */
