// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "Sprite.h"
#include "../Game.h"

extern char G_cSpriteAlphaDegree;

extern int G_iAddTable31[64][510], G_iAddTable63[64][510];
extern int G_iAddTransTable31[510][64], G_iAddTransTable63[510][64]; 

extern long    G_lTransG100[64][64], G_lTransRB100[64][64];
extern long    G_lTransG70[64][64], G_lTransRB70[64][64];
extern long    G_lTransG50[64][64], G_lTransRB50[64][64];
extern long    G_lTransG25[64][64], G_lTransRB25[64][64];
extern long    G_lTransG2[64][64], G_lTransRB2[64][64];


extern CGame * G_pGame;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSprite::CSprite(std::ifstream & hPakFile, std::wstring & cPakFileName, short sNthFile, bool bAlphaEffect)
{
	int iASDstart;

	m_stBrush	= NULL;
	m_bIsSurfaceEmpty = TRUE;

	m_cAlphaDegree = 1;
	m_bOnCriticalSection = FALSE;
	m_iTotalFrame = 0;
	_localimage = _localshadow = 0;

	hPakFile.seekg (24 + sNthFile*8, std::ios::beg);
	hPakFile.read ((char*)&iASDstart, 4);
	hPakFile.seekg (iASDstart+100, std::ios::beg);
	hPakFile.read ((char*)&m_iTotalFrame, 4);

	// 	SetFilePointer(hPakFile, 24 + sNthFile*8, NULL, FILE_BEGIN);
	// 	ReadFile(hPakFile, &iASDstart,  4, &nCount, NULL); // i는 ASD파일의 시작위치
	// 	SetFilePointer(hPakFile, iASDstart+100, NULL, FILE_BEGIN); 
	// 	ReadFile(hPakFile, &m_iTotalFrame,  4, &nCount, NULL);
	m_dwBitmapFileStartLoc = iASDstart  + (108 + (12*m_iTotalFrame));
	m_stBrush = new stBrush[m_iTotalFrame];
	hPakFile.read ((char*)m_stBrush, 12*m_iTotalFrame);
	//ReadFile(hPakFile, m_stBrush, 12*m_iTotalFrame, &nCount, NULL);
	m_cPakFileName = cPakFileName;
	m_bAlphaEffect = bAlphaEffect;
	wPageid = sNthFile;
	subtextures = new ITexture *[m_iTotalFrame];
	for (int i = 0; i < m_iTotalFrame; ++i)
		subtextures[i] = 0;
	//_pMakeSpriteSurface();
}

CSprite::~CSprite(void)
{
	if (m_stBrush != NULL) delete[] m_stBrush;
	delete[] subtextures;
	int test;
	if (_localimage)
		test = _localimage->getReferenceCount();
	if (_localimage) _localimage->drop();
	if (_localshadow) _localshadow->drop();
}

// IDirectDrawSurface7 * CSprite::_pMakeSpriteSurface()
// {
//  IDirectDrawSurface7 * pdds4;
//  HDC hDC;
//  WORD * wp;
// 
// 	m_bOnCriticalSection = TRUE;
// 
// 	if( m_stBrush == NULL ) return NULL;
// 
// 	CMyDib mydib(m_cPakFileName, m_dwBitmapFileStartLoc);
// 	m_wBitmapSizeX = mydib.m_wWidthX;
// 	m_wBitmapSizeY = mydib.m_wWidthY;
// 	pdds4 = m_pDDraw->pCreateOffScreenSurface(m_wBitmapSizeX, m_wBitmapSizeY);
//     if (pdds4 == NULL) return NULL; 
// 	pdds4->GetDC(&hDC);
// 	mydib.PaintImage(hDC);
// 	pdds4->ReleaseDC(hDC);
// 
// 	DDSURFACEDESC2  ddsd;
// 	ddsd.dwSize = 124;
// 	if (pdds4->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK) return NULL;
// 	pdds4->Unlock(NULL);
// 
// 	wp = (WORD *)ddsd.lpSurface;
// 	m_wColorKey = *wp;
// 
// 	m_bOnCriticalSection = FALSE;
// 
//     return pdds4;
// }
video::ITexture * CSprite::_pMakeSpriteSurface()
{
	if( m_stBrush == NULL ) return NULL;
	if (!m_bIsSurfaceEmpty) return NULL;

	CMyDib mydib(m_cPakFileName, m_dwBitmapFileStartLoc);

	if (!mydib.success)
	{
		//failed to load pak
		return 0;
	}
	m_wBitmapSizeX = mydib.m_wWidthX;
	m_wBitmapSizeY = mydib.m_wWidthY;

	//ECF_A8R8G8B8 - desired
	//ECF_R5G6B5 - most hb paks are
	core::dimension2d<u32> size;
	std::wstringstream ss;
	ss << m_cPakFileName << wPageid;
	io::IReadFile * bmpfile = G_pGame->device->getFileSystem()->createMemoryReadFile(mydib.m_lpDib, mydib.fh.bfSize, ss.str().c_str(), false);
	_localimage = G_pGame->driver->getTexture(bmpfile);
	G_pGame->driver->makeColorKeyTexture(_localimage, core::position2d<s32>(0,0));
	bmpfile->drop();
	size = _localimage->getSize();

	m_bIsSurfaceEmpty = FALSE;

	//CreateShadow();

	return _localimage;
}
CSprite * CSprite::CreateSprite(wchar_t * cPakFileName, short sNthFile, bool bAlphaEffect)
{
	std::wstringstream ss;
	ss << L"sprites\\" << cPakFileName << L".pak";
	std::wstring str = ss.str();
	std::ifstream szfile(str.c_str(),std::ios::in|std::ios::binary);

	if (!szfile.is_open())
	{
		//error
		//MessageBoxW(NULL, (L"Error loading pak: " + str).c_str(), L"ERROR", MB_OK);
	}

	return new CSprite(szfile,std::wstring(cPakFileName), sNthFile, bAlphaEffect);
}
// video::ITexture * Sprite::CreateTexture()
// {
// 	return _pMakeSpriteSurface();
// }
void CSprite::DrawShadow(int sX, int sY, int sFrame, uint64_t dwTime, video::SColor color)
{
	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
	G_pGame->driver->draw2DImage(_localshadow, core::position2d<s32>(sX+m_stBrush[sFrame].pvx,sY+m_stBrush[sFrame].pvy),
		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+m_stBrush[sFrame].szx,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
		color, true);
}
void CSprite::DrawSubSprite(int sX, int sY, int sFrame, uint64_t dwTime, video::SColor color)
{
	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
	G_pGame->driver->draw2DImage(subtextures[sFrame], core::position2d<s32>(sX,sY),
		core::rect<s32>(0,0,m_stBrush[sFrame].szx,m_stBrush[sFrame].szy), 0,
		video::SColor(255,255,255,255), true);
}
void CSprite::DrawSpriteNCK(int sX, int sY, int sFrame, uint64_t dwTime, video::SColor color)
{
	DrawRGBNCK(sX, sY, sFrame, dwTime, color);
}
// void CSprite::DrawHBColor(int sX, int sY, int sFrame, uint64_t dwTime, short sR, short sG, short sB)
// {
// 	//hb format 1 (default) R5G6B5toA8R8G8B8();
// 	//hb format 2 A1R5G5B5toA8R8G8B8();
// }
void CSprite::DrawRGBNCK(int sX, int sY, int sFrame, uint64_t dwTime, video::SColor color)
{
	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
	G_pGame->driver->draw2DImage(_localimage, core::position2d<s32>(sX+m_stBrush[sFrame].pvx,sY+m_stBrush[sFrame].pvy),
		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+m_stBrush[sFrame].szx,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
		color, false);


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
void CSprite::DrawSprite(int sX, int sY, int sFrame, uint64_t dwTime, video::SColor color)
{
	DrawRGB(sX, sY, sFrame, dwTime, color);
}
void CSprite::DrawRGB(int sX, int sY, int sFrame, uint64_t dwTime, video::SColor color)
{
	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
	G_pGame->driver->draw2DImage(_localimage, core::position2d<s32>(sX+m_stBrush[sFrame].pvx,sY+m_stBrush[sFrame].pvy),
		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+m_stBrush[sFrame].szx,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
		color, true);

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
void CSprite::DrawWidth(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime, video::SColor color)
{
	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
	G_pGame->driver->draw2DImage(_localimage, core::position2d<s32>(sX,sY),
		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+sWidth,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
		color, true);
}


// Crash is because it tries to read non-existing sprite. but what sprite?;O Well let's use trace and 1337 skills to work it out :P 1 sec just need to do some business with my friend
// he is leaving in a minute.
// aaah.. imma go peeeeee then :D
void CSprite::PutSpriteFast(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
	RECT rcRect;
	if( this == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if( m_stBrush == NULL ) return;
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left)
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top)
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE)
	{
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	m_pDDraw->m_lpBackB4->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutSpriteFastDst(LPDIRECTDRAWSURFACE7 lpDstS, int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left)
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE)
	{	if( _iOpenSprite() == FALSE ) return;
	}else // AlphaDegree
	{	if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) 
			{	_SetAlphaDegree();
			}else 
			{	_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
	}	}	}

	//SetRect(&rcRect,  sx, sy, sx + szx, sy + szy); // our fictitious sprite bitmap is 
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;
	lpDstS->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
	m_bOnCriticalSection = FALSE;
}


void CSprite::PutSpriteFastNoColorKey(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;
	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;
  	dX = sX + pvx;
	dY = sY + pvy;
	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{	sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) 
		{	m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{	szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) 
		{	m_rcBound.top = -1;
			return;
	}	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{	sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) 
		{	m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{	szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) 
		{	m_rcBound.top = -1;
			return;
	}	}
	
	m_dwRefTime = dwTime;
	if (m_bIsSurfaceEmpty == TRUE)
	{	if( _iOpenSprite() == FALSE ) return;
	}else 
	{	if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) 
			{	_SetAlphaDegree();
			}else 
			{	_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
	}	}	}

	//SetRect(&rcRect,  sx, sy, sx + szx, sy + szy); // our fictitious sprite bitmap is 
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	m_pDDraw->m_lpBackB4->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutSpriteFastNoColorKeyDst(LPDIRECTDRAWSURFACE7 lpDstS, int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSpriteNCK(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) 
	{	if( _iOpenSprite() == FALSE ) return;
	}else 
	{	if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) 
			{	_SetAlphaDegree();
			}else 
			{	_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
	}	}	}

	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	lpDstS->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutSpriteFastFrontBuffer(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&rcRect,  sx, sy, sx + szx, sy + szy); // our fictitious sprite bitmap is 
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	m_pDDraw->m_lpFrontB4->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutSpriteFastWidth(int sX, int sY, int sFrame, int sWidth, DWORD dwTime)
{
	DrawWidth(sX, sY, sFrame, sWidth, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (sWidth < szx)
		szx = sWidth;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&rcRect,  sx, sy, sx + szx, sy + szy); // our fictitious sprite bitmap is 
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	m_pDDraw->m_lpBackB4->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}


void CSprite::iRestore()
{
/*
 HDC     hDC;
	
	if (m_bIsSurfaceEmpty) return;
	if( m_stBrush == NULL ) return;
	if (m_lpSurface->IsLost() == DD_OK) return;

	m_lpSurface->Restore();
	DDSURFACEDESC2  ddsd;
	ddsd.dwSize = 124;
	if (m_lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK) return;
	m_pSurfaceAddr = (WORD *)ddsd.lpSurface;
	m_lpSurface->Unlock(NULL);
	CMyDib mydib(m_cPakFileName, m_dwBitmapFileStartLoc);
	m_lpSurface->GetDC(&hDC);
	mydib.PaintImage(hDC);
	m_lpSurface->ReleaseDC(hDC);*/
}

void CSprite::PutShadowSprite(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short sx,sy,szx,szy,pvx,pvy;
	int  ix, iy;
	WORD * pSrc, * pDst;

	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}

	int iSangX, iSangY;
	pSrc = (WORD *)m_pSurfaceAddr + sx + sy*m_sPitch;
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr;// + dX + ((dY+szy-1)*m_pDDraw->m_sBackB4Pitch);

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		for( iy=0 ; iy<szy ; iy+= 3 )
		{
			for( ix=0 ; ix<szx ; ix++ )
			{
				iSangX = (sX+pvx)+ix+(iy-szy)/3;
				iSangY = (sY+pvy)+(iy+szy+szy)/3;
				if (pSrc[ix] != m_wColorKey)
				{
					//if( iSangX >= 0 && iSangX < 640 && iSangY >= 0 && iSangY < 427 )
					if( iSangX >= 0 && iSangX < 800 && iSangY >= 0 && iSangY < 547 ) // 800x600 Resolution xRisenx Right?
					{
						pDst[iSangY*m_pDDraw->m_sBackB4Pitch+iSangX] = ((pDst[iSangY*m_pDDraw->m_sBackB4Pitch+iSangX] & 0xE79C) >> 2);
					}
				}
			}
			pSrc += m_sPitch + m_sPitch + m_sPitch;
		}
		break;
	case 2:
		for( iy=0 ; iy<szy ; iy+= 3 )
		{
			for( ix=0 ; ix<szx ; ix++ )
			{
				iSangX = sX+pvx+ix+(iy-szy)/3;
				iSangY = sY+pvy+(iy+szy+szy)/3;
				if (pSrc[ix] != m_wColorKey)
				{
					//if( iSangX >= 0 && iSangX < 640 && iSangY >= 0 && iSangY < 427 )
					if( iSangX >= 0 && iSangX < 800 && iSangY >= 0 && iSangY < 547 ) // 800x600 Resolution xRisenx
					{
						pDst[iSangY*m_pDDraw->m_sBackB4Pitch+iSangX] = ((pDst[iSangY*m_pDDraw->m_sBackB4Pitch+iSangX] & 0x739C) >> 2);
					}
				}
			}
			pSrc += m_sPitch + m_sPitch + m_sPitch;
		}
		break;
	}
	m_bOnCriticalSection = FALSE;
}


void CSprite::PutShadowSpriteClip(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
	int  ix, iy;
	WORD * pSrc, * pDst;

	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;
	
	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;
	
	if (dX < m_pDDraw->m_rcClipArea.left)
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy+szy-1)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY+szy-1)*m_pDDraw->m_sBackB4Pitch);

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		for (iy = 0; iy < szy; iy += 3) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey)
					if ( (dX - (iy/3) + ix)	> 0 )
						pDst[ix] = (pDst[ix] & 0xE79C) >> 2; 
			}
			pSrc -= m_sPitch + m_sPitch + m_sPitch;
			pDst -= m_pDDraw->m_sBackB4Pitch + 1;
		}
		break;

	case 2:
		for (iy = 0; iy < szy; iy += 3) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey)
					if ( (dX - (iy/3) + ix)	> 0 )
						pDst[ix] = (pDst[ix] & 0x739C) >> 2;
			}
			pSrc -= m_sPitch + m_sPitch + m_sPitch;
			pDst -= m_pDDraw->m_sBackB4Pitch + 1;
		}
		break;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutTransSprite(int sX, int sY, int sFrame, DWORD dwTime, int alphaDepth)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(alphaDepth,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB100[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG100[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB100[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG100[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSprite_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime, int alphaDepth)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(alphaDepth,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB100[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG100[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB100[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG100[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSprite70(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(180,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB70[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG70[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB70[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB70[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG70[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB70[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutTransSprite70_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(180,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB70[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG70[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB70[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB70[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG70[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB70[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSprite50(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(125,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB50[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG50[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB50[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB50[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG50[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB50[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutTransSprite50_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(125,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB50[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG50[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB50[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB50[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG50[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB50[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSprite25(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(64,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB25[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11] <<11) | (G_lTransG25[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5] <<5) | G_lTransRB25[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB25[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG25[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB25[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutTransSprite25_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(64,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB25[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11] <<11) | (G_lTransG25[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5] <<5) | G_lTransRB25[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB25[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG25[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB25[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutTransSprite2(int sX, int sY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(180,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB2[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG2[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB2[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB2[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG2[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB2[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutShiftTransSprite2(int sX, int sY, int shX, int shY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = 128;//m_stBrush[sFrame].szx;
	szy = 128;//m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	sx += shX;
	sy += shY;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB2[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG2[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB2[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB2[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG2[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB2[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutFadeSprite(short sX, short sY, short sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 //int dX,dY,sx,sy,szx,szy,pvx,pvy,sTmp;

	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		for (iy = 0; iy < szy; iy++) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey) 
					pDst[ix] = ((pDst[ix] & 0xE79C) >> 2); 
				
			}
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
		}
		break;

	case 2:
		for (iy = 0; iy < szy; iy++) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey)	
					pDst[ix] = ((pDst[ix] & 0x739C) >> 2);
				
			}
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
		}
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutFadeSpriteDst(WORD * pDstAddr, short sPitch, short sX, short sY, short sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 //int           iRet, dX,dY,sx,sy,szx,szy,pvx,pvy,sTmp;

	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)pDstAddr + dX + ((dY)*sPitch);

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		for (iy = 0; iy < szy; iy++) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey) 
					pDst[ix] = ((pDst[ix] & 0xE79C) >> 2); 
				
			}
			pSrc += m_sPitch;
			pDst += sPitch;
		}
		break;

	case 2:
		for (iy = 0; iy < szy; iy++) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey)	
					pDst[ix] = ((pDst[ix] & 0x739C) >> 2);
				
			}
			pSrc += m_sPitch;
			pDst += sPitch;
		}
		break;
	}

	m_bOnCriticalSection = FALSE;
}


// bool CSprite::_iOpenSprite()
// {
//   	if (m_lpSurface != NULL) return FALSE;
// 	m_lpSurface = _pMakeSpriteSurface(); 
// 	if (m_lpSurface == NULL) return FALSE;
// 	m_pDDraw->iSetColorKey(m_lpSurface, m_wColorKey);
// 	m_bIsSurfaceEmpty  = FALSE;
// 	DDSURFACEDESC2  ddsd;
// 	ddsd.dwSize = 124;
// 	if (m_lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK) return FALSE;	
// 	m_pSurfaceAddr = (WORD *)ddsd.lpSurface;
// 	m_sPitch = (short)ddsd.lPitch >> 1;	
// 	m_lpSurface->Unlock(NULL);
// 	_SetAlphaDegree();
// //	m_dwRefTime = timeGetTime();
// 	return TRUE;
// }
// 
// void CSprite::_iCloseSprite()
// {
// 	if( m_stBrush == NULL ) return;
// 	if (m_lpSurface == NULL) return;
// 	if (m_lpSurface->IsLost() != DD_OK)	return;
// 	m_lpSurface->Release();
// 	m_lpSurface = NULL;
// 	m_bIsSurfaceEmpty = TRUE;
// 	m_cAlphaDegree = 1;
// }

void CSprite::PutSpriteRGB(int sX, int sY, int sFrame, uint32_t color, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(color));
}

void CSprite::PutSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,sRed,sGreen,sBlue));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy, iRedPlus255, iGreenPlus255, iBluePlus255;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	if ((szx == 0) || (szy == 0)) return;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	iRedPlus255   = sRed +255;
	iGreenPlus255 = sGreen +255;
	iBluePlus255  = sBlue +255;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)(( G_iAddTable31[(pSrc[ix]&0xF800)>>11][iRedPlus255] <<11) | ( G_iAddTable63[(pSrc[ix]&0x7E0)>>5][iGreenPlus255] <<5) | G_iAddTable31[(pSrc[ix]&0x1F)][iBluePlus255]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_iAddTable31[(pSrc[ix]&0x7C00)>>10][iRedPlus255]<<10) | (G_iAddTable31[(pSrc[ix]&0x3E0)>>5][iGreenPlus255]<<5) | G_iAddTable31[(pSrc[ix]&0x1F)][iBluePlus255]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSpriteRGB(int sX, int sY, int sFrame, uint32_t color, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(color));
}

void CSprite::PutTransSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(180,sRed,sGreen,sBlue));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 short ix, iy, iRedPlus255, iGreenPlus255, iBluePlus255;
 WORD  * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	if ((szx == 0) || (szy == 0)) return;

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	iRedPlus255   = sRed +255;
	iGreenPlus255 = sGreen +255;
	iBluePlus255  = sBlue +255;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0xF800)>>11][((pSrc[ix]&0xF800)>>11)] + iRedPlus255][(pDst[ix]&0xF800)>>11]<<11) | (G_iAddTransTable63[G_lTransG100[(pDst[ix]&0x7E0)>>5][((pSrc[ix]&0x7E0)>>5)] + iGreenPlus255][(pDst[ix]&0x7E0)>>5]<<5) | G_iAddTransTable31[m_pDDraw->m_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)] + iBluePlus255][(pDst[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10] +iRedPlus255][(pDst[ix]&0x7C00)>>10]<<10) | (G_iAddTransTable31[G_lTransG100[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5] +iGreenPlus255][(pDst[ix]&0x3E0)>>5]<<5) | G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)] +iBluePlus255][(pDst[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSpriteRGB_NoColorKey(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(180,sRed,sGreen,sBlue));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 short ix, iy, iRedPlus255, iGreenPlus255, iBluePlus255;
 WORD  * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	iRedPlus255   = sRed +255;
	iGreenPlus255 = sGreen +255;
	iBluePlus255  = sBlue +255;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0xF800)>>11][((pSrc[ix]&0xF800)>>11)] + iRedPlus255][(pDst[ix]&0xF800)>>11]<<11) | (G_iAddTransTable63[G_lTransG100[(pDst[ix]&0x7E0)>>5][((pSrc[ix]&0x7E0)>>5)] + iGreenPlus255][(pDst[ix]&0x7E0)>>5]<<5) | G_iAddTransTable31[m_pDDraw->m_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)] + iBluePlus255][(pDst[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10] +iRedPlus255][(pDst[ix]&0x7C00)>>10]<<10) | (G_iAddTransTable31[G_lTransG100[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5] +iGreenPlus255][(pDst[ix]&0x3E0)>>5]<<5) | G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)] +iBluePlus255][(pDst[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::_GetSpriteRect(int sX, int sY, int sFrame)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
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
	
	SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_sPivotX = pvx;
	m_sPivotY = pvy;
}

void CSprite::_SetAlphaDegree()
{
	return;
	WORD * pSrc, wR, wG, wB, wTemp, ix, iy;
 int iR, iG, iB, sRed, sGreen, sBlue;
 
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_bOnCriticalSection = TRUE;
	if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
		
		m_cAlphaDegree = G_cSpriteAlphaDegree;
		switch (m_cAlphaDegree) {
		case 1:
			sRed = sGreen = sBlue = 0;
			break;

		case 2:
			sRed   = -3;
			sGreen = -3;
			sBlue  =  2;
			break;
		}

		pSrc = (WORD *)m_pSurfaceAddr;
		
		switch (m_pDDraw->m_cPixelFormat) {
		case 1:
			for (iy = 0; iy < m_wBitmapSizeY; iy++) 
			{	for (ix = 0; ix < m_wBitmapSizeX; ix++) 
				{	if (pSrc == NULL) return; 
					if (pSrc[ix] != m_wColorKey) 
					{	wR = (WORD)(pSrc[ix]&0xF800)>>11;
						wG = (WORD)(pSrc[ix]&0x7E0)>>5;
						wB = (WORD)(pSrc[ix]&0x1F);
						iR = (int)wR + sRed;
						iG = (int)wG + sGreen;
						iB = (int)wB + sBlue;
						
						if (iR < 0) iR = 0;
						else if (iR > 31) iR = 31;
						if (iG < 0) iG = 0;
						else if (iG > 63) iG = 63;
						if (iB < 0) iB = 0;
						else if (iB > 31) iB = 31;
						
						wTemp = (WORD)((iR<<11) | (iG<<5) | iB);
						if (wTemp != m_wColorKey)
							 pSrc[ix] = wTemp;
						else pSrc[ix] = (WORD)((iR<<11) | (iG<<5) | (iB+1));
				}	}
				pSrc += m_sPitch;
			}
			break;
			
		case 2:
			for (iy = 0; iy < m_wBitmapSizeY; iy++) 
			{	for (ix = 0; ix < m_wBitmapSizeX; ix++) 
				{	if (pSrc == NULL) return; 
					if (pSrc[ix] != m_wColorKey)	
					{	wR = (WORD)(pSrc[ix]&0x7C00)>>10;
						wG = (WORD)(pSrc[ix]&0x3E0)>>5;
						wB = (WORD)(pSrc[ix]&0x1F);
						iR = (int)wR + sRed;
						iG = (int)wG + sGreen;
						iB = (int)wB + sBlue;						
						if (iR < 0) iR = 0;
						else if (iR > 31) iR = 31;
						if (iG < 0) iG = 0;
						else if (iG > 31) iG = 31;
						if (iB < 0) iB = 0;
						else if (iB > 31) iB = 31;						
						wTemp = (WORD)((iR<<10) | (iG<<5) | iB);
						if (wTemp != m_wColorKey)
							 pSrc[ix] = wTemp;
						else pSrc[ix] = (WORD)((iR<<10) | (iG<<5) | (iB+1));				
				}	}
				pSrc += m_sPitch;
			}
			break;	
	}	}

	m_bOnCriticalSection = FALSE;
}

BOOL CSprite::_bCheckCollison(int sX, int sY, short sFrame, int msX, int msY)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
	int  ix, iy;
	WORD * pSrc;
	int  tX, tY;
	
	if( this == NULL ) return FALSE;
	if( m_stBrush == NULL ) return FALSE;
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return FALSE;
	if( m_bIsSurfaceEmpty == TRUE ) return FALSE;
	if( msX < 0+3 ) return FALSE;
	if( msX > G_pGame->GetWidth()-3 ) return FALSE;
	if( msY < 0+3 ) return FALSE;
	if( msY > G_pGame->GetHeight()-3 ) return FALSE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if( msX < dX ) return FALSE;
	if( msX > dX+szx ) return FALSE;
	if( msY < dY ) return FALSE;
	if( msY > dY+szy ) return FALSE;

//	if (dX < m_pDDraw->m_rcClipArea.left+3) return FALSE;
//	if (dX+szx > m_pDDraw->m_rcClipArea.right-3) return FALSE;
//	if (dY < m_pDDraw->m_rcClipArea.top+3) return FALSE;
//	if (dY+szy > m_pDDraw->m_rcClipArea.bottom-3) return FALSE;

	if (dX < 0+3)
	{
		sx = sx	+ (0+3 - dX);
		szx = szx - (0+3 - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return FALSE;
		}
		dX = (short)0+3;
	}
	else if (dX+szx > G_pGame->GetWidth()-3)
	{
		szx = szx - ((dX+szx) - (short)G_pGame->GetWidth()-3);
		if (szx < 0) {
			m_rcBound.top = -1;
			return FALSE;
		}
	}

	if (dY < 0+3)
	{
		sy = sy	+ (0+3 - dY);
		szy = szy - (0+3 - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return FALSE;
		}
		dY = (short)0+3;
	}
	else if (dY+szy > G_pGame->GetHeight()-3)
	{
		szy = szy - ((dY+szy) - (short)G_pGame->GetHeight()-3);
		if (szy < 0) {
			m_rcBound.top = -1;
			return FALSE;
		}
	}
	
	SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	tX = dX;
	tY = dY;


//	pSrc += m_sPitch * ( msY - tY );
//	if( pSrc[msX-tX] != m_wColorKey ) return TRUE;
//	else return FALSE;

	if( msY < tY + 3 ) return FALSE;
	if( msX < tX + 3 ) return FALSE;
	ECOLOR_FORMAT fmt = _localimage->getColorFormat();
	return TRUE;
	// 
	if (fmt == ECF_A8R8G8B8)
	{
		//else fails
		void * data = _localimage->lock();
		uint32_t * pixel = (uint32_t*)data;
		
		_localimage->unlock();
	}
	return FALSE;
	//pSrc = (WORD*)data;
	pSrc += m_sPitch * ( msY - tY - 3 );
	for( iy=0 ; iy<=6 ; iy++ )
	{
		for( ix=msX-tX-3 ; ix<=msX-tX+3 ; ix++ )
		{
			if( pSrc[ix] != m_wColorKey )
			{
				return TRUE;
			}
		}
		pSrc += m_sPitch;
	}
	return FALSE;
}

void CSprite::PutShiftSpriteFast(int sX, int sY, int shX, int shY, int sFrame, DWORD dwTime)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = 128;//m_stBrush[sFrame].szx;
	szy = 128;//m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	sx += shX;
	sy += shY;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&rcRect,  sx, sy, sx + szx, sy + szy); // our fictitious sprite bitmap is 
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	m_pDDraw->m_lpBackB4->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutRevTransSprite(int sX, int sY, int sFrame, DWORD dwTime, int alphaDepth)
{
	DrawSprite(sX, sY, sFrame, dwTime, irr::video::SColor(255,255,255,255));
	return;
	int  ix, iy;
	int  iR, iG, iB;
	WORD * pSrc, * pDst;
	int dX,dY,sx,sy,szx,szy,pvx,pvy;//,sTmp;
	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;
	
	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;
	
	dX = sX + pvx;
	dY = sY + pvy;
		
	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
		
	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
		
	m_dwRefTime = dwTime;
		
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}
	
	SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
		
	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);
	
	if ((szx == 0) || (szy == 0)) return;
		
	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					iR = (int)m_pDDraw->m_lFadeRB[((pDst[ix]&0xF800)>>11)][((pSrc[ix]&0xF800)>>11) +alphaDepth];
					iG = (int)m_pDDraw->m_lFadeG[(pDst[ix]&0x7E0)>>5][((pSrc[ix]&0x7E0)>>5) +alphaDepth +alphaDepth];
					iB = (int)m_pDDraw->m_lFadeRB[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F) +alphaDepth];
					pDst[ix] = (WORD)((iR<<11) | (iG<<5) | iB);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
		
	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					iR = (int)m_pDDraw->m_lFadeRB[(pDst[ix]&0x7C00)>>10][((pSrc[ix]&0x7C00)>>10) +alphaDepth];
					iG = (int)m_pDDraw->m_lFadeG[(pDst[ix]&0x3E0)>>5][((pSrc[ix]&0x3E0)>>5) +alphaDepth];
					iB = (int)m_pDDraw->m_lFadeRB[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F) +alphaDepth];
					pDst[ix] = (WORD)((iR<<10) | (iG<<5) | iB);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}
	m_bOnCriticalSection = FALSE;
}

void ReadFramePositions(HANDLE hPakFile, std::vector<int> & framePositions, int frames)
{
	return;
	DWORD * dwp, count;
	char * fileHeader = new char[frames*8 +8];
	SetFilePointer(hPakFile, 24, NULL, FILE_BEGIN);
	ReadFile(hPakFile, fileHeader,  frames *8, &count, NULL);
	dwp = (DWORD *) fileHeader;
	for(int i = 0; i < frames; i++, dwp+=2)
	{
		framePositions.push_back(*dwp);
	}
	delete [] fileHeader;
}

