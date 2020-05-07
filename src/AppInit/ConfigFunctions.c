
//aca podes hacer funciones para ayudarte a extraer la info desde el archivo de configuracion

#include "ConfigFunctions.h"
#include "../TeamUtils/LogManager.h"

//crea un config a partir del nombre de un archivo de configuracion, y lo devuelve
t_config* leerConfigDesde(String nombreDeArchivo) {
	t_config* config = config_create(nombreDeArchivo);
	t_log* logger = newLoggerFrom("team.log");

	if(config == NULL){
		log_error(logger, "No se pudo abrir el archivo de configuracion '%s'", nombreDeArchivo);
		exit(1);
	}

	log_info(logger,"Archivo de configuracion '%s' leido correctamente", nombreDeArchivo);
	destroyLogger(logger);

	return config;
}

//con esto, transformas la lista de strings que te viene del config, en una t_list*
t_list* crearListaCon(String* strings, int cantidad) {
	t_list* lista = list_create();

	for(int index=0; index<cantidad; index++) {
		String unString = strings[index];

		list_add(lista, unString);
	}

	return lista;
}

//esto convierte un string de tipo "1|2" de posicion, en una t_posicion.
t_posicion* posicionDesde(String stringDePosicion) {
	t_posicion* posicion = newPosicion();

	posicion->x = atoi(string_split(stringDePosicion,"|")[0]);
	posicion->y = atoi(string_split(stringDePosicion,"|")[1]);

	return posicion;
}


