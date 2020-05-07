
//esta clase wrappea al log que te dan desde utils. Asi es accessible desde cualquier clase que la importe en su include
#include "LogManager.h"

t_log* newLoggerFrom(String logFileName) {
	t_log * log = malloc(sizeof(t_log));
	log = log_create(logFileName, "TEAM", 1, 0);

	if(log == NULL){
		printf("No pude crear el logger \n");
		exit(1);
	}

	return log;
}

void destroyLogger(t_log* logger) {
	log_destroy(logger);
}

void quickLog(String message) {
	t_log* logger = newLoggerFrom("team.log");

	log_info(logger, message);

	destroyLogger(logger);
}
