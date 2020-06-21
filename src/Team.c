
#include "Team.h"


int main(int argc, char *argv[]) {

	t_log* logger = iniciar_logger();

	t_config* config = leerConfigDesde("src/team.config");
	//obtiene la lista de entrenadores desde el config
	entrenadores = getEntrenadoresDesde("src/team.config");
	quickLog("Ya fueron todos los entrenadores cargados con sus posiciones, objetivos y atrapados");

//	Entrenador* entrenadorPrueba = crearEntrenadorEnExec();
//	list_add(entrenadores, entrenadorPrueba);
//	log_info(logger, "Se cargo un nuevo entrenador en exec que esta en exec por atrapar al pokemon: %s", entrenadorPrueba->movimientoEnExec->pokemonNecesitado->nombre);
//	log_info(logger, "El entrenador se encontraba en la posicion (%d, %d)", entrenadorPrueba->posicion->posicionX, entrenadorPrueba->posicion->posicionY);

	Entrenador* entrenador1 = list_get(entrenadores, 0);
	pasarAExec(entrenador1);
	log_info(logger, "El entrenador paso a estar en exec para atrapar a %s", entrenador1->movimientoEnExec->pokemonNecesitado->nombre);

	pokemonesLibres = list_create();


	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");

	puertoTeam = config_get_int_value(config, "PUERTO_TEAM");

	objetivosTotales = getObjetivosTotalesDesde(entrenadores);
	objetivosAtrapados = getTotalAtrapadosDesde(entrenadores);
	objetivosGlobales = getObjetivosGlobalesDesde(objetivosTotales, objetivosAtrapados);
	log_info(logger, "La cantidad de pokemones objetivos es: %d",list_size(objetivosTotales));
	log_info(logger, "La cantidad de pokemones atrapados es: %d",list_size(objetivosAtrapados));
	log_info(logger, "La cantidad de pokemones globales que faltan por atrapar es: %d",list_size(objetivosGlobales));

	iniciarVariables();

	//Lanzar hilo para escuchar a GameBoy
//	pthread_t hiloEscuchaGameBoy;
//	pthread_create(&hiloEscuchaGameBoy, NULL, (void*) escucharGameBoy, NULL);

	//Lanzar hilo para concetarme a Broker
	pthread_t hiloConexionBroker;
	pthread_create(&hiloConexionBroker, NULL, (void*) generarSocketsConBroker, NULL);
//	//los recursos son liberados cuando termina la funcion sin esperar un join
//	pthread_detach(hiloConexionBroker);


//	//verificar que el id como respuesta vuelva a enviarse a traves de ese socket
//	//recibe los nombres de pokemones encontrados libres con sus posiciones
//	//y si se necesitan (estan en los objetivos globales) se agregan a la lista de pokemones libres
	//recibirLocalizedYGuardalos(suscripcionLocalized, objetivosGlobales, pokemonesLibres);
	//recibirAppearedYGuardarlos(suscripcionAppeared, objetivosGlobales, pokemonesLibres);


	crearHilosDeEntrenadores(entrenadores);
	quickLog("Se crea un hilo por cada entrenador");


	//probando las funciones del planificador
//	PokemonEnElMapa* pokemonDePrueba1 = newPokemon();
//	t_posicion posicionDePrueba1;
//	posicionDePrueba1.posicionX = 1;
//	posicionDePrueba1.posicionY = 1;
//	setPosicionTo(pokemonDePrueba1, posicionDePrueba1);
//	setNombreTo(pokemonDePrueba1, "Delficapa");
//	list_add(pokemonesLibres, pokemonDePrueba1);
//	pasarDeNewAReady(entrenadores, pokemonesLibres);
//	Entrenador* entrenador1 = list_get(entrenadores, 0);
//	log_info(logger, "El estado del primer entrenador tendria que pasar de new a ready y se muestra : %d", entrenador1->estado);
//	//

	pthread_join(hiloConexionBroker, NULL);
	//pthread_join(hiloEscuchaGameBoy, NULL);

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
