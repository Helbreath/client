// Msg.h: interface for the CMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSG_H__4A385380_AADF_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_MSG_H__4A385380_AADF_11D2_B143_00001C7030A6__INCLUDED_

#include <string.h>
#include "common.h"

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
	static uint32_t nmsgs;

	CMsg(char cType, char * pMsg, uint32_t dwTime);
	CMsg();
	virtual ~CMsg();

	char   m_cType;
	char * m_pMsg;
	short  m_sX, m_sY;
	uint64_t  m_dwTime;
	uint64_t m_msgid;
	bool used;

	int    m_iObjectID;

};

#endif // !defined(AFX_MSG_H__4A385380_AADF_11D2_B143_00001C7030A6__INCLUDED_)
