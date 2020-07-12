
#ifndef SRC_APPINIT_CONEXION_H_
#define SRC_APPINIT_CONEXION_H_

#include "ConfigFunctions.h"
#include "../Model/Mapa.h"
#include "EnvioMensajes.h"

//typedef struct{
//	int socketEscucha;
//	int socketACK;
//}t_par_de_sockets;

pthread_t escucharLocalizedPokemon;

void* escucharGameBoy(void);
int generarSocketsConBroker();
void crearHilosDeEscucha();
void* escucharColaAppearedPokemonGameBoy(void);
void* escucharColaAppearedPokemon(void);
void* escucharColaIdsCatchPokemon();
void* escucharColaCaughtPokemon(void);
void* escucharColaLocalizedPokemon(void);

#endif /* SRC_APPINIT_CONEXION_H_ */
