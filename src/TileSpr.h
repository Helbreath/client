// TileSpr.h: interface for the CTileSpr class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

class CTileSpr
{
public:
    CTileSpr();
    virtual ~CTileSpr();
    short m_sTileSprite;
    short m_sTileSpriteFrame;
    short m_sObjectSprite;
    short m_sObjectSpriteFrame;
    bool  m_bIsMoveAllowed;
    bool  m_bIsTeleport;
};
