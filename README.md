# tp-2020-1c-Fritsplank

##2020-05-31 Conexión
###Cambios hechos:

- Agrego RESPUESTA_ID como mensaje en enum.

- Saco el id_correlativo de los struct t_caught_pokemon y otros para agregarlo en el  header (t_paquete)

- A raíz de esto agrego IdCorrelativo a los métodos de enviar_*mensaje*_pokemon.

- Creo función para mandar un ID Correlativo (Lo que le manda el Broker al Team en el catch) -> void enviar_respuesta_id

- Mensaje ACK -> Pensé un mensaje que no tenga struct. Simplemente es un mensaje que los módulos le responden al broker con el ID del mensaje que recibieron en ID_CORRELATIVO del ACK que están mandando.
- Descomenté esto en enviar que no sé porque estaba comentado:
	free(paquete->buffer);
	free(paquete);

- Creé un nuevo iniciar_log porque estaba hardcodeado el nombre "broker", entonces no te deja iniciar loggers en ningún módulo que tengan otro nombre (De todos modos dejé comentado el anterior).
	Pensé en un iniciar_log que le enviás el nombre que querés que tenga (Si estás en Team le mandás Team, en GameCard Gamecard, duh).
	Relacionado a esto, habría que ver si tiene sentido cambiar la firma de por ejemplo "liberar_conexion" para que acepte por parámetro un logger, así no tiene que crear uno adentro).
	Lo mismo con "enviar" y todos los otros métodos que loggeen.