#ifndef COMMON_H
#define COMMON_H
#pragma once

#include "newsocket.h"
#include <iostream>
#include <sstream>
#include <memory>
#include <time.h>		   
#include <sys/timeb.h>
#include "version.h"
#include "typedefs.h"
#include "lgnSvcs.h"
#include "maps.h"
#ifdef WIN32
#include <windows.h>
#endif

extern uint64_t unixtime();
extern uint32_t unixseconds();

#ifndef WIN32
#define ZeroMemory(a,b) memset(a, 0, b)
typedef struct tagRECT
{
	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;
} RECT, *LPRECT;

void SetRect(LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom)
{
	lprc->left = xLeft;
	lprc->top = yTop;
	lprc->right = xRight;
	lprc->bottom = yBottom;
}
#endif

using namespace std;

#define MAXITEMTYPES			2000 // 1500->2000 xRisenx
#define MAXNPCTYPES			200
#define MAXPARTYMEMBERS		25
#define MAXSIDES				4
#define MAXBANKITEMS			120 // 120->300 xRisenx
#define MAXITEMS		75			// 50->75 xRisenx
#define MAXGUILDSMAN	128 
#define MAXMAGICTYPE	100
#define MAXSKILLTYPE	25
#define MAXTELEPORTLIST	20

enum Side
{
	NEUTRAL,
	ARESDEN,
	ELVINE,
	ISTRIA,
	HOSTILE
};

static const char * sideName[MAXSIDES] = { "Traveller", "Aresden", "Elvine"/*, "Istria" */};
static const char * sideMap[MAXSIDES] = { "default", "aresden", "elvine", /*"istria"*/};
static const char * sideMapJail[MAXSIDES] = { "default", "arejail", "elvjail", /*"istjail"*/};
static const char * sideMapFarm[MAXSIDES] = { "default", "arefarm", "elvfarm", /*"istfarm"*/};
static const char * sideMapRes[MAXSIDES] = { "default", "resurr1", "resurr2", /*"resurr3"*/};

#define ITEMS_PER_TILE	12
#define MAXGROUNDITEMS	5000

#define TILECLEANTIME		MINUTE(15)
#define TILECLEANTIMEPLAYER	MINUTE(30)

#define SPECABLTYTIMESEC	1200


#define SEND_MAIL_COST			300
#define SEND_MAIL_ITEM_COST	700
#define GUILDBOARD_POST_COST	500
#define MAX_MAIL_MSG_LENGTH	400+1
#define MAX_MAIL_ITEMS			10

#define MAXITEMSOCKETS		3

// ----------------------------------Guild-------------------------------------------------------------------------------------
#define MAXGUILDBANKITEMS		75*4
#define GUILDSUMMONSGOLDCOST	5000
#define GUILDSUMMONSTIME		MINUTE(5)

enum GuildUpgrades{
	GU_WAREHOUSE,
	GU_SUMMONS,
	GU_CAPTAINCY,
	GU_RAIDMASTERS,
	GU_HUNTMASTERS,
	GU_MAX
};

const struct GuildUpgrade{
	string name;
	GuildUpgrades type;
	uint8_t maxLvl;
	uint32_t costGold[5];
	uint32_t costMaj[5];
	uint32_t costCont[5];
} gldUps[GU_MAX] = {
	{
		"Warehouse", GU_WAREHOUSE, 4,
		{0, 200000, 500000, 1000000, 5000000},	// gold
		{0, 1000, 2000, 10000, 50000},							// maj
		{0, 1000, 3000, 8000, 20000}					// cont
	},{
		"Summons", GU_SUMMONS, 4,
		{0, 200000, 500000, 1000000, 5000000},
		{0, 1000, 2000, 10000, 50000},
		{0, 1000, 3000, 8000, 20000}
	},{
		"Captaincy", GU_CAPTAINCY, 4,
		{0, 100000, 400000, 1000000, 5000000},
		{0, 500, 1000, 8000, 20000},
		{0, 2000, 5000, 10000, 50000}
	},{
		"Raidmasters", GU_RAIDMASTERS, 4,
		{0, 500000, 1000000, 3000000, 6000000},
		{0, 1500, 3000, 12000, 24000},
		{0, 4000, 8000, 24000, 48000}
	},{
		"Huntmasters", GU_HUNTMASTERS, 4,
		{0, 500000, 1000000, 3000000, 6000000},
		{0, 1500, 3000, 12000, 24000},
		{0, 4000, 8000, 24000, 48000}
	}
};

const uint32_t maxGWHItems[5] = { 0, 75, 75*2, 75*3, 75*4 };


enum GuildRank
{
	GUILDRANK_MASTER,
	GUILDRANK_CAPTAIN,
	GUILDRANK_HUNTMASTER,
	GUILDRANK_RAIDMASTER,
	GUILDRANK_GUILDSMAN,
	GUILDRANK_RECRUIT,
	GUILDRANK_MAX,
	GUILDRANK_NONE = -1
};

const struct GuildRankPermissions
{
	bool canDisband;
	bool canInvite;
	bool canBan;
	bool canSummon;
	bool canWithdrawWH;
	bool crusadeCommander;
	bool canPromote;
	bool canPing;
} gldRankPerm[GUILDRANK_MAX] = {
	{true, true, true, true, true, true, true, true},		// GUILDRANK_MASTER
	{false, true, true, false, true, true, true, true},	// GUILDRANK_CAPTAIN
	{false, false, false, false, true, false, false, true},	// GUILDRANK_HUNTMASTER
	{false, false, false, false, true, false, false, true},	// GUILDRANK_RAIDMASTER
	{false, false, false, false, true, false, false, false},	// GUILDRANK_GUILDSMAN
	{false, false, false, false, false, false, false, false}	// GUILDRANK_RECRUIT
};

#define GUILDSTARTRANK		GUILDRANK_RECRUIT

//-----------------------------------------------------------------------------------------------------------------------

enum ChatType
{
	CHAT_NORMAL,
	CHAT_GUILD,
	CHAT_SHOUT,
	CHAT_NATIONSHOUT,
	CHAT_PARTY,
	CHAT_GM = 10,
	CHAT_WHISPER = 20,
	CHAT_MAX
};

//-----------------------------------------------------------------------------------------------------------------------

typedef uint32_t UnitStatus;

enum StatusFlags
{
	STATUS_INVISIBILITY =		1 << 4,	// 0x00000010
	STATUS_BERSERK =				1 << 5,	// 0x00000020
	STATUS_FROZEN =				1 << 6,	// 0x00000040
	STATUS_POISON =				1 << 7,	// 0x00000080

	STATUS_ANGELSTR =				1 << 12,	// 0x00001000
	STATUS_ANGELDEX =				1 << 13,	// 0x00002000
	STATUS_ANGELINT =				1 << 14,	// 0x00004000
	STATUS_ANGELMAG =				1 << 15,	// 0x00008000

	STATUS_RELICHOLDER = 		1 << 17,	// 0x00020000 - previously STATUS_HEROFLAG
	STATUS_AFK =					1 << 18,	// 0x00040000
	STATUS_GREENSLATE =			1 << 16,	// 0x00010000
	STATUS_REDSLATE =				1 << 22,	// 0x00400000
	STATUS_BLUESLATE =			1 << 23,	// 0x00800000
	STATUS_ILLUSIONMOVEMENT =	1 << 21,	// 0x00200000
	STATUS_ILLUSION =				1 << 24,	// 0x01000000
	STATUS_DEFENSESHIELD =		1 << 25,	// 0x02000000
	STATUS_PFM =					1 << 26,	// 0x04000000
	STATUS_PFA =					1 << 27,	// 0x08000000
	STATUS_PK =						1 << 20,	// 0x00100000
	STATUS_SIDE =					0xF0000000,
	STATUS_ALL =					0xFFFFFFFF,
	STATUS_ENEMYFLAGS =			STATUS_ALL - ( STATUS_PFA |
												STATUS_PFM |
												STATUS_DEFENSESHIELD |
												STATUS_ILLUSION |
												STATUS_POISON )
};

//-----------------------------------------------------------------------------------------------------------------------

enum GMFlags
{
	GMFLAG_INVINCIBLE =		1,
	GMFLAG_NOAGGRO =				1 << 1,
	GMFLAG_ETHEREAL =				1 << 2
};

enum Weather
{
	WEATHER_SUNNY,
	WEATHER_LIGHTRAIN,
	WEATHER_MEDIUMRAIN,
	WEATHER_HEAVYRAIN,
	WEATHER_LIGHTSNOW,
	WEATHER_MEDIUMSNOW,
	WEATHER_HEAVYSNOW
};

enum WeatherElement
{
	ELEMENT_NONE =			0,
	ELEMENT_EARTH =		1,
	ELEMENT_AIR =			2,
	ELEMENT_FIRE =			3,
	ELEMENT_WATER =		4,
	ELEMENT_MAX
};
//----------------------------------Events-------------------------------------------------------------------------------------

#define RELICVICTORYTIME		(10 * 1000)

enum EventType{
	ET_NONE,
	ET_CAPTURE,
	ET_DESTROY_SHIELD,
	ET_CRUSADE,
	ET_APOCALYPSE,
	ET_MONSTEREVENT, // Monster Kill Event xRisenx
	ET_MAX
};
static const char * eventName[] = { "", "Capture the Relic", "Destroy the Shield", "Crusade", "Apocalypse", "MonsterEvent"};

struct Casualties{
	uint32_t deaths;
	uint32_t kills;
};

//----------------------------------------------------------------------------------------------------------------------------------------

enum HeroReq{
	HR_CAPE,
	HR_HELM,
	HR_CAP,
	HR_PLATE,
	HR_ROBE,
	HR_HAUBERK,
	HR_LEGGINGS,
	HR_MAX
};

static const struct HeroItemCost{
	uint32_t EK;
	uint32_t contribution;
}heroPrice[HR_MAX] =
{
	{300,0},		// HR_CAPE
	{150,20},	// HR_HELM
	{100,20},	// HR_CAP
	{300,30},	// HR_PLATE
	{200,20},	// HR_ROBE
	{100,10},	// HR_HAUBERK
	{150,15}		// HR_LEGGINGS
};

static const uint32_t HeroItemID[HR_MAX][MAXSIDES-1][2] =
{
	{	// HR_CAPE
		{400, 400},		// ARESDEN
		{401, 401},		// ELVINE
		{1009, 1009}	// ISTRIA
	},
	{	// HR_HELM
		{403, 404},		// ARESDEN
		{405, 406},		// ELVINE
		{1010, 1011}	// ISTRIA
	},
	{	// HR_CAP
		{407, 408},		// ARESDEN
		{409, 410},		// ELVINE
		{1012, 1013}	// ISTRIA
	},
	{	// HR_PLATE
		{411, 412},		// ARESDEN
		{413, 414},		// ELVINE
		{1014, 1015}	// ISTRIA
	},
	{	// HR_ROBE
		{415, 416},		// ARESDEN
		{417, 418},		// ELVINE
		{1016, 1017}	// ISTRIA
	},
	{	// HR_HAUBERK
		{419, 420},		// ARESDEN
		{421, 422},		// ELVINE
		{1018, 1019}	// ISTRIA
	},
	{	// HR_LEGGINGS
		{423, 424},		// ARESDEN
		{425, 426},		// ELVINE
		{1020, 1021}	// ISTRIA
	}
};

//----------------------------------------------------------------------------------------------------------------------------------------

enum Directions{
	CENTER,
	NORTH,
	NORTHEAST,
	EAST,
	SOUTHEAST,
	SOUTH,
	SOUTHWEST,
	WEST,
	NORTHWEST
};

//----------------------------------------------------------------------------------------------------------------------------------------

#define atoul(str)			strtoul(str, NULL, 10)
#define atoull(str)			_strtoui64(str, NULL, 10)
#define strcpyn(dst, src)	strncpy(dst, src, sizeof(dst))

#define logb(n, b)	(log(n)/log(b))

struct Point{
	int32_t x,y;
	
	bool operator == (Point p)
	{
		return (p.x == x && p.y == y) ? true : false;
	}

	bool operator != (Point p)
	{
		return !(p == *this);
	}
	
	Point operator ++ (int)
	{
		Point copy = *this;
		x++;
		y++;
		return copy;
	}

	Point operator -- (int)
	{
		Point copy = *this;
		x--;
		y--;
		return copy;
	}
	
	Point & operator += (const Point & p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}

	Point & operator -= (const Point & p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}
	
	Point operator + (const Point & p) const
	{
		Point copy = *this;
		return (copy += p);
	}

	Point operator - (const Point & p) const
	{
		Point copy = *this;
		return (copy -= p);
	}
};

#define GetBoolText(val)	( val ? "True" : "False" )


//----------------------------------------------- TYPEDEFS ----------------------------------------------------------------------------------

typedef map<int16_t, class CGuild*> GuildMap;
typedef GuildMap::iterator GuildMapIter;
typedef GuildMap::const_iterator GuildMapCIter;
typedef std::auto_ptr<class CAstoria> Astoria;

typedef std::list<class CClient*> ClientList;
typedef ClientList::iterator ClientListIter;
typedef ClientList::const_iterator ClientListCIter;

typedef uint32_t CharID;

#endif //COMMON_H
