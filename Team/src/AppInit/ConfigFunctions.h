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

t_config* leerConfigDesde(String nombreDeArchivo);
t_list* crearListaConStringsDeConfig(String* strings);
t_posicion* posicionDesde(String stringDePosicion);
t_list* pokemonesDesdeString(String nombrePokemons);


#endif /* SRC_APPINIT_CONFIGFUNCTIONS_H_ */
