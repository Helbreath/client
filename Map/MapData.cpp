// MapData.cpp: implementation of the CMapData class.
//
//////////////////////////////////////////////////////////////////////

#include "MapData.h"
#include <fstream>
#include "../Game.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapData::CMapData(class CGame * pGame)
{
	int i;
	m_pGame = pGame;
	memset(m_iObjectIDcacheLocX, 0, sizeof(m_iObjectIDcacheLocX));
	memset(m_iObjectIDcacheLocY, 0, sizeof(m_iObjectIDcacheLocY));
	m_dwDOframeTime = m_dwFrameTime = unixtime();

	for (i = 0; i < TOTALCHARACTERS; i++ ) 
	{	m_stFrame[i][OBJECTMOVE].m_sMaxFrame = 7;
	}
	for (i = 1; i <= 6; i++) 
	{	m_stFrame[i][OBJECTSTOP].m_sMaxFrame		= 14;
		m_stFrame[i][OBJECTSTOP].m_sFrameTime		= 60;
		m_stFrame[i][OBJECTMOVE].m_sMaxFrame		= 7;
		m_stFrame[i][OBJECTMOVE].m_sFrameTime		= 70;
		m_stFrame[i][OBJECTDAMAGEMOVE].m_sMaxFrame	= 3;
		m_stFrame[i][OBJECTDAMAGEMOVE].m_sFrameTime = (uint16_t)50 / SPEEDHAX_HIT;
		m_stFrame[i][OBJECTRUN].m_sMaxFrame			= 7;
		m_stFrame[i][OBJECTRUN].m_sFrameTime		= (uint16_t)41/SPEEDHAX_RUN;
		m_stFrame[i][OBJECTATTACK].m_sFrameTime		= (uint16_t)78/SPEEDHAX_ATK;
		m_stFrame[i][OBJECTATTACK].m_sMaxFrame		= 7;
		m_stFrame[i][OBJECTATTACKMOVE].m_sMaxFrame	= 12;
		m_stFrame[i][OBJECTATTACKMOVE].m_sFrameTime	= 78;
		m_stFrame[i][OBJECTMAGIC].m_sMaxFrame		= 15;
		m_stFrame[i][OBJECTMAGIC].m_sFrameTime		= 88;
		m_stFrame[i][OBJECTGETITEM].m_sMaxFrame		= 3;
		m_stFrame[i][OBJECTGETITEM].m_sFrameTime	= 150;
		m_stFrame[i][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
		m_stFrame[i][OBJECTDAMAGE].m_sFrameTime		= 70/SPEEDHAX_HIT;
		m_stFrame[i][OBJECTDYING].m_sMaxFrame		= 12;
		m_stFrame[i][OBJECTDYING].m_sFrameTime		= 80;
	}

	// Slime
	m_stFrame[10][OBJECTSTOP].m_sFrameTime		= 240;
	m_stFrame[10][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[10][OBJECTMOVE].m_sFrameTime		= 120;//240;
	m_stFrame[10][OBJECTATTACK].m_sFrameTime	= 90;
	m_stFrame[10][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[10][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[10][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[10][OBJECTDYING].m_sFrameTime		= 240;
	m_stFrame[10][OBJECTDYING].m_sMaxFrame		= 7;

	// Skeleton
	m_stFrame[11][OBJECTSTOP].m_sFrameTime		= 150;
	m_stFrame[11][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[11][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[11][OBJECTATTACK].m_sFrameTime	= 90;
	m_stFrame[11][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[11][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[11][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[11][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[11][OBJECTDYING].m_sMaxFrame		= 7;

	// Stone-Golem
	m_stFrame[12][OBJECTSTOP].m_sFrameTime		= 210;
	m_stFrame[12][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[12][OBJECTMOVE].m_sFrameTime		= 100;//210;
	m_stFrame[12][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[12][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[12][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[12][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[12][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[12][OBJECTDYING].m_sMaxFrame		= 7;

	// Cyclops
	m_stFrame[13][OBJECTSTOP].m_sFrameTime		= 210;
	m_stFrame[13][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[13][OBJECTMOVE].m_sFrameTime		= 80;//210;
	m_stFrame[13][OBJECTATTACK].m_sFrameTime	= 90;
	m_stFrame[13][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[13][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[13][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[13][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[13][OBJECTDYING].m_sMaxFrame		= 7;

	// Orc
	m_stFrame[14][OBJECTSTOP].m_sFrameTime		= 180;
	m_stFrame[14][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[14][OBJECTMOVE].m_sFrameTime		= 80;//150;
	m_stFrame[14][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[14][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[14][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[14][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[14][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[14][OBJECTDYING].m_sMaxFrame		= 7;
	
	// ShopKeeper-W
	m_stFrame[15][OBJECTSTOP].m_sFrameTime		= 180;
	m_stFrame[15][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[15][OBJECTMOVE].m_sFrameTime		= 100;//150;
	m_stFrame[15][OBJECTATTACK].m_sFrameTime	= 150;
	m_stFrame[15][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[15][OBJECTDAMAGE].m_sFrameTime	= 180;
	m_stFrame[15][OBJECTDAMAGE].m_sMaxFrame		= 3;
	m_stFrame[15][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[15][OBJECTDYING].m_sMaxFrame		= 7;

	// Giant Ant
	m_stFrame[16][OBJECTSTOP].m_sFrameTime		= 120;
	m_stFrame[16][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[16][OBJECTMOVE].m_sFrameTime		= 60;//120;
	m_stFrame[16][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[16][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[16][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[16][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[16][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[16][OBJECTDYING].m_sMaxFrame		= 7;

	// Scorpion
	m_stFrame[17][OBJECTSTOP].m_sFrameTime		= 120;
	m_stFrame[17][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[17][OBJECTMOVE].m_sFrameTime		= 45;//120;
	m_stFrame[17][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[17][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[17][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[17][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[17][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[17][OBJECTDYING].m_sMaxFrame		= 7;

	// Zombie
	m_stFrame[18][OBJECTSTOP].m_sFrameTime		= 210;
	m_stFrame[18][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[18][OBJECTMOVE].m_sFrameTime		= 130;//270;
	m_stFrame[18][OBJECTATTACK].m_sFrameTime	= 150;
	m_stFrame[18][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[18][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[18][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[18][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[18][OBJECTDYING].m_sMaxFrame		= 7;

	// Gandlf
	m_stFrame[19][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[19][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[19][OBJECTMOVE].m_sFrameTime		= 100;//210;
	m_stFrame[19][OBJECTATTACK].m_sFrameTime	= 150;
	m_stFrame[19][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[19][OBJECTDAMAGE].m_sFrameTime	= 180;
	m_stFrame[19][OBJECTDAMAGE].m_sMaxFrame		= 3;
	m_stFrame[19][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[19][OBJECTDYING].m_sMaxFrame		= 7;

	// Howard
	m_stFrame[20][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[20][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[20][OBJECTMOVE].m_sFrameTime		= 100;//210;
	m_stFrame[20][OBJECTATTACK].m_sFrameTime	= 150;
	m_stFrame[20][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[20][OBJECTDAMAGE].m_sFrameTime	= 180;
	m_stFrame[20][OBJECTDAMAGE].m_sMaxFrame		= 3;
	m_stFrame[20][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[20][OBJECTDYING].m_sMaxFrame		= 7;

	// Guard
	m_stFrame[21][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[21][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[21][OBJECTMOVE].m_sFrameTime		= 80;//150;
	m_stFrame[21][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[21][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[21][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[21][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[21][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[21][OBJECTDYING].m_sMaxFrame		= 7;

	// Amphisbena
	m_stFrame[22][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[22][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[22][OBJECTMOVE].m_sFrameTime		= 80;//150;
	m_stFrame[22][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[22][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[22][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[22][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[22][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[22][OBJECTDYING].m_sMaxFrame		= 7;

	// Clay-Golem
	m_stFrame[23][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[23][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[23][OBJECTMOVE].m_sFrameTime		= 80;//150;
	m_stFrame[23][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[23][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[23][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[23][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[23][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[23][OBJECTDYING].m_sMaxFrame		= 7;

	// Tom 
	m_stFrame[24][OBJECTSTOP].m_sFrameTime		= 150;
	m_stFrame[24][OBJECTSTOP].m_sMaxFrame		= 7;

	// William 
	m_stFrame[25][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[25][OBJECTSTOP].m_sMaxFrame		= 7;

	// Kenedy 
	m_stFrame[26][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[26][OBJECTSTOP].m_sMaxFrame		= 7;

	// Hellbound
	m_stFrame[27][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[27][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[27][OBJECTMOVE].m_sFrameTime		= 50;
	m_stFrame[27][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[27][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[27][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[27][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[27][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[27][OBJECTDYING].m_sMaxFrame		= 7;

	// Troll
	m_stFrame[28][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[28][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[28][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[28][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[28][OBJECTATTACK].m_sMaxFrame		= 5;
	m_stFrame[28][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[28][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[28][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[28][OBJECTDYING].m_sMaxFrame		= 9;

	// Orge
	m_stFrame[29][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[29][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[29][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[29][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[29][OBJECTATTACK].m_sMaxFrame		= 5;
	m_stFrame[29][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[29][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[29][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[29][OBJECTDYING].m_sMaxFrame		= 9;

	// Liche
	m_stFrame[30][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[30][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[30][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[30][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[30][OBJECTATTACK].m_sMaxFrame		= 5;
	m_stFrame[30][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[30][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[30][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[30][OBJECTDYING].m_sMaxFrame		= 9;

	// Demon
	m_stFrame[31][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[31][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[31][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[31][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[31][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[31][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[31][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[31][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[31][OBJECTDYING].m_sMaxFrame		= 9;

	// Unicorn
	m_stFrame[32][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[32][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[32][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[32][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[32][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[32][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[32][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[32][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[32][OBJECTDYING].m_sMaxFrame		= 11;

	// WereWolf
	m_stFrame[33][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[33][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[33][OBJECTMOVE].m_sFrameTime		= 120;
	m_stFrame[33][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[33][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[33][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[33][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[33][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[33][OBJECTDYING].m_sMaxFrame		= 11;

	// Dummy
	m_stFrame[34][OBJECTSTOP].m_sFrameTime		= 240;
	m_stFrame[34][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[34][OBJECTMOVE].m_sFrameTime		= 120;
	m_stFrame[34][OBJECTATTACK].m_sFrameTime	= 90;
	m_stFrame[34][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[34][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[34][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[34][OBJECTDYING].m_sFrameTime		= 240;
	m_stFrame[34][OBJECTDYING].m_sMaxFrame		= 7;

	// Energy-Ball
	m_stFrame[35][OBJECTSTOP].m_sFrameTime		= 80;
	m_stFrame[35][OBJECTSTOP].m_sMaxFrame		= 9;
	m_stFrame[35][OBJECTMOVE].m_sFrameTime		= 20;
	m_stFrame[35][OBJECTMOVE].m_sMaxFrame		= 3;
	m_stFrame[35][OBJECTATTACK].m_sFrameTime	= 80;
	m_stFrame[35][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[35][OBJECTDAMAGE].m_sFrameTime	= 80;
	m_stFrame[35][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[35][OBJECTDYING].m_sFrameTime		= 80;
	m_stFrame[35][OBJECTDYING].m_sMaxFrame		= 7;

	// Crossbow Guard Tower
	m_stFrame[36][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[36][OBJECTSTOP].m_sMaxFrame		= 0;
	m_stFrame[36][OBJECTMOVE].m_sFrameTime		= 80;
	m_stFrame[36][OBJECTMOVE].m_sMaxFrame		= 0;
	m_stFrame[36][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[36][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[36][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[36][OBJECTDAMAGE].m_sMaxFrame		= 0;
	m_stFrame[36][OBJECTDYING].m_sFrameTime		= 200;
	m_stFrame[36][OBJECTDYING].m_sMaxFrame		= 6;

	// Cannon Guard Tower
	m_stFrame[37][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[37][OBJECTSTOP].m_sMaxFrame		= 0;
	m_stFrame[37][OBJECTMOVE].m_sFrameTime		= 80;
	m_stFrame[37][OBJECTMOVE].m_sMaxFrame		= 0;
	m_stFrame[37][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[37][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[37][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[37][OBJECTDAMAGE].m_sMaxFrame		= 0;
	m_stFrame[37][OBJECTDYING].m_sFrameTime		= 200;
	m_stFrame[37][OBJECTDYING].m_sMaxFrame		= 6;

	// Mana Collector
	m_stFrame[38][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[38][OBJECTSTOP].m_sMaxFrame		= 0;
	m_stFrame[38][OBJECTMOVE].m_sFrameTime		= 80;
	m_stFrame[38][OBJECTMOVE].m_sMaxFrame		= 0;
	m_stFrame[38][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[38][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[38][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[38][OBJECTDAMAGE].m_sMaxFrame		= 0;
	m_stFrame[38][OBJECTDYING].m_sFrameTime		= 200;
	m_stFrame[38][OBJECTDYING].m_sMaxFrame		= 6;

	// Detector
	m_stFrame[39][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[39][OBJECTSTOP].m_sMaxFrame		= 0;
	m_stFrame[39][OBJECTMOVE].m_sFrameTime		= 80;
	m_stFrame[39][OBJECTMOVE].m_sMaxFrame		= 0;
	m_stFrame[39][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[39][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[39][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[39][OBJECTDAMAGE].m_sMaxFrame		= 0;
	m_stFrame[39][OBJECTDYING].m_sFrameTime		= 200;
	m_stFrame[39][OBJECTDYING].m_sMaxFrame		= 6;

	// Energy Shield Generator
	m_stFrame[40][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[40][OBJECTSTOP].m_sMaxFrame		= 0;
	m_stFrame[40][OBJECTMOVE].m_sFrameTime		= 80;
	m_stFrame[40][OBJECTMOVE].m_sMaxFrame		= 0;
	m_stFrame[40][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[40][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[40][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[40][OBJECTDAMAGE].m_sMaxFrame		= 0;
	m_stFrame[40][OBJECTDYING].m_sFrameTime		= 200;
	m_stFrame[40][OBJECTDYING].m_sMaxFrame		= 6;

	// Grand Magic Generator
	m_stFrame[41][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[41][OBJECTSTOP].m_sMaxFrame		= 0;
	m_stFrame[41][OBJECTMOVE].m_sFrameTime		= 80;
	m_stFrame[41][OBJECTMOVE].m_sMaxFrame		= 0;
	m_stFrame[41][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[41][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[41][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[41][OBJECTDAMAGE].m_sMaxFrame		= 0;
	m_stFrame[41][OBJECTDYING].m_sFrameTime		= 200;
	m_stFrame[41][OBJECTDYING].m_sMaxFrame		= 6;

	// Mana Stone
	m_stFrame[42][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[42][OBJECTSTOP].m_sMaxFrame		= 0;
	m_stFrame[42][OBJECTMOVE].m_sFrameTime		= 80;
	m_stFrame[42][OBJECTMOVE].m_sMaxFrame		= 0;
	m_stFrame[42][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[42][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[42][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[42][OBJECTDAMAGE].m_sMaxFrame		= 0;
	m_stFrame[42][OBJECTDYING].m_sFrameTime		= 200;
	m_stFrame[42][OBJECTDYING].m_sMaxFrame		= 0;

	// Light War Beetle
	m_stFrame[43][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[43][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[43][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[43][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[43][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[43][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[43][OBJECTDAMAGE].m_sMaxFrame		= 3 +7;
	m_stFrame[43][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[43][OBJECTDYING].m_sMaxFrame		= 9;

	// God's Hand Knight
	m_stFrame[44][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[44][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[44][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[44][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[44][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[44][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[44][OBJECTDAMAGE].m_sMaxFrame		= 3 +7;
	m_stFrame[44][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[44][OBJECTDYING].m_sMaxFrame		= 9;

	// God's Hand Knight with Armored Battle Steed
	m_stFrame[45][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[45][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[45][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[45][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[45][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[45][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[45][OBJECTDAMAGE].m_sMaxFrame		= 3 +7;
	m_stFrame[45][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[45][OBJECTDYING].m_sMaxFrame		= 9;

	// Temple Knight
	m_stFrame[46][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[46][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[46][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[46][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[46][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[46][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[46][OBJECTDAMAGE].m_sMaxFrame		= 3 +7;
	m_stFrame[46][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[46][OBJECTDYING].m_sMaxFrame		= 9;

	// Battle Golem
	m_stFrame[47][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[47][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[47][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[47][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[47][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[47][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[47][OBJECTDAMAGE].m_sMaxFrame		= 3 +7;
	m_stFrame[47][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[47][OBJECTDYING].m_sMaxFrame		= 9;

	// Stalker
	m_stFrame[48][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[48][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[48][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[48][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[48][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[48][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[48][OBJECTDAMAGE].m_sMaxFrame		= 3 +7;
	m_stFrame[48][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[48][OBJECTDYING].m_sMaxFrame		= 9;

	// Hellclaw
	m_stFrame[49][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[49][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[49][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[49][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[49][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[49][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[49][OBJECTDAMAGE].m_sMaxFrame		= 3 +7;
	m_stFrame[49][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[49][OBJECTDYING].m_sMaxFrame		= 9;

	// Tigerworm
	m_stFrame[50][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[50][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[50][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[50][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[50][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[50][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[50][OBJECTDAMAGE].m_sMaxFrame		= 3 +7;
	m_stFrame[50][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[50][OBJECTDYING].m_sMaxFrame		= 9;
 
	// Catapult
	m_stFrame[51][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[51][OBJECTSTOP].m_sMaxFrame		= 0;
	m_stFrame[51][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[51][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[51][OBJECTATTACK].m_sMaxFrame		= 4;
	m_stFrame[51][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[51][OBJECTDAMAGE].m_sMaxFrame		= 0;
	m_stFrame[51][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[51][OBJECTDYING].m_sMaxFrame		= 6;

	// Gargoyle
	m_stFrame[52][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[52][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[52][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[52][OBJECTATTACK].m_sFrameTime	= 70;
	m_stFrame[52][OBJECTATTACK].m_sMaxFrame		= 9;
	m_stFrame[52][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[52][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[52][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[52][OBJECTDYING].m_sMaxFrame		= 11 +3;

	// Beholder
	m_stFrame[53][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[53][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[53][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[53][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[53][OBJECTATTACK].m_sMaxFrame		= 12;
	m_stFrame[53][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[53][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[53][OBJECTDYING].m_sFrameTime		= 70;
	m_stFrame[53][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// DarkElf
	m_stFrame[54][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[54][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[54][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[54][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[54][OBJECTATTACK].m_sMaxFrame		= 9;
	m_stFrame[54][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[54][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[54][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[54][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// Bunny
	m_stFrame[55][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[55][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[55][OBJECTMOVE].m_sFrameTime		= 70;
	m_stFrame[55][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[55][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[55][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[55][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[55][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[55][OBJECTDYING].m_sMaxFrame		= 7 +3;

    // Cat
	m_stFrame[56][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[56][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[56][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[56][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[56][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[56][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[56][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[56][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[56][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// Giant-Frog
	m_stFrame[57][OBJECTSTOP].m_sFrameTime		= 300;
	m_stFrame[57][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[57][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[57][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[57][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[57][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[57][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[57][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[57][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// Mountain Giant
	m_stFrame[58][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[58][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[58][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[58][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[58][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[58][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[58][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[58][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[58][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// Ettin
	m_stFrame[59][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[59][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[59][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[59][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[59][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[59][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[59][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[59][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[59][OBJECTDYING].m_sMaxFrame		= 7 +3;

   	// Cannibal-Plant
	m_stFrame[60][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[60][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[60][OBJECTMOVE].m_sFrameTime		= 120;
	m_stFrame[60][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[60][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[60][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[60][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[60][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[60][OBJECTDYING].m_sMaxFrame		= 7 +3;

   	// Rudolph
	m_stFrame[61][OBJECTSTOP].m_sFrameTime		= 200;
	m_stFrame[61][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[61][OBJECTMOVE].m_sFrameTime		= 90;//60;
	m_stFrame[61][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[61][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[61][OBJECTDAMAGE].m_sFrameTime	= 60;
	m_stFrame[61][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[61][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[61][OBJECTDYING].m_sMaxFrame		= 7 +3;

   	// DireBoar
	m_stFrame[62][OBJECTSTOP].m_sFrameTime		= 200;
	m_stFrame[62][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[62][OBJECTMOVE].m_sFrameTime		= 60;
	m_stFrame[62][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[62][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[62][OBJECTDAMAGE].m_sFrameTime	= 60;
	m_stFrame[62][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[62][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[62][OBJECTDYING].m_sMaxFrame		= 7 +3;

   	// Frost
	m_stFrame[63][OBJECTSTOP].m_sFrameTime		= 200;
	m_stFrame[63][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[63][OBJECTMOVE].m_sFrameTime		= 60;
	m_stFrame[63][OBJECTATTACK].m_sFrameTime	= 80;
	m_stFrame[63][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[63][OBJECTDAMAGE].m_sFrameTime	= 60;
	m_stFrame[63][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[63][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[63][OBJECTDYING].m_sMaxFrame		= 5 +3;//7 +3;

   	// Crops
	m_stFrame[64][OBJECTSTOP].m_sFrameTime		= 200;
	m_stFrame[64][OBJECTSTOP].m_sMaxFrame		= 40;
	m_stFrame[64][OBJECTMOVE].m_sFrameTime		= 200;
	m_stFrame[64][OBJECTATTACK].m_sFrameTime	= 200;
	m_stFrame[64][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[64][OBJECTDAMAGE].m_sFrameTime	= 200;
	m_stFrame[64][OBJECTDAMAGE].m_sMaxFrame		= 3;
	m_stFrame[64][OBJECTDYING].m_sFrameTime		= 200;
	m_stFrame[64][OBJECTDYING].m_sMaxFrame		= 3;

   	// IceGolem
	m_stFrame[65][OBJECTSTOP].m_sFrameTime		= 200;
	m_stFrame[65][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[65][OBJECTMOVE].m_sFrameTime		= 140;
	m_stFrame[65][OBJECTATTACK].m_sFrameTime	= 105;
	m_stFrame[65][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[65][OBJECTDAMAGE].m_sFrameTime	= 60;
	m_stFrame[65][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[65][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[65][OBJECTDYING].m_sMaxFrame		= 7 +3;

   	// Wyvern
	m_stFrame[66][OBJECTSTOP].m_sFrameTime		= 100;
	m_stFrame[66][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[66][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[66][OBJECTATTACK].m_sFrameTime	= 80;
	m_stFrame[66][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[66][OBJECTDAMAGE].m_sFrameTime	= 60;
	m_stFrame[66][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[66][OBJECTDYING].m_sFrameTime		= 65;
	m_stFrame[66][OBJECTDYING].m_sMaxFrame		= 15 +3;

   	// McGaffin
	m_stFrame[67][OBJECTSTOP].m_sFrameTime		= 200;
	m_stFrame[67][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[67][OBJECTMOVE].m_sFrameTime		= 120;
	m_stFrame[68][OBJECTMOVE].m_sMaxFrame		= 3;
	m_stFrame[67][OBJECTATTACK].m_sFrameTime	= 80;
	m_stFrame[67][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[67][OBJECTDAMAGE].m_sFrameTime	= 60;
	m_stFrame[67][OBJECTDAMAGE].m_sMaxFrame		= 3;
	m_stFrame[67][OBJECTDYING].m_sFrameTime		= 65;
	m_stFrame[67][OBJECTDYING].m_sMaxFrame		= 3+3;

   	// Perry
	m_stFrame[68][OBJECTSTOP].m_sFrameTime		= 200;
	m_stFrame[68][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[68][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[68][OBJECTMOVE].m_sMaxFrame		= 3;
	m_stFrame[68][OBJECTATTACK].m_sFrameTime	= 80;
	m_stFrame[68][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[68][OBJECTDAMAGE].m_sFrameTime	= 60;
	m_stFrame[68][OBJECTDAMAGE].m_sMaxFrame		= 3;
	m_stFrame[68][OBJECTDYING].m_sFrameTime		= 65;
	m_stFrame[68][OBJECTDYING].m_sMaxFrame		= 3 +3;

   	// Devlin
	m_stFrame[69][OBJECTSTOP].m_sFrameTime		= 200;
	m_stFrame[69][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[69][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[68][OBJECTMOVE].m_sMaxFrame		= 3;
	m_stFrame[69][OBJECTATTACK].m_sFrameTime	= 80;
	m_stFrame[69][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[69][OBJECTDAMAGE].m_sFrameTime	= 60;
	m_stFrame[69][OBJECTDAMAGE].m_sMaxFrame		= 3;
	m_stFrame[69][OBJECTDYING].m_sFrameTime		= 65;
	m_stFrame[69][OBJECTDYING].m_sMaxFrame		= 3 +3;

// Snoopy: Added all 351 creature
	// Dragon
	m_stFrame[70][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[70][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[70][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[70][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[70][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[70][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[70][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[70][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[70][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// Centaur
	m_stFrame[71][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[71][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[71][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[71][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[71][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[71][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[71][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[71][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[71][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// ClawTurtle
	m_stFrame[72][OBJECTSTOP].m_sFrameTime		= 100;
	m_stFrame[72][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[72][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[72][OBJECTATTACK].m_sFrameTime	= 80;
	m_stFrame[72][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[72][OBJECTDAMAGE].m_sFrameTime	= 60;
	m_stFrame[72][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[72][OBJECTDYING].m_sFrameTime		= 65;
	m_stFrame[72][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 73 FireWyvern
	m_stFrame[73][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[73][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[73][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[73][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[73][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[73][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[73][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[73][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[73][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// GiantCrayFish
	m_stFrame[74][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[74][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[74][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[74][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[74][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[74][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[74][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[74][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[74][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 75 Gi Lizard
	m_stFrame[75][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[75][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[75][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[75][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[75][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[75][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[75][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[75][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[75][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 76 Gi Tree
	m_stFrame[76][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[76][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[76][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[76][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[76][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[76][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[76][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[76][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[76][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 77 Master Orc
	m_stFrame[77][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[77][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[77][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[77][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[77][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[77][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[77][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[77][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[77][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 78 Minos
	m_stFrame[78][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[78][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[78][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[78][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[78][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[78][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[78][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[78][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[78][OBJECTDYING].m_sMaxFrame		= 7 +3;

	//79 Nizie
	m_stFrame[79][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[79][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[79][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[79][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[79][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[79][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[79][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[79][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[79][OBJECTDYING].m_sMaxFrame		= 7 +3;

	//80 Tentocle
	m_stFrame[80][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[80][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[80][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[80][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[80][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[80][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[80][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[80][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[80][OBJECTDYING].m_sMaxFrame		= 7 +3;

	//81 Abaddon
	m_stFrame[81][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[81][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[81][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[81][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[81][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[81][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[81][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[81][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[81][OBJECTDYING].m_sMaxFrame		= 15+3;

	//82  Sorceress
	m_stFrame[82][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[82][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[82][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[82][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[82][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[82][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[82][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[82][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[82][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 83 DSK
	m_stFrame[83][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[83][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[83][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[83][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[83][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[83][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[83][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[83][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[83][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 84 MasterElf
	m_stFrame[84][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[84][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[84][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[84][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[84][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[84][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[84][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[84][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[84][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 85 DSK
	m_stFrame[85][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[85][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[85][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[85][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[85][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[85][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[85][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[85][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[85][OBJECTDYING].m_sMaxFrame		= 7 +3;

	//86 HBT
	m_stFrame[86][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[86][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[86][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[86][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[86][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[86][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[86][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[86][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[86][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 87 Crossbow turret
	m_stFrame[87][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[87][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[87][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[87][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[87][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[87][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[87][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[87][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[87][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 88 Barbarian
	m_stFrame[88][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[88][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[88][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[88][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[88][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[88][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[88][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[88][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[88][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 89 Ancient Great Cannon
	m_stFrame[89][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[89][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[89][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[89][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[89][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[89][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[89][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[89][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[89][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// 90 Gail
	m_stFrame[90][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[90][OBJECTSTOP].m_sMaxFrame		= 7;

	// 91 Gate
	m_stFrame[91][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[91][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[91][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[91][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[91][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[91][OBJECTDYING].m_sMaxFrame		= 7 +3;

	m_stFrame[NPC_PRINCESS][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_PRINCESS][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_PRINCESS][OBJECTMOVE].m_sFrameTime		= 100;//210;
	m_stFrame[NPC_PRINCESS][OBJECTATTACK].m_sFrameTime	= 150;
	m_stFrame[NPC_PRINCESS][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[NPC_PRINCESS][OBJECTDAMAGE].m_sFrameTime	= 180;
	m_stFrame[NPC_PRINCESS][OBJECTDAMAGE].m_sMaxFrame		= 3;
	m_stFrame[NPC_PRINCESS][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[NPC_PRINCESS][OBJECTDYING].m_sMaxFrame		= 7;

	// Orc Lord
	//m_stFrame[NPC_ORCLORD][OBJECTSTOP].m_sFrameTime		= 180;
	//m_stFrame[NPC_ORCLORD][OBJECTSTOP].m_sMaxFrame		= 3;
	//m_stFrame[NPC_ORCLORD][OBJECTMOVE].m_sFrameTime		= 80;//150;
	//m_stFrame[NPC_ORCLORD][OBJECTATTACK].m_sFrameTime	= 120;
	//m_stFrame[NPC_ORCLORD][OBJECTATTACK].m_sMaxFrame	= 3;
	//m_stFrame[NPC_ORCLORD][OBJECTDAMAGE].m_sFrameTime	= 150;
	//m_stFrame[NPC_ORCLORD][OBJECTDAMAGE].m_sMaxFrame	= 3 +4;
	//m_stFrame[NPC_ORCLORD][OBJECTDYING].m_sFrameTime	= 180;
	//m_stFrame[NPC_ORCLORD][OBJECTDYING].m_sMaxFrame		= 7;

	// Dark Unicorn
	m_stFrame[NPC_DARKUNICORN][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_DARKUNICORN][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[NPC_DARKUNICORN][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[NPC_DARKUNICORN][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[NPC_DARKUNICORN][OBJECTATTACK].m_sMaxFrame	= 7;
	m_stFrame[NPC_DARKUNICORN][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[NPC_DARKUNICORN][OBJECTDAMAGE].m_sMaxFrame	= 3 +4;
	m_stFrame[NPC_DARKUNICORN][OBJECTDYING].m_sFrameTime	= 100;
	m_stFrame[NPC_DARKUNICORN][OBJECTDYING].m_sMaxFrame		= 11;

	// Black Demon
	m_stFrame[NPC_BLACK_DEMON][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_BLACK_DEMON][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[NPC_BLACK_DEMON][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[NPC_BLACK_DEMON][OBJECTATTACK].m_sFrameTime	= 120;
	m_stFrame[NPC_BLACK_DEMON][OBJECTATTACK].m_sMaxFrame	= 7;
	m_stFrame[NPC_BLACK_DEMON][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[NPC_BLACK_DEMON][OBJECTDAMAGE].m_sMaxFrame	= 3 +4;
	m_stFrame[NPC_BLACK_DEMON][OBJECTDYING].m_sFrameTime	= 100;
	m_stFrame[NPC_BLACK_DEMON][OBJECTDYING].m_sMaxFrame		= 9;

	// Earth Wyvern
	m_stFrame[NPC_EARTH_WYVERN][OBJECTSTOP].m_sFrameTime	= 250;
	m_stFrame[NPC_EARTH_WYVERN][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_EARTH_WYVERN][OBJECTMOVE].m_sFrameTime	= 90;
	m_stFrame[NPC_EARTH_WYVERN][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[NPC_EARTH_WYVERN][OBJECTATTACK].m_sMaxFrame	= 7;
	m_stFrame[NPC_EARTH_WYVERN][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[NPC_EARTH_WYVERN][OBJECTDAMAGE].m_sMaxFrame	= 7;
	m_stFrame[NPC_EARTH_WYVERN][OBJECTDYING].m_sFrameTime	= 150;
	m_stFrame[NPC_EARTH_WYVERN][OBJECTDYING].m_sMaxFrame	= 7 +3;

	// Wind Wyvern
	m_stFrame[NPC_WIND_WYVERN][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_WIND_WYVERN][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_WIND_WYVERN][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[NPC_WIND_WYVERN][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[NPC_WIND_WYVERN][OBJECTATTACK].m_sMaxFrame	= 7;
	m_stFrame[NPC_WIND_WYVERN][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[NPC_WIND_WYVERN][OBJECTDAMAGE].m_sMaxFrame	= 7;
	m_stFrame[NPC_WIND_WYVERN][OBJECTDYING].m_sFrameTime	= 150;
	m_stFrame[NPC_WIND_WYVERN][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// Dark Wyvern
	m_stFrame[NPC_DARK_WYVERN][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_DARK_WYVERN][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_DARK_WYVERN][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[NPC_DARK_WYVERN][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[NPC_DARK_WYVERN][OBJECTATTACK].m_sMaxFrame	= 7;
	m_stFrame[NPC_DARK_WYVERN][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[NPC_DARK_WYVERN][OBJECTDAMAGE].m_sMaxFrame	= 7;
	m_stFrame[NPC_DARK_WYVERN][OBJECTDYING].m_sFrameTime	= 150;
	m_stFrame[NPC_DARK_WYVERN][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// Grass Wyvern
	m_stFrame[NPC_GRASS_WYVERN][OBJECTSTOP].m_sFrameTime	= 250;
	m_stFrame[NPC_GRASS_WYVERN][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_GRASS_WYVERN][OBJECTMOVE].m_sFrameTime	= 90;
	m_stFrame[NPC_GRASS_WYVERN][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[NPC_GRASS_WYVERN][OBJECTATTACK].m_sMaxFrame	= 7;
	m_stFrame[NPC_GRASS_WYVERN][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[NPC_GRASS_WYVERN][OBJECTDAMAGE].m_sMaxFrame	= 7;
	m_stFrame[NPC_GRASS_WYVERN][OBJECTDYING].m_sFrameTime	= 150;
	m_stFrame[NPC_GRASS_WYVERN][OBJECTDYING].m_sMaxFrame	= 7 +3;

	// Sky Wyvern
	m_stFrame[NPC_SKY_WYVERN][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_SKY_WYVERN][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_SKY_WYVERN][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[NPC_SKY_WYVERN][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[NPC_SKY_WYVERN][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[NPC_SKY_WYVERN][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[NPC_SKY_WYVERN][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[NPC_SKY_WYVERN][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[NPC_SKY_WYVERN][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// Sea Wyvern
	m_stFrame[NPC_SEA_WYVERN][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_SEA_WYVERN][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_SEA_WYVERN][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[NPC_SEA_WYVERN][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[NPC_SEA_WYVERN][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[NPC_SEA_WYVERN][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[NPC_SEA_WYVERN][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[NPC_SEA_WYVERN][OBJECTDYING].m_sFrameTime		= 150;
	m_stFrame[NPC_SEA_WYVERN][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// Metal Wyvern
	m_stFrame[NPC_METAL_WYVERN][OBJECTSTOP].m_sFrameTime	= 250;
	m_stFrame[NPC_METAL_WYVERN][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_METAL_WYVERN][OBJECTMOVE].m_sFrameTime	= 90;
	m_stFrame[NPC_METAL_WYVERN][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[NPC_METAL_WYVERN][OBJECTATTACK].m_sMaxFrame	= 7;
	m_stFrame[NPC_METAL_WYVERN][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[NPC_METAL_WYVERN][OBJECTDAMAGE].m_sMaxFrame	= 7;
	m_stFrame[NPC_METAL_WYVERN][OBJECTDYING].m_sFrameTime	= 150;
	m_stFrame[NPC_METAL_WYVERN][OBJECTDYING].m_sMaxFrame	= 7 +3;

	// Void Wyvern
	m_stFrame[NPC_VOID_WYVERN][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_VOID_WYVERN][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_VOID_WYVERN][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[NPC_VOID_WYVERN][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[NPC_VOID_WYVERN][OBJECTATTACK].m_sMaxFrame	= 7;
	m_stFrame[NPC_VOID_WYVERN][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[NPC_VOID_WYVERN][OBJECTDAMAGE].m_sMaxFrame	= 7;
	m_stFrame[NPC_VOID_WYVERN][OBJECTDYING].m_sFrameTime	= 150;
	m_stFrame[NPC_VOID_WYVERN][OBJECTDYING].m_sMaxFrame		= 7 +3;

	// Eternal Dragon
	/*m_stFrame[NPC_ETERNAL_DRAGON][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTDYING].m_sMaxFrame		= 15 +3;*/
	// Eternal Dragon
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTMOVE].m_sFrameTime		= 90;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTATTACK].m_sFrameTime	= 100;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTDAMAGE].m_sFrameTime	= 100;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[NPC_ETERNAL_DRAGON][OBJECTDYING].m_sMaxFrame		= 15+3;

	// Enraged Troll
	m_stFrame[NPC_ENRAGED_TROLL][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_ENRAGED_TROLL][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[NPC_ENRAGED_TROLL][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[NPC_ENRAGED_TROLL][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[NPC_ENRAGED_TROLL][OBJECTATTACK].m_sMaxFrame		= 5;
	m_stFrame[NPC_ENRAGED_TROLL][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[NPC_ENRAGED_TROLL][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[NPC_ENRAGED_TROLL][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[NPC_ENRAGED_TROLL][OBJECTDYING].m_sMaxFrame		= 9;

	// Enraged Cyclops
	m_stFrame[NPC_ENRAGED_CYCLOPS][OBJECTSTOP].m_sFrameTime		= 210;
	m_stFrame[NPC_ENRAGED_CYCLOPS][OBJECTSTOP].m_sMaxFrame		= 3;
	m_stFrame[NPC_ENRAGED_CYCLOPS][OBJECTMOVE].m_sFrameTime		= 80;//210;
	m_stFrame[NPC_ENRAGED_CYCLOPS][OBJECTATTACK].m_sFrameTime	= 90;
	m_stFrame[NPC_ENRAGED_CYCLOPS][OBJECTATTACK].m_sMaxFrame		= 3;
	m_stFrame[NPC_ENRAGED_CYCLOPS][OBJECTDAMAGE].m_sFrameTime	= 150;
	m_stFrame[NPC_ENRAGED_CYCLOPS][OBJECTDAMAGE].m_sMaxFrame		= 3 +4;
	m_stFrame[NPC_ENRAGED_CYCLOPS][OBJECTDYING].m_sFrameTime		= 180;
	m_stFrame[NPC_ENRAGED_CYCLOPS][OBJECTDYING].m_sMaxFrame		= 7;

	// Enraged Stalker
	m_stFrame[NPC_ENRAGED_STALKER][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_ENRAGED_STALKER][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_ENRAGED_STALKER][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[NPC_ENRAGED_STALKER][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[NPC_ENRAGED_STALKER][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[NPC_ENRAGED_STALKER][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[NPC_ENRAGED_STALKER][OBJECTDAMAGE].m_sMaxFrame		= 3 +7;
	m_stFrame[NPC_ENRAGED_STALKER][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[NPC_ENRAGED_STALKER][OBJECTDYING].m_sMaxFrame		= 9;

	// Enraged Gargoyle
	m_stFrame[NPC_ENRAGED_GAGOYLE][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_ENRAGED_GAGOYLE][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_ENRAGED_GAGOYLE][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[NPC_ENRAGED_GAGOYLE][OBJECTATTACK].m_sFrameTime	= 70;
	m_stFrame[NPC_ENRAGED_GAGOYLE][OBJECTATTACK].m_sMaxFrame		= 9;
	m_stFrame[NPC_ENRAGED_GAGOYLE][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[NPC_ENRAGED_GAGOYLE][OBJECTDAMAGE].m_sMaxFrame		= 7;
	m_stFrame[NPC_ENRAGED_GAGOYLE][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[NPC_ENRAGED_GAGOYLE][OBJECTDYING].m_sMaxFrame		= 11 +3;

	// Enraged Hellclaw
	m_stFrame[NPC_ENRAGED_HELLCLAW][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_ENRAGED_HELLCLAW][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_ENRAGED_HELLCLAW][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[NPC_ENRAGED_HELLCLAW][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[NPC_ENRAGED_HELLCLAW][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[NPC_ENRAGED_HELLCLAW][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[NPC_ENRAGED_HELLCLAW][OBJECTDAMAGE].m_sMaxFrame		= 3 +7;
	m_stFrame[NPC_ENRAGED_HELLCLAW][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[NPC_ENRAGED_HELLCLAW][OBJECTDYING].m_sMaxFrame		= 9;

	// Enraged Tigerworm
	m_stFrame[NPC_ENRAGED_TIGERWORM][OBJECTSTOP].m_sFrameTime		= 250;
	m_stFrame[NPC_ENRAGED_TIGERWORM][OBJECTSTOP].m_sMaxFrame		= 7;
	m_stFrame[NPC_ENRAGED_TIGERWORM][OBJECTMOVE].m_sFrameTime		= 100;
	m_stFrame[NPC_ENRAGED_TIGERWORM][OBJECTATTACK].m_sFrameTime	= 60;
	m_stFrame[NPC_ENRAGED_TIGERWORM][OBJECTATTACK].m_sMaxFrame		= 7;
	m_stFrame[NPC_ENRAGED_TIGERWORM][OBJECTDAMAGE].m_sFrameTime	= 120;
	m_stFrame[NPC_ENRAGED_TIGERWORM][OBJECTDAMAGE].m_sMaxFrame		= 3 +7;
	m_stFrame[NPC_ENRAGED_TIGERWORM][OBJECTDYING].m_sFrameTime		= 100;
	m_stFrame[NPC_ENRAGED_TIGERWORM][OBJECTDYING].m_sMaxFrame		= 9;

	// CLEROTH - NEW MONSTERS
	// AirElemental
	//m_stFrame[110][OBJECTSTOP].m_sFrameTime		= 40;
	//m_stFrame[110][OBJECTSTOP].m_sMaxFrame		= 7;
	//m_stFrame[110][OBJECTMOVE].m_sFrameTime		= 80;//150;
	//m_stFrame[110][OBJECTATTACK].m_sFrameTime	= 120;
	//m_stFrame[110][OBJECTATTACK].m_sMaxFrame	= 3;
	//m_stFrame[110][OBJECTDAMAGE].m_sFrameTime	= 150;
	//m_stFrame[110][OBJECTDAMAGE].m_sMaxFrame	= 3 +4;
	//m_stFrame[110][OBJECTDYING].m_sFrameTime	= 180;
	//m_stFrame[110][OBJECTDYING].m_sMaxFrame		= 7;

}

void CMapData::Init()
{
	int x, y;
	m_dwFrameCheckTime = unixtime();
	m_dwFrameAdjustTime = 0;
	//m_sPivotX = -1;
	//m_sPivotY = -1;
    m_sPivotX = 0;
    m_sPivotY = 0;
    for (y = 0; y < 550; y++)
	for (x = 0; x < 550; x++)
		m_pData[x][y].Clear();

	for (x = 0; x < 30000; x++) {
		m_iObjectIDcacheLocX[x] = 0;
		m_iObjectIDcacheLocY[x] = 0;
	}
}

CMapData::~CMapData()
{
}

void CMapData::OpenMapDataFile(char * cFn)
{
	HANDLE hFileRead;
	uint32_t  nCount;
	char cHeader[260];
	char *cp, *cpMapData;
	memset( cHeader, 0, sizeof(cHeader) );


	std::ifstream szfile(cFn, std::ios::in | std::ios::binary);
	if (!szfile.is_open()) return;
	szfile.seekg(0, std::ios::beg);
	szfile.read((char*)&cHeader, 256);
	_bDecodeMapInfo(cHeader);
	cpMapData = new char[m_sMapSizeX*m_sMapSizeY * 10];
	szfile.read((char*)cpMapData, m_sMapSizeX*m_sMapSizeY * 10);
	szfile.close();

	cp = cpMapData;
	short *sp;
	for (int y=0 ; y<m_sMapSizeY ; y++ )
	{
		for(int x=0 ; x<m_sMapSizeX ; x++ )
		{
			sp = (short *)cp;
			m_tile[x][y].m_sTileSprite        = *sp;
			cp += 2;
			sp = (short *)cp;
			m_tile[x][y].m_sTileSpriteFrame   = *sp;
			cp += 2;
			sp = (short *)cp;
			m_tile[x][y].m_sObjectSprite      = *sp;
			cp += 2;
			sp = (short *)cp;
			m_tile[x][y].m_sObjectSpriteFrame = *sp;
			cp += 2;
			if (((*cp) & 0x80) != 0)
				 m_tile[x][y].m_bIsMoveAllowed = false;
			else m_tile[x][y].m_bIsMoveAllowed = true;
			if (((*cp) & 0x40) != 0)
				 m_tile[x][y].m_bIsTeleport = true;
			else m_tile[x][y].m_bIsTeleport = false;
			cp += 2;
		}
	}
	delete[] cpMapData;
}

void CMapData::_bDecodeMapInfo(char * pHeader)
{
 int i;
 char * token, cReadMode;
 char seps[] = "= ,\t\n";
	for (i = 0; i < 256; i++)
		if (pHeader[i] == 0) pHeader[i] = ' ';

	cReadMode = 0;

	token = strtok( pHeader, seps );
	while( token != 0 )
	{
		if (cReadMode != 0)
		{
			switch (cReadMode)
			{
			case 1:
				m_sMapSizeX = atoi(token);
				cReadMode = 0;
				break;
			case 2:
				m_sMapSizeY = atoi(token);
				cReadMode = 0;
				break;
			}
		}
		else
		{
			if (memcmp(token, "MAPSIZEX",8) == 0) cReadMode = 1;
			if (memcmp(token, "MAPSIZEY",8) == 0) cReadMode = 2;
		}
		token = strtok( 0, seps );
	}
}

void CMapData::ShiftMapData(char cDir) // 800x600 Resolution xRisenx Map Settings Fixed
{
    return;
	int ix, iy;
	for (iy = 0; iy < MAPDATASIZEY; iy++) 
		for (ix = 0; ix < MAPDATASIZEX; ix++) 
			m_pTmpData[ix][iy].Clear();

	switch (cDir) { 
	case 1: // NORTH
		//orig 21x, 15y
		for (ix = 0; ix < /*26*/ 27; ix++) 
			for (iy = 0; iy < /*19*/ 19; iy++)
				//memcpy(&m_pTmpData[4+5+ix][6+5+iy], &m_pData[4+5+ix][5+5+iy], sizeof(class CTile));//orig
				memcpy(&m_pTmpData[7+ix][9+iy], &m_pData[7+ix][8+iy], sizeof(class CTile));
		m_sPivotY--;
		break;
	case 2:
		//orig 20x, 15y
		for (ix = 0; ix < /*25*/ 26; ix++) 
			for (iy = 0; iy < /*19*/ 19; iy++) 
				//memcpy(&m_pTmpData[4+5+ix][6+5+iy], &m_pData[5+5+ix][5+5+iy], sizeof(class CTile));//orig
				memcpy(&m_pTmpData[7+ix][9+iy], &m_pData[8+ix][8+iy], sizeof(class CTile));
		m_sPivotX++;
		m_sPivotY--;
		break;
	case 3: // EAST
		//orig 20x, 16y
		for (ix = 0; ix < /*25*/ 26; ix++) 
			for (iy = 0; iy < /*20*/ 20; iy++) 
				//memcpy(&m_pTmpData[4+5+ix][5+5+iy], &m_pData[5+5+ix][5+5+iy], sizeof(class CTile));//orig
				memcpy(&m_pTmpData[7+ix][8+iy], &m_pData[8+ix][8+iy], sizeof(class CTile));  
		m_sPivotX++;
		break;
	case 4:
		//orig 20x 15y
		for (ix = 0; ix < /*25*/ 26; ix++) 
			for (iy = 0; iy < /*19*/ 19; iy++) 
				//memcpy(&m_pTmpData[4+5+ix][5+5+iy], &m_pData[5+5+ix][6+5+iy], sizeof(class CTile));//orig
				memcpy(&m_pTmpData[7+ix][8+iy], &m_pData[8+ix][9+iy], sizeof(class CTile));  
		m_sPivotX++;
		m_sPivotY++;
		break;
	case 5: // SOUTH
		//orig 21x 15y
		for (ix = 0; ix < /*26*/ 27; ix++) 
			for (iy = 0; iy < /*19*/ 19; iy++) 
				//memcpy(&m_pTmpData[4+5+ix][5+5+iy], &m_pData[4+5+ix][6+5+iy], sizeof(class CTile));//orig
				memcpy(&m_pTmpData[7+ix][8+iy], &m_pData[7+ix][9+iy], sizeof(class CTile));  
		m_sPivotY++;
		break;
	case 6:
		//orig 20x, 15y
		for (ix = 0; ix < /*25*/ 26; ix++) 
			for (iy = 0; iy < /*19*/ 19; iy++) 
				//memcpy(&m_pTmpData[5+5+ix][5+5+iy], &m_pData[4+5+ix][6+5+iy], sizeof(class CTile));//orig
				memcpy(&m_pTmpData[8+ix][8+iy], &m_pData[7+ix][9+iy], sizeof(class CTile));  
		m_sPivotX--;
		m_sPivotY++;
		break;
	case 7: // WEST
		//orig 20x 16y
		for (ix = 0; ix < /*25*/ 26; ix++) 
			for (iy = 0; iy < /*20*/ 20; iy++)
				//memcpy(&m_pTmpData[5+5+ix][5+5+iy], &m_pData[4+5+ix][5+5+iy], sizeof(class CTile));//orig
				memcpy(&m_pTmpData[8+ix][8+iy], &m_pData[7+ix][8+iy], sizeof(class CTile));  
		m_sPivotX--;
		break;
	case 8:
		//orig 20x 15y
		for (ix = 0; ix < /*25*/ 26; ix++) 
			for (iy = 0; iy < /*19*/ 19; iy++) 
				//memcpy(&m_pTmpData[5+5+ix][6+5+iy], &m_pData[4+5+ix][5+5+iy], sizeof(class CTile));//orig
				memcpy(&m_pTmpData[8+ix][9+iy], &m_pData[7+ix][8+iy], sizeof(class CTile));  
		m_sPivotX--;
		m_sPivotY--;
		break;
	}
	memcpy(&m_pData[0][0], &m_pTmpData[0][0], sizeof(m_pData));
}

bool CMapData::bGetIsLocateable(short sX, short sY)
{
	int dX, dY;
// 	if ((sX < m_sPivotX) || (sX > m_sPivotX + MAPDATASIZEX) ||
// 		(sY < m_sPivotY) || (sY > m_sPivotY + MAPDATASIZEY)) return false;
    dX = sX;// -m_sPivotX;
    dY = sY;// -m_sPivotY;
	if (m_pData[dX][dY].m_sOwnerType != 0) return false;
	if (m_tile[sX][sY].m_bIsMoveAllowed == false) return false;
	if (m_pData[dX][dY].m_sDynamicObjectType == DYNAMICOBJECT_MINERAL1) return false;
	if (m_pData[dX][dY].m_sDynamicObjectType == DYNAMICOBJECT_MINERAL2) return false;
	if (m_pData[dX][dY].m_sDynamicObjectType == DYNAMICOBJECT_ARESDENFLAG) return false;
	if (m_pData[dX][dY].m_sDynamicObjectType == DYNAMICOBJECT_ELVINEFLAG) return false;

	if (m_pData[dX+1][dY+1].m_sOwnerType == 66) return false;
	if (m_pData[dX+1]  [dY].m_sOwnerType == 66) return false;
	if ((dY > 0) && (m_pData[dX+1][dY-1].m_sOwnerType == 66)) return false;
	if (m_pData  [dX][dY+1].m_sOwnerType == 66) return false;
	if (m_pData  [dX]  [dY].m_sOwnerType == 66) return false;
	if ((dY > 0) && (m_pData  [dX][dY-1].m_sOwnerType == 66)) return false;
	if ((dX > 0) && (m_pData[dX-1][dY+1].m_sOwnerType == 66)) return false;
	if ((dX > 0) && (m_pData[dX-1]  [dY].m_sOwnerType == 66)) return false;
	if ((dX > 0) && (dY > 0) && (m_pData[dX-1][dY-1].m_sOwnerType == 66)) return false;

	if (m_pData[dX+1][dY+1].m_sOwnerType == 73) return false;
	if (m_pData[dX+1]  [dY].m_sOwnerType == 73) return false;
	if ((dY > 0) && (m_pData[dX+1][dY-1].m_sOwnerType == 73)) return false;
	if (m_pData  [dX][dY+1].m_sOwnerType == 73) return false;
	if (m_pData  [dX]  [dY].m_sOwnerType == 73) return false;
	if ((dY > 0) && (m_pData  [dX][dY-1].m_sOwnerType == 73)) return false;
	if ((dX > 0) && (m_pData[dX-1][dY+1].m_sOwnerType == 73)) return false;
	if ((dX > 0) && (m_pData[dX-1]  [dY].m_sOwnerType == 73)) return false;
	if ((dX > 0) && (dY > 0) && (m_pData[dX-1][dY-1].m_sOwnerType == 73)) return false;

	if (m_pData[dX+1][dY+1].m_sOwnerType == 81) return false;
	if (m_pData[dX+1]  [dY].m_sOwnerType == 81) return false;
	if ((dY > 0) && (m_pData[dX+1][dY-1].m_sOwnerType == 81)) return false;
	if (m_pData  [dX][dY+1].m_sOwnerType == 81) return false;
	if (m_pData  [dX]  [dY].m_sOwnerType == 81) return false;
	if ((dY > 0) && (m_pData  [dX][dY-1].m_sOwnerType == 81)) return false;
	if ((dX > 0) && (m_pData[dX-1][dY+1].m_sOwnerType == 81)) return false;
	if ((dX > 0) && (m_pData[dX-1]  [dY].m_sOwnerType == 81)) return false;
	if ((dX > 0) && (dY > 0) && (m_pData[dX-1][dY-1].m_sOwnerType == 81)) return false;

	if (m_pData[dX+1][dY+1].m_sOwnerType == 91) return false;
	if (m_pData[dX+1]  [dY].m_sOwnerType == 91) return false;
	if ((dY > 0) && (m_pData[dX+1][dY-1].m_sOwnerType == 91)) return false;
	if (m_pData  [dX][dY+1].m_sOwnerType == 91) return false;
	if (m_pData  [dX]  [dY].m_sOwnerType == 91) return false;
	if ((dY > 0) && (m_pData  [dX][dY-1].m_sOwnerType == 91)) return false;
	if ((dX > 0) && (m_pData[dX-1][dY+1].m_sOwnerType == 91)) return false;
	if ((dX > 0) && (m_pData[dX-1]  [dY].m_sOwnerType == 91)) return false;
	if ((dX > 0) && (dY > 0) && (m_pData[dX-1][dY-1].m_sOwnerType == 91)) return false;
	return true;
}

bool CMapData::bIsTeleportLoc(short sX, short sY)
{
// 	if ((sX < m_sPivotX) || (sX > m_sPivotX + MAPDATASIZEX) ||
// 		(sY < m_sPivotY) || (sY > m_sPivotY + MAPDATASIZEY)) return false;

	if (m_tile[sX][sY].m_bIsTeleport == false) return false;

	return true;
}
#ifdef SHOWALLDAMAGE // Remove Critical xRisenx
bool __fastcall CMapData::bSetOwner(uint16_t wObjectID, int sX, int sY, int sType, int cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, UnitStatus iStatus, char * pName, short sAction, int sV1, short sV2, short sV3, int iPreLoc, int iFrame)
#else
bool __fastcall CMapData::bSetOwner(uint16_t wObjectID, int sX, int sY, int sType, int cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, short sHeadApprValue, short sBodyApprValue, short sArmApprValue, short sLegApprValue, UnitStatus iStatus, char * pName, short sAction, short sV1, short sV2, short sV3, int iPreLoc, int iFrame)
#endif
{
    int   iX, iY, dX, dY;
    int   iChatIndex, iAdd;
    char  cTmpName[12];
    uint64_t dwTime;
    int   iEffectType, iEffectFrame, iEffectTotalFrame;

	//if ((m_sPivotX == -1) || (m_sPivotY == -1)) return false;
	memset(cTmpName, 0, sizeof(cTmpName));
	strcpy(cTmpName, pName);
	dwTime = m_dwFrameTime;
	iEffectType = iEffectFrame = iEffectTotalFrame = 0;
	if (  0 && (wObjectID < 30000)//TODO: fix
		&& (  (sX < m_sPivotX) || (sX >= m_sPivotX + MAPDATASIZEX)
		   || (sY < m_sPivotY) || (sY >= m_sPivotY + MAPDATASIZEY)) )
	{
		if (m_iObjectIDcacheLocX[wObjectID] > 0)
		{
            iX = m_iObjectIDcacheLocX[wObjectID];// -m_sPivotX;
            iY = m_iObjectIDcacheLocY[wObjectID];// -m_sPivotY;
			if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
			{
				m_iObjectIDcacheLocX[wObjectID] = 0;
				m_iObjectIDcacheLocY[wObjectID] = 0;
				return false;
			}

			if (m_pData[iX][iY].m_wObjectID == wObjectID)
			{
				m_pData[iX][iY].m_sOwnerType = 0;
				memset(m_pData[iX][iY].m_cOwnerName, 0, sizeof(m_pData[iX][iY].m_cOwnerName));
				memset(pName, 0, strlen(pName));

				if (m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iChatMsg ] != 0)
				{
					delete m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iChatMsg ];
					m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iChatMsg ] = 0;
				}
				m_pData[iX][iY].m_iChatMsg = 0;
				m_pData[iX][iY].m_iEffectType = 0;
				m_iObjectIDcacheLocX[wObjectID] = 0;
				m_iObjectIDcacheLocY[wObjectID] = 0;
				return false;
			}
		}
		else if (m_iObjectIDcacheLocX[wObjectID] < 0)
		{
            iX = abs(m_iObjectIDcacheLocX[wObjectID]);// -m_sPivotX;
            iY = abs(m_iObjectIDcacheLocY[wObjectID]);// -m_sPivotY;
			if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
			{
				m_iObjectIDcacheLocX[wObjectID] = 0;
				m_iObjectIDcacheLocY[wObjectID] = 0;
				return false;
			}
			if ((m_pData[iX][iY].m_cDeadOwnerFrame == -1) && (m_pData[iX][iY].m_wDeadObjectID == wObjectID))
			{
				m_pData[iX][iY].m_cDeadOwnerFrame = 0;
				memset(pName, 0, strlen(pName));
				if (m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iDeadChatMsg ] != 0)
				{	delete m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iDeadChatMsg ];
					m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iDeadChatMsg ] = 0;
				}
				m_pData[iX][iY].m_iDeadChatMsg = 0;
				m_iObjectIDcacheLocX[wObjectID] = 0;
				m_iObjectIDcacheLocY[wObjectID] = 0;
				return false;
			}	
		}

		for (iX = 0; iX < MAPDATASIZEX; iX++)
		for (iY = 0; iY < MAPDATASIZEY; iY++)
		{
			if (m_pData[iX][iY].m_wObjectID == wObjectID)
			{
			    m_pData[iX][iY].m_sOwnerType = 0;
				ZeroMemory(m_pData[iX][iY].m_cOwnerName, sizeof(m_pData[iX][iY].m_cOwnerName));
				ZeroMemory(pName, strlen(pName));
				if (m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iChatMsg ] != 0)
				{
					delete m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iChatMsg ];
					m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iChatMsg ] = 0;
				}
				m_pData[iX][iY].m_iChatMsg = 0;
				m_iObjectIDcacheLocX[wObjectID] = 0;
				m_iObjectIDcacheLocY[wObjectID] = 0;
				m_pData[iX][iY].m_iEffectType = 0;
				return false;
			}

			if ((m_pData[iX][iY].m_cDeadOwnerFrame == -1) && (m_pData[iX][iY].m_wDeadObjectID == wObjectID))
			{
				m_pData[iX][iY].m_cDeadOwnerFrame = 0;
				ZeroMemory(pName, strlen(pName));
				if (m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iDeadChatMsg ] != 0)
				{
					delete m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iDeadChatMsg ];
					m_pGame->m_pChatMsgList[ m_pData[iX][iY].m_iDeadChatMsg ] = 0;
				}
				m_pData[iX][iY].m_iDeadChatMsg = 0;
				m_iObjectIDcacheLocX[wObjectID] = 0;
				m_iObjectIDcacheLocY[wObjectID] = 0;
				return false;
			}
		}
		ZeroMemory(pName, strlen(pName));
		return false;
	}
	iChatIndex = 0;

	if ((wObjectID < 30000) && (sAction != OBJECTNULLACTION))
	{
        ZeroMemory(cTmpName, sizeof(cTmpName));
		strcpy(cTmpName, pName);
        dX = sX;// -m_sPivotX;
        dY = sY;// -m_sPivotY;
		if (m_iObjectIDcacheLocX[wObjectID] > 0)
		{
            iX = m_iObjectIDcacheLocX[wObjectID];// -m_sPivotX;
            iY = m_iObjectIDcacheLocY[wObjectID];// -m_sPivotY;
			if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
			{
                m_iObjectIDcacheLocX[wObjectID] = 0;
				m_iObjectIDcacheLocY[wObjectID] = 0;
				return false;
			}
			if (m_pData[iX][iY].m_wObjectID == wObjectID) 
			{
                iChatIndex = m_pData[iX][iY].m_iChatMsg;
				iEffectType  = m_pData[iX][iY].m_iEffectType;
				iEffectFrame = m_pData[iX][iY].m_iEffectFrame;
				iEffectTotalFrame = m_pData[iX][iY].m_iEffectTotalFrame;

				m_pData[iX][iY].m_wObjectID  = 0; //-1; v1.41
				m_pData[iX][iY].m_iChatMsg   = 0; // v1.4
				m_pData[iX][iY].m_sOwnerType = 0;
				ZeroMemory(m_pData[iX][iY].m_cOwnerName, sizeof(m_pData[iX][iY].m_cOwnerName));
				m_iObjectIDcacheLocX[wObjectID] = sX;
				m_iObjectIDcacheLocY[wObjectID] = sY;
				goto EXIT_SEARCH_LOOP;
			}
		}
        else if (m_iObjectIDcacheLocX[wObjectID] < 0)
		{
            iX = abs(m_iObjectIDcacheLocX[wObjectID]);// -m_sPivotX;
            iY = abs(m_iObjectIDcacheLocY[wObjectID]);// -m_sPivotY;
			if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
			{
                m_iObjectIDcacheLocX[wObjectID] = 0;
				m_iObjectIDcacheLocY[wObjectID] = 0;
				return false;
			}
			if ((m_pData[iX][iY].m_cDeadOwnerFrame == -1) && (m_pData[iX][iY].m_wDeadObjectID == wObjectID))
			{
                iChatIndex = m_pData[iX][iY].m_iDeadChatMsg;
				iEffectType  = m_pData[iX][iY].m_iEffectType;
				iEffectFrame = m_pData[iX][iY].m_iEffectFrame;
				iEffectTotalFrame = m_pData[iX][iY].m_iEffectTotalFrame;
				m_pData[iX][iY].m_wDeadObjectID   = 0;
				m_pData[iX][iY].m_iDeadChatMsg    = 0; // v1.4
				m_pData[iX][iY].m_sDeadOwnerType  = 0;
				m_iObjectIDcacheLocX[wObjectID] = -1*sX;
				m_iObjectIDcacheLocY[wObjectID] = -1*sY;
				goto EXIT_SEARCH_LOOP;
		}	}

		iAdd = 9; // Changed from 7 -> 9 800x600 Resolution
		for (iX = sX - iAdd; iX <= sX + iAdd; iX++)
		for (iY = sY - iAdd; iY <= sY + iAdd; iY++)
		{
//             if (iX < m_sPivotX) break;
// 			else if (iX >= m_sPivotX + MAPDATASIZEX) break;
// 			if (iY < m_sPivotY) break;
// 			else if (iY >= m_sPivotY + MAPDATASIZEY) break;
			//if (memcmp(m_pData[iX - m_sPivotX][iY - m_sPivotY].m_cOwnerName, cTmpName, 10) == 0) {
			if (m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_wObjectID == wObjectID)
			{
                iChatIndex = m_pData[iX - m_sPivotX][iY/* - m_sPivotY*/].m_iChatMsg;
				iEffectType  = m_pData[iX - m_sPivotX][iY/* - m_sPivotY*/].m_iEffectType;
				iEffectFrame = m_pData[iX - m_sPivotX][iY/* - m_sPivotY*/].m_iEffectFrame;
				iEffectTotalFrame = m_pData[iX - m_sPivotX][iY/* - m_sPivotY*/].m_iEffectTotalFrame;
				m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_wObjectID  = 0; //-1; v1.41
				m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_iChatMsg   = 0;
				m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_sOwnerType = 0;
				m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_iEffectType = 0;
				ZeroMemory(m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_cOwnerName, sizeof(m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_cOwnerName));
				m_iObjectIDcacheLocX[wObjectID] = sX;
				m_iObjectIDcacheLocY[wObjectID] = sY;
				goto EXIT_SEARCH_LOOP;
			}

			//if (memcmp(m_pData[iX - m_sPivotX][iY - m_sPivotY].m_cDeadOwnerName, cTmpName, 10) == 0) {
			if (m_pData[iX/* - m_sPivotX*/][iY - m_sPivotY].m_wDeadObjectID == wObjectID)
			{
                iChatIndex = m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_iDeadChatMsg;
				iEffectType  = m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_iEffectType;
				iEffectFrame = m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_iEffectFrame;
				iEffectTotalFrame = m_pData[iX/* - m_sPivotX*/][iY - m_sPivotY].m_iEffectTotalFrame;
				m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_wDeadObjectID  = 0; //-1; v1.41
				m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_iDeadChatMsg   = 0;
				m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_sDeadOwnerType = 0;
				ZeroMemory(m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_cDeadOwnerName, sizeof(m_pData[iX/* - m_sPivotX*/][iY/* - m_sPivotY*/].m_cDeadOwnerName));
				m_iObjectIDcacheLocX[wObjectID] = -1*sX;
				m_iObjectIDcacheLocY[wObjectID] = -1*sY;
				goto EXIT_SEARCH_LOOP;
		    }
        }
		m_iObjectIDcacheLocX[wObjectID] = sX;
		m_iObjectIDcacheLocY[wObjectID] = sY;
	}
    else
	{
        if (sAction != OBJECTNULLACTION)// ObjectID
			wObjectID -= 30000;
		// v1.5 Crash
		if (wObjectID >= 30000) return false;
		if (m_iObjectIDcacheLocX[wObjectID] > 0)
        {
            iX = m_iObjectIDcacheLocX[wObjectID];// -m_sPivotX;
            iY = m_iObjectIDcacheLocY[wObjectID];// -m_sPivotY;
			if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
			{
                m_iObjectIDcacheLocX[wObjectID] = 0;
				m_iObjectIDcacheLocY[wObjectID] = 0;
				return false;
			}
			if (m_pData[iX][iY].m_wObjectID == wObjectID)
			{
                dX = iX;
				dY = iY;
				switch (sAction)
                {
                    case OBJECTRUN:
                    case OBJECTMOVE:
                    case OBJECTDAMAGEMOVE:
                    case OBJECTATTACKMOVE:
                        switch (cDir)
                        {
                            case 1: dY--; break;
                            case 2: dY--; dX++; break;
                            case 3: dX++; break;
                            case 4: dX++; dY++; break;
                            case 5: dY++; break;
                            case 6: dX--; dY++; break;
                            case 7: dX--; break;
                            case 8: dX--; dY--; break;
                        }
                        break;
                    default:
                        break;
				}
				if (   (wObjectID != (uint16_t)m_pGame->m_sPlayerObjectID)
					&& (m_pData[dX][dY].m_sOwnerType != 0) && (m_pData[dX][dY].m_wObjectID != wObjectID))
				{
                    m_pGame->RequestFullObjectData(wObjectID);
					ZeroMemory(pName, strlen(pName));
					return false;
				}
				iChatIndex = m_pData[iX][iY].m_iChatMsg;
				if (sAction != OBJECTNULLACTION)
				{
                    sType      = m_pData[iX][iY].m_sOwnerType;
					sAppr1     = m_pData[iX][iY].m_sAppr1;
					sAppr2     = m_pData[iX][iY].m_sAppr2;
					sAppr3     = m_pData[iX][iY].m_sAppr3;
					sAppr4     = m_pData[iX][iY].m_sAppr4;
					iApprColor = m_pData[iX][iY].m_iApprColor;
					sHeadApprValue     = m_pData[iX][iY].m_sHeadApprValue; // Re-Coding Sprite xRisenx
					sBodyApprValue     = m_pData[iX][iY].m_sBodyApprValue; // Re-Coding Sprite xRisenx
					sArmApprValue      = m_pData[iX][iY].m_sArmApprValue; // Re-Coding Sprite xRisenx
					sLegApprValue      = m_pData[iX][iY].m_sLegApprValue; // Re-Coding Sprite xRisenx
					iStatus    = m_pData[iX][iY].m_iStatus;
					iEffectType  = m_pData[iX][iY].m_iEffectType;
					iEffectFrame = m_pData[iX][iY].m_iEffectFrame;
					iEffectTotalFrame = m_pData[iX][iY].m_iEffectTotalFrame;
				}
				ZeroMemory(cTmpName, sizeof(cTmpName));
				memcpy(cTmpName, m_pData[iX][iY].m_cOwnerName, 10);
				ZeroMemory(pName, sizeof(pName));
				memcpy(pName, m_pData[iX][iY].m_cOwnerName, 10);
				m_pData[iX][iY].m_wObjectID  = 0; //-1; v1.41
				m_pData[iX][iY].m_iChatMsg   = 0;
				m_pData[iX][iY].m_sOwnerType = 0;
				m_pData[iX][iY].m_iEffectType = 0;
				ZeroMemory(m_pData[iX][iY].m_cOwnerName, sizeof(m_pData[iX][iY].m_cOwnerName));
                m_iObjectIDcacheLocX[wObjectID] = dX;// +m_sPivotX;
                m_iObjectIDcacheLocY[wObjectID] = dY;// +m_sPivotY;
				goto EXIT_SEARCH_LOOP;
			}
		}
        else if (m_iObjectIDcacheLocX[wObjectID] < 0)
		{
            iX = abs(m_iObjectIDcacheLocX[wObjectID]);// -m_sPivotX;
            iY = abs(m_iObjectIDcacheLocY[wObjectID]);// -m_sPivotY;
			if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
			{
                m_iObjectIDcacheLocX[wObjectID] = 0;
				m_iObjectIDcacheLocY[wObjectID] = 0;
				return false;
			}
			if ((m_pData[iX][iY].m_cDeadOwnerFrame == -1) && (m_pData[iX][iY].m_wDeadObjectID == wObjectID))
			{
                dX = iX;
				dY = iY;
				switch (sAction) {
                    case OBJECTMOVE:
                    case OBJECTRUN:
                    case OBJECTDAMAGEMOVE:
                    case OBJECTATTACKMOVE:
                        switch (cDir)
                        {
                            case 1: dY--; break;
                            case 2: dY--; dX++; break;
                            case 3: dX++; break;
                            case 4: dX++; dY++; break;
                            case 5: dY++; break;
                            case 6: dX--; dY++; break;
                            case 7: dX--; break;
                            case 8: dX--; dY--; break;
                        }
                        break;
                    default:
                        break;
				}
				if ((wObjectID != (uint16_t)m_pGame->m_sPlayerObjectID) &&
					(m_pData[dX][dY].m_sOwnerType != 0) && (m_pData[dX][dY].m_wObjectID != wObjectID))
				{
                    m_pGame->RequestFullObjectData(wObjectID);
					ZeroMemory(pName, strlen(pName));
					return false;
				}
                iChatIndex = m_pData[iX][iY].m_iDeadChatMsg;
                if (sAction != OBJECTNULLACTION)
                {
			   		sType      = m_pData[iX][iY].m_sDeadOwnerType;
					sAppr1     = m_pData[iX][iY].m_sDeadAppr1;
					sAppr2     = m_pData[iX][iY].m_sDeadAppr2;
					sAppr3     = m_pData[iX][iY].m_sDeadAppr3;
					sAppr4     = m_pData[iX][iY].m_sDeadAppr4;
					iApprColor = m_pData[iX][iY].m_iDeadApprColor;
					sHeadApprValue = m_pData[iX][iY].m_sDeadHeadApprValue; // Re-Coding Sprite xRisenx
					sBodyApprValue = m_pData[iX][iY].m_sDeadBodyApprValue; // Re-Coding Sprite xRisenx
					sArmApprValue = m_pData[iX][iY].m_sDeadArmApprValue; // Re-Coding Sprite xRisenx
					sLegApprValue = m_pData[iX][iY].m_sDeadLegApprValue; // Re-Coding Sprite xRisenx
					iStatus    = m_pData[iX][iY].m_iDeadStatus;
				}
				ZeroMemory(cTmpName, sizeof(cTmpName));
				memcpy(cTmpName, m_pData[iX][iY].m_cDeadOwnerName, 10);
				ZeroMemory(pName, sizeof(pName));
				memcpy(pName, m_pData[iX][iY].m_cDeadOwnerName, 10);
				m_pData[iX][iY].m_wDeadObjectID  = 0; // -1; v1.41
				m_pData[iX][iY].m_iDeadChatMsg   = 0;
				m_pData[iX][iY].m_sDeadOwnerType = 0;
				ZeroMemory(m_pData[iX][iY].m_cDeadOwnerName, sizeof(m_pData[iX][iY].m_cDeadOwnerName));
				m_iObjectIDcacheLocX[wObjectID] = -1*(dX/* + m_sPivotX*/);
				m_iObjectIDcacheLocY[wObjectID] = -1*(dY/* + m_sPivotY*/);
				goto EXIT_SEARCH_LOOP;
		    }
        }

		for (iX = 0; iX < MAPDATASIZEX; iX++)
		for (iY = 0; iY < MAPDATASIZEY; iY++)
		{
            if (m_pData[iX][iY].m_wObjectID == wObjectID)
			{
            dX = iX;
				dY = iY;
				switch (sAction)
                {
                    case OBJECTRUN:
                    case OBJECTMOVE:
                    case OBJECTDAMAGEMOVE:
                    case OBJECTATTACKMOVE:
                        switch (cDir)
                        {
                            case 1: dY--; break;
                            case 2: dY--; dX++; break;
                            case 3: dX++; break;
                            case 4: dX++; dY++; break;
                            case 5: dY++; break;
                            case 6: dX--; dY++; break;
                            case 7: dX--; break;
                            case 8: dX--; dY--; break;
                        }
                        break;
                    default:
                        break;
				}
				if (   (wObjectID != (uint16_t)m_pGame->m_sPlayerObjectID) 
					&& (m_pData[dX][dY].m_sOwnerType != 0) && (m_pData[dX][dY].m_wObjectID != wObjectID))
				{
                    m_pGame->RequestFullObjectData(wObjectID);
					ZeroMemory(pName, strlen(pName));
					return false;
				}
				iChatIndex = m_pData[iX][iY].m_iChatMsg;
				if (sAction != OBJECTNULLACTION)
                {
					sType      = m_pData[iX][iY].m_sOwnerType;
					sAppr1     = m_pData[iX][iY].m_sAppr1;
					sAppr2     = m_pData[iX][iY].m_sAppr2;
					sAppr3     = m_pData[iX][iY].m_sAppr3;
					sAppr4     = m_pData[iX][iY].m_sAppr4;
					iApprColor = m_pData[iX][iY].m_iApprColor; // v1.4
					sHeadApprValue     = m_pData[iX][iY].m_sHeadApprValue; // Re-Coding Sprite xRisenx
					sBodyApprValue     = m_pData[iX][iY].m_sBodyApprValue; // Re-Coding Sprite xRisenx
					sArmApprValue     = m_pData[iX][iY].m_sArmApprValue; // Re-Coding Sprite xRisenx
					sLegApprValue     = m_pData[iX][iY].m_sLegApprValue; // Re-Coding Sprite xRisenx
					iStatus    = m_pData[iX][iY].m_iStatus;
					iEffectType  = m_pData[iX][iY].m_iEffectType;
					iEffectFrame = m_pData[iX][iY].m_iEffectFrame;
					iEffectTotalFrame = m_pData[iX][iY].m_iEffectTotalFrame;
				}
				ZeroMemory(cTmpName, sizeof(cTmpName));
				memcpy(cTmpName, m_pData[iX][iY].m_cOwnerName, 10);
				ZeroMemory(pName, sizeof(pName));
				memcpy(pName, m_pData[iX][iY].m_cOwnerName, 10);
				m_pData[iX][iY].m_wObjectID  = 0; //-1; v1.41
				m_pData[iX][iY].m_iChatMsg   = 0;
				m_pData[iX][iY].m_sOwnerType = 0;
				m_pData[iX][iY].m_iEffectType = 0;
				ZeroMemory(m_pData[iX][iY].m_cOwnerName, sizeof(m_pData[iX][iY].m_cOwnerName));
                m_iObjectIDcacheLocX[wObjectID] = dX;// +m_sPivotX;
                m_iObjectIDcacheLocY[wObjectID] = dY;// +m_sPivotY;
				goto EXIT_SEARCH_LOOP;
			}
			if (m_pData[iX][iY].m_wDeadObjectID == wObjectID)
			{
                dX = iX;
				dY = iY;
				switch (sAction) {
                    case OBJECTMOVE:
                    case OBJECTRUN:
                    case OBJECTDAMAGEMOVE:
                    case OBJECTATTACKMOVE:
                        switch (cDir)
                        {
                            case 1: dY--; break;
                            case 2: dY--; dX++; break;
                            case 3: dX++; break;
                            case 4: dX++; dY++; break;
                            case 5: dY++; break;
                            case 6: dX--; dY++; break;
                            case 7: dX--; break;
                            case 8: dX--; dY--; break;
                        }
                        break;
                    default:
                        break;
				}
				if ((wObjectID != (uint16_t)m_pGame->m_sPlayerObjectID) &&
					(m_pData[dX][dY].m_sOwnerType != 0) && (m_pData[dX][dY].m_wObjectID != wObjectID))
				{
                    m_pGame->RequestFullObjectData(wObjectID);
					ZeroMemory(pName, strlen(pName));
					return false;
				}
				iChatIndex = m_pData[iX][iY].m_iDeadChatMsg;
				if (sAction != OBJECTNULLACTION)
                {
			   		sType      = m_pData[iX][iY].m_sDeadOwnerType;
					sAppr1     = m_pData[iX][iY].m_sDeadAppr1;
					sAppr2     = m_pData[iX][iY].m_sDeadAppr2;
					sAppr3     = m_pData[iX][iY].m_sDeadAppr3;
					sAppr4     = m_pData[iX][iY].m_sDeadAppr4;
					iApprColor = m_pData[iX][iY].m_iDeadApprColor; // v1.4
					sHeadApprValue = m_pData[iX][iY].m_sDeadHeadApprValue; // Re-Coding Sprite xRisenx
					sBodyApprValue = m_pData[iX][iY].m_sDeadBodyApprValue; // Re-Coding Sprite xRisenx
					sArmApprValue = m_pData[iX][iY].m_sDeadArmApprValue; // Re-Coding Sprite xRisenx
					sLegApprValue = m_pData[iX][iY].m_sDeadLegApprValue; // Re-Coding Sprite xRisenx
					iStatus    = m_pData[iX][iY].m_iDeadStatus;
				}
				ZeroMemory(cTmpName, sizeof(cTmpName));
				memcpy(cTmpName, m_pData[iX][iY].m_cDeadOwnerName, 10);
				ZeroMemory(pName, sizeof(pName));
				memcpy(pName, m_pData[iX][iY].m_cDeadOwnerName, 10);
				m_pData[iX][iY].m_wDeadObjectID  = 0; //-1; v1.41
				m_pData[iX][iY].m_iDeadChatMsg   = 0;
				m_pData[iX][iY].m_sDeadOwnerType = 0;
				m_pData[iX][iY].m_iEffectType    = 0;
				ZeroMemory(m_pData[iX][iY].m_cDeadOwnerName, sizeof(m_pData[iX][iY].m_cDeadOwnerName));
				m_iObjectIDcacheLocX[wObjectID] = -1*(dX/* + m_sPivotX*/);
				m_iObjectIDcacheLocY[wObjectID] = -1*(dY/* + m_sPivotY*/);
				goto EXIT_SEARCH_LOOP;
		    }
        }
		m_pGame->RequestFullObjectData(wObjectID);
		ZeroMemory(pName, strlen(pName));
		return false;
	}

EXIT_SEARCH_LOOP:;

	if (sAction == OBJECTDYING)
	{
        dX = sX;// -m_sPivotX;
        dY = sY;// -m_sPivotY;
	}
	if ((iPreLoc == 0) && (m_pData[dX][dY].m_sOwnerType != 0) && (m_pData[dX][dY].m_cOwnerAction == OBJECTDYING))
	{
        m_pData[dX][dY].m_wDeadObjectID        = m_pData[dX][dY].m_wObjectID;
		m_pData[dX][dY].m_sDeadOwnerType       = m_pData[dX][dY].m_sOwnerType;
		m_pData[dX][dY].m_cDeadDir             = m_pData[dX][dY].m_cDir;
		m_pData[dX][dY].m_sDeadAppr1           = m_pData[dX][dY].m_sAppr1;
		m_pData[dX][dY].m_sDeadAppr2           = m_pData[dX][dY].m_sAppr2;
		m_pData[dX][dY].m_sDeadAppr3           = m_pData[dX][dY].m_sAppr3;
		m_pData[dX][dY].m_sDeadAppr4           = m_pData[dX][dY].m_sAppr4;
		m_pData[dX][dY].m_iDeadApprColor       = m_pData[dX][dY].m_iApprColor; // v1.4
		m_pData[dX][dY].m_sDeadHeadApprValue   = m_pData[dX][dY].m_sHeadApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_sDeadBodyApprValue   = m_pData[dX][dY].m_sBodyApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_sDeadArmApprValue    = m_pData[dX][dY].m_sArmApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_sDeadLegApprValue    = m_pData[dX][dY].m_sLegApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_iDeadStatus          = m_pData[dX][dY].m_iStatus;
		m_pData[dX][dY].m_cDeadOwnerFrame      = -1;
		m_pData[dX][dY].m_dwDeadOwnerTime	   = dwTime;
		memcpy(m_pData[dX][dY].m_cDeadOwnerName, m_pData[dX][dY].m_cOwnerName, 11);
		m_pData[dX][dY].m_iDeadChatMsg         = m_pData[dX][dY].m_iChatMsg;
		m_pData[dX][dY].m_wObjectID  = 0;
		m_pData[dX][dY].m_sOwnerType = 0;
		m_pData[dX][dY].m_iChatMsg   = 0;
		ZeroMemory(m_pData[dX][dY].m_cOwnerName, sizeof(m_pData[dX][dY].m_cOwnerName));
		m_iObjectIDcacheLocX[m_pData[dX][dY].m_wDeadObjectID] = -1*m_iObjectIDcacheLocX[m_pData[dX][dY].m_wDeadObjectID];//dX; // v1.4
		m_iObjectIDcacheLocY[m_pData[dX][dY].m_wDeadObjectID] = -1*m_iObjectIDcacheLocY[m_pData[dX][dY].m_wDeadObjectID];//dY;

		if (m_pData[dX][dY].m_iEffectType != 0)
		{
            m_pData[dX][dY].m_iEffectType  = 0;
			m_pData[dX][dY].m_iEffectFrame = 0;
			m_pData[dX][dY].m_iEffectTotalFrame = 0;
			m_pData[dX][dY].m_dwEffectTime      = 0;
	    }
    }

	if (m_pData[dX][dY].m_sOwnerType != 0)
	{
        if (   (wObjectID != (uint16_t)m_pGame->m_sPlayerObjectID)
			&& (m_pData[dX][dY].m_wObjectID == (uint16_t)m_pGame->m_sPlayerObjectID))
            return false;
        else
            return false;
    }

	if (iPreLoc == 0)
	{
        m_pData[dX][dY].m_wObjectID   = wObjectID;
		m_pData[dX][dY].m_sOwnerType  = sType;
		m_pData[dX][dY].m_cDir        = cDir;
		m_pData[dX][dY].m_sAppr1      = sAppr1;
		m_pData[dX][dY].m_sAppr2      = sAppr2;
		m_pData[dX][dY].m_sAppr3      = sAppr3;
		m_pData[dX][dY].m_sAppr4      = sAppr4;
		m_pData[dX][dY].m_iApprColor  = iApprColor; // v1.4
		m_pData[dX][dY].m_sHeadApprValue = sHeadApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_sBodyApprValue = sBodyApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_sArmApprValue = sArmApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_sLegApprValue = sLegApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_iStatus     = iStatus;
		m_pData[dX][dY].m_sV1         = sV1;
		m_pData[dX][dY].m_sV2         = sV2;
		m_pData[dX][dY].m_sV3         = sV3;
		m_pData[dX][dY].m_iEffectType  = iEffectType;
		m_pData[dX][dY].m_iEffectFrame = iEffectFrame;
		m_pData[dX][dY].m_iEffectTotalFrame = iEffectTotalFrame;
		ZeroMemory(m_pData[dX][dY].m_cOwnerName, sizeof(m_pData[dX][dY].m_cOwnerName));
		strcpy(m_pData[dX][dY].m_cOwnerName, cTmpName);
		if ((sAction != OBJECTNULLACTION) && (sAction != MSGTYPE_CONFIRM) && (sAction != MSGTYPE_REJECT))
		{
            m_pData[dX][dY].m_cOwnerFrame  = iFrame; // 0
			m_pData[dX][dY].m_cOwnerAction = (char)sAction;
		}
		m_pData[dX][dY].m_dwOwnerTime        = dwTime;
		m_pData[dX][dY].m_iChatMsg = iChatIndex;
		if ((sAppr4 & 0x00F0) != 0)
		{
            m_pData[dX][dY].m_iEffectType = (sAppr4 & 0x00F0) >> 4;
			if (sAction == OBJECTNULLACTION)
			{
                m_pData[dX][dY].m_iEffectFrame = 0;
				m_pData[dX][dY].m_dwEffectTime = dwTime;
			}
			switch (m_pData[dX][dY].m_iEffectType)
            {
			    case 1: m_pData[dX][dY].m_iEffectTotalFrame = 13; break;
			    case 2: m_pData[dX][dY].m_iEffectTotalFrame = 11; break;
			}
		}
        else
		{
            m_pData[dX][dY].m_iEffectType = 0;
		}
	}
    else // iPreLoc == 1
	{
        m_pData[dX][dY].m_wDeadObjectID   = wObjectID;
		m_pData[dX][dY].m_sDeadOwnerType  = sType;
		m_pData[dX][dY].m_cDeadDir        = cDir;
		m_pData[dX][dY].m_sDeadAppr1      = sAppr1;
		m_pData[dX][dY].m_sDeadAppr2      = sAppr2;
		m_pData[dX][dY].m_sDeadAppr3      = sAppr3;
		m_pData[dX][dY].m_sDeadAppr4      = sAppr4;
		m_pData[dX][dY].m_iDeadApprColor  = iApprColor; // v1.4
		m_pData[dX][dY].m_sDeadHeadApprValue      = sHeadApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_sDeadBodyApprValue      = sBodyApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_sDeadArmApprValue      = sArmApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_sDeadLegApprValue      = sLegApprValue; // Re-Coding Sprite xRisenx
		m_pData[dX][dY].m_iDeadStatus     = iStatus;
		ZeroMemory(m_pData[dX][dY].m_cDeadOwnerName, sizeof(m_pData[dX][dY].m_cDeadOwnerName));
		strcpy(m_pData[dX][dY].m_cDeadOwnerName, cTmpName);
		m_pData[dX][dY].m_dwDeadOwnerTime  = dwTime;
		m_pData[dX][dY].m_iDeadChatMsg = iChatIndex;
		if ((sAppr4 & 0x00F0) != 0)
		{
            m_pData[dX][dY].m_iEffectType = (sAppr4 & 0x00F0) >> 4;
			if (sAction == OBJECTNULLACTION)
			{
                m_pData[dX][dY].m_iEffectFrame = 0;
				m_pData[dX][dY].m_dwEffectTime = dwTime;
			}
			switch (m_pData[dX][dY].m_iEffectType)
            {
                case 1: m_pData[dX][dY].m_iEffectTotalFrame = 13; break;
                case 2: m_pData[dX][dY].m_iEffectTotalFrame = 11; break;
			}
		}
        else
		{
            m_pData[dX][dY].m_iEffectType = 0;
	    }
    }
	return true;
}

#ifdef SHOWALLDAMAGE // Remove Critical xRisenx
bool __fastcall CMapData::bGetOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, int * pStatus, char * pName, char * pAction, char * pFrame, int * pChatIndex, int * pV1, short * pV2)
#else
bool __fastcall CMapData::bGetOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, short * pHeadApprValue, short * pBodyApprValue, short * pArmApprValue, short * pLegApprValue, int * pStatus, char * pName, char * pAction, char * pFrame, int * pChatIndex, short * pV1, short * pV2)
#endif
{
    int dX, dY;

// 	if ((sX < m_sPivotX) || (sX > m_sPivotX + MAPDATASIZEX) ||
// 		(sY < m_sPivotY) || (sY > m_sPivotY + MAPDATASIZEY)) {
// 		ZeroMemory(pName, sizeof(pName));
// 		return false;
// 	}

    dX = sX;// -m_sPivotX;
    dY = sY;// -m_sPivotY;

	*pOwnerType = m_pData[dX][dY].m_sOwnerType;
	*pAction    = m_pData[dX][dY].m_cOwnerAction;
	*pDir       = m_pData[dX][dY].m_cDir;
	*pFrame     = m_pData[dX][dY].m_cOwnerFrame;
	*pAppr1     = m_pData[dX][dY].m_sAppr1;
	*pAppr2     = m_pData[dX][dY].m_sAppr2;
	*pAppr3     = m_pData[dX][dY].m_sAppr3;
	*pAppr4     = m_pData[dX][dY].m_sAppr4;
	*pApprColor = m_pData[dX][dY].m_iApprColor; // v1.4
	*pHeadApprValue = m_pData[dX][dY].m_sHeadApprValue; // Re-Coding Sprite xRisenx
	*pBodyApprValue = m_pData[dX][dY].m_sBodyApprValue; // Re-Coding Sprite xRisenx
	*pArmApprValue  = m_pData[dX][dY].m_sArmApprValue; // Re-Coding Sprite xRisenx
	*pLegApprValue  = m_pData[dX][dY].m_sLegApprValue; // Re-Coding Sprite xRisenx
	*pStatus    = m_pData[dX][dY].m_iStatus;
	*pChatIndex = m_pData[dX][dY].m_iChatMsg;
	*pV1        = m_pData[dX][dY].m_sV1;
	*pV2        = m_pData[dX][dY].m_sV2;

	strcpy(pName, m_pData[dX][dY].m_cOwnerName);

	return true;
}

int CMapData::getChatMsgIndex(uint16_t wObjectID) const 
{ 
	for (uint32_t dX = 0; dX < MAPDATASIZEX; dX++)
	{
		for (uint32_t dY = 0; dY < MAPDATASIZEY; dY++) {

			if (m_pData[dX][dY].m_wObjectID == wObjectID) {
				return m_pData[dX][dY].m_iChatMsg;
			}
			if (m_pData[dX][dY].m_wDeadObjectID == wObjectID) {
				return m_pData[dX][dY].m_iDeadChatMsg;
			}
		}
	}

	return -1;
}


bool __fastcall CMapData::bGetDeadOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, short * pHeadApprValue, short * pBodyApprValue, short * pArmApprValue, short * pLegApprValue, char * pFrame, char * pName, short * pItemSprite, short * pItemSpriteFrame, int * pChatIndex)
{
    int dX, dY;

// 	if ((sX < m_sPivotX) || (sX > m_sPivotX + MAPDATASIZEX) ||
// 		(sY < m_sPivotY) || (sY > m_sPivotY + MAPDATASIZEY)) {
// 
// 		ZeroMemory(pName, sizeof(pName));
// 		*pItemSprite = 0;
// 		return false;
// 	}

    dX = sX;// -m_sPivotX;
    dY = sY;// -m_sPivotY;

	*pOwnerType = m_pData[dX][dY].m_sDeadOwnerType;
	*pDir       = m_pData[dX][dY].m_cDeadDir;
	*pAppr1     = m_pData[dX][dY].m_sDeadAppr1;
	*pAppr2     = m_pData[dX][dY].m_sDeadAppr2;
	*pAppr3     = m_pData[dX][dY].m_sDeadAppr3;
	*pAppr4     = m_pData[dX][dY].m_sDeadAppr4;
	*pApprColor = m_pData[dX][dY].m_iDeadApprColor; // v1.4
	*pHeadApprValue = m_pData[dX][dY].m_sDeadHeadApprValue; // Re-Coding Sprite xRisenx
	*pBodyApprValue = m_pData[dX][dY].m_sDeadBodyApprValue; // Re-Coding Sprite xRisenx
	*pArmApprValue  = m_pData[dX][dY].m_sDeadArmApprValue; // Re-Coding Sprite xRisenx
	*pLegApprValue  = m_pData[dX][dY].m_sDeadLegApprValue; // Re-Coding Sprite xRisenx
	*pFrame     = m_pData[dX][dY].m_cDeadOwnerFrame;
	*pChatIndex = m_pData[dX][dY].m_iDeadChatMsg;

	strcpy(pName, m_pData[dX][dY].m_cDeadOwnerName);

	*pItemSprite      = m_pData[dX][dY].m_sItemSprite;
	*pItemSpriteFrame = m_pData[dX][dY].m_sItemSpriteFrame;

	return true;
}

int CMapData::iObjectFrameCounter(char * cPlayerName, short sViewPointX, short sViewPointY)
{
	int dX,dY, sVal;
	uint64_t dwTime, dwRealTime, dwFrameTime;
	int  iDelay;
	int  iRet, iSoundIndex, iSkipFrame;
	int  cDir, cTotalFrame, cFrameMoveDots;
	static uint64_t S_dwUpdateTime = unixtime();
	int   sWeaponType, sCenterX, sCenterY, sDist;//, sAbsX, sAbsY;

	bool  bAutoUpdate = false, dynObjsNeedUpdate = false;
	short dx, dy;
	long  lPan;

	iRet = 0;
	dwTime = dwRealTime = unixtime();
	if ((dwTime - m_dwFrameTime) >= 70)
		m_dwFrameTime = dwTime;

    sVal = sViewPointX;// -(m_sPivotX * 32);
    sCenterX = (sVal / 32) + ((m_pGame->GetWidth() / 32) / 2);// 10;
    sVal = sViewPointY;// -(m_sPivotY * 32);
    sCenterY = (sVal / 32) + (((m_pGame->GetHeight()-60) / 32) / 2);// 8;
    m_sRectX = m_pGame->m_sVDL_X;// -m_sPivotX;
    m_sRectY = m_pGame->m_sVDL_Y;// -m_sPivotY;
	if ((dwTime - S_dwUpdateTime) > 40) bAutoUpdate = true;
	dynObjsNeedUpdate = (dwTime - m_dwDOframeTime) > 100;

	//for (dX = 0; dX < MAPDATASIZEX; dX++)
	//for (dY = 0; dY < MAPDATASIZEY; dY++)
	for (dX = sViewPointX/32; dX <= (sViewPointX / 32) + (m_pGame->GetWidth() / 32); dX++) //19 +/- 11
	for (dY = sViewPointY/32; dY <= (sViewPointY / 32) + ((m_pGame->GetHeight() - 60) / 32); dY++) //17 +/- 9
	//{
	// Risen added for mob animation fail fix
	//for (dX = 0; dX < MAPDATASIZEX; dX++)
	//for (dY = 0; dY < MAPDATASIZEY; dY++)
	{
        if ((dX <= 0) || (dY <= 0))
            continue;
		sDist = (abs(sCenterX - dX)+abs(sCenterY - dY))/2;
		//lPan = -(sCenterX - dX)*1000;
	// Risen added for mob animation fail fix
		// Dynamic Object
		if (dynObjsNeedUpdate)//00496B99  JBE 00496F43
		{	
			//sDist = (abs(sCenterX - dX)+abs(sCenterY - dY))/2;
			//lPan = -(sCenterX - dX)*1000;
			m_pData[dX][dY].m_iEffectFrame++;
			switch (m_pData[dX][dY].m_iEffectType) {
			case 1:
				if (m_pData[dX][dY].m_iEffectTotalFrame < m_pData[dX][dY].m_iEffectFrame)
					m_pData[dX][dY].m_iEffectFrame = 4;
				break;
			case 2:
				if (m_pData[dX][dY].m_iEffectTotalFrame < m_pData[dX][dY].m_iEffectFrame)
					m_pData[dX][dY].m_iEffectFrame = 3;
				break;
			}
			if (m_pData[dX][dY].m_sDynamicObjectType != 0)
			{	
				m_pData[dX][dY].m_cDynamicObjectFrame++;
				switch (m_pData[dX][dY].m_sDynamicObjectType) {
				case DYNAMICOBJECT_SPIKE:
					if (m_pData[dX][dY].m_cDynamicObjectFrame >= 13)
						m_pData[dX][dY].m_cDynamicObjectFrame = 0;
					break;

				case DYNAMICOBJECT_ICESTORM:
					if (m_pData[dX][dY].m_cDynamicObjectFrame >= 10)
						m_pData[dX][dY].m_cDynamicObjectFrame = 0;
					//if (m_pData[dX][dY].m_cDynamicObjectFrame == 1)
					//	m_pGame->PlaySound('E', 16, sDist);
					break;

				case DYNAMICOBJECT_FIRE:// Firewall
				case DYNAMICOBJECT_FIRE3: // by Snoopy(FireBow)
					if (m_pData[dX][dY].m_cDynamicObjectFrame >= 24)
						m_pData[dX][dY].m_cDynamicObjectFrame = 0;
					if (m_pData[dX][dY].m_cDynamicObjectFrame == 1) {
						m_pGame->PlaySound('E', 9, sDist);
					}
					break;

				case DYNAMICOBJECT_FIRE2:	//  // Crusade buildings burning.
					if (m_pData[dX][dY].m_cDynamicObjectFrame > 27)
						m_pData[dX][dY].m_cDynamicObjectFrame = 0;
					if (m_pData[dX][dY].m_cDynamicObjectFrame == 1) {
						m_pGame->PlaySound('E', 9, sDist);
					}
					if ((m_pData[dX][dY].m_cDynamicObjectFrame % 6) == 0) {
						m_pGame->bAddNewEffect(65, (/*m_sPivotX+*/dX)*32 +(rand()%10-5) +5, (/*m_sPivotY+*/dY)*32, 0, 0, 0, 0);
						m_pGame->bAddNewEffect(67, (/*m_sPivotX+*/dX)*32, (/*m_sPivotY+*/dY)*32, 0, 0, 0, 0);
					}
					break;

				case DYNAMICOBJECT_FISHOBJECT:
					if ((rand() % 12) == 1)
						m_pGame->bAddNewEffect(13, (/*m_sPivotX +*/ dX)*32 + m_pData[dX][dY].m_cDynamicObjectData1, (/*m_sPivotY +*/ dY)*32 + m_pData[dX][dY].m_cDynamicObjectData2, 0, 0, 0);
					break;

				case DYNAMICOBJECT_FISH:
					if ((dwTime - m_pData[dX][dY].m_dwDynamicObjectTime) < 100) break;
					m_pData[dX][dY].m_dwDynamicObjectTime = dwTime;
					if (m_pData[dX][dY].m_cDynamicObjectFrame >= 15) m_pData[dX][dY].m_cDynamicObjectFrame = 0;
					if ((rand() % 15) == 1) m_pGame->bAddNewEffect(13, (/*m_sPivotX +*/ dX)*32 + m_pData[dX][dY].m_cDynamicObjectData1, (/*m_sPivotY +*/ dY)*32 + m_pData[dX][dY].m_cDynamicObjectData2, 0, 0, 0);
					cDir = m_pGame->m_Misc.cGetNextMoveDir(m_pData[dX][dY].m_cDynamicObjectData1, m_pData[dX][dY].m_cDynamicObjectData2, 0, 0);
					switch (cDir) {
					case 1:
						m_pData[dX][dY].m_cDynamicObjectData4 -= 2;
						break;
					case 2:
						m_pData[dX][dY].m_cDynamicObjectData4 -= 2;
						m_pData[dX][dY].m_cDynamicObjectData3 += 2;
						break;
					case 3:
						m_pData[dX][dY].m_cDynamicObjectData3 += 2;
						break;
					case 4:
						m_pData[dX][dY].m_cDynamicObjectData3 += 2;
						m_pData[dX][dY].m_cDynamicObjectData4 += 2;
						break;
					case 5:
						m_pData[dX][dY].m_cDynamicObjectData4 += 2;
						break;
					case 6:
						m_pData[dX][dY].m_cDynamicObjectData3 -= 2;
						m_pData[dX][dY].m_cDynamicObjectData4 += 2;
						break;
					case 7:
						m_pData[dX][dY].m_cDynamicObjectData3 -= 2;
						break;
					case 8:
						m_pData[dX][dY].m_cDynamicObjectData3 -= 2;
						m_pData[dX][dY].m_cDynamicObjectData4 -= 2;
						break;
					}

					if (m_pData[dX][dY].m_cDynamicObjectData3 < -12) m_pData[dX][dY].m_cDynamicObjectData3 = -12;
					if (m_pData[dX][dY].m_cDynamicObjectData3 >  12) m_pData[dX][dY].m_cDynamicObjectData3 =  12;
					if (m_pData[dX][dY].m_cDynamicObjectData4 < -12) m_pData[dX][dY].m_cDynamicObjectData4 = -12;
					if (m_pData[dX][dY].m_cDynamicObjectData4 >  12) m_pData[dX][dY].m_cDynamicObjectData4 =  12;

					m_pData[dX][dY].m_cDynamicObjectData1 += m_pData[dX][dY].m_cDynamicObjectData3;
					m_pData[dX][dY].m_cDynamicObjectData2 += m_pData[dX][dY].m_cDynamicObjectData4;
					break;

				case DYNAMICOBJECT_PCLOUD_BEGIN:
					if (m_pData[dX][dY].m_cDynamicObjectFrame >= 8) {
						m_pData[dX][dY].m_sDynamicObjectType  = DYNAMICOBJECT_PCLOUD_LOOP;
						m_pData[dX][dY].m_cDynamicObjectFrame = rand() % 8;
					}
					break;

				case DYNAMICOBJECT_PCLOUD_LOOP:
					if (m_pData[dX][dY].m_cDynamicObjectFrame >= 8) {
						m_pData[dX][dY].m_cDynamicObjectFrame = 0;
					}
					break;

				case DYNAMICOBJECT_PCLOUD_END:
					if (m_pData[dX][dY].m_cDynamicObjectFrame >= 8) {
						m_pData[dX][dY].m_sDynamicObjectType  = 0;
					}
					break;

				case DYNAMICOBJECT_ARESDENFLAG:
					if (m_pData[dX][dY].m_cDynamicObjectFrame >= 4) {
						m_pData[dX][dY].m_cDynamicObjectFrame  = 0;
					}
					break;

				case DYNAMICOBJECT_ELVINEFLAG:
					if (m_pData[dX][dY].m_cDynamicObjectFrame >= 8) {
						m_pData[dX][dY].m_cDynamicObjectFrame  = 4;
					}
					break;
				}	
			}	
		}

		// Dead think 00496F43
		if (m_pData[dX][dY].m_sDeadOwnerType != 0)
			if ((m_pData[dX][dY].m_cDeadOwnerFrame >= 0) && ((dwTime - m_pData[dX][dY].m_dwDeadOwnerTime) > 150)) {
				m_pData[dX][dY].m_dwDeadOwnerTime = dwTime;
				m_pData[dX][dY].m_cDeadOwnerFrame++;
				if (iRet == 0) {	
					iRet = -1;
					S_dwUpdateTime = dwTime;
				}
				if (m_pData[dX][dY].m_cDeadOwnerFrame > 10) {	
					m_pData[dX][dY].m_wDeadObjectID  = 0;
					m_pData[dX][dY].m_sDeadOwnerType = 0;
					ZeroMemory(m_pData[dX][dY].m_cDeadOwnerName, sizeof(m_pData[dX][dY].m_cDeadOwnerName));
				}	
			}

			// Alive thing 00496FD8
		if (m_pData[dX][dY].m_sOwnerType != 0) {	
				bool isClientPlayer = (memcmp(m_pData[dX][dY].m_cOwnerName, cPlayerName, 10) == 0) ? true : false; //(dX == sCenterX && dY == sCenterY);
				sDist = (abs(sCenterX - dX)+abs(sCenterY - dY))/2;
				lPan = -(sCenterX - dX)*1000;
				switch (m_pData[dX][dY].m_cOwnerAction) {
				case OBJECTATTACK:
				case OBJECTATTACKMOVE:
					iDelay = (m_pData[dX][dY].m_iStatus & 0x000F)*12;
					break;
				case OBJECTMAGIC:
					if(isClientPlayer)
						iDelay = -(m_pGame->m_cSkillMastery[SKILL_MAGIC] * 0.13);
					else
						iDelay = -(100 * 0.13);
					break;
				default:
					iDelay = 0;
					break;
				}

				if ((m_pData[dX][dY].m_iStatus & STATUS_FROZEN) != 0)
					if(m_pData[dX][dY].m_sOwnerType <= 6)
						// Player
						iDelay += (m_stFrame[m_pData[dX][dY].m_sOwnerType][m_pData[dX][dY].m_cOwnerAction].m_sFrameTime)>>2;
					else
						// Monster
						iDelay += (m_stFrame[m_pData[dX][dY].m_sOwnerType][m_pData[dX][dY].m_cOwnerAction].m_sFrameTime)/2;

				if ((m_pData[dX][dY].m_iStatus & STATUS_RELICHOLDER) != 0)
					if(m_pData[dX][dY].m_sOwnerType <= 6)
						// Player
						iDelay += (m_stFrame[m_pData[dX][dY].m_sOwnerType][m_pData[dX][dY].m_cOwnerAction].m_sFrameTime)/3;


				dwFrameTime = m_stFrame[m_pData[dX][dY].m_sOwnerType][m_pData[dX][dY].m_cOwnerAction].m_sFrameTime + iDelay;

				if ((dwTime - m_pData[dX][dY].m_dwOwnerTime) > dwFrameTime)
				{	
					if ((dwTime - m_pData[dX][dY].m_dwOwnerTime) >= (dwFrameTime +dwFrameTime) && dwFrameTime != 0)
					{	
						iSkipFrame = ((dwTime - m_pData[dX][dY].m_dwOwnerTime)/dwFrameTime);
						if (iSkipFrame > 3) iSkipFrame = 3;
						m_pData[dX][dY].m_cOwnerFrame += iSkipFrame;
					}else
					{	
						m_pData[dX][dY].m_cOwnerFrame++;
					}
					m_pData[dX][dY].m_dwOwnerTime = dwTime;
					if (iRet == 0)
					{	
						iRet = -1;
						S_dwUpdateTime = dwTime;
					}
					if(isClientPlayer)
					{	
						iRet = 1;
						S_dwUpdateTime = dwTime;
						if ((dwRealTime - m_dwFrameCheckTime) > dwFrameTime)
							m_dwFrameAdjustTime = ((dwRealTime - m_dwFrameCheckTime) - dwFrameTime);
						m_dwFrameCheckTime = dwRealTime;
					}
					if (m_pData[dX][dY].m_cOwnerFrame > m_stFrame[m_pData[dX][dY].m_sOwnerType][m_pData[dX][dY].m_cOwnerAction].m_sMaxFrame)
					{	
						//if (   (m_sRectX <= dX) && ((m_sRectX + 20) >= dX)
					//	&& (m_sRectY <= dY) && ((m_sRectY + 15) >= dY)) {
					//BESK RESOLUTION
					if (   (m_sRectX <= dX) && ((m_sRectX + 24) >= dX)
						&& (m_sRectY <= dY) && ((m_sRectY + 18) >= dY)) {
					//end resolution
								if (m_pData[dX][dY].m_cOwnerAction == OBJECTDYING) {	
									m_pData[dX][dY].m_wDeadObjectID        = m_pData[dX][dY].m_wObjectID;
									m_pData[dX][dY].m_sDeadOwnerType       = m_pData[dX][dY].m_sOwnerType;
									m_pData[dX][dY].m_cDeadDir             = m_pData[dX][dY].m_cDir;
									m_pData[dX][dY].m_sDeadAppr1           = m_pData[dX][dY].m_sAppr1;
									m_pData[dX][dY].m_sDeadAppr2           = m_pData[dX][dY].m_sAppr2;
									m_pData[dX][dY].m_sDeadAppr3           = m_pData[dX][dY].m_sAppr3;
									m_pData[dX][dY].m_sDeadAppr4           = m_pData[dX][dY].m_sAppr4;
									m_pData[dX][dY].m_iDeadApprColor       = m_pData[dX][dY].m_iApprColor;
									m_pData[dX][dY].m_sDeadHeadApprValue   = m_pData[dX][dY].m_sHeadApprValue; // Re-Coding Sprite xRisenx
									m_pData[dX][dY].m_sDeadBodyApprValue   = m_pData[dX][dY].m_sBodyApprValue; // Re-Coding Sprite xRisenx
									m_pData[dX][dY].m_sDeadArmApprValue   = m_pData[dX][dY].m_sArmApprValue; // Re-Coding Sprite xRisenx
									m_pData[dX][dY].m_sDeadLegApprValue   = m_pData[dX][dY].m_sLegApprValue; // Re-Coding Sprite xRisenx
									m_pData[dX][dY].m_iDeadStatus          = m_pData[dX][dY].m_iStatus;
									m_pData[dX][dY].m_iDeadChatMsg         = m_pData[dX][dY].m_iChatMsg;
									m_pData[dX][dY].m_cDeadOwnerFrame	   = -1;
									memcpy(m_pData[dX][dY].m_cDeadOwnerName, m_pData[dX][dY].m_cOwnerName, 11);
									m_pData[dX][dY].m_wObjectID  = 0;
									m_pData[dX][dY].m_sOwnerType = 0;
									ZeroMemory(m_pData[dX][dY].m_cOwnerName, sizeof(m_pData[dX][dY].m_cOwnerName));
									m_iObjectIDcacheLocX[m_pData[dX][dY].m_wDeadObjectID] = -1*m_iObjectIDcacheLocX[m_pData[dX][dY].m_wDeadObjectID]; //dX;
									m_iObjectIDcacheLocY[m_pData[dX][dY].m_wDeadObjectID] = -1*m_iObjectIDcacheLocY[m_pData[dX][dY].m_wDeadObjectID]; //dY;
								}else
								{	
									m_pData[dX][dY].m_cOwnerAction = OBJECTSTOP;
									m_pData[dX][dY].m_dwOwnerTime  = dwTime;
									m_pData[dX][dY].m_cOwnerFrame  = 0;
								}
								if(isClientPlayer)
								{	
									iRet = 2;
									S_dwUpdateTime = dwTime;
								}
						}else
						{	
							m_pData[dX][dY].m_wObjectID  = 0;
							m_pData[dX][dY].m_sOwnerType = 0;
							ZeroMemory(m_pData[dX][dY].m_cOwnerName, sizeof(m_pData[dX][dY].m_cOwnerName));
							if (m_pGame->m_pChatMsgList[ m_pData[dX][dY].m_iChatMsg ] != 0) {	
								delete m_pGame->m_pChatMsgList[ m_pData[dX][dY].m_iChatMsg ];
								m_pGame->m_pChatMsgList[ m_pData[dX][dY].m_iChatMsg ] = 0;
							}	
						}	
					}
					if ( m_pData[dX][dY].m_cOwnerAction == OBJECTSTOP ) {
						switch (m_pData[dX][dY].m_sOwnerType) 
						{
						case 1:
						case 2:
						case 3:
						case 4:
						case 5:
						case 6: // glowing armor/weapon
							if (m_pData[dX][dY].m_cOwnerFrame % 3)
							{	
								if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0))
								{	
									m_pGame->bAddNewEffect(54, (/*m_sPivotX+*/dX)*32 +(rand()%20-10), (/*m_sPivotY+*/dY)*32 -(rand()%50) -5, 0, 0, -(rand()%8), 0);
								}
								// Snoopy: Angels
								//if (( rand() % ((m_pData[dX][dY].m_iStatus & 0x00000F00) >> 8) > rand()%6) // Angel stars
								//	&& ((m_pData[dX][dY].m_iStatus & 0x0000F000) != 0) 
								//	&& ((m_pData[dX][dY].m_iStatus & 0x10) == 0)) 
								//{	
								//	m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +(rand()%20+15), (m_sPivotY+dY)*32 -(rand()%40) -30, NULL, NULL, -(rand()%8), 0);
								//}	
							}
							break;
						case 40: // ESG
						case 41: // GMG
						case 42: // ManaStone
							if ((rand() % 40) == 25)
							{	m_pGame->bAddNewEffect(54, (/*m_sPivotX+*/dX)*32 +(rand()%60-30), (/*m_sPivotY+*/dY)*32 -(rand()%100) -5, 0, 0, -(rand()%12), 0);
							m_pGame->bAddNewEffect(54, (/*m_sPivotX+*/dX)*32 +(rand()%60-30), (/*m_sPivotY+*/dY)*32 -(rand()%100) -5, 0, 0, -(rand()%12), 0);
							m_pGame->bAddNewEffect(54, (/*m_sPivotX+*/dX)*32 +(rand()%60-30), (/*m_sPivotY+*/dY)*32 -(rand()%100) -5, 0, 0, -(rand()%12), 0);
							m_pGame->bAddNewEffect(54, (/*m_sPivotX+*/dX)*32 +(rand()%60-30), (/*m_sPivotY+*/dY)*32 -(rand()%100) -5, 0, 0, -(rand()%12), 0);
							}
							break;
						case 65: // IceGolem
							if( m_pData[dX][dY].m_cOwnerFrame == 3 ) {	
								m_pGame->bAddNewEffect( 75, (m_sPivotX+dX)*32 +(rand()%40-20), (m_sPivotY+dY)*32 +(rand()%40-20), 0, 0, 0);
								m_pGame->bAddNewEffect( 75, (m_sPivotX+dX)*32 +(rand()%40-20), (m_sPivotY+dY)*32 +(rand()%40-20), 0, 0, 0);
							}
							if( m_pData[dX][dY].m_cOwnerFrame == 2 ) {
								m_pGame->bAddNewEffect( 76, (m_sPivotX+dX)*32 +(rand()%40-20), (m_sPivotY+dY)*32 +(rand()%40-20), 0, 0, 0);
								m_pGame->bAddNewEffect( 76, (m_sPivotX+dX)*32 +(rand()%40-20), (m_sPivotY+dY)*32 +(rand()%40-20), 0, 0, 0);
							}
							if( m_pData[dX][dY].m_cOwnerFrame == 1 ) {
								m_pGame->bAddNewEffect( 77, (m_sPivotX+dX)*32 +(rand()%40-20), (m_sPivotY+dY)*32 +(rand()%40-20), 0, 0, 0);
								m_pGame->bAddNewEffect( 77, (m_sPivotX+dX)*32 +(rand()%40-20), (m_sPivotY+dY)*32 +(rand()%40-20), 0, 0, 0);
							}
							break;
						}	
					} 
					else if ( m_pData[dX][dY].m_cOwnerAction == OBJECTMOVE ) { //2 //004977BF
						switch (m_pData[dX][dY].m_sOwnerType) {
						case 1:
						case 2:
						case 3:
						case 4:
						case 5:
						case 6:
						case 46: // TK
						case 53: // Beholder
						case 54: // Dark-Elf
							if ((m_pData[dX][dY].m_cOwnerFrame == 1) || (m_pData[dX][dY].m_cOwnerFrame == 5)) {
								m_pGame->PlaySound('C', 8, sDist, lPan);
								if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0)) {
									cTotalFrame = 8;
									cFrameMoveDots = 32 / cTotalFrame;
									dx = dy = 0;
									switch (m_pData[dX][dY].m_cDir) {
									case 1 : dy =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
									case 2 : dy =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
									case 3 : dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
									case 4 : dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
									case 5 : dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
									case 6 : dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
									case 7 : dx =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
									case 8 : dx =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
									}
									m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +dx +(rand()%20-10), (m_sPivotY+dY)*32 +dy -(rand()%50) -5, 0, 0, -(rand()%8), 0);
									m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +dx +(rand()%20-10), (m_sPivotY+dY)*32 +dy -(rand()%50) -5, 0, 0, -(rand()%8), 0);
								}
								// Snoopy: Angels
								if (   (((m_pData[dX][dY].m_iStatus & 0x00000F00) >> 8) > rand()%6) // Angel stars
									&& ((m_pData[dX][dY].m_iStatus & 0x0000F000) != 0) 
									&& ((m_pData[dX][dY].m_iStatus & 0x10) == 0)) {
										m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +(rand()%15+10), (m_sPivotY+dY)*32 -(rand()%30) -50, 0, 0, -(rand()%8), 0);
								}	
							}
							break;

						case 82: // Snoopy: Sorceress
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 149, sDist, lPan);
							break;

						case 83: // Snoopy: ATK
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 142, sDist, lPan);
							break;

						case 84: // Snoopy: MasterElf
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('C', 10, sDist, lPan);
							break;

						case 85: // Snoopy: DSK
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 147, sDist, lPan);
							break;

						case 10: // Slime
						case 50: // TW
						case 127: // Enraged Tigerworm
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 1, sDist, lPan);
							break;

						case 11: // SKel
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 13, sDist, lPan);
							break;

						case 123: // Enraged Cyclops
						case 13: // Cyclops
						case 49: // HC
						case 126: // Enraged Hellclaw
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 41, sDist, lPan);
							break;

						case 14: // Orc
						case 48: // SK
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 9, sDist, lPan);
							break;

						case 16: // Ant
						case 43: // LWBeetle
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 29, sDist, lPan);
							break;

						case 17: // Scorpion
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 21, sDist, lPan);
							break;

						case 18: // Zombie
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 17, sDist, lPan);
							break;

						case 22: // Snake
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 25, sDist, lPan);
							break;

						case 23: // Clay-Golem
						case 52: // Gargoyle
						case 125: // Enraged Gagoyle
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 37, sDist, lPan);
							break;

						case 27: // HH
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 5, sDist, lPan);
							break;

						//case 122: // Enraged Troll
						case 28: // Troll
						case 78: // Snoopy: Ajout Minaus
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 46, sDist, lPan);
							break;

						case 29: // Ogre
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 51, sDist, lPan);
							break;

						case 30: // Liche
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 55, sDist, lPan);
							break;

						case 31: // DD
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 59, sDist, lPan);
							break;

						case 32: // Uni
						case 45: // GHKABS
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 63, sDist, lPan);
							break;

						case 33: // WW
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 67, sDist, lPan);
							break;

						case 55://Rabbit
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 71, sDist, lPan);
							break;

						case 56://Cat
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 72, sDist, lPan);
							break;

						case 57://Giant-Frog
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 73, sDist, lPan);
							break;

						case 58://Mountain Giant
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 87, sDist, lPan);
							break;

						case 59://Ettin
						case 77: // Snoopy: MasterMageOrc
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 91, sDist, lPan);
							break;

						case 60://Cannibal Plant
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 95, sDist, lPan);
							break;

						case 61://Rudolph
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('C', 11, sDist, lPan);
							break;

						case 62: // DireBoar
						case 74: // Snoopy: GiantCrayFish
						case 88: // Snoopy: Barbarian
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 87, sDist, lPan);
							break;

						case 63://Frost
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 25, sDist, lPan);
							break;

						case 12: // Stone-Golem
						case 47: // BG
						case 65: // Snoopy: IceGolem
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 33, sDist, lPan);
							break;
							/*	case 65: // IceGolem particulier a la v2.20, absent de la v3.51
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
							{	m_pGame->PlaySound('M', 33, sDist, lPan);
							switch (m_pData[dX][dY].m_cDir) {
							case 1 : dx = 0; dy = -1; break;
							case 2 : dy = -1; dx = 1; break;
							case 3 : dx = 1; dy = 0; break;
							case 4 : dx = 1; dy = 1; break;
							case 5 : dx = 0; dy = 1; break;
							case 6 : dy = 1; dx = -1; break;
							case 7 : dx = -1; dy = 0; break;
							case 8 : dx = -1; dy = -1; break;
							}
							//m_pGame->bAddNewEffect( 75, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, dx, dy, 0 );
							//m_pGame->bAddNewEffect( 76, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, dx, dy, 0 );
							//m_pGame->bAddNewEffect( 77, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, dx, dy, 0 );
							}
							break;*/

						case 73: // Snoopy: Fire-Wyvern
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 106, sDist, lPan);
							break;

						case 80: // Snoopy: Tentocle
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 110, sDist, lPan);
							break;

						case 72: // Snoopy: Claw Turtle
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 114, sDist, lPan);
							break;

						case 71: // Snoopy: Centaurus
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 117, sDist, lPan);
							break;

						case 76: // Snoopy: Giant Tree
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 122, sDist, lPan);
							break;

						case 75: // Snoopy: Giant Lizard
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 126, sDist, lPan);
							break;

						case 70: // Snoopy: Dragon
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 130, sDist, lPan);
							break;

						case 79: // Snoopy: Nizie
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 134, sDist, lPan);
							break;

						case 81: // void CGame::DrawDruncncity();Abaddon
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 136, sDist, lPan);
							break;

						default:
							if ((m_pData[dX][dY].m_cOwnerFrame == 1) || (m_pData[dX][dY].m_cOwnerFrame == 3))
								m_pGame->PlaySound('C', 8, sDist, lPan);
							break;
						}	
					} 
					else if ( m_pData[dX][dY].m_cOwnerAction == OBJECTRUN ) {
						switch (m_pData[dX][dY].m_sOwnerType) {
								case 1:
								case 2:
								case 3:
								case 4:
								case 5:
								case 6:
								case 44: // GHK
									if ((m_pData[dX][dY].m_cOwnerFrame == 1) || (m_pData[dX][dY].m_cOwnerFrame == 5)) {
										cTotalFrame = 8;
										cFrameMoveDots = 32 / cTotalFrame;
										dx = dy = 0;
										switch (m_pData[dX][dY].m_cDir) {
										case 1 : dy =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
										case 2 : dy =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
										case 3 : dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
										case 4 : dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
										case 5 : dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
										case 6 : dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
										case 7 : dx =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
										case 8 : dx =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
										}
										if (m_pGame->m_weather >= WEATHER_LIGHTRAIN && m_pGame->m_weather <= WEATHER_HEAVYRAIN)
											m_pGame->bAddNewEffect(32, (m_sPivotX+dX)*32 +dx, (m_sPivotY+dY)*32 +dy, 0, 0, 0, 0);
										else m_pGame->bAddNewEffect(14, (m_sPivotX+dX)*32 +dx, (m_sPivotY+dY)*32 +dy, 0, 0, 0, 0);
										if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0)) {
											m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +dx +(rand()%20-10), (m_sPivotY+dY)*32 +dy -(rand()%50) -5, 0, 0, -(rand()%8), 0);
											m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +dx +(rand()%20-10), (m_sPivotY+dY)*32 +dy -(rand()%50) -5, 0, 0, -(rand()%8), 0);
										}
										// Snoopy: Angels
										if (  (((m_pData[dX][dY].m_iStatus & 0x00000F00) >> 8) > rand()%6) // Angel stars
											&& ((m_pData[dX][dY].m_iStatus & 0x0000F000) != 0) 
											&& ((m_pData[dX][dY].m_iStatus & 0x10) == 0)) {
												m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +(rand()%15+10), (m_sPivotY+dY)*32 -(rand()%30) -50, 0, 0, -(rand()%8), 0);
										}
										m_pGame->PlaySound('C', 10, sDist, lPan);
									}
									break;
								}
					}
					else if ( m_pData[dX][dY].m_cOwnerAction == OBJECTATTACKMOVE ) {
						switch (m_pData[dX][dY].m_sOwnerType) {
						case 1:
						case 2:
						case 3:
						case 4:
						case 5:
						case 6:
							if (m_pData[dX][dY].m_cOwnerFrame == 2) {
								m_pGame->PlaySound('C', 4, sDist); //bruit fleche
								cTotalFrame = 8;
								cFrameMoveDots = 32 / cTotalFrame;
								dx = dy = 0;
								switch (m_pData[dX][dY].m_cDir) {
								case 1 : dy =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
								case 2 : dy =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
								case 3 : dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
								case 4 : dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
								case 5 : dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
								case 6 : dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
								case 7 : dx =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
								case 8 : dx =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy =  cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
								}
								if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0)) {
									m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +dx +(rand()%20-10), (m_sPivotY+dY)*32 +dy -(rand()%50) -5, 0, 0, -(rand()%8), 0);
									m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +dx +(rand()%20-10), (m_sPivotY+dY)*32 +dy -(rand()%50) -5, 0, 0, -(rand()%8), 0);
								}
								//Snoopy: Angels						
								if (  (((m_pData[dX][dY].m_iStatus & 0x00000F00) >> 8) > rand()%6) // Angel stars
									&& ((m_pData[dX][dY].m_iStatus & 0x0000F000) != 0) 
									&& ((m_pData[dX][dY].m_iStatus & 0x10) == 0)) {
										m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +(rand()%15+10), (m_sPivotY+dY)*32 -(rand()%30) -50, 0, 0, -(rand()%8), 0);
								}
							}else if (m_pData[dX][dY].m_cOwnerFrame == 4) {
								if(m_pGame->m_weather >= WEATHER_LIGHTRAIN && (m_pGame->m_weather <= WEATHER_HEAVYRAIN)) {
									m_pGame->bAddNewEffect(32, (m_sPivotX+dX)*32 +((rand() % 20)-10), (m_sPivotY+dY)*32 +((rand() % 20)-10), 0, 0, 0, 0);
									m_pGame->bAddNewEffect(32, (m_sPivotX+dX)*32 +((rand() % 20)-10), (m_sPivotY+dY)*32 +((rand() % 20)-10), 0, 0, 0, 0);
									m_pGame->bAddNewEffect(32, (m_sPivotX+dX)*32 +((rand() % 20)-10), (m_sPivotY+dY)*32 +((rand() % 20)-10), 0, 0, 0, 0);
									m_pGame->bAddNewEffect(32, (m_sPivotX+dX)*32 +((rand() % 20)-10), (m_sPivotY+dY)*32 +((rand() % 20)-10), 0, 0, 0, 0);
									m_pGame->bAddNewEffect(32, (m_sPivotX+dX)*32 +((rand() % 20)-10), (m_sPivotY+dY)*32 +((rand() % 20)-10), 0, 0, 0, 0);
								}else	{
									m_pGame->bAddNewEffect(14, (m_sPivotX+dX)*32 +((rand() % 20)-10), (m_sPivotY+dY)*32 +((rand() % 20)-10), 0, 0, 0, 0);
									m_pGame->bAddNewEffect(14, (m_sPivotX+dX)*32 +((rand() % 20)-10), (m_sPivotY+dY)*32 +((rand() % 20)-10), 0, 0, 0, 0);
									m_pGame->bAddNewEffect(14, (m_sPivotX+dX)*32 +((rand() % 20)-10), (m_sPivotY+dY)*32 +((rand() % 20)-10), 0, 0, 0, 0);
									m_pGame->bAddNewEffect(14, (m_sPivotX+dX)*32 +((rand() % 20)-10), (m_sPivotY+dY)*32 +((rand() % 20)-10), 0, 0, 0, 0);
									m_pGame->bAddNewEffect(14, (m_sPivotX+dX)*32 +((rand() % 20)-10), (m_sPivotY+dY)*32 +((rand() % 20)-10), 0, 0, 0, 0);
								}
								m_pGame->PlaySound('C', 11, sDist, lPan);
							}else if (m_pData[dX][dY].m_cOwnerFrame == 5) {
								sWeaponType = ((m_pData[dX][dY].m_sAppr2 & 0x0FF0) >> 4);
								if ((sWeaponType >= 1) && (sWeaponType <= 2)) {
									m_pGame->PlaySound('C', 1, sDist, lPan);
								}else if ((sWeaponType >= 3) && (sWeaponType <= 19)) {
									m_pGame->PlaySound('C', 2, sDist, lPan);
								}else if ((sWeaponType >= 20) && (sWeaponType <= 39)) {
									m_pGame->PlaySound('C', 18, sDist, lPan);
								}else if ((sWeaponType >= 41) && (sWeaponType <= 59)) {
									m_pGame->PlaySound('C', 3, sDist, lPan);
								}	
							}
							break;
						}	
								}
					else if ( (m_pData[dX][dY].m_cOwnerAction == OBJECTATTACK) ) {
						switch (m_pData[dX][dY].m_sOwnerType) {
						case 65: // IceGolem
							if (m_pData[dX][dY].m_cOwnerFrame == 2) {
								m_pGame->bAddNewEffect(73, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, 0, 0, 0, 0 );
							}
							break;
						case 87: // void CGame::DrawDruncncity();Crossbow Turret (Heldenian)
							if (m_pData[dX][dY].m_cOwnerFrame == 2) {
								m_pGame->bAddNewEffect(250, m_sPivotX + m_pData[dX][dY].m_sV1, m_sPivotY + m_pData[dX][dY].m_sV2
									, m_sPivotX + m_pData[dX][dY].m_sV1 + dX, m_sPivotY + m_pData[dX][dY].m_sV2 + dY, 0, 87);
								//m_pGame->PlaySound('E', 43, sDist, lPan); // Son "wouufffff"
							}
							break;
						case 89: // void CGame::DrawDruncncity();AGT (Heldenian)
							if (m_pData[dX][dY].m_cOwnerFrame == 2) {
								m_pGame->bAddNewEffect(2, m_sPivotX + m_pData[dX][dY].m_sV1, m_sPivotY + m_pData[dX][dY].m_sV2
									, m_sPivotX + m_pData[dX][dY].m_sV1 + dX, m_sPivotY + m_pData[dX][dY].m_sV2 + dY, 0, 89);
								//m_pGame->PlaySound('E', 43, sDist, lPan); // Son "wouufffff"
							}
							break;
						case 1:
						case 2:
						case 3:
						case 4:
						case 5:
						case 6:
							if ((m_pData[dX][dY].m_sV3 >= 20) && (m_pData[dX][dY].m_cOwnerFrame == 2)) {
								//if  ( m_pGame->bHasHeroSet(m_pData[dX][dY].m_sAppr3, m_pData[dX][dY].m_sAppr4, m_pData[dX][dY].m_sOwnerType) == 1)
								if (m_pGame->bHasHeroSet(m_pData[dX][dY].m_sHeadApprValue, m_pData[dX][dY].m_sBodyApprValue, m_pData[dX][dY].m_sArmApprValue, m_pData[dX][dY].m_sLegApprValue, m_pData[dX][dY].m_sOwnerType) == 1)
								{	
									m_pGame->bAddNewEffect(243, m_sPivotX + dX, m_sPivotY + dY, m_sPivotX + dX , m_sPivotY + dY, 0, 1);
								}
								switch (m_pData[dX][dY].m_sOwnerType) {
								case 1:
								case 2:
								case 3: 
									m_pGame->PlaySound('C', 23, sDist, lPan);	
									break;

								case 4:
								case 5:
								case 6:
									m_pGame->PlaySound('C', 24, sDist, lPan);	
									break;
								}
							}

							if (m_pData[dX][dY].m_cOwnerFrame == 5) {
								if ((m_pData[dX][dY].m_sAppr2 & 0xF000) != 0){ // not Peace mode
									if (m_pData[dX][dY].m_sV3 != 1){ // autre que corp à corp
										m_pGame->bAddNewEffect(m_pData[dX][dY].m_sV3, m_sPivotX + dX, m_sPivotY + dY
											, m_sPivotX + dX + m_pData[dX][dY].m_sV1, m_sPivotY + dY + m_pData[dX][dY].m_sV2
											, 0, m_pData[dX][dY].m_sOwnerType);
										if (m_pData[dX][dY].m_sV3 >= 20) m_pGame->PlaySound('E', 43, sDist, lPan); // Son "loupé"
									}
									if (((m_pData[dX][dY].m_sAppr2 & 0x0FF0) >> 4) == 15){ // StormBlade
										m_pGame->bAddNewEffect(81, m_sPivotX + dX, m_sPivotY + dY
											, m_sPivotX + dX + m_pData[dX][dY].m_sV1, m_sPivotY + dY + m_pData[dX][dY].m_sV2
											, 0, m_pData[dX][dY].m_sOwnerType);
									}else	{	
										m_pGame->bAddNewEffect(250, m_sPivotX + dX, m_sPivotY + dY
											, m_sPivotX + dX + m_pData[dX][dY].m_sV1, m_sPivotY + dY + m_pData[dX][dY].m_sV2
											, 0, m_pData[dX][dY].m_sOwnerType);
									}	
								}
								// Weapon Glare = m_sAppr4 & 0x000F
								if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0)) {
									m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +(rand()%20-10), (m_sPivotY+dY)*32 -(rand()%50) -5, 0, 0, -(rand()%8), 0);
									m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +(rand()%20-10), (m_sPivotY+dY)*32 -(rand()%50) -5, 0, 0, -(rand()%8), 0);
								}							
								//Snoopy: Angels
								if (  (((m_pData[dX][dY].m_iStatus & 0x00000F00) >> 8) > rand()%6) // Angel stars
									&& ((m_pData[dX][dY].m_iStatus & 0x0000F000) != 0) 
									&& ((m_pData[dX][dY].m_iStatus & 0x10) == 0)) {
										m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +(rand()%15+10), (m_sPivotY+dY)*32 -(rand()%30) -50, 0, 0, -(rand()%8), 0);
								}
							}
							break;

						default:
							if (m_pData[dX][dY].m_cOwnerFrame == 2) {
								if  (m_pData[dX][dY].m_sV3 == 2 ){ // Arrow flying...
									m_pGame->bAddNewEffect(2, m_sPivotX + dX, m_sPivotY + dY
										, m_sPivotX + dX + m_pData[dX][dY].m_sV1
										, m_sPivotY + dY + m_pData[dX][dY].m_sV2
										, 0, m_pData[dX][dY].m_sOwnerType*1000);
								}	
							}
							break;
						}

						switch (m_pData[dX][dY].m_sOwnerType) {
							case 1:
							case 2:
							case 3:
							case 4:
							case 5:
							case 6:
								if ((m_pData[dX][dY].m_sAppr2 & 0xF000) != 0){
									sWeaponType = ((m_pData[dX][dY].m_sAppr2 & 0x0FF0) >> 4);
									if ((sWeaponType >= 1) && (sWeaponType <= 2)){
										if (m_pData[dX][dY].m_cOwnerFrame == 5){
											m_pGame->PlaySound('C', 1, sDist, lPan);
										}
									}else if ((sWeaponType >= 3) && (sWeaponType <= 19)){
										if (m_pData[dX][dY].m_cOwnerFrame == 5){
											m_pGame->PlaySound('C', 2, sDist, lPan);
										}
									}else if ((sWeaponType >= 20) && (sWeaponType <= 39)){
										if (m_pData[dX][dY].m_cOwnerFrame == 2){
											m_pGame->PlaySound('C', 18, sDist, lPan);
										}
									}else if ((sWeaponType >= 41) && (sWeaponType <= 59)){
										if (m_pData[dX][dY].m_cOwnerFrame == 3){
											m_pGame->PlaySound('C', 3, sDist, lPan);
										}	
									}	
								}
								break;

							case 83: // Snoopy: ATK
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 140, sDist, lPan);
								break;

							case 84: // Snoopy: MasterElf
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('C', 8, sDist, lPan);
								break;

							case 85: // Snoopy: DSK
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 145, sDist, lPan);
								break;

							case 53: // Beholder
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('E', 46, sDist, lPan);
								break;

							case 54: // DE
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('C', 3, sDist, lPan);
								break;

							case 50: // TW
							case 127: // Enraged Tigerworm
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('C', 1, sDist, lPan);
								break;

							case 10: // Slime
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 2, sDist, lPan);
								break;

							case 11: // Skell
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 14, sDist, lPan);
								break;

							case 12: // Stone-Golem
							case 65: // ICeGolem
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 34, sDist, lPan);
								break;

							case 123: // Enraged Cyclops
							case 13: // Cyclops
							case 49: // HC
							case 126: // Enraged Hellclaw
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 42, sDist, lPan);
								break;

							case 44: // GHK
							case 45: // GHKABS
							case 46: // TK
							case 52: // GG
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('C', 2, sDist, lPan);
								break;

							case 14: // orc
							case 48: // SK
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 10, sDist, lPan);
								break;

							case 16: // Ant
							case 43: // LWB
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 30, sDist, lPan);
								break;

							case 17: // Scorpion
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 22, sDist, lPan);
								break;

							case 18: // Zombie
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 18, sDist, lPan);
								break;

							case 22: // Snake
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 26, sDist, lPan);
								break;

							case 23: // Clay-Golem
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 38, sDist, lPan);
								break;

							case 27: // HH
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 6, sDist, lPan);
								break;

							//case 122: // Enraged Troll
							case 28: // Troll
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 47, sDist, lPan);
								break;

							case 29: // Ogre
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 52, sDist, lPan);
								break;

							case 30: // Liche
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 56, sDist, lPan);
								break;

							case 31: // DD
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 60, sDist, lPan);
								break;

							case 32: // Uni
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 64, sDist, lPan);
								break;

							case 33: // WW
								if (m_pData[dX][dY].m_cOwnerFrame == 1)
									m_pGame->PlaySound('M', 68, sDist, lPan);
								break;

							case 55://Rabbit
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 75, sDist, lPan);
								break;

							case 56://Cat
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 76, sDist, lPan);
								break;

							case 57://Giant-Frog
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 77, sDist, lPan);
								break;

							case 58://Mountain Giant
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 88, sDist, lPan);
								break;

							case 59://Ettin
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 92, sDist, lPan);
								break;

							case 60://Cannibal Plant
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 96, sDist, lPan);
								break;

							case 61://Rudolph
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 38, sDist, lPan);
								break;

							case 62://DireBoar
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 68, sDist, lPan);
								break;

							case 63://Frost
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('C', 4, sDist, lPan);
								break;

							case 77: // Snoopy: Master MageOrc
							case 88: // Snoopy: Barbarian
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 78, sDist, lPan);
								break;

							case 74: // Snoopy: GiantCrayFish
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 100, sDist, lPan);
								break;

							case 73: // Snoopy: Fire Wyvern
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 107, sDist, lPan);
								break;

							case 80: // Snoopy: Tentocle
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 111, sDist, lPan);
								break;

							case 81: // Snoopy: Abaddon
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 137, sDist, lPan);
								break;

							case 72: // Snoopy: Claw-Turtle
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 115, sDist, lPan);
								break;

							case 71: // Snoopy: Centaurus
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 119, sDist, lPan);
								break;

							case 76: // Snoopy: Giant-Tree
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 123, sDist, lPan);
								break;

							case 75: // Snoopy: GiantLizard
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 127, sDist, lPan);
								break;

							case 70: // Snoopy: Dragon
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 131, sDist, lPan);
								break;

							case 79: //Snoopy:  Nizie
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 135, sDist, lPan);
								break;

							case 78: // Snoopy: Minaus
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 104, sDist, lPan);
								break;

							case 86: // Snoopy: Heavy BattleTank
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 151, sDist, lPan);
								break;

							case 87: // Snoopy: Crosbow Turret
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 153, sDist, lPan);
								break;

							case 89: // Snoopy: Cannon Turret
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
									m_pGame->PlaySound('M', 155, sDist, lPan);
								break;

							case 34: // Dummy
							case 35: // Snoopy: EnergySphere
							default:
								if (m_pData[dX][dY].m_cOwnerFrame == 2) {
									m_pGame->PlaySound('C', 2, sDist, lPan);
								}
								break;
							}
					}
					else if ( m_pData[dX][dY].m_cOwnerAction == OBJECTDAMAGE ) {
						switch (m_pData[dX][dY].m_sOwnerType) {
						case 1:
						case 2:
						case 3:  // Men
						case 44: // GHK
						case 45: // GHKABS
						case 46: // TK
							if (m_pData[dX][dY].m_cOwnerFrame == 4)
							{	if (m_pData[dX][dY].m_sV2 == -1)
							iSoundIndex = 5;
							else if (m_pData[dX][dY].m_sV2 == 0)
								iSoundIndex = 5;
							else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
								iSoundIndex = 6;
							else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
								iSoundIndex = 6;
							else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
								iSoundIndex = 7;
							else iSoundIndex = 5;
							m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
							m_pGame->bAddNewEffect(1, m_sPivotX + dX, m_sPivotY + dY, 0, 0, 0, 4);
							}
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('C', 12, sDist, lPan);
							break;
						case 4:
						case 5:
						case 6: // Women
							if (m_pData[dX][dY].m_cOwnerFrame == 4)
							{	if (m_pData[dX][dY].m_sV2 == -1)
							iSoundIndex = 5;
							else if (m_pData[dX][dY].m_sV2 == 0)
								iSoundIndex = 5;
							else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
								iSoundIndex = 6;
							else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
								iSoundIndex = 6;
							else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
								iSoundIndex = 7;
							else iSoundIndex = 5;
							m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
							m_pGame->bAddNewEffect(1, m_sPivotX + dX, m_sPivotY + dY, 0, 0, 0, 4);
							}
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('C', 13, sDist, lPan);
							break;

						default:
							if (m_pData[dX][dY].m_cOwnerFrame == 4)
							{	
								if (m_pData[dX][dY].m_sV2 == -1)
									iSoundIndex = 5;  // Hand Attack
								else if (m_pData[dX][dY].m_sV2 == 0)
									iSoundIndex = 5;  // Hand Attack
								else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
									iSoundIndex = 6;  // Blade hit
								else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
									iSoundIndex = 6;  // Blade hit
								else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
									iSoundIndex = 7; // Arrow hit
								else iSoundIndex = 5;

								m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
								if (iSoundIndex == 7){ // Change the effect for Arrows hitting (no more at fixed heigh with arrow flying but on damage)
									m_pGame->bAddNewEffect(14, (m_sPivotX + dX)*32, (m_sPivotY + dY)*32, 0, 0, 0, m_pData[dX][dY].m_sOwnerType);
								}else	{	
									m_pGame->bAddNewEffect(1, m_sPivotX + dX, m_sPivotY + dY, 0, 0, 0, 4);
								}	
							}

							switch (m_pData[dX][dY].m_sOwnerType) {
							case 88: // Snoopy: Barbarian
								if (m_pData[dX][dY].m_cOwnerFrame == 1) m_pGame->PlaySound('M', 144, sDist, lPan);
								break;

							case 83: // Snoopy: ATK
								if (m_pData[dX][dY].m_cOwnerFrame == 1) m_pGame->PlaySound('M', 143, sDist, lPan);
								break;

							case 84: // Snoopy: MasterElf
								if (m_pData[dX][dY].m_cOwnerFrame == 1) m_pGame->PlaySound('C', 7, sDist, lPan);
								break;

							case 85: // Snoopy: DSK
								if (m_pData[dX][dY].m_cOwnerFrame == 1) m_pGame->PlaySound('M', 148, sDist, lPan);
								break;

							case 54: // DE
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('C', 13, sDist, lPan);
								break;

							case 10: // Slime
							case 53: // BB
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 3, sDist, lPan);
								break;

							case 11: // Skell
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 15, sDist, lPan);
								break;

							case 12: // Stone-Golem
							case 65: // IceGolem
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 35, sDist, lPan);
								break;

							case 123: // Enraged Cyclops
							case 13: // Cyclops
							case 49: // HC
							case 126: // Enraged Hellclaw
							case 52: // GG
							case 125: // Enraged Gargoyle
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 43, sDist, lPan);
								break;

							case 14: // Orc
							case 48: // SK
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 11, sDist, lPan);
								break;

							case 16: // Ant
							case 43: // LWB
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 31, sDist, lPan);
								break;

							case 17: // Scorp
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 23, sDist, lPan);
								break;

							case 18: // Zombie
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 19, sDist, lPan);
								break;

							case 22: // Snake
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 27, sDist, lPan);
								break;

							case 23: // Clay-Golem
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 39, sDist, lPan);
								break;

							case 27: // HH
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 7, sDist, lPan);
								break;

							//case 122: // Enraged Troll
							case 28: // Troll
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 48, sDist, lPan);
								break;

							case 29: // Ogre
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 53, sDist, lPan);
								break;

							case 30: // Liche
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 57, sDist, lPan);
								break;

							case 31: // DD
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 61, sDist, lPan);
								break;

							case 32: // Uni
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 65, sDist, lPan);
								break;

							case 33: // WW
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 69, sDist, lPan);
								break;

							case 34: // dummy
							case 35: // Snoopy: EnergyBall
								if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 2, sDist, lPan);
								break;

							case 55://Rabbit
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 79, sDist, lPan);
								break;

							case 56://Cat
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 80, sDist, lPan);
								break;

							case 57://Giant-Frog
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 81, sDist, lPan);
								break;

							case 58: // Mountain Giant
							case 77: // Snoopy: MasterMageOrc
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 89, sDist, lPan);
								break;

							case 59://Ettin
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 93, sDist, lPan);
								break;
							case 60://Cannabl Plant
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 97, sDist, lPan);
								break;
							case 61://Rudolph
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 69, sDist, lPan);
								break;
							case 62://DireBoar
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 78, sDist, lPan);
								break;
							case 63://Frost
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('C', 13, sDist, lPan);
								break;

							case 74: // Snoopy: Giant CrayFish
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 101, sDist, lPan);
								break;

							case 78: // Snoopy: Minaus
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 102, sDist, lPan);
								break;

							case 80: // Snoopy: Tentocle
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 108, sDist, lPan);
								break;

							case 81: // Snoopy: Abaddon
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 138, sDist, lPan);
								break;

							case 72: // Snoopy: ClawTurtle
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 112, sDist, lPan);
								break;

							case 71: // Snoopy: Centaurus
							case 82: // Snoopy: Sorceress
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 116, sDist, lPan);
								break;

							case 76: // Snoopy: GiantTree
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 120, sDist, lPan);
								break;

							case 75: // Snoopy: GiantLizard
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 124, sDist, lPan);
								break;

							case 70: // Snoopy: Dragon
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 128, sDist, lPan);
								break;

							case 79: // Snoopy: Nizie
								if ( (m_pData[dX][dY].m_cOwnerFrame == 1) ) m_pGame->PlaySound('M', 132, sDist, lPan);
								break;
							}
							break;
							}	
					}
					else if ( m_pData[dX][dY].m_cOwnerAction == OBJECTDAMAGEMOVE ) {
						switch (m_pData[dX][dY].m_sOwnerType) {
						case 1:
						case 2:
						case 3:
							if (m_pData[dX][dY].m_cOwnerFrame == 1)
							{	if (m_pData[dX][dY].m_sV2 == -1)
							iSoundIndex = 5;
							else if (m_pData[dX][dY].m_sV2 == 0)
								iSoundIndex = 5;
							else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
								iSoundIndex = 6;
							else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
								iSoundIndex = 6;
							else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
								iSoundIndex = 7;
							else iSoundIndex = 5;

							m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
							m_pGame->bAddNewEffect(1, m_sPivotX + dX, m_sPivotY + dY, 0, 0, 0, 4);
							}
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('C', 12, sDist, lPan);
							break;

						case 4:
						case 5:
						case 6:
							if (m_pData[dX][dY].m_cOwnerFrame == 1)
							{	if (m_pData[dX][dY].m_sV2 == -1)
							iSoundIndex = 5;
							else if (m_pData[dX][dY].m_sV2 == 0)
								iSoundIndex = 5;
							else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
								iSoundIndex = 6;
							else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
								iSoundIndex = 6;
							else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
								iSoundIndex = 7;
							else iSoundIndex = 5;
							m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
							m_pGame->bAddNewEffect(1, m_sPivotX + dX, m_sPivotY + dY, 0, 0, 0, 4);
							}
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('C', 13, sDist, lPan);
							break;

						default:
							if (m_pData[dX][dY].m_cOwnerFrame == 1)
							{	if (m_pData[dX][dY].m_sV2 == -1)
							iSoundIndex = 5;
							else if (m_pData[dX][dY].m_sV2 == 0)
								iSoundIndex = 5;
							else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
								iSoundIndex = 6;
							else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
								iSoundIndex = 6;
							else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
								iSoundIndex = 7;
							else iSoundIndex = 5;
							m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
							m_pGame->bAddNewEffect(1, m_sPivotX + dX, m_sPivotY + dY, 0, 0, 0, 4);
							}

						switch (m_pData[dX][dY].m_sOwnerType) {
						case 83: //Snoopy:  ATK
							if (m_pData[dX][dY].m_cOwnerFrame == 1)
								m_pGame->PlaySound('M', 143, sDist, lPan);
							break;
						case 84: // Snoopy: MasterElf
							if (m_pData[dX][dY].m_cOwnerFrame == 1)
								m_pGame->PlaySound('C', 7, sDist, lPan);
							break;
						case 88: // Snoopy: Barbarian
							if (m_pData[dX][dY].m_cOwnerFrame == 1)
								m_pGame->PlaySound('M', 144, sDist, lPan);
							break;
						case 85: // Snoopy: DSK
							if (m_pData[dX][dY].m_cOwnerFrame == 1)
								m_pGame->PlaySound('M', 148, sDist, lPan);
							break;

						case 10: // Slime
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 3, sDist, lPan);
							break;

						case 11: // Skell
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 15, sDist, lPan);
							break;

						case 12: // Stone Golem
						case 65: // IceGolem
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 35, sDist, lPan);
							break;

						case 123: // Enraged Cyclops
						case 13: // Cyclops
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 43, sDist, lPan);
							break;

						case 14: // Orc
						case 48: // SK
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 11, sDist, lPan);
							break;

						case 16: // Ant
						case 43: // LWB
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 31, sDist, lPan);
							break;

						case 17: // Scorpion
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 23, sDist, lPan);
							break;

						case 18: // Zombie
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 19, sDist, lPan);
							break;

						case 22: // Snake
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 27, sDist, lPan);
							break;

						case 23: // Clay-Golem
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 39, sDist, lPan);
							break;

						case 27: // HH
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 7, sDist, lPan);
							break;

						//case 122: // Enraged Troll
						case 28: // Troll
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 48, sDist, lPan);
							break;

						case 29: // Ogre
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 53, sDist, lPan);
							break;

						case 30: // Liche
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 57, sDist, lPan);
							break;

						case 31: // DD
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 61, sDist, lPan);
							break;

						case 32: // Uni
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 65, sDist, lPan);
							break;

						case 33: // WW
							if (m_pData[dX][dY].m_cOwnerFrame == 2)
								m_pGame->PlaySound('M', 69, sDist, lPan);
							break;
						case 55://Rabbit
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 79, sDist, lPan);
							break;

						case 56://Cat
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 80, sDist, lPan);
							break;

						case 57://Giant-Frog
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 81, sDist, lPan);
							break;

						case 58://Mountain Giant
						case 77: // Snoopy: MasterMageOrc
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 89, sDist, lPan);
							break;

						case 59://Ettin
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 93, sDist, lPan);
							break;

						case 60://Cannibal Plant
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 97, sDist, lPan);
							break;

						case 61://Rudolph
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 69, sDist, lPan);
							break;
						case 62://DireBoar
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 78, sDist, lPan);
							break;

						case 74: //Snoopy:  GiantCrayFish
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 101, sDist, lPan);
							break;

						case 78: // Snoopy: Minos
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 101, sDist, lPan);
							break;

						case 80: // Snoopy: Tentocle
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 108, sDist, lPan);
							break;

						case 81: // Snoopy: Abaddon
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 138, sDist, lPan);
							break;

						case 72: // Snoopy: ClawTurtle
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 112, sDist, lPan);
							break;

						case 71: // Snoopy: Centaurus
						case 82: // Snoopy: Sorceress
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 116, sDist, lPan);
							break;

						case 76: // Snoopy: GiantTree
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 120, sDist, lPan);
							break;

						case 75: // Snoopy: GiantLizard
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 124, sDist, lPan);
							break;

						case 70: // Snoopy: Dragon
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 128, sDist, lPan);
							break;

						case 79: // Snoopy: Nizie
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 132, sDist, lPan);
							break;

						default:
							break;
						}
					break;
					}	
				}
				else if ( m_pData[dX][dY].m_cOwnerAction == OBJECTMAGIC ) {
					switch (m_pData[dX][dY].m_sOwnerType) {
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
						if (m_pData[dX][dY].m_cOwnerFrame == 1) {
							m_pGame->PlaySound('C', 16, sDist, lPan);	
							if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0)) {
								m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +(rand()%20-10), (m_sPivotY+dY)*32 -(rand()%50) -5, 0, 0, -(rand()%8), 0);
								m_pGame->bAddNewEffect(54, (m_sPivotX+dX)*32 +(rand()%20-10), (m_sPivotY+dY)*32 -(rand()%50) -5, 0, 0, -(rand()%8), 0);
							}
							//if (m_pGame->bHasHeroSet(m_pData[dX][dY].m_sAppr3, m_pData[dX][dY].m_sAppr4, m_pData[dX][dY].m_sOwnerType) == 2)
							if (m_pGame->bHasHeroSet(m_pData[dX][dY].m_sHeadApprValue, m_pData[dX][dY].m_sBodyApprValue, m_pData[dX][dY].m_sArmApprValue, m_pData[dX][dY].m_sLegApprValue, m_pData[dX][dY].m_sOwnerType) == 2)
							{	
								m_pGame->bAddNewEffect(242, m_sPivotX + dX, m_sPivotY + dY, m_sPivotX + dX , m_sPivotY + dY, 0, 1);
							}
							if (m_pData[dX][dY].m_sV1 >= 70)
								m_pGame->bAddNewEffect(57, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, 0, 0, 0, 0);
						}
						break;
					}
				}
				else if ( m_pData[dX][dY].m_cOwnerAction == OBJECTDYING ) {
					switch (m_pData[dX][dY].m_sOwnerType) {
					case 1:
					case 2:
					case 3:
					case 44: // GHK
					case 45: // GHKABS
					case 46: // TK
						if (m_pData[dX][dY].m_cOwnerFrame == 6)
						{	if (m_pData[dX][dY].m_sV2 == -1)
						iSoundIndex = 5;
						else if (m_pData[dX][dY].m_sV2 == 0)
							iSoundIndex = 5;
						else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
							iSoundIndex = 6;
						else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
							iSoundIndex = 6;
						else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
							iSoundIndex = 7;
						else iSoundIndex = 5;
						m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
						m_pGame->bAddNewEffect(1, m_sPivotX + dX, m_sPivotY + dY, 0, 0, 0, 12);
						}
						if (m_pData[dX][dY].m_cOwnerFrame == 7)
							m_pGame->PlaySound('C', 14, sDist, lPan);
						break;

					case 4:
					case 5:
					case 6:
					case 54: // DE
						if (m_pData[dX][dY].m_cOwnerFrame == 6)
						{	if (m_pData[dX][dY].m_sV2 == -1)
						iSoundIndex = 5;
						else if (m_pData[dX][dY].m_sV2 == 0)
							iSoundIndex = 5;
						else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
							iSoundIndex = 6;
						else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
							iSoundIndex = 6;
						else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
							iSoundIndex = 7;
						else iSoundIndex = 5;
						m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
						m_pGame->bAddNewEffect(1, m_sPivotX + dX, m_sPivotY + dY, 0, 0, 0, 12);
						}
						if (m_pData[dX][dY].m_cOwnerFrame == 7)
							m_pGame->PlaySound('C', 15, sDist, lPan);
						break;

					default:
						if (m_pData[dX][dY].m_cOwnerFrame == 4)
						{	if (m_pData[dX][dY].m_sV2 == -1)
						iSoundIndex = 5;
						else if (m_pData[dX][dY].m_sV2 == 0)
							iSoundIndex = 5;
						else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
							iSoundIndex = 6;
						else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
							iSoundIndex = 6;
						else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
							iSoundIndex = 7;
						else iSoundIndex = 5;
						m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
						m_pGame->bAddNewEffect(1, m_sPivotX + dX, m_sPivotY + dY, 0, 0, 0, 12);
						}

						switch (m_pData[dX][dY].m_sOwnerType) {
						case 53: // BB
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 39, sDist, lPan);
							break;

						case 10: // Slime
						case 34: // Dummy
						case 35: // Snoopy: EnergyBall
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 4, sDist, lPan);
							break;

						case 11: // Skell
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 16, sDist, lPan);
							break;

						case 12: // Stone-Golem
						case 47: // BG
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 36, sDist, lPan);
							break;

						case 65: // IceGolem
//							if (m_pData[dX][dY].m_cOwnerFrame == 1)
//								m_pGame->bAddNewEffect(74, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, 0, 0, 0 );
							if (m_pData[dX][dY].m_cOwnerFrame == 5) {
								m_pGame->bAddNewEffect(74, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, 0, 0, 0 );
								m_pGame->PlaySound('M', 36, sDist, lPan);
							}
							break;

						case 123: // Enraged Cyclops
						case 13: // Cyclops
						case 49: // HC
						case 126: // Enraged Hellclaw
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 44, sDist, lPan);
							break;

						case 14: // Orc
						case 48: // SK
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 12, sDist, lPan);
							break;

						case 16: // Ant
						case 43: // LWB
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 32, sDist, lPan);
							break;

						case 17: // Scorp
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 24, sDist, lPan);
							break;

						case 18: // Zombie
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 20, sDist, lPan);
							break;

						case 22: // Snake
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 28, sDist, lPan);
							break;

						case 23: // Clay-Golem
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 40, sDist, lPan);
							break;

						case 27: // HH
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 8, sDist, lPan);
							break;

						//case 122: // Enraged Troll
						case 28: // Troll
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 49, sDist, lPan);
							break;

						case 29: // Ogre
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 54, sDist, lPan);
							break;

						case 30: // Liche
						case 50: // TW
						case 127: // Enraged Tigerworm
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 58, sDist, lPan);
							break;

						case 31: // DD
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 62, sDist, lPan);
							break;

						case 32: // Uni
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 66, sDist, lPan);
							break;

						case 33: // WW
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('M', 70, sDist, lPan);
							break;

						case 36: // AGT
						case 37: // CGT
						case 38: // MS
						case 39: // DT
						case 40: // ESG
						case 41: // GMG
						case 42: // ManaStone
							if (m_pData[dX][dY].m_cOwnerFrame == 3)
							{	m_pGame->bAddNewEffect(66, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, 0, 0, 0, 0);
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60), 0, 0, -1*(rand() % 2));
							}
							break;

						case 87: // Snoopy: CrossBowTurret
							if (m_pData[dX][dY].m_cOwnerFrame == 3)
							{	m_pGame->bAddNewEffect(66, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, 0, 0, 0, 0);
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60), 0, 0, -1*(rand() % 2));
							}
							if (m_pData[dX][dY].m_cOwnerFrame == 1)
								m_pGame->PlaySound('M', 154, sDist, lPan);
							break;

						case 89: // Snoopy: CannonTurret
							if (m_pData[dX][dY].m_cOwnerFrame == 3)
							{	m_pGame->bAddNewEffect(66, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, 0, 0, 0, 0);
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60), 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60), 0, 0, -1*(rand() % 2));
							}
							if (m_pData[dX][dY].m_cOwnerFrame == 1)
								m_pGame->PlaySound('M', 156, sDist, lPan);
							break;

						case 51: // CP
							if (m_pData[dX][dY].m_cOwnerFrame == 1)
							{	m_pGame->bAddNewEffect(66, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32 -30, 0, 0, 0, 0);
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60) -30, 0, 0, -1*(rand() % 2));
							}
							break;

						case 52: // GG
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
							{	m_pGame->PlaySound('M', 44, sDist, lPan);
							}
							if (m_pData[dX][dY].m_cOwnerFrame == 11)
							{	m_pGame->bAddNewEffect(66, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32 -30, 0, 0, 0, 0);
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(12, (m_sPivotX+dX)*32 + 5 - (rand() % 10), (m_sPivotY+dY)*32 + 5 - (rand() % 10) -30, 0, 0, -1*(rand() % 2));

							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60) -30, 0, 0, -1*(rand() % 2));
							m_pGame->bAddNewEffect(65, (m_sPivotX+dX)*32 + 30 - (rand() % 60), (m_sPivotY+dY)*32 + 30 - (rand() % 60) -30, 0, 0, -1*(rand() % 2));
							}
							break;

						case 55:// Rabbit
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 83, sDist, lPan);
							break;

						case 56: // Cat
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 84, sDist, lPan);
							break;

						case 57://Giant-Frog
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 85, sDist, lPan);
							break;

						case 58://Mountain Giant
						case 77: // Snoopy: MasterMageOrc
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 90, sDist, lPan);
							break;

						case 59://Ettin
						case 88: // Snoopy: Barbarian
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 94, sDist, lPan);
							break;

						case 83: // Snoopy: ATK
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 141, sDist, lPan);
							break;

						case 85: // Snoopy: DSK
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 146, sDist, lPan);
							break;

						case 61://Rudolph
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 65, sDist, lPan);
							break;

						case 62://DireBoar
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 94, sDist, lPan);
							break;

						case 66: // Wyvern
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('E', 7, sDist, lPan);
							break;

						case 70: // Snoopy: Dragon
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 129, sDist, lPan);
							break;

						case 71: // Snoopy: Centaur
						case 82: // Snoopy: Sorceress
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 129, sDist, lPan);
							break;

						case 72: // Snoopy: ClawTurtle
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 113, sDist, lPan);
							break;

						case 73: // Snoopy: FireWyvern
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 105, sDist, lPan);
							break;


						case 60: // Cannibal Plant
						case 74: // Snoopy: GiantGrayFish
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 98, sDist, lPan);
							break;

						case 75: //Snoopy:
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 125, sDist, lPan);
							break;

						case 76: // Snoopy:
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 121, sDist, lPan);
							break;

						case 78: // Snoopy:
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 103, sDist, lPan);
							break;

						case 79: // Snoopy:
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 133, sDist, lPan);
							break;

						case 80: //Snoopy: Tentocle
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 109, sDist, lPan);
							break;

						case 81: // Snoopy: Abaddon
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 139, sDist, lPan);
							break;

						case 84: // Snoopy: MasterElf
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 150, sDist, lPan);
							break;

						case 86: // Snoopy: HBT
							if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
								m_pGame->PlaySound('M', 152, sDist, lPan);
							break;

						default:
							if (m_pData[dX][dY].m_cOwnerFrame == 5)
								m_pGame->PlaySound('C', 15, sDist, lPan);
							break;

						case 63: // Frost
						case 91: // Snoopy: Gate
							break;
						}
						break;
					}	
				}	
			}	
		}	
	}

	if (bAutoUpdate == true)
	{	
		S_dwUpdateTime = dwTime;
		if (iRet == 0)
			return -1;
	}
	if (dynObjsNeedUpdate) m_dwDOframeTime = dwTime; //v1.4
	return iRet;
}


bool CMapData::bSetItem(short sX, short sY, short sItemSpr, short sItemSprFrame, uint32_t cItemColor, /*DWORD dwItemAttribute,*/ bool bDropEffect) // 1234 Added dwItemAttribute
{int dX, dY;
 int sAbsX, sAbsY, sDist;
	if ((sX < m_sPivotX) || (sX >= m_sPivotX + MAPDATASIZEX) ||
		(sY < m_sPivotY) || (sY >= m_sPivotY + MAPDATASIZEY))
	{	return false;
	}

	dX = sX - m_sPivotX;
	dY = sY - m_sPivotY;

	m_pData[dX][dY].m_sItemSprite      = sItemSpr;
	m_pData[dX][dY].m_sItemSpriteFrame = sItemSprFrame;
	m_pData[dX][dY].m_cItemColor       = cItemColor;
	//m_pData[dX][dY].m_dwItemAttribute = dwItemAttribute; // 1234 Added This Line

	/*sAbsX = abs(((m_pGame->m_sViewPointX / 32) + 10) - sX);
	sAbsY = abs(((m_pGame->m_sViewPointY / 32) + 7)  - sY);*/
	sAbsX = abs(((m_pGame->m_sViewPointX / 32) + /*10*/ 12) - sX); //BESK RESOLUTION
	sAbsY = abs(((m_pGame->m_sViewPointY / 32) + /*7*/ 9)  - sY);//BESK RESOLUTION

	if (sAbsX > sAbsY) sDist = sAbsX;
	else sDist = sAbsY;

	if (sItemSpr != 0)
	{	if (bDropEffect == true) 
		{	m_pGame->PlaySound('E', 11, sDist);
			m_pGame->bAddNewEffect(14, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, 0, 0, 0, 0);
			m_pGame->bAddNewEffect(14, (m_sPivotX+dX)*32 +(10-(rand()%20)), (m_sPivotY+dY)*32 +(10-(rand()%20)), 0, 0, (rand() % 2), 0);
			m_pGame->bAddNewEffect(14, (m_sPivotX+dX)*32 +(10-(rand()%20)), (m_sPivotY+dY)*32 +(10-(rand()%20)), 0, 0, (rand() % 2), 0);
	}	}

	return true;
}

bool __fastcall CMapData::bSetDeadOwner(uint16_t wObjectID, short sX, short sY, short sType, char cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, short sHeadApprValue, short sBodyApprValue, short sArmApprValue, short sLegApprValue, int iStatus, char * pName)
{
 int  dX, dY;
 char pTmpName[12];
 bool bEraseFlag = false;

	ZeroMemory(pTmpName, sizeof(pTmpName));
	if (pName != 0) strcpy(pTmpName, pName);
	if ((sX < m_sPivotX) || (sX >= m_sPivotX + MAPDATASIZEX) ||
		(sY < m_sPivotY) || (sY >= m_sPivotY + MAPDATASIZEY))
	{	for (dX = 0; dX < MAPDATASIZEX; dX++)
		for (dY = 0; dY < MAPDATASIZEY; dY++)
		{	if (memcmp(m_pData[dX][dY].m_cDeadOwnerName, pTmpName, 10) == 0)
			{	m_pData[dX][dY].m_sDeadOwnerType = 0;
				ZeroMemory(m_pData[dX][dY].m_cDeadOwnerName, sizeof(m_pData[dX][dY].m_cDeadOwnerName));
		}	}
		return false;
	}

	for (dX = sX - 2; dX <= sX + 2; dX++)
	for (dY = sY - 2; dY <= sY + 2; dY++)
	{	if (dX < m_sPivotX) break;
		else
		if (dX > m_sPivotX + MAPDATASIZEX) break;
		if (dY < m_sPivotY) break;
		else
		if (dY > m_sPivotY + MAPDATASIZEY) break;

		if (memcmp(m_pData[dX - m_sPivotX][dY - m_sPivotY].m_cDeadOwnerName, pTmpName, 10) == 0)
		{	m_pData[dX - m_sPivotX][dY - m_sPivotY].m_sDeadOwnerType = 0;
			ZeroMemory(m_pData[dX - m_sPivotX][dY - m_sPivotY].m_cDeadOwnerName, sizeof(m_pData[dX - m_sPivotX][dY - m_sPivotY].m_cDeadOwnerName));
			bEraseFlag = true;
	}	}

	if (bEraseFlag != true) {
		for (dX = 0; dX < MAPDATASIZEX; dX++)
		for (dY = 0; dY < MAPDATASIZEY; dY++) {

			if (memcmp(m_pData[dX][dY].m_cDeadOwnerName, pTmpName, 10) == 0) {
				m_pData[dX][dY].m_sDeadOwnerType = 0;
				ZeroMemory(m_pData[dX][dY].m_cDeadOwnerName, sizeof(m_pData[dX][dY].m_cDeadOwnerName));
			}

		}
	}

	dX = sX - m_sPivotX;
	dY = sY - m_sPivotY;

	m_pData[dX][dY].m_wDeadObjectID   = wObjectID;
	m_pData[dX][dY].m_sDeadOwnerType  = sType;
	m_pData[dX][dY].m_cDeadDir        = cDir;
	m_pData[dX][dY].m_sDeadAppr1      = sAppr1;
	m_pData[dX][dY].m_sDeadAppr2      = sAppr2;
	m_pData[dX][dY].m_sDeadAppr3      = sAppr3;
	m_pData[dX][dY].m_sDeadAppr4      = sAppr4;
	m_pData[dX][dY].m_iDeadApprColor  = iApprColor;
	m_pData[dX][dY].m_sDeadHeadApprValue      = sHeadApprValue; // Re-Coding Sprite xRisenx
	m_pData[dX][dY].m_sDeadBodyApprValue      = sBodyApprValue; // Re-Coding Sprite xRisenx
	m_pData[dX][dY].m_sDeadArmApprValue      = sArmApprValue; // Re-Coding Sprite xRisenx
	m_pData[dX][dY].m_sDeadLegApprValue      = sLegApprValue; // Re-Coding Sprite xRisenx
	m_pData[dX][dY].m_iDeadStatus     = iStatus;
	m_pData[dX][dY].m_cDeadOwnerFrame = -1;
	strcpy(m_pData[dX][dY].m_cDeadOwnerName, pTmpName);

	m_iObjectIDcacheLocX[wObjectID] = -1*sX; //dX;
	m_iObjectIDcacheLocY[wObjectID] = -1*sY; //dY;

	return true;
}

bool __fastcall CMapData::bSetChatMsgOwner(uint16_t wObjectID, short sX, short sY, int iIndex)
{
 int dX, dY;

	if ((sX == -10) && (sY == -10)) goto SCMO_FULL_SEARCH;

	if ((sX < m_sPivotX) || (sX >= m_sPivotX + MAPDATASIZEX) ||
		(sY < m_sPivotY) || (sY >= m_sPivotY + MAPDATASIZEY))
	{	return false;
	}
	for (dX = sX - 4; dX <= sX + 4; dX++)
	for (dY = sY - 4; dY <= sY + 4; dY++)
	{	if (dX < m_sPivotX) break;
		else
		if (dX > m_sPivotX + MAPDATASIZEX) break;
		if (dY < m_sPivotY) break;
		else
		if (dY > m_sPivotY + MAPDATASIZEY) break;

		if (m_pData[dX - m_sPivotX][dY - m_sPivotY].m_wObjectID == wObjectID) {
			m_pData[dX - m_sPivotX][dY - m_sPivotY].m_iChatMsg = iIndex;
			return true;
		}
		if (m_pData[dX - m_sPivotX][dY - m_sPivotY].m_wDeadObjectID == wObjectID) {
			m_pData[dX - m_sPivotX][dY - m_sPivotY].m_iDeadChatMsg = iIndex;
			return true;
		}
	}

SCMO_FULL_SEARCH:;

	for (dX = 0; dX < MAPDATASIZEX; dX++)
	for (dY = 0; dY < MAPDATASIZEY; dY++) {

		if (m_pData[dX][dY].m_wObjectID == wObjectID) {
			m_pData[dX][dY].m_iChatMsg = iIndex;
			return true;
		}
		if (m_pData[dX][dY].m_wDeadObjectID == wObjectID) {
			m_pData[dX][dY].m_iDeadChatMsg = iIndex;
			return true;
		}
	}

	return false;
}

void CMapData::ClearChatMsg(short sX, short sY)
{
	// v1.411
	if (m_pGame->m_pChatMsgList[m_pData[sX - m_sPivotX][sY - m_sPivotY].m_iChatMsg] != 0) {
		delete m_pGame->m_pChatMsgList[m_pData[sX - m_sPivotX][sY - m_sPivotY].m_iChatMsg];
		m_pGame->m_pChatMsgList[m_pData[sX - m_sPivotX][sY - m_sPivotY].m_iChatMsg] = 0;
	}

	m_pData[sX - m_sPivotX][sY - m_sPivotY].m_iChatMsg = 0;
}

void CMapData::ClearDeadChatMsg(short sX, short sY)
{
	m_pData[sX - m_sPivotX][sY - m_sPivotY].m_iDeadChatMsg = 0;
}

bool __fastcall CMapData::bGetOwner(short sX, short sY, char * pName, short * pOwnerType, int * pOwnerStatus, uint16_t * pObjectID, short * dynObjectType)
{
 int dX, dY;

	if ((sX < m_sPivotX) || (sX > m_sPivotX + MAPDATASIZEX) ||
		(sY < m_sPivotY) || (sY > m_sPivotY + MAPDATASIZEY)) {
		ZeroMemory(pName, sizeof(pName));
		return false;
	}

	dX = sX - m_sPivotX;
	dY = sY - m_sPivotY;

	*pOwnerType = m_pData[dX][dY].m_sOwnerType;
	strcpy(pName, m_pData[dX][dY].m_cOwnerName);
	*pOwnerStatus = m_pData[dX][dY].m_iStatus;
	*pObjectID    = m_pData[dX][dY].m_wObjectID;
	if (dynObjectType) 
		*dynObjectType = m_pData[dX][dY].m_sDynamicObjectType;

	return true;
}

bool CMapData::bSetDynamicObject(short sX, short sY, uint16_t wID, short sType, bool bIsEvent)
{
 int dX, dY, sPrevType;

	if ((sX < m_sPivotX) || (sX >= m_sPivotX + MAPDATASIZEX) ||
		(sY < m_sPivotY) || (sY >= m_sPivotY + MAPDATASIZEY))
	{	return false;
	}

	dX = sX - m_sPivotX;
	dY = sY - m_sPivotY;

	sPrevType = m_pData[dX][dY].m_sDynamicObjectType;

	m_pData[dX][dY].m_sDynamicObjectType  = sType;
	m_pData[dX][dY].m_cDynamicObjectFrame = rand() % 5;
	m_pData[dX][dY].m_dwDynamicObjectTime = unixtime();

	m_pData[dX][dY].m_cDynamicObjectData1 = 0;
	m_pData[dX][dY].m_cDynamicObjectData2 = 0;
	m_pData[dX][dY].m_cDynamicObjectData3 = 0;
	m_pData[dX][dY].m_cDynamicObjectData4 = 0;

	switch (sType) {
	case 0:
		if (sPrevType == DYNAMICOBJECT_FIRE)
		{	m_pGame->bAddNewEffect(15, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, 0, 0, 0, 0);
			m_pGame->bAddNewEffect(15, (m_sPivotX+dX)*32 +(10-(rand()%20)), (m_sPivotY+dY)*32  +(20-(rand()%40)), 0, 0, 0, 0);
			m_pGame->bAddNewEffect(15, (m_sPivotX+dX)*32 +(10-(rand()%20)), (m_sPivotY+dY)*32  +(20-(rand()%40)), 0, 0, 0, 0);
			m_pGame->bAddNewEffect(15, (m_sPivotX+dX)*32 +(10-(rand()%20)), (m_sPivotY+dY)*32  +(20-(rand()%40)), 0, 0, 0, 0);
		}else if ((sPrevType == DYNAMICOBJECT_PCLOUD_BEGIN) || (sPrevType == DYNAMICOBJECT_PCLOUD_LOOP)) 
		{	m_pData[dX][dY].m_sDynamicObjectType  = DYNAMICOBJECT_PCLOUD_END;
			m_pData[dX][dY].m_cDynamicObjectFrame = 0;
			m_pData[dX][dY].m_dwDynamicObjectTime = unixtime();
		}
		break;

	case DYNAMICOBJECT_FISH:
		m_pData[dX][dY].m_cDynamicObjectData1 = (rand() % 40) - 20;
		m_pData[dX][dY].m_cDynamicObjectData2 = (rand() % 40) - 20;
		m_pData[dX][dY].m_cDynamicObjectData3 = (rand() % 10) - 5;
		m_pData[dX][dY].m_cDynamicObjectData4 = (rand() % 10) - 5;
		break;

	case DYNAMICOBJECT_PCLOUD_BEGIN:
		if (bIsEvent == false)
		{	m_pData[dX][dY].m_sDynamicObjectType = DYNAMICOBJECT_PCLOUD_LOOP;
			m_pData[dX][dY].m_cDynamicObjectFrame = rand() % 8;
		}else m_pData[dX][dY].m_cDynamicObjectFrame = -1*(rand() % 8);
		break;

	case DYNAMICOBJECT_ARESDENFLAG:
		m_pData[dX][dY].m_cDynamicObjectFrame = (rand() % 4);
		break;

	case DYNAMICOBJECT_ELVINEFLAG:
		m_pData[dX][dY].m_cDynamicObjectFrame = 4+ (rand() % 4);
		break;
	}
	return true;
}

void CMapData::GetOwnerStatusByObjectID(uint16_t wObjectID, char *pOwnerType, char *pDir, short *pAppr1, short *pAppr2, short *pAppr3, short *pAppr4, short *pHeadApprValue, short *pBodyApprValue, short *pArmApprValue, short *pLegApprValue, int * pStatus, int * pColor, char * pName) // Re-Coding Sprite xRisenx
{ int iX, iY;
	for (iX = 0; iX < MAPDATASIZEX; iX++)
	for (iY = 0; iY < MAPDATASIZEY; iY++)
	if (m_pData[iX][iY].m_wObjectID == wObjectID)
	{	*pOwnerType = (char)m_pData[iX][iY].m_sOwnerType;
		*pDir       = m_pData[iX][iY].m_cDir;
		*pAppr1     = m_pData[iX][iY].m_sAppr1;
		*pAppr2     = m_pData[iX][iY].m_sAppr2;
		*pAppr3     = m_pData[iX][iY].m_sAppr3;
		*pAppr4     = m_pData[iX][iY].m_sAppr4;
		*pStatus    = m_pData[iX][iY].m_iStatus;
		*pColor     = m_pData[iX][iY].m_iApprColor;
		*pHeadApprValue     = m_pData[iX][iY].m_sHeadApprValue; // Re-Coding Sprite xRisenx
		*pBodyApprValue     = m_pData[iX][iY].m_sBodyApprValue; // Re-Coding Sprite xRisenx
		*pArmApprValue     = m_pData[iX][iY].m_sArmApprValue; // Re-Coding Sprite xRisenx
		*pLegApprValue     = m_pData[iX][iY].m_sLegApprValue; // Re-Coding Sprite xRisenx
		strcpy(pName, m_pData[iX][iY].m_cOwnerName);
		return;
	}
}
