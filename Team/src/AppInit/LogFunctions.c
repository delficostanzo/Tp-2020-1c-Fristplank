/*
 * LogFunctions.c
 *
 *  Created on:
 *      Author: fritsplank
 */

#include "LogFunctions.h"

void quickLog(char* mensaje) {
	t_log* logger = iniciar_logger();
	log_info(logger, mensaje);
	log_destroy(logger);
}

t_log* iniciar_logger(void) {
	t_log* log = log_create("team.log", "TEAM", 0, 0);
	if (log == NULL) {
		printf("No pude crear el logger \n");
		exit(1);
	}
	return log;
}

t_log* iniciarLogOficial(void) {
	t_log * log = malloc(sizeof(t_log));
	log = log_create(LogOficial, "LOG", 1, 0);
	if (log == NULL) {
		printf("No pude crear el log oficial \n");
		exit(1);
	}
	return log;
}

void destruirLog(t_log* logger) {
	if (logger != NULL) {
		log_destroy(logger);
	}
}

