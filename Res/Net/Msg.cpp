// Msg.cpp: implementation of the CMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "Msg.h"
ChatType CMsg::predremove;
uint32 CMsg::nmsgs = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsg::CMsg(char cType, char * pMsg, DWORD dwTime)
{
	m_cType = cType;

	m_pMsg = NULL;
	m_pMsg = new char [strlen(pMsg) + 1];
	ZeroMemory(m_pMsg, strlen(pMsg) + 1);
	strcpy(m_pMsg, pMsg);
	m_dwTime = dwTime;
	m_iObjectID = -1;

	m_msgid = nmsgs++;
}

CMsg::~CMsg()
{
	if (m_pMsg) delete[] m_pMsg;
}

bool CMsg::RemovePred(const CMsg * msg)
{
	return msg->m_dwTime == predremove;
}
