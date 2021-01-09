// CharInfo.h: interface for the CCharInfo class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

class CCharInfo  
{
public:
    CCharInfo();
    virtual ~CCharInfo();
    string   m_cName, m_cMapName;
    int64_t id;
    uint16_t m_sSkinCol, m_sSex, m_sAppr1, m_sAppr2, m_sAppr3, m_sAppr4;
    uint16_t m_sHeadApprValue, m_sBodyApprValue, m_sArmApprValue, m_sLegApprValue; // Re-Coding Sprite xRisenx
    uint16_t m_sStr, m_sVit, m_sDex, m_sInt, m_sMag, m_sChr;
    uint16_t m_sLevel;
    int64_t  m_iExp;
    int32_t  m_iApprColor;
    int32_t  m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute;
};
