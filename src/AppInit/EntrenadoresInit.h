#ifndef SRC_APPINIT_ENTRENADORESINIT_H_
#define SRC_APPINIT_ENTRENADORESINIT_H_

#include "utils.h"
#include "../Model/Trainer.h"
#include "ConfigFunctions.h"

t_list* getEntrenadoresDesde(String nombreDeArchivo);
t_list* getObjetivosGlobalesDesde(t_list* entrenadores);
t_list* getObjetivosTotalesDesde(t_list* entrenadores);
t_list* getTotalAtrapadosDesde(t_list* entrenadores);

#endif /* SRC_APPINIT_ENTRENADORESINIT_H_ */
