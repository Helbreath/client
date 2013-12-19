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
	uint16_t m_wColorNums;//���� bmp�� �÷���
	LPSTR m_lpDib;
	LPBITMAPINFO m_bmpInfo; //bmp �������
	LPBITMAPINFOHEADER bmpInfoHeader;
	BITMAPFILEHEADER fh;
	bool success;
};