
#include "Team.h"


int main(int argc, char *argv[]) {

	logger = iniciar_logger();

	config = leerConfigDesde("src/team.config");
	iniciarVariables();
	//obtiene la lista de entrenadores desde el config
	entrenadores = getEntrenadoresDesde("src/team.config");
	quickLog("$-Ya fueron todos los entrenadores cargados con sus posiciones, objetivos y atrapados");

//	Entrenador* entrenador1 = list_get(entrenadores, 0);
//	pasarAReadyEntrenador(entrenador1);
//	log_info(logger, "El entrenador paso a estar en exec para atrapar a %s", entrenador1->movimientoEnExec->pokemonNecesitado->nombre);

	objetivosTotales = getObjetivosTotalesDesde(entrenadores); //son pokes copias
	objetivosAtrapados = getTotalAtrapadosDesde(entrenadores);
	objetivosGlobales = getObjetivosGlobalesDesde(objetivosTotales, objetivosAtrapados);

	cantidadDeEspeciesTotales = list_size(objetivosGlobales);
	//llegan bien sus nombres
	log_info(logger, "$-La cantidad de pokemones objetivos es: %d",list_size(objetivosTotales));
	log_info(logger, "$-La cantidad de pokemones atrapados es: %d",list_size(objetivosAtrapados));
	log_info(logger, "$-La cantidad de pokemones globales que faltan por atrapar es: %d",list_size(objetivosGlobales));


	//Lanzar hilo para escuchar a GameBoy
//	pthread_t hiloEscuchaGameBoy;
//	pthread_create(&hiloEscuchaGameBoy, NULL, (void*) escucharGameBoy, NULL);


	while(!generarSocketsConBroker()){
		sleep(TIEMPO_RECONEXION);
	}

	//Lanzar hilo para concetarme a Broker
	pthread_t hilosEscuchaBroker;
	pthread_create(&hilosEscuchaBroker, NULL, (void*) crearHilosDeEscucha, NULL);



//	//verificar que el id como respuesta vuelva a enviarse a traves de ese socket
//	//recibe los nombres de pokemones encontrados libres con sus posiciones
//	//y si se necesitan (estan en los objetivos globales) se agregan a la lista de pokemones libres
	//recibirLocalizedYGuardalos(suscripcionLocalized, objetivosGlobales, pokemonesLibres);
	//recibirAppearedYGuardarlos(suscripcionAppeared, objetivosGlobales, pokemonesLibres);


	crearHilosDeEntrenadores();
	quickLog("$-Se crea un hilo por cada entrenador");

	planificarEntrenadores();

	pthread_join(hilosEscuchaBroker, NULL);
	//pthread_join(hiloEscuchaGameBoy, NULL);

	//list_destroy_and_destroy_elements(entrenadores, free);


	return 0;
}



