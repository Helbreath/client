// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__0089D9E2_74E6_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_SPRITE_H__0089D9E2_74E6_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <irrlicht.h>
#include <sstream>
#include <stdint.h>
#include "common.h"

using namespace irr;
using namespace video;

//#include "DXC_ddraw.h"
//#include "Mydib.h"

typedef struct stBrushtag
{
	short sx;
	short sy;
	short szx;
	short szy;
	short pvx;
	short pvy;
} stBrush;

class CSprite  
{
public:
	video::ITexture * _pMakeSpriteSurface();
	video::ITexture * _localimage;
	video::ITexture * _localshadow;
	video::ITexture ** subtextures;


	CSprite(std::ifstream & hPakFile, std::wstring & cPakFileName, short sNthFile, bool bAlphaEffect = true);
	//CSprite(HANDLE hPakFile, class DXC_ddraw * pDDraw, char * cPakFileName, short sNthFile, bool bAlphaEffect = TRUE, std::vector<int> * framePositions = NULL);
	virtual ~CSprite();
	static CSprite * CreateSprite(wchar_t * cPakFileName, short sNthFile, bool bAlphaEffect = true);
	void DrawSubSprite(int sX, int sY, int sFrame, uint64_t dwTime = 0, video::SColor color = video::SColor(255,255,255,255));
	void DrawSpriteNCK(int sX, int sY, int sFrame, uint64_t dwTime = 0, video::SColor color = video::SColor(255,255,255,255));
	void DrawRGBNCK(int sX, int sY, int sFrame, uint64_t dwTime, video::SColor color = video::SColor(255,255,255,255));
	void DrawSprite(int sX, int sY, int sFrame, uint64_t dwTime = 0, video::SColor color = video::SColor(255,255,255,255));
    void DrawScaledSprite(int sX, int sY, int sFrame, int sWidth, int sHeight, uint64_t dwTime = 0, video::SColor color = video::SColor(255, 255, 255, 255));
	void DrawRGB(int sX, int sY, int sFrame, uint64_t dwTime, video::SColor color = video::SColor(255,255,255,255));
	void DrawWidth(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime, video::SColor color = video::SColor(255,255,255,255));
	void DrawShadow(int sX, int sY, int sFrame, uint64_t dwTime, video::SColor color = video::SColor(255,255,255,255));
	void CreateShadow();


	void PutSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint32_t dwTime);

	void PutSpriteFast(int sX, int sY, int sFrame, uint32_t dwTime);
	void PutShiftSpriteFast(int sX, int sY, int shX, int shY, int sFrame, uint32_t dwTime);
	void PutShiftTransSprite2(int sX, int sY, int shX, int shY, int sFrame, uint32_t dwTime);
	void PutSpriteFastFrontBuffer(int sX, int sY, int sFrame, uint32_t dwTime);
	void PutSpriteFastWidth(int sX, int sY, int sFrame, int sWidth, uint32_t dwTime);
	void PutSpriteFastNoColorKey(int sX, int sY, int sFrame, uint32_t dwTime);

	void PutTransSprite(int sX, int sY, int sFrame, uint32_t dwTime, int alphaDepth = 30);
	void PutTransSprite2(int sX, int sY, int sFrame, uint32_t dwTime);
	void PutTransSprite_NoColorKey(int sX, int sY, int sFrame, uint32_t dwTime, int alphaDepth = 0);
	void PutTransSpriteRGB_NoColorKey(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint32_t dwTime);
	void PutTransSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint32_t dwTime);
	void PutTransSprite70(int sX, int sY, int sFrame, uint32_t dwTime);
	void PutTransSprite50(int sX, int sY, int sFrame, uint32_t dwTime);
	void PutTransSprite25(int sX, int sY, int sFrame, uint32_t dwTime);
	void PutTransSprite70_NoColorKey(int sX, int sY, int sFrame, uint32_t dwTime);
	void PutTransSprite50_NoColorKey(int sX, int sY, int sFrame, uint32_t dwTime);
	void PutTransSprite25_NoColorKey(int sX, int sY, int sFrame, uint32_t dwTime);

	void PutTransSpriteRGB(int sX, int sY, int sFrame, uint32_t color, uint32_t dwTime);
	void PutSpriteRGB(int sX, int sY, int sFrame, uint32_t color, uint32_t dwTime);

	void PutShadowSpriteClip(int sX, int sY, int sFrame, uint32_t dwTime);
	void PutShadowSprite(int sX, int sY, int sFrame, uint32_t dwTime);

	void PutRevTransSprite(int sX, int sY, int sFrame, uint32_t dwTime, int alphaDepth = 0);

	void PutFadeSprite(short sX, short sY, short sFrame, uint32_t dwTime);
	void PutFadeSpriteDst(uint16_t * pDstAddr, short sPitch, short sX, short sY, short sFrame, uint32_t dwTime);


	bool _bCheckCollison(int sX, int sY, short sFrame, int msX, int msY);
	void _GetSpriteRect(int sX, int sY, int sFrame);
	bool _iOpenSprite() { return _pMakeSpriteSurface() != 0; }
	void _iCloseSprite() { /*OutputDebugStringW((L"Unloaded image: " + m_cPakFileName + L"\n").c_str()); if (_localimage) _localimage->drop(); m_bIsSurfaceEmpty = TRUE;*/ }

	RECT	m_rcBound;
	uint32_t	m_dwRefTime;
	bool	m_bIsSurfaceEmpty;
	bool	m_bOnCriticalSection;
	bool	m_bAlphaEffect;
	short	m_sPivotX, m_sPivotY;
	uint16_t * m_pSurfaceAddr;
	uint32_t	m_dwBitmapFileStartLoc;
	short	m_sPitch;
	int		m_iTotalFrame;
	char	m_cAlphaDegree;
	uint16_t	m_wBitmapSizeX, m_wBitmapSizeY;
	uint16_t	m_wColorKey;
	std::wstring m_cPakFileName;
//	char	m_cPakFileName[16];
	stBrush* m_stBrush;
	uint16_t	wPageid;
};

#endif // !defined(AFX_SPRITE_H__0089D9E2_74E6_11D2_A8E6_00001C7030A6__INCLUDED_)
