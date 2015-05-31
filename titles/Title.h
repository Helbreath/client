// Title.h: interface for the Title class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TITLE_H__3F70EF8A_123D_437C_9067_F4B6A2FE8FD2__INCLUDED_)
#define AFX_TITLE_H__3F70EF8A_123D_437C_9067_F4B6A2FE8FD2__INCLUDED_

#include "common.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Title  
{
public:
	Title();
	virtual ~Title();

	char m_cSubject[21];

	uint32_t m_iCurrentNo;

	int m_iNextRankPoints;
	int m_iRank;
	int m_iServerTitleIndex;

};

#endif // !defined(AFX_TITLE_H__3F70EF8A_123D_437C_9067_F4B6A2FE8FD2__INCLUDED_)
