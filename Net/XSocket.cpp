/*
// XSocket.cpp: implementation of the XSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "XSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XSocket::XSocket(HWND hWnd, int iBlockLimit)
{
	int i;
	
	m_cType       = NULL;
	m_pRcvBuffer  = NULL;
	m_pSndBuffer  = NULL;
	m_Sock        = INVALID_SOCKET;
	m_dwBufferSize = 0;

	m_cStatus   = XSOCKSTATUS_READINGHEADER;      
	m_dwReadSize = 3;    
	m_dwTotalReadSize = 0;

	for (i = 0; i < XSOCKBLOCKLIMIT; i++) {
		m_iUnsentDataSize[i] = 0;
		m_pUnsentDataList[i] = NULL;
	}
	
	m_sHead = 0;
	m_sTail = 0;

	m_WSAErr = NULL;

	m_hWnd = hWnd;
	m_bIsAvailable = FALSE;
	m_bIsWriteEnabled = FALSE;

	m_iBlockLimit = iBlockLimit;
}

XSocket::~XSocket()
{
	int i;	
	if (m_pRcvBuffer != NULL) delete[] m_pRcvBuffer;
	if (m_pSndBuffer != NULL) delete[] m_pSndBuffer;
	for (i = 0; i < XSOCKBLOCKLIMIT; i++)
		if (m_pUnsentDataList[i] != NULL) delete[] m_pUnsentDataList[i];

	_CloseConn(); 
}

bool XSocket::bInitBufferSize(uint32_t dwBufferSize)
{
	if (m_pRcvBuffer != NULL) delete[] m_pRcvBuffer;
	if (m_pSndBuffer != NULL) delete[] m_pSndBuffer;

	m_pRcvBuffer = new char[dwBufferSize+8];
	if (m_pRcvBuffer == NULL) return FALSE;
	
	m_pSndBuffer = new char[dwBufferSize+8];
	if (m_pSndBuffer == NULL) return FALSE;

	m_dwBufferSize = dwBufferSize;

	return TRUE;
}

int XSocket::iOnSocketEvent(WPARAM wParam, LPARAM lParam)
{
 int WSAEvent;

	if (m_cType != XSOCK_NORMALSOCK) return XSOCKEVENT_SOCKETMISMATCH;
	if (m_cType == NULL) return XSOCKEVENT_NOTINITIALIZED;

	if ((SOCKET)wParam != m_Sock) return XSOCKEVENT_SOCKETMISMATCH;
	WSAEvent = WSAGETSELECTEVENT(lParam);

	switch (WSAEvent) {
	case FD_CONNECT:
		if (WSAGETSELECTERROR(lParam) != 0) {
			if (bConnect(m_pAddr, m_iPortNum, m_uiMsg) == FALSE) return XSOCKEVENT_SOCKETERROR;
			return XSOCKEVENT_RETRYINGCONNECTION;
		}
		else {
			m_bIsAvailable = TRUE;
			return XSOCKEVENT_CONNECTIONESTABLISH;
		}
		break;

	case FD_READ:
		if (WSAGETSELECTERROR(lParam) != 0) {
			m_WSAErr = WSAGETSELECTERROR(lParam);
			return XSOCKEVENT_SOCKETERROR;
		}
		else return _iOnRead();
		break;
	
	case FD_WRITE:
		m_bIsWriteEnabled = TRUE;
		return _iSendUnsentData();
		break;

	case FD_CLOSE:
		m_cType = XSOCK_SHUTDOWNEDSOCK; 
		return XSOCKEVENT_SOCKETCLOSED;
		break;
	}

	return XSOCKEVENT_UNKNOWN;
}

bool XSocket::bBlockConnect(char * pAddr, int iPort, unsigned int uiMsg)
{
 SOCKADDR_IN	 saTemp;
 int             iRet;
 DWORD			 dwOpt;
 struct hostent * hp;

	if (m_cType == XSOCK_LISTENSOCK) return FALSE;
	if (m_Sock  != INVALID_SOCKET) closesocket(m_Sock);

	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET) 
		return FALSE;
	
	memset(&saTemp,0,sizeof(saTemp));
	hp = gethostbyname(pAddr);
	if (hp) {
		memcpy(&(saTemp.sin_addr),hp->h_addr, hp->h_length); 
		saTemp.sin_family = hp->h_addrtype;
		saTemp.sin_port = htons(iPort);
	}
	else {
		return FALSE;
	}
	
	iRet = connect(m_Sock, (struct sockaddr *) &saTemp, sizeof(saTemp));
	if (iRet == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			m_WSAErr = WSAGetLastError();
			return FALSE;
		}
	}
 
 	dwOpt = 8192*5;
 	setsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF, (const char FAR *)&dwOpt, sizeof(dwOpt));
 	setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF, (const char FAR *)&dwOpt, sizeof(dwOpt));

	strcpy(m_pAddr, pAddr);
	m_iPortNum = iPort;

	m_uiMsg = uiMsg;
	m_cType = XSOCK_NORMALSOCK;

	return TRUE;
}


bool XSocket::bConnect(char * pAddr, int iPort, unsigned int uiMsg, bool addrIsUrl)
{
 SOCKADDR_IN	 saTemp;
 u_long          arg;
 int             iRet;
 DWORD			 dwOpt;

	if (m_cType == XSOCK_LISTENSOCK) return FALSE;
	if (m_Sock  != INVALID_SOCKET) closesocket(m_Sock);

	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET) 
		return FALSE;
	
	arg = 1;
		
	memset(&saTemp,0,sizeof(saTemp));
	if (addrIsUrl){
		hostent * hp = gethostbyname(pAddr);
		if (hp) {
			memcpy(&(saTemp.sin_addr),hp->h_addr, hp->h_length); 
			saTemp.sin_family = hp->h_addrtype;
		}
		else return false;
	}
	else
	{
		saTemp.sin_family = AF_INET;
		saTemp.sin_addr.s_addr = inet_addr(pAddr);
	}
	saTemp.sin_port = htons(iPort);

	WSAAsyncSelect(m_Sock, m_hWnd, uiMsg, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	m_uiMsg = uiMsg;

	iRet = connect(m_Sock, (struct sockaddr *) &saTemp, sizeof(saTemp));
	if (iRet == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			m_WSAErr = WSAGetLastError();
			return FALSE;
		}
	}
 
 	dwOpt = 8192*5;
 	setsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF, (const char FAR *)&dwOpt, sizeof(dwOpt));
 	setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF, (const char FAR *)&dwOpt, sizeof(dwOpt));

	strcpy(m_pAddr, pAddr);
	m_iPortNum = iPort;

	m_cType = XSOCK_NORMALSOCK;

	return TRUE;
}


int XSocket::_iOnRead()
{
 int iRet, WSAErr;
 uint16_t  * wp;	

	if (m_cStatus == XSOCKSTATUS_READINGHEADER) {
		
		iRet = recv(m_Sock, (char *)(m_pRcvBuffer + m_dwTotalReadSize), m_dwReadSize, 0);	
		
		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				m_WSAErr = WSAErr;
				return XSOCKEVENT_SOCKETERROR;
			}
			else return XSOCKEVENT_BLOCK;
		}
		else 
		if (iRet == 0) {
			m_cType = XSOCK_SHUTDOWNEDSOCK;
			return XSOCKEVENT_SOCKETCLOSED;
		}
		
		m_dwReadSize      -= iRet;
		m_dwTotalReadSize += iRet;
		
		if (m_dwReadSize == 0) {
			m_cStatus = XSOCKSTATUS_READINGBODY;
			wp = (uint16_t *)(m_pRcvBuffer + 1);
			m_dwReadSize = (int)(*wp - 3); 
			
			if (m_dwReadSize == 0) {
				m_cStatus        = XSOCKSTATUS_READINGHEADER;
				m_dwReadSize      = 3;
				m_dwTotalReadSize = 0;
				return XSOCKEVENT_READCOMPLETE;
			}
			else 
			if (m_dwReadSize > m_dwBufferSize) {
				m_cStatus        = XSOCKSTATUS_READINGHEADER;
				m_dwReadSize      = 3;
				m_dwTotalReadSize = 0;
				return XSOCKEVENT_MSGSIZETOOLARGE;
			}
		}
		return XSOCKEVENT_ONREAD;
	}
	else
	if (m_cStatus == XSOCKSTATUS_READINGBODY) {
		
		iRet = recv(m_Sock, (char *)(m_pRcvBuffer + m_dwTotalReadSize), m_dwReadSize, 0);
		
		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				m_WSAErr = WSAErr;
				return XSOCKEVENT_SOCKETERROR;
			}
			else return XSOCKEVENT_BLOCK;
		}
		else 
		if (iRet == 0) {
			m_cType = XSOCK_SHUTDOWNEDSOCK;
			return XSOCKEVENT_SOCKETCLOSED;
		}

		m_dwReadSize      -= iRet;
		m_dwTotalReadSize += iRet;
		
		if (m_dwReadSize == 0) 
		{	m_cStatus        = XSOCKSTATUS_READINGHEADER;
			m_dwReadSize      = 3;
			m_dwTotalReadSize = 0;
		}
		else return XSOCKEVENT_ONREAD;
	}
	return XSOCKEVENT_READCOMPLETE;
}



int XSocket::_iSend(char * cData, int iSize, bool bSaveFlag)
{
 int  iOutLen, iRet, WSAErr;

	if (m_pUnsentDataList[m_sHead] != NULL) {
		if (bSaveFlag == TRUE) {
			iRet = _iRegisterUnsentData(cData, iSize);
			switch (iRet) {
			case -1:
				return XSOCKEVENT_CRITICALERROR;
			case 0:
				return XSOCKEVENT_QUENEFULL;
			case 1:
				break;
			}
			return XSOCKEVENT_BLOCK;
		}
		else return 0;
	}

	iOutLen = 0;
	while (iOutLen < iSize) {
		
		iRet = send(m_Sock, (cData + iOutLen), iSize - iOutLen, 0); 
		
		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				m_WSAErr = WSAErr;
				return XSOCKEVENT_SOCKETERROR;
			}
			else {
				if (bSaveFlag == TRUE) {
					iRet = _iRegisterUnsentData((cData + iOutLen), (iSize - iOutLen));
					switch (iRet) {
					case -1:
						return XSOCKEVENT_CRITICALERROR;
						break;
					case 0:
						return XSOCKEVENT_QUENEFULL;
						break;
					case 1:
						break;
					}
				}
				return XSOCKEVENT_BLOCK;
			}
		} else iOutLen += iRet;
		
	}
	
	return iOutLen;
}

int XSocket::_iSend_ForInternalUse(char * cData, int iSize)
{
 int  iOutLen, iRet, WSAErr;

	iOutLen = 0;
	while (iOutLen < iSize) {
		
		iRet = send(m_Sock, (cData + iOutLen), iSize - iOutLen, 0); 
		
		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				m_WSAErr = WSAErr;
				return XSOCKEVENT_SOCKETERROR;
			}
			else {
				return iOutLen;
			}
		} else iOutLen += iRet;
	}
	
	return iOutLen;
}



int XSocket::_iRegisterUnsentData(char * cData, int iSize)
{
	if (m_pUnsentDataList[m_sTail] != NULL) return 0;
	
	m_pUnsentDataList[m_sTail] = new char[iSize];
	if (m_pUnsentDataList[m_sTail] == NULL) return -1;
	memcpy(m_pUnsentDataList[m_sTail], cData, iSize);
	m_iUnsentDataSize[m_sTail] = iSize;
	m_sTail++;
	//if (m_sTail >= XSOCKBLOCKLIMIT) m_sTail = 0;
	if (m_sTail >= m_iBlockLimit) m_sTail = 0;

	return 1;
}



int XSocket::_iSendUnsentData()
{
 int iRet;
 char * pTemp;
	
	while (m_pUnsentDataList[m_sHead] != NULL) {
		
		iRet = _iSend_ForInternalUse(m_pUnsentDataList[m_sHead], m_iUnsentDataSize[m_sHead]);

		if (iRet == m_iUnsentDataSize[m_sHead]) 
		{	delete[] m_pUnsentDataList[m_sHead];
			m_pUnsentDataList[m_sHead] = NULL;
			m_iUnsentDataSize[m_sHead] = 0;
			m_sHead++;
			//if (m_sHead >= XSOCKBLOCKLIMIT) m_sHead = 0;
			if (m_sHead >= m_iBlockLimit) m_sHead = 0;
		}else 
		{	if (iRet < 0) 
				return iRet;

			pTemp = new char[m_iUnsentDataSize[m_sHead] - iRet];
			memcpy(pTemp, m_pUnsentDataList[m_sHead] + iRet, m_iUnsentDataSize[m_sHead] - iRet);

			delete[] m_pUnsentDataList[m_sHead];
			m_pUnsentDataList[m_sHead] = pTemp;

			return XSOCKEVENT_UNSENTDATASENDBLOCK;
		}
	}

	return XSOCKEVENT_UNSENTDATASENDCOMPLETE;
}


int XSocket::iSendMsg(char * cData, uint32_t dwSize, char cKey)
{
 uint16_t * wp;
 int    i, iRet;

	if (dwSize > m_dwBufferSize) return XSOCKEVENT_MSGSIZETOOLARGE;

	if (m_cType != XSOCK_NORMALSOCK) return XSOCKEVENT_SOCKETMISMATCH;
	if (m_cType == NULL) return XSOCKEVENT_NOTINITIALIZED;

#ifdef _DEBUG
	cKey = 0;
#endif
//	m_pSndBuffer[0] = cKey;

	wp  = (uint16_t *)(m_pSndBuffer + 1);
	*wp = (WORD)(dwSize + 3);

	memcpy((char *)(m_pSndBuffer + 3), cData, dwSize);
// 	if (cKey != NULL) {
// 		for (i = 0; i < (int)(dwSize); i++) {
// 			m_pSndBuffer[3+i] += (i ^ cKey);
// 			m_pSndBuffer[3+i] = (char)( m_pSndBuffer[3+i] ^ (cKey ^ (dwSize - i)) );
// 		}
// 	}
	
	if (m_bIsWriteEnabled == FALSE) {
		iRet = _iRegisterUnsentData(m_pSndBuffer, dwSize +3);
	} 
	else iRet = _iSend(m_pSndBuffer, dwSize + 3, TRUE);

	if (iRet < 0) return iRet;
	else return (iRet - 3);
}

bool XSocket::bListen(char * pAddr, int iPort, unsigned int uiMsg)
{
 SOCKADDR_IN	 saTemp;

	if (m_cType != NULL) return FALSE;
	if (m_Sock  != INVALID_SOCKET) closesocket(m_Sock);

	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET) 
		return FALSE;
	
	memset(&saTemp,0,sizeof(saTemp));
	saTemp.sin_family = AF_INET;
	saTemp.sin_addr.s_addr = inet_addr(pAddr);
	saTemp.sin_port = htons(iPort);
	if ( bind(m_Sock, (PSOCKADDR)&saTemp, sizeof(saTemp)) == SOCKET_ERROR) {
		closesocket(m_Sock);
		return FALSE;
	}
	
	if (listen(m_Sock, 5) == SOCKET_ERROR) {
		closesocket(m_Sock);
		return FALSE;
	}

	WSAAsyncSelect(m_Sock, m_hWnd, uiMsg, FD_ACCEPT);

	m_uiMsg = uiMsg;
	m_cType = XSOCK_LISTENSOCK;

	return TRUE;
}

bool XSocket::bAccept(class XSocket * pXSock, unsigned int uiMsg)
{
 SOCKET			AcceptedSock;
 sockaddr		Addr;
 int iLength;
 DWORD			dwOpt;

	if (m_cType != XSOCK_LISTENSOCK) return FALSE;
	if (pXSock == NULL) return FALSE;

	iLength = sizeof(Addr); 
	AcceptedSock = accept(m_Sock, (struct sockaddr FAR *)&Addr,(int FAR *)&iLength);
	if (AcceptedSock == INVALID_SOCKET) 
		return FALSE;
		
	pXSock->m_Sock = AcceptedSock;
	WSAAsyncSelect(pXSock->m_Sock, m_hWnd, uiMsg, FD_READ | FD_WRITE | FD_CLOSE);

	pXSock->m_uiMsg = uiMsg;
 	pXSock->m_cType = XSOCK_NORMALSOCK;

 	dwOpt = 8192*5;
 	setsockopt(pXSock->m_Sock, SOL_SOCKET, SO_RCVBUF, (const char FAR *)&dwOpt, sizeof(dwOpt));
 	setsockopt(pXSock->m_Sock, SOL_SOCKET, SO_SNDBUF, (const char FAR *)&dwOpt, sizeof(dwOpt));

	return TRUE;
}

void XSocket::_CloseConn()
{
 char cTmp[100];
 bool bFlag = TRUE;	
 int  iRet;

	if (m_Sock == INVALID_SOCKET) return;

	shutdown(m_Sock, 0x01);
	while (bFlag == TRUE) {
		iRet = recv(m_Sock, cTmp, sizeof(cTmp), 0);
		if (iRet == SOCKET_ERROR) bFlag = FALSE;
		if (iRet == 0) bFlag = FALSE;
	}

	closesocket(m_Sock);

	m_cType = XSOCK_SHUTDOWNEDSOCK;
}

char * XSocket::pGetRcvDataPointer(uint32_t * pMsgSize, char * pKey)
{
 uint16_t * wp;
 uint32_t  dwSize;
 int i;
 char cKey;
	
	cKey = m_pRcvBuffer[0];
	if (pKey != NULL) *pKey = cKey;	

	wp = (uint16_t *)(m_pRcvBuffer + 1);
	*pMsgSize = (*wp) - 3;	
	dwSize    = (*wp) - 3;

	
	if (cKey != NULL) {
		for (i = 0; i < (int)(dwSize); i++) {
			m_pRcvBuffer[3+i] = (char)( m_pRcvBuffer[3+i] ^ (cKey ^ (dwSize - i)) );
			m_pRcvBuffer[3+i] -= (i ^ cKey);
		}
	}
	return (m_pRcvBuffer + 3);
}

int XSocket::iGetPeerAddress(char * pAddrString)
{
 SOCKADDR_IN sockaddr;
 int iRet, iLen;
	
	iLen = sizeof(sockaddr);
	iRet = getpeername(m_Sock, (struct sockaddr *)&sockaddr, &iLen);
	strcpy(pAddrString, (const char *)inet_ntoa(sockaddr.sin_addr));

	return iRet;
}

int XSocket::iSendMsgBlockingMode(char *buf,int nbytes)
{
 int   nleft =0,nwriten = 0 ;

	nleft =  nbytes;
	while (nleft > 0) {
		nwriten = send(m_Sock,buf,nleft,0);
		if (nwriten < 0){
			return nwriten;
		}
		if (nwriten == 0) break;
		nleft -= nwriten;
		buf +=  nwriten ;
	}
	return (nbytes - nleft);
}

int XSocket::SendRawMsg(char * cData, uint32_t dwSize)
{
	if (dwSize > m_dwBufferSize) return XSOCKEVENT_MSGSIZETOOLARGE;
	if (m_cType != XSOCK_NORMALSOCK) return XSOCKEVENT_SOCKETMISMATCH;
	if (m_cType == NULL) return XSOCKEVENT_NOTINITIALIZED;

	if(!m_pSndBuffer) return XSOCKEVENT_NOTINITIALIZED;

	memcpy(m_pSndBuffer, cData, dwSize);

	return _iSend(m_pSndBuffer, dwSize, TRUE);
}

void XSocket::GetMACaddress(char * buffer)
{
	ASTAT Adapter;
	NCB Ncb;
	UCHAR uRetCode;
	char NetName[50];
	LANA_ENUM   lenum;
	int      i;

	memset( &Ncb, 0, sizeof(Ncb) );
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (UCHAR *)&lenum;
	Ncb.ncb_length = sizeof(lenum);
	uRetCode = Netbios( &Ncb );
	//printf( "The NCBENUM return code is: 0x%x \n", uRetCode );

	for(i=0; i < lenum.length ;i++)
	{
		memset( &Ncb, 0, sizeof(Ncb) );
		Ncb.ncb_command = NCBRESET;
		Ncb.ncb_lana_num = lenum.lana[i];

		uRetCode = Netbios( &Ncb );
		//printf( "The NCBRESET on LANA %d return code is: 0x%x \n",
		//	lenum.lana[i], uRetCode );

		memset( &Ncb, 0, sizeof (Ncb) );
		Ncb.ncb_command = NCBASTAT;
		Ncb.ncb_lana_num = lenum.lana[i];

		strcpy((char *)Ncb.ncb_callname,  "*               " );
		Ncb.ncb_buffer = (UCHAR *) &Adapter;
		Ncb.ncb_length = sizeof(Adapter);

		uRetCode = Netbios( &Ncb );
		//printf( "The NCBASTAT on LANA %d return code is: 0x%x \n",
		//	lenum.lana[i], uRetCode );
		if ( uRetCode == 0 )
		{
			wsprintfA(buffer, "%02x%02x%02x%02x%02x%02x",
				Adapter.adapt.adapter_address[0],
				Adapter.adapt.adapter_address[1],
				Adapter.adapt.adapter_address[2],
				Adapter.adapt.adapter_address[3],
				Adapter.adapt.adapter_address[4],
				Adapter.adapt.adapter_address[5]);
			break;
		}
	}

}

*/
