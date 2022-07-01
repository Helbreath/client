
#include "helbreath.h"


static char __cSpace[] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 8, 7, 8, 8, 9, 10, 9, 7, 8, 8, 8, 8, 8, 8, 8,
                          15, 16, 12, 17, 14, 15, 14, 16, 10, 13, 19, 10, 17, 17, 15, 14, 15, 16, 13, 17, 16, 16, 20, 17, 16, 14,
                          8, 8, 8, 8, 8, 8, 8, 6, 7, 8, 7, 7, 7, 7, 4, 7, 7, 4, 11, 7, 8, 8, 7, 8, 6, 5, 8, 9, 14, 8, 9, 8, 8, 8, 8, 8,
                          8, 8, 8, 8, 8, 8, 8 };
void helbreath::PutString_SprFont(int iX, int iY, std::string_view pStr, short sR, short sG, short sB)
{
    int iXpos;
    uint32_t iCnt;
    uint64_t dwTime = G_dwGlobalTime;
    char cTmpStr[100];

    ZeroMemory(cTmpStr, sizeof(cTmpStr));
    strcpy(cTmpStr, pStr.data());
    iXpos = iX;
    for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
    {
        if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122))
        {
            m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteRGB(iXpos + 1, iY, cTmpStr[iCnt] - 33, sR + 11, sG + 7, sB + 6, dwTime);
            if ((sR == 0) && (sG == 0) && (sB == 0))
                m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 33, dwTime);
            else
                m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 33, sR, sG, sB, dwTime);
            iXpos += __cSpace[cTmpStr[iCnt] - 33];
        }
        else
            iXpos += 5;
    }
}

void helbreath::PutString_SprFont2(int iX, int iY, std::string_view pStr, short sR, short sG, short sB)
{
    int iXpos;
    uint32_t iCnt;
    uint64_t dwTime = G_dwGlobalTime;
    char cTmpStr[200];

    ZeroMemory(cTmpStr, sizeof(cTmpStr));
    strcpy(cTmpStr, pStr.data());

    iXpos = iX;
    for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
    {
        if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122))
        {
            m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos + 1, iY, cTmpStr[iCnt] - 33, dwTime);
            m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos + 1, iY + 1, cTmpStr[iCnt] - 33, dwTime);
            if ((sR == 0) && (sG == 0) && (sB == 0))
                m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 33, dwTime);
            else
                m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 33, sR, sG, sB, dwTime);
            iXpos += __cSpace[cTmpStr[iCnt] - 33];
        }
        else
            iXpos += 5;
    }
}

void helbreath::PutString_SprFont3(int iX, int iY, std::string_view pStr, short sR, short sG, short sB, bool bTrans, int iType)
{
    int iXpos, iAdd;
    uint32_t iCnt;
    uint64_t dwTime = G_dwGlobalTime;
    char cTmpStr[128];

    ZeroMemory(cTmpStr, sizeof(cTmpStr));
    strcpy(cTmpStr, pStr.data());

    if (iType != -1)
    {
        iAdd = 95 * iType;
        iXpos = iX;
        for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
        {
            if ((cTmpStr[iCnt] >= 32) && (cTmpStr[iCnt] <= 126))
            {

                if (bTrans == false)
                {
                    m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteFast(iXpos, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteFast(iXpos + 1, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    if ((sR == 0) && (sG == 0) && (sB == 0))
                        m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    else
                        m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);
                }
                else
                    m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutTransSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);

                iXpos += (m_pSprite[SPRID_INTERFACE_SPRFONTS2]->brush[cTmpStr[iCnt] - 32 + iAdd].szx);
            }
            else
                iXpos += 5;
        }
    }
    else
    {
        iAdd = 0;
        iXpos = iX;
        for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
        {
            if ((cTmpStr[iCnt] >= 32) && (cTmpStr[iCnt] <= 126))
            {

                if (bTrans == false)
                {
                    m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteFast(iXpos, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteFast(iXpos + 1, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    if ((sR == 0) && (sG == 0) && (sB == 0))
                        m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    else
                        m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);
                }
                else
                    m_pSprite[SPRID_INTERFACE_FONT2]->PutTransSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);

                iXpos += (m_pSprite[SPRID_INTERFACE_FONT2]->brush[cTmpStr[iCnt] - 32 + iAdd].szx);
            }
            else
                iXpos += 5;
        }
    }
}

static char __cSpace2[] = { 6, 4, 6, 6, 6, 6, 6, 6, 6, 6, 6 }; //{8,6,9,8,8,9,8,8,8,8};
void helbreath::PutString_SprNum(int iX, int iY, char * pStr, short sR, short sG, short sB)
{
    int iXpos;
    unsigned char iCnt;
    uint64_t dwTime = G_dwGlobalTime;
    char cTmpStr[200];
    ZeroMemory(cTmpStr, sizeof(cTmpStr));
    strcpy(cTmpStr, pStr);
    //m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,Color(255,sR, sG, sB), &wR, &wG, &wB);
    iXpos = iX;
    for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
    {
        if ((cTmpStr[iCnt] >= 0x30) && (cTmpStr[iCnt] <= 0x39))
        {
            m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSprite(iXpos + 2, iY, cTmpStr[iCnt] - 0x30 + 6, dwTime);
            m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSprite(iXpos + 1, iY + 1, cTmpStr[iCnt] - 0x30 + 6, dwTime);
            if ((sR == 0) && (sG == 0) && (sB == 0))
                m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSprite(iXpos, iY, cTmpStr[iCnt] - 0x30 + 6, dwTime);
            else
                m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 0x30 + 6, sR, sG, sB, dwTime);
            iXpos += __cSpace2[cTmpStr[iCnt] - 0x30];
        }
    }
}

void helbreath::PutString(int iX, int iY, std::string pString, Color color, bool bHide, char cBGtype)
{
    int i;
    if (pString.length() == 0)
        return;
    if (bHide == false)
    {
        switch (cBGtype)
        {
            case 0:
                PutFontString("default", iX + 1, iY, pString, color);
                break;
            case 1:
                PutFontString("default", iX, iY + 1, pString, Color(255, 5, 5, 255));
                PutFontString("default", iX + 1, iY + 1, pString, Color(255, 5, 5, 255));
                PutFontString("default", iX + 1, iY, pString, Color(255, 5, 5, 255));
                break;
        }
        PutFontString("default", iX, iY, pString, color);
    }
    else
    {
        for (i = 0; i < pString.length(); ++i)
            if (pString[i] != 0)
                pString[i] = '*';

        switch (cBGtype)
        {
            case 0:
                PutFontString("default", iX + 1, iY, pString, color);
                break;
            case 1:
                PutFontString("default", iX, iY + 1, pString, Color(255, 5, 5, 255));
                PutFontString("default", iX + 1, iY + 1, pString, Color(255, 5, 5, 255));
                PutFontString("default", iX + 1, iY, pString, Color(255, 5, 5, 255));
                break;
        }
        PutFontString("default", iX, iY, pString, color);
    }
}
void helbreath::PutChatString(int iX, int iY, char * pString, Color color)
{
    PutFontString("default", iX, iY, pString, color); //TODO: make 'chat' font?
}
void helbreath::PutFontStringSize(std::string fontname, int iX, int iY, std::string text, Color color, int size)
{
    try
    {
        _text.setFont(_font.at(fontname));
        _text.setString(text);
        _text.setFillColor(color);
        _text.setPosition((float)iX, (float)iY);
        _text.setCharacterSize(size);
        visible.draw(_text);
    }
    catch (const out_of_range & oor)
    {
        //error
        //__asm int 3;
        __debugbreak();
    }
}

void helbreath::PutFontString(std::string fontname, int iX, int iY, std::string text, Color color)
{
    try
    {
        _text.setFont(_font.at(fontname));
        _text.setString(text);
        _text.setFillColor(color);
        _text.setPosition((float)iX, (float)iY);
        _text.setCharacterSize(12);
        visible.draw(_text);
    }
    catch (const out_of_range & oor)
    {
        //error
        //__asm int 3;
        __debugbreak();
    }
}

void helbreath::PutAlignedString(int iX1, int iX2, int iY, std::string text, Color color)
{
    try
    {
        _text.setFont(_font.at("default"));
        _text.setString(text);
        _text.setFillColor(color);
        sf::FloatRect bounds = _text.getLocalBounds();
        // todo - properly make an aligned string function
        //_text.setPosition((float)((float(iX2) - iX1) / 2 + bounds.width), (float)iY);
        _text.setPosition(iX1, (float)iY);
        _text.setCharacterSize(12);

        visible.draw(_text);
    }
    catch (const out_of_range & oor)
    {
        //error
        //__asm int 3;
        __debugbreak();
    }
}
