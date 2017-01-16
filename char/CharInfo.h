// CharInfo.h: interface for the CCharInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARINFO_H__DD443240_AD62_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_CHARINFO_H__DD443240_AD62_11D2_B143_00001C7030A6__INCLUDED_

#include "../Shared/common.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CCharInfo  
{
public:
	CCharInfo();
	virtual ~CCharInfo();
	string   m_cName, m_cMapName;
	uint16_t m_sSkinCol, m_sSex, m_sAppr1, m_sAppr2, m_sAppr3, m_sAppr4;
	uint16_t m_sHeadApprValue, m_sBodyApprValue, m_sArmApprValue, m_sLegApprValue; // Re-Coding Sprite xRisenx
	uint16_t m_sStr, m_sVit, m_sDex, m_sInt, m_sMag, m_sChr;
	uint16_t m_sLevel;
	int64_t  m_iExp;
	int32_t  m_iApprColor;
	int32_t  m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute;
};

#endif // !defined(AFX_CHARINFO_H__DD443240_AD62_11D2_B143_00001C7030A6__INCLUDED_)
