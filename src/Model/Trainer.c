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

Entrenador* buscarPorNumero(int numero) {

	int tieneNumero(Entrenador* entrenador) {
		return entrenador->numeroEntrenador == numero;
	}

	Entrenador* entrenadorBuscado;
	pthread_mutex_lock(&mutexEntrenadores);
	entrenadorBuscado = list_find(entrenadores, (erasedTypeFilter) tieneNumero);
	pthread_mutex_unlock(&mutexEntrenadores);
	return entrenadorBuscado;
}

//imaginate esto como setters
void setPosicionA(Entrenador* entrenador, t_posicion* posicion) {
	entrenador->posicion = posicion;
}

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

/////////////char de enums/////////
char* obtenerCharDeMov(ObjetivoEnExec objetivo) {
	char* charObje;
	switch(objetivo){
			case MOVERyATRAPAR:
				charObje = "moverse y atrapar";
				break;
			case MOVEReINTERCAMBIAR:
				charObje = "moverse e intercambiar";
				break;
	}
	return charObje;
}

//////////READY/////////////
void agregarAListaReady(Entrenador* entrenadorAReady){
	pthread_mutex_lock(&mutexListaEntrenadoresReady);
	//los va agregando al final de la lista
	list_add(listaEntrenadoresReady, entrenadorAReady);
	pthread_mutex_unlock(&mutexListaEntrenadoresReady);
}

void sacarDeListaReady(Entrenador* entrenadorASacar) {
	int esElEntrenador(Entrenador* entrenador) {
		return entrenador->numeroEntrenador == entrenadorASacar->numeroEntrenador;
	}

	//si ese entrenador esta en la lista sacarlo
	pthread_mutex_lock(&mutexListaEntrenadoresReady);
	if(list_is_empty(listaEntrenadoresReady) != 1){
		list_remove_by_condition(listaEntrenadoresReady, (erasedTypeFilter) esElEntrenador);
	}
	pthread_mutex_unlock(&mutexListaEntrenadoresReady);
}


////////////ATRAPA///////////////

void agregarAtrapado(Entrenador* entrenador, PokemonEnElMapa* pokemonAtrapado){
	setPokemonA(entrenador->pokemonesAtrapados, pokemonAtrapado);
}

void estadoSiAtrapo(Entrenador* entrenador) {
	quickLog("$-El entrenador va a cambiar de estado por atrapar un pokemon");
	if(sonIguales(entrenador->pokemonesObjetivos,entrenador->pokemonesAtrapados)){
		//ya agarro todos sus pokemones
		pasarAExit(entrenador);
		log_info(LO, "El entrenador %d paso a estado exit porque ya tiene atrapados todos sus objetivos", entrenador->numeroEntrenador);
	}
	else if(tienenLaMismaCantidad(entrenador->pokemonesObjetivos,entrenador->pokemonesAtrapados)){
		asignarMovimientoPorDeadlock(entrenador);
		pasarADeadlock(entrenador);
		log_info(LO, "El entrenador %d paso a block por deadlock porque no puede atrapar mas y sus atrapados no son los mismos que los objetivos", entrenador->numeroEntrenador);
	}
	else {
		pasarADormido(entrenador);
		log_info(LO, "El entrenador %d paso a block dormido esperando que le den un pokemon para atrapar", entrenador->numeroEntrenador);

		quickLog("$-Quedo bloqueado dormido el entrenador");

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

		log_info(logger, "$-Tiene %d pokemones atrapados y %d pokemones objetivos por atrapar", cantidadAtrapados, cantidadObjetivos);

	}
}

int sonIguales(t_list* objetivos, t_list* atrapados) {

	int estaEnAtrapados(PokemonEnElMapa* objetivo) {

		//nunca va a estar vacia porque esta fijandose que hace con el poke nuevo atrapado por el entrenador
		if(list_is_empty(atrapados) != 1){
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
	pthread_mutex_lock(&entrenador->mutexEstado);
	entrenador->estado = 4;
	entrenador->motivo = 2;
	pthread_mutex_unlock(&entrenador->mutexEstado);
}

void pasarADeadlock(Entrenador* entrenador) {
	pthread_mutex_lock(&entrenador->mutexEstado);
	entrenador->estado = 4;
	entrenador->motivo = 3;
	pthread_mutex_unlock(&entrenador->mutexEstado);
}

void pasarAExit(Entrenador* entrenador) {
	pthread_mutex_lock(&entrenador->mutexEstado);
	entrenador->estado = 5;
	pthread_mutex_unlock(&entrenador->mutexEstado);
}

int noEstaEnExit(Entrenador* entrenador){
	//sem_wait(&semaforoEstados);
	pthread_mutex_lock(&entrenador->mutexEstado);
	int cumple = entrenador->estado != 5;
	pthread_mutex_unlock(&entrenador->mutexEstado);
	//sem_post(&semaforoEstados);
	return cumple;
}

void pasarABlockEsperando(Entrenador* entrenador) {
	//sem_wait(&semaforoEstados);
	pthread_mutex_lock(&entrenador->mutexEstado);
	entrenador->estado = 4;
	entrenador->motivo = 1;
	pthread_mutex_unlock(&entrenador->mutexEstado);
	//sem_post(&semaforoEstados);
	log_info(logger, "$-El estado del entrenador paso a: %d", entrenador->estado);
}

/////////////INTERCAMBIO////////////////

//int puedeIntercambiar(Entrenador* entrenador, PokemonEnElMapa* pokemonDado){
//	//condicione del entrenador preguntando
//	MovimientoEnExec* movimientoEnExec = entrenador->movimientoEnExec;
//	char* nombrePokemonAIntercambiar = movimientoEnExec->pokemonAIntercambiar->nombre;
//	char* nombrePokemonNecesitado = movimientoEnExec->pokemonNecesitado->nombre;
//
//	//condiciones del entrenador que cumple la condicion
//	char* nombrePokemonDado = pokemonDado->nombre;
//
//	return strcmp(nombrePokemonNecesitado, nombrePokemonDado) == 0;
//}


void asignarMovimientoPorDeadlock(Entrenador* entrenador){
	PokemonEnElMapa* atrapadoDeMas = asignarPokemonCopia(buscarAtrapadoDeMas(entrenador));
	PokemonEnElMapa* objetivoNoCumplido = asignarPokemonCopia(buscarObjetivosQueFalta(entrenador));

	atrapadoDeMas->cantidad = 1;
	objetivoNoCumplido->cantidad = 1;
	MovimientoEnExec* movimiento = malloc(sizeof(MovimientoEnExec));
	movimiento->objetivo = 2;
	movimiento->pokemonAIntercambiar = atrapadoDeMas;
	movimiento->pokemonNecesitado = objetivoNoCumplido;
	entrenador->movimientoEnExec = movimiento;
	//lo vuelvo a poner en 5 por si ya venia de un deadlock que le disminuyo esta cantidad
	entrenador->ciclosCPUFaltantesIntercambio = 5;
}


PokemonEnElMapa* buscarAtrapadoDeMas(Entrenador* entrenador){
	t_list* atrapados = entrenador->pokemonesAtrapados;
	t_list* objetivos = entrenador->pokemonesObjetivos;

	int noEstaComoObjetivo(PokemonEnElMapa* atrapado) {
		if(estaElPokemon(atrapado, objetivos)){
			//tienen el mismo nombre
			PokemonEnElMapa* objetivo = buscarPorNombre(atrapado->nombre, objetivos);
			//pero hay un atrapado de mas
			return (atrapado->cantidad) > (objetivo->cantidad);
		}
		return 1;
	}
	//TRAE EL QUE ESTA DE MAS, EL QUE SOBRA
	return list_find(atrapados, (erasedTypeFilter) noEstaComoObjetivo);
}

PokemonEnElMapa* buscarObjetivosQueFalta(Entrenador* entrenador){
	t_list* atrapados = entrenador->pokemonesAtrapados;
	t_list* objetivos = entrenador->pokemonesObjetivos;

	int noEstaComoAtrapado(PokemonEnElMapa* objetivo) {
		if(estaElPokemon(objetivo, atrapados)){
			//tienen el mismo nombre
			PokemonEnElMapa* atrapado = buscarPorNombre(objetivo->nombre, atrapados);
			//pero hay uno atrapado de mas
			return (atrapado->cantidad) < (objetivo->cantidad);
		}
		return 1;
	}
	//TRAE EL OBJETIVO QUE FALTA
	return list_find(objetivos, (erasedTypeFilter) noEstaComoAtrapado);
}


//siempre usar entre mutex de entrenadores
int esteComoIntercambio(Entrenador* entrenador) {

	int estaComoIntercambio(Entrenador* entrenadorMoviendose) {
		return entrenadorMoviendose->movimientoEnExec->numeroDelEntrenadorIntercambio == entrenador->numeroEntrenador;
	}

	pthread_mutex_lock(&mutexEntrenadores);
	int cumple  = list_any_satisfy(entrenadores, (erasedTypeFilter) estaComoIntercambio);
	pthread_mutex_unlock(&mutexEntrenadores);

	return cumple;
}




