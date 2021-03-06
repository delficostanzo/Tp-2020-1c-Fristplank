#ifndef SRC_APPINIT_ENTRENADORESINIT_H_
#define SRC_APPINIT_ENTRENADORESINIT_H_

#include "utils.h"
#include "../Model/Trainer.h"
#include "ConfigFunctions.h"

t_list* getEntrenadoresDesde(char* nombreDeArchivo);
t_list* getObjetivosGlobalesDesde(t_list* pokemonesObjetivos, t_list* pokemonesAtrapados);
t_list* getObjetivosTotalesDesde(t_list* entrenadores);
t_list* getTotalAtrapadosDesde(t_list* entrenadores);

#endif /* SRC_APPINIT_ENTRENADORESINIT_H_ */
