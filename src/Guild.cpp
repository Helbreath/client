//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "helbreath.h"

extern helbreath * game;

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

void CGuild::AddMember(GuildMember member, std::string name)
{
    m_onlineList[name] = member;
}

void CGuild::RemoveMember(std::string name)
{
    m_onlineList.erase( name );
}

uint32_t CGuild::CanSummon(std::string map) const
{
    if(m_gold < GUILDSUMMONSGOLDCOST)
        return CANSUMMON_GOLD;

    switch(m_upgrades[ GU_SUMMONS ])
    {
    case 1:
        if(map == "2ndmiddle")
            return CANSUMMON_OK;
        else{
            return CANSUMMON_LV1;
        }
        
    case 2:
        if(map == "2ndmiddle" || game->m_side == ARESDEN && map == "arebar" || game->m_side == ELVINE && map == "elvbar")
            return CANSUMMON_OK;
        else{
            return CANSUMMON_LV2;
        }

    case 3:
        if(map == "2ndmiddle" || map == "catacombs" ||
            (game->m_side == ARESDEN && map == "arebar") || 
            (game->m_side == ELVINE && map == "elvbar"))
        {
            return CANSUMMON_OK;
        }else{
            return CANSUMMON_LV3;
        }

    case 4:
        if(map == "2ndmiddle" || map == "catacombs" || map == "outlands" ||
            (game->m_side == ARESDEN && map == "arebar") || 
            (game->m_side == ELVINE && map == "elvbar"))
        {
            return CANSUMMON_OK;
        }else{
            return CANSUMMON_LV4;
        }
    default:
        return CANSUMMON_NONE;
    }
}
