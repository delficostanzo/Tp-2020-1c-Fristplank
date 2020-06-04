#include <stdio.h>
#include <stdlib.h>
#include "Team.h"
#include "./Model/Pokemon.h"
#include "./AppInit/EntrenadoresInit.h"
#include "./AppInit/HilosEntrenadores.h"

int main(int argc, char *argv[]) {
	int conexionBroker; //cliente del broker
	int conexionGameBoy; //servidor del gameboy

	//t_log* logger = iniciar_log("team");
	t_log* logger = iniciar_logger();

	t_config* config = leerConfigDesde("src/team.config");
	t_list* entrenadores = getEntrenadoresDesde("src/team.config");

	quickLog("LLegamos hasta aca");
	//	char** POSICONES_ENTRENADORES = config_get_array_value(config, "POSICONES_ENTRENADORES");
	//	char** POKEMON_ENTRENADORES = config_get_array_value(config, "POKEMON_ENTRENADORES");
	//	char** OBJETIVOS_ENTRENADORES = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
	//	int TIEMPO_RECONEXION = config_get_int_value(config, "TIEMPO_RECONEXION");
	//	int RETARDO_CICLO_CPU = config_get_int_value(config, "RETARDO_CICLO_CPU");
	//	char* ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	//	int QUANTUM = config_get_int_value(config, "QUANTUM");
	char* IP_BROKER = config_get_string_value(config, "IP_BROKER");
	//	int ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");
	int PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");
	//	char* LOG_FILE =  config_get_string_value(config,"LOG_FILE");

	int puertoTeam = config_get_int_value(config, "PUERTO_TEAM");

	//conexionBroker = crear_conexion(IP_BROKER, PUERTO_BROKER);
	conexionGameBoy = crearSocket();

	if (escuchaEn(conexionGameBoy, puertoTeam)) {
		quickLog("Escuchando conexiones del GameBoy");
	}

	//se queda bloqueado esperando que el gameboy se conecte
	int socketGameBoy = aceptarConexion(conexionGameBoy);

	id_proceso idProcesoConectado;
	idProcesoConectado = iniciarHandshake(socketGameBoy, TEAM);
	log_info(logger, "El id del proceso con el que me conecte es: %d",
			idProcesoConectado);

	conexionBroker = crearSocket();

	if (conectarA(conexionBroker, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Conectando al Broker");
	}

	id_proceso idProcesoBroker;
	idProcesoBroker = responderHandshake(conexionBroker, TEAM);
	log_info(logger, "El id del proceso con el que me conecte es: %d", idProcesoBroker);

	int suscripcionAppeared = crearSocket();
	int suscripcionCaught = crearSocket();
	int suscripcionLocalized = crearSocket();

	if (conectarA(suscripcionAppeared, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de appeared_pokemon");
	}
	if (conectarA(suscripcionCaught, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de caught_pokemon");
	}
	if (conectarA(suscripcionLocalized, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de localized_pokemon");
	}


	pthread_t* hilosDeEntrenadores[list_size(entrenadores)];

	//pasar de array a t_list

	crearHilosDeEntrenadores(entrenadores);
	quickLog("Se crea un hilo por cada entrenador");

	while (true) {
		quickLog("cualquier cosa");
		sleep(3);
	}

	//casos de prueba para ver si funciona bien el mapa
	PokemonEnElMapa* pokemonDePrueba = newPokemon();
	t_posicion posicionDePrueba;
	posicionDePrueba.posicionX = 2;
	posicionDePrueba.posicionY = 2;
	setPosicionTo(pokemonDePrueba, posicionDePrueba);
	Entrenador* entrenadorMasCercano = newEntrenador();
	entrenadorMasCercano = entrenadorMasCercanoA(pokemonDePrueba, entrenadores);

	//CONEXION//
	log_info(logger, "La posicion x: %d , la posicion y: %d del entrenador mas cerca",
			entrenadorMasCercano->posicion->posicionX,
			entrenadorMasCercano->posicion->posicionY);

	//t_list* objetivosGlobales = getObjetivosGlobalesDesde("src/team.config");
	t_list* objetivosGlobales = getObjetivosGlobalesDesde(entrenadores);
	quickLog("primer objetivo de pokemones");
	quickLog(((PokemonEnElMapa*) list_get(objetivosGlobales, 0))->nombre);
	quickLog("segundo objetivo de pokemones");
	quickLog(((PokemonEnElMapa*) list_get(objetivosGlobales, 1))->nombre);

	// recibir mensaje
	//t_paquete* mensaje = recibir_mensaje(conexion); //lo recibimos y la funcion recibir mensaje lo mete en un paquete

	//loguear mensaje recibido

	//log_info(logger, "El mensaje recibido es: %s\n", mensaje);

	//terminar_programa(conexion, logger, config);

	list_destroy_and_destroy_elements(entrenadores, free);

	return 0;
}


void terminar_programa(int conexion, t_log* logger, t_config* config) {
	if (logger != NULL) {
		log_destroy(logger);
	}

	if (config != NULL) {
		config_destroy(config); //destruye la esctructura de config en memoria, no elimina el archivo de config
	}

	liberar_conexion(conexion);

}
