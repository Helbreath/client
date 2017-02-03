

#include "newsocket.h"
#include "Game.h"
#include "..\shared\buffer.h"
#include <algorithm>
#include <iostream>

#include "lan_eng.h"
#include <boost/asio/ssl.hpp>



extern class CGame * G_pGame;

extern IrrlichtDevice * device;
extern video::IVideoDriver * driver;
extern scene::ISceneManager* smgr;

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
extern int iHungerStatus; // Hunger Bar xRisenx
extern bool m_bStuckHack; // Anti Hack xRisenx


void CGame::DrawDialogBox_Text()
{
    short sX, sY;
    int i, iTotalLines;

    sX = m_dialogBoxes[18].m_X;
    sY = m_dialogBoxes[18].m_Y;

    iTotalLines = 0;
    for (i = 0; i < TEXTDLGMAXLINES; i++)
        if (m_pMsgTextList[i] != 0) iTotalLines++;

    Point pointerLoc = m_dialogBoxes[18].HandleScroll(17, iTotalLines);

    if (iTotalLines > 17)
    {
        DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 1);
        DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
    }

    for (i = 0; i < 17; i++)
        if (m_pMsgTextList[i + m_dialogBoxes[18].sView] != 0)
        {
            if (m_bDialogTrans == false)
            {
                switch (m_pMsgTextList[i + m_dialogBoxes[18].sView]->m_pMsg[0])
                {
                    case '_': PutAlignedString(sX + 24, sX + 236, sY + 50 + i * 13, (m_pMsgTextList[i + m_dialogBoxes[18].sView]->m_pMsg + 1), video::SColor(255, 255, 255, 255)); break;
                    case ';': PutAlignedString(sX + 24, sX + 236, sY + 50 + i * 13, (m_pMsgTextList[i + m_dialogBoxes[18].sView]->m_pMsg + 1), video::SColor(255, 4, 0, 50)); break;
                    default: PutAlignedString(sX + 24, sX + 236, sY + 50 + i * 13, m_pMsgTextList[i + m_dialogBoxes[18].sView]->m_pMsg, video::SColor(255, 45, 25, 25)); break;
                }
            }
            else PutAlignedString(sX + 24, sX + 236, sY + 50 + i * 13, m_pMsgTextList[i + m_dialogBoxes[18].sView]->m_pMsg, video::SColor(255, 0, 0, 0));
        }

    if (m_dialogBoxes[18].OnButton() == 1)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
}

void CGame::DrawDialogBox_15AgeMsg()
{
    return;
    // Snoopy: removed FeedbackCard & minus 15 age

}


void CGame::DrawDialogBox_WarningMsg()
{
    short sX, sY;
    char onButton = m_dialogBoxes[6].OnButton();

    sX = m_dialogBoxes[6].m_X;
    sY = m_dialogBoxes[6].m_Y;


    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, sX, sY, 2);

    PutString2(sX + 72, sY + 32, MSG_WARNING1, 200, 200, 25);//" ** This is a battle area **"
    PutString(sX + 30, sY + 55, MSG_WARNING2, video::SColor(255, 220, 130, 45));//"This is a dangerous area where you"
    PutString(sX + 30, sY + 72, MSG_WARNING3, video::SColor(255, 220, 130, 45));//"cannot protected from others' attack."
    PutString(sX + 30, sY + 90, MSG_WARNING4, video::SColor(255, 220, 130, 45));//"To play the game in safe, go to the"
    PutString(sX + 30, sY + 108, MSG_WARNING5, video::SColor(255, 220, 130, 45));//" cityhall and change to civilian mode."

    if (onButton == 1)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 122, sY + 127, 1);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 122, sY + 127, 0);

}

void CGame::DrawDialogBox_ItemDrop()
{
    short sX, sY;
    char cTxt[120], cStr1[64], cStr2[64], cStr3[64];

    char onButton = m_dialogBoxes[4].OnButton();
    sX = m_dialogBoxes[4].m_X;
    sY = m_dialogBoxes[4].m_Y;

    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME1, sX, sY, 2);

    GetItemName(m_pItemList[m_dialogBoxes[4].sView]->m_cName, m_pItemList[m_dialogBoxes[4].sView]->m_dwAttribute, cStr1, cStr2, cStr3);

    if (strlen(m_dialogBoxes[4].cStr) == 0)  wsprintfA(cTxt, "%s", cStr1);

    if (m_bIsSpecial)
    {
        PutString3(sX + 35, sY + 20, cTxt, video::SColor(255, 0, 255, 0));
    }
    else
    {
        PutString3(sX + 35, sY + 20, cTxt, video::SColor(255, 4, 0, 50));
    }
    PutString3(sX + 35, sY + 36, DRAW_DIALOGBOX_ITEM_DROP1, video::SColor(255, 4, 0, 50));

    if (m_bItemDrop)
    {
        if (onButton == 3)
        {
            PutString3(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP2, video::SColor(255, 255, 255, 255));
        }
        else
        {
            PutString3(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP2, video::SColor(255, 4, 0, 50));
        }
    }
    else
    {
        if (onButton == 3)
        {
            PutString3(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP3, video::SColor(255, 255, 255, 255));
        }
        else
        {
            PutString3(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP3, video::SColor(255, 4, 0, 50));
        }
    }

    if (onButton == 1)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 19);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 18);

    if (onButton == 2)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 3);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 2);
}

void CGame::DrawDialogBox_NpcTalk()
{
    short sX, sY;
    int i, iTotalLines;
    double d1, d2, d3;

    char onButton = m_dialogBoxes[4].OnButton();
    sX = m_dialogBoxes[21].m_X;
    sY = m_dialogBoxes[21].m_Y;
    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);

    switch (m_dialogBoxes[21].GetMode())
    {
        case 0: //  OK
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 1: // Accept / Decline
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 33);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 32);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 41);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 40);
            break;

        case 2: // Next
            if (onButton == 1)
                PutString_SprFont(sX + RBTNPOSX, sY + BTNPOSY, "Next", 6, 6, 20);
            else PutString_SprFont(sX + RBTNPOSX, sY + BTNPOSY, "Next", 0, 0, 7);
            break;
    }

    for (i = 0; i < 17; i++)
        if ((i < TEXTDLGMAXLINES) && (m_pMsgTextList2[i + m_dialogBoxes[21].sView] != 0))
        {
            PutAlignedString(sX, sX + m_dialogBoxes[21].sSizeX, sY + 57 + i * 15, m_pMsgTextList2[i + m_dialogBoxes[21].sView]->m_pMsg, video::SColor(255, 45, 25, 25));
        }

    iTotalLines = 0;
    for (i = 0; i < TEXTDLGMAXLINES; i++)
        if (m_pMsgTextList2[i] != 0) iTotalLines++;

    Point pointerLoc = m_dialogBoxes[21].HandleScroll(17, iTotalLines);
    if (iTotalLines > 17)
    {
        DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
        DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
    }
}

void CGame::DrawDialogBox_Slates()
{
    int iAdjX, iAdjY;
    short sX, sY;
    uint32_t dwTime = m_dwCurTime;

    iAdjX = 5;
    iAdjY = 8;

    switch (m_dialogBoxes[40].GetMode())
    {
        // Slates Dialog - Diuuude
        case 1:
            sX = m_dialogBoxes[40].m_X;
            sY = m_dialogBoxes[40].m_Y;
            iAdjX = -1;
            iAdjY = -7;

            DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX, sY, 4);

            if (m_dialogBoxes[40].sV1 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX + 20, sY + 12, 5);
            }
            if (m_dialogBoxes[40].sV2 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX + 20, sY + 87, 6);
            }
            if (m_dialogBoxes[40].sV3 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX + 85, sY + 32, 7);
            }
            if (m_dialogBoxes[40].sV4 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX + 70, sY + 97, 8);
            }

            if ((m_dialogBoxes[40].sV1 != -1) && (m_dialogBoxes[40].sV2 != -1) && (m_dialogBoxes[40].sV3 != -1) && (m_dialogBoxes[40].sV4 != -1))
            {
                if (m_dialogBoxes[40].OnButton() == 1)
                    PutString_SprFont(sX + 120, sY + 150, "Casting", 6, 6, 20);
                else PutString_SprFont(sX + 120, sY + 150, "Casting", 0, 0, 7);
            }

            break;

            // Slates Dialog - Diuuude
        case 2:
            PlaySound('E', 16, 0);
            if (m_dialogBoxes[40].cStr[0] != 0)
            {
                sX = m_dialogBoxes[40].m_X + iAdjX + (m_dialogBoxes[40].cStr[0] - (rand() % (m_dialogBoxes[40].cStr[0] * 2)));
                sY = m_dialogBoxes[40].m_Y + iAdjY + (m_dialogBoxes[40].cStr[0] - (rand() % (m_dialogBoxes[40].cStr[0] * 2)));
            }
            else
            {
                sX = m_dialogBoxes[40].m_X;
                sY = m_dialogBoxes[40].m_Y;
            }
            m_pSprite[SPRID_INTERFACE_ND_INVENTORY]->PutSpriteFast(sX, sY, 4, dwTime);
            m_pSprite[SPRID_INTERFACE_ND_INVENTORY]->PutSpriteFast(sX + 22, sY + 14, 3, dwTime);
            //PutString_SprFont(sX + iAdjX + 170, sY + iAdjY + 170, "KURURURURURURURURU!!!", 20,6,6);
            PutAlignedString(199, 438, 201, "KURURURURURURURURU!!!", video::SColor(255, 220, 140, 160));
            PutAlignedString(200, 439, 200, "KURURURURURURURURU!!!", video::SColor(255, 90, 220, 200));

            if ((dwTime - m_dialogBoxes[40].dwT1) > 1000)
            {
                m_dialogBoxes[40].dwT1 = dwTime;
                m_dialogBoxes[40].cStr[0]++;
            }
            if (m_dialogBoxes[40].cStr[0] >= 5)
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_CREATESLATE, 0, m_dialogBoxes[40].sV1, m_dialogBoxes[40].sV2, m_dialogBoxes[40].sV3, 0, m_dialogBoxes[40].sV4);
                DisableDialogBox(40);
            }
            break;
    }
}

void CGame::DlgBoxClick_Slates()
{
    switch (m_dialogBoxes[40].GetMode())
    {
        case 1:
            if ((m_dialogBoxes[40].sV1 != -1) && (m_dialogBoxes[40].sV2 != -1) && (m_dialogBoxes[40].sV3 != -1) && (m_dialogBoxes[40].sV4 != -1))
            {
                if (m_dialogBoxes[40].OnButton() == 1)
                {
                    m_dialogBoxes[40].SetMode(2);
                    PlaySound('E', 14, 5);
                }
            }
            break;
    }
}

void CGame::DlgBoxClick_NpcTalk()
{
    char onButton = m_dialogBoxes[21].OnButton();
    switch (m_dialogBoxes[21].GetMode())
    {
        case 0: //  OK
            if (onButton == 1)
            {
                DisableDialogBox(21);
            }
            break;

        case 1: // Accept / Decline
            if (onButton == 1)
            {
                // Accept
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_QUESTACCEPTED, 0, 0, 0, 0, 0);
                DisableDialogBox(21);
            }
            else if (onButton == 2)
            {
                // Cancel
                DisableDialogBox(21);
            }
            break;

        case 2:	// Next
            if (onButton == 1)
            {
                DisableDialogBox(21);
            }
            break;
    }
    if (onButton)PlaySound('E', 14, 5);
}


void CGame::DrawDialogBox_Chat()
{
    char G_cTxt[120];

    CDialogBox & dlg = m_dialogBoxes[DIALOG_CHAT];
    char onButton = dlg.OnButton();
    short sX = dlg.m_X;
    short sY = dlg.m_Y;

    if (G_pGame->m_stMCursor.sZ != 0 && dlg.IsTop())
    {
        dlg.sView = dlg.sView + G_pGame->m_stMCursor.sZ/*/30*/;
        ////DIRECTX m_dInput.m_sZ = 0;//DIRECTX
    }
    if (dlg.sView < 0) dlg.sView = 0;
    if (dlg.sView > MAXCHATSCROLLMSGS - 8) dlg.sView = MAXCHATSCROLLMSGS - 8;

    //uncomment	ButtonStringToggle(dlg, 2, DRAW_CHAT1, m_chatToggle[CHAT_SHOUT]);
    //uncomment	ButtonStringToggle(dlg, 3, DRAW_CHAT2, m_chatToggle[CHAT_NATIONSHOUT]);
    //uncomment	ButtonStringToggle(dlg, 4, DRAW_CHAT3, m_chatToggle[CHAT_NORMAL]);
    //uncomment	ButtonStringToggle(dlg, 5, DRAW_CHAT4, m_chatToggle[CHAT_GUILD]);
    //uncomment	ButtonStringToggle(dlg, 6, DRAW_CHAT5, m_chatToggle[CHAT_PARTY]);
    //uncomment	ButtonStringToggle(dlg, 7, DRAW_CHAT6, m_chatToggle[CHAT_WHISPER]);
    //uncomment	ButtonStringToggle(dlg, 8, DRAW_CHAT7, m_chatToggle[CHAT_GM]);
    //uncomment	ButtonStringToggle(dlg, 9, DRAW_CHAT8);

    uint32_t chatsize = m_chatDisplay->size();

    if (dlg.sView >= chatsize)
        dlg.sView = (chatsize == 0) ? 0 : chatsize - 1;

    double d1 = (double)dlg.sView;
    double d2 = (double)(105);
    double d3 = (d1*d2) / (MAXCHATSCROLLMSGS - 8);
    int iPointerLoc = (int)d3;
    iPointerLoc = 105 - iPointerLoc;
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX + 346, sY + 33 + iPointerLoc, 7);

    std::vector<CMsg*>::reverse_iterator it = m_chatDisplay->rbegin() + dlg.sView;

    for (int i = 0; i < 8; i++, ++it)
    {
        if (it == m_chatDisplay->rend())
            break;

        CMsg * msg = (*it);

        int32_t r = 0, g = 0, b = 0;
        switch (msg->m_dwTime)
        {
            case CHAT_NORMAL:
                r = 230; g = 230; b = 230;
                break;
            case CHAT_GUILD:
                r = 130; g = 200; b = 130;
                break;
            case CHAT_SHOUT:
                r = 255; g = 130; b = 130;
                break;
            case CHAT_NATIONSHOUT:
                r = 130; g = 130; b = 255;
                break;
            case CHAT_PARTY:
                r = 230; g = 230; b = 130;
                break;
            case CHAT_GM:
                r = 180; g = 255; b = 180;
                break;
            case CHAT_WHISPER:
                r = 150; g = 150; b = 170;
                break;
        }

        PutFontString(font[FONT_TREBMS8PX], sX + 25, sY + 130 - i * 13, msg->m_pMsg, video::SColor(255, r, g, b), false, 1);
    }


    if ((G_pGame->m_stMCursor.LB != 0) && dlg.IsTop())
    {
        if ((G_pGame->m_stMCursor.sX >= sX + 336) && (G_pGame->m_stMCursor.sX <= sX + 361) && (G_pGame->m_stMCursor.sY >= sY + 28) && (G_pGame->m_stMCursor.sY <= sY + 140))
        {
            d1 = (double)(G_pGame->m_stMCursor.sY - (sY + 28));
            d2 = ((MAXCHATSCROLLMSGS - 8)*d1) / 105.0f;
            dlg.sView = MAXCHATSCROLLMSGS - 8 - (int)d2;
        }

        if ((G_pGame->m_stMCursor.sX >= sX + 336) && (G_pGame->m_stMCursor.sX <= sX + 361) && (G_pGame->m_stMCursor.sY > sY + 18) && (G_pGame->m_stMCursor.sY < sY + 28))
            dlg.sView = MAXCHATSCROLLMSGS - 8;

        if ((G_pGame->m_stMCursor.sX >= sX + 336) && (G_pGame->m_stMCursor.sX <= sX + 361) && (G_pGame->m_stMCursor.sY > sY + 140) && (G_pGame->m_stMCursor.sY < sY + 163))
            dlg.sView = 0;
    }
    else dlg.bIsScrollSelected = false;

}

void CGame::DlgBoxClick_Chat()
{
    ChatType t;

    switch (m_dialogBoxes[DIALOG_CHAT].OnButton())
    {
        case 2:
            t = CHAT_SHOUT;
            break;
        case 3:
            t = CHAT_NATIONSHOUT;
            break;
        case 4:
            t = CHAT_NORMAL;
            break;
        case 5:
            t = CHAT_GUILD;
            break;
        case 6:
            t = CHAT_PARTY;
            break;
        case 7:
            t = CHAT_WHISPER;
            break;
        case 8:
            t = CHAT_GM;
            break;
        case 9:
            for (int i = 0; i < CHAT_MAX; i++)
            {
                if (m_chatMsgs[i].size() > 0)
                {
                    EnableChat((ChatType)i, true);
                }
                m_chatToggle[i] = true;
            }

            return;

        default:
            return;
    }

    EnableChat(t, !m_chatToggle[t]);
}

void CGame::DlgBoxDoubleClick_Chat()
{
    ChatType t;

    switch (m_dialogBoxes[DIALOG_CHAT].OnButton())
    {
        case 2:
            t = CHAT_SHOUT;
            break;
        case 3:
            t = CHAT_NATIONSHOUT;
            break;
        case 4:
            t = CHAT_NORMAL;
            break;
        case 5:
            t = CHAT_GUILD;
            break;
        case 6:
            t = CHAT_PARTY;
            break;
        case 7:
            t = CHAT_WHISPER;
            break;
        case 8:
            t = CHAT_GM;
            break;
        case 9:
        default:
            return;
    }

    for (int i = 0; i < CHAT_MAX; i++)
    {
        if (m_chatToggle[i] && i != t)
        {
            EnableChat((ChatType)i, false);
        }
    }

    EnableChat(t, true);
}

void CGame::DlgBoxClick_ItemUpgrade()
{
    int i, iSoX, iSoM;
    char onButton = m_dialogBoxes[34].OnButton();
    if (onButton) PlaySound('E', 14, 5);

    switch (m_dialogBoxes[34].GetMode())
    {
        case 1:	// Upgrade Majestic, items in the window
            if ((m_dialogBoxes[34].sV1 != -1) && onButton == 1)
            {
                int iValue = (m_pItemList[m_dialogBoxes[34].sV1]->m_dwAttribute & 0xF0000000) >> 28;
                iValue = iValue*(iValue + 6) / 8 + 2;
                if (m_iGizonItemUpgradeLeft < iValue) break;

                PlaySound('E', 44, 0);
                m_dialogBoxes[34].SetMode(2);
                m_dialogBoxes[34].dwV1 = unixtime();
            }
            else if (onButton == 2)
            {	// Cancel
                DisableDialogBox(34);
            }
            break;

        case 3: // Success
        case 4: // Not possible
        case 7: // Lost item
        case 8: // Failed
        case 9: // Failed
        case 10:// Failed
        case 12:// 12 Need stone!
            if (onButton == 1)
            {	// OK
                DisableDialogBox(34);
            }
            break;
        case 5: // main menu
            if (onButton == 1)
            {
                iSoX = iSoM = 0;
                for (i = 0; i < MAXITEMS; i++)
                    if (m_pItemList[i] != 0)
                    {
                        if ((m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 128)) iSoX++;
                        if ((m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 129)) iSoM++;
                    }
                if ((iSoX > 0) || (iSoM > 0))
                {
                    m_dialogBoxes[34].SetMode(6);
                    m_dialogBoxes[34].sV2 = iSoX;
                    m_dialogBoxes[34].sV3 = iSoM;
                }
                else AddEventList(DRAW_DIALOGBOX_ITEMUPGRADE30, 10); //"Stone of Xelima or Merien is not present."
            }
            else if (onButton == 2)
            {
                m_dialogBoxes[34].SetMode(1);
            }
            else if (onButton == 3)
            {	// Cancel
                DisableDialogBox(34);
            }
            else if (onButton == 4) // Upgrade Hero System xRisenx
            {
                m_dialogBoxes[34].SetMode(6);
            }
            break;
        case 6: // Upgrade Xelima / Merien
            if ((m_dialogBoxes[34].sV1 != -1) && onButton == 1)
            {	// Upgrade
                PlaySound('E', 44, 0);
                m_dialogBoxes[34].SetMode(2);
                m_dialogBoxes[34].dwV1 = unixtime();
            }
            if (onButton == 2)
            {	// Cancel
                DisableDialogBox(34);
            }
            break;

        case 13: // Upgrade Hero System xRisenx
            if ((m_dialogBoxes[34].sV1 != -1) && onButton == 1)
            {	// Upgrade
                PlaySound('E', 44, 0);
                m_dialogBoxes[34].SetMode(2);
                m_dialogBoxes[34].dwV1 = unixtime();
            }
            if (onButton == 2)
            {	// Cancel
                DisableDialogBox(34);
            }
            break;
    }
}


void CGame::DlgBoxClick_SellList()
{
    int i, x;
    char onButton = m_dialogBoxes[31].OnButton();
    i = onButton - 1;

    if (onButton > 0 && onButton <= MAXSELLLIST)
    {
        if (m_pItemList[m_stSellItemList[i].iIndex] != 0)
        {
            m_bIsItemDisabled[m_stSellItemList[i].iIndex] = false;
            m_stSellItemList[i].iIndex = -1;

            PlaySound('E', 14, 5);

            for (x = 0; x < MAXSELLLIST - 1; x++)
                if (m_stSellItemList[x].iIndex == -1)
                {
                    m_stSellItemList[x].iIndex = m_stSellItemList[x + 1].iIndex;
                    m_stSellItemList[x].iAmount = m_stSellItemList[x + 1].iAmount;

                    m_stSellItemList[x + 1].iIndex = -1;
                    m_stSellItemList[x + 1].iAmount = 0;
                }
        }
        return;
    }

    if (onButton == MAXSELLLIST + 1) // Sell
    {
        bSendCommand(MSGID_REQUEST_SELLITEMLIST);
        PlaySound('E', 14, 5);
        DisableDialogBox(31);
    }
    else if (onButton == MAXSELLLIST + 2) // Cancel
    {
        PlaySound('E', 14, 5);
        DisableDialogBox(31);
    }
}

void CGame::DlgBoxClick_LevelUpSettings()
{
    short
        cStat = NONE;
    int statChange;

    short msY = G_pGame->m_stMCursor.sY;
    short msX = G_pGame->m_stMCursor.sX;
    short sY = m_dialogBoxes[12].m_Y;
    short sX = m_dialogBoxes[12].m_X;
    char onButton = m_dialogBoxes[12].OnButton();

    if (m_bCtrlPressed == true)	statChange = 10;
    else	statChange = 1;

    if (onButton == 3 || onButton == 4)
        cStat = STAT_STR;
    if (onButton == 5 || onButton == 6)
        cStat = STAT_VIT;
    if (onButton == 7 || onButton == 8)
        cStat = STAT_DEX;
    if (onButton == 9 || onButton == 10)
        cStat = STAT_INT;
    if (onButton == 11 || onButton == 12)
        cStat = STAT_MAG;
    if (onButton == 13 || onButton == 14)
        cStat = STAT_CHR;

    if (cStat != NONE && onButton % 2 == 1)
    {
        if (statChange > m_iLU_Point)
            statChange = m_iLU_Point;

        if (m_stat[cStat] - m_angelStat[cStat] + m_luStat[cStat] + statChange > Stats_Limit)
            //if(m_stat[cStat] - m_angelStat[cStat] + m_luStat[cStat] + statChange > 450)
            //statChange = Stats_Limit - (m_stat[cStat] - m_angelStat[cStat] + m_luStat[cStat]);
            //statChange = 450 - (m_stat[cStat] - m_angelStat[cStat] + m_luStat[cStat]);
            statChange = Stats_Limit - (m_stat[cStat] - m_angelStat[cStat] + m_luStat[cStat]);
        if (statChange < 0) statChange = 0;

        if (m_iLU_Point >= statChange)
            m_luStat[cStat] += statChange;
        else
        {
            statChange = m_luStat[cStat];
            m_luStat[cStat] = 0;
        }

        PlaySound('E', 14, 5);
        m_iLU_Point -= statChange;
    }

    if (cStat != NONE && onButton % 2 == 0)
    {
        if (statChange > m_luStat[cStat])
        {
            statChange = m_luStat[cStat];
            m_luStat[cStat] = 0;
        }
        else	m_luStat[cStat] -= statChange;

        PlaySound('E', 14, 5);
        m_iLU_Point += statChange;
    }

    if (onButton == 1)
    {
        if (m_dialogBoxes[12].sV1 != m_iLU_Point)
            // Send command to HG - Diuuude, Only if changed - Snoopy
            bSendCommand(MSGID_LEVELUPSETTINGS);
        //m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;
        DisableDialogBox(12);
        PlaySound('E', 14, 5);
    }
    else if (onButton == 2)
    {	// Change stats with Majestic
        if ((m_iGizonItemUpgradeLeft > 0) && (m_iLU_Point <= 0)
            && (m_luStat[STAT_STR] == 0) && (m_luStat[STAT_VIT] == 0) && (m_luStat[STAT_DEX] == 0) && (m_luStat[STAT_INT] == 0) && (m_luStat[STAT_MAG] == 0) && (m_luStat[STAT_CHR] == 0))
        {
            DisableDialogBox(12);
            EnableDialogBox(42, 0, 0, 0);
            //PlaySound('E', 14, 5);
        }
    }
}

void CGame::DlgBoxClick_Text()
{
    if (m_dialogBoxes[18].OnButton() == 1)
    {
        DisableDialogBox(18);
        PlaySound('E', 14, 5);
    }
}

void CGame::DlgBoxClick_Inventory()
{
    int i;
    char onButton = m_dialogBoxes[2].OnButton();

    if (onButton == 1)
    {
        if (m_iGizonItemUpgradeLeft == 0)
        {
            m_iGizonItemUpgradeLeft = 0;
        }
        EnableDialogBox(34, 5, 0, 0);
        PlaySound('E', 14, 5);
    }
    else if (onButton == 2)
    {
        //{	if (m_cSkillMastery[SKILL_MANUFACTURING] == 0)
        //	{	AddEventList(DLGBOXCLICK_INVENTORY1, 10);
        //		AddEventList(DLGBOXCLICK_INVENTORY2, 10);//"The manufacturing manual is purchasable in Blacksmith."
        //	}else 
        if (m_bSkillUsingStatus == true)
        {
            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY5, 10);//""You are already using another skill."
            return;
        }
        else if (_bIsItemOnHand() == true)
        {
            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);//""Your hands should be free to use this item."
            return;
        }
        else
        {
            for (i = 0; i < MAXITEMS; i++)
                if ((m_pItemList[i] != 0) && (m_pItemList[i]->m_cItemType == ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX)
                    && (m_pItemList[i]->m_sSpriteFrame == 113)
                    && (m_pItemList[i]->m_wCurLifeSpan > 0))
                {
                    EnableDialogBox(26, 3, 0, 0, 0);
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY12, 10);//"Using a manufacturing skill..."
                    PlaySound('E', 14, 5);
                    return;
                }
            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY14, 10);
        }
        PlaySound('E', 14, 5);
    }
}

void CGame::DlgBoxClick_Character()
{
    char onButton = m_dialogBoxes[1].OnButton();

    if (onButton == 1)
    {
        EnableDialogBox(28, 1, 0, 0);
        DisableDialogBox(1);
        PlaySound('E', 14, 5);
    }
    else if (onButton == 2)
    {
        EnableDialogBox(32, 0, 0, 0);
        DisableDialogBox(1);
        PlaySound('E', 14, 5);
    }
    else if (onButton == 3)
    {
        EnableDialogBox(12, 0, 0, 0);
        DisableDialogBox(1);
        PlaySound('E', 14, 5);

    }
    else if (onButton == 4)
    {
        if (m_iGuildRank != GUILDRANK_NONE)
        {
            EnableDialogBox(57, 0, 0, 0);
            DisableDialogBox(1);
            PlaySound('E', 14, 5);
        }
        else
        {
            wsprintfA(G_cTxt, GUILD_ERROR1);
            AddEventList(G_cTxt, 10);
        }
    }
#ifdef TitleClient
    else if (onButton == 5)
    {
        EnableDialogBox(63, 0, 0, 0);
        DisableDialogBox(1);
        PlaySound('E', 14, 5);
    }
#endif
}

void CGame::DlgBoxClick_FeedBackCard()
{

}

void CGame::DlgBoxClick_MagicShop()
{

    int i, j, iCPivot;

    iCPivot = m_dialogBoxes[16].sView * 10;

    char onButton = m_dialogBoxes[16].OnButton();
    for (i = 0, j = 0; i < 9; i++)
    {
        if ((m_pMagicCfgList[iCPivot + i] != 0) && (m_pMagicCfgList[iCPivot + i]->m_bIsVisible))
        {
            if (onButton == ++j)
            {
                if (m_cMagicMastery[iCPivot + i] == 0)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_STUDYMAGIC, 0, 0, 0, 0, m_pMagicCfgList[iCPivot + i]->m_cName);
                    //bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_STUDYMAGIC, NULL, iCPivot + i, NULL, NULL, NULL);
                    PlaySound('E', 14, 5);
                }
                return;
            }
        }
    }
    if (onButton == 10)
    {
        m_dialogBoxes[16].SetMode(1);
        PlaySound('E', 14, 5);
        return;
    }
    else if (onButton == 11)	m_dialogBoxes[16].sView = 0;
    else if (onButton == 12)	m_dialogBoxes[16].sView = 1;
    else if (onButton == 13)	m_dialogBoxes[16].sView = 2;
    else if (onButton == 14)	m_dialogBoxes[16].sView = 3;
    else if (onButton == 15)	m_dialogBoxes[16].sView = 4;
    else if (onButton == 16)	m_dialogBoxes[16].sView = 5;
    else if (onButton == 17)	m_dialogBoxes[16].sView = 6;
    else if (onButton == 18)	m_dialogBoxes[16].sView = 7;
    else if (onButton == 19)	m_dialogBoxes[16].sView = 8;
    else if (onButton == 20)	m_dialogBoxes[16].sView = 9;
}

void CGame::DlgBoxClick_15AgeMsg()
{	// Snoopy: removed feedback card
    if (m_dialogBoxes[5].OnButton() == 1)
        DisableDialogBox(5);
}


void CGame::DlgBoxClick_WarningMsg()
{
    if (m_dialogBoxes[6].OnButton() == 1)
        DisableDialogBox(6);
}

void CGame::DlgBoxClick_ItemDrop()
{
    if (m_cCommand < 0) return;

    switch (m_dialogBoxes[4].OnButton())
    {
        case 1:
            m_dialogBoxes[4].SetMode(3);
            bSendCommand(MSGID_COMMAND_COMMON,
                         COMMONTYPE_ITEMDROP,
                         0,
                         m_dialogBoxes[4].sView,
                         1,
                         0,
                         m_pItemList[m_dialogBoxes[4].sView]->m_cName);
            //m_stDialogBoxInfo[40].sView ;
            DisableDialogBox(4);
            break;

        case 2:
            for (int i = 0; i < MAXSELLLIST; i++)
                m_bIsItemDisabled[i] = false;

            DisableDialogBox(4);
            break;

        case 3:
            m_bItemDrop = !m_bItemDrop;
            break;
    }
}

void CGame::DrawDialogBox_GuildContribute()
{
    short sX, sY, szX;
    char cTxt[120], cStr1[64], cStr2[64], cStr3[64];

    sX = m_dialogBoxes[58].m_X;
    sY = m_dialogBoxes[58].m_Y;
    szX = m_dialogBoxes[58].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 5);

    switch (m_dialogBoxes[58].sView)
    {
        case 0: // Gold
            PutAlignedString(sX, sX + szX, sY + 20, DRAW_DIALOGBOX_GUILDCONTRIBUTE1);
            PutAlignedString(sX, sX + szX, sY + 35, DRAW_DIALOGBOX_GUILDCONTRIBUTE2);

            if (iGetTopDialogBoxIndex() != 58)
                PutString(sX + 40, sY + 57, m_cAmountString, video::SColor(255, 255, 255, 255), false, 2);
            wsprintfA(cTxt, "__________ (0 ~ %d)", GetGold());
            PutString(sX + 38, sY + 62, cTxt, video::SColor(255, 25, 35, 25));
            break;

        case 1: // Maj
            PutAlignedString(sX, sX + szX, sY + 20, DRAW_DIALOGBOX_GUILDCONTRIBUTE3);
            PutAlignedString(sX, sX + szX, sY + 35, DRAW_DIALOGBOX_GUILDCONTRIBUTE4);

            if (iGetTopDialogBoxIndex() != 58)
                PutString(sX + 40, sY + 57, m_cAmountString, video::SColor(255, 255, 255, 255), false, 2);
            wsprintfA(cTxt, "__________ (0 ~ %d)", m_iGizonItemUpgradeLeft);
            PutString(sX + 38, sY + 62, cTxt, video::SColor(255, 25, 35, 25));
            break;

        case 2: // Cont
            PutAlignedString(sX, sX + szX, sY + 20, DRAW_DIALOGBOX_GUILDCONTRIBUTE7);
            PutAlignedString(sX, sX + szX, sY + 35, DRAW_DIALOGBOX_GUILDCONTRIBUTE8);

            if (iGetTopDialogBoxIndex() != 58)
                PutString(sX + 40, sY + 57, m_cAmountString, video::SColor(255, 255, 255, 255), false, 2);
            wsprintfA(cTxt, "__________ (0 ~ %d)", m_iContribution);
            PutString(sX + 38, sY + 62, cTxt, video::SColor(255, 25, 35, 25));
            break;
    }

}

void CGame::DlgBoxClick_ItemSellorRepair()
{
    char onButton = m_dialogBoxes[23].OnButton();
    switch (m_dialogBoxes[23].GetMode())
    {
        case 1:
            if (onButton == 1) // Sell
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_SELLITEMCONFIRM, 0, m_dialogBoxes[23].sV1, m_dialogBoxes[23].sV4, m_dialogBoxes[23].sV3, m_pItemList[m_dialogBoxes[23].sV1]->m_cName); //v1.2
                m_dialogBoxes[23].SetMode(3);
            }
            else if (onButton == 2) // Cancel
            {
                m_bIsItemDisabled[m_dialogBoxes[23].sV1] = false;
                DisableDialogBox(23);
            }
            break;

        case 2:
            if (onButton == 1) // Repair
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_REPAIRITEMCONFIRM, 0, m_dialogBoxes[23].sV1, 0, 0, m_pItemList[m_dialogBoxes[23].sV1]->m_cName);
                m_dialogBoxes[23].SetMode(4);
            }
            else if (onButton == 2) // Cancel
            {
                m_bIsItemDisabled[m_dialogBoxes[23].sV1] = false;
                DisableDialogBox(23);
            }
            break;
    }
}


bool CGame::DlgBoxPress_Inventory()
{
    int i;
    char  cItemID;
    short sX, sY, x1, x2, y1, y2;

#ifdef _DEBUG
    AddEventList("Press Inventory", 10);
#endif

    if (m_bIsDialogEnabled[2] == false) return false;
    if (m_bIsDialogEnabled[17] == true) return false;
    if (m_bIsDialogEnabled[4] == true) return false;

    short msY = G_pGame->m_stMCursor.sY;
    short msX = G_pGame->m_stMCursor.sX;

    sX = m_dialogBoxes[2].m_X;
    sY = m_dialogBoxes[2].m_Y;

    for (i = 0; i < MAXITEMS; i++)
        if (m_cItemOrder[MAXITEMS - 1 - i] != -1)
        {
            cItemID = m_cItemOrder[MAXITEMS - 1 - i];

            if (m_pItemList[cItemID] != 0)
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + 32 + m_pItemList[cItemID]->m_sX,
                                                                                                       sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame);
                x1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
                y1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
                x2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
                y2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;
                if ((m_bIsItemDisabled[cItemID] == false) && (m_bIsItemEquipped[cItemID] == false) && (msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
                {

                    if (m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_bCheckCollison(sX + 32 + m_pItemList[cItemID]->m_sX, sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame, msX, msY) == true)
                    {
                        _SetItemOrder(0, cItemID);
                        if ((m_bIsGetPointingMode == true) && (m_iPointCommandType < 100) && (m_iPointCommandType >= 0)
                            && (m_pItemList[m_iPointCommandType] != 0)
                            && (m_pItemList[m_iPointCommandType]->m_cItemType == ITEMTYPE_USE_DEPLETE_DEST)
                            && (m_iPointCommandType != cItemID))
                        {
                            PointCommandHandler(0, 0, cItemID);
                            //m_bCommandAvailable  = FALSE;
                            m_bIsGetPointingMode = false;
                        }
                        else
                        {
                            m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                            m_stMCursor.sSelectedObjectID = cItemID;
                            m_stMCursor.sDistX = msX - x1 + (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotX;
                            m_stMCursor.sDistY = msY - y1 + (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotY;
                        }
                        return true;
                    }
                }
            }
        }
    return false;
}

void CGame::DlgBoxClick_GuildMenu()
{
    short sX, sY;
    char cTemp[21];

    char onButton = m_dialogBoxes[7].OnButton();
    sX = m_dialogBoxes[7].m_X;
    sY = m_dialogBoxes[7].m_Y;


    switch (m_dialogBoxes[7].GetMode())
    {
        case 0:
            switch (onButton)
            {
                case 1:
                    if (m_iGuildRank != GUILDRANK_NONE) return;
                    //if (m_stat[STAT_CHR] < 20) return;
                    if (m_iLevel < 100) return; // Guild Req xRisenx
                    if (m_bIsCrusadeMode) return;
                    EndInputString();
                    StartInputString(sX + 75, sY + 140, 21, m_cGuildName);
                    m_dialogBoxes[7].SetMode(1);
                    break;
                case 2:
                    if (m_iGuildRank != GUILDRANK_MASTER) return;
                    if (m_bIsCrusadeMode) return;
                    m_dialogBoxes[7].SetMode(5);
                    break;
                case 3:
                    m_dialogBoxes[7].SetMode(9);
                    break;
                case 4:
                    m_dialogBoxes[7].SetMode(11);
                    break;
                case 5:
                    if (m_iFightzoneNumber < 0) break;
                    if (m_iGuildRank != GUILDRANK_MASTER) return;
                    if (m_iFightzoneNumber == 0) m_dialogBoxes[7].SetMode(13);
                    else  m_dialogBoxes[7].SetMode(19);
                    break;
            }
            break;
        case 1:
            switch (onButton)
            {
                case 1:// Submit
                    if (strcmp(m_cGuildName, "NONE") == 0) return;
                    if (strlen(m_cGuildName) == 0) return;
                    bSendCommand(MSGID_REQUEST_CREATENEWGUILD, MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
                    m_dialogBoxes[7].SetMode(2);
                    EndInputString();
                    break;
                case 2:	// Cancel
                    m_dialogBoxes[7].SetMode(0);
                    EndInputString();
                    break;
            }
            break;
        case 3:
        case 4:
        case 7:
        case 8:
        case 10:
        case 12:
            switch (onButton)
            {
                case 1:
                    m_dialogBoxes[7].SetMode(0);
                    break;
            }
            break;

        case 9:
            switch (onButton)
            {
                case 1:
                    ZeroMemory(cTemp, sizeof(cTemp));
                    strcpy(cTemp, "GuildAdmissionTicket");
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_PURCHASEITEM, 0, 1, 0, 0, cTemp);
                    m_dialogBoxes[7].SetMode(0);
                    break;
                case 2:	// Cancel
                    m_dialogBoxes[7].SetMode(0);
                    break;
            }
            break;

        case 11:
            switch (onButton)
            {
                case 1:// Purchase
                    ZeroMemory(cTemp, sizeof(cTemp));
                    strcpy(cTemp, "GuildSecessionTicket");
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_PURCHASEITEM, 0, 1, 0, 0, cTemp);
                    m_dialogBoxes[7].SetMode(0);
                    break;
                case 2:	// Cancel
                    m_dialogBoxes[7].SetMode(0);
                    break;
            }
            break;

        case 5:
            switch (onButton)
            {
                case 1:// Confirm
                    bSendCommand(MSGID_REQUEST_DISBANDGUILD, MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
                    m_dialogBoxes[7].SetMode(6);
                    break;
                case 2:// Cancel
                    m_dialogBoxes[7].SetMode(0);
                    break;
            }
            break;

        case 13:
            switch (onButton)
            {
                case 1:
                    bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 1, 0, 0, 0);
                    m_dialogBoxes[7].SetMode(18);
                    m_iFightzoneNumberTemp = 1;
                    break;
                case 2:
                    bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 2, 0, 0, 0);
                    m_dialogBoxes[7].SetMode(18);
                    m_iFightzoneNumberTemp = 2;
                    break;
                case 3:
                    bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 3, 0, 0, 0);
                    m_dialogBoxes[7].SetMode(18);
                    m_iFightzoneNumberTemp = 3;
                    break;
                case 4:
                    bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 4, 0, 0, 0);
                    m_dialogBoxes[7].SetMode(18);
                    m_iFightzoneNumberTemp = 4;
                    break;
                case 5:
                    bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 5, 0, 0, 0);
                    m_dialogBoxes[7].SetMode(18);
                    m_iFightzoneNumberTemp = 5;
                    break;
                case 6:
                    bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 6, 0, 0, 0);
                    m_dialogBoxes[7].SetMode(18);
                    m_iFightzoneNumberTemp = 6;
                    break;
                case 7:
                    bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 7, 0, 0, 0);
                    m_dialogBoxes[7].SetMode(18);
                    m_iFightzoneNumberTemp = 7;
                    break;
                case 8:
                    bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 8, 0, 0, 0);
                    m_dialogBoxes[7].SetMode(18);
                    m_iFightzoneNumberTemp = 8;
                    break;
                case 9:
                    // Cancel
                    m_dialogBoxes[7].SetMode(0);
                    break;
            }

            break;

        case 14://
        case 15://
        case 16://
        case 17://
        case 21://
        case 22://
            switch (onButton)
            {
                case 1: // OK
                    m_dialogBoxes[7].SetMode(0);
                    break;
            }
            break;
    }
    if (onButton) PlaySound('E', 14, 5);
}

void CGame::DlgBoxClick_Guild()
{
    CDialogBox & dlg = m_dialogBoxes[DIALOG_GUILD];
    char cTemp[21];
    char txt[200];
    int iAdjX, iAdjY;
    int maxLines, line;
    MemberList::iterator it;
    uint32_t view;
    uint32_t diff;

    short szX = dlg.sSizeX;
    char onButton = dlg.OnButton();

    iAdjX = -13;
    iAdjY = 30;

    switch (dlg.GetMode())
    {
        case 0:
            switch (onButton)
            {
                case 1: // see online members
                    dlg.SetMode(1);
                    break;

                case 2: // give gold
                    m_dialogBoxes[58].m_X = G_pGame->m_stMCursor.sX - 140;
                    m_dialogBoxes[58].m_Y = G_pGame->m_stMCursor.sY - 70;
                    if (m_dialogBoxes[58].m_Y < 0)
                        m_dialogBoxes[58].m_Y = 0;
                    EnableDialogBox(58, 0);
                    break;

                case 3: // give maj
                    m_dialogBoxes[58].m_X = G_pGame->m_stMCursor.sX - 140;
                    m_dialogBoxes[58].m_Y = G_pGame->m_stMCursor.sY - 70;
                    if (m_dialogBoxes[58].m_Y < 0)
                        m_dialogBoxes[58].m_Y = 0;
                    EnableDialogBox(58, 1);
                    break;

                case 4: // give cont
                    m_dialogBoxes[58].m_X = G_pGame->m_stMCursor.sX - 140;
                    m_dialogBoxes[58].m_Y = G_pGame->m_stMCursor.sY - 70;
                    if (m_dialogBoxes[58].m_Y < 0)
                        m_dialogBoxes[58].m_Y = 0;
                    EnableDialogBox(58, 2);
                    break;

                case 5: // upgrades
                    dlg.SetMode(2);
                    break;

                case 6: // message board
                    bSendCommand(MSGID_REQ_GUILDBOARD);
                    dlg.SetMode(8);
                    dlg.sView = 0;
                    break;
            }
            if (onButton)
                PlaySound('E', 14, 5);
            break;

        case 1:
            maxLines = 16;
            if (gldRankPerm[m_iGuildRank].canSummon)
                maxLines--;

            if (gldRankPerm[m_iGuildRank].canPromote)
                maxLines--;

            if (onButton == 1) // back
            {
                dlg.SetMode(0);
                PlaySound('E', 14, 5);
            }

            if (!gldRankPerm[m_iGuildRank].canSummon && !gldRankPerm[m_iGuildRank].canPromote)
                return;

            view = dlg.sView;
            it = m_guild.m_onlineList.begin();

            while (view > 0 && it != m_guild.m_onlineList.end())
            {
                ++it;
                view--;
            }

            for (int i = 0; i < maxLines; i++, ++it)
            {
                if (it == m_guild.m_onlineList.end())
                    break;
                if (i + dlg.sView < m_guild.m_onlineList.size())
                {
                    // member name
                    if (onButton == i + 2)
                    {
                        if (!gldRankPerm[m_iGuildRank].canSummon)
                            return;

                        diff = time(0) - (*it).second.summonTime;
                        if (diff < MINUTE(5))
                        {
                            wsprintfA(txt, DLGBOX_CLICK_GUILD7, (MINUTE(5) - diff) / 60, (MINUTE(5) - diff) % 60);
                            AddEventList(txt, 10);
                            wsprintfA(txt, DLGBOX_CLICK_GUILD8, (*it).first.c_str());
                            AddEventList(txt, 10);
                            return;
                        }

                        if (unixtime() - m_dwDamagedTime < 10 * 1000)
                        {
                            AddEventList(DLGBOXCLICK_GUILDSUMMONS1, CHAT_GM);
                            return;
                        }

                        switch (m_guild.CanSummon(m_cMapName))
                        {
                            case CANSUMMON_OK:
                                (*it).second.summonTime = time(0);
                                wsprintfA(txt, DLGBOX_CLICK_GUILD6, (*it).first.c_str());
                                AddEventList(txt, 10);
                                bSendCommand(MSGID_REQGUILDSUMMONS, 0, 0, 0, 0, 0, (*it).first.c_str());
                                break;
                            case CANSUMMON_LV1:
                                AddEventList(DLGBOX_CLICK_GUILD2, 10);
                                break;
                            case CANSUMMON_LV2:
                                AddEventList(DLGBOX_CLICK_GUILD3, 10);
                                break;
                            case CANSUMMON_LV3:
                                AddEventList(DLGBOX_CLICK_GUILD2, 10);
                                AddEventList(DLGBOX_CLICK_GUILD4, 10);
                                break;
                            case CANSUMMON_LV4:
                                AddEventList(DLGBOX_CLICK_GUILD2, 10);
                                AddEventList(DLGBOX_CLICK_GUILD5, 10);
                                break;
                            case CANSUMMON_NONE:
                                AddEventList(DLGBOX_CLICK_GUILD1, 10);
                                break;
                            case CANSUMMON_GOLD:
                                AddEventList(DLGBOX_CLICK_GUILD9, 10);
                                break;
                        }
                    }

                    //rank
                    if (onButton == i + 18)
                    {
                        if (!gldRankPerm[m_iGuildRank].canPromote || (*it).second.rank < m_iGuildRank)
                            return;

                        m_dialogBoxes[44].m_X = G_pGame->m_stMCursor.sX - 125;
                        m_dialogBoxes[44].m_Y = G_pGame->m_stMCursor.sY - 50;
                        if (m_dialogBoxes[44].m_Y < 0)
                            m_dialogBoxes[44].m_Y = 0;
                        EnableDialogBox(44, 0, (*it).second.rank, 0, (*it).first.c_str());
                    }
                }
            }
            break;

        case 2:
            switch (onButton)
            {
                case 1: // warehouse upgrade
                    dlg.SetMode(3);
                    break;

                case 2:// teleport upgrade
                    dlg.SetMode(4);
                    break;

                case 3:// captaincy upgrade
                    dlg.SetMode(5);
                    break;

                case 4:// raidmasters upgrade
                    dlg.SetMode(6);
                    break;

                case 5:// huntmasters upgrade
                    dlg.SetMode(7);
                    break;

                case 7: // back
                    dlg.SetMode(0);
                    break;
            }

            if (onButton)
                PlaySound('E', 14, 5);
            break;

        case 3: // warehouse upgrade
                // upgrade
            if (m_iGuildRank == GUILDRANK_MASTER && onButton == 1)
            {
                if (m_guild.m_upgrades[GU_WAREHOUSE] >= gldUps[GU_WAREHOUSE].maxLvl)
                {
                    AddEventList(DLGBOX_CLICK_GUILD14, CHAT_GM);
                    return;
                }

                if (m_guild.m_gold < gldUps[GU_WAREHOUSE].costGold[m_guild.m_upgrades[GU_WAREHOUSE] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD11, CHAT_GM);
                    return;
                }

                if (m_guild.m_maj < gldUps[GU_WAREHOUSE].costMaj[m_guild.m_upgrades[GU_WAREHOUSE] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD12, CHAT_GM);
                    return;
                }

                if (m_guild.m_cont < gldUps[GU_WAREHOUSE].costCont[m_guild.m_upgrades[GU_WAREHOUSE] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD13, CHAT_GM);
                    return;
                }

                bSendCommand(MSGID_GUILDUPGRADE, 0, 0, GU_WAREHOUSE, 0, 0, 0);
            }

            // back
            else if (onButton == 2)
            {
                dlg.SetMode(2);
                PlaySound('E', 14, 5);
            }

            break;

        case 4: // teleport upgrade
                // upgrade
            if (m_iGuildRank == GUILDRANK_MASTER && onButton == 1)
            {
                if (m_guild.m_upgrades[GU_SUMMONS] >= gldUps[GU_SUMMONS].maxLvl)
                {
                    AddEventList(DLGBOX_CLICK_GUILD14, CHAT_GM);
                    return;
                }

                if (m_guild.m_gold < gldUps[GU_SUMMONS].costGold[m_guild.m_upgrades[GU_SUMMONS] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD11, CHAT_GM);
                    return;
                }

                if (m_guild.m_maj < gldUps[GU_SUMMONS].costMaj[m_guild.m_upgrades[GU_SUMMONS] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD12, CHAT_GM);
                    return;
                }

                if (m_guild.m_cont < gldUps[GU_SUMMONS].costCont[m_guild.m_upgrades[GU_SUMMONS] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD13, CHAT_GM);
                    return;
                }

                bSendCommand(MSGID_GUILDUPGRADE, 0, 0, GU_SUMMONS, 0, 0, 0);
            }

            // back
            else if (onButton == 2)
            {
                dlg.SetMode(2);
                PlaySound('E', 14, 5);
            }

            break;

        case 5: // Captain upgrade
                // upgrade
            if (m_iGuildRank == GUILDRANK_MASTER && onButton == 1)
            {
                if (m_guild.m_upgrades[GU_CAPTAINCY] >= gldUps[GU_CAPTAINCY].maxLvl)
                {
                    AddEventList(DLGBOX_CLICK_GUILD14, CHAT_GM);
                    return;
                }

                if (m_guild.m_gold < gldUps[GU_CAPTAINCY].costGold[m_guild.m_upgrades[GU_CAPTAINCY] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD11, CHAT_GM);
                    return;
                }

                if (m_guild.m_maj < gldUps[GU_CAPTAINCY].costMaj[m_guild.m_upgrades[GU_CAPTAINCY] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD12, CHAT_GM);
                    return;
                }

                if (m_guild.m_cont < gldUps[GU_CAPTAINCY].costCont[m_guild.m_upgrades[GU_CAPTAINCY] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD13, CHAT_GM);
                    return;
                }

                bSendCommand(MSGID_GUILDUPGRADE, 0, 0, GU_CAPTAINCY, 0, 0, 0);
            }

            // back
            else if (onButton == 2)
            {
                dlg.SetMode(2);
                PlaySound('E', 14, 5);
            }

            break;

        case 6: // raidmaster upgrade
                // upgrade
            if (m_iGuildRank == GUILDRANK_MASTER && onButton == 1)
            {
                if (m_guild.m_upgrades[GU_RAIDMASTERS] >= gldUps[GU_RAIDMASTERS].maxLvl)
                {
                    AddEventList(DLGBOX_CLICK_GUILD14, CHAT_GM);
                    return;
                }

                if (m_guild.m_gold < gldUps[GU_RAIDMASTERS].costGold[m_guild.m_upgrades[GU_RAIDMASTERS] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD11, CHAT_GM);
                    return;
                }

                if (m_guild.m_maj < gldUps[GU_RAIDMASTERS].costMaj[m_guild.m_upgrades[GU_RAIDMASTERS] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD12, CHAT_GM);
                    return;
                }

                if (m_guild.m_cont < gldUps[GU_RAIDMASTERS].costCont[m_guild.m_upgrades[GU_RAIDMASTERS] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD13, CHAT_GM);
                    return;
                }

                bSendCommand(MSGID_GUILDUPGRADE, 0, 0, GU_RAIDMASTERS, 0, 0, 0);
            }

            // back
            else if (onButton == 2)
            {
                dlg.SetMode(2);
                PlaySound('E', 14, 5);
            }

            break;

        case 7: // huntmaster upgrade
                // upgrade
            if (m_iGuildRank == GUILDRANK_MASTER && onButton == 1)
            {
                if (m_guild.m_upgrades[GU_HUNTMASTERS] >= gldUps[GU_HUNTMASTERS].maxLvl)
                {
                    AddEventList(DLGBOX_CLICK_GUILD14, CHAT_GM);
                    return;
                }

                if (m_guild.m_gold < gldUps[GU_HUNTMASTERS].costGold[m_guild.m_upgrades[GU_HUNTMASTERS] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD11, CHAT_GM);
                    return;
                }

                if (m_guild.m_maj < gldUps[GU_HUNTMASTERS].costMaj[m_guild.m_upgrades[GU_HUNTMASTERS] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD12, CHAT_GM);
                    return;
                }

                if (m_guild.m_cont < gldUps[GU_HUNTMASTERS].costCont[m_guild.m_upgrades[GU_HUNTMASTERS] + 1])
                {
                    AddEventList(DLGBOX_CLICK_GUILD13, CHAT_GM);
                    return;
                }

                bSendCommand(MSGID_GUILDUPGRADE, 0, 0, GU_HUNTMASTERS, 0, 0, 0);
            }

            // back
            else if (onButton == 2)
            {
                dlg.SetMode(2);
                PlaySound('E', 14, 5);
            }

            break;

        case 8: // guild message board
            if (onButton == 1)
            {
                // Post
                if (GetItemCount("Gold") < GUILDBOARD_POST_COST)
                {
                    AddEventList(NOTIFY_MSG_HANDLER84, CHAT_GM);
                    break;
                }

                ZeroMemory(dlg.cStr2, sizeof(dlg.cStr2)); // title
                ZeroMemory(dlg.txt, sizeof(dlg.txt)); // msg
                dlg.sV1 = -1;
                dlg.bFlag = false;
                dlg.SetMode(10);

                G_pGame->m_stMCursor.sX = dlg.GetButton(1).left + 1;
                G_pGame->m_stMCursor.sY = dlg.GetButton(1).top + 1;
                DlgBoxClick_Guild();
            }
            if (onButton == 3)
            {
                dlg.SetMode(0);
            }
            else if (onButton >= 4 && onButton < 4 + 15)
            {
                uint32_t listHit = onButton - 4 + dlg.sView;

                if (listHit >= m_gldBoard.size())
                    break;

                if (!m_gldBoard[m_gldBoard.size() - 1 - listHit]->msg)
                    bSendCommand(MSGID_REQ_GUILDPOSTDATA, 0, 0, m_gldBoard[m_gldBoard.size() - 1 - listHit]->id);

                dlg.sV1 = m_gldBoard.size() - 1 - listHit;
                dlg.SetMode(9);
            }
            break;

        case 9: // post view
            switch (onButton)
            {
                case 4: // delete
                    if (m_iGuildRank != GUILDRANK_MASTER && m_iGuildRank != GUILDRANK_CAPTAIN)
                        break;

                    EnableDialogBox(DIALOG_YESNO, 2);
                    break;

                case 5: // back
                    dlg.sV1 = -1;
                    dlg.SetMode(8);
                    break;
            }
            break;

        case 10:
            EndInputString();

            switch (onButton)
            {
                case 1: // title:
                    StartInputString(dlg.GetButton(1).left + 30, dlg.GetButton(1).top - 1, 31, dlg.cStr2);
                    break;

                case 2: // msg:
                    StartInputString(dlg.GetButton(2).left, dlg.GetButton(2).top + 15, sizeof(dlg.txt), dlg.txt, false, dlg.GetButton(2).right);
                    break;

                case 3: // send
                    if (!dlg.bFlag)
                    {
                        bSendCommand(MSGID_REQ_POSTGUILDBOARD);
                        dlg.bFlag = true;
                    }
                    break;

                case 4: // cancel
                    dlg.SetMode(8);
                    break;
            }
            break;
    }
}

void CGame::DlgBoxClick_GuildOp()
{
    short sX, sY;
    char cName[12], cName20[24];
    char onButton = m_dialogBoxes[8].OnButton();

    ZeroMemory(cName, sizeof(cName));
    ZeroMemory(cName20, sizeof(cName20));

    switch (m_stGuildOpList[0].cOpMode)
    {
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            if (onButton == 1)
            {
                _ShiftGuildOperationList();
                if (m_stGuildOpList[0].cOpMode == 0) DisableDialogBox(8);
            }
            return;
    }

    if (onButton == 2)
    {
        // Approve
        PlaySound('E', 14, 5);

        switch (m_stGuildOpList[0].cOpMode)
        {
            case 1:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_JOINGUILDAPPROVE, 0, 0, 0, 0, cName20);
                break;

            case 2:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_DISMISSGUILDAPPROVE, 0, 0, 0, 0, cName20);
                break;
        }
        _ShiftGuildOperationList();
        if (m_stGuildOpList[0].cOpMode == 0) DisableDialogBox(8);
    }

    if (onButton == 1)
    {
        // Reject
        PlaySound('E', 14, 5);

        switch (m_stGuildOpList[0].cOpMode)
        {
            case 1:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_JOINGUILDREJECT, 0, 0, 0, 0, cName20);
                break;

            case 2:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_DISMISSGUILDREJECT, 0, 0, 0, 0, cName20);
                break;
        }

        _ShiftGuildOperationList();
        if (m_stGuildOpList[0].cOpMode == 0) DisableDialogBox(8);
    }
}


void CGame::DrawDialogBox_GuideMap()
{
    int  m_iMaxMapIndex = SPRID_INTERFACE_GUIDEMAP + m_cMapIndex + 1;
    int  m_iMinMapIndex = SPRID_INTERFACE_GUIDEMAP;
    int  m_iMinMapSquare = m_cMapIndex;
    int  pointX, pointY;
    //telescope.pak:  frames 1..31 <=> SPRID_INTERFACE_GUIDEMAP +1...+31 <=> m_cMapIndex 0..30
    //telescope1.pak: frames 1..3  <=> SPRID_INTERFACE_GUIDEMAP +36..+38 <=> m_cMapIndex 35..37 et +
    if (m_cMapIndex < 0) return;
    if (m_cMapIndex >= 35)
    {
        m_iMaxMapIndex = SPRID_INTERFACE_GUIDEMAP + m_cMapIndex + 1;
        m_iMinMapIndex = SPRID_INTERFACE_GUIDEMAP + 35;
        m_iMinMapSquare = m_cMapIndex - 35;
    }
    short sX, sY, shX, shY, szX, szY;

    sX = m_dialogBoxes[9].m_X;
    sY = m_dialogBoxes[9].m_Y;
    szX = m_dialogBoxes[9].sSizeX;
    szY = m_dialogBoxes[9].sSizeY;

    if (sX < 20) sX = 0;
    if (sY < 20) sY = 0;
    //if( sX > 640-128-20 ) sX = 640-128;
    if (sX > GetWidth() - 128 - 20) sX = GetWidth() - 128; // 800x600 Resolution xRisenx
                                                           //if( sY > 427-128-20 ) sY = 427-128;
    if (sY > GetHeight() - 13 - 128 - 20) sY = GetHeight() - 13 - 128; // 800x600 xRisenx
    for (shX = -2; shX<130; shX++)
    {
        //DIRECTX m_DDraw.PutPixel( sX+shX, sY-2  , 50,50,50);
        //DIRECTX m_DDraw.PutPixel( sX+shX, sY-1  , 50,50,50);
        //DIRECTX m_DDraw.PutPixel( sX+shX, sY+128, 50,50,50);
        //DIRECTX m_DDraw.PutPixel( sX+shX, sY+129, 50,50,50);
    }
    for (shY = -2; shY<130; shY++)
    {
        //DIRECTX m_DDraw.PutPixel( sX-2  , sY+shY, 50,50,50);
        //DIRECTX m_DDraw.PutPixel( sX-1  , sY+shY, 50,50,50);
        //DIRECTX m_DDraw.PutPixel( sX+128, sY+shY, 50,50,50);
        //DIRECTX m_DDraw.PutPixel( sX+129, sY+shY, 50,50,50);
    }
    if (m_bZoomMap)
    {
        shX = m_sPlayerX - 64;
        shY = m_sPlayerY - 64;
        if (shX < 0) shX = 0;
        if (shY < 0) shY = 0;
        if (shX > m_pMapData->m_sMapSizeX - 128) shX = m_pMapData->m_sMapSizeX - 128;
        if (shY > m_pMapData->m_sMapSizeY - 128) shY = m_pMapData->m_sMapSizeY - 128;
        if (m_bDialogTrans) m_pSprite[m_iMaxMapIndex]->PutShiftTransSprite2(sX, sY, shX, shY, 0, m_dwCurTime);
        else m_pSprite[m_iMaxMapIndex]->PutShiftSpriteFast(sX, sY, shX, shY, 0, m_dwCurTime);

        if (m_iPartyStatus && !m_iIlusionOwnerH)
        {
            for (int i = 0; i < m_stPartyMember.size(); i++)
            {
                if (m_stPartyMember[i]->sX == 0 || m_stPartyMember[i]->sY == 0)
                    continue;

                if (m_stPartyMember[i]->sX >= shX && m_stPartyMember[i]->sX <= shX + 128 &&
                    m_stPartyMember[i]->sY >= shY && m_stPartyMember[i]->sY <= shY + 128)
                {
                    pointX = (sX - shX + m_stPartyMember[i]->sX) - 1;
                    pointY = (sY - shY + m_stPartyMember[i]->sY) - 1;

                    for (int pX = 0; pX < 3; pX++)
                    {
                        //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY, 60,220,60);
                        // 						if(pX==1)//DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 50,170,250);
                        // 						else //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 60,220,60);
                        //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 2, 60,220,60);
                    }
                }
            }
        }

        if (m_mapPings.size() > 0)
        {
            std::vector<MapPing>::iterator remIt = remove_if(m_mapPings.begin(), m_mapPings.end(), MapPing::Expired);
            m_mapPings.erase(remIt, m_mapPings.end());

            foreach(ping, m_mapPings)
            {
                if (ping->pos.x == 0 || ping->pos.y == 0)
                    continue;

                if (ping->pos.x < shX)
                    pointX = sX;
                else if (ping->pos.x > shX + 128)
                    pointX = sX + 128;
                else
                    pointX = sX + ping->pos.x - shX;

                if (ping->pos.y < shY)
                    pointY = sY;
                else if (ping->pos.y > shY + 128)
                    pointY = sY + 128;
                else
                    pointY = sY + ping->pos.y - shY;

                pointX--;
                pointY--;

                uint32_t frame = (m_dwCurTime % 700) / 140;
                m_pEffectSpr[168]->PutTransSprite_NoColorKey(pointX, pointY, frame, m_dwCurTime);
                for (int pX = 0; pX < 3; pX++)
                {
                    //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY, 60,220,60);
                    // 					if(pX==1)//DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 50,170,250);
                    // 					else //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 60,220,60);
                    //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 2, 60,220,60);
                }
            }
        }

        m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX - shX + m_sPlayerX, sY - shY + m_sPlayerY, 37, m_dwCurTime);

        if (m_relicX != 0 && m_relicY != 0)
        {
            if (m_relicX >= shX && m_relicX <= shX + 128 && m_relicY >= shY && m_relicY <= shY + 128)
            {
                pointX = (sX - shX + m_relicX) - 1;
                pointY = (sY - shY + m_relicY) - 1;
                for (int pX = 0; pX < 3; pX++)
                {
                    //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY, 230,60,60);
                    // 					if(pX==1)//DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 50,170,250);
                    // 					else //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 230,60,60);
                    //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 2, 230,60,60);
                }
            }
        }

        if ((m_dwCurTime - m_dwMonsterEventTime) < 30000)
        {
            if ((m_dwCurTime % 500)< 370)
            {
                if (m_sEventX >= shX && m_sEventX <= shX + 128 && m_sEventY >= shY && m_sEventY <= shY + 128)
                    m_pSprite[SPRID_INTERFACE_MONSTER]->PutSpriteFast(sX + m_sEventX - shX, sY + m_sEventY - shY, m_sMonsterID, m_dwCurTime);
            }
        }
        else
        {
            m_dwMonsterEventTime = 0;
            m_sMonsterID = 0;
        }
    }
    else // Sans zoom
    {
        if (m_bDialogTrans) m_pSprite[m_iMinMapIndex]->PutTransSprite2(sX, sY, m_iMinMapSquare, m_dwCurTime);
        else m_pSprite[m_iMinMapIndex]->PutSpriteFastNoColorKey(sX, sY, m_iMinMapSquare, m_dwCurTime);

        if (m_iPartyStatus != 0 && m_iIlusionOwnerH == 0)
        {
            for (int i = 0; i < m_stPartyMember.size(); i++)
                if (m_stPartyMember[i]->sX != 0 && m_stPartyMember[i]->sY != 0)
                {
                    pointX = sX + ((m_stPartyMember[i]->sX * 128) / (m_pMapData->m_sMapSizeX)) - 1;
                    pointY = sY + ((m_stPartyMember[i]->sY * 128) / (m_pMapData->m_sMapSizeY)) - 1;
                    for (int pX = 0; pX < 3; pX++)
                    {
                        //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY, 60,215,60);
                        // 						if(pX==1)//DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 50,170,250);
                        // 						else //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 60,215,60);
                        //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 2, 60,215,60);
                    }
                }
        }

        if (m_mapPings.size() > 0)
        {
            std::vector<MapPing>::iterator remIt = remove_if(m_mapPings.begin(), m_mapPings.end(), MapPing::Expired);
            m_mapPings.erase(remIt, m_mapPings.end());

            foreach(ping, m_mapPings)
            {
                if (ping->pos.x == 0 || ping->pos.y == 0)
                    continue;

                pointX = sX + (ping->pos.x * 128) / m_pMapData->m_sMapSizeX - 1;
                pointY = sY + (ping->pos.y * 128) / m_pMapData->m_sMapSizeY - 1;

                uint32_t frame = (m_dwCurTime % 700) / 140;
                m_pEffectSpr[168]->PutTransSprite_NoColorKey(pointX, pointY, frame, m_dwCurTime);
                for (int pX = 0; pX < 3; pX++)
                {
                    //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY, 60,220,60);
                    // 					if(pX==1)//DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 50,170,250);
                    // 					else //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 60,220,60);
                    //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 2, 60,220,60);
                }
            }
        }

        shX = (m_sPlayerX * 128) / (m_pMapData->m_sMapSizeX);
        shY = (m_sPlayerY * 128) / (m_pMapData->m_sMapSizeY);
        m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + shX, sY + shY, 37, m_dwCurTime);

        if (m_relicX != 0 && m_relicY != 0)
        {
            pointX = sX + ((m_relicX * 128) / (m_pMapData->m_sMapSizeX)) - 1;
            pointY = sY + ((m_relicY * 128) / (m_pMapData->m_sMapSizeY)) - 1;
            for (int pX = 0; pX < 3; pX++)
            {
                //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY, 230,60,60);
                // 				if(pX==1)//DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 50,170,250);
                // 				else //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 1, 230,60,60);
                //DIRECTX m_DDraw.PutPixel(pointX + pX, pointY + 2, 230,60,60);
            }
        }

        if ((G_dwGlobalTime - m_dwMonsterEventTime) < 30000)
        {
            if ((m_dwCurTime % 500)< 370)
            {
                shX = (m_sEventX * 128) / (m_pMapData->m_sMapSizeX);
                shY = (m_sEventY * 128) / (m_pMapData->m_sMapSizeY);
                m_pSprite[SPRID_INTERFACE_MONSTER]->PutSpriteFast(sX + shX, sY + shY, m_sMonsterID, m_dwCurTime);
            }
        }
        else
        {
            m_dwMonsterEventTime = 0;
            m_sMonsterID = 0;
        }
    }

    if (G_pGame->m_stMCursor.LB != 0) return;
    if (G_pGame->m_stMCursor.sX >= sX && G_pGame->m_stMCursor.sX < sX + szY && G_pGame->m_stMCursor.sY >= sY && G_pGame->m_stMCursor.sY < sY + szY)
    {
        //if( sY > 213 ) shY = sY - 17;
        if (sY > 273) shY = sY - 17; // 800x600 Resolution xRisenx
        else shY = sY + szY + 4;
        if (m_bZoomMap) PutString(sX, shY, MSG_GUIDEMAP_MIN, video::SColor(255, 200, 200, 120));//"(-)
        else PutString(sX, shY, MSG_GUIDEMAP_MAX, video::SColor(255, 200, 200, 120));//"(+)

        if (m_bZoomMap)
        {
            shX = m_sPlayerX - 64;
            shY = m_sPlayerY - 64;
            if (shX < 0) shX = 0;
            if (shY < 0) shY = 0;
            if (shX > m_pMapData->m_sMapSizeX - 128) shX = m_pMapData->m_sMapSizeX - 128;
            if (shY > m_pMapData->m_sMapSizeY - 128) shY = m_pMapData->m_sMapSizeY - 128;
            shX += G_pGame->m_stMCursor.sX - sX;
            shY += G_pGame->m_stMCursor.sY - sY;
        }
        else
        {
            shX = (G_pGame->m_stMCursor.sX - sX)*m_pMapData->m_sMapSizeX / 128;
            shY = (G_pGame->m_stMCursor.sY - sY)*m_pMapData->m_sMapSizeY / 128;
        }
        wsprintfA(G_cTxt, "%d, %d", shX, shY);
        if (m_cMapIndex == 11) // Aresden: Fixed by Snoopy for v3.51 maps
        {
            if (shX >  46 && shX <  66 && shY > 107 && shY < 127) strcpy(G_cTxt, MSG_MAPNAME_MAGICTOWER);
            else if (shX > 103 && shX < 123 && shY > 86 && shY < 116)  strcpy(G_cTxt, MSG_MAPNAME_GUILDHALL);
            else if (shX > 176 && shX < 196 && shY >  62 && shY < 82)  strcpy(G_cTxt, MSG_MAPNAME_CATH);
            //else if( shX > 135 && shX < 155 && shY > 113 && shY < 133 ) strcpy( G_cTxt, MSG_MAPNAME_CITYHALL );
            else if (shX > 135 && shX < 155 && shY > 113 && shY < 133) strcpy(G_cTxt, MSG_MAPNAME_TOWNHALL);
            else if (shX > 97 && shX < 117 && shY > 175 && shY < 195) strcpy(G_cTxt, MSG_MAPNAME_WAREHOUSE);
            else if (shX > 223 && shX < 243 && shY > 124 && shY < 144) strcpy(G_cTxt, MSG_MAPNAME_WAREHOUSE);
            else if (shX > 118 && shX < 138 && shY > 157 && shY < 177) strcpy(G_cTxt, MSG_MAPNAME_SHOP);
            else if (shX > 148 && shX < 178 && shY > 188 && shY < 208) strcpy(G_cTxt, MSG_MAPNAME_BLACKSMITH);
            //else if( shX > 69 && shX < 89   && shY > 199 && shY < 219 ) strcpy( G_cTxt, MSG_MAPNAME_DUNGEON );
            else if (shX > 69 && shX < 89 && shY > 199 && shY < 219) strcpy(G_cTxt, MSG_MAPNAME_CATACOMBS);
            else if (shX > 21 && shX < 41 && shY > 266 && shY < 286) strcpy(G_cTxt, MSG_MAPNAME_HUNT);
            //else if( shX > 20 && shX < 40  && shY > 13   && shY < 33 )  strcpy( G_cTxt, MSG_MAPNAME_ML );
            else if (shX > 20 && shX < 40 && shY > 13 && shY < 33)  strcpy(G_cTxt, MSG_MAPNAME_OUTLANDS);
            else if (shX > 246 && shX < 266 && shY > 16 && shY < 36)  strcpy(G_cTxt, MSG_MAPNAME_ML);
            //else if( shX > 265 && shX < 285 && shY > 195 && shY < 215 ) strcpy( G_cTxt, MSG_MAPNAME_FARM);
            else if (shX > 265 && shX < 285 && shY > 195 && shY < 215) strcpy(G_cTxt, MSG_MAPNAME_PROMISELAND);
            else if (shX > 88 && shX < 108 && shY > 150 && shY < 170) strcpy(G_cTxt, MSG_MAPNAME_CMDHALL);
        }
        else if (m_cMapIndex == 3) // Elvine: Fixed by Snoopy for v3.51 maps
        {
            if (shX >  170 && shX < 190 && shY >  65 && shY < 85)      strcpy(G_cTxt, MSG_MAPNAME_MAGICTOWER);
            else if (shX >  67 && shX < 87 && shY > 130 && shY < 150)  strcpy(G_cTxt, MSG_MAPNAME_GUILDHALL);
            else if (shX > 121 && shX < 141 && shY >  66 && shY < 86)  strcpy(G_cTxt, MSG_MAPNAME_CATH);
            //else if( shX > 135 && shX < 155 && shY > 117 && shY < 137 ) strcpy( G_cTxt, MSG_MAPNAME_CITYHALL );
            else if (shX > 135 && shX < 155 && shY > 117 && shY < 137) strcpy(G_cTxt, MSG_MAPNAME_TOWNHALL);
            else if (shX > 190 && shX < 213 && shY > 118 && shY < 138) strcpy(G_cTxt, MSG_MAPNAME_WAREHOUSE);
            else if (shX > 73 && shX < 103 && shY > 165 && shY < 185) strcpy(G_cTxt, MSG_MAPNAME_WAREHOUSE);
            else if (shX > 217 && shX < 237 && shY > 142 && shY < 162) strcpy(G_cTxt, MSG_MAPNAME_SHOP);
            else if (shX > 216 && shX < 256 && shY > 99 && shY < 119) strcpy(G_cTxt, MSG_MAPNAME_BLACKSMITH);
            //else if( shX > 251 && shX < 271 && shY >  73 && shY < 93 )  strcpy( G_cTxt, MSG_MAPNAME_DUNGEON );
            else if (shX > 251 && shX < 271 && shY >  73 && shY < 93)  strcpy(G_cTxt, MSG_MAPNAME_CATACOMBS);
            else if (shX > 212 && shX < 232 && shY > 13 && shY < 33)  strcpy(G_cTxt, MSG_MAPNAME_HUNT);
            //else if( shX > 16 && shX < 36   && shY > 262 && shY < 282 ) strcpy( G_cTxt, MSG_MAPNAME_ML );
            else if (shX > 16 && shX < 36 && shY > 262 && shY < 282) strcpy(G_cTxt, MSG_MAPNAME_OUTLANDS);
            else if (shX > 244 && shX < 264 && shY > 248 && shY < 268) strcpy(G_cTxt, MSG_MAPNAME_ML);
            //else if( shX > 264 && shX < 284 && shY > 177 && shY < 207 ) strcpy( G_cTxt, MSG_MAPNAME_FARM);
            else if (shX > 264 && shX < 284 && shY > 177 && shY < 207) strcpy(G_cTxt, MSG_MAPNAME_PROMISELAND);
            else if (shX > 207 && shX < 227 && shY > 79 && shY < 99)  strcpy(G_cTxt, MSG_MAPNAME_CMDHALL);
        }
        else if (m_cMapIndex == 5) // Elvine Farm: Fixed by Snoopy for v3.51 maps
        {
            if (shX >  62 && shX < 82 && shY >  187 && shY < 207) strcpy(G_cTxt, MSG_MAPNAME_WAREHOUSE);
            else if (shX > 81 && shX < 101 && shY > 169 && shY < 189) strcpy(G_cTxt, MSG_MAPNAME_SHOP);
            else if (shX > 101 && shX < 131 && shY > 180 && shY < 200) strcpy(G_cTxt, MSG_MAPNAME_BLACKSMITH);
            else if (shX > 130 && shX < 150 && shY > 195 && shY < 215) strcpy(G_cTxt, MSG_MAPNAME_DUNGEON);
            else if (shX > 86 && shX < 106 && shY > 139 && shY < 159) strcpy(G_cTxt, MSG_MAPNAME_BARRACK);
        }
        else if (m_cMapIndex == 6) // Areden's Farm: Fixed by Snoopy for v3.51 maps
        {
            if (shX >  30 && shX < 50 && shY >  80 && shY < 100) strcpy(G_cTxt, MSG_MAPNAME_WAREHOUSE);
            else if (shX > 55 && shX < 85 && shY > 80 && shY < 100)  strcpy(G_cTxt, MSG_MAPNAME_BLACKSMITH);
            else if (shX > 52 && shX < 72 && shY > 80 && shY < 100)  strcpy(G_cTxt, MSG_MAPNAME_SHOP);
            else if (shX > 70 && shX < 90 && shY > 60 && shY < 80)   strcpy(G_cTxt, MSG_MAPNAME_DUNGEON);
            else if (shX > 45 && shX < 65 && shY > 123 && shY < 143) strcpy(G_cTxt, MSG_MAPNAME_BARRACK);
        }
        /*else if ( m_cMapIndex == 41 )
        {
        if     ( shX > 192 && shX < 212 && shY > 165 && shY < 185 ) strcpy( G_cTxt, "Warehouse");
        else if( shX > 145 && shX < 165 && shY > 190 && shY < 210 ) strcpy( G_cTxt, "BlackSmith");
        else if( shX > 136 && shX < 156 && shY > 162 && shY < 182 ) strcpy( G_cTxt, "Shop");
        else if( shX > 203 && shX < 223 && shY > 140 && shY < 160 ) strcpy( G_cTxt, "Magicshop");
        else if( shX > 80  && shX < 100 && shY > 164 && shY < 184 ) strcpy( G_cTxt, "Church" );
        else if( shX > 92  && shX < 112 && shY > 107 && shY < 127 ) strcpy( G_cTxt, "Cityhall" );
        else if( shX > 55  && shX < 75  && shY > 141 && shY < 161 ) strcpy( G_cTxt, "Warehouse");

        else if( shX > 204 && shX < 224 && shY > 18  && shY < 38  ) strcpy( G_cTxt, "Middleland");
        else if( shX > 37 && shX < 57 && shY > 30  && shY < 50  ) strcpy( G_cTxt, "Middleland");

        else if( shX > 223 && shX < 243 && shY > 28  && shY < 48  ) strcpy( G_cTxt, "Dungeon");
        else if( shX > 43  && shX < 63  && shY > 250 && shY < 270 ) strcpy( G_cTxt, "Huntzone");
        else if( shX > 245  && shX < 265  && shY > 150 && shY < 170 ) strcpy( G_cTxt, "Farm");

        else if( shX > 144  && shX < 153  && shY > 93 && shY < 111 ) strcpy( G_cTxt, "GuildHall");
        else if( shX > 185  && shX < 205  && shY > 75 && shY < 95) strcpy( G_cTxt, "Command Hall");

        else if( shX > 223 && shX < 243 && shY > 28  && shY < 48  ) strcpy( G_cTxt, "Dungeon");
        }*/

        /*else if( m_cMapIndex == 42 )
        {
        if( shX >  30 && shX < 50 && shY >  80 && shY < 100 ) strcpy( G_cTxt, "Warehouse");
        else if( shX > 55 && shX < 85 && shY > 80 && shY < 100 )  strcpy( G_cTxt, "BlackSmith");
        else if( shX > 52 && shX < 72 && shY > 80 && shY < 100 )  strcpy( G_cTxt, "Shop");
        else if( shX > 70 && shX < 90 && shY > 60 && shY < 80 )   strcpy( G_cTxt, "Dungeon");
        else if( shX > 45 && shX < 65 && shY > 123 && shY < 143 ) strcpy( G_cTxt, "Barrack");
        }*/

        else if (m_cMapIndex == 7) // New Beginner Map xRisenx
        {
            if (shX >  57 && shX < 77 && shY >  79 && shY < 99) strcpy(G_cTxt, "Safe Shop");
            else if (shX > 12 && shX < 32 && shY > 63 && shY < 83)  strcpy(G_cTxt, "Aresden");
            else if (shX > 87 && shX < 107 && shY > 16 && shY < 36)  strcpy(G_cTxt, "Elvine");
        }

        else if (m_cMapIndex == 31) // Burning Outlands xRisenx
        {
            if (shX > 125 && shX < 145 && shY > 247 && shY < 267)  strcpy(G_cTxt, "Aresden");
            else if (shX > 179 && shX < 199 && shY > 10 && shY < 30)  strcpy(G_cTxt, "Elvine");
        }

        else if (m_cMapIndex == 32) // Catacombs Of Dawn xRisenx
        {
            if (shX >  144 && shX < 164 && shY >  123 && shY < 143) strcpy(G_cTxt, "Nothing Yet!");
            else if (shX > 254 && shX < 274 && shY > 215 && shY < 235)  strcpy(G_cTxt, "Aresden");
            else if (shX > 60 && shX < 80 && shY > 20 && shY < 40)  strcpy(G_cTxt, "Elvine");
        }

        PutString(G_pGame->m_stMCursor.sX - 10, G_pGame->m_stMCursor.sY - 13, G_cTxt, video::SColor(255, 200, 200, 120));
    }
}

void CGame::DrawDialogBoxes()
{
    int i, k, sprite, frame;
    CDialogBox *dialog;
    bool isTrans;

    if (m_bIsObserverMode == true) return;

    int topDialog = iGetTopDialogBoxIndex();

    m_dialogBoxes[topDialog].SetTop(true);
    ////DIRECTX m_dInput.m_sZ = 0;//DIRECTX

    for (i = 0; i < MAXDIALOGBOX; i++)
        if (m_cDialogBoxOrder[i] != 0)
        {
            dialog = &m_dialogBoxes[m_cDialogBoxOrder[i]];

            isTrans = dialog->GetTrans() ? m_bDialogTrans : false;

            if (dialog->HasBackground(sprite, frame))
            {
                DrawNewDialogBox(sprite, dialog->m_X, dialog->m_Y, frame, false, isTrans);
            }
            if (dialog->HasTitleText(frame))
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, dialog->m_X, dialog->m_Y, frame, false, isTrans);
            }

            dialog->Draw();

#ifdef _DEBUG
            if (m_showBtns)
            {
                std::vector<RECT> rects;
                m_dialogBoxes[m_cDialogBoxOrder[i]].GetAllButtons(&rects);
                k = 1;
                for (std::vector<RECT>::iterator j = rects.begin(); j != rects.end(); j++, k++)
                {
                    DrawLine(j->left, j->top - 1, j->left, j->bottom - 1, 255, 255, 255);
                    DrawLine(j->left - 1, j->top, j->right - 1, j->top, 255, 255, 255);
                    DrawLine(j->left, j->bottom, j->right - 1, j->bottom, 255, 255, 255);
                    DrawLine(j->right, j->top, j->right, j->bottom - 1, 255, 255, 255);
                    wsprintfA(G_cTxt, "%d", k);
                    PutAlignedString(j->left, j->right, j->top - 7 + (j->bottom - j->top) / 2, G_cTxt, video::SColor(255, 255, 255, 255));
                }
            }
#endif

        }
    m_dialogBoxes[topDialog].SetTop(false);

    if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
    {
        if (m_iSuperAttackLeft > 0) // Attack Icon Placement On Action Bar xRisenx
        {
            if (GetAsyncKeyState(VK_MENU) >> 15)
                //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutTransSprite(368+127, 439+120, 3, m_dwCurTime); // 800x600 Resolution xRisenx Right? +127 +120
                m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutTransSprite(12, 557, 45, m_dwCurTime); // Fixed xRisenx
            wsprintfA(G_cTxt, "%d", m_iSuperAttackLeft);
            //PutString_SprFont2(380+127, 454+120, G_cTxt, 220, 200, 200); // 800x600 Resolution xRisenx Right? +127 +120
            PutString_SprFont2(14, 578, G_cTxt, 220, 200, 200);	// Fixed xRisenx
        }
    }
    else
    {
        if (m_iSuperAttackLeft > 0)
        {
            wsprintfA(G_cTxt, "%d", m_iSuperAttackLeft);
            //PutString_SprFont2(380+127, 454+120, G_cTxt, 10, 10, 10); // 800x600 Resolution xRisenx Right? +127 +120
            PutString_SprFont2(14, 578, G_cTxt, 10, 10, 10);	// Fixed xRisenx
        }
    }
}

void CGame::DlgBoxClick_TopPanel() // New Top Bar xRisenx
{
    char onButton = m_dialogBoxes[62].OnButton();
    switch (onButton)
    {
        case 1: // Combat Mode Toggle
            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);
            break;
    }
    if (onButton)
        PlaySound('E', 14, 5);
}

void CGame::DlgBoxClick_IconPanel()
{
    // CLEROTH - LU
    /*	if ((msX > 322)	&& (msX < 355) && (434 < msY) && (475 > msY))
    {	if (m_iLU_Point <= 0) return;
    if (m_bIsDialogEnabled[12] == TRUE)
    DisableDialogBox(12);
    else EnableDialogBox(12);
    PlaySound('E', 14, 5);
    }*/
    char onButton = m_dialogBoxes[30].OnButton();
    switch (onButton)
    {
        case 1: // Crusade
            if (m_bIsCrusadeMode == false) return;
            switch (m_iCrusadeDuty)
            {
                case 1: // Fighter
                    EnableDialogBox(38);
                    break;

                case 2: // Constructor
                    EnableDialogBox(37);
                    break;

                case 3: // Commander
                    EnableDialogBox(36);
                    break;
            }
            break;

        case 2: // Combat Mode Toggle
            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);
            break;

        case 3: // Character
            if (m_bIsDialogEnabled[1] == true)
                DisableDialogBox(1);
            else EnableDialogBox(1);
            break;

        case 4: // Inventory
            if (m_bIsDialogEnabled[2] == true)
                DisableDialogBox(2);
            else EnableDialogBox(2);
            break;

        case 5: // Magic
            if (m_bIsDialogEnabled[3] == true)
                DisableDialogBox(3);
            else EnableDialogBox(3);
            break;

        case 6: // Skill
                //if (m_bIsDialogEnabled[15] == TRUE)
                //	 DisableDialogBox(15);
                //else EnableDialogBox(15);
                //break;
            if (m_bIsDialogEnabled[63] == true)
                DisableDialogBox(63);
            else EnableDialogBox(63);
            break;
        case 9: // Title
            if (m_bIsDialogEnabled[70] == true)
                DisableDialogBox(70);
            else EnableDialogBox(70);
            break;

        case 7: // Chat
            if (m_bIsDialogEnabled[10] == true)
                DisableDialogBox(10);
            else EnableDialogBox(10);
            break;

        case 10: // News
            if (m_bIsDialogEnabled[35] == true)
                DisableDialogBox(35);
            else EnableDialogBox(35);
            break;

        case 8: // System Menu
            if (m_bIsDialogEnabled[19] == true)
                DisableDialogBox(19);
            else EnableDialogBox(19);
            break;
    }
    if (onButton)
        PlaySound('E', 14, 5);
}


void CGame::DlgBoxClick_Party()
{
    char onButton = m_dialogBoxes[32].OnButton();
    switch (m_dialogBoxes[32].GetMode())
    {
        case 0:
            if (m_iPartyStatus == 0)
            {
                if (onButton == 1)
                {
                    m_dialogBoxes[32].SetMode(2);
                    m_bIsGetPointingMode = true;
                    m_iPointCommandType = 200;
                    PlaySound('E', 14, 5);
                }
            }

            if (m_iPartyStatus != 0)
            {
                if (onButton == 2)
                {
                    m_dialogBoxes[32].SetMode(11);
                    PlaySound('E', 14, 5);
                }
            }

            if (m_iPartyStatus != 0)
            {
                if (onButton == 3)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_JOINPARTY, 0, 2, 0, 0, m_cMCName);
                    m_dialogBoxes[32].SetMode(4);
                    PlaySound('E', 14, 5);
                }
            }


            if (onButton == 4)
            {
                m_partyAutoAccept = !m_partyAutoAccept;
                PlaySound('E', 14, 5);
            }

            if (onButton == 5) DisableDialogBox(32);
            break;

        case 1:
            if (onButton == 1)
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_ACCEPTJOINPARTY, 0, 1, 0, 0, m_dialogBoxes[32].cStr);
                DisableDialogBox(32);
                PlaySound('E', 14, 5);
            }

            if (onButton == 2)
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_ACCEPTJOINPARTY, 0, 0, 0, 0, m_dialogBoxes[32].cStr);
                DisableDialogBox(32);
                PlaySound('E', 14, 5);
            }
            break;

        case 2:
            if (onButton == 1)
            {
                m_dialogBoxes[32].SetMode(0);
                PlaySound('E', 14, 5);
            }
            break;

        case 3:
            // cancel
            if (onButton == 1)
            {
                m_dialogBoxes[32].SetMode(0);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_ACCEPTJOINPARTY, 0, 2, 0, 0, m_dialogBoxes[32].cStr);
                DisableDialogBox(32);
                PlaySound('E', 14, 5);
            }
            break;

        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            if (onButton == 1)
            {
                m_dialogBoxes[32].SetMode(0);
                PlaySound('E', 14, 5);
            }
            break;

        case 11:
            if (onButton == 1)
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_JOINPARTY, 0, 0, 0, 0, m_cMCName);
                m_dialogBoxes[32].SetMode(5);
                PlaySound('E', 14, 5);
            }

            if (onButton == 2)
            {
                m_dialogBoxes[32].SetMode(0);
                PlaySound('E', 14, 5);
            }
            break;
    }
}


void CGame::DlgBoxClick_CrusadeJob()
{
    char onButton = m_dialogBoxes[33].OnButton();

    switch (m_dialogBoxes[33].GetMode())
    {
        case 1:
            if (m_side == NEUTRAL)
            {
                DisableDialogBox(33);
                PlaySound('E', 14, 5);
            }
            else
            {
                if (onButton == 1 && m_iGuildRank != GUILDRANK_NONE && gldRankPerm[m_iGuildRank].crusadeCommander)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, 0, 3, 0, 0, 0);
                    DisableDialogBox(33);
                    PlaySound('E', 14, 5);
                }

                if (onButton == 2)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, 0, 1, 0, 0, 0);
                    DisableDialogBox(33);
                    PlaySound('E', 14, 5);
                }

                if (m_iGuildRank != GUILDRANK_NONE)
                {
                    if (onButton == 3)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, 0, 2, 0, 0, 0);
                        DisableDialogBox(33);
                        PlaySound('E', 14, 5);
                    }
                }
            }

            if (onButton == 4)
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 813, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 2:
            if (onButton == 1)
            {
                switch (m_iCrusadeDuty)
                {
                    case 1: EnableDialogBox(18, 803, 0, 0); break;
                    case 2: EnableDialogBox(18, 805, 0, 0); break;
                    case 3: EnableDialogBox(18, 808, 0, 0); break;
                }
            }
            else if (onButton == 2)
            {
                DisableDialogBox(33);
                PlaySound('E', 14, 5);
            }
            break;
    }
}

void CGame::DrawNewDialogBox(char cType, int sX, int sY, int iFrame, bool bIsNoColorKey, bool bIsTrans)
{
    uint32_t dwTime = G_dwGlobalTime;

    if (m_pSprite[cType] == 0) return;
    if (bIsNoColorKey == false)
    {
        if (bIsTrans == true)
            m_pSprite[cType]->PutTransSprite2(sX, sY, iFrame, dwTime);
        else m_pSprite[cType]->PutSpriteFast(sX, sY, iFrame, dwTime);
    }
    else m_pSprite[cType]->PutSpriteFastNoColorKey(sX, sY, iFrame, dwTime);
}

void CGame::DlgBoxClick_Commander() // Snoopy: Fixed for 351
{
    short  tX, tY;
    double d1, d2, d3;
    if (m_bIsCrusadeMode == false) return;

    short sX = m_dialogBoxes[36].m_X;
    short sY = m_dialogBoxes[36].m_Y;
    short msX = G_pGame->m_stMCursor.sX;
    short msY = G_pGame->m_stMCursor.sY;

    char onButton = m_dialogBoxes[36].OnButton();
    switch (m_dialogBoxes[36].GetMode())
    {
        case 0: // Main
            switch (onButton)
            {
                case 1:
                    m_dialogBoxes[36].SetMode(1);
                    PlaySound('E', 14, 5);
                    break;

                case 2:
                    if (m_iTeleportLocX == -1)
                    {
                        SetTopMsg(CRUSADE_MESSAGE15, 5);
                    }
                    else if (strcmp(m_cMapName, m_cTeleportMapName) == 0)
                    {
                        SetTopMsg(CRUSADE_MESSAGE16, 5);
                    }
                    else
                    {
                        m_dialogBoxes[36].SetMode(2);
                        PlaySound('E', 14, 5);
                    }
                    break;

                case 3:
                    m_dialogBoxes[36].SetMode(3);
                    m_dialogBoxes[36].sV1 = 0;
                    PlaySound('E', 14, 5);
                    break;

                case 4:
                    m_dialogBoxes[36].SetMode(4);
                    PlaySound('E', 14, 5);
                    break;

                case 5:
                    DisableDialogBox(18);
                    EnableDialogBox(18, 808, 0, 0);
                    PlaySound('E', 14, 5);
                    break;
            }
            break;

        case 1: // Set TP
            switch (onButton)
            {
                case 1:
                    d1 = (double)(G_pGame->m_stMCursor.sX - (sX + 15));
                    d2 = (double)(250.0f); //(752.0f);
                    d3 = (d2*d1) / 125.0f;
                    tX = (int)d3;
                    d1 = (double)(G_pGame->m_stMCursor.sY - (sY + 60));
                    d2 = (double)(250.0f); //(680.0f);
                    d3 = (d2*d1) / (125.0f); //253.0f;
                    tY = (int)d3;
                    if (tX < 30) tX = 30;
                    if (tY < 30) tY = 30;
                    if (tX > 220) tX = 220;//722;
                    if (tY > 220) tY = 220;//650;
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SETGUILDTELEPORTLOC, 0, tX, tY, 0, "2ndmiddle");
                    m_dialogBoxes[36].SetMode(0);
                    PlaySound('E', 14, 5);
                    _RequestMapStatus("2ndmiddle", 1);
                    break;

                case 2:
                    m_dialogBoxes[36].SetMode(0);
                    PlaySound('E', 14, 5);
                    break;

                case 3:
                    DisableDialogBox(18);
                    EnableDialogBox(18, 809, 0, 0);
                    PlaySound('E', 14, 5);
            }
            break;

        case 2: // Use TP
            switch (onButton)
            {
                case 1:
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GUILDTELEPORT, 0, 0, 0, 0, 0);
                    DisableDialogBox(36);
                    PlaySound('E', 14, 5);
                    break;

                case 2:
                    m_dialogBoxes[36].SetMode(0);
                    PlaySound('E', 14, 5);
                    break;

                case 3:
                    DisableDialogBox(18);
                    EnableDialogBox(18, 810, 0, 0);
                    PlaySound('E', 14, 5);
            }
            break;

        case 3: // Summon Unit
            if (m_side == ARESDEN)
            {
                switch (onButton)
                {
                    case 1:
                        if (m_iConstructionPoint >= 3000)
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 47, 1, m_dialogBoxes[36].sV1, 0);
                            PlaySound('E', 14, 5);
                            DisableDialogBox(36);
                        }
                        break;

                    case 2:
                        if (m_iConstructionPoint >= 2000)
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 46, 1, m_dialogBoxes[36].sV1, 0);
                            PlaySound('E', 14, 5);
                            DisableDialogBox(36);
                        }
                        break;

                    case 3:
                        if (m_iConstructionPoint >= 1000)
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 43, 1, m_dialogBoxes[36].sV1, 0);
                            PlaySound('E', 14, 5);
                            DisableDialogBox(36);
                        }
                        break;

                    case 4:
                        if (m_iConstructionPoint >= 1500)
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 51, 1, m_dialogBoxes[36].sV1, 0);
                            PlaySound('E', 14, 5);
                            DisableDialogBox(36);
                        }
                        break;
                }
            }
            else if (m_side == ELVINE)
            {
                switch (onButton)
                {
                    case 1:
                        if (m_iConstructionPoint >= 3000)
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 45, 1, m_dialogBoxes[36].sV1, 0);
                            PlaySound('E', 14, 5);
                            DisableDialogBox(36);
                        }
                        break;
                    case 2:
                        if (m_iConstructionPoint >= 2000)
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 44, 1, m_dialogBoxes[36].sV1, 0);
                            PlaySound('E', 14, 5);
                            DisableDialogBox(36);
                        }
                        break;
                    case 3:
                        if (m_iConstructionPoint >= 1000)
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 43, 1, m_dialogBoxes[36].sV1, 0);
                            PlaySound('E', 14, 5);
                            DisableDialogBox(36);
                        }
                        break;
                    case 4:
                        if (m_iConstructionPoint >= 1500)
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 51, 1, m_dialogBoxes[36].sV1, 0);
                            PlaySound('E', 14, 5);
                            DisableDialogBox(36);
                        }
                        break;
                }
            }
            switch (onButton)
            {
                case 5:
                    m_dialogBoxes[36].sV1 = 0;
                    PlaySound('E', 14, 5);
                    break;
                case 6:
                    m_dialogBoxes[36].sV1 = 1;
                    PlaySound('E', 14, 5);
                    break;
                case 7:
                    m_dialogBoxes[36].SetMode(0);
                    PlaySound('E', 14, 5);
                    break;
                case 8:
                    DisableDialogBox(18);
                    EnableDialogBox(18, 811, 0, 0);
                    PlaySound('E', 14, 5);
                    break;
            }
            break;

        case 4: // Set constr
            switch (onButton)
            {
                case 1:
                    d1 = (double)(G_pGame->m_stMCursor.sX - (sX + 15));
                    d2 = (double)(250.0);//(752.0f);
                    d3 = (d2*d1) / 125.0f;
                    tX = (int)d3;
                    d1 = (double)(G_pGame->m_stMCursor.sY - (sY + 60));
                    d2 = (double)(250.0);//(680.0f);
                    d3 = (d2*d1) / (125.0);//253.0f;
                    tY = (int)d3;
                    if (tX < 30) tX = 30;
                    if (tY < 30) tY = 30;
                    if (tX > 220) tX = 220;//722;
                    if (tY > 220) tY = 220;//650;
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SETGUILDCONSTRUCTLOC, 0, tX, tY, 0, "2ndmiddle");
                    m_dialogBoxes[36].SetMode(0);
                    PlaySound('E', 14, 5);
                    _RequestMapStatus("2ndmiddle", 1);
                    break;
                case 2:
                    m_dialogBoxes[36].SetMode(0);
                    PlaySound('E', 14, 5);
                    break;
                case 3:
                    DisableDialogBox(18);
                    EnableDialogBox(18, 812, 0, 0);
                    PlaySound('E', 14, 5);
                    break;
            }
            break;
    }
}


void CGame::DlgBoxClick_Constructor()  // Snoopy: Fixed for 351
{ //short sX, sY;
    if (m_bIsCrusadeMode == false) return;
    //sX = m_stDialogBoxInfo[37].sX;
    //sY = m_stDialogBoxInfo[37].sY;
    char onButton = m_dialogBoxes[37].OnButton();
    switch (m_dialogBoxes[37].GetMode())
    {
        case 0: // Main
            switch (onButton)
            {
                case 1:
                    if (m_iConstructLocX == -1)
                    {
                        SetTopMsg(CRUSADE_MESSAGE14, 5);
                    }
                    else
                    {
                        m_dialogBoxes[37].SetMode(1);
                        PlaySound('E', 14, 5);
                    }
                    break;
                case 2:
                    if (m_iTeleportLocX == -1)
                    {
                        SetTopMsg(CRUSADE_MESSAGE15, 5);
                    }
                    else if (strcmp(m_cMapName, m_cTeleportMapName) == 0)
                    {
                        SetTopMsg(CRUSADE_MESSAGE16, 5);
                    }
                    else
                    {
                        m_dialogBoxes[37].SetMode(2);
                        PlaySound('E', 14, 5);
                    }
                    break;
                case 3:
                    DisableDialogBox(18);
                    EnableDialogBox(18, 805, 0, 0);
                    PlaySound('E', 14, 5);
                    break;
            }
            break;

        case 1: // Choose building
            switch (onButton)
            {
                case 1:
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 38, 1, m_dialogBoxes[36].sV1, 0);
                    DisableDialogBox(37);
                    break;

                case 2:
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 39, 1, m_dialogBoxes[36].sV1, 0);
                    DisableDialogBox(37);
                    break;

                case 3:
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 36, 1, m_dialogBoxes[36].sV1, 0);
                    DisableDialogBox(37);
                    break;

                case 4:
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SUMMONWARUNIT, 0, 37, 1, m_dialogBoxes[36].sV1, 0);
                    DisableDialogBox(37);
                    break;

                case 5:
                    m_dialogBoxes[37].SetMode(0);
                    break;

                case 6:
                    DisableDialogBox(18);
                    EnableDialogBox(18, 806, 0, 0);
                    break;
            }
            if (onButton) PlaySound('E', 14, 5);
            break;

        case 2: // Use TP
            switch (onButton)
            {
                case 1:
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GUILDTELEPORT, 0, 0, 0, 0, 0);
                    DisableDialogBox(37);
                    break;

                case 2:
                    m_dialogBoxes[37].SetMode(0);
                    break;

                case 3:
                    DisableDialogBox(18);
                    EnableDialogBox(18, 807, 0, 0);
                    break;
            }
            if (onButton) PlaySound('E', 14, 5);
            break;
    }
}


void CGame::DlgBoxClick_Soldier() // Snoopy: Fixed for 351
{
    if (m_bIsCrusadeMode == false) return;
    char onButton = m_dialogBoxes[38].OnButton();
    switch (m_dialogBoxes[38].GetMode())
    {
        case 0: // Main dlg
            switch (onButton)
            {
                case 1:
                    if (m_iTeleportLocX == -1)
                    {
                        SetTopMsg(CRUSADE_MESSAGE15, 5);
                    }
                    else if (strcmp(m_cMapName, m_cTeleportMapName) == 0)
                    {
                        SetTopMsg(CRUSADE_MESSAGE16, 5);
                    }
                    else
                    {
                        m_dialogBoxes[38].SetMode(1);
                        PlaySound('E', 14, 5);
                    }
                    break;
                case 2:
                    DisableDialogBox(18);
                    EnableDialogBox(18, 803, 0, 0);
                    PlaySound('E', 14, 5);
                    break;
            }
            break;

        case 1: // Use TP
            switch (onButton)
            {
                case 1:
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GUILDTELEPORT, 0, 0, 0, 0, 0);
                    DisableDialogBox(38);
                    break;
                case 2:
                    m_dialogBoxes[38].SetMode(0);
                    break;
                case 3:
                    DisableDialogBox(18);
                    EnableDialogBox(18, 804, 0, 0);
            }
            if (onButton) PlaySound('E', 14, 5);
            break;
    }
}

void CGame::DrawDialogBox_TopPanel()
{
    uint32_t dwTime = m_dwCurTime;

    char onButton = m_dialogBoxes[62].OnButton();
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(m_dialogBoxes[62].m_X, m_dialogBoxes[62].m_Y, 43, dwTime); // Icon Pannel Background

    if (m_bIsCombatMode)
    {
        if (m_bIsSafeAttackMode)
            //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(368+127, 440+120, 4, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
            m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(506 + 127, 433 + 120, 47, dwTime); // Fixed xRisenx
                                                                                                       //else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(368+127, 440+120, 5, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
        else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(506 + 127, 433 + 120, 46, dwTime); // Fixed xRisenx
    }
    PutAlignedString(103 + 25, 245 + 25, 456 + 120, G_cTxt, video::SColor(255, 250, 250, 220)); // Fixed xRisenx
}
void CGame::DrawDialogBox_IconPanel()
{
    uint32_t dwTime = m_dwCurTime;

    char onButton = m_dialogBoxes[30].OnButton();
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(m_dialogBoxes[30].m_X, m_dialogBoxes[30].m_Y, 14, dwTime); // Icon Pannel Background
                                                                                                                       //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(m_dialogBoxes[62].m_X, m_dialogBoxes[62].m_Y, 43, dwTime); // Icon Pannel Background

    if (m_bIsCombatMode)
    {
        if (m_bIsSafeAttackMode)
            //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(368+127, 440+120, 4, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
            //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(506+127, 433+120, 4, dwTime); // Fixed xRisenx
            m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(12, GetHeight() - 43, 47, dwTime); // Fixed xRisenx
                                                                                                       //else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(368+127, 440+120, 5, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
        else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(12, GetHeight() - 43, 46, dwTime); // Fixed xRisenx
    }

    wsprintfA(G_cTxt, "Contri Pts: %d - Majs Pts: %d", m_iContribution, m_iGizonItemUpgradeLeft);
    PutAlignedString(188, 342, 555, G_cTxt, video::SColor(255, 0, 20, 0));
    //wsprintfA(G_cTxt, "Def Ratio: %d - Hit Ratio: %d",m_iAC ,m_iTHAC0); // Armor Def dont show, Hitrate Only shows weaps. Fix later xRisenx
    wsprintfA(G_cTxt, "Def Ratio: %d - Hit Ratio: %d", m_iDefenseRatio, m_iTHAC0); // Hitrate Only shows weaps. Fix later xRisenx
    PutAlignedString(188, 342, 565, G_cTxt, video::SColor(255, 0, 20, 0));

    if ((m_bIsCrusadeMode) && (m_iCrusadeDuty != 0))
    {
        if (m_side == ARESDEN)
        {
            if (onButton == 1)
                //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 1, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
                m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(129, 554, 1, dwTime); // Fixed xRisenx
                                                                                              //else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 2, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
            else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(129, 554, 2, dwTime); // Fixed xRisenx
        }
        else if (m_side == ELVINE)
        {
            if (onButton == 1)
                //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 0, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
                m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(129, 554, 0, dwTime); // Fixed xRisenx
                                                                                              //else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 15, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
            else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(129, 554, 15, dwTime); // Fixed xRisenx
        }
    }

    //// CLEROTH - LU
    //if ( m_iLU_Point > 0 ) {
    //	if (onButton == 1)
    //			 m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 17, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
    //		else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 18, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
    //}

    short textX = G_pGame->m_stMCursor.sX - 10;
    short textY = G_pGame->m_stMCursor.sY - 20;
    if (onButton == 3)
    { // Character    
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(573, 555, 49, dwTime);
        wsprintfA(G_cTxt, "Character Menu");
        PutString(textX, textY, G_cTxt, video::SColor(255, 250, 250, 220));
    }
    else if (onButton == 4)
    { // Inventory
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(610, 555, 50, dwTime);
        wsprintfA(G_cTxt, "Inventory");
        PutString(textX, textY, G_cTxt, video::SColor(255, 250, 250, 220));
    }
    else if (onButton == 5)
    { // Magic
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(647, 555, 51, dwTime);
        wsprintfA(G_cTxt, "Spell Book");
        PutString(textX, textY, G_cTxt, video::SColor(255, 250, 250, 220));
    }
    else if (onButton == 6)
    { // Skill
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(684, 555, 52, dwTime);
        wsprintfA(G_cTxt, "Title List"); // Titles xRisenx
        PutString(textX, textY, G_cTxt, video::SColor(255, 250, 250, 220));
    }
    else if (onButton == 7)
    { // History
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(721, 555, 53, dwTime);
        wsprintfA(G_cTxt, "Chat History");
        PutString(textX, textY, G_cTxt, video::SColor(255, 250, 250, 220));
    }
    else if (onButton == 8)
    { // System Menu
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(758, 555, 54, dwTime);
        wsprintfA(G_cTxt, "Options");
        PutString(textX, textY, G_cTxt, video::SColor(255, 250, 250, 220));
    }

    // Experience Status Bar xRisenx
    if (onButton == 12)
    {
        wsprintfA(G_cTxt, MSG_EXP "%d / %d", m_iExp, iGetLevelExp(m_iLevel + 1));
        PutString(textX, textY, G_cTxt, video::SColor(255, 250, 250, 220));
    }

    // Magic Icons xRisenx
    if (m_bPfm)
    {
        wsprintfA(G_cTxt, "%d", m_sPfm);
        PutString2(551, 585, G_cTxt, 255, 255, 255);
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(542, 560, 22, dwTime);
    }
    if (m_bPfm)
    {
        wsprintfA(G_cTxt, "Pfm");
        PutString2(547, 548, G_cTxt, 255, 255, 255);
    }
    if (m_bZerk)
    {
        wsprintfA(G_cTxt, "%d", m_sZerk);
        PutString2(522, 585, G_cTxt, 255, 255, 255);
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(513, 560, 21, dwTime);
    }
    if (m_bZerk)
    {
        wsprintfA(G_cTxt, "Zerk");
        PutString2(517, 548, G_cTxt, 255, 255, 255);
    }
    if (m_bInv)
    {
        wsprintfA(G_cTxt, "%d", m_sInv);
        PutString2(493, 585, G_cTxt, 255, 255, 255);
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(484, 560, 20, dwTime);
    }
    if (m_bInv)
    {
        wsprintfA(G_cTxt, "Inv");
        PutString2(490, 548, G_cTxt, 255, 255, 255);
    }
    if (m_bShield)
    {
        wsprintfA(G_cTxt, "%d", m_sShield);
        PutString2(551, 585, G_cTxt, 255, 255, 255);
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(542, 560, 24, dwTime);
    }
    if (m_bShield)
    {
        wsprintfA(G_cTxt, "Def");
        PutString2(548, 548, G_cTxt, 255, 255, 255);
    }
    if (m_bPfa)
    {
        wsprintfA(G_cTxt, "%d", m_sPfa);
        PutString2(551, 585, G_cTxt, 255, 255, 255);
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(542, 560, 23, dwTime);
    }
    if (m_bPfa)
    {
        wsprintfA(G_cTxt, "Pfa");
        PutString2(548, 548, G_cTxt, 255, 255, 255);
    }
#ifdef MonsterBarClient
    if (bNpcBar)
    {
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(20, 35, 26, dwTime);		// ShadowEvil edited for mouse cordinates
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(20, 42, 27, dwTime);		// ShadowEvil edited for mouse cordinates
    }
#endif
    wsprintfA(G_cTxt, "%s(%d,%d)", m_cMapMessage, m_sPlayerX, m_sPlayerY);
    PutAlignedString(188, 342, 576, G_cTxt, video::SColor(255, 250, 250, 220)); // Fixed xRisenx

}

void CGame::DrawDialogBox_GaugePanel()
{
    uint32_t iMaxPoint, iBarWidth, iTemp;
    // HP bar
    //iMaxPoint = m_stat[STAT_VIT]*3 + m_iLevel*2 + m_stat[STAT_STR]/2;
    iMaxPoint = m_stat[STAT_VIT] * 8 + m_stat[STAT_STR] * 2 + m_stat[STAT_INT] * 2 + m_iLevel * 8; // HP System xRisenx
    if (m_iHP > iMaxPoint) m_iHP = iMaxPoint;
    /*iBarWidth = 101 - (m_iHP*101)/iMaxPoint;
    if( iBarWidth < 0 ) iBarWidth = 0;
    if( iBarWidth > 101 ) iBarWidth = 101;*/
    iBarWidth = 66 - (m_iHP * 66) / iMaxPoint;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 66) iBarWidth = 66;
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(23, 437,  12, iBarWidth, m_dwCurTime);
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(23, 437+120,  12, iBarWidth, m_dwCurTime); // 800x600 Resolution xRisenx Right?
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(4, 434+120,  12, iBarWidth, m_dwCurTime); // Fixed xRisenx
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(53, 556, 44, iBarWidth, m_dwCurTime); // Fixed xRisenx

    wsprintfA(G_cTxt, "(%d)", (short)m_iHP);
    if (m_bIsPoisoned)
    {
        //PutString_SprNum(85, 441+120, G_cTxt, m_wR[5]*11, m_wG[5]*11, m_wB[5]*11); // 800x600 Resolution xRisenx Right?
        PutString_SprNum(77, 557, G_cTxt, m_wR[5] * 11, m_wG[5] * 11, m_wB[5] * 11); // Fixed xRisenx
                                                                                     //PutString_SprFont3(35, 439+120, "Poisoned", m_wR[5]*8, m_wG[5]*8, m_wB[5]*8, TRUE, 2); // 800x600 Resolution xRisenx Right?
        PutString_SprFont3(77, 557, "", m_wR[5] * 8, m_wG[5] * 8, m_wB[5] * 8, true, 2); // Fixed xRisenx
    }
    //else PutString_SprNum(80, 441+120, G_cTxt, 200, 100, 100); // 800x600 Resolution xRisenx Right?
    //else PutString_SprNum(58, 435+120, G_cTxt, 200, 100, 100); // Fixed xRisenx
    else PutString_SprNum(77, 557, G_cTxt, 200, 100, 100); // Fixed xRisenx

                                                           //MP bar
    iMaxPoint = m_stat[STAT_MAG] * 3 + m_iLevel * 2 + m_stat[STAT_INT] * 2; // MP System xRisenx
    if (m_iMP > iMaxPoint) m_iMP = iMaxPoint;
    iBarWidth = 66 - (m_iMP * 66) / iMaxPoint;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 66) iBarWidth = 66;
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(23, 459,  12, iBarWidth, m_dwCurTime);
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(23, 459+120,  12, iBarWidth, m_dwCurTime); // 800x600 Resolution xRisenx Right?
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(53, 571, 44, iBarWidth, m_dwCurTime); // Fixed xRisenx
    wsprintfA(G_cTxt, "%d", m_iMP);
    //PutString_SprNum(80, 463+120, G_cTxt, 100, 100, 200);  // 800x600 Resolution xRisenx Right?
    PutString_SprNum(77, 572, G_cTxt, 100, 100, 200);  // Fixed xRisenx

                                                       // SP bar
    iMaxPoint = m_iLevel * 17 + m_stat[STAT_STR]; // SP System xRisenx
    if (m_iSP > iMaxPoint) m_iSP = iMaxPoint;
    //iBarWidth = 167 - (m_iSP*167)/iMaxPoint;
    iBarWidth = 66 - (m_iSP * 66) / iMaxPoint; // Fixed xRisenx
    if (iBarWidth < 0) iBarWidth = 0;
    //if( iBarWidth > 167 ) iBarWidth = 167;
    if (iBarWidth > 66) iBarWidth = 66; // Fixed xRisenx
                                        //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(147, 434, 13, iBarWidth, m_dwCurTime);
                                        //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(147, 434+120, 13, iBarWidth, m_dwCurTime); // 800x600 Resolution xRisenx Right?
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(53, 586, 44, iBarWidth, m_dwCurTime); // Fixed xRisenx
    iTemp = m_iSP;
    wsprintfA(G_cTxt, "%d", iTemp, iMaxPoint);
    //PutString_SprNum(225, 436+120, G_cTxt, 100, 100, 200); // 800x600 Resolution xRisenx Right?
    PutString_SprNum(77, 587, G_cTxt, 100, 200, 100); // Fixed xRisenx

                                                      // Experience Gauge
    iMaxPoint = m_levelExpTable[m_iLevel + 1] - m_levelExpTable[m_iLevel];
    iTemp = m_iExp - m_levelExpTable[m_iLevel];
    //iBarWidth = 167 - (iTemp*167)/iMaxPoint;
    //iBarWidth = 800 - (iTemp*800)/iMaxPoint; // Fixed xRisenx
    iBarWidth = (iTemp * 800) / iMaxPoint; // Fixed xRisenx
    if (iBarWidth < 0) iBarWidth = 0;
    //if( iBarWidth > 167 ) iBarWidth = 167;
    if (iBarWidth > 800) iBarWidth = 800; // Fixed xRisenx
                                          //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(147 + iBarWidth, 434/*+13*/+120, 13, (167-iBarWidth), m_dwCurTime); // 800x600 Resolution xRisenx Right?
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(0, 427 + 120, 17, iBarWidth, m_dwCurTime); // Fixed xRisenx
    iTemp = iTemp - iMaxPoint;
    //wsprintfA(G_cTxt, "%d", iTemp);
    //PutString_SprNum(216, 449+120, G_cTxt, 100, 100, 200); // 800x600 Resolution xRisenx Right?
    //PutString_SprNum(216, 449+120, G_cTxt, 100, 100, 200); // 800x600 Resolution xRisenx Right?

    // Hunger Bar xRisenx
    iMaxPoint = 100;
    iTemp = iHungerStatus;
    iBarWidth = 42 - (iHungerStatus * 42) / 100;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 42) iBarWidth = 42;
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(123, 556, 25, iBarWidth, m_dwCurTime);
    iTemp = iHungerStatus;
#ifdef MonsterBarClient
    if (bNpcBar)
    {
        iMaxPoint = 100;
        iBarWidth = 66 - (iNpcBar_HP * 66) / iMaxPoint;
        if (iBarWidth < 0) iBarWidth = 0;
        if (iBarWidth > 66) iBarWidth = 66;
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(20, 35, 28, iBarWidth, m_dwCurTime);		// ShadowEvil Edited for Mouse cordinates
        iTemp = iNpcBar_HP;

        iMaxPoint = 100;
        iBarWidth = 66 - (iNpcBar_MP * 66) / iMaxPoint;
        if (iBarWidth < 0) iBarWidth = 0;
        if (iBarWidth > 66) iBarWidth = 66;
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(20, 42, 28, iBarWidth, m_dwCurTime);		// ShadowEvil Edited for Mouse cordinates
        iTemp = iNpcBar_MP;
    }
#endif
}

void CGame::DrawDialogBox_Inventory()
{
    int i;
    short sX, sY;
    uint32_t dwTime = m_dwCurTime;
    uint32_t ItemColor;
    int uTotalItem = 0;
    char onButton = m_dialogBoxes[2].OnButton();
    sX = m_dialogBoxes[2].m_X;
    sY = m_dialogBoxes[2].m_Y;
    //DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX, sY, 0);
    for (i = 0; i < MAXITEMS; i++)
        if ((m_cItemOrder[i] != -1) && (m_pItemList[m_cItemOrder[i]] != 0))
        {
            uTotalItem++; // Added Current/Max xRisenx
            if (((m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_ITEM) && (m_stMCursor.sSelectedObjectID == m_cItemOrder[i])) || (m_bIsItemEquipped[m_cItemOrder[i]] == true))
            {

            }
            else
            {
                ItemColor = m_pItemList[m_cItemOrder[i]]->m_ItemColor;
                if (m_bIsItemDisabled[m_cItemOrder[i]] == true)
                {
                    if (ItemColor == 0)
                        m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->PutTransSprite2(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                                                                                                                        sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame, dwTime);
                    else
                    {
                        if ((m_pItemList[m_cItemOrder[i]]->m_cEquipPos == EQUIPPOS_LHAND)
                            || (m_pItemList[m_cItemOrder[i]]->m_cEquipPos == EQUIPPOS_RHAND)
                            || (m_pItemList[m_cItemOrder[i]]->m_cEquipPos == EQUIPPOS_TWOHAND))
                        {
                            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->PutTransSpriteRGB(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                                                                                                                              sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame,
                                                                                                                              ItemColor,
                                                                                                                              dwTime);
                        }
                        else
                        {
                            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->PutTransSpriteRGB(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                                                                                                                              sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame,
                                                                                                                              ItemColor,
                                                                                                                              dwTime);
                        }
                    }
                }
                else
                {
                    if (ItemColor == 0)
                        m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->PutSpriteFast(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                                                                                                                      sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame, dwTime);
                    else
                    {
                        if ((m_pItemList[m_cItemOrder[i]]->m_cEquipPos == EQUIPPOS_LHAND)
                            || (m_pItemList[m_cItemOrder[i]]->m_cEquipPos == EQUIPPOS_RHAND)
                            || (m_pItemList[m_cItemOrder[i]]->m_cEquipPos == EQUIPPOS_TWOHAND))
                        {

                            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->PutSpriteRGB(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                                                                                                                         sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame,
                                                                                                                         ItemColor,
                                                                                                                         dwTime);
                        }
                        else
                        {
                            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->PutSpriteRGB(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                                                                                                                         sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame,
                                                                                                                         ItemColor,
                                                                                                                         dwTime);
                        }
                    }
                }
                if ((m_pItemList[m_cItemOrder[i]]->m_cItemType == ITEMTYPE_CONSUME)
                    || (m_pItemList[m_cItemOrder[i]]->m_cItemType == ITEMTYPE_ARROW))
                {
                    DisplayCommaNumber_G_cTxt((int)m_pItemList[m_cItemOrder[i]]->m_dwCount); // nbe show, as US: 1,200,000
                    PutString2(sX + 29 + m_pItemList[m_cItemOrder[i]]->m_sX + 10, sY + 41 + m_pItemList[m_cItemOrder[i]]->m_sY + 10, G_cTxt, 200, 200, 200);
                }
            }
        }
    if (onButton == 1)
    {
        DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX + 23, sY + 172, 1);
    }
    else if (onButton == 2)
    {
        DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX + 140, sY + 172, 2);
    }

    wsprintfA(G_cTxt, "%d/%d", uTotalItem, MAXITEMS);
    PutString2(sX + 102, sY + 174, G_cTxt, 200, 200, 200);
}

bool CGame::DlgBoxPress_Character()
{
    int i;
    short sX, sY, sSprH, sFrame;
    char cEquipPoiStatus[MAXITEMEQUIPPOS];

    if (m_bIsDialogEnabled[17] == true) return false;

    short msX = G_pGame->m_stMCursor.sX;
    short msY = G_pGame->m_stMCursor.sY;
    sX = m_dialogBoxes[1].m_X;
    sY = m_dialogBoxes[1].m_Y;
    for (i = 0; i < MAXITEMEQUIPPOS; i++) cEquipPoiStatus[i] = -1;
    for (i = 0; i < MAXITEMS; i++)
    {
        if ((m_pItemList[i] != 0) && (m_bIsItemEquipped[i] == true))	cEquipPoiStatus[m_pItemList[i]->m_cEquipPos] = i;
    }

    if ((m_sPlayerType >= 1) && (m_sPlayerType <= 3))
    {
        if (cEquipPoiStatus[EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 72, sY + 135, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_HEAD];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 32, sY + 193, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_RFINGER];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_LFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 100, sY + 185, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_LFINGER];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 35, sY + 120, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_NECK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 57, sY + 186, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_TWOHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 57, sY + 186, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_RHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 90, sY + 170, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_LHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_FULLBODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_BODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_BOOTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_ARMS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_PANTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 41, sY + 137, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_BACK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
    }
    else if ((m_sPlayerType >= 4) && (m_sPlayerType <= 6))
    {
        if (cEquipPoiStatus[EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 72, sY + 139, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_HEAD];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 32, sY + 193, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_RFINGER];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_LFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 100, sY + 185, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_LFINGER];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 35, sY + 120, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_NECK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 60, sY + 191, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_TWOHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 60, sY + 191, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_RHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 84, sY + 175, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_LHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_BODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_FULLBODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_BOOTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_ARMS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_PANTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 45, sY + 143, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[EQUIPPOS_BACK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
    }
    return false;
}

void CGame::DlgBoxClick_CityhallMenu()
{
    const enum modes
    {
        LEGIONPTSSERVICES = 9,
        TELEPORTLIST = 10
    };
    short sX, sY;
    int i;
    sX = m_dialogBoxes[13].m_X;
    sY = m_dialogBoxes[13].m_Y;
    short szX = m_dialogBoxes[13].sSizeX;
    char onButton = m_dialogBoxes[13].OnButton();
    switch (m_dialogBoxes[13].GetMode())
    {
        case 0:
            switch (onButton)
            {
                case 1:
                    if (m_side != NEUTRAL) return;
                    m_dialogBoxes[13].SetMode(1); // citizenship rq
                    break;
                case 2:
                    if (m_iRewardGold <= 0) return;
                    m_dialogBoxes[13].SetMode(5); // rq reward gold
                    break;
                case 3:
                    if (m_iEnemyKillCount < 100) return;
                    m_dialogBoxes[13].SetMode(7);
                    break;
                case 4:
                    if (m_stQuest.bIsQuestCompleted) return;
                    if (m_stQuest.sQuestType == 0) return;
                    m_dialogBoxes[13].SetMode(8);
                    break;
                case 5: // Fantasy point services
                    m_dialogBoxes[13].SetMode(LEGIONPTSSERVICES);
                    bSendCommand(MSGID_REQUEST_LGNPTS, 0, 0, 0, 0, 0, 0);
                    break;
                case 6: // Teleport List
                    m_dialogBoxes[13].SetMode(TELEPORTLIST);
                    bSendCommand(MSGID_REQUEST_TELEPORT_LIST, 0, 0, 0, 0, 0, "William");
                    break;
                case 7: // Quest List
                    bSendCommand(MSGID_REQUEST_QUEST_LIST, 0, 0, 0, 0, 0, "William");
                    EnableDialogBox(61);
                    break;
                case 8: // Change Part in Crusade
                    if (m_bIsCrusadeMode == false) return;
                    EnableDialogBox(33, 1, 0, 0);
                    break;
            }
            if (onButton) PlaySound('E', 14, 5);
            break;

        case 1:
            if (onButton == 1)
            {	// Yes Click
                bSendCommand(MSGID_REQUEST_CIVILRIGHT, MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
                m_dialogBoxes[13].SetMode(2);
            }
            else if (onButton == 2)
            {	// No Click
                m_dialogBoxes[13].SetMode(0);
            }
            if (onButton) PlaySound('E', 14, 5);
            break;

        case 3:	//
        case 4:
            if (onButton == 1)
            {
                // No Click
                m_dialogBoxes[13].SetMode(0);
                PlaySound('E', 14, 5);
            }
            break;

        case 5:
            if (onButton == 1)
            {
                // Yes
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_GETREWARDMONEY, 0, 0, 0, 0, 0);
                m_dialogBoxes[13].SetMode(0);
            }
            else if (onButton == 2)
            {
                // No
                m_dialogBoxes[13].SetMode(0);
            }
            if (onButton) PlaySound('E', 14, 5);
            break;

            // 3.51 Cityhall Menu - Request Hero's Items - Diuuude - fix by Drajwer
        case 7:
            int iReqHeroItemID;
            if (onButton)
            {
                ZeroMemory(m_cTakeHeroItemName, sizeof(m_cTakeHeroItemName));
                m_dialogBoxes[13].SetMode(11);
                PlaySound('E', 14, 5);
            }

            switch (onButton)
            {
                case 1: // Hero's Cape
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU47, strlen(DRAW_DIALOGBOX_CITYHALL_MENU47));
                    m_dialogBoxes[13].sV1 = HR_CAPE;
                    break;
                case 2:// Hero's Helm
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU48, strlen(DRAW_DIALOGBOX_CITYHALL_MENU48));
                    m_dialogBoxes[13].sV1 = HR_HELM;
                    break;
                case 3:// Hero's Cap
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU49, strlen(DRAW_DIALOGBOX_CITYHALL_MENU49));
                    m_dialogBoxes[13].sV1 = HR_CAP;
                    break;
                case 4:// Hero's Armor
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU50, strlen(DRAW_DIALOGBOX_CITYHALL_MENU50));
                    m_dialogBoxes[13].sV1 = HR_PLATE;
                    break;
                case 5:	// Hero's Robe
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU51, strlen(DRAW_DIALOGBOX_CITYHALL_MENU51));
                    m_dialogBoxes[13].sV1 = HR_ROBE;
                    break;
                case 6:	// Hero's Hauberk
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU52, strlen(DRAW_DIALOGBOX_CITYHALL_MENU52));
                    m_dialogBoxes[13].sV1 = HR_HAUBERK;
                    break;
                case 7: // Hero's Leggings
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU53, strlen(DRAW_DIALOGBOX_CITYHALL_MENU53));
                    m_dialogBoxes[13].sV1 = HR_LEGGINGS;
            }

            break;

        case 8:
            if (onButton == 1)
            { // Yes
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_CANCELQUEST, 0, 0, 0, 0, 0);
            }

            if (onButton)
            { //Either button
                m_dialogBoxes[13].SetMode(0);
                PlaySound('E', 14, 5);
            }
            break;

        case LEGIONPTSSERVICES:
            if (onButton == 1)
            {
                PlaySound('E', 14, 5);
                bSendCommand(MSGID_REQUEST_LGNPTS, 0, 0, 0, 0, 0, 0);
                break;
            }

            i = onButton - 2;
            if (onButton >= 2)
            {
                uint16_t cmd = lgnPtsSvcs[i + m_dialogBoxes[13].sView].cmd;

                if (cmd >= CMD_LGNSVC_TRADETOKEN1 && cmd <= CMD_LGNSVC_TRADETOKEN100)
                {
                    if (GetItemCount("XtremeTokens") >= lgnPtsSvcs[i + m_dialogBoxes[13].sView].price
                        || GetItemCount("XtremeDonateTokens") >= lgnPtsSvcs[i + m_dialogBoxes[13].sView].price)
                    {
                        bSendCommand(MSGID_REQUEST_LGNSVC, cmd, 0, 0, 0, 0, 0);
                        m_cash += lgnPtsSvcs[i + m_dialogBoxes[13].sView].price;
                    }
                    else
                    {
                        AddEventList(MSG_NOTIFY_LEGIONPT4, CHAT_GM, true);
                    }
                }
                else if (cmd >= CMD_LGNSVC_TRADESCAN && cmd <= CMD_LGNSVC_TRADEBSW)
                {
                    int32_t spellid = -1;

                    switch (cmd)
                    {
                        case CMD_LGNSVC_TRADESCAN:
                            spellid = 67;
                            break;
                        case CMD_LGNSVC_TRADEMAGICSHIELD:
                            spellid = 84;
                            break;
                        case CMD_LGNSVC_TRADEICESTORM:
                            spellid = 55;
                            break;
                        case CMD_LGNSVC_TRADEMASSHEAL:
                            spellid = 85;
                            break;
                        case CMD_LGNSVC_TRADESOTG:
                            spellid = 87;
                            break;
                        case CMD_LGNSVC_TRADECOTG:
                            spellid = 92;
                            break;
                        case CMD_LGNSVC_TRADELC:
                            spellid = 88;
                            break;
                        case CMD_LGNSVC_TRADEMB:
                            spellid = 98;
                            break;
                        case CMD_LGNSVC_TRADEFSW:
                            spellid = 97;
                            break;
                        case CMD_LGNSVC_TRADEBSW:
                            spellid = 70;
                            break;
                    }

                    if (spellid != -1 && m_cMagicMastery[spellid])
                    {
                        bSendCommand(MSGID_REQUEST_LGNSVC, cmd, 0, 0, 0, 0, 0);
                        m_cash -= lgnPtsSvcs[i + m_dialogBoxes[13].sView].price;
                    }
                    else
                    {
                        AddEventList(MSG_NOTIFY_LEGIONPT5, CHAT_GM, true);
                    }
                }
                else
                {
                    if (m_cash >= lgnPtsSvcs[i + m_dialogBoxes[13].sView].price)
                    {
                        bSendCommand(MSGID_REQUEST_LGNSVC, cmd, 0, 0, 0, 0, 0);
                        m_cash -= lgnPtsSvcs[i + m_dialogBoxes[13].sView].price;
                    }
                    else
                    {
                        AddEventList(MSG_NOTIFY_LEGIONPT1, CHAT_GM, true);
                        AddEventList(MSG_NOTIFY_LEGIONPT2, CHAT_GM, true);
                        AddEventList(MSG_NOTIFY_LEGIONPT3, CHAT_GM, true);
                    }
                }
            }
            break;

        case TELEPORTLIST:
            if (m_iTeleportMapCount > 0)
            {
                i = onButton - 1;
                if (onButton)
                {
                    bSendCommand(MSGID_REQUEST_CHARGED_TELEPORT, 0, 0, m_stTeleportList[i].iIndex, 0, 0, 0);
                    DisableDialogBox(51);
                    return;
                }
            }
            break;

        case 11: // Hero confirm
            if (onButton == 1)
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_GETHEROMANTLE, 0, m_dialogBoxes[13].sV1, 0, 0, 0);
                m_dialogBoxes[13].SetMode(0);
            }
            else if (onButton == 2)
            {
                m_dialogBoxes[13].SetMode(7);
            }
            if (onButton) PlaySound('E', 14, 5);
            break;
    }
}

void CGame::DlgBoxClick_ShutDownMsg()
{
    if (m_dialogBoxes[25].OnButton() == 1)
    {
        DisableDialogBox(25);
        PlaySound('E', 14, 5);
    }
}

bool CGame::DlgBoxPress_SkillDlg()
{
    int i, iAdjX, iAdjY;
    char  cItemID;
    short sX, sY, x1, y1, x2, y2, sArray[10];

    short msX = G_pGame->m_stMCursor.sX;
    short msY = G_pGame->m_stMCursor.sY;
    sX = m_dialogBoxes[26].m_X;
    sY = m_dialogBoxes[26].m_Y;
    iAdjX = 5;
    iAdjY = 10;

    switch (m_dialogBoxes[26].GetMode())
    {
        case 1:
            ZeroMemory(sArray, sizeof(sArray));
            sArray[1] = m_dialogBoxes[26].sV1;
            sArray[2] = m_dialogBoxes[26].sV2;
            sArray[3] = m_dialogBoxes[26].sV3;
            sArray[4] = m_dialogBoxes[26].sV4;
            sArray[5] = m_dialogBoxes[26].sV5;
            sArray[6] = m_dialogBoxes[26].sV6;
            for (i = 1; i <= 6; i++)
                if ((sArray[i] != -1) && (m_pItemList[sArray[i]] != 0))
                {
                    cItemID = (char)sArray[i];
                    switch (i)
                    {
                        case 1: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55, sY + iAdjY + 55, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 2: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 1, sY + iAdjY + 55, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 3: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 2, sY + iAdjY + 55, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 4: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55, sY + iAdjY + 100, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 5: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 1, sY + iAdjY + 100, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 6: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 2, sY + iAdjY + 100, m_pItemList[cItemID]->m_sSpriteFrame); break;
                    }
                    x1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
                    y1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
                    x2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
                    y2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;
                    if ((msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
                    {
                        switch (i)
                        {
                            case 1: m_dialogBoxes[26].sV1 = -1; break;
                            case 2: m_dialogBoxes[26].sV2 = -1; break;
                            case 3: m_dialogBoxes[26].sV3 = -1; break;
                            case 4: m_dialogBoxes[26].sV4 = -1; break;
                            case 5: m_dialogBoxes[26].sV5 = -1; break;
                            case 6: m_dialogBoxes[26].sV6 = -1; break;
                        }
                        m_bIsItemDisabled[cItemID] = false;
                        m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                        m_stMCursor.sSelectedObjectID = cItemID;
                        m_stMCursor.sDistX = msX + iAdjX - x1 + (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotX;
                        m_stMCursor.sDistY = msY + iAdjY - y1 + (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotY;
                        return true;
                    }
                }
            break;

        case 4:
            ZeroMemory(sArray, sizeof(sArray));
            sArray[1] = m_dialogBoxes[26].sV1;
            sArray[2] = m_dialogBoxes[26].sV2;
            sArray[3] = m_dialogBoxes[26].sV3;
            sArray[4] = m_dialogBoxes[26].sV4;
            sArray[5] = m_dialogBoxes[26].sV5;
            sArray[6] = m_dialogBoxes[26].sV6;
            for (i = 1; i <= 6; i++)
                if ((sArray[i] != -1) && (m_pItemList[sArray[i]] != 0))
                {
                    cItemID = (char)sArray[i];
                    switch (i)
                    {
                        case 1: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 55 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 2: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 55 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 3: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 55 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 4: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 100 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 5: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 100 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 6: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 100 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                    }
                    x1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
                    y1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
                    x2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
                    y2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;

                    if ((msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
                    {
                        switch (i)
                        {
                            case 1: m_dialogBoxes[26].sV1 = -1; break;
                            case 2: m_dialogBoxes[26].sV2 = -1; break;
                            case 3: m_dialogBoxes[26].sV3 = -1; break;
                            case 4: m_dialogBoxes[26].sV4 = -1; break;
                            case 5: m_dialogBoxes[26].sV5 = -1; break;
                            case 6: m_dialogBoxes[26].sV6 = -1; break;
                        }
                        m_bIsItemDisabled[cItemID] = false;
                        m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                        m_stMCursor.sSelectedObjectID = cItemID;
                        m_stMCursor.sDistX = msX + iAdjX - x1 + (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotX;
                        m_stMCursor.sDistY = msY + iAdjY - y1 + (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotY;
                        m_dialogBoxes[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                        return true;
                    }
                }
            break;
            // Crafting
        case 7:
            ZeroMemory(sArray, sizeof(sArray));
            sArray[1] = m_dialogBoxes[26].sV1;
            sArray[2] = m_dialogBoxes[26].sV2;
            sArray[3] = m_dialogBoxes[26].sV3;
            sArray[4] = m_dialogBoxes[26].sV4;
            sArray[5] = m_dialogBoxes[26].sV5;
            sArray[6] = m_dialogBoxes[26].sV6;
            for (i = 1; i <= 6; i++)
                if ((sArray[i] != -1) && (m_pItemList[sArray[i]] != 0))
                {
                    cItemID = (char)sArray[i];
                    switch (i)
                    {
                        case 1: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55, sY + iAdjY + 55, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 2: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 65 + 45 * 1, sY + iAdjY + 40, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 3: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 65 + 45 * 2, sY + iAdjY + 55, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 4: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 65, sY + iAdjY + 100, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 5: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 65 + 45 * 1, sY + iAdjY + 115, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 6: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 75 + 45 * 2, sY + iAdjY + 100, m_pItemList[cItemID]->m_sSpriteFrame); break;
                    }
                    x1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
                    y1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
                    x2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
                    y2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;
                    if ((msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
                    {
                        switch (i)
                        {
                            case 1: m_dialogBoxes[26].sV1 = -1; break;
                            case 2: m_dialogBoxes[26].sV2 = -1; break;
                            case 3: m_dialogBoxes[26].sV3 = -1; break;
                            case 4: m_dialogBoxes[26].sV4 = -1; break;
                            case 5: m_dialogBoxes[26].sV5 = -1; break;
                            case 6: m_dialogBoxes[26].sV6 = -1; break;
                        }
                        m_bIsItemDisabled[cItemID] = false;
                        m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                        m_stMCursor.sSelectedObjectID = cItemID;
                        m_stMCursor.sDistX = msX + iAdjX - x1 + (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotX;
                        m_stMCursor.sDistY = msY + iAdjY - y1 + (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotY;
                        return true;
                    }
                }
            break;
    }

    return false;
}
/*********************************************************************************************************************
**  void CGame::DlgBoxClick_Exchange()											**
**  description			:: modifyed for MultiTrade																	**
**********************************************************************************************************************/
void CGame::DlgBoxClick_Exchange()
{
    char onButton = m_dialogBoxes[27].OnButton();
    switch (m_dialogBoxes[27].GetMode())
    {
        case 1: // Not yet confirmed the exchange
            if (onButton == 1) // Exchange
            {
                if ((m_stDialogBoxExchangeInfo[0].sV1 != -1) && (m_stDialogBoxExchangeInfo[4].sV1 != -1))
                {	/*bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_CONFIRMEXCHANGEITEM, NULL
                    , m_stDialogBoxExchangeInfo[0].sV1 // ItemID; inutilisé par serveur
                    , m_stDialogBoxExchangeInfo[0].sV3 // Amount; inutilisé par serveur
                    , NULL, NULL);	*/
                    PlaySound('E', 14, 5);
                    m_dialogBoxes[27].SetMode(2);
                    // Show confirmation Diag instead.
                    EnableDialogBox(41, 0, 0, 0);
                    m_dialogBoxes[41].SetMode(1);
                }
                return;
            }
            else if (onButton == 2 && (m_bIsDialogEnabled[41] == false)) // Cancel only possible if confirmation is not activated
            {
                DisableDialogBox(27);
                DisableDialogBox(22);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_CANCELEXCHANGEITEM, 0, 0, 0, 0, 0);
                PlaySound('E', 14, 5);
                return;
            }
            break;

        case 2: // Someone already confirmed the exchange
                /*	if (onButton == 1)  // Cancel
                {
                DisableDialogBox(27);
                DisableDialogBox(22);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_CANCELEXCHANGEITEM, NULL, NULL, NULL, NULL, NULL);
                PlaySound('E', 14, 5);
                return;
                }*/
            break;
    }
}
/*********************************************************************************************************************
**  void CGame::DlgBoxClick_ConfirmExchange()									**
**  description			:: click on confirmation diag																**
**********************************************************************************************************************/
void CGame::DlgBoxClick_ConfirmExchange()
{
    char onButton = m_dialogBoxes[41].OnButton();
    switch (m_dialogBoxes[41].GetMode())
    {
        case 1: // Not yet confirmed the exchange
                // yes
            if (onButton == 1)
            {
                if ((m_stDialogBoxExchangeInfo[0].sV1 != -1) && (m_stDialogBoxExchangeInfo[4].sV1 != -1))
                {
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_CONFIRMEXCHANGEITEM, 0
                                 , m_stDialogBoxExchangeInfo[0].sV1 // ItemID; inutilisé par serveur
                                 , m_stDialogBoxExchangeInfo[0].sV3 // Amount; inutilisé par serveur
                                 , 0, 0);
                    PlaySound('E', 14, 5);
                    m_dialogBoxes[27].SetMode(2);
                    m_dialogBoxes[41].SetMode(2);
                }
                return;
            }
            else if (onButton == 2)// No
            {
                DisableDialogBox(41);
                DisableDialogBox(27);
                DisableDialogBox(22);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_CANCELEXCHANGEITEM, 0, 0, 0, 0, 0);
                PlaySound('E', 14, 5);
                return;
            }
            break;
        case 2: // waiting for other side to confirm
            break;
    }
}

void CGame::DlgBoxClick_Quest()
{
    if (m_dialogBoxes[28].OnButton() == 1)
    {
        DisableDialogBox(28);
        PlaySound('E', 14, 5);
    }
}

void CGame::DlgBoxClick_Help()
{
    char onButton = m_dialogBoxes[35].OnButton();

    if (onButton > 0 && onButton < 16)
        DisableDialogBox(18);

    switch (onButton)
    {
        case 1:
            EnableDialogBox(18, 1000, 0, 0);
            break;
        case 2:
            EnableDialogBox(18, 900, 0, 0);
            break;
        case 3:
            EnableDialogBox(18, 901, 0, 0);
            break;
        case 4:
            EnableDialogBox(18, 902, 0, 0);
            break;
        case 5:
            EnableDialogBox(18, 903, 0, 0);
            m_bIsF1HelpWindowEnabled = true;
            break;
        case 6:
            EnableDialogBox(18, 904, 0, 0); //
            break;
        case 7:
            EnableDialogBox(18, 905, 0, 0); //
            break;
        case 8:
            EnableDialogBox(18, 906, 0, 0); //
            break;
        case 9:
            EnableDialogBox(18, 907, 0, 0); //
            break;
        case 10:
            EnableDialogBox(18, 908, 0, 0); //
            break;
        case 11:
            EnableDialogBox(18, 909, 0, 0); //
            break;
        case 12:
            EnableDialogBox(18, 910, 0, 0); //
            break;
        case 13:
            EnableDialogBox(18, 911, 0, 0); // FAQ
            break;
        case 14:
            EnableDialogBox(18, 912, 0, 0); //
            break;
        case 15:
            PlaySound('E', 14, 5);
            DisableDialogBox(35);
    }
}

void CGame::DlgBoxClick_Mailbox()
{
    CDialogBox & dlg = m_dialogBoxes[DIALOG_MAILBOX];
    char onButton = dlg.OnButton();

    switch (dlg.GetMode())
    {
        case 0:
            if (onButton == 1)
            {
                // send mail
                if (m_dialogBoxes[DIALOG_MAILBOX].bFlag)
                {
                    AddEventList(NOTIFY_MSG_HANDLER88, CHAT_GM);
                    break;
                }

                if (GetItemCount("Gold") < SEND_MAIL_COST)
                {
                    AddEventList(NOTIFY_MSG_HANDLER83, CHAT_GM);
                    break;
                }

                ZeroMemory(dlg.cStr, sizeof(dlg.cStr)); // to
                ZeroMemory(dlg.cStr2, sizeof(dlg.cStr2)); // title
                ZeroMemory(dlg.txt, sizeof(dlg.txt)); // msg
                dlg.sV1 = -1;
                dlg.bFlag = false;
                dlg.vvec.clear(); // items
                dlg.SetMode(1);

                G_pGame->m_stMCursor.sX = dlg.GetButton(1).left + 1;
                G_pGame->m_stMCursor.sY = dlg.GetButton(1).top + 1;
                DlgBoxClick_Mailbox();
            }
            else if (onButton >= 3 && onButton < 3 + 16)
            {
                uint32_t listHit = onButton - 3 + dlg.sView;

                if (listHit >= m_mails.size())
                    break;

                if (!m_mails[m_mails.size() - 1 - listHit]->msg)
                    bSendCommand(MSGID_REQ_MAILDATA, 0, 0, m_mails[m_mails.size() - 1 - listHit]->id);

                dlg.sV1 = m_mails.size() - 1 - listHit;
                dlg.SetMode(2);
            }
            break;

        case 1:
            EndInputString();
            if (dlg.bFlag)
                break;

            switch (onButton)
            {
                case 1: // to:
                    StartInputString(dlg.GetButton(1).left + 20, dlg.GetButton(1).top - 1, 11, dlg.cStr);
                    break;

                case 2: // title:
                    StartInputString(dlg.GetButton(2).left + 30, dlg.GetButton(2).top - 1, 31, dlg.cStr2);
                    break;

                case 3: // msg:
                    StartInputString(dlg.GetButton(3).left, dlg.GetButton(3).top + 15, sizeof(dlg.txt), dlg.txt, false, dlg.GetButton(3).right);
                    break;

                case 4: // attach items:
                    dlg.SetMode(3);
                    break;

                case 5: // send
                    if (GetItemCount("Gold") < SEND_MAIL_COST + dlg.vvec.size() * SEND_MAIL_ITEM_COST)
                    {
                        AddEventList(NOTIFY_MSG_HANDLER87, CHAT_GM);
                        break;
                    }

                    bSendCommand(MSGID_REQ_SENDMAIL);
                    dlg.bFlag = true;
                    break;

                case 6: // cancel
                    for (std::vector<MailItemSend>::iterator it = dlg.vvec.begin();
                         it != dlg.vvec.end();
                         ++it)
                    {
                        m_bIsItemDisabled[(*it).index] = false;
                    }
                    dlg.vvec.clear();
                    dlg.SetMode(0);
                    break;
            }
            break;

        case 2:
            switch (onButton)
            {
                case 4: // attached items
                    if (m_mails[dlg.sV1]->items.size() == 0)
                        break;

                    dlg.SetMode(4);
                    break;

                case 5: // delete
                    EnableDialogBox(DIALOG_YESNO, 1);
                    break;

                case 6: // back
                    dlg.sV1 = -1;
                    dlg.SetMode(0);
                    break;
            }
            break;

        case 3:
            if (onButton >= 1 && onButton <= 10 && onButton <= dlg.vvec.size())
            {
                // item detach
                m_bIsItemDisabled[dlg.vvec[onButton - 1].index] = false;
                dlg.vvec.erase(dlg.vvec.begin() + onButton - 1);
            }
            else if (onButton == 11)
            {
                // back
                dlg.SetMode(1);
            }
            else if (onButton == 12)
            {
                // cancel
                for (std::vector<MailItemSend>::iterator it = dlg.vvec.begin();
                     it != dlg.vvec.end();
                     ++it)
                {
                    m_bIsItemDisabled[(*it).index] = false;
                }
                dlg.vvec.clear();
                dlg.SetMode(1);
            }
            break;

        case 4:
            if (onButton == 15)
            {
                // back
                dlg.SetMode(2);
            }
            else if (onButton >= 1 && onButton <= m_mails[dlg.sV1]->items.size())
            {
                if (_iGetTotalItemNum() >= 75)
                { // Bag Fix xRisenx
                    AddEventList(DLGBOX_CLICK_BANK1, 10);
                    return;
                }
                dlg.sV2 = onButton - 1;
                bSendCommand(MSGID_REQ_RETRIEVEMAILITEM);

                dlg.SetMode(5);
            }
            break;
    }
}

void CGame::DlgBoxDoubleClick_Character()
{
    char cEquipPoiStatus[MAXITEMEQUIPPOS], cItemID = -1;
    short sX, sY, sSprH, sFrame;
    int i;
    if (m_bIsDialogEnabled[17] == true) return;

    short msX = G_pGame->m_stMCursor.sX;
    short msY = G_pGame->m_stMCursor.sY;

    sX = m_dialogBoxes[1].m_X;
    sY = m_dialogBoxes[1].m_Y;

    for (i = 0; i < MAXITEMEQUIPPOS; i++)
        cEquipPoiStatus[i] = -1;

    for (i = 0; i < MAXITEMS; i++)
    {
        if ((m_pItemList[i] != 0) && (m_bIsItemEquipped[i] == true))	cEquipPoiStatus[m_pItemList[i]->m_cEquipPos] = i;
    }
    if ((m_sPlayerType >= 1) && (m_sPlayerType <= 3))
    {
        if (cEquipPoiStatus[EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 41, sY + 137, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_BACK];
        }
        if (cEquipPoiStatus[EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_PANTS];
        }
        if (cEquipPoiStatus[EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_ARMS];
        }
        if (cEquipPoiStatus[EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_BOOTS];
        }
        if (cEquipPoiStatus[EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_BODY];
        }
        if (cEquipPoiStatus[EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_FULLBODY];
        }
        if (cEquipPoiStatus[EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 90, sY + 170, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_LHAND];
        }
        if (cEquipPoiStatus[EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 57, sY + 186, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_RHAND];
        }
        if (cEquipPoiStatus[EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 57, sY + 186, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_TWOHAND];
        }
        if (cEquipPoiStatus[EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 35, sY + 120, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_NECK];
        }
        if (cEquipPoiStatus[EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 32, sY + 193, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_RFINGER];
        }
        if (cEquipPoiStatus[EQUIPPOS_LFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 100, sY + 185, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_LFINGER];
        }
        if (cEquipPoiStatus[EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 72, sY + 135, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_HEAD];
        }
    }
    else if ((m_sPlayerType >= 4) && (m_sPlayerType <= 6))
    {
        if (cEquipPoiStatus[EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 45, sY + 143, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_BACK];
        }
        if (cEquipPoiStatus[EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_BOOTS];
        }
        if (cEquipPoiStatus[EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_PANTS];
        }
        if (cEquipPoiStatus[EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_ARMS];
        }
        if (cEquipPoiStatus[EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_BOOTS];
        }
        if (cEquipPoiStatus[EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_BODY];
        }
        if (cEquipPoiStatus[EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_FULLBODY];
        }
        if (cEquipPoiStatus[EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 84, sY + 175, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_LHAND];
        }
        if (cEquipPoiStatus[EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 60, sY + 191, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_RHAND];
        }
        if (cEquipPoiStatus[EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 60, sY + 191, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_TWOHAND];
        }
        if (cEquipPoiStatus[EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 35, sY + 120, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_NECK];
        }
        if (cEquipPoiStatus[EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 32, sY + 193, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_RFINGER];
        }
        if (cEquipPoiStatus[EQUIPPOS_LFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 100, sY + 185, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_LFINGER];
        }
        if (cEquipPoiStatus[EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 72, sY + 139, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[EQUIPPOS_HEAD];
        }
    }

    if (cItemID == -1 || m_pItemList[cItemID] == 0) return;
    if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_EAT) || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_ARROW) || (m_pItemList[cItemID]->m_dwCount > 1)) return;
    if ((m_bIsDialogEnabled[11] == true) && (m_bIsDialogEnabled[23] == false) && (m_dialogBoxes[39].sV3 == 24))
        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_REPAIRITEM, 0, cItemID, m_dialogBoxes[39].sV3, 0, m_pItemList[cItemID]->m_cName, m_dialogBoxes[39].sV4);
    else
    {
        if (m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] == true)
        {
            char cStr1[64], cStr2[64], cStr3[64];
            GetItemName(m_pItemList[m_stMCursor.sSelectedObjectID], cStr1, cStr2, cStr3);
            ZeroMemory(G_cTxt, sizeof(G_cTxt));
            wsprintfA(G_cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);//"
            AddEventList(G_cTxt, 10);
            if (memcmp(m_pItemList[m_stMCursor.sSelectedObjectID]->m_cName, "AngelicPendant", 14) == 0) PlaySound('E', 53, 0);
            else PlaySound('E', 29, 0);

            // Remove Angelic Stats
            if ((m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos >= 11)
                && (m_pItemList[m_stMCursor.sSelectedObjectID]->m_cItemType == 1))
            {
                char cItemID = m_stMCursor.sSelectedObjectID;
                if (memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(STR)", 19) == 0)
                {
                    m_angelStat[STAT_STR] = 0;
                }
                else if (memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(DEX)", 19) == 0)
                {
                    m_angelStat[STAT_DEX] = 0;
                }
                else if (memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(INT)", 19) == 0)
                {
                    m_angelStat[STAT_INT] = 0;
                }
                else if (memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(MAG)", 19) == 0)
                {
                    m_angelStat[STAT_MAG] = 0;
                }
            }
            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_RELEASEITEM, 0, m_stMCursor.sSelectedObjectID, 0, 0, 0);
            m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] = false;
            m_sItemEquipmentStatus[m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos] = -1;
            m_stMCursor.cSelectedObjectType = 0;
            m_stMCursor.sSelectedObjectID = 0;
        }
    }
}

void CGame::DlgBoxDoubleClick_GuideMap()
{
    short si = m_stMCursor.sCursorFrame;
    if (si != 0) return;
    if (m_cMapIndex < 0) return;

    Point p = GetGuideMapPos(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);

    if (p.x < 20 || p.y < 20) return;
    if (p.x > m_pMapData->m_sMapSizeX - 20 || p.y > m_pMapData->m_sMapSizeY - 20) return;
    m_sCommX = p.x;
    m_sCommY = p.y;

    if (m_bCtrlPressed)
    {
        wsprintfA(G_cTxt, "/tp %s %u %u", m_cMapName, m_sCommX, m_sCommY);
        bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, G_cTxt);
        return;
    }

    if (m_bRunningMode && m_iSP > 0)
        m_cCommand = OBJECTRUN;
    else m_cCommand = OBJECTMOVE;
    GetPlayerTurn();
}

void CGame::DlgBoxDoubleClick_Inventory()
{
    int i;
    char  cItemID, cTxt[120];
    short sX, sY, x1, x2, y1, y2;
    char cStr1[64], cStr2[64], cStr3[64];

    short msX = G_pGame->m_stMCursor.sX;
    short msY = G_pGame->m_stMCursor.sY;
    //if (m_iHP <= 0) return;
    if (m_bItemUsingStatus == true)
    {
        AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY1, 10);
        return;
    }
    sX = m_dialogBoxes[2].m_X;
    sY = m_dialogBoxes[2].m_Y;
    for (i = 0; i < MAXITEMS; i++)
    {
        if (m_cItemOrder[MAXITEMS - 1 - i] == -1) continue;
        cItemID = m_cItemOrder[MAXITEMS - 1 - i];
        if (m_pItemList[cItemID] == 0) continue;

        m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + 32 + m_pItemList[cItemID]->m_sX, sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame);
        // Order
        x1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
        y1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
        x2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
        y2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;

        if ((m_bIsItemDisabled[cItemID] == false) && (m_bIsItemEquipped[cItemID] == false) && (msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
        {	// Order
            _SetItemOrder(0, cItemID);
            GetItemName(m_pItemList[cItemID], cStr1, cStr2, cStr3);

            if (m_bIsDialogEnabled[11] && (m_bIsDialogEnabled[23] == false) && (m_bIsDialogEnabled[23] == false) && (m_dialogBoxes[39].sV3 == 24))
            {
                if (m_pItemList[cItemID]->m_cEquipPos != EQUIPPOS_NONE)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_REPAIRITEM, 0, cItemID, m_dialogBoxes[39].sV3, 0, m_pItemList[cItemID]->m_cName, m_dialogBoxes[39].sV4);
                    return;
                }
            }

            if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_DEPLETE)
                || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_PERM)
                || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_ARROW)
                || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_EAT))
            {
                if (bCheckItemOperationEnabled(cItemID) == false) return;
                if ((unixtime() - m_dwDamagedTime) < 20 * 1000)
                {
                    if ((m_pItemList[cItemID]->m_sSprite == 6) && (m_pItemList[cItemID]->m_sSpriteFrame == 9))
                    {
                        wsprintfA(G_cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY3, cStr1);//"Item %s: Scrolls cannot be used until 10 seconds after taking damage."
                        AddEventList(G_cTxt, 10);
                        return;
                    }
                    if ((m_pItemList[cItemID]->m_sSprite == 6) && (m_pItemList[cItemID]->m_sSpriteFrame == 89))
                    {
                        wsprintfA(G_cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY3, cStr1);//"Item %s: Scrolls cannot be used until 10 seconds after taking damage."
                        AddEventList(G_cTxt, 10);
                        return;
                    }
                }
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_USEITEM, 0, cItemID, 0, 0, 0);

                if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_DEPLETE)
                    || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_EAT))
                {
                    m_bIsItemDisabled[cItemID] = true;
                    m_bItemUsingStatus = true;
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_SKILL)
            {
                if (_bIsItemOnHand() == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);//"Your hands should be free to use this item."
                    return;
                }
                if (m_bSkillUsingStatus == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY5, 10);//"You are already using another skill."
                    return;
                }
                if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY6, 10);//"You can't use this item because it is exhausted."
                }
                else
                {
                    m_bIsGetPointingMode = true;
                    m_iPointCommandType = cItemID;
                    wsprintfA(cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY7, cStr1);//"Item %s: Select a position which you want to use."
                    AddEventList(cTxt, 10);
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_DEPLETE_DEST)
            {
                if (_bIsItemOnHand() == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);//"Your hands should be free to use this item."
                    return;
                }
                if (m_bSkillUsingStatus == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY13, 10);//"You are already using another skill."
                    return;
                }
                if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY6, 10);//"You can't use this item because it is exhausted."
                }
                else
                {
                    m_bIsGetPointingMode = true;
                    m_iPointCommandType = cItemID;
                    wsprintfA(cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY8, cStr1);//"Item %s: Select an item which you want to use."
                    AddEventList(cTxt, 10);
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX)
            {
                if (_bIsItemOnHand() == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);//"Your hands should be free to use this item."
                    return;
                }

                if (m_bSkillUsingStatus == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY5, 10);//"You are already using another skill."
                    return;
                }

                if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY6, 10);//"You can't use this item because it is exhausted."
                }
                else
                {
                    switch (m_pItemList[cItemID]->m_sSpriteFrame)
                    {
                        case 55: // Alchemy pot
                                 //if (m_cSkillMastery[SKILL_ALCHEMY] == 0)
                                 //{	AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY9, 10);//"You should learn alchemy skill to use this item."
                                 //}else
                                 //{	
                            EnableDialogBox(26, 1, 0, 0, 0);
                            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY10, 10);//"Using alchemy skill..."
                                                                               //}
                            break;
                        case 113: // Smith's Anvil
                                  //if (m_cSkillMastery[SKILL_MANUFACTURING] == 0)
                                  //{	AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY11, 10);//"You should learn manufacturing skill to use this item.."
                                  //}/else
                                  //{	
                            EnableDialogBox(26, 3, 0, 0, 0);
                            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY12, 10);//"Using a manufacturing skill..."
                                                                               //}
                            break;

                            // Crafting
                        case 0:
                            //if (m_pItemList[i]->m_sSprite == 22)
                        {
                            EnableDialogBox(26, 7, 0, 0, 0);
                            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY17, 10);	//  "Initiating item Crafting..."
                        }
                        break;

                        case 151:
                        case 152:
                        case 153:
                        case 154:
                            EnableDialogBox(40, 1, 0, 0, 0);
                            break;
                    }
                }
            }

            if (m_bIsDialogEnabled[26] == true)
            {
                if (m_dialogBoxes[26].GetMode() == 1 ||
                    m_dialogBoxes[26].GetMode() == 4 ||
                    m_dialogBoxes[26].GetMode() == 7)
                {
                    bItemDrop_SkillDialog();
                    bItemDrop_ExternalScreen(cItemID, m_dialogBoxes[26].m_X + 50, m_dialogBoxes[26].m_Y + 50);
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_EQUIP)
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = (short)cItemID;
                bItemDrop_Character();
                m_stMCursor.cSelectedObjectType = 0;
                m_stMCursor.sSelectedObjectID = 0;
            }
            return;
        }
    }
}

void CGame::DlgBoxClick_SysMenu()
{
    char onButton = m_dialogBoxes[19].OnButton();
    switch (onButton)
    {
        case 1:
            m_cDetailLevel = 0;
            AddEventList(NOTIFY_MSG_DETAIL_LEVEL_LOW, 10);
            PlaySound('E', 14, 5);
            break;
        case 2:
            m_cDetailLevel = 1;
            AddEventList(NOTIFY_MSG_DETAIL_LEVEL_MEDIUM, 10);
            PlaySound('E', 14, 5);
            break;
        case 3:
            m_cDetailLevel = 2;
            AddEventList(NOTIFY_MSG_DETAIL_LEVEL_HIGH, 10);
            PlaySound('E', 14, 5);
            break;
        case 4:
            if (m_bSoundFlag)
            {
                if (m_bSoundStat == true)
                {
                    //DIRECTXm_pESound[38]->bStop();
                    m_bSoundStat = false;
                    AddEventList(NOTIFY_MSG_SOUND_OFF, 10);
                }
                else
                {
                    m_bSoundStat = true;
                    AddEventList(NOTIFY_MSG_SOUND_ON, 10);
                }
            }
            break;
        case 5:
            if (m_bSoundFlag)
            {
                if (m_bMusicStat == true) 	// Music Off
                {
                    m_bMusicStat = false;
                    AddEventList(NOTIFY_MSG_MUSIC_OFF, 10);
                    if (m_bSoundFlag)
                    {
                        // 					if (m_pBGM != NULL)
                        // 					{
                        // 						m_pBGM->bStop();
                        // 						delete m_pBGM;
                        // 						m_pBGM = NULL;
                        // 					}//DIRECTX
                    }
                }
                else // Music On
                {
                    if (m_bSoundFlag)
                    {
                        m_bMusicStat = true;
                        AddEventList(NOTIFY_MSG_MUSIC_ON, 10);
                        StartBGM();
                    }
                }
            }
            break;
        case 6:
            if (m_bWhisper == true)
            {
                m_bWhisper = false;
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND7, 10);
            }
            else
            {
                m_bWhisper = true;
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND6, 10);
            }
            break;
        case 7:
            if (m_bShout == true)
            {
                m_bShout = false;
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND9, 10);
            }
            else
            {
                m_bShout = true;
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND8, 10);
            }
            break;
        case 8://Transparency Change
            m_bDialogTrans = !m_bDialogTrans;
            break;
        case 9:
            if (m_bIsDialogEnabled[9]) DisableDialogBox(9);//Guide Map Toggle
            else EnableDialogBox(9, 0, 0, 0, 0);
            break;
        case 14:
            if (m_bIsDialogEnabled[59]) DisableDialogBox(59);
            else EnableDialogBox(59, 0, 0, 0, 0);
            break;
    }

    if (m_bForceDisconn) return;

    switch (onButton)
    {
        case 10:
            if (!m_bSkillUsingStatus)
            {
                if (m_cLogOutCount == -1)

#ifdef _DEBUG
                    m_cLogOutCount = 1;
#else
                    m_cLogOutCount = 11;
#endif
                else
                {
                    m_cLogOutCount = -1;
                    AddEventList(DLGBOX_CLICK_SYSMENU2, 10);
                    DisableDialogBox(19);
                }
                PlaySound('E', 14, 5);
            }
            else
            {
                AddEventList(SKILL_LOGOUT, 10);
                DisableDialogBox(19);
            }
            break;
        case 11:
            if ((m_iHP <= 0) && (m_cRestartCount == -1))
            {
                m_cRestartCount = 5;
                m_dwRestartCountTime = unixtime();
                DisableDialogBox(19);
                wsprintfA(G_cTxt, DLGBOX_CLICK_SYSMENU1, m_cRestartCount); // "Restarting game....%d"
                AddEventList(G_cTxt, 10);
                PlaySound('E', 14, 5);
            }
            break;
    }
}

void CGame::DrawDialogBox_Bank()
{
    short sX, sY, szX;
    int  i, iTotalLines, iPointerLoc;
    double d1, d2, d3;
    uint32_t ItemColor;
    char cStr1[64], cStr2[64], cStr3[64];
    bool bFlag = false;

    char onButton = m_dialogBoxes[14].OnButton();
    sX = m_dialogBoxes[14].m_X;
    sY = m_dialogBoxes[14].m_Y;
    szX = m_dialogBoxes[14].sSizeX - 5;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 21);
    ZeroMemory(cStr1, sizeof(cStr1));
    ZeroMemory(cStr2, sizeof(cStr2));
    ZeroMemory(cStr3, sizeof(cStr3));

    switch (m_dialogBoxes[14].GetMode())
    {
        case -1:
            PutString(sX + 30 + 15, sY + 70, DRAW_DIALOGBOX_BANK1, video::SColor(255, 0, 0, 0));//"
            PutString(sX + 30 + 15, sY + 85, DRAW_DIALOGBOX_BANK2, video::SColor(255, 0, 0, 0));//"
            break;

        case 0:
            for (i = 0; i < m_dialogBoxes[14].sV1; i++)
            {
                if (!m_pBankList[i + m_dialogBoxes[14].sView] || i + m_dialogBoxes[14].sView >= MAXBANKITEMS)
                    continue;
                if (isItemLoaded == false)
                {
                    _LoadItemContents();
                    isItemLoaded = true;
                }
                std::vector<string> * lines = GetItemName(m_pBankList[i + m_dialogBoxes[14].sView], true);
                std::vector<string>::iterator iter = lines->begin(), end = lines->end();

                if (onButton == i + 1)
                {
                    bFlag = true;

                    if (iter != end)
                    {
                        PutAlignedString(sX, sX + szX, sY + 110 + i * 15, iter->c_str(), 255, 255, 255);
                    }

                    uint32_t maxWidth = 0;

                    while (iter != end)
                    {
                        if (iter->size() > maxWidth)
                            maxWidth = iter->size();

                        ++iter;
                    }

                    //DIRECTX m_DDraw.DrawShadowBox(sX + 70 - 3, sY + 25 - 1, sX + 70 + maxWidth * 5.5, sY + 25 + 13*lines->size());

                    iter = lines->begin();

                    int iLoc = 0;
                    if (iter != end)
                    {
                        PutString(sX + 70, sY + 25, iter->c_str(), m_itemColor, false, 1);
                        iLoc += 13;
                        ++iter;
                    }

                    while (iter != end)
                    {
                        PutString(sX + 70, sY + 25 + iLoc, iter->c_str(), video::SColor(255, 150, 150, 150), false, 1);
                        iLoc += 13;
                        ++iter;
                    }

                    ItemColor = m_pBankList[i + m_dialogBoxes[14].sView]->m_ItemColor;
                    if (ItemColor == 0)
                    {
                        m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pBankList[i + m_dialogBoxes[14].sView]->m_sSprite]->PutSpriteFast(sX + 40, sY + 68, m_pBankList[i + m_dialogBoxes[14].sView]->m_sSpriteFrame, m_dwCurTime);
                    }
                    else
                    {
                        if (m_pBankList[i + m_dialogBoxes[14].sView]->m_cEquipPos == EQUIPPOS_LHAND
                            || m_pBankList[i + m_dialogBoxes[14].sView]->m_cEquipPos == EQUIPPOS_RHAND
                            || m_pBankList[i + m_dialogBoxes[14].sView]->m_cEquipPos == EQUIPPOS_TWOHAND)
                        {
                            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pBankList[i + m_dialogBoxes[14].sView]->m_sSprite]->PutSpriteRGB(sX + 40, sY + 68, m_pBankList[i + m_dialogBoxes[14].sView]->m_sSpriteFrame,
                                                                                                                                     ItemColor, m_dwCurTime);
                        }
                        else
                            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pBankList[i + m_dialogBoxes[14].sView]->m_sSprite]->PutSpriteRGB(sX + 40, sY + 68, m_pBankList[i + m_dialogBoxes[14].sView]->m_sSpriteFrame,
                                                                                                                                     ItemColor, m_dwCurTime);
                    }
                }
                else if (iter != end)
                {
                    if (m_itemColor != video::SColor(255, 255, 255, 255))
                        PutAlignedString(sX, sX + szX, sY + 110 + i * 15, iter->c_str(), m_itemColor);
                    else
                        PutAlignedString(sX, sX + szX, sY + 110 + i * 15, iter->c_str(), 0, 0, 0);
                }
            }

            iTotalLines = 0;
            for (i = 0; i < MAXBANKITEMS; i++)
                if (m_pBankList[i] != 0) iTotalLines++;

            Point pointerLoc = m_dialogBoxes[14].HandleScroll(13, iTotalLines);
            if (iTotalLines > m_dialogBoxes[14].sV1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
            }

            if (bFlag == false)
            {
                PutAlignedString(sX, sX + szX, sY + 35, DRAW_DIALOGBOX_BANK3);
                PutAlignedString(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_BANK4);
                PutAlignedString(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_BANK5);
                wsprintfA(G_cTxt, DRAW_DIALOGBOX_BANK6, iTotalLines, MAXBANKITEMS);
                PutAlignedString(sX, sX + szX, sY + 85, G_cTxt, 255, 255, 255);
            }
            break;
    }
}

void CGame::DrawDialogBox_FeedBackCard()
{
}

void CGame::DrawDialogBox_Character()
{
    short sX, sY, sSprH, sFrame;
    int i, iR, iG, iB, iSkirtDraw = 0;
    char cTxt2[120], cEquipPoiStatus[MAXITEMEQUIPPOS];
    uint32_t ItemColor;
    char cCollison;

    char onButton = m_dialogBoxes[1].OnButton();
    sX = m_dialogBoxes[1].m_X;
    sY = m_dialogBoxes[1].m_Y;
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 0, FALSE, m_bDialogTrans);

    ZeroMemory(G_cTxt, sizeof(G_cTxt));
    strcpy(G_cTxt, m_cPlayerName);


    if (m_iPKCount > 0)
    {
        strcat(G_cTxt, " : ");
        ZeroMemory(cTxt2, sizeof(cTxt2));
        wsprintfA(cTxt2, DRAW_DIALOGBOX_CHARACTER1, m_iPKCount);
        strcat(G_cTxt, cTxt2);
    }

    /*wsprintfA(cTxt2, DRAW_DIALOGBOX_CHARACTER2, m_iContribution);
    strcat(G_cTxt, cTxt2);
    wsprintfA(cTxt2, DRAW_DIALOGBOX_CHARACTER10, m_iGizonItemUpgradeLeft);
    strcat(G_cTxt, cTxt2);*/
    PutAlignedString(sX + 24, sX + 252, sY + 50, G_cTxt, 45, 20, 20);

    ZeroMemory(G_cTxt, sizeof(G_cTxt));

    sstream txt;
    txt << sideName[m_side];

    if (m_side != NEUTRAL)
    {
        txt << MSG_COMBATANT;

        if (m_iGuildRank != GUILDRANK_NONE)
        {
            txt << '(' << m_cGuildName << ' ' << GetGuildRankName(m_iGuildRank) << ')';
        }
    }

    PutAlignedString(sX, sX + 275, sY + 64, txt.str().c_str(), 45, 25, 25);

    // Level
    wsprintfA(G_cTxt, "%d", m_iLevel);
    PutAlignedString(sX + 180, sX + 250, sY + 106, G_cTxt, 45, 25, 25);
    // Exp
    wsprintfA(G_cTxt, "%d", m_iExp);
    PutAlignedString(sX + 180, sX + 250, sY + 125, G_cTxt, 45, 25, 25);
    // Next.Exp
    wsprintfA(G_cTxt, "%d", m_levelExpTable[m_iLevel + 1]);
    PutAlignedString(sX + 180, sX + 250, sY + 142, G_cTxt, 45, 25, 25);

    // Hp
    wsprintfA(G_cTxt, "%d/%d", m_iHP, m_stat[STAT_VIT] * 8 + m_stat[STAT_STR] * 2 + m_stat[STAT_INT] * 2 + m_iLevel * 8); // HP System xRisenx
    PutAlignedString(sX + 180, sX + 250, sY + 173, G_cTxt, 45, 25, 25);

    // Mp
    wsprintfA(G_cTxt, "%d/%d", m_iMP, m_stat[STAT_MAG] * 3 + m_iLevel * 2 + m_stat[STAT_INT] * 2); // MP System xRisenx
    PutAlignedString(sX + 180, sX + 250, sY + 191, G_cTxt, 45, 25, 25);

    // Sp
    wsprintfA(G_cTxt, "%d/%d", m_iSP, m_iLevel * 17 + m_stat[STAT_STR]); // SP System xRisenx
    PutAlignedString(sX + 180, sX + 250, sY + 208, G_cTxt, 45, 25, 25);

    // Max.Load
    wsprintfA(G_cTxt, "%d/%d", m_weight, (m_stat[STAT_STR] * 5 + m_iLevel * 5));
    PutAlignedString(sX + 180, sX + 250, sY + 240, G_cTxt, 45, 25, 25);

    // Enemy Kills
    wsprintfA(G_cTxt, "%d/%d", m_iEnemyKillCount, m_iEnemyKillTotalCount);
    PutAlignedString(sX + 180, sX + 250, sY + 257, G_cTxt, 45, 25, 25);

    // Str
    wsprintfA(G_cTxt, "%d", m_stat[STAT_STR]);
    if (m_angelStat[STAT_STR] == 0)
        PutAlignedString(sX + 48, sX + 82, sY + 285, G_cTxt, 45, 25, 25);
    else
        PutAlignedString(sX + 48, sX + 82, sY + 285, G_cTxt, 0, 0, 192);


    wsprintfA(G_cTxt, "%d", m_stat[STAT_VIT]);
    PutAlignedString(sX + 218, sX + 251, sY + 285, G_cTxt, 45, 25, 25);

    // Dex
    wsprintfA(G_cTxt, "%d", m_stat[STAT_DEX]);
    if (m_angelStat[STAT_DEX] == 0)
    {
        PutAlignedString(sX + 48, sX + 82, sY + 302, G_cTxt, 45, 25, 25);
    }
    else
    {
        PutAlignedString(sX + 48, sX + 82, sY + 302, G_cTxt, 0, 0, 192);
    }

    // Int
    wsprintfA(G_cTxt, "%d", m_stat[STAT_INT]);
    if (m_angelStat[STAT_INT] == 0)
    {
        PutAlignedString(sX + 135, sX + 167, sY + 285, G_cTxt, 45, 25, 25);
    }
    else
    {
        PutAlignedString(sX + 135, sX + 167, sY + 285, G_cTxt, 0, 0, 192);
    }

    // Mag
    wsprintfA(G_cTxt, "%d", m_stat[STAT_MAG]);
    if (m_angelStat[STAT_MAG] == 0)
    {
        PutAlignedString(sX + 135, sX + 167, sY + 302, G_cTxt, 45, 25, 25);
    }
    else
    {
        PutAlignedString(sX + 135, sX + 167, sY + 302, G_cTxt, 0, 0, 192);
    }

    // Chr
    wsprintfA(G_cTxt, "%d", m_stat[STAT_CHR]);
    PutAlignedString(sX + 218, sX + 251, sY + 302, G_cTxt, 45, 25, 25);

    for (i = 0; i < MAXITEMEQUIPPOS; i++)
        cEquipPoiStatus[i] = -1;

    for (i = 0; i < MAXITEMS; i++)
    {
        if ((m_pItemList[i] != 0) && (m_bIsItemEquipped[i] == true))	cEquipPoiStatus[m_pItemList[i]->m_cEquipPos] = i;
    }
    if ((m_sPlayerType >= 1) && (m_sPlayerType <= 3))
    {
        cCollison = -1;
        m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 0]->PutSpriteFast(sX + 171, sY + 290, m_sPlayerType - 1, m_dwCurTime);
        if (cEquipPoiStatus[EQUIPPOS_HEAD] == -1)
        {
            _GetHairColorRGB(((m_sPlayerAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 18]->PutSpriteRGB(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x0F00) >> 8, iR, iG, iB, m_dwCurTime);
        }

        m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 19]->PutSpriteFast(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x000F), m_dwCurTime);

        if (cEquipPoiStatus[EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_BACK]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 41, sY + 137, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 41, sY + 137, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 41, sY + 137, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_BACK;
        }

        if (cEquipPoiStatus[EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_PANTS]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_PANTS;
        }

        if (cEquipPoiStatus[EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_ARMS]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_ARMS;
        }

        if (cEquipPoiStatus[EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_BOOTS]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_BOOTS;
        }

        if (cEquipPoiStatus[EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_BODY]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_BODY;
        }

        if (cEquipPoiStatus[EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_FULLBODY]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_FULLBODY;
        }

        if (cEquipPoiStatus[EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_LHAND]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 90, sY + 170, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 90, sY + 170, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 90, sY + 170, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_LHAND;
        }

        if (cEquipPoiStatus[EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_RHAND]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 57, sY + 186, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 57, sY + 186, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 57, sY + 186, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_RHAND;
        }

        if (cEquipPoiStatus[EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_TWOHAND]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 57, sY + 186, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 57, sY + 186, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 57, sY + 186, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_TWOHAND;
        }

        if (cEquipPoiStatus[EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_NECK]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 35, sY + 120, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 35, sY + 120, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 35, sY + 120, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_NECK;
        }

        if (cEquipPoiStatus[EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_RFINGER]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 32, sY + 193, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 32, sY + 193, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 32, sY + 193, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_RFINGER;
        }

        if (cEquipPoiStatus[EQUIPPOS_LFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_LFINGER]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 100, sY + 185, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 100, sY + 185, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 100, sY + 185, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_LFINGER;
        }

        if (cEquipPoiStatus[EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_HEAD]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutSpriteRGB(sX + 72, sY + 135, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSpriteRGB(sX + 72, sY + 135, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 72, sY + 135, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_HEAD;
        }
        if (cCollison != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[cCollison]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[cCollison]]->m_sSpriteFrame;
            if (cCollison == EQUIPPOS_HEAD)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 72, sY + 135, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_RFINGER)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 32, sY + 193, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_LFINGER)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 100, sY + 185, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_NECK)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 35, sY + 120, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_TWOHAND)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 57, sY + 186, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_RHAND)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 57, sY + 186, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_LHAND)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 90, sY + 170, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_BODY)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_FULLBODY)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_BOOTS)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_ARMS)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_PANTS)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_BACK)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->PutTransSprite(sX + 41, sY + 137, sFrame, m_dwCurTime);
        }
    }
    else if ((m_sPlayerType >= 4) && (m_sPlayerType <= 6))
    {
        cCollison = -1;
        m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 40]->PutSpriteFast(sX + 171, sY + 290, m_sPlayerType - 4, m_dwCurTime);

        if (cEquipPoiStatus[EQUIPPOS_HEAD] == -1)
        {
            _GetHairColorRGB(((m_sPlayerAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 18 + 40]->PutSpriteRGB(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x0F00) >> 8, iR, iG, iB, m_dwCurTime);
        }

        m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 19 + 40]->PutSpriteFast(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x000F), m_dwCurTime);

        if (cEquipPoiStatus[EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if ((sSprH == 12) && (sFrame == 0)) iSkirtDraw = 1;
        }
        if (cEquipPoiStatus[EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_BACK]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_BACK]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 45, sY + 143, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 45, sY + 143, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 45, sY + 143, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_BACK;
        }

        if ((cEquipPoiStatus[EQUIPPOS_BOOTS] != -1) && (iSkirtDraw == 1))
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_BOOTS]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_BOOTS;
        }

        if (cEquipPoiStatus[EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_PANTS]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_PANTS]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_PANTS;
        }

        if (cEquipPoiStatus[EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_ARMS]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_ARMS]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_ARMS;
        }

        if ((cEquipPoiStatus[EQUIPPOS_BOOTS] != -1) && (iSkirtDraw == 0))
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_BOOTS]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_BOOTS]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_BOOTS;
        }

        if (cEquipPoiStatus[EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_BODY]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_BODY]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_BODY;
        }
        if (cEquipPoiStatus[EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_FULLBODY]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_FULLBODY]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 171, sY + 290, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_FULLBODY;
        }

        if (cEquipPoiStatus[EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_LHAND]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_LHAND]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 84, sY + 175, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 84, sY + 175, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 84, sY + 175, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_LHAND;
        }

        if (cEquipPoiStatus[EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_RHAND]]->m_ItemColor;
            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_RHAND]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 60, sY + 191, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 60, sY + 191, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 60, sY + 191, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_RHAND;
        }

        if (cEquipPoiStatus[EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_TWOHAND]]->m_ItemColor;
            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_TWOHAND]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 60, sY + 191, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 60, sY + 191, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 60, sY + 191, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_TWOHAND;
        }

        if (cEquipPoiStatus[EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_NECK]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_NECK]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 35, sY + 120, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 35, sY + 120, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 35, sY + 120, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_NECK;
        }

        if (cEquipPoiStatus[EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_RFINGER]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_RFINGER]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 32, sY + 193, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 32, sY + 193, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 32, sY + 193, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_RFINGER;
        }

        if (cEquipPoiStatus[EQUIPPOS_LFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_LFINGER]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_LFINGER]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 100, sY + 185, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 100, sY + 185, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 100, sY + 185, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_LFINGER;
        }

        if (cEquipPoiStatus[EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_sSpriteFrame;
            ItemColor = m_pItemList[cEquipPoiStatus[EQUIPPOS_HEAD]]->m_ItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[EQUIPPOS_HEAD]] == false)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutSpriteRGB(sX + 72, sY + 139, sFrame, ItemColor, m_dwCurTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSpriteRGB(sX + 72, sY + 139, sFrame, ItemColor, m_dwCurTime);
            }
            if (m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 72, sY + 139, sFrame, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY))
                cCollison = EQUIPPOS_HEAD;
        }
        if (cCollison != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[cCollison]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[cCollison]]->m_sSpriteFrame;
            if (cCollison == EQUIPPOS_HEAD)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 72, sY + 139, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_RFINGER)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 32, sY + 193, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_LFINGER)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 100, sY + 185, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_NECK)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 35, sY + 120, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_TWOHAND)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 60, sY + 191, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_RHAND)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 60, sY + 191, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_LHAND)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 84, sY + 175, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_BODY)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_FULLBODY)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_BOOTS)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_ARMS)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_PANTS)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == EQUIPPOS_BACK)
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->PutTransSprite(sX + 45, sY + 143, sFrame, m_dwCurTime);
        }
    }


    if (onButton == 1)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 15, sY + 326, 5, false, m_bDialogTrans);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 15, sY + 326, 4, false, m_bDialogTrans);

    if (onButton == 2)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 98, sY + 326, 45, false, m_bDialogTrans);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 98, sY + 326, 44, false, m_bDialogTrans);

    if (onButton == 3)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 180, sY + 326, 11, false, m_bDialogTrans);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 180, sY + 326, 10, false, m_bDialogTrans);

    if (onButton == 4)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 15, sY + 348, 23, false, m_bDialogTrans);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 15, sY + 348, 22, false, m_bDialogTrans);
#ifdef TitleClient
    if (onButton == 5)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 180, sY + 348, 53, false, m_bDialogTrans);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 180, sY + 348, 52, false, m_bDialogTrans);
#endif
    /*	if (onButton == 4)
    DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 15, sY +348, 71, FALSE, m_bDialogTrans);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 15, sY +348, 70, FALSE, m_bDialogTrans);*/	// Not even used? wtf? lol
}

// 3.51 Level Up Dialog - Diuuude
void CGame::DrawDialogBox_LevelUpSetting()
{
    short sX, sY, szX;
    uint32_t dwTime = m_dwCurTime;
    char cTxt[120];
    int iStats;

#ifdef _DEBUG
    wsprintfA(G_cTxt, "m_stat[STAT_STR]: %d   m_angelStat[STAT_STR]: %d", m_stat[STAT_STR], m_angelStat[STAT_STR]);
    PutString(10, 10, G_cTxt, video::SColor(255, 255, 255, 255));
#endif

    char onButton = m_dialogBoxes[12].OnButton();
    sX = m_dialogBoxes[12].m_X;
    sY = m_dialogBoxes[12].m_Y;
    szX = m_dialogBoxes[12].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 2);
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, sX + 16, sY + 100, 4);

    PutAlignedString(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_LEVELUP_SETTING1);
    PutAlignedString(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_LEVELUP_SETTING2);

    // Points Left - Display in green if > 0
    PutString(sX + 20, sY + 85, DRAW_DIALOGBOX_LEVELUP_SETTING3, video::SColor(255, 0, 0, 0));
    wsprintfA(cTxt, "%d", m_iLU_Point);
    if (m_iLU_Point > 0)
    {
        PutString(sX + 73, sY + 102, cTxt, video::SColor(255, 0, 255, 0));
    }
    else
    {
        PutString(sX + 73, sY + 102, cTxt, video::SColor(255, 0, 0, 0));
    }
    // Strength
    PutString(sX + 24, sY + 125, DRAW_DIALOGBOX_LEVELUP_SETTING4, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_STR] - m_angelStat[STAT_STR]);
    PutString(sX + 109, sY + 125, cTxt, video::SColor(255, 0, 0, 0));
    iStats = m_stat[STAT_STR] + m_luStat[STAT_STR] - m_angelStat[STAT_STR];
    wsprintfA(cTxt, "%d", iStats);
    if (iStats != m_stat[STAT_STR] - m_angelStat[STAT_STR])
    {
        PutString(sX + 162, sY + 125, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 125, cTxt, video::SColor(255, 0, 0, 0));
    }
    if (onButton == 3 && (m_stat[STAT_STR] < Stats_Limit))
        //if (onButton == 3 && (m_stat[STAT_STR] < 450))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 127, 5, dwTime);
    if (onButton == 4 && (m_luStat[STAT_STR] > 0))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 127, 6, dwTime);


    PutString(sX + 24, sY + 144, DRAW_DIALOGBOX_LEVELUP_SETTING5, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_VIT]);
    PutString(sX + 109, sY + 144, cTxt, video::SColor(255, 0, 0, 0));
    iStats = m_stat[STAT_VIT] + m_luStat[STAT_VIT];
    wsprintfA(cTxt, "%d", iStats);
    if (iStats != m_stat[STAT_VIT])
    {
        PutString(sX + 162, sY + 144, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 144, cTxt, video::SColor(255, 0, 0, 0));
    }
    if (onButton == 5 && (m_stat[STAT_VIT] < Stats_Limit))
        //if (onButton == 5 && (m_stat[STAT_VIT] < 450))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 146, 5, dwTime);
    if (onButton == 6 && (m_luStat[STAT_VIT] > 0))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 146, 6, dwTime);

    // Dexterity
    PutString(sX + 24, sY + 163, DRAW_DIALOGBOX_LEVELUP_SETTING6, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_DEX] - m_angelStat[STAT_DEX]);
    PutString(sX + 109, sY + 163, cTxt, video::SColor(255, 0, 0, 0));
    iStats = m_stat[STAT_DEX] + m_luStat[STAT_DEX] - m_angelStat[STAT_DEX];
    wsprintfA(cTxt, "%d", iStats);
    if (iStats != m_stat[STAT_DEX] - m_angelStat[STAT_DEX])
    {
        PutString(sX + 162, sY + 163, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 163, cTxt, video::SColor(255, 0, 0, 0));
    }
    if (onButton == 7 && (m_stat[STAT_DEX] < Stats_Limit))
        //if (onButton == 7 && (m_stat[STAT_DEX] < 450))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 165, 5, dwTime);
    if (onButton == 8 && (m_luStat[STAT_DEX] > 0))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 165, 6, dwTime);

    // Intelligence
    PutString(sX + 24, sY + 182, DRAW_DIALOGBOX_LEVELUP_SETTING7, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_INT] - m_angelStat[STAT_INT]);
    PutString(sX + 109, sY + 182, cTxt, video::SColor(255, 0, 0, 0));
    iStats = m_stat[STAT_INT] + m_luStat[STAT_INT] - m_angelStat[STAT_INT];
    wsprintfA(cTxt, "%d", iStats);
    if (iStats != m_stat[STAT_INT] - m_angelStat[STAT_INT])
    {
        PutString(sX + 162, sY + 182, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 182, cTxt, video::SColor(255, 0, 0, 0));
    }
    if (onButton == 9 && (m_stat[STAT_INT] < Stats_Limit))
        //if (onButton == 9 && (m_stat[STAT_INT] < 450))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 184, 5, dwTime);
    if (onButton == 10 && (m_luStat[STAT_INT] > 0))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 184, 6, dwTime);

    // Magic
    PutString(sX + 24, sY + 201, DRAW_DIALOGBOX_LEVELUP_SETTING8, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_MAG] - m_angelStat[STAT_MAG]);
    PutString(sX + 109, sY + 201, cTxt, video::SColor(255, 0, 0, 0));
    iStats = m_stat[STAT_MAG] + m_luStat[STAT_MAG] - m_angelStat[STAT_MAG];
    wsprintfA(cTxt, "%d", iStats);
    if (iStats != m_stat[STAT_MAG] - m_angelStat[STAT_MAG])
    {
        PutString(sX + 162, sY + 201, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 201, cTxt, video::SColor(255, 0, 0, 0));
    }
    if (onButton == 11 && (m_stat[STAT_MAG] < Stats_Limit))
        //if (onButton == 11 && (m_stat[STAT_MAG] < 450))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 203, 5, dwTime);
    if (onButton == 12 && (m_luStat[STAT_MAG] > 0))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 203, 6, dwTime);

    // Charisma
    PutString(sX + 24, sY + 220, DRAW_DIALOGBOX_LEVELUP_SETTING9, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_CHR]);
    PutString(sX + 109, sY + 220, cTxt, video::SColor(255, 0, 0, 0));
    iStats = m_stat[STAT_CHR] + m_luStat[STAT_CHR];
    wsprintfA(cTxt, "%d", iStats);
    if (iStats != m_stat[STAT_CHR])
    {
        PutString(sX + 162, sY + 220, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 220, cTxt, video::SColor(255, 0, 0, 0));
    }
    if (onButton == 13 && (m_stat[STAT_CHR] < Stats_Limit))
        //if (onButton == 13 && (m_stat[STAT_CHR] < 450))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 222, 5, dwTime);
    if (onButton == 14 && (m_luStat[STAT_CHR] > 0))
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 222, 6, dwTime);

    if (onButton == 1)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
    if ((m_luStat[STAT_STR] == 0) && (m_luStat[STAT_VIT] == 0) && (m_luStat[STAT_DEX] == 0) && (m_luStat[STAT_INT] == 0) && (m_luStat[STAT_MAG] == 0) && (m_luStat[STAT_CHR] == 0))
    {
        if (onButton == 2)
        {
            if (m_iLU_Point <= 0) DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 21);
        }
        else
        {
            if (m_iLU_Point <= 0) DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 20);
        }
    }
}

void CGame::DrawDialogBox_CityHallMenu()
{
    const enum modes
    {
        LEGIONPTSSERVICES = 9,
        TELEPORTLIST = 10
    };
    short sX, sY, szX;
    int  iTotalLines/*, iPointerLoc*/;
    double d1, d2, d3;
    char cTxt[120];
    int i;
    Point pointerLoc;

    char onButton = m_dialogBoxes[13].OnButton();

    sX = m_dialogBoxes[13].m_X;
    sY = m_dialogBoxes[13].m_Y;
    szX = m_dialogBoxes[13].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 18);//CityHall Menu Text

    switch (m_dialogBoxes[13].GetMode())
    {
        case 0:
            // citizenship req
            if (m_side == NEUTRAL)
            {
                if (onButton == 1)
                    PutAlignedString(sX, sX + szX, sY + 70, DRAW_DIALOGBOX_CITYHALL_MENU1, video::SColor(255, 255, 255, 255));
                else PutAlignedString(sX, sX + szX, sY + 70, DRAW_DIALOGBOX_CITYHALL_MENU1, video::SColor(255, 4, 0, 50));
            }
            else PutAlignedString(sX, sX + szX, sY + 70, DRAW_DIALOGBOX_CITYHALL_MENU1, video::SColor(255, 65, 65, 65));

            if (m_iRewardGold > 0)
            {
                if (onButton == 2)
                    PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU4, video::SColor(255, 255, 255, 255));
                else PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU4, video::SColor(255, 4, 0, 50));
            }
            else PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU4, video::SColor(255, 65, 65, 65));

            // 3.51 Cityhall Menu - Request Hero's Items - Diuuude
            if ((m_iEnemyKillCount >= 100) && (m_iContribution >= 10))
            {
                if (onButton == 3)
                    PutAlignedString(sX, sX + szX, sY + 120, DRAW_DIALOGBOX_CITYHALL_MENU8, video::SColor(255, 255, 255, 255));
                else PutAlignedString(sX, sX + szX, sY + 120, DRAW_DIALOGBOX_CITYHALL_MENU8, video::SColor(255, 4, 0, 50));
            }
            else PutAlignedString(sX, sX + szX, sY + 120, DRAW_DIALOGBOX_CITYHALL_MENU8, video::SColor(255, 65, 65, 65));

            // Cancel quest
            if (m_stQuest.sQuestType != 0 && !m_stQuest.bIsQuestCompleted)
            {
                if (onButton == 4)
                    PutAlignedString(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_CITYHALL_MENU10, video::SColor(255, 255, 255, 255));
                else PutAlignedString(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_CITYHALL_MENU10, video::SColor(255, 4, 0, 50));
            }
            else PutAlignedString(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_CITYHALL_MENU10, video::SColor(255, 65, 65, 65));

            // Legion point services
            if (onButton == 5)
                PutAlignedString(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU11, video::SColor(255, 255, 255, 255));
            else
                PutAlignedString(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU11, video::SColor(255, 4, 0, 50));

            // Teleport list
            if ((m_bIsCrusadeMode == false))
            {
                if (onButton == 6)
                    PutAlignedString(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU12, video::SColor(255, 255, 255, 255));
                else PutAlignedString(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU12, video::SColor(255, 4, 0, 50));
            }
            else    PutAlignedString(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU12, video::SColor(255, 65, 65, 65));

            // Quest list
            if (onButton == 7)
                PutAlignedString(sX, sX + szX, sY + 220, DRAW_DIALOGBOX_CITYHALL_MENU9, video::SColor(255, 255, 255, 255));
            else PutAlignedString(sX, sX + szX, sY + 220, DRAW_DIALOGBOX_CITYHALL_MENU9, video::SColor(255, 4, 0, 50));

            //Change crusade role
            if (m_bIsCrusadeMode && m_side != NEUTRAL)
            {
                if (onButton == 8)
                    PutAlignedString(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU13, video::SColor(255, 255, 255, 255));
                else PutAlignedString(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU13, video::SColor(255, 4, 0, 50));
            }
            else    PutAlignedString(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU13, video::SColor(255, 65, 65, 65));

            PutAlignedString(sX, sX + szX, sY + 285, DRAW_DIALOGBOX_CITYHALL_MENU17);//"Select an Item you want."

            break;

        case 1: // become citizen warning
            PutAlignedString(sX, sX + szX, sY + 80, DRAW_DIALOGBOX_CITYHALL_MENU18, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU19, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_CITYHALL_MENU20, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU21, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU22, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU23, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU24, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 200, DRAW_DIALOGBOX_CITYHALL_MENU25, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 215, DRAW_DIALOGBOX_CITYHALL_MENU26, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 230, DRAW_DIALOGBOX_CITYHALL_MENU27, video::SColor(255, 55, 25, 25));//"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 18);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 2);
            break;

        case 2: // Offering Citizenship.
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU28, video::SColor(255, 55, 25, 25));//"
            break;

        case 3: //Congratulations!! You acquired Citizenship"
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU29, video::SColor(255, 55, 25, 25));//"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 4: //Failed on acquiring citizenship!"
            PutAlignedString(sX, sX + szX, sY + 80, DRAW_DIALOGBOX_CITYHALL_MENU30, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 100, DRAW_DIALOGBOX_CITYHALL_MENU31, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 115, DRAW_DIALOGBOX_CITYHALL_MENU32, video::SColor(255, 55, 25, 25));//"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 5: //The prize gold for your"
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU33, video::SColor(255, 55, 25, 25));//"
            wsprintfA(cTxt, DRAW_DIALOGBOX_CITYHALL_MENU34, m_iRewardGold);//"
            PutAlignedString(sX, sX + szX, sY + 140, cTxt, video::SColor(255, 55, 25, 25));
            PutAlignedString(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU35, video::SColor(255, 55, 25, 25));//"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 18);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 2);
            break;

        case 7:// 3.51 Cityhall Menu - Request Hero's Items - Diuuude
            PutAlignedString(sX, sX + szX, sY + 60, DRAW_DIALOGBOX_CITYHALL_MENU46, video::SColor(255, 255, 255, 255));// Here are the Hero's Item aivable :
                                                                                                                       // Hero's Cape (EK 300)
            if (m_iEnemyKillCount >= 300)
            {
                if (onButton == 1)
                    PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU47, video::SColor(255, 255, 255, 255));// On mouse over Mode
                else PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU47, video::SColor(255, 4, 0, 50));// Normal Mode
            }
            else PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU47, video::SColor(255, 65, 65, 65));// Disabled Mode
                                                                                                                         // Hero's Helm (EK 150 - Contrib 20)
            if ((m_iEnemyKillCount >= 150) && (m_iContribution >= 20))
            {
                if (onButton == 2)
                    PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU48, video::SColor(255, 255, 255, 255));// On mouse over Mode
                else PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU48, video::SColor(255, 4, 0, 50));// Normal Mode
            }
            else PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU48, video::SColor(255, 65, 65, 65));// Disabled Mode
                                                                                                                          // Hero's Cap (EK 100 - Contrib 20)
            if ((m_iEnemyKillCount >= 100) && (m_iContribution >= 20))
            {
                if (onButton == 3)
                    PutAlignedString(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU49, video::SColor(255, 255, 255, 255));// On mouse over Mode
                else PutAlignedString(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU49, video::SColor(255, 4, 0, 50));// Normal Mode
            }
            else PutAlignedString(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU49, video::SColor(255, 65, 65, 65));// Disabled Mode
                                                                                                                          // Hero's Armor (EK 300 - Contrib 30)
            if ((m_iEnemyKillCount >= 300) && (m_iContribution >= 30))
            {
                if (onButton == 4)
                    PutAlignedString(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_CITYHALL_MENU50, video::SColor(255, 255, 255, 255));// On mouse over Mode
                else PutAlignedString(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_CITYHALL_MENU50, video::SColor(255, 4, 0, 50));// Normal Mode
            }
            else PutAlignedString(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_CITYHALL_MENU50, video::SColor(255, 65, 65, 65));// Disabled Mode
                                                                                                                          // Hero's Robe (EK 200 - Contrib 20)
            if ((m_iEnemyKillCount >= 200) && (m_iContribution >= 20))
            {
                if (onButton == 5)
                    PutAlignedString(sX, sX + szX, sY + 215, DRAW_DIALOGBOX_CITYHALL_MENU51, video::SColor(255, 255, 255, 255));// On mouse over Mode
                else PutAlignedString(sX, sX + szX, sY + 215, DRAW_DIALOGBOX_CITYHALL_MENU51, video::SColor(255, 4, 0, 50));// Normal Mode
            }
            else PutAlignedString(sX, sX + szX, sY + 215, DRAW_DIALOGBOX_CITYHALL_MENU51, video::SColor(255, 65, 65, 65));// Disabled Mode
                                                                                                                          // Hero's Hauberk (EK 100 - Contrib 10)
            if ((m_iEnemyKillCount >= 100) && (m_iContribution >= 10))
            {
                if (onButton == 6)
                    PutAlignedString(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU52, video::SColor(255, 255, 255, 255));// On mouse over Mode
                else PutAlignedString(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU52, video::SColor(255, 4, 0, 50));// Normal Mode
            }
            else PutAlignedString(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU52, video::SColor(255, 65, 65, 65));// Disabled Mode
                                                                                                                          // Hero's Leggings (EK 150 - Contrib 15)
            if ((m_iEnemyKillCount >= 150) && (m_iContribution >= 15))
            {
                if (onButton == 7)
                    PutAlignedString(sX, sX + szX, sY + 275, DRAW_DIALOGBOX_CITYHALL_MENU53, video::SColor(255, 255, 255, 255));// On mouse over Mode
                else PutAlignedString(sX, sX + szX, sY + 275, DRAW_DIALOGBOX_CITYHALL_MENU53, video::SColor(255, 4, 0, 50));// Normal Mode
            }
            else PutAlignedString(sX, sX + szX, sY + 275, DRAW_DIALOGBOX_CITYHALL_MENU53, video::SColor(255, 65, 65, 65));// Disabled Mode
            break;

        case 8: // cancel current quest?
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU54, video::SColor(255, 55, 25, 25));//"
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU55, video::SColor(255, 55, 25, 25));//"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 18);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 2);
            break;

        case LEGIONPTSSERVICES:
            wsprintfA(cTxt, "Xtreme points: %u", m_cash);
            PutAlignedString(sX, sX + szX, sY + 35, cTxt, video::SColor(255, 55, 25, 25));
            if (onButton == 1)
                PutAlignedString(sX + 160, sX + szX, sY + 35, DRAW_DIALOGBOX_CITYHALL_MENU14, video::SColor(255, 255, 255, 255));
            else
                PutAlignedString(sX + 160, sX + szX, sY + 35, DRAW_DIALOGBOX_CITYHALL_MENU14, video::SColor(255, 4, 0, 50));

            PutAlignedString(sX - 20, sX + szX, sY + 55, DRAW_DIALOGBOX_CITYHALL_MENU15, video::SColor(255, 55, 25, 25));
            PutAlignedString(sX + 180, sX + szX, sY + 55, DRAW_DIALOGBOX_CITYHALL_MENU16, video::SColor(255, 55, 25, 25));

            pointerLoc = m_dialogBoxes[13].HandleScroll(16, CMD_LGNSVC_MAX);

            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);

            for (i = 0; i < 16; i++)
            {
                if (i + m_dialogBoxes[13].sView < CMD_LGNSVC_MAX)
                {
                    if (onButton == i + 2)
                    {
                        PutAlignedString(sX - 30, sX + szX, sY + 75 + 15 * i, lgnPtsSvcs[i + m_dialogBoxes[13].sView].desc, video::SColor(255, 255, 255, 255));
                        wsprintfA(cTxt, "%u", lgnPtsSvcs[i + m_dialogBoxes[13].sView].price);
                        PutAlignedString(sX + 190, sX + szX, sY + 75 + 15 * i, cTxt, video::SColor(255, 255, 255, 255));
                    }
                    else
                    {
                        PutAlignedString(sX - 30, sX + szX, sY + 75 + 15 * i, lgnPtsSvcs[i + m_dialogBoxes[13].sView].desc, video::SColor(255, 4, 0, 50));
                        wsprintfA(cTxt, "%u", lgnPtsSvcs[i + m_dialogBoxes[13].sView].price);
                        PutAlignedString(sX + 190, sX + szX, sY + 75 + 15 * i, cTxt, video::SColor(255, 4, 0, 50));
                    }
                }
            }

            break;

        case TELEPORTLIST:// Teleport List
            if (m_iTeleportMapCount > 0)
            {	//PutAlignedString(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_CITYHALL_MENU69, 55,25,25); // "Teleporting to dungeon level 2."
                //PutAlignedString(sX, sX + szX, sY + 80, DRAW_DIALOGBOX_CITYHALL_MENU70, 55,25,25); // "5000Gold is required"
                //PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU71, 55,25,25); //
                //PutAlignedString(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_CITYHALL_MENU72, 55,25,25);//
                //PutString2(sX + 35, sY + 250, DRAW_DIALOGBOX_CITYHALL_MENU72_1, 55,25,25);//"Civilians cannot go some area."
                for (int i = 0; i<m_iTeleportMapCount; i++)
                {
                    if (m_stTeleportList[i].donateEvent)
                    {
                        wsprintfA(G_cTxt, DRAW_DIALOGBOX_CITYHALL_MENU78, 5);
                    }
                    else
                    {
                        ZeroMemory(cTxt, sizeof(cTxt));
                        GetOfficialMapName(m_stTeleportList[i].mapname, cTxt);
                        wsprintfA(G_cTxt, DRAW_DIALOGBOX_CITYHALL_MENU77, cTxt, m_stTeleportList[i].iCost);
                    }

                    if (onButton == i + 1)
                        PutAlignedString(sX, sX + szX, sY + 130 + i * 15, G_cTxt, video::SColor(255, 255, 255, 255));
                    else
                        PutAlignedString(sX, sX + szX, sY + 130 + i * 15, G_cTxt, video::SColor(255, 250, 250, 0));
                }
            }
            else if (m_iTeleportMapCount == -1)
            {
                PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU73, video::SColor(255, 55, 25, 25));
                PutAlignedString(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_CITYHALL_MENU74, video::SColor(255, 55, 25, 25));
            }
            else
            {
                PutAlignedString(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_CITYHALL_MENU76, video::SColor(255, 55, 25, 25));//"There is no area that you can teleport."
            }
            break;

        case 11: //drajwer - ask for tooking hero mantle
            PutAlignedString(sX, sX + szX - 1, sY + 125, m_cTakeHeroItemName, video::SColor(255, 55, 25, 25));
            PutAlignedString(sX + 1, sX + szX, sY + 125, m_cTakeHeroItemName, video::SColor(255, 55, 25, 25));
            PutAlignedString(sX, sX + szX, sY + 260, DRAW_DIALOGBOX_CITYHALL_MENU46A, video::SColor(255, 55, 25, 25)); // would you like..
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 18);
            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 2);
            break;

        case 12: // Bleeding Island Teleport
            PutAlignedString(sX, sX + szX - 1, sY + 125, "Bleeding Island", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX + 1, sX + szX, sY + 125, "Bleeding Island", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX, sX + szX - 1, sY + 140, "(100) Gold Required", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX + 1, sX + szX, sY + 140, "(100) Gold Required", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX, sX + szX, sY + 260, "Would you like to teleport here?", video::SColor(255, 55, 25, 25)); // would you like..
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 18);
            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 2);
            break;

        case 13: // Players Arena Teleport
            PutAlignedString(sX, sX + szX - 1, sY + 125, "Players Arena", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX + 1, sX + szX, sY + 125, "Players Arena", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX, sX + szX - 1, sY + 140, "(500) Gold Required", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX + 1, sX + szX, sY + 140, "(500) Gold Required", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX, sX + szX, sY + 260, "Would you like to teleport here?", video::SColor(255, 55, 25, 25)); // would you like..
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 18);
            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 2);
            break;

        case 14: // Dungeon Level 2 Teleport
            PutAlignedString(sX, sX + szX - 1, sY + 125, "Dark Crypt", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX + 1, sX + szX, sY + 125, "Dark Crypt", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX, sX + szX - 1, sY + 140, "(5000) Gold Required", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX + 1, sX + szX, sY + 140, "(5000) Gold Required", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX, sX + szX, sY + 260, "Would you like to teleport here?", video::SColor(255, 55, 25, 25)); // would you like..
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 18);
            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 2);
            break;

        case 15: // Dungeon Level 2 Teleport
            PutAlignedString(sX, sX + szX - 1, sY + 125, "Promiseland", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX + 1, sX + szX, sY + 125, "Promiseland", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX, sX + szX - 1, sY + 140, "(5000) Gold Required", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX + 1, sX + szX, sY + 140, "(5000) Gold Required", video::SColor(255, 55, 25, 25));
            PutAlignedString(sX, sX + szX, sY + 260, "Would you like to teleport here?", video::SColor(255, 55, 25, 25)); // would you like..
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 18);
            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 2);
            break;

    }
}
/*********************************************************************************************************************
**  void CGame::DrawDialogBox_ConfirmExchange()
**  description			:: paints the trade windows	Added confirmation window in v3.51								**
**********************************************************************************************************************/
void CGame::DrawDialogBox_ConfirmExchange()
{
    short sX, sY;
    char onButton = m_dialogBoxes[41].OnButton();
    sX = m_dialogBoxes[41].m_X;
    sY = m_dialogBoxes[41].m_Y;

    switch (m_dialogBoxes[41].GetMode())
    {
        case 1: // Question
            PutString3(sX + 35, sY + 30, "Do you really want to exchange?", video::SColor(255, 4, 0, 50));

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 18);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 2);
            break;

        case 2: // Waiting for response
            PutString3(sX + 45, sY + 36, "Waiting for response...", video::SColor(255, 4, 0, 50));
            break;
    }
}

void CGame::DrawDialogBox_YesNo()
{
    CDialogBox & dlg = m_dialogBoxes[DIALOG_YESNO];
    char onButton = dlg.OnButton();
    short sX = dlg.m_X;
    short sY = dlg.m_Y;

    switch (dlg.GetMode())
    {
        case 1: // Mail deletion
            PutString3(sX + 35, sY + 30, DRAW_DIALOGBOX_YESNO1, video::SColor(255, 4, 0, 50));
            break;

        case 2: // Post deletion
            PutString3(sX + 35, sY + 30, DRAW_DIALOGBOX_YESNO2, video::SColor(255, 4, 0, 50));
            break;
    }

    DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, (onButton == 1) ? 19 : 18);
    DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, (onButton == 2) ? 3 : 2);
}

void CGame::DlgBoxClick_YesNo()
{
    CDialogBox & dlg = m_dialogBoxes[DIALOG_YESNO];
    CDialogBox * mdlg;
    char onButton = dlg.OnButton();

    switch (dlg.GetMode())
    {
        case 1: // delete mail
            mdlg = &m_dialogBoxes[DIALOG_MAILBOX];
            if (onButton == 1) // yes
            {
                if (mdlg->sV1 != -1)
                {
                    mdlg->SetMode(0);
                    bSendCommand(MSGID_REQ_DELETEMAIL, 0, 0, m_mails[mdlg->sV1]->id);
                    m_mails.erase(m_mails.begin() + mdlg->sV1);
                }
                DisableDialogBox(DIALOG_YESNO);
                PlaySound('E', 14, 5);
                return;
            }
            else if (onButton == 2) // No
            {
                DisableDialogBox(DIALOG_YESNO);
                PlaySound('E', 14, 5);
                return;
            }
            break;

        case 2: // delete guild post
            mdlg = &m_dialogBoxes[DIALOG_GUILD];
            if (onButton == 1) // yes
            {
                if (mdlg->sV1 != -1)
                {
                    mdlg->SetMode(8);
                    bSendCommand(MSGID_REQ_DELETEGUILDPOST, 0, 0, m_gldBoard[mdlg->sV1]->id);
                    m_gldBoard.erase(m_gldBoard.begin() + mdlg->sV1);
                }
                DisableDialogBox(DIALOG_YESNO);
                PlaySound('E', 14, 5);
                return;
            }
            else if (onButton == 2) // No
            {
                DisableDialogBox(DIALOG_YESNO);
                PlaySound('E', 14, 5);
                return;
            }
            break;
    }
}

/*********************************************************************************************************************
**  void CGame::DrawDialogBox_Exchange()
**  description			:: paints the trade windows																	**
**********************************************************************************************************************/
void CGame::DrawDialogBox_Exchange()
{
    short sX, sY, szX, sXadd;
    uint32_t dwTime = m_dwCurTime;
    char cItemColor, cTxt[120], cTxt2[128];
    char cNameStr[120], cSubStr1[120], cSubStr2[120];
    int iLoc, i;

    char onButton = m_dialogBoxes[27].OnButton();
    sX = m_dialogBoxes[27].m_X;
    sY = m_dialogBoxes[27].m_Y;
    szX = m_dialogBoxes[27].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_NEWEXCHANGE, sX, sY, 0);
    switch (m_dialogBoxes[27].GetMode())
    {
        case 1: // Not yet confirmed exchange
            PutAlignedString(sX + 80, sX + 180, sY + 38, m_cPlayerName, 35, 55, 35); // my name
            if (m_stDialogBoxExchangeInfo[4].sV1 != -1) // Other name if applicable
                PutAlignedString(sX + 250, sX + 540, sY + 38, m_stDialogBoxExchangeInfo[4].cStr2, 35, 55, 35);

            if ((m_stDialogBoxExchangeInfo[0].sV1 != -1) && (m_stDialogBoxExchangeInfo[4].sV1 == -1))
            {
                PutAlignedString(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE9, 55, 25, 25); // Please wait until other player decides
                PutAlignedString(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE10, 55, 25, 25);// to exchange. If you want to cancel the
                PutAlignedString(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE11, 55, 25, 25);// exchange press the CANCEL button now.
                PutString_SprFont(sX + 220, sY + 310, "Exchange", 15, 15, 15);
            }
            else if ((m_stDialogBoxExchangeInfo[0].sV1 == -1) && (m_stDialogBoxExchangeInfo[4].sV1 != -1))
            {
                PutAlignedString(sX, sX + szX, sY + 205 + 10, DRAW_DIALOGBOX_EXCHANGE12, 55, 25, 25);// Other player offered an item exchange
                PutAlignedString(sX, sX + szX, sY + 220 + 10, DRAW_DIALOGBOX_EXCHANGE13, 55, 25, 25);// Select an item which you want to exc-
                PutAlignedString(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE14, 55, 25, 25);// hange with above item, drag it to the
                PutAlignedString(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE15, 55, 25, 25);// blank and press the EXCHANGE button.
                PutAlignedString(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE16, 55, 25, 25);// And you can also reject an offer by
                PutAlignedString(sX, sX + szX, sY + 280 + 10, DRAW_DIALOGBOX_EXCHANGE17, 55, 25, 25);// pressing the CANCEL button.
                PutString_SprFont(sX + 220, sY + 310, "Exchange", 15, 15, 15);
            }
            else if ((m_stDialogBoxExchangeInfo[0].sV1 != -1) && (m_stDialogBoxExchangeInfo[4].sV1 != -1))
            {
                PutAlignedString(sX, sX + szX, sY + 205 + 10, DRAW_DIALOGBOX_EXCHANGE18, 55, 25, 25);// The preparation for item exchange
                PutAlignedString(sX, sX + szX, sY + 220 + 10, DRAW_DIALOGBOX_EXCHANGE19, 55, 25, 25);// has been finished. Press the EXCHANGE
                PutAlignedString(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE20, 55, 25, 25);// button to exchange as above. Press the
                PutAlignedString(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE21, 55, 25, 25);// CANCEL button to cancel. Occasionally
                PutAlignedString(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE22, 55, 25, 25);// when you press the EXCHANGE button, you
                PutAlignedString(sX, sX + szX, sY + 280 + 10, DRAW_DIALOGBOX_EXCHANGE23, 55, 25, 25);// will not be able to cancel the exchange.
                if (onButton == 1)
                    PutString_SprFont(sX + 220, sY + 310, "Exchange", 6, 6, 20);
                else PutString_SprFont(sX + 220, sY + 310, "Exchange", 0, 0, 7);
            }
            if (onButton == 2 && (m_bIsDialogEnabled[41] == false))
                PutString_SprFont(sX + 450, sY + 310, "Cancel", 6, 6, 20);
            else PutString_SprFont(sX + 450, sY + 310, "Cancel", 0, 0, 7);

            for (i = 0; i<8; i++) // draw items
            {
                sXadd = (58 * i) + 48; if (i>3) sXadd += 20;
                if (m_stDialogBoxExchangeInfo[i].sV1 != -1)
                {
                    cItemColor = m_stDialogBoxExchangeInfo[i].sV4;
                    if (cItemColor == 0)
                    {
                        m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->PutSpriteFast(sX + sXadd, sY + 130, m_stDialogBoxExchangeInfo[i].sV2, dwTime);
                    }
                    else
                    {
                        switch (m_stDialogBoxExchangeInfo[i].sV1)
                        {//sV1 : Sprite
                            case 1: //  Swds
                            case 2: //  Bows
                            case 3: //  Shields
                            case 15: // Axes hammers
                            case 17: // Wands
                                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->PutSpriteRGB(sX + sXadd, sY + 130
                                                                                                                      , m_stDialogBoxExchangeInfo[i].sV2, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                                break;
                            default: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->PutSpriteRGB(sX + sXadd, sY + 130
                                                                                                                           , m_stDialogBoxExchangeInfo[i].sV2, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                                break;
                        }
                    }

                    // If pointer over item then show this item data
                    if (G_pGame->m_stMCursor.sX >= sX + sXadd - 6 && G_pGame->m_stMCursor.sX <= sX + sXadd + 42
                        && G_pGame->m_stMCursor.sY >= sY + 61 && G_pGame->m_stMCursor.sY <= sY + 200)
                    {
                        std::vector<string> * lines = GetItemName(m_stDialogBoxExchangeInfo[i].cStr1, m_stDialogBoxExchangeInfo[i].dwV1,
                                                                  m_stDialogBoxExchangeInfo[i].sockets, m_stDialogBoxExchangeInfo[i].sV3);

                        iLoc = 0;

                        if (m_stDialogBoxExchangeInfo[i].sV5 != -1) // completion
                        {
                            string str;
                            cTxt[0] = 0;
                            // Crafting Magins completion fix
                            if (m_stDialogBoxExchangeInfo[i].sV1 == 22)
                            {
                                if (m_stDialogBoxExchangeInfo[i].sV2 > 5 && m_stDialogBoxExchangeInfo[i].sV2 < 10)
                                {
                                    wsprintfA(cTxt, GET_ITEM_NAME2, (m_stDialogBoxExchangeInfo[i].sV7 - 100)); //Completion - 100
                                }
                            }
                            else if (m_stDialogBoxExchangeInfo[i].sV1 == 6)
                            {
                                wsprintfA(cTxt, GET_ITEM_NAME1, (m_stDialogBoxExchangeInfo[i].sV7 - 100)); //Purity
                            }
                            else
                            {
                                wsprintfA(cTxt, GET_ITEM_NAME2, m_stDialogBoxExchangeInfo[i].sV7); //Completion
                            }
                            if (cTxt[0])
                            {
                                str.append(cTxt);
                                lines->push_back(str);
                            }
                        }

                        {
                            string str;
                            wsprintfA(cTxt, DRAW_DIALOGBOX_EXCHANGE3, m_stDialogBoxExchangeInfo[i].sV5, m_stDialogBoxExchangeInfo[i].sV6);

                            str.append(cTxt);
                            lines->push_back(str);
                        }

                        std::vector<string>::iterator iter = lines->begin(), end = lines->end();

                        uint32_t maxWidth = 0;

                        while (iter != end)
                        {
                            if (iter->size() > maxWidth)
                                maxWidth = iter->size();

                            ++iter;
                        }
                        //DIRECTX m_DDraw.DrawShadowBox(sX + 5-3, sY + 215 - 1, sX + 5 + maxWidth * 5.5, sY + 215+1 + 13*lines->size());

                        iter = lines->begin();

                        if (video::SColor(m_itemColor) != video::SColor(255, 255, 255, 255))
                        {
                            PutString(sX + 5, sY + 215 + iLoc, iter->c_str(), m_itemColor, false, 1);
                        }
                        else
                        {
                            PutString(sX + 5, sY + 215 + iLoc, iter->c_str(), video::SColor(255, 180, 180, 180), false, 1);
                        }
                        iLoc += 13;
                        ++iter;

                        while (iter != end)
                        {
                            PutString(sX + 5, sY + 215 + iLoc, iter->c_str(), video::SColor(255, 150, 150, 150), false, 1);
                            iLoc += 13;
                            ++iter;
                        }
                    }
                }
            }
            break;

        case 2: // You have confirmed the exchange
            PutAlignedString(sX + 80, sX + 180, sY + 38, m_cPlayerName, video::SColor(255, 35, 55, 35)); // my name
            if (m_stDialogBoxExchangeInfo[4].sV1 != -1) // Other name if applicable
                PutAlignedString(sX + 250, sX + 540, sY + 38, m_stDialogBoxExchangeInfo[4].cStr2, video::SColor(255, 35, 55, 35));

            wsprintfA(cTxt, DRAW_DIALOGBOX_EXCHANGE33, m_stDialogBoxExchangeInfo[4].cStr2);
            PutAlignedString(sX, sX + szX, sY + 205 + 10, cTxt, video::SColor(255, 55, 25, 25));                     // Please wait until %s agrees to
            PutAlignedString(sX, sX + szX, sY + 220 + 10, DRAW_DIALOGBOX_EXCHANGE34, video::SColor(255, 55, 25, 25));// exchange. The exchange can't be achieved
            PutAlignedString(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE35, video::SColor(255, 55, 25, 25));// unless both people agree.
            PutAlignedString(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE36, video::SColor(255, 55, 25, 25));//  If other player does not decide to exchange
            PutAlignedString(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE37, video::SColor(255, 55, 25, 25));// you can cancel the exchange by pressing the
            PutAlignedString(sX, sX + szX, sY + 280 + 10, DRAW_DIALOGBOX_EXCHANGE38, video::SColor(255, 55, 25, 25));// CANCEL button. But if other player already
            PutAlignedString(sX, sX + szX, sY + 295 + 10, DRAW_DIALOGBOX_EXCHANGE39, video::SColor(255, 55, 25, 25));// decided to exchange, you can't cancel anymore

            for (i = 0; i<8; i++) // draw items
            {
                sXadd = (58 * i) + 48; if (i>3) sXadd += 20;
                if (m_stDialogBoxExchangeInfo[i].sV1 != -1)
                {
                    cItemColor = m_stDialogBoxExchangeInfo[i].sV4;
                    if (cItemColor == 0)
                    {
                        m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->PutSpriteFast(sX + sXadd, sY + 130, m_stDialogBoxExchangeInfo[i].sV2, dwTime);
                    }
                    else
                    {
                        switch (m_stDialogBoxExchangeInfo[i].sV1)
                        {
                            case 1: // Swds
                            case 2: // Bows
                            case 3: // Shields
                            case 15: // Axes hammers
                            case 17: // Wands
                                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->PutSpriteRGB(sX + sXadd, sY + 130
                                                                                                                      , m_stDialogBoxExchangeInfo[i].sV2, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                                break;
                            default: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->PutSpriteRGB(sX + sXadd, sY + 130
                                                                                                                           , m_stDialogBoxExchangeInfo[i].sV2, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                                break;
                        }
                    }
                    // If pointer over item then show this item data
                    if (G_pGame->m_stMCursor.sX >= sX + sXadd - 6 && G_pGame->m_stMCursor.sX <= sX + sXadd + 42
                        && G_pGame->m_stMCursor.sY >= sY + 61 && G_pGame->m_stMCursor.sY <= sY + 200)
                    {
                        std::vector<string> * lines = GetItemName(m_stDialogBoxExchangeInfo[i].cStr1, m_stDialogBoxExchangeInfo[i].dwV1,
                                                                  m_stDialogBoxExchangeInfo[i].sockets, m_stDialogBoxExchangeInfo[i].sV3);

                        iLoc = 0;

                        if (m_stDialogBoxExchangeInfo[i].sV5 != -1) // completion
                        {
                            string str;
                            // Crafting Magins completion fix
                            if (m_stDialogBoxExchangeInfo[i].sV1 == 22)
                            {
                                if (m_stDialogBoxExchangeInfo[i].sV2 > 5 && m_stDialogBoxExchangeInfo[i].sV2 < 10)
                                {
                                    wsprintfA(cTxt, GET_ITEM_NAME2, (m_stDialogBoxExchangeInfo[i].sV7 - 100)); //Completion - 100
                                }
                            }
                            else if (m_stDialogBoxExchangeInfo[i].sV1 == 6)
                            {
                                wsprintfA(cTxt, GET_ITEM_NAME1, (m_stDialogBoxExchangeInfo[i].sV7 - 100)); //Purity
                            }
                            else
                            {
                                wsprintfA(cTxt, GET_ITEM_NAME2, m_stDialogBoxExchangeInfo[i].sV7); //Completion
                            }
                            str.append(cTxt);
                            lines->push_back(str);
                        }

                        {
                            string str;
                            wsprintfA(cTxt, DRAW_DIALOGBOX_EXCHANGE3, m_stDialogBoxExchangeInfo[i].sV5, m_stDialogBoxExchangeInfo[i].sV6);

                            str.append(cTxt);
                            lines->push_back(str);
                        }

                        std::vector<string>::iterator iter = lines->begin(), end = lines->end();

                        uint32_t maxWidth = 0;

                        while (iter != end)
                        {
                            if (iter->size() > maxWidth)
                                maxWidth = iter->size();

                            ++iter;
                        }
                        //DIRECTX m_DDraw.DrawShadowBox(sX + 5-3, sY + 215 - 1, sX + 5 + maxWidth * 5.5, sY + 215+1 + 13*lines->size());

                        iter = lines->begin();

                        if (video::SColor(m_itemColor) != video::SColor(255, 255, 255, 255))
                        {
                            PutString(sX + 5, sY + 215 + iLoc, iter->c_str(), video::SColor(m_itemColor), false, 1);
                        }
                        else
                        {
                            PutString(sX + 5, sY + 215 + iLoc, iter->c_str(), video::SColor(255, 180, 180, 180), false, 1);
                        }
                        iLoc += 13;
                        ++iter;

                        while (iter != end)
                        {
                            PutString(sX + 5, sY + 215 + iLoc, iter->c_str(), video::SColor(255, 150, 150, 150), false, 1);
                            iLoc += 13;
                            ++iter;
                        }
                    }
                }
            }
            break;
    }
}

void CGame::DrawDialogBox_Fishing()
{

    short sX, sY;
    uint32_t dwTime = m_dwCurTime;
    char  cTxt[120];

    char onButton = m_dialogBoxes[24].OnButton();
    sX = m_dialogBoxes[24].m_X;
    sY = m_dialogBoxes[24].m_Y;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME1, sX, sY, 2);

    char cStr1[64], cStr2[64], cStr3[64];
    GetItemName(m_dialogBoxes[24].cStr, 0, cStr1, cStr2, cStr3);

    switch (m_dialogBoxes[24].GetMode())
    {
        case 0:
            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_dialogBoxes[24].sV3]->PutSpriteFast(sX + 18 + 35, sY + 18 + 17, m_dialogBoxes[24].sV4, dwTime);
            wsprintfA(cTxt, "%s", cStr1);
            PutString(sX + 98, sY + 14, cTxt, video::SColor(255, 255, 255, 255));
            wsprintfA(cTxt, DRAW_DIALOGBOX_FISHING1, m_dialogBoxes[24].sV2);
            PutString(sX + 98, sY + 28, cTxt, video::SColor(255, 0, 0, 0));
            PutString(sX + 97, sY + 43, DRAW_DIALOGBOX_FISHING2, video::SColor(255, 0, 0, 0));
            wsprintfA(cTxt, "%d %%", m_dialogBoxes[24].sV1);
            PutString_SprFont(sX + 157, sY + 40, cTxt, 10, 0, 0);
            if (onButton == 1)
                PutString_SprFont(sX + 160, sY + 70, "Try Now!", 6, 6, 20);
            else PutString_SprFont(sX + 160, sY + 70, "Try Now!", 0, 0, 7);
            break;
    }

}

void CGame::DrawDialogBox_GuildMenu()
{
    short sX, sY, szX;
    int iAdjX, iAdjY;

    char onButton = m_dialogBoxes[7].OnButton();
    sX = m_dialogBoxes[7].m_X;
    sY = m_dialogBoxes[7].m_Y;
    szX = m_dialogBoxes[7].sSizeX;

    iAdjX = -13;
    iAdjY = 30;
    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 19);

    switch (m_dialogBoxes[7].GetMode())
    {
        case 0:
            if ((m_iGuildRank == -1) && /*(m_stat[STAT_CHR] >= 20) &&*/ (m_iLevel >= 100))
            { // Guild Req xRisenx
                if (onButton == 1)
                    PutAlignedString(sX, sX + szX, sY + iAdjY + 65, DRAW_DIALOGBOX_GUILDMENU1, 255, 255, 255);//"
                else PutAlignedString(sX, sX + szX, sY + iAdjY + 65, DRAW_DIALOGBOX_GUILDMENU1, 4, 0, 50);//"
            }
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 65, DRAW_DIALOGBOX_GUILDMENU1, 45, 45, 45);//"

            if (m_iGuildRank == GUILDRANK_MASTER)
            {
                if (onButton == 2)
                    PutAlignedString(sX, sX + szX, sY + iAdjY + 85, DRAW_DIALOGBOX_GUILDMENU4, 255, 255, 255);//"
                else PutAlignedString(sX, sX + szX, sY + iAdjY + 85, DRAW_DIALOGBOX_GUILDMENU4, 4, 0, 50);//"
            }
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 85, DRAW_DIALOGBOX_GUILDMENU4, 65, 65, 65);//"

            if (onButton == 3)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 105, DRAW_DIALOGBOX_GUILDMENU7, 255, 255, 255);//"
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 105, DRAW_DIALOGBOX_GUILDMENU7, 4, 0, 50);//"

            if (onButton == 4)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 125, DRAW_DIALOGBOX_GUILDMENU9, 255, 255, 255);//"
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 125, DRAW_DIALOGBOX_GUILDMENU9, 4, 0, 50);//"

            if (m_iGuildRank == GUILDRANK_MASTER && m_iFightzoneNumber == 0)
            {
                if (onButton == 5)
                    PutAlignedString(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU11, 255, 255, 255);//"
                else PutAlignedString(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU11, 4, 0, 50);//"

            }
            else if (m_iGuildRank == GUILDRANK_MASTER && m_iFightzoneNumber > 0)
            {
                if (onButton == 6)
                    PutAlignedString(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU13, 255, 255, 255);//"
                else PutAlignedString(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU13, 4, 0, 50);//"

            }
            else if (m_iFightzoneNumber < 0)
            {
                PutAlignedString(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU13, 65, 65, 65);//"
            }
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU11, 65, 65, 65);//"

            PutAlignedString(sX, sX + szX, sY + iAdjY + 205, DRAW_DIALOGBOX_GUILDMENU17);//"
            break;

        case 1:
            PutAlignedString(sX + 24, sX + 239, sY + 125, DRAW_DIALOGBOX_GUILDMENU18, 55, 25, 25);//"
            PutString(sX + 75, sY + 144, "____________________", video::SColor(255, 25, 35, 25));

            if (iGetTopDialogBoxIndex() != 7)
                PutString(sX + 75, sY + 140, m_cGuildName, video::SColor(255, 255, 255, 255), 16, false, 2);

            if (onButton == 1)
            {
                if ((strcmp(m_cGuildName, "NONE") == 0) || (strlen(m_cGuildName) == 0))
                {
                    DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 24);//Create Gray Button
                }
                else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 25);//Create Highlight Button
            }
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 24);//Create Gray Button

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);//Red Cancel Button
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);//Gray Cancel Button
            break;

        case 2:
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU19, 55, 25, 25);//"
            break;
        case 3:
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_GUILDMENU20, 55, 25, 25);//"
            PutAlignedString(sX, sX + szX, sY + 140, m_cGuildName, 55, 25, 25);
            PutAlignedString(sX, sX + szX, sY + 144, "____________________", 25, 35, 25);
            PutAlignedString(sX, sX + szX, sY + 160, DRAW_DIALOGBOX_GUILDMENU21, 55, 25, 25);//"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;
        case 4:
            PutAlignedString(sX, sX + szX, sY + 135, DRAW_DIALOGBOX_GUILDMENU22, 55, 25, 25);//"
            PutAlignedString(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_GUILDMENU23, 55, 25, 25);//"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;
        case 5:
            PutAlignedString(sX, sX + szX, sY + 90, DRAW_DIALOGBOX_GUILDMENU24);
            PutAlignedString(sX, sX + szX, sY + 105, m_cGuildName, 35, 35, 35);
            PutAlignedString(sX, sX + szX, sY + 109, "____________________", 0, 0, 0);
            PutAlignedString(sX, sX + szX, sY + 130, DRAW_DIALOGBOX_GUILDMENU25);//"
            PutAlignedString(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_GUILDMENU26);//"
            PutAlignedString(sX, sX + szX, sY + 160, DRAW_DIALOGBOX_GUILDMENU27);//"
            PutAlignedString(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_GUILDMENU28, 55, 25, 25);//"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 18);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 2);
            break;
        case 6:
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU29, 55, 25, 25);//"
            break;
        case 7:
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU30, 55, 25, 25);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;
        case 8:
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU31, 55, 25, 25);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;
        case 9:
            PutAlignedString(sX, sX + szX, sY + iAdjY + 60, DRAW_DIALOGBOX_GUILDMENU32);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 75, DRAW_DIALOGBOX_GUILDMENU33);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 90, DRAW_DIALOGBOX_GUILDMENU34);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 105, DRAW_DIALOGBOX_GUILDMENU35);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 130, DRAW_DIALOGBOX_GUILDMENU36);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 31);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 30);
            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;
        case 10:
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU37, 55, 25, 25);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;
        case 11:
            PutAlignedString(sX, sX + szX, sY + iAdjY + 60, DRAW_DIALOGBOX_GUILDMENU38);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 75, DRAW_DIALOGBOX_GUILDMENU39);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 90, DRAW_DIALOGBOX_GUILDMENU40);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 105, DRAW_DIALOGBOX_GUILDMENU41);//
            PutAlignedString(sX, sX + szX, sY + iAdjY + 130, DRAW_DIALOGBOX_GUILDMENU42);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 31);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 30);
            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;
        case 12:
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU43, 55, 25, 25);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 13:
            PutAlignedString(sX, sX + szX, sY + iAdjY + 40, DRAW_DIALOGBOX_GUILDMENU44);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 55, DRAW_DIALOGBOX_GUILDMENU45);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 70, DRAW_DIALOGBOX_GUILDMENU46);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 85, DRAW_DIALOGBOX_GUILDMENU47);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 100, DRAW_DIALOGBOX_GUILDMENU48);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 115, DRAW_DIALOGBOX_GUILDMENU49);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 130, DRAW_DIALOGBOX_GUILDMENU50);//"

            if (onButton == 1)
                PutString(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 170, DRAW_DIALOGBOX_GUILDMENU51, video::SColor(255, 255, 255, 255));
            else PutString(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 170, DRAW_DIALOGBOX_GUILDMENU51, video::SColor(255, 4, 0, 50));

            if (onButton == 2)
                PutString(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 170, DRAW_DIALOGBOX_GUILDMENU53, video::SColor(255, 255, 255, 255));
            else PutString(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 170, DRAW_DIALOGBOX_GUILDMENU53, video::SColor(255, 4, 0, 50));

            if (onButton == 3)
                PutString(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 190, DRAW_DIALOGBOX_GUILDMENU55, video::SColor(255, 255, 255, 255));
            else PutString(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 190, DRAW_DIALOGBOX_GUILDMENU55, video::SColor(255, 4, 0, 50));

            if (onButton == 4)
                PutString(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 190, DRAW_DIALOGBOX_GUILDMENU57, video::SColor(255, 255, 255, 255));
            else PutString(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 190, DRAW_DIALOGBOX_GUILDMENU57, video::SColor(255, 4, 0, 50));

            if (onButton == 5)
                PutString(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 210, DRAW_DIALOGBOX_GUILDMENU59, video::SColor(255, 255, 255, 255));
            else PutString(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 210, DRAW_DIALOGBOX_GUILDMENU59, video::SColor(255, 4, 0, 50));

            if (onButton == 6)
                PutString(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 210, DRAW_DIALOGBOX_GUILDMENU61, video::SColor(255, 255, 255, 255));
            else PutString(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 210, DRAW_DIALOGBOX_GUILDMENU61, video::SColor(255, 4, 0, 50));

            if (onButton == 7)
                PutString(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 230, DRAW_DIALOGBOX_GUILDMENU63, video::SColor(255, 255, 255, 255));
            else PutString(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 230, DRAW_DIALOGBOX_GUILDMENU63, video::SColor(255, 4, 0, 50));

            if (onButton == 8)
                PutString(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 230, DRAW_DIALOGBOX_GUILDMENU65, video::SColor(255, 255, 255, 255));
            else PutString(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 230, DRAW_DIALOGBOX_GUILDMENU65, video::SColor(255, 4, 0, 50));

            if (onButton == 9)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;

        case 14:
            PutAlignedString(sX, sX + szX, sY + 130, DRAW_DIALOGBOX_GUILDMENU66, 55, 25, 25);//"
            PutAlignedString(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_GUILDMENU67, 55, 25, 25);//"
            PutAlignedString(sX, sX + szX, sY + 160, DRAW_DIALOGBOX_GUILDMENU68, 55, 25, 25);//
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 15:
            PutAlignedString(sX, sX + szX, sY + 135, DRAW_DIALOGBOX_GUILDMENU69, 55, 25, 25);//"
            PutAlignedString(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_GUILDMENU70, 55, 25, 25);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 16:
            PutAlignedString(sX, sX + szX, sY + 135, DRAW_DIALOGBOX_GUILDMENU71, 55, 25, 25);//"
            PutAlignedString(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_GUILDMENU72, 55, 25, 25);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 17:
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU73, 55, 25, 25);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 18:
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU74, 55, 25, 25);//"
            break;

        case 19:
            if (m_iFightzoneNumber >0)
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET, 0, 0, 0, 0, 0);
            m_dialogBoxes[7].SetMode(0);
            break;

        case 20:
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_GUILDMENU75, 55, 25, 25);//"
            PutString(sX + 75, sY + 144, "____________________", video::SColor(255, 25, 35, 25));
            PutString(sX + 75, sY + 140, m_cGuildName, video::SColor(255, 255, 255, 255), false, 2);
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 25);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 24);
            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;

        case 21:
            PutAlignedString(sX, sX + szX, sY + iAdjY + 95, DRAW_DIALOGBOX_GUILDMENU76, 55, 25, 25);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 110, DRAW_DIALOGBOX_GUILDMENU77, 55, 25, 25);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 135, DRAW_DIALOGBOX_GUILDMENU78, 55, 25, 25);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 150, DRAW_DIALOGBOX_GUILDMENU79, 55, 25, 25);//"
            PutAlignedString(sX, sX + szX, sY + iAdjY + 165, DRAW_DIALOGBOX_GUILDMENU80, 55, 25, 25);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 22:
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU81, 55, 25, 25);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;
    }
}

void CGame::DrawDialogBox_Guild()
{
    CDialogBox & dlg = m_dialogBoxes[DIALOG_GUILD];

    char txt[200];
    int iTotalLines, iPointerLoc, maxLines, line = 0;
    double d1, d2, d3;
    uint32_t view;
    MemberList::const_iterator it;
    Point pointerLoc;

    char onButton = dlg.OnButton();
    short sX = dlg.m_X;
    short sY = dlg.m_Y;
    short szX = dlg.sSizeX;

    int iAdjX = -13, cropX = 22;
    int iAdjY = 30;

    if (dlg.GetMode() != 1 && // members list
        !(dlg.GetMode() >= 8 && dlg.GetMode() <= 10)) // message board
    {
        wsprintfA(txt, DRAW_DIALOGBOX_GUILD4, m_guild.GetOnlineCnt() + 1);
        PutAlignedString(sX + cropX, sX + szX - cropX, sY + 35, txt);

        DisplayCommaNumber_G_cTxt(m_guild.m_gold);
        wsprintfA(txt, DRAW_DIALOGBOX_GUILD10, G_cTxt);
        PutAlignedString(sX + cropX, sX + szX - cropX, sY + 50, txt);

        DisplayCommaNumber_G_cTxt(m_guild.m_maj);
        wsprintfA(txt, DRAW_DIALOGBOX_GUILD11, G_cTxt);
        PutAlignedString(sX + cropX, sX + szX - cropX, sY + 35, txt);

        DisplayCommaNumber_G_cTxt(m_guild.m_cont);
        wsprintfA(txt, DRAW_DIALOGBOX_GUILD29, G_cTxt);
        PutAlignedString(sX + cropX, sX + szX - cropX, sY + 50, txt);
    }

    switch (dlg.GetMode())
    {
        case 0:
            // online members
            //uncomment		ButtonString(dlg, 1, DRAW_DIALOGBOX_GUILD1);
            // give gold
            //uncomment		ButtonString(dlg, 2, DRAW_DIALOGBOX_GUILD2);
            // give maj
            //uncomment		ButtonString(dlg, 3, DRAW_DIALOGBOX_GUILD3);
            // give cont
            //uncomment		ButtonString(dlg, 4, DRAW_DIALOGBOX_GUILD43);
            // Upgrades
            //uncomment		ButtonString(dlg, 5, DRAW_DIALOGBOX_GUILD12);
            // Message board
            //uncomment		ButtonString(dlg, 6, DRAW_DIALOGBOX_GUILD45);

            break;

        case 1:
            PutAlignedString(sX + 5, sX + 5 + szX / 3, sY + 35, DRAW_DIALOGBOX_GUILD5, 55, 25, 25);
            PutAlignedString(sX + szX / 3 - 16, sX + szX / 3 * 2 - 11, sY + 35, DRAW_DIALOGBOX_GUILD6, 55, 25, 25);
            PutAlignedString(sX + szX / 3 * 2 - 38, sX + szX, sY + 35, DRAW_DIALOGBOX_GUILD7, 55, 25, 25);


            iTotalLines = m_guild.GetOnlineCnt();

            maxLines = 16;
            if (gldRankPerm[m_iGuildRank].canSummon)
                maxLines--;

            if (gldRankPerm[m_iGuildRank].canPromote)
                maxLines--;

            pointerLoc = dlg.HandleScroll(maxLines, iTotalLines);

            if (iTotalLines > maxLines)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
            }

            view = dlg.sView;

            it = m_guild.m_onlineList.begin();

            while (view > 0 && it != m_guild.m_onlineList.end())
            {
                ++it;
                view--;
            }

            for (int i = 0; i < maxLines; i++, ++it)
            {
                if (it == m_guild.m_onlineList.end())
                    break;
                if (i + dlg.sView < m_guild.m_onlineList.size())
                {
                    if (gldRankPerm[m_iGuildRank].canSummon && onButton == i + 2)
                    {
                        PutAlignedString(sX + 5, sX + 5 + szX / 3, sY + 55 + 15 * i, (*it).first.c_str(), 255, 255, 255);
                    }
                    else
                    {
                        PutAlignedString(sX + 5, sX + 5 + szX / 3, sY + 55 + 15 * i, (*it).first.c_str());
                    }

                    if ((*it).second.rank < GUILDRANK_MAX)
                    {
                        if (gldRankPerm[m_iGuildRank].canPromote && onButton == i + 18 &&
                            m_iGuildRank < (*it).second.rank)
                        {
                            PutAlignedString(sX + szX / 3 - 16, sX + szX / 3 * 2 - 11, sY + 55 + 15 * i,
                                             GetGuildRankName((*it).second.rank), 255, 255, 255);
                        }
                        else
                        {
                            PutAlignedString(sX + szX / 3 - 16, sX + szX / 3 * 2 - 11, sY + 55 + 15 * i,
                                             GetGuildRankName((*it).second.rank));
                        }
                    }
                    GetOfficialMapName(GetMapName((*it).second.map), txt);
                    PutAlignedString(sX + szX / 3 * 2 - 40 + 1, sX + szX / 3 * 3 + 1, sY + 55 + 15 * i, txt);
                }
            }

            if (gldRankPerm[m_iGuildRank].canSummon)
            {
                wsprintfA(txt, DRAW_DIALOGBOX_GUILD8, GUILDSUMMONSGOLDCOST);
                PutAlignedString(sX, sX + szX, sY + 265, txt, 55, 25, 25);
            }

            if (gldRankPerm[m_iGuildRank].canPromote)
            {
                PutAlignedString(sX, sX + szX, sY + 280, DRAW_DIALOGBOX_GUILD9, 55, 25, 25);
            }

            if (onButton == 1)
                PutAlignedString(sX, sX + szX, sY + 295 + 15 * line, DRAW_DIALOGBOX_GUILD15, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 295 + 15 * line, DRAW_DIALOGBOX_GUILD15, 4, 0, 50);
            break;

        case 2:
            // warehouse
            sprintf(txt, DRAW_DIALOGBOX_GUILD13, m_guild.m_upgrades[GU_WAREHOUSE]);
            if (onButton == 1)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, txt, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, txt, 4, 0, 50);
            line++;

            // teleport
            sprintf(txt, DRAW_DIALOGBOX_GUILD14, m_guild.m_upgrades[GU_SUMMONS]);
            if (onButton == 2)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, txt, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, txt, 4, 0, 50);
            line++;

            // captaincy
            sprintf(txt, DRAW_DIALOGBOX_GUILD33, m_guild.m_upgrades[GU_CAPTAINCY], m_guild.m_captains,
                    GetGuildRankName(GUILDRANK_CAPTAIN));
            if (onButton == 3)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, txt, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, txt, 4, 0, 50);
            line++;

            // raidmasters
            sprintf(txt, DRAW_DIALOGBOX_GUILD34, m_guild.m_upgrades[GU_RAIDMASTERS], m_guild.m_raidmasters,
                    GetGuildRankName(GUILDRANK_RAIDMASTER));
            if (onButton == 4)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, txt, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, txt, 4, 0, 50);
            line++;

            // huntmasters
            sprintf(txt, DRAW_DIALOGBOX_GUILD35, m_guild.m_upgrades[GU_HUNTMASTERS], m_guild.m_huntmasters,
                    GetGuildRankName(GUILDRANK_HUNTMASTER));
            if (onButton == 5)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, txt, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, txt, 4, 0, 50);
            line++;

            line++;
            // back
            if (onButton == 7)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, DRAW_DIALOGBOX_GUILD15, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * line, DRAW_DIALOGBOX_GUILD15, 4, 0, 50);

            break;

        case 3:
            sprintf(txt, DRAW_DIALOGBOX_GUILD16, m_guild.m_upgrades[GU_WAREHOUSE]);
            PutAlignedString(sX + cropX, sX + szX, sY + iAdjY + 45 + 15 * 0, txt);

            // upgrade
            if (m_iGuildRank == GUILDRANK_MASTER)
            {
                if (onButton == 1)
                    PutAlignedString(sX + szX / 3 * 2, sX + szX, sY + iAdjY + 45 + 15 * 0, DRAW_DIALOGBOX_GUILD17, 255, 255, 255);
                else PutAlignedString(sX + szX / 3 * 2, sX + szX, sY + iAdjY + 45 + 15 * 0, DRAW_DIALOGBOX_GUILD17, 4, 0, 50);
            }

            // back
            if (onButton == 2)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * 1, DRAW_DIALOGBOX_GUILD15, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * 1, DRAW_DIALOGBOX_GUILD15, 4, 0, 50);

            PutAlignedString(sX + cropX, sX + szX, sY + iAdjY + 45 + 15 * 2, DRAW_DIALOGBOX_GUILD18, true);

            for (int i = 0; i < gldUps[GU_WAREHOUSE].maxLvl; i++)
            {
                sprintf(txt, DRAW_DIALOGBOX_GUILD20, i + 1);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (5 + i * 2), txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD21, gldUps[GU_WAREHOUSE].costGold[i + 1] / 1000,
                        gldUps[GU_WAREHOUSE].costMaj[i + 1] / 1000, gldUps[GU_WAREHOUSE].costCont[i + 1] / 1000);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (5 + i * 2), txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD22, maxGWHItems[i + 1]);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (6 + i * 2), txt);
            }
            break;

        case 4:
            sprintf(txt, DRAW_DIALOGBOX_GUILD23, m_guild.m_upgrades[GU_SUMMONS]);
            PutAlignedString(sX + cropX, sX + szX, sY + iAdjY + 45 + 15 * 0, txt);

            // upgrade
            if (m_iGuildRank == GUILDRANK_MASTER)
            {
                if (onButton == 1)
                    PutAlignedString(sX + szX / 3 * 2, sX + szX, sY + iAdjY + 45 + 15 * 0, DRAW_DIALOGBOX_GUILD17, 255, 255, 255);
                else PutAlignedString(sX + szX / 3 * 2, sX + szX, sY + iAdjY + 45 + 15 * 0, DRAW_DIALOGBOX_GUILD17, 4, 0, 50);
            }

            // back
            if (onButton == 2)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * 1, DRAW_DIALOGBOX_GUILD15, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * 1, DRAW_DIALOGBOX_GUILD15, 4, 0, 50);


            sprintf(txt, DRAW_DIALOGBOX_GUILD24, GUILDSUMMONSGOLDCOST);
            PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 2, txt, true);

            {
                int lvl = 1;
                sprintf(txt, DRAW_DIALOGBOX_GUILD20, lvl);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 6, txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD21, gldUps[GU_SUMMONS].costGold[lvl] / 1000,
                        gldUps[GU_SUMMONS].costMaj[lvl] / 1000, gldUps[GU_SUMMONS].costCont[lvl] / 1000);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 6, txt);

                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 7, DRAW_DIALOGBOX_GUILD25, true);
            }
            {
                int lvl = 2;
                sprintf(txt, DRAW_DIALOGBOX_GUILD20, lvl);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 8, txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD21, gldUps[GU_SUMMONS].costGold[lvl] / 1000,
                        gldUps[GU_SUMMONS].costMaj[lvl] / 1000, gldUps[GU_SUMMONS].costCont[lvl] / 1000);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 8, txt);

                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 9, DRAW_DIALOGBOX_GUILD26, true);
            }
            {
                int lvl = 3;
                sprintf(txt, DRAW_DIALOGBOX_GUILD20, lvl);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 10, txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD21, gldUps[GU_SUMMONS].costGold[lvl] / 1000,
                        gldUps[GU_SUMMONS].costMaj[lvl] / 1000, gldUps[GU_SUMMONS].costCont[lvl] / 1000);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 10, txt);

                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 11, DRAW_DIALOGBOX_GUILD27, true);
            }
            {
                int lvl = 4;
                sprintf(txt, DRAW_DIALOGBOX_GUILD20, lvl);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 13, txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD21, gldUps[GU_SUMMONS].costGold[lvl] / 1000,
                        gldUps[GU_SUMMONS].costMaj[lvl] / 1000, gldUps[GU_SUMMONS].costCont[lvl] / 1000);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 13, txt);

                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 14, DRAW_DIALOGBOX_GUILD28, true);
            }
            break;

        case 5:
            sprintf(txt, DRAW_DIALOGBOX_GUILD30, m_guild.m_upgrades[GU_CAPTAINCY]);
            PutAlignedString(sX + cropX, sX + szX, sY + iAdjY + 45 + 15 * 0, txt);

            sprintf(txt, DRAW_DIALOGBOX_GUILD44, GetGuildRankName(GUILDRANK_CAPTAIN), m_guild.m_captains);
            PutAlignedString(sX + cropX, sX + szX, sY + iAdjY + 45 + 15 * 1, txt);

            // upgrade
            if (m_iGuildRank == GUILDRANK_MASTER)
            {
                if (onButton == 1)
                    PutAlignedString(sX + szX / 3 * 2, sX + szX, sY + iAdjY + 45 + 15 * 0, DRAW_DIALOGBOX_GUILD17, 255, 255, 255);
                else PutAlignedString(sX + szX / 3 * 2, sX + szX, sY + iAdjY + 45 + 15 * 0, DRAW_DIALOGBOX_GUILD17, 4, 0, 50);
            }

            // back
            if (onButton == 2)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * 1, DRAW_DIALOGBOX_GUILD15, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * 1, DRAW_DIALOGBOX_GUILD15, 4, 0, 50);

            PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 2, DRAW_DIALOGBOX_GUILD36, true);

            for (int i = 0; i < gldUps[GU_CAPTAINCY].maxLvl; i++)
            {
                sprintf(txt, DRAW_DIALOGBOX_GUILD20, i + 1);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (7 + i * 2), txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD21, gldUps[GU_CAPTAINCY].costGold[i + 1] / 1000,
                        gldUps[GU_CAPTAINCY].costMaj[i + 1] / 1000, gldUps[GU_CAPTAINCY].costCont[i + 1] / 1000);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (7 + i * 2), txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD37, i + 1);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (8 + i * 2), txt);
            }
            break;

        case 6:
            sprintf(txt, DRAW_DIALOGBOX_GUILD31, m_guild.m_upgrades[GU_RAIDMASTERS]);
            PutAlignedString(sX + cropX, sX + szX, sY + iAdjY + 45 + 15 * 0, txt);

            sprintf(txt, DRAW_DIALOGBOX_GUILD44, GetGuildRankName(GUILDRANK_RAIDMASTER), m_guild.m_raidmasters);
            PutAlignedString(sX + cropX, sX + szX, sY + iAdjY + 45 + 15 * 1, txt);

            // upgrade
            if (m_iGuildRank == GUILDRANK_MASTER)
            {
                if (onButton == 1)
                    PutAlignedString(sX + szX / 3 * 2, sX + szX, sY + iAdjY + 45 + 15 * 0, DRAW_DIALOGBOX_GUILD17, 255, 255, 255);
                else PutAlignedString(sX + szX / 3 * 2, sX + szX, sY + iAdjY + 45 + 15 * 0, DRAW_DIALOGBOX_GUILD17, 4, 0, 50);
            }

            // back
            if (onButton == 2)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * 1, DRAW_DIALOGBOX_GUILD15, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * 1, DRAW_DIALOGBOX_GUILD15, 4, 0, 50);

            PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 2, DRAW_DIALOGBOX_GUILD40, true);

            for (int i = 0; i < gldUps[GU_RAIDMASTERS].maxLvl; i++)
            {
                sprintf(txt, DRAW_DIALOGBOX_GUILD20, i + 1);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (8 + i * 2), txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD21, gldUps[GU_RAIDMASTERS].costGold[i + 1] / 1000,
                        gldUps[GU_RAIDMASTERS].costMaj[i + 1] / 1000, gldUps[GU_RAIDMASTERS].costCont[i + 1] / 1000);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (8 + i * 2), txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD38, i + 1);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (9 + i * 2), txt);
            }
            break;

        case 7:
            sprintf(txt, DRAW_DIALOGBOX_GUILD32, m_guild.m_upgrades[GU_HUNTMASTERS]);
            PutAlignedString(sX + cropX, sX + szX, sY + iAdjY + 45 + 15 * 0, txt);

            sprintf(txt, DRAW_DIALOGBOX_GUILD44, GetGuildRankName(GUILDRANK_HUNTMASTER), m_guild.m_huntmasters);
            PutAlignedString(sX + cropX, sX + szX, sY + iAdjY + 45 + 15 * 1, txt);

            // upgrade
            if (m_iGuildRank == GUILDRANK_MASTER)
            {
                if (onButton == 1)
                    PutAlignedString(sX + szX / 3 * 2, sX + szX, sY + iAdjY + 45 + 15 * 0, DRAW_DIALOGBOX_GUILD17, 255, 255, 255);
                else PutAlignedString(sX + szX / 3 * 2, sX + szX, sY + iAdjY + 45 + 15 * 0, DRAW_DIALOGBOX_GUILD17, 4, 0, 50);
            }

            // back
            if (onButton == 2)
                PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * 1, DRAW_DIALOGBOX_GUILD15, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + iAdjY + 45 + 15 * 1, DRAW_DIALOGBOX_GUILD15, 4, 0, 50);

            PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * 2, DRAW_DIALOGBOX_GUILD41, true);

            for (int i = 0; i < gldUps[GU_HUNTMASTERS].maxLvl; i++)
            {
                sprintf(txt, DRAW_DIALOGBOX_GUILD20, i + 1);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (8 + i * 2), txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD21, gldUps[GU_HUNTMASTERS].costGold[i + 1] / 1000,
                        gldUps[GU_HUNTMASTERS].costMaj[i + 1] / 1000, gldUps[GU_HUNTMASTERS].costCont[i + 1] / 1000);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (8 + i * 2), txt);

                sprintf(txt, DRAW_DIALOGBOX_GUILD39, i + 1);
                PutAlignedString(sX + cropX, sX + szX - cropX, sY + iAdjY + 45 + 15 * (9 + i * 2), txt);
            }
            break;

        case 8:
            if (m_gldBoard.size() > 15)
            {
                Point pointerLoc = dlg.HandleScroll(15, m_gldBoard.size());

                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
            }

            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX19, GUILDBOARD_POST_COST);
            //uncomment		ButtonString(dlg, 1, G_cTxt);

            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX20, m_gldBoard.size());
            PutAlignedString(sX + 161, sX + szX, sY + 35, G_cTxt);

            PutAlignedString(sX - 20, sX + szX / 2, sY + 55, DRAW_DIALOGBOX_MAILBOX2);
            PutAlignedString(sX + 56, sX + szX, sY + 55, DRAW_DIALOGBOX_MAILBOX21);
            PutAlignedString(sX + 158, sX + szX, sY + 55, DRAW_DIALOGBOX_MAILBOX4);

            for (int i = 0; i < 15; i++)
            {
                int idx = i + dlg.sView;
                if (idx < m_gldBoard.size())
                {
                    idx = m_gldBoard.size() - 1 - idx;
                    PutAlignedString(sX + 25, sX + szX / 3 * 2, sY + 75 + 15 * i, m_gldBoard[idx]->title.c_str(), (onButton == i + 4));
                    PutAlignedString(sX + 56, sX + szX, sY + 75 + 15 * i, m_gldBoard[idx]->sender.c_str(), (onButton == i + 4));
                    wsprintfA(G_cTxt, "%.2u/%.2u", m_gldBoard[idx]->date.day, m_gldBoard[idx]->date.month);
                    PutAlignedString(sX + 158, sX + szX, sY + 75 + 15 * i, G_cTxt, (onButton == i + 4));
                }
            }

            //uncomment		ButtonString(dlg, 3, DRAW_DIALOGBOX_MAILBOX16);
            break;

        case 9:
            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX12, m_gldBoard[dlg.sV1]->sender.c_str());
            //PutLabel(dlg.GetButton(1), G_cTxt, POS_LEFT);

            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX17, m_gldBoard[dlg.sV1]->title.c_str());
            //PutLabel(dlg.GetButton(2), G_cTxt, POS_LEFT);

            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX13, strlen(dlg.txt), MAX_MAIL_MSG_LENGTH - 1);
            //PutLabel(dlg.GetButton(3), G_cTxt, POS_LEFT);

            //if(m_gldBoard[dlg.sV1]->msg)
            //	PutAlignedString(dlg.GetButton(3).left, dlg.GetButton(3).right, dlg.GetButton(3).top + 15, m_gldBoard[dlg.sV1]->msg->c_str(), false, true);

            //uncomment		if(m_iGuildRank == GUILDRANK_MASTER || m_iGuildRank == GUILDRANK_CAPTAIN)
            //uncomment			ButtonString(dlg, 4, DRAW_DIALOGBOX_MAILBOX15);

            //uncomment		ButtonString(dlg, 5, DRAW_DIALOGBOX_MAILBOX16);
            break;

        case 10:
            //uncomment		ButtonString(dlg, 1, DRAW_DIALOGBOX_MAILBOX6, POS_LEFT);
            PutAlignedString(dlg.GetButton(1).left + 30, dlg.GetButton(1).right, dlg.GetButton(1).top - 1, dlg.cStr2, false);

            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX7, strlen(dlg.txt), MAX_MAIL_MSG_LENGTH - 1);
            //uncomment		ButtonString(dlg, 2, G_cTxt, POS_LEFT);
            //PutAlignedString(dlg.GetButton(2).left, dlg.GetButton(2).right, dlg.GetButton(2).top + 15, dlg.txt, false, true);

            //uncomment		if(!dlg.bFlag)
            //uncomment			ButtonString(dlg, 3, DRAW_DIALOGBOX_MAILBOX22);
            //uncomment		else
            //uncomment			ButtonString(dlg, 3, DRAW_DIALOGBOX_MAILBOX23);

            //uncomment		ButtonString(dlg, 4, DRAW_DIALOGBOX_MAILBOX11);
            break;
    }
}

void CGame::DrawDialogBox_GuildQuery()
{
    char txt[120];

    char onButton = m_dialogBoxes[44].OnButton();
    short x = m_dialogBoxes[44].m_X;
    short y = m_dialogBoxes[44].m_Y;
    short width = m_dialogBoxes[44].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, x, y, 6);

    switch (m_dialogBoxes[44].GetMode())
    {
        case 0:
            wsprintfA(txt, DRAW_DIALOGBOX_GUILDQUERY, m_dialogBoxes[44].cStr);
            PutAlignedString(x, x + width, y + 10, txt, 45, 25, 25);

            if (m_iGuildRank < GUILDRANK_CAPTAIN)
            {
                sprintf(txt, DRAW_DIALOGBOX_GUILDQUERY2, GetGuildRankName(GUILDRANK_CAPTAIN),
                        m_guild.m_upgrades[GU_CAPTAINCY] - m_guild.m_captains);
                if (m_guild.m_upgrades[GU_CAPTAINCY] - m_guild.m_captains == 0 ||
                    m_dialogBoxes[44].sV1 == GUILDRANK_CAPTAIN)
                {
                    PutAlignedString(x, x + 135, y + 25, txt, 50, 50, 50);
                }
                else
                {
                    if (onButton == 1)
                        PutAlignedString(x, x + 135, y + 25, txt, 255, 255, 255);
                    else
                        PutAlignedString(x, x + 135, y + 25, txt);
                }
            }

            sprintf(txt, DRAW_DIALOGBOX_GUILDQUERY2, GetGuildRankName(GUILDRANK_RAIDMASTER),
                    m_guild.m_upgrades[GU_RAIDMASTERS] - m_guild.m_raidmasters);
            if (m_guild.m_upgrades[GU_RAIDMASTERS] - m_guild.m_raidmasters == 0 ||
                m_dialogBoxes[44].sV1 == GUILDRANK_RAIDMASTER)
            {
                PutAlignedString(x + 135, x + width, y + 25, txt, 50, 50, 50);
            }
            else
            {
                if (onButton == 2)
                    PutAlignedString(x + 135, x + width, y + 25, txt, 255, 255, 255);
                else
                    PutAlignedString(x + 135, x + width, y + 25, txt);
            }

            sprintf(txt, DRAW_DIALOGBOX_GUILDQUERY2, GetGuildRankName(GUILDRANK_HUNTMASTER),
                    m_guild.m_upgrades[GU_HUNTMASTERS] - m_guild.m_huntmasters);
            if (m_guild.m_upgrades[GU_HUNTMASTERS] - m_guild.m_huntmasters == 0 ||
                m_dialogBoxes[44].sV1 == GUILDRANK_HUNTMASTER)
            {
                PutAlignedString(x, x + 135, y + 40, txt, 50, 50, 50);
            }
            else
            {
                if (onButton == 3)
                    PutAlignedString(x, x + 135, y + 40, txt, 255, 255, 255);
                else
                    PutAlignedString(x, x + 135, y + 40, txt);
            }

            if (m_dialogBoxes[44].sV1 == GUILDRANK_GUILDSMAN)
                PutAlignedString(x + 135, x + width, y + 40, GetGuildRankName(GUILDRANK_GUILDSMAN), 50, 50, 50);
            else if (onButton == 4)
                PutAlignedString(x + 135, x + width, y + 40, GetGuildRankName(GUILDRANK_GUILDSMAN), 255, 255, 255);
            else
                PutAlignedString(x + 135, x + width, y + 40, GetGuildRankName(GUILDRANK_GUILDSMAN));

            if (m_dialogBoxes[44].sV1 == GUILDRANK_RECRUIT)
                PutAlignedString(x, x + 135, y + 55, GetGuildRankName(GUILDRANK_RECRUIT), 50, 50, 50);
            else if (onButton == 5)
                PutAlignedString(x, x + 135, y + 55, GetGuildRankName(GUILDRANK_RECRUIT), 255, 255, 255);
            else
                PutAlignedString(x, x + 135, y + 55, GetGuildRankName(GUILDRANK_RECRUIT));

            break;
    }
}


void CGame::DlgBoxClick_GuildQuery()
{
    char onButton = m_dialogBoxes[44].OnButton();
    switch (m_dialogBoxes[44].GetMode())
    {
        case 0:
            if (onButton == 1 &&
                m_guild.m_upgrades[GU_CAPTAINCY] - m_guild.m_captains > 0 &&
                m_dialogBoxes[44].sV1 != GUILDRANK_CAPTAIN &&
                m_iGuildRank < GUILDRANK_CAPTAIN)
            {
                bSendCommand(MSGID_GUILDSMANCHANGE, 0, 0, GUILDRANK_CAPTAIN, 0, 0, m_dialogBoxes[44].cStr);
                DisableDialogBox(44);
            }
            else if (onButton == 2 &&
                     m_guild.m_upgrades[GU_RAIDMASTERS] - m_guild.m_raidmasters > 0 &&
                     m_dialogBoxes[44].sV1 != GUILDRANK_RAIDMASTER)
            {
                bSendCommand(MSGID_GUILDSMANCHANGE, 0, 0, GUILDRANK_RAIDMASTER, 0, 0, m_dialogBoxes[44].cStr);
                DisableDialogBox(44);
            }
            else if (onButton == 3 &&
                     m_guild.m_upgrades[GU_HUNTMASTERS] - m_guild.m_huntmasters > 0 &&
                     m_dialogBoxes[44].sV1 != GUILDRANK_HUNTMASTER)
            {
                bSendCommand(MSGID_GUILDSMANCHANGE, 0, 0, GUILDRANK_HUNTMASTER, 0, 0, m_dialogBoxes[44].cStr);
                DisableDialogBox(44);
            }
            else if (onButton == 4 &&
                     m_dialogBoxes[44].sV1 != GUILDRANK_GUILDSMAN)
            {
                bSendCommand(MSGID_GUILDSMANCHANGE, 0, 0, GUILDRANK_GUILDSMAN, 0, 0, m_dialogBoxes[44].cStr);
                DisableDialogBox(44);
            }
            else if (onButton == 5 &&
                     m_dialogBoxes[44].sV1 != GUILDRANK_RECRUIT)
            {
                bSendCommand(MSGID_GUILDSMANCHANGE, 0, 0, GUILDRANK_RECRUIT, 0, 0, m_dialogBoxes[44].cStr);
                DisableDialogBox(44);
            }
            break;
    }
}
void CGame::DrawDialogBox_GuildOperation()
{
    short sX, sY;

    char onButton = m_dialogBoxes[8].OnButton();
    sX = m_dialogBoxes[8].m_X;
    sY = m_dialogBoxes[8].m_Y;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 19);

    switch (m_stGuildOpList[0].cOpMode)
    {
        case 1:
            PutAlignedString(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION1);
            PutAlignedString(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            PutAlignedString(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            PutAlignedString(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION2);
            PutAlignedString(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION3);
            PutAlignedString(sX + 24, sX + 248, sY + 120, DRAW_DIALOGBOX_GUILD_OPERATION4);
            PutAlignedString(sX + 24, sX + 248, sY + 160, DRAW_DIALOGBOX_GUILD_OPERATION5, 55, 25, 25);

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 33);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 32);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 35);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 34);
            break;

        case 2:
            PutAlignedString(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION6);
            PutAlignedString(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            PutAlignedString(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            PutAlignedString(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION7);
            PutAlignedString(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION8);
            PutAlignedString(sX + 24, sX + 248, sY + 120, DRAW_DIALOGBOX_GUILD_OPERATION9);
            PutAlignedString(sX + 24, sX + 248, sY + 160, DRAW_DIALOGBOX_GUILD_OPERATION10, 55, 25, 25);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 33);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 32);

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 35);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 34);
            break;

        case 3:
            PutAlignedString(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION11);
            PutAlignedString(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            PutAlignedString(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            PutAlignedString(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION12);
            PutAlignedString(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION13);

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 4:
            PutAlignedString(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION14);
            PutAlignedString(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            PutAlignedString(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            PutAlignedString(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION15);
            PutAlignedString(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION16);

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 5:
            PutAlignedString(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION17);
            PutAlignedString(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            PutAlignedString(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            PutAlignedString(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION18);
            PutAlignedString(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION19);
            PutAlignedString(sX + 24, sX + 248, sY + 120, DRAW_DIALOGBOX_GUILD_OPERATION20);

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 6:
            PutAlignedString(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION21);
            PutAlignedString(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            PutAlignedString(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            PutAlignedString(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION22);
            PutAlignedString(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION23);

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 7:
            PutAlignedString(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION24);
            //PutString(sX + 60, sY + 65, m_stGuildOpList[0].cName,video::SColor(255,35,35,35));
            //PutString(sX + 60, sY + 69, "____________________",video::SColor(255,0,0,0));
            PutAlignedString(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION25);
            PutAlignedString(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION26);

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;
    }
}

int  _tmp_iMCProb[] = { 0, 300, 250, 200, 150, 100, 80, 70, 60, 50, 40 };
int  _tmp_iMLevelPenalty[] = { 0,   5,   5,   8,   8,   10, 14, 28, 32, 36, 40 };

void CGame::DrawDialogBox_Magic()
{
    short sX, sY, sMagicCircle, sLevelMagic;
    int  iCPivot, i, j, iYloc, iResult, iManaCost;
    char cTxt[120], cMana[10];
    uint32_t dwTime = m_dwCurTime;
    double dV1, dV2, dV3, dV4;

    char onButton = m_dialogBoxes[3].OnButton();
    sX = m_dialogBoxes[3].m_X;
    sY = m_dialogBoxes[3].m_Y;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME1, sX, sY, 1, FALSE, m_bDialogTrans);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 7, FALSE, m_bDialogTrans);

    if (iGetTopDialogBoxIndex() == 3 && G_pGame->m_stMCursor.sZ != 0)
    {
        if (G_pGame->m_stMCursor.sZ > 0) m_dialogBoxes[3].sView--;
        if (G_pGame->m_stMCursor.sZ < 0) m_dialogBoxes[3].sView++;
        //DIRECTX m_dInput.m_sZ = 0;
    }
    if (m_dialogBoxes[3].sView < 0) m_dialogBoxes[3].sView = 9;
    if (m_dialogBoxes[3].sView > 9) m_dialogBoxes[3].sView = 0;

    //Circle
    ZeroMemory(cTxt, sizeof(cTxt));
    switch (m_dialogBoxes[3].sView)
    {
        case 0: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC1);  break;//"Circle One"
        case 1: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC2);  break;//"Circle Two"
        case 2: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC3);  break;//"Circle Three"
        case 3: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC4);  break;//"Circle Four"
        case 4: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC5);  break;//"Circle Five"
        case 5: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC6);  break;//"Circle Six"
        case 6: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC7);  break;//"Circle Seven"
        case 7: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC8);  break;//"Circle Eight"
        case 8: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC9);  break;//"Circle Nine"
        case 9: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC10); break;//"Circle Ten"
    }
    PutAlignedString(sX + 3, sX + 256, sY + 50, cTxt);
    PutAlignedString(sX + 4, sX + 257, sY + 50, cTxt);
    iCPivot = m_dialogBoxes[3].sView * 10;
    iYloc = 0;

    for (i = 0, j = 0; i < 9; i++)
    {
        if ((m_cMagicMastery[iCPivot + i] != 0) && (m_pMagicCfgList[iCPivot + i] != 0))
        {
            j++;
            wsprintfA(cTxt, "%s", m_pMagicCfgList[iCPivot + i]->m_cName);

            m_Misc.ReplaceString(cTxt, '-', ' ');
            iManaCost = iGetManaCost(iCPivot + i);
            if (iManaCost > m_iMP)
            {
                if (m_Misc.bCheckIMEString(cTxt) == false)
                {
                    PutString3(sX + 30, sY + 73 + iYloc, cTxt, video::SColor(255, 41, 16, 41));
                }
                else PutString_SprFont(sX + 30, sY + 70 + iYloc, cTxt, 5, 5, 5);
                wsprintfA(cMana, "%3d", iManaCost);
                PutString_SprFont(sX + 206, sY + 70 + iYloc, cMana, 5, 5, 5);
            }
            else
                if (onButton == j)
                {
                    if (m_Misc.bCheckIMEString(cTxt) == false)
                    {
                        PutString3(sX + 30, sY + 73 + iYloc, cTxt, video::SColor(255, 255, 255, 255));
                    }
                    else PutString_SprFont(sX + 30, sY + 70 + iYloc, cTxt, 250, 250, 250);
                    wsprintfA(cMana, "%3d", iManaCost);
                    PutString_SprFont(sX + 206, sY + 70 + iYloc, cMana, 250, 250, 250);
                }
                else
                {
                    if (m_Misc.bCheckIMEString(cTxt) == false)
                    {
                        PutString3(sX + 30, sY + 73 + iYloc, cTxt, video::SColor(255, 8, 0, 66));
                    }
                    else PutString_SprFont(sX + 30, sY + 70 + iYloc, cTxt, 1, 1, 8);
                    wsprintfA(cMana, "%3d", iManaCost);
                    PutString_SprFont(sX + 206, sY + 70 + iYloc, cMana, 1, 1, 8);
                }

            iYloc += 18;
        }

    }

    if (iYloc == 0)
    {
        PutAlignedString(sX + 3, sX + 256, sY + 100, DRAW_DIALOGBOX_MAGIC11);//"
        PutAlignedString(sX + 3, sX + 256, sY + 115, DRAW_DIALOGBOX_MAGIC12);//"
        PutAlignedString(sX + 3, sX + 256, sY + 130, DRAW_DIALOGBOX_MAGIC13);//"
        PutAlignedString(sX + 3, sX + 256, sY + 145, DRAW_DIALOGBOX_MAGIC14);//"
        PutAlignedString(sX + 3, sX + 256, sY + 160, DRAW_DIALOGBOX_MAGIC15);//"
    }

    m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 30, sY + 250, 19, dwTime);

    switch (m_dialogBoxes[3].sView)
    {
        case 0: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 30, sY + 250, 20, dwTime); break;
        case 1: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 43, sY + 250, 21, dwTime); break;
        case 2: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 61, sY + 250, 22, dwTime); break;
        case 3: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 86, sY + 250, 23, dwTime); break;
        case 4: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 106, sY + 250, 24, dwTime); break;
        case 5: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 121, sY + 250, 25, dwTime); break;
        case 6: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 142, sY + 250, 26, dwTime); break;
        case 7: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 169, sY + 250, 27, dwTime); break;
        case 8: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 202, sY + 250, 28, dwTime); break;
        case 9: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 222, sY + 250, 29, dwTime); break;
    }

    sMagicCircle = m_dialogBoxes[3].sView + 1;
    if (m_cSkillMastery[SKILL_MAGIC] == 0)
        dV1 = 1.0f;
    else dV1 = (double)m_cSkillMastery[SKILL_MAGIC];
    dV2 = (double)(dV1 / 100.0f);
    dV3 = (double)_tmp_iMCProb[sMagicCircle];
    dV1 = dV2 * dV3;
    iResult = (int)dV1;
    if (m_stat[STAT_INT] > 50) iResult += (m_stat[STAT_INT] - 50) / 2;
    sLevelMagic = (m_iLevel / 10);
    if (sMagicCircle != sLevelMagic)
    {
        if (sMagicCircle > sLevelMagic)
        {
            dV1 = (double)(m_iLevel - sLevelMagic * 10);
            dV2 = (double)abs(sMagicCircle - sLevelMagic)*_tmp_iMLevelPenalty[sMagicCircle];
            dV3 = (double)abs(sMagicCircle - sLevelMagic) * 10;
            dV4 = (dV1 / dV3)*dV2;
            iResult -= abs(abs(sMagicCircle - sLevelMagic)*_tmp_iMLevelPenalty[sMagicCircle] - (int)dV4);
        }
        else
        {
            iResult += 5 * abs(sMagicCircle - sLevelMagic);
        }
    }

    switch (m_weather)
    {
        case WEATHER_LIGHTRAIN:		iResult = iResult - (iResult / 24); break;
        case WEATHER_MEDIUMRAIN:	iResult = iResult - (iResult / 12); break;
        case WEATHER_HEAVYRAIN:		iResult = iResult - (iResult / 5);  break;
    }

    for (i = 0; i<MAXITEMS; i++)
    {
        if (!m_pItemList[i])
            continue;

        if (m_bIsItemEquipped[i])
        {
            if ((m_pItemList[i]->m_dwAttribute & 0x00F00000) >> 20 == 10)
            {
                dV1 = (double)iResult;
                dV2 = (double)(((m_pItemList[i]->m_dwAttribute & 0x000F0000) >> 16) * 3);
                dV3 = dV1 + dV2;
                iResult = (int)dV3;
                break;
            }
        }
    }

    if (iResult > 100) iResult = 100;
    if (m_iSP < 1) iResult = iResult * 9 / 10;
    if (iResult < 1) iResult = 1;

    ZeroMemory(cTxt, sizeof(cTxt));
    wsprintfA(cTxt, DRAW_DIALOGBOX_MAGIC16, iResult);//"
    PutAlignedString(sX, sX + 256, sY + 267, cTxt);
    PutAlignedString(sX + 1, sX + 257, sY + 267, cTxt);


    if (onButton == 20)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + 285, 49, false, m_bDialogTrans);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + 285, 48, false, m_bDialogTrans);
}

void CGame::DrawDialogBox_MagicShop()
{
    short sX, sY;
    uint32_t dwTime = m_dwCurTime;
    int  i, j;
    int  iCPivot, iYloc;
    char cTxt[120], cMana[10];

    char onButton = m_dialogBoxes[16].OnButton();
    sX = m_dialogBoxes[16].m_X;
    sY = m_dialogBoxes[16].m_Y;

    switch (m_dialogBoxes[16].GetMode())
    {
        case 1:
            //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, sX, sY, 1);
            //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 14);

            PutString_SprFont(sX + 45, sY + 40, DRAW_MANASAVEINFO, 5, 5, 5);

            PutString3(sX + 50, sY + 55, DRAW_MANASAVEINFO1, video::SColor(255, 45, 25, 25));
            PutString(sX + 50, sY + 70, DRAW_MANASAVEINFO2, video::SColor(255, 255, 255, 255));
            PutString(sX + 50, sY + 80, DRAW_MANASAVEINFO3, video::SColor(255, 255, 255, 255));
            //PutString(sX + 50, sY + 90, DRAW_MANASAVEINFO4,video::SColor(255,255, 255, 255));

            PutString3(sX + 50, sY + 105, DRAW_MANASAVEINFO5, video::SColor(255, 45, 25, 25));
            PutString(sX + 50, sY + 120, DRAW_MANASAVEINFO6, video::SColor(255, 255, 255, 255));
            PutString(sX + 50, sY + 130, DRAW_MANASAVEINFO7, video::SColor(255, 255, 255, 255));
            //PutString(sX + 50, sY + 140, DRAW_MANASAVEINFO8,video::SColor(255,255, 255, 255));

            PutString3(sX + 50, sY + 155, DRAW_MANASAVEINFO9, video::SColor(255, 45, 25, 25));
            PutString(sX + 50, sY + 170, DRAW_MANASAVEINFO10, video::SColor(255, 255, 255, 255));

            PutString3(sX + 50, sY + 185, DRAW_MANASAVEINFO11, video::SColor(255, 45, 25, 25));
            PutString(sX + 50, sY + 200, DRAW_MANASAVEINFO12, video::SColor(255, 255, 255, 255));
            PutString(sX + 50, sY + 210, DRAW_MANASAVEINFO13, video::SColor(255, 255, 255, 255));
            PutString(sX + 50, sY + 220, DRAW_MANASAVEINFO14, video::SColor(255, 255, 255, 255));
            PutString(sX + 50, sY + 230, DRAW_MANASAVEINFO15, video::SColor(255, 255, 255, 255));
            PutString(sX + 50, sY + 240, DRAW_MANASAVEINFO16, video::SColor(255, 255, 255, 255));
            PutString(sX + 50, sY + 250, DRAW_MANASAVEINFO17, video::SColor(255, 255, 255, 255));

            PutString3(sX + 50, sY + 265, DRAW_MANASAVEINFO18, video::SColor(255, 45, 25, 25));
            PutString(sX + 50, sY + 280, DRAW_MANASAVEINFO19, video::SColor(255, 255, 255, 255));
            //PutString3(sX + 50, sY + 245, DRAW_MANASAVEINFO16,video::SColor(255,45,25,25));
            //PutString(sX + 50, sY + 260, DRAW_MANASAVEINFO17,video::SColor(255,255, 255, 255));
            //PutString(sX + 50, sY + 270, DRAW_MANASAVEINFO18,video::SColor(255,255, 255, 255));
            //PutString(sX + 50, sY + 280, DRAW_MANASAVEINFO19,video::SColor(255,255, 255, 255));

            break;

        default:
            //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, sX, sY, 1);
            //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 14);

            if (iGetTopDialogBoxIndex() == 16 && G_pGame->m_stMCursor.sZ != 0)
            {
                if (G_pGame->m_stMCursor.sZ > 0) m_dialogBoxes[16].sView--;
                if (G_pGame->m_stMCursor.sZ < 0) m_dialogBoxes[16].sView++;
                //DIRECTX m_dInput.m_sZ = 0;
            }
            if (m_dialogBoxes[16].sView < 0) m_dialogBoxes[16].sView = 9;
            if (m_dialogBoxes[16].sView > 9) m_dialogBoxes[16].sView = 0;

            PutString3(sX + 20, sY - 35 + 90, DRAW_DIALOGBOX_MAGICSHOP11, video::SColor(255, 45, 25, 25));

            PutString3(sX + 155, sY - 35 + 90, DRAW_DIALOGBOX_MAGICSHOP16, video::SColor(255, 45, 25, 25));

            PutString3(sX + 225, sY - 35 + 90, DRAW_DIALOGBOX_MAGICSHOP12, video::SColor(255, 45, 25, 25));

            PutString3(sX + 255, sY - 35 + 90, DRAW_DIALOGBOX_MAGICSHOP13, video::SColor(255, 45, 25, 25));

            iCPivot = m_dialogBoxes[16].sView * 10;

            iYloc = 0;
            for (i = 0, j = 0; i < 9; i++)
            {
                if ((m_pMagicCfgList[iCPivot + i] != 0) && (m_pMagicCfgList[iCPivot + i]->m_bIsVisible))
                {
                    j++;
                    wsprintfA(cTxt, "%s", m_pMagicCfgList[iCPivot + i]->m_cName);
                    m_Misc.ReplaceString(cTxt, '-', ' ');
                    if (m_cMagicMastery[iCPivot + i] != 0)
                    {
                        if (m_Misc.bCheckIMEString(cTxt) == false)
                        {
                            PutString3(sX + 20, sY + 73 + iYloc, cTxt, video::SColor(255, 41, 16, 41));
                        }
                        else PutString_SprFont(sX + 20, sY + 70 + iYloc, cTxt, 5, 5, 5);
                        wsprintfA(cMana, "%3d", m_pMagicCfgList[iCPivot + i]->m_sValue1);
                        PutString_SprFont(sX + 190, sY + 70 + iYloc, cMana, 5, 5, 5);
                        wsprintfA(cMana, "%3d", m_pMagicCfgList[iCPivot + i]->m_sValue2);
                        PutString_SprFont(sX + 220, sY + 70 + iYloc, cMana, 5, 5, 5);
                        wsprintfA(cMana, "%3d", m_pMagicCfgList[iCPivot + i]->m_sValue3);
                        PutString_SprFont(sX + 250, sY + 70 + iYloc, cMana, 5, 5, 5);
                    }
                    else
                        if (onButton == j)
                        {
                            if (m_Misc.bCheckIMEString(cTxt) == false)
                            {
                                PutString3(sX + 20, sY + 73 + iYloc, cTxt, video::SColor(255, 255, 255, 255));
                            }
                            else PutString_SprFont(sX + 20, sY + 70 + iYloc, cTxt, 250, 250, 250);
                            wsprintfA(cMana, "%3d", m_pMagicCfgList[iCPivot + i]->m_sValue1);
                            PutString_SprFont(sX + 190, sY + 70 + iYloc, cMana, 250, 250, 250);
                            wsprintfA(cMana, "%3d", m_pMagicCfgList[iCPivot + i]->m_sValue2);
                            PutString_SprFont(sX + 220, sY + 70 + iYloc, cMana, 250, 250, 250);
                            wsprintfA(cMana, "%3d", m_pMagicCfgList[iCPivot + i]->m_sValue3);
                            PutString_SprFont(sX + 250, sY + 70 + iYloc, cMana, 250, 250, 250);
                        }
                        else
                        {
                            if (m_Misc.bCheckIMEString(cTxt) == false)
                            {
                                PutString3(sX + 20, sY + 73 + iYloc, cTxt, video::SColor(255, 8, 0, 66));
                            }
                            else PutString_SprFont(sX + 20, sY + 70 + iYloc, cTxt, 1, 1, 8);
                            wsprintfA(cMana, "%3d", m_pMagicCfgList[iCPivot + i]->m_sValue1);
                            PutString_SprFont(sX + 190, sY + 70 + iYloc, cMana, 1, 1, 8);
                            wsprintfA(cMana, "%3d", m_pMagicCfgList[iCPivot + i]->m_sValue2);
                            PutString_SprFont(sX + 220, sY + 70 + iYloc, cMana, 1, 1, 8);
                            wsprintfA(cMana, "%3d", m_pMagicCfgList[iCPivot + i]->m_sValue3);
                            PutString_SprFont(sX + 250, sY + 70 + iYloc, cMana, 1, 1, 8);
                        }
                    iYloc += 18;
                }
            }
            m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX + 55, sY + 250, 19, dwTime);

            switch (m_dialogBoxes[16].sView)
            {
                case 0: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX - 20 + 44 + 31, sY + 250, 20, dwTime); break;
                case 1: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX - 20 + 57 + 31, sY + 250, 21, dwTime); break;
                case 2: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX - 20 + 75 + 31, sY + 250, 22, dwTime); break;
                case 3: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX - 20 + 100 + 31, sY + 250, 23, dwTime); break;
                case 4: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX - 20 + 120 + 31, sY + 250, 24, dwTime); break;
                case 5: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX - 20 + 135 + 31, sY + 250, 25, dwTime); break;
                case 6: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX - 20 + 156 + 31, sY + 250, 26, dwTime); break;
                case 7: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX - 20 + 183 + 31, sY + 250, 27, dwTime); break;
                case 8: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX - 20 + 216 + 31, sY + 250, 28, dwTime); break;
                case 9: m_pSprite[SPRID_INTERFACE_SPRFONTS]->PutSpriteFast(sX - 20 + 236 + 31, sY + 250, 29, dwTime); break;
            }
            PutString3(sX + 90, sY + 260, DRAW_DIALOGBOX_MAGICSHOP17, video::SColor(255, 45, 25, 25));
            PutAlignedString(sX, sX + m_dialogBoxes[16].sSizeX, sY + 275, DRAW_DIALOGBOX_MAGICSHOP14, 45, 25, 25);
            PutAlignedString(sX, sX + m_dialogBoxes[16].sSizeX + 1, sY + 275, DRAW_DIALOGBOX_MAGICSHOP14, 45, 25, 25);
            break;
    }
}

void CGame::DrawDialogBox_ShutDownMsg()
{
    short sX, sY, szX;

    char onButton = m_dialogBoxes[25].OnButton();
    sX = m_dialogBoxes[25].m_X;
    sY = m_dialogBoxes[25].m_Y;
    szX = m_dialogBoxes[25].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, sX, sY,2);

    switch (m_dialogBoxes[25].GetMode())
    {
        case 1:
            ZeroMemory(G_cTxt, sizeof(G_cTxt));
            if (m_dialogBoxes[25].sV1 != 0) wsprintfA(G_cTxt, DRAW_DIALOGBOX_NOTICEMSG1, m_dialogBoxes[25].sV1);
            else strcpy(G_cTxt, DRAW_DIALOGBOX_NOTICEMSG2);
            PutAlignedString(sX, sX + szX, sY + 31, G_cTxt, 100, 10, 10);
            PutAlignedString(sX, sX + szX, sY + 48, DRAW_DIALOGBOX_NOTICEMSG3);
            PutAlignedString(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_NOTICEMSG4);

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 210, sY + 127, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 210, sY + 127, 0);
            break;

        case 2:
            PutAlignedString(sX, sX + szX, sY + 31, DRAW_DIALOGBOX_NOTICEMSG7, 100, 10, 10);
            PutAlignedString(sX, sX + szX, sY + 48, DRAW_DIALOGBOX_NOTICEMSG8);
            PutAlignedString(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_NOTICEMSG9);
            PutAlignedString(sX, sX + szX, sY + 82, DRAW_DIALOGBOX_NOTICEMSG10);
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 210, sY + 127, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 210, sY + 127, 0);
            break;
    }
}

void CGame::DrawDialogBox_NpcActionQuery()
{
    short sX, sY, szX;

    char cTxt[120], cTxt2[120], cStr1[64], cStr2[64], cStr3[64];

    char onButton = m_dialogBoxes[20].OnButton();

    ZeroMemory(cStr1, sizeof(cStr1));
    ZeroMemory(cStr2, sizeof(cStr2));
    ZeroMemory(cStr3, sizeof(cStr3));

    sX = m_dialogBoxes[20].m_X;
    sY = m_dialogBoxes[20].m_Y;
    szX = m_dialogBoxes[20].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 5); //5
    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 6); //6

    switch (m_dialogBoxes[20].GetMode())
    {
        case 0: // npc
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 5);
            switch (m_dialogBoxes[20].sV3)
            {
                case 15:
                    PutString(sX + 33, sY + 23, NPC_NAME_SHOP_KEEPER, video::SColor(255, 45, 25, 25));
                    PutString(sX + 33 - 1, sY + 23 - 1, NPC_NAME_SHOP_KEEPER, video::SColor(255, 255, 255, 255));
                    break;
                case 19:
                    PutString(sX + 33, sY + 23, NPC_NAME_MAGICIAN, video::SColor(255, 45, 25, 25));
                    PutString(sX + 33 - 1, sY + 23 - 1, NPC_NAME_MAGICIAN, video::SColor(255, 255, 255, 255));
                    break;
                case 20:
                    PutString(sX + 33, sY + 23, NPC_NAME_WAREHOUSE_KEEPER, video::SColor(255, 45, 25, 25));
                    PutString(sX + 33 - 1, sY + 23 - 1, NPC_NAME_WAREHOUSE_KEEPER, video::SColor(255, 255, 255, 255));
                    break;
                case 24:
                    PutString(sX + 33, sY + 23, NPC_NAME_BLACKSMITH_KEEPER, video::SColor(255, 45, 25, 25));
                    PutString(sX + 33 - 1, sY + 23 - 1, NPC_NAME_BLACKSMITH_KEEPER, video::SColor(255, 255, 255, 255));
                    break;
                case 25:
                    PutString(sX + 33, sY + 23, NPC_NAME_CITYHALL_OFFICER, video::SColor(255, 45, 25, 25));
                    PutString(sX + 33 - 1, sY + 23 - 1, NPC_NAME_CITYHALL_OFFICER, video::SColor(255, 255, 255, 255));
                    break;
                case 26: // "Guildhall Officer"
                    PutString(sX + 33, sY + 23, NPC_NAME_GUILDHALL_OFFICER, video::SColor(255, 45, 25, 25));
                    PutString(sX + 33 - 1, sY + 23 - 1, NPC_NAME_GUILDHALL_OFFICER, video::SColor(255, 255, 255, 255));
                    break;
                case 90: //Gail
                    PutString(sX + 33, sY + 23, NPC_NAME_CMDHALL_OFFICER, video::SColor(255, 45, 25, 25));
                    PutString(sX + 33 - 1, sY + 23 - 1, NPC_NAME_CMDHALL_OFFICER, video::SColor(255, 255, 255, 255));
                    break;
            }

            if (m_dialogBoxes[20].sV3 == 25)
            {   // OFFER
                if (onButton == 1)
                {
                    PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY13, video::SColor(255, 255, 255, 255)); // "Offer"
                }
                else
                {
                    PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY13, video::SColor(255, 4, 0, 50));
                }
            }
            else if (m_dialogBoxes[20].sV3 == 20)
            {   // WITHDRAW
                if (onButton == 1)
                {
                    PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY17, video::SColor(255, 255, 255, 255)); // "Withdraw"
                }
                else
                {
                    PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY17, video::SColor(255, 4, 0, 50));
                }
            }
            else if (m_dialogBoxes[20].sV3 == 19) // CLEROTH: MAGICIAN
            {   // LEARN
                if (onButton == 1)
                {
                    PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY19, video::SColor(255, 255, 255, 255)); // "Learn"
                }
                else
                {
                    PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY19, video::SColor(255, 4, 0, 50));
                }
            }
            else
            {   // TRADE
                if (onButton == 1)
                {
                    PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, video::SColor(255, 255, 255, 255)); // "Trade"
                }
                else
                {
                    PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, video::SColor(255, 4, 0, 50));
                }
            }

            if (m_bIsDialogEnabled[21] == false)
            {
                if (onButton == 2)
                {
                    PutString3(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, video::SColor(255, 255, 255, 255)); // "Talk"
                }
                else
                {
                    PutString3(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, video::SColor(255, 4, 0, 50));
                }
            }
            break;
            ZeroMemory(_tmp_cName, sizeof(_tmp_cName));
        case 1: // Other char
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 6);
            GetItemName(m_pItemList[m_dialogBoxes[20].sV1]->m_cName, m_pItemList[m_dialogBoxes[20].sV1]->m_dwAttribute, cStr1, cStr2, cStr3);
            wsprintfA(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY29, m_dialogBoxes[20].sV3, cStr1); //"%d %s to"
                                                                                             //wsprintfA(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY29_2, m_dialogBoxes[20].sV3, cStr1, m_dialogBoxes[20].cStr); //"%d %s to %s" // Added other player name on give! xRisenx
            wsprintfA(cTxt2, DRAW_DIALOGBOX_NPCACTION_QUERY29_1, m_dialogBoxes[20].cStr); // "%s"

            PutString(sX + 24, sY + 25, cTxt, video::SColor(255, 45, 25, 25));
            PutString(sX + 24, sY + 40, cTxt2, video::SColor(255, 45, 25, 25));

            if (onButton == 1)
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY30, video::SColor(255, 255, 255, 255));//"Give"
            }
            else
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY30, video::SColor(255, 4, 0, 50));
            }

            if (onButton == 2)
            {
                PutString3(sX + 155, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY34, video::SColor(255, 255, 255, 255));//"Exchange"
            }
            else
            {
                PutString3(sX + 155, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY34, video::SColor(255, 4, 0, 50));
            }
            break;

        case 2: // Shop / BS
                //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 5);
            GetItemName(m_pItemList[m_dialogBoxes[20].sV1]->m_cName, m_pItemList[m_dialogBoxes[20].sV1]->m_dwAttribute, cStr1, cStr2, cStr3);

            wsprintfA(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY29, m_dialogBoxes[20].sV3, cStr1);//"%d %s to"
            wsprintfA(cTxt2, DRAW_DIALOGBOX_NPCACTION_QUERY29_1, m_dialogBoxes[20].cStr);//"%s"

            PutString(sX + 24, sY + 20, cTxt, video::SColor(255, 45, 25, 25));
            PutString(sX + 24, sY + 35, cTxt2, video::SColor(255, 45, 25, 25));

            if (onButton == 1)
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, video::SColor(255, 255, 255, 255));//"Sell"
            }
            else
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, video::SColor(255, 4, 0, 50));
            }

            if ((m_pItemList[m_dialogBoxes[20].sV1]->m_cItemType != ITEMTYPE_CONSUME) &&
                (m_pItemList[m_dialogBoxes[20].sV1]->m_cItemType != ITEMTYPE_ARROW))
            {
                if (onButton == 2)
                {
                    PutString3(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY43, video::SColor(255, 255, 255, 255));//"Repair"
                }
                else
                {
                    PutString3(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY43, video::SColor(255, 4, 0, 50));
                }
            }
            break;

        case 3: // WH
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 6);
            GetItemName(m_pItemList[m_dialogBoxes[20].sV1]->m_cName, m_pItemList[m_dialogBoxes[20].sV1]->m_dwAttribute, cStr1, cStr2, cStr3);

            wsprintfA(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY29, m_dialogBoxes[20].sV3, cStr1);//"%d %s to"
            wsprintfA(cTxt2, DRAW_DIALOGBOX_NPCACTION_QUERY29_1, m_dialogBoxes[20].cStr);//"%s"

            PutAlignedString(sX, sX + 240, sY + 20, cTxt, 45, 25, 25);
            PutAlignedString(sX, sX + 240, sY + 35, cTxt2, 45, 25, 25);

            if (onButton == 1)
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, video::SColor(255, 255, 255, 255));//"Deposit"
            }
            else
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, video::SColor(255, 4, 0, 50));
            }
            break;

        case 4: // talk to npc or unicorn
                //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 5);
            switch (m_dialogBoxes[20].sV3)
            {
                case 21:
                    PutString(sX + 35, sY + 25, NPC_NAME_GUARD, video::SColor(255, 45, 25, 25));//
                    PutString(sX + 35 - 1, sY + 25 - 1, NPC_NAME_GUARD, video::SColor(255, 255, 255, 255));
                    break;
                case 32:
                    PutString(sX + 35, sY + 25, NPC_NAME_UNICORN, video::SColor(255, 45, 25, 25));//"
                    PutString(sX + 35 - 1, sY + 25 - 1, NPC_NAME_UNICORN, video::SColor(255, 255, 255, 255));//"
                    break;
                case 67:
                    PutString(sX + 35, sY + 25, NPC_NAME_MCGAFFIN, video::SColor(255, 45, 25, 25));
                    PutString(sX + 35 - 1, sY + 25 - 1, NPC_NAME_MCGAFFIN, video::SColor(255, 255, 255, 255));
                    break;
                case 68:
                    PutString(sX + 35, sY + 25, NPC_NAME_PERRY, video::SColor(255, 45, 25, 25));
                    PutString(sX + 35 - 1, sY + 25 - 1, NPC_NAME_PERRY, video::SColor(255, 255, 255, 255));
                    break;
                case 69:
                    PutString(sX + 35, sY + 25, NPC_NAME_DEVLIN, video::SColor(255, 45, 25, 25));
                    PutString(sX + 35 - 1, sY + 25 - 1, NPC_NAME_DEVLIN, video::SColor(255, 255, 255, 255));
                    break;

            }

            if (m_bIsDialogEnabled[21] == false)
            {
                if (onButton == 1)
                {
                    PutString3(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, video::SColor(255, 255, 255, 255));
                }
                else
                {
                    PutString3(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, video::SColor(255, 4, 0, 50));
                }
            }
            break;

        case 5: // Shop / BS
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 6);
            switch (m_dialogBoxes[20].sV3)
            {
                case 15:
                    PutString(sX + 33, sY + 23, NPC_NAME_SHOP_KEEPER, video::SColor(255, 45, 25, 25));//Shop Keeper"
                    PutString(sX + 33 - 1, sY + 23 - 1, NPC_NAME_SHOP_KEEPER, video::SColor(255, 255, 255, 255));
                    break;
                case 24:
                    PutString(sX + 33, sY + 23, NPC_NAME_BLACKSMITH_KEEPER, video::SColor(255, 45, 25, 25));//"BlackSmith Keeper"
                    PutString(sX + 33 - 1, sY + 23 - 1, NPC_NAME_BLACKSMITH_KEEPER, video::SColor(255, 255, 255, 255));//"
                    break;
            }

            if (onButton == 1)
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, video::SColor(255, 255, 255, 255));//"Trade"
            }
            else
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, video::SColor(255, 4, 0, 50));
            }

            if (onButton == 2)
            {
                PutString3(sX + 28 + 75, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, video::SColor(255, 255, 255, 255));//"Sell"
            }
            else
            {
                PutString3(sX + 28 + 75, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, video::SColor(255, 4, 0, 50));
            }
            if (m_bIsDialogEnabled[21] == false)
            {
                if (onButton == 3)
                {
                    PutString3(sX + 155, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, video::SColor(255, 255, 255, 255));//"Talk"
                }
                else
                {
                    PutString3(sX + 155, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, video::SColor(255, 4, 0, 50));
                }
            }
            break;

        case 6: //Gail
                //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 5);
            if (onButton == 1)
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, video::SColor(255, 255, 255, 255)); // "Trade"
            }
            else
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, video::SColor(255, 4, 0, 50));
            }
            PutString(sX + 33, sY + 23, DRAW_DIALOGBOX_NPCACTION_QUERY49, video::SColor(255, 45, 25, 25));
            PutString(sX + 33 - 1, sY + 23 - 1, DRAW_DIALOGBOX_NPCACTION_QUERY49, video::SColor(255, 255, 255, 255));
            break;

        case 7: // Jehovah - New Blacksmith Function
                //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 5);
            if (onButton == 1)
            {
                PutString3(sX + 25, sY + 29, DRAW_BLACKSMITH1, video::SColor(255, 255, 255, 255)); // "Buy Weapons"
            }
            else
            {
                PutString3(sX + 25, sY + 29, DRAW_BLACKSMITH1, video::SColor(255, 4, 0, 50));
            }

            if (onButton == 2)
            {
                PutString3(sX + 125, sY + 29, DRAW_BLACKSMITH2, video::SColor(255, 255, 255, 255)); // "Buy (M) Armor"
            }
            else
            {
                PutString3(sX + 125, sY + 29, DRAW_BLACKSMITH2, video::SColor(255, 4, 0, 50));
            }

            if (onButton == 3)
            {
                PutString3(sX + 125, sY + 46, DRAW_BLACKSMITH3, video::SColor(255, 255, 255, 255)); // "Buy (W) Armor"
            }
            else
            {
                PutString3(sX + 125, sY + 46, DRAW_BLACKSMITH3, video::SColor(255, 4, 0, 50));
            }

            if (onButton == 4)
            {
                PutString3(sX + 25, sY + 46, DRAW_BLACKSMITH4, video::SColor(255, 255, 255, 255)); // "Sell"
            }
            else
            {
                PutString3(sX + 25, sY + 46, DRAW_BLACKSMITH4, video::SColor(255, 4, 0, 50));
            }

            if (onButton == 5)
            {
                PutString3(sX + 25, sY + 63, DRAW_BLACKSMITH5, video::SColor(255, 255, 255, 255)); // "Repair All"
            }
            else
            {
                PutString3(sX + 25, sY + 63, DRAW_BLACKSMITH5, video::SColor(255, 4, 0, 50));
            }

            PutString(sX + 65, sY + 5, DRAW_BLACKSMITH, video::SColor(255, 0, 0, 0));
            PutString(sX + 65 - 1, sY + 5 - 1, DRAW_BLACKSMITH, video::SColor(255, 255, 255, 255));
            PutString(sX + 61, sY + 5, "________________", video::SColor(255, 0, 0, 0));
            break;

        case 8: // Jehovah - New Shopkeeper Function
                //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 5);
            if (onButton == 1)
            {
                PutString3(sX + 25, sY + 30, DRAW_SHOP1, video::SColor(255, 255, 255, 255)); // "Buy Potions"
            }
            else
            {
                PutString3(sX + 25, sY + 30, DRAW_SHOP1, video::SColor(255, 4, 0, 50));
            }

            if (onButton == 2)
            {
                PutString3(sX + 125, sY + 30, DRAW_SHOP2, video::SColor(255, 255, 255, 255)); // "Buy Misc."
            }
            else
            {
                PutString3(sX + 125, sY + 30, DRAW_SHOP2, video::SColor(255, 4, 0, 50));
            }

            if (onButton == 3)
            {
                PutString3(sX + 125, sY + 50, DRAW_SHOP3, video::SColor(255, 255, 255, 255)); // "Buy Outfits"
            }
            else
            {
                PutString3(sX + 125, sY + 50, DRAW_SHOP3, video::SColor(255, 4, 0, 50));
            }

            if (onButton == 4)
            {
                PutString3(sX + 25, sY + 50, DRAW_SHOP4, video::SColor(255, 255, 255, 255)); // "Sell Items"
            }
            else
            {
                PutString3(sX + 25, sY + 50, DRAW_SHOP4, video::SColor(255, 4, 0, 50));
            }

            PutString(sX + 70, sY + 5, DRAW_SHOP, video::SColor(255, 0, 0, 0));
            PutString(sX + 70 - 1, sY + 5 - 1, DRAW_SHOP, video::SColor(255, 255, 255, 255));
            PutString(sX + 66, sY + 5, "____________", video::SColor(255, 0, 0, 0));
            break;

        case 9: // Jehovah - New Cityhall Officer Function
            if (onButton == 1)
            {
                PutString3(sX + 25, sY + 30, DRAW_CITYHALL1, video::SColor(255, 255, 255, 255)); // "Offer"
            }
            else
            {
                PutString3(sX + 25, sY + 30, DRAW_CITYHALL1, video::SColor(255, 4, 0, 50));
            }

            if (onButton == 2)
            {
                PutString3(sX + 125, sY + 30, DRAW_CITYHALL4, video::SColor(255, 255, 255, 255)); // "Mailbox"
            }
            else
            {
                PutString3(sX + 125, sY + 30, DRAW_CITYHALL4, video::SColor(255, 4, 0, 50));
            }

            //if (onButton == 3){
            //	PutString3(sX + 125, sY + 50, DRAW_CITYHALL2,video::SColor(255,255,255,255)); // "Buy Weapons"
            //}else{
            //	PutString3(sX + 125, sY + 50, DRAW_CITYHALL2,video::SColor(255,4,0,50));
            //}

            if (onButton == 4)
            {
                PutString3(sX + 25, sY + 50, DRAW_CITYHALL3, video::SColor(255, 255, 255, 255)); // "Talk"
            }
            else
            {
                PutString3(sX + 25, sY + 50, DRAW_CITYHALL3, video::SColor(255, 4, 0, 50));
            }

            PutString(sX + 70, sY + 5, DRAW_CITYHALL, video::SColor(255, 0, 0, 0));
            PutString(sX + 70 - 1, sY + 5 - 1, DRAW_CITYHALL, video::SColor(255, 255, 255, 255));
            PutString(sX + 66, sY + 5, "____________", video::SColor(255, 0, 0, 0));
            break;

        case 10: //GuildBank
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 6);
            GetItemName(m_pItemList[m_dialogBoxes[20].sV1]->m_cName, m_pItemList[m_dialogBoxes[20].sV1]->m_dwAttribute, cStr1, cStr2, cStr3);

            wsprintfA(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY29, m_dialogBoxes[20].sV3, cStr1);//"%d %s to"
            wsprintfA(cTxt2, DRAW_DIALOGBOX_NPCACTION_QUERY29_1, m_dialogBoxes[20].cStr);//"%s"

            PutAlignedString(sX, sX + 240, sY + 20, cTxt, 45, 25, 25);
            PutAlignedString(sX, sX + 240, sY + 35, cTxt2, 45, 25, 25);

            if (onButton == 1)
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, video::SColor(255, 255, 255, 255));//"Deposit"
            }
            else
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, video::SColor(255, 4, 0, 50));
            }
            break;

        case 11: //Repair all items
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 6);

            wsprintfA(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY51, m_dialogBoxes[20].sV2); //"your items for %d gold?"

            PutAlignedString(sX, sX + 243, sY + 20, DRAW_DIALOGBOX_NPCACTION_QUERY50, 45, 25, 25); //"Would you like me to repair"
            PutAlignedString(sX, sX + 243, sY + 35, cTxt, 45, 25, 25);

            if (onButton == 1)
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY43, video::SColor(255, 255, 255, 255));//"Repair"
            }
            else
            {
                PutString3(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY43, video::SColor(255, 4, 0, 50));
            }
            break;
    }
}

void CGame::DrawDialogBox_Party()
{
    short sX, sY, szX;
    int  i, iTotalLines, iPointerLoc;
    double d1, d2, d3;
    Point pointerLoc;

    char onButton = m_dialogBoxes[32].OnButton();
    sX = m_dialogBoxes[32].m_X;
    sY = m_dialogBoxes[32].m_Y;
    szX = m_dialogBoxes[32].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 3);

    switch (m_dialogBoxes[32].GetMode())
    {
        case 0:
            if (m_iPartyStatus == 0)
            {
                if (onButton == 1)
                    PutAlignedString(sX, sX + szX, sY + 85, DRAW_DIALOGBOX_PARTY1, 255, 255, 255);//"
                else PutAlignedString(sX, sX + szX, sY + 85, DRAW_DIALOGBOX_PARTY1, 4, 0, 50);//"
            }
            else PutAlignedString(sX, sX + szX, sY + 85, DRAW_DIALOGBOX_PARTY1, 65, 65, 65);//"

            if (m_iPartyStatus != 0)
            {
                if (onButton == 2)
                    PutAlignedString(sX, sX + szX, sY + 105, DRAW_DIALOGBOX_PARTY4, 255, 255, 255);//"
                else PutAlignedString(sX, sX + szX, sY + 105, DRAW_DIALOGBOX_PARTY4, 4, 0, 50);//"
            }
            else PutAlignedString(sX, sX + szX, sY + 105, DRAW_DIALOGBOX_PARTY4, 65, 65, 65);//"

            if (m_iPartyStatus != 0)
            {
                if (onButton == 3)
                    PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 255, 255, 255);//"
                else PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 4, 0, 50);//"
            }
            else PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 65, 65, 65);//"

            if (m_partyAutoAccept)
            {
                if (onButton == 4)
                    PutAlignedString(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_PARTY57, 255, 255, 255);
                else PutAlignedString(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_PARTY57, 4, 0, 50);//"
            }
            else
            {
                if (onButton == 4)
                    PutAlignedString(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_PARTY56, 255, 255, 255);
                else PutAlignedString(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_PARTY56, 4, 0, 50);
            }

            switch (m_iPartyStatus)
            {
                case 0:
                    PutAlignedString(sX, sX + szX, sY + 175, DRAW_DIALOGBOX_PARTY10);//"
                    PutAlignedString(sX, sX + szX, sY + 190, DRAW_DIALOGBOX_PARTY11);//"
                    PutAlignedString(sX, sX + szX, sY + 205, DRAW_DIALOGBOX_PARTY12);//"
                    break;

                case 1:
                case 2:
                    PutAlignedString(sX, sX + szX, sY + 175, DRAW_DIALOGBOX_PARTY13);//"
                    PutAlignedString(sX, sX + szX, sY + 190, DRAW_DIALOGBOX_PARTY14);//"
                    PutAlignedString(sX, sX + szX, sY + 205, DRAW_DIALOGBOX_PARTY15);//"
                    break;
            }

            if (onButton == 5)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;
            break;

        case 1: //
            wsprintfA(G_cTxt, DRAW_DIALOGBOX_PARTY16, m_dialogBoxes[32].cStr);
            PutAlignedString(sX, sX + szX, sY + 95, G_cTxt);
            PutAlignedString(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY17);//"join the party. You can"
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY18);//"share the experience by"
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY19);//"the ratio of level if"
            PutAlignedString(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY20);//"you join the party."
            PutAlignedString(sX, sX + szX, sY + 175, DRAW_DIALOGBOX_PARTY21);//"Would you like to join party?"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 18);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 2);
            break;

        case 2:
            PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY22);//"Click the character which"
            PutAlignedString(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY23);//"
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY24);//"
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY25);//"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;

        case 3:
            wsprintfA(G_cTxt, DRAW_DIALOGBOX_PARTY26, m_dialogBoxes[32].cStr);//"
            PutAlignedString(sX, sX + szX, sY + 95, G_cTxt);
            PutAlignedString(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY27);//"
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY28);//"
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY29);//"
            PutAlignedString(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY30);//"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;

        case 4:
            PutAlignedString(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_PARTY31);//"
            PutAlignedString(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_PARTY32);//"

            iTotalLines = m_stPartyMember.size();
            pointerLoc = m_dialogBoxes[32].HandleScroll(12, iTotalLines);

            if (iTotalLines > 12)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
            }

            for (i = 0; i < 12; i++)
            {
                if ((i + m_dialogBoxes[32].sView) < m_stPartyMember.size())
                {
                    PutAlignedString(sX + 10, sX + 260, sY + 95 + 15 * (i), (char*)m_stPartyMember[i + m_dialogBoxes[32].sView]->cName.c_str());
                }
            }

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 5:
            PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY33);//"
            PutAlignedString(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY34);//"
            break;

        case 6:
            PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY35);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 7:
            PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY36);//"
            PutAlignedString(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY37);//"
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY38);//"
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY39);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 8:
            PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY40);//"
            PutAlignedString(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY41);//"
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY42);//"
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY43);//"
            PutAlignedString(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY44);//"
            PutAlignedString(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_PARTY45);//"


            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 9:
            PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY46);//"
            PutAlignedString(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY47);//"
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY48);//"
            PutAlignedString(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY49);//"
            PutAlignedString(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY50);//"
            PutAlignedString(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_PARTY51);//"

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 10:
            PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY52);//"
            PutAlignedString(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY53);//"
            PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY54);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 11: //
            PutAlignedString(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY55);//"
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 19);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 18);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 3);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 2);
            break;
    }
}

void CGame::DrawDialogBox_QueryDropItemAmount()
{
    short sX, sY;
    char cTxt[120], cStr1[64], cStr2[64], cStr3[64];

    sX = m_dialogBoxes[17].m_X;
    sY = m_dialogBoxes[17].m_Y;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 5);

    switch (m_dialogBoxes[17].GetMode())
    {
        case 1:
            GetItemName(m_pItemList[m_dialogBoxes[17].sView]->m_cName, m_pItemList[m_dialogBoxes[17].sView]->m_dwAttribute, cStr1, cStr2, cStr3);
            if (strlen(m_dialogBoxes[17].cStr) == 0)
                wsprintfA(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT1, cStr1);
            else wsprintfA(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT2, cStr1, m_dialogBoxes[17].cStr);//"%s:

            if (m_dialogBoxes[17].sV3 < 1000)
                PutString(sX + 30, sY + 20, cTxt, video::SColor(255, 55, 25, 25));

            PutString(sX + 30, sY + 35, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT3, video::SColor(255, 55, 25, 25));//"
            if (iGetTopDialogBoxIndex() != 17)
                PutString(sX + 40, sY + 57, m_cAmountString, video::SColor(255, 255, 255, 255), false, 2);
            wsprintfA(cTxt, "__________ (0 ~ %d)", m_pItemList[m_dialogBoxes[17].sView]->m_dwCount);
            PutString(sX + 38, sY + 62, cTxt, video::SColor(255, 25, 35, 25));
            break;

        case 20:
            GetItemName(m_pItemList[m_dialogBoxes[17].sView]->m_cName, m_pItemList[m_dialogBoxes[17].sView]->m_dwAttribute, cStr1, cStr2, cStr3);
            if (strlen(m_dialogBoxes[17].cStr) == 0)
                wsprintfA(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT1, cStr1);//"%s:
            else wsprintfA(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT2, cStr1, m_dialogBoxes[17].cStr);//"%s:

            if (m_dialogBoxes[17].sV3 < 1000)
                PutString(sX + 30, sY + 20, cTxt, video::SColor(255, 55, 25, 25));

            PutString(sX + 30, sY + 35, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT3, video::SColor(255, 55, 25, 25));//"
            PutString(sX + 40, sY + 57, m_cAmountString, video::SColor(255, 255, 255, 255), false, 2);
            wsprintfA(cTxt, "__________ (0 ~ %d)", m_pItemList[m_dialogBoxes[17].sView]->m_dwCount);
            PutString(sX + 38, sY + 62, cTxt, video::SColor(255, 25, 35, 25));
            break;

        case NPC_PRINCESS:
            GetItemName(m_pItemList[m_dialogBoxes[17].sView]->m_cName, m_pItemList[m_dialogBoxes[17].sView]->m_dwAttribute, cStr1, cStr2, cStr3);
            if (strlen(m_dialogBoxes[17].cStr) == 0)
                wsprintfA(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT1, cStr1);//"%s:
            else wsprintfA(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT2, cStr1, m_dialogBoxes[17].cStr);//"%s:

            if (m_dialogBoxes[17].sV3 < 1000)
                PutString(sX + 30, sY + 20, cTxt, video::SColor(255, 55, 25, 25));

            PutString(sX + 30, sY + 35, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT3, video::SColor(255, 55, 25, 25));//"
            PutString(sX + 40, sY + 57, m_cAmountString, video::SColor(255, 255, 255, 255), false, 2);
            wsprintfA(cTxt, "__________ (0 ~ %d)", m_pItemList[m_dialogBoxes[17].sView]->m_dwCount);
            PutString(sX + 38, sY + 62, cTxt, video::SColor(255, 25, 35, 25));
            break;
    }
}


void CGame::DrawDialogBox_Quest()
{
    short sX, sY, szX;
    char cTxt[120], cTemp[21];

    char onButton = m_dialogBoxes[28].OnButton();

    sX = m_dialogBoxes[28].m_X;
    sY = m_dialogBoxes[28].m_Y;
    szX = m_dialogBoxes[28].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT , sX, sY, 4);

    switch (m_dialogBoxes[28].GetMode())
    {
        case 1:
            switch (m_stQuest.sQuestType)
            {
                case 0:
                    PutAlignedString(sX, sX + szX, sY + 50 + 115 - 30, DRAW_DIALOGBOX_QUEST1, 55, 25, 25); // " You are not on a quest."
                    break;

                case 1:	//  Hunt
                    if (m_stQuest.bIsQuestCompleted == false)
                        PutAlignedString(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST2, 55, 25, 25); // "You are on a monster conquering quest."
                    else PutAlignedString(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST3, 55, 25, 25); // "You accomplished the monster conquering quest."

                    ZeroMemory(cTxt, sizeof(cTxt));
                    wsprintfA(cTxt, "Rest Monster : %d", m_stQuest.sCurrentCount); // Snoopy: "Rest Monster : %s"
                    PutAlignedString(sX, sX + szX, sY + 50 + 20, cTxt, 55, 25, 25);  // m_stQuest.sCurrentCount

                    ZeroMemory(cTemp, sizeof(cTemp));
                    switch (m_stQuest.sWho)
                    {
                        case 1:
                        case 2:
                        case 3: break;
                        case 4: strcpy(cTemp, NPC_NAME_CITYHALL_OFFICER); break;
                        case 5:
                        case 6:
                        case 7: break;
                    }
                    ZeroMemory(cTxt, sizeof(cTxt));
                    wsprintfA(cTxt, DRAW_DIALOGBOX_QUEST5, cTemp); // "Client: %s"
                    PutAlignedString(sX, sX + szX, sY + 50 + 45, cTxt, 55, 25, 25);

                    ZeroMemory(cTemp, sizeof(cTemp));
                    GetNpcName(m_stQuest.sTargetType, cTemp);
                    ZeroMemory(cTxt, sizeof(cTxt));
                    wsprintfA(cTxt, NPC_TALK_HANDLER16, m_stQuest.sTargetCount, cTemp);
                    PutAlignedString(sX, sX + szX, sY + 50 + 60, cTxt, 55, 25, 25);

                    ZeroMemory(cTxt, sizeof(cTxt));
                    if (memcmp(m_stQuest.cTargetName, "NONE", 4) == 0)
                    {
                        strcpy(cTxt, DRAW_DIALOGBOX_QUEST31); // "Location : Anywhere"
                        PutAlignedString(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);
                    }
                    else
                    {
                        ZeroMemory(cTemp, sizeof(cTemp));
                        GetOfficialMapName(m_stQuest.cTargetName, cTemp);
                        wsprintfA(cTxt, DRAW_DIALOGBOX_QUEST32, cTemp); // "Map : %s"
                        PutAlignedString(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);

                        if (m_stQuest.sX != 0)
                        {
                            ZeroMemory(cTxt, sizeof(cTxt));
                            wsprintfA(cTxt, DRAW_DIALOGBOX_QUEST33, m_stQuest.sX, m_stQuest.sY, m_stQuest.sRange); // "Position: %d, %d Range: %d block"
                            PutAlignedString(sX, sX + szX, sY + 50 + 90, cTxt, 55, 25, 25);
                        }
                    }

                    ZeroMemory(cTxt, sizeof(cTxt));
                    wsprintfA(cTxt, DRAW_DIALOGBOX_QUEST34, m_stQuest.sContribution); // "Contribution: %d"
                    PutAlignedString(sX, sX + szX, sY + 50 + 105, cTxt, 55, 25, 25);
                    break;

                case 7: //
                    if (m_stQuest.bIsQuestCompleted == false)
                        PutAlignedString(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST26, 55, 25, 25);
                    else PutAlignedString(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST27, 55, 25, 25);

                    ZeroMemory(cTemp, sizeof(cTemp));
                    switch (m_stQuest.sWho)
                    {
                        case 1:
                        case 2:
                        case 3: break;
                        case 4: strcpy(cTemp, NPC_NAME_CITYHALL_OFFICER); break;
                        case 5:
                        case 6:
                        case 7: break;
                    }
                    ZeroMemory(cTxt, sizeof(cTxt));
                    wsprintfA(cTxt, DRAW_DIALOGBOX_QUEST29, cTemp);
                    PutAlignedString(sX, sX + szX, sY + 50 + 45, cTxt, 55, 25, 25);

                    PutAlignedString(sX, sX + szX, sY + 50 + 60, DRAW_DIALOGBOX_QUEST30, 55, 25, 25);

                    ZeroMemory(cTxt, sizeof(cTxt));
                    if (memcmp(m_stQuest.cTargetName, "NONE", 4) == 0)
                    {
                        strcpy(cTxt, DRAW_DIALOGBOX_QUEST31);
                        PutAlignedString(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);
                    }
                    else
                    {
                        ZeroMemory(cTemp, sizeof(cTemp));
                        GetOfficialMapName(m_stQuest.cTargetName, cTemp);
                        wsprintfA(cTxt, DRAW_DIALOGBOX_QUEST32, cTemp);
                        PutAlignedString(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);

                        if (m_stQuest.sX != 0)
                        {
                            ZeroMemory(cTxt, sizeof(cTxt));
                            wsprintfA(cTxt, DRAW_DIALOGBOX_QUEST33, m_stQuest.sX, m_stQuest.sY, m_stQuest.sRange);
                            PutAlignedString(sX, sX + szX, sY + 50 + 90, cTxt, 55, 25, 25);
                        }
                    }

                    ZeroMemory(cTxt, sizeof(cTxt));
                    wsprintfA(cTxt, DRAW_DIALOGBOX_QUEST34, m_stQuest.sContribution);//" %dPoint"
                    PutAlignedString(sX, sX + szX, sY + 50 + 105, cTxt, 55, 25, 25);
                    break;
            }
            break;

        case 2:
            PutAlignedString(sX, sX + szX, sY + 50 + 115 - 30, DRAW_DIALOGBOX_QUEST35, 55, 25, 25);
            break;
    }

    if (onButton == 1)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
}

void CGame::DrawDialogBox_SellList()
{
    short sX, sY, szX;
    int  i, iItem;
    char cTemp[255], cStr1[64], cStr2[64], cStr3[64];

    char onButton = m_dialogBoxes[31].OnButton();
    sX = m_dialogBoxes[31].m_X;
    sY = m_dialogBoxes[31].m_Y;
    szX = m_dialogBoxes[31].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 11);

    iItem = 0;
    for (i = 0; i < MAXSELLLIST; i++)
        if (m_stSellItemList[i].iIndex != -1)
        {
            ZeroMemory(cTemp, sizeof(cTemp));
            GetItemName(m_pItemList[m_stSellItemList[i].iIndex]->m_cName, m_pItemList[m_stSellItemList[i].iIndex]->m_dwAttribute, cStr1, cStr2, cStr3);
            if (m_stSellItemList[i].iAmount > 1)
            {
                wsprintfA(cTemp, DRAW_DIALOGBOX_SELL_LIST1, m_stSellItemList[i].iAmount, cStr1);
                if (onButton == i + 1)
                    PutAlignedString(sX, sX + szX, sY + 55 + i * 15, cTemp, 255, 255, 255);
                else
                {
                    if (m_bIsSpecial)
                        PutAlignedString(sX, sX + szX, sY + 55 + i * 15, cTemp, 0, 255, 0);
                    else
                        PutAlignedString(sX, sX + szX, sY + 55 + i * 15, cTemp, 45, 25, 25);
                }
            }
            else
            {
                if (onButton == i + 1)
                {
                    if ((strlen(cStr2) == 0) && (strlen(cStr3) == 0)) PutAlignedString(sX, sX + szX, sY + 55 + i * 15, cStr1, 255, 255, 255);
                    else
                    {
                        ZeroMemory(G_cTxt, sizeof(G_cTxt));
                        if ((strlen(cStr1) + strlen(cStr2) + strlen(cStr3)) < 36)
                        {
                            if ((strlen(cStr2)>0) && (strlen(cStr3)>0)) wsprintfA(G_cTxt, "%s(%s, %s)", cStr1, cStr2, cStr3);
                            else wsprintfA(G_cTxt, "%s(%s%s)", cStr1, cStr2, cStr3);
                            PutAlignedString(sX, sX + szX, sY + 55 + i * 15, G_cTxt, 255, 255, 255);

                        }
                        else
                        {
                            if ((strlen(cStr2)>0) && (strlen(cStr3)>0)) wsprintfA(G_cTxt, "(%s, %s)", cStr2, cStr3);
                            else wsprintfA(G_cTxt, "(%s%s)", cStr2, cStr3);
                            PutAlignedString(sX, sX + szX, sY + 55 + i * 15, cStr1, 255, 255, 255);
                            PutAlignedString(sX, sX + szX, sY + 55 + i * 15 + 15, G_cTxt, 200, 200, 200);
                            i++;
                        }
                    }
                }
                else
                {
                    if ((strlen(cStr2) == 0) && (strlen(cStr3) == 0))
                    {
                        if (m_bIsSpecial)
                            PutAlignedString(sX, sX + szX, sY + 55 + i * 15, cStr1, 0, 255, 0);
                        else
                            PutAlignedString(sX, sX + szX, sY + 55 + i * 15, cStr1, 45, 25, 25);
                    }
                    else
                    {
                        ZeroMemory(G_cTxt, sizeof(G_cTxt));
                        if ((strlen(cStr1) + strlen(cStr2) + strlen(cStr3)) < 36)
                        {
                            if ((strlen(cStr2)>0) && (strlen(cStr3)>0)) wsprintfA(G_cTxt, "%s(%s, %s)", cStr1, cStr2, cStr3);
                            else wsprintfA(G_cTxt, "%s(%s%s)", cStr1, cStr2, cStr3);

                            if (m_bIsSpecial)
                                PutAlignedString(sX, sX + szX, sY + 55 + i * 15, G_cTxt, 0, 255, 0);
                            else
                                PutAlignedString(sX, sX + szX, sY + 55 + i * 15, G_cTxt, 45, 25, 25);

                        }
                        else
                        {
                            if (m_bIsSpecial)
                                PutAlignedString(sX, sX + szX, sY + 55 + i * 15, cStr1, 0, 255, 0);
                            else
                                PutAlignedString(sX, sX + szX, sY + 55 + i * 15, cStr1, 45, 25, 25);
                        }
                    }
                }
            }
        }
        else iItem++;

        if (iItem == MAXSELLLIST)
        {
            PutAlignedString(sX, sX + szX, sY + 55 + 30 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST2);//"
            PutAlignedString(sX, sX + szX, sY + 55 + 45 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST3);//"
            PutAlignedString(sX, sX + szX, sY + 55 + 60 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST4);//"
            PutAlignedString(sX, sX + szX, sY + 55 + 75 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST5);//"
            PutAlignedString(sX, sX + szX, sY + 55 + 95 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST6);//"
            PutAlignedString(sX, sX + szX, sY + 55 + 110 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST7);//"
            PutAlignedString(sX, sX + szX, sY + 55 + 125 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST8);//"
            PutAlignedString(sX, sX + szX, sY + 55 + 155 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST9);//"

        }

        if (onButton == MAXSELLLIST + 1 && (iItem < MAXSELLLIST))
            DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 39);
        else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 38);

        if (onButton == MAXSELLLIST + 2)
            DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
        else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
}

void CGame::DrawDialogBox_SellorRepairItem()
{
    short sX, sY;
    uint32_t dwTime = m_dwCurTime;
    char cItemID, cItemColor, cTxt[120], cTemp[120], cStr2[120], cStr3[120];

    char onButton = m_dialogBoxes[23].OnButton();
    sX = m_dialogBoxes[23].m_X;
    sY = m_dialogBoxes[23].m_Y;

    switch (m_dialogBoxes[23].GetMode())
    {
        case 1:
            //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
            DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 11);

            cItemID = m_dialogBoxes[23].sV1;

            cItemColor = m_pItemList[cItemID]->m_ItemColor;
            if (cItemColor == 0)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->PutSpriteFast(sX + 62 + 15, sY + 84 + 30,
                                                                                                      m_pItemList[cItemID]->m_sSpriteFrame, dwTime);
            else
            {
                switch (m_pItemList[cItemID]->m_sSprite)
                {
                    case 1: // Swds
                    case 2: // Bows
                    case 3: // Shields
                    case 15: // Axes hammers
                        m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->PutSpriteRGB(sX + 62 + 15, sY + 84 + 30
                                                                                                             , m_pItemList[cItemID]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                        break;
                    default: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->PutSpriteRGB(sX + 62 + 15, sY + 84 + 30
                                                                                                                  , m_pItemList[cItemID]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                        break;
                }
            }
            ZeroMemory(cTemp, sizeof(cTemp));
            ZeroMemory(cStr2, sizeof(cStr2));
            ZeroMemory(cStr3, sizeof(cStr3));

            GetItemName(m_pItemList[cItemID]->m_cName, m_pItemList[cItemID]->m_dwAttribute, cTemp, cStr2, cStr3);
            if (m_dialogBoxes[23].sV4 == 1) strcpy(cTxt, cTemp);
            else wsprintfA(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM1, m_dialogBoxes[23].sV4, cTemp);

            if (m_bIsSpecial)
            {
                PutAlignedString(sX + 25, sX + 240, sY + 60, cTxt, 0, 255, 0);
                PutAlignedString(sX + 25 + 1, sX + 240 + 1, sY + 60, cTxt, 0, 255, 0);
            }
            else
            {
                PutAlignedString(sX + 25, sX + 240, sY + 60, cTxt, 45, 25, 25);
                PutAlignedString(sX + 25 + 1, sX + 240 + 1, sY + 60, cTxt, 45, 25, 25);
            }

            wsprintfA(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM2, m_dialogBoxes[23].sV2);
            PutString(sX + 95 + 15, sY + 53 + 60, cTxt, video::SColor(255, 45, 25, 25));
            wsprintfA(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM3, m_dialogBoxes[23].sV3);
            PutString(sX + 95 + 15, sY + 53 + 75, cTxt, video::SColor(255, 45, 25, 25));
            PutString(sX + 55, sY + 190, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM4, video::SColor(255, 45, 25, 25));

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 39);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 38);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;

        case 2:
            //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
            DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 10);
            cItemID = m_dialogBoxes[23].sV1;
            cItemColor = m_pItemList[cItemID]->m_ItemColor;
            if (cItemColor == 0)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->PutSpriteFast(sX + 62 + 15, sY + 84 + 30,
                                                                                                      m_pItemList[cItemID]->m_sSpriteFrame, dwTime);
            else
            {
                switch (m_pItemList[cItemID]->m_sSprite)
                {
                    case 1: // Swds
                    case 2: // Bows
                    case 3: // Shields
                    case 15: // Axes hammers
                        m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->PutSpriteRGB(sX + 62 + 15, sY + 84 + 30
                                                                                                             , m_pItemList[cItemID]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                        break;

                    default: m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->PutSpriteRGB(sX + 62 + 15, sY + 84 + 30
                                                                                                                  , m_pItemList[cItemID]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                        break;
                }
            }
            ZeroMemory(cTemp, sizeof(cTemp));
            ZeroMemory(cStr2, sizeof(cStr2));
            ZeroMemory(cStr3, sizeof(cStr3));
            GetItemName(m_pItemList[cItemID], cTemp, cStr2, cStr3);
            wsprintfA(cTxt, "%s", cTemp);
            //		PutAlignedString(sX + 25, sX + 240, sY + 60, cTxt, 45,25,25);
            //		PutAlignedString(sX + 25 +1, sX + 240 +1, sY + 60, cTxt, 45,25,25);
            if (m_bIsSpecial)
            {
                PutAlignedString(sX + 25, sX + 240, sY + 60, cTxt, 0, 255, 0);
                PutAlignedString(sX + 25 + 1, sX + 240 + 1, sY + 60, cTxt, 0, 255, 0);
            }
            else
            {
                PutAlignedString(sX + 25, sX + 240, sY + 60, cTxt, 45, 25, 25);
                PutAlignedString(sX + 25 + 1, sX + 240 + 1, sY + 60, cTxt, 45, 25, 25);
            }
            wsprintfA(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM2, m_dialogBoxes[23].sV2);
            PutString(sX + 95 + 15, sY + 53 + 60, cTxt, video::SColor(255, 45, 25, 25));
            wsprintfA(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM6, m_dialogBoxes[23].sV3);
            PutString(sX + 95 + 15, sY + 53 + 75, cTxt, video::SColor(255, 45, 25, 25));
            PutString(sX + 55, sY + 190, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM7, video::SColor(255, 45, 25, 25));

            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 43);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 42);

            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;

        case 3:
            //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
            DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 11);

            PutString(sX + 55, sY + 100, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM8, video::SColor(255, 45, 25, 25));//"
            PutString(sX + 55, sY + 120, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM9, video::SColor(255, 45, 25, 25));//"
            PutString(sX + 55, sY + 135, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM10, video::SColor(255, 45, 25, 25));//"
            break;

        case 4:
            //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
            DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 10);

            PutString(sX + 55, sY + 100, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM11, video::SColor(255, 45, 25, 25));//"
            PutString(sX + 55, sY + 120, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM9, video::SColor(255, 45, 25, 25));//"
            PutString(sX + 55, sY + 135, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM10, video::SColor(255, 45, 25, 25));//"
            break;
    }
}


void CGame::DrawDialogBox_Skill()
{
    short sX, sY;
    int  iTotalLines, iPointerLoc;
    char cTemp[255], cTemp2[255];
    double d1, d2, d3;
    Point pointerLoc;

    char onButton = m_dialogBoxes[15].OnButton();
    sX = m_dialogBoxes[15].m_X;
    sY = m_dialogBoxes[15].m_Y;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 0); // Normal Dialog
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 1); // Skill Dialog Title Bar

    switch (m_dialogBoxes[15].GetMode())
    {
        case 0:
            for (int line = 0, skillIndex = 0; line < 17; skillIndex++)
                if (line < MAXSKILLTYPE && m_pSkillCfgList[skillIndex + m_dialogBoxes[15].sView])
                {
                    ZeroMemory(cTemp, sizeof(cTemp));
                    wsprintfA(cTemp, "%s", m_pSkillCfgList[skillIndex + m_dialogBoxes[15].sView]->m_cName);
                    m_Misc.ReplaceString(cTemp, '-', ' ');
                    ZeroMemory(cTemp2, sizeof(cTemp2));
                    wsprintfA(cTemp2, "%3d%%", m_pSkillCfgList[skillIndex + m_dialogBoxes[15].sView]->m_iLevel);

                    video::SColor colour = video::SColor(255, 5, 5, 5);
                    if (m_pSkillCfgList[skillIndex + m_dialogBoxes[15].sView]->m_bIsUseable
                        && m_pSkillCfgList[skillIndex + m_dialogBoxes[15].sView]->m_iLevel != 0)
                    {
                        if (onButton == line + 1)
                        {
                            colour = video::SColor(255, 255, 255, 255);
                        }
                        else
                        {
                            colour = video::SColor(255, 34, 30, 120);
                        }
                    }

                    PutString(sX + 30, sY + 45 + line * 15, cTemp, colour);
                    PutString(sX + 183, sY + 45 + line * 15, cTemp2, colour);

                    if (m_iDownSkillIndex == (skillIndex + m_dialogBoxes[15].sView))
                        m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSpriteRGB(sX + 215, sY + 47 + line * 15, 21, 50, 50, 50, m_dwTime);
                    else m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + 215, sY + 47 + line * 15, 20, m_dwTime);
                    line++;
                }

            iTotalLines = 0;
            for (int i = 0; i < MAXSKILLTYPE; i++)
                if (m_pSkillCfgList[i] != 0) iTotalLines++;

            pointerLoc = m_dialogBoxes[15].HandleScroll(17, iTotalLines);

            if (iTotalLines > 17)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 1);
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
            }

            break;
    }
}

void CGame::DrawDialogBox_SkillDlg()
{
    int i, iLoc, iAdjX, iAdjY, iAmount;
    char cTemp[120], cTemp2[120];
    short sX, sY, szX;
    char cStr1[64], cStr2[64], cStr3[64];
    uint32_t dwTime = m_dwCurTime;

    iAdjX = 5;
    iAdjY = 8;
    char onButton = m_dialogBoxes[26].OnButton();
    switch (m_dialogBoxes[26].GetMode())
    {
        case 1: // Alchemy waiting incredients
            if (m_dialogBoxes[26].cStr[0] != 0)
            {
                sX = m_dialogBoxes[26].m_X + iAdjX + (m_dialogBoxes[26].cStr[0] - (rand() % (m_dialogBoxes[26].cStr[0] * 2)));
                sY = m_dialogBoxes[26].m_Y + iAdjY + (m_dialogBoxes[26].cStr[0] - (rand() % (m_dialogBoxes[26].cStr[0] * 2)));
            }
            else
            {
                sX = m_dialogBoxes[26].m_X;
                sY = m_dialogBoxes[26].m_Y;
            }

            m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX, sY, 1, dwTime);

            if (m_dialogBoxes[26].sV1 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV1]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV1]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV2 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV2]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV2]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV3 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV3]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV3]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV4 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV4]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV4]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV5 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV5]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV5]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV6 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV6]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV6]->m_sSpriteFrame, dwTime);

            if (onButton == 1)
                PutString_SprFont(sX + iAdjX + 60, sY + iAdjY + 175, "Try Now!", 16, 16, 30);
            else PutString_SprFont(sX + iAdjX + 60, sY + iAdjY + 175, "Try Now!", 6, 6, 20);

            break;

        case 2: // Alchemy, creating a potion
            if (m_dialogBoxes[26].cStr[0] != 0)
            {
                sX = m_dialogBoxes[26].m_X + iAdjX + (m_dialogBoxes[26].cStr[0] - (rand() % (m_dialogBoxes[26].cStr[0] * 2)));
                sY = m_dialogBoxes[26].m_Y + iAdjY + (m_dialogBoxes[26].cStr[0] - (rand() % (m_dialogBoxes[26].cStr[0] * 2)));
            }
            else
            {
                sX = m_dialogBoxes[26].m_X;
                sY = m_dialogBoxes[26].m_Y;
            }
            m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX, sY, 1, dwTime);

            if (m_dialogBoxes[26].sV1 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV1]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV1]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV2 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV2]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV2]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV3 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV3]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV3]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV4 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV4]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV4]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV5 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV5]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV5]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV6 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV6]->m_sSprite]->PutTransSprite50(sX + iAdjX + 55 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                                                                                 m_pItemList[m_dialogBoxes[26].sV6]->m_sSpriteFrame, dwTime);

            PutString_SprFont(sX + iAdjX + 60, sY + iAdjY + 175, "Creating...", 20, 6, 6);


            if ((dwTime - m_dialogBoxes[26].dwT1) > 1000)
            {
                m_dialogBoxes[26].dwT1 = dwTime;
                m_dialogBoxes[26].cStr[0]++;
            }

            if (m_dialogBoxes[26].cStr[0] >= 4)
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_CREATEPOTION, 0, 0, 0, 0, 0);
                DisableDialogBox(26);
                PlaySound('E', 42, 0);
            }
            break;

        case 3: // Manuf: Choose what you want to create
            sX = m_dialogBoxes[26].m_X;
            sY = m_dialogBoxes[26].m_Y;
            szX = m_dialogBoxes[26].sSizeX;
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 0);
            DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 8);
            PutString(sX + iAdjX + 49, sY + iAdjY + 33, "Name", video::SColor(255, 0, 0, 0));
            PutString(sX + iAdjX + 171, sY + iAdjY + 33, "Max.Title", video::SColor(255, 0, 0, 0));

            iLoc = 0;
            for (i = 0; i < 13; i++)
                if (m_pDispBuildItemList[i + m_dialogBoxes[26].sView] != 0)
                {

                    ZeroMemory(cTemp, sizeof(cTemp));
                    GetItemName(m_pDispBuildItemList[i + m_dialogBoxes[26].sView]->m_cName, 0, cStr1, cStr2, cStr3);
                    wsprintfA(cTemp, "%s", cStr1);
                    ZeroMemory(cTemp2, sizeof(cTemp2));
                    wsprintfA(cTemp2, "%d%", m_pDispBuildItemList[i + m_dialogBoxes[26].sView]->m_iMaxSkill);

                    if (onButton == i + 1)
                    {
                        PutString(m_dialogBoxes[26].ButtonX(i + 1), m_dialogBoxes[26].ButtonY(i + 1), cTemp, video::SColor(255, 255, 255, 255));
                        PutString(m_dialogBoxes[26].ButtonX(i + 1) + 155, m_dialogBoxes[26].ButtonY(i + 1), cTemp2, video::SColor(255, 255, 255, 255));
                    }
                    else
                    {
                        if (m_pDispBuildItemList[i + m_dialogBoxes[26].sView]->m_bBuildEnabled == true)
                        {
                            PutString(m_dialogBoxes[26].ButtonX(i + 1), m_dialogBoxes[26].ButtonY(i + 1), cTemp, video::SColor(255, 34, 30, 120));
                            PutString(m_dialogBoxes[26].ButtonX(i + 1) + 155, m_dialogBoxes[26].ButtonY(i + 1), cTemp2, video::SColor(255, 34, 30, 120));
                        }
                        else
                        {
                            PutString(m_dialogBoxes[26].ButtonX(i + 1), m_dialogBoxes[26].ButtonY(i + 1), cTemp, video::SColor(255, 45, 25, 25));
                            PutString(m_dialogBoxes[26].ButtonX(i + 1) + 155, m_dialogBoxes[26].ButtonY(i + 1), cTemp2, video::SColor(255, 45, 25, 25));
                        }
                    }

                    iLoc++;
                }
            if ((m_dialogBoxes[26].sView >= 1) && (m_pDispBuildItemList[m_dialogBoxes[26].sView - 1] != 0))
                m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + iAdjX + 225, sY + iAdjY + 210, 23, dwTime);
            else m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutTransSpriteRGB(sX + iAdjX + 225, sY + iAdjY + 210, 23, 5, 5, 5, dwTime);

            if (m_pDispBuildItemList[m_dialogBoxes[26].sView + 13] != 0)
                m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + iAdjX + 225, sY + iAdjY + 230, 24, dwTime);
            else m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutTransSpriteRGB(sX + iAdjX + 225, sY + iAdjY + 230, 24, 5, 5, 5, dwTime);

            if ((G_pGame->m_stMCursor.LB != 0) && m_dialogBoxes[26].IsTop())
            {
                if (onButton == 14)
                {
                    m_dialogBoxes[26].sView--;
                }

                if (onButton == 15)
                {
                    if (m_pDispBuildItemList[m_dialogBoxes[26].sView + 13] != 0)
                        m_dialogBoxes[26].sView++;
                }
            }
            if ((G_pGame->m_stMCursor.sZ != 0) && m_dialogBoxes[26].IsTop())
            {
                m_dialogBoxes[26].sView = m_dialogBoxes[26].sView - G_pGame->m_stMCursor.sZ / 60;
                //DIRECTX m_dInput.m_sZ = 0;
            }
            if (m_pDispBuildItemList[m_dialogBoxes[26].sView + 12] == 0)
            {
                while (1)
                {
                    m_dialogBoxes[26].sView--;
                    if (m_dialogBoxes[26].sView < 1) break;
                    if (m_pDispBuildItemList[m_dialogBoxes[26].sView + 12] != 0) break;
                }
            }
            if (m_dialogBoxes[26].sView < 0) m_dialogBoxes[26].sView = 0;

            PutAlignedString(sX, sX + m_dialogBoxes[26].sSizeX, sY + 260, DRAW_DIALOGBOX_SKILLDLG2, 55, 25, 25);//" List of items which you can make with"
            PutAlignedString(sX, sX + m_dialogBoxes[26].sSizeX, sY + 275, DRAW_DIALOGBOX_SKILLDLG3, 55, 25, 25);//"your current skill. The items you can"
            PutAlignedString(sX, sX + m_dialogBoxes[26].sSizeX, sY + 290, DRAW_DIALOGBOX_SKILLDLG4, 55, 25, 25);//"make now with your current stuff will"
            PutAlignedString(sX, sX + m_dialogBoxes[26].sSizeX, sY + 305, DRAW_DIALOGBOX_SKILLDLG5, 55, 25, 25);//"be displayed in blue. "
            PutAlignedString(sX, sX + m_dialogBoxes[26].sSizeX, sY + 330, DRAW_DIALOGBOX_SKILLDLG6, 55, 25, 25);//"Select an item you want to manufacture."

            strcpy(cTemp, DRAW_DIALOGBOX_SKILLDLG35);
            strcat(cTemp, m_manuAutoFill ? DRAW_ENABLED : DRAW_DISABLED);
            //PutAlignedString(&m_dialogBoxes[26].GetButton(16), cTemp, onButton == 16 ?video::SColor(255,255,255,255) :video::SColor(255,34,30,120));//uncomment

            break;

        case 4: // Manuf: Waiting for incredients
            sX = m_dialogBoxes[26].m_X;
            sY = m_dialogBoxes[26].m_Y;
            szX = m_dialogBoxes[26].sSizeX;
            iAdjX = -1;
            iAdjY = -7;
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 0);
            DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 8);
            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iSprH]->PutSpriteFast(sX + iAdjX + 62 + 5, sY + iAdjY + 84 + 17,
                                                                                                                           m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iSprFrame, dwTime);

            ZeroMemory(cTemp, sizeof(cTemp));
            GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cName, 0, cStr1, cStr2, cStr3);
            wsprintfA(cTemp, "%s", cStr1);
            PutString(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55, cTemp, video::SColor(255, 255, 255, 255));

            wsprintfA(cTemp, DRAW_DIALOGBOX_SKILLDLG7 // "Skill level: %d/%d"
                      , m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iSkillLimit
                      , m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iMaxSkill);
            PutString(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55 + 2 * 15, cTemp, video::SColor(255, 45, 25, 25));
            PutString(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55 + 3 * 15 + 5, DRAW_DIALOGBOX_SKILLDLG8, video::SColor(255, 45, 25, 25));//"Ingredients Needed:"

            iLoc = 4;
            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[1] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName1, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[1];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[1] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[2] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName2, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[2];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[2] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[3] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName3, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[3];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[3] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[4] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName4, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[4];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[4] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[5] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName5, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[5];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[5] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[6] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName6, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[6];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[6] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bBuildEnabled == true)
            {
                m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
                m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 45 * 1 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
                m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 45 * 2 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
                m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 100 + 180, 2, dwTime);
                m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 45 * 1 + 13, sY + iAdjY + 100 + 180, 2, dwTime);
                m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 45 * 2 + 13, sY + iAdjY + 100 + 180, 2, dwTime);

                if (m_dialogBoxes[26].sV1 != -1)
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV1]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 55 + 180,
                                                                                  m_pItemList[m_dialogBoxes[26].sV1]->m_sSpriteFrame, dwTime);

                if (m_dialogBoxes[26].sV2 != -1)
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV2]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 55 + 180,
                                                                                  m_pItemList[m_dialogBoxes[26].sV2]->m_sSpriteFrame, dwTime);

                if (m_dialogBoxes[26].sV3 != -1)
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV3]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 55 + 180,
                                                                                  m_pItemList[m_dialogBoxes[26].sV3]->m_sSpriteFrame, dwTime);

                if (m_dialogBoxes[26].sV4 != -1)
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV4]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 100 + 180,
                                                                                  m_pItemList[m_dialogBoxes[26].sV4]->m_sSpriteFrame, dwTime);

                if (m_dialogBoxes[26].sV5 != -1)
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV5]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 100 + 180,
                                                                                  m_pItemList[m_dialogBoxes[26].sV5]->m_sSpriteFrame, dwTime);

                if (m_dialogBoxes[26].sV6 != -1)
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV6]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 100 + 180,
                                                                                  m_pItemList[m_dialogBoxes[26].sV6]->m_sSpriteFrame, dwTime);

                PutAlignedString(sX, sX + szX, sY + iAdjY + 230 + 75, DRAW_DIALOGBOX_SKILLDLG15, 55, 25, 25);//" Click MANUFACTURE button after"
                PutAlignedString(sX, sX + szX, sY + iAdjY + 245 + 75, DRAW_DIALOGBOX_SKILLDLG16, 55, 25, 25);//"dragging ingredients in the blanks"
                PutAlignedString(sX, sX + szX, sY + iAdjY + 260 + 75, DRAW_DIALOGBOX_SKILLDLG17, 55, 25, 25);//"to manufacture above item."

                if (onButton == 1)
                    PutString_SprFont(sX + iAdjX + 25, sY + iAdjY + 330 + 23, "Back", 6, 6, 20);
                else PutString_SprFont(sX + iAdjX + 25, sY + iAdjY + 330 + 23, "Back", 0, 0, 7);


                if (onButton == 2)
                {
                    if (m_dialogBoxes[26].cStr[4] == 1)
                        PutString_SprFont(sX + iAdjX + 153, sY + iAdjY + 330 + 23, "Manufacture", 6, 6, 20);
                    else PutString_SprFont(sX + iAdjX + 153, sY + iAdjY + 330 + 23, "Manufacture", 10, 10, 10);
                }
                else
                {
                    if (m_dialogBoxes[26].cStr[4] == 1)
                        PutString_SprFont(sX + iAdjX + 153, sY + iAdjY + 330 + 23, "Manufacture", 0, 0, 7);
                    else PutString_SprFont(sX + iAdjX + 153, sY + iAdjY + 330 + 23, "Manufacture", 10, 10, 10);
                }
            }
            else
            {
                PutAlignedString(sX, sX + szX, sY + iAdjY + 200 + 75, DRAW_DIALOGBOX_SKILLDLG18, 55, 25, 25);//"There are not enough ingredients to"
                PutAlignedString(sX, sX + szX, sY + iAdjY + 215 + 75, DRAW_DIALOGBOX_SKILLDLG19, 55, 25, 25);//"manufacture. Needed materials are"
                PutAlignedString(sX, sX + szX, sY + iAdjY + 230 + 75, DRAW_DIALOGBOX_SKILLDLG20, 55, 25, 25);//"displayed in grey."
                if (onButton == 1)
                    PutString_SprFont(sX + iAdjX + 25, sY + iAdjY + 330 + 23, "Back", 6, 6, 20);
                else PutString_SprFont(sX + iAdjX + 25, sY + iAdjY + 330 + 23, "Back", 0, 0, 7);
            }
            break;

        case 5: // Manuf: in progress
            sX = m_dialogBoxes[26].m_X;
            sY = m_dialogBoxes[26].m_Y;
            iAdjX = -1;
            iAdjY = -7;

            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 0);
            DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 8);
            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iSprH]->PutSpriteFast(sX + iAdjX + 62 + 5, sY + iAdjY + 84 + 17,
                                                                                                                           m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iSprFrame, dwTime);

            ZeroMemory(cTemp, sizeof(cTemp));
            GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cName, 0, cStr1, cStr2, cStr3);
            wsprintfA(cTemp, "%s", cStr1);
            PutString(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55, cTemp, video::SColor(255, 255, 255, 255));

            wsprintfA(cTemp, DRAW_DIALOGBOX_SKILLDLG7 // "Skill level: %d/%d"
                      , m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iSkillLimit, m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iMaxSkill);
            PutString(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55 + 2 * 15, cTemp, video::SColor(255, 45, 25, 25));
            PutString(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55 + 3 * 15 + 5, DRAW_DIALOGBOX_SKILLDLG8, video::SColor(255, 45, 25, 25));//"Ingredients Needed:"

            iLoc = 4;
            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[1] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName1, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[1];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[1] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[2] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName2, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[2];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[2] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[3] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName3, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[3];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[3] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[4] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName4, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[4];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[4] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[5] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName5, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[5];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[5] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[6] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cElementName6, 0, cStr1, cStr2, cStr3);
                iAmount = m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iElementCount[6];
                wsprintfA(cTemp, "%d %s", iAmount, cStr1);
                if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bElementFlag[6] == true)
                    PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 45, 25, 25));
                else PutString(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, video::SColor(255, 120, 120, 120));
                iLoc++;
            }

            m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
            m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 45 * 1 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
            m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 45 * 2 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
            m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 100 + 180, 2, dwTime);
            m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 45 * 1 + 13, sY + iAdjY + 100 + 180, 2, dwTime);
            m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + iAdjX + 55 + 30 + 45 * 2 + 13, sY + iAdjY + 100 + 180, 2, dwTime);

            if (m_dialogBoxes[26].sV1 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV1]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 55 + 180,
                                                                              m_pItemList[m_dialogBoxes[26].sV1]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV2 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV2]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 55 + 180,
                                                                              m_pItemList[m_dialogBoxes[26].sV2]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV3 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV3]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 55 + 180,
                                                                              m_pItemList[m_dialogBoxes[26].sV3]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV4 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV4]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 100 + 180,
                                                                              m_pItemList[m_dialogBoxes[26].sV4]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV5 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV5]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 100 + 180,
                                                                              m_pItemList[m_dialogBoxes[26].sV5]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV6 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV6]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 100 + 180,
                                                                              m_pItemList[m_dialogBoxes[26].sV6]->m_sSpriteFrame, dwTime);

            PutString(sX + iAdjX + 33, sY + iAdjY + 230 + 75, DRAW_DIALOGBOX_SKILLDLG29, video::SColor(255, 55, 25, 25));//" Manufacturing the items...."
            PutString(sX + iAdjX + 33, sY + iAdjY + 245 + 75, DRAW_DIALOGBOX_SKILLDLG30, video::SColor(255, 55, 25, 25));//"Please wait until manufacture finishes."

            if ((dwTime - m_dialogBoxes[26].dwT1) > 400) //if ((dwTime - m_stDialogBoxInfo[26].dwT1) > 1000) 
            {
                m_dialogBoxes[26].dwT1 = dwTime;
                m_dialogBoxes[26].cStr[1]++;
                if (m_dialogBoxes[26].cStr[1] >= 7) m_dialogBoxes[26].cStr[1] = 7;
            }

            if (m_dialogBoxes[26].cStr[1] == 4)
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_BUILDITEM, 0, 0, 0, 0, m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cName);
                m_dialogBoxes[26].cStr[1]++;
            }
            break;

        case 6: // Manuf: Done
            sX = m_dialogBoxes[26].m_X;
            sY = m_dialogBoxes[26].m_Y;
            iAdjX = -1;
            iAdjY = -7;

            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 0);
            DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 8);
            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iSprH]->PutSpriteFast(sX + iAdjX + 62 + 5, sY + iAdjY + 84 + 17,
                                                                                                                           m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_iSprFrame, dwTime);

            ZeroMemory(cTemp, sizeof(cTemp));
            GetItemName(m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_cName, 0, cStr1, cStr2, cStr3);

            wsprintfA(cTemp, "%s", cStr1);
            PutString(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55, cTemp, video::SColor(255, 255, 255, 255));

            if (m_dialogBoxes[26].cStr[2] == 1)
            {
                PutString(sX + iAdjX + 33 + 11, sY + iAdjY + 200 - 45, DRAW_DIALOGBOX_SKILLDLG31, video::SColor(255, 55, 25, 25));//" Success in manufacture!"

                if (m_dialogBoxes[26].sV1 == ITEMTYPE_MATERIAL)
                {
                    wsprintfA(G_cTxt, DRAW_DIALOGBOX_SKILLDLG32, m_dialogBoxes[26].cStr[3]);//"The purity of product is %d%%."
                    PutString(sX + iAdjX + 33 + 11, sY + iAdjY + 215 - 45, G_cTxt, video::SColor(255, 55, 25, 25));
                }
                else
                {
                    wsprintfA(G_cTxt, DRAW_DIALOGBOX_SKILLDLG33, (int)m_dialogBoxes[26].cStr[3] + 100);//"The completion of product is %d%%."
                    PutString(sX + iAdjX + 33, sY + iAdjY + 215 - 45, G_cTxt, video::SColor(255, 55, 25, 25));
                }
            }
            else
            {
                PutString(sX + iAdjX + 33 + 11, sY + iAdjY + 200, DRAW_DIALOGBOX_SKILLDLG34, video::SColor(255, 55, 25, 25));//"Failed on manufacture."
            }

            if (onButton == 1)
                PutString_SprFont(sX + iAdjX + 35, sY + iAdjY + 330 + 23, "Back", 6, 6, 20);
            else PutString_SprFont(sX + iAdjX + 35, sY + iAdjY + 330 + 23, "Back", 0, 0, 7);
            break;


        case 7: // Crafting, wait for incredients
            if (m_dialogBoxes[26].cStr[0] != 0)
            {
                sX = m_dialogBoxes[26].m_X + iAdjX + (m_dialogBoxes[26].cStr[0] - (rand() % (m_dialogBoxes[26].cStr[0] * 2)));
                sY = m_dialogBoxes[26].m_Y + iAdjY + (m_dialogBoxes[26].cStr[0] - (rand() % (m_dialogBoxes[26].cStr[0] * 2)));
            }
            else
            {
                sX = m_dialogBoxes[26].m_X;
                sY = m_dialogBoxes[26].m_Y;
            }
            m_pSprite[SPRID_INTERFACE_CRAFTING]->PutSpriteFast(sX, sY, 0, dwTime);

            if (m_dialogBoxes[26].sV1 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV1]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                                                                              m_pItemList[m_dialogBoxes[26].sV1]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV2 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV2]->m_sSprite]->PutSpriteFast(sX + iAdjX + 65 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 40 + (1 - (rand() % 3)),
                                                                              m_pItemList[m_dialogBoxes[26].sV2]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV3 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV3]->m_sSprite]->PutSpriteFast(sX + iAdjX + 65 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                                                                              m_pItemList[m_dialogBoxes[26].sV3]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV4 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV4]->m_sSprite]->PutSpriteFast(sX + iAdjX + 65 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                                                                              m_pItemList[m_dialogBoxes[26].sV4]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV5 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV5]->m_sSprite]->PutSpriteFast(sX + iAdjX + 65 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 115 + (1 - (rand() % 3)),
                                                                              m_pItemList[m_dialogBoxes[26].sV5]->m_sSpriteFrame, dwTime);

            if (m_dialogBoxes[26].sV6 != -1)
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_dialogBoxes[26].sV6]->m_sSprite]->PutSpriteFast(sX + iAdjX + 75 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                                                                              m_pItemList[m_dialogBoxes[26].sV6]->m_sSpriteFrame, dwTime);
            if (onButton == 1)
                PutString_SprFont(sX + iAdjX + 60, sY + iAdjY + 175, "Try Now!", 16, 16, 30);
            else PutString_SprFont(sX + iAdjX + 60, sY + iAdjY + 175, "Try Now!", 6, 6, 20);
            break;

        case 8: // Crafting in progress
            if (m_dialogBoxes[26].cStr[0] != 0)
            {
                sX = m_dialogBoxes[26].m_X + iAdjX + (m_dialogBoxes[26].cStr[0] - (rand() % (m_dialogBoxes[26].cStr[0] * 2)));
                sY = m_dialogBoxes[26].m_Y + iAdjY + (m_dialogBoxes[26].cStr[0] - (rand() % (m_dialogBoxes[26].cStr[0] * 2)));
            }
            else
            {
                sX = m_dialogBoxes[26].m_X;
                sY = m_dialogBoxes[26].m_Y;
            }
            m_pSprite[SPRID_INTERFACE_CRAFTING]->PutSpriteFast(sX, sY, 0, dwTime);

            if (m_dialogBoxes[26].sV1 != -1)
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV1]->m_sSprite]->PutSpriteFast(sX + iAdjX + 55 + (1 - (rand() % 3)) + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                                                                                  m_pItemList[m_dialogBoxes[26].sV1]->m_sSpriteFrame, dwTime);
                if ((m_pItemList[m_dialogBoxes[26].sV1]->m_cItemType == ITEMTYPE_EQUIP)
                    && (m_pItemList[m_dialogBoxes[26].sV1]->m_cEquipPos == EQUIPPOS_NECK))
                    m_iContributionPrice = 10;
            }
            if (m_dialogBoxes[26].sV2 != -1)
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV2]->m_sSprite]->PutSpriteFast(sX + iAdjX + 65 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 40 + (1 - (rand() % 3)),
                                                                                  m_pItemList[m_dialogBoxes[26].sV2]->m_sSpriteFrame, dwTime);
                if ((m_pItemList[m_dialogBoxes[26].sV2]->m_cItemType == ITEMTYPE_EQUIP)
                    && (m_pItemList[m_dialogBoxes[26].sV2]->m_cEquipPos == EQUIPPOS_NECK))
                    m_iContributionPrice = 10;
            }
            if (m_dialogBoxes[26].sV3 != -1)
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV3]->m_sSprite]->PutSpriteFast(sX + iAdjX + 65 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                                                                                  m_pItemList[m_dialogBoxes[26].sV3]->m_sSpriteFrame, dwTime);
                if ((m_pItemList[m_dialogBoxes[26].sV3]->m_cItemType == ITEMTYPE_EQUIP)
                    && (m_pItemList[m_dialogBoxes[26].sV3]->m_cEquipPos == EQUIPPOS_NECK))
                    m_iContributionPrice = 10;
            }
            if (m_dialogBoxes[26].sV4 != -1)
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV4]->m_sSprite]->PutSpriteFast(sX + iAdjX + 65 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                                                                                  m_pItemList[m_dialogBoxes[26].sV4]->m_sSpriteFrame, dwTime);
                if ((m_pItemList[m_dialogBoxes[26].sV4]->m_cItemType == ITEMTYPE_EQUIP)
                    && (m_pItemList[m_dialogBoxes[26].sV4]->m_cEquipPos == EQUIPPOS_NECK))
                    m_iContributionPrice = 10;
            }
            if (m_dialogBoxes[26].sV5 != -1)
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV5]->m_sSprite]->PutSpriteFast(sX + iAdjX + 65 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 115 + (1 - (rand() % 3)),
                                                                                  m_pItemList[m_dialogBoxes[26].sV5]->m_sSpriteFrame, dwTime);
                if ((m_pItemList[m_dialogBoxes[26].sV5]->m_cItemType == ITEMTYPE_EQUIP)
                    && (m_pItemList[m_dialogBoxes[26].sV5]->m_cEquipPos == EQUIPPOS_NECK))
                    m_iContributionPrice = 10;
            }
            if (m_dialogBoxes[26].sV6 != -1)
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_dialogBoxes[26].sV6]->m_sSprite]->PutSpriteFast(sX + iAdjX + 75 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                                                                                  m_pItemList[m_dialogBoxes[26].sV6]->m_sSpriteFrame, dwTime);
                if ((m_pItemList[m_dialogBoxes[26].sV6]->m_cItemType == ITEMTYPE_EQUIP)
                    && (m_pItemList[m_dialogBoxes[26].sV6]->m_cEquipPos == EQUIPPOS_NECK))
                    m_iContributionPrice = 10;
            }
            PutString_SprFont(sX + iAdjX + 60, sY + iAdjY + 175, "Creating...", 20, 6, 6);

            if ((dwTime - m_dialogBoxes[26].dwT1) > 1000)
            {
                m_dialogBoxes[26].dwT1 = dwTime;
                m_dialogBoxes[26].cStr[1]++;
            }
            if (m_dialogBoxes[26].cStr[1] >= 5)//m_pDispCraftItemList
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_CRAFTITEM, 0, 0, 0, 0, 0);
                DisableDialogBox(26);
                PlaySound('E', 42, 0);
            }
            break;
    }
}

void CGame::DrawDialogBox_SysMenu()
{
    short sX, sY;
    char onButton = m_dialogBoxes[19].OnButton();

    sX = m_dialogBoxes[19].m_X;
    sY = m_dialogBoxes[19].m_Y;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME1, sX, sY, 0);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 6);
    PutString(sX + 23, sY + 63, DRAW_DIALOGBOX_SYSMENU_DETAILLEVEL, video::SColor(255, 255, 255, 255));
    if (m_cDetailLevel == 0)
        PutString(sX + 121, sY + 63, DRAW_DIALOGBOX_SYSMENU_LOW, video::SColor(255, 255, 255, 255));
    else PutString(sX + 121, sY + 63, DRAW_DIALOGBOX_SYSMENU_LOW, video::SColor(255, 45, 25, 25));
    if (m_cDetailLevel == 1)
        PutString(sX + 153, sY + 63, DRAW_DIALOGBOX_SYSMENU_NORMAL, video::SColor(255, 255, 255, 255));
    else PutString(sX + 153, sY + 63, DRAW_DIALOGBOX_SYSMENU_NORMAL, video::SColor(255, 45, 25, 25));
    if (m_cDetailLevel == 2)
        PutString(sX + 205, sY + 63, DRAW_DIALOGBOX_SYSMENU_HIGH, video::SColor(255, 255, 255, 255));
    else PutString(sX + 205, sY + 63, DRAW_DIALOGBOX_SYSMENU_HIGH, video::SColor(255, 45, 25, 25));

    PutString(sX + 23, sY + 84, DRAW_DIALOGBOX_SYSMENU_SOUND, video::SColor(255, 255, 255, 255));
    if (m_bSoundFlag)
    {
        if (m_bSoundStat) PutString(sX + 85, sY + 85, DRAW_DIALOGBOX_SYSMENU_ON, video::SColor(255, 255, 255, 255));
        else PutString(sX + 83, sY + 85, DRAW_DIALOGBOX_SYSMENU_OFF, video::SColor(255, 200, 200, 200));
    }
    else PutString(sX + 68, sY + 85, DRAW_DIALOGBOX_SYSMENU_DISABLED, video::SColor(255, 100, 100, 100));

    PutString(sX + 123, sY + 84, DRAW_DIALOGBOX_SYSMENU_MUSIC, video::SColor(255, 255, 255, 255));
    if (m_bSoundFlag)
    {
        if (m_bMusicStat) PutString(sX + 180, sY + 85, DRAW_DIALOGBOX_SYSMENU_ON, video::SColor(255, 255, 255, 255));
        else PutString(sX + 178, sY + 85, DRAW_DIALOGBOX_SYSMENU_OFF, video::SColor(255, 200, 200, 200));
    }
    else PutString(sX + 163, sY + 85, DRAW_DIALOGBOX_SYSMENU_DISABLED, video::SColor(255, 100, 100, 100));

    PutString(sX + 23, sY + 106, DRAW_DIALOGBOX_SYSMENU_WHISPER, video::SColor(255, 255, 255, 255));
    if (m_bWhisper) PutString(sX + 85, sY + 106, DRAW_DIALOGBOX_SYSMENU_ON, video::SColor(255, 255, 255, 255));
    else PutString(sX + 82, sY + 106, DRAW_DIALOGBOX_SYSMENU_OFF, video::SColor(255, 200, 200, 200));

    PutString(sX + 123, sY + 106, DRAW_DIALOGBOX_SYSMENU_SHOUT, video::SColor(255, 255, 255, 255));
    if (m_bShout) PutString(sX + 180, sY + 106, DRAW_DIALOGBOX_SYSMENU_ON, video::SColor(255, 255, 255, 255));
    else PutString(sX + 177, sY + 106, DRAW_DIALOGBOX_SYSMENU_OFF, video::SColor(255, 200, 200, 200));

    PutString(sX + 23, sY + 124, DRAW_DIALOGBOX_SYSMENU_SOUNDVOLUME, video::SColor(255, 255, 255, 255));
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX + 130 + m_cSoundVolume, sY + 129, 8);

    PutString(sX + 23, sY + 141, DRAW_DIALOGBOX_SYSMENU_MUSICVOLUME, video::SColor(255, 255, 255, 255));
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX + 130 + m_cMusicVolume, sY + 145, 8);

    PutString(sX + 23, sY + 158, DRAW_DIALOGBOX_SYSMENU_TRANSPARENCY, video::SColor(255, 255, 255, 255));
    if (m_bDialogTrans) PutString(sX + 208, sY + 158, DRAW_DIALOGBOX_SYSMENU_ON, video::SColor(255, 255, 255, 255));
    else PutString(sX + 207, sY + 158, DRAW_DIALOGBOX_SYSMENU_OFF, video::SColor(255, 200, 200, 200));

    PutString(sX + 23, sY + 180, DRAW_DIALOGBOX_SYSMENU_GUIDEMAP, video::SColor(255, 255, 255, 255));
    if (m_bIsDialogEnabled[9]) PutString(sX + 99, sY + 180, DRAW_DIALOGBOX_SYSMENU_ON, video::SColor(255, 255, 255, 255));
    else PutString(sX + 98, sY + 180, DRAW_DIALOGBOX_SYSMENU_OFF, video::SColor(255, 200, 200, 200));

    if (onButton == 14) // Extended Menu Fixed xRisenx
        (DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 163, sY + 177, 51));
    else (DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 163, sY + 177, 50));

    SYSTEMTIME SysTime;
    GetLocalTime(&SysTime);
    ZeroMemory(G_cTxt, sizeof(G_cTxt));
    wsprintfA(G_cTxt, "M:%d - D:%d - H:%d - Min:%d - S:%d", SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
    PutString(sX + 23, sY + 204, G_cTxt, video::SColor(255, 255, 255, 255));

    //v2.172
#ifdef _DEBUG
    PutString3(sX + 23, sY + 41, UPDATE_SCREEN_ON_SELECT_CHARACTER36, video::SColor(255, 45, 25, 25)); // "Test server"
#else

    if (strcmp(m_cWorldServerName, NAME_WORLDNAME1) == 0)
        PutString3(sX + 23, sY + 41, MSG_WORLDNAME1, video::SColor(255, 45, 25, 25));

    else if (strcmp(m_cWorldServerName, NAME_WORLDNAME2) == 0)
        PutString3(sX + 23, sY + 41, MSG_WORLDNAME2, video::SColor(255, 45, 25, 25));
#endif

    if ((G_pGame->m_stMCursor.LB != 0) && m_dialogBoxes[19].IsTop())
        //  (msX >= sX + 120) && (msX <= sX + 242) && (msY >= sY +122) && (msY <= sY +138)
    {
        if (onButton == 12)
        {
            m_cSoundVolume = G_pGame->m_stMCursor.sX - (sX + 127);
            if (m_cSoundVolume > 100) m_cSoundVolume = 100;
            if (m_cSoundVolume < 0) m_cSoundVolume = 0;
        }
        else if (onButton == 13)
        {
            m_cMusicVolume = G_pGame->m_stMCursor.sX - (sX + 127);
            if (m_cMusicVolume > 100) m_cMusicVolume = 100;
            if (m_cMusicVolume < 0) m_cMusicVolume = 0;
            if (m_bSoundFlag)
            {
                int iVol;
                iVol = (m_cMusicVolume - 100) * 20;
                if (iVol > 0) iVol = 0;
                if (iVol < -10000) iVol = -10000;
                // 				if (m_pBGM != NULL)
                // 				{
                // 					m_pBGM->bStop(TRUE);
                // 					m_pBGM->Play(FALSE, 0, iVol);
                // 				}//DIRECTX
            }
        }
    }
    else m_dialogBoxes[19].bIsScrollSelected = false;

    // Log-Out
    if (m_cLogOutCount == -1)
    {
        if (onButton == 10)
            DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + 225, 9);
        else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + 225, 8);
    }
    else
    {  //Continue
        if (onButton == 10)
            DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + 225, 7);
        else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + 225, 6);
    }
    // Restart
    if ((m_iHP <= 0) && (m_cRestartCount == -1))
    {
        if (onButton == 11)
            DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + 225, 37);
        else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + 225, 36);
    }
}


void CGame::DlgBoxClick_Bank()
{
    int i;
    char onButton = m_dialogBoxes[14].OnButton();
    switch (m_dialogBoxes[14].GetMode())
    {
        case -1:
            break;

        case 0:
            for (i = 0; i < m_dialogBoxes[14].sV1; i++)
                if (onButton == i + 1)
                {
                    if ((m_pBankList[m_dialogBoxes[14].sView + i] != 0) && ((m_dialogBoxes[14].sView + i) < MAXBANKITEMS))
                    {
                        if (_iGetTotalItemNum() >= 75)
                        { // Bag Fix xRisenx
                            AddEventList(DLGBOX_CLICK_BANK1, 10);
                            return;
                        }
                        bSendCommand(MSGID_REQUEST_RETRIEVEITEM, 0, 0, (m_dialogBoxes[14].sView + i), 0, 0, 0);
                        m_dialogBoxes[14].SetMode(-1);
                        PlaySound('E', 14, 5);
                    }
                    return;
                }

            break;
    }
}

void CGame::DlgBoxClick_Fish()
{
    switch (m_dialogBoxes[24].GetMode())
    {
        case 0:
            if (m_dialogBoxes[24].OnButton() == 1)
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_GETFISHTHISTIME, 0, 0, 0, 0, 0);
                AddEventList(DLGBOX_CLICK_FISH1, 10);
                DisableDialogBox(24);

                PlaySound('E', 14, 5);
            }
            break;
    }
}

void CGame::DlgBoxClick_Magic()
{
    int i, j, iCPivot;
    iCPivot = m_dialogBoxes[3].sView * 10;

    char onButton = m_dialogBoxes[3].OnButton();
    for (i = 0, j = 0; i < 9; i++)
    {
        if ((m_cMagicMastery[iCPivot + i] != 0) && (m_pMagicCfgList[iCPivot + i] != 0))
        {
            j++;
            if (onButton == j)
            {
                UseMagic(iCPivot + i);
                PlaySound('E', 14, 5);
                return;
            }
        }
    }

    if (onButton == 10)
        m_dialogBoxes[3].sView = 0;
    if (onButton == 11)
        m_dialogBoxes[3].sView = 1;
    if (onButton == 12)
        m_dialogBoxes[3].sView = 2;
    else if (onButton == 13)
        m_dialogBoxes[3].sView = 3;
    else if (onButton == 14)
        m_dialogBoxes[3].sView = 4;
    else if (onButton == 15)
        m_dialogBoxes[3].sView = 5;
    else if (onButton == 16)
        m_dialogBoxes[3].sView = 6;
    else if (onButton == 17)
        m_dialogBoxes[3].sView = 7;
    else if (onButton == 18)
        m_dialogBoxes[3].sView = 8;
    else if (onButton == 19)
        m_dialogBoxes[3].sView = 9;
    else if (onButton == 20)
    {
        //if (m_cSkillMastery[SKILL_ALCHEMY] == 0) AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY16, 10);
        //else
        //{	
        for (i = 0; i < MAXITEMS; i++)
            if ((m_pItemList[i] != 0) && (m_pItemList[i]->m_cItemType == ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX) &&
                (m_pItemList[i]->m_sSpriteFrame == 55))
            {
                EnableDialogBox(26, 1, 0, 0, 0);
                AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY10, 10);
                PlaySound('E', 14, 5);
                return;
            }
        AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY15, 10);
        //}
        PlaySound('E', 14, 5);
    }
}

void CGame::DlgBoxClick_NpcActionQuery()
{

    int   absX, absY;
    if (m_bIsDialogEnabled[27] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    char onButton = m_dialogBoxes[20].OnButton();
    switch (m_dialogBoxes[20].GetMode())
    {
        case 0: // Talk to npc
            if (onButton == 1)
            {
                EnableDialogBox(m_dialogBoxes[20].sV1, m_dialogBoxes[20].sV2, 0, 0);
                DisableDialogBox(20);
            }
            if ((m_bIsDialogEnabled[21] == false) && onButton == 2)
            {
                switch (m_dialogBoxes[20].sV1)
                {
                    case 7:	// Guild
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 1, 0, 0, 0);
                        AddEventList(TALKING_TO_GUILDHALL_OFFICER, 10);
                        break;
                    case 11: // BS or Shop
                        switch (m_dialogBoxes[20].sV2)
                        {
                            case 1:
                                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 2, 0, 0, 0);
                                AddEventList(TALKING_TO_SHOP_KEEPER, 10);
                                break;
                            case 2:
                                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 3, 0, 0, 0);
                                AddEventList(TALKING_TO_BLACKSMITH_KEEPER, 10);
                                break;
                        }
                        break;
                    case 13: // CityHall officer
                        if (m_stQuest.bIsQuestCompleted)
                        {
                            bSendCommand(MSGID_REQUEST_ACCEPT_QUEST, 0, 0, 0, 0, 0, 0);
                        }
                        else
                        {
                            bSendCommand(MSGID_REQUEST_QUEST_LIST, 0, 0, 0, 0, 0, "William");
                            EnableDialogBox(61, 0, 0, 0);
                        }
                        AddEventList(TALKING_TO_CITYHALL_OFFICER, 10);
                        break;
                    case 14: // WH keeper
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 5, 0, 0, 0);
                        AddEventList(TALKING_TO_WAREHOUSE_KEEPER, 10);
                        break;
                    case 16: // Magicmerchant
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 6, 0, 0, 0);
                        AddEventList(TALKING_TO_MAGICIAN, 10);
                        break;
                    case 56: //GuildBank
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 10, 0, 0, 0);
                        AddEventList(TALKING_TO_GUILDBANK_KEEPER, 10);
                        break;
                }
                DisableDialogBox(20);
            }
            break;

        case 1: // On other player
            if (onButton == 1)
            {
                absX = abs(m_dialogBoxes[20].sV5 - m_sPlayerX);
                absY = abs(m_dialogBoxes[20].sV6 - m_sPlayerY);
                if ((absX <= 4) && (absY <= 4))
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, m_dialogBoxes[20].sV1, m_dialogBoxes[20].sV3, m_dialogBoxes[20].sV5, m_dialogBoxes[20].sV6, m_pItemList[m_dialogBoxes[20].sV1]->m_cName, m_dialogBoxes[20].sV4); //v1.4
                else AddEventList(DLGBOX_CLICK_NPCACTION_QUERY7, 10); //"Too far to give the item."
                DisableDialogBox(20);
            }
            else if (onButton == 2)
            {
                absX = abs(m_dialogBoxes[20].sV5 - m_sPlayerX);
                absY = abs(m_dialogBoxes[20].sV6 - m_sPlayerY);
                if ((absX <= 4) && (absY <= 4))
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_EXCHANGEITEMTOCHAR, m_dialogBoxes[20].sV1, m_dialogBoxes[20].sV3, m_dialogBoxes[20].sV5, m_dialogBoxes[20].sV6, m_pItemList[m_dialogBoxes[20].sV1]->m_cName, m_dialogBoxes[20].sV4); //v1.4
                else AddEventList(DLGBOX_CLICK_NPCACTION_QUERY8, 10); //"Too far to exchange item."
                DisableDialogBox(20);
            }
            break;

        case 2: // Item on Shop/BS
            if (onButton == 1)
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_SELLITEM, 0, m_dialogBoxes[20].sV1, m_dialogBoxes[20].sV2, m_dialogBoxes[20].sV3, m_pItemList[m_dialogBoxes[20].sV1]->m_cName, m_dialogBoxes[20].sV4);
                DisableDialogBox(20);
            }
            else if (onButton == 2)
            {
                if (m_dialogBoxes[20].sV3 == 1)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_REPAIRITEM, 0, m_dialogBoxes[20].sV1, m_dialogBoxes[20].sV2, 0, m_pItemList[m_dialogBoxes[20].sV1]->m_cName, m_dialogBoxes[20].sV4);
                    DisableDialogBox(20);
                }
            }
            break;

        case 3: // Put item in the WH
            if (onButton == 1)
            {
                absX = abs(m_dialogBoxes[20].sV5 - m_sPlayerX);
                absY = abs(m_dialogBoxes[20].sV6 - m_sPlayerY);
                if ((absX <= 8) && (absY <= 8))
                {
                    if (_iGetBankItemCount() >= MAXBANKITEMS)
                    {
                        AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);//"here is no empty space left in warehouse."
                    }
                    else bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, m_dialogBoxes[20].sV1, m_dialogBoxes[20].sV3, m_dialogBoxes[20].sV5, m_dialogBoxes[20].sV6, m_pItemList[m_dialogBoxes[20].sV1]->m_cName, m_dialogBoxes[20].sV4); //v1.4
                }
                else AddEventList(DLGBOX_CLICK_NPCACTION_QUERY7, 10);//"Too far to give the item."

                DisableDialogBox(20);
            }
            break;

        case 4: // talk to npc or Unicorn
            if ((m_bIsDialogEnabled[21] == false) && onButton == 1)
            {
                switch (m_dialogBoxes[20].sV3)
                {
                    case 21: // Guard
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 21, 0, 0, 0);
                        AddEventList(TALKING_TO_GUARD, 10);//"Talking to Guard..."
                        break;

                    case 32: // Unicorn
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 32, 0, 0, 0);
                        AddEventList(TALKING_TO_UNICORN, 10);//"Talking to Unicorn..."
                        break;
                    case 67:
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 67, 0, 0, 0);
                        AddEventList(TALKING_TO_MCGAFFIN, 10);//"Talking to a town man..."
                        break;
                    case 68:
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 68, 0, 0, 0);
                        AddEventList(TALKING_TO_PERRY, 10);//"Talking to a town maiden..."
                        break;
                    case 69:
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 69, 0, 0, 0);
                        AddEventList(TALKING_TO_DEVLIN, 10);//"Talking to a town magician..."
                        break;
                }
            }
            DisableDialogBox(20);
            break;


        case 5: // Talk  slit shop
            if (onButton == 1)
            {
                if ((m_dialogBoxes[20].sV2 == 1) && (m_dialogBoxes[20].sV1 == 11))
                {
                    EnableDialogBox(52, 0, 0, 0);
                    DisableDialogBox(20);
                }
                if ((m_dialogBoxes[20].sV2 == 2) && (m_dialogBoxes[20].sV1 == 11))
                {
                    EnableDialogBox(53, 0, 0, 0);
                    DisableDialogBox(20);
                }
                if ((m_dialogBoxes[20].sV2 != 2) && (m_dialogBoxes[20].sV2 != 1) && (m_dialogBoxes[20].sV1 != 11))
                {
                    EnableDialogBox(m_dialogBoxes[20].sV1, m_dialogBoxes[20].sV2, 0, 0);
                    DisableDialogBox(20);
                }
            }
            if (onButton == 2)
            {
                EnableDialogBox(31, 0, 0, 0);
                DisableDialogBox(20);
            }
            if ((m_bIsDialogEnabled[21] == false) && onButton == 3)
            {
                switch (m_dialogBoxes[20].sV1)
                {
                    case 7:	// Guild
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 1, 0, 0, 0);
                        AddEventList(TALKING_TO_GUILDHALL_OFFICER, 10);//"Talking to Guildhall Officer..."
                        break;

                    case 11: //
                        switch (m_dialogBoxes[20].sV2)
                        {
                            case 1:
                                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 2, 0, 0, 0);
                                AddEventList(TALKING_TO_SHOP_KEEPER, 10);//"Talking to Shop Keeper..."
                                break;
                            case 2:
                                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 3, 0, 0, 0);
                                AddEventList(TALKING_TO_BLACKSMITH_KEEPER, 10);//"
                                break;
                        }
                        break;

                    case 13:
                        if (m_stQuest.bIsQuestCompleted)
                        {
                            bSendCommand(MSGID_REQUEST_ACCEPT_QUEST, 0, 0, 0, 0, 0, 0);
                        }
                        else
                        {
                            bSendCommand(MSGID_REQUEST_QUEST_LIST, 0, 0, 0, 0, 0, "William");
                            EnableDialogBox(61, 0, 0, 0);
                            AddEventList(TALKING_TO_CITYHALL_OFFICER, 10);//"
                        }
                        break;

                    case 14: //
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 5, 0, 0, 0);
                        AddEventList(TALKING_TO_WAREHOUSE_KEEPER, 10);//
                        break;

                    case 16: //
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TALKTONPC, 0, 6, 0, 0, 0);
                        AddEventList(TALKING_TO_MAGICIAN, 10);//"
                        break;
                }
                DisableDialogBox(20);
            }
            break;

        case 6: // Snoopy: Added Gail
            if (onButton == 1)
            {
                EnableDialogBox(51, 0, 0, 0);
                DisableDialogBox(20);
            }
            break;

        case 7: // Jehovah - New Blacksmith Function
            if (onButton == 1) // Buy Weapons
            {
                EnableDialogBox(11, 4, 0, 0);
                DisableDialogBox(20);
            }
            if (onButton == 2) // Buy (M) Armor
            {
                EnableDialogBox(11, 5, 0, 0);
                DisableDialogBox(20);
            }
            if (onButton == 3) // Buy (W) Armor
            {
                EnableDialogBox(11, 6, 0, 0);
                DisableDialogBox(20);
            }
            if (onButton == 4) // Sell items
            {
                EnableDialogBox(31, 0, 0, 0);
                DisableDialogBox(20);
            }
            if (onButton == 5) // Repair All
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_REPAIRALL, 0, 0, 0, 0, 0);
                DisableDialogBox(20);
            }
            break;

        case 8: // Jehovah - New Shopkeeper Function
            if (onButton == 1) // Buy Potions
            {
                EnableDialogBox(11, 1, 0, 0);
                DisableDialogBox(20);
            }
            if (onButton == 2) // Buy Misc.
            {
                EnableDialogBox(11, 2, 0, 0);
                DisableDialogBox(20);
            }
            if (onButton == 3) // Buy Outfits
            {
                EnableDialogBox(11, 3, 0, 0);
                DisableDialogBox(20);
            }
            if (onButton == 4) // Sell items
            {
                EnableDialogBox(31, 0, 0, 0);
                DisableDialogBox(20);
            }
            break;

        case 9: // Jehovah - New Cityhall Officer Function
            if (onButton == 1) // Offer
            {
                EnableDialogBox(13, 0, 0, 0);
                DisableDialogBox(20);
            }
            else if (onButton == 2) // Mailbox
            {
                EnableDialogBox(DIALOG_MAILBOX, 0, 0, 0);
                DisableDialogBox(20);
            }
            //else if (onButton == 3) // Buy Weapons
            //{	
            //	EnableDialogBox(53, 0, NULL, NULL);
            //	DisableDialogBox(20);
            //}
            else if (onButton == 4) // Talk
            {
                if (m_stQuest.bIsQuestCompleted)
                {
                    bSendCommand(MSGID_REQUEST_ACCEPT_QUEST, 0, 0, 0, 0, 0, 0);
                }
                else
                {
                    bSendCommand(MSGID_REQUEST_QUEST_LIST, 0, 0, 0, 0, 0, "William");
                    EnableDialogBox(61, 0, 0, 0);
                    AddEventList(TALKING_TO_CITYHALL_OFFICER, 10);//"
                }
            }
            break;

        case 10: // Put item in the GWH
            if (onButton == 1)
            {
                absX = abs(m_dialogBoxes[20].sV5 - m_sPlayerX);
                absY = abs(m_dialogBoxes[20].sV6 - m_sPlayerY);
                if ((absX <= 8) && (absY <= 8))
                {
                    if (m_guildBankMap.size() >= maxGWHItems[m_guild.m_upgrades[GU_WAREHOUSE]])
                        AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);//"here is no empty space left in warehouse."
                    else
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, m_dialogBoxes[20].sV1, m_dialogBoxes[20].sV3, m_dialogBoxes[20].sV5, m_dialogBoxes[20].sV6, m_pItemList[m_dialogBoxes[20].sV1]->m_cName, m_dialogBoxes[20].sV4); //v1.4
                }
                else
                    AddEventList(DLGBOX_CLICK_NPCACTION_QUERY7, 10);//"Too far to give the item."

                DisableDialogBox(20);
            }
            break;

        case 11: // Repair all items from blacksmith
            if (onButton == 1)
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_REPAIRALLCONFIRM, 0, m_dialogBoxes[20].sV2, 0, 0, 0);
                DisableDialogBox(20);
            }
            break;
    }
}


void CGame::DlgBoxClick_Shop()
{
    int i, maxWeight;
    short sX, sY;
    char cTemp[21];
    bool isArrow = false;

    char onButton = m_dialogBoxes[11].OnButton();
    sX = m_dialogBoxes[11].m_X;
    sY = m_dialogBoxes[11].m_Y;
    maxWeight = m_stat[STAT_STR] * 5 + m_iLevel * 20; // changed from 5 to 20

    switch (m_dialogBoxes[11].GetMode())
    {
        case 0:
            if (onButton)
            {
                i = onButton - 1;
                if (m_pItemForSaleList[m_dialogBoxes[11].sView + i] != 0)
                {
                    if (!strcmp(m_pItemForSaleList[m_dialogBoxes[11].sView + i]->m_cName, "10Arrows"))
                    {
                        if (m_weight >= maxWeight - 1)
                        {
                            AddEventList(DLGBOX_CLICK_SHOP1, 10);
                            return;
                        }
                    }
                    else if (!strcmp(m_pItemForSaleList[m_dialogBoxes[11].sView + i]->m_cName, "100Arrows"))
                    {
                        if (m_weight >= maxWeight - 10)
                        {
                            AddEventList(DLGBOX_CLICK_SHOP1, 10);
                            return;
                        }
                    }

                    if (_iGetTotalItemNum() >= 75)
                    { // Bag Fix xRisenx
                        AddEventList(DLGBOX_CLICK_SHOP1, 10);//" anything because your bag is full."
                        return;
                    }

                    m_dialogBoxes[11].SetMode(m_dialogBoxes[11].sView + i + 1);
                    PlaySound('E', 14, 5);
                }
                return;
            }
            break;

        default:

            switch (onButton)
            {
                case 1: m_dialogBoxes[11].sV3 += 10;	break;
                case 2: m_dialogBoxes[11].sV3 -= 10;	break;
                case 3: m_dialogBoxes[11].sV3++;		break;
                case 4: m_dialogBoxes[11].sV3--;		break;
            }

            if (!strcmp(m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_cName, "100Arrows"))
            {
                isArrow = true;
                if (m_weight + (m_dialogBoxes[11].sV3 * 10) > maxWeight)
                {
                    m_dialogBoxes[11].sV3 = (maxWeight - m_weight) / 10;
                }
                if (m_dialogBoxes[11].sV3 > 99) m_dialogBoxes[11].sV3 = 99;
            }
            else if (!strcmp(m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_cName, "10Arrows"))
            {
                isArrow = true;
                if (m_weight + m_dialogBoxes[11].sV3 > maxWeight)
                {
                    m_dialogBoxes[11].sV3 = (maxWeight - m_weight - 1);
                }
                if (m_dialogBoxes[11].sV3 > 99) m_dialogBoxes[11].sV3 = 99;
            }
            else if (m_dialogBoxes[11].sV3 >= (75 - _iGetTotalItemNum()))
            { // Bag Fix xRisenx
                m_dialogBoxes[11].sV3 = (75 - _iGetTotalItemNum()); // Bag Fix xRisenx
            }
            if (m_dialogBoxes[11].sV3 <= 1)
            {
                m_dialogBoxes[11].sV3 = 1;
            }


            if (onButton == 5)
            {	// Purchase Click
                if (isArrow && _iGetTotalItemNum() >= 75) // Bag Fix xRisenx
                {
                    AddEventList(DLGBOX_CLICK_SHOP1, 10);
                }
                else if (!isArrow && ((75 - _iGetTotalItemNum()) < m_dialogBoxes[11].sV3))
                { // Bag Fix xRisenx
                    AddEventList(DLGBOX_CLICK_SHOP1, 10);//"You cannot buy anything because your bag is full."
                }
                else
                {
                    ZeroMemory(cTemp, sizeof(cTemp));
                    strcpy(cTemp, m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_cName);
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_PURCHASEITEM, 0, m_dialogBoxes[11].sV3, 0, 0, cTemp);
                }
                m_dialogBoxes[11].SetMode(0);
                m_dialogBoxes[11].sV3 = 1;
                PlaySound('E', 14, 5);
            }
            else if (onButton == 6)
            {	// Cancel Click
                m_dialogBoxes[11].SetMode(0);
                m_dialogBoxes[11].sV3 = 1;
                PlaySound('E', 14, 5);
            }
            break;
    }
}

void CGame::DlgBoxClick_Skill()
{
    char onButton = m_dialogBoxes[15].OnButton();
    short skillIndex = (onButton > 17) ? (onButton - 17) : onButton;

    if (m_dialogBoxes[15].GetMode() == 0)
    {
        for (int i = 0, skill = 0; i < MAXSKILLTYPE; i++)
        {
            if (m_pSkillCfgList[i + m_dialogBoxes[15].sView] != 0)
            { //FIXME
                if (++skill != skillIndex) continue;
                skillIndex = i;
                if (onButton >= 1 && onButton <= 17)
                {
                    if (m_pSkillCfgList[skillIndex + m_dialogBoxes[15].sView]->m_bIsUseable
                        && m_pSkillCfgList[skillIndex + m_dialogBoxes[15].sView]->m_iLevel != 0)
                    {
                        if (m_bSkillUsingStatus)
                        {
                            AddEventList(DLGBOX_CLICK_SKILL1, 10); // "You are already using other skill."
                            return;
                        }
                        if ((m_bCommandAvailable == false) || (m_iHP <= 0))
                        {
                            AddEventList(DLGBOX_CLICK_SKILL2, 10); // "You can't use a skill while you are moving."
                            return;
                        }
                        if (m_bIsGetPointingMode)
                            return;

                        switch (m_pSkillCfgList[skillIndex + m_dialogBoxes[15].sView]->m_cUseMethod)
                        {
                            case 0:
                            case 2:
                                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_USESKILL, 0, (skillIndex + m_dialogBoxes[15].sView), 0, 0, 0);
                                m_bSkillUsingStatus = true;
                                DisableDialogBox(15);
                                PlaySound('E', 14, 5);
                                break;
                        }
                    }
                }
                else if (onButton >= 18 && onButton <= 36)
                {
                    if (m_dialogBoxes[15].bFlag == false)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_SETDOWNSKILLINDEX, 0, skillIndex + m_dialogBoxes[15].sView, 0, 0, 0);
                        PlaySound('E', 14, 5);
                        m_dialogBoxes[15].bFlag = true;

                    }
                }
                break;
            }
        }
    }
}

void CGame::DlgBoxClick_SkillDlg()
{
    int i, iAdjX, iAdjY, buildItem;
    short sX, sY;

    sX = m_dialogBoxes[26].m_X;
    sY = m_dialogBoxes[26].m_Y;

    iAdjX = 5;
    iAdjY = 8;

    char onButton = m_dialogBoxes[26].OnButton();
    switch (m_dialogBoxes[26].GetMode())
    {
        case 1:
            if (onButton)
            {
                m_dialogBoxes[26].SetMode(2);
                m_dialogBoxes[26].cStr[0] = 1;
                m_dialogBoxes[26].dwT1 = m_dwCurTime;
                PlaySound('E', 14, 5);
                AddEventList(DLGBOX_CLICK_SKILLDLG1, 10); // "Mixing a potion..."
                PlaySound('E', 41, 0);
            }
            break;
            // Crafting
        case 7:
            if (onButton)
            {
                DebugLog("Tag1 DlgBoxClick_SkillDlg");
                if (m_dialogBoxes[26].sV1 == -1)
                {
                    AddEventList(DLGBOX_CLICK_SKILLDLG2, 10); // "There is not enough crafting materials. Please put in more materials."
                    PlaySound('E', 14, 5);
                }
                else
                {
                    m_dialogBoxes[26].SetMode(8);
                    m_dialogBoxes[26].dwT1 = m_dwCurTime;
                    m_dialogBoxes[26].cStr[1] = 1;
                    PlaySound('E', 14, 5);
                    AddEventList(DLGBOX_CLICK_SKILLDLG3, 10); // "Crafting the item in progress"
                    PlaySound('E', 51, 0);
                }
            }
            break;

        case 3:
            i = onButton - 1;
            if (onButton && onButton < 14)
            {
                if (m_pDispBuildItemList[i + m_dialogBoxes[26].sView] != 0)
                {
                    m_dialogBoxes[26].SetMode(4);
                    m_dialogBoxes[26].cStr[0] = buildItem = i + m_dialogBoxes[26].sView;
                    PlaySound('E', 14, 5);
                    if (m_manuAutoFill)
                    {
                        for (i = 0; i < MAXITEMS; i++)
                        {
                            if (m_pItemList[i] == 0) continue;
                            int usedCount = 0;
                            if (m_dialogBoxes[26].sV1 == -1 && m_pDispBuildItemList[buildItem]->m_iElementCount[1] &&
                                !strcmp(m_pDispBuildItemList[buildItem]->m_cElementName1, m_pItemList[i]->m_cName))
                            {
                                m_dialogBoxes[26].sV1 = i;
                                usedCount++;
                                if (m_pItemList[i]->m_dwCount == usedCount) continue;
                            }
                            if (m_dialogBoxes[26].sV2 == -1 && m_pDispBuildItemList[buildItem]->m_iElementCount[2] &&
                                !strcmp(m_pDispBuildItemList[buildItem]->m_cElementName2, m_pItemList[i]->m_cName))
                            {
                                m_dialogBoxes[26].sV2 = i;
                                usedCount++;
                                if (m_pItemList[i]->m_dwCount == usedCount) continue;
                            }
                            if (m_dialogBoxes[26].sV3 == -1 && m_pDispBuildItemList[buildItem]->m_iElementCount[3] &&
                                !strcmp(m_pDispBuildItemList[buildItem]->m_cElementName3, m_pItemList[i]->m_cName))
                            {
                                m_dialogBoxes[26].sV3 = i;
                                usedCount++;
                                if (m_pItemList[i]->m_dwCount == usedCount) continue;
                            }
                            if (m_dialogBoxes[26].sV4 == -1 && m_pDispBuildItemList[buildItem]->m_iElementCount[4] &&
                                !strcmp(m_pDispBuildItemList[buildItem]->m_cElementName4, m_pItemList[i]->m_cName))
                            {
                                m_dialogBoxes[26].sV4 = i;
                                usedCount++;
                                if (m_pItemList[i]->m_dwCount == usedCount) continue;
                            }
                            if (m_dialogBoxes[26].sV5 == -1 && m_pDispBuildItemList[buildItem]->m_iElementCount[5] &&
                                !strcmp(m_pDispBuildItemList[buildItem]->m_cElementName5, m_pItemList[i]->m_cName))
                            {
                                m_dialogBoxes[26].sV5 = i;
                                usedCount++;
                                if (m_pItemList[i]->m_dwCount == usedCount) continue;
                            }
                            if (m_dialogBoxes[26].sV6 == -1 && m_pDispBuildItemList[buildItem]->m_iElementCount[6] &&
                                !strcmp(m_pDispBuildItemList[buildItem]->m_cElementName6, m_pItemList[i]->m_cName))
                            {
                                m_dialogBoxes[26].sV6 = i;
                            }
                        }
                    }
                    m_dialogBoxes[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                }
            }
            if (onButton == 16)
            {
                m_manuAutoFill = !m_manuAutoFill;
                PlaySound('E', 14, 5);
            }
            break;

        case 4:
            iAdjX = -1;
            iAdjY = -7;
            if (m_pDispBuildItemList[m_dialogBoxes[26].cStr[0]]->m_bBuildEnabled == true)
            {
                if (onButton == 1)
                {
                    // Back
                    m_dialogBoxes[26].SetMode(3);
                    PlaySound('E', 14, 5);

                    if ((m_dialogBoxes[26].sV1 != -1) && (m_pItemList[m_dialogBoxes[26].sV1] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV1] = false;
                    if ((m_dialogBoxes[26].sV2 != -1) && (m_pItemList[m_dialogBoxes[26].sV2] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV2] = false;
                    if ((m_dialogBoxes[26].sV3 != -1) && (m_pItemList[m_dialogBoxes[26].sV3] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV3] = false;
                    if ((m_dialogBoxes[26].sV4 != -1) && (m_pItemList[m_dialogBoxes[26].sV4] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV4] = false;
                    if ((m_dialogBoxes[26].sV5 != -1) && (m_pItemList[m_dialogBoxes[26].sV5] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV5] = false;
                    if ((m_dialogBoxes[26].sV6 != -1) && (m_pItemList[m_dialogBoxes[26].sV6] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV6] = false;

                    m_dialogBoxes[26].sV1 = -1;
                    m_dialogBoxes[26].sV2 = -1;
                    m_dialogBoxes[26].sV3 = -1;
                    m_dialogBoxes[26].sV4 = -1;
                    m_dialogBoxes[26].sV5 = -1;
                    m_dialogBoxes[26].sV6 = -1;
                    m_dialogBoxes[26].cStr[0] = 0;
                    m_dialogBoxes[26].cStr[1] = 0;
                    m_dialogBoxes[26].cStr[4] = 0;
                }

                //if (onButton == 2)
                //{	// Manufacture
                if (m_dialogBoxes[26].cStr[4] == 1)
                {
                    m_dialogBoxes[26].SetMode(5);
                    m_dialogBoxes[26].cStr[1] = 0;
                    m_dialogBoxes[26].dwT1 = m_dwCurTime;
                    PlaySound('E', 14, 5);
                    PlaySound('E', 44, 0);
                }
                //}
            }
            else
            {
                if (onButton == 1) 	// Back
                {
                    m_dialogBoxes[26].SetMode(3);
                    PlaySound('E', 14, 5);

                    if ((m_dialogBoxes[26].sV1 != -1) && (m_pItemList[m_dialogBoxes[26].sV1] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV1] = false;
                    if ((m_dialogBoxes[26].sV2 != -1) && (m_pItemList[m_dialogBoxes[26].sV2] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV2] = false;
                    if ((m_dialogBoxes[26].sV3 != -1) && (m_pItemList[m_dialogBoxes[26].sV3] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV3] = false;
                    if ((m_dialogBoxes[26].sV4 != -1) && (m_pItemList[m_dialogBoxes[26].sV4] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV4] = false;
                    if ((m_dialogBoxes[26].sV5 != -1) && (m_pItemList[m_dialogBoxes[26].sV5] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV5] = false;
                    if ((m_dialogBoxes[26].sV6 != -1) && (m_pItemList[m_dialogBoxes[26].sV6] != 0))
                        m_bIsItemDisabled[m_dialogBoxes[26].sV6] = false;

                    m_dialogBoxes[26].sV1 = -1;
                    m_dialogBoxes[26].sV2 = -1;
                    m_dialogBoxes[26].sV3 = -1;
                    m_dialogBoxes[26].sV4 = -1;
                    m_dialogBoxes[26].sV5 = -1;
                    m_dialogBoxes[26].sV6 = -1;
                    m_dialogBoxes[26].cStr[0] = 0;
                    m_dialogBoxes[26].cStr[1] = 0;
                    m_dialogBoxes[26].cStr[4] = 0;
                }
            }
            break;

        case 6:
            iAdjX = -1;
            iAdjY = -7;
            //if (onButton == 1) {
            // Back
            m_dialogBoxes[26].SetMode(3);
            PlaySound('E', 14, 5);

            if ((m_dialogBoxes[26].sV1 != -1) && (m_pItemList[m_dialogBoxes[26].sV1] != 0))
                m_bIsItemDisabled[m_dialogBoxes[26].sV1] = false;
            if ((m_dialogBoxes[26].sV2 != -1) && (m_pItemList[m_dialogBoxes[26].sV2] != 0))
                m_bIsItemDisabled[m_dialogBoxes[26].sV2] = false;
            if ((m_dialogBoxes[26].sV3 != -1) && (m_pItemList[m_dialogBoxes[26].sV3] != 0))
                m_bIsItemDisabled[m_dialogBoxes[26].sV3] = false;
            if ((m_dialogBoxes[26].sV4 != -1) && (m_pItemList[m_dialogBoxes[26].sV4] != 0))
                m_bIsItemDisabled[m_dialogBoxes[26].sV4] = false;
            if ((m_dialogBoxes[26].sV5 != -1) && (m_pItemList[m_dialogBoxes[26].sV5] != 0))
                m_bIsItemDisabled[m_dialogBoxes[26].sV5] = false;
            if ((m_dialogBoxes[26].sV6 != -1) && (m_pItemList[m_dialogBoxes[26].sV6] != 0))
                m_bIsItemDisabled[m_dialogBoxes[26].sV6] = false;

            m_dialogBoxes[26].sV1 = -1;
            m_dialogBoxes[26].sV2 = -1;
            m_dialogBoxes[26].sV3 = -1;
            m_dialogBoxes[26].sV4 = -1;
            m_dialogBoxes[26].sV5 = -1;
            m_dialogBoxes[26].sV6 = -1;
            m_dialogBoxes[26].cStr[0] = 0;
            m_dialogBoxes[26].cStr[1] = 0;
            m_dialogBoxes[26].cStr[4] = 0;
            //}
            break;
    }
}

void CGame::DrawDialogBox_Commander()
{
    short sX, sY, szX, szY, MapSzX, MapSzY;
    uint32_t dwTime = G_dwGlobalTime;
    double dV1, dV2, dV3;
    int i, tX, tY;

    char onButton = m_dialogBoxes[36].OnButton();
    sX = m_dialogBoxes[36].m_X;
    sY = m_dialogBoxes[36].m_Y;
    szX = m_dialogBoxes[36].sSizeX;
    if ((dwTime - m_dwCommanderCommandRequestedTime) > 1000 * 10)
    {
        _RequestMapStatus("2ndmiddle", 3);
        _RequestMapStatus("2ndmiddle", 1);
        m_dwCommanderCommandRequestedTime = dwTime;
    }

    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX, sY - 5, 0, false, m_bDialogTrans); // Main image
    DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX + 1, sY, 15, false, m_bDialogTrans);

    switch (m_dialogBoxes[36].GetMode())
    {
        case 0: // Main dlg
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 340, 3, dwTime); // btn
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 340, 1, dwTime); // btn
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100, sY + 340, 2, dwTime); // btn
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150, sY + 340, 30, dwTime); // Btn TP
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 4, dwTime); // btn ?
                                                                                                           //PutAlignedString(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_COMMANDER1);//"Middleland warfare building structure situation"
            PutAlignedString(sX, sX + szX, sY + 37, DRAW_DIALOGBOX_COMMANDER1);//"Middleland warfare building structure situation"

            if (onButton == 1)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 340, 17, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER2, 255, 255, 255);//"Middleland teleport position set"
            }
            else if (onButton == 2)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 340, 15, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER3, 255, 255, 255);//"Teleport to the selected area"
            }
            else if (onButton == 3)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100, sY + 340, 16, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER4, 255, 255, 255);//"Recall warfare units"
            }
            else if (onButton == 4)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150, sY + 340, 24, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER5, 255, 255, 255);//"Middleland construction building set"
            }
            else if (onButton == 5)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 18, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER6, 255, 255, 255);//"Commander's duty help"
            }
            DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans);// Map
            break;

        case 1: // Set TP
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 340, 20, dwTime); //btn <-
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 4, dwTime);  //btn ?
            PutAlignedString(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_COMMANDER7); // "Click and select the teleport location."

            if (onButton == 2)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 340, 19, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER8, 255, 255, 255);
            }
            else if (onButton == 3)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 18, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER9, 255, 255, 255);
            }
            DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans);

            if (onButton == 1) // shows TP posit on map following the mouse
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY, 42, false, true);
            }
            break;

        case 2: // Use TP
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 340, 1, dwTime);       // Btn TP
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 340, 20, dwTime); // Btn <-
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 4, dwTime);  // Btn ?
            PutAlignedString(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_COMMANDER10); // "Teleport to the selected area"

            if (onButton == 1)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 340, 15, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER11, 255, 255, 255);
            }
            else if (onButton == 2)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 340, 19, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER12, 255, 255, 255);
            }
            else if (onButton == 3)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 18, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER13, 255, 255, 255);
            }
            DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans);
            break;

        case 3: // Choose summon
            if (m_side == ARESDEN)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 220, 6, dwTime);
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 220, 5, dwTime);
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100, sY + 220, 7, dwTime);
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150, sY + 220, 35, dwTime);
            }
            else if (m_side == ELVINE)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 220, 9, dwTime);
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 220, 8, dwTime);
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100, sY + 220, 7, dwTime);
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150, sY + 220, 35, dwTime);
            }
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 340, 20, dwTime);
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 4, dwTime);

            PutAlignedString(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_COMMANDER14); // "Recall warfare units"

            wsprintfA(G_cTxt, "%s %d", DRAW_DIALOGBOX_COMMANDER15, m_iConstructionPoint);
            PutAlignedString(sX, sX + 323, sY + 190, G_cTxt);

            if (m_side == ARESDEN)
            {
                if (onButton == 1)
                {
                    if (m_iConstructionPoint >= 3000)
                    {
                        m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 220, 11, dwTime);
                    }
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER16, 255, 255, 255);
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 50, DRAW_DIALOGBOX_COMMANDER17, 255, 255, 255);//"3000"
                }
                else if (onButton == 2)
                {
                    if (m_iConstructionPoint >= 2000)
                    {
                        m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 220, 10, dwTime);
                    }
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER18, 255, 255, 255);//"(Temple Knight)"
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 50, DRAW_DIALOGBOX_COMMANDER19, 255, 255, 255);//"2000"
                }
                else if (onButton == 3)
                {
                    if (m_iConstructionPoint >= 1000)
                    {
                        m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100, sY + 220, 12, dwTime);
                    }
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER20, 255, 255, 255);//"(Light War Beetle)"
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 50, DRAW_DIALOGBOX_COMMANDER21, 255, 255, 255);//" 1000"
                }
                else if (onButton == 4)
                {
                    if (m_iConstructionPoint >= 5000)
                    {
                        m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150, sY + 220, 29, dwTime);
                    }
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER22, 255, 255, 255);//"(Catapult)"
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 50, DRAW_DIALOGBOX_COMMANDER23, 255, 255, 255);// 1500"
                }
                else if (onButton == 5)
                {
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER24, 255, 255, 255);
                }
                else if (onButton == 6)
                {
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER25, 255, 255, 255);
                }
                else if (onButton == 7)
                {
                    m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 340, 19, dwTime);
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER26, 255, 255, 255);
                }
                else if (onButton == 8)
                {
                    m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 18, dwTime);
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER27, 255, 255, 255); //"Warfare units recall related help"
                }
            }
            else if (m_side == ELVINE)
            {
                if (onButton == 1)
                {
                    if (m_iConstructionPoint >= 3000)
                    {
                        m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 220, 14, dwTime);
                    }
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER28, 255, 255, 255);//"(God's Hand Knight Cavalry)"
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 50, DRAW_DIALOGBOX_COMMANDER29, 255, 255, 255);//"3000"
                }
                else if (onButton == 2)
                {
                    if (m_iConstructionPoint >= 2000)
                    {
                        m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 220, 13, dwTime);
                    }
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER30, 255, 255, 255);
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 50, DRAW_DIALOGBOX_COMMANDER31, 255, 255, 255);//"2000"
                }
                else if (onButton == 3)
                {
                    if (m_iConstructionPoint >= 1000)
                    {
                        m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100, sY + 220, 12, dwTime);
                    }
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER32, 255, 255, 255);//"(Light War Beetle)"
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 50, DRAW_DIALOGBOX_COMMANDER33, 255, 255, 255);//" 1000"
                }
                else if (onButton == 4)
                {
                    if (m_iConstructionPoint >= 5000)
                    {
                        m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150, sY + 220, 29, dwTime);
                    }
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER34, 255, 255, 255);//"(Catapult)"
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 50, DRAW_DIALOGBOX_COMMANDER35, 255, 255, 255);//" 1500"
                }
                else if (onButton == 5)
                {
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER36, 255, 255, 255);
                }
                else if (onButton == 6)
                {
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER37, 255, 255, 255);
                }
                else if (onButton == 7)
                {
                    m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 340, 19, dwTime);
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER38, 255, 255, 255);

                }
                else if (onButton == 8)
                {
                    m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 18, dwTime);
                    PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER39, 255, 255, 255);
                }
            }
            PutAlignedString(sX, sX + 323, sY + 80, DRAW_DIALOGBOX_COMMANDER40);
            PutAlignedString(sX, sX + 323, sY + 95, DRAW_DIALOGBOX_COMMANDER41);
            PutAlignedString(sX, sX + 323, sY + 110, DRAW_DIALOGBOX_COMMANDER42);

            switch (m_dialogBoxes[36].sV1)
            {
                case 0:
                    PutAlignedString(sX, sX + 323, sY + 140, DRAW_DIALOGBOX_COMMANDER43, 255, 255, 255);//"Guard Mode"
                    PutAlignedString(sX, sX + 323, sY + 160, DRAW_DIALOGBOX_COMMANDER44, 4, 0, 50);//"Hold Mode"
                    break;
                case 1:
                    PutAlignedString(sX, sX + 323, sY + 140, DRAW_DIALOGBOX_COMMANDER43, 4, 0, 50);//"Guard Mode"
                    PutAlignedString(sX, sX + 323, sY + 160, DRAW_DIALOGBOX_COMMANDER44, 255, 255, 255);//"Hold Mode"
                    break;
            }
            break;

        case 4: // Set Construction point
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 340, 20, dwTime);
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 4, dwTime);
            PutAlignedString(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_COMMANDER47); // "Select the construction area and click."

            if (onButton == 1)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 340, 19, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER48, 255, 255, 255); // "Returns to commander's main menu"
            }
            else if (onButton == 2)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 18, dwTime);
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_COMMANDER49, 255, 255, 255); // "Construction area set related help"
            }
            DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21);
            if ((G_pGame->m_stMCursor.sX >= sX + 15) && (G_pGame->m_stMCursor.sX <= sX + 15 + 278)
                && (G_pGame->m_stMCursor.sY >= sY + 60) && (G_pGame->m_stMCursor.sY <= sY + 60 + 272)) // shows posit on map following the mouse
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY, 41, false, true);
            }
            break;
    }

    switch (m_dialogBoxes[36].GetMode())
    {
        case 0: // Main
        case 1: // Set TP
        case 2: // Use TP
        case 4: // Set construc
            szX = 0;
            szY = 0;
            MapSzX = 0;
            MapSzY = 0;
            if (strcmp(m_cStatusMapName, "aresden") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "elvine") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "middleland") == 0)
            {
                szX = 279;
                szY = 280;	  // Fix for ML
                MapSzX = 524; // Fix for ML
                MapSzY = 524; // Fix for ML
            }
            else if (strcmp(m_cStatusMapName, "2ndmiddle") == 0)
            {
                szX = 125; // 279 -> 125
                szY = 125; // 280 -> 125
                MapSzX = 250; // 524 -> 250
                MapSzY = 250; // 524 -> 250
            }
            if (szX != 0)
            {
                for (i = 0; i < MAXCRUSADESTRUCTURES; i++)
                    if (m_stCrusadeStructureInfo[i].cType != 0)
                    {
                        dV1 = (double)MapSzX;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sX;
                        dV3 = (dV2*(double)szX) / dV1;
                        tX = (int)dV3;
                        dV1 = (double)MapSzY;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sY;
                        dV3 = (dV2*(double)szY) / dV1;
                        tY = (int)dV3;
                        switch (m_stCrusadeStructureInfo[i].cType)
                        {
                            case 38:
                                if (m_stCrusadeStructureInfo[i].cSide == 1)
                                    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 39, false, true); // Base
                                else DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 37, false, true);
                                break;
                            case 36:
                            case 37:
                            case 39:
                                if (m_stCrusadeStructureInfo[i].cSide == 1)
                                    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 38, false, true); // Base
                                else DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 36, false, true);
                                break;
                            case 42:
                                DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 40); // ManaStone
                                break;
                        }
                    }
                if (m_iTeleportLocX != -1)
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iTeleportLocX;
                    dV3 = (dV2*(double)szX) / dV1;
                    tX = (int)dV3;
                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iTeleportLocY;
                    dV3 = (dV2*(double)szY) / dV1;
                    tY = (int)dV3;
                    if ((m_dialogBoxes[36].GetMode() == 1) && (tY >= 30) && (tY <= 494)) // Place the TP
                    {
                        DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, false, true); //TP
                    }
                    else
                        DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, false, true); //TP
                }
                if ((m_dialogBoxes[36].GetMode() != 2) && (m_iConstructLocX != -1))
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iConstructLocX;
                    dV3 = (dV2*(double)szX) / dV1;
                    tX = (int)dV3;
                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iConstructLocY;
                    dV3 = (dV2*(double)szY) / dV1;
                    tY = (int)dV3;
                    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 41, false, true); // Contr pt
                }
                if (strcmp(m_cMapName, "2ndmiddle") == 0)
                {
                    dV1 = (double)MapSzX;//m_pMapData->m_sMapSizeX;
                    dV2 = (double)m_sPlayerX;
                    dV3 = (dV2*(double)szX) / dV1;
                    tX = (int)dV3;
                    dV1 = (double)MapSzY;//m_pMapData->m_sMapSizeY;
                    dV2 = (double)m_sPlayerY;
                    dV3 = (dV2*(double)szY) / dV1;
                    tY = (int)dV3;
                    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 43); // Player place
                }
            }
            if (m_dialogBoxes[36].GetMode() != 3)
            {
                if ((G_pGame->m_stMCursor.sX >= sX + 15) && (G_pGame->m_stMCursor.sX <= sX + 15 + (MapSzX)) // 278 -> (MapSzX-30)
                    && (G_pGame->m_stMCursor.sY >= sY + 60) && (G_pGame->m_stMCursor.sY <= sY + 60 + (MapSzY))) // 272 -> (MapSzY-30) Draw mouse coordinates
                {
                    dV1 = (double)(G_pGame->m_stMCursor.sX - (sX + 15));
                    dV2 = (double)MapSzX;//(752.0f);
                    dV3 = (dV2*dV1) / szX;//280.0f;
                    tX = (int)dV3;
                    dV1 = (double)(G_pGame->m_stMCursor.sY - (sY + 60));
                    dV2 = (double)MapSzX;//(680.0f);
                    dV3 = (dV2*dV1) / szY;//253.0f;
                    tY = (int)dV3;
                    if (tX < 30) tX = 30;
                    if (tY < 30) tY = 30;
                    if (tX > MapSzX - 30) tX = MapSzX - 30;
                    if (tY > MapSzY - 30) tY = MapSzY - 30;
                    wsprintfA(G_cTxt, "%d,%d", tX, tY);
                    PutString_SprFont3(G_pGame->m_stMCursor.sX + 10, G_pGame->m_stMCursor.sY - 10, G_cTxt, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, false, 2);
                }
            }
            break;
    }
}

void CGame::DrawDialogBox_Constructor()
{
    short sX, sY, szX, szY, MapSzX, MapSzY;
    uint32_t dwTime = G_dwGlobalTime;
    double dV1, dV2, dV3;
    int tX, tY;
    char cMapName[12];

    char onButton = m_dialogBoxes[37].OnButton();
    sX = m_dialogBoxes[37].m_X;
    sY = m_dialogBoxes[37].m_Y;
    szX = m_dialogBoxes[37].sSizeX;
    if ((dwTime - m_dwCommanderCommandRequestedTime) > 1000 * 10)
    {
        _RequestMapStatus("2ndmiddle", 3);
        _RequestMapStatus("2ndmiddle", 1);
        m_dwCommanderCommandRequestedTime = dwTime;
    }

    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX, sY - 5, 0, false, m_bDialogTrans); // Main image
    DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 16, false, m_bDialogTrans);

    switch (m_dialogBoxes[37].GetMode())
    {
        case 0: // Main dlg
            if (m_iConstructLocX != -1)
            {
                ZeroMemory(cMapName, sizeof(cMapName));
                GetOfficialMapName(m_cConstructMapName, cMapName);
                wsprintfA(G_cTxt, DRAW_DIALOGBOX_CONSTRUCTOR1, cMapName, m_iConstructLocX, m_iConstructLocY);
                PutAlignedString(sX, sX + szX, sY + 40, G_cTxt);
            }
            else PutAlignedString(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_CONSTRUCTOR2); //"Construction unavailable: Unable to construct"
            DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans); // Map

            if (onButton == 1)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 340, 24, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 340, 30, dwTime);

            if (onButton == 2)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 340, 15, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 340, 1, dwTime);

            if (onButton == 3)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 18, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 4, dwTime);

            if (onButton == 1)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR3, 255, 255, 255);
            }
            else if (onButton == 2)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR4, 255, 255, 255);
            }
            else if (onButton == 3)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR5, 255, 255, 255);
            }
            break;

        case 1: // Select building
            PutAlignedString(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_CONSTRUCTOR6); //"Select war building"
            PutAlignedString(sX, sX + 323, sY + 80, DRAW_DIALOGBOX_CONSTRUCTOR7); //"Constructs the war building here."
            PutAlignedString(sX, sX + 323, sY + 95, DRAW_DIALOGBOX_CONSTRUCTOR8); //"Press the building icon and"
            PutAlignedString(sX, sX + 323, sY + 110, DRAW_DIALOGBOX_CONSTRUCTOR9); //"the base will be formed and slowly finished"
            PutAlignedString(sX, sX + 323, sY + 125, DRAW_DIALOGBOX_CONSTRUCTOR10);//"Attack the building with Pickaxe."
            PutAlignedString(sX, sX + 323, sY + 140, DRAW_DIALOGBOX_CONSTRUCTOR11);//"Building is not activated"
            PutAlignedString(sX, sX + 323, sY + 155, DRAW_DIALOGBOX_CONSTRUCTOR12);//"while constructing."

            if (onButton == 1)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 220, 27, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 220, 33, dwTime);

            if (onButton == 2)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 220, 28, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 220, 34, dwTime);

            if (onButton == 3)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100, sY + 220, 26, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100, sY + 220, 32, dwTime);

            if (onButton == 4)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150, sY + 220, 25, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150, sY + 220, 31, dwTime);

            if (onButton == 5)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 322, 19, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 322, 20, dwTime);

            if (onButton == 6)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 322, 18, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 322, 4, dwTime);

            if (onButton == 1)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR13, 255, 255, 255);//"(Mana Collector)"
            }
            else if (onButton == 2)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR14, 255, 255, 255);//"(Detector)"
            }
            else if (onButton == 3)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR15, 255, 255, 255);//"(Arrow Guard Tower)"
            }
            else if (onButton == 4)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR16, 255, 255, 255);//(Cannon Guard Tower)"
            }
            else if (onButton == 5)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR17, 255, 255, 255);
            }
            else if (onButton == 6)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR18, 255, 255, 255);
            }
            break;

        case 2: // Teleport
            PutAlignedString(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_CONSTRUCTOR19); //"Teleport to position that commander pointed"
            DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans); // map

            if (onButton == 1)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 340, 15, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 50, sY + 340, 1, dwTime);

            if (onButton == 2)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 340, 19, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 100 + 74, sY + 340, 20, dwTime);

            if (onButton == 3)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 18, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 4, dwTime);

            if (onButton == 1)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR20, 255, 255, 255);
            }
            else if (onButton == 2)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR21, 255, 255, 255);
            }
            else if (onButton == 3)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_CONSTRUCTOR22, 255, 255, 255);
            }
            break;
    }

    switch (m_dialogBoxes[37].GetMode())
    {
        case 0: // Main dlg
        case 2: // TP
            szX = 0;
            szY = 0;
            MapSzX = 0;
            MapSzY = 0;
            if (strcmp(m_cStatusMapName, "aresden") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "elvine") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "middleland") == 0)
            {
                szX = 279;
                szY = 280;	  // Fix for ML
                MapSzX = 524; // Fix for ML
                MapSzY = 524; // Fix for ML
            }
            else if (strcmp(m_cStatusMapName, "2ndmiddle") == 0)
            {
                szX = 125; // 279 -> 125
                szY = 125; // 280 -> 125
                MapSzX = 250; // 524 -> 250
                MapSzY = 250; // 524 -> 250
            }
            if (szX != 0)
            {
                for (int i = 0; i < MAXCRUSADESTRUCTURES; i++)
                    if (m_stCrusadeStructureInfo[i].cType == 42)
                    {
                        dV1 = (double)MapSzX;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sX;
                        dV3 = (dV2*(double)szX) / dV1;
                        tX = (int)dV3;
                        dV1 = (double)MapSzY;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sY;
                        dV3 = (dV2*(double)szY) / dV1;
                        tY = (int)dV3;
                        switch (m_stCrusadeStructureInfo[i].cType)
                        {
                            case 42:
                                DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 40); // ManaStone
                                break;
                        }
                    }
                if (m_iTeleportLocX != -1)
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iTeleportLocX;
                    dV3 = (dV2*(double)szX) / dV1;
                    tX = (int)dV3;
                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iTeleportLocY;
                    dV3 = (dV2*(double)szY) / dV1;
                    tY = (int)dV3;
                    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, false, true); //TP
                }
                if ((m_dialogBoxes[37].GetMode() != 2) && (m_iConstructLocX != -1))
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iConstructLocX;
                    dV3 = (dV2*(double)szX) / dV1;
                    tX = (int)dV3;
                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iConstructLocY;
                    dV3 = (dV2*(double)szY) / dV1;
                    tY = (int)dV3;
                    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 41, false, true); // Contr pt
                }
                if (strcmp(m_cMapName, "2ndmiddle") == 0)
                {
                    dV1 = (double)MapSzX;//m_pMapData->m_sMapSizeX;
                    dV2 = (double)m_sPlayerX;
                    dV3 = (dV2*(double)szX) / dV1;
                    tX = (int)dV3;
                    dV1 = (double)MapSzY;//m_pMapData->m_sMapSizeY;
                    dV2 = (double)m_sPlayerY;
                    dV3 = (dV2*(double)szY) / dV1;
                    tY = (int)dV3;
                    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 43); // Player place
                }
            }
            if ((G_pGame->m_stMCursor.sX >= sX + 15) && (G_pGame->m_stMCursor.sX <= sX + 15 + (MapSzX)) // 278 -> (MapSzX-30)
                && (G_pGame->m_stMCursor.sY >= sY + 60) && (G_pGame->m_stMCursor.sY <= sY + 60 + (MapSzY))) // 272 -> (MapSzY-30) Draw mouse coordinates
            {
                dV1 = (double)(G_pGame->m_stMCursor.sX - (sX + 15));
                dV2 = (double)MapSzX;//(752.0f);
                dV3 = (dV2*dV1) / szX;//280.0f;
                tX = (int)dV3;
                dV1 = (double)(G_pGame->m_stMCursor.sY - (sY + 60));
                dV2 = (double)MapSzX;//(680.0f);
                dV3 = (dV2*dV1) / szY;//253.0f;
                tY = (int)dV3;
                if (tX < 30) tX = 30;
                if (tY < 30) tY = 30;
                if (tX > MapSzX - 30) tX = MapSzX - 30;
                if (tY > MapSzY - 30) tY = MapSzY - 30;
                wsprintfA(G_cTxt, "%d,%d", tX, tY);
                PutString_SprFont3(G_pGame->m_stMCursor.sX + 10, G_pGame->m_stMCursor.sY - 10, G_cTxt, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, false, 2);
            }
            break;
    }
}

void CGame::DrawDialogBox_Soldier()
{
    short sX, sY, szX, szY, MapSzX, MapSzY;
    uint32_t dwTime = G_dwGlobalTime;
    char cMapName[120];
    double dV1, dV2, dV3;
    int tX, tY;

    char onButton = m_dialogBoxes[38].OnButton();
    sX = m_dialogBoxes[38].m_X;
    sY = m_dialogBoxes[38].m_Y;
    szX = m_dialogBoxes[38].sSizeX;
    if ((dwTime - m_dwCommanderCommandRequestedTime) > 1000 * 10)
    {
        _RequestMapStatus("2ndmiddle", 3);
        _RequestMapStatus("2ndmiddle", 1);
        m_dwCommanderCommandRequestedTime = dwTime;
    }

    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX, sY - 5, 0, false, m_bDialogTrans);
    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans);
    DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX + 1, sY, 17, false, m_bDialogTrans); // Crusade Soldier Menu Text

    switch (m_dialogBoxes[38].GetMode())
    {
        case 0: // Main dlg, Map
            if (m_iTeleportLocX != -1)
            {
                ZeroMemory(cMapName, sizeof(cMapName));
                GetOfficialMapName(m_cTeleportMapName, cMapName);
                wsprintfA(G_cTxt, DRAW_DIALOGBOX_SOLDIER1, cMapName, m_iTeleportLocX, m_iTeleportLocY);
                PutAlignedString(sX, sX + szX, sY + 40, G_cTxt);					// "Teleport position: %s near %d, %d"
            }
            else PutAlignedString(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_SOLDIER2); //"Teleport position is not set."

            if (onButton == 1)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 340, 15, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 340, 1, dwTime);

            if (onButton == 2)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 18, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 4, dwTime);

            if (onButton == 1)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_SOLDIER3, 255, 255, 255); // "Teleport to position that commander pointed"
            }
            else if (onButton == 2)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_SOLDIER4, 255, 255, 255); // "Help for soldier"
            }
            break;

        case 1: // TP now
            PutAlignedString(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_SOLDIER5); // "Teleport to position that commander pointed"
            if (onButton == 1)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 340, 15, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20, sY + 340, 1, dwTime);

            if (onButton == 2)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74 - 50, sY + 340, 19, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74 - 50, sY + 340, 20, dwTime);

            if (onButton == 3)
            {
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 18, dwTime);
            }
            else m_pSprite[SPRID_INTERFACE_ND_CRUSADE]->PutSpriteFast(sX + 20 + 150 + 74, sY + 340, 4, dwTime);

            if (onButton == 1)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_SOLDIER6, 255, 255, 255);
            }
            else if (onButton == 2)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_SOLDIER7, 255, 255, 255);
            }
            else if (onButton == 3)
            {
                PutString2(G_pGame->m_stMCursor.sX + 20, G_pGame->m_stMCursor.sY + 35, DRAW_DIALOGBOX_SOLDIER8, 255, 255, 255);//"Help for teleport"
            }
            break;
    }

    switch (m_dialogBoxes[38].GetMode())
    {
        case 0: // Main
        case 1: // TP
            szX = 0;
            szY = 0;
            MapSzX = 0;
            MapSzY = 0;
            if (strcmp(m_cStatusMapName, "aresden") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "elvine") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "middleland") == 0)
            {
                szX = 279;
                szY = 280;	  // Fix for ML
                MapSzX = 524; // Fix for ML
                MapSzY = 524; // Fix for ML
            }
            else if (strcmp(m_cStatusMapName, "2ndmiddle") == 0)
            {
                szX = 125; // 279 -> 125
                szY = 125; // 280 -> 125
                MapSzX = 250; // 524 -> 250
                MapSzY = 250; // 524 -> 250
            }
            if (szX != 0)
            {
                for (int i = 0; i < MAXCRUSADESTRUCTURES; i++)
                    if (m_stCrusadeStructureInfo[i].cType == 42)
                    {
                        dV1 = (double)MapSzX;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sX;
                        dV3 = (dV2*(double)szX) / dV1;
                        tX = (int)dV3;
                        dV1 = (double)MapSzY;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sY;
                        dV3 = (dV2*(double)szY) / dV1;
                        tY = (int)dV3;
                        switch (m_stCrusadeStructureInfo[i].cType)
                        {
                            case 42:
                                DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 40); // ManaStone
                                break;
                        }
                    }
                if (m_iTeleportLocX != -1)
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iTeleportLocX;
                    dV3 = (dV2*(double)szX) / dV1;
                    tX = (int)dV3;
                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iTeleportLocY;
                    dV3 = (dV2*(double)szY) / dV1;
                    tY = (int)dV3;
                    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, false, true); //TP
                }
                if (strcmp(m_cMapName, "2ndmiddle") == 0)
                {
                    dV1 = (double)MapSzX;//m_pMapData->m_sMapSizeX;
                    dV2 = (double)m_sPlayerX;
                    dV3 = (dV2*(double)szX) / dV1;
                    tX = (int)dV3;
                    dV1 = (double)MapSzY;//m_pMapData->m_sMapSizeY;
                    dV2 = (double)m_sPlayerY;
                    dV3 = (dV2*(double)szY) / dV1;
                    tY = (int)dV3;
                    DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 43); // Player place
                }
            }
            if ((G_pGame->m_stMCursor.sX >= sX + 15) && (G_pGame->m_stMCursor.sX <= sX + 15 + (MapSzX)) // 278 -> (MapSzX-30)
                && (G_pGame->m_stMCursor.sY >= sY + 60) && (G_pGame->m_stMCursor.sY <= sY + 60 + (MapSzY))) // 272 -> (MapSzY-30) Draw mouse coordinates
            {
                dV1 = (double)(G_pGame->m_stMCursor.sX - (sX + 15));
                dV2 = (double)MapSzX;//(752.0f);
                dV3 = (dV2*dV1) / szX;//280.0f;
                tX = (int)dV3;
                dV1 = (double)(G_pGame->m_stMCursor.sY - (sY + 60));
                dV2 = (double)MapSzX;//(680.0f);
                dV3 = (dV2*dV1) / szY;//253.0f;
                tY = (int)dV3;
                if (tX < 30) tX = 30;
                if (tY < 30) tY = 30;
                if (tX > MapSzX - 30) tX = MapSzX - 30;
                if (tY > MapSzY - 30) tY = MapSzY - 30;
                wsprintfA(G_cTxt, "%d,%d", tX, tY);
                PutString_SprFont3(G_pGame->m_stMCursor.sX + 10, G_pGame->m_stMCursor.sY - 10, G_cTxt, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, false, 2);
            }
            break;
    }
}

void CGame::DrawDialogBox_Help()
{
    short sX, sY, szX;
    char onButton = m_dialogBoxes[35].OnButton();
    sX = m_dialogBoxes[35].m_X;
    sY = m_dialogBoxes[35].m_Y;

    //uncomment
    // 	if (onButton == 1)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(1), DRAW_DIALOGBOX_HELP2,video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(1), DRAW_DIALOGBOX_HELP2,video::SColor(255,4,0,50));
    // 	if (onButton == 2)
    // 		 PutAlignedString(&m_dialogBoxes[35].GetButton(2), DRAW_DIALOGBOX_HELP1,video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(2), DRAW_DIALOGBOX_HELP1,video::SColor(255,4,0,50));
    // 	if (onButton == 3)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(3), DRAW_DIALOGBOX_HELP3,video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(3), DRAW_DIALOGBOX_HELP3,video::SColor(255,4,0,50));
    // 	if (onButton == 4)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(4), DRAW_DIALOGBOX_HELP4,RGB(255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(4), DRAW_DIALOGBOX_HELP4,video::SColor(255,4,0,50));
    // 	if (onButton == 5)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(5), DRAW_DIALOGBOX_HELP5,RGB(255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(5), DRAW_DIALOGBOX_HELP5,video::SColor(255,4,0,50));
    // 	if (onButton == 6)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(6), DRAW_DIALOGBOX_HELP6,video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(6), DRAW_DIALOGBOX_HELP6,video::SColor(255,4,0,50));
    // 	if (onButton == 7)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(7), DRAW_DIALOGBOX_HELP7,video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(7), DRAW_DIALOGBOX_HELP7,video::SColor(255,4,0,50));
    // 	if (onButton == 8)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(8), DRAW_DIALOGBOX_HELP8,video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(8), DRAW_DIALOGBOX_HELP8,video::SColor(255,4,0,50));
    // 	if (onButton == 9)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(9), DRAW_DIALOGBOX_HELP9,video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(9), DRAW_DIALOGBOX_HELP9,video::SColor(255,4,0,50));
    // 	if (onButton == 10)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(10), DRAW_DIALOGBOX_HELP10,video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(10), DRAW_DIALOGBOX_HELP10,video::SColor(255,4,0,50));
    // 	if (onButton == 11)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(11), DRAW_DIALOGBOX_HELP11,video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(11), DRAW_DIALOGBOX_HELP11,video::SColor(255,4,0,50));
    // 	if (onButton == 12)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(12), DRAW_DIALOGBOX_HELP12,video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(12), DRAW_DIALOGBOX_HELP12,video::SColor(255,4,0,50));
    // 	if (onButton == 13)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(13), "F.A.Q.",video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(13), "F.A.Q.",video::SColor(255,4,0,50));
    // 	if (onButton == 14)
    // 		PutAlignedString(&m_dialogBoxes[35].GetButton(14), DRAW_DIALOGBOX_HELP13,video::SColor(255,255,255,255));
    // 	else PutAlignedString(&m_dialogBoxes[35].GetButton(14),DRAW_DIALOGBOX_HELP13,RGB(4,0,50));

    if (onButton == 15)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
}

void CGame::DrawDialogBox_Mailbox()
{
    Point pointerLoc;
    std::vector<CItem*> * items;

    CDialogBox & dlg = m_dialogBoxes[DIALOG_MAILBOX];
    char onButton = dlg.OnButton();
    short sX = dlg.m_X;
    short sY = dlg.m_Y;
    short szX = dlg.sSizeX;
    bool viewing = false;
    if (isItemLoaded == false)
    {
        _LoadItemContents();		// Item Display - ShadowEvil
        isItemLoaded = true;
    }
    isCommon = isUncommon = isRare = isEpic = isLegendary = false;
    PutString_SprFont(sX + 102, sY + 13, DRAW_DIALOGBOX_MAILBOX, 7, 0, 0);
    //PutString_SprFont2(sX + 102, sY + 13, DRAW_DIALOGBOX_MAILBOX, 210, 30, 0);

    switch (dlg.GetMode())
    {
        case 0:
            if (m_mails.size() > 16)
            {
                Point pointerLoc = dlg.HandleScroll(16, m_mails.size());

                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
            }

            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX1, SEND_MAIL_COST);
            //uncomment		ButtonString(dlg, 1, G_cTxt);

            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX18, m_mails.size());
            PutAlignedString(sX + 161, sX + szX, sY + 35, G_cTxt);

            PutAlignedString(sX - 20, sX + szX / 2, sY + 55, DRAW_DIALOGBOX_MAILBOX2);
            PutAlignedString(sX + 56, sX + szX, sY + 55, DRAW_DIALOGBOX_MAILBOX3);
            PutAlignedString(sX + 158, sX + szX, sY + 55, DRAW_DIALOGBOX_MAILBOX4);

            for (int i = 0; i < 16; i++)
            {
                int idx = i + dlg.sView;
                if (idx < m_mails.size())
                {
                    idx = m_mails.size() - 1 - idx;
                    PutAlignedString(sX + 25, sX + szX / 3 * 2, sY + 75 + 15 * i, m_mails[idx]->title.c_str(), (onButton == i + 3));
                    PutAlignedString(sX + 56, sX + szX, sY + 75 + 15 * i, m_mails[idx]->sender.c_str(), (onButton == i + 3));
                    wsprintfA(G_cTxt, "%.2u/%.2u", m_mails[idx]->date.day, m_mails[idx]->date.month);
                    PutAlignedString(sX + 158, sX + szX, sY + 75 + 15 * i, G_cTxt, (onButton == i + 3));
                }
            }
            break;

        case 1:
            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX29, SEND_MAIL_COST + dlg.vvec.size() * SEND_MAIL_ITEM_COST);
            PutAlignedString(sX + 150, sX + szX, sY + 35, G_cTxt);

            //uncomment		ButtonString(dlg, 1, DRAW_DIALOGBOX_MAILBOX5, POS_LEFT);
            PutAlignedString(dlg.GetButton(1).left + 20, dlg.GetButton(1).right, dlg.GetButton(1).top - 1, dlg.cStr, false);

            //uncomment		ButtonString(dlg, 2, DRAW_DIALOGBOX_MAILBOX6, POS_LEFT);
            PutAlignedString(dlg.GetButton(2).left + 30, dlg.GetButton(2).right, dlg.GetButton(2).top - 1, dlg.cStr2, false);

            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX7, strlen(dlg.txt), MAX_MAIL_MSG_LENGTH - 1);
            //uncomment		ButtonString(dlg, 3, G_cTxt, POS_LEFT);
            //uncomment PutAlignedString(dlg.GetButton(3).left, dlg.GetButton(3).right, dlg.GetButton(3).top + 15, dlg.txt, false, true);

            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX8, dlg.vvec.size());
            //uncomment		ButtonString(dlg, 4, G_cTxt);

            //uncomment		if(!dlg.bFlag)
            //uncomment			ButtonString(dlg, 5, DRAW_DIALOGBOX_MAILBOX9);
            //uncomment		else
            //uncomment			ButtonString(dlg, 5, DRAW_DIALOGBOX_MAILBOX10);

            //uncomment		ButtonString(dlg, 6, DRAW_DIALOGBOX_MAILBOX11);
            break;

        case 2:
            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX12, m_mails[dlg.sV1]->sender.c_str());
            //uncomment PutLabel(dlg.GetButton(1), G_cTxt, POS_LEFT);

            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX17, m_mails[dlg.sV1]->title.c_str());
            //uncomment PutLabel(dlg.GetButton(2), G_cTxt, POS_LEFT);

            sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX13, strlen(dlg.txt), MAX_MAIL_MSG_LENGTH - 1);
            //uncomment PutLabel(dlg.GetButton(3), G_cTxt, POS_LEFT);

            if (m_mails[dlg.sV1]->msg)
                //uncomment PutAlignedString(dlg.GetButton(3).left, dlg.GetButton(3).right, dlg.GetButton(3).top + 15, m_mails[dlg.sV1]->msg->c_str(), false, true);

                if (m_mails[dlg.sV1]->items.size() > 0)
                {
                    sprintf(G_cTxt, DRAW_DIALOGBOX_MAILBOX14, m_mails[dlg.sV1]->items.size());
                    //uncomment			ButtonString(dlg, 4, G_cTxt);
                }

            //uncomment		ButtonString(dlg, 5, DRAW_DIALOGBOX_MAILBOX15);
            //uncomment		ButtonString(dlg, 6, DRAW_DIALOGBOX_MAILBOX16);
            break;

        case 3:
            PutAlignedString(sX, sX + szX, sY + 45, DRAW_DIALOGBOX_MAILBOX24);
            PutAlignedString(sX, sX + szX, sY + 60, DRAW_DIALOGBOX_MAILBOX25);
            PutAlignedString(sX, sX + szX, sY + 75, DRAW_DIALOGBOX_MAILBOX26);
            wsprintfA(G_cTxt, DRAW_DIALOGBOX_MAILBOX27, dlg.vvec.size(), MAX_MAIL_ITEMS);
            PutAlignedString(sX, sX + szX, sY + 90, G_cTxt, 255, 255, 255);

            for (int i = 0; i < 10; i++)
            {
                if (i < dlg.vvec.size())
                {
                    const char * itemname = GetItemName(m_pItemList[dlg.vvec[i].index], true)->begin()->c_str();
                    //uncomment				if(dlg.vvec[i].count == 1)
                    //uncomment					ButtonString(dlg, i+1, itemname);
                    //uncomment				else {
                    //uncomment					wsprintfA(G_cTxt, "%u %s", dlg.vvec[i].count, itemname);
                    //uncomment					ButtonString(dlg, i+1, G_cTxt);
                    //uncomment				}
                }
            }

            //uncomment		ButtonString(dlg, 11, DRAW_DIALOGBOX_MAILBOX28);
            //uncomment		ButtonString(dlg, 12, DRAW_DIALOGBOX_MAILBOX11);
            break;

        case 4:
            //uncomment		ButtonString(dlg, 15, DRAW_DIALOGBOX_MAILBOX16);

            if (isItemLoaded == false)
            {
                _LoadItemContents();		// Item Display - ShadowEvil
                isItemLoaded = true;
            }
            isCommon = isUncommon = isRare = isEpic = isLegendary = false;


            items = &m_mails[dlg.sV1]->items;

            for (int i = 0;
                 i < 13, i + dlg.sView < items->size();
                 i++)
            {
                std::vector<string> * lines = GetItemName((*items)[i + dlg.sView], true);		// Added true, since we're not counting the number of items in a stack.
                std::vector<string>::iterator iter = lines->begin(), end = lines->end();

                if (onButton == i + 1)
                {
                    viewing = true;

                    if (iter != end)
                    {
                        PutAlignedString(sX, sX + szX, sY + 110 + i * 15, iter->c_str(), 255, 255, 255);
                    }

                    uint32_t maxWidth = 0;

                    while (iter != end)
                    {
                        if (iter->size() > maxWidth)
                            maxWidth = iter->size();

                        ++iter;
                    }

                    //DIRECTX m_DDraw.DrawShadowBox(sX + 70 - 3, sY + 25 - 1, sX + 70 + maxWidth * 5.5, sY + 25 + 13*lines->size());

                    iter = lines->begin();

                    int iLoc = 0;
                    if (iter != end)
                    {
                        PutString(sX + 70, sY + 25, iter->c_str(), m_itemColor, false, 1);
                        iLoc += 13;
                        ++iter;
                    }

                    while (iter != end)
                    {
                        PutString(sX + 70, sY + 25 + iLoc, iter->c_str(), video::SColor(255, 150, 150, 150), false, 1);
                        iLoc += 13;
                        ++iter;
                    }

                    uint32_t ItemColor = (*items)[i + dlg.sView]->m_ItemColor;
                    {
                        if ((*items)[i + dlg.sView]->m_cEquipPos == EQUIPPOS_LHAND
                            || (*items)[i + dlg.sView]->m_cEquipPos == EQUIPPOS_RHAND
                            || (*items)[i + dlg.sView]->m_cEquipPos == EQUIPPOS_TWOHAND)
                        {
                            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + (*items)[i + dlg.sView]->m_sSprite]->PutSpriteRGB(sX + 40, sY + 68, (*items)[i + dlg.sView]->m_sSpriteFrame,
                                                                                                                    ItemColor, m_dwCurTime);
                        }
                        else
                            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + (*items)[i + dlg.sView]->m_sSprite]->PutSpriteRGB(sX + 40, sY + 68, (*items)[i + dlg.sView]->m_sSpriteFrame,
                                                                                                                    ItemColor, m_dwCurTime);
                    }
                }
                else if (iter != end)
                {
                    if (video::SColor(m_itemColor) != video::SColor(255, 255, 255, 255))
                        PutAlignedString(sX, sX + szX, sY + 110 + i * 15, iter->c_str(), video::SColor(m_itemColor));
                    else
                        PutAlignedString(sX, sX + szX, sY + 110 + i * 15, iter->c_str(), video::SColor(255, 0, 0, 0));
                }
            }

            pointerLoc = dlg.HandleScroll(13, items->size());
            if (items->size() > 13)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
            }

            if (!viewing)
            {
                PutAlignedString(sX, sX + szX, sY + 35, DRAW_DIALOGBOX_MAILBOX30);
            }
            break;

        case 5:
            PutAlignedString(sX, sX + szX, sY + 70, DRAW_DIALOGBOX_MAILBOX31);
            break;
    }
}

void CGame::DrawDialogBox_ItemUpgrade()
{
    int i, sX, sY, iValue;
    char cItemColor, cStr1[120], cStr2[120], cStr3[120];
    uint32_t dwTime = unixtime();

    char onButton = m_dialogBoxes[34].OnButton();
    sX = m_dialogBoxes[34].m_X;
    sY = m_dialogBoxes[34].m_Y;
    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT , sX, sY, 5); //Item Upgrade Text

    switch (m_dialogBoxes[34].GetMode())
    {
        case 1://Gizon box Drag item needed to be upgraded"
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 30, DRAW_DIALOGBOX_ITEMUPGRADE1); // Drag item needed to be upgraded"
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 45, DRAW_DIALOGBOX_ITEMUPGRADE2); // "from the inventory. Then press"
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 60, DRAW_DIALOGBOX_ITEMUPGRADE3); // 'Upgrade' button."
            DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 46);
            wsprintfA(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE11, m_iGizonItemUpgradeLeft);		// "Item upgrade point : %d"
            PutAlignedString(sX + 24, sX + 248, sY + 100, G_cTxt);
            if (m_dialogBoxes[34].sV1 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
                iValue = (m_pItemList[m_dialogBoxes[34].sV1]->m_dwAttribute & 0xF0000000) >> 28;
                iValue = iValue*(iValue + 6) / 8 + 2;
                if ((m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos >= 11)
                    && (m_pItemList[m_dialogBoxes[34].sV1]->m_cItemType == 1))
                {
                    if ((memcmp(m_pItemList[m_dialogBoxes[34].sV1]->m_cName, "AngelicPendant(STR)", 19) == 0)
                        || (memcmp(m_pItemList[m_dialogBoxes[34].sV1]->m_cName, "AngelicPendant(DEX)", 19) == 0)
                        || (memcmp(m_pItemList[m_dialogBoxes[34].sV1]->m_cName, "AngelicPendant(INT)", 19) == 0)
                        || (memcmp(m_pItemList[m_dialogBoxes[34].sV1]->m_cName, "AngelicPendant(MAG)", 19) == 0))
                    {
                        iValue = (m_pItemList[m_dialogBoxes[34].sV1]->m_dwAttribute & 0xF0000000) >> 28;
                        switch (iValue)
                        {
                            case 0: iValue = 50; break;
                            case 1: iValue = 75; break;
                            case 2: iValue = 100; break;
                            case 3: iValue = 125; break;
                            case 4: iValue = 150; break;
                            case 5: iValue = 300; break;
                            case 6: iValue = 400; break;
                            case 7: iValue = 500; break;
                            case 8: iValue = 600; break;
                            case 9: iValue = 700; break;
                            case 10: iValue = 1000; break;
                            case 11: iValue = 1250; break;
                            case 12: iValue = 1500; break;
                            case 13: iValue = 1750; break;
                            case 14: iValue = 2000; break;
                        }
                    }
                }
                wsprintfA(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE12, iValue); //"Needed upgrade point : %d"
                if (m_iGizonItemUpgradeLeft < iValue)
                    PutAlignedString(sX + 24, sX + 248, sY + 115, G_cTxt, 195, 25, 25);
                else PutAlignedString(sX + 24, sX + 248, sY + 115, G_cTxt);
                i = m_dialogBoxes[34].sV1;
                cItemColor = m_pItemList[m_dialogBoxes[34].sV1]->m_ItemColor;
                if ((m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_LHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_RHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_TWOHAND))
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182, m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                }
                else
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182, m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                }
                ZeroMemory(cStr1, sizeof(cStr1));
                ZeroMemory(cStr2, sizeof(cStr2));
                ZeroMemory(cStr3, sizeof(cStr3));
                GetItemName(m_pItemList[m_dialogBoxes[34].sV1], cStr1, cStr2, cStr3);
                PutAlignedString(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                PutAlignedString(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                PutAlignedString(sX + 24, sX + 248, sY + 260 + 20, cStr3);
                if (m_iGizonItemUpgradeLeft < iValue)
                    DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 46);
                else
                {
                    if (onButton == 1)
                        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 47);
                    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 46);
                }
            }
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 46);
            // Cancel
            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;

        case 2: // in progress
            PutAlignedString(sX + 24, sX + 248, sY + 55 + 30 + 282 - 117 - 170, DRAW_DIALOGBOX_ITEMUPGRADE5);
            PutAlignedString(sX + 24, sX + 248, sY + 55 + 45 + 282 - 117 - 170, DRAW_DIALOGBOX_ITEMUPGRADE6);
            if (m_dialogBoxes[34].sV1 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
                i = m_dialogBoxes[34].sV1;
                cItemColor = m_pItemList[m_dialogBoxes[34].sV1]->m_ItemColor;
                if ((m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_LHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_RHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_TWOHAND))
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182
                                                                                                                       , m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                }
                else
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182
                                                                                                                       , m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                }
                if ((rand() % 5) == 0) m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutTransSprite25(sX + 134, sY + 182, m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, dwTime);
                ZeroMemory(cStr1, sizeof(cStr1));
                ZeroMemory(cStr2, sizeof(cStr2));
                ZeroMemory(cStr3, sizeof(cStr3));
                GetItemName(m_pItemList[m_dialogBoxes[34].sV1], cStr1, cStr2, cStr3);
                PutAlignedString(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                PutAlignedString(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                PutAlignedString(sX + 24, sX + 248, sY + 260 + 20, cStr3);
            }
            if (((dwTime - m_dialogBoxes[34].dwV1) / 1000 > 4) && (m_dialogBoxes[34].dwV1 != 0))
            {
                m_dialogBoxes[34].dwV1 = 0;
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_UPGRADEITEM, 0, m_dialogBoxes[34].sV1, 0, 0, 0);
            }
            break;

        case 3: // Congratulations
            PutAlignedString(sX + 24, sX + 248, sY + 55 + 30 + 282 - 117 - 170, DRAW_DIALOGBOX_ITEMUPGRADE7);//"Congratulation! item upgrade"
            PutAlignedString(sX + 24, sX + 248, sY + 55 + 45 + 282 - 117 - 170, DRAW_DIALOGBOX_ITEMUPGRADE8);//"has been succeeded."
            if (m_dialogBoxes[34].sV1 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
                i = m_dialogBoxes[34].sV1;
                cItemColor = m_pItemList[m_dialogBoxes[34].sV1]->m_ItemColor;
                if ((m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_LHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_RHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_TWOHAND))
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182
                                                                                                                       , m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                }
                else
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182
                                                                                                                       , m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                }
                ZeroMemory(cStr1, sizeof(cStr1));
                ZeroMemory(cStr2, sizeof(cStr2));
                ZeroMemory(cStr3, sizeof(cStr3));
                GetItemName(m_pItemList[m_dialogBoxes[34].sV1], cStr1, cStr2, cStr3);
                PutAlignedString(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                PutAlignedString(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                PutAlignedString(sX + 24, sX + 248, sY + 260 + 20, cStr3);
            }
            // OK
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 4: // Failed
            PutAlignedString(sX + 24, sX + 248, sY + 55 + 30 + 282 - 117 - 170, DRAW_DIALOGBOX_ITEMUPGRADE9);//"Item upgrade has failed."
            if ((m_dialogBoxes[34].sV1 != -1) && (m_pItemList[m_dialogBoxes[34].sV1] == 0))
            {
                PlaySound('E', 24, 0, 0);
                m_dialogBoxes[34].SetMode(7);
                return;
            }
            if (m_dialogBoxes[34].sV1 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
                i = m_dialogBoxes[34].sV1;
                cItemColor = m_pItemList[m_dialogBoxes[34].sV1]->m_ItemColor;
                if ((m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_LHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_RHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_TWOHAND))
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182
                                                                                                                       , m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                }
                else
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182
                                                                                                                       , m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                }
                ZeroMemory(cStr1, sizeof(cStr1));
                ZeroMemory(cStr2, sizeof(cStr2));
                ZeroMemory(cStr3, sizeof(cStr3));
                GetItemName(m_pItemList[m_dialogBoxes[34].sV1], cStr1, cStr2, cStr3);
                PutAlignedString(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                PutAlignedString(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                PutAlignedString(sX + 24, sX + 248, sY + 260 + 20, cStr3);
            }

            // OK
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 5: // select Stone/ Gizon
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 45, DRAW_DIALOGBOX_ITEMUPGRADE13); // "Select which kind of items to upgrade."
            if (onButton == 1)
            {
                PutAlignedString(sX + 24, sX + 248, sY + 100, DRAW_DIALOGBOX_ITEMUPGRADE14, 255, 255, 255); // "Normal item upgrade"
                PutAlignedString(sX + 24, sX + 248, sY + 150, DRAW_DIALOGBOX_ITEMUPGRADE16); // "Upgrading a normal item."
                PutAlignedString(sX + 24, sX + 248, sY + 165, DRAW_DIALOGBOX_ITEMUPGRADE17); // "With upgrading weapon"
                PutAlignedString(sX + 24, sX + 248, sY + 180, DRAW_DIALOGBOX_ITEMUPGRADE18); // "items, Stone of Xelima"
                PutAlignedString(sX + 24, sX + 248, sY + 195, DRAW_DIALOGBOX_ITEMUPGRADE19); // "is needed. With upgrading"
                PutAlignedString(sX + 24, sX + 248, sY + 210, DRAW_DIALOGBOX_ITEMUPGRADE20); // "armor items, Stone of"
                PutAlignedString(sX + 24, sX + 248, sY + 225, DRAW_DIALOGBOX_ITEMUPGRADE21); // "Merien is needed."
                PutAlignedString(sX + 24, sX + 248, sY + 255, DRAW_DIALOGBOX_ITEMUPGRADE26); // "* Majestic items cannot be"
                PutAlignedString(sX + 24, sX + 248, sY + 270, DRAW_DIALOGBOX_ITEMUPGRADE27); // "upgraded with this performance."
            }
            else PutAlignedString(sX + 24, sX + 248, sY + 100, DRAW_DIALOGBOX_ITEMUPGRADE14, 4, 0, 50); // "Normal item upgrade"
            if (onButton == 2)
            {
                PutAlignedString(sX + 24, sX + 248, sY + 120, DRAW_DIALOGBOX_ITEMUPGRADE15, 255, 255, 255); // "Majestic item upgrade"
                PutAlignedString(sX + 24, sX + 248, sY + 150, DRAW_DIALOGBOX_ITEMUPGRADE22); // "Upgrading majestic items."
                PutAlignedString(sX + 24, sX + 248, sY + 165, DRAW_DIALOGBOX_ITEMUPGRADE23); // "In order to upgrade majestic items,"
                PutAlignedString(sX + 24, sX + 248, sY + 180, DRAW_DIALOGBOX_ITEMUPGRADE24); // "enough upgrade points for"
                PutAlignedString(sX + 24, sX + 248, sY + 195, DRAW_DIALOGBOX_ITEMUPGRADE25); // "majestic items are required."
                PutAlignedString(sX + 24, sX + 248, sY + 225, DRAW_DIALOGBOX_ITEMUPGRADE28); // "* Normal items cannot be"
                PutAlignedString(sX + 24, sX + 248, sY + 240, DRAW_DIALOGBOX_ITEMUPGRADE29); // "upgraded with this performance."
            }
            else PutAlignedString(sX + 24, sX + 248, sY + 120, DRAW_DIALOGBOX_ITEMUPGRADE15, 4, 0, 50); // "Majestic item upgrade"
                                                                                                        // Cancel
            if (onButton == 3)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);

            if (onButton == 4) // Upgrade Hero System xRisenx
            {
                PutAlignedString(sX + 24, sX + 248, sY + 140, DRAW_DIALOGBOX_ITEMUPGRADE43, 255, 255, 255); // "Hero item upgrade"
                PutAlignedString(sX + 24, sX + 248, sY + 160, DRAW_DIALOGBOX_ITEMUPGRADE44); // "Upgrading a normal item."
                PutAlignedString(sX + 24, sX + 248, sY + 175, DRAW_DIALOGBOX_ITEMUPGRADE45); // "With upgrading weapon"
                PutAlignedString(sX + 24, sX + 248, sY + 190, DRAW_DIALOGBOX_ITEMUPGRADE46); // "items, Stone of Xelima"
                                                                                             //PutAlignedString(sX + 24, sX + 248, sY + 205, DRAW_DIALOGBOX_ITEMUPGRADE47); // "is needed. With upgrading"
                                                                                             //PutAlignedString(sX + 24, sX + 248, sY + 220, DRAW_DIALOGBOX_ITEMUPGRADE48); // "armor items, Stone of"
                                                                                             //PutAlignedString(sX + 24, sX + 248, sY + 235, DRAW_DIALOGBOX_ITEMUPGRADE49); // "Merien is needed."
                PutAlignedString(sX + 24, sX + 248, sY + 250, DRAW_DIALOGBOX_ITEMUPGRADE26); // "* Majestic items cannot be"
                PutAlignedString(sX + 24, sX + 248, sY + 265, DRAW_DIALOGBOX_ITEMUPGRADE27); // "upgraded with this performance."
            }
            else PutAlignedString(sX + 24, sX + 248, sY + 140, DRAW_DIALOGBOX_ITEMUPGRADE43, 4, 0, 50); // "Normal item upgrade"

            break;

        case 6: // Stone upgrade
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 30, DRAW_DIALOGBOX_ITEMUPGRADE31); // "After bring a normal item to"
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 45, DRAW_DIALOGBOX_ITEMUPGRADE32); //
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 60, DRAW_DIALOGBOX_ITEMUPGRADE33); //
            if (m_dialogBoxes[34].sV2 == 0)
            {
                PutAlignedString(sX + 24, sX + 248, sY + 20 + 80, DRAW_DIALOGBOX_ITEMUPGRADE41, 195, 25, 25);//"You can upgrade only armor."
            }
            else
            {
                wsprintfA(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE34, m_dialogBoxes[34].sV2);//"The number of upgrade(weapon): %d"
                PutAlignedString(sX + 24, sX + 248, sY + 20 + 80, G_cTxt);
            }
            if (m_dialogBoxes[34].sV3 == 0)
            {
                PutAlignedString(sX + 24, sX + 248, sY + 20 + 95, DRAW_DIALOGBOX_ITEMUPGRADE42, 195, 25, 25);//"You can upgrade only weapon."
            }
            else
            {
                wsprintfA(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE35, m_dialogBoxes[34].sV3);//"The number of upgrade(armor): %d"
                PutAlignedString(sX + 24, sX + 248, sY + 20 + 95, G_cTxt);
            }

            DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 46);

            if (m_dialogBoxes[34].sV1 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
                i = m_dialogBoxes[34].sV1;
                cItemColor = m_pItemList[m_dialogBoxes[34].sV1]->m_ItemColor;
                if ((m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_LHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_RHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_TWOHAND))
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182
                                                                                                                       , m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                }
                else
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182
                                                                                                                       , m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                }

                ZeroMemory(cStr1, sizeof(cStr1));
                ZeroMemory(cStr2, sizeof(cStr2));
                ZeroMemory(cStr3, sizeof(cStr3));
                GetItemName(m_pItemList[m_dialogBoxes[34].sV1], cStr1, cStr2, cStr3);
                PutAlignedString(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                PutAlignedString(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                PutAlignedString(sX + 24, sX + 248, sY + 260 + 20, cStr3);
                if (onButton == 1)
                    DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 47);
                else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 46);
            }
            // Cancel
            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;

        case 7: // lost item
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 130, DRAW_DIALOGBOX_ITEMUPGRADE36);//"Due to the failure of upgrade, the"
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 145, DRAW_DIALOGBOX_ITEMUPGRADE37);//"item is gone. Try next time!"
                                                                                             // OK
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 8: // impossible deja an max
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 130, DRAW_DIALOGBOX_ITEMUPGRADE38);//"Upgrading the item is not possible any more."
                                                                                             // OK
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 9: // impossible
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 130, DRAW_DIALOGBOX_ITEMUPGRADE39);//"Upgrading the item is not possible."
                                                                                             // OK
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 10: // plus de gizons
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 130, DRAW_DIALOGBOX_ITEMUPGRADE40);//"A point to upgrade items is not present"
                                                                                             // OK
            if (onButton == 1)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
            break;

        case 13: // Upgrade Hero System xRisenx
            DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 30, DRAW_DIALOGBOX_ITEMUPGRADE31); // "After bring a normal item to"
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 45, DRAW_DIALOGBOX_ITEMUPGRADE32); //
            PutAlignedString(sX + 24, sX + 248, sY + 20 + 60, DRAW_DIALOGBOX_ITEMUPGRADE33); //
            DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 46);
            wsprintfA(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE47, m_iGizonItemUpgradeLeft);		// "Item upgrade point : %d"
            PutAlignedString(sX + 24, sX + 248, sY + 100, G_cTxt);
            if (m_dialogBoxes[34].sV1 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME3, sX, sY, 3);





                i = m_dialogBoxes[34].sV1;
                cItemColor = m_pItemList[m_dialogBoxes[34].sV1]->m_ItemColor;
                if ((m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_LHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_RHAND)
                    || (m_pItemList[m_dialogBoxes[34].sV1]->m_cEquipPos == EQUIPPOS_TWOHAND))
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182
                                                                                                                       , m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                }
                else
                {
                    m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_dialogBoxes[34].sV1]->m_sSprite]->PutSpriteRGB(sX + 134, sY + 182
                                                                                                                       , m_pItemList[m_dialogBoxes[34].sV1]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                }

                ZeroMemory(cStr1, sizeof(cStr1));
                ZeroMemory(cStr2, sizeof(cStr2));
                ZeroMemory(cStr3, sizeof(cStr3));
                GetItemName(m_pItemList[m_dialogBoxes[34].sV1], cStr1, cStr2, cStr3);
                PutAlignedString(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                PutAlignedString(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                PutAlignedString(sX + 24, sX + 248, sY + 260 + 20, cStr3);
                if (onButton == 1)
                    DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 47);
                else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 46);
            }
            // Cancel
            if (onButton == 2)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;
    }
}

// 3.51 Level Up Dialog - Changes stats Using Majestic points - Diuuude
void CGame::DrawDialogBox_ChangeStatsMajestic()
{
    short sX, sY, szX;
    uint32_t dwTime = m_dwCurTime;
    char cTxt[120];
    int iStats, statsChanged, majPointsUsed;

    char onButton = m_dialogBoxes[42].OnButton();
    sX = m_dialogBoxes[42].m_X;
    sY = m_dialogBoxes[42].m_Y;
    szX = m_dialogBoxes[42].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 2);
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, sX + 16, sY + 100, 4);

    PutAlignedString(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_LEVELUP_SETTING14);
    PutAlignedString(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_LEVELUP_SETTING15);

    statsChanged = 0;
    for (int i = 0; i < 6; i++) statsChanged -= m_luStat[i];
    majPointsUsed = (statsChanged + 2) / 3;

    PutString(sX + 20, sY + 85, DRAW_DIALOGBOX_LEVELUP_SETTING16, video::SColor(255, 0, 0, 0));
    wsprintfA(cTxt, "%d", m_iGizonItemUpgradeLeft - majPointsUsed);
    if (majPointsUsed > 0)
    {
        PutString(sX + 75, sY + 102, cTxt, video::SColor(255, 0, 255, 0));
    }
    else
    {
        PutString(sX + 75, sY + 102, cTxt, video::SColor(255, 0, 0, 0));
    }

    // Strength
    PutString(sX + 24, sY + 125, DRAW_DIALOGBOX_LEVELUP_SETTING4, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_STR] - m_angelStat[STAT_STR]);
    PutString(sX + 109, sY + 125, cTxt, video::SColor(255, 25, 35, 25));
    iStats = m_stat[STAT_STR] + m_luStat[STAT_STR] - m_angelStat[STAT_STR];
    wsprintfA(cTxt, "%d", iStats);
    if (m_luStat[STAT_STR] != 0)
    {
        PutString(sX + 162, sY + 125, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 125, cTxt, video::SColor(255, 25, 35, 25));
    }
    if (onButton == 1 && m_luStat[STAT_STR] != 0)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 127, 5, dwTime);
    if (onButton == 7 && m_stat[STAT_STR] > 10)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 127, 6, dwTime);


    PutString(sX + 24, sY + 144, DRAW_DIALOGBOX_LEVELUP_SETTING5, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_VIT]);
    PutString(sX + 109, sY + 144, cTxt, video::SColor(255, 25, 35, 25));
    iStats = m_stat[STAT_VIT] + m_luStat[STAT_VIT];
    wsprintfA(cTxt, "%d", iStats);
    if (m_luStat[STAT_VIT] != 0)
    {
        PutString(sX + 162, sY + 144, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 144, cTxt, video::SColor(255, 25, 35, 25));
    }
    if (onButton == 2 && m_luStat[STAT_VIT] != 0)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 146, 5, dwTime);
    if (onButton == 8 && m_stat[STAT_VIT] > 10)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 146, 6, dwTime);

    // Dexterity
    PutString(sX + 24, sY + 163, DRAW_DIALOGBOX_LEVELUP_SETTING6, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_DEX] - m_angelStat[STAT_DEX]);
    PutString(sX + 109, sY + 163, cTxt, video::SColor(255, 25, 35, 25));
    iStats = m_stat[STAT_DEX] + m_luStat[STAT_DEX] - m_angelStat[STAT_DEX];
    wsprintfA(cTxt, "%d", iStats);
    if (m_luStat[STAT_DEX] != 0)
    {
        PutString(sX + 162, sY + 163, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 163, cTxt, video::SColor(255, 25, 35, 25));
    }
    if (onButton == 3 && m_luStat[STAT_DEX] != 0)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 165, 5, dwTime);
    if (onButton == 9 && m_stat[STAT_DEX] > 10)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 165, 6, dwTime);

    // Intelligence
    PutString(sX + 24, sY + 182, DRAW_DIALOGBOX_LEVELUP_SETTING7, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_INT] - m_angelStat[STAT_INT]);
    PutString(sX + 109, sY + 182, cTxt, video::SColor(255, 25, 35, 25));
    iStats = m_stat[STAT_INT] + m_luStat[STAT_INT] - m_angelStat[STAT_INT];
    wsprintfA(cTxt, "%d", iStats);
    if (m_luStat[STAT_INT] != 0)
    {
        PutString(sX + 162, sY + 182, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 182, cTxt, video::SColor(255, 25, 35, 25));
    }
    if (onButton == 4 && m_luStat[STAT_INT] != 0)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 184, 5, dwTime);
    if (onButton == 10 && m_stat[STAT_INT] > 10)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 184, 6, dwTime);

    // Magic
    PutString(sX + 24, sY + 201, DRAW_DIALOGBOX_LEVELUP_SETTING8, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_MAG] - m_angelStat[STAT_MAG]);
    PutString(sX + 109, sY + 201, cTxt, video::SColor(255, 25, 35, 25));
    iStats = m_stat[STAT_MAG] + m_luStat[STAT_MAG] - m_angelStat[STAT_MAG];
    wsprintfA(cTxt, "%d", iStats);
    if (m_luStat[STAT_MAG] != 0)
    {
        PutString(sX + 162, sY + 201, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 201, cTxt, video::SColor(255, 25, 35, 25));
    }
    if (onButton == 5 && m_luStat[STAT_MAG] != 0)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 203, 5, dwTime);
    if (onButton == 11 && m_stat[STAT_MAG] > 10)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 203, 6, dwTime);

    // Charisma
    PutString(sX + 24, sY + 220, DRAW_DIALOGBOX_LEVELUP_SETTING9, video::SColor(255, 5, 5, 5));
    wsprintfA(cTxt, "%d", m_stat[STAT_CHR]);
    PutString(sX + 109, sY + 220, cTxt, video::SColor(255, 25, 35, 25));
    iStats = m_stat[STAT_CHR] + m_luStat[STAT_CHR];
    wsprintfA(cTxt, "%d", iStats);
    if (m_luStat[STAT_CHR] != 0)
    {
        PutString(sX + 162, sY + 220, cTxt, video::SColor(255, 255, 0, 0));
    }
    else
    {
        PutString(sX + 162, sY + 220, cTxt, video::SColor(255, 25, 35, 25));
    }
    if (onButton == 6 && m_luStat[STAT_CHR] != 0)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 195, sY + 222, 5, dwTime);
    if (onButton == 12 && m_stat[STAT_CHR] > 10)
        m_pSprite[SPRID_INTERFACE_ND_GAME4]->PutSpriteFast(sX + 210, sY + 222, 6, dwTime);

    if (onButton == 14)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 17);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 16);

    if (onButton == 13)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
}


// 3.51 LevelUp Box - Change stats using Majestic - Diuuude
void CGame::DlgBoxClick_ChangeStatsMajestic()
{
    char onButton = m_dialogBoxes[42].OnButton();

    if (!onButton) return;

    int statChanged = onButton > 6 ? onButton - 7 : onButton - 1;
    int changeAmount = m_bCtrlPressed ? 5 : 1;
    int totalChanged = 0;

    for (int i = 0; i < 6; i++) totalChanged -= m_luStat[i];

    if (totalChanged && onButton <= 6)
    {
        if (m_luStat[statChanged])
        {
            changeAmount = changeAmount > -m_luStat[statChanged] ? -m_luStat[statChanged] : changeAmount;
            m_luStat[statChanged] += changeAmount;
            m_iLU_Point -= changeAmount;
        }
    }

    if ((totalChanged / 3) < m_iGizonItemUpgradeLeft && onButton >= 7 && onButton <= 12)
    {
        if ((totalChanged + changeAmount + 2) / 3 > m_iGizonItemUpgradeLeft)
            changeAmount = (m_iGizonItemUpgradeLeft * 3) - totalChanged;

        if (m_stat[statChanged] + m_luStat[statChanged] - changeAmount < 10)
            changeAmount = m_stat[statChanged] + m_luStat[statChanged] - 10;

        m_luStat[statChanged] -= changeAmount;
        m_iLU_Point += changeAmount;
    }

    if (statChanged && onButton == 13)
    {
        bSendCommand(MSGID_STATECHANGEPOINT);
        DisableDialogBox(42);
    }

    if (onButton == 14)
    {
        for (int i = 0; i < 6; i++)	m_luStat[i] = 0;
        m_iLU_Point -= totalChanged;
        DisableDialogBox(42);
    }

    PlaySound('E', 14, 5);
}

void CGame::DrawDialogBox_Resurect()
{
    char onButton = m_dialogBoxes[50].OnButton();
    short sX = m_dialogBoxes[50].m_X;
    short sY = m_dialogBoxes[50].m_Y;
    short szX = m_dialogBoxes[50].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME1, sX, sY, 2);

    PutAlignedString(sX, sX + szX, sY + 20, DLGBOX_RESURRECT1, 4, 0, 50);
    PutAlignedString(sX, sX + szX, sY + 35, DLGBOX_RESURRECT2, 4, 0, 50);

    if (onButton == 1)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 65, 19);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 65, 18);

    if (onButton == 2)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 65, 3);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 65, 2);
}

void CGame::DlgBoxClick_Resurect()
{
    char onButton = m_dialogBoxes[50].OnButton();
    if (onButton == 1)
    {   // yes
        bSendCommand(MSGID_REQUEST_RESURRECTPLAYER_YES, 0, 0, 0, 0, 0, 0, 0);
        DisableDialogBox(50);
    }
    else if (onButton == 2)
    {	// no
        bSendCommand(MSGID_REQUEST_RESURRECTPLAYER_NO, 0, 0, 0, 0, 0, 0, 0);
        DisableDialogBox(50);
    }
}

void CGame::DrawDialogBox_GuildSummons()
{
    char onButton = m_dialogBoxes[49].OnButton();
    short sX = m_dialogBoxes[49].m_X;
    short sY = m_dialogBoxes[49].m_Y;
    short szX = m_dialogBoxes[49].sSizeX;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME1, sX, sY, 2);

    PutAlignedString(sX, sX + szX, sY + 15, DLGBOX_GLDSUMMONS1, 4, 0, 50);
    PutAlignedString(sX, sX + szX, sY + 30, m_dialogBoxes[49].cStr, 40, 225, 50);
    PutAlignedString(sX, sX + szX, sY + 45, DLGBOX_GLDSUMMONS2, 4, 0, 50);


    if (onButton == 1)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 65, 19);
    else
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 65, 18);


    if (onButton == 2)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 65, 3);
    else
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 65, 2);
}

void CGame::DlgBoxClick_GuildSummons()
{
    char onButton = m_dialogBoxes[49].OnButton();

    if (onButton == 1)
    {
        // yes
        if (unixtime() - m_dwDamagedTime < 10 * 1000)
        {
            AddEventList(DLGBOXCLICK_GUILDSUMMONS1, CHAT_GM);
        }
        else
        {
            bSendCommand(MSGID_RSPGUILDSUMMONS, 0, 0, true, 0, 0, 0, 0);
            DisableDialogBox(49);
        }
    }
    else if (onButton == 2)
    {
        // no
        bSendCommand(MSGID_RSPGUILDSUMMONS, 0, 0, false, 0, 0, 0, 0);
        DisableDialogBox(49);
    }
}

/*********************************************************************************************************************
**  void DrawDialogBox_CMDHallMenu()
**  description			: Draw the diag																				**
**********************************************************************************************************************/
void CGame::DrawDialogBox_CMDHallMenu()
{
    short sX, sY, szX;
    char cTxt[120];

    char onButton = m_dialogBoxes[51].OnButton();
    sX = m_dialogBoxes[51].m_X;
    sY = m_dialogBoxes[51].m_Y;
    szX = m_dialogBoxes[51].sSizeX;
    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);

    switch (m_dialogBoxes[51].GetMode())
    {
        case 0: // initial diag
            if (onButton == 1)
                PutAlignedString(sX, sX + szX, sY + 70, DRAW_CMDHALL1, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 70, DRAW_CMDHALL1, 4, 0, 50);

            if (onButton == 2)
                PutAlignedString(sX, sX + szX, sY + 95, DRAW_CMDHALL2, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 95, DRAW_CMDHALL2, 4, 0, 50);

            if (onButton == 3)
                PutAlignedString(sX, sX + szX, sY + 120, DRAW_CMDHALL3, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 120, DRAW_CMDHALL3, 4, 0, 50);
            // Angels
            if (onButton == 4)
                PutAlignedString(sX, sX + szX, sY + 145, DRAW_CMDHALL4, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 145, DRAW_CMDHALL4, 4, 0, 50);
            // Dragons
            if (onButton == 5)
                PutAlignedString(sX, sX + szX, sY + 170, DRAW_CMDHALL32, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 170, DRAW_CMDHALL32, 4, 0, 50);
            break;

        case 1: // TP diag
            if (m_iTeleportMapCount > 0)
            {
                for (int i = 0; i < m_iTeleportMapCount; i++)
                {
                    ZeroMemory(cTxt, sizeof(cTxt));
                    GetOfficialMapName(m_stTeleportList[i].mapname, cTxt);
                    wsprintfA(G_cTxt, DRAW_DIALOGBOX_CITYHALL_MENU77, cTxt, m_stTeleportList[i].iCost);

                    if (onButton == i + 1)
                        PutAlignedString(sX, sX + szX, sY + 130 + i * 15, G_cTxt, 255, 255, 255);
                    else
                        PutAlignedString(sX, sX + szX, sY + 130 + i * 15, G_cTxt, 250, 250, 0);
                }
            }
            else if (m_iTeleportMapCount == -1)
            {
                PutAlignedString(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU73, 55, 25, 25);
                PutAlignedString(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_CITYHALL_MENU74, 55, 25, 25);
            }
            else
            {
                PutAlignedString(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_CITYHALL_MENU76, 55, 25, 25);//"There is no area that you can teleport."
            }
            break;

        case 2: // Soldier diag
            PutAlignedString(sX, sX + szX, sY + 45, DRAW_CMDHALL5, 255, 255, 255);
            if ((m_iConstructionPoint >= 2000) && (m_bIsCrusadeMode == false))
            {
                if (onButton == 1)
                    PutAlignedString(sX, sX + szX, sY + 70, DRAW_CMDHALL6, 255, 255, 255);
                else PutAlignedString(sX, sX + szX, sY + 70, DRAW_CMDHALL6, 4, 0, 50);
            }
            else PutAlignedString(sX, sX + szX, sY + 70, DRAW_CMDHALL6, 65, 65, 65);

            if ((m_iConstructionPoint >= 3000) && (m_bIsCrusadeMode == false))
            {
                if (onButton == 2)
                    PutAlignedString(sX, sX + szX, sY + 95, DRAW_CMDHALL7, 255, 255, 255);
                else PutAlignedString(sX, sX + szX, sY + 95, DRAW_CMDHALL7, 4, 0, 50);
            }
            else PutAlignedString(sX, sX + szX, sY + 95, DRAW_CMDHALL7, 65, 65, 65);

            if ((m_iConstructionPoint >= 1500) && (m_bIsCrusadeMode == false))
            {
                if (onButton == 3)
                    PutAlignedString(sX, sX + szX, sY + 120, DRAW_CMDHALL8, 255, 255, 255);
                else PutAlignedString(sX, sX + szX, sY + 120, DRAW_CMDHALL8, 4, 0, 50);
            }
            else PutAlignedString(sX, sX + szX, sY + 120, DRAW_CMDHALL8, 65, 65, 65);

            if ((m_iConstructionPoint >= 3000) && (m_bIsCrusadeMode == false))
            {
                if (onButton == 4)
                    PutAlignedString(sX, sX + szX, sY + 145, DRAW_CMDHALL9, 255, 255, 255);
                else PutAlignedString(sX, sX + szX, sY + 145, DRAW_CMDHALL9, 4, 0, 50);
            }
            else PutAlignedString(sX, sX + szX, sY + 145, DRAW_CMDHALL9, 65, 65, 65);

            if ((m_iConstructionPoint >= 4000) && (m_bIsCrusadeMode == false))
            {
                if (onButton == 5)
                    PutAlignedString(sX, sX + szX, sY + 170, DRAW_CMDHALL10, 255, 255, 255);
                else PutAlignedString(sX, sX + szX, sY + 170, DRAW_CMDHALL10, 4, 0, 50);
            }
            else PutAlignedString(sX, sX + szX, sY + 170, DRAW_CMDHALL10, 65, 65, 65);

            if ((m_iConstructionPoint >= 3000) && (m_bIsCrusadeMode == false))
            {
                if (onButton == 6)
                    PutAlignedString(sX, sX + szX, sY + 195, DRAW_CMDHALL11, 255, 255, 255);
                else PutAlignedString(sX, sX + szX, sY + 195, DRAW_CMDHALL11, 4, 0, 50);
            }
            else PutAlignedString(sX, sX + szX, sY + 195, DRAW_CMDHALL11, 65, 65, 65);

            PutAlignedString(sX, sX + szX, sY + 220, DRAW_CMDHALL12, 4, 0, 50);
            wsprintfA(G_cTxt, DRAW_CMDHALL13, m_iConstructionPoint);
            PutAlignedString(sX, sX + szX, sY + 250, G_cTxt, 4, 0, 50);
            PutAlignedString(sX, sX + szX, sY + 280, DRAW_CMDHALL14, 4, 0, 50);
            PutAlignedString(sX, sX + szX, sY + 300, DRAW_CMDHALL15, 4, 0, 50);
            break;

        case 3: // Hire a Flag Diag
            PutAlignedString(sX, sX + szX, sY + 45, DRAW_CMDHALL16, 4, 0, 50);
            PutAlignedString(sX, sX + szX, sY + 70, DRAW_CMDHALL17, 4, 0, 50);
            if (onButton == 1)
                PutAlignedString(sX, sX + szX, sY + 140, DRAW_CMDHALL18, 255, 255, 255);
            else
                PutAlignedString(sX, sX + szX, sY + 140, DRAW_CMDHALL18, 250, 250, 0);
            break;

        case 4: // Tutelar Angel Diag
            PutAlignedString(sX, sX + szX, sY + 45, DRAW_CMDHALL19, 4, 0, 50);
            PutAlignedString(sX, sX + szX, sY + 70, DRAW_CMDHALL20, 4, 0, 50);
            PutAlignedString(sX, sX + szX, sY + 95, DRAW_CMDHALL21, 4, 0, 50);
            wsprintfA(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE11, m_iGizonItemUpgradeLeft);// "Item upgrade point : %d"
            PutAlignedString(sX, sX + szX, sY + 140, G_cTxt, 0, 0, 0);

            if (onButton == 1 && (m_iGizonItemUpgradeLeft > 99))
                PutAlignedString(sX, sX + szX, sY + 175, DRAW_CMDHALL22, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 175, DRAW_CMDHALL22, 250, 250, 0);

            if (onButton == 2 && (m_iGizonItemUpgradeLeft > 99))
                PutAlignedString(sX, sX + szX, sY + 200, DRAW_CMDHALL23, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 200, DRAW_CMDHALL23, 250, 250, 0);

            if (onButton == 3 && (m_iGizonItemUpgradeLeft > 99))
                PutAlignedString(sX, sX + szX, sY + 225, DRAW_CMDHALL24, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 225, DRAW_CMDHALL24, 250, 250, 0);

            if (onButton == 4 && (m_iGizonItemUpgradeLeft > 99))
                PutAlignedString(sX, sX + szX, sY + 250, DRAW_CMDHALL25, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 250, DRAW_CMDHALL25, 250, 250, 0);

            break;

        case 5: // Dragons Diag
            PutAlignedString(sX, sX + szX, sY + 45, DRAW_CMDHALL26, 4, 0, 50);
            PutAlignedString(sX, sX + szX, sY + 70, DRAW_CMDHALL27, 4, 0, 50);
            PutAlignedString(sX, sX + szX, sY + 95, DRAW_CMDHALL28, 4, 0, 50);
            wsprintfA(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE11, m_iGizonItemUpgradeLeft);// "Item upgrade point : %d"
            PutAlignedString(sX, sX + szX, sY + 140, G_cTxt, 0, 0, 0);

            if (onButton == 1 && (m_iGizonItemUpgradeLeft > 4999))
                PutAlignedString(sX, sX + szX, sY + 175, DRAW_CMDHALL29, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 175, DRAW_CMDHALL29, 250, 250, 0);

            if (onButton == 2 && (m_iGizonItemUpgradeLeft > 4999))
                PutAlignedString(sX, sX + szX, sY + 200, DRAW_CMDHALL30, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 200, DRAW_CMDHALL30, 250, 250, 0);

            if (onButton == 3 && (m_iGizonItemUpgradeLeft > 4999))
                PutAlignedString(sX, sX + szX, sY + 225, DRAW_CMDHALL31, 255, 255, 255);
            else PutAlignedString(sX, sX + szX, sY + 225, DRAW_CMDHALL31, 250, 250, 0);

            break;
    }
}

/*********************************************************************************************************************
**  void DlgBoxClick_CMDHallMenu();											**
**  description			: Response of the player if he accepts resurection or not									**
**********************************************************************************************************************/
void CGame::DlgBoxClick_CMDHallMenu()
{
    char onButton = m_dialogBoxes[51].OnButton();
    switch (m_dialogBoxes[51].GetMode())
    {
        case 0: // initial diag
            if (onButton == 1)
            {
                m_dialogBoxes[51].SetMode(1); // TP diag
                m_iTeleportMapCount = -1;
                bSendCommand(MSGID_REQUEST_TELEPORT_LIST, 0, 0, 0, 0, 0, "Gail");
                PlaySound('E', 14, 5);
            }
            if (onButton == 2)
            {
                m_dialogBoxes[51].SetMode(2); // Soldier Diag
                PlaySound('E', 14, 5);
            }
            if (onButton == 3
                /*&& (m_iEnemyKillCount >= 3)*/)
            {
                m_dialogBoxes[51].SetMode(3); // Flag Diag
                PlaySound('E', 14, 5);
            }

            if (onButton == 4)
            {
                m_dialogBoxes[51].SetMode(4); // Angel Diag
                PlaySound('E', 14, 5);
            }

            if (onButton == 5)
            {
                m_dialogBoxes[51].SetMode(5); // Dragon Diag
                PlaySound('E', 14, 5);
            }
            break;

        case 1: // TP now
            if (m_iTeleportMapCount > 0 && onButton)
            {
                bSendCommand(MSGID_REQUEST_CHARGED_TELEPORT, 0, 0, m_stTeleportList[onButton - 1].iIndex, 0, 0, 0);
                DisableDialogBox(51);
                return;
            }
            break;

        case 2: // Buy a soldier scroll
            if (onButton == 1
                && (m_iConstructionPoint >= 2000))// Sorceress
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 875, 1, 2, 3, 4, "Gail", 5);
                PlaySound('E', 14, 5);
            }
            if (onButton == 2
                && (m_iConstructionPoint >= 3000)) // ATK
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 876, 0, 0, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            if (onButton == 3
                && (m_iConstructionPoint >= 1500)) // Elf
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 877, 0, 0, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            if (onButton == 4
                && (m_iConstructionPoint >= 3000)) // DSK
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 878, 0, 0, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            if (onButton == 5
                && (m_iConstructionPoint >= 4000)) // HBT
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 879, 0, 0, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            if (onButton == 6
                && (m_iConstructionPoint >= 3000)) // Barbarian
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 880, 0, 0, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 3: // Buy a Flag
            if (onButton == 1 && (m_iEnemyKillCount >= 3))
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_GETOCCUPYFLAG, 0, 0, 0, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 4: // Buy an Angel
            if (onButton && (m_iGizonItemUpgradeLeft >= 100)) // Changed Angel Cost from 5 to 100 xRisenx
            {
                bSendCommand(MSGID_REQUEST_ANGEL, 0, 0, onButton, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 5: // Buy an Dragon
            if (onButton && (m_iGizonItemUpgradeLeft >= 5000))
            {
                bSendCommand(MSGID_REQUEST_ANGEL, 0, 0, onButton, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            break;
    }
}
void CGame::DrawDialogBox_FriendList()
{
    short sX, sY, szX, szY;
    int y;
    char cTxt[120];
    int nickheight = 16;

    char onButton = m_dialogBoxes[43].OnButton();
    sX = m_dialogBoxes[43].m_X;
    sY = m_dialogBoxes[43].m_Y;
    szX = m_dialogBoxes[43].sSizeX;
    szY = m_dialogBoxes[43].sSizeY;
    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    PutString_SprFont(sX + 90, sY + 35, MSG_FRIENDSLIST3, 1, 1, 8);
    switch (m_dialogBoxes[43].GetMode())
    {
        case 0:
            int i = 0;
            for (i = 0; i < m_iTotalFriends; i++)
            {
                if (strcmp(friendsList[i].friendName, "") != 0)
                {
                    y = sY + 45 + ((i + 1)*nickheight);
                    PutString(sX + 25, y, friendsList[i].friendName, video::SColor(255, 0, 0, 0));
                    if (friendsList[i].online)
                    {
                        PutString(sX + 90, y, MSG_FRIENDSLIST4, video::SColor(255, 0, 200, 0));
                    }
                    else
                    {
                        PutString(sX + 90, y, MSG_FRIENDSLIST4B, video::SColor(255, 200, 0, 0));
                    }
                }
            }
            if (m_iFriendIndex != -1)
            {
                PutString(sX + 25, sY + 45 + ((m_iFriendIndex + 1)*nickheight), friendsList[m_iFriendIndex].friendName, video::SColor(255, 255, 255, 255));
                wsprintfA(cTxt, MSG_FRIENDSLIST5, friendsList[m_iFriendIndex].friendName);
            }
            else
            {
                wsprintfA(cTxt, MSG_FRIENDSLIST6);
            }
            m_dialogBoxes[43].sV1 = m_iFriendIndex;
            PutString(sX + 25, sY + 45 + (13 * nickheight), cTxt, video::SColor(255, 0, 0, 0));
            if (onButton == 2)
            {
                PutString(sX + 25, sY + 45 + (14 * nickheight), MSG_FRIENDSLIST7, video::SColor(255, 255, 255, 255));
            }
            else
            {
                PutString(sX + 25, sY + 45 + (14 * nickheight), MSG_FRIENDSLIST7, video::SColor(255, 0, 0, 0));
            }
            if (onButton == 3)
            {
                PutString(sX + 80, sY + 45 + (14 * nickheight), MSG_FRIENDSLIST8, video::SColor(255, 255, 255, 255));
            }
            else
            {
                PutString(sX + 80, sY + 45 + (14 * nickheight), MSG_FRIENDSLIST8, video::SColor(255, 0, 0, 0));
            }
            if (onButton == 4)
            {
                PutString(sX + 140, sY + 45 + (14 * nickheight), MSG_FRIENDSLIST9, video::SColor(255, 255, 255, 255));
            }
            else
            {
                PutString(sX + 140, sY + 45 + (14 * nickheight), MSG_FRIENDSLIST9, video::SColor(255, 0, 0, 0));
            }
            break;
    }
}

void CGame::DlgBoxClick_FriendList() // 43 - drajwer
{
    short sX, sY, szX, szY;
    int tmp, i;
    char cTxt[100];

    sX = m_dialogBoxes[43].m_X;
    sY = m_dialogBoxes[43].m_Y;
    szX = m_dialogBoxes[43].sSizeX;
    szY = m_dialogBoxes[43].sSizeY;
    short msY = G_pGame->m_stMCursor.sY;

    char onButton = m_dialogBoxes[43].OnButton();
    if (onButton == 1)
    {
        tmp = msY - sY - 45 - 16;
        tmp = tmp / 16;
        if (strcmp(friendsList[tmp].friendName, "") != 0)
        {
            m_iFriendIndex = tmp;
        }
        else
        {
            m_iFriendIndex = -1;
        }
        return;
    }
    if (onButton == 2)
    {
        if (m_iTotalFriends == 12)
        {
            AddEventList(MSG_FRIENDSLIST1, 10);
            return;
        }
        m_bIsGetPointingMode = true;
        m_iPointCommandType = 250;
        PlaySound('E', 14, 5);
        AddEventList(MSG_FRIENDSLIST2, 10); //pointing mode
        return;
    }

    if (m_iFriendIndex == -1) return;

    if (onButton == 3)
    {
        for (i = m_iFriendIndex + 1; i < 13; i++)
        {
            ZeroMemory(friendsList[i - 1].friendName, sizeof(friendsList[i - 1].friendName));
            memcpy(friendsList[i - 1].friendName, friendsList[i].friendName, 10);
            ZeroMemory(friendsList[i].friendName, sizeof(friendsList[i].friendName));
            friendsList[i - 1].online = friendsList[i].online;
        }
        m_iTotalFriends--;
        SaveFriendList();
    }

    if (onButton == 4)
    {
        ZeroMemory(cTxt, sizeof(cTxt));
        wsprintfA(cTxt, "/to %s", friendsList[m_iFriendIndex].friendName);
        bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, cTxt);
    }
}

void CGame::DrawDialogBox_Shop()
{
    short sX, sY;
    uint32_t dwTime = m_dwCurTime;
    int  i, iTemp;
    char cTemp[255], cStr2[255], cStr3[255];

    int  iCost/*, iDiscountCost, iDiscountRatio*/;

    double dTmp1, dTmp2, dTmp3;
    int  iTotalLines, iPointerLoc;
    bool bFlagStatLow = false;
    bool bFlagRedShown = false;
    double d1, d2, d3;
    Point pointerLoc;

    char onButton = m_dialogBoxes[11].OnButton();
    sX = m_dialogBoxes[11].m_X;
    sY = m_dialogBoxes[11].m_Y;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 11);

    switch (m_dialogBoxes[11].GetMode())
    {
        case 0:
            iTotalLines = 0;
            for (i = 0; i < MAXMENUITEMS; i++)
                if (m_pItemForSaleList[i] != 0) iTotalLines++;

            pointerLoc = m_dialogBoxes[11].HandleScroll(13, iTotalLines);

            if (iTotalLines > 13)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
            }

            PutAlignedString(sX + 22, sX + 165, sY + 45, DRAW_DIALOGBOX_SHOP1); // "ITEM"
            PutAlignedString(sX + 23, sX + 166, sY + 45, DRAW_DIALOGBOX_SHOP1);
            PutAlignedString(sX + 153, sX + 250, sY + 45, DRAW_DIALOGBOX_SHOP3);
            PutAlignedString(sX + 154, sX + 251, sY + 45, DRAW_DIALOGBOX_SHOP3);

            for (i = 0; i < 13; i++)
                if (((i + m_dialogBoxes[11].sView) < MAXMENUITEMS) && (m_pItemForSaleList[i + m_dialogBoxes[11].sView] != 0))
                {
                    ZeroMemory(cTemp, sizeof(cTemp));
                    GetItemName(m_pItemForSaleList[i + m_dialogBoxes[11].sView], cTemp, cStr2, cStr3);
                    if (onButton == i + 1)
                    {
                        PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 65, cTemp, 255, 255, 255);
                    }
                    else PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 65, cTemp, 4, 0, 50);
                }

            for (i = 0; i < 13; i++)
                if (((i + m_dialogBoxes[11].sView) < MAXMENUITEMS) && (m_pItemForSaleList[i + m_dialogBoxes[11].sView] != 0))
                {
                    /*iDiscountRatio = ((m_stat[STAT_CHR] - 10)  / 4) ;
                    dTmp1 = (double)iDiscountRatio;
                    dTmp2 = dTmp1 / 100.0f;
                    dTmp1 = (double)m_pItemForSaleList[i + m_dialogBoxes[11].sView]->m_wPrice;
                    dTmp3 = dTmp1 * dTmp2;
                    iDiscountCost = (int)dTmp3;
                    iCost = (int)(m_pItemForSaleList[i + m_dialogBoxes[11].sView]->m_wPrice * ((100 + m_cDiscount)/100.) );
                    iCost = iCost - iDiscountCost;

                    if (iCost < (m_pItemForSaleList[i + m_dialogBoxes[11].sView]->m_wPrice/2))
                    iCost = (m_pItemForSaleList[i + m_dialogBoxes[11].sView]->m_wPrice/2) - 1;*/

                    iCost = (int)(m_pItemForSaleList[i + m_dialogBoxes[11].sView]->m_wPrice * ((100 + m_cDiscount) / 100.));

                    ZeroMemory(cTemp, sizeof(cTemp));
                    wsprintfA(cTemp, "%6d", iCost);
                    if (onButton == i + 1)
                        PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 65, cTemp, 255, 255, 255);
                    else	PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 65, cTemp, 4, 0, 50);
                }
            break;

        default:
            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sSprite]->PutSpriteFast(sX + 62 + 30 - 35, sY + 84 + 30 - 10,
                                                                                                                                 m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sSpriteFrame, dwTime);

            ZeroMemory(cTemp, sizeof(cTemp));
            GetItemName(m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1], cTemp, cStr2, cStr3);

            PutAlignedString(sX + 25, sX + 240, sY + 50, cTemp, 255, 255, 255);
            PutAlignedString(sX + 26, sX + 241, sY + 50, cTemp, 255, 255, 255);

            strcpy(cTemp, DRAW_DIALOGBOX_SHOP3); //"PRICE"
            PutString3(sX + 90, sY + 78 + 30 - 10, cTemp, video::SColor(255, 40, 10, 10));
            strcpy(cTemp, DRAW_DIALOGBOX_SHOP6); // "Weight"
            PutString3(sX + 90, sY + 93 + 30 - 10, cTemp, video::SColor(255, 40, 10, 10));

            /*iDiscountRatio = ((m_stat[STAT_CHR] -10) / 4) ;
            dTmp1 = (double)iDiscountRatio;
            dTmp2 = dTmp1 / 100.0f;
            dTmp1 = (double)m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wPrice;
            dTmp3 = dTmp1 * dTmp2;
            iDiscountCost = (int)dTmp3;
            iCost = (int)(m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wPrice * ((100 +m_cDiscount)/100.) );
            iCost = iCost - iDiscountCost;

            if (iCost < (m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wPrice/2))
            iCost = (m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wPrice/2) - 1;*/

            iCost = (int)(m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wPrice * ((100 + m_cDiscount) / 100.));

            wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP7, iCost);
            //": %d Gold"
            PutString(sX + 140, sY + 98, cTemp, video::SColor(255, 45, 25, 25));

            iTemp = m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wWeight / 100;
            wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP8, iTemp);
            //": %d Stone"
            PutString(sX + 140, sY + 113, cTemp, video::SColor(255, 45, 25, 25));

            switch (m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_cEquipPos)
            {
                case EQUIPPOS_RHAND:
                case EQUIPPOS_TWOHAND:
                    strcpy(cTemp, DRAW_DIALOGBOX_SHOP9);  // Damage
                    PutString3(sX + 90, sY + 145, cTemp, video::SColor(255, 40, 10, 10));
                    strcpy(cTemp, DRAW_DIALOGBOX_SHOP10); //"Speed(Min.~Max.)"
                    PutString3(sX + 40, sY + 175, cTemp, video::SColor(255, 40, 10, 10));

                    if (m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue3 != 0)
                    {
                        wsprintfA(cTemp, ": %dD%d+%d (S-M)", m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue1,
                                  m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue2,
                                  m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue3);
                    }
                    else
                    {
                        wsprintfA(cTemp, ": %dD%d (S-M)", m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue1,
                                  m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue2);
                    }
                    PutString(sX + 140, sY + 145, cTemp, video::SColor(255, 45, 25, 25));
                    if (m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue6 != 0)
                    {
                        wsprintfA(cTemp, ": %dD%d+%d (L)", m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue4,
                                  m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5,
                                  m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue6);
                    }
                    else
                    {
                        wsprintfA(cTemp, ": %dD%d (L)", m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue4,
                                  m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5);
                    }
                    PutString(sX + 140, sY + 160, cTemp, video::SColor(255, 45, 25, 25));
                    if (m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_cSpeed == 0)   wsprintfA(cTemp, ": 0(10~10)");
                    else wsprintfA(cTemp, ": %d(%d ~ %d)", m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_cSpeed, iTemp, m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_cSpeed * 13);
                    PutString(sX + 140, sY + 175, cTemp, video::SColor(255, 45, 25, 25));

                    if ((m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wWeight / 100) > m_stat[STAT_STR])
                    {
                        wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP11, (m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wWeight / 100));
                        PutAlignedString(sX + 25, sX + 240, sY + 258, cTemp, 195, 25, 25);
                        PutAlignedString(sX + 26, sX + 241, sY + 258, cTemp, 195, 25, 25); // *Your STR should be at least %d to use this item."
                        bFlagRedShown = true;
                    }
                    break;

                case EQUIPPOS_LHAND:
                    strcpy(cTemp, DRAW_DIALOGBOX_SHOP12); // "Defence"
                    PutString3(sX + 90, sY + 145, cTemp, video::SColor(255, 40, 10, 10));
                    wsprintfA(cTemp, ": +%d%", m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue1);
                    PutString(sX + 140, sY + 145, cTemp, video::SColor(255, 45, 25, 25));
                    if ((m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wWeight / 100) > m_stat[STAT_STR])
                    {
                        wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP11, (m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wWeight / 100));
                        PutAlignedString(sX + 25, sX + 240, sY + 258, cTemp, 195, 25, 25); // "*Your STR should be at least %d to use this item."
                        PutAlignedString(sX + 26, sX + 241, sY + 258, cTemp, 195, 25, 25);
                        bFlagRedShown = true;
                    }
                    break;

                case EQUIPPOS_HEAD:
                case EQUIPPOS_BODY:
                case EQUIPPOS_BOOTS:
                case EQUIPPOS_ARMS:
                case EQUIPPOS_PANTS:
                    strcpy(cTemp, DRAW_DIALOGBOX_SHOP12); // "Defence"
                    PutString3(sX + 90, sY + 145, cTemp, video::SColor(255, 40, 10, 10));
                    wsprintfA(cTemp, ": +%d%", m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue1);
                    PutString(sX + 140, sY + 145, cTemp, video::SColor(255, 45, 25, 25));
                    strcpy(cTemp, DRAW_DIALOGBOX_SHOP28); // "Defence"
                    PutString3(sX + 90, sY + 170, cTemp, video::SColor(255, 40, 10, 10));
                    wsprintfA(cTemp, ": +%d%", m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue2);
                    PutString(sX + 140, sY + 170, cTemp, video::SColor(255, 45, 25, 25));
                    bFlagStatLow = false;
                    switch (m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue4)
                    {
                        case 10://"Available for above Str %d"
                            wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP15, m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5);
                            if (m_stat[STAT_STR] >= m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5)
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 45, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 45, 25, 25);
                            }
                            else
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 125, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 125, 25, 25);
                                bFlagStatLow = true; // Means some red msg need to be shown at the bottom
                            }
                            break;
                        case 11: // "Available for above Dex %d"
                            wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP16, m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5);
                            if (m_stat[STAT_DEX] >= m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5)
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 45, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 45, 25, 25);
                            }
                            else
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 125, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 125, 25, 25);
                                bFlagStatLow = true; // Means some red msg need to be shown at the bottom
                            }
                            break;
                        case 12: // "Available for above Vit %d"
                            wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP17, m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5);
                            if (m_stat[STAT_VIT] >= m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5)
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 45, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 45, 25, 25);
                            }
                            else
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 125, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 125, 25, 25);
                                bFlagStatLow = true; // Means some red msg need to be shown at the bottom
                            }
                            break;
                        case 13: // "Available for above Int %d"
                            wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP18, m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5);
                            if (m_stat[STAT_INT] >= m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5)
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 45, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 45, 25, 25);
                            }
                            else
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 125, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 125, 25, 25);
                                bFlagStatLow = true; // Means some red msg need to be shown at the bottom
                            }
                            break;
                        case 14: // "Available for above Mag %d"
                            wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP19, m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5);
                            if (m_stat[STAT_MAG] >= m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5)
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 45, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 45, 25, 25);
                            }
                            else
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 125, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 125, 25, 25);
                                bFlagStatLow = true; // Means some red msg need to be shown at the bottom
                            }
                            break;
                        case 15: // "Available for above Chr %d"
                            wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP20, m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5);
                            if (m_stat[STAT_CHR] >= m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sItemEffectValue5)
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 45, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 45, 25, 25);
                            }
                            else
                            {
                                PutAlignedString(sX + 25, sX + 240, sY + 160, cTemp, 125, 25, 25);
                                PutAlignedString(sX + 26, sX + 241, sY + 160, cTemp, 125, 25, 25);
                                bFlagStatLow = true; // Means some red msg need to be shown at the bottom
                            }
                            break;

                        default:
                            break;
                    }
                    if ((m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wWeight / 100) > m_stat[STAT_STR])
                    {
                        wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP11, (m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_wWeight / 100));
                        PutAlignedString(sX + 25, sX + 240, sY + 288, cTemp, 195, 25, 25);
                        PutAlignedString(sX + 26, sX + 241, sY + 288, cTemp, 195, 25, 25); // "*Your STR should be at least %d to use this item."
                        bFlagRedShown = true;
                    }
                    else if (bFlagStatLow == true) // Means some stat is too low
                    {
                        strcpy(cTemp, DRAW_DIALOGBOX_SHOP21); // "(Warning!) Your stat is too low for this item."
                        PutAlignedString(sX + 25, sX + 240, sY + 258, cTemp, 195, 25, 25);
                        PutAlignedString(sX + 26, sX + 241, sY + 258, cTemp, 195, 25, 25);
                        bFlagRedShown = true;
                    }
                    else if ((strstr(m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_cName, "(M)") != 0)
                             && (m_sPlayerType > 3))
                    {
                        strcpy(cTemp, DRAW_DIALOGBOX_SHOP22); // "(Warning!) only for male."
                        PutAlignedString(sX + 25, sX + 240, sY + 258, cTemp, 195, 25, 25);
                        PutAlignedString(sX + 26, sX + 241, sY + 258, cTemp, 195, 25, 25);
                        bFlagRedShown = true;
                    }
                    else if ((strstr(m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_cName, "(W)") != 0)
                             && (m_sPlayerType <= 3))
                    {
                        strcpy(cTemp, DRAW_DIALOGBOX_SHOP23); // "(Warning!) only for female."
                        PutAlignedString(sX + 25, sX + 240, sY + 258, cTemp, 195, 25, 25);
                        PutAlignedString(sX + 26, sX + 241, sY + 258, cTemp, 195, 25, 25);
                        bFlagRedShown = true;
                    }
                    break;

                case EQUIPPOS_NONE:
                    break;
            }

            if (m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sLevelLimit != 0)
            {
                strcpy(cTemp, DRAW_DIALOGBOX_SHOP24); // "Level"
                if (m_iLevel >= m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sLevelLimit)
                {
                    PutString(sX + 90, sY + 190, cTemp, video::SColor(255, 40, 10, 10));
                    PutString(sX + 91, sY + 190, cTemp, video::SColor(255, 40, 10, 10));
                    wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP25, m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sLevelLimit);
                    PutString(sX + 140, sY + 190, cTemp, video::SColor(255, 45, 25, 25));// ": above %d"
                }
                else
                {
                    PutString(sX + 90, sY + 190, cTemp, video::SColor(255, 40, 10, 10));
                    PutString(sX + 91, sY + 190, cTemp, video::SColor(255, 40, 10, 10));
                    wsprintfA(cTemp, DRAW_DIALOGBOX_SHOP25, m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_sLevelLimit);
                    PutString(sX + 140, sY + 190, cTemp, video::SColor(255, 125, 25, 25));// ": above %d"
                    if (bFlagRedShown == false)
                    {
                        strcpy(cTemp, DRAW_DIALOGBOX_SHOP26); // "(Warning!) Your level is too low for this item."
                        PutAlignedString(sX + 25, sX + 240, sY + 258, cTemp, 195, 25, 25);
                        PutAlignedString(sX + 25 + 1, sX + 240 + 1, sY + 258, cTemp, 195, 25, 25);
                        bFlagRedShown = true;
                    }
                }
            }

            m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 156, sY + 219, 19, dwTime);
            m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 170, sY + 219, 19, dwTime);
            PutString(sX + 123 - 35, sY + 237 - 10, DRAW_DIALOGBOX_SHOP27, video::SColor(255, 40, 10, 10)); // "Quantity:"
            PutString(sX + 124 - 35, sY + 237 - 10, DRAW_DIALOGBOX_SHOP27, video::SColor(255, 40, 10, 10));
            if (iGetTopDialogBoxIndex() == 11 && G_pGame->m_stMCursor.sZ != 0)
            {
                m_dialogBoxes[11].sV3 = m_dialogBoxes[11].sV3 + G_pGame->m_stMCursor.sZ / 60;
                //DIRECTX m_dInput.m_sZ = 0;
            }

            int maxWeight = m_stat[STAT_STR] * 5 + m_iLevel * 5;
            if (!strcmp(m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_cName, "100Arrows"))
            {
                if (m_weight + (m_dialogBoxes[11].sV3 * 10) >  maxWeight)
                {
                    m_dialogBoxes[11].sV3 = (maxWeight - m_weight) / 10;
                }
                if (m_dialogBoxes[11].sV3 > 99) m_dialogBoxes[11].sV3 = 99;
            }
            else if (!strcmp(m_pItemForSaleList[m_dialogBoxes[11].GetMode() - 1]->m_cName, "10Arrows"))
            {
                if (m_weight + m_dialogBoxes[11].sV3 >  maxWeight)
                {
                    m_dialogBoxes[11].sV3 = (maxWeight - m_weight);
                }
                if (m_dialogBoxes[11].sV3 > 99) m_dialogBoxes[11].sV3 = 99;
            }
            else if (m_dialogBoxes[11].sV3 > (75 - _iGetTotalItemNum())) // Bag Fix xRisenx
                m_dialogBoxes[11].sV3 = (75 - _iGetTotalItemNum()); // Bag Fix xRisenx

            if (m_dialogBoxes[11].sV3 < 1) m_dialogBoxes[11].sV3 = 1;

            if (m_dialogBoxes[11].sV3 >= 10)
            {
                ZeroMemory(cTemp, sizeof(cTemp));
                _itoa(m_dialogBoxes[11].sV3, cTemp, 10);
                cTemp[1] = 0;
                PutString(sX - 35 + 186, sY - 10 + 237, cTemp, video::SColor(255, 40, 10, 10));
                PutString(sX - 35 + 187, sY - 10 + 237, cTemp, video::SColor(255, 40, 10, 10));
                ZeroMemory(cTemp, sizeof(cTemp));
                _itoa(m_dialogBoxes[11].sV3, cTemp, 10);
                PutString(sX - 35 + 200, sY - 10 + 237, (cTemp + 1), video::SColor(255, 40, 10, 10));
                PutString(sX - 35 + 201, sY - 10 + 237, (cTemp + 1), video::SColor(255, 40, 10, 10));
            }
            else
            {
                PutString(sX - 35 + 186, sY - 10 + 237, "0", video::SColor(255, 40, 10, 10));
                PutString(sX - 35 + 187, sY - 10 + 237, "0", video::SColor(255, 40, 10, 10));
                ZeroMemory(cTemp, sizeof(cTemp));
                _itoa(m_dialogBoxes[11].sV3, cTemp, 10);
                PutString(sX - 35 + 200, sY - 10 + 237, (cTemp), video::SColor(255, 40, 10, 10));
                PutString(sX - 35 + 201, sY - 10 + 237, (cTemp), video::SColor(255, 40, 10, 10));
            }
            m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 156, sY + 244, 20, dwTime);
            m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 170, sY + 244, 20, dwTime);

            if (onButton == 5)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 31);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + LBTNPOSX, sY + BTNPOSY, 30);

            if (onButton == 6)
                DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 17);
            else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 16);
            break;
    }
}

void CGame::DrawDialogBox_DKMenuWeapons()
{
    short sX, sY, szX;

    char onButton = m_dialogBoxes[53].OnButton();
    sX = m_dialogBoxes[53].m_X;
    sY = m_dialogBoxes[53].m_Y;
    szX = m_dialogBoxes[53].sSizeX;
    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);

    if (onButton == 1)
        PutAlignedString(sX, sX + szX, sY + 50 + 15 * 0, "Dark Mage Wand", 255, 255, 255);
    else PutAlignedString(sX, sX + szX, sY + 50 + 15 * 0, "Dark Mage Wand", 4, 0, 50);

    if (onButton == 2)
        PutAlignedString(sX, sX + szX, sY + 50 + 15 * 1, "Dark Knight Rapier", 255, 255, 255);
    else PutAlignedString(sX, sX + szX, sY + 50 + 15 * 1, "Dark Knight Rapier", 4, 0, 50);

    if (onButton == 3)
        PutAlignedString(sX, sX + szX, sY + 50 + 15 * 2, "Dark Knight Great Sword", 255, 255, 255);
    else PutAlignedString(sX, sX + szX, sY + 50 + 15 * 2, "Dark Knight Great Sword", 4, 0, 50);

    if (onButton == 4)
        PutAlignedString(sX, sX + szX, sY + 50 + 15 * 3, "Dark Knight Flameberge", 255, 255, 255);
    else PutAlignedString(sX, sX + szX, sY + 50 + 15 * 3, "Dark Knight Flameberge", 4, 0, 50);

    if (onButton == 5)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);

}

void CGame::DlgBoxClick_DKMenuWeapons()
{
    char onButton = m_dialogBoxes[53].OnButton();
    // DK Weapons
    if (onButton == 1 && (m_iLevel >= MAXLEVEL)) // Dark Mage Staff
    {
        bSendCommand(MSGID_REQUEST_DKITEM, 0, 0, 10, 0, 0, "Gail", 0);
        PlaySound('E', 14, 5);
    }
    if (onButton == 2 && (m_iLevel >= MAXLEVEL)) // Dark Knight Rapier
    {
        bSendCommand(MSGID_REQUEST_DKITEM, 0, 0, 11, 0, 0, "Gail", 0);
        PlaySound('E', 14, 5);
    }
    if (onButton == 3 && (m_iLevel >= MAXLEVEL)) // Dark Knight Great Sword
    {
        bSendCommand(MSGID_REQUEST_DKITEM, 0, 0, 12, 0, 0, "Gail", 0);
        PlaySound('E', 14, 5);
    }
    if (onButton == 4 && (m_iLevel >= MAXLEVEL)) // Dark Knight Flameberge
    {
        bSendCommand(MSGID_REQUEST_DKITEM, 0, 0, 13, 0, 0, "Gail", 0);
        PlaySound('E', 14, 5);
    }
    if (onButton == 5)
    {
        PlaySound('E', 14, 5);
        DisableDialogBox(53);
    }
}

void CGame::DlgBoxClick_GuideMap()
{
    if (m_bCtrlPressed == false) return;

    short shX;
    short shY;
    int recallPoint = 0;
    int town;
    int range = 25;
    int recallPoints[2][5][2] = {
        { { 140,49 },{ 68,125 },{ 170,145 },{ 140,205 },{ 116,245 } }, //Aresden
        { { 158,57 },{ 110,89 },{ 170,145 },{ 242,129 },{ 158,249 } } //Elvine
    };

    if (m_bZoomMap)
    {
        shX = m_sPlayerX - 64;
        shY = m_sPlayerY - 64;
        if (shX < 0) shX = 0;
        else if (shX > m_pMapData->m_sMapSizeX - 128) shX = m_pMapData->m_sMapSizeX - 128;
        if (shY < 0) shY = 0;
        else if (shY > m_pMapData->m_sMapSizeY - 128) shY = m_pMapData->m_sMapSizeY - 128;
        shX += G_pGame->m_stMCursor.sX - m_dialogBoxes[9].m_X;
        shY += G_pGame->m_stMCursor.sY - m_dialogBoxes[9].m_Y;
    }
    else
    {
        shX = (G_pGame->m_stMCursor.sX - m_dialogBoxes[9].m_X)*m_pMapData->m_sMapSizeX / 128;
        shY = (G_pGame->m_stMCursor.sY - m_dialogBoxes[9].m_Y)*m_pMapData->m_sMapSizeY / 128;
    }

    if (m_cMapIndex == 11) town = 0;
    else if (m_cMapIndex == 3) town = 1;
    else return;

    for (int i = 0; i < 6; i++)
        if (shX > (recallPoints[town][i][0] - range) && shX < (recallPoints[town][i][0] + range) &&
            shY >(recallPoints[town][i][1] - range) && shY < (recallPoints[town][i][1] + range))
        {
            recallPoint = i + 1;
            break;
        }

    if (recallPoint != 0)
    {
        bSendCommand(MSGID_REQUEST_SETRECALLPNT, 0, 0, recallPoint, 0, 0, 0, 0);
        PlaySound('E', 14, 5);
    }
}

void CGame::DlgBoxClick_GuildBank()
{
    int i;
    short sX, sY;

    char onButton = m_dialogBoxes[56].OnButton();
    sX = m_dialogBoxes[56].m_X;
    sY = m_dialogBoxes[56].m_Y;

    ItemMapIter it = m_guildBankIt;
    int j = 0;

    switch (m_dialogBoxes[56].GetMode())
    {
        case 0:
            for (i = 0; i < m_dialogBoxes[56].sV1; i++)
            {
                if (onButton == i + 1)
                {
                    if (_iGetTotalItemNum() >= 75)
                    { // Bag Fix xRisenx
                        AddEventList(DLGBOX_CLICK_BANK1, 10);
                        return;
                    }

                    if (m_iGuildRank == GUILDRANK_NONE || !gldRankPerm[m_iGuildRank].canWithdrawWH)
                    {
                        AddEventList(DLGBOX_CLICK_BANK2, 10);
                        return;
                    }

                    while (j < i && it != m_guildBankMap.end())
                    {
                        ++it;
                        j++;
                    }

                    if (it == m_guildBankMap.end()) return;

                    bSendCommand(MSGID_REQUEST_RETRIEVEITEM_GUILDBANK, 0, 0, (*it).second, 0, 0, 0);
                    m_dialogBoxes[56].SetMode(-1);
                    PlaySound('E', 56, 5);
                }
            }
            break;
    }
}

void CGame::DrawDialogBox_GuildBank()
{
    short sX, sY, szX;
    int  iTotalLines;
    double d1, d2, d3;
    char cItemColor, cStr1[64], cStr2[64], cStr3[64];
    bool bFlag = false;
    Point pointerLoc;
    int iPointerLoc;
    char onButton = m_dialogBoxes[56].OnButton();
    sX = m_dialogBoxes[56].m_X;
    sY = m_dialogBoxes[56].m_Y;
    szX = m_dialogBoxes[56].sSizeX - 5;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 21);
    ZeroMemory(cStr1, sizeof(cStr1));
    ZeroMemory(cStr2, sizeof(cStr2));
    ZeroMemory(cStr3, sizeof(cStr3));


    ItemMapIter it;
    CItem * item;

    switch (m_dialogBoxes[56].GetMode())
    {
        case -1:
            PutString(sX + 30 + 15, sY + 70, DRAW_DIALOGBOX_BANK1, video::SColor(255, 0, 0, 0));
            PutString(sX + 30 + 15, sY + 85, DRAW_DIALOGBOX_BANK2, video::SColor(255, 0, 0, 0));
            break;

        case 0:
            iTotalLines = m_guildBankMap.size();
            iPointerLoc = 0;
            if (iTotalLines > m_dialogBoxes[56].sV1)
            {
                d1 = (double)m_dialogBoxes[56].sView;
                d2 = (double)(iTotalLines - m_dialogBoxes[56].sV1);
                d3 = (274.0f * d1) / d2;
                iPointerLoc = (int)d3;
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX + 242, sY + iPointerLoc + 35, 7);
            }

            it = m_guildBankIt;
            for (int i = 0; i < m_dialogBoxes[56].sV1 && it != m_guildBankMap.end(); i++, ++it)
            {
                item = (*it).first;
                if (!item) continue;
                if (isItemLoaded == false)
                {
                    _LoadItemContents();		// Item Display - ShadowEvil
                    isItemLoaded = true;
                }
                std::vector<string> * lines = GetItemName(item, true);
                std::vector<string>::iterator iter = lines->begin(), end = lines->end();
                if (onButton == i + 1)
                {
                    bFlag = true;

                    if (iter != end)
                    {
                        PutAlignedString(sX, sX + szX, sY + 110 + i * 15, iter->c_str(), 255, 255, 255);
                    }

                    uint32_t maxWidth = 0;

                    while (iter != end)
                    {
                        if (iter->size() > maxWidth)
                            maxWidth = iter->size();

                        ++iter;
                    }

                    //DIRECTX m_DDraw.DrawShadowBox(sX + 70 - 3, sY + 25 - 1, sX + 70 + maxWidth * 5.5, sY + 25 + 13*lines->size());

                    iter = lines->begin();

                    int iLoc = 0;
                    if (iter != end)
                    {
                        PutString(sX + 70, sY + 25, iter->c_str(), video::SColor(m_itemColor), false, 1);
                        iLoc += 13;
                        ++iter;
                    }

                    while (iter != end)
                    {
                        PutString(sX + 70, sY + 25 + iLoc, iter->c_str(), video::SColor(255, 150, 150, 150), false, 1);
                        iLoc += 13;
                        ++iter;
                    }

                    cItemColor = item->m_ItemColor;
                    if (cItemColor == 0)
                    {
                        m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + item->m_sSprite]->PutSpriteFast(sX + 40, sY + 68, item->m_sSpriteFrame, m_dwCurTime);
                    }
                    else
                    {
                        if (item->m_cEquipPos == EQUIPPOS_LHAND
                            || item->m_cEquipPos == EQUIPPOS_RHAND
                            || item->m_cEquipPos == EQUIPPOS_TWOHAND)
                        {
                            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + item->m_sSprite]->PutSpriteRGB(sX + 40, sY + 68, item->m_sSpriteFrame,
                                                                                                 m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
                        }
                        else
                            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + item->m_sSprite]->PutSpriteRGB(sX + 40, sY + 68, item->m_sSpriteFrame,
                                                                                                 m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
                    }
                }
                else
                {
                    if (video::SColor(m_itemColor) != video::SColor(255, 255, 255, 255))
                        PutAlignedString(sX, sX + szX, sY + 110 + i * 15, iter->c_str(), video::SColor(m_itemColor));
                    else
                        PutAlignedString(sX, sX + szX, sY + 110 + i * 15, iter->c_str(), video::SColor(255, 0, 0, 0));
                }
            }

            int32_t oldView = m_dialogBoxes[56].sView;

            if (G_pGame->m_stMCursor.LB != 0 && (iGetTopDialogBoxIndex() == 56) && iTotalLines > m_dialogBoxes[56].sV1)
            {
                if ((G_pGame->m_stMCursor.sX >= sX + 230) && (G_pGame->m_stMCursor.sX <= sX + 260) && (G_pGame->m_stMCursor.sY >= sY + 40) && (G_pGame->m_stMCursor.sY <= sY + 320))
                {
                    d1 = (double)(G_pGame->m_stMCursor.sY - (sY + 35));
                    d2 = (double)(iTotalLines - m_dialogBoxes[56].sV1);
                    d3 = (d1 * d2) / 274.0f;
                    m_dialogBoxes[56].sView = (int)(d3 + 0.5);
                }
                else if ((G_pGame->m_stMCursor.sX >= sX + 230) && (G_pGame->m_stMCursor.sX <= sX + 260) && (G_pGame->m_stMCursor.sY > sY + 10) && (G_pGame->m_stMCursor.sY < sY + 40)) m_dialogBoxes[56].sView = 0;
            }
            else m_dialogBoxes[56].bIsScrollSelected = false;
            if (iGetTopDialogBoxIndex() == 56 && G_pGame->m_stMCursor.sZ != 0)
            {
                if (iTotalLines > 50)
                    m_dialogBoxes[56].sView = m_dialogBoxes[56].sView - G_pGame->m_stMCursor.sZ / 30;
                else
                {
                    if (G_pGame->m_stMCursor.sZ > 0 && m_guildBankIt != m_guildBankMap.begin())
                    {
                        m_dialogBoxes[56].sView--;
                    }
                    if (G_pGame->m_stMCursor.sZ < 0 && iTotalLines - m_dialogBoxes[56].sV1 - m_dialogBoxes[56].sView > 0)
                    {
                        m_dialogBoxes[56].sView++;
                    }
                }
                //DIRECTX m_dInput.m_sZ = 0;
            }
            if (iTotalLines > m_dialogBoxes[56].sV1 && m_dialogBoxes[56].sView > iTotalLines - m_dialogBoxes[56].sV1) m_dialogBoxes[56].sView = iTotalLines - m_dialogBoxes[56].sV1;
            if (iTotalLines <= m_dialogBoxes[56].sV1) m_dialogBoxes[56].sView = 0;

            if (m_dialogBoxes[56].sView < 0) m_dialogBoxes[56].sView = 0;


            if (oldView < m_dialogBoxes[56].sView)
            {
                while (oldView != m_dialogBoxes[56].sView)
                {
                    oldView++;
                    ++m_guildBankIt;
                }
            }
            else if (oldView > m_dialogBoxes[56].sView)
            {
                while (oldView != m_dialogBoxes[56].sView)
                {
                    oldView--;
                    --m_guildBankIt;
                }
            }


            if (bFlag == false)
            {
                PutAlignedString(sX, sX + szX, sY + 45, DRAW_DIALOGBOX_BANK3);
                PutAlignedString(sX, sX + szX, sY + 60, DRAW_DIALOGBOX_BANK4);
                PutAlignedString(sX, sX + szX, sY + 75, DRAW_DIALOGBOX_BANK5);
                wsprintfA(G_cTxt, DRAW_DIALOGBOX_BANK6, iTotalLines, maxGWHItems[m_guild.m_upgrades[GU_WAREHOUSE]]);
                PutAlignedString(sX, sX + szX, sY + 90, G_cTxt, 255, 255, 255);
            }
            break;
    }
}

void CGame::DrawDialogBox_ExtendedSysMenu()
{
    char onButton = m_dialogBoxes[59].OnButton();
    char menuTxt[64];

    PutString_SprFont(m_dialogBoxes[59].m_X + 120, m_dialogBoxes[59].m_Y + 25, DRAW_EXTENDEDSYSMENU0, 1, 1, 8);

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU1);
    strcat(menuTxt, m_showGrid ? DRAW_ENABLED : DRAW_DISABLED);
    RECT r = m_dialogBoxes[59].GetButton(1);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 1 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU2);
    strcat(menuTxt, m_tabbedNotification ? DRAW_ENABLED : DRAW_DISABLED);
    r = m_dialogBoxes[59].GetButton(2);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 2 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU3);
    strcat(menuTxt, m_ekScreenshot ? DRAW_ENABLED : DRAW_DISABLED);
    r = m_dialogBoxes[59].GetButton(3);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 3 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU4);
    strcat(menuTxt, m_showAllDmg ? DRAW_ENABLED : DRAW_DISABLED);
    r = m_dialogBoxes[59].GetButton(4);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 4 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU5);
    strcat(menuTxt, m_showTimeStamp ? DRAW_ENABLED : DRAW_DISABLED);
    r = m_dialogBoxes[59].GetButton(5);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 5 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU6);
    strcat(menuTxt, m_bigItems ? DRAW_ENABLED : DRAW_DISABLED);
    r = m_dialogBoxes[59].GetButton(6);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 6 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU7);
    strcat(menuTxt, m_windowsKey ? DRAW_ENABLED : DRAW_DISABLED);
    r = m_dialogBoxes[59].GetButton(7);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 7 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU8);
    strcat(menuTxt, m_showTime ? DRAW_ENABLED : DRAW_DISABLED);
    r = m_dialogBoxes[59].GetButton(8);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 8 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU9);
    r = m_dialogBoxes[59].GetButton(9);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 9 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU10);
    r = m_dialogBoxes[59].GetButton(10);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 10 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU11);
    r = m_dialogBoxes[59].GetButton(11);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 11 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

    strcpy(menuTxt, DRAW_EXTENDEDSYSMENU12);
    r = m_dialogBoxes[59].GetButton(12);
    PutAlignedString(r.left, r.right, r.top, menuTxt, onButton == 12 ? video::SColor(255, 255, 255, 255) : video::SColor(255, 4, 0, 50));

}

void CGame::DlgBoxClick_ExtendedSysMenu()
{
    char onButton = m_dialogBoxes[59].OnButton();

    switch (onButton)
    {
        case 1:
            m_showGrid = !m_showGrid;
            break;

        case 2:
            m_tabbedNotification = !m_tabbedNotification;
            if (m_tabbedNotification)
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND18, 10);
            else
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND19, 10);
            break;

        case 3:
            m_ekScreenshot = !m_ekScreenshot;
            if (m_ekScreenshot)
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND16, 10);
            else
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND17, 10);
            break;

        case 4:
            m_showAllDmg = !m_showAllDmg;
            if (m_showAllDmg)
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND12, 10);
            else
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND13, 10);
            break;

        case 5:
            m_showTimeStamp = !m_showTimeStamp;
            if (m_showTimeStamp)
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND10, 10);
            else
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND11, 10);
            break;

        case 6:
            m_bigItems = !m_bigItems;
            if (m_bigItems)
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND14, 10);
            else
                AddEventList(BCHECK_LOCAL_CHAT_COMMAND15, 10);
            break;

        case 7:
            m_windowsKey = !m_windowsKey;
            //SetKeyboardHook(!m_windowsKey);
            break;

        case 8:
            m_showTime = !m_showTime;
            break;

        case 9:
            LoadFriendList();
            UpdateFriendsStatus();
            EnableDialogBox(43, 0, 0, 0);
            break;

        case 10:
            if (m_bIsDialogEnabled[60])  DisableDialogBox(60);
            else EnableDialogBox(60, 0, 0, 0);
            break;

        case 11:
            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, "/afk");
            break;

        case 12:
            m_bShowFPS = !m_bShowFPS;
            break;
    }
}

void CGame::DrawDialogBox_MuteList()
{
    char onButton = m_dialogBoxes[60].OnButton();
    char playerTxt[32];
    int mutes = 0;
    int overButton;
    const int maxShown = 15;

    short sX = m_dialogBoxes[60].m_X;
    short sY = m_dialogBoxes[60].m_Y;

    PutString_SprFont(sX + 98, sY + 36, DRAW_MUTELIST, 1, 1, 8);

    for (std::set<string>::iterator it = m_MuteList.begin(); it != m_MuteList.end(); ++it)
    {
        mutes++;

        if (mutes > m_dialogBoxes[60].sView &&
            mutes < m_dialogBoxes[60].sView + maxShown)
        {
            overButton = mutes + 3 - m_dialogBoxes[60].sView;
            //uncomment 			PutAlignedString(&m_dialogBoxes[60].GetButton(overButton), it->c_str(), (onButton == overButton) ?video::SColor(255,255,255,255) :video::SColor(255,0,0,0));
        }
    }

    wsprintfA(playerTxt, MSG_FRIENDSLIST5, m_dialogBoxes[60].cStr);
    PutString(sX + 30, m_dialogBoxes[60].ButtonY(2) - 15, playerTxt, video::SColor(255, 0, 0, 0));

    //uncomment 	PutAlignedString(&m_dialogBoxes[60].GetButton(2), DRAW_MUTELIST1, onButton == 2 ?video::SColor(255,255,255,255) :video::SColor(255,0,0,0));

    Point pointerLoc = m_dialogBoxes[60].HandleScroll(maxShown, mutes);
    if (mutes > maxShown)
    {
        DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
        DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
    }
}

void CGame::DlgBoxClick_MuteList()
{
    char onButton = m_dialogBoxes[60].OnButton();
    const int maxShown = 15;
    int mutes = 0;
    char playerTxt[128];

    for (std::set<string>::iterator it = m_MuteList.begin(); it != m_MuteList.end(); ++it)
    {
        mutes++;

        if (onButton == mutes + 3 - m_dialogBoxes[60].sView)
        {
            strcpy(m_dialogBoxes[60].cStr, it->c_str());
            break;
        }
    }

    if (onButton == 2)
    {
        if (m_MuteList.erase(m_dialogBoxes[60].cStr))
        {
            wsprintfA(playerTxt, BCHECK_LOCAL_CHAT_COMMAND1, m_dialogBoxes[60].cStr);
            AddEventList(playerTxt, 10);
            SaveMuteList();
            m_dialogBoxes[60].cStr[0] = '\0';
        }
    }
}

void CGame::DrawDialogBox_QuestList()
{
    short sX, sY;
    char cTemp[255], cTemp2[255], cTemp3[255], cTargetName[21], cNpcName[21];
    Point pointerLoc;
    COLORREF colour;

    char onButton = m_dialogBoxes[61].OnButton();
    sX = m_dialogBoxes[61].m_X;
    sY = m_dialogBoxes[61].m_Y;

    switch (m_dialogBoxes[61].GetMode())
    {
        case 0:
            if (!m_stQuest.bIsQuestCompleted)
            {
                if (m_stQuest.sQuestType == 0)
                {
                    PutAlignedString(sX, sX + 90, sY + 30, "Find", 255, 255, 255);
                    PutAlignedString(sX, sX + 300, sY + 30, "Location", 255, 255, 255);
                    PutAlignedString(sX, sX + 435, sY + 30, "contri", 255, 255, 255);

                    pointerLoc = m_dialogBoxes[61].HandleScroll(17, m_iQuestCount);

                    if (m_iQuestCount > 17)
                    {
                        DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                        DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
                    }

                    for (int i = 0; i < 17; i++)
                    {
                        if (i + m_dialogBoxes[61].sView < m_iQuestCount)
                        {
                            ZeroMemory(cNpcName, sizeof(cNpcName));
                            GetNpcName(m_stQuestList[i + m_dialogBoxes[61].sView].TargetType, cNpcName);
                            ZeroMemory(cTargetName, sizeof(cTargetName));
                            GetOfficialMapName(m_stQuestList[i + m_dialogBoxes[61].sView].TargetName, cTargetName);

                            if (m_stQuestList[i + m_dialogBoxes[61].sView].sX != 0)
                            {
                                ZeroMemory(cTemp, sizeof(cTemp));
                                wsprintfA(cTemp, "%s", cTargetName);
                                m_Misc.ReplaceString(cTemp, '-', ' ');

                                ZeroMemory(cTemp2, sizeof(cTemp2));
                                wsprintfA(cTemp2, "%3d,%3d Range:%2d", m_stQuestList[i + m_dialogBoxes[61].sView].sX, m_stQuestList[i + m_dialogBoxes[61].sView].sY, m_stQuestList[i + m_dialogBoxes[61].sView].Range);

                                ZeroMemory(cTemp3, sizeof(cTemp3));
                                wsprintfA(cTemp3, "%d", m_stQuestList[i + m_dialogBoxes[61].sView].Contribution);
                            }
                            else
                            {
                                ZeroMemory(cTemp, sizeof(cTemp));
                                wsprintfA(cTemp, "%d %s", m_stQuestList[i + m_dialogBoxes[61].sView].MaxCount, cNpcName);
                                m_Misc.ReplaceString(cTemp, '-', ' ');

                                ZeroMemory(cTemp2, sizeof(cTemp2));
                                wsprintfA(cTemp2, "%s", cTargetName);
                                m_Misc.ReplaceString(cTemp2, '-', ' ');

                                ZeroMemory(cTemp3, sizeof(cTemp3));
                                wsprintfA(cTemp3, "%d", m_stQuestList[i + m_dialogBoxes[61].sView].Contribution);
                            }

                            if (onButton == i + 1)
                                colour = video::SColor(255, 255, 255, 255).color;
                            else colour = video::SColor(255, 4, 0, 50).color;

                            PutString(sX + 20, sY + 45 + i * 15, cTemp, colour);
                            PutString(sX + 123, sY + 45 + i * 15, cTemp2, colour);
                            PutString(sX + 215, sY + 45 + i * 15, cTemp3, colour);
                        }
                    }

                }
                else
                {
                    PutAlignedString(sX, sX + 250, sY + 125, DRAW_DIALOGBOX_QUESTLIST_MENU1, 55, 25, 25);
                    PutAlignedString(sX, sX + 250, sY + 140, DRAW_DIALOGBOX_QUESTLIST_MENU2, 55, 25, 25);
                }
            }
            else
            {
                PutAlignedString(sX, sX + 255, sY + 125, DRAW_DIALOGBOX_QUESTLIST_MENU3, 55, 25, 25);
                PutAlignedString(sX, sX + 250, sY + 140, DRAW_DIALOGBOX_QUESTLIST_MENU4, 55, 25, 25);
            }
            break;
    }
}

void CGame::DlgBoxClick_QuestList()
{
    char onButton = m_dialogBoxes[61].OnButton();

    if (m_iQuestCount > 0 && onButton && !m_stQuest.bIsQuestCompleted)
    {
        bSendCommand(MSGID_REQUEST_ACCEPT_QUEST, 0, 0, m_stQuestList[(onButton - 1) + m_dialogBoxes[61].sView].iIndex, 0, 0, 0);
        DisableDialogBox(61);
        DisableDialogBox(13);
        return;
    }
}

void CGame::DrawDialogBox_Titles()
{
    short sX, sY;
    uint32_t dwTime = m_dwCurTime;
    int  i, iTemp;
    char cTemp[255], cStr2[255], cStr3[255], cTemp2[256];
    int  iCurr, iNext;

    double dTmp1, dTmp2, dTmp3;
    int  iTotalLines;
    bool bFlagStatLow = false;
    bool bFlagRedShown = false;
    double d1, d2, d3;
    Point pointerLoc;

    char onButton = m_dialogBoxes[63].OnButton();
    sX = m_dialogBoxes[63].m_X;
    sY = m_dialogBoxes[63].m_Y;

    DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 23);
    //PutString_SprFont(sX+100,sY+35,"Titles",7,0,0); // previously 1,1,8

    switch (m_dialogBoxes[63].GetMode())
    {
        case 0:
            iTotalLines = 0;
            for (i = 0; i<50; i++)
            {
                if (m_pTitles[i] != 0) iTotalLines++;
            }

            pointerLoc = m_dialogBoxes[63].HandleScroll(14, iTotalLines);

            if (iTotalLines > 14)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 1);
                DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
            }
            if (iTotalLines != 0)
            {
                for (int line = 0, TitleIndex = 0; line < 14; line++)
                {
                    if (line < iTotalLines /*&& m_pTitles[TitleIndex+m_dialogBoxes[63].sView]*/)
                    {
                        ZeroMemory(cTemp, sizeof(cTemp));
                        ZeroMemory(cTemp2, sizeof(cTemp2));
                        GetTitleName(m_pTitles[TitleIndex + m_dialogBoxes[63].sView]->m_cSubject, m_pTitles[TitleIndex + m_dialogBoxes[63].sView]->m_iRank, cTemp2);
                        m_Misc.ReplaceString(cTemp, '-', ' ');
                        memcpy(cTemp, cTemp2, 23); // shorten the string to 23 characters

                        if ((TitleIndex + m_dialogBoxes[63].sView) == m_iTitleIndex)// Changed from 53 to 60 xRisenx
                        {
                            PutString2(sX + 25, sY + TitleIndex * 18 + 65 - 30, cTemp, 255, 255, 204);
                        }
                        else
                        {
                            if ((G_pGame->m_stMCursor.sX >= sX + 20) && (G_pGame->m_stMCursor.sX <= sX + 220) && (G_pGame->m_stMCursor.sY >= sY + TitleIndex * 18 + 65 - 30) && (G_pGame->m_stMCursor.sY <= sY + TitleIndex * 18 + 79 - 30))
                            {
                                PutString2(sX + 25, sY + TitleIndex * 18 + 65 - 30, cTemp, 255, 255, 255);
                            }
                            else
                            {
                                PutString2(sX + 25, sY + TitleIndex * 18 + 65 - 30, cTemp, 128, 128, 128);
                            }
                        }

                        iCurr = (int)(m_pTitles[TitleIndex + m_dialogBoxes[63].sView]->m_iCurrentNo);

                        iNext = 99999;
                        if (m_pTitles[TitleIndex + m_dialogBoxes[63].sView] != 0)// Changed from 53 to 60 xRisenx
                        {
                            iNext = m_pTitles[TitleIndex + m_dialogBoxes[63].sView]->m_iNextRankPoints;// Changed from 53 to 60 xRisenx
                        }

                        ZeroMemory(cTemp, sizeof(cTemp));
                        wsprintfA(cTemp, "(%d/%d)", iCurr, iNext);

                        if ((TitleIndex + m_dialogBoxes[63].sView) == m_iTitleIndex)// Changed from 53 to 60 xRisenx
                        {
                            PutString2(sX + 145, sY + TitleIndex * 18 + 65 - 30, cTemp, 255, 255, 204);
                        }
                        else
                        {
                            if ((G_pGame->m_stMCursor.sX >= sX + 20) && (G_pGame->m_stMCursor.sX <= sX + 230) && (G_pGame->m_stMCursor.sY >= sY + TitleIndex * 18 + 65 - 30) && (G_pGame->m_stMCursor.sY <= sY + TitleIndex * 18 + 79 - 30))
                            {
                                PutString2(sX + 145, sY + TitleIndex * 18 + 65 - 30, cTemp, 255, 255, 255);
                            }
                            else
                            {
                                PutString2(sX + 145, sY + TitleIndex * 18 + 65 - 30, cTemp, 128, 128, 128);
                            }
                        }
                        TitleIndex++;
                    }
                }
            }
            // Clear Title
            if (m_iTitleIndex == -1)
            {
                PutString2(sX + 25, sY + 45 + (14 * 18), "Clear Title", 128, 128, 128); // No title choosen
                                                                                        //DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX+, sY +225, 22);
            }
            else
            {
                if ((G_pGame->m_stMCursor.sX >= sX + 20) && (G_pGame->m_stMCursor.sX <= sX + 82) && (G_pGame->m_stMCursor.sY >= sY + 299) && (G_pGame->m_stMCursor.sY<sY + 312))
                {
                    PutString2(sX + 25, sY + 45 + (14 * 18), "Clear Title", 7, 0, 0); // Super Black when title is choosen and mouse is there
                }
                else
                {
                    PutString2(sX + 25, sY + 45 + (14 * 18), "Clear Title", 255, 255, 255); // White when title is choosen and mouse not there
                }
            }

            // Refresh Button
            if ((m_dwCurTime - m_dwReqTitlesTime) > 2000)
            {
                if ((G_pGame->m_stMCursor.sX >= sX + 89) && (G_pGame->m_stMCursor.sX <= sX + 139) && (G_pGame->m_stMCursor.sY >= sY + 299) && (G_pGame->m_stMCursor.sY<sY + 312))
                {
                    PutString2(sX + 95, sY + 45 + (14 * 18), "Refresh", 255, 255, 255); // White when mouse is there without pressing 
                }
                else
                {
                    PutString2(sX + 95, sY + 45 + (14 * 18), "Refresh", 128, 128, 128); // Gray when mouse is not there without pressing 
                }
            }
            else
            {
                PutString2(sX + 95, sY + 45 + (14 * 18), "Refresh", 65, 65, 65); // Grey & Black when Pressed
            }

            /*		// Choose Button
            if(m_iTitleIndex == -1)
            {
            PutString2(sX+150,sY+45+(14*18),"Choose", 128,128,128); // No title choosen
            }
            else
            {
            if ((G_pGame->m_stMCursor.sX>=sX+144) && (G_pGame->m_stMCursor.sX<=sX+192) && (G_pGame->m_stMCursor.sY>=sY+299) && (G_pGame->m_stMCursor.sY<sY+312))
            {
            PutString2(sX+150,sY+45+(14*18),"Choose", 7,0,0); // Super Black when title is choosen and mouse is there
            }
            else {
            PutString2(sX+150,sY+45+(14*18),"Choose", 255,255,255); // White when title is choosen and mouse not there
            }
            }*/

            // Close Button
            if ((G_pGame->m_stMCursor.sX>sX + 194) && (G_pGame->m_stMCursor.sX<sX + 233) && (G_pGame->m_stMCursor.sY >= sY + 299) && (G_pGame->m_stMCursor.sY<sY + 312))
            {
                PutString2(sX + 200, sY + 45 + (14 * 18), "Close", 255, 255, 255); // White when mouse is there without pressing
            }
            else
            {
                PutString2(sX + 200, sY + 45 + (14 * 18), "Close", 128, 128, 128); // Grey when mouse is not there
            }
            break;

        default:
            break;
    }
}

void CGame::DlgBoxClick_Titles()
{
    int i;
    short sX, sY, szX, szY;
    char cTemp[21];
    sX = m_dialogBoxes[63].m_X;
    sY = m_dialogBoxes[63].m_Y;
    szX = m_dialogBoxes[63].sSizeX;
    szY = m_dialogBoxes[63].sSizeY;
    char onButton = m_dialogBoxes[63].OnButton();

    for (i = 0; i < 14; i++)
        //if (onButton == 1) {
        if ((G_pGame->m_stMCursor.sX >= sX + 20) && (G_pGame->m_stMCursor.sX <= sX + 220) && (G_pGame->m_stMCursor.sY >= sY + i * 18 + 65 - 30) && (G_pGame->m_stMCursor.sY <= sY + i * 18 + 79 - 30))
        {
            if (m_pTitles[m_dialogBoxes[63].sView + i] != 0)
            {
                PlaySound('E', 14, 5);
                m_iTitleIndex = (m_dialogBoxes[63].sView + i);
                bSendCommand(MSGID_SETTITLE, 0, 0, m_pTitles[m_iTitleIndex]->m_iServerTitleIndex, 0, 0, 0);
                wsprintfA(G_cTxt, "You have chosen \'%s Lvl.%d\' as your title", m_pTitles[m_iTitleIndex]->m_cSubject, m_pTitles[m_iTitleIndex]->m_iRank);
                AddEventList(G_cTxt, 10);
            }
            return;
        }

    // Refresh Monster Count
    if ((G_pGame->m_stMCursor.sX>sX + 95) && (G_pGame->m_stMCursor.sX<sX + 95 + 40) && (G_pGame->m_stMCursor.sY>sY + 45 + 14 * 18) && (G_pGame->m_stMCursor.sY<sY + 45 + 15 * 18))
        //if (onButton == 2)
    {
        if ((m_dwCurTime - m_dwReqTitlesTime) > 2000)
        {
            bSendCommand(MSGID_REQUEST_TITLES, 0, 0, 0, 0, 0, 0);
            m_dwReqTitlesTime = m_dwCurTime;
            m_cCommandCount--;
        }
    }

    /*	// Choose button
    if ((G_pGame->m_stMCursor.sX>sX+150) && (G_pGame->m_stMCursor.sX<sX+150+40) && (G_pGame->m_stMCursor.sY>sY+45+14*18) && (G_pGame->m_stMCursor.sY<sY+45+15*18))
    //if (onButton == 3)
    {
    if(m_iTitleIndex == -1)
    {
    AddEventList("Your title has not been choosen.", 10);
    }
    else
    {
    //DisableDialogBox(63); I'd rather not disable the dialog box. Leave that up to the user.
    }
    }*/

    // Close button
    if ((G_pGame->m_stMCursor.sX>sX + 200) && (G_pGame->m_stMCursor.sX<sX + 200 + 35) && (G_pGame->m_stMCursor.sY>sY + 45 + 14 * 18) && (G_pGame->m_stMCursor.sY<sY + 45 + 15 * 18))
        //if (onButton == 4)
    {
        DisableDialogBox(63);
    }

    // Clear Title
    if ((G_pGame->m_stMCursor.sX>sX + 25) && (G_pGame->m_stMCursor.sX<sX + 25 + 55) && (G_pGame->m_stMCursor.sY>sY + 45 + 14 * 18) && (G_pGame->m_stMCursor.sY<sY + 45 + 15 * 18))
        //if (onButton == 5)
    {
        PlaySound('E', 14, 5);
        m_iTitleIndex = -1;
        bSendCommand(MSGID_SETTITLE, 0, 0, m_iTitleIndex, 0, 0, 0);
        AddEventList("Your title has been cleared.", 10);
    }
}

