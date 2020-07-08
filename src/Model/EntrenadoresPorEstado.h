/*
 * EntrenadoresPorEstado.h
 *
 *  Created on: 15 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MODEL_ENTRENADORESPORESTADO_H_
#define SRC_MODEL_ENTRENADORESPORESTADO_H_

#include "utils.h"
#include "Mapa.h"


t_list* entrenadoresBloqueadosPorDeadlock(void);
t_list* entrenadoresDormidos(void);
t_list* entrenadoresEsperandoRespuesta(void);
Entrenador* entrenadorExec(void);

#endif /* SRC_MODEL_ENTRENADORESPORESTADO_H_ */
