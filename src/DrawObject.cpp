

#include "newsocket.h"
#include "Game.h"
#include "buffer.h"
#include <algorithm>
#include <iostream>

#include "lan_eng.h"
#include <asio/ssl.hpp>

extern CGame * G_pGame;

// extern bool CheckCheating();
// extern bool CheckHackProgram();

extern char G_cSpriteAlphaDegree;

extern char G_cCmdLine[256], G_cCmdLineTokenA[120], G_cCmdLineTokenA_Lowercase[120], G_cCmdLineTokenB[120], G_cCmdLineTokenC[120], G_cCmdLineTokenD[120], G_cCmdLineTokenE[120];
extern void * G_hWnd;
extern void * G_hInstance;
//extern void SetKeyboardHook(bool enable);

extern char _cDrawingOrder[];
extern char _cMantleDrawingOrder[];
extern char _cMantleDrawingOrderOnRun[];


extern short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;
extern int _tmp_iStatus;
extern short _tmp_sHeadApprValue, _tmp_sBodyApprValue, _tmp_sArmApprValue, _tmp_sLegApprValue; // Re-Coding Sprite xRisenx
extern char  _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
extern int   _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY; // 21.171 2002-6-14
extern uint16_t  _tmp_wObjectID;
extern char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
extern uint16_t  wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int iFocuiStatus;
extern int   iFocusApprColor;
extern short sFocusHeadApprValue, sFocusBodyApprValue, sFocusArmApprValue, sFocusLegApprValue; // Re-Coding Sprite xRisenx
extern int iHungerStatus ; // Hunger Bar xRisenx
extern bool m_bStuckHack ; // Anti Hack xRisenx

bool   CGame::DrawObject_OnAttack(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iWeapon, iAdd, iShieldIndex, iMantleIndex;
    bool bInv = false;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 81 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }
    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);
    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (player_name == _tmp_cName) bInv = true;
        else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
        else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
        else return false;
    }
    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
                if (iWeapon == 0) iAdd = 6;
                if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
                if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + iAdd;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/

                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 4;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 5;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 5;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 5;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 5;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 5;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 5;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 5;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 5;*/
                iWeaponIndex = -1;
                iShieldIndex = -1;
            }
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
                if (iWeapon == 0) iAdd = 6;
                if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
                if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + iAdd;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 4;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
                // xRisenx
                if (_tmp_sHeadApprValue == 0) // So its tmp that gets a wrong value somewhere
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + iAdd; // 4112 // Should be 10 thats why iHelmIndex is wierd.. cause you take 4112 *15 = iHelmIndex
                                                                                  /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                                                                                  iHelmIndex = -1;
                                                                                  else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 5;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 5;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 5;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 5;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 5;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 5;*/

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 5;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 5;*/
                iWeaponIndex = -1;
                iShieldIndex = -1;
            }
            break;

        default:
            if (_tmp_sAppr2 != 0)
            {
                iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                _tmp_cFrame = _tmp_sAppr2 - 1;
            }
            else if (_tmp_sOwnerType == 66) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
            else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
            else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
            else if (_tmp_sOwnerType == 106) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Earth Wyvern
            else if (_tmp_sOwnerType == 107) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Wind Wyvern
            else if (_tmp_sOwnerType == 108) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Dark Wyvern
            else if (_tmp_sOwnerType == 109) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Grass Wyvern
            else if (_tmp_sOwnerType == 110) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sky Wyvern
            else if (_tmp_sOwnerType == 111) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sea Wyvern
            else if (_tmp_sOwnerType == 112) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Metal Wyvern
            else if (_tmp_sOwnerType == 113) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Void Wyvern
            else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
            iUndiesIndex = -1;
            iHairIndex = -1;
            iBodyArmorIndex = -1;
            iArmArmorIndex = -1;
            iBootsIndex = -1;
            iPantsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }
    if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

    if (m_bIsCrusadeMode ||
        (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0))
    {
        DrawObjectFOE(sX, sY, _tmp_cFrame);
    }

    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // FireWyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
            case 127: // Enraged Tigerworm
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
        }
        CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);
        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                {
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
                }
                else
                {
                    m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            }

            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

            if (_tmp_sOwnerType == 81) // Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
            }
            else if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
            }
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (((_tmp_sAppr2 & 0x000F) == 8) && (iShieldGlare == 1))
            {
                m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
            }
            else
                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                    switch (iShieldGlare)
                    {
                        case 0: break;
                            //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                        case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
        }
        else
        {
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

            if (_tmp_sOwnerType == 81) // Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
            }
            else if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            }
        }

        if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 6, -2, 6, dwTime);


        if ((_tmp_iStatus & 0x20) != 0) // Berserk
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        DrawAngel((_tmp_cDir - 1), sX + 20, sY - 20, _tmp_cFrame % 8, dwTime);
        CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }
    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }

    // Snoopy: Abaddon effects
    if (_tmp_sOwnerType == 81)
    {
        int randFrame = _tmp_cFrame % 12;
        m_pEffectSpr[154]->PutTransSprite70(sX - 50, sY - 50, randFrame, dwTime);
        m_pEffectSpr[155]->PutTransSprite70(sX - 20, sY - 80, randFrame, dwTime);
        m_pEffectSpr[156]->PutTransSprite70(sX + 70, sY - 50, randFrame, dwTime);
        m_pEffectSpr[157]->PutTransSprite70(sX - 30, sY, randFrame, dwTime);
        m_pEffectSpr[158]->PutTransSprite70(sX - 60, sY + 90, randFrame, dwTime);
        m_pEffectSpr[159]->PutTransSprite70(sX + 65, sY + 85, randFrame, dwTime);
        switch (_tmp_cDir)
        {
            case 1:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 108, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 50, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 2:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 70, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 3:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 105, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 90, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 4:
                m_pEffectSpr[153]->PutTransSprite70(sX - 35, sY + 100, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 80, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 5:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 65, sY - 5, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 6:
                m_pEffectSpr[153]->PutTransSprite70(sX + 45, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 31, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 7:
                m_pEffectSpr[153]->PutTransSprite70(sX + 40, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 30, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 8:
                m_pEffectSpr[153]->PutTransSprite70(sX + 20, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 20, sY + 16, _tmp_iEffectFrame % 15, dwTime);
                break;
        }
    }
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
    return false;
}

bool CGame::DrawObject_OnAttackMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iWeapon, iAdd, iShieldIndex, iMantleIndex, dx, dy, dsx, dsy;
    int cFrameMoveDots;
    bool bInv = false, bDashDraw = false;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 81 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }
    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);
    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (player_name == _tmp_cName) bInv = true;
        else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
        else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
        else return false;
    }

    switch (_tmp_cFrame)
    {
        case 4:  _tmp_cFrame = 4; break;
        case 5:  _tmp_cFrame = 4; break;
        case 6:  _tmp_cFrame = 4; break;
        case 7:  _tmp_cFrame = 4; break;
        case 8:  _tmp_cFrame = 4; break;
        case 9:  _tmp_cFrame = 4; break;
        case 10: _tmp_cFrame = 5; break;
        case 11: _tmp_cFrame = 6; break;
        case 12: _tmp_cFrame = 7; break;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
                if (iWeapon == 0) iAdd = 6;
                if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
                if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + iAdd;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 4;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 5;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 5;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 5;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 5;*/
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 5;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 5;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 5;*/
                iWeaponIndex = -1;
                iShieldIndex = -1;
            }
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
                if (iWeapon == 0) iAdd = 6;
                if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
                if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + iAdd;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 4;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 5;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 5;*/
                }
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 5;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 5;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 5;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 5;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 5;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 5;*/
                iWeaponIndex = -1;
                iShieldIndex = -1;
            }
            break;
        default:
            iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
            iUndiesIndex = -1;
            iHairIndex = -1;
            iBodyArmorIndex = -1;
            iArmArmorIndex = -1;
            iBootsIndex = -1;
            iPantsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }
    dx = 0;
    dy = 0;
    if ((_tmp_cFrame >= 1) && (_tmp_cFrame <= 3))
    {
        switch (_tmp_cFrame)
        {
            case 1: cFrameMoveDots = 26; break;
            case 2: cFrameMoveDots = 16; break;
            case 3: cFrameMoveDots = 0;  break;
        }
        switch (_tmp_cDir)
        {
            case 1: dy = cFrameMoveDots; break;
            case 2: dy = cFrameMoveDots; dx = -cFrameMoveDots; break;
            case 3: dx = -cFrameMoveDots; break;
            case 4: dx = -cFrameMoveDots; dy = -cFrameMoveDots; break;
            case 5: dy = -cFrameMoveDots; break;
            case 6: dy = -cFrameMoveDots; dx = cFrameMoveDots; break;
            case 7: dx = cFrameMoveDots; break;
            case 8: dx = cFrameMoveDots; dy = cFrameMoveDots; break;
        }
        switch (_tmp_cFrame)
        {
            case 1: dy++;    break;
            case 2: dy += 2; break;
            case 3: dy++;    break;
        }
        switch (_tmp_cFrame)
        {
            case 2: bDashDraw = true; cFrameMoveDots = 26; break;
            case 3: bDashDraw = true; cFrameMoveDots = 16; break;
        }
        dsx = 0;
        dsy = 0;
        switch (_tmp_cDir)
        {
            case 1: dsy = cFrameMoveDots; break;
            case 2: dsy = cFrameMoveDots; dsx = -cFrameMoveDots; break;
            case 3: dsx = -cFrameMoveDots; break;
            case 4: dsx = -cFrameMoveDots; dsy = -cFrameMoveDots; break;
            case 5: dsy = -cFrameMoveDots; break;
            case 6: dsy = -cFrameMoveDots; dsx = cFrameMoveDots; break;
            case 7: dsx = cFrameMoveDots; break;
            case 8: dsx = cFrameMoveDots; dsy = cFrameMoveDots; break;
        }
    }
    else if (_tmp_cFrame > 3)
    {
        dx = 0;
        dy = 0;
    }
    else
    {
        switch (_tmp_cDir)
        {
            case 1: dy = 32; break;
            case 2: dy = 32; dx = -32; break;
            case 3: dx = -32; break;
            case 4: dx = -32; dy = -32; break;
            case 5: dy = -32; break;
            case 6: dy = -32; dx = 32; break;
            case 7: dx = 32; break;
            case 8: dx = 32; dy = 32; break;
        }
    }
    if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

    if (m_bIsCrusadeMode ||
        (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0))
    {
        DrawObjectFOE(sX + dx, sY + dy, _tmp_cFrame);
    }

    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
            case 127: // Enraged Tigerworm
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
        }
        CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);
        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            }

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

                else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
        }
        else
        {
            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }


            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            }
        }

        if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 6, -2, 6, dwTime);

        // Berserk
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
        DrawAngel(8 + (_tmp_cDir - 1), sX + dx + 20, sY + dy - 20, _tmp_cFrame % 8, dwTime);
        CheckActiveAura2(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

        if (bDashDraw == true)
        {
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dsx, sY + dsy, _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            if (iWeaponIndex != -1) m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dsx, sY + dsy, _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            if (iShieldIndex != -1) m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dsx, sY + dsy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
        }
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    _tmp_dx = dx;
    _tmp_dy = dy;

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}


bool   CGame::DrawObject_OnMagic(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iHelmIndex, iMantleIndex;
    bool bInv = false;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (player_name == _tmp_cName)
            bInv = true;
        else
        {
            if (_tmp_iChatIndex != 0)
            {
                if (m_pChatMsgList[_tmp_iChatIndex] != 0)
                {
                    m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
                    m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
                }
                else
                {
                    m_pMapData->ClearChatMsg(indexX, indexY);
                }
            }
            return false;
        }
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (8 * 8);
            iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 8;
            iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 8;
            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                // xRisenx
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 8;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 8;*/
            }
            // xRisenx
            if (_tmp_sArmApprValue == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 8;
            /*if ((_tmp_sAppr3 & 0x000F) == 0)
            iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 8;*/

            // xRisenx
            if (_tmp_sLegApprValue == 0)
                iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 8;
            /*if ((_tmp_sAppr3 & 0x0F00) == 0)
            iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 8;*/
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 8;
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 8;

            // xRisenx
            if (_tmp_sHeadApprValue == 0)
                iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 8;
            /*if ((_tmp_sAppr3 & 0x00F0) == 0)
            iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 8;*/
            break;
        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (8 * 8);
            iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 8;
            iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 8;
            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                // xRisenx
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 8;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 8;*/
            }
            // xRisenx
            if (_tmp_sArmApprValue == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 8;
            /*if ((_tmp_sAppr3 & 0x000F) == 0)
            iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 8;*/

            // xRisenx
            if (_tmp_sLegApprValue == 0)
                iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 8;
            /*if ((_tmp_sAppr3 & 0x0F00) == 0)
            iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 8;*/
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 8;
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 8;

            // xRisenx
            if (_tmp_sHeadApprValue == 0)
                iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 8;
            /*if ((_tmp_sAppr3 & 0x00F0) == 0)
            iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 8;*/
            break;
    }

    /*
    switch (_tmp_cFrame) {
    case 15:
    _tmp_cFrame = 14;
    break;
    case 16:
    _tmp_cFrame = 14;
    break;
    case 17:
    _tmp_cFrame = 15;
    break;
    case 18:
    _tmp_cFrame = 15;
    break;
    case 19:
    _tmp_cFrame = 15;
    break;
    case 20:
    _tmp_cFrame = 15;
    break;
    case 21:
    _tmp_cFrame = 15;
    break;
    case 22:
    _tmp_cFrame = 15;
    break;
    case 23:
    _tmp_cFrame = 14;
    break;
    }
    */
    if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

    if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX, sY, _tmp_cFrame);
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 127: // Enraged Tigerworm
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
        }
        CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);

        if (bInv == true)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
        else
        {
            if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
        }
        SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

        if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }

        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }

        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }

        if (iMantleIndex != -1)
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 6, -2, 6, dwTime);

        if ((_tmp_iStatus & 0x20) != 0) 	// Berserk
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        DrawAngel(32 + (_tmp_cDir - 1), sX + 20, sY - 20, _tmp_cFrame % 16, dwTime);
        CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }
    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
    return false;
}

bool   CGame::DrawObject_OnGetItem(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iHelmIndex, iMantleIndex;
    bool bInv = false;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (player_name ==  _tmp_cName) bInv = true;
        else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
        else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
        else return false;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (9 * 8);
            iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 9;
            iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 9;
            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                // xRisenx
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 9;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 9;*/

            }
            // xRisenx
            if (_tmp_sArmApprValue == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 9;
            /*if ((_tmp_sAppr3 & 0x000F) == 0)
            iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 9;*/
            // xRisenx
            if (_tmp_sLegApprValue == 0)
                iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 9;
            /*if ((_tmp_sAppr3 & 0x0F00) == 0)
            iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 9;*/
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 9;
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 9;
            // xRisenx
            if (_tmp_sHeadApprValue == 0)
                iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 9;
            /*if ((_tmp_sAppr3 & 0x00F0) == 0)
            iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 9; 		*/
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (9 * 8);
            iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 9;
            iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 9;
            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                // xRisenx
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 9;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 9;*/
            }
            // xRisenx
            if (_tmp_sArmApprValue == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 9;
            /*if ((_tmp_sAppr3 & 0x000F) == 0)
            iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 9;*/
            // xRisenx
            if (_tmp_sLegApprValue == 0)
                iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 9;
            /*if ((_tmp_sAppr3 & 0x0F00) == 0)
            iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 9;*/
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 9;
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 9;
            // xRisenx
            if (_tmp_sHeadApprValue == 0)
                iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 9;
            /*if ((_tmp_sAppr3 & 0x00F0) == 0)
            iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 9;*/
            break;
        default:
            iUndiesIndex = -1;
            iHairIndex = -1;
            iArmArmorIndex = -1;
            iBodyArmorIndex = -1;
            iPantsIndex = -1;
            iBootsIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }
    if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;
    if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX, sY, _tmp_cFrame);

    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 127: // Enraged Tigerworm
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
        }
        CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);

        if (bInv == true)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX, sY, _tmp_cFrame, dwTime);
        else
        {
            if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
        }
        SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

        if (iUndiesIndex != -1)
        {
            if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
        }

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }
        }

        if (iPantsIndex != -1)
        {
            if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }
        }

        if (iArmArmorIndex != -1)
        {
            if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }
        }

        if (iBodyArmorIndex != -1)
        {
            if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }
        }

        if (iHelmIndex != -1)
        {
            if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }
        }

        if (iMantleIndex != -1)
        {
            if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
        }

        if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 6, -2, 6, dwTime);

        if ((_tmp_iStatus & 0x20) != 0) // Berserk
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        DrawAngel(40 + (_tmp_cDir - 1), sX + 20, sY - 20, _tmp_cFrame % 4, dwTime);
        CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }
    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
    return false;
}

bool CGame::DrawObject_OnDamage(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iWeaponIndex, iShieldIndex, iHelmIndex, iR, iG, iB;
    int iAdd, iDrawMode, iMantleIndex;
    char cFrame;
    bool bInv = false;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 81 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }
    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);
    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (player_name == _tmp_cName) bInv = true;
        else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
        else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
        else return false;
    }
    cFrame = _tmp_cFrame;
    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if (cFrame < 4)
            {
                if ((_tmp_sAppr2 & 0xF000) != 0) iAdd = 1;
                else iAdd = 0;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + iAdd;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + iAdd;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * iAdd + (_tmp_cDir - 1);
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
                iDrawMode = 0;
            }
            else
            {
                cFrame -= 4;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 10;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 10;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 10;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 10;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 10;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 10;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 10;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 5;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 10;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 10;*/
                iDrawMode = 1;
            }
            break;
        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
            if (cFrame < 4)
            {
                if ((_tmp_sAppr2 & 0xF000) != 0) iAdd = 1;
                else iAdd = 0;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + iAdd;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + iAdd;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * iAdd + (_tmp_cDir - 1);
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
                iDrawMode = 0;
            }
            else
            {
                cFrame -= 4;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 10;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 10;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 10;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 10;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 10;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 10;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 10;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 5;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 10;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 10;*/
                iDrawMode = 1;
            }
            break;

        default:
            if (cFrame < 4)
            {
                if (_tmp_sAppr2 != 0)
                {
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 67) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 68) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 69) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 81) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 91) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 106) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Earth Wyvern
                else if (_tmp_sOwnerType == 107) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Wind Wyvern
                else if (_tmp_sOwnerType == 108) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Dark Wyvern
                else if (_tmp_sOwnerType == 109) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Grass Wyvern
                else if (_tmp_sOwnerType == 110) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sky Wyvern
                else if (_tmp_sOwnerType == 111) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sea Wyvern
                else if (_tmp_sOwnerType == 112) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Metal Wyvern
                else if (_tmp_sOwnerType == 113) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Void Wyvern
                else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            }
            else
            {
                cFrame -= 4;
                if (_tmp_sAppr2 != 0)
                {
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 67) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 68) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 69) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 81) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 91) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
                else if (_tmp_sOwnerType == 106) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Earth Wyvern
                else if (_tmp_sOwnerType == 107) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Wind Wyvern
                else if (_tmp_sOwnerType == 108) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Dark Wyvern
                else if (_tmp_sOwnerType == 109) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Grass Wyvern
                else if (_tmp_sOwnerType == 110) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sky Wyvern
                else if (_tmp_sOwnerType == 111) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sea Wyvern
                else if (_tmp_sOwnerType == 112) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Metal Wyvern
                else if (_tmp_sOwnerType == 113) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Void Wyvern
                else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
            }
            iUndiesIndex = -1;
            iHairIndex = -1;
            iArmArmorIndex = -1;
            iBodyArmorIndex = -1;
            iPantsIndex = -1;
            iBootsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            iDrawMode = 0;
            break;
    }
    if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

    if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX, sY, cFrame);

    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 127: // Enraged Tigerworm
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
                }
                break;
        }
        CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);
        if (iDrawMode == 1) // Etrange, 1 semble impossible avec des mobs !
        {
            if (_cDrawingOrder[_tmp_cDir] == 1)
            {
                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                    DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                    switch (iWeaponGlare)
                    {
                        case 0: break;
                        case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if (_tmp_sOwnerType == 35)
                    m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

                if (_tmp_sOwnerType == 81) // Abaddon
                {
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                }
                else if (bInv == true)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
                }
                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                    switch (iShieldGlare)
                    {
                        case 0: break;
                            //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                        case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }
            else
            {
                if (_tmp_sOwnerType == 35)
                    m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

                if (_tmp_sOwnerType == 81) // Abaddon
                {
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                }
                else if (bInv == true)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
                }
                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                    switch (iShieldGlare)
                    {
                        case 0: break;
                            //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                        case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                    DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                    switch (iWeaponGlare)
                    {
                        case 0: break;
                        case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }
            }

            if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 6, -2, 6, dwTime);

            if ((_tmp_iStatus & 0x20) != 0) 	// Berserk
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 0, -5, -5, dwTime);
            DrawAngel(16 + (_tmp_cDir - 1), sX + 20, sY - 20, cFrame % 4, dwTime);
            CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

        }
        else // DrawMode != 1
        {
            if (_cDrawingOrder[_tmp_cDir] == 1)
            {
                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                    DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                    switch (iWeaponGlare)
                    {
                        case 0: break;
                        case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }
                if (_tmp_sOwnerType == 35)
                    m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

                if (_tmp_sOwnerType == 81) // Abaddon
                {
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                }
                else if (bInv == true)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
                }
                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                    switch (iShieldGlare)
                    {
                        case 0: break;
                            //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                        case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }
            else
            {
                if (_tmp_sOwnerType == 35)
                    m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

                if (_tmp_sOwnerType == 81) // Abaddon
                {
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                }
                else if (bInv == true)
                {
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                }
                else
                {
                    if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
                }

                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                    switch (iShieldGlare)
                    {
                        case 0: break;
                            //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                        case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                    DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                    switch (iWeaponGlare)
                    {
                        case 0: break;
                        case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }
            }

            if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 6, -2, 6, dwTime);

            if ((_tmp_iStatus & 0x20) != 0)	// Berserk
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 0, -5, -5, dwTime);
            DrawAngel(16 + (_tmp_cDir - 1), sX + 20, sY - 20, cFrame % 4, dwTime);
            CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);
        }
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }
    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    // Snoopy: Abaddon effects
    if (_tmp_sOwnerType == 81)
    {
        int randFrame = _tmp_cFrame % 12;
        m_pEffectSpr[154]->PutTransSprite70(sX - 50, sY - 50, randFrame, dwTime);
        m_pEffectSpr[155]->PutTransSprite70(sX - 20, sY - 80, randFrame, dwTime);
        m_pEffectSpr[156]->PutTransSprite70(sX + 70, sY - 50, randFrame, dwTime);
        m_pEffectSpr[157]->PutTransSprite70(sX - 30, sY, randFrame, dwTime);
        m_pEffectSpr[158]->PutTransSprite70(sX - 60, sY + 90, randFrame, dwTime);
        m_pEffectSpr[159]->PutTransSprite70(sX + 65, sY + 85, randFrame, dwTime);
        switch (_tmp_cDir)
        {
            case 1:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 108, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 50, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 2:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 70, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 3:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 105, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 90, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 4:
                m_pEffectSpr[153]->PutTransSprite70(sX - 35, sY + 100, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 80, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 5:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 65, sY - 5, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 6:
                m_pEffectSpr[153]->PutTransSprite70(sX + 45, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 31, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 7:
                m_pEffectSpr[153]->PutTransSprite70(sX + 40, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 30, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 8:
                m_pEffectSpr[153]->PutTransSprite70(sX + 20, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 20, sY + 16, _tmp_iEffectFrame % 15, dwTime);
                break;
        }
    }
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnDying(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iHelmIndex, iMantleIndex;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;
    char cFrame;

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }
    cFrame = _tmp_cFrame;

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if (cFrame < 6)
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 0;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // Dax
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 0;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 0;*/
                }
                // Dax
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 0;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 0;*/

                // Dax
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 0;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 0;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 0;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 0;

                // Dax
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 0;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 0;*/
            }
            else
            {
                cFrame -= 6;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 11;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // Dax
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 11;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 11;*/
                }
                // Dax
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 11;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 11;*/

                // Dax
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 11;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 11;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

                // Dax
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 11;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 11;*/
            }
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
            if (cFrame < 6)
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 0;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // Dax
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 0;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 0;*/
                }
                // Dax
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 0;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 0;*/

                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 0;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 0;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 0;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 0;

                // Dax
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 0;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 0;*/
            }
            else
            {
                cFrame -= 6;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 11;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // Dax
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 11;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 11;*/
                }
                // Dax
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 11;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 11;*/

                // Dax
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 11;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 11;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

                // Dax
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 11;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 11;*/
            }
            break;

        default:
            if (cFrame < 4)
            {
                if (_tmp_sAppr2 != 0)
                {
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 81) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 91) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                iUndiesIndex = -1;
                iHairIndex = -1;
                iArmArmorIndex = -1;
                iBodyArmorIndex = -1;
                iPantsIndex = -1;
                iBootsIndex = -1;
                iMantleIndex = -1;
                iHelmIndex = -1;
                switch (_tmp_sOwnerType)
                {
                    case 36: // AGT
                    case 37: // CGT
                    case 38: // MS
                    case 39: // DT
                    case 40: // ESG
                    case 41: // GMG
                    case 42: // ManaStone
                        if (_tmp_sAppr2 == 0) cFrame = 0;
                        break;
                    case 51: cFrame = 0; break;
                }
            }
            else
            {
                switch (_tmp_sOwnerType)
                {
                    case 51: cFrame = 0; break;
                    default: cFrame -= 4; break;
                }
                if (_tmp_sAppr2 != 0)
                {
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 81) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 91) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 106) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Earth Wyvern
                else if (_tmp_sOwnerType == 107) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Wind Wyvern
                else if (_tmp_sOwnerType == 108) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Dark Wyvern
                else if (_tmp_sOwnerType == 109) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Grass Wyvern
                else if (_tmp_sOwnerType == 110) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Sky Wyvern
                else if (_tmp_sOwnerType == 111) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Sea Wyvern
                else if (_tmp_sOwnerType == 112) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Metal Wyvern
                else if (_tmp_sOwnerType == 113) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Void Wyvern
                else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                iUndiesIndex = -1;
                iHairIndex = -1;
                iArmArmorIndex = -1;
                iBodyArmorIndex = -1;
                iPantsIndex = -1;
                iBootsIndex = -1;
                iMantleIndex = -1;
                iHelmIndex = -1;
            }
            break;
    }
    if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;
    if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX, sY, cFrame);

    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 127: // Enraged Tigerworm
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
                }
                break;
        }
        if (_tmp_sOwnerType == 81)
        {
            m_pEffectSpr[152]->PutTransSprite70(sX - 80, sY - 15, _tmp_iEffectFrame % 27, dwTime); // Explosion Abaddon
            m_pEffectSpr[152]->PutTransSprite70(sX, sY - 15, _tmp_iEffectFrame % 27, dwTime);
            m_pEffectSpr[152]->PutTransSprite70(sX - 40, sY, _tmp_iEffectFrame % 27, dwTime);
            m_pEffectSpr[163]->PutTransSprite70(sX - 90, sY - 80, _tmp_iEffectFrame % 12, dwTime); // Ames qui s'envolent
            m_pEffectSpr[160]->PutTransSprite70(sX - 60, sY - 50, _tmp_iEffectFrame % 12, dwTime);
            m_pEffectSpr[161]->PutTransSprite70(sX - 30, sY - 20, _tmp_iEffectFrame % 12, dwTime);
            m_pEffectSpr[162]->PutTransSprite70(sX, sY - 100, _tmp_iEffectFrame % 12, dwTime);
            m_pEffectSpr[163]->PutTransSprite70(sX + 30, sY - 30, _tmp_iEffectFrame % 12, dwTime);
            m_pEffectSpr[162]->PutTransSprite70(sX + 60, sY - 90, _tmp_iEffectFrame % 12, dwTime);
            m_pEffectSpr[163]->PutTransSprite70(sX + 90, sY - 50, _tmp_iEffectFrame % 12, dwTime);
            switch (_tmp_cDir)
            {
                case 1: m_pEffectSpr[140]->PutTransSprite70(sX, sY, cFrame, dwTime); break; // Abbadon dying
                case 2: m_pEffectSpr[141]->PutTransSprite70(sX, sY, cFrame, dwTime); break; // fixed sprit IDs
                case 3: m_pEffectSpr[142]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
                case 4: m_pEffectSpr[143]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
                case 5: m_pEffectSpr[144]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
                case 6: m_pEffectSpr[145]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
                case 7: m_pEffectSpr[146]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
                case 8: m_pEffectSpr[147]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
            }
        }
        else if (_tmp_sOwnerType == 66) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
        else if (_tmp_sOwnerType == 73)
        {	//m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
            m_pSprite[33]->PutTransSprite(sX, sY, cFrame, dwTime);
            switch (_tmp_cDir)
            {
                case 1: m_pEffectSpr[141]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break; // Abbadon qui meurt
                case 2: m_pEffectSpr[142]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
                case 3: m_pEffectSpr[143]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
                case 4: m_pEffectSpr[144]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
                case 5: m_pEffectSpr[145]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
                case 6: m_pEffectSpr[146]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
                case 7: m_pEffectSpr[147]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
                    //case 8: m_pEffectSpr[148]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
                case 8: m_pEffectSpr[141]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break; //due to buggy Sprite nb
            }
        }
        else
        {
            if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
        }

        SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

        if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }

        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }

        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }

        if (iMantleIndex != -1)
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 6, -2, 6, dwTime);

        if ((_tmp_iStatus & 0x20) != 0) // Berserk
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 0, -5, -5, dwTime);
        DrawAngel(24 + (_tmp_cDir - 1), sX + 20, sY - 20, _tmp_cFrame, dwTime);
        CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }
    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
    return false;
}

bool   CGame::DrawObject_OnDead(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iFrame, iMantleIndex, iHelmIndex;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 66) return false;

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iFrame = 7;
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
            iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 11;
            iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;
            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                // Dax
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 11;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 11;*/
            }
            // Dax
            if (_tmp_sArmApprValue == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 11;
            /*if ((_tmp_sAppr3 & 0x000F) == 0)
            iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 11;*/

            // Dax
            if (_tmp_sLegApprValue == 0)
                iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 11;
            /*if ((_tmp_sAppr3 & 0x0F00) == 0)
            iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 11;*/
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

            // Dax
            if (_tmp_sHeadApprValue == 0)
                iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 11;
            /*if ((_tmp_sAppr3 & 0x00F0) == 0)
            iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 11;*/
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
            iFrame = 7;
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
            iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 11;
            iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;
            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 11;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 11;*/
            }
            // Dax
            if (_tmp_sArmApprValue == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 11;
            /*if ((_tmp_sAppr3 & 0x000F) == 0)
            iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 11;*/

            // Dax
            if (_tmp_sLegApprValue == 0)
                iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 11;
            /*if ((_tmp_sAppr3 & 0x0F00) == 0)
            iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 11;*/
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

            // Dax
            if (_tmp_sHeadApprValue == 0)
                iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 11;
            /*if ((_tmp_sAppr3 & 0x00F0) == 0)
            iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 11;*/
            break;
        default:
            switch (_tmp_sOwnerType)
            {
                case 122: // Enraged Troll
                case 28: // Troll
                case 29: // Ogre
                case 30: // Liche
                case 31: // DD		// les 2 dernieres sont pas bonnes pour un mort !
                case 63: // Frost	// les 2 dernieres sont pas bonnes pour un mort !
                    iFrame = 5;
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                case 32: // Uni
                case 33: // WW
                case 43: // LWB
                case 44: // GHK
                case 45: // GHKABS
                case 46: // TK
                case 47: // BG
                case 48: // SK
                case 49: // HC
                case 50: // TW
                case 53: // BB
                case 54: // DE
                case 55: // Rabbit
                case 56: // Cat
                case 57: // Frog
                case 58: // MG
                case 59: // Ettin
                case 60: // Plant
                case 61: // Rudolph
                case 62: // Direboar
                case 64: // Crops  ----------- Crop ici! etonant, pourtant !
                case 65: // IceGolem
                case 70: // Dragon..........Ajouts par Snoopy
                case 71: // Centaur
                case 72: // ClawTurtle
                case 74: // GiantCrayfish
                case 75: // Gi Lizard
                case 76: // Gi Tree
                case 77: // Master Orc
                case 78: // Minaus
                case 79: // Nizie
                case 80: // Tentocle
                case 82: // Sorceress
                case 83: // ATK
                case 84: // MasterElf
                case 85: // DSK
                case 88: // Barbarian
                case 126: // Enraged Hellclaw
                case 127: // Enraged Tigerworm
                    iFrame = 7;
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                case 86: // HBT
                case 87: // CT
                case 89: // AGC
                    iFrame = 7;
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                    break;

                case 66: // Wyvern
                    iFrame = 15;
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                    break;

                case 73: // FireWyvern
                    iFrame = 7;
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                    bTrans = true; // Prevents showing hugly corpse
                    break;

                case 81: // Abaddon
                    iFrame = 0;
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                    bTrans = true; // Prevents showing hugly corpse
                    break;

                case 51: // CP
                    iFrame = 0;
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                case 52: // GG
                    iFrame = 11;
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                case 91: // Gate
                    iFrame = 5;
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                    break;

                default: // 40*4 (10...27)
                    iFrame = 3;
                    iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;
            }
            iUndiesIndex = -1;
            iHairIndex = -1;
            iArmArmorIndex = -1;
            iBodyArmorIndex = -1;
            iPantsIndex = -1;
            iBootsIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }
    if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;
    if (bTrans == false)
    {
        if (_tmp_cFrame == -1)
        {
            _tmp_cFrame = 7;
            if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, iFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, iFrame, dwTime);

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if (iMantleIndex != -1)
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
            if ((_tmp_iStatus & 0x0F00) >> 8 == 9)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, iFrame, 6, -2, 6, dwTime);
        }
        else if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, iFrame, -2 * _tmp_cFrame + 5, -2 * _tmp_cFrame - 5, -2 * _tmp_cFrame - 5, dwTime);
        else
        {
            if ((_tmp_iStatus & 0x0F00) >> 8 == 9)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, iFrame, -2 * _tmp_cFrame + 6, -2 * _tmp_cFrame - 2, -2 * _tmp_cFrame + 6, dwTime);
            else
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, iFrame, -2 * _tmp_cFrame, -2 * _tmp_cFrame, -2 * _tmp_cFrame, dwTime);
        }
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearDeadChatMsg(indexX, indexY);
        }
    }
    // Snoopy: Abaddon effects
    if (_tmp_sOwnerType == 81)
    {
        Abaddon_corpse(sX, sY); // By Snoopy....
    }
    else if (_tmp_sOwnerType == 73)
    {
        //m_pEffectSpr[35]->PutTransSprite70(sX+120, sY+120, rand(), dwTime);
        m_pEffectSpr[35]->PutTransSprite70(sX + 20, sY - 15, rand() % 10, dwTime);
    }
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
    return false;
}

bool   CGame::DrawObject_OnMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    int dx, dy;
    int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iShieldIndex, iAdd, iMantleIndex;
    bool bInv = false;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66 || _tmp_sOwnerType == 73)	bInv = true; //Energy-Ball, Wyvern

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }
    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);
    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (player_name == _tmp_cName) bInv = true;
        else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
        else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
        else return false;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iAdd = 3;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + iAdd;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 3;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 2;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 2;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 2;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 2;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 2;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 2;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else  iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 2;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 2;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 2;*/
            }
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iAdd = 3;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + iAdd;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 3;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 2;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 2;*/
                }
                // xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 2;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 2;*/
                // xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 2;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 2;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 2;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;
                // xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 2;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 2;*/
            }
            break;

        default:
            if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
            iUndiesIndex = -1;
            iHairIndex = -1;
            iBodyArmorIndex = -1;
            iArmArmorIndex = -1;
            iBootsIndex = -1;
            iPantsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }
    dx = 0;
    dy = 0;
    switch (_tmp_cDir)
    {
        case 1: dy = 28 - (_tmp_cFrame << 2); break;
        case 2: dy = 28 - (_tmp_cFrame << 2); dx = (_tmp_cFrame << 2) - 28; break;
        case 3: dx = (_tmp_cFrame << 2) - 28; break;
        case 4: dx = (_tmp_cFrame << 2) - 28; dy = (_tmp_cFrame << 2) - 28; break;
        case 5: dy = (_tmp_cFrame << 2) - 28; break;
        case 6: dy = (_tmp_cFrame << 2) - 28; dx = 28 - (_tmp_cFrame << 2); break;
        case 7: dx = 28 - (_tmp_cFrame << 2); break;
        case 8: dx = 28 - (_tmp_cFrame << 2); dy = 28 - (_tmp_cFrame << 2); break;
    }
    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:

        case 28: // Troll.
        case 29: // Orge.
        case 30: // Liche
        case 31: // DD
        case 32: // Uni
        case 33: // ww

        case 43: // LWB
        case 44: // GHK
        case 45: // GHKABS
        case 46: // TK
        case 47: // BG
        case 48: // SK
        case 49: // HC
        case 50: // TW

        case 52: // GG
        case 53: // BB
        case 54: // DE
        case 55: // Rabbit
        case 56: // Cat
        case 57: // Frog
        case 58: // MG
        case 59: // Ettin
        case 60: // Plant
        case 61: // Rudolph
        case 62: // DireBoar
        case 63: // Frost

        case 65: // Ice-Golem
        case 66: // Wyvern

        case 70: // Dragon..........Ajouts par Snoopy
        case 71: // Centaur
        case 72: // ClawTurtle
        case 73: // FireWyvern
        case 74: // GiantCrayfish
        case 75: // Gi Lizard
        case 76: // Gi Tree
        case 77: // Master Orc
        case 78: // Minaus
        case 79: // Nizie
        case 80: // Tentocle
        case 81: // Abaddon
        case 82: // Sorceress
        case 83: // ATK
        case 84: // MasterElf
        case 85: // DSK
        case 86: // HBT
        case 87: // CT
        case 88: // Barbarian
        case 89: // AGC
        case 90: // Gail
        case 106: // Earth Wyvern
        case 107: // Wind Wyvern
        case 108: // Dark Wyvern
        case 109: // Grass Wyvern
        case 110: // Sky Wyvern
        case 111: // Sea Wyvern
        case 112: // Metal Wyvern
        case 113: // Void Wyvern
        case 114: // Eternal Dragon
        case 122: // Enraged Troll
                  //case 123: // Enraged Cyclops
        case 124: // Enraged Stalker
        case 125: // Enraged Gagoyle
        case 126: // Enraged Hellclaw
        case 127: // Enraged Tigerworm
            break;

        default:
            _tmp_cFrame = _tmp_cFrame / 2;
            break;
    }
    if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;
    if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX + dx, sY + dy, _tmp_cFrame);

    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (_tmp_sOwnerType == 65) // IceGolem
    {	/*m_pEffectSpr[77]->PutTransSprite70(sX+dx, sY+dy, _tmp_cFrame, dwTime);*/
        switch (rand() % 3)
        {
            case 0:	m_pEffectSpr[76]->PutTransSprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime); break;
            case 1:	m_pEffectSpr[77]->PutTransSprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime); break;
            case 2:	m_pEffectSpr[78]->PutTransSprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime); break;
        }
    }
    if (bTrans == false)
    {
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
            case 127: // Enraged Tigerworm
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
        }
        CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);
        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX + dx, sY + dy, 1, dwTime);

            if (_tmp_sOwnerType == 81) // Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            else if (bInv == true)
                //m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }
                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }
        }
        else
        {
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX + dx, sY + dy, 1, dwTime);

            if (_tmp_sOwnerType == 81) // Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            else if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }
                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }

        if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 6, -2, 6, dwTime);

        // Berserk
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
        DrawAngel(40 + (_tmp_cDir - 1), sX + dx + 20, sY + dy - 20, _tmp_cFrame % 4, dwTime);
        CheckActiveAura2(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    _tmp_dx = dx;
    _tmp_dy = dy;
    // Snoopy: Abaddon effects
    if (_tmp_sOwnerType == 81)
    {
        int randFrame = _tmp_iEffectFrame % 12;
        m_pEffectSpr[154]->PutTransSprite70(sX - 50, sY - 50, randFrame, dwTime);
        m_pEffectSpr[155]->PutTransSprite70(sX - 20, sY - 80, randFrame, dwTime);
        m_pEffectSpr[156]->PutTransSprite70(sX + 70, sY - 50, randFrame, dwTime);
        m_pEffectSpr[157]->PutTransSprite70(sX - 30, sY, randFrame, dwTime);
        m_pEffectSpr[158]->PutTransSprite70(sX - 60, sY + 90, randFrame, dwTime);
        m_pEffectSpr[159]->PutTransSprite70(sX + 65, sY + 85, randFrame, dwTime);
        switch (_tmp_cDir)
        {
            case 1:
                m_pEffectSpr[153]->PutTransSprite70(sX + dx, sY + dy + 108, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX + dx - 50, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 2:
                m_pEffectSpr[153]->PutTransSprite70(sX + dx, sY + dy + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX + dx - 70, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 3:
                m_pEffectSpr[153]->PutTransSprite70(sX + dx, sY + dy + 105, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX + dx - 90, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 4:
                m_pEffectSpr[153]->PutTransSprite70(sX + dx - 35, sY + dy + 100, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX + dx - 80, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 5:
                m_pEffectSpr[153]->PutTransSprite70(sX + dx, sY + dy + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX + dx - 65, sY + dy - 5, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 6:
                m_pEffectSpr[153]->PutTransSprite70(sX + dx + 45, sY + dy + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX + dx - 31, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 7:
                m_pEffectSpr[153]->PutTransSprite70(sX + dx + 40, sY + dy + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX + dx - 30, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 8:
                m_pEffectSpr[153]->PutTransSprite70(sX + dx + 20, sY + dy + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX + dx - 20, sY + dy + 16, _tmp_iEffectFrame % 15, dwTime);
                break;
        }
    }
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
    return false;
}

bool CGame::DrawObject_OnDamageMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    int cFrame, cDir;
    int dx, dy;
    int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iShieldIndex, iMantleIndex;
    bool bInv = false;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 67 || _tmp_sOwnerType == 68 || _tmp_sOwnerType == 69 || _tmp_sOwnerType == 81) return false;
    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }
    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);
    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (player_name == _tmp_cName) bInv = true;
        else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
        else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
        else return false;
    }
    cDir = _tmp_cDir;
    switch (_tmp_cDir)
    {
        case 1: _tmp_cDir = 5; break;
        case 2: _tmp_cDir = 6; break;
        case 3: _tmp_cDir = 7; break;
        case 4: _tmp_cDir = 8; break;
        case 5: _tmp_cDir = 1; break;
        case 6: _tmp_cDir = 2; break;
        case 7: _tmp_cDir = 3; break;
        case 8: _tmp_cDir = 4; break;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);
            iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 10;
            iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;
            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                // xRisenx
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 10;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 10;*/
            }
            // xRisenx
            if (_tmp_sArmApprValue == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 10;
            /*if ((_tmp_sAppr3 & 0x000F) == 0)
            iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 10;*/
            // xRisenx
            if (_tmp_sLegApprValue == 0)
                iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 10;
            /*if ((_tmp_sAppr3 & 0x0F00) == 0)
            iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 10;*/
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;
            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 5;
            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;
            // xRisenx
            if (_tmp_sHeadApprValue == 0)
                iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 10;
            /*if ((_tmp_sAppr3 & 0x00F0) == 0)
            iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 10;*/
            break;
        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);
            iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 10;
            iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;
            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                // xRisenx
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 10;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 10;*/
            }
            // xRisenx
            if (_tmp_sArmApprValue == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 10;
            /*if ((_tmp_sAppr3 & 0x000F) == 0)
            iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 10;*/
            // xRisenx
            if (_tmp_sLegApprValue == 0)
                iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 10;
            /*if ((_tmp_sAppr3 & 0x0F00) == 0)
            iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 10;*/
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;
            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 5;
            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;
            // xRisenx
            if (_tmp_sHeadApprValue == 0)
                iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 10;
            /*if ((_tmp_sAppr3 & 0x00F0) == 0)
            iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 10;*/
            break;
        default:
            if (_tmp_sOwnerType == 66)      iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
            else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);// Ne devrait pas arriver!
            else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);// Ne devrait pas arriver!
            else if (_tmp_sOwnerType == 106) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Earth Wyvern
            else if (_tmp_sOwnerType == 107) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Wind Wyvern
            else if (_tmp_sOwnerType == 108) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Dark Wyvern
            else if (_tmp_sOwnerType == 109) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Grass Wyvern
            else if (_tmp_sOwnerType == 110) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sky Wyvern
            else if (_tmp_sOwnerType == 111) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sea Wyvern
            else if (_tmp_sOwnerType == 112) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Metal Wyvern
            else if (_tmp_sOwnerType == 113) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Void Wyvern
            else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
            iUndiesIndex = -1;
            iHairIndex = -1;
            iArmArmorIndex = -1;
            iBodyArmorIndex = -1;
            iPantsIndex = -1;
            iBootsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }
    dx = 0;
    dy = 0;
    switch (_tmp_cDir)
    {
        case 1: dy = 28 - (_tmp_cFrame << 2); break;
        case 2: dy = 28 - (_tmp_cFrame << 2); dx = (_tmp_cFrame << 2) - 28; break;
        case 3: dx = (_tmp_cFrame << 2) - 28; break;
        case 4: dx = (_tmp_cFrame << 2) - 28; dy = (_tmp_cFrame << 2) - 28; break;
        case 5: dy = (_tmp_cFrame << 2) - 28; break;
        case 6: dy = (_tmp_cFrame << 2) - 28; dx = 28 - (_tmp_cFrame << 2); break;
        case 7: dx = 28 - (_tmp_cFrame << 2); break;
        case 8: dx = 28 - (_tmp_cFrame << 2); dy = 28 - (_tmp_cFrame << 2); break;
    }
    cFrame = _tmp_cFrame;
    if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;
    if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX + dx, sY + dy, cFrame);
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->PutTransSprite(sX + dy, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
            case 127: // Enraged Tigerworm
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, cFrame, dwTime);
                }
                break;
        }

        CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);
        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
            }
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
        }
        else
        {
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
            }
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }

        if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dx, cFrame, 6, -2, 6, dwTime);

        if ((_tmp_iStatus & 0x20) != 0) 	// Berserk
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, 0, -5, -5, dwTime);
        DrawAngel(16 + (_tmp_cDir - 1), sX + dx + 20, sY + dy - 20, cFrame % 4, dwTime);
        CheckActiveAura2(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
    }
    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    _tmp_dx = dx;
    _tmp_dy = dy;
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
    return false;
}

bool CGame::DrawObject_OnMove_ForMenu(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    short dx, dy;
    int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iShieldIndex, iAdd, iMantleIndex;
    bool bInv = false;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
    iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
    iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
    iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
    iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
    iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
    iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
    iHelmColor = (_tmp_iApprColor & 0x0000000F);

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iAdd = 3;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // Re-Coding Sprite xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + iAdd;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else
                    {	iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;
                    }*/
                }
                else iBodyArmorIndex = -1;
                // Re-Coding Sprite xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + iAdd;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
                // Re-Coding Sprite xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 3;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // Re-Coding Sprite xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 2;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 2;*/
                }
                else iBodyArmorIndex = -1;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 2;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 2;*/
                // Re-Coding Sprite xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 2;
                // Re-Coding Sprite xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 2;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 2;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;
                // Re-Coding Sprite xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 2;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 2;*/
            }
            break;
        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iAdd = 3;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // Re-Coding Sprite xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + iAdd;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
                }
                else  iBodyArmorIndex = -1;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + iAdd;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
                // Re-Coding Sprite xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + iAdd;
                // Re-Coding Sprite xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + iAdd;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 3;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + iAdd;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;
                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    // Re-Coding Sprite xRisenx
                    if (_tmp_sBodyApprValue == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 2;
                    /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                    else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 2;*/
                }
                else iBodyArmorIndex = -1;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 2;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 2;*/
                // Re-Coding Sprite xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 2;
                // Re-Coding Sprite xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 2;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 2;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;
                // Re-Coding Sprite xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 2;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 2;*/
            }
            break;
        default:
            iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
            iUndiesIndex = -1;
            iHairIndex = -1;
            iBodyArmorIndex = -1;
            iArmArmorIndex = -1;
            iBootsIndex = -1;
            iPantsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iHelmIndex = -1;
            break;
    }
    dx = 0;
    dy = 0;
    if (_cDrawingOrder[_tmp_cDir] == 1)
    {
        if (iWeaponIndex != -1)
        {
            if (iWeaponColor == 0)
                m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
        }
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
            case 127: // Enraged Tigerworm
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
        }
        if (bInv == true)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);


        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }
        if (iUndiesIndex != -1)
        {
            if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
        }

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
        }
        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }
        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }
        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }
        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }
        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }
        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }
        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iShieldIndex != -1)
        {
            if (iShieldColor == 0)
                m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
        }
        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }
    }
    else
    {
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 127: // Enraged Tigerworm
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
        }

        if (bInv == true)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);

        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }
        if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
        }
        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }
        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }
        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }
        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }
        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }
        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }
        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iShieldIndex != -1)
        {
            if (iShieldColor == 0)
                m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
        }
        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }
        if (iWeaponIndex != -1)
        {
            if (iWeaponColor == 0)
                m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
        }
    }

    if (_tmp_iChatIndex != 0)
    {
        if (m_pChatMsgList[_tmp_iChatIndex] != 0)
        {
            DrawChatMsgBox(sX + dx, sY + dy, _tmp_iChatIndex, false);
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    _tmp_dx = dx;
    _tmp_dy = dy;
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
    return false;
}


bool   CGame::DrawObject_OnStop(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iBodyArmorIndex, iArmArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iShieldIndex, iMantleIndex;
    bool bInv = false;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66 || _tmp_sOwnerType == 81) bInv = true; //Energy-Ball, Wyvern
    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);
    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (player_name == _tmp_cName) bInv = true;
        else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
        else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
        else return false;
    }

    // CLEROTH - Single-direction monsters
    switch (_tmp_sOwnerType)
    {
        case 110: // Air Elemental
            _tmp_cDir = 1; // North
            break;
        case 91: // Snoopy: Gate
            if (_tmp_cDir <= 3) _tmp_cDir = 3;
            else  _tmp_cDir = 5;
            break;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            _tmp_cFrame = _tmp_cFrame / 2;
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (1 * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 1;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 1;
                //if ((_tmp_sAppr4 & 0x80) == 0)
                //{	
                // Re-Coding Sprite xRisenx
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 1;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 1;*/
                //}
                // Re-Coding Sprite xRisenx
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 1;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 1;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 1;*/
                // Re-Coding Sprite xRisenx
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 1;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 1;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 1 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 1;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 1;
                // Re-Coding Sprite xRisenx
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 1;
                //if (_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 1;*/
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);
                iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15;
                //if ((_tmp_sAppr4 & 0x80) == 0)
                //{	
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15;*/
                //}
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15;*/

                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15;
                /*if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15;*/
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 0 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 0;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15;
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 0;
                //if (_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 0;*/
            }
            break;

        case 4:
        case 5:
        case 6:
            _tmp_cFrame = _tmp_cFrame / 2;
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (1 * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 1;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 1;
                //if ((_tmp_sAppr4 & 0x80) == 0)
                //{	
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 1;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 1;*/
                //}
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 1;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 1;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 1;*/
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 1;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 1;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 1 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 1;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 1;
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 1;
                //if (_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0;
                /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 1;*/
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);
                iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15;
                //if ((_tmp_sAppr4 & 0x80) == 0)
                //{	
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15; // 11800 + (6 * 15) = 11891
                                                                                     /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                                                                                     iBodyArmorIndex = -1;
                                                                                     else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15;*/
                                                                                     //}
                if (_tmp_sArmApprValue == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15;
                /*if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
                else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15;*/
                if (_tmp_sLegApprValue == 0)
                    iPantsIndex = -1;
                else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15;
                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 0 + (_tmp_cDir - 1);
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 0;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15;
                if (_tmp_sHeadApprValue == 0)
                    iHelmIndex = -1;
                else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 0; // 12800 + (4096 * 15) = 74241 [CRASH POINT]
                                                                               //if (_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0;
                                                                               /*if ((_tmp_sAppr3 & 0x00F0) == 0)
                                                                               iHelmIndex = -1;
                                                                               else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 0;*/
            }
            break;
        case 20: // Howard
        case 24: // Tom
            _tmp_sAppr2 = 0;
        default:
            if (_tmp_sAppr2 != 0)
            {
                iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                _tmp_cFrame = (_tmp_sAppr2 & 0x00FF) - 1;
            }
            /*	else if (_tmp_sOwnerType == 66) iBodyIndex =  SPRID_MOB  +  (_tmp_sOwnerType - 10 )*8*7 + (0 * 8);
            else if (_tmp_sOwnerType == 73) iBodyIndex =  SPRID_MOB  +  (_tmp_sOwnerType - 10 )*8*7 + (0 * 8);
            else if (_tmp_sOwnerType == 81) iBodyIndex =  SPRID_MOB  +  (_tmp_sOwnerType - 10 )*8*7 + (0 * 8);*/
            else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            iUndiesIndex = -1;
            iHairIndex = -1;
            iBodyArmorIndex = -1;
            iArmArmorIndex = -1;
            iBootsIndex = -1;
            iPantsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }
    if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

    if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX, sY, _tmp_cFrame);
    switch (_tmp_sOwnerType)
    { // hum? la lumiere en dessous ?
        case 15: // ShopKeeper
        case 19: // Gandalf
        case 20: // Howard
        case 24: // Tom
        case 25: // William
        case 26: // Kenedy
        case 51: // CP
        case 86: // HBT
        case 90: // Gail
        case NPC_PRINCESS:
            m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);
            break;
    }
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }
    if (bTrans == false)
    {
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
            case 127: // Enraged Tigerworm
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
        }
        CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);
        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX, sY, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

            if (_tmp_sOwnerType == 81) // Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);

            }
            else if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    //else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir-1) * 8 + _tmp_cFrame, m_wR[iPantsColor] -m_wR[0], m_wG[iPantsColor] -m_wG[0], m_wB[iPantsColor] -m_wB[0], dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 255, 0, 0, dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }
                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }
        }
        else
        {
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);
            if (_tmp_sOwnerType == 81) // Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
            }
            else if (bInv == true)
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
            }
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    //			else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir-1) * 8 + _tmp_cFrame, m_wR[iPantsColor] -m_wR[0], m_wG[iPantsColor] -m_wG[0], m_wB[iPantsColor] -m_wB[0], dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 255, 0, 0, dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        // 11896 = iBodyArmorIndex value for last sprite crash
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }
                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX, sY, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }
        if (_tmp_sOwnerType == 64) // crop
        {
            switch (_tmp_cFrame)
            {
                case 0: // color effect for crop
                    m_pEffectSpr[84]->PutTransSprite(sX + 52, sY + 54, (dwTime % 3000) / 120, dwTime);
                    break;
                case 1: // color effect for crop
                    m_pEffectSpr[83]->PutTransSprite(sX + 53, sY + 59, (dwTime % 3000) / 120, dwTime);
                    break;
                case 2: // color effect for crop
                    m_pEffectSpr[82]->PutTransSprite(sX + 53, sY + 65, (dwTime % 3000) / 120, dwTime);
                    break;
            }
        }

        if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 6, -2, 6, dwTime);

        // Berserk
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        DrawAngel(40 + (_tmp_cDir - 1), sX + 20, sY - 20, _tmp_cFrame % 4, dwTime);
        CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else
        {
            DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
        }
#ifdef _DEBUG
        DrawStatusText(sX, sY);
#endif
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    // Snoopy: Abaddon effects
    if (_tmp_sOwnerType == 81)
    {
        int randFrame = _tmp_cFrame % 12;
        m_pEffectSpr[154]->PutTransSprite70(sX - 50, sY - 50, randFrame, dwTime);
        m_pEffectSpr[155]->PutTransSprite70(sX - 20, sY - 80, randFrame, dwTime);
        m_pEffectSpr[156]->PutTransSprite70(sX + 70, sY - 50, randFrame, dwTime);
        m_pEffectSpr[157]->PutTransSprite70(sX - 30, sY, randFrame, dwTime);
        m_pEffectSpr[158]->PutTransSprite70(sX - 60, sY + 90, randFrame, dwTime);
        m_pEffectSpr[159]->PutTransSprite70(sX + 65, sY + 85, randFrame, dwTime);
        switch (_tmp_cDir)
        {
            case 1:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 108, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 50, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 2:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 70, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 3:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 105, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 90, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 4:
                m_pEffectSpr[153]->PutTransSprite70(sX - 35, sY + 100, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 80, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 5:
                m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 65, sY - 5, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 6:
                m_pEffectSpr[153]->PutTransSprite70(sX + 45, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 31, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 7:
                m_pEffectSpr[153]->PutTransSprite70(sX + 40, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 30, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 8:
                m_pEffectSpr[153]->PutTransSprite70(sX + 20, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->PutTransSprite70(sX - 20, sY + 16, _tmp_iEffectFrame % 15, dwTime);
                break;
        }
    }
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
    return false;
}


bool   CGame::DrawObject_OnRun(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
    int dx, dy;
    int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iWeaponIndex, iShieldIndex, iHelmIndex, iR, iG, iB, iMantleIndex;
    bool bInv = false;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }
    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);
    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (player_name == _tmp_cName) bInv = true;
        else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
        else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
        else return false;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (4 * 8);
            iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 4;
            iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 4;
            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                // xRisenx
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 4;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 4;*/
            }
            // xRisenx
            if (_tmp_sArmApprValue == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 4;
            /*if ((_tmp_sAppr3 & 0x000F) == 0)
            iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 4;*/
            // xRisenx
            if (_tmp_sLegApprValue == 0)
                iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 4;
            /*if ((_tmp_sAppr3 & 0x0F00) == 0)
            iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 4;*/
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 4;
            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else
            {
                iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 6 + (_tmp_cDir - 1);
            }
            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 6;
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 4;
            // xRisenx
            if (_tmp_sHeadApprValue == 0)
                iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 4;
            /*if ((_tmp_sAppr3 & 0x00F0) == 0)
            iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 4;*/
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (4 * 8);
            iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 4;
            iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 4;
            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                // xRisenx
                if (_tmp_sBodyApprValue == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 4;
                /*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                iBodyArmorIndex = -1;
                else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 4;*/
            }
            // xRisenx
            if (_tmp_sArmApprValue == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 4;
            /*if ((_tmp_sAppr3 & 0x000F) == 0)
            iArmArmorIndex = -1;
            else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 4;*/
            // xRisenx
            if (_tmp_sLegApprValue == 0)
                iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 4;
            /*if ((_tmp_sAppr3 & 0x0F00) == 0)
            iPantsIndex = -1;
            else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 4;*/
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 4;
            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else
            {
                iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 6 + (_tmp_cDir - 1);
            }
            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 6;
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 4;
            // xRisenx
            if (_tmp_sHeadApprValue == 0)
                iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 4;
            /*if ((_tmp_sAppr3 & 0x00F0) == 0)
            iHelmIndex = -1;
            else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 4;*/
            break;

        default:
            iUndiesIndex = -1;
            iHairIndex = -1;
            iArmArmorIndex = -1;
            iBodyArmorIndex = -1;
            iPantsIndex = -1;
            iBootsIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }
    dx = 0;
    dy = 0;
    switch (_tmp_cDir)
    {
        case 1: dy = 28 - (_tmp_cFrame << 2); break;
        case 2: dy = 28 - (_tmp_cFrame << 2); dx = (_tmp_cFrame << 2) - 28; break;
        case 3: dx = (_tmp_cFrame << 2) - 28; break;
        case 4: dx = (_tmp_cFrame << 2) - 28; dy = (_tmp_cFrame << 2) - 28; break;
        case 5: dy = (_tmp_cFrame << 2) - 28; break;
        case 6: dy = (_tmp_cFrame << 2) - 28; dx = 28 - (_tmp_cFrame << 2); break;
        case 7: dx = 28 - (_tmp_cFrame << 2); break;
        case 8: dx = 28 - (_tmp_cFrame << 2); dy = 28 - (_tmp_cFrame << 2); break;
    }
    if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

    if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX + dx, sY + dy, _tmp_cFrame);

    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        switch (_tmp_sOwnerType)
        { // Pas d'ombre pour ces mobs
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
            case 106: // Earth Wyvern
            case 107: // Wind Wyvern
            case 108: // Dark Wyvern
            case 109: // Grass Wyvern
            case 110: // Sky Wyvern
            case 111: // Sea Wyvern
            case 112: // Metal Wyvern
            case 113: // Void Wyvern
            case 114: // Eternal Dragon
            case 127: // Enraged Tigerworm
                break;
            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
        }
        CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);
        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if (bInv == true)
                //m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }
                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime); // GM effect
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }
        }
        else
        {
            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & STATUS_FROZEN) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }
                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }
                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }

        if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dx, _tmp_cFrame, 6, -2, 6, dwTime);

        if ((_tmp_iStatus & 0x20) != 0) 	// Berserk
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
        DrawAngel(40 + (_tmp_cDir - 1), sX + dx + 20, sY + dy - 20, _tmp_cFrame % 4, dwTime);
        CheckActiveAura2(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    _tmp_dx = dx;
    _tmp_dy = dy;
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1)
        && (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY)
        && (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY)
        && (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX)
        && (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
    return false;
}


void CGame::_Draw_CharacterBody(short sX, short sY, short sType)
{
    uint64_t dwTime = m_dwCurTime;
    int  iR, iG, iB;

    if (sType <= 3)
    {
        m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 0]->PutSpriteFast(sX, sY, sType - 1, dwTime);
        _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
        m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 18]->PutSpriteRGB(sX, sY, (_tmp_sAppr1 & 0x0F00) >> 8, iR, iG, iB, dwTime);

        m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 19]->PutSpriteFast(sX, sY, (_tmp_sAppr1 & 0x000F), dwTime);
    }
    else
    {
        m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 40]->PutSpriteFast(sX, sY, sType - 4, dwTime);
        _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
        m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 18 + 40]->PutSpriteRGB(sX, sY, (_tmp_sAppr1 & 0x0F00) >> 8, iR, iG, iB, dwTime);
        m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 19 + 40]->PutSpriteFast(sX, sY, (_tmp_sAppr1 & 0x000F), dwTime);
    }
}
