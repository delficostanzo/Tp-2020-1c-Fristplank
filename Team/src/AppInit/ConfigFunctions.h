/*
 * ConfigFunctions.h
 *
 *  Created on:
 *      Author: fritsplank
 */

#ifndef SRC_APPINIT_CONFIGFUNCTIONS_H_
#define SRC_APPINIT_CONFIGFUNCTIONS_H_

#include "utils.h"
#include<commons/string.h>
#include<commons/config.h>
#include"../Model/Pokemon.h"
#include"../Model/Trainer.h"
#include"LogFunctions.h"

t_config* leerConfigDesde(char* nombreDeArchivo);
t_list* crearListaConStringsDeConfig(char** strings);
t_posicion* posicionDesde(char* stringDePosicion);
t_list* pokemonesDesdeString(char* nombrePokemons);


#endif /* SRC_APPINIT_CONFIGFUNCTIONS_H_ */
