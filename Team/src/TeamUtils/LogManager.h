
#ifndef SRC_TEAMUTILS_LOGMANAGER_H_
#define SRC_TEAMUTILS_LOGMANAGER_H_

//esta clase wrappea al log que te dan desde utils. Asi es accessible desde cualquier clase que la importe en su include
#include "utils.h"


//crea un nuevo logger apuntando al archivo con el nombre dado
t_log* newLoggerFrom(String logFileName);

//destruye una instancia de logger
void destroyLogger(t_log* logger);

//log rapido al archivo log default: "team.log"
void quickLog(String message);

#endif /* SRC_TEAMUTILS_LOGMANAGER_H_ */
