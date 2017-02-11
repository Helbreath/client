// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "Sprite.h"
#include "Game.h"

extern char G_cSpriteAlphaDegree;

extern int G_iAddTable31[64][510], G_iAddTable63[64][510];
extern int G_iAddTransTable31[510][64], G_iAddTransTable63[510][64]; 

extern long    G_lTransG100[64][64], G_lTransRB100[64][64];
extern long    G_lTransG70[64][64], G_lTransRB70[64][64];
extern long    G_lTransG50[64][64], G_lTransRB50[64][64];
extern long    G_lTransG25[64][64], G_lTransRB25[64][64];
extern long    G_lTransG2[64][64], G_lTransRB2[64][64];


extern CGame * G_pGame;

#ifndef WIN32
typedef struct tagBITMAPINFOHEADER{
	uint32_t      biSize;
	int32_t       biWidth;
	int32_t       biHeight;
	uint16_t      biPlanes;
	uint16_t      biBitCount;
	uint32_t      biCompression;
	uint32_t      biSizeImage;
	int32_t       biXPelsPerMeter;
	int32_t       biYPelsPerMeter;
	uint32_t      biClrUsed;
	uint32_t      biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	uint8_t    rgbBlue;
	uint8_t    rgbGreen;
	uint8_t    rgbRed;
	uint8_t    rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER    bmiHeader;
	RGBQUAD             bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSprite::CSprite(std::ifstream & hPakFile, std::string & cPakFileName, short sNthFile, bool bAlphaEffect)
{
	int iASDstart;

	m_stBrush	= 0;
	m_bIsSurfaceEmpty = true;

	m_cAlphaDegree = 1;
	m_bOnCriticalSection = false;
	m_iTotalFrame = 0;

    sprite = 0;

	hPakFile.seekg (24 + sNthFile*8, std::ios::beg);
	hPakFile.read ((char*)&iASDstart, 4);
	hPakFile.seekg (iASDstart+100, std::ios::beg);
	hPakFile.read ((char*)&m_iTotalFrame, 4);

	// 	SetFilePointer(hPakFile, 24 + sNthFile*8, NULL, FILE_BEGIN);
	// 	ReadFile(hPakFile, &iASDstart,  4, &nCount, NULL); // i´Â ASDÆÄÀÏÀÇ ½ÃÀÛÀ§Ä¡
	// 	SetFilePointer(hPakFile, iASDstart+100, NULL, FILE_BEGIN); 
	// 	ReadFile(hPakFile, &m_iTotalFrame,  4, &nCount, NULL);
	m_dwBitmapFileStartLoc = iASDstart  + (108 + (12*m_iTotalFrame));
	m_stBrush = new stBrush[m_iTotalFrame];
	hPakFile.read ((char*)m_stBrush, 12*m_iTotalFrame);
	//ReadFile(hPakFile, m_stBrush, 12*m_iTotalFrame, &nCount, NULL);
	m_cPakFileName = cPakFileName;
	m_bAlphaEffect = bAlphaEffect;
	wPageid = sNthFile;
    sprite = new Sprite[m_iTotalFrame];
 	for (int i = 0; i < m_iTotalFrame; ++i)
        sprite[i] = Sprite();
// 	subtextures = new ITexture *[m_iTotalFrame];
// 	for (int i = 0; i < m_iTotalFrame; ++i)
// 		subtextures[i] = 0;
	//_pMakeSpriteSurface();
}

CSprite::~CSprite(void)
{
	if (m_stBrush != 0) delete[] m_stBrush;
/*	delete[] subtextures;*/
	int test;
// 	if (_localimage)
// 		test = _localimage->getReferenceCount();
// 	if (_localimage) _localimage->drop();
// 	if (_localshadow) _localshadow->drop();
}

bool CSprite::_pMakeSpriteSurface()
{
    if ((m_stBrush == 0) || (!m_bIsSurfaceEmpty))
    {
        return false;
    }

	//CMyDib mydib(m_cPakFileName, m_dwBitmapFileStartLoc);

	BITMAPFILEHEADER fh;
	char * m_lpDib;
	std::stringstream ss;
	ss << "sprites\\" << m_cPakFileName << ".pak";
	std::ifstream szfile(ss.str().c_str(), std::ios::in | std::ios::binary);
	if (!szfile.is_open())
	{
		//MessageBoxW(NULL, (std::wstring("Cannot open \"") + ss.str() + "\"").c_str(), "Error", MB_OK);
		return false;
	}

	szfile.seekg(m_dwBitmapFileStartLoc, std::ios::beg);
	szfile.read((char*)&fh, 14);
	m_lpDib = (char *)new char[fh.bfSize];
	szfile.seekg(m_dwBitmapFileStartLoc, std::ios::beg);
	szfile.read((char*)m_lpDib, 14);
	szfile.read((char*)m_lpDib + 14, fh.bfSize - 14);
	szfile.close();
	LPBITMAPINFO m_bmpInfo; //bmp �������
	LPBITMAPINFOHEADER bmpInfoHeader;

	bmpInfoHeader = (LPBITMAPINFOHEADER)(m_lpDib + 14);
	m_bmpInfo = (LPBITMAPINFO)(m_lpDib + 14);
	m_wBitmapSizeX = (uint16_t)(bmpInfoHeader->biWidth);
	m_wBitmapSizeY = (uint16_t)(bmpInfoHeader->biHeight);

	//ECF_A8R8G8B8 - desired
	//ECF_R5G6B5 - most hb paks are

    sf::Image img;
    img.loadFromMemory(m_lpDib, fh.bfSize);
    img.createMaskFromColor(img.getPixel(0, 0));
    _localimage.loadFromImage(img);

    for (int i = 0; i < m_iTotalFrame; ++i)
    {
        sprite[i].setTexture(_localimage);
        sprite[i].setTextureRect(IntRect(m_stBrush[i].sx, m_stBrush[i].sy, m_stBrush[i].szx, m_stBrush[i].szy));
    }

	m_bIsSurfaceEmpty = false;

    //CreateShadow();

	return true;
}

#include "boost\filesystem.hpp"

CSprite * CSprite::CreateSprite(char * cPakFileName, short sNthFile, bool bAlphaEffect)
{
	std::wstringstream ss;
	ss << "sprites\\" << cPakFileName << ".pak";
	std::wstring str = ss.str();
	std::ifstream szfile(str.c_str(),std::ios::in|std::ios::binary);

	if (!szfile.is_open())
	{
		//error
		//MessageBoxW(NULL, ("Error loading pak: " + str).c_str(), "ERROR", MB_OK);
	}

// 	ss.str("");
// 	ss << "sprites\\raws\\" << cPakFileName << "\\";
// 	boost::filesystem::path dir(ss.str().c_str());
// 	if (!boost::filesystem::exists(dir))
// 		boost::filesystem::create_directory(dir);

	return new CSprite(szfile,std::string(cPakFileName), sNthFile, bAlphaEffect);
}

void CSprite::DrawShadow(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
// 	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
// 	G_pGame->driver->draw2DImage(_localshadow, core::position2d<s32>(sX+m_stBrush[sFrame].pvx,sY+m_stBrush[sFrame].pvy),
// 		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+m_stBrush[sFrame].szx,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
// 		color, true);
}

void CSprite::DrawSubSprite(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
    sprite[sFrame].setColor(color);
    sprite[sFrame].setPosition(sX + m_stBrush[sFrame].pvx, sY + m_stBrush[sFrame].pvy);
    G_pGame->draw(sprite[sFrame]);
// 	G_pGame->driver->draw2DImage(subtextures[sFrame], core::position2d<s32>(sX,sY),
// 		core::rect<s32>(0,0,m_stBrush[sFrame].szx,m_stBrush[sFrame].szy), 0,
// 		Color(255,255,255,255), true);
}

void CSprite::DrawSpriteNCK(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
	DrawRGBNCK(sX, sY, sFrame, dwTime, color);
}

void CSprite::DrawRGBNCK(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
    sprite[sFrame].setColor(color);
    sprite[sFrame].setPosition(sX + m_stBrush[sFrame].pvx, sY + m_stBrush[sFrame].pvy);
    G_pGame->draw(sprite[sFrame]);

//     G_pGame->driver->draw2DImage(_localimage, core::position2d<s32>(sX+m_stBrush[sFrame].pvx,sY+m_stBrush[sFrame].pvy),
// 		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+m_stBrush[sFrame].szx,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
// 		color, false);


	short dX,dY,sx,sy,szx,szy,pvx,pvy;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;
}

void CSprite::DrawSprite(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
	DrawRGB(sX, sY, sFrame, dwTime, color);
}

void CSprite::DrawRGB(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
    sprite[sFrame].setColor(color);
    sprite[sFrame].setPosition(sX + m_stBrush[sFrame].pvx, sY + m_stBrush[sFrame].pvy);
    G_pGame->draw(sprite[sFrame]);
// 	G_pGame->driver->draw2DImage(_localimage, core::position2d<s32>(sX+m_stBrush[sFrame].pvx,sY+m_stBrush[sFrame].pvy),
// 		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+m_stBrush[sFrame].szx,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
// 		color, true);

	short dX,dY,sx,sy,szx,szy,pvx,pvy;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;
}

void CSprite::DrawScaledSprite(int sX, int sY, int sFrame, int sWidth, int sHeight, uint64_t dwTime, Color color)
{
    if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
    sf::FloatRect f = sprite[sFrame].getLocalBounds();
    sprite[sFrame].setScale((sWidth / f.width)*100, (sHeight / f.height)*100);
    sprite[sFrame].setColor(color);
    sprite[sFrame].setPosition(sX + m_stBrush[sFrame].pvx, sY + m_stBrush[sFrame].pvy);
    G_pGame->draw(sprite[sFrame]);
    sprite[sFrame].setScale(1.0, 1.0);
// 	G_pGame->driver->draw2DImage(_localimage, core::position2d<s32>(sX,sY),
// 		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+sWidth,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy+sHeight), 0,
// 		color, true);
}

void CSprite::DrawWidth(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime, Color color)
{
	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
    sprite[sFrame].setTextureRect(IntRect(m_stBrush[sFrame].sx, m_stBrush[sFrame].sy, sWidth, m_stBrush[sFrame].szy));
    sprite[sFrame].setColor(color);
    sprite[sFrame].setPosition(sX + m_stBrush[sFrame].pvx, sY + m_stBrush[sFrame].pvy);
    G_pGame->draw(sprite[sFrame]);
    sprite[sFrame].setTextureRect(IntRect(m_stBrush[sFrame].sx, m_stBrush[sFrame].sy, m_stBrush[sFrame].szx, m_stBrush[sFrame].szy));
    // 	G_pGame->driver->draw2DImage(_localimage, core::position2d<s32>(sX,sY),
// 		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+sWidth,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
// 		color, true);
}

void CSprite::PutSpriteFast(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(255,255,255,255));
	return;
}

void CSprite::PutSpriteFastNoColorKey(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(255,255,255,255));
	return;
}

void CSprite::PutSpriteFastFrontBuffer(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(255,255,255,255));
	return;
}

void CSprite::PutSpriteFastWidth(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime)
{
	DrawWidth(sX, sY, sFrame, sWidth, dwTime, Color(255,255,255,255));
	return;
}

void CSprite::PutShadowSprite(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(255,255,255,255));
	return;
}

void CSprite::PutShadowSpriteClip(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(255,255,255,255));
	return;
}

void CSprite::PutTransSprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(alphaDepth,255,255,255));
	return;
}

void CSprite::PutTransSprite_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(alphaDepth,255,255,255));
	return;
}

void CSprite::PutTransSprite70(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(180,255,255,255));
	return;
}

void CSprite::PutTransSprite70_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(180,255,255,255));
	return;
}

void CSprite::PutTransSprite50(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(125,255,255,255));
	return;
}

void CSprite::PutTransSprite50_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(125,255,255,255));
	return;
}

void CSprite::PutTransSprite25(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(64,255,255,255));
	return;
}

void CSprite::PutTransSprite25_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(64,255,255,255));
	return;
}

void CSprite::PutTransSprite2(int sX, int sY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(180,255,255,255));
	return;
}

void CSprite::PutShiftTransSprite2(int sX, int sY, int shX, int shY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(255,255,255,255));
	return;
}

void CSprite::PutFadeSprite(short sX, short sY, short sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(255,255,255,255));
	return;
}

void CSprite::PutFadeSpriteDst(uint16_t * pDstAddr, short sPitch, short sX, short sY, short sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(255,255,255,255));
	return;
}

void CSprite::PutSpriteRGB(int sX, int sY, int sFrame, uint32_t color, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(color));
}

void CSprite::PutSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(255,sRed,sGreen,sBlue));
	return;
}

void CSprite::PutTransSpriteRGB(int sX, int sY, int sFrame, uint32_t color, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(color));
}

void CSprite::PutTransSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(180, sRed, sGreen, sBlue));
	return;
}

void CSprite::PutTransSpriteRGB_NoColorKey(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(180,sRed,sGreen,sBlue));
	return;
}

void CSprite::_GetSpriteRect(int sX, int sY, int sFrame)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
	if( this == 0 ) return;
	if( m_stBrush == 0 ) return;
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	
	m_rcBound.top    = -1;
	m_rcBound.bottom = -1;
	m_rcBound.left   = -1;
	m_rcBound.right  = -1;

	if (dX < 0) 								  
	{
		sx = sx	+ (0 - dX);							
		szx = szx - (0 - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)0;
	}
	else if (dX+szx > G_pGame->GetWidth())
	{
		szx = szx - ((dX+szx) - (short)G_pGame->GetWidth());
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < 0) 								  
	{
		sy = sy	+ (0 - dY);
		szy = szy - (0 - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)0;
	}
	else if (dY+szy > G_pGame->GetHeight())
	{
		szy = szy - ((dY+szy) - (short)G_pGame->GetHeight());
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_sPivotX = pvx;
	m_sPivotY = pvy;
}

bool CSprite::_bCheckCollison(int sX, int sY, short sFrame, int msX, int msY)
{
// 	short dX,dY,sx,sy,szx,szy,pvx,pvy;
// 	int  ix, iy;
// 	uint16_t * pSrc;
// 	int  tX, tY;
// 	
// 	if( this == 0 ) return false;
// 	if (m_stBrush == 0) return false;
// 	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return false;
// 	if (m_bIsSurfaceEmpty == true) return false;
// 	if (msX < 0 + 3) return false;
// 	if (msX > G_pGame->GetWidth() - 3) return false;
// 	if (msY < 0 + 3) return false;
// 	if (msY > G_pGame->GetHeight() - 3) return false;
// 
// 	sx  = m_stBrush[sFrame].sx;
// 	sy  = m_stBrush[sFrame].sy;
// 	szx = m_stBrush[sFrame].szx;
// 	szy = m_stBrush[sFrame].szy;
// 	pvx = m_stBrush[sFrame].pvx;
// 	pvy = m_stBrush[sFrame].pvy;
// 
//   	dX = sX + pvx;
// 	dY = sY + pvy;
// 
// 	if (msX < dX) return false;
// 	if (msX > dX + szx) return false;
// 	if (msY < dY) return false;
// 	if (msY > dY + szy) return false;
// 
// 	if (dX < 0+3)
// 	{
// 		sx = sx	+ (0+3 - dX);
// 		szx = szx - (0+3 - dX);
// 		if (szx < 0) {
// 			m_rcBound.top = -1;
// 			return false;
// 		}
// 		dX = (short)0+3;
// 	}
// 	else if (dX+szx > G_pGame->GetWidth()-3)
// 	{
// 		szx = szx - ((dX+szx) - (short)G_pGame->GetWidth()-3);
// 		if (szx < 0) {
// 			m_rcBound.top = -1;
// 			return false;
// 		}
// 	}
// 
// 	if (dY < 0+3)
// 	{
// 		sy = sy	+ (0+3 - dY);
// 		szy = szy - (0+3 - dY);
// 		if (szy < 0) {
// 			m_rcBound.top = -1;
// 			return false;
// 		}
// 		dY = (short)0+3;
// 	}
// 	else if (dY+szy > G_pGame->GetHeight()-3)
// 	{
// 		szy = szy - ((dY+szy) - (short)G_pGame->GetHeight()-3);
// 		if (szy < 0) {
// 			m_rcBound.top = -1;
// 			return false;
// 		}
// 	}
// 	
// 	SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
// 
// 	pSrc = (uint16_t *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
// 	tX = dX;
// 	tY = dY;
// 
// 
// //	pSrc += m_sPitch * ( msY - tY );
// //	if( pSrc[msX-tX] != m_wColorKey ) return TRUE;
// //	else return FALSE;
// 
// 	if (msY < tY + 3) return false;
// 	if (msX < tX + 3) return false;
// 	ECOLOR_FORMAT fmt = _localimage->getColorFormat();
// 	return true;
// 	// 
// 	if (fmt == ECF_A8R8G8B8)
// 	{
// 		//else fails
// 		void * data = _localimage->lock();
// 		uint32_t * pixel = (uint32_t*)data;
// 		
// 		_localimage->unlock();
// 	}
// 	return false;
// 	//pSrc = (WORD*)data;
// 	pSrc += m_sPitch * ( msY - tY - 3 );
// 	for( iy=0 ; iy<=6 ; iy++ )
// 	{
// 		for( ix=msX-tX-3 ; ix<=msX-tX+3 ; ix++ )
// 		{
// 			if( pSrc[ix] != m_wColorKey )
// 			{
// 				return false;
// 			}
// 		}
// 		pSrc += m_sPitch;
// 	}
	return false;
}

void CSprite::PutShiftSpriteFast(int sX, int sY, int shX, int shY, int sFrame, uint64_t dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(255,255,255,255));
	return;
}

void CSprite::PutRevTransSprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth)
{
	DrawSprite(sX, sY, sFrame, dwTime, Color(255,255,255,255));
	return;
}

