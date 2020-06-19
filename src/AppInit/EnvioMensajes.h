
#ifndef SRC_APPINIT_ENVIOMENSAJES_H_
#define SRC_APPINIT_ENVIOMENSAJES_H_

#include "CreacionEstructurasMsj.h"
#include "../Model/Pokemon.h"
#include "../Model/Mapa.h"


void enviarGetDesde(t_list* objetivosGlobales, int socketGet);
void recibirIdGet(int socketGet);
t_paquete* recibirLocalizedYGuardalos(int socketLocalized, t_list* pokemonesGlobales, t_list* pokemonesLibres);
t_paquete* recibirAppearedYGuardarlos(int socketAppeared, t_list* pokemonesGlobales, t_list* pokemonesLibres);
t_paquete* recibirCaught(int socketCaught);
void ejecutarRespuestaCaught(int idCatchQueResponde, t_paquete* paqueteCaught);
#endif /* SRC_APPINIT_ENVIOMENSAJES_H_ */
