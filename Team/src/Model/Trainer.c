#include "Trainer.h"


void pasarAExit(Entrenador* entrenador);
static int sumaCantidades(t_list* pokemones);
static int tienenLaMismaCantidad(t_list* objetivos, t_list* atrapados);
static int sonIguales(t_list* objetivos, t_list* atrapados);
static PokemonEnElMapa* buscarAtrapadoDeMas(Entrenador* entrenador);
static PokemonEnElMapa* buscarObjetivosQueFalta(Entrenador* entrenador);


typedef bool(*erasedTypeFilter)(void*);

//inicializa y devuelve un trainer nuevo. Es el new, al estilo Java. ESTADO NEW
Entrenador* newEntrenador() {
	return malloc(sizeof(Entrenador));
}

//imaginate esto como setters
void setPosicionA(Entrenador* entrenador, t_posicion* posicion) {
	entrenador->posicion = posicion;
}

//void setPokemonesObjetivosA(Entrenador* entrenador, t_list* pokemones) {
//	entrenador->pokemonesObjetivos = pokemones;
//}

//hacer el ADD solo si esa especie no estaba antes en la lista
void setPokemonA(t_list* listaPokemones, PokemonEnElMapa* nuevoPokemon) {

	if(list_is_empty(listaPokemones) == 1){
		list_add(listaPokemones, nuevoPokemon);
	}
	//YA HAY UNO DE ESA ESPECIE EN LA LISTA DE POKEMONES PASADA POR PARAMETRO
	else if(buscarPorNombre(nuevoPokemon->nombre, listaPokemones) != NULL){
		//le aumento la cantidad
		PokemonEnElMapa* pokemonASumar = buscarPorNombre(nuevoPokemon->nombre, listaPokemones);
		pokemonASumar->cantidad += nuevoPokemon->cantidad;
	}
		else {
		list_add(listaPokemones, nuevoPokemon);
		}
}


//void setPokemonesAtrapadosA(Entrenador* entrenador, t_list* pokemones) {
//	entrenador->pokemonesAtrapados = pokemones;
//}

t_posicion* newPosicion() {
	return malloc(sizeof(t_posicion));
}

Entrenador* entrenadorMasCercanoA(PokemonEnElMapa* pokemon, t_list* entrenadores) {
	typedef bool(*erasedTypeSort)(void*, void*);

	bool estaMasCerca(Entrenador* entrenador1, Entrenador* entrenador2) {
		int distanciaEntrenador1 = distanciaEntre(entrenador1->posicion, &(pokemon->posicion));
		int distanciaEntrenador2 = distanciaEntre(entrenador2->posicion, &(pokemon->posicion));
		return distanciaEntrenador1 <= distanciaEntrenador2;

	}

	list_sort(entrenadores, (erasedTypeSort)estaMasCerca);
	return list_get(entrenadores, 0);
}

int distanciaEntre(t_posicion* posicion1, t_posicion* posicion2) {
	int distanciaEnX = posicion1->posicionX - posicion2->posicionX;
	int distanciaEnY = posicion1->posicionY - posicion2->posicionY;
	return abs(distanciaEnX) + abs(distanciaEnY);
}

////////////ATRAPA///////////////

//void atrapar(Entrenador* entrenador, PokemonEnElMapa* pokemon) {
//	int distanciaHastaPokemon = distanciaEntre(&(pokemon->posicion), entrenador->posicion);
//	entrenador->ciclosCPUConsumido += distanciaHastaPokemon;
//	entrenador->posicion = &(pokemon->posicion);
//	//el socket ya esta conectado con el broker en Conexion
//	enviarCatchDesde(entrenador);
//	quickLog("Esta por cambiarle el estado a bloqueado para esperar respuesta");
//	pasarABlockEsperando(entrenador);
//}

void agregarAtrapado(Entrenador* entrenador, PokemonEnElMapa* pokemonAtrapado){
	setPokemonA(entrenador->pokemonesAtrapados, pokemonAtrapado);
}

void estadoSiAtrapo(Entrenador* entrenador) {
	t_log* logger = iniciar_logger();
	quickLog("El entrenador va a cambiar de estado por atrapar un pokemon");
	if(sonIguales(entrenador->pokemonesObjetivos,entrenador->pokemonesAtrapados)){
		//ya agarro todos sus pokemones
		pasarAExit(entrenador);
	}
	else if(tienenLaMismaCantidad(entrenador->pokemonesObjetivos,entrenador->pokemonesAtrapados)){
		pasarADeadlock(entrenador);
		asignarMovimientoPorDeadlock(entrenador);
	}
	else {
		pasarADormido(entrenador);
		quickLog("Quedo bloqueado dormido el entrenador");

		int cantidadAtrapados;
		int cantidadObjetivos;
		if(list_is_empty(entrenador->pokemonesAtrapados)) {
			cantidadAtrapados = 0;
		} else {
			cantidadAtrapados = list_size(entrenador->pokemonesAtrapados);
		}
		if(list_is_empty(entrenador->pokemonesObjetivos)) {
			cantidadObjetivos = 0;
		} else {
			cantidadObjetivos = list_size(entrenador->pokemonesAtrapados);
		}

		log_info(logger, "Tiene %d pokemones atrapados y %d pokemones objetivos por atrapar", cantidadAtrapados, cantidadObjetivos);
		destruirLog(logger);
	}
}

int sonIguales(t_list* objetivos, t_list* atrapados) {

	int estaEnAtrapados(PokemonEnElMapa* objetivo) {

		//nunca va a estar vacia porque esta fijandose que hace con el poke nuevo atrapado por el entrenador
		if(list_is_empty(atrapados)){
			int esUnoDeLosObje(PokemonEnElMapa* atrapado){
				return strcmp(atrapado->nombre, objetivo->nombre) == 0;
			}

			PokemonEnElMapa* pokemonAtrapado = list_find(atrapados, (erasedTypeFilter)esUnoDeLosObje);
			if(pokemonAtrapado != NULL){
				return pokemonAtrapado->cantidad == objetivo->cantidad;
			}
			return 0;
		}

		return 0;
	}

	//todos sus objetivos estan en la lista de sus atrapados con la misma cantidad
	return list_all_satisfy(objetivos, (erasedTypeFilter) estaEnAtrapados);
}

int tienenLaMismaCantidad(t_list* objetivos, t_list* atrapados) {
	return sumaCantidades(objetivos) == sumaCantidades(atrapados);
}

int sumaCantidades(t_list* pokemones) {
	typedef void*(*erasedTypeFold)(void*, void*);

	int sumarCantidad(int primerCantidad, PokemonEnElMapa* pokemon) {
		int cantidadPoke = pokemon->cantidad;
		return primerCantidad + cantidadPoke;
	}

	if(list_is_empty(pokemones)) {
		return 0;
	}

	int cantidadFinal = list_fold(pokemones, 0, (erasedTypeFold)(sumarCantidad));
	return cantidadFinal;
}

void pasarADormido(Entrenador* entrenador) {
	entrenador->estado = 4;
	entrenador->motivo = 2;
}

void pasarADeadlock(Entrenador* entrenador) {
	entrenador->estado = 4;
	entrenador->motivo = 3;
}

void pasarAExit(Entrenador* entrenador) {
	entrenador->estado = 5;
}

int noEstaEnExit(Entrenador* entrenador){
	sem_wait(&semaforoEstados);
	int cumple = entrenador->estado != 5;
	sem_post(&semaforoEstados);
	return cumple;
}

void pasarABlockEsperando(Entrenador* entrenador) {
	t_log* logger = iniciar_logger();
	sem_wait(&semaforoEstados);
	entrenador->estado = 4;
	entrenador->motivo = 1;
	sem_post(&semaforoEstados);
	log_info(logger, "El estado del entrenador paso a: %d", entrenador->estado);
	destruirLog(logger);
}

/////////////INTERCAMBIO////////////////

int puedeIntercambiar(Entrenador* entrenador, PokemonEnElMapa* pokemonInnecesario, PokemonEnElMapa* pokemonDado){
	//condicione del entrenador preguntando
	MovimientoEnExec* movimientoEnExec = entrenador->movimientoEnExec;
	char* nombrePokemonAIntercambiar = movimientoEnExec->pokemonAIntercambiar->nombre;
	char* nombrePokemonNecesitado = movimientoEnExec->pokemonNecesitado->nombre;

	//condiciones del entrenador que cumple la condicion
	char* nombrePokemonInnecesario = pokemonInnecesario->nombre;
	char* nombrePokemonDado = pokemonDado->nombre;

	return nombrePokemonAIntercambiar == nombrePokemonInnecesario && nombrePokemonNecesitado == nombrePokemonDado;
}


void asignarMovimientoPorDeadlock(Entrenador* entrenador){
	PokemonEnElMapa* atrapadoDeMas = buscarAtrapadoDeMas(entrenador);
	PokemonEnElMapa* objetivoNoCumplido = buscarObjetivosQueFalta(entrenador);

	MovimientoEnExec* movimiento = malloc(sizeof(MovimientoEnExec));
	movimiento->objetivo = 2;
	movimiento->pokemonAIntercambiar = asignarPokemonCopia(atrapadoDeMas);
	movimiento->pokemonNecesitado = asignarPokemonCopia(objetivoNoCumplido);
	entrenador->movimientoEnExec = movimiento;

}


PokemonEnElMapa* buscarAtrapadoDeMas(Entrenador* entrenador){
	t_list* atrapados = entrenador->pokemonesAtrapados;
	t_list* objetivos = entrenador->pokemonesObjetivos;

	int noEstaComoObjetivo(PokemonEnElMapa* atrapado) {
		if(estaElPokemon(atrapado, objetivos)){
			//tienen el mismo nombre
			PokemonEnElMapa* atrapado = buscarPorNombre(atrapado->nombre, atrapados);
			PokemonEnElMapa* objetivo = buscarPorNombre(atrapado->nombre, objetivos);
			//pero hay uno atrapado de mas
			return (atrapado->cantidad) > (objetivo->cantidad);
		}
		return 0;
	}
	//TRAE EL QUE ESTA DE MAS, SOBRA
	return list_find(atrapados, (erasedTypeFilter) noEstaComoObjetivo);
}

PokemonEnElMapa* buscarObjetivosQueFalta(Entrenador* entrenador){
	t_list* atrapados = entrenador->pokemonesAtrapados;
	t_list* objetivos = entrenador->pokemonesObjetivos;

	int noEstaComoAtrapado(PokemonEnElMapa* objetivo) {
		if(estaElPokemon(objetivo, atrapados)){
			//tienen el mismo nombre
			PokemonEnElMapa* atrapado = buscarPorNombre(atrapado->nombre, atrapados);
			PokemonEnElMapa* objetivo = buscarPorNombre(atrapado->nombre, objetivos);
			//pero hay uno atrapado de mas
			return (atrapado->cantidad) > (objetivo->cantidad);
		}
		return 0;
	}
	//TRAE EL QUE FALTA
	return list_find(objetivos, (erasedTypeFilter) noEstaComoAtrapado);
}





