#include <windows.h>
#include <winbase.h>
#include <string>
#include <stdint.h>
#include <sstream>
#include <fstream>

/*
class CMyDib
{
public:
	CMyDib(char *szFilename, unsigned long dwFilePointer);
	~CMyDib();
	void PaintImage(HDC hDC);
	WORD m_wWidthX;
	WORD m_wWidthY;
	WORD m_wColorNums;//bmp
	LPSTR m_lpDib;
	LPBITMAPINFO m_bmpInfo; //bmp
};*/
class CMyDib
{
public:
	CMyDib(std::wstring szFilename, unsigned long dwFilePointer);
	~CMyDib();
	uint16_t m_wWidthX;
	uint16_t m_wWidthY;
	uint16_t m_wColorNums;//현재 bmp의 컬러수
	LPSTR m_lpDib;
	LPBITMAPINFO m_bmpInfo; //bmp 정보헤더
	LPBITMAPINFOHEADER bmpInfoHeader;
	BITMAPFILEHEADER fh;
	bool success;
};