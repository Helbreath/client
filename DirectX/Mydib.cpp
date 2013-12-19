//MYDIB.CPP BMP
//#include <afx.h>
//#include <AfxWin.h>
#include <windows.h>
#include <stdio.h>
#include "mydib.h"

/*
CMyDib::CMyDib(char *szFilename, unsigned long dwFilePointer)
{
	BITMAPFILEHEADER fh; //bmp 
	m_lpDib = NULL;	
	HANDLE hFileRead;
	DWORD nCount;
	char PathName[28];

	if(memcmp(szFilename, "lgn_", 4) == 0)
		wsprintf( PathName, "sprites\\%s.lpk", szFilename );
	else
		wsprintf( PathName, "sprites\\%s.pak", szFilename );

	hFileRead = CreateFile(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	SetFilePointer(hFileRead, dwFilePointer, NULL, FILE_BEGIN);
	ReadFile(hFileRead, (char *)&fh, 14, &nCount, NULL);//sizeof(bmpHeader)==14
	m_lpDib = (LPSTR)new char[fh.bfSize-14];
	ReadFile(hFileRead, (char *)m_lpDib, fh.bfSize-14, &nCount, NULL);
	CloseHandle(hFileRead);
	LPBITMAPINFOHEADER bmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;
	m_bmpInfo = (LPBITMAPINFO)m_lpDib;
	m_wWidthX = (WORD)(bmpInfoHeader->biWidth);
	m_wWidthY = (WORD)(bmpInfoHeader->biHeight);
	if (bmpInfoHeader->biClrUsed == 0)
	{	if(bmpInfoHeader->biBitCount == 24) m_wColorNums = 0;
		else if(bmpInfoHeader->biBitCount == 8) m_wColorNums = 256;
		else if(bmpInfoHeader->biBitCount == 1) m_wColorNums = 2;
		else if(bmpInfoHeader->biBitCount == 4) m_wColorNums = 16;
		else m_wColorNums = 0;
	}else m_wColorNums = (WORD)(bmpInfoHeader->biClrUsed);
}

CMyDib::~CMyDib()
{	if(m_lpDib != NULL) delete[] m_lpDib;
}

void CMyDib::PaintImage(HDC hDC)
{	if (m_lpDib == NULL) return;
	SetDIBitsToDevice(hDC, 0, 0, m_wWidthX, m_wWidthY, 0, 0, 0, m_wWidthY, m_lpDib + *(LPDWORD)m_lpDib + 4*m_wColorNums, m_bmpInfo, DIB_RGB_COLORS);
}*/

CMyDib::CMyDib(std::wstring szFilename, unsigned long dwFilePointer)
{
	success = false;
	//BITMAPFILEHEADER fh; //bmp 화일의 헤더
	m_lpDib = NULL;		//이미지 포인터와 버퍼를 null로 설정한다.
//	HANDLE hFileRead;
//	DWORD nCount;
	std::wstringstream ss;
	ss << L"sprites\\" << szFilename << L".pak";
	std::ifstream szfile(ss.str().c_str(),std::ios::in|std::ios::binary);
	if (!szfile.is_open())
	{
		//MessageBoxW(NULL, (std::wstring(L"Cannot open \"") + ss.str() + L"\"").c_str(), L"Error", MB_OK);
		return;
	}
	szfile.seekg (dwFilePointer, std::ios::beg);
	szfile.read ((char*)&fh, 14);
	//m_lpDib = (LPSTR)new char[fh.bfSize-14];
	m_lpDib = (LPSTR)new char[fh.bfSize];
	szfile.seekg (dwFilePointer, std::ios::beg);
	szfile.read ((char*)m_lpDib, 14);
	szfile.read ((char*)m_lpDib+14, fh.bfSize-14);
	szfile.close();

/*
	fstream file(L"thing.bmp", ios::out|ios::binary);
	file.write ((char*)m_lpDib, fh.bfSize);
	file.close();*/

	bmpInfoHeader = (LPBITMAPINFOHEADER)(m_lpDib+14);
	m_bmpInfo = (LPBITMAPINFO)(m_lpDib+14);
	m_wWidthX = (uint16_t)(bmpInfoHeader->biWidth);
	m_wWidthY = (uint16_t)(bmpInfoHeader->biHeight);
	if (bmpInfoHeader->biClrUsed == 0)
	{
		if(bmpInfoHeader->biBitCount == 24) m_wColorNums = 0;
		else if(bmpInfoHeader->biBitCount == 8) m_wColorNums = 256;
		else if(bmpInfoHeader->biBitCount == 1) m_wColorNums = 2;
		else if(bmpInfoHeader->biBitCount == 4) m_wColorNums = 16;
		else m_wColorNums = 0;
	}
	else m_wColorNums = (uint16_t)(bmpInfoHeader->biClrUsed);
	success = true;
}

CMyDib::~CMyDib()
{
	if(m_lpDib != NULL) delete[] m_lpDib;
}
