//Translation starts. Aphro~ 07/06/2010
static const char * guildRankName[GUILDRANK_MAX] = {
	"Jefe del Gremio",
	"Capitán",
	"Lider de Caza",
	"Lider de Incursión",
	"Miembro del Gremio",
	"Recluta"
};

#define GetGuildRankName(i)	( ((uint32)i < GUILDRANK_MAX) ? guildRankName[i] : guildRankName[GUILDRANK_GUILDSMAN] )

#define QUESTHELPER			"Ayudante para la Misión"
#define QUESTHELPERCOMPLETE	"Misión Completa!"

#define NPCLVL1		"Nivel 10~25"
#define NPCLVL2		"Nivel 25~50"
#define NPCLVL3		"Nivel 50~75"
#define NPCLVL4		"Nivel 75~85"
#define NPCLVL5		"Nivel 85~110"
#define NPCLVL6		"Nivel 110~135"
#define NPCLVL7		"Nivel 135~160"
#define NPCLVL8		"Boss"
#define NPCLVL9		"Elite"

#define MSG_KILLER1 "50+ Asesinatos"
#define MSG_KILLER2 "100+ Asesinatos" 
#define MSG_KILLER3 "150+ Asesinatos" 
#define MSG_KILLER4 "200+ Asesinatos" 
#define MSG_KILLER5 "250+ Asesinatos" 
#define MSG_KILLER6 "300+ Asesinatos" 
#define MSG_KILLER7 "350+ Asesinatos"  
#define MSG_KILLER8 "400+ Asesinatos" 
#define MSG_KILLER9 "450+ Asesinatos" 
#define MSG_KILLER10 "500+ Asesinatos"
#define MSG_KILLER11 "1000+ Asesinatos" 

#define	UPDATE_SCREEN_ON_CONNECTING1	"Presione la tecla ESC si pasa mucho tiempo"
#define	UPDATE_SCREEN_ON_CONNECTING2	"sin conectarse para volver al menú principal."
#define	UPDATE_SCREEN_ON_CONNECTING3	"Conectando al servidor. Espere por favor..."
#define	UPDATE_SCREEN_ON_CONNECTING4	"Fuck! the fucking thing fucked up..."

#define	UPDATE_SCREEN_ON_LOADING_PROGRESS1	"Cargando los datos del juego."
#define	UPDATE_SCREEN_ON_LOADING_PROGRESS2	"Por favor espere."

#define	UPDATE_SCREEN_ON_WAIT_INIT_DATA1	"Presione la tecla ESC si pasa mucho tiempo"
#define	UPDATE_SCREEN_ON_WAIT_INIT_DATA2	"sin conectarse para volver al menú principal."
#define	UPDATE_SCREEN_ON_WAIT_INIT_DATA3	"Conectando al servidor. Espere por favor..."

#define	UPDATE_SCREEN_ON_CONNECTION_LOST	"Conexion perdida!"

// #define MSG_WORLDNAME1	"ABADDON Server" // moved to globaldef.h
// #define MSG_WORLDNAME2	"APOCALYPSE Server"
#define MSG_WORLDNAME3	"3rd Server"
#define MSG_WORLDNAME4	"4th Server"
#define MSG_WORLDNAME5	"5th Server"
#define MSG_WORLDNAME6	"6th Server"
#define MSG_WORLDNAME7	"7th Server"
#define MSG_WORLDNAME8	"8th Server"
#define MSG_WORLDNAME9	"9th Server"
#define MSG_WORLDNAME10	"10th Server"
#define MSG_WORLDNAME11	"11th Server"
#define MSG_WORLDNAME12	"12th Server"
#define MSG_WORLDNAME13	"13th Server"
#define MSG_WORLDNAME14	"14th Server"
#define MSG_WORLDNAME15	"15th Server"
#define MSG_WORLDNAME16	"16th Server"

// Slates - Diuuude
#define MSG_NOTIFY_SLATE_CREATESUCCESS	"La ancient tablet se completó!"
#define MSG_NOTIFY_SLATE_CREATEFAIL		"Las tabletas se destruyeron con un agudo sonido."
#define MSG_NOTIFY_SLATECLEAR			"El efecto de la ancient tablet desapareció."
#define MSG_NOTIFY_SLATE_INVINCIBLE		"Sos invencible gracias a la protección de Kloness, el Dios de la justicia!"
#define MSG_NOTIFY_SLATE_MANA			"Tu mana sera rellenado por el grán mago."
#define MSG_NOTIFY_SLATE_EXP			"Adquiriste la sabiduría. Ahora, la experiencia que ganes de tus enemigos, aumentara."
#define MSG_NOTIFY_SLATE_ALREADYUSING	"Ya estas usando otra tableta."
#define MSG_NOTIFY_SLATE_WRONG_MAP		"No la puedes usar en este lugar."

// Snoopy: 360+ reversing
#define MSG_NOTIFY_SLATE_BERSERK		"Magia Berserk lanzada!"
#define MSG_NOTIFY_LOTERY_LOST			"You draw a blank. Please try again next time.." //Usage??? Didn't translate.
#define MSG_NOTIFY_CRAFTING_NO_CONTRIB  "No tienes los suficientes puntos de contribución."
#define MSG_NOTIFY_CRAFTING_NO_PART		"No tienes el suficiente material."
#define MSG_NOTIFY_CRAFTING_FAILED		"La operacion falló."
#define MSG_NOTIFY_ANGEL_RECEIVED		"Has recibido un Angel de la guarda."
#define MSG_NOTIFY_ANGEL_FAILED			"Imposible recibir un Angel de la guarda." // Invented by Snoopy...
#define MSG_NOTIFY_ANGEL_MAJESTIC		"Necesitas mas Majestic Points."
#define MSG_NOTIFY_ANGEL_LOW_LVL		"Solo los personajes que reciban Majestic Points pueden recibir un angel."
#define MSG_NOTIFY_NOT_RELEASED			"No puedes sacarte el objeto."	
#define BDLBBOX_DOUBLE_CLICK_INVENTORY17	"Comenzando la operacion..."
#define DLGBOX_CLICK_SKILLDLG2				"No hay suficientes materiales. Por favor pon mas materiales."
#define DLGBOX_CLICK_SKILLDLG3				"La creacion del objeto esta en progreso."				


#define	_BDRAW_ON_CREATE_NEW_CHARACTER1	"Escribe el nombre del personaje."
#define	_BDRAW_ON_CREATE_NEW_CHARACTER2	"Elije el sexo."
#define	_BDRAW_ON_CREATE_NEW_CHARACTER3	"Elije el color de piel."
#define	_BDRAW_ON_CREATE_NEW_CHARACTER4	"Puntos especiales restantes: %d"


#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER1	"Escribe el nombre del personaje."
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER2	"Elije el sexo."
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER3	"Elije el color de piel."
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER4	"Elije el tipo de pelo."
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER5	"Elije el color del pelo."
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER6	"Elije el color de su ropa interior."

#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER7	"Determina la fuerza inicial de tu"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER8	"personaje. A medida que la FUERZA(STR)"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER9	"aumenta, la salud del personaje"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER10	"y su stamina suben."
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER11	"Podras equipar armas pesadas y escudos."

#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER12	"Determina la vitalidad inicial de tu"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER13	"personaje. A medida que la VITALIDAD(VIT)"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER14	"aumenta, aumenta la cantidad de puntos"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER15	"de vida y la reecuperacion de los mismos."
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER16	"Aumenta en poco grado la defensa fisica. "

#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER17	"Determina la destreza inicial de tu"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER18	"personaje. A medida que la DESTREZA(DEX)"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER19	"aumenta, la evasion a los golpes aumenta"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER20	"y la probabilidad de dañar tambien."

#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER21	"Determina la inteligencia inicial de tu"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER22	"personaje. A medida que la INTELIGENCIA(INT)"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER23	"aumenta, mas hechizos podras aprender,"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER24	"la probabilidad de lanzarlos y tu mana aumentaran tambien."//90% chance of fit problem

#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER25	"Determina la magia inicial de tu"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER26	"personaje. A medida que la MAGIA(MAG)"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER27	"aumenta, la resistencia a la magia,"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER28	"la cantidad y la recuperacion tambien lo haran."
                                                 
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER29	"Determina el carisma inicial de tu"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER30	"personaje. A medida que el CARISMA(CHR)"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER31	"aumenta, podras crear tu propio gremio"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER32	"y el costo de los items se reducirá."

#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER35	"Por favor escribe un nombre."

#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER36	"Debes elegir una clase."

#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER39	"No puedes usar caracteres especiales"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER40	"en el nombre del personaje."
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER41	"Por favor escribelo otra vez."

#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER42	"El nombre del personaje no corresponde."
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER43	"Por favor escribe otro nombre"

#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER44	"Crea el personaje "
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER45	"con la siguiente configuracion."
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER46	"     "
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER47	"     "
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER48	"     "

#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER49	"Volver al menu principal"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER50	"Otorga configuracion para un Warrior"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER51	"Otorga configuracion para un Mage"
#define	UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER52	"Otorga configuracion para un Priest"


#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT1	"Escribe la ID de tu cuenta."
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT2	"( Solo letras y numeros )"
//#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT3	"and numbers, no special keywords."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT4	"Escribe la contraseña de tu cuenta."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT8	"Confirma la contraseña."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT11	"Escribe tu nombre real"
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT12	"Debes escribir tu nombre real bien"
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT13	"para confirmar la cuenta "
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT14	"y el propietario"

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT15   "     "
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT16   "     "
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT17   "     "
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT18   "     "
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT19   "     "
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT20   "     "

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT21	"Escribe tu direccion de E-mail"
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT22	"Debes escribir bien tu E-mail"
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT23	"para confirmar ser el propietario."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT25	"Escribe la pregunta secreta para poder"
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT26	"recuperar la contraseña si te la olvidas."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT29	"Responde la pregunta."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT33	"Crear la cuenta con lo dado."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT35	"Por favor escribe el nombre de la cuenta."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT38	"Por favor escribe la contraseña"

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT42	"Por favor confirme la contraseña"

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT46	"El nombre real no esta escrito"
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT47	"Por favor escribe tu nombre real."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT50	"Por favor escribe tu E-mail."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT52	"La cuenta contiene caracteres especiales."
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT53	"Por favor escribela denuevo."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT56	"La contraseña contiene caracteres especiales."
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT57	"Por favor escribela denuevo."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT63	"La contraseña y la confirmacion son distintas."
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT64	"Por favor vuelve a escribir la contraseña"
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT65	"y la confirmacion."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT67	"Por favor escriba la pregunta"

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT69	"Por favor escriba la respuesta."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT73	"La pregunta contiene caracteres especiales."
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT74	"Escriba otra pregunta."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT77	"La respuesta contiene caracteres especiales."
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT78	"Escriba otra respuesta."

#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT80	"Borrar todo."
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT81	"Volver al menu principal."
#define	UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT82	"Imposible crear cuenta! Las contraseñas difieren!"

//Usage?? Didn't translate. Aphro~
#define	UPDATE_SCREEN_ON_LOGIN1	"Searching the fastest gateway."
#define	UPDATE_SCREEN_ON_LOGIN2	"Por favor espere un momento..."

#define	ON_KEY_UP1	" No hay nada grabado en este atajo."
#define	ON_KEY_UP2	" Equipe algo o tire algun hechizo"
#define	ON_KEY_UP3	" y presione [Control]-[F2] para grabar."

#define	ON_KEY_UP4	"Objeto(%s %s %s) : grabado en [F2]"
#define	ON_KEY_UP5	"Magia(%s) : grabado en [F2]"

#define	ON_KEY_UP6	" No hay nada grabado en este atajo."
#define	ON_KEY_UP7	" Equipe algo o tire algun hechizo"
#define	ON_KEY_UP8	" y presione [Control]-[F2] para grabar."

#define	ON_KEY_UP9	"Tus manos deben estar libres para invocar un hechizo."
#define	ON_KEY_UP10	"No puedes invocar una magia mientras usas una habilidad."

#define	ON_KEY_UP12	" No hay nada grabado en este atajo."
#define	ON_KEY_UP13	" Equipe algo o tire algun hechizo"
#define	ON_KEY_UP14	" y presione [control]-[F3] para grabar."

#define	ON_KEY_UP15	"Objeto(%s %s %s) : grabado en [F3]"
#define	ON_KEY_UP16	"Magia(%s) : grabado en [F3]"

#define	ON_KEY_UP17	" No hay nada grabado en este atajo."
#define	ON_KEY_UP18	" Equipe algo o tire algun hechizo"
#define	ON_KEY_UP19	" y presione [control]-[F3] para grabar."

#define	ON_KEY_UP26	"No tienes equipado ningun objeto con habilidad especial."
#define	ON_KEY_UP28	"Ya estas usando una habilidad especial."

#define	ON_KEY_UP29	"Habilidad que disminuye el HP un 50%: puede ser usada luego de %d seg"
#define	ON_KEY_UP30	"Habilidad que congela al enemigo: puede ser usada luego de %d seg"
#define	ON_KEY_UP31	"Habilidad que paraliza al enemigo: puede ser usada luego de %d seg"
#define	ON_KEY_UP32	"Habilidad que mata a tu enemigo de una vez: puede ser usada luego de %d seg"
#define	ON_KEY_UP33	"Habilidad que adquieres HP al mismo tiempo que se la sacas a tu enemigo: puede ser usada luego de %d seg"
#define	ON_KEY_UP34	"Habilidad que rompe el arma del enemigo: puede ser usada luego de %d seg"
#define	ON_KEY_UP35	"Habilidad que protege del daño fisico: puede ser usada luego de %d seg"
#define	ON_KEY_UP36	"Habilidad que hace al personaje intocable: puede ser usada luego de %d seg"

#define	ON_KEY_UP37	"Habilidad que disminuye el HP un 50%: puede ser usada luego de %d minutos"
#define	ON_KEY_UP38	"Habilidad que congela al enemigo: puede ser usada luego de %d minutos"
#define	ON_KEY_UP39	"Habilidad que paraliza al enemigo: puede ser usada luego de %d minutos"
#define	ON_KEY_UP40	"Habilidad que mata a tu enemigo de una vez: puede ser usada luego de %d minutos"
#define	ON_KEY_UP41	"Habilidad que adquieres HP al mismo tiempo que se la sacas a tu enemigo: puede ser usada luego de %d minutos"
#define	ON_KEY_UP42	"Habilidad que rompe el arma del enemigo: puede ser usada luego de %d minutos"
#define	ON_KEY_UP43	"Habilidad que protege del daño fisico: puede ser usada luego de %d minutos"
#define	ON_KEY_UP44	"Habilidad que hace al personaje intocable: puede ser usada luego de %d minutos"

#define	UPDATE_SCREEN_ON_QUERY_FORCE_LOGIN1	"¿Quieres"
#define	UPDATE_SCREEN_ON_QUERY_FORCE_LOGIN2	"interrumpir la conexion?"


#define	UPDATE_SCREEN_ON_SELECT_CHARACTER1	"Entrar con el siguiente personaje."
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER2	"Puedes entrar presionando el boton"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER3	"o presionando la tecla enter luego de seleccionar"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER4	"el personaje."

#define	UPDATE_SCREEN_ON_SELECT_CHARACTER5	"Crear nuevo personaje."

#define	UPDATE_SCREEN_ON_SELECT_CHARACTER6	"Borrar el personaje seleccionado. Los personajes"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER7	"borrados no pueden ser recuperados."
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER8	"Tampoco puedes borrar un personaje mayor"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER9	"de nivel 50. Si quieres borrar un personaje"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER10	"mayor a nivel 50, envia un e-mail"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER11	"al Game Master."

#define	UPDATE_SCREEN_ON_SELECT_CHARACTER12	"Cambiar la contraseña."
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER13	"Volver al menu principal."

#define	UPDATE_SCREEN_ON_SELECT_CHARACTER14	"No hay personajes en la cuenta."
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER15	"Cada cuenta puede tenes 4 personajes"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER16	"maximo. Puedes crear un nuevo personaje"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER17	"haciendo click en new character o "
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER18	"haciendo click en el espacio en blanco."

#define	UPDATE_SCREEN_ON_SELECT_CHARACTER19	"Puedes comenzar haciendo click en"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER20	"el boton START, y crear un nuevo personaje."
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER21	"Si quieres borrarlo, presiona el boton"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER22	"DELETE CHARACTER despues de haber"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER23	"seleccionado el personaje que"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER24	"quieras borrar."

#define	UPDATE_SCREEN_ON_SELECT_CHARACTER25	"Puedes comenzar haciendo click en"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER26	"el boton START. Si quieres borra el personaje"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER27	"presiona el boton DELETE CHARACTER"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER28	"despues de seleccionarlo."

#define	UPDATE_SCREEN_ON_SELECT_CHARACTER31	"   "
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER34	"   "
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER36	"Test Server"

#define	UPDATE_SCREEN_ON_SELECT_CHARACTER37	"Cuenta valida hasta (%d/%d/%d)"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER38	"Tiempo que queda: %dD %dHr %dMin"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER39	"Cuenta no registrada."
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER40	"Tiempo de IP: %dHr %dMin"
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER41	"IP meter-rate term: %dD %dHr %dMin"
//#define	UPDATE_SCREEN_ON_SELECT_CHARACTER42	"It is not a registered IP."
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER42	" "
#define	UPDATE_SCREEN_ON_SELECT_CHARACTER43	"Ultimo salida : %d/%d/%d  %d : %d"

#define	UPDATE_SCREEN_ON_WATING_RESPONSE1	"Presione la tecla ESC si pasa mucho tiempo"
#define	UPDATE_SCREEN_ON_WATING_RESPONSE2	"sin conectarse para volver al menu principal."
#define	UPDATE_SCREEN_ON_WATING_RESPONSE3	"Conectando al servidor. Espere por favor.."


#define	UPDATE_SCREEN_ON_QUERY_DELETE_CHARACTER1	"Nombre del personaje"
#define	UPDATE_SCREEN_ON_QUERY_DELETE_CHARACTER2	"¿Quieres eliminar el personaje?"


#define	NOTIFY_MSG_HANDLER1	"%s se unio al grupo."
#define	NOTIFY_MSG_HANDLER2	"%s salio del grupo."
#define	NOTIFY_MSG_HANDLER3	"No puedes moverte de esta posicion por %d segundos excepto %s"
#define	NOTIFY_MSG_HANDLER4	"Usando habilidad especial"

#define	NOTIFY_MSG_HANDLER5	"Disminucion del HP en 50%! Dura %d segundos!"
#define	NOTIFY_MSG_HANDLER6	"Congelacion del enemigo! Dura %d segundos!"
#define	NOTIFY_MSG_HANDLER7	"Paralizacion del enemigo! Dura %d segundos!"
#define	NOTIFY_MSG_HANDLER8	"Mata al enemigo de una! Dura %d segundos!"
#define	NOTIFY_MSG_HANDLER9	"Gana HP al mismo tiempo que sacas! Dura %d segundos!"
#define	NOTIFY_MSG_HANDLER10	"Rompimiento de armas enemigas! Dura %d segundos!"
#define	NOTIFY_MSG_HANDLER11	"Proteccion de daños fisicos! Dura %d segundos!"
#define	NOTIFY_MSG_HANDLER12	"Sos intocable por %d segundos!"

#define	NOTIFY_MSG_HANDLER13	"La habilidad especial ha sido preparada!"
#define	NOTIFY_MSG_HANDLER14	"Habilidad que disminuye el HP un 50%: puede ser usada luego de %d minutos"
#define	NOTIFY_MSG_HANDLER15	"Habilidad que congela al enemigo: puede ser usada luego de %d minutos"
#define	NOTIFY_MSG_HANDLER16	"Habilidad que paraliza al enemigo: puede ser usada luego de %d minutos"
#define	NOTIFY_MSG_HANDLER17	"Habilidad que mata a tu enemigo de una vez: puede ser usada luego de %d minutos"
#define	NOTIFY_MSG_HANDLER18	"Habilidad que adquieres HP al mismo tiempo que se la sacas a tu enemigo: puede ser usada luego de %d minutos"
#define	NOTIFY_MSG_HANDLER19	"Habilidad que rompe el arma del enemigo: puede ser usada luego de %d minutos"
#define	NOTIFY_MSG_HANDLER20	"Habilidad que protege del daño fisico: puede ser usada luego de %d minutos"
#define	NOTIFY_MSG_HANDLER21	"Habilidad que hace al personaje intocable: puede ser usada luego de %d minutos"

#define	NOTIFY_MSG_HANDLER22	"Habilidad que disminuye el HP un 50%: puede ser usada luego de %d seg"
#define	NOTIFY_MSG_HANDLER23	"Habilidad que congela al enemigo: puede ser usada luego de %d seg"
#define	NOTIFY_MSG_HANDLER24	"Habilidad que paraliza al enemigo: puede ser usada luego de %d seg"
#define	NOTIFY_MSG_HANDLER25	"Habilidad que mata a tu enemigo de una vez: puede ser usada luego de %d seg"
#define	NOTIFY_MSG_HANDLER26	"Habilidad que adquieres HP al mismo tiempo que se la sacas a tu enemigo: puede ser usada luego de %d seg"
#define	NOTIFY_MSG_HANDLER27	"Habilidad que rompe el arma del enemigo: puede ser usada luego de %d seg"
#define	NOTIFY_MSG_HANDLER28	"Habilidad que protege del daño fisico: puede ser usada luego de %d seg"
#define	NOTIFY_MSG_HANDLER29	"Habilidad que hace al personaje intocable: puede ser usada luego de %d seg"

#define	NOTIFY_MSG_HANDLER30	"La habilidad especial se ha terminado!Estara disponible dentro de 20 minutos!."
#define	NOTIFY_MSG_HANDLER31	"La habilidad especial ha terminado."
#define	NOTIFY_MSG_HANDLER32	"Ahora puedes usar una habilidad especial!"

#define	NOTIFY_MSG_HANDLER33	"Empujaste la esfera de energia hacia el portal enemigo! Tu contribucion disminuyo 10 puntos!"
#define	NOTIFY_MSG_HANDLER34	"%s(Aresden) empujo la esfera de energia al portal enemigo!!!..."
#define	NOTIFY_MSG_HANDLER34_ELV	"%s(Elvine) empujo la esfera de energia al portal enemigo!!!..."
#define	NOTIFY_MSG_HANDLER35	"Felicitaciones! Empujaste la esfera de energia al portal y ganaste Contribucion!"
#define	NOTIFY_MSG_HANDLER36	"Elvine %s : La emboco!"
#define	NOTIFY_MSG_HANDLER37	"Aresden %s : La emboco!"
#define	NOTIFY_MSG_HANDLER38	"La esfera de energia fue dejada en (%d, %d) de Middleland!"
#define	NOTIFY_MSG_HANDLER39	"El jugador que empuje la esfera de energia hacia su portal ganara Contribucion."
#define	NOTIFY_MSG_HANDLER40	"Modo de observacion ENCENCIDO. Presione 'SHIFT + ESC' para salir..."
#define	NOTIFY_MSG_HANDLER41	"Modo de observacion APAGADO."
#define	NOTIFY_MSG_HANDLER42	"Fabricacion del objeto lograda!"
#define	NOTIFY_MSG_HANDLER43	"Fallo al fabricar el objeto."
#define	NOTIFY_MSG_HANDLER44	"Felicitaciones! Completaste la Quest!"
#define	NOTIFY_MSG_HANDLER45	"No puedes poner la bandera aqui."
#define	NOTIFY_MSG_HANDLER46	"Mezclaste correctamente la pocion."
#define	NOTIFY_MSG_HANDLER47	"Fallo al mezclar la pocion."
#define	NOTIFY_MSG_HANDLER48	"Tu nivel de Alchemy es muy bajo para lograr la pocion."
#define	NOTIFY_MSG_HANDLER49	"Ingredientes incorrectos para lograr la pocion."
#define	NOTIFY_MSG_HANDLER50	"Modo de ataque seguro activado."
#define	NOTIFY_MSG_HANDLER51	"Modo de ataque seguro desactivado."
#define	NOTIFY_MSG_HANDLER52	"No puedes seguir pescando porque fuiste molestado!"
#define	NOTIFY_MSG_HANDLER53	"El otro jugador saco el pez primero! Que mala suerte!"
#define	NOTIFY_MSG_HANDLER54	"No puedes pescar mas. El pez se fue."
#define	NOTIFY_MSG_HANDLER55	"Pescaste satisfactoriamente!!!"
#define	NOTIFY_MSG_HANDLER56	"Fallo al pescar..."
#define	NOTIFY_MSG_HANDLER58	"No puedes usar este comando administrativo."
#define	NOTIFY_MSG_HANDLER59	"No tienes derechos para usar este comando de guildmaster"
#define	NOTIFY_MSG_HANDLER60	"Echaste correctamente al miembro."
#define	NOTIFY_MSG_HANDLER61	"No es tu compañero de gremio."
#define	NOTIFY_MSG_HANDLER62	"Se acabo el tiempo, fuiste forzado a volver."
#define	NOTIFY_MSG_HANDLER63	"No puedes depositar mas objetos."
#define	NOTIFY_MSG_HANDLER64	"Los travelers no pueden pasar el nivel 50. Conviertete en ciudadano."
#define	NOTIFY_MSG_HANDLER65	"No puedes llevar mas objetos."
#define	NOTIFY_MSG_HANDLER66	"Tu mochila esta llena."
#define	NOTIFY_MSG_HANDLER67	"No tienes suficiente oro."
#define	NOTIFY_MSG_HANDLER68	"El Modo de vuelta forzosa fue desactivado."
#define	NOTIFY_MSG_HANDLER69	"El Modo de vuelta forzosa fue activado."
#define	NOTIFY_MSG_HANDLER70	"El lide de tu gremio cancelo la reserva."
#define	NOTIFY_MSG_HANDLER71	"Tu reserva fue cancelada por estar fuera de tiempo."
#define	NOTIFY_MSG_HANDLER72	"Adquiriste el tiket de admision."
#define	NOTIFY_MSG_HANDLER73	"Tu reserva de la arena %d fue satisfactoria."
#define	NOTIFY_MSG_HANDLER74	"No puedes volver a tu ciudad si recibiste daño. Espera 10 segundos."
#define	NOTIFY_MSG_HANDLER74B	"No puedes volver a tu ciudad en este mapa."
#define NOTIFY_MSG_HANDLER75	"El deposito del gremio no puede ser cargado."
#define NOTIFY_MSG_HANDLER76	"Vuelve a intentarlo en unos segundos."

#define	RESERVE_FIGHTZONE_RESPONSE_HANDLER1	"Se reservo la arena satisfactoriamente!"
#define	RESERVE_FIGHTZONE_RESPONSE_HANDLER2	"No se puede reservar la arena."


#define	UPDATE_SCREEN_ON_LOG_MSG1	"Cuenta"
#define	UPDATE_SCREEN_ON_LOG_MSG2	"Contraseña"
#define	UPDATE_SCREEN_ON_LOG_MSG3	"Nueva Contraseña"
#define	UPDATE_SCREEN_ON_LOG_MSG4	"Confirmar Contraseña"
#define	UPDATE_SCREEN_ON_LOG_MSG5	"Contraseña incorrecta"
#define	UPDATE_SCREEN_ON_LOG_MSG6	"No existe esa cuenta."
#define	UPDATE_SCREEN_ON_LOG_MSG7	"Revisa la ID de tu cuenta"
#define	UPDATE_SCREEN_ON_LOG_MSG8	"No puedes conectarte con el game server"
#define	UPDATE_SCREEN_ON_LOG_MSG9	"porque el game server no esta disponible."
#define	UPDATE_SCREEN_ON_LOG_MSG10	"Vuelve a intentar mas tarde."
#define	UPDATE_SCREEN_ON_LOG_MSG11	"Nueva cuenta creada!"
#define	UPDATE_SCREEN_ON_LOG_MSG12	"Puedes entrar con tu ID"
#define	UPDATE_SCREEN_ON_LOG_MSG13	"No se puede crear la nueva cuenta."
#define	UPDATE_SCREEN_ON_LOG_MSG14	"Ya existe el nombre de la cuenta."
#define	UPDATE_SCREEN_ON_LOG_MSG15	"Escribe otro nombre para la cuenta."
#define	UPDATE_SCREEN_ON_LOG_MSG16	"Nuevo personaje creado!"
#define	UPDATE_SCREEN_ON_LOG_MSG17	"Imposible crear nuevo personaje!"
#define	UPDATE_SCREEN_ON_LOG_MSG18	"Ya existe el nombre del personaje."
#define	UPDATE_SCREEN_ON_LOG_MSG19	"Escribe otro nombre para el personaje."
#define	UPDATE_SCREEN_ON_LOG_MSG20	"Personaje borrado satisfacotriamente."
#define	UPDATE_SCREEN_ON_LOG_MSG21	"Contraseña cambiada correctamente."
#define	UPDATE_SCREEN_ON_LOG_MSG22	"Imposible cambiar contraseña!"
#define	UPDATE_SCREEN_ON_LOG_MSG23	"No puedes entrar por un error en"
#define	UPDATE_SCREEN_ON_LOG_MSG24	"los archivos. Contacta al GM."

//Didn't translate this, only for premium hb usa system. Aphro~
#define	UPDATE_SCREEN_ON_LOG_MSG25	"Character above trial level can not"
#define	UPDATE_SCREEN_ON_LOG_MSG26	"log in. You need to login in the"
#define	UPDATE_SCREEN_ON_LOG_MSG27	"registered IP or registered account."

#define	UPDATE_SCREEN_ON_LOG_MSG28	"Your registered IP group have exceeded"
#define	UPDATE_SCREEN_ON_LOG_MSG29	"the limit."

#define	UPDATE_SCREEN_ON_LOG_MSG30	"No puedes entrar porque el game server"
#define	UPDATE_SCREEN_ON_LOG_MSG31	"esta desactivado. Intenta mas tarde."

#define	UPDATE_SCREEN_ON_LOG_MSG32	"Cuenta bloqueada."
#define	UPDATE_SCREEN_ON_LOG_MSG33	"Intenta mas tarde."
#define	UPDATE_SCREEN_ON_LOG_MSG34	"No puedes entrar porque la cuenta esta bloqueada."
#define	UPDATE_SCREEN_ON_LOG_MSG35	"Cuenta bloqueada hasta: %dY %dM %dD"
#define	UPDATE_SCREEN_ON_LOG_MSG38	"Cerrando conexion."
#define	UPDATE_SCREEN_ON_LOG_MSG39	"Intenta en unos segundos."
#define	UPDATE_SCREEN_ON_LOG_MSG40	"El world server no esta disponible."
#define	UPDATE_SCREEN_ON_LOG_MSG41	"Intenta mas tarde."
#define	UPDATE_SCREEN_ON_LOG_MSG42	"Maximo de personajes alcanzado."

#define	RETIEVE_ITEM_HANDLER4	"Sacaste %s."


#define	ERASE_ITEM	"El objeto (%s %s %s) : ha sido quitado de [F%d]."
#define	ERASE_ITEM1	"El objet (%s %s %s) : ha sido quitado de [F2]."
#define	ERASE_ITEM2	"El objeto (%s %s %s) : ha sido quitado de [F3]."


#define	BDLBBOX_DOUBLE_CLICK_INVENTORY1	"Puede utilizarlo luego de usar otro objeto."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY3	"El objeto %s: Solo puedes usarlo luego de 20 segundos de haber recibido daño."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY4	"Tus manos deben estar libres para usar el objeto."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY5	"Ya estas usando otra habilidad."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY6	"No puedes usar este objeto debido a que esta roto."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY7	"Objeto %s: Elije la posicion en donde lo quieres usar."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY8	"Objeto %s: Elije el objeto que quieres usar."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY9	"Debes aprender la habilidad de Alchemy para hacer esto..."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY10	"Usando habilidad de Alchemy..."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY11	"Debes aprender la habilidad de Manufacturing para hacer esto..."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY12	"Usando la habilidad de Manufacturing..."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY13	"Ya estas usando otra habilidad."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY14	"Necesitas un Smiths Anvil para manufacturar un objeto. Compralo en Blacksmith."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY15	"Necesitas un Alchemy Bowl para crear el objeto. Compralo en Shop."
#define	BDLBBOX_DOUBLE_CLICK_INVENTORY16	"Debes aprender la habilidad de Alchemy primero. Compra el manual en Shop."


#define	UPDATE_SCREEN_ON_CHANGE_PASSWORD1	"Cuenta"
#define	UPDATE_SCREEN_ON_CHANGE_PASSWORD2	"Contraseña"
#define	UPDATE_SCREEN_ON_CHANGE_PASSWORD3	"Nueva contraseña"
#define	UPDATE_SCREEN_ON_CHANGE_PASSWORD4	"Confirma contraseña"
#define	UPDATE_SCREEN_ON_CHANGE_PASSWORD5	"Escribe la nueva contraseña. Debe ser"
#define	UPDATE_SCREEN_ON_CHANGE_PASSWORD6	"de al menos 8 letras y debe ser"
#define	UPDATE_SCREEN_ON_CHANGE_PASSWORD7	"distinta a la anterior."


#define	DLGBOX_CLICK_SYSMENU1	"Reiniciando el juego....%d"
#define	DLGBOX_CLICK_SYSMENU2	"Desconexion cancelada."

//Didn't translate it, hb basic mob names. Aphro~
#define	NPC_NAME_SLIME				"Slime"
#define	NPC_NAME_SKELETON			"Skeleton"
#define	NPC_NAME_STONEGOLEM			"Stone Golem"
#define	NPC_NAME_CYCLOPS			"Cyclops"
#define	NPC_NAME_ORC				"Orc"
#define	NPC_NAME_SHOP_KEEPER		"Shop Keeper"
#define	NPC_NAME_GIANTANT			"Giant Ant"
#define	NPC_NAME_GIANTSCORPION		"Giant Scorpion"
#define	NPC_NAME_ZOMBIE				"Zombie"
#define	NPC_NAME_MAGICIAN			"Sorcerer"
#define	NPC_NAME_WAREHOUSE_KEEPER	"Warehouse Keeper"
#define	NPC_NAME_GUILD_WAREHOUSE_KEEPER	"Guild Warehouse Keeper"
#define	NPC_NAME_GUARD				"Guard"
#define	NPC_NAME_SNAKE				"Snake"
#define	NPC_NAME_CLAYGOLEM			"Clay Golem"
#define	NPC_NAME_BLACKSMITH_KEEPER	"BlackSmith Keeper"
#define	NPC_NAME_CITYHALL_OFFICER	"Cityhall Officer"
#define	NPC_NAME_GUILDHALL_OFFICER	"Guildhall Officer"
#define	NPC_NAME_CMDHALL_OFFICER	"Heldenian staff officer"
#define	NPC_NAME_HELHOUND			"Helhound"
#define	NPC_NAME_TROLL				"Troll"
#define	NPC_NAME_OGRE				"Ogre"
#define	NPC_NAME_LICHE				"Liche"
#define	NPC_NAME_DEMON				"Demon"
#define	NPC_NAME_UNICORN			"Unicorn"
#define	NPC_NAME_WEREWOLF			"Werewolf"
#define	NPC_NAME_DUMMY				"Dummy"
#define	NPC_NAME_RABBIT				"Rabbit"
#define	NPC_NAME_CAT				"Cat"
#define NPC_NAME_FROG				"Frog"
#define NPC_NAME_ETTIN			"Ettin"
#define NPC_NAME_MOUNTAIN_GIANT	"Mountain Giant"
#define NPC_NAME_CANNIBAL		"Plant"
#define NPC_NAME_RUDOLPH		"Rudolph"
#define NPC_NAME_DIREBOAR		"Direboar"
#define NPC_NAME_FROST			"Frost"
#define NPC_NAME_ICEGOLEM		"Ice Golem"
#define NPC_NAME_WYVERN			"Wyvern"
#define NPC_NAME_MCGAFFIN		"Mcgaffin"
#define NPC_NAME_PERRY			"Perry"
#define NPC_NAME_DEVLIN			"Devlin"
#define NPC_NAME_WATERMELON		"WaterMelon"
#define NPC_NAME_PUMPKIN		"Pumpkin"
#define NPC_NAME_GARLIC			"Garlic"
#define NPC_NAME_BARLEY			"Barley"
#define NPC_NAME_CARROT			"Carrot"
#define NPC_NAME_RADISH			"Radish"
//Snoopy:
#define	NPC_NAME_CORN			"Corn"
#define	NPC_NAME_BFLOWER		"Chinese Bell Flower"
#define	NPC_NAME_MELON			"Melone"
#define	NPC_NAME_TOMATO			"Tomato"
#define	NPC_NAME_GRAPPE			"Grapes"
#define	NPC_NAME_BLUEGRAPPE		"Blue Grape"
#define	NPC_NAME_MUSHROM		"Mushroom"
#define	NPC_NAME_GINSENG		"Ginseng"
#define NPC_NAME_CROP			"Crop"



#define	NPC_NAME_ENERGYSPHERE			"Energy Sphere"
#define	NPC_NAME_ARROWGUARDTOWER_CK		"Arrow Guard Tower Kit"
#define	NPC_NAME_ARROWGUARDTOWER		"Arrow Guard Tower"
#define	NPC_NAME_CANNONGUARDTOWER_CK	"Cannon Guard Tower Kit"
#define	NPC_NAME_CANNONGUARDTOWER		"Cannon Guard Tower"
#define	NPC_NAME_MANACOLLECTOR_CK		"Mana Collector Kit"
#define	NPC_NAME_MANACOLLECTOR			"Mana Collector"
#define	NPC_NAME_DETECTOR_CK			"Detector Kit"
#define	NPC_NAME_DETECTOR				"Detector"
#define	NPC_NAME_ENERGYSHIELD			"Energy Shield Generator"
#define	NPC_NAME_ASTORIASHIELD			"Astoria Shield"
#define	NPC_NAME_GRANDMAGICGENERATOR	"Grand Magic Generator"
#define	NPC_NAME_MANASTONE				"Mana Stone"
#define	NPC_NAME_LIGHTWARBEETLE			"Light War Beetle"
#define	NPC_NAME_GODSHANDKNIGHT			"God's Hand Knight"
#define	NPC_NAME_GODSHANDKNIGHT_CK		"God's Hand Knight"
#define	NPC_NAME_TEMPLEKNIGHT			"Temple Knight"
#define	NPC_NAME_BATTLEGOLEM			"Battle Golem"
#define	NPC_NAME_STALKER				"Stalker"
#define	NPC_NAME_HELLCLAW				"Helclaw"
#define	NPC_NAME_TIGERWORM				"Tiger Worm"
#define	NPC_NAME_CATAPULT				"Catapult"
#define	NPC_NAME_GARGOYLE				"Gargoyle"
#define	NPC_NAME_BEHOLDER				"Beholder"
#define	NPC_NAME_DARKELF				"Darkelf"

//Snoopy: New mob's names
#define	 NPC_NAME_DRAGON		"Dragon"
#define	 NPC_NAME_CENTAUR		"Centaur"
#define	 NPC_NAME_CLAWTUR		"Claw Turtle"
#define	 NPC_NAME_FIREWYV		"Fire Wyvern"
#define	 NPC_NAME_GICRAYF		"Giant Crayfish"
#define	 NPC_NAME_GILIZAR		"Giant Lizard"
#define  NPC_NAME_GITREE		"Giant Tree"
#define	 NPC_NAME_MASTORC		"Master Mage Orc"
#define	 NPC_NAME_MINAUS		"Minotaur"
#define	 NPC_NAME_NIZIE			"Nizie"
#define NPC_NAME_TENTOCL		"Tentocle"
#define NPC_NAME_ABADDON		"Abaddon"
#define NPC_NAME_SORCERS		"Sorceress"
#define NPC_NAME_ATK			"Ancient Temple Knight"
#define NPC_NAME_MASTELF		"Elf Master"
#define NPC_NAME_DSK			"Dark Shadow Knight"
#define NPC_NAME_HBT			"Heavy Battle Tank"
#define NPC_NAME_CT				"Crossbow Turret"
#define NPC_NAME_BARBAR			"Barbarian"
#define NPC_NAME_AGC			"Ancient Giant Cannon"
#define NPC_NAME_GAIL			"Gail"
#define NPC_NAME_GATE			"Gate"

// CLEROTH - NEW MONSTERS
#define NPC_NAME_AIRLEMENTAL			"Air Elemental"

#define	DRAW_OBJECT_NAME50	" Berserked"
#define	DRAW_OBJECT_NAME51	" Frozen"
#define	DRAW_OBJECT_NAME52	"Clairvoyant"
#define	DRAW_OBJECT_NAME53	"Destruction of Magic Protection"
#define	DRAW_OBJECT_NAME54	"Anti-Physical Damage"
#define	DRAW_OBJECT_NAME55	"Anti-Magic Damage"
#define	DRAW_OBJECT_NAME56	"Poisonous"
#define	DRAW_OBJECT_NAME57	"Critical Poisonous"
#define	DRAW_OBJECT_NAME58	"Explosive"
#define	DRAW_OBJECT_NAME59	"Critical Explosive"
#define	DRAW_OBJECT_NAME59B	"Highly Trained"
#define	DRAW_OBJECT_NAME60	"Traveller"
#define	DRAW_OBJECT_NAME61	"Traveller,"
#define	DRAW_OBJECT_NAME62	"Aresden"
#define	DRAW_OBJECT_NAME63	"Citizen of Aresden,"
#define	DRAW_OBJECT_NAME64	"Aresden"
#define	DRAW_OBJECT_NAME65	"Criminal of Aresden,"
#define	DRAW_OBJECT_NAME66	"Your Guildsman,"
#define	DRAW_OBJECT_NAME67	"Aresden Guildsman"
#define	DRAW_OBJECT_NAME68	"Aresden Guildsman,"
#define	DRAW_OBJECT_NAME69	"Your Guildmaster,"
#define	DRAW_OBJECT_NAME70	"Aresden Guildmaster"
#define	DRAW_OBJECT_NAME71	"Aresden Guildsman"
#define	DRAW_OBJECT_NAME72	"Aresden Guildmaster"
#define	DRAW_OBJECT_NAME73	"Aresden Guildmaster,"
#define	DRAW_OBJECT_NAME74	"Elvine"
#define	DRAW_OBJECT_NAME75	"Citizen of Elvine,"
#define	DRAW_OBJECT_NAME76	"Elvine"
#define	DRAW_OBJECT_NAME77	"Criminal of Elvine,"
#define	DRAW_OBJECT_NAME78	"Your Guildsman,"
#define	DRAW_OBJECT_NAME79	"Elvine Guildsman"
#define	DRAW_OBJECT_NAME80	"Elvine Guildsman,"
#define	DRAW_OBJECT_NAME81	"Your Guildmaster,"
#define	DRAW_OBJECT_NAME82	"Elvine Guildmaster"
#define	DRAW_OBJECT_NAME83	"Elvine Guildsman"
#define	DRAW_OBJECT_NAME84	"Elvine Guildmaster"
#define	DRAW_OBJECT_NAME85	"Elvine Guildmaster,"
#define	DRAW_OBJECT_NAME86	"Criminal,"
#define	DRAW_OBJECT_NAME87	"Unknown"
#define	DRAW_OBJECT_NAME88	"Neutral"
#define	DRAW_OBJECT_NAME89	"(Friendly)"
#define	DRAW_OBJECT_NAME90	"(Enemy)"
#define	DRAW_OBJECT_NAME91	"(Threat)"



#define	UPDATE_SCREEN_ON_VERSION_NO_MATCH1	"La version de tu cliente no coincide con la del servidor!"
#define	UPDATE_SCREEN_ON_VERSION_NO_MATCH2	"Ejecuta el programa \"HelbreathXtreme.exe\" y vuelve a intentarlo."


#define	DRAW_VERSION1	"Version para administradores."


#define	USE_RED_POTION1	"Puedes usarlo luego de utilizar otro objeto."
#define	USE_RED_POTION2	"No puedes usar pociones mientras intercambias."


#define	USE_BLUE_POTION1	"Puedes usarlo luego de utilizar otro objeto."
#define	USE_BLUE_POTION2	"No puedes usar pociones mientras intercambias."

#define	GET_OFFICIAL_MAP_NAME1	"Aresden Farm"
#define	GET_OFFICIAL_MAP_NAME2	"Elvine Farm"
#define	GET_OFFICIAL_MAP_NAME3	"Beginner Zone"
#define	GET_OFFICIAL_MAP_NAME4	"Aresden Barrack 1"
#define	GET_OFFICIAL_MAP_NAME5	"Aresden Barrack 1"
#define	GET_OFFICIAL_MAP_NAME6	"Aresden Barrack 2"
#define	GET_OFFICIAL_MAP_NAME7	"Aresden Barrack 2"
#define	GET_OFFICIAL_MAP_NAME8	"Elvine Barrack 1"
#define	GET_OFFICIAL_MAP_NAME9	"Elvine Barrack 1"
#define	GET_OFFICIAL_MAP_NAME10	"Elvine Barrack 2"
#define	GET_OFFICIAL_MAP_NAME11	"Elvine Barrack 2"
#define	GET_OFFICIAL_MAP_NAME12	"Arena 1"
#define	GET_OFFICIAL_MAP_NAME13	"Arena 2"
#define	GET_OFFICIAL_MAP_NAME14	"Arena 3"
#define	GET_OFFICIAL_MAP_NAME15	"Arena 4"
#define	GET_OFFICIAL_MAP_NAME16	"Arena 5"
#define	GET_OFFICIAL_MAP_NAME17	"Arena 6"
#define	GET_OFFICIAL_MAP_NAME18	"Arena 7"
#define	GET_OFFICIAL_MAP_NAME19	"Arena 8"
#define	GET_OFFICIAL_MAP_NAME20	"Arena 9"
#define	GET_OFFICIAL_MAP_NAME21	"Aresden Dungeon"
#define	GET_OFFICIAL_MAP_NAME22	"Aresden City"
#define	GET_OFFICIAL_MAP_NAME23	"Elvine Dungeon"
#define	GET_OFFICIAL_MAP_NAME24	"Elvine City"
#define	GET_OFFICIAL_MAP_NAME25	"Dungeon L2"
#define	GET_OFFICIAL_MAP_NAME26	"Dungeon L3"
#define	GET_OFFICIAL_MAP_NAME27	"Bleeding Island"
#define	GET_OFFICIAL_MAP_NAME28	"Middleland"
#define	GET_OFFICIAL_MAP_NAME29	"Rocky Highland"
#define	GET_OFFICIAL_MAP_NAME30	"Eternal Field"
#define	GET_OFFICIAL_MAP_NAME31	"Death Valley"
#define	GET_OFFICIAL_MAP_NAME32	"Silent Wood"
#define	GET_OFFICIAL_MAP_NAME33	"Aresden Blacksmith"
#define	GET_OFFICIAL_MAP_NAME34	"Elvine Blacksmith"
#define	GET_OFFICIAL_MAP_NAME35	"Aresden Cityhall"
#define	GET_OFFICIAL_MAP_NAME36	"Elvine Cityhall"
#define	GET_OFFICIAL_MAP_NAME37	"Aresden Guildhall"
#define	GET_OFFICIAL_MAP_NAME38	"Elvine Guildhall"
#define	GET_OFFICIAL_MAP_NAME39	"Aresden Shop"
#define	GET_OFFICIAL_MAP_NAME40	"Elvine Shop"
#define	GET_OFFICIAL_MAP_NAME41	"Magic Tower"
#define	GET_OFFICIAL_MAP_NAME42	"Magic Tower"
#define	GET_OFFICIAL_MAP_NAME43	"Aresden Town Hall"
#define	GET_OFFICIAL_MAP_NAME44	"Elvine Town Hall"
#define	GET_OFFICIAL_MAP_NAME45	"Aresden Warehouse"
#define	GET_OFFICIAL_MAP_NAME46	"Elvine Warehouse"
#define	GET_OFFICIAL_MAP_NAME47	"Aresien Church"
#define	GET_OFFICIAL_MAP_NAME48	"Eldiniel Church"
#define	GET_OFFICIAL_MAP_NAME49	"Aresden-Middle DMZ"
#define	GET_OFFICIAL_MAP_NAME50	"Aresden-Middle DMZ"
#define	GET_OFFICIAL_MAP_NAME51	"Elvine-Middle DMZ"
#define	GET_OFFICIAL_MAP_NAME52	"Elvine-Middle DMZ"
#define	GET_OFFICIAL_MAP_NAME53	"Dungeon L4"
#define	GET_OFFICIAL_MAP_NAME54	"Revival Zone"
#define	GET_OFFICIAL_MAP_NAME55	"Revival Zone"
#define	GET_OFFICIAL_MAP_NAME56	"Aresien Garden"
#define	GET_OFFICIAL_MAP_NAME57	"Eldiniel Garden"
#define	GET_OFFICIAL_MAP_NAME58	"Middleland Mine"
#define	GET_OFFICIAL_MAP_NAME59	"Middleland Dungeon"
#define	GET_OFFICIAL_MAP_NAME60	"Tower of Hell 1"
#define	GET_OFFICIAL_MAP_NAME61	"Tower of Hell 2"
#define	GET_OFFICIAL_MAP_NAME62	"Tower of Hell 3"
#define GET_OFFICIAL_MAP_NAME63	"Aresden Jail"
#define GET_OFFICIAL_MAP_NAME64	"Elvine Jail"
#define GET_OFFICIAL_MAP_NAME65	"Promiseland"
#define GET_OFFICIAL_MAP_NAME66	"Ice Map"
//Snoopy:
#define GET_OFFICIAL_MAP_NAME70	"Druncnian City"
#define GET_OFFICIAL_MAP_NAME71	"Infernia A"
#define GET_OFFICIAL_MAP_NAME72	"Infernia B"
#define GET_OFFICIAL_MAP_NAME73	"Maze"
#define GET_OFFICIAL_MAP_NAME74	"Procella"
#define GET_OFFICIAL_MAP_NAME75	"Abaddon"
#define GET_OFFICIAL_MAP_NAME76	"Battle Field"
#define GET_OFFICIAL_MAP_NAME77	"God's Heldenian"
#define GET_OFFICIAL_MAP_NAME78	"Heldenian Rampart"
#define GET_OFFICIAL_MAP_NAME79	"Command Hall"
#define GET_OFFICIAL_MAP_NAME80	"Barracks Level 1"
#define GET_OFFICIAL_MAP_NAME81	"Barracks Level 2"
#define GET_OFFICIAL_MAP_NAME82	"Begginers Blacksmith"
#define GET_OFFICIAL_MAP_NAME83	"Unknown"
#define GET_OFFICIAL_MAP_NAME84 "Astoria"
// Fantasy Maps xRisenx
#define GET_OFFICIAL_MAP_NAME85 "Safe Shop"
#define GET_OFFICIAL_MAP_NAME86 "PvP Barrack"
#define GET_OFFICIAL_MAP_NAME87 "Dark Crypt"

#define	BCHECK_LOCAL_CHAT_COMMAND1	"El personaje %s ha sido quitado de la lista de usuarios ignorados."
#define	BCHECK_LOCAL_CHAT_COMMAND2	"No puedes ignorar tus propios mensajes."
#define	BCHECK_LOCAL_CHAT_COMMAND3	"El personaje %s ha sido agregado a la lista de usuarios ignorados."
#define	BCHECK_LOCAL_CHAT_COMMAND4	"No hay espacio para agregar mas personajes a tu lista de usuarios ignorados."
#define	BCHECK_LOCAL_CHAT_COMMAND5	"El nombre del personaje que quieres ignorar es muy largo."
#define	BCHECK_LOCAL_CHAT_COMMAND6	"Posibilitado a escuchar conversaciones privadas."
#define	BCHECK_LOCAL_CHAT_COMMAND7	"Inposibilitado a escuchar conversaciones privadas."
#define	BCHECK_LOCAL_CHAT_COMMAND8	"Posibilitado a escribir en conversaciones publicas."
#define	BCHECK_LOCAL_CHAT_COMMAND9	"Inposibilitado a escribir en conversaciones publicas."
#define	BCHECK_LOCAL_CHAT_COMMAND10	"La marca del tiempo en los mensajes ha sido activada."
#define	BCHECK_LOCAL_CHAT_COMMAND11	"La marca del tiempo en los mensajes ha sido desactivada."
#define	BCHECK_LOCAL_CHAT_COMMAND12	"El daño es ahora mostrado por grupos."
#define	BCHECK_LOCAL_CHAT_COMMAND13	"El daño es ahora mostrado uno despues de otro."
#define	BCHECK_LOCAL_CHAT_COMMAND14	"Los objetos del piso ahora son mostrados grandes."
#define	BCHECK_LOCAL_CHAT_COMMAND15	"Los objetos del piso ahora son mostrados normales."
#define	BCHECK_LOCAL_CHAT_COMMAND16	"La auto-foto esta ahora activada cada vez que mates a un enemigo."
#define	BCHECK_LOCAL_CHAT_COMMAND17	"La auto-foto esta ahora desactivada."
#define	BCHECK_LOCAL_CHAT_COMMAND18	"Ahora seras notificado fuera del juego cuando te nombren."
#define	BCHECK_LOCAL_CHAT_COMMAND19	"Ya no seras notificado fuera del juego cuando te nombren."
#define	BCHECK_LOCAL_CHAT_COMMAND20	"Los objetos seran seleccionados automaticamente durante el proceso de manufacutra."
#define	BCHECK_LOCAL_CHAT_COMMAND21	"Los objetos deberan ser seleccionados manual durante el proceso de manufacutra."


#define	BCHECK_ITEM_OPERATION_ENABLE1	"La transaccion de objetos no ha finalizado aun."
#define	BCHECK_ITEM_OPERATION_ENABLE2	"La transaccion de objetos no ha finalizado aun."
#define	BCHECK_ITEM_OPERATION_ENABLE3	"La transaccion de objetos no ha finalizado aun."
#define	BCHECK_ITEM_OPERATION_ENABLE4	"La transaccion de objetos no ha finalizado aun."
#define	BCHECK_ITEM_OPERATION_ENABLE5	"La transaccion de objetos no ha finalizado aun."
#define	BCHECK_ITEM_OPERATION_ENABLE6	"La transaccion de objetos no ha finalizado aun."


#define	CLEAR_SKILL_USING_STATUS1	"Habilidad dejada de usar."


#define	NPC_TALK_HANDLER16	"Objetivo: mata %d %s"
#define	NPC_TALK_HANDLER17	"Ubicacion: Cualquier"
#define	NPC_TALK_HANDLER18	"Mapa: %s"
#define	NPC_TALK_HANDLER19	"Posición: %d,%d entre %d cuadrados"
#define	NPC_TALK_HANDLER20	"Contribución:% Puntos"
#define	NPC_TALK_HANDLER21	"Objective : Observa al enemigo."
#define	NPC_TALK_HANDLER22	"Ubicación : Cualquier"
#define	NPC_TALK_HANDLER23	"Mapa : %s"
#define	NPC_TALK_HANDLER24	"Posición: %d,%d entre %d cuadrados"
#define	NPC_TALK_HANDLER25	"Contribución: %d puntos"
#define	NPC_TALK_HANDLER26	"Plan de operacion: Invasion al enemigo"
#define	NPC_TALK_HANDLER27	"Teletransporte directo al lugar enemigo"
#define	NPC_TALK_HANDLER28	"e intenta destruir las mayores facilidades"
#define	NPC_TALK_HANDLER29	"o intenta crear una tactia de guerilla"
#define	NPC_TALK_HANDLER30	"para obstruir sus acciones."
#define	NPC_TALK_HANDLER31	"Lugar de teletransporte: presupuesto desconocido"
#define	NPC_TALK_HANDLER32	"Lugar de teletransporte: %s"
#define	NPC_TALK_HANDLER33	"Te agradeceria si lo logras."
#define	NPC_TALK_HANDLER34	"Entonces, ¿Te animas?"
#define	NPC_TALK_HANDLER35	"¿Haras esta Misión?"


#define	NPC_NAME_MERCENARY	"Mercenario"
#define BGET_NPC_NAME23	", Miembro del grupo"

//Didn't translate, hb basic item names. Aphro~
#define	GET_ITEM_NAME1	"Pureza: %d%%"
#define	GET_ITEM_NAME2	"Completion: %d%%"
#define	GET_ITEM_NAME3	"Critical "
#define	GET_ITEM_NAME4	"Poisoning "
#define	GET_ITEM_NAME5	"Righteous "
#define	GET_ITEM_NAME6	"Agile "
#define	GET_ITEM_NAME7	"Light "
#define	GET_ITEM_NAME8	"Sharp "
#define	GET_ITEM_NAME9	"Strong "
#define	GET_ITEM_NAME10	"Ancient "
#define	GET_ITEM_NAME11	"Special "
#define	GET_ITEM_NAME12	"Mana Converting "
#define	GET_ITEM_NAME13	"Critical "
#define	GET_ITEM_NAME14	"Critical Hit Damage+%d"
#define	GET_ITEM_NAME15	"Poison Damage+%d"
#define	GET_ITEM_NAME16	"Attack Speed -1"
#define	GET_ITEM_NAME17	"%d%% light"
#define	GET_ITEM_NAME18	"Damage added"
#define	GET_ITEM_NAME19	"Endurance +%d%%"
#define	GET_ITEM_NAME20	"Extra Damage added"
#define	GET_ITEM_NAME21	"Magic Casting Probability+%d%%"
#define	GET_ITEM_NAME22	"Replace %d%% damage to mana"
#define	GET_ITEM_NAME23	"Crit Increase Chance %d%%"
#define	GET_ITEM_NAME24	"Poison Resistance+%d%%"
#define	GET_ITEM_NAME25	"Hitting Probability+%d"
#define	GET_ITEM_NAME26	"Defense Ratio+%d"
#define	GET_ITEM_NAME27	"HP recovery %d%%"
#define	GET_ITEM_NAME28	"SP recovery %d%%"
#define	GET_ITEM_NAME29	"MP recovery %d%%"
#define	GET_ITEM_NAME30	"Magic Resistance+%d%%"
#define	GET_ITEM_NAME31	"Physical Absorption+%d%%"
#define	GET_ITEM_NAME32	"Magic Absorption+%d%%"
#define	GET_ITEM_NAME33	"Consecutive Attack Damage+%d"
#define	GET_ITEM_NAME34	"Experience+%d%%"
#define	GET_ITEM_NAME35	"Gold+%d%%"
#define	GET_ITEM_NAME36	"Endurance %d/3"
#define	GET_ITEM_NAME37	"Uselo en un objeto 'Socketed' para darle atributos."
#define	GET_ITEM_NAME38	"Uselo en un objeto 'Non-Socketed' para darle un 'Socket'"
// Flawless Weapons xRisenx
//#define	GET_ITEM_NAME40	"Flawless Sharp"
//#define	GET_ITEM_NAME41	"Flawless Ancient"

#define	LNG_DAMAGE			"Daño"
#define	LNG_MAGICDAMAGE	"Daño magico"


#define	CALC_SOCKET_CLOSED1	"Game exits because paying sockect disconnected."//Usage?? Didn't translate. Aphro~


#define	POINT_COMMAND_HANDLER1	"No se selecciono ningun personaje para unir al grupo. No se ha podido crear un grupo."

#define	UPDATE_SCREEN_ONGAME1	"No puedes tirar o pasar objetos antes de que termine la transacción."
#define	UPDATE_SCREEN_ONGAME2	"No puedes tirar o pasar objetos antes de que termine la transacción."
#define	UPDATE_SCREEN_ONGAME3	"No puedes tirar o pasar objetos antes de que termine la transacción."
#define	UPDATE_SCREEN_ONGAME4	"No puedes tirar o pasar objetos antes de que termine la transacción."
#define	UPDATE_SCREEN_ONGAME5	"No puedes darte objetos a vos mismo."
#define	UPDATE_SCREEN_ONGAME6	"No puedes ver mas de 12 objetos al mismo tiempo."
#define	UPDATE_SCREEN_ONGAME7	"Demasiado lejos para dar el objeto."
#define	UPDATE_SCREEN_ONGAME8	"La cantidad que introduciste es incorrecta."
#define	UPDATE_SCREEN_ONGAME9	"Introduciste mayor cantidad de la que puedes cargar."
#define	UPDATE_SCREEN_ONGAME10	"Dureza: %d"
#define	UPDATE_SCREEN_ONGAME12	"Presione F1 para ayuda."
#define	UPDATE_SCREEN_ONGAME13	"Cerrando conexion...%d"
#define	UPDATE_SCREEN_ONGAME14	"Reiniciando el juego...%d"
#define	UPDATE_SCREEN_ONGAME15	"Usos: %d"

#define	MOTION_RESPONSE_HANDLER2	"La cuenta regresiva para salir del servidor se cancela si el HP disminuye."


#define	COMMAND_PROCESSOR1	"Cancelado!"
#define	COMMAND_PROCESSOR2	"Network Traffic/Lag. Por favor espere un momento!"
#define	COMMAND_PROCESSOR3	"Network Traffic/Lag. Por favor espere un momento!"
#define	COMMAND_PROCESSOR4	"Network Traffic/Lag. Por favor espere un momento!"
#define	COMMAND_PROCESSOR5	"Network Traffic/Lag. Por favor espere un momento!"
#define	COMMAND_PROCESSOR6	"Critico!"
#define	COMMAND_PROCESSOR7	"La cuenta regresiva para salir del servidor se cancelo porque te moviste."

#define	DRAW_DIALOGBOX_BANK1	"Volviendo a tomar los objetos."
#define	DRAW_DIALOGBOX_BANK2	"Por favor espere a que termine el proceso."

#define	DRAW_DIALOGBOX_BANK3	"Mueva un objeto de su mochila"
#define	DRAW_DIALOGBOX_BANK4	"hacia el banco para depositarlo."
#define	DRAW_DIALOGBOX_BANK5	"Clickee en el objeto para retirarlo."
#define	DRAW_DIALOGBOX_BANK6	"Espacio usado: %d/%d"

#define	DRAW_DIALOGBOX_CHARACTER1	"Criminal (%d)"
#define	DRAW_DIALOGBOX_CHARACTER2	"Contribución (%d) "
#define	DRAW_DIALOGBOX_CHARACTER3	"Aresden %s Jefe del Gremio"
#define	DRAW_DIALOGBOX_CHARACTER4	"Elvine %s Jefe del Gremio"
#define	DRAW_DIALOGBOX_CHARACTER5	"Aresden %s Miembro del Gremio"
#define	DRAW_DIALOGBOX_CHARACTER6	"Elvine %s Miembro del Gremio"
#define	DRAW_DIALOGBOX_CHARACTER7	"Viajero"
#define	DRAW_DIALOGBOX_CHARACTER8	"Ciudadano de Aresden"
#define	DRAW_DIALOGBOX_CHARACTER9	"Ciudadano de Elvine"
#define	DRAW_DIALOGBOX_CHARACTER10	"Majestics (%d)"


#define	DRAW_DIALOGBOX_LEVELUP_SETTING1	"Cuando subes de nivel,"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING2	"debes subir tus estadisticas."
#define	DRAW_DIALOGBOX_LEVELUP_SETTING3	"* Puntos restantes:"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING4	"Fuerza"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING5	"Vitalidad"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING6	"Destreza"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING7	"Inteligencia"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING8	"Magia"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING9	"Carisma"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING10	"Al subir de nivel, tus estadistica(s)"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING11	"seran incrementadas por esta configuracion."
#define	DRAW_DIALOGBOX_LEVELUP_SETTING12	"Presiona el boton OK cuando hayas"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING13	"terminado la configuracion"
//#define	DRAW_DIALOGBOX_LEVELUP_SETTING14	"Level up setting is not finished yet."
//#define	DRAW_DIALOGBOX_LEVELUP_SETTING15	"Current level up setting is incorrect."
//#define	DRAW_DIALOGBOX_LEVELUP_SETTING16	"Because when you level up, your specific"
//#define	DRAW_DIALOGBOX_LEVELUP_SETTING17	"stat(s) will be over the limit"


// MJ Stats Change - Diuuude
#define NOTIFY_MSG_STATECHANGE_SUCCESS		"Las estadisticas han sido cambiadas."
#define NOTIFY_MSG_STATECHANGE_FAILED			"Las estadisticas NO han sido cambiadas."
#define	DRAW_DIALOGBOX_LEVELUP_SETTING14			"Puedes cambiar 3 puntos de estadisticas"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING15			"por cada Majestic Point ."
#define	DRAW_DIALOGBOX_LEVELUP_SETTING16			"* Majestic Points Restantes"
#define	DRAW_DIALOGBOX_LEVELUP_SETTING17			"Las estadisticas han sido cambiadas usando Majestic Points."


#define	DRAW_DIALOGBOX_CITYHALL_MENU1	"Pedido de ciudadania."
#define	DRAW_DIALOGBOX_CITYHALL_MENU4	"Tomar el oro del premio."


// 3.51 Cityhall Menu - Diuuude
#define	DRAW_DIALOGBOX_CITYHALL_MENU8	"Tomar los items de HEROE"

//#define	DRAW_DIALOGBOX_CITYHALL_MENU8	"Take the HERO's Items."
#define	DRAW_DIALOGBOX_CITYHALL_MENU10	"Cancelar la Misión."
#define	DRAW_DIALOGBOX_CITYHALL_MENU11	"Xtreme puntos servicios."
#define	DRAW_DIALOGBOX_CITYHALL_MENU12	"Teletransporte Lista."
#define	DRAW_DIALOGBOX_CITYHALL_MENU13	"Cambiar mi deber en la crusada."
#define	DRAW_DIALOGBOX_CITYHALL_MENU14	"Actualizar"
#define	DRAW_DIALOGBOX_CITYHALL_MENU15	"Nombre"
#define	DRAW_DIALOGBOX_CITYHALL_MENU16	"Precio"
#define	DRAW_DIALOGBOX_CITYHALL_MENU17	"Elije el objeto deseado."

#define	DRAW_DIALOGBOX_CITYHALL_MENU18	"Si te conviertes en ciudadano, el nivel de"
#define	DRAW_DIALOGBOX_CITYHALL_MENU19	"restriccion de viajero es removido."
#define	DRAW_DIALOGBOX_CITYHALL_MENU20	"Podes comprar y vender casi todo tipo de objetos"
#define	DRAW_DIALOGBOX_CITYHALL_MENU21	"producidos por la ciudad. Tambien hechando"
#define	DRAW_DIALOGBOX_CITYHALL_MENU22	"enemigos de la otra ciudad puedes ganar"
#define	DRAW_DIALOGBOX_CITYHALL_MENU23	"oro como premio. Tambien podras"
#define	DRAW_DIALOGBOX_CITYHALL_MENU24	"entrar en gremios."
#define	DRAW_DIALOGBOX_CITYHALL_MENU25	"¿Cumpliras el deber"
#define	DRAW_DIALOGBOX_CITYHALL_MENU26	"y la responsabilidad "
#define	DRAW_DIALOGBOX_CITYHALL_MENU27	"como ciudadano de tu pueblo?"

#define	DRAW_DIALOGBOX_CITYHALL_MENU28	"Pidiendo ciudadania..."
#define	DRAW_DIALOGBOX_CITYHALL_MENU29	"Felicitaciones!! Adquiriste la ciudadania!"
#define	DRAW_DIALOGBOX_CITYHALL_MENU30	"Fallo al pedir la ciudadania."

#define	DRAW_DIALOGBOX_CITYHALL_MENU31	"Niveles bajos o personajes criminales"
#define	DRAW_DIALOGBOX_CITYHALL_MENU32	"no pueden ser ciudadanos."

#define	DRAW_DIALOGBOX_CITYHALL_MENU33	"El precio en oro por tu actividad"
#define	DRAW_DIALOGBOX_CITYHALL_MENU34	"es de %d. ¿Deseas recibir"
#define	DRAW_DIALOGBOX_CITYHALL_MENU35	"este oro ahora?"

// 3.51 Cityhall Menu - Request Hero's Items - Diuuude
#define	DRAW_DIALOGBOX_CITYHALL_MENU46	"Aqui estan los objetos de HEROE disponibles:"
#define	DRAW_DIALOGBOX_CITYHALL_MENU47	"Capa de Heroe (EK 300)"
#define	DRAW_DIALOGBOX_CITYHALL_MENU48	"Casco de Heroe (EK 150 - Contrib 20)"
#define	DRAW_DIALOGBOX_CITYHALL_MENU49	"Bonete de Heroe (EK 100 - Contrib 20)"
#define	DRAW_DIALOGBOX_CITYHALL_MENU50	"Armadura de Heroe (EK 300 - Contrib 30)"
#define	DRAW_DIALOGBOX_CITYHALL_MENU51	"Tunica de Heroe (EK 200 - Contrib 20)"
#define	DRAW_DIALOGBOX_CITYHALL_MENU52	"Cota de malla de Heroe (EK 100 - Contrib 10)"
#define	DRAW_DIALOGBOX_CITYHALL_MENU53	"Piernas de Heroe (EK 150 - Contrib 15)"

#define	DRAW_DIALOGBOX_CITYHALL_MENU54	"¿Quieres cancelar"
#define	DRAW_DIALOGBOX_CITYHALL_MENU55	"la Misión actual?"


#define	DRAW_DIALOGBOX_EXCHANGE1	"Mi objeto"
#define	DRAW_DIALOGBOX_EXCHANGE2	"Cantidad: %d"
#define	DRAW_DIALOGBOX_EXCHANGE3	"Dureza: %d/%d"
//#define	DRAW_DIALOGBOX_EXCHANGE4	"Completion: %d%%"
#define	DRAW_DIALOGBOX_EXCHANGE5	"Objetos de %s."

#define	DRAW_DIALOGBOX_EXCHANGE9	"Por favor espere mientras el otro jugador"
#define	DRAW_DIALOGBOX_EXCHANGE10	"decide. Si quieres cancelar el intercambio,"
#define	DRAW_DIALOGBOX_EXCHANGE11	"presiona el boton CANCEL ahora."

#define	DRAW_DIALOGBOX_EXCHANGE12	"Otro jugador ofreci algo para intercambiar"
#define	DRAW_DIALOGBOX_EXCHANGE13	"Elije un objeto que quieres intercambiar"
#define	DRAW_DIALOGBOX_EXCHANGE14	"y llevalo al espacio en blanco."
#define	DRAW_DIALOGBOX_EXCHANGE15	"Luego presiona el boton de EXCHANGE."
#define	DRAW_DIALOGBOX_EXCHANGE16	"Tambien puedes rechazarlo presionando"
#define	DRAW_DIALOGBOX_EXCHANGE17	"el boton CANCEL"

#define	DRAW_DIALOGBOX_EXCHANGE18	"La preparacion para el intercambio"
#define	DRAW_DIALOGBOX_EXCHANGE19	"termino. Presiona el boton EXCHANGE"
#define	DRAW_DIALOGBOX_EXCHANGE20	"para intercambiar. Presiona el"
#define	DRAW_DIALOGBOX_EXCHANGE21	"boton CANCEL para cancelar. Casi"
#define	DRAW_DIALOGBOX_EXCHANGE22	"siempre cuando apretas el boton"
#define	DRAW_DIALOGBOX_EXCHANGE23	"EXCHANGE no hay vuelta atras."

#define	DRAW_DIALOGBOX_EXCHANGE33	"Espera mientras %s acepta el"
#define	DRAW_DIALOGBOX_EXCHANGE34	"intercambio. El intercambio no puede"
#define	DRAW_DIALOGBOX_EXCHANGE35	"darse al menos que ambos lo acepten."

#define	DRAW_DIALOGBOX_EXCHANGE36	"Si el otro jugador no acepta el intercambio"
#define	DRAW_DIALOGBOX_EXCHANGE37	"puedes cancelar el intercambio presionando"
#define	DRAW_DIALOGBOX_EXCHANGE38	"el boton CANCEL. Pero si el otro jugador ya"
#define	DRAW_DIALOGBOX_EXCHANGE39	"presiono exchange, no podras cancelar."



#define	DRAW_DIALOGBOX_FISHING1	"Valor: %d oro"
#define	DRAW_DIALOGBOX_FISHING2	"Probabilidad:"



// ***************************************************************************************************************************
// ***************************************************************************************************************************
// **************************************************** GUILD ***********************************************************************
// ***************************************************************************************************************************
// ***************************************************************************************************************************

#define DRAW_DIALOGBOX_GUILD1	"Ver miembros conectados"
#define DRAW_DIALOGBOX_GUILD2	"Dar oro al gremio"
#define DRAW_DIALOGBOX_GUILD3	"Dar Majestics algremio"
#define DRAW_DIALOGBOX_GUILD4	"Miembros conectados: %u"
#define DRAW_DIALOGBOX_GUILD5	"Nombre"
#define DRAW_DIALOGBOX_GUILD6	"Rango"
#define DRAW_DIALOGBOX_GUILD7	"Mapa"
#define DRAW_DIALOGBOX_GUILD8	"Clickea el nombre para convocar (%u oro)"
#define DRAW_DIALOGBOX_GUILD9	"Clickea el rango para cambiarlo"
#define DRAW_DIALOGBOX_GUILD10	"Oro: %s"
#define DRAW_DIALOGBOX_GUILD11	"Majestics: %s"
#define DRAW_DIALOGBOX_GUILD12	"Aumentar"
#define DRAW_DIALOGBOX_GUILD13	"Banco del gremio (nivel: %u)"
#define DRAW_DIALOGBOX_GUILD14	"Convocaciones (nivel: %u)"
#define DRAW_DIALOGBOX_GUILD15	"Volver"
#define DRAW_DIALOGBOX_GUILD16	"Nivel del banco del gremio: %u"
#define DRAW_DIALOGBOX_GUILD17	"Aumentar"
#define DRAW_DIALOGBOX_GUILD18	"El banco del gremio esta siendo compartido con todos los miembros del mismo."
#define DRAW_DIALOGBOX_GUILD20	"Nivel %u:"
#define DRAW_DIALOGBOX_GUILD21	"%uM Oro, %u Maj., %uk Cont."
#define DRAW_DIALOGBOX_GUILD22	"% Espacios para objetos."
#define DRAW_DIALOGBOX_GUILD23	"Nivel de convocacion: %u"
#define DRAW_DIALOGBOX_GUILD24	"Esta reforma permite traer a un mimebro de tu gremio por un costo de %u de oro (Solo disponible en algunas areas)."
#define DRAW_DIALOGBOX_GUILD25	"Disponible en Middleland"
#define DRAW_DIALOGBOX_GUILD26	"Disponible en Middleland, Icebound"
#define DRAW_DIALOGBOX_GUILD27	"Disponible en Middleland, Icebound, Tower of Hell 1, Dungeons Lv.1-2"
#define DRAW_DIALOGBOX_GUILD28	"Disponible en Middleland, Icebound, Tower of Hell 1-3, Dungeons Lv.1-4"
#define DRAW_DIALOGBOX_GUILD29	"Cont.: %s"
#define DRAW_DIALOGBOX_GUILD30	"Nivel de capitanazgo: %u"
#define DRAW_DIALOGBOX_GUILD31	"NIvel de lideres de incursion: %u"
#define DRAW_DIALOGBOX_GUILD32	"Lideres de caza: %u"
#define DRAW_DIALOGBOX_GUILD33	"Capitanazgo (nivel: %u, %u %ss)"
#define DRAW_DIALOGBOX_GUILD34	"Lideres de incursion (nivel: %u, %u %ss)"
#define DRAW_DIALOGBOX_GUILD35	"Lideres de caza (nivel: %u, %u %ss)"
#define DRAW_DIALOGBOX_GUILD36	"Esta reforma permite al Jefe del gremio darle rango de capitan a un miembro. Los capitanes tienen permiso para invitar y hechar miembros de un gremio."
#define DRAW_DIALOGBOX_GUILD37	"%u Capitan(es)."
#define DRAW_DIALOGBOX_GUILD38	"%u Lider(es)de incrusion."
#define DRAW_DIALOGBOX_GUILD39	"%u Lider(es)de caza."
#define DRAW_DIALOGBOX_GUILD40	"Esta reforma permite al Jefe del gremio darle rango de Lider de incrusion a un miembro. Los lideres de incrusion aumentan el daño de ellos y de todos los miembros de su gremio. Siempre y cuando esten en un grupo."
#define DRAW_DIALOGBOX_GUILD41	"Esta reforma permite al Jefe del gremio darle rango de Lider de caza a un miembro. Los lideres de caza aumentan la defensa contra mounstruos y la probabilidad de drop. Siempre y cuando esten en grupo."
#define DRAW_DIALOGBOX_GUILD42	"El gremio %s subio a nivel %u!"
#define DRAW_DIALOGBOX_GUILD43	"Dar contribucion al gremio"
#define DRAW_DIALOGBOX_GUILD44	"%ss: %u"


#define DLGBOX_CLICK_GUILD1	"No tienes el nivel de gremio necesario para utilizar el teletransporte."
#define DLGBOX_CLICK_GUILD2	"Solo puedes convocar jugadores en Middleland."
#define DLGBOX_CLICK_GUILD3	"Solo puedes convocar jugadores a Middleland y Icebound."
#define DLGBOX_CLICK_GUILD4	" Icebound, Tower of Hell 1 and Dungeon Lv.1-2"
#define DLGBOX_CLICK_GUILD5	" Icebound, Tower of Hell and Dungeons"
#define DLGBOX_CLICK_GUILD6	"\"%s\" ha sido avisado que ha sido convocado.."
#define DLGBOX_CLICK_GUILD7	"Debes esperar %u minuto(s) y %u segundo(s)"
#define DLGBOX_CLICK_GUILD8	"para convocar \"%s\" devuelta."
#define DLGBOX_CLICK_GUILD9	"Tu gremio no tiene el suficiente oro para convocar a un jugador."
#define DLGBOX_CLICK_GUILD10	"Debes esperar 10 segundos luego de recibir daño para convocar." 
#define DLGBOX_CLICK_GUILD11	"Tu gremio no tiene suficiente oro" 
#define DLGBOX_CLICK_GUILD12	"Tu gremio no tiene suficientes puntos Majestic." 
#define DLGBOX_CLICK_GUILD13	"Tu gremio no tiene suficiente contribución." 
#define DLGBOX_CLICK_GUILD14	"Ésta reforma ya esta al nivel máximo." 

#define DRAW_DIALOGBOX_GUILDCONTRIBUTE1	"¿Cuanto oro te"
#define DRAW_DIALOGBOX_GUILDCONTRIBUTE2	"gustaria dar?"
#define DRAW_DIALOGBOX_GUILDCONTRIBUTE3	"¿Cuantos Majestic"
#define DRAW_DIALOGBOX_GUILDCONTRIBUTE4	"te gustaria dar?"
#define DRAW_DIALOGBOX_GUILDCONTRIBUTE5	"Has dado %u de oro a tu gremio."
#define DRAW_DIALOGBOX_GUILDCONTRIBUTE6	"Has dado %u Puntos Majestics a tu gremio."
#define DRAW_DIALOGBOX_GUILDCONTRIBUTE7	"¿Cuanta contribución"
#define DRAW_DIALOGBOX_GUILDCONTRIBUTE8	"te gustaría dar?"
#define DRAW_DIALOGBOX_GUILDCONTRIBUTE9	"Has dado %u puntos de contribución a tu gremio."

#define DRAW_DIALOGBOX_GUILDQUERY		"Que rango deseas asignarle a %s?"
#define DRAW_DIALOGBOX_GUILDQUERY2		"%s (%u restante)"

#define GUILD_MEMBERONLINE			"%s \"%s\" se ha conectado."
#define GUILD_MEMBEROFFLINE		"%s \"%s\" se ha desconectado."
#define GUILD_MEMBERRANKCHANGE	"Tu rango fue cambiado de: %s. A %s."
#define GUILD_MEMBERRANKCHANGE2	"El rango de %s  fue cambiado de: %s. A %s."


#define	DRAW_DIALOGBOX_GUILDMENU1	"Crear un nuevo gremio (20 Carisma)"
#define	DRAW_DIALOGBOX_GUILDMENU4	"Romper gremio"
#define	DRAW_DIALOGBOX_GUILDMENU7	"Comprar un ticket de admisión"
#define	DRAW_DIALOGBOX_GUILDMENU9	"Comprar un ticket de destitución "
#define	DRAW_DIALOGBOX_GUILDMENU11	"Reservar la arena"
#define	DRAW_DIALOGBOX_GUILDMENU13	"Obtener un ticket para entrar"

#define	DRAW_DIALOGBOX_GUILDMENU17	"* Selecciona una acción"
#define	DRAW_DIALOGBOX_GUILDMENU18	"Escribe un nombre para tu gremio."
#define	DRAW_DIALOGBOX_GUILDMENU19	"Creando nuevo gremio..."
#define	DRAW_DIALOGBOX_GUILDMENU20	"Un nuevo gremio se ha creado!."
#define	DRAW_DIALOGBOX_GUILDMENU21	"Eres el Jefe del gremio."
#define	DRAW_DIALOGBOX_GUILDMENU22	"Error al crear nuevo gremio."
#define	DRAW_DIALOGBOX_GUILDMENU23	"El nombre elejido ya existe."
#define	DRAW_DIALOGBOX_GUILDMENU24	"Destitución del gremio:"
#define	DRAW_DIALOGBOX_GUILDMENU25	"Rompe tu gremio. Tus privilegios"
#define	DRAW_DIALOGBOX_GUILDMENU26	"como Jefe de Gremio, desaparecerán."
#define	DRAW_DIALOGBOX_GUILDMENU27	"Y los miembros del gremio seran"
#define	DRAW_DIALOGBOX_GUILDMENU28	"automaticamente destituidos."
#define	DRAW_DIALOGBOX_GUILDMENU28_1	"¿Acepta la ruptura del gremio?"

#define	DRAW_DIALOGBOX_GUILDMENU29	"Rompiendo el gremio..."
#define	DRAW_DIALOGBOX_GUILDMENU30	"Tu gremio se disolvió."
#define	DRAW_DIALOGBOX_GUILDMENU31	"Error al tratar de romper el gremio."

#define	DRAW_DIALOGBOX_GUILDMENU32	"El precio para el ticket de admisión al"
#define	DRAW_DIALOGBOX_GUILDMENU33	"gremio es  de 50 gold. Daselo a algun"
#define	DRAW_DIALOGBOX_GUILDMENU34	"Jefe del gremio, el decidira si"
#define	DRAW_DIALOGBOX_GUILDMENU35	"entras o no."
#define	DRAW_DIALOGBOX_GUILDMENU36	"¿Deseas comprar uno?"
#define	DRAW_DIALOGBOX_GUILDMENU37	"Compraste un ticket de admisión."

#define	DRAW_DIALOGBOX_GUILDMENU38	"El precio para el ticket de destitución"
#define	DRAW_DIALOGBOX_GUILDMENU39	"al gremio es de 50 gold. Daselo a algun"
#define	DRAW_DIALOGBOX_GUILDMENU40	"Jefe del gremio o al GuildOfficer"
#define	DRAW_DIALOGBOX_GUILDMENU41	"y saldras del gremio autmaticamente."
#define	DRAW_DIALOGBOX_GUILDMENU42	"¿Deseas comprar uno?"
#define	DRAW_DIALOGBOX_GUILDMENU43	"Compraste un ticket de destitución."

#define	DRAW_DIALOGBOX_GUILDMENU44	"El costo para reservar"
#define	DRAW_DIALOGBOX_GUILDMENU45	"la arena es de 1500gold."
#define	DRAW_DIALOGBOX_GUILDMENU46	"Clickea la arnea que"
#define	DRAW_DIALOGBOX_GUILDMENU47	"deseas reservar. Podras"
#define	DRAW_DIALOGBOX_GUILDMENU48	"recibir 50 tickets para ir,"
#define	DRAW_DIALOGBOX_GUILDMENU49	"y podras usar la aren a por"
#define	DRAW_DIALOGBOX_GUILDMENU50	"2 horas."

#define	DRAW_DIALOGBOX_GUILDMENU51	"Arena 1"
#define	DRAW_DIALOGBOX_GUILDMENU53	"Arena 2"
#define	DRAW_DIALOGBOX_GUILDMENU55	"Arena 3"
#define	DRAW_DIALOGBOX_GUILDMENU57	"Arena 4"
#define	DRAW_DIALOGBOX_GUILDMENU59	"Arena 5"
#define	DRAW_DIALOGBOX_GUILDMENU61	"Arena 6"
#define	DRAW_DIALOGBOX_GUILDMENU63	"Arena 7"
#define	DRAW_DIALOGBOX_GUILDMENU65	"Arena 8"

#define	DRAW_DIALOGBOX_GUILDMENU66	"Has reservado la arena."
#define	DRAW_DIALOGBOX_GUILDMENU67	"No puedes reservar otra"
#define	DRAW_DIALOGBOX_GUILDMENU68	"arena."

#define	DRAW_DIALOGBOX_GUILDMENU69	"Las arenas no estan disponibles en este momento"
#define	DRAW_DIALOGBOX_GUILDMENU70	"Por favor, intenta mas tarde."
#define	DRAW_DIALOGBOX_GUILDMENU71	"La arena seleccionada ya está reservada."
#define	DRAW_DIALOGBOX_GUILDMENU72	"Por favor, elije otra."
#define	DRAW_DIALOGBOX_GUILDMENU73	"No tienes oro suficiente para reservar..."
#define	DRAW_DIALOGBOX_GUILDMENU74	"Reservando la arena..."
#define	DRAW_DIALOGBOX_GUILDMENU75	"Escribe un nombre para tu gremio."
#define	DRAW_DIALOGBOX_GUILDMENU76	"Esta arena no esta disponible hoy."
#define	DRAW_DIALOGBOX_GUILDMENU77	"Por favor, intenta con otra."
#define	DRAW_DIALOGBOX_GUILDMENU78	"Aresden puede usar las arenas 2 4 6 8"
#define	DRAW_DIALOGBOX_GUILDMENU79	"en los dias pares. Elvine puede usar"
#define	DRAW_DIALOGBOX_GUILDMENU80	"las 1 3 5 7 en dias impares."
#define	DRAW_DIALOGBOX_GUILDMENU81	"Ya reservaste otra arena."


#define	DRAW_DIALOGBOX_GUILD_OPERATION1	"Pedido de admisión al gremio:"
#define	DRAW_DIALOGBOX_GUILD_OPERATION2	"* Este jugador quiere entrar al gremio. "
#define	DRAW_DIALOGBOX_GUILD_OPERATION3	"Puedes decidir si puede entrar"
#define	DRAW_DIALOGBOX_GUILD_OPERATION4	"o no del gremio."
#define	DRAW_DIALOGBOX_GUILD_OPERATION5	"Toma una decisión."

#define	DRAW_DIALOGBOX_GUILD_OPERATION6	"Pedido de expulsion del gremio:"
#define	DRAW_DIALOGBOX_GUILD_OPERATION7	"* Este jugador quiere salir del gremio."
#define	DRAW_DIALOGBOX_GUILD_OPERATION8	"Puedes decidir si puede salir"
#define	DRAW_DIALOGBOX_GUILD_OPERATION9	"o no del gremio."
#define	DRAW_DIALOGBOX_GUILD_OPERATION10	"Toma una decisión."

#define	DRAW_DIALOGBOX_GUILD_OPERATION11	"Aprobación para entrar al gremio:"
#define	DRAW_DIALOGBOX_GUILD_OPERATION12	"* El Jefe aprobó tu pedido."
#define	DRAW_DIALOGBOX_GUILD_OPERATION13	"Te convertiste en Miembro del Gremio."
#define	DRAW_DIALOGBOX_GUILD_OPERATION14	"Aprobación para entrar al gremio RECHAZADA:"
#define	DRAW_DIALOGBOX_GUILD_OPERATION15	"* El Jefe desaprobó tu pedido."
#define	DRAW_DIALOGBOX_GUILD_OPERATION16	""
#define	DRAW_DIALOGBOX_GUILD_OPERATION17	"Aprobación para salir del gremio:"
#define	DRAW_DIALOGBOX_GUILD_OPERATION18	"* El Jefe aprobo tu pedido."
#define	DRAW_DIALOGBOX_GUILD_OPERATION19	"Ya no eres un Miembro del Gremio."
#define	DRAW_DIALOGBOX_GUILD_OPERATION20	"Eres de nuevo un ciudadano normal."
#define	DRAW_DIALOGBOX_GUILD_OPERATION21	"Aporbación para salir del gremio RECHAZADA:"
#define	DRAW_DIALOGBOX_GUILD_OPERATION22	"* El Jefe desaprobó tu pedido."
#define	DRAW_DIALOGBOX_GUILD_OPERATION23	""
#define	DRAW_DIALOGBOX_GUILD_OPERATION24	"* Tu guild se disolvió."
#define	DRAW_DIALOGBOX_GUILD_OPERATION25	"* Tu guild se disolvió."
#define	DRAW_DIALOGBOX_GUILD_OPERATION26	"No eres mas un Miembro del Gremio."


// ------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------

#define	DRAW_DIALOGBOX_MAGIC1	"Circle One"
#define	DRAW_DIALOGBOX_MAGIC2	"Circle Two"
#define	DRAW_DIALOGBOX_MAGIC3	"Circle Three"
#define	DRAW_DIALOGBOX_MAGIC4	"Circle Four"
#define	DRAW_DIALOGBOX_MAGIC5	"Circle Five"
#define	DRAW_DIALOGBOX_MAGIC6	"Circle Six"
#define	DRAW_DIALOGBOX_MAGIC7	"Circle Seven"
#define	DRAW_DIALOGBOX_MAGIC8	"Circle Eight"
#define	DRAW_DIALOGBOX_MAGIC9	"Circle Nine"
#define	DRAW_DIALOGBOX_MAGIC10	"Circle Ten"
#define	DRAW_DIALOGBOX_MAGIC11	"No has aprendido ninguna magia."
#define	DRAW_DIALOGBOX_MAGIC12	"Puedes aprender magias en la"
#define	DRAW_DIALOGBOX_MAGIC13	"WizzardTower. Para aprender un"
#define	DRAW_DIALOGBOX_MAGIC14	"hechizo necesitas oro y INT."
#define	DRAW_DIALOGBOX_MAGIC15  "  "
#define	DRAW_DIALOGBOX_MAGIC16	"Porbabilidad de logro: %d%%"


#define	DRAW_DIALOGBOX_MAGICSHOP1	"Circle One"
#define	DRAW_DIALOGBOX_MAGICSHOP2	"Circle Two"
#define	DRAW_DIALOGBOX_MAGICSHOP3	"Circle Three"
#define	DRAW_DIALOGBOX_MAGICSHOP4	"Circle Four"
#define	DRAW_DIALOGBOX_MAGICSHOP5	"Circle Five"
#define	DRAW_DIALOGBOX_MAGICSHOP6	"Circle Six"
#define	DRAW_DIALOGBOX_MAGICSHOP7	"Circle Seven"
#define	DRAW_DIALOGBOX_MAGICSHOP8	"Circle Eight"
#define	DRAW_DIALOGBOX_MAGICSHOP9	"Circle Nine"
#define	DRAW_DIALOGBOX_MAGICSHOP10	"Circle Ten"
#define	DRAW_DIALOGBOX_MAGICSHOP11	"Nombre"
#define	DRAW_DIALOGBOX_MAGICSHOP12	"Int"
#define	DRAW_DIALOGBOX_MAGICSHOP13	"Costo"
#define	DRAW_DIALOGBOX_MAGICSHOP14	"Elije la magia que quieres aprender."
// CLEROTH - MAGIC CFG
#define DRAW_DIALOGBOX_MAGICSHOP15	"Str Req."
#define	DRAW_DIALOGBOX_MAGICSHOP16	"Consumo"
#define	DRAW_DIALOGBOX_MAGICSHOP17	"Tabla de ahorro de mana"

#define	DRAW_DIALOGBOX_NOTICEMSG1	"El servidor se cerrara dentro de %d Minutos!"
#define	DRAW_DIALOGBOX_NOTICEMSG2	"El servidor se cerrara pronto!!"
#define	DRAW_DIALOGBOX_NOTICEMSG3	""
#define	DRAW_DIALOGBOX_NOTICEMSG4	""

#define	DRAW_DIALOGBOX_NOTICEMSG7	"El cierre ha comenzado!!!!"
#define	DRAW_DIALOGBOX_NOTICEMSG8	"Tu conexion sera cerrada pronto automaticamente"
#define	DRAW_DIALOGBOX_NOTICEMSG9	"No des items ni los tires, se pueden perder"
#define	DRAW_DIALOGBOX_NOTICEMSG10	"esos datos."


#define	DRAW_DIALOGBOX_NPCACTION_QUERY13	"Ofrecer"
#define	DRAW_DIALOGBOX_NPCACTION_QUERY17	"Sacar objetos"
// CLEROTH
#define DRAW_DIALOGBOX_NPCACTION_QUERY19	"Aprender"

#define	DRAW_DIALOGBOX_NPCACTION_QUERY21	"Comerciar"
#define	DRAW_DIALOGBOX_NPCACTION_QUERY25	"Hablar"
#define DRAW_DIALOGBOX_NPCACTION_QUERY29	"%d %s a"
#define DRAW_DIALOGBOX_NPCACTION_QUERY29_1  "%s"
#define	DRAW_DIALOGBOX_NPCACTION_QUERY30	"Dar"
#define	DRAW_DIALOGBOX_NPCACTION_QUERY34	"Cambiar"
#define	DRAW_DIALOGBOX_NPCACTION_QUERY39	"Vender"
#define	DRAW_DIALOGBOX_NPCACTION_QUERY43	"Reparar"
#define	DRAW_DIALOGBOX_NPCACTION_QUERY48	"Depositar"
#define DRAW_DIALOGBOX_NPCACTION_QUERY49 "Heldenian staff officer"

#define	DRAW_DIALOGBOX_PARTY1	"Unirte a un grupo."
#define	DRAW_DIALOGBOX_PARTY4	"Salir del grupo."
#define	DRAW_DIALOGBOX_PARTY7	"Ver la lista con los miembros del grupo."

#define	DRAW_DIALOGBOX_PARTY10	"No estas en un grupo."
#define	DRAW_DIALOGBOX_PARTY11	"Puedes hacer un grupo"
#define	DRAW_DIALOGBOX_PARTY12	"con otros jugadores."

#define	DRAW_DIALOGBOX_PARTY13	"Estas en un grupo. Puedes"
#define	DRAW_DIALOGBOX_PARTY14	"salir del grupo o ver"
#define	DRAW_DIALOGBOX_PARTY15	"la lista con los miembros."

#define	DRAW_DIALOGBOX_PARTY16	"%s te ofrece a unirte a"
#define	DRAW_DIALOGBOX_PARTY17	"su grupo. Podras compartir"
#define	DRAW_DIALOGBOX_PARTY18	"la experiencia ganada en"
#define	DRAW_DIALOGBOX_PARTY19	"relacion a los niveles"
#define	DRAW_DIALOGBOX_PARTY20	"si te unes al grupo."

#define	DRAW_DIALOGBOX_PARTY21	"¿Deseas unirte al grupo?"

#define	DRAW_DIALOGBOX_PARTY22	"Clickea al personaje que"
#define	DRAW_DIALOGBOX_PARTY23	"quieres invitar.Presiona"
#define	DRAW_DIALOGBOX_PARTY24	"el boton CANCEL si no"
#define	DRAW_DIALOGBOX_PARTY25	"quieres unirte a un grupo."

#define	DRAW_DIALOGBOX_PARTY26	"Le preguntaste a %s"
#define	DRAW_DIALOGBOX_PARTY27	"Si quiere unirse, por favor,"
#define	DRAW_DIALOGBOX_PARTY28	"espera hasta que responda."
#define	DRAW_DIALOGBOX_PARTY29	"presiona el boton CANCEL si"
#define	DRAW_DIALOGBOX_PARTY30	"no quieres crear un grupo."

#define	DRAW_DIALOGBOX_PARTY31	"Los nombres de los"
#define	DRAW_DIALOGBOX_PARTY32	"personajes del grupo."
#define	DRAW_DIALOGBOX_PARTY33	"Saliendo del grupo."
#define	DRAW_DIALOGBOX_PARTY34	"Por favor espere..."
#define	DRAW_DIALOGBOX_PARTY35	"Has salido del grupo."

#define	DRAW_DIALOGBOX_PARTY36	"Error al tratar de salir del grupo."
#define	DRAW_DIALOGBOX_PARTY37	"Si fallas repetidas veces al intentar"
#define	DRAW_DIALOGBOX_PARTY38	"de salir del grupo, desconectate "
#define	DRAW_DIALOGBOX_PARTY39	"y saldras automaticamente."

#define	DRAW_DIALOGBOX_PARTY40	"Te has unido al grupo."
#define	DRAW_DIALOGBOX_PARTY41	"Puedes compartir experiencia"
#define	DRAW_DIALOGBOX_PARTY42	"con los participantes. Y no"
#define	DRAW_DIALOGBOX_PARTY43	"es necesario activar el modo"
#define	DRAW_DIALOGBOX_PARTY44	"de ataque seguro, asi juegas"
#define	DRAW_DIALOGBOX_PARTY45	"tranquilo y de forma eficaz."

#define	DRAW_DIALOGBOX_PARTY46	"Error al unirte al grupo."
#define	DRAW_DIALOGBOX_PARTY47	"Tal vez el jugador rechazo"
#define	DRAW_DIALOGBOX_PARTY48	"el pedido, se lo ofreciste"
#define	DRAW_DIALOGBOX_PARTY49	"a un enemigo, o se lo"
#define	DRAW_DIALOGBOX_PARTY50	"ofreciste a un jugador que"
#define	DRAW_DIALOGBOX_PARTY51	"no esta en modo de paz."

#define	DRAW_DIALOGBOX_PARTY52	"Tu grupo se disolvió."
#define	DRAW_DIALOGBOX_PARTY53	"Si el numero de miembros es de 1,"
#define	DRAW_DIALOGBOX_PARTY54	"El grupo se disolverá automaticamente."

#define	DRAW_DIALOGBOX_PARTY55	"¿Deseas salir del grupo?"

#define	DRAW_DIALOGBOX_PARTY56 "Activar auto aprobación al grupo."
#define	DRAW_DIALOGBOX_PARTY57 "Desactivar auto aprobación al grupo."


#define	DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT1	"Tirando %s."
#define	DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT2	"Darle %s a %s."
#define	DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT3	"Decide la cantidad."
#define	DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT4	"Tirando %s."
#define	DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT5	"Darle %s a %s."
#define	DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT6	"Decide la cantidad."


#define	DRAW_DIALOGBOX_QUEST1	"No estas en una Misión."
#define	DRAW_DIALOGBOX_QUEST2	"Estas en una misión de conquista de mounstruos."
#define	DRAW_DIALOGBOX_QUEST3	"Lograste la misión de conquista de mounstruos."
#define	DRAW_DIALOGBOX_QUEST5	"Cliente: %s"

#define	DRAW_DIALOGBOX_QUEST26	"Estas en una Misión de observación."
#define	DRAW_DIALOGBOX_QUEST27	"Lograste la Misión de observación."
#define	DRAW_DIALOGBOX_QUEST29	"Cliente: %s"
#define	DRAW_DIALOGBOX_QUEST30	"Objetivo: Observar al enemigo."
#define	DRAW_DIALOGBOX_QUEST31	"Ubicación: Cualquiera"
#define	DRAW_DIALOGBOX_QUEST32	"Mapa : %s"
#define	DRAW_DIALOGBOX_QUEST33	"Posición: %d, %d Range: %d block"
#define	DRAW_DIALOGBOX_QUEST34	"Contribución: %d"
#define	DRAW_DIALOGBOX_QUEST35	"Tu misión ha sido cancelada."



#define	DRAW_DIALOGBOX_SELL_LIST1  	"%d %s"
#define	DRAW_DIALOGBOX_SELL_LIST2	"Arrastra el objeto que quieras"
#define	DRAW_DIALOGBOX_SELL_LIST3	"vender. Puedes vender hasta"
#define	DRAW_DIALOGBOX_SELL_LIST4	"12 objetos al mismo tiempo."
#define	DRAW_DIALOGBOX_SELL_LIST5	"No puedes vender items gastados."
#define	DRAW_DIALOGBOX_SELL_LIST6	"Si quieres remover el objeto"
#define	DRAW_DIALOGBOX_SELL_LIST7	"de la lista, clickea en su "
#define	DRAW_DIALOGBOX_SELL_LIST8	"nombre y saldra automaticamente."
#define	DRAW_DIALOGBOX_SELL_LIST9   "* Pon aquí los objetos para vender *"

#define	DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM1	"%d %s"
#define	DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM2	"Dureza: %d"
#define	DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM3	"Valor: %d Gold"
#define	DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM4	"¿Deseas venderlo?"
#define	DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM6	"Costo: %d Gold"
#define	DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM7	"¿Deseas repararlo?"
#define	DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM8	"Procesando la venta del objeto.."
#define	DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM9	"No puedes dar o vender"
#define	DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM10	"hasta que esto termine."
#define	DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM11	"Procesando la reparación del objeto..."


#define	DRAW_DIALOGBOX_SHOP1	"OBJETO"
#define	DRAW_DIALOGBOX_SHOP3	"PRECIO"
#define	DRAW_DIALOGBOX_SHOP6	"Peso"
#define	DRAW_DIALOGBOX_SHOP7	": %d Gold"
#define	DRAW_DIALOGBOX_SHOP8	": %d Stone"
#define	DRAW_DIALOGBOX_SHOP9	"Daño"
#define	DRAW_DIALOGBOX_SHOP10	"Velocidad(Min.~Max.)"
#define	DRAW_DIALOGBOX_SHOP11	"*Tu FUERZA debe ser al menos de %d para usar este objeto."
#define	DRAW_DIALOGBOX_SHOP12	"Defensa"
#define	DRAW_DIALOGBOX_SHOP15	"Disponible para más de %d de Fuerza"
#define	DRAW_DIALOGBOX_SHOP16	"Disponible para más de %d de Destreza"
#define	DRAW_DIALOGBOX_SHOP17	"Disponible para más de %d de Vitalidad"
#define	DRAW_DIALOGBOX_SHOP18	"Disponible para más de %d de Inteligencia"
#define	DRAW_DIALOGBOX_SHOP19	"Disponible para más de %d de Magia"
#define	DRAW_DIALOGBOX_SHOP20	"Disponible para más de %d de Carisma"
#define	DRAW_DIALOGBOX_SHOP21	"(Cuidado!) Tus estadisticas son muy bajas para usar esto."
#define	DRAW_DIALOGBOX_SHOP22	"(Cuidado!) Solo para hombres."
#define	DRAW_DIALOGBOX_SHOP23	"(Cuidado!) Solo para mujeres."
#define	DRAW_DIALOGBOX_SHOP24	"Nivel"
#define	DRAW_DIALOGBOX_SHOP25	": mas de %d"
#define	DRAW_DIALOGBOX_SHOP26	"(Cuidado!) Tu nivel es muy bajo para usar esto."
#define	DRAW_DIALOGBOX_SHOP27	"Cantidad:"



#define	DRAW_DIALOGBOX_SKILLDLG2	"Listado de objetos que puedes crear con"
#define	DRAW_DIALOGBOX_SKILLDLG3	"tu habilidad actual. Los objetos que"
#define	DRAW_DIALOGBOX_SKILLDLG4	"puedas crear, serán mostrados en"
#define	DRAW_DIALOGBOX_SKILLDLG5	"Azul."
#define	DRAW_DIALOGBOX_SKILLDLG6	"Selecciona un objeto que quieras manufacturar."
#define	DRAW_DIALOGBOX_SKILLDLG7	"Nivel de habilidad: %d/%d"
#define	DRAW_DIALOGBOX_SKILLDLG8	"Ingredientes necesarios:"

#define	DRAW_DIALOGBOX_SKILLDLG15	"Clickea el boton MANUFACTURE luego"
#define	DRAW_DIALOGBOX_SKILLDLG16	"de haber arrastrado los ingredientes"
#define	DRAW_DIALOGBOX_SKILLDLG17	"a los espacios en blanco."

#define	DRAW_DIALOGBOX_SKILLDLG18	"No hay suficientes ingredientes para"
#define	DRAW_DIALOGBOX_SKILLDLG19	"manufacturar. Los materiales necesitados"
#define	DRAW_DIALOGBOX_SKILLDLG20	"son mostrados en gris."

#define	DRAW_DIALOGBOX_SKILLDLG29	"Manufacturando los objetos..."
#define	DRAW_DIALOGBOX_SKILLDLG30	"Espera a que la manufacutra termine."
#define	DRAW_DIALOGBOX_SKILLDLG31	"Éxito al manufacturar!"
#define	DRAW_DIALOGBOX_SKILLDLG32	"La pureza del producto es de: %d%%."
#define	DRAW_DIALOGBOX_SKILLDLG33	"La afinación del producto es de: %d%%."
#define	DRAW_DIALOGBOX_SKILLDLG34	"Error al manufacturar."

#define	DRAW_DIALOGBOX_SYSMENU_DETAILLEVEL   "Nivel de detalles"
#define	DRAW_DIALOGBOX_SYSMENU_LOW           "Bajo"
#define	DRAW_DIALOGBOX_SYSMENU_NORMAL        "Normal"
#define	DRAW_DIALOGBOX_SYSMENU_HIGH          "Alto"
#define	DRAW_DIALOGBOX_SYSMENU_SOUND         "Sonido"
#define	DRAW_DIALOGBOX_SYSMENU_ON            "Encendido"
#define	DRAW_DIALOGBOX_SYSMENU_OFF           "Apagado"
#define	DRAW_DIALOGBOX_SYSMENU_DISABLED      "Desactivado"
#define	DRAW_DIALOGBOX_SYSMENU_MUSIC         "Musica"
#define	DRAW_DIALOGBOX_SYSMENU_SOUND         "Sonido"
#define	DRAW_DIALOGBOX_SYSMENU_MUSICVOLUME   "Volumen de la musica"
#define	DRAW_DIALOGBOX_SYSMENU_SOUNDVOLUME   "Volumen del sonido"
#define	DRAW_DIALOGBOX_SYSMENU_TRANSPARENCY  "Transparencia en dialogos"
#define DRAW_DIALOGBOX_SYSMENU_GUIDEMAP      "Mapa de guia"
#define DRAW_DIALOGBOX_SYSMENU_WHISPER       "Mensajes Privados"
#define DRAW_DIALOGBOX_SYSMENU_SHOUT         "Grito"

#define DLGBOX_GLDSUMMONS1			"El Jefe del gremio te ha convocado"
#define DLGBOX_GLDSUMMONS2			"¿Deseas teletransportarte?"
#define DLGBOXCLICK_GUILDSUMMONS1	"No puedes teletransportarte hasta 10 segundos luego de recibir daño." 

#define DLGBOX_RESURRECT1		"Alguien quiere revivirte"
#define DLGBOX_RESURRECT2		"¿Deseas revivir aqui?"

#define	BITEMDROP_CHARACTER1	"El objeto esta gastado. Reparalo para usarlo."
#define	BITEMDROP_CHARACTER2	"El objeto es muy pesado. Necesitas mas Fuerza."
#define	BITEMDROP_CHARACTER3	"El objeto no esta disponible."
#define	BITEMDROP_CHARACTER4	"Tu nivel es demasiado bajo para usarlo."
#define	BITEMDROP_CHARACTER5	"No puedes usar un objeto mientras usas una habilidad."
#define	BITEMDROP_CHARACTER6	"Este objeto no es para hombre."
#define	BITEMDROP_CHARACTER7	"Este objeto no es para mujer."
#define	ITEM_EQUIPMENT_RELEASED	"Te has sacado el siguiente objeto: %s."
#define	BITEMDROP_CHARACTER9	"Te has equipado: %s."


#define	BITEMDROP_INVENTORY1	"No puedes sacarte equipamiento mientras usas una habilidad."


#define	BITEMDROP_SELLLIST1	"Ese objeto ya esta en la lista."
#define	BITEMDROP_SELLLIST2	"No puedes vender oro."
#define	BITEMDROP_SELLLIST3	"No puedes vender más de 12 objetos a la vez."


#define	BITEMDROP_SKILLDIALOG1	"La transacción no ha finalizado."
#define	BITEMDROP_SKILLDIALOG4	"No hay mas lugar para los ingredientes."


#define	DLGBOX_CLICK_BANK1	"No puedes retirar el objeto porque tu mochila esta llena."
#define	DLGBOX_CLICK_BANK2	"Tu rango actual no permite que retires objetos."


#define	DLGBOX_CLICK_FISH1	"Intentando pescar..."


#define	DLGBOX_CLICK_MAGIC1	"Tus manos deben estar libres para usar una magia."
#define	DLGBOX_CLICK_MAGIC2	"No puedes usar una magia mientras usas una habilidad."


#define	TALKING_TO_GUILDHALL_OFFICER	"Hablando con el Guildhall Officer..."
#define	TALKING_TO_SHOP_KEEPER			"Hablando con el Shop Keeper..."
#define	TALKING_TO_BLACKSMITH_KEEPER	"Hablando con el Blacksmith Keeper..."
#define	TALKING_TO_CITYHALL_OFFICER	"Hablando con el Cityhall Officer..."
#define	TALKING_TO_WAREHOUSE_KEEPER	"Hablando con el Warehouse Keeper..."
#define	TALKING_TO_MAGICIAN	"Hablando con el Sorcerer..."
#define	DLGBOX_CLICK_NPCACTION_QUERY7	"Demasiado lejos para dar el objeto."
#define	DLGBOX_CLICK_NPCACTION_QUERY8	"Demasiado lejos para intercambiar el objeto."
#define	DLGBOX_CLICK_NPCACTION_QUERY9	"No hay mas cupo en tu banco."
#define	TALKING_TO_GUARD	"Hablando con el Guard..."
#define	TALKING_TO_UNICORN	"Hablando con el Unicorn..."

#define	TALKING_TO_GUILDBANK_KEEPER	"Hablando con el Guild Banker..."

#define	DLGBOX_CLICK_SHOP1	"No puedes comprar nada porque tu mochila esta llena."


#define	DLGBOX_CLICK_SKILL1	"Ya estas usando otra habilidad."
#define	DLGBOX_CLICK_SKILL2	"No puedes usar una habilidad cuando estas en movimiento."


#define	DLGBOX_CLICK_SKILLDLG1	"Mezclando una poción..."

#define NOTIFYMSG_SPELLDISABLED		"La magia %s ha sido desactivada . No puedes usarla."
#define NOTIFYMSG_SPELLENABLED		"La magia %s ha sido activada. Puedes volver a usarla."
#define NOTIFYMSG_ARMORDISABLED		"El uso de armaduras ha sido desactivado."
#define NOTIFYMSG_ARMORENABLED		"El uso de armaduras ha sido activado."
#define NOTIFYMSG_SHIELDDISABLED	"El uso de escudos ha sido desactivado."
#define NOTIFYMSG_SHIELDENABLED		"El uso de escudos ha sido activado."
#define NOTIFYMSG_CHATDISABLED		"El chat ha sido desactivado. Solo puedes hablar con los GMs."
#define NOTIFYMSG_CHATENABLED		"El chat ha sido activado. Puedes volver a hablar nuevamente."
#define NOTIFYMSG_PARTYDISABLED		"El uso de grupos ha sido desactivado."
#define NOTIFYMSG_PARTYENABLED		"El uso de grupos ha sido activado."
#define NOTIFYMSG_EVENTRESET		"Todas las restricciones impuestas por el evento han sido removidas."
#define NOTIFYMSG_EVENTTPOPEN		"El teletransporte al evento esta disponible desde CityHall"
#define NOTIFYMSG_EVENTTPCLOSE		"El teletransporte al evento ha sido cerrado."
#define NOTIFYMSG_EVENTILLUSIONON		"El efecto illusion ha sido activado para el evento."
#define NOTIFYMSG_EVENTILLUSIONOFF		"El efecto illusion ha sido desactivado."

#define	NOTIFYMSG_CANCEL_EXCHANGEITEM1	"El intercambio ha sido cancelado!"
#define	NOTIFYMSG_CANCEL_EXCHANGEITEM2	"Tal vez el otro jugador lo canceló."


#define	NOTIFYMSG_CANNOT_GIVE_ITEM1	"%d %s: dropped to the ground. Imposible transferirselo a %s."
#define	NOTIFYMSG_CANNOT_GIVE_ITEM2	"El objeto %s ha sido tirado al piso. Imposible transferirselo a %s."


#define	NOTIFYMSG_THROW_ITEM1	"Tiraste %d %s al piso."
#define	NOTIFYMSG_THROW_ITEM2	"Tiraste el %s al piso."

#define	NOTIFYMSG_CANNOT_JOIN_MOREGUILDMAN1	"%s no puede unirse a tu gremio."
#define	NOTIFYMSG_CANNOT_JOIN_MOREGUILDMAN2	"El gremio esta lleno."


#define	NOTIFYMSG_DISMISS_GUILDMAN1	"%s salió de tu gremio."


#define	NOTIFYMSG_CANNOT_RATING1	"Debes ser un ciudadano amistoso para evaluar a otros."
#define	NOTIFYMSG_CANNOT_RATING2	"Puedes evaluar a otros personajes luego de %d segundos."


#define	NOTIFYMSG_CANNOT_REPAIR_ITEM1	"Item %s: No tienes que reparar este objeto."
#define	NOTIFYMSG_CANNOT_REPAIR_ITEM2	"Item %s: No puedes reparar este objeto aquí."



#define	NOTIFYMSG_CANNOT_SELL_ITEM1	"Item %s: No puedes vender este objeto aquí."
#define	NOTIFYMSG_CANNOT_SELL_ITEM2	"Item %s: No puedes vender un objeto gastado."
#define	NOTIFYMSG_CANNOT_SELL_ITEM3	"Item %s: No puedes vender este objeto."
#define	NOTIFYMSG_CANNOT_SELL_ITEM4	"Debes ser ciudadano para vender este objeto."
#define	NOTIFYMSG_CANNOT_SELL_ITEM5	"No puedes vender mas objetos porque tu mochila esta llena."
#define	NOTIFYMSG_CANNOT_SELL_ITEM6	"Intenta devuelta cuando liberes peso de tu mochila."


#define	NOTIFYMSG_CHARISMA_UP	"Tu carisma a aumentado %d Puntos."
#define	NOTIFYMSG_CHARISMA_DOWN	"Tu carisma a disminuido %d Puntos."


#define	NOTIFYMSG_DROPITEMFIN_ERASEITEM3	"Has dejado caer un %s."
#define	NOTIFYMSG_DROPITEMFIN_ERASEITEM4	"Dejaste caer %d %s."
#define	NOTIFYMSG_DROPITEMFIN_ERASEITEM5	"Dejaste caer %s." // Snoopy


#define	NOTIFYMSG_ENEMYKILL_REWARD1	"Mataste a %s!"
#define	NOTIFYMSG_ENEMYKILL_REWARD2	"Mataste a %s %s del gremio %s."
#define	EXP_DECREASED	"Tu experiencia disminuyo %d puntos."
#define	EXP_INCREASED	"Tu experiencia aumento %d puntos."
#define	NOTIFYMSG_ENEMYKILL_REWARD5	"Tu Enemy-Kill-Count disminuyo %d puntos."
#define	NOTIFYMSG_ENEMYKILL_REWARD6	"Tu Enemy-Kill-Count aumento %d puntos."


#define	NOTIFYMSG_EVENTFISHMODE1	"Comenzando a pescar..."


#define	NOTIFYMSG_EXCHANGEITEM_COMPLETE1	"Objeto intercambiado!"


#define	NOTIFYMSG_FORCE_DISCONN1	"Estas siendo forzado a desconectarte. No puedes cancelar esta cuenta regresiva."


#define	NOTIFYMSG_GIVEITEMFIN_COUNTCHANGED1	"Le diste %s a %s."
#define	NOTIFYMSG_GIVEITEMFIN_COUNTCHANGED2	"Le diste %d %s a %s."


#define	NOTIFYMSG_GIVEITEMFIN_ERASEITEM2	"Dejaste caer %d %s. Imposible de transferir."
#define	NOTIFYMSG_GIVEITEMFIN_ERASEITEM3	"Le diste %d %s al Warehouse Keeper."
#define	NOTIFYMSG_GIVEITEMFIN_ERASEITEM4	"Le diste %d %s al Guildhall Officer."
#define	NOTIFYMSG_GIVEITEMFIN_ERASEITEM5	"Le diste %d %s al Cityhall Officer."
#define	NOTIFYMSG_GIVEITEMFIN_ERASEITEM6	"Le diste %d %s al Sorcerer."
#define	NOTIFYMSG_GIVEITEMFIN_ERASEITEM7	"Le diste %d %s al Blacksmith Keeper."
#define	NOTIFYMSG_GIVEITEMFIN_ERASEITEM8	"Le diste %d %s a %s."


#define	NOTIFYMSG_GLOBAL_ATTACK_MODE1	"El modo de prohibición de ataque ha sido activado!"
#define	NOTIFYMSG_GLOBAL_ATTACK_MODE2	"Todo ataque es invalido en este mapa."
#define	NOTIFYMSG_GLOBAL_ATTACK_MODE3	"El modo de prohibición de ataque ha sido desactivado!"



#define	NOTIFYMSG_HUNGER1	"Estas sintiendo hambre..."
#define	NOTIFYMSG_HUNGER2	"Estas hambriento..."
#define	NOTIFYMSG_HUNGER3	"Estas muy habmbriento..."
#define	NOTIFYMSG_HUNGER4	"Estas famélico..."
#define	NOTIFYMSG_HUNGER5	"Estas muriendo del hambre!!!"


#define	NOTIFYMSG_ITEMCOLOR_CHANGE1	"Has pintado %s."
#define	NOTIFYMSG_ITEMCOLOR_CHANGE2	"El objeto %s: no puede ser pintado."

#define	NOTIFYMSG_ITESOCKET_CHANGE1	"Le agregaste un socket a %s."

#define	NOTIFYMSG_ITEMDEPlETED_ERASEITEM2	"Objeto %s: gastado."
#define	NOTIFYMSG_ITEMDEPlETED_ERASEITEM3	"Usaste %s."
#define	NOTIFYMSG_ITEMDEPlETED_ERASEITEM4	"Comiste %s."
#define	NOTIFYMSG_ITEMDEPlETED_ERASEITEM5	"Usaste %s."
#define	NOTIFYMSG_ITEMDEPlETED_ERASEITEM6	"El item %s se rompió."


#define	NOTIFYMSG_ITEMLIFE_SPANEND1	"Objeto %s: roto!"


#define	NOTIFYMSG_ITEMOBTAINED1	"Adquiriste %d %s."
#define	NOTIFYMSG_ITEMOBTAINED2	"Adquiriste %s."


#define	NOTIFYMSG_ITEMPURCHASED	"Compraste un %s con %d oro."

#define	NOTIFYMSG_ITEMREPAIRED1	"Objeto %s: reparado."
#define	NOTIFYMSG_ITEMREPAIRED2	"Objeto %s: parcialmente reparado."


#define	NOTIFYMSG_ITEMTOBANK2	"Objeto %d %s: guardado."
#define	NOTIFYMSG_ITEMTOBANK3	"Objeto %s: guardado"


//#define	NOTIFYMSG_KILLED1	"you have died!"
//#define	NOTIFYMSG_KILLED2	"You were killed by %s's attack!"
//#define	NOTIFYMSG_KILLED3	" Click the restart button in the system menu to start again"
//#define	NOTIFYMSG_KILLED4	" or click the Log Out button to exit"
// Snoopy: reduced by one line (my server sends a msg with killed ID)
#define	NOTIFYMSG_KILLED1	"Moriste!  Clickea el boton RESTART en el system menu "
#define	NOTIFYMSG_KILLED3	"para volver a empezar, o el boton Log Out para salir."



#define	NOTIFYMSG_LEVELUP1	"Level up!!! Nivel %d!"
#define	NOTIFYMSG_LEVELUP2	"Level up!"


#define	NOTIFYMSG_LIMITED_LEVEL1	" Trial version user can not rise anymore the experience!"//Usage??? Didn't translate it. Aphro~


#define	NOTIFYMSG_MAGICEFFECT_OFF1	"La protección contra las flechas ha terminado."
#define	NOTIFYMSG_MAGICEFFECT_OFF2	"La protección contra la magia ha terminado."
#define	NOTIFYMSG_MAGICEFFECT_OFF3	"La protección contra los golpes fisicos ha terminado."
#define	NOTIFYMSG_MAGICEFFECT_OFF4	"El efecto del Hold-Person a terminado."
#define	NOTIFYMSG_MAGICEFFECT_OFF5	"El efecto de paralisis ha terminado."
#define	NOTIFYMSG_MAGICEFFECT_OFF6	"El efecto de invisibilidad ha terminado."
#define	NOTIFYMSG_MAGICEFFECT_OFF7	"El efecto de confusión del lenguaje a terminado."
#define	NOTIFYMSG_MAGICEFFECT_OFF8	"El efecto de confusión a terminado."
#define	NOTIFYMSG_MAGICEFFECT_OFF9	"El efecto de ilusión a terminado."
#define	NOTIFYMSG_MAGICEFFECT_OFF10	"Envenenamiento curado."
#define	NOTIFYMSG_MAGICEFFECT_OFF11	"El efecto de berserk ha terminado"
#define	NOTIFYMSG_MAGICEFFECT_OFF12	"Transformation magic effect released."//Polymorph??? usage?? didn't translate it. Aphro~
#define	NOTIFYMSG_MAGICEFFECT_OFF13	"El efecto de conegelamiento ha terminado."
#define	NOTIFYMSG_MAGICEFFECT_OFF14 "La protección absoluta contra la magia ha terminado."

// Snoopy
#define	NOTIFYMSG_MAGICEFFECT_OFF15  "Al fin, tus sentidos han vuelto a la normalidad."



 #define	NOTIFYMSG_MAGICEFFECT_ON1	"Estas completamente protegido contra las flechas!"
 #define	NOTIFYMSG_MAGICEFFECT_ON2	"Estas protegido contra la magia!"
 #define	NOTIFYMSG_MAGICEFFECT_ON3	"Tu defensa física ha aumentado por un escudo magico!"
 #define	NOTIFYMSG_MAGICEFFECT_ON4	"Fuiste atrapado por el hechizo de Hold Person! Imposible moverte!"
 #define	NOTIFYMSG_MAGICEFFECT_ON5	"Fuiste atrapado por el hechizo de paralisis! Imposible moverte!"
 #define	NOTIFYMSG_MAGICEFFECT_ON6	"Eres invisible, nadie podra verte!"
 #define	NOTIFYMSG_MAGICEFFECT_ON7	"Nadie te entiende porque fuiste hechizado con la magia de Confuse Language!"
 #define	NOTIFYMSG_MAGICEFFECT_ON8	"Fuiste hechizado por la magia Confusion! Imposible determinar la lealtad!"
 #define	NOTIFYMSG_MAGICEFFECT_ON9	"Fuiste hechizado por la magia Illusion! Imposible saber quien es quien!"

#define	NOTIFYMSG_MAGICEFFECT_ON10	"Fuiste envenenado!"
//#define	NOTIFYMSG_MAGICEFFECT_ON11	"Berserk magic casted! You deal double physical damage for 40 seconds"
#define	NOTIFYMSG_MAGICEFFECT_ON11	"Magia Berserk lanzada!"
#define	NOTIFYMSG_MAGICEFFECT_ON12	"Transformation magic effect, you have been transformed as a monster"//Polymorph again.. Aphro~
#define	NOTIFYMSG_MAGICEFFECT_ON13	"Has sido congelado! Tu movimiento se reduce un 50%."
#define	NOTIFYMSG_MAGICEFFECT_ON14  "Estas completamente protegido contra la magia."
// Snoopy
 #define	NOTIFYMSG_MAGICEFFECT_ON15	"Te encuentras confundido y nervioso por culpa del hechizo!"



#define	NOTIFYMSG_MAGICSTUDY_FAIL1	"Magia (%s) :Error al tratar de aprenderla! Insuficiente oro o int!"
#define	NOTIFYMSG_MAGICSTUDY_FAIL2	"Magia (%s): Fallaste al aprenderla!"
#define	NOTIFYMSG_MAGICSTUDY_FAIL3	"Tu INT debe ser mayor a %d para aprenderla."
// CLEROTH
#define	NOTIFYMSG_MAGICSTUDY_FAIL4	"Error al aprender (%s)! Costo:%i  Int:%i  Str:%i"

#define	NOTIFYMSG_MAGICSTUDY_SUCCESS1	"Magia(%s): aprendida!"

#define	NOTIFYMSG_HP_UP	"Tu HP aumentó %d puntos."
#define	NOTIFYMSG_HP_DOWN	"Tu HP disminuyó %d puntos."
#define	NOTIFYMSG_MP_UP	"Tu MP aumentó %d puntos."
#define	NOTIFYMSG_MP_DOWN	"Tu MP disminuyó %d puntos."
#define	NOTIFYMSG_SP_UP	"Tu SP aumentó %d puntos."
#define	NOTIFYMSG_SP_DOWN	"Tu SP disminuyó %d puntos."
#define	NOTIFYMSG_HP2	"La cuenta regresiva se detiene si tu HP disminuye."
#define	NOTIFYMSG_HP3	"Cuidado! Tu HP esta muy baja!! Peligro!!"

#define	NOTIFYMSG_NEW_GUILDMAN1	"%s se unió a tu gremio."


#define	NOTIFYMSG_PK_CAPTURED1	"Nivel %d gano una batalla contra el criminal %s(%d)!"
#define	NOTIFYMSG_PK_CAPTURED3	"Podras recibir %d gold de premio."

#define	NOTIFYMSG_PK_PENALTY1	"Recibiste una penalización por matar a un ciudadano de tu nación! PK-Count(%d)"
#define	NOTIFYMSG_PK_PENALTY2	"Tu experiencia disminuyó %d puntos!"

#define	NOTIFYMSG_PLAYER_SHUTUP1	"Recibiste una penalización y no puedas hablar por %d minutos"
#define	NOTIFYMSG_PLAYER_SHUTUP2	"Le diste a %s una penalizació y no podrá hablar por %d minutos."


#define	NOTIFYMSG_PLAYER_STATUS1	"El jugador %s está conectado al servidor."
#define	NOTIFYMSG_PLAYER_STATUS2	"El jugador%s está conectado al %s(%d %d)."
#define	NOTIFYMSG_PLAYER_STATUS3    "El jugador %s no está conectado al servidor."


#define	NOTIFYMSG_QUEST_REWARD1	"Precio: %d puntos de experiencia"
#define	NOTIFYMSG_QUEST_REWARD2	"Precio: %d %s"
#define	NOTIFYMSG_QUEST_REWARD3	"La contribución ha aumentado %d puntos."
#define	NOTIFYMSG_QUEST_REWARD4	"La contribución ha disminuido %d puntos."


#define	NOTIFYMSG_RATING_PLAYER1	"Otro jugador te evaluó positivamente!."
#define	NOTIFYMSG_RATING_PLAYER2	"Le diste una evaluación positiva a %s!"
#define	NOTIFYMSG_RATING_PLAYER3	"Le diste una evaluación negativa a %s!"

#define	NOTIFYMSG_SHOW_MAP1	"No hay ningun mapa para esta area..."


#define	NOTIFYMSG_SKILL1	"La habilidad %s: aumentó %d%% puntos."
#define	NOTIFYMSG_SKILL2	"La habilidad %s: disminuyó %d%% puntos."


#define	NOTIFYMSG_SKILL_TRAIN_SUCCESS1	"Habilidad(%s): Has aprendido una nueva habilidad! El nivel de tu habilidad es de%d%%."


#define	NOTIFYMSG_SKILL_USINGEND1	"Has fallado al intentar de usar la habilidad!"
#define	NOTIFYMSG_SKILL_USINGEND2	"Has logrado usar la habilidad!"

#define	NOTIFYMSG_TOTAL_USER1	"Total de usuarios conectados: %d."



#define	NOTIFYMSG_EVENTSTART_CTR	"Ha comenzado el evento %s en Astoria!"
#define	NOTIFYMSG_EVENTSTART			" Puedes transportarte desde el Command Hall de tu ciudad."

#define	NOTIFYMSG_RELICGRABBED		"%s levanto la reliquia!!"
#define	NOTIFYMSG_RELICINALTAR		"La reliquia fue puesta en el altar de %s!!"
#define	NOTIFYMSG_EVENTWINNER			"%s ganó el evento %s."
#define	NOTIFYMSG_EVENTWINNERCTR	"As a citizen of the winning nation, you can now teleport to Middleland in City Hall."
#define	NOTIFYMSG_EVENTWINNERDTS	"As a citizen of the winning nation, you've gained an increase in drops from monsters"
#define	NOTIFYMSG_EVENTWINNERDTS2	"until the next Destroy the Shield event, and 50 Contribution."
#define	NOTIFYMSG_EVENTWINNERDTS2B	"until the next Destroy the Shield event."

#define	NOTIFYMSG_TELEPORT_REJECTED	"Cuidado!: Tus archivos de mapas no son correctos, asegurate de tenerlos actualizados."

#define	NOTIFYMSG_WHISPERMODE1	"Conversando en privado con %s."
#define	NOTIFYMSG_WHISPERMODE2	"Modo de conversacion privada, desactivado."


#define	DRAWDIALOGBOX_CRUSADEJOB1	"Decide tu labor."
#define	DRAWDIALOGBOX_CRUSADEJOB2	"Los labores están indicados"
#define	DRAWDIALOGBOX_CRUSADEJOB3	"en la parte de abajo."
#define	DRAWDIALOGBOX_CRUSADEJOB4	"Elije la labor que desees."

#define	DRAWDIALOGBOX_CRUSADEJOB5	"Comandante"
#define	DRAWDIALOGBOX_CRUSADEJOB6	"Comandante"
#define	DRAWDIALOGBOX_CRUSADEJOB7	"Soldado"
#define	DRAWDIALOGBOX_CRUSADEJOB8	"Soldado"
#define	DRAWDIALOGBOX_CRUSADEJOB9	"Constructor"
#define	DRAWDIALOGBOX_CRUSADEJOB10	"Clickea el boton HELP que está"
#define	DRAWDIALOGBOX_CRUSADEJOB17	"abajo para ver los detalles."
#define	DRAWDIALOGBOX_CRUSADEJOB18	"Tu labor en esta crusada sera de"
#define	DRAWDIALOGBOX_CRUSADEJOB19	"Soldado"
#define	DRAWDIALOGBOX_CRUSADEJOB20	"Constructor."
#define	DRAWDIALOGBOX_CRUSADEJOB21	"Comandante."
#define	DRAWDIALOGBOX_CRUSADEJOB22	"Para un informe mas detallado"
#define	DRAWDIALOGBOX_CRUSADEJOB23	"Clickee aquí."
#define	DRAWDIALOGBOX_CRUSADEJOB25	"Si quieres cambiar tu labor, ve"
#define	DRAWDIALOGBOX_CRUSADEJOB26	"con el cityhall officer."

#define	SHOW_EVENT_LIST1	"Estas usando una habilidad..."


#define	REQUEST_TELEPORT_AND_WAIT_DATA1	"No puedes entrar al beginner dungeon. Solo los jugadores de 30 a 80 pueden entrar."
#define	REQUEST_TELEPORT_AND_WAIT_DATA2 "No puedes entrar a las estructuras enemigas."


#define	INITDATA_RESPONSE_HANDLER1	"Esto es %s."


#define	MOTION_EVENT_HANDLER1	"Cambiado a modo de ataque."
#define	MOTION_EVENT_HANDLER2	"Cambiado a modo de paz."


#define	DRAW_DIALOGBOX_COMMANDER1	"Ubicación de las estructuras en Middleland."
#define	DRAW_DIALOGBOX_COMMANDER2	"Ubicación del teletransporte a Middleland."
#define	DRAW_DIALOGBOX_COMMANDER3	"Ir al lugar seleccionado."
#define	DRAW_DIALOGBOX_COMMANDER4	"Quitar las unidades de guerra."
#define	DRAW_DIALOGBOX_COMMANDER5	"Ubicación del punto de construcción en Middleland."
#define	DRAW_DIALOGBOX_COMMANDER6	"El deber del Comandante."
#define	DRAW_DIALOGBOX_COMMANDER7	"Elije y clickea la ubicación deseada."
#define	DRAW_DIALOGBOX_COMMANDER8	"Volver al menu principal para comandantes."
#define	DRAW_DIALOGBOX_COMMANDER9	"Ayuda para la ubicación del teletransporte a Middleland."
#define	DRAW_DIALOGBOX_COMMANDER10	"Ir al lugar seleccionado."
#define	DRAW_DIALOGBOX_COMMANDER11	"Confirmación del teletransporte. (Clickea una vez más.)"
#define	DRAW_DIALOGBOX_COMMANDER12	"Volver al menu principal para comandantes."
#define	DRAW_DIALOGBOX_COMMANDER13	"Ayuda para mover el teletransporte de Middleland."
#define	DRAW_DIALOGBOX_COMMANDER14	"Quitar las unidades de guerra."
#define	DRAW_DIALOGBOX_COMMANDER15	"Recall Points actuales:"
#define	DRAW_DIALOGBOX_COMMANDER16	"Battle Golem"
#define	DRAW_DIALOGBOX_COMMANDER17	"Recall point 3000"
#define	DRAW_DIALOGBOX_COMMANDER18	"Temple Knight"
#define	DRAW_DIALOGBOX_COMMANDER19	"Recall point 2000"
#define	DRAW_DIALOGBOX_COMMANDER20	"Light War Beetle"
#define	DRAW_DIALOGBOX_COMMANDER21	"Recall point 1000"
#define	DRAW_DIALOGBOX_COMMANDER22	"Catapult"
#define	DRAW_DIALOGBOX_COMMANDER23	"Recall point 1500"
#define	DRAW_DIALOGBOX_COMMANDER24	"Seguir al creador en modo seguro."
#define	DRAW_DIALOGBOX_COMMANDER25	"Proteger la región seleccionada."
#define	DRAW_DIALOGBOX_COMMANDER26	"Volver al menu principal para comandantes."
#define	DRAW_DIALOGBOX_COMMANDER27	"Ayuda para las unidades de guerra."
#define	DRAW_DIALOGBOX_COMMANDER28	"God's Hand Knight Cavalry"
#define	DRAW_DIALOGBOX_COMMANDER29	"Recall point 3000"
#define	DRAW_DIALOGBOX_COMMANDER30	"God's Hand Knight"
#define	DRAW_DIALOGBOX_COMMANDER31	"Recall point 2000"
#define	DRAW_DIALOGBOX_COMMANDER32	"Light War Beetle"
#define	DRAW_DIALOGBOX_COMMANDER33	"Recall point 1000"
#define	DRAW_DIALOGBOX_COMMANDER34	"Catapult"
#define	DRAW_DIALOGBOX_COMMANDER35	"Recall point 1500"
#define	DRAW_DIALOGBOX_COMMANDER36	"Seguir al creador en modo seguro."
#define	DRAW_DIALOGBOX_COMMANDER37	"Proteger la región seleccionada."
#define	DRAW_DIALOGBOX_COMMANDER38	"Volver al menu principal para comandantes."
#define	DRAW_DIALOGBOX_COMMANDER39	"Ayuda para las unidades de guerra."
#define	DRAW_DIALOGBOX_COMMANDER40	"Elije la unidad deseada."
#define	DRAW_DIALOGBOX_COMMANDER41	"Clickea el icono de la unidad. Para más detalles."
#define	DRAW_DIALOGBOX_COMMANDER42	"Para ver los contenidos presiona el botón help."
#define	DRAW_DIALOGBOX_COMMANDER43	"Guard Mode"
#define	DRAW_DIALOGBOX_COMMANDER44	"Hold Mode"
#define	DRAW_DIALOGBOX_COMMANDER47	"Elije la región de construcción y clickeala."
#define	DRAW_DIALOGBOX_COMMANDER48	"Volver al menu principal para comandantes."
#define	DRAW_DIALOGBOX_COMMANDER49	"Ayuda para ubicar el punto de construcción."


#define	DRAW_DIALOGBOX_CONSTRUCTOR1	"Ubicacion de la construcción: %s cerca de %d,%d"
#define	DRAW_DIALOGBOX_CONSTRUCTOR2	"Punto de construcción no disponible: Imposible construir."
#define	DRAW_DIALOGBOX_CONSTRUCTOR3	"Elije una estructura."
#define	DRAW_DIALOGBOX_CONSTRUCTOR4	"El comandante elije el lugar para el teletransporte."
#define	DRAW_DIALOGBOX_CONSTRUCTOR5	"Ayuda para el labor de Constructor."
#define	DRAW_DIALOGBOX_CONSTRUCTOR6	"Elije una estructura."
#define	DRAW_DIALOGBOX_CONSTRUCTOR7	"Construye las estructuras aquí."
#define	DRAW_DIALOGBOX_CONSTRUCTOR8	"Presiona el icono de la estructura,"
#define	DRAW_DIALOGBOX_CONSTRUCTOR9	"la base se creara y se ira formando lentamente"
#define	DRAW_DIALOGBOX_CONSTRUCTOR10	"Construye la estructura con la Pick Axe."
#define	DRAW_DIALOGBOX_CONSTRUCTOR11	"La construcción no esta activada."
#define	DRAW_DIALOGBOX_CONSTRUCTOR12	"Mientras construyes.."
#define	DRAW_DIALOGBOX_CONSTRUCTOR13	"Mana Collector"
#define	DRAW_DIALOGBOX_CONSTRUCTOR14	"Detector"
#define	DRAW_DIALOGBOX_CONSTRUCTOR15	"Arrow Guard Tower"
#define	DRAW_DIALOGBOX_CONSTRUCTOR16	"Cannon Guard Tower"
#define	DRAW_DIALOGBOX_CONSTRUCTOR17	"Back to Construction menu"
#define	DRAW_DIALOGBOX_CONSTRUCTOR18	"Ayuda para construir."
#define	DRAW_DIALOGBOX_CONSTRUCTOR19	"Ir al lugar que el comandante asigno."
#define	DRAW_DIALOGBOX_CONSTRUCTOR20	"Confirmación del teletransporte. (Clickea una vez más.)"
#define	DRAW_DIALOGBOX_CONSTRUCTOR21	"Volver al modo de constructor."
#define	DRAW_DIALOGBOX_CONSTRUCTOR22	"Ayuda para el teletransporte."


#define DRAW_DIALOGBOX_ITEM_DROP1   "¿Estas seguro que quieres dejar caer esto?"
#define DRAW_DIALOGBOX_ITEM_DROP2   "No preguntar para el mismo item. Activado"
#define DRAW_DIALOGBOX_ITEM_DROP3   "No preguntar para el mismo item. Desactivado"


#define	DRAW_DIALOGBOX_SOLDIER1	"Posicion del teletransporte: %s cerca de %d, %d"
#define	DRAW_DIALOGBOX_SOLDIER2	"Punto de teletransporte no disponible."
#define	DRAW_DIALOGBOX_SOLDIER3	"Ir al lugar que el comandante ha asignado."
#define	DRAW_DIALOGBOX_SOLDIER4	"Ayuda para la labor de Soldado."
#define	DRAW_DIALOGBOX_SOLDIER5	"Ir al lugar que el comandante ha asignado."
#define	DRAW_DIALOGBOX_SOLDIER6	"Confirmación del teletransporte. (Clickea una vez más.)"
#define	DRAW_DIALOGBOX_SOLDIER7	"Volver al modo de soldado."
#define	DRAW_DIALOGBOX_SOLDIER8	"Ayuda para el teletransporte."

#define	DRAW_DIALOGBOX_HELP1	"¿Mundo del Helbreath?"
#define	DRAW_DIALOGBOX_HELP2	"Noticias"
#define	DRAW_DIALOGBOX_HELP3	"¿Como moverse?"
#define	DRAW_DIALOGBOX_HELP4	"Ataque, Defensa, Enemigos, Amigos"
#define	DRAW_DIALOGBOX_HELP5	"Interfaz"
#define	DRAW_DIALOGBOX_HELP6	"Magias"
#define	DRAW_DIALOGBOX_HELP7	"Puntos especificos, sistema de habilidades"
#define	DRAW_DIALOGBOX_HELP8	"Gremios"
#define	DRAW_DIALOGBOX_HELP9	"Objetos"
#define	DRAW_DIALOGBOX_HELP10	"Comunicación"
#define	DRAW_DIALOGBOX_HELP11	"¿Que es una Crusada?"
#define	DRAW_DIALOGBOX_HELP12	"Comandos"
#define	DRAW_DIALOGBOX_HELP13	"Guia para principiantes"

#define NOTIFYMSG_CREATE_SCREENSHOT1 "Screenshot guardada en %s."
#define NOTIFYMSG_CREATE_SCREENSHOT2 "No puedes guardar mas Screenshots."

#define DRAW_DIALOGBOX_ITEMUPGRADE1 "Arrastra un objeto de"
#define DRAW_DIALOGBOX_ITEMUPGRADE2 "tu inventario. Luego presiona el"
#define DRAW_DIALOGBOX_ITEMUPGRADE3 "boton UPGRADE."
#define DRAW_DIALOGBOX_ITEMUPGRADE5 "El mejoramiento del objeto esta en proceso..."
#define DRAW_DIALOGBOX_ITEMUPGRADE6 "Espere un momento..."
#define DRAW_DIALOGBOX_ITEMUPGRADE7 "Felicitaciones! El mejoramiento"
#define DRAW_DIALOGBOX_ITEMUPGRADE8 "del objeto ha sido exitoso."
#define DRAW_DIALOGBOX_ITEMUPGRADE9 "El mejoramiento del objeto falló."
#define DRAW_DIALOGBOX_ITEMUPGRADE11 "Puntos para mejorar: %d"
#define DRAW_DIALOGBOX_ITEMUPGRADE12 "Puntos necesarios: %d"

#define DRAW_DIALOGBOX_ITEMUPGRADE13 "Elije que tipo de mejoramiento quieres."
#define DRAW_DIALOGBOX_ITEMUPGRADE14 "Mejoramiento normal"
#define DRAW_DIALOGBOX_ITEMUPGRADE15 "Mejoramiento por medio de Maj. Pts."
#define DRAW_DIALOGBOX_ITEMUPGRADE16 "Mejorando objetos de manera normal."
#define DRAW_DIALOGBOX_ITEMUPGRADE17 "Para mejorar armas, es"
#define DRAW_DIALOGBOX_ITEMUPGRADE18 "necesario una Stone of"
#define DRAW_DIALOGBOX_ITEMUPGRADE19 "Xelima. Para mejorar"
#define DRAW_DIALOGBOX_ITEMUPGRADE20 "armaduras, es necesario"
#define DRAW_DIALOGBOX_ITEMUPGRADE21 "una Stone of Merien."
#define DRAW_DIALOGBOX_ITEMUPGRADE22 "Mejorando objetos con Maj. Pts."
#define DRAW_DIALOGBOX_ITEMUPGRADE23 "Para mejorar objetos con Maj. Pts,"
#define DRAW_DIALOGBOX_ITEMUPGRADE24 "es necesario tener los Puntos"
#define DRAW_DIALOGBOX_ITEMUPGRADE25 "requeridos para hacer el mejoramiento."
#define DRAW_DIALOGBOX_ITEMUPGRADE26 "*Los objetos mejorables con Maj.Pts."
#define DRAW_DIALOGBOX_ITEMUPGRADE27 "No pueden ser mejorados de esta forma."
#define DRAW_DIALOGBOX_ITEMUPGRADE28 "*Los objetos normales no pueden"
#define DRAW_DIALOGBOX_ITEMUPGRADE29 "ser mejorados de esta forma."
#define DRAW_DIALOGBOX_ITEMUPGRADE30 "No se encuentra una Xelima o Merien."
#define DRAW_DIALOGBOX_ITEMUPGRADE31 "Luego de arrastrar un objeto de tu"
#define DRAW_DIALOGBOX_ITEMUPGRADE32 "invenetario, presiona el boton "
#define DRAW_DIALOGBOX_ITEMUPGRADE33 "UPGRADE."
#define DRAW_DIALOGBOX_ITEMUPGRADE34 "Objetos mejorables(Armas): %d"
#define DRAW_DIALOGBOX_ITEMUPGRADE35 "Objetos mejorables(Armaduras): %d"
#define DRAW_DIALOGBOX_ITEMUPGRADE36 "Debido a que el mejoramiento falló,"
#define DRAW_DIALOGBOX_ITEMUPGRADE37 "el objeto desaparecio."


#define DRAW_DIALOGBOX_ITEMUPGRADE38 "No puedes mejorar mas este objeto."
#define DRAW_DIALOGBOX_ITEMUPGRADE39 "No se pudo mejorar este objeto."
#define DRAW_DIALOGBOX_ITEMUPGRADE40 "No tienes los puntos necesarios para mejorar."
#define DRAW_DIALOGBOX_ITEMUPGRADE41 "Solo puedes mejorar armaduras."
#define DRAW_DIALOGBOX_ITEMUPGRADE42 "Solo puedes mejorar armas."

#define NOTIFY_MSG_HANDLER_GIZONITEMUPGRADELEFT1 "Has ganado un Majestic Point."

#define DLGBOXCLICK_INVENTORY1	"Para manufacturar un objeto primero debes aprender la habilidad."
#define DLGBOXCLICK_INVENTORY2	"El manual para aprender la habilidad esta disponible en Shop."

#define NOTIFY_MSG_CONVERT_WALKING_MODE "Convertido a modo de caminar."
#define NOTIFY_MSG_CONVERT_RUNNING_MODE "Converitdo a modo de correr."
#define NOTIFY_MSG_MUSIC_OFF			"Musica apgada"
#define NOTIFY_MSG_MUSIC_ON				"Music encendida"
#define NOTIFY_MSG_SOUND_OFF			"Sonido apagado"
#define NOTIFY_MSG_SOUND_ON				"Sonido encendido"

#define NOTIFY_MSG_STRUCTURE_HP         "Durabilidad de la estructura:"
#define NOTIFY_MSG_CRUSADESTART_NONE    "Los principiantes no pueden ir a la ciudad durante una batalla."

#define NOTIFY_MSG_DETAIL_LEVEL_LOW		"Nivel de detalle: Bajo"
#define NOTIFY_MSG_DETAIL_LEVEL_MEDIUM	"Nivel de detalle: Medio"
#define NOTIFY_MSG_DETAIL_LEVEL_HIGH	"Nivel de detalle: Alto"

#define	MSG_SHORTCUT1	"No hay nada grabado aquí."
#define	MSG_SHORTCUT2	"Para grabar un objeto o una magia en F%d."
#define	MSG_SHORTCUT3	"Tira la magia o equipate el objeto y luego presion [control]-[F%d] para grabarlo."
#define	MSG_SHORTCUT4	"El objeto (%s %s %s) ha sido grabado en [F%d]."
#define	MSG_SHORTCUT5	"La magia (%s) ha sido grabada en [F%d]."


#define NOTIFY_MSG_FORCERECALLTIME1 "Tienes %d minutos antes de ser forzado a volver. La magia Recall no funciona en ciudades enemigas."
#define NOTIFY_MSG_FORCERECALLTIME2 "Pronto seras forzado a volver."

#define MSG_TOTAL_NUMBER "Total : %d"

#define MSG_EXP	"Exp: "

#define MSG_CHARACTERNAME	"Nombre"
#define MSG_GENDER			"Sexo"
#define MSG_SKINCOLOR		"Color de piel"
#define MSG_HAIRSTYLE		"Estilo de pelo"
#define MSG_HAIRCOLOR		"Color de pelo"
#define MSG_UNDERWEARCOLOR	"Color de ropa interior"
#define MSG_STRENGTH		"Strength"
#define MSG_VITALITY		"Vitality"
#define MSG_DEXTERITY		"Dexterity"
#define MSG_INTELLIGENCE	"Intelligence"
#define MSG_MAGIC			"Magic"
#define MSG_CHARISMA		"Charisma"
#define MSG_HITPOINT		"Hit Point"
#define MSG_MANAPOINT		"Mana Point"
#define MSG_STAMINARPOINT	"Staminar Point"

#define MSG_GETNEXTMOVEDIR	"Un civil no puede ir a lugares peligrosos. Debes cambiar tu estado en el CityHall."
#define MSG_GUIDEMAP_MIN	"(-) : Minimizar mapa"
#define MSG_GUIDEMAP_MAX	"(+) : Maximizar mapa"
#define MSG_MAPNAME_CITYHALL	"Cityhall"
#define MSG_MAPNAME_MAGICTOWER	"Magicshop"
#define MSG_MAPNAME_DUNGEON		"Dungeon"
#define MSG_MAPNAME_WAREHOUSE	"Warehouse"
#define MSG_MAPNAME_BARRACK		"Barrack"
#define MSG_MAPNAME_GUILDHALL	"GuildHall"
#define MSG_MAPNAME_SHOP		"Shop"
#define MSG_MAPNAME_BLACKSMITH	"BlackSmith"
#define MSG_MAPNAME_CATH		"Church"
//Snoopy
#define MSG_MAPNAME_CMDHALL		"Command Hall"
#define MSG_MAPNAME_FARM		"Entrada a Farm"
#define MSG_MAPNAME_HUNT		"Entrada a Hunt-Zone"
#define MSG_MAPNAME_ML			"Entrada a MiddleLand"

#define MSG_WARNING1	" ** Esta es un area de batalla **"
#define MSG_WARNING2	"Esta es un area peligrosa donde no"
#define MSG_WARNING3	"te puedes proteger de los ataques."
#define MSG_WARNING4	"Para jugar en modo a salvo, debes ir"
#define MSG_WARNING5	"al cityhall y cambiar tu modo."

#define	MSG_SAFEZONE	"Ésta es un area segura."
#define	MSG_DANGERZONE	"No puedes ser protegido de los ataques de otros en esta area."

#define	MSG_COMBATANT	" Combatiente"
#define MSG_PK			" Criminal"

#define MSG_FORCEATTACK_ON	"Modo de auto ataque activado."
#define MSG_FORCEATTACK_OFF	"Modo de auto ataque desactivado."

#define DRAW_DIALOGBOX_CITYHALL_MENU56	"Cambiar el modo de juego."
#define DRAW_DIALOGBOX_CITYHALL_MENU46A "¿Te gustaría recibir este objeto?"

#define DRAW_DIALOGBOX_CITYHALL_MENU57	"*Eres un civil ahora.*"
#define DRAW_DIALOGBOX_CITYHALL_MENU58	"*Eres un combatiente ahora.*"
#define DRAW_DIALOGBOX_CITYHALL_MENU59	"(El modo de civil te mantiene seguro.)"

#define DRAW_DIALOGBOX_CITYHALL_MENU60	"Civil:"
#define DRAW_DIALOGBOX_CITYHALL_MENU61	"Estas a salvo del enemigo"
#define DRAW_DIALOGBOX_CITYHALL_MENU62	"y puedes subir de nivel"
#define DRAW_DIALOGBOX_CITYHALL_MENU63	"tranquilo."

#define DRAW_DIALOGBOX_CITYHALL_MENU64	"Combatiente:"
#define DRAW_DIALOGBOX_CITYHALL_MENU65	"No estas a salvo del enemigo."
#define DRAW_DIALOGBOX_CITYHALL_MENU66	"No puedes ser protegido en"
#define DRAW_DIALOGBOX_CITYHALL_MENU67	"el modo de combatiente."

#define DRAW_DIALOGBOX_CITYHALL_MENU68	"¿Te gustaría cambiar el modo de juego?"

#define MSG_GAMEMODE_CHANGED		"Modo de juego cambiado."

#define DRAW_DIALOGBOX_CITYHALL_MENU69	"Teletransporte a Dungeon level 2."
#define DRAW_DIALOGBOX_CITYHALL_MENU70	"Es requerido 5000 gold"
#define DRAW_DIALOGBOX_CITYHALL_MENU71	"para usar el teletransporte."
#define DRAW_DIALOGBOX_CITYHALL_MENU72	"¿Quieres teletransportarte?"
#define DRAW_DIALOGBOX_CITYHALL_MENU72_1 "Los civiles no pueden ir a algunas areas."

#define DRAW_DIALOGBOX_CITYHALL_MENU73	"Buscando las posibles areas para el teletransporte."
#define DRAW_DIALOGBOX_CITYHALL_MENU74	"Por favor espere un momento."
#define DRAW_DIALOGBOX_CITYHALL_MENU76	"No hay ningun area disponible."

#define DRAW_DIALOGBOX_CITYHALL_MENU77  "Mapa: %s Costo: %d"

#define MSG_NOTIFY_AGRICULTURENOAREA	"Esta no es un area para sembrar."
#define MSG_NOTIFY_AGRICULTURESKILLLIMIT	"El intento de cultivo no es posible debido a tu habilidad de Farm."
#define MSG_NOTIFY_NOMOREAGRICULTURE	"No puedes sembrar más."


#define	TALKING_TO_MCGAFFIN				"Hablandole a un hombre del pueblo..."
#define	TALKING_TO_PERRY				"Hablandole a una doncella del pueblo..."
#define	TALKING_TO_DEVLIN				"Hablandole a un mago del pueblo..."

#define RESPONSE_CHARGED_TELEPORT1 "Tu nivel es muy bajo para ir a esa area."
#define RESPONSE_CHARGED_TELEPORT2 "Los civiles no pueden ir a esa area."
#define RESPONSE_CHARGED_TELEPORT3 "Los viajeros no pueden ir a esa area."
#define RESPONSE_CHARGED_TELEPORT4 "Los criminales no pueden ir a esa area."
#define RESPONSE_CHARGED_TELEPORT5 "No puedes teletransportarte a la misma ciudad."
#define RESPONSE_CHARGED_TELEPORT6 "Necesitas mas oro para teletransportarte."
#define RESPONSE_CHARGED_TELEPORT7 ".........................."

#define MSG_NOTIFY_CHECKSUM "Error al cargar los archivos!Consiga la ultima actualización!"
#define MSG_NOTIFY_LOGIN "El login.cfg contiene información erronea."
#define MSG_NOTIFY_ITEMNAME "El Item.cfg contiene información erronea."
#define MSG_NOTIFY_MAGICCFG "El MAGICCFGTXT.cfg contiene información erronea."
#define MSG_NOTIFY_SKILLCFG "El SKILLCFGTXT.cfg contiene información erronea."
#define MSG_NOTIFY_DIRECTX7 "Este programa requiere DirectX7.0a!"

#define MSG_NOTIFY_LEGIONPT1 "No tienes suficientes Xtreme Points!."
#define MSG_NOTIFY_LEGIONPT2 " Dirigete a http://www.helbreathx.net/donate.php"
#define MSG_NOTIFY_LEGIONPT3 " Para informarte como obtener los puntos."

#define MSG_NOTIFY_HELP "Presiona F1 para ver noticias y ayuda."

#define MSG_FRIENDSLIST1 "No puedes tener mas de 12 amigos en la lista."
#define MSG_FRIENDSLIST2 "Clickea al personaje que deseas agregar."
#define MSG_FRIENDSLIST3 "Lista de amigos"
#define MSG_FRIENDSLIST4 "Conectado"
#define MSG_FRIENDSLIST4B "Desconectado"
#define MSG_FRIENDSLIST5 "Nombre personaje: %s"
#define MSG_FRIENDSLIST6 "Nombre personaje: "
#define MSG_FRIENDSLIST7 "Agregar"
#define MSG_FRIENDSLIST8 "Borrar"
#define MSG_FRIENDSLIST9 "Enviar mensaje personal"

#define DRAW_CMDHALL1 "Teletransporte a BattleField"
#define DRAW_CMDHALL2 "Contrate un soldado."
#define DRAW_CMDHALL3 "Sacar Banderas"
#define DRAW_CMDHALL4 "Recivir un Tutelary Angel"
#define DRAW_CMDHALL5 "Contrataras un soldado usando summon points"
#define DRAW_CMDHALL6 "Sorceress				2000 Point"
#define DRAW_CMDHALL7 "Ancient Temple Knight	3000 Point"
#define DRAW_CMDHALL8 "Elf Master				1500 Point"
#define DRAW_CMDHALL9 "Dark Shadow Knight		3000 Point"
#define DRAW_CMDHALL10 "Heavy Battle Tank      4000 Point"
#define DRAW_CMDHALL11 "Barbarian              3000 Point"
#define DRAW_CMDHALL12 "Debes unirte a un gremio para contratar soldados."
#define DRAW_CMDHALL13 "Summon points : %d"
#define DRAW_CMDHALL14 "12000 Maximum summon points."
#define DRAW_CMDHALL15 "Scrolls maximos 5."
#define DRAW_CMDHALL16 "Obtendras banderas con puntos de EK."
#define DRAW_CMDHALL17 "El precio es de 10EK por bandera."
#define DRAW_CMDHALL18 "Sacar bandera."
//DEJE ACA
#define DRAW_CMDHALL19 "5 Majestic Points seran reducidos de tu"
#define DRAW_CMDHALL20 "cuenta si recibes el Tutelary Angel."
#define DRAW_CMDHALL21 "¿Deseas recibir el Ángel?"
#define DRAW_CMDHALL22 "Te sera entregado un Tutelary Angel (STR)"
#define DRAW_CMDHALL23 "Te sera entregado un Tutelary Angel (DEX)"
#define DRAW_CMDHALL24 "Te sera entregado un Tutelary Angel (INT)"
#define DRAW_CMDHALL25 "Te sera entregado un Tutelary Angel (MAG)."

#define DRAW_BLACKSMITH "Blacksmith Keeper"
#define DRAW_BLACKSMITH1 "Comprar Armas/Misc."
#define DRAW_BLACKSMITH2 "Comprar Armaduras (M)"
#define DRAW_BLACKSMITH3 "Comprar Armaduras (W)"
#define DRAW_BLACKSMITH4 "Vender objetos"

#define DRAW_SHOP "Shop Keeper"
#define DRAW_SHOP1 "Comprar pociones"
#define DRAW_SHOP2 "Comprar Misc."
#define DRAW_SHOP3 "Comprar ropas"
#define DRAW_SHOP4 "Vender objetos"

#define DRAW_CITYHALL "Cityhall Officer"
#define DRAW_CITYHALL1 "Ofrecer"
#define DRAW_CITYHALL2 "Comprar armas (DK)"
#define DRAW_CITYHALL3 "Hablar" 

#define DRAW_MANASAVEINFO "Cuadro de ahorro de mana"

#define DRAW_MANASAVEINFO1 "Varas y collares con 10% ahorro de mana"
#define DRAW_MANASAVEINFO2 "MagicWand(MS10), WizMagicWand(MS10)"	
#define DRAW_MANASAVEINFO3 "MagicNecklace(MS10), BerserkWand(MS.10)" 
#define DRAW_MANASAVEINFO4 "KlonessWand(MS.10), ResurWand(MS.10)"

#define DRAW_MANASAVEINFO5 "Varas y collares con 20% ahorro de mana"
#define DRAW_MANASAVEINFO6 "MagicWand(MS20), NecklaceOfLiche"	
#define DRAW_MANASAVEINFO7 "BerserkWand(MS.20), KlonessWand(MS.20)" 
#define DRAW_MANASAVEINFO8 "ResurWand(MS.20), WizMagicWand(MS20)"

#define DRAW_MANASAVEINFO9 "Varas y collares con 30% ahorro de mana"
#define DRAW_MANASAVEINFO10 "MagicWand(MS30-LLF)"

#define DRAW_MANASAVEINFO11 "Collares magicos con ahorro variado"
#define DRAW_MANASAVEINFO12 "MagicNecklace(MS12) ManaSave 12%"
#define DRAW_MANASAVEINFO13 "MagicNecklace(MS14) ManaSave 14%"
#define DRAW_MANASAVEINFO14 "MagicNecklace(MS16) ManaSave 16%"
#define DRAW_MANASAVEINFO15 "MagicNecklace(MS18) ManaSave 18%"	

#define DRAW_MANASAVEINFO16 "Varas con ahorro variado"
#define DRAW_MANASAVEINFO17 "DMMagicStaff ManaSave 22%"
#define DRAW_MANASAVEINFO18 "DMMagicWand ManaSave 25%"
#define DRAW_MANASAVEINFO19 "BlackMageTemple ManaSave 28%"

#define CRUSADE_MESSAGE0 "El generador magico de Aresden ha sido activado. Pronto la magia caerá! Evacuen!"
#define CRUSADE_MESSAGE1 "Pronto la lluvia de meteoros comenzará. Evacuen!"
#define CRUSADE_MESSAGE2 "Información sobre el daño del ataque de Elvine"
#define CRUSADE_MESSAGE3 "Contenido del daño de Aresden"
#define CRUSADE_MESSAGE4 "Cantidad de estructuras destruidas:"
#define CRUSADE_MESSAGE5 "Estructuras dañadas:"
#define CRUSADE_MESSAGE6 "Cantidad de muertes:"
#define CRUSADE_MESSAGE7 "Información sobre el daño del ataque de Aresden"
#define CRUSADE_MESSAGE8 "Contenido del daño de Elvine"
#define CRUSADE_MESSAGE9 "La batalla entre Aresden y Elvine ha comenzado!"
#define CRUSADE_MESSAGE10 "Aresden's Grand Magic generator was activated."
#define CRUSADE_MESSAGE11 "El generador de Elvine ha sido activado."
#define CRUSADE_MESSAGE12 "Limite de estructuras!Destruya alguna y vuelva a construir."
#define CRUSADE_MESSAGE13 "Puntos de unidad de guerra."
#define CRUSADE_MESSAGE14 "El comandante todavia no ha elejido el punto de construcción."
#define CRUSADE_MESSAGE15 "El comandante todavia no ha elejido el punto de teletransporte."
#define CRUSADE_MESSAGE16 "No puede usar esta habilidad mientras el mapa esta abierto."
#define CRUSADE_MESSAGE17 "Ataque de meteoros!"
#define CRUSADE_MESSAGE18 "Estas muy cerca de la otra estructura. Elije otro lugar."
#define CRUSADE_MESSAGE19 "Solo puedes construir cerca del punto de construcción."
#define CRUSADE_MESSAGE20 "Construcción límite de cantidad"
#define CRUSADE_MESSAGE20B "Solo puedes evocar unidades de guerra en Aresden, Elvine y Middleland."
#define CRUSADE_MESSAGE21 "All out war contribution"
#define CRUSADE_MESSAGE22 "Felicitaciones! Tu nación"
#define CRUSADE_MESSAGE23 "ha sido victoriosa!"
#define CRUSADE_MESSAGE24 "Por ser un ciudadano"
#define CRUSADE_MESSAGE25 "victorioso, recibirás"
#define CRUSADE_MESSAGE26 "un premio."
#define CRUSADE_MESSAGE27 "Puntos de experiencia de contribución con la guerra:"
#define CRUSADE_MESSAGE28 "Desafortunadamente! Tu nación"
#define CRUSADE_MESSAGE29 "ha perdido la guerra."
#define CRUSADE_MESSAGE30 "Por ser un ciudadano,"
#define CRUSADE_MESSAGE31 "perdedor, no recibirás"
#define CRUSADE_MESSAGE32 "ningun premio."
#define CRUSADE_MESSAGE33 "Tal vez seas victorioso"
#define CRUSADE_MESSAGE34 "en la proxima batalla."
#define CRUSADE_MESSAGE35 "La guerra ha terminado."
#define CRUSADE_MESSAGE36 "Ha habido un empate entre" 
#define CRUSADE_MESSAGE37 "las naciones."
#define CRUSADE_MESSAGE38 "Aresden ha sido victorioso"
#define CRUSADE_MESSAGE39 "y le puso fin a la guerra."
#define CRUSADE_MESSAGE40 "Elvine fue victorioso"
#define CRUSADE_MESSAGE41 "y le puso fin a la guerra."
#define CRUSADE_MESSAGE42 "Felicitaciones"
#define CRUSADE_MESSAGE43 "Por ser un ciudadano"
#define CRUSADE_MESSAGE44 "victorioso, recibiras"
#define CRUSADE_MESSAGE45 "un premio."
#define CRUSADE_MESSAGE46 "Desafortunadamente,"
#define CRUSADE_MESSAGE47 "como ciudadano de la nación,"
#define CRUSADE_MESSAGE48 "perdedora no recibiras"
#define CRUSADE_MESSAGE49 "ningun premio."
#define CRUSADE_MESSAGE50 "La batalla de la cual has participado "
#define CRUSADE_MESSAGE51 "ha terminado"
#define CRUSADE_MESSAGE52 "hecho"
#define CRUSADE_MESSAGE53 "Debes conectarte despues de que termine"
#define CRUSADE_MESSAGE54 "la anterior y antes de que termine"
#define CRUSADE_MESSAGE55 "la proxima batalla y recibiras" 
#define CRUSADE_MESSAGE56 "el premio."
#define CRUSADE_MESSAGE57 "La batalla entre Aresden y Elvine ha terminado."
#define CRUSADE_MESSAGE58 "Estructuras restantes:"
#define CRUSADE_MESSAGE59 "No has dañado las estructuras enemigas"
#define CRUSADE_MESSAGE60 "con el ataque magico. Debes infiltrarte"
#define CRUSADE_MESSAGE61 "a la ciudad enemiga y destruir"
#define CRUSADE_MESSAGE62 "los escudos contra magia (Magic Shield)"
#define CRUSADE_MESSAGE63 "Has destruido la estructura enemiga;"
#define CRUSADE_MESSAGE64 "si destruyes todas las estructuras,"
#define CRUSADE_MESSAGE65 "enemigas, ganarás la guerra."
#define CRUSADE_MESSAGE66 "Has dañado las estructuras enemigas"
#define CRUSADE_MESSAGE67 "con el ataque magico. Si continuas,"
#define CRUSADE_MESSAGE68 "las estructuras enemigas seran destruidas."
#define CRUSADE_MESSAGE69 "El ataque magico del enemigo no le hizo"
#define CRUSADE_MESSAGE70 "ningun daño a nuestras estructuras."
#define CRUSADE_MESSAGE71 "Si quieres frenar el ataque enemigo,"
#define CRUSADE_MESSAGE72 "debes destruir los Mana Collectors enemigos"
#define CRUSADE_MESSAGE73 "en middleland cerca de las MagicStones."
#define CRUSADE_MESSAGE74 "El ataque enemigo ha destruido"
#define CRUSADE_MESSAGE75 "nuestras estructuras!"
#define CRUSADE_MESSAGE76 "Si todas las estructuras son"
#define CRUSADE_MESSAGE77 "dañadas, estaras cerca de la"
#define CRUSADE_MESSAGE78 "derrota. Para frenar su ataque."
#define CRUSADE_MESSAGE79 "debes destruir los Mana Collectors enemigos"
#define CRUSADE_MESSAGE80 "situadoes en middleland cerca de"
#define CRUSADE_MESSAGE81 "las MagicStones"
#define CRUSADE_MESSAGE82 "Nuestras estructuras han sido dañadas" 
#define CRUSADE_MESSAGE83 "por las fuerzas enemigas! Si todas"
#define CRUSADE_MESSAGE84 "las estructuras son destruidas"
#define CRUSADE_MESSAGE85 "seremos derrotados."
#define CRUSADE_MESSAGE86 "Para frenar su ataque "
#define CRUSADE_MESSAGE87 "debes destruir los Mana Collectors enemigos"
#define CRUSADE_MESSAGE88 "que se encuentran cerca de las MagicStones."
#define CRUSADE_MESSAGE89 "que estan situadas en Middleland."
#define CRUSADE_MESSAGE90 "El generador magico de Elvine ha sido activado. Pronto la magia caerá! Evacuen!"

#define NOTIFYMSG_QUEST_STARTED "La misión ha comenzado. Puedes ver la información presionando el boton 'Quest' en el menú del personaje."

#define LNG_STARTLAUNCHER "Ejecuta el programa \"HelbreathXtreme.exe\" para jugar!"

#define	NOTIFYMSG_NOTHING_TO_REPAIR "Usted no tiene ningún artículo que requiere reparación."

#define MSG_TARGETNEUTRAL_ON	"Ahora es posible golpear unidades neutrales."
#define MSG_TARGETNEUTRAL_OFF	"Ya no es posible golpear unidades neutrales."

#define DRAW_BLACKSMITH5 "Reparacion todo"

#define	DRAW_DIALOGBOX_NPCACTION_QUERY50 "Te gustaria que repare algo"
#define	DRAW_DIALOGBOX_NPCACTION_QUERY51 "sus artículos para  %d oro?"

#define	DRAW_DIALOGBOX_SKILLDLG35	"El relleno automático es "

#define DRAW_ENABLED 	"Activado"
#define DRAW_DISABLED	"Desactivado"
#define DRAW_EXTENDEDSYSMENU0 "Menu extendido" 
#define DRAW_EXTENDEDSYSMENU1 "La Grilla esta "
#define DRAW_EXTENDEDSYSMENU2 "Las Notificaciones estan "
#define DRAW_EXTENDEDSYSMENU3 "Ek en Screenshoots esta "
#define DRAW_EXTENDEDSYSMENU4 "Mostrar todos los daños esta "
#define DRAW_EXTENDEDSYSMENU5 "El tiempo del chat es "
#define DRAW_EXTENDEDSYSMENU6 "Los items grandes estan "
#define DRAW_EXTENDEDSYSMENU7 "La tecla de windows es "
#define DRAW_EXTENDEDSYSMENU8 "Mostrar tiempo esta "
#define DRAW_EXTENDEDSYSMENU9 "Mostrar lista de amigos esta"
#define DRAW_EXTENDEDSYSMENU10 "Mostrar lista de callados esta"
#define DRAW_EXTENDEDSYSMENU11 "Modo Afk esta"
#define DRAW_EXTENDEDSYSMENU12 "Mostrar FPS esta"

#define DRAW_MUTELIST "lista de callados esta"
#define DRAW_MUTELIST1 "Remover"
