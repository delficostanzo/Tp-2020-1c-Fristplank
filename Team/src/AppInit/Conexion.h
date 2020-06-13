
#ifndef SRC_APPINIT_CONEXION_H_
#define SRC_APPINIT_CONEXION_H_

#include "ConfigFunctions.h"
#include "../Model/Mapa.h"

//typedef struct{
//	int socketEscucha;
//	int socketACK;
//}t_par_de_sockets;

void* escucharGameBoy(void);
void* generarSocketsConBroker(void);
void* escucharColaAppearedPokemonGameBoy(void);
void* escucharColaAppearedPokemon(void);
void* escucharColaCaughtPokemon(void);
void* escucharColaLocalizedPokemon(void);

#endif /* SRC_APPINIT_CONEXION_H_ */
