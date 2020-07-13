#include "Broker.h"

int main(void) {

	t_config* config;
	logger = iniciar_logger();
	config = leer_config();

	TAMANO_MEMORIA = config_get_int_value(config, "TAMANO_MEMORIA");
	TAMANO_MINIMO_PARTICION = config_get_int_value(config,
			"TAMANO_MINIMO_PARTICION");
	ALGORITMO_MEMORIA = config_get_string_value(config, "ALGORITMO_MEMORIA");
	ALGORITMO_REEMPLAZO = config_get_string_value(config,
			"ALGORITMO_REEMPLAZO");
	ALGORITMO_PARTICION_LIBRE = config_get_string_value(config,
			"ALGORITMO_PARTICION_LIBRE");
	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");
	int FRECUENCIA_COMPACTACION = config_get_int_value(config, "FRECUENCIA_COMPACTACION");
	//char* LOG_FILE =  config_get_string_value(config,"LOG_FILE");

	log_info(logger, "Lei la IP %s y PUERTO %d\n", IP_BROKER, PUERTO_BROKER);

	iniciarMemoria();
	iniciarColas();
	init_repositorio_suscriptores();
	pthread_mutex_init(&mutexMemoria,NULL);
	pthread_mutex_init(&mutexColas,NULL);
	pthread_mutex_init(&mutexIdMensaje,NULL);


	//crear conexion, un socket conectado
	//int conexion = crearSocket();

	pthread_t threadClientes;
	pthread_create(&threadClientes, NULL, (void*) esperarClientes, NULL);
	//sem
	//pthread_t threadAdministrador;
	//pthread_create(&threadAdministrador,NULL,(void*)administarColas,NULL);

	//loguear mensaje recibido
	//log_info(logger, "El mensaje recibido es: %s\n", mensaje);
	pthread_join(threadClientes,NULL);
	//terminar_programa(conexion, logger, config);


}

	t_config* leer_config() {
		t_config* config =
				config_create(
						"/home/utnso/Escritorio/Workspaces/tp-2020-1c-Fritsplank/Broker/src/broker.config");

		if (config == NULL) {
			printf("No pude leer la config \n");
			exit(2);
		}
		log_info(logger, "Archivo de configuracion seteado");
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


	int crearID() {
		return IDmsg++;

	}

	void iniciarMemoria() {
		//while()
		log_info(logger, "Inicializando Memoria Cache");
		memoriaCache = malloc(TAMANO_MEMORIA);

		if (string_equals_ignore_case(ALGORITMO_MEMORIA, "PARTICIONES"))
			log_info(logger,
					"LA MEMORIA UTILIZA ALGORITMO DE PARTICIONES DINAMICAS"); //no me reconoce el strcpm.
		//else if (ALGORITMO_MEMORIA == "BS") dividirMemoria();
		else
			log_error(logger, "Error al iniciar la Memoria Cache");
	}
	void iniciarColas() {
		pthread_mutex_lock(&mutexColas);
		IDmsg = -1;
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

	t_log* iniciar_logger(void) {
			t_log * log = malloc(sizeof(t_log));
			log = log_create("broker.log", "BROKER", 1, LOG_LEVEL_DEBUG);
			if (log == NULL) {
				printf("No pude crear el logger \n");
				exit(1);
			}
			log_info(log, "Logger Iniciado");
			return log;
		}
