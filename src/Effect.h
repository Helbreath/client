//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "common.h"

class effect
{
public:
    effect();
    virtual ~effect();

    short m_sType;
    char  m_cFrame, m_cMaxFrame;
    char  m_cDir;
    uint64_t m_dwTime, m_dwFrameTime;
    int   m_sX, m_sY, m_dX, m_dY;
    int   m_mX, m_mY, m_mX2, m_mY2, m_mX3, m_mY3;
    int   m_iErr;
    int   m_rX, m_rY;
    int   m_iV1;
};
