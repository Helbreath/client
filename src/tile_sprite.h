//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "common.h"

class tile_sprite
{
public:
    tile_sprite();
    virtual ~tile_sprite();
    short m_sTileSprite;
    short m_sTileSpriteFrame;
    short m_sObjectSprite;
    short m_sObjectSpriteFrame;
    bool  m_bIsMoveAllowed;
    bool  m_bIsTeleport;
};
