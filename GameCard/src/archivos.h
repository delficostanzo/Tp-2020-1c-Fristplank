/*
 * archivos.h
 *
 *  Created on: 13 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_ARCHIVOS_H_
#define SRC_ARCHIVOS_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include"GameCard.h"

int checkingOpenFile(char* filePath);
void cambiarAAbierto(char* filePath);
void cambiarACerrado(char* filePath);
void crearArchivo(char* filePath);

/*Pide archivo para uso
 * si no existe, lo crea
 */
void pedirArchivoParaUso(char* filePath);

int checkArchivoExiste(char* filePath);

#endif /* SRC_ARCHIVOS_H_ */
