#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#define NAME_WORLDNAME1			"WS1" 
#define MSG_WORLDNAME1			"Helbreath Legion" 

#define NAME_WORLDNAME2			"WS2"
#define MSG_WORLDNAME2			"Helbreath Legion Beta"

#define MSG_HOMEPAGE			"http://www.helbreathlegion.net/"

//#define SERVER_IP			"74.55.185.122"	// dedi
#define SERVER_IP			"192.168.1.110" // Coders server
#define SERVER_PORT			323

#define ENGLISH 0
#define SPANISH 1

#define LANGUAGE	ENGLISH
//#define LANGUAGE	SPANISH

#define DEF_SNOW_POSITION_Y		430

/*** Some more compilation options ***/
//#define USING_WIN_IME		// Used for unicode characters (unchecked)
#define MAKE_ACCOUNT		// Enable acc creation. Missing sprites, fixed!
//#define SELECTSERVER		// Show/Hides server selection screen.

#define COMMA_GOLD		// Allows to show commas number (ie: 1,200,000)

//#define  _DEBUG				// Multi-Client, Instant-logout, Version check, Badwords,
								// Mics debug msgs, CtrlQ for admins, Show status, Cursor position
#ifdef _DEBUG
	#define SPEEDHAX_RUN	1.5
	#define SPEEDHAX_ATK 	2
	#define SPEEDHAX_HIT	3
	#define LOGINCFG
	#define _SCDEBUG
	#define WINDOWED_MODE	// Shows HB in a windowed mode (for debug purpose only, sprite will bug....)
#else
	#define SPEEDHAX_RUN	1
	#define SPEEDHAX_ATK 	1
	#define SPEEDHAX_HIT	1
	#define UPDATER
#endif
		
#endif