//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <cstdio>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <cstdint>
#include "common.h"
#include "global_def.h"

using sf::Color;

typedef struct stBrushtag
{
    short sx;
    short sy;
    short szx;
    short szy;
    short pvx;
    short pvy;
} stBrush;

class sprite
{
public:
    bool _pMakeSpriteSurface();
    sf::Texture _localimage;
    //sf::Texture * _localshadow;
    //sf::Texture ** subtextures;

    sprite(std::ifstream & hPakFile, std::string & cPakFileName, short sNthFile, bool bAlphaEffect = true);
    virtual ~sprite();
    static sprite * CreateSprite(std::string cPakFileName, short sNthFile, bool bAlphaEffect = true);
    void DrawSubSprite(int sX, int sY, int sFrame, uint64_t dwTime = 0, Color color = Color(255, 255, 255, 255));
    void DrawSpriteNCK(int sX, int sY, int sFrame, uint64_t dwTime = 0, Color color = Color(255, 255, 255, 255));
    void DrawRGBNCK(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255, 255));
    void DrawSprite(int sX, int sY, int sFrame, uint64_t dwTime = 0, Color color = Color(255, 255, 255, 255));
    void DrawScaledSprite(int sX, int sY, int sFrame, int sWidth, int sHeight, uint64_t dwTime = 0, Color color = Color(255, 255, 255, 255));
    void DrawRGB(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255, 255));
    void draw_to(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255, 255), int draw_mode = DS_VISIBLE);
    void DrawWidth(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime, Color color = Color(255, 255, 255, 255));
    void DrawShadow(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255, 255));
    void CreateShadow();

    void PutSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime);

    void PutSpriteFast(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutShiftSpriteFast(int sX, int sY, int shX, int shY, int sFrame, uint64_t dwTime);
    void PutShiftTransSprite2(int sX, int sY, int shX, int shY, int sFrame, uint64_t dwTime);
    void PutSpriteFastFrontBuffer(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutSpriteFastWidth(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime);
    void PutSpriteFastNoColorKey(int sX, int sY, int sFrame, uint64_t dwTime);

    void PutTransSprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth = 30);
    void PutTransSprite2(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth = 0);
    void PutTransSpriteRGB_NoColorKey(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime);
    void PutTransSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime);
    void PutTransSprite70(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite50(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite25(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite70_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite50_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite25_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime);

    void PutTransSpriteRGB(int sX, int sY, int sFrame, uint32_t color, uint64_t dwTime);
    void PutSpriteRGB(int sX, int sY, int sFrame, uint32_t color, uint64_t dwTime);

    void PutShadowSpriteClip(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutShadowSprite(int sX, int sY, int sFrame, uint64_t dwTime);

    void PutRevTransSprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth = 0);

    void PutFadeSprite(short sX, short sY, short sFrame, uint64_t dwTime);
    void PutFadeSpriteDst(uint16_t * pDstAddr, short sPitch, short sX, short sY, short sFrame, uint64_t dwTime);

    bool _bCheckCollison(int sX, int sY, short sFrame, int msX, int msY);
    void _GetSpriteRect(int sX, int sY, int sFrame);
    bool _iOpenSprite() { return _pMakeSpriteSurface(); }
    void _iCloseSprite() { /*OutputDebugStringW(("Unloaded image: " + m_cPakFileName + "\n").c_str()); if (_localimage) _localimage->drop(); m_bIsSurfaceEmpty = TRUE;*/ }

    RECT m_rcBound;
    uint64_t m_dwRefTime;
    bool m_bIsSurfaceEmpty;
    bool m_bOnCriticalSection;
    bool m_bAlphaEffect;
    short m_sPivotX, m_sPivotY;
    uint16_t * m_pSurfaceAddr;
    uint32_t m_dwBitmapFileStartLoc;
    short m_sPitch;
    uint64_t m_iTotalFrame;
    char m_cAlphaDegree;
    uint16_t m_wBitmapSizeX, m_wBitmapSizeY;
    uint16_t m_wColorKey;
    std::string m_cPakFileName;
    stBrush * brush;
    uint64_t wPageid;

private:
    sf::Sprite * sprite_;
};
