
#ifndef TEAM_H_
#define TEAM_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<utils.h>
#include"Model/Trainer.h"
#include "Parser/ConfigParser.h"

//agrego las variables que se agregan en el archivo de conf ademas del ip y puerto
// los primeros 3 no estoy segura se esta bien asi. Se supone que son una lista de listas. Son bienvenidos a revisar si eso esta bien :)


t_log* iniciar_logger(void);
t_config* leerConfigDesde(char* nombreDeArchivo);
void terminar_programa(int, t_log*, t_config*);

#endif /* TEAM_H_ */
