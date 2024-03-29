//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "common.h"

enum magic_types
{
    MAGICTYPE_NONE,
    MAGICTYPE_DAMAGE_SPOT,
    MAGICTYPE_HPUP_SPOT,
    MAGICTYPE_DAMAGE_AREA,
    MAGICTYPE_SPDOWN_SPOT, // unused
    MAGICTYPE_SPDOWN_AREA,
    MAGICTYPE_SPUP_SPOT, // unused
    MAGICTYPE_SPUP_AREA,
    MAGICTYPE_TELEPORT,
    MAGICTYPE_SUMMON,
    MAGICTYPE_CREATE,
    MAGICTYPE_PROTECT,
    MAGICTYPE_HOLDOBJECT,
    MAGICTYPE_INVISIBILITY,
    MAGICTYPE_CREATE_DYNAMIC,
    MAGICTYPE_POSSESSION,
    MAGICTYPE_CONFUSE,
    MAGICTYPE_POISON,
    MAGICTYPE_BERSERK,
    MAGICTYPE_DAMAGE_LINEAR,
    MAGICTYPE_POLYMORPH,
    MAGICTYPE_DAMAGE_AREA_NOSPOT,
    MAGICTYPE_TREMOR,
    MAGICTYPE_ICE,
    MAGICTYPE_DAMAGE_SPOT_SPDOWN, // no spells
    MAGICTYPE_DAMAGE_AREA_NOSPOT_SPDOWN,

    MAGICTYPE_DAMAGE_AREA_ARMOR_BREAK,
    MAGICTYPE_ICE_LINEAR,

    MAGICTYPE_CANCELLATION,
    MAGICTYPE_INHIBITION,
    MAGICTYPE_DAMAGE_LINEAR_SPDOWN,
    MAGICTYPE_SCAN,
    MAGICTYPE_RESURRECTION,
    MAGICTYPE_DAMAGE_AREA_MOVE,
    MAGICTYPE_SOTG_LINEAR,
    MAGICTYPE_COTG_LINEAR,
    MAGICTYPE_FSW_LINEAR,
    MAGICTYPE_MB_LINEAR,
};

enum magic_protect
{
    MAGICPROTECT_PFA = 1,
    MAGICPROTECT_PFM = 2,
    MAGICPROTECT_DS = 3,
    MAGICPROTECT_GDS = 4,
    MAGICPROTECT_AMP = 5,
};

class magic
{
public:
    magic() = default;
    ~magic() = default;
    std::string m_cName;
    int m_sValue1, m_sValue2, m_sValue3;
    int m_sValue4;
    bool m_bIsVisible;
};
