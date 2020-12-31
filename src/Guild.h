#ifndef GUILD_H
#define GUILD_H
#pragma once

#include "common.h"

struct GuildMember{
    uint8_t rank;
    uint8_t map;
    time_t summonTime;
};

enum CanSummon{
    CANSUMMON_OK,
    CANSUMMON_LV1,
    CANSUMMON_LV2,
    CANSUMMON_LV3,
    CANSUMMON_LV4,
    CANSUMMON_GOLD,
    CANSUMMON_NONE
};

typedef std::unordered_map<string, GuildMember> MemberList;

class CGuild
{
public:
    CGuild();
    ~CGuild();
    void Reset();

    void AddMember(GuildMember member, std::string name);
    void RemoveMember(std::string name);
    uint32_t GetOnlineCnt()	const { return m_onlineList.size(); }
    uint32_t CanSummon(std::string map) const;
        
    uint8_t m_upgrades[GU_MAX];
    uint32_t m_gold;
    uint32_t m_maj;
    uint32_t m_cont;
    uint8_t m_captains, m_raidmasters, m_huntmasters;
    MemberList m_onlineList;
};

#endif