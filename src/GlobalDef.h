//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#define NAME_WORLDNAME1			"xtreme" 
#define MSG_WORLDNAME1			"Helbreath Xtreme" 

#define NAME_WORLDNAME2			"WS2"
#define MSG_WORLDNAME2			"Helbreath Xtreme Beta"

#define MSG_HOMEPAGE			"http://www.helbreathx.net"
#define MSG_HOMEPAGE2			"http://forum.helbreathx.net"

#ifdef _DEBUG
#define SERVER_IP			"127.0.0.1"
#else
#define SERVER_IP			"xtreme.helbreathx.net"
#endif
#define SERVER_PORT			8443

#define ENGLISH 0
#define SPANISH 1

#define DS_VISIBLE 0
#define DS_BG 1
#define DS_CS 2
#define DS_WIN 3


#define LANGUAGE	ENGLISH
//#define LANGUAGE	SPANISH

#define DEF_SNOW_POSITION_Y		430

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
#define WINDOWED_MODE
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

//#define RaiseCriticalLimit
//#define Remove Critical
//#define MonsterDetails
#define RemoveCriticalClient
#define TitleClient
//#define ARENAMAP "GladArena"
//#define Above100StatsClient
//#define MoreColors