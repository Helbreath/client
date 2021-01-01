// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "Sprite.h"
#include "Game.h"
#include <sodium.h>

extern char G_cSpriteAlphaDegree;
extern bool isrunning;

extern int G_iAddTable31[64][510], G_iAddTable63[64][510];
extern int G_iAddTransTable31[510][64], G_iAddTransTable63[510][64];

extern long G_lTransG100[64][64], G_lTransRB100[64][64];
extern long G_lTransG70[64][64], G_lTransRB70[64][64];
extern long G_lTransG50[64][64], G_lTransRB50[64][64];
extern long G_lTransG25[64][64], G_lTransRB25[64][64];
extern long G_lTransG2[64][64], G_lTransRB2[64][64];

extern CGame * G_pGame;

#ifndef WIN32
typedef struct tagBITMAPINFOHEADER
{
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER, * LPBITMAPINFOHEADER, * PBITMAPINFOHEADER;

typedef struct tagRGBQUAD
{
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
} BITMAPINFO, * LPBITMAPINFO, * PBITMAPINFO;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSprite::CSprite(std::ifstream & hPakFile, std::string & cPakFileName, short sNthFile, bool bAlphaEffect)
{
    //int iASDstart;

    m_stBrush = 0;
    m_bIsSurfaceEmpty = true;

    m_cAlphaDegree = 1;
    m_bOnCriticalSection = false;
    m_iTotalFrame = 0;

    sprite = 0;

    m_cPakFileName = cPakFileName;
    m_bAlphaEffect = bAlphaEffect;
    wPageid = sNthFile;

    //_pMakeSpriteSurface();
}

CSprite::~CSprite(void)
{
    if (m_stBrush != 0)
        delete[] m_stBrush;
    if (sprite)
        delete[] sprite;
}

bool CSprite::_pMakeSpriteSurface()
{
    if (/*(m_stBrush == 0) || */ (!m_bIsSurfaceEmpty) || !isrunning)
    {
        return false;
    }

    unsigned char * m_lpDib;
    std::stringstream ss;
    ss << "sprites\\" << m_cPakFileName << ".pak";
    std::ifstream szfile(ss.str().c_str(), std::ios::in | std::ios::binary);
    if (!szfile.is_open())
    {
        return false;
    }

    // replaces object constructor
    if (!sprite)
    {
        int iASDstart;
        szfile.seekg(24 + wPageid * 8, std::ios::beg);
        szfile.read((char *)&iASDstart, 4);
        szfile.seekg(iASDstart + 100, std::ios::beg);
        szfile.read((char *)&m_iTotalFrame, 4);

        //if (cPakFileName == "GameDialog") __debugbreak();

        m_dwBitmapFileStartLoc = iASDstart + (108 + (12 * m_iTotalFrame));
        m_stBrush = new stBrush[m_iTotalFrame];
        szfile.read((char *)m_stBrush, 12 * m_iTotalFrame);

        sprite = new Sprite[m_iTotalFrame];
    }
    //////////////////////////////////////////////////////////////////////////

    szfile.seekg(m_dwBitmapFileStartLoc, std::ios::beg);

    uint32_t filesize;
    uint32_t encryption;
    unsigned char header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    szfile.read((char *)&filesize, 4);
    m_lpDib = (unsigned char *)new unsigned char[filesize];
    szfile.read((char *)&encryption, 4);
    if (encryption == 0x3f8ebc3d)
        szfile.read((char *)header, 24);
    szfile.read((char *)m_lpDib, filesize);

    //check for invalid frames
    if (filesize > 100'000'000)
        __debugbreak();

    int a = szfile.gcount();
    szfile.close();
    if (encryption == 0x3f8ebc3d)
    {
        if (!G_pGame->has_key)
        {
            //error, has not received decryption key
            MessageBoxA(NULL, fmt::format("Cannot open {}", m_cPakFileName).c_str(), "Error", MB_OK);
            delete[] m_lpDib;
            isrunning = false;
            return false;
        }
        unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES] = { 0xf4, 0xb7, 0xdd, 0x28, 0xc5, 0xaf, 0xff, 0x3f, 0xdf, 0xd5, 0xa2, 0x9d, 0x68, 0x58, 0xde, 0x91, 0xd7, 0x4c, 0x4c, 0xf5, 0xbe, 0x74, 0xc3, 0xa7, 0xc9, 0xf3, 0x5, 0x81, 0x43, 0xee, 0x6f, 0x71 };
        unsigned char * decrypted = new unsigned char[filesize * 1.5];
        unsigned long long decrypted_len;

        if (crypto_aead_xchacha20poly1305_ietf_decrypt(decrypted, &decrypted_len, NULL, m_lpDib, filesize, 0, 0, header, G_pGame->key) != -1)
        {
            delete[] m_lpDib;
            m_lpDib = new unsigned char[decrypted_len];
            memcpy(m_lpDib, decrypted, decrypted_len);
            delete[] decrypted;
        }
        else
        {
            // error decrypting
            delete[] decrypted;
        }
    }
    else if (encryption == 0xa8b73afe)
    {
        //do nothing
    }

    sf::MemoryInputStream is;
    is.open((char *)m_lpDib, filesize);

    if (!_localimage.loadFromStream(is))
    {
        std::cout << "Failed to load " << m_cPakFileName << "\n";
        delete[] m_lpDib;
        return false;
    }
    delete[] m_lpDib;

    for (int i = 0; i < m_iTotalFrame; ++i)
    {
        sprite[i].setTexture(_localimage);
        sprite[i].setTextureRect(IntRect(m_stBrush[i].sx, m_stBrush[i].sy, m_stBrush[i].szx, m_stBrush[i].szy));
    }

    m_bIsSurfaceEmpty = false;

    //CreateShadow();

    return true;
}

CSprite * CSprite::CreateSprite(std::string cPakFileName, short sNthFile, bool bAlphaEffect)
{
    std::ifstream szfile(fmt::format("sprites\\{}.pak", cPakFileName), std::ios::in | std::ios::binary);

    if (!szfile.is_open())
    {
        //error
        //MessageBoxW(NULL, ("Error loading pak: " + str).c_str(), "ERROR", MB_OK);
    }

    return new CSprite(szfile, cPakFileName, sNthFile, bAlphaEffect);
}

// class test : public sf::Sprite
// {
// public:
// 	void trythis()
// 	{
// 		this->
// 	}
// };

void CSprite::DrawShadow(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
    // 	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
    //
    // 	test newSprite;
    // 	newSprite.setTexture(*sprite[sFrame].getTexture());
    //
    // 	newSprite[sFrame].setColor(Color(0,0,0,127));
    // 	newSprite[sFrame].setPosition(sX + m_stBrush[sFrame].pvx, sY + m_stBrush[sFrame].pvy);
    // 	newSprite[sFrame].trythis();
    //     G_pGame->draw(newSprite[sFrame]);

    // 	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
    // 	G_pGame->driver->draw2DImage(_localshadow, core::position2d<s32>(sX+m_stBrush[sFrame].pvx,sY+m_stBrush[sFrame].pvy),
    // 		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+m_stBrush[sFrame].szx,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
    // 		color, true);
}

void CSprite::DrawSubSprite(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
    if (m_bIsSurfaceEmpty)
        if (_iOpenSprite() == false)
            return;
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
    if (m_bIsSurfaceEmpty)
        if (_iOpenSprite() == false)
            return;
    sprite[sFrame].setColor(color);
    sprite[sFrame].setPosition(sX + m_stBrush[sFrame].pvx, sY + m_stBrush[sFrame].pvy);
    G_pGame->draw(sprite[sFrame]);

    //     G_pGame->driver->draw2DImage(_localimage, core::position2d<s32>(sX+m_stBrush[sFrame].pvx,sY+m_stBrush[sFrame].pvy),
    // 		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+m_stBrush[sFrame].szx,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
    // 		color, false);

    short dX, dY, sx, sy, szx, szy, pvx, pvy;

    sx = m_stBrush[sFrame].sx;
    sy = m_stBrush[sFrame].sy;
    szx = m_stBrush[sFrame].szx;
    szy = m_stBrush[sFrame].szy;
    pvx = m_stBrush[sFrame].pvx;
    pvy = m_stBrush[sFrame].pvy;

    dX = sX + pvx;
    dY = sY + pvy;

    m_rcBound.left = dX;
    m_rcBound.top = dY;
    m_rcBound.right = dX + szx;
    m_rcBound.bottom = dY + szy;
}

void CSprite::DrawSprite(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
    DrawRGB(sX, sY, sFrame, dwTime, color);
}

void CSprite::DrawRGB(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
    if (m_bIsSurfaceEmpty)
        if (_iOpenSprite() == false)
            return;
    if (sFrame >= m_iTotalFrame)
    {
        std::cout << "Sprite out_of_bounds! - " << sFrame << " - " << m_cPakFileName << '\n';
        return;
    }
    sprite[sFrame].setColor(color);
    sprite[sFrame].setPosition(sX + m_stBrush[sFrame].pvx, sY + m_stBrush[sFrame].pvy);
    G_pGame->draw(sprite[sFrame]);
    // 	G_pGame->driver->draw2DImage(_localimage, core::position2d<s32>(sX+m_stBrush[sFrame].pvx,sY+m_stBrush[sFrame].pvy),
    // 		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+m_stBrush[sFrame].szx,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
    // 		color, true);

    short dX, dY, sx, sy, szx, szy, pvx, pvy;

    sx = m_stBrush[sFrame].sx;
    sy = m_stBrush[sFrame].sy;
    szx = m_stBrush[sFrame].szx;
    szy = m_stBrush[sFrame].szy;
    pvx = m_stBrush[sFrame].pvx;
    pvy = m_stBrush[sFrame].pvy;

    dX = sX + pvx;
    dY = sY + pvy;

    m_rcBound.left = dX;
    m_rcBound.top = dY;
    m_rcBound.right = dX + szx;
    m_rcBound.bottom = dY + szy;
}

void CSprite::draw_to(int sX, int sY, int sFrame, uint64_t dwTime, Color color, int draw_mode)
{
    if (m_bIsSurfaceEmpty)
        if (_iOpenSprite() == false)
            return;
    if (sFrame >= m_iTotalFrame || sFrame < 0)
    {
        std::cout << "Sprite out_of_bounds! - " << sFrame << " - " << m_cPakFileName << '\n';
        return;
    }
    sprite[sFrame].setColor(color);
    sprite[sFrame].setPosition(sX + m_stBrush[sFrame].pvx, sY + m_stBrush[sFrame].pvy);
    G_pGame->draw_to(sprite[sFrame], draw_mode);
}

void CSprite::DrawScaledSprite(int sX, int sY, int sFrame, int sWidth, int sHeight, uint64_t dwTime, Color color)
{
    if (m_bIsSurfaceEmpty)
        if (_iOpenSprite() == false)
            return;
    sf::FloatRect f = sprite[sFrame].getLocalBounds();
    sprite[sFrame].setScale((sWidth / f.width) * 100, (sHeight / f.height) * 100);
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
    if (m_bIsSurfaceEmpty)
        if (_iOpenSprite() == false)
            return;
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
    DrawSprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 255));
    return;
}

void CSprite::PutSpriteFastNoColorKey(int sX, int sY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 255));
    return;
}

void CSprite::PutSpriteFastFrontBuffer(int sX, int sY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 255));
    return;
}

void CSprite::PutSpriteFastWidth(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime)
{
    DrawWidth(sX, sY, sFrame, sWidth, dwTime, Color(255, 255, 255, 255));
    return;
}

void CSprite::PutShadowSprite(int sX, int sY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 255));
    return;
}

void CSprite::PutShadowSpriteClip(int sX, int sY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 255));
    return;
}

void CSprite::PutTransSprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(alphaDepth, 255, 255, 255));
    return;
}

void CSprite::PutTransSprite_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(alphaDepth, 255, 255, 255));
    return;
}

void CSprite::PutTransSprite70(int sX, int sY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(180, 255, 255, 255));
    return;
}

void CSprite::PutTransSprite70_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(180, 255, 255, 255));
    return;
}

void CSprite::PutTransSprite50(int sX, int sY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(125, 255, 255, 255));
    return;
}

void CSprite::PutTransSprite50_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(125, 255, 255, 255));
    return;
}

void CSprite::PutTransSprite25(int sX, int sY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(64, 255, 255, 255));
    return;
}

void CSprite::PutTransSprite25_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(64, 255, 255, 255));
    return;
}

void CSprite::PutTransSprite2(int sX, int sY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(180, 255, 255, 255));
    return;
}

void CSprite::PutShiftTransSprite2(int sX, int sY, int shX, int shY, int sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 255));
    return;
}

void CSprite::PutFadeSprite(short sX, short sY, short sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 255));
    return;
}

void CSprite::PutFadeSpriteDst(uint16_t * pDstAddr, short sPitch, short sX, short sY, short sFrame, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 255));
    return;
}

void CSprite::PutSpriteRGB(int sX, int sY, int sFrame, uint32_t color, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(color));
}

void CSprite::PutSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(255, sRed, sGreen, sBlue));
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
    DrawSprite(sX, sY, sFrame, dwTime, Color(180, sRed, sGreen, sBlue));
    return;
}

void CSprite::_GetSpriteRect(int sX, int sY, int sFrame)
{
    short dX, dY, sx, sy, szx, szy, pvx, pvy;
    if (this == 0)
        return;
    if (m_stBrush == 0)
        return;
    if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0))
        return;

    sx = m_stBrush[sFrame].sx;
    sy = m_stBrush[sFrame].sy;
    szx = m_stBrush[sFrame].szx;
    szy = m_stBrush[sFrame].szy;
    pvx = m_stBrush[sFrame].pvx;
    pvy = m_stBrush[sFrame].pvy;

    dX = sX + pvx;
    dY = sY + pvy;

    m_rcBound.top = -1;
    m_rcBound.bottom = -1;
    m_rcBound.left = -1;
    m_rcBound.right = -1;

    if (dX < 0)
    {
        sx = sx + (0 - dX);
        szx = szx - (0 - dX);
        if (szx < 0)
        {
            m_rcBound.top = -1;
            return;
        }
        dX = (short)0;
    }
    else if (dX + szx > G_pGame->GetWidth())
    {
        szx = szx - ((dX + szx) - (short)G_pGame->GetWidth());
        if (szx < 0)
        {
            m_rcBound.top = -1;
            return;
        }
    }

    if (dY < 0)
    {
        sy = sy + (0 - dY);
        szy = szy - (0 - dY);
        if (szy < 0)
        {
            m_rcBound.top = -1;
            return;
        }
        dY = (short)0;
    }
    else if (dY + szy > G_pGame->GetHeight())
    {
        szy = szy - ((dY + szy) - (short)G_pGame->GetHeight());
        if (szy < 0)
        {
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
    DrawSprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 255));
    return;
}

void CSprite::PutRevTransSprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth)
{
    DrawSprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 255));
    return;
}
