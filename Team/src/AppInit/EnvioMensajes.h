
#ifndef SRC_APPINIT_ENVIOMENSAJES_H_
#define SRC_APPINIT_ENVIOMENSAJES_H_

#include "CreacionEstructurasMsj.h"
#include "../Model/EntrenadoresPorEstado.h"
#include<commons/string.h>
#include"Conexion.h"
#include"Planificador.h"

Entrenador* entrenadorQueEspera;

typedef struct{
	char* nombrePoke;
	t_posicion posicion;
}argumentosAAgregar;



void enviarGetDesde(int socketGet);
void enviarCatchDesde(Entrenador* entrenadorEsperando);
void recibirIdGet(int socketGet);
t_paquete* recibirLocalizedYGuardalos(int socketLocalized);
t_paquete* recibirAppearedYGuardarlos(int socketAppeared);
//void recibirIdCatch(int socketIdCatch);
t_paquete* recibirCaught(int socketCaught);
void ejecutarRespuestaCaught(int idCatchQueResponde, t_paquete* paqueteCaught);
int seNecesita(char* pokemon);
void agregarPokemonSiLoNecesita(argumentosAAgregar* argus);
#endif /* SRC_APPINIT_ENVIOMENSAJES_H_ */
