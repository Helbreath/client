//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "TileSpr.h"

CTileSpr::CTileSpr()
    : m_sTileSprite(0),
    m_sTileSpriteFrame(0),
    m_sObjectSprite(0),
    m_sObjectSpriteFrame(0),
    m_bIsMoveAllowed(true),
    m_bIsTeleport(false)
{
}

CTileSpr::~CTileSpr()
{
}
