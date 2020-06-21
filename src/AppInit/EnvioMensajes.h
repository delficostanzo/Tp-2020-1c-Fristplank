
#ifndef SRC_APPINIT_ENVIOMENSAJES_H_
#define SRC_APPINIT_ENVIOMENSAJES_H_

#include "CreacionEstructurasMsj.h"
#include "../Model/Mapa.h"
#include "../Model/Trainer.h"

void enviarGetDesde(int socketGet);
void enviarCatchDesde(Entrenador* entrenadorEsperando);
void recibirIdGet(int socketGet);
t_paquete* recibirLocalizedYGuardalos(int socketLocalized);
t_paquete* recibirAppearedYGuardarlos(int socketAppeared);
t_paquete* recibirCaught(int socketCaught);
void ejecutarRespuestaCaught(int idCatchQueResponde, t_paquete* paqueteCaught);
#endif /* SRC_APPINIT_ENVIOMENSAJES_H_ */
