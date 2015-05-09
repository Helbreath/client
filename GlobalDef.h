#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#define NAME_WORLDNAME1			"Xtreme" 
#define MSG_WORLDNAME1			"Helbreath Xtreme" 

#define NAME_WORLDNAME2			"WS2"
#define MSG_WORLDNAME2			"Helbreath Xtreme Beta"

#define MSG_HOMEPAGE			"http://www.helbreathx.net"
#define MSG_HOMEPAGE2			"http://forum.helbreathx.net"

#define HBF_MAJOR			1
#define HBF_MINOR			0 // Added new Version detail xRisenx
#define HBF_LOWER			3

//#define SERVER_IP			"31.186.251.201" // Coders server
#ifndef _DEBUG
#define SERVER_IP			"127.0.0.1"//"104.131.215.91"//"192.168.125.134"//"127.0.0.1"//"31.186.251.201" // Coders server
#else
#define SERVER_IP			"127.0.0.1"
#endif
//#define SERVER_IP			"192.168.0.13" //vm
#define SERVER_PORT			2848

#define ENGLISH 0
#define SPANISH 1

#define LANGUAGE	ENGLISH
//#define LANGUAGE	SPANISH

#define DEF_SNOW_POSITION_Y		430

/*** Some more compilation options ***/
//#define USING_WIN_IME		// Used for unicode characters (unchecked)
#define MAKE_ACCOUNT		// Enable acc creation. Missing sprites, fixed!
#define SELECTSERVER		// Show/Hides server selection screen.

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
	#define isArcherEnabled		FALSE
#else
	#define SPEEDHAX_RUN	1
	#define SPEEDHAX_ATK 	1
	#define SPEEDHAX_HIT	1
	#define LOGINCFG
	#define UPDATER
	#define ENCRYPT_CODE	"NAAJiMXqV2x1CyBSABX9J6sfoIjbjScp7V8iN5SivjSEVbP7IzrLHTG2gnKp8yvQ3lBT5mC6OPS5Xg6mVTRqn8pyzxXMyfmbVj3DZO8x934Phs1TlJRfUbX25lZuPl6Q"
	#define isArcherEnabled		FALSE
#endif

	// Define's Added by xRisenx
	//#define RaiseCriticalLimit
	//#define Remove Critical
	//#define MonsterDetails
	//#define MonsterBarClient
	#define RemoveCriticalClient
	#define TitleClient
	//#define ARENAMAP "GladArena" // Gladiator Arena xRisenx
	//#define Above100StatsClient
	//#define MoreColors
#endif
