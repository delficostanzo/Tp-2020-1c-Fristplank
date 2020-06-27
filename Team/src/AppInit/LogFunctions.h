/*
 * LogFunctions.h
 *
 *  Created on:
 *      Author: fritsplank
 */

#ifndef SRC_APPINIT_LOGFUNCTIONS_H_
#define SRC_APPINIT_LOGFUNCTIONS_H_

#include "utils.h"

void quickLog(char* mensaje);
t_log* iniciar_logger(void);
void destruirLog(t_log* logger);

#endif /* SRC_APPINIT_LOGFUNCTIONS_H_ */
