// Msg.h: interface for the CMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSG_H__4A385380_AADF_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_MSG_H__4A385380_AADF_11D2_B143_00001C7030A6__INCLUDED_

#include <windows.h>
#include <string.h>
#include <winbase.h>
#include "../../Shared/common.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMsg  
{
public:
	static bool OrderPred(const CMsg * a, const CMsg * b)
	{
		return a->m_msgid < b->m_msgid;
	};
	static bool RemovePred(const CMsg * msg);
	static void SetPred(ChatType t)	{ predremove = t; }
	static ChatType predremove;
	static uint32 nmsgs;

	CMsg(char cType, char * pMsg, DWORD dwTime);
	virtual ~CMsg();

	char   m_cType;
	char * m_pMsg;
	short  m_sX, m_sY;
	DWORD  m_dwTime;
	uint32 m_msgid;

	int    m_iObjectID;

};

#endif // !defined(AFX_MSG_H__4A385380_AADF_11D2_B143_00001C7030A6__INCLUDED_)
