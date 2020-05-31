
#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>

#include<pthread.h>

#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/string.h>

#include"conexion.h"

typedef enum
{
	NEW_POKEMON = 1,
	APPEARED_POKEMON = 2,
	CATCH_POKEMON = 3,
	CAUGHT_POKEMON = 4,
	GET_POKEMON = 5,
	LOCALIZED_POKEMON = 6,
	RESPUESTA_ID = 7,
	ACK = 8,
	GAMEBOYSUSCRIBE = 9
}op_code;

typedef struct
{
	int posicionX;
	int posicionY;
} t_posicion;

typedef struct
{
	int lengthOfPokemon;
	char* pokemon;
	t_posicion* posicion;
	int cantidad;
} t_new_pokemon;

typedef struct
{
	int lengthOfPokemon;
	char* pokemon;
	t_posicion* posicion;
} t_appeared_pokemon;

typedef struct
{
	int lengthOfPokemon;
	char* pokemon;
	t_posicion* posicion;
} t_catch_pokemon;

typedef struct
{
	bool ok;
} t_caught_pokemon;

typedef struct
{
	int lengthOfPokemon;
	char* pokemon;
} t_get_pokemon;


typedef struct
{
	int lengthOfPokemon;
	char* pokemon;
	t_list* listaPosiciones;
} t_localized_pokemon;

typedef struct
{
	int idCorrelativo;
} t_respuesta_id;

typedef struct
{
	op_code codigoCola;
} t_gameboy_suscribe;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	int ID;
	int ID_CORRELATIVO;
	t_buffer* buffer;
} t_paquete;

int crear_conexion(char* ip, char* puerto);

void enviar_new_pokemon(t_new_pokemon* new_pokemon, int socket_cliente, int ID, int IDCORRELATIVO);
void enviar_appeared_pokemon(t_appeared_pokemon* appeared_pokemon, int socket_cliente, int ID, int IDCORRELATIVO);
void enviar_catch_pokemon(t_catch_pokemon* catch_pokemon, int socket_cliente, int ID, int IDCORRELATIVO);
void enviar_caught_pokemon(t_caught_pokemon* caught_pokemon, int socket_cliente, int ID, int IDCORRELATIVO);
void enviar_get_pokemon(t_get_pokemon* get_pokemon, int socket_cliente, int ID, int IDCORRELATIVO);
void enviar_localized_pokemon(t_localized_pokemon* localized_pokemon, int socket_cliente, int ID, int IDCORRELATIVO);
void enviar_ACK(int socket_cliente, int ID, int IDCORRELATIVO);

void enviar_gameboy_suscribe(t_gameboy_suscribe* gameboy_suscribe, int socket_cliente, int ID, int IDCORRELATIVO);

t_paquete* recibir_mensaje(int socket_cliente);

void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);
//t_log* iniciar_log(void);
t_log* iniciar_log(char* nombreModulo);

#endif /* UTILS_H_ */
