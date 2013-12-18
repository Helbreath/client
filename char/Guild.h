#ifndef GUILD_H
#define GUILD_H
#pragma once

#include "common.h"

struct GuildMember{
	uint8 rank;
	uint8 map;
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

typedef HashMap<string, GuildMember> MemberList;

class CGuild
{
public:
	CGuild();
	~CGuild();
	void Reset();
	
	void AddMember(GuildMember member, string name);
	void RemoveMember(string name);
	uint32 GetOnlineCnt()	const { return m_onlineList.size(); }
	uint32 CanSummon(string map) const;
		
	uint8 m_upgrades[GU_MAX];
	uint32 m_gold;
	uint32 m_maj;
	uint32 m_cont;
	uint8 m_captains, m_raidmasters, m_huntmasters;
	MemberList m_onlineList;
};

#endif