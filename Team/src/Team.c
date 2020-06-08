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
	//obtiene la lista de entrenadores desde el config
	quickLog("Se arrancan a cargar los entrenadores");
	t_list* entrenadores = getEntrenadoresDesde("src/team.config");
	quickLog("Ya fueron todos los entrenadores cargados con sus posiciones, objetivos y atrapados");

	//lista de los pokemones libres que faltan atrapar y que se necesitan
	//arranca vacia hasta que recibamos los localized y appeared de los pokes
	t_list* pokemonesLibres = list_create();

	//probando las funciones del planificador
	PokemonEnElMapa* pokemonDePrueba1 = newPokemon();
	t_posicion posicionDePrueba1;
	posicionDePrueba1.posicionX = 1;
	posicionDePrueba1.posicionY = 1;
	setPosicionTo(pokemonDePrueba1, posicionDePrueba1);
	setNombreTo(pokemonDePrueba1, "Delficapa");
	list_add(pokemonesLibres, pokemonDePrueba1);
	pasarDeNewAReady(entrenadores, pokemonesLibres);
	Entrenador* entrenador1 = list_get(entrenadores, 0);
	log_info(logger, "El estado del primer entrenador tendria que pasar de new a ready y se muestra : %d", entrenador1->estado);
	//


	//	int TIEMPO_RECONEXION = config_get_int_value(config, "TIEMPO_RECONEXION");
	//	int RETARDO_CICLO_CPU = config_get_int_value(config, "RETARDO_CICLO_CPU");
	//	char* ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	//	int QUANTUM = config_get_int_value(config, "QUANTUM");
	char* IP_BROKER = config_get_string_value(config, "IP_BROKER");
	//	int ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");
	int PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");
	//	char* LOG_FILE =  config_get_string_value(config,"LOG_FILE");

	int puertoTeam = config_get_int_value(config, "PUERTO_TEAM");
	quickLog("LLegamos hasta aca");

	t_list* objetivosTotales = getObjetivosTotalesDesde(entrenadores);
	t_list* objetivosAtrapados = getTotalAtrapadosDesde(entrenadores);
	t_list* objetivosGlobales = getObjetivosGlobalesDesde2(objetivosTotales, objetivosAtrapados);
	log_info(logger, "La cantidad de pokemones objetivos es: %d",list_size(objetivosTotales));
	log_info(logger, "La cantidad de pokemones atrapados es: %d",list_size(objetivosAtrapados));
	log_info(logger, "La cantidad de pokemones globales que faltan por atrapar es: %d",list_size(objetivosGlobales));

//	conexionGameBoy = crearSocket();
//
//	//TODO: Si muere la conexion del broker, escucho al GameBoy. Y intentar reconexion del Broker varias veces
//	if (escuchaEn(conexionGameBoy, puertoTeam)) {
//		quickLog("Escuchando conexiones del GameBoy");
//	}
//
//	//se queda bloqueado esperando que el gameboy se conecte
//	int socketGameBoy = aceptarConexion(conexionGameBoy);
//
//	id_proceso idProcesoConectado;
//	idProcesoConectado = iniciarHandshake(socketGameBoy, TEAM);
//	log_info(logger, "El id del proceso con el que me conecte es: %d",
//			idProcesoConectado);
//
	// CONEXION BROKER
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
	int socketGet = crearSocket();
	int socketCatch = crearSocket();

	if (conectarA(suscripcionAppeared, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de appeared_pokemon");
	}
	if (conectarA(suscripcionCaught, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de caught_pokemon");
	}
	if (conectarA(suscripcionLocalized, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de localized_pokemon");
	}
	if (conectarA(socketGet, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Ya se conecto a la cola de get para poder enviarle mensajes");
	}
	if (conectarA(socketCatch, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Ya se conecto a la cola de catch para poder enviarle mensajes");
	}

	//ENVIA MSJ GET CON LA LISTA DE LOS OBJETIVOS GLOBALES
	//DENTRO DE SELECTS
	enviarGetDesde(objetivosGlobales, socketGet);
	//verificar que el mensaje vuelva a enviarse a traves de ese socket
	recibirIdGet(socketGet);

	//recibe los nombres de pokemones encontrados libres con sus posiciones
	//y si se necesitan (estan en los objetivos globales) se agregan a la lista de pokemones libres
	recibirLocalizedYGuardalos(suscripcionLocalized, objetivosGlobales, pokemonesLibres);
	recibirAppearedYGuardarlos(suscripcionAppeared, objetivosGlobales, pokemonesLibres);

	//pasar de array a t_list

	crearHilosDeEntrenadores(entrenadores);
	quickLog("Se crea un hilo por cada entrenador");

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

	t_paquete* paquetePrueba = recibir_mensaje(suscripcionCaught);

	//DESEREALIZAMOS
	t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));
	if(recv(suscripcionCaught, &(caught_pokemon->ok), sizeof(uint32_t), 0) !=-1) {
		quickLog("Se pudo recibir el puto mensaje");
	}
	log_info(logger, "El resultado del mensaje es: %d", caught_pokemon->ok);

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
