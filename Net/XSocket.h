/*
// XSocket.h: interface for the XSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSOCKET_H__F9D1BA42_7338_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_XSOCKET_H__F9D1BA42_7338_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define _WINSOCK2API_
//#define  FD_SETSIZE 2000
#include <windows.h>
#include <windowsx.h>
#include <winsock2.h>
//#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <winbase.h>

#define XSOCK_LISTENSOCK			1
#define XSOCK_NORMALSOCK			2				
#define XSOCK_SHUTDOWNEDSOCK		3				

#define XSOCKSTATUS_READINGHEADER	11
#define XSOCKSTATUS_READINGBODY		12

#define XSOCKEVENT_SOCKETMISMATCH			-121
#define XSOCKEVENT_CONNECTIONESTABLISH		-122
#define XSOCKEVENT_RETRYINGCONNECTION		-123
#define XSOCKEVENT_ONREAD					-124
#define XSOCKEVENT_READCOMPLETE				-125
#define XSOCKEVENT_UNKNOWN					-126
#define XSOCKEVENT_SOCKETCLOSED				-127
#define XSOCKEVENT_BLOCK					-128
#define XSOCKEVENT_SOCKETERROR				-129
#define XSOCKEVENT_CRITICALERROR			-130
#define XSOCKEVENT_NOTINITIALIZED			-131
#define XSOCKEVENT_MSGSIZETOOLARGE			-132
#define XSOCKEVENT_CONFIRMCODENOTMATCH		-133
#define XSOCKEVENT_QUENEFULL                -134
#define XSOCKEVENT_UNSENTDATASENDBLOCK		-135
#define XSOCKEVENT_UNSENTDATASENDCOMPLETE	-136

#define XSOCKBLOCKLIMIT						300	

class XSocket 
{
public:		
	void * operator new (size_t size) 
	{
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);	
	};
	
	void operator delete(void * mem)
	{
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, mem); 		
	};
	int iSendMsgBlockingMode(char *buf,int nbytes);
	int iGetPeerAddress(char * pAddrString);
	char * pGetRcvDataPointer(DWORD * pMsgSize, char * pKey = NULL);
	BOOL bAccept(class XSocket * pXSock, unsigned int uiMsg);
	BOOL bListen(char * pAddr, int iPort, unsigned int uiMsg);
	
	int iSendMsg(char * cData, DWORD dwSize, char cKey = NULL);
	BOOL bConnect(char * pAddr, int iPort, unsigned int uiMsg, bool addrIsUrl = false);
	BOOL bBlockConnect(char * pAddr, int iPort, unsigned int uiMsg);
	int  iOnSocketEvent(WPARAM wParam, LPARAM lParam);
	BOOL bInitBufferSize(DWORD dwBufferSize);
	static void GetMACaddress(char * buffer);
	int SendRawMsg(char * cData, DWORD dwSize);
	XSocket(HWND hWnd, int iBlockLimit);
	virtual ~XSocket();

 	typedef struct _ASTAT_
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER    NameBuff [30];
	}ASTAT;

	int  m_WSAErr;
	BOOL m_bIsAvailable;
	BOOL m_bIsWriteEnabled;

	void _CloseConn();
	
	int _iSendUnsentData();
	int _iRegisterUnsentData(char * cData, int iSize);
	int _iSend(char * cData, int iSize, BOOL bSaveFlag);
	int _iSend_ForInternalUse(char * cData, int iSize);
	int _iOnRead();
	
	char   m_cType;
	char * m_pRcvBuffer;
	char * m_pSndBuffer;
	DWORD  m_dwBufferSize;
	
	SOCKET m_Sock;
	char   m_cStatus;
	DWORD  m_dwReadSize;
	DWORD  m_dwTotalReadSize;
	char   m_pAddr[30];
	int    m_iPortNum;

	char * m_pUnsentDataList[XSOCKBLOCKLIMIT];
	int    m_iUnsentDataSize[XSOCKBLOCKLIMIT];
	short  m_sHead, m_sTail;

	unsigned int m_uiMsg;
	HWND         m_hWnd;

	int			 m_iBlockLimit;
};

#endif // !defined(AFX_XSOCKET_H__F9D1BA42_7338_11D2_A8E6_00001C7030A6__INCLUDED_)
*/
