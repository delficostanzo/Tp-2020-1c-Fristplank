/*
 * memoriaCache.h
 *
 *  Created on: 20 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIACACHE_H_
#define SRC_MEMORIACACHE_H_
#include "Broker.h"
#include "colas.h"
#include "particiones.h"

#define CENTINELA '\0';

int cantidadParticionesBorradas;

void cachearMensaje(t_metadata * meta, void * mensaje);
void cachearNewPokemon(t_metadata * meta, void * mensaje, void ** mensajeACachear);
void cachearLocalizedPokemon(t_metadata * meta, void * mensaje, void ** mensajeACachear);
void cachearGetPokemon(t_metadata * meta, void * mensaje, void ** mensajeACachear);
void cachearAppearedPokemon(t_metadata * meta, void * mensaje, void ** mensajeACachear);
void cachearCatchPokemon(t_metadata * meta, void * mensaje, void ** mensajeACachear);
void cachearCaughtPokemon(t_metadata * meta, void * mensaje, void ** mensajeACachear);

void * descachearMensaje(void * mensajeEnMemoria,t_metadata * meta);
void * descachearNewPokemon(void* mensajeEnMemoria);
void * descachearGetPokemon(void* mensajeEnMemoria);
void * descachearLocalizedPokemon(void* mensajeEnMemoria);
void * descachearAppearedPokemon(void* mensajeEnMemoria);
void * descachearCatchPokemon(void* mensajeEnMemoria);
void * descachearCaughtPokemon(void* mensajeEnMemoria);

void escribirMemoria(void * mensaje, t_metadata * meta);
void * leerMemoria(t_metadata * meta);
void modificarUltimaReferencia(t_metadata * meta, char tipoReferencia);

#endif /* SRC_MEMORIACACHE_H_ */
