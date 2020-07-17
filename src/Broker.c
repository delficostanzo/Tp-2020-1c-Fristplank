#include "Broker.h"

int main(void) {

	iniciarBroker();

	pthread_t threadClientes;
	pthread_create(&threadClientes, NULL, (void*) esperarClientes, NULL);

	pthread_join(threadClientes,NULL);

//	terminar_programa(conexion, logger, config);


}

void iniciarBroker(void){
	config = leer_config();
	iniciarMemoria();
	iniciarColas();
	init_repositorio_suscriptores();
	pthread_mutex_init(&mutexMemoria,NULL);
	pthread_mutex_init(&mutexColas,NULL);
	pthread_mutex_init(&mutexIdMensaje,NULL);
	pthread_mutex_init(&mutexLRUcounter,NULL);
	pthread_mutex_init(&mutexEnvio, NULL);
}

t_config* leer_config() {
	t_config* config = config_create("../src/broker.config");

	if (config == NULL) {
		printf("No pude leer la config \n");
		exit(2);
	}

	LOG_FILE = config_get_string_value(config, "LOG_FILE");
	TAMANO_MEMORIA = config_get_int_value(config, "TAMANO_MEMORIA");
	TAMANO_MINIMO_PARTICION = config_get_int_value(config, "TAMANO_MINIMO_PARTICION");
	ALGORITMO_MEMORIA = config_get_string_value(config, "ALGORITMO_MEMORIA");
	ALGORITMO_REEMPLAZO = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
	ALGORITMO_PARTICION_LIBRE = config_get_string_value(config, "ALGORITMO_PARTICION_LIBRE");
	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");
	FRECUENCIA_COMPACTACION = config_get_int_value(config, "FRECUENCIA_COMPACTACION");

	logger = iniciar_logger(LOG_FILE);

	log_info(logger, "CONFIG FILE | IP Broker: %s", IP_BROKER);
	log_info(logger, "CONFIG FILE | Puerto Broker: %d", PUERTO_BROKER);
	log_debug(logger, "CONFIG FILE | Tamaño de memoria: %d", TAMANO_MEMORIA);
	log_debug(logger, "CONFIG FILE | Tamaño mínimo de partición: %d", TAMANO_MINIMO_PARTICION);
	log_debug(logger, "CONFIG FILE | Algoritmo de memoria: %s", ALGORITMO_MEMORIA);
	log_debug(logger, "CONFIG FILE | Algoritmo de reemplazo: %s", ALGORITMO_REEMPLAZO);
	log_debug(logger, "CONFIG FILE | Algoritmo de partición libre: %s", ALGORITMO_PARTICION_LIBRE);
	log_debug(logger, "CONFIG FILE | Frecuencia de compactación (si aplica): %d", FRECUENCIA_COMPACTACION);

	return config;
}

void terminar_programa(int conexion, t_log* logger, t_config* config) {
	if (logger != NULL) {
		log_destroy(logger);
	}

	if (config != NULL) {
		config_destroy(config); //destruye la esctructura de config en memoria, no lo esta eliminando el archivo de config
	}

	liberar_conexion(conexion);

}

void iniciarMemoria() {

	log_info(logger, "Inicializando Memoria Cache");

	if(TAMANO_MEMORIA < 1){
		log_error(logger, "LA CANTIDAD DE MEMORIA INDICADA ES NULA O NEGATIVA. SE ABORTA.");
		exit(0);
	}

	else{
		if (string_equals_ignore_case(ALGORITMO_MEMORIA, "PARTICIONES")) {
			log_info(logger, "Memoria: PARTICIONES DINÁMICAS");
			memoriaCache = malloc(TAMANO_MEMORIA);

			if(memoriaCache == NULL){
				log_error(logger, "NO HAY MEMORIA SUFICIENTE PARA ALOJAR LA CANTIDAD DE MEMORIA INDICADA. SE ABORTA.");
				exit(0);
			}
		}

		else if (string_equals_ignore_case(ALGORITMO_MEMORIA, "BS")) {
			log_info(logger, "Memoria: BUDDY SYSTEM");

			if (!esPotenciaDeDos(TAMANO_MEMORIA) || !esPotenciaDeDos(TAMANO_MINIMO_PARTICION)){
				log_error(logger, "LA CANTIDAD DE MEMORIA INDICADA NO ES POTENCIA DE DOS. SE ABORTA.");
				exit(0);
			}

			iniciarBuddySystem();
		}
		else{
			log_error(logger, "EL TIPO DE ESTRUCTURA DE MEMORIA CACHE INDICADA ES INVALIDA. SE ABORTA.");
			exit(0);
		}
	}
}

void iniciarColas() {
	pthread_mutex_lock(&mutexColas);
	IDmsg = -1;
	LRUcounter = -1;
	cantidadParticionesEliminadas = 0;
	cola[0].nombreCola = NEW_POKEMON;
	cola[0].suscriptores = list_create();
	cola[0].mensajes = list_create();
	cola[1].nombreCola = APPEARED_POKEMON;
	cola[1].suscriptores = list_create();
	cola[1].mensajes = list_create();
	cola[2].nombreCola = CATCH_POKEMON;
	cola[2].suscriptores = list_create();
	cola[2].mensajes = list_create();
	cola[3].nombreCola = CAUGHT_POKEMON;
	cola[3].suscriptores = list_create();
	cola[3].mensajes = list_create();
	cola[4].nombreCola = GET_POKEMON;
	cola[4].suscriptores = list_create();
	cola[4].mensajes = list_create();
	cola[5].nombreCola = LOCALIZED_POKEMON;
	cola[5].suscriptores = list_create();
	cola[5].mensajes = list_create();
	pthread_mutex_unlock(&mutexColas);
}

t_log* iniciar_logger(char* logFile) {
		t_log * log = malloc(sizeof(t_log));
		log = log_create(logFile, "BROKER", 1, LOG_LEVEL_DEBUG);
		if (log == NULL) {
			printf("No pude crear el logger \n");
			exit(1);
		}
		log_info(log, "Logger Iniciado");
		return log;
	}
