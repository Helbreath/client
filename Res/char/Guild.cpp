#include <windows.h>
#include "..\game.h"

extern CGame * G_pGame;

CGuild::CGuild()
{
}

CGuild::~CGuild(void)
{
}

void CGuild::Reset()
{ 
	m_onlineList.clear();

	for(int i = 0; i < GU_MAX; i++)
		m_upgrades[i] = -1;
}

void CGuild::AddMember(GuildMember member, string name)
{
	m_onlineList[name] = member;
}

void CGuild::RemoveMember(string name)
{
	m_onlineList.erase( name );
}

uint32 CGuild::CanSummon(string map) const
{
	if(m_gold < GUILDSUMMONSGOLDCOST)
		return CANSUMMON_GOLD;

	switch(m_upgrades[ GU_SUMMONS ])
	{
	case 1:
		if(map == "Middleland")
			return CANSUMMON_OK;
		else{
			return CANSUMMON_LV1;
		}
		
	case 2:
		if(map == "Middleland" || map == "Icebound")
			return CANSUMMON_OK;
		else{
			return CANSUMMON_LV2;
		}

	case 3:
		if(map == "Middleland" || map == "Icebound" || map == "Toh1" || map == "Dglv2" ||
			(G_pGame->m_side == ARESDEN && map == "Aresdend1") || 
			(G_pGame->m_side == ELVINE && map == "Elvined1"))
		{
			return CANSUMMON_OK;
		}else{
			return CANSUMMON_LV3;
		}

	case 4:
		if(map == "Middleland" || map == "Icebound" || map == "Toh1" || map == "Toh2" || map == "Toh3" ||
			(G_pGame->m_side == ARESDEN && map == "Aresdend1") || 
			(G_pGame->m_side == ELVINE && map == "Elvined1") ||
			map == "Dglv2" || map == "Dglv3" || map == "Dglv4")
		{
			return CANSUMMON_OK;
		}else{
			return CANSUMMON_LV4;
		}
	default:
		return CANSUMMON_NONE;
	}
}