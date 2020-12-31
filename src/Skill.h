// Skill.h: interface for the CSkill class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

#include "skilltypes.h"

class CSkill
{
public:
    CSkill();
    virtual ~CSkill();

    char m_cName[21];

    int m_iLevel;
    bool m_bIsUseable;
    char m_cUseMethod;
};
