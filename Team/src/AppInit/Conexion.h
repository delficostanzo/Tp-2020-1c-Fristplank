
#ifndef SRC_APPINIT_CONEXION_H_
#define SRC_APPINIT_CONEXION_H_

#include "ConfigFunctions.h"
#include "../Model/Mapa.h"
#include "EnvioMensajes.h"
#include "../Model/CierreTeam.h"



void* escucharGameBoy(void);
int generarSocketsConBroker();
void crearHilosDeEscucha();
void* escucharColaAppearedPokemonGameBoy(void);
void* escucharColaAppearedPokemon(void);
void* escucharColaIdsCatchPokemon();
void* escucharColaCaughtPokemon(void);
void* escucharColaLocalizedPokemon(void);
void reconectarBroker();

#endif /* SRC_APPINIT_CONEXION_H_ */
