//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "common.h"

#pragma once

class CCharInfo  
{
public:
    CCharInfo() = default;
    ~CCharInfo() = default;
    std::string m_cName, m_cMapName;
    int64_t id = 0;
    uint16_t m_sSkinCol = 0;
    uint16_t m_sSex = 0;
    uint16_t m_sAppr1 = 0;
    uint16_t m_sAppr2 = 0;
    uint16_t m_sAppr3 = 0;
    uint16_t m_sAppr4 = 0;
    uint16_t m_sHeadApprValue = 0;
    uint16_t m_sBodyApprValue = 0;
    uint16_t m_sArmApprValue = 0;
    uint16_t m_sLegApprValue = 0; // Re-Coding Sprite xRisenx
    uint16_t m_sStr = 0;
    uint16_t m_sVit = 0;
    uint16_t m_sDex = 0;
    uint16_t m_sInt = 0;
    uint16_t m_sMag = 0;
    uint16_t m_sChr = 0;
    uint16_t m_sLevel = 0;
    int64_t m_iExp = 0;
    int32_t m_iApprColor = 0;
    int32_t m_iYear = 0;
    int32_t m_iMonth = 0;
    int32_t m_iDay = 0;
    int32_t m_iHour = 0;
    int32_t m_iMinute = 0;
};
