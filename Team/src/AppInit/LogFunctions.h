/*
 * LogFunctions.h
 *
 *  Created on:
 *      Author: fritsplank
 */

#ifndef SRC_APPINIT_LOGFUNCTIONS_H_
#define SRC_APPINIT_LOGFUNCTIONS_H_

#include "utils.h"
#include "../Model/Mapa.h"

void quickLog(char* mensaje);
t_log* iniciar_logger(void);
t_log* iniciarLogOficial(void);
void destruirLog(t_log* logger);

#endif /* SRC_APPINIT_LOGFUNCTIONS_H_ */
