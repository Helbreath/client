// CharInfo.cpp: implementation of the CCharInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "CharInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharInfo::CCharInfo()
{
	memset(m_cName, 0, sizeof(m_cName));
	memset(m_cMapName, 0, sizeof(m_cMapName));
	m_iApprColor = 0;
	m_iYear   = 0;
	m_iMonth  = 0;
	m_iDay    = 0;
	m_iHour   = 0;
	m_iMinute = 0;
}

CCharInfo::~CCharInfo()
{
}
