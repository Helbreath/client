#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#define NAME_WORLDNAME1			"WS1" 
#define MSG_WORLDNAME1			"Helbreath Redemption" 

#define NAME_WORLDNAME2			"WS2"
#define MSG_WORLDNAME2			"Helbreath Redemption Beta"

#define MSG_HOMEPAGE			"http://hbredemption.no-ip.biz:8888/"

#ifdef LOCALSERVER
#define SERVER_IP		"127.0.0.1"
#else
#define SERVER_IP		"hbredemption.no-ip.biz"
#endif

#define SERVER_PORT				7948
#define TOP50VOTE				"helbreathtop.com/button.php?u=lifeplaysu420"
#define WEBSITE					"http://hbredemption.no-ip.biz:8888/"
#define ENGLISH 0
#define SPANISH 1
#define LANGUAGE	ENGLISH
//#define LANGUAGE	SPANISH

#define DEF_SNOW_POSITION_Y		430
#define MAXONLINELIST	120
/*** Some more compilation options ***/
//#define USING_WIN_IME		// Used for unicode characters (unchecked)
#define MAKE_ACCOUNT		// Enable acc creation. Missing sprites, fixed!
//#define SELECTSERVER		// Show/Hides server selection screen.

//#define  _DEBUG				// Multi-Client, Instant-logout, Version check, Badwords,
								// Mics debug msgs, CtrlQ for admins, Show status, Cursor position
#ifdef _DEBUG
	#define SPEEDHAX_RUN	1.5
	#define SPEEDHAX_ATK 	2
	#define SPEEDHAX_HIT	3
	#define LOGINCFG
	#define _SCDEBUG
	#define MULTICLIENT
	//#define UPDATER
	#define WINDOWED_MODE	// Shows HB in a windowed mode (for debug purpose only, sprite will bug....)
#else
	#define SPEEDHAX_RUN	1
	#define SPEEDHAX_ATK 	1.1
	#define SPEEDHAX_HIT	1.1
	//#define UPDATER
	#define MULTICLIENT
#endif
		
#endif