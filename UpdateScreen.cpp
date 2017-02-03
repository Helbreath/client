

#include "newsocket.h"
#include "Game.h"
#include "..\shared\buffer.h"
#include <algorithm>
#include <iostream>

#include "lan_eng.h"
#include <boost/asio/ssl.hpp>

extern void MakeSprite(wchar_t * FileName, int iStart, short sCount, bool bAlphaEffect);
extern void MakeTileSpr(wchar_t * FileName, short sStart, short sCount, bool bAlphaEffect);
extern void MakeEffectSpr(wchar_t * FileName, short sStart, short sCount, bool bAlphaEffect);

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


void CGame::UpdateScreen_OnMainMenu()
{
    uint32_t dwTime = G_dwGlobalTime;

    m_iItemDropCnt = 0;
    m_bItemDrop = false;

    if (m_cGameModeCount == 0)
    {
        if (m_pSprite[SPRID_INTERFACE_ND_LOADING] != 0)
        {
            delete m_pSprite[SPRID_INTERFACE_ND_LOADING];
            m_pSprite[SPRID_INTERFACE_ND_LOADING] = 0;
        }
        EndInputString();

        m_stMCursor.sX = GetWidth() / 2;
        m_stMCursor.sY = GetHeight() / 2; // 800x600 Resolution xRisenx
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    //DrawNewDialogBox(SPRID_INTERFACE_ND_MAINMENU, -1, -1, 0, true);

    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;
    //m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSpriteRGB(msX, msY, 1, 0, 0, 0, dwTime);

    if (m_bEnterPressed == true)
    {
        // Enter
        PlaySound('E', 14, 5);
        m_bEnterPressed = false;
        switch (m_cCurFocus)
        {
            case 1:
                ChangeGameMode(GAMEMODE_ONSELECTSERVER);
                return;
            case 2:
#ifdef MAKE_ACCOUNT
                ClearContents_OnSelectCharacter();
                //ChangeGameMode(GAMEMODE_ONAGREEMENT);
                ChangeGameMode(GAMEMODE_ONCREATENEWACCOUNT);
#else
                GoHomepage();
#endif
                return;
                /*case 3:
                delete pMI;
                ChangeGameMode(GAMEMODE_ONQUIT);
                return;*/
            case 3:
                GoHomepage(true);
                return;

            case 4:
                GoHomepage(false);
                return;

            case 5:
                ChangeGameMode(GAMEMODE_ONQUIT);
                return;
        }
    }

    DrawVersion();
}

void CGame::UpdateScreen_OnLoading(bool bActive)
{
    int i;
    //std::vector<int> framePositions;
    /*if( bActive )*/ UpdateScreen_OnLoading_Progress();

    switch (m_cLoading)
    {
        case 0:
        {	//m_hPakFile = CreateFileA("sprites\\interface.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 2);
                m_pSprite[SPRID_MOUSECURSOR] = CSprite::CreateSprite(L"interface", 0, false);
                m_pSprite[SPRID_INTERFACE_SPRFONTS] = CSprite::CreateSprite(L"interface", 1, false);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }

            //m_hPakFile = CreateFileA("sprites\\Newmaps.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 5);
                m_pSprite[SPRID_INTERFACE_NEWMAPS1] = CSprite::CreateSprite(L"Newmaps", 0, false);
                m_pSprite[SPRID_INTERFACE_NEWMAPS2] = CSprite::CreateSprite(L"Newmaps", 1, false);
                m_pSprite[SPRID_INTERFACE_NEWMAPS3] = CSprite::CreateSprite(L"Newmaps", 2, false);
                m_pSprite[SPRID_INTERFACE_NEWMAPS4] = CSprite::CreateSprite(L"Newmaps", 3, false);
                m_pSprite[SPRID_INTERFACE_NEWMAPS5] = CSprite::CreateSprite(L"Newmaps", 4, false);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }

            //m_hPakFile = CreateFileA("sprites\\LoginDialog.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                m_pSprite[SPRID_INTERFACE_ND_LOGIN] = CSprite::CreateSprite(L"LoginDialog", 0, false);
#ifdef MAKE_ACCOUNT
                m_pSprite[SPRID_INTERFACE_ND_NEWACCOUNT] = CSprite::CreateSprite(L"LoginDialog", 1, false);
                //m_pSprite[SPRID_INTERFACE_ND_AGREEMENT] = CSprite::CreateSprite(L"LoginDialog", 9, FALSE);
#endif
                //CloseHandle(m_hPakFile);
            }
            /*#ifdef MAKE_ACCOUNT			// CLEROTH - ACC - Snoopy: fixed to use without special pak
            //m_hPakFile = CreateFileA("sprites\\CreateNewAcc.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if( m_hPakFile != INVALID_HANDLE_VALUE ) {
            m_pSprite[SPRID_INTERFACE_ND_NEWACCOUNT] = CSprite::CreateSprite(L"CreateNewAcc", 0, FALSE);
            //CloseHandle(m_hPakFile);
            }else
            {	//m_hPakFile = CreateFileA("sprites\\New-Dialog.lpk", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            m_pSprite[SPRID_INTERFACE_ND_NEWACCOUNT] = CSprite::CreateSprite(L"New-Dialog", 3, FALSE);
            //CloseHandle(m_hPakFile);
            }
            #endif*/

            //MakeLegionSprite( "itempack", SPRID_ITEMPACK_PIVOTPOINT+1, 27, FALSE);
            ////m_hPakFile = CreateFileA("sprites\\itempack.lpk", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            //if( m_hPakFile != INVALID_HANDLE_VALUE ) {
            //	ReadFramePositions(m_hPakFile, framePositions, 20);
            //	m_pSprite[SPRID_ITEMPACK_PIVOTPOINT+20] = CSprite::CreateSprite(L"itempack", 17, FALSE); //
            //	m_pSprite[SPRID_ITEMPACK_PIVOTPOINT+21] = CSprite::CreateSprite(L"itempack", 18, FALSE); //
            //	m_pSprite[SPRID_ITEMPACK_PIVOTPOINT+22] = CSprite::CreateSprite(L"itempack", 19, FALSE); // Angels
            //	//CloseHandle(m_hPakFile);
            //	framePositions.clear();

            //m_hPakFile = CreateFileA("sprites\\New-Dialog.lpk", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                m_pSprite[SPRID_INTERFACE_ND_MAINMENU] = CSprite::CreateSprite(L"New-Dialog", 1, false);
                m_pSprite[SPRID_INTERFACE_ND_QUIT] = CSprite::CreateSprite(L"New-Dialog", 2, false);
                //CloseHandle(m_hPakFile);
            }

            //m_hPakFile = CreateFileA("sprites\\GameDialog.lpk", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                m_pSprite[SPRID_INTERFACE_ND_GAME1] = CSprite::CreateSprite(L"GameDialog", 0, false);
                m_pSprite[SPRID_INTERFACE_ND_GAME2] = CSprite::CreateSprite(L"GameDialog", 1, false);
                m_pSprite[SPRID_INTERFACE_ND_GAME3] = CSprite::CreateSprite(L"GameDialog", 2, false);
                m_pSprite[SPRID_INTERFACE_ND_GAME4] = CSprite::CreateSprite(L"GameDialog", 3, false);
                m_pSprite[SPRID_INTERFACE_ND_CRUSADE] = CSprite::CreateSprite(L"GameDialog", 4, false);
                //m_pSprite[SPRID_INTERFACE_GUIDEMAP] =      CSprite::CreateSprite(L"GameDialog", 5, FALSE);
                m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL] = CSprite::CreateSprite(L"GameDialog", 6, false);
                m_pSprite[SPRID_INTERFACE_ND_INVENTORY] = CSprite::CreateSprite(L"GameDialog", 7, false);
                m_pSprite[SPRID_INTERFACE_ND_SELECTCHAR] = CSprite::CreateSprite(L"GameDialog", 8, false);
                m_pSprite[SPRID_INTERFACE_ND_NEWCHAR] = CSprite::CreateSprite(L"GameDialog", 9, false);
                m_pSprite[SPRID_INTERFACE_ND_NEWEXCHANGE] = CSprite::CreateSprite(L"GameDialog", 10, false);
                //CloseHandle(m_hPakFile);
            }

            //m_hPakFile = CreateFileA("sprites\\DialogText.lpk", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                m_pSprite[SPRID_INTERFACE_ND_TEXT] = CSprite::CreateSprite(L"DialogText", 0, false);
                m_pSprite[SPRID_INTERFACE_ND_BUTTON] = CSprite::CreateSprite(L"DialogText", 1, false);
                //CloseHandle(m_hPakFile);
            }
            MakeSprite(L"Telescope", SPRID_INTERFACE_GUIDEMAP, 34, false);	  // 32->34 xRisenx
            MakeSprite(L"Telescope2", SPRID_INTERFACE_GUIDEMAP + 35, 4, false); // Snoopy: Ajout.351 (heldenian maps)
            MakeSprite(L"monster", SPRID_INTERFACE_MONSTER, 1, false);
            m_cLoading = 4;
        }
        break;
        case 4:
        {
            MakeTileSpr(L"maptiles1", 0, 32, true);
            MakeTileSpr(L"structures1", 50, 20, true);
            MakeTileSpr(L"Sinside1", 70, 27, false);
            MakeTileSpr(L"Trees1", 100, 46, true);
            MakeTileSpr(L"TreeShadows", 150, 46, true);
            MakeTileSpr(L"objects1", 200, 10, true); // snoopy: 8->10
            MakeTileSpr(L"objects2", 211, 5, true);
            MakeTileSpr(L"objects3", 216, 4, true);
            MakeTileSpr(L"objects4", 220, 2, true); //snoopy: 1->2
            m_cLoading = 8;
        }
        break;
        case 8:
        {
            MakeTileSpr(L"Tile223-225", 223, 3, true);
            MakeTileSpr(L"Tile226-229", 226, 4, true);
            MakeTileSpr(L"objects5", 230, 9, true);	// Snoopy
            MakeTileSpr(L"objects6", 238, 4, true);	// Snoopy
            MakeTileSpr(L"objects7", 242, 7, true);	// Snoopy
            MakeTileSpr(L"maptiles2", 300, 15, true);//- Index 300
            MakeTileSpr(L"maptiles4", 320, 10, true);
            MakeTileSpr(L"maptiles5", 330, 19, true);
            MakeTileSpr(L"maptiles6", 349, 4, true);
            MakeTileSpr(L"maptiles353-361", 353, 9, true);
            MakeTileSpr(L"Tile363-366", 363, 4, true);
            MakeTileSpr(L"Tile367-367", 367, 1, true); // Add by Snoopy (fountains)
            MakeTileSpr(L"Tile370-381", 370, 12, true);// Tile370~381
            MakeTileSpr(L"Tile382-387", 382, 6, true);
            MakeTileSpr(L"Tile388-402", 388, 15, true);
            m_cLoading = 12;
        }
        break;
        case 12:
        {
            MakeTileSpr(L"Tile403-405", 403, 3, true);
            MakeTileSpr(L"Tile406-421", 406, 16, true);
            MakeTileSpr(L"Tile422-429", 422, 8, true);
            MakeTileSpr(L"Tile430-443", 430, 14, true);
            MakeTileSpr(L"Tile444-444", 444, 1, true);
            MakeTileSpr(L"Tile445-461", 445, 17, true);
            MakeTileSpr(L"Tile462-473", 462, 12, true);	// Diuuude
            MakeTileSpr(L"Tile474-478", 474, 5, true);	// Diuuude
            MakeTileSpr(L"Tile479-488", 479, 10, true);	// Diuuude
            MakeTileSpr(L"Tile489-522", 489, 34, true);	// Diuuude Drunken City
            MakeTileSpr(L"Tile523-530", 523, 8, true);	// Diuuude Rampart
            MakeTileSpr(L"Tile531-540", 531, 10, true);	// Diuuude GodH + Pont
            MakeTileSpr(L"Tile541-545", 541, 5, true);	// Diuuude GodH

                                                        // SPRID_ITEMPACK_PIVOTPOINT+0
            MakeSprite(L"itempack", SPRID_ITEMPACK_PIVOTPOINT + 1, 27, false);
            //m_hPakFile = CreateFileA("sprites\\itempack.lpk", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 21);
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + 20] = CSprite::CreateSprite(L"itempack", 17, false); //
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + 21] = CSprite::CreateSprite(L"itempack", 18, false); //
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + 22] = CSprite::CreateSprite(L"itempack", 19, false); // Angels
                                                                                                           //m_pSprite[SPRID_ITEMPACK_PIVOTPOINT+23] = CSprite::CreateSprite(L"itempack", 20, FALSE); // Peices
                                                                                                           //CloseHandle(m_hPakFile);
                                                                                                           //framePositions.clear();
            }

            // SPRID_ITEMGROUND_PIVOTPOINT+1
            MakeSprite(L"itemground", SPRID_ITEMGROUND_PIVOTPOINT + 1, 19, false);
            //m_hPakFile = CreateFileA("sprites\\itemground.lpk", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 20);
                m_pSprite[SPRID_ITEMGROUND_PIVOTPOINT + 20] = CSprite::CreateSprite(L"itemground", 17, false);
                m_pSprite[SPRID_ITEMGROUND_PIVOTPOINT + 21] = CSprite::CreateSprite(L"itemground", 18, false);
                m_pSprite[SPRID_ITEMGROUND_PIVOTPOINT + 22] = CSprite::CreateSprite(L"itemground", 19, false);//Angels
                                                                                                              //CloseHandle(m_hPakFile);
                                                                                                              //framePositions.clear();
            }
            MakeSprite(L"item-dynamic", SPRID_ITEMDYNAMIC_PIVOTPOINT, 3, false);
            m_cLoading = 16;
        }
        break;
        case 16:
        {
            //m_hPakFile = CreateFileA("sprites\\itemequipM.lpk", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 15);
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 0] = CSprite::CreateSprite(L"itemequipM", 0, false);	// body
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 1] = CSprite::CreateSprite(L"itemequipM", 1, false);	// 1-swords
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 2] = CSprite::CreateSprite(L"itemequipM", 2, false);	// 2-bows
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 3] = CSprite::CreateSprite(L"itemequipM", 3, false);	// 3-shields
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 4] = CSprite::CreateSprite(L"itemequipM", 4, false);	// 4-tunics
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 5] = CSprite::CreateSprite(L"itemequipM", 5, false);	// 5-shoes
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 7] = CSprite::CreateSprite(L"itemequipM", 6, false);	// 6-berk
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 8] = CSprite::CreateSprite(L"itemequipM", 7, false);	// 7-hoses
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 9] = CSprite::CreateSprite(L"itemequipM", 8, false);	// 8-bodyarmor
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 15] = CSprite::CreateSprite(L"itemequipM", 11, false); // Axe hammer
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 17] = CSprite::CreateSprite(L"itemequipM", 12, false); // Wands
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 18] = CSprite::CreateSprite(L"itemequipM", 9, false);  // hair
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 19] = CSprite::CreateSprite(L"itemequipM", 10, false); // undies
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 20] = CSprite::CreateSprite(L"itemequipM", 13, false); // capes
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 21] = CSprite::CreateSprite(L"itemequipM", 14, false); // helm
                                                                                                              //CloseHandle(m_hPakFile);
                                                                                                              //framePositions.clear();
            }

            //m_hPakFile = CreateFileA("sprites\\itempack.lpk", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 16] = CSprite::CreateSprite(L"itempack", 15); // Necks
                                                                                                     //Snoopy: Angels pandents
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 22] = CSprite::CreateSprite(L"itempack", 19); // Angels
                                                                                                     //CloseHandle(m_hPakFile);
            }

            //m_hPakFile = CreateFileA("sprites\\itemequipW.lpk", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 15);
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 40] = CSprite::CreateSprite(L"itemequipW", 0, false); // body
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 41] = CSprite::CreateSprite(L"itemequipW", 1, false); // 1-swords
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 42] = CSprite::CreateSprite(L"itemequipW", 2, false); // 2-bows
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 43] = CSprite::CreateSprite(L"itemequipW", 3, false); // 3-shields
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 45] = CSprite::CreateSprite(L"itemequipW", 4, false); // 4-shoes
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 50] = CSprite::CreateSprite(L"itemequipW", 5, false); // 5-Soustif
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 51] = CSprite::CreateSprite(L"itemequipW", 6, false); // 6 berk
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 52] = CSprite::CreateSprite(L"itemequipW", 7, false); // 7 hose
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 53] = CSprite::CreateSprite(L"itemequipW", 8, false); // 8-hoses
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 55] = CSprite::CreateSprite(L"itemequipW", 11, false); // Axe hammer
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 57] = CSprite::CreateSprite(L"itemequipW", 12, false); // Wands
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 58] = CSprite::CreateSprite(L"itemequipW", 9, false); // hair
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 59] = CSprite::CreateSprite(L"itemequipW", 10, false);// undies
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 60] = CSprite::CreateSprite(L"itemequipW", 13, false);// capes
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 61] = CSprite::CreateSprite(L"itemequipW", 14, false);// helm
                                                                                                             //CloseHandle(m_hPakFile);
                                                                                                             //framePositions.clear();
            }

            //m_hPakFile = CreateFileA("sprites\\itempack.lpk", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 56] = CSprite::CreateSprite(L"itempack", 15);// necks
                                                                                                    //Snoopy: Angels pandents
                m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 62] = CSprite::CreateSprite(L"itempack", 19); // Angels
                                                                                                     //CloseHandle(m_hPakFile);
            }
            MakeSprite(L"Bm", 500 + 15 * 8 * 0, 96, true);// Black Man (Type: 1)
            MakeSprite(L"Wm", 500 + 15 * 8 * 1, 96, true);// White Man (Type: 2)
            MakeSprite(L"Ym", 500 + 15 * 8 * 2, 96, true);// Yellow Man (Type: 3)
            m_cLoading = 20;
        }
        break;
        case 20:
        {
            MakeSprite(L"TutelarAngel1", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 0, 48, false);//(STR)
            MakeSprite(L"TutelarAngel2", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 1, 48, false);//(DEX)
            MakeSprite(L"TutelarAngel3", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 2, 48, false);//(INT)
            MakeSprite(L"TutelarAngel4", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 3, 48, false);//(MAG)
            MakeSprite(L"Bw", 500 + 15 * 8 * 3, 96, true);// Black Woman (Type: 4)
            MakeSprite(L"Ww", 500 + 15 * 8 * 4, 96, true);// White Woman (Type: 5)
            MakeSprite(L"Yw", 500 + 15 * 8 * 5, 96, true);// Yellow Woman (Type: 6)
            m_cLoading = 24;
        }
        break;
        case 24:
        {
            MakeSprite(L"slm", SPRID_MOB + 7 * 8 * 0, 40, true);// Slime (Type: 10)
            MakeSprite(L"ske", SPRID_MOB + 7 * 8 * 1, 40, true);// Skeleton (Type: 11)
            MakeSprite(L"Gol", SPRID_MOB + 7 * 8 * 2, 40, true);// Stone-Golem (Type: 12)
            MakeSprite(L"Cyc", SPRID_MOB + 7 * 8 * 3, 40, true);// Cyclops (Type: 13)
            MakeSprite(L"Orc", SPRID_MOB + 7 * 8 * 4, 40, true);// Orc (Type: 14)
                                                                //MakeSprite( L"OrcLord",	  SPRID_MOB   + 7*8*93, 40, TRUE);// OrcLord (Type: 103)
            MakeSprite(L"Shopkpr", SPRID_MOB + 7 * 8 * 5, 8, true);		// ShopKeeper-Woman (Type: 15)
            MakeSprite(L"Ant", SPRID_MOB + 7 * 8 * 6, 40, true);//  Giant-Ant (Type: 16)
            MakeSprite(L"Scp", SPRID_MOB + 7 * 8 * 7, 40, true);//  Scorpion (Type: 17)
            MakeSprite(L"Zom", SPRID_MOB + 7 * 8 * 8, 40, true);//  Zombie (Type: 18)
            MakeSprite(L"Gandlf", SPRID_MOB + 7 * 8 * 9, 8, true);
            MakeSprite(L"Howard", SPRID_MOB + 7 * 8 * 10, 8, true);
            MakeSprite(L"Guard", SPRID_MOB + 7 * 8 * 11, 40, true);// Guard (Type: 21)
            MakeSprite(L"Amp", SPRID_MOB + 7 * 8 * 12, 40, true);// Amphis (Type: 22)
            MakeSprite(L"Cla", SPRID_MOB + 7 * 8 * 13, 40, true);// Clay-Golem (Type: 23)
            MakeSprite(L"tom", SPRID_MOB + 7 * 8 * 14, 8, true);// Tom (Type: 24)
            MakeSprite(L"William", SPRID_MOB + 7 * 8 * 15, 8, true);// William (Type: 25)
            m_cLoading = 28;
        }
        break;
        case 28:
        {
            MakeSprite(L"Kennedy", SPRID_MOB + 7 * 8 * 16, 8, true);// Kennedy (Type: 26)
            MakeSprite(L"Helb", SPRID_MOB + 7 * 8 * 17, 40, true);// Hellbound (Type: 27)
            MakeSprite(L"Troll", SPRID_MOB + 7 * 8 * 18, 40, true);// Troll (Type: 28)
            MakeSprite(L"EnragedTroll", SPRID_MOB + 7 * 8 * 112, 40, true);// Enraged Troll (Type: 122)
            MakeSprite(L"Orge", SPRID_MOB + 7 * 8 * 19, 40, true);// Orge (Type: 29)
            MakeSprite(L"Liche", SPRID_MOB + 7 * 8 * 20, 40, true);// Liche (Type: 30)
            MakeSprite(L"Demon", SPRID_MOB + 7 * 8 * 21, 40, true);// Demon (Type: 31)
            MakeSprite(L"BlackDemon", SPRID_MOB + 7 * 8 * 95, 40, true);// Black Demon (Type: 105)
            MakeSprite(L"Unicorn", SPRID_MOB + 7 * 8 * 22, 40, true);// Unicorn (Type: 32)
            MakeSprite(L"DarkUnicorn", SPRID_MOB + 7 * 8 * 94, 40, true);// DarkUnicorn (Type: 104)
            MakeSprite(L"WereWolf", SPRID_MOB + 7 * 8 * 23, 40, true);// WereWolf (Type: 33)
            MakeSprite(L"Dummy", SPRID_MOB + 7 * 8 * 24, 40, true);// Dummy (Type: 34)
                                                                   //m_hPakFile = CreateFileA("sprites\\Effect5.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL); // Energy-Ball (Type: 35)
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 1);
                for (i = 0; i < 40; i++)
                    m_pSprite[SPRID_MOB + i + 7 * 8 * 25] = CSprite::CreateSprite(L"Effect5", 0, true);

                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }
            m_cLoading = 32;
        }
        break;
        case 32:
        {
            MakeSprite(L"GT-Arrow", SPRID_MOB + 7 * 8 * 26, 40, true);// Arrow-GuardTower (Type: 36)
            MakeSprite(L"GT-Cannon", SPRID_MOB + 7 * 8 * 27, 40, true);// Cannon-GuardTower (Type: 37)
            MakeSprite(L"ManaCollector", SPRID_MOB + 7 * 8 * 28, 40, true);// Mana Collector (Type: 38)
            MakeSprite(L"Detector", SPRID_MOB + 7 * 8 * 29, 40, true);// Detector (Type: 39)
            MakeSprite(L"ESG", SPRID_MOB + 7 * 8 * 30, 40, true);// ESG (Type: 40)
            MakeSprite(L"GMG", SPRID_MOB + 7 * 8 * 31, 40, true);// GMG (Type: 41)
            MakeSprite(L"ManaStone", SPRID_MOB + 7 * 8 * 32, 40, true);// ManaStone (Type: 42)
            MakeSprite(L"LWB", SPRID_MOB + 7 * 8 * 33, 40, true);// Light War Beetle (Type: 43)
            MakeSprite(L"GHK", SPRID_MOB + 7 * 8 * 34, 40, true);// God's Hand Knight (Type: 44)
            MakeSprite(L"GHKABS", SPRID_MOB + 7 * 8 * 35, 40, true);// God's Hand Knight with Armored Battle Steed (Type: 45)
            MakeSprite(L"TK", SPRID_MOB + 7 * 8 * 36, 40, true);// Temple Knight (Type: 46)
            MakeSprite(L"BG", SPRID_MOB + 7 * 8 * 37, 40, true);// Battle Golem (Type: 47)
            m_cLoading = 36;
        }
        break;
        case 36:
        {
            MakeSprite(L"Stalker", SPRID_MOB + 7 * 8 * 38, 40, true);// Stalker (Type: 48)
            MakeSprite(L"Hellclaw", SPRID_MOB + 7 * 8 * 39, 40, true);// Hellclaw (Type: 49)
            MakeSprite(L"Tigerworm", SPRID_MOB + 7 * 8 * 40, 40, true);// Tigerworm (Type: 50)
            MakeSprite(L"Catapult", SPRID_MOB + 7 * 8 * 41, 40, true);// Catapult (Type: 51)
            MakeSprite(L"Gagoyle", SPRID_MOB + 7 * 8 * 42, 40, true);// Gargoyle (Type: 52)
            MakeSprite(L"EnragedGagoyle", SPRID_MOB + 7 * 8 * 115, 40, true);// Enraged Gagoyle (Type: 125)
            MakeSprite(L"Beholder", SPRID_MOB + 7 * 8 * 43, 40, true);// Beholder (Type: 53)
            MakeSprite(L"DarkElf", SPRID_MOB + 7 * 8 * 44, 40, true);// Dark-Elf (Type: 54)
            MakeSprite(L"Bunny", SPRID_MOB + 7 * 8 * 45, 40, true);// Bunny (Type: 55)
            MakeSprite(L"Cat", SPRID_MOB + 7 * 8 * 46, 40, true);// Cat (Type: 56)
            MakeSprite(L"GiantFrog", SPRID_MOB + 7 * 8 * 47, 40, true);// GiantFrog (Type: 57)
            MakeSprite(L"MTGiant", SPRID_MOB + 7 * 8 * 48, 40, true);// Mountain Giant (Type: 58)
            m_cLoading = 40;
        }
        break;
        case 40:
        {
            MakeSprite(L"Ettin", SPRID_MOB + 7 * 8 * 49, 40, true);// Ettin (Type: 59)
            MakeSprite(L"CanPlant", SPRID_MOB + 7 * 8 * 50, 40, true);// Cannibal Plant (Type: 60)
            MakeSprite(L"Rudolph", SPRID_MOB + 7 * 8 * 51, 40, true);// Rudolph (Type: 61)
            MakeSprite(L"DireBoar", SPRID_MOB + 7 * 8 * 52, 40, true);// Boar (Type: 62)
            MakeSprite(L"frost", SPRID_MOB + 7 * 8 * 53, 40, true);// Frost (Type: 63)
            MakeSprite(L"Crop", SPRID_MOB + 7 * 8 * 54, 40, true);// Crop(Type: 64)
            MakeSprite(L"IceGolem", SPRID_MOB + 7 * 8 * 55, 40, true);// IceGolem (Type: 65)
            MakeSprite(L"Wyvern", SPRID_MOB + 7 * 8 * 56, 24, true);// Wyvern (Type: 66)
            MakeSprite(L"McGaffin", SPRID_MOB + 7 * 8 * 57, 16, true);// McGaffin (Type: 67)
            MakeSprite(L"Perry", SPRID_MOB + 7 * 8 * 58, 16, true);// Perry (Type: 68)
            MakeSprite(L"Devlin", SPRID_MOB + 7 * 8 * 59, 16, true);// Devlin (Type: 69)
            MakeSprite(L"Barlog", SPRID_MOB + 7 * 8 * 60, 40, true);// Barlog (Type: 70)
            MakeSprite(L"Centaurus", SPRID_MOB + 7 * 8 * 61, 40, true);// Centaurus (Type: 71)
            MakeSprite(L"ClawTurtle", SPRID_MOB + 7 * 8 * 62, 40, true);// Claw-Turtle (Type: 72)
            MakeSprite(L"FireWyvern", SPRID_MOB + 7 * 8 * 63, 24, true);// Fire-Wyvern (Type: 73)
            MakeSprite(L"GiantCrayfish", SPRID_MOB + 7 * 8 * 64, 40, true);// Giant-Crayfish (Type: 74)
            MakeSprite(L"GiantLizard", SPRID_MOB + 7 * 8 * 65, 40, true);// Giant-Lizard (Type: 75)
            MakeSprite(L"DarkWyvern", SPRID_MOB + 7 * 8 * 98, 24, true);// Dark-Wyvern (Type: 108)
            MakeSprite(L"EarthWyvern", SPRID_MOB + 7 * 8 * 96, 24, true);// Earth-Wyvern (Type: 106)
            MakeSprite(L"GrassWyvern", SPRID_MOB + 7 * 8 * 99, 24, true);// Grass-Wyvern (Type: 109)
            MakeSprite(L"MetalWyvern", SPRID_MOB + 7 * 8 * 102, 24, true);// Metal-Wyvern (Type: 112)
            MakeSprite(L"SeaWyvern", SPRID_MOB + 7 * 8 * 101, 24, true);// Sea-Wyvern (Type: 111)
            MakeSprite(L"SkyWyvern", SPRID_MOB + 7 * 8 * 100, 24, true);// Sky-Wyvern (Type: 110)
            MakeSprite(L"VoidWyvern", SPRID_MOB + 7 * 8 * 103, 24, true);// Void-Wyvern (Type: 113)
            MakeSprite(L"WindWyvern", SPRID_MOB + 7 * 8 * 97, 24, true);// Wind-Wyvern (Type: 107)
            m_cLoading = 44;
        }
        break;
        case 44:
        {	// New NPCs - Diuuude - fixed by Snoopy
            MakeSprite(L"GiantPlant", SPRID_MOB + 7 * 8 * 66, 40, true);// Giant-Plant (Type: 76)
            MakeSprite(L"MasterMageOrc", SPRID_MOB + 7 * 8 * 67, 40, true);// MasterMage-Orc (Type: 77)
            MakeSprite(L"Minotaurs", SPRID_MOB + 7 * 8 * 68, 40, true);// Minotaurs (Type: 78)
            MakeSprite(L"Nizie", SPRID_MOB + 7 * 8 * 69, 40, true);// Nizie (Type: 79)
            MakeSprite(L"Tentocle", SPRID_MOB + 7 * 8 * 70, 40, true);// Tentocle (Type: 80)
            MakeSprite(L"yspro", SPRID_MOB + 7 * 8 * 71, 32, true);// Abaddon (Type: 81)
            MakeSprite(L"Sorceress", SPRID_MOB + 7 * 8 * 72, 40, true);// Sorceress (Type: 82)
            MakeSprite(L"TPKnight", SPRID_MOB + 7 * 8 * 73, 40, true);// TPKnight (Type: 83)
            MakeSprite(L"ElfMaster", SPRID_MOB + 7 * 8 * 74, 40, true);// ElfMaster (Type: 84)
            MakeSprite(L"DarkKnight", SPRID_MOB + 7 * 8 * 75, 40, true);// DarkKnight (Type: 85)
            MakeSprite(L"HBTank", SPRID_MOB + 7 * 8 * 76, 32, true);// HeavyBattleTank (Type: 86)
            MakeSprite(L"CBTurret", SPRID_MOB + 7 * 8 * 77, 32, true);// CBTurret (Type: 87)
            MakeSprite(L"Babarian", SPRID_MOB + 7 * 8 * 78, 40, true);// Babarian (Type: 88)
            MakeSprite(L"ACannon", SPRID_MOB + 7 * 8 * 79, 32, true);// ACannon (Type: 89)
            MakeSprite(L"EternalDragon", SPRID_MOB + 7 * 8 * 104, 32, true);// Eternal dragon (Type: 114) 
                                                                            //MakeSprite( L"ZombieKing",	  SPRID_MOB   + 7*8*109, 32, TRUE);// Zombie King (Type: 119)
                                                                            //MakeSprite( L"Puma",			  SPRID_MOB   + 7*8*108, 32, TRUE);// Puma (Type: 118)
                                                                            //MakeSprite( L"Alligator",	  SPRID_MOB   + 7*8*107, 32, TRUE);// Alligator (Type: 117)
            MakeSprite(L"EnragedCyclops", SPRID_MOB + 7 * 8 * 113, 40, true);// Enraged Cyclops (Type: 123)
            MakeSprite(L"EnragedStalker", SPRID_MOB + 7 * 8 * 114, 40, true);// Enraged Stalker (Type: 124)
            MakeSprite(L"EnragedHellclaw", SPRID_MOB + 7 * 8 * 116, 40, true);// Enraged Hellclaw (Type: 126)
            MakeSprite(L"EnragedTW", SPRID_MOB + 7 * 8 * 117, 40, true);// Enraged Tigerworm (Type: 127)
            m_cLoading = 48;
        }
        break;
        case 48:
        {
            MakeSprite(L"Gail", SPRID_MOB + 7 * 8 * 80, 8, true); // Gail (Type: 90)
            MakeSprite(L"Gate", SPRID_MOB + 7 * 8 * 81, 24, true);// Heldenian Gate (Type: 91)/**/
            MakeSprite(L"Scarecrow", SPRID_MOB + 7 * 8 * 82, 40, true);
            MakeSprite(L"Princess", SPRID_MOB + 7 * 8 * 92, 8, true);// Princess 102
                                                                     //m_hPakFile = CreateFileA("sprites\\Mpt.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 96);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 0] = CSprite::CreateSprite(L"Mpt", i + 12 * 0, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 1] = CSprite::CreateSprite(L"Mpt", i + 12 * 1, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 2] = CSprite::CreateSprite(L"Mpt", i + 12 * 2, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 3] = CSprite::CreateSprite(L"Mpt", i + 12 * 3, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 4] = CSprite::CreateSprite(L"Mpt", i + 12 * 4, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 5] = CSprite::CreateSprite(L"Mpt", i + 12 * 5, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 6] = CSprite::CreateSprite(L"Mpt", i + 12 * 6, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 7] = CSprite::CreateSprite(L"Mpt", i + 12 * 7, true);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }
            m_cLoading = 52;
        }
        break;

        case 52:
        {	//m_hPakFile = CreateFileA("sprites\\Mhr.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 96);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 0] = CSprite::CreateSprite(L"Mhr", i + 12 * 0, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 1] = CSprite::CreateSprite(L"Mhr", i + 12 * 1, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 2] = CSprite::CreateSprite(L"Mhr", i + 12 * 2, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 3] = CSprite::CreateSprite(L"Mhr", i + 12 * 3, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 4] = CSprite::CreateSprite(L"Mhr", i + 12 * 4, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 5] = CSprite::CreateSprite(L"Mhr", i + 12 * 5, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 6] = CSprite::CreateSprite(L"Mhr", i + 12 * 6, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 7] = CSprite::CreateSprite(L"Mhr", i + 12 * 7, true);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }
            MakeSprite(L"MLArmor", SPRID_BODYARMOR_M + 15 * 1, 12, true);
            MakeSprite(L"MCMail", SPRID_BODYARMOR_M + 15 * 2, 12, true);
            MakeSprite(L"MSMail", SPRID_BODYARMOR_M + 15 * 3, 12, true);
            MakeSprite(L"MPMail", SPRID_BODYARMOR_M + 15 * 4, 12, true);
            //MakeSprite( L"MPMail",	SPRID_BODYARMOR_M + 15*25, 12, TRUE);
            MakeSprite(L"Mtunic", SPRID_BODYARMOR_M + 15 * 5, 12, true);
            MakeSprite(L"MRobe1", SPRID_BODYARMOR_M + 15 * 6, 12, true);
            MakeSprite(L"MSanta", SPRID_BODYARMOR_M + 15 * 7, 12, true);
            MakeSprite(L"MHPMail1", SPRID_BODYARMOR_M + 15 * 8, 12, true); //hero
            MakeSprite(L"MHPMail2", SPRID_BODYARMOR_M + 15 * 9, 12, true); //hero
            MakeSprite(L"MHRobe1", SPRID_BODYARMOR_M + 15 * 10, 12, true); //hero
            MakeSprite(L"MHRobe2", SPRID_BODYARMOR_M + 15 * 11, 12, true); //hero
            MakeSprite(L"AncHeroArmorM", SPRID_BODYARMOR_M + 15 * 12, 12, true); //hero // Black Ancient Hero Armor M
            MakeSprite(L"AncHeroRobeM", SPRID_BODYARMOR_M + 15 * 13, 12, true); //hero // Black Ancient Hero Robe M
            MakeSprite(L"ALegRobeM", SPRID_BODYARMOR_M + 15 * 14, 12, true); // Aresden Ancient Hero Robe M
            MakeSprite(L"ELegRobeM", SPRID_BODYARMOR_M + 15 * 15, 12, true); // Elvine Ancient Hero Robe M
            MakeSprite(L"ALegArmorM", SPRID_BODYARMOR_M + 15 * 16, 12, true); // Aresden Ancient Hero Armor M
            MakeSprite(L"ELegArmorM", SPRID_BODYARMOR_M + 15 * 17, 12, true); // Elvine Ancient Hero Armor M
            MakeSprite(L"MShirt", SPRID_BERK_M + 15 * 1, 12, true);
            MakeSprite(L"MHauberk", SPRID_BERK_M + 15 * 2, 12, true);
            //MakeSprite( L"MHauberk",	  SPRID_BERK_M + 15*25, 12, TRUE);
            MakeSprite(L"MHHauberk1", SPRID_BERK_M + 15 * 3, 12, true);
            MakeSprite(L"MHHauberk2", SPRID_BERK_M + 15 * 4, 12, true);
            MakeSprite(L"AncHeroHauberkM", SPRID_BERK_M + 15 * 5, 12, true); // Black Ancient Hero Hauberk M
            MakeSprite(L"ALegHauberkM", SPRID_BERK_M + 15 * 6, 12, true); // Aresden Ancient Hero Hauberk M
            MakeSprite(L"ELegHauberkM", SPRID_BERK_M + 15 * 7, 12, true); // Elvine Ancient Hero Hauberk M
            m_cLoading = 56;
        }
        break;
        case 56:
        {
            MakeSprite(L"MTrouser", SPRID_LEGG_M + 15 * 1, 12, true);
            MakeSprite(L"MHTrouser", SPRID_LEGG_M + 15 * 2, 12, true);
            MakeSprite(L"MCHoses", SPRID_LEGG_M + 15 * 3, 12, true);
            MakeSprite(L"MLeggings", SPRID_LEGG_M + 15 * 4, 12, true);
            //MakeSprite( L"MLeggings",SPRID_LEGG_M + 15*25, 12, TRUE);
            MakeSprite(L"MHLeggings1", SPRID_LEGG_M + 15 * 5, 12, true); // hero
            MakeSprite(L"MHLeggings2", SPRID_LEGG_M + 15 * 6, 12, true); // hero
            MakeSprite(L"AncHeroLegM", SPRID_LEGG_M + 15 * 7, 12, true); // hero // Black Ancient Hero Leggings M
            MakeSprite(L"ALegLeggingsM", SPRID_LEGG_M + 15 * 8, 12, true); // Aresden Ancient Leggings M 
            MakeSprite(L"ELegLeggingsM", SPRID_LEGG_M + 15 * 9, 12, true); // Elvine Ancient Leggings M

            MakeSprite(L"MShoes", SPRID_BOOT_M + 15 * 1, 12, true);
            MakeSprite(L"MLBoots", SPRID_BOOT_M + 15 * 2, 12, true);
            //m_hPakFile = CreateFileA("sprites\\Msw.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 672);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 1] = CSprite::CreateSprite(L"Msw", i + 56 * 0, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 2] = CSprite::CreateSprite(L"Msw", i + 56 * 1, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 3] = CSprite::CreateSprite(L"Msw", i + 56 * 2, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 4] = CSprite::CreateSprite(L"Msw", i + 56 * 3, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 6] = CSprite::CreateSprite(L"Msw", i + 56 * 5, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 7] = CSprite::CreateSprite(L"Msw", i + 56 * 6, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 8] = CSprite::CreateSprite(L"Msw", i + 56 * 7, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 9] = CSprite::CreateSprite(L"Msw", i + 56 * 8, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 10] = CSprite::CreateSprite(L"Msw", i + 56 * 9, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 11] = CSprite::CreateSprite(L"Msw", i + 56 * 10, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 12] = CSprite::CreateSprite(L"Msw", i + 56 * 11, true);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }
            m_cLoading = 60;
        }
        break;
        case 60:
        {
            MakeSprite(L"Mswx", SPRID_WEAPON_M + 64 * 5, 56, true);
            MakeSprite(L"Msw2", SPRID_WEAPON_M + 64 * 13, 56, true);
            MakeSprite(L"Msw3", SPRID_WEAPON_M + 64 * 14, 56, true);
            MakeSprite(L"MStormBringer", SPRID_WEAPON_M + 64 * 15, 56, true);
            MakeSprite(L"MDarkExec", SPRID_WEAPON_M + 64 * 16, 56, true);
            MakeSprite(L"MKlonessBlade", SPRID_WEAPON_M + 64 * 17, 56, true);
            MakeSprite(L"MKlonessAstock", SPRID_WEAPON_M + 64 * 18, 56, true);
            MakeSprite(L"MDebastator", SPRID_WEAPON_M + 64 * 19, 56, true);
            MakeSprite(L"MAxe1", SPRID_WEAPON_M + 64 * 20, 56, true);// Axe
            MakeSprite(L"MAxe2", SPRID_WEAPON_M + 64 * 21, 56, true);
            MakeSprite(L"MAxe3", SPRID_WEAPON_M + 64 * 22, 56, true);
            MakeSprite(L"MAxe4", SPRID_WEAPON_M + 64 * 23, 56, true);
            MakeSprite(L"MAxe5", SPRID_WEAPON_M + 64 * 24, 56, true);
            MakeSprite(L"MPickAxe1", SPRID_WEAPON_M + 64 * 25, 56, true);
            MakeSprite(L"MAxe6", SPRID_WEAPON_M + 64 * 26, 56, true);
            MakeSprite(L"Mhoe", SPRID_WEAPON_M + 64 * 27, 56, true);
            MakeSprite(L"MKlonessAxe", SPRID_WEAPON_M + 64 * 28, 56, true);
            MakeSprite(L"MLightBlade", SPRID_WEAPON_M + 64 * 29, 56, true);
            m_cLoading = 64;
        }
        break;
        case 64:
        {
            MakeSprite(L"MHammer", SPRID_WEAPON_M + 64 * 30, 56, true);
            MakeSprite(L"MBHammer", SPRID_WEAPON_M + 64 * 31, 56, true);
            MakeSprite(L"MBabHammer", SPRID_WEAPON_M + 64 * 32, 56, true);
            MakeSprite(L"MBShadowSword", SPRID_WEAPON_M + 64 * 33, 56, true);
            MakeSprite(L"MBerserkWand", SPRID_WEAPON_M + 64 * 34, 56, true);
            MakeSprite(L"Mstaff1", SPRID_WEAPON_M + 64 * 35, 56, true);// Staff
            MakeSprite(L"Mstaff2", SPRID_WEAPON_M + 64 * 36, 56, true);
            MakeSprite(L"MStaff3", SPRID_WEAPON_M + 64 * 37, 56, true);
            MakeSprite(L"MReMagicWand", SPRID_WEAPON_M + 64 * 38, 56, true);
            MakeSprite(L"MKlonessWand", SPRID_WEAPON_M + 64 * 39, 56, true);
            MakeSprite(L"Staff4M", SPRID_WEAPON_M + 64 * 40, 56, true);
            // Bows 40 41 below
            MakeSprite(L"MDirectBow", SPRID_WEAPON_M + 64 * 43, 56, true);
            MakeSprite(L"MFireBow", SPRID_WEAPON_M + 64 * 44, 56, true);
            m_cLoading = 68;
        }
        break;
        case 68:
        {
            MakeSprite(L"Mbo", SPRID_WEAPON_M + 64 * 41, 56, true);
            //m_hPakFile = CreateFileA("sprites\\Mbo.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 112);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 42] = CSprite::CreateSprite(L"Mbo", i + 56 * 1, true);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }
            //m_hPakFile = CreateFileA("sprites\\Msh.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 63);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 1] = CSprite::CreateSprite(L"Msh", i + 7 * 0, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 2] = CSprite::CreateSprite(L"Msh", i + 7 * 1, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 3] = CSprite::CreateSprite(L"Msh", i + 7 * 2, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 4] = CSprite::CreateSprite(L"Msh", i + 7 * 3, true);
                for (i = 0; i < 7; i++)	m_pSprite[SPRID_SHIELD_M + i + 8 * 5] = CSprite::CreateSprite(L"Msh", i + 7 * 4, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 6] = CSprite::CreateSprite(L"Msh", i + 7 * 5, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 7] = CSprite::CreateSprite(L"Msh", i + 7 * 6, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 8] = CSprite::CreateSprite(L"Msh", i + 7 * 7, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 9] = CSprite::CreateSprite(L"Msh", i + 7 * 8, true);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }
            m_cLoading = 72;
        }
        break;
        case 72:
        {
            MakeSprite(L"Mmantle01", SPRID_MANTLE_M + 15 * 1, 12, true);
            MakeSprite(L"Mmantle02", SPRID_MANTLE_M + 15 * 2, 12, true);
            MakeSprite(L"Mmantle03", SPRID_MANTLE_M + 15 * 3, 12, true);
            MakeSprite(L"Mmantle04", SPRID_MANTLE_M + 15 * 4, 12, true);
            MakeSprite(L"Mmantle05", SPRID_MANTLE_M + 15 * 5, 12, true);
            MakeSprite(L"Mmantle06", SPRID_MANTLE_M + 15 * 6, 12, true);
            MakeSprite(L"Mmantle07", SPRID_MANTLE_M + 15 * 7, 12, true);
            MakeSprite(L"Mmantle08", SPRID_MANTLE_M + 15 * 8, 12, true);
            MakeSprite(L"AAncHeroCapeM", SPRID_MANTLE_M + 15 * 9, 12, true); // Aresden Black Ancient Hero Cape M
            MakeSprite(L"EAncHeroCapeM", SPRID_MANTLE_M + 15 * 10, 12, true); // Elvine Black Ancient Hero Cape M
            MakeSprite(L"MHelm1", SPRID_HEAD_M + 15 * 1, 12, true);
            MakeSprite(L"MHelm2", SPRID_HEAD_M + 15 * 2, 12, true);
            MakeSprite(L"MHelm3", SPRID_HEAD_M + 15 * 3, 12, true);
            MakeSprite(L"MHelm4", SPRID_HEAD_M + 15 * 4, 12, true);
            MakeSprite(L"NMHelm1", SPRID_HEAD_M + 15 * 5, 12, true);
            //MakeSprite( L"NMHelm1", SPRID_HEAD_M + 15*25, 12, TRUE);
            MakeSprite(L"NMHelm2", SPRID_HEAD_M + 15 * 6, 12, true);
            MakeSprite(L"NMHelm3", SPRID_HEAD_M + 15 * 7, 12, true);
            MakeSprite(L"NMHelm4", SPRID_HEAD_M + 15 * 8, 12, true);
            MakeSprite(L"MHHelm1", SPRID_HEAD_M + 15 * 9, 12, true);
            MakeSprite(L"MHHelm2", SPRID_HEAD_M + 15 * 10, 12, true);
            MakeSprite(L"MHCap1", SPRID_HEAD_M + 15 * 11, 12, true);
            MakeSprite(L"MHCap2", SPRID_HEAD_M + 15 * 12, 12, true);
            MakeSprite(L"AncHeroHelmM", SPRID_HEAD_M + 15 * 13, 12, true); // Black Ancient Hero Helm M
            MakeSprite(L"AncHeroCapM", SPRID_HEAD_M + 15 * 14, 12, true); // Black Ancient Hero Cap M
            MakeSprite(L"ALegHelmM", SPRID_HEAD_M + 15 * 15, 12, true); // Aresden Ancient Helm M
            MakeSprite(L"ELegHelmM", SPRID_HEAD_M + 15 * 16, 12, true); // Elvine Ancient Helm M
            MakeSprite(L"ALegCapM", SPRID_HEAD_M + 15 * 17, 12, true); // Aresden Ancient Cap M
            MakeSprite(L"ELegCapM", SPRID_HEAD_M + 15 * 18, 12, true); // Elvine Ancient Cap M
            m_cLoading = 76;
        }
        break;
        case 76:
        {	//m_hPakFile = CreateFileA("sprites\\Wpt.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 96);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 0] = CSprite::CreateSprite(L"Wpt", i, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 1] = CSprite::CreateSprite(L"Wpt", i + 12, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 2] = CSprite::CreateSprite(L"Wpt", i + 12 * 2, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 3] = CSprite::CreateSprite(L"Wpt", i + 12 * 3, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 4] = CSprite::CreateSprite(L"Wpt", i + 12 * 4, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 5] = CSprite::CreateSprite(L"Wpt", i + 12 * 5, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 6] = CSprite::CreateSprite(L"Wpt", i + 12 * 6, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 7] = CSprite::CreateSprite(L"Wpt", i + 12 * 7, true);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }

            //m_hPakFile = CreateFileA("sprites\\Whr.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 96);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 0] = CSprite::CreateSprite(L"Whr", i + 0, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 1] = CSprite::CreateSprite(L"Whr", i + 12, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 2] = CSprite::CreateSprite(L"Whr", i + 12 * 2, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 3] = CSprite::CreateSprite(L"Whr", i + 12 * 3, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 4] = CSprite::CreateSprite(L"Whr", i + 12 * 4, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 5] = CSprite::CreateSprite(L"Whr", i + 12 * 5, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 6] = CSprite::CreateSprite(L"Whr", i + 12 * 6, true);
                for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 7] = CSprite::CreateSprite(L"Whr", i + 12 * 7, true);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }
            m_cLoading = 80;
        }
        break;
        case 80:
        {
            MakeSprite(L"WBodice1", SPRID_BODYARMOR_W + 15 * 1, 12, true);
            MakeSprite(L"WBodice2", SPRID_BODYARMOR_W + 15 * 2, 12, true);
            MakeSprite(L"WLArmor", SPRID_BODYARMOR_W + 15 * 3, 12, true);
            MakeSprite(L"WCMail", SPRID_BODYARMOR_W + 15 * 4, 12, true);
            MakeSprite(L"WSMail", SPRID_BODYARMOR_W + 15 * 5, 12, true);
            MakeSprite(L"WPMail", SPRID_BODYARMOR_W + 15 * 6, 12, true);
            MakeSprite(L"WRobe1", SPRID_BODYARMOR_W + 15 * 7, 12, true);
            MakeSprite(L"WSanta", SPRID_BODYARMOR_W + 15 * 8, 12, true);
            MakeSprite(L"WHPMail1", SPRID_BODYARMOR_W + 15 * 9, 12, true); //hero
            MakeSprite(L"WHPMail2", SPRID_BODYARMOR_W + 15 * 10, 12, true); //hero
            MakeSprite(L"WHRobe1", SPRID_BODYARMOR_W + 15 * 11, 12, true); // hero
            MakeSprite(L"WHRobe2", SPRID_BODYARMOR_W + 15 * 12, 12, true); // hero
            MakeSprite(L"AncHeroArmorW", SPRID_BODYARMOR_W + 15 * 13, 12, true); //hero // Black Ancient Hero Armor W
            MakeSprite(L"AncHeroRobeW", SPRID_BODYARMOR_W + 15 * 14, 12, true); // hero // Black Ancient Hero Robe W
            MakeSprite(L"ALegRobeW", SPRID_BODYARMOR_W + 15 * 15, 12, true); // Aresden Ancient Robe W
            MakeSprite(L"ELegRobeW", SPRID_BODYARMOR_W + 15 * 16, 12, true); // Elvine Ancient Robe W
            MakeSprite(L"ALegArmorW", SPRID_BODYARMOR_W + 15 * 17, 12, true); // Aresden Ancient Armor W
            MakeSprite(L"ELegArmorW", SPRID_BODYARMOR_W + 15 * 18, 12, true); // Elvine Ancient Armor W

            MakeSprite(L"WChemiss", SPRID_BERK_W + 15 * 1, 12, true);
            MakeSprite(L"WShirt", SPRID_BERK_W + 15 * 2, 12, true);
            MakeSprite(L"WHauberk", SPRID_BERK_W + 15 * 3, 12, true);
            MakeSprite(L"WHHauberk1", SPRID_BERK_W + 15 * 4, 12, true);
            MakeSprite(L"WHHauberk2", SPRID_BERK_W + 15 * 5, 12, true);
            MakeSprite(L"AncHeroHauberkW", SPRID_BERK_W + 15 * 6, 12, true); // Black Ancient Hero Hauberk W
            MakeSprite(L"ALegHauberkW", SPRID_BERK_W + 15 * 7, 12, true); // Aresden Ancient Hauberk W
            MakeSprite(L"ELegHauberkW", SPRID_BERK_W + 15 * 8, 12, true); // Elvine Ancient Hauberk W
            MakeSprite(L"WSkirt", SPRID_LEGG_W + 15 * 1, 12, true);
            MakeSprite(L"WTrouser", SPRID_LEGG_W + 15 * 2, 12, true);
            MakeSprite(L"WHTrouser", SPRID_LEGG_W + 15 * 3, 12, true);
            MakeSprite(L"WCHoses", SPRID_LEGG_W + 15 * 4, 12, true);
            MakeSprite(L"WLeggings", SPRID_LEGG_W + 15 * 5, 12, true);
            MakeSprite(L"WHLeggings1", SPRID_LEGG_W + 15 * 6, 12, true);
            MakeSprite(L"WHLeggings2", SPRID_LEGG_W + 15 * 7, 12, true);
            MakeSprite(L"AncHeroLegW", SPRID_LEGG_W + 15 * 8, 12, true); // Black Ancient Hero Leggings W
            MakeSprite(L"ALegLeggingsW", SPRID_LEGG_W + 15 * 9, 12, true); // Aresden Ancient Leggings W
            MakeSprite(L"ELegLeggingsW", SPRID_LEGG_W + 15 * 10, 12, true); // Elvine Ancient Leggings W
            MakeSprite(L"WShoes", SPRID_BOOT_W + 15 * 1, 12, true);
            MakeSprite(L"WLBoots", SPRID_BOOT_W + 15 * 2, 12, true);
            m_cLoading = 84;
        }
        break;
        case 84:
        {	//m_hPakFile = CreateFileA("sprites\\Wsw.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 672);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 1] = CSprite::CreateSprite(L"Wsw", i + 56 * 0, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 2] = CSprite::CreateSprite(L"Wsw", i + 56 * 1, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 3] = CSprite::CreateSprite(L"Wsw", i + 56 * 2, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 4] = CSprite::CreateSprite(L"Wsw", i + 56 * 3, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 6] = CSprite::CreateSprite(L"Wsw", i + 56 * 5, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 7] = CSprite::CreateSprite(L"Wsw", i + 56 * 6, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 8] = CSprite::CreateSprite(L"Wsw", i + 56 * 7, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 9] = CSprite::CreateSprite(L"Wsw", i + 56 * 8, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 10] = CSprite::CreateSprite(L"Wsw", i + 56 * 9, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 11] = CSprite::CreateSprite(L"Wsw", i + 56 * 10, true);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 12] = CSprite::CreateSprite(L"Wsw", i + 56 * 11, true);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }
            MakeSprite(L"Wswx", SPRID_WEAPON_W + 64 * 5, 56, true);
            MakeSprite(L"Wsw2", SPRID_WEAPON_W + 64 * 13, 56, true);
            MakeSprite(L"Wsw3", SPRID_WEAPON_W + 64 * 14, 56, true); // TheVampire
            MakeSprite(L"WStormBringer", SPRID_WEAPON_W + 64 * 15, 56, true);
            MakeSprite(L"WDarkExec", SPRID_WEAPON_W + 64 * 16, 56, true);
            MakeSprite(L"WKlonessBlade", SPRID_WEAPON_W + 64 * 17, 56, true);
            MakeSprite(L"WKlonessAstock", SPRID_WEAPON_W + 64 * 18, 56, true);
            MakeSprite(L"WDebastator", SPRID_WEAPON_W + 64 * 19, 56, true);
            m_cLoading = 88;
        }
        break;
        case 88:
        {
            MakeSprite(L"WAxe1", SPRID_WEAPON_W + 64 * 20, 56, true);// Axe
            MakeSprite(L"WAxe2", SPRID_WEAPON_W + 64 * 21, 56, true);
            MakeSprite(L"WAxe3", SPRID_WEAPON_W + 64 * 22, 56, true);
            MakeSprite(L"WAxe4", SPRID_WEAPON_W + 64 * 23, 56, true);
            MakeSprite(L"WAxe5", SPRID_WEAPON_W + 64 * 24, 56, true);
            MakeSprite(L"WpickAxe1", SPRID_WEAPON_W + 64 * 25, 56, true);
            MakeSprite(L"WAxe6", SPRID_WEAPON_W + 64 * 26, 56, true);
            MakeSprite(L"Whoe", SPRID_WEAPON_W + 64 * 27, 56, true);
            MakeSprite(L"WKlonessAxe", SPRID_WEAPON_W + 64 * 28, 56, true);
            MakeSprite(L"WLightBlade", SPRID_WEAPON_W + 64 * 29, 56, true);
            MakeSprite(L"WHammer", SPRID_WEAPON_W + 64 * 30, 56, true);
            MakeSprite(L"WBHammer", SPRID_WEAPON_W + 64 * 31, 56, true);
            MakeSprite(L"WBabHammer", SPRID_WEAPON_W + 64 * 32, 56, true);
            MakeSprite(L"WBShadowSword", SPRID_WEAPON_W + 64 * 33, 56, true);
            MakeSprite(L"WBerserkWand", SPRID_WEAPON_W + 64 * 34, 56, true);
            MakeSprite(L"Wstaff1", SPRID_WEAPON_W + 64 * 35, 56, true);
            MakeSprite(L"Wstaff2", SPRID_WEAPON_W + 64 * 36, 56, true);
            MakeSprite(L"WStaff3", SPRID_WEAPON_W + 64 * 37, 56, true);
            MakeSprite(L"WKlonessWand", SPRID_WEAPON_W + 64 * 39, 56, true);
            MakeSprite(L"WReMagicWand", SPRID_WEAPON_W + 64 * 38, 56, true);
            MakeSprite(L"Staff4W", SPRID_WEAPON_W + 64 * 40, 56, true);
            // bows 40 41 below
            MakeSprite(L"WDirectBow", SPRID_WEAPON_W + 64 * 43, 56, true);
            MakeSprite(L"WFireBow", SPRID_WEAPON_W + 64 * 44, 56, true);
            m_cLoading = 92;
        }
        break;
        case 92:
        {
            MakeSprite(L"Wmantle01", SPRID_MANTLE_W + 15 * 1, 12, true);
            MakeSprite(L"Wmantle02", SPRID_MANTLE_W + 15 * 2, 12, true);
            MakeSprite(L"Wmantle03", SPRID_MANTLE_W + 15 * 3, 12, true);
            MakeSprite(L"Wmantle04", SPRID_MANTLE_W + 15 * 4, 12, true);
            MakeSprite(L"Wmantle05", SPRID_MANTLE_W + 15 * 5, 12, true);
            MakeSprite(L"Wmantle06", SPRID_MANTLE_W + 15 * 6, 12, true);
            MakeSprite(L"Wmantle07", SPRID_MANTLE_W + 15 * 7, 12, true);
            MakeSprite(L"Wmantle08", SPRID_MANTLE_W + 15 * 8, 12, true);
            MakeSprite(L"AAncHeroCapeW", SPRID_MANTLE_W + 15 * 9, 12, true); // Aresden Black Ancient Hero Cape W
            MakeSprite(L"EAncHeroCapeW", SPRID_MANTLE_W + 15 * 10, 12, true); // Elvine Black Ancient Hero Cape W
            MakeSprite(L"WHelm1", SPRID_HEAD_W + 15 * 1, 12, true);
            MakeSprite(L"WHelm4", SPRID_HEAD_W + 15 * 4, 12, true);
            MakeSprite(L"NWHelm1", SPRID_HEAD_W + 15 * 5, 12, true);
            MakeSprite(L"NWHelm2", SPRID_HEAD_W + 15 * 6, 12, true);
            MakeSprite(L"NWHelm3", SPRID_HEAD_W + 15 * 7, 12, true);
            MakeSprite(L"NWHelm4", SPRID_HEAD_W + 15 * 8, 12, true);
            MakeSprite(L"WHHelm1", SPRID_HEAD_W + 15 * 9, 12, true);
            MakeSprite(L"WHHelm2", SPRID_HEAD_W + 15 * 10, 12, true);
            MakeSprite(L"WHCap1", SPRID_HEAD_W + 15 * 11, 12, true);
            MakeSprite(L"WHCap2", SPRID_HEAD_W + 15 * 12, 12, true);
            MakeSprite(L"AncHeroHelmW", SPRID_HEAD_W + 15 * 13, 12, true); // Black Ancient Hero Helm W
            MakeSprite(L"AncHeroCapW", SPRID_HEAD_W + 15 * 14, 12, true); // Black Ancient Hero Cap W
            MakeSprite(L"ALegHelmW", SPRID_HEAD_W + 15 * 15, 12, true); // Aresden Ancient Helm M
            MakeSprite(L"ELegHelmW", SPRID_HEAD_W + 15 * 16, 12, true); // Elvine Ancient Helm M
            MakeSprite(L"ALegCapW", SPRID_HEAD_W + 15 * 17, 12, true); // Aresden Ancient Cap M
            MakeSprite(L"ELegCapW", SPRID_HEAD_W + 15 * 18, 12, true); // Elvine Ancient Cap M
            m_cLoading = 96;
        }
        break;
        case 96:
        {
            MakeSprite(L"Wbo", SPRID_WEAPON_W + 64 * 41, 56, true);// Bow
                                                                   //m_hPakFile = CreateFileA("sprites\\Wbo.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 112);
                for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 42] = CSprite::CreateSprite(L"Wbo", i + 56 * 1, true);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }
            //m_hPakFile = CreateFileA("sprites\\Wsh.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 63);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 1] = CSprite::CreateSprite(L"Wsh", i + 7 * 0, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 2] = CSprite::CreateSprite(L"Wsh", i + 7 * 1, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 3] = CSprite::CreateSprite(L"Wsh", i + 7 * 2, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 4] = CSprite::CreateSprite(L"Wsh", i + 7 * 3, true);
                for (i = 0; i < 7; i++)	m_pSprite[SPRID_SHIELD_W + i + 8 * 5] = CSprite::CreateSprite(L"Wsh", i + 7 * 4, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 6] = CSprite::CreateSprite(L"Wsh", i + 7 * 5, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 7] = CSprite::CreateSprite(L"Wsh", i + 7 * 6, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 8] = CSprite::CreateSprite(L"Wsh", i + 7 * 7, true);
                for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 9] = CSprite::CreateSprite(L"Wsh", i + 7 * 8, true);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }
            m_cLoading = 100;
        }
        break;
        case 100:
        {
            MakeEffectSpr(L"effect", 0, 10, false);
            MakeEffectSpr(L"effect2", 10, 3, false);
            MakeEffectSpr(L"effect3", 13, 6, false);
            MakeEffectSpr(L"effect4", 19, 5, false);
            //m_hPakFile = CreateFileA("sprites\\effect5.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
            if (m_hPakFile != INVALID_HANDLE_VALUE)
            {
                //ReadFramePositions(m_hPakFile, framePositions, 8);
                for (i = 0; i <= 6; i++)
                    m_pEffectSpr[i + 24] = CSprite::CreateSprite(L"effect5", i + 1, false);
                //CloseHandle(m_hPakFile);
                //framePositions.clear();
            }
            MakeEffectSpr(L"CruEffect1", 31, 9, false);
            MakeEffectSpr(L"effect6", 40, 5, false);
            MakeEffectSpr(L"effect7", 45, 12, false);
            MakeEffectSpr(L"effect8", 57, 9, false);
            MakeEffectSpr(L"effect9", 66, 21, false);

            MakeEffectSpr(L"effect10", 87, 2, false); // Effets Hero items
            MakeEffectSpr(L"effect11", 89, 14, false); // Cancel, stormBlade, resu, GateHeldenian....etc
                                                       //NB: Charge 15 du client 3.51, mais il n'y a que 14 ds le PAK
            MakeEffectSpr(L"effect11s", 104, 1, false); // effet sort mais je ne sais pas lequel
                                                        // Manque des effets ici .....
                                                        // MakeEffectSpr( L"effect13", 108, 2, FALSE); // not loaded by client 351 (Heldenian gates death)
                                                        //MakeEffectSpr( L"yseffect2", 141, 8, FALSE); // Wrong in 351 client...
            MakeEffectSpr(L"yseffect2", 140, 8, false); // Abaddon's death
            MakeEffectSpr(L"effect12", 148, 4, false); // Slates auras
            MakeEffectSpr(L"yseffect3", 152, 16, false); // Fumerolles ou ame qui s'envole
            MakeEffectSpr(L"yseffect4", 133, 7, false); // Abaddon's map thunder.
            MakeEffectSpr(L"effects", 168, 1, false); // minimap ping
                                                      //MakeLegionEffectSpr( "effects2", 169, 20); // New Magic Animations xRisenx

            SetupDialogBoxes();

            if (m_bSoundFlag) // Attention il y a un autre systeme de chargement ds la v351
            {
                for (i = 1; i <= 24; i++)
                {
                    wsprintfA(G_cTxt, "sounds\\C%d.wav", i);
                    //DIRECTX					m_pCSound[i] = new class CSoundBuffer(m_DSound.m_lpDS, m_DSound.m_DSCaps, G_cTxt);
                }

                for (i = 1; i <= 156; i++)
                {
                    wsprintfA(G_cTxt, "sounds\\M%d.wav", i);
                    //DIRECTX					m_pMSound[i] = new class CSoundBuffer(m_DSound.m_lpDS, m_DSound.m_DSCaps, G_cTxt);
                }
                for (i = 1; i <= 54; i++)
                {
                    wsprintfA(G_cTxt, "sounds\\E%d.wav", i);
                    //DIRECTX					m_pESound[i] = new class CSoundBuffer(m_DSound.m_lpDS, m_DSound.m_DSCaps, G_cTxt);
                }
            }
            isItemLoaded = false;
            //ChangeGameMode(GAMEMODE_ONMAINMENU);
            if (autologin)
            {
                ChangeGameMode(GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_LOGIN;
                ZeroMemory(m_cMsg, sizeof(m_cMsg));
                strcpy(m_cMsg, "11");
                if (_socket != nullptr)
                {
                    _socket->stop();
                }
                if (new_connection_ != nullptr)
                {
                    new_connection_->stop();
                }

                CreateSocket();
                boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
                new_connection_->socket().async_connect(endpoint,
                                                        boost::bind(&CGame::handle_connect, this,
                                                                    boost::asio::placeholders::error));
                break;
            }
            ChangeGameMode(oldmode);
            //TopsiteVote();
        }
        break;
    }

    //TODO: hardware cursors
    /*cursors = device->getGUIEnvironment()->addEmptySpriteBank("cursor");

    cursors->addTexture();
    m_pSprite[SPRID_MOUSECURSOR]->_localimage
    //device->getGUIEnvironment()->getSkin()->setIcon(EGDI_WINDOW_CLOSE, spriteIndex);*/
}

void CGame::UpdateScreen_OnLoading_Progress()
{
    //DIRECTX m_DDraw.ClearBackB4();
    DrawNewDialogBox(SPRID_INTERFACE_ND_LOADING, 0, 0, 0, true);
    DrawVersion(true);
    int iBarWidth;
    iBarWidth = m_cLoading * 2.72;
    m_pSprite[SPRID_INTERFACE_ND_LOADING]->PutSpriteFastWidth(265, 538, 1, iBarWidth, G_dwGlobalTime);
    //DIRECTX m_DDraw.iFlip();
    //Sleep(500);
}


void CGame::UpdateScreen_OnSelectCharacter()
{
    short sX, sY, msX, msY, msZ;
    char  cLB, cRB, cMB, cTotalChar;
    char  cMIresult;
    uint32_t dwTime;
    static uint32_t dwCTime;

    int iMIbuttonNum;

    dwTime = unixtime();
    sX = 0;
    sY = 0;
    cTotalChar = 0;

    //need better way to accomplish first pass data
    if (m_cGameModeCount == 0)
    {
        G_cSpriteAlphaDegree = 1;
        InitGameSettings();

        dwCTime = unixtime();

        lock_guard<std::mutex> lock(uimtx);
        Window * myRoot = System::getSingleton().getDefaultGUIContext().getRootWindow();
        if (ui.selectcharacter == nullptr)
        {
            ui.selectcharacter = WindowManager::getSingleton().loadLayoutFromFile("selectcharacter.layout");
            myRoot->addChild(ui.selectcharacter);
            Window * entergame = ui.selectcharacter->getChild("entergame");
            Window * deletechar = ui.selectcharacter->getChild("deletechar");
            Window * newchar = ui.selectcharacter->getChild("newchar");
            entergame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::SubscriberSlot(&CGame::UIEnterGame, this));
            //deletechar->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::SubscriberSlot(&CGame::UIDeleteChar, this));
            //newchar->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::SubscriberSlot(&CGame::UINewChar, this));
            int i = 0;
            for (shared_ptr<CCharInfo> character : m_pCharList)
            {
                stringstream ss;
                ss << "char" << i;
                Window * charselecttext = ui.selectcharacter->getChild(ss.str());
                ss.str("");
                ss << "selectchar" << i;
                Window * charselectbutton = ui.selectcharacter->getChild(ss.str());
                ss.str("");
                ss << "[colour='FF00A2B2']" << m_pCharList[i]->m_cName << "\n[colour='FF19EAFF']" << m_pCharList[i]->m_sLevel << "\n[colour='FF00E8FF']" << m_pCharList[i]->m_cMapName;
                charselectbutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::SubscriberSlot(&CGame::UISelectCharacterClicked, this));
                charselecttext->setText(ss.str());
                charselecttext->setVisible(true);
                charselecttext->setProperty("BackgroundColours", "tl:FF592500 tr:FF592500 bl:FF592500 br:FF592500");
                ++i;
            }
            Window * charrender = ui.selectcharacter->getChild("charwindow")->getChild("charrender")->getChild("charrender2");

            CEGUI::RenderingSurface *surface = charrender->getRenderingSurface();
            CEGUI::RenderTarget &baseTarget = surface->getRenderTarget();
            CEGUI::IrrlichtTextureTarget &target = dynamic_cast<CEGUI::IrrlichtTextureTarget&>(baseTarget);
            CEGUI::Texture &baseTexture = target.getTexture();
            CEGUI::IrrlichtTexture &texture = dynamic_cast<CEGUI::IrrlichtTexture&>(baseTexture);
            //irr::video::ITexture * irrTexture = texture.getIrrlichtTexture();
            texture.setIrrlichtTexture(charselect);


        }
        else
        {
            //login = myRoot->getChild("login");
        }
        Window * wnd = myRoot->getChild("selectcharacter")->getChild("selectchar0");
        CEGUI::WindowEventArgs e(wnd);
        //(*(CEGUI::WindowEventArgs*)e)
        wnd->fireEvent(CEGUI::PushButton::EventClicked, e);
    }

    ui.selectcharacter->getChild("charwindow")->getChild("charrender")->invalidate(true);

    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_cArrowPressed != 0)
    {
        switch (m_cArrowPressed)
        {
            case 2:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;
            case 4:
                m_cCurFocus--;
                if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
                break;
        }
        m_cArrowPressed = 0;
    }

    if (m_bEscPressed == true)
    {
        isItemLoaded = false;
        ChangeGameMode(GAMEMODE_ONMAINMENU);
        m_bEscPressed = false;
        return;
    }

    if (m_bEnterPressed == true)
    {
        m_bEnterPressed = false;
        PlaySound('E', 14, 5);

        if (m_pCharList[m_cCurFocus - 1] != 0)
        {
            if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
            {
                ZeroMemory(m_cPlayerName, sizeof(m_cPlayerName));
                strcpy(m_cPlayerName, m_pCharList[m_cCurFocus - 1]->m_cName.c_str());
                m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;
                if (m_Misc.bCheckValidString(m_cPlayerName) == true)
                {
                    m_pSprite[SPRID_INTERFACE_ND_LOGIN]->_iCloseSprite();
                    m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->_iCloseSprite();
                    ChangeGameMode(GAMEMODE_ONCONNECTING);
                    m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                    m_wEnterGameType = ENTERGAMEMSGTYPE_NEW;
                    ZeroMemory(m_cMsg, sizeof(m_cMsg));
                    strcpy(m_cMsg, "33");
                    ZeroMemory(m_cMapName, sizeof(m_cMapName));
                    memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName.c_str(), 10);
                    return;
                }
            }
        }
        else
        {
            _InitOnCreateNewCharacter();
            ChangeGameMode(GAMEMODE_ONCREATENEWCHARACTER);
            return;
        }
    }

    ////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
    UpdateScreen_OnSelectCharacter(sX, sY, msX, msY);

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    DrawVersion();
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;

    if (cMIresult)
    {
        PlaySound('E', 14, 5);

        switch (iMIbuttonNum)
        {
            case 1:
            case 2:
            case 3:
            case 4:
                if (m_cCurFocus != iMIbuttonNum)
                    m_cCurFocus = iMIbuttonNum;
                else
                {
                    if (m_pCharList[m_cCurFocus - 1] != 0)
                    {
                        if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
                        {
                            ZeroMemory(m_cPlayerName, sizeof(m_cPlayerName));
                            strcpy(m_cPlayerName, m_pCharList[m_cCurFocus - 1]->m_cName.c_str());
                            m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;
                            if (m_Misc.bCheckValidString(m_cPlayerName) == true)
                            {
                                m_pSprite[SPRID_INTERFACE_ND_LOGIN]->_iCloseSprite();
                                m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->_iCloseSprite();
                                ChangeGameMode(GAMEMODE_ONCONNECTING);
                                m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                                m_wEnterGameType = ENTERGAMEMSGTYPE_NEW;
                                ZeroMemory(m_cMsg, sizeof(m_cMsg));
                                strcpy(m_cMsg, "33");
                                ZeroMemory(m_cMapName, sizeof(m_cMapName));
                                memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName.c_str(), 10);
                                return;
                            }
                        }
                    }
                    else
                    {
                        _InitOnCreateNewCharacter();
                        ChangeGameMode(GAMEMODE_ONCREATENEWCHARACTER);
                        return;
                    }
                }
                break;

            case 5:
                if (m_pCharList[m_cCurFocus - 1] != 0)
                {
                    if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
                    {
                        ZeroMemory(m_cPlayerName, sizeof(m_cPlayerName));
                        strcpy(m_cPlayerName, m_pCharList[m_cCurFocus - 1]->m_cName.c_str());
                        m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;

                        if (m_Misc.bCheckValidString(m_cPlayerName) == true)
                        {
                            m_pSprite[SPRID_INTERFACE_ND_LOGIN]->_iCloseSprite();
                            m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->_iCloseSprite();
                            ChangeGameMode(GAMEMODE_ONCONNECTING);
                            m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                            m_wEnterGameType = ENTERGAMEMSGTYPE_NEW;
                            ZeroMemory(m_cMsg, sizeof(m_cMsg));
                            strcpy(m_cMsg, "33");
                            ZeroMemory(m_cMapName, sizeof(m_cMapName));
                            memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName.c_str(), 10);
                            return;
                        }
                    }
                }
                break;

            case 6:
                if (m_iTotalChar < 4)
                {
                    _InitOnCreateNewCharacter();
                    ChangeGameMode(GAMEMODE_ONCREATENEWCHARACTER);
                    return;
                }
                break;

            case 7:
                if ((m_pCharList[m_cCurFocus - 1] != 0) && (m_pCharList[m_cCurFocus - 1]->m_sLevel <= MAXDELETELEVEL))
                {
                    ChangeGameMode(GAMEMODE_ONQUERYDELETECHARACTER);
                    m_wEnterGameType = m_cCurFocus;
                    return;
                }
                break;

            case 8:
                ChangeGameMode(GAMEMODE_ONCHANGEPASSWORD);
                return;

            case 9:
                isItemLoaded = false;
                ChangeGameMode(GAMEMODE_ONMAINMENU);
                return;
        }
    }

    //	if (m_cGameModeCount < 6) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
    //	if (m_cGameModeCount < 2) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);

    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void CGame::UpdateScreen_OnConnecting()
{
    short sX, sY, msX, msY, msZ;
    char cLB, cRB, cMB;
    uint32_t dwTime = unixtime();
    static uint32_t dwMTime, dwCTime;

    if (m_cGameModeCount == 0)
    {
        m_bEnterPressed = false;
        m_bEscPressed = false;
        dwCTime = dwMTime = unixtime();

        try
        {
            lock_guard<std::mutex> lock(uimtx);
            Window * myRoot = System::getSingleton().getDefaultGUIContext().getRootWindow();
            if (ui.connecting == nullptr)
            {
                ui.connecting = WindowManager::getSingleton().loadLayoutFromFile("connecting.layout");
                ui.connecting->setModalState(true);
                myRoot->addChild(ui.connecting);
            }
            else
            {
                myRoot->addChild(ui.connecting);
                //login = myRoot->getChild("login");
            }
        }
        catch (CEGUI::AlreadyExistsException & e)
        {

        }

        if (_socket == nullptr)
        {
            boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
            CreateSocket();
            new_connection_->socket().async_connect(endpoint,
                                                    boost::bind(&CGame::handle_connect, this,
                                                                boost::asio::placeholders::error));
        }
        else
        {
            if (_socket->handshake_complete)
                ConnectionEstablishHandler(SERVERTYPE_LOG);
        }
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {

        if ((dwTime - m_dwTime) > 1000)
        {
            isItemLoaded = false;
            ChangeGameMode(GAMEMODE_ONMAINMENU);
            gamemode = 0;
            if (_socket)
            {
                _socket->stop();
            }
            if (new_connection_)
            {
                new_connection_->stop();
            }
            WindowManager::getSingleton().destroyWindow(ui.connecting);
            ui.connecting = nullptr;
        }
        m_bEscPressed = false;
        return;
    }

    if ((dwTime - dwMTime) > 150) dwMTime = dwTime;

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    //DIRECTX m_DDraw.ClearBackB4();

    m_bIsHideLocalCursor = true;
    switch (m_cMsg[0])
    {
        case '0':
            _Draw_UpdateScreen_OnCreateNewAccount();
            break;

        case '1':
            sX = 146;
            sY = 114;
            _Draw_OnLogin(m_cAccountName, m_cAccountPassword, 0, 0);
            break;

        case '2':
            _bDraw_OnCreateNewCharacter(m_cPlayerName, 0, 0, 0);
            break;

        case '3':
            UpdateScreen_OnSelectCharacter(0, 0, 0, 0);
            break;

        case '4':
            // Change Password
            UpdateScreen_OnSelectCharacter(0, 0, 0, 0, true);
            break;

        case '5':
            //DIRECTX m_DDraw.ClearBackB4();
            break;
    }
    m_bIsHideLocalCursor = false;

    ////DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
    ////DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx//DIRECTX
    //DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162+80,125+80,2); // xRisenx added x+80 and y+80
    wsprintfA(G_cTxt, "Connecting to Server... (%d)", (dwTime - m_dwTime) / 1000);
    //PutString_SprFont(172 + 35 + 80, 190 + 80, G_cTxt, 7,0,0); // xRisenx added x+80 and y+80
    try
    {
        //BUG: check later
        lock_guard<mutex> lock(uimtx);
        if (ui.connecting != nullptr)
        {
            ui.connecting->getChild("connecttext")->setText(G_cTxt);
        }
    }
    catch (CEGUI::UnknownObjectException & e)
    {
    }


    // 	if ((dwTime - m_dwTime) > 7000)
    // 	{
    // 		PutAlignedString(180+80, 463+80, 195+30+80, UPDATE_SCREEN_ON_CONNECTING1);//"Press ESC key during long time of no"  // xRisenx added x+80 and y+80
    // 		PutAlignedString(180+80, 463+80, 195+45+80, UPDATE_SCREEN_ON_CONNECTING2);//"connection and return to the main menu."  // xRisenx added x+80 and y+80
    // 	}
    // 	else PutAlignedString(180+80, 463+80, 195+30+80, UPDATE_SCREEN_ON_CONNECTING3);//"  Connecting to server. Please wait..."  // xRisenx added x+80 and y+80
    DrawVersion();
    ////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 8, dwTime);
    m_stMCursor.sCursorFrame = 8;

    //	if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void CGame::UpdateScreen_OnWaitInitData()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    uint32_t dwTime = unixtime();

    msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


    if (m_cGameModeCount == 0)
    {
        m_bEnterPressed = false;
        m_bEscPressed = false;
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {
        if ((dwTime - m_dwTime) > 7000)
        {
            isItemLoaded = false;
            ChangeGameMode(GAMEMODE_ONMAINMENU);
            gamemode = 0;
            if (_socket)
            {
                _socket->stop();
            }
        }
        m_bEscPressed = false;
        return;
    }

    ////DIRECTX m_DDraw.ClearBackB4();//DIRECTX
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162 + 80, 125 + 80, 2); // xRisenx added x+80 and y+80

    wsprintfA(G_cTxt, "Waiting for response... %dsec", (dwTime - m_dwTime) / 1000);
    PutString_SprFont(172 + 44 + 80, 190 + 80, G_cTxt, 7, 0, 0); // xRisenx added x+80 and y+80
    if ((dwTime - m_dwTime) > 7000)
    {
        PutAlignedString(174 + 80, 467 + 80, 190 + 30 + 80, UPDATE_SCREEN_ON_WAIT_INIT_DATA1);//"Press ESC key during long time of no" // xRisenx added x+80 and y+80
        PutAlignedString(174 + 80, 467 + 80, 190 + 45 + 80, UPDATE_SCREEN_ON_WAIT_INIT_DATA2);//"connection and return to the main menu." // xRisenx added x+80 and y+80
    }
    else PutAlignedString(174 + 80, 467 + 80, 195 + 30 + 80, UPDATE_SCREEN_ON_WAIT_INIT_DATA3);//  Connecting to server. Please wait..." // xRisenx added x+80 and y+80

    DrawVersion();
    ////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 8, dwTime);
    m_stMCursor.sCursorFrame = 8;

    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();//DIRECTX
}

void CGame::UpdateScreen_OnConnectionLost()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    static uint32_t dwTime;

    msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


    if (m_cGameModeCount == 0)
    {
        // 		dwTime = timeGetTime();
        // 		if (m_bSoundFlag) m_pESound[38]->bStop();
        // 		if ((m_bSoundFlag) && (m_bMusicStat == TRUE))
        // 		{
        // 			if (m_pBGM != NULL) m_pBGM->bStop();
        // 		}//DIRECTX
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;
    ////DIRECTX m_DDraw.ClearBackB4();//DIRECTX
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162 + 80, 125 + 80, 2); // xRisenx added x+80 and y+80
    PutString_SprFont(172 + 54 + 80, 180 + 80, "Connection Lost!", 7, 0, 0); // xRisenx added x+80 and y+80
    PutString(172 + 50 + 80, 180 + 30 + 80, UPDATE_SCREEN_ON_CONNECTION_LOST, video::SColor(255, 0, 0, 0));//" // xRisenx added x+80 and y+80
    DrawVersion();
    ////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;

    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();//DIRECTX

    if ((unixtime() - m_dwTime) > 5000)
    {
        isItemLoaded = false;
        // ShadowEvil - Party close on DC fix
        m_iPartyStatus = 0;
        m_iTotalPartyMember = 0;
        ClearPartyMembers();
        ChangeGameMode(GAMEMODE_ONMAINMENU);
    }
}

void CGame::UpdateScreen_OnCreateNewCharacter()		// DrawCreateCharacter
{
    int i = 0;
    int iMIbuttonNum;
    static int iPoint;
    char cLB, cRB, cMB, cMIresult;
    static char cName[12];
    static char cPrevFocus;
    short msX, msY, msZ;
    bool bFlag;
    static uint32_t dwMTime;
    uint32_t dwTime = unixtime();

    msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


    if (m_cGameModeCount == 0)
    {
        iPoint = m_createStat[STAT_STR] + m_createStat[STAT_VIT] + m_createStat[STAT_DEX] + m_createStat[STAT_INT] + m_createStat[STAT_MAG] + m_createStat[STAT_CHR];
        iPoint = 70 - iPoint;
        cPrevFocus = 1;
        m_cCurFocus = 1;
        m_cMaxFocus = 6;
        m_bEnterPressed = false;
        m_cArrowPressed = 0;
        dwMTime = unixtime();
        StartInputString(239 + 3, 153 + 3, 11, cName);
        ClearInputString();
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_cArrowPressed != 0)
    {
        switch (m_cArrowPressed)
        {
            case 1:
                m_cCurFocus--;
                if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
                break;

            case 3:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;
        }
        m_cArrowPressed = 0;
    }

    if (cPrevFocus != m_cCurFocus)
    {
        EndInputString();
        switch (m_cCurFocus)
        {
            case 1:
                StartInputString(239 + 3, 153 + 3, 11, cName);
                break;
        }
        cPrevFocus = m_cCurFocus;
    }

    if (m_bEscPressed == true)
    {
        ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
        m_bEscPressed = false;
        return;
    }

    ////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
    bFlag = _bDraw_OnCreateNewCharacter(cName, msX, msY, iPoint);

    if ((dwTime - dwMTime) > 100)
    {
        m_cMenuFrame++;
        dwMTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    DrawVersion();
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;
    if (cMIresult)
    {
        PlaySound('E', 14, 5);
        switch (iMIbuttonNum)
        {
            case 1:
                m_cCurFocus = 1;
                break;
            case 2:
                m_cGender = 2;			// Female
                break;
            case 3:
                m_cGender = 1;			// Male
                break;
            case 4:
                m_cSkinCol--;
                if (m_cSkinCol < 1) m_cSkinCol = 3;
                break;
            case 5:
                m_cSkinCol++;
                if (m_cSkinCol > 3) m_cSkinCol = 1;
                break;
            case 6:
                m_cHairStyle--;
                if (m_cHairStyle < 0) m_cHairStyle = 7;
                break;
            case 7:
                m_cHairStyle++;
                if (m_cHairStyle > 7) m_cHairStyle = 0;
                break;
            case 8:
                m_cHairCol--;
                if (m_cHairCol < 0) m_cHairCol = 15;
                break;
            case 9:
                m_cHairCol++;
                if (m_cHairCol > 15) m_cHairCol = 0;
                break;
            case 10:
                m_cUnderCol--;
                if (m_cUnderCol < 0) m_cUnderCol = 7;
                break;
            case 11:
                m_cUnderCol++;
                if (m_cUnderCol > 7) m_cUnderCol = 0;
                break;
            case 12:
                if (iPoint > 0)
                {
                    if (m_createStat[STAT_STR] < 14)
                    {
                        m_createStat[STAT_STR]++;
                        iPoint--;
                    }
                }
                break;
            case 13:
                if (m_createStat[STAT_STR] > 10)
                {
                    m_createStat[STAT_STR]--;
                    iPoint++;
                }
                break;
            case 14:
                if (iPoint > 0)
                {
                    if (m_createStat[STAT_VIT] < 14)
                    {
                        m_createStat[STAT_VIT]++;
                        iPoint--;
                    }
                }
                break;
            case 15:
                if (m_createStat[STAT_VIT] > 10)
                {
                    m_createStat[STAT_VIT]--;
                    iPoint++;
                }
                break;
            case 16:
                if (iPoint > 0)
                {
                    if (m_createStat[STAT_DEX] < 14)
                    {
                        m_createStat[STAT_DEX]++;
                        iPoint--;
                    }
                }
                break;
            case 17:
                if (m_createStat[STAT_DEX] > 10)
                {
                    m_createStat[STAT_DEX]--;
                    iPoint++;
                }
                break;
            case 18:
                if (iPoint > 0)
                {
                    if (m_createStat[STAT_INT] < 14)
                    {
                        m_createStat[STAT_INT]++;
                        iPoint--;
                    }
                }
                break;
            case 19:
                if (m_createStat[STAT_INT] > 10)
                {
                    m_createStat[STAT_INT]--;
                    iPoint++;
                }
                break;
            case 20:
                if (iPoint > 0)
                {
                    if (m_createStat[STAT_MAG] < 14)
                    {
                        m_createStat[STAT_MAG]++;
                        iPoint--;
                    }
                }
                break;
            case 21:
                if (m_createStat[STAT_MAG] > 10)
                {
                    m_createStat[STAT_MAG]--;
                    iPoint++;
                }
                break;
            case 22:
                if (iPoint > 0)
                {
                    if (m_createStat[STAT_CHR] < 14)
                    {
                        m_createStat[STAT_CHR]++;
                        iPoint--;
                    }
                }
                break;
            case 23:
                if (m_createStat[STAT_CHR] > 10)
                {
                    m_createStat[STAT_CHR]--;
                    iPoint++;
                }
                break;

            case 24:
                if (m_cCurFocus != 2)
                {
                    m_cCurFocus = 2;
                    return;
                }
                if (bFlag == false) return;
                //if (m_Misc.bCheckValidName(m_cPlayerName) == FALSE) break;
                if (m_Misc.bCheckValidName(cName) == false) break;
                ZeroMemory(m_cPlayerName, sizeof(m_cPlayerName));
                strcpy(m_cPlayerName, cName);
                ChangeGameMode(GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_CREATENEWCHARACTER;
                ZeroMemory(m_cMsg, sizeof(m_cMsg));
                strcpy(m_cMsg, "22");
                return;


            case 25:
                if (m_cCurFocus != 3)
                {
                    m_cCurFocus = 3;
                    return;
                }
                ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
                return;


            case 26: // WARRIOR
                if (m_cCurFocus != 4)
                {
                    m_cCurFocus = 4;
                    return;
                }
                b_cWarrior = true;
                b_cMage = false;
                b_cBattleMage = false;
                b_cArcher = false;
                m_createStat[STAT_MAG] = 10;
                m_createStat[STAT_INT] = 10;
                m_createStat[STAT_CHR] = 10;
                m_createStat[STAT_STR] = 14;
                m_createStat[STAT_VIT] = 12;
                m_createStat[STAT_DEX] = 14;
                iPoint = m_createStat[STAT_STR] + m_createStat[STAT_VIT] + m_createStat[STAT_DEX] + m_createStat[STAT_INT] + m_createStat[STAT_MAG] + m_createStat[STAT_CHR];
                iPoint = 70 - iPoint;
                break;

            case 27: // MAGE
                if (m_cCurFocus != 5)
                {
                    m_cCurFocus = 5;
                    return;
                }
                b_cWarrior = false;
                b_cMage = true;
                b_cBattleMage = false;
                b_cArcher = false;
                m_createStat[STAT_MAG] = 14;
                m_createStat[STAT_INT] = 14;
                m_createStat[STAT_CHR] = 10;
                m_createStat[STAT_STR] = 10;
                m_createStat[STAT_VIT] = 12;
                m_createStat[STAT_DEX] = 10;
                iPoint = m_createStat[STAT_STR] + m_createStat[STAT_VIT] + m_createStat[STAT_DEX] + m_createStat[STAT_INT] + m_createStat[STAT_MAG] + m_createStat[STAT_CHR];
                iPoint = 70 - iPoint;
                break;

            case 28: // Battle Mage xRisenx
                if (m_cCurFocus != 6)
                {
                    m_cCurFocus = 6;
                    return;
                }
                b_cWarrior = false;
                b_cMage = false;
                b_cBattleMage = true;
                b_cArcher = false;
                m_createStat[STAT_MAG] = 14;
                m_createStat[STAT_INT] = 10;
                m_createStat[STAT_CHR] = 10;
                m_createStat[STAT_STR] = 14;
                m_createStat[STAT_VIT] = 12;
                m_createStat[STAT_DEX] = 10;
                iPoint = m_createStat[STAT_STR] + m_createStat[STAT_VIT] + m_createStat[STAT_DEX] + m_createStat[STAT_INT] + m_createStat[STAT_MAG] + m_createStat[STAT_CHR];
                iPoint = 70 - iPoint;
                break;

            case 29: // Archer xRisenx
                if (isArcherEnabled == false) return;
                if (m_cCurFocus != 7)
                {
                    m_cCurFocus = 7;
                    return;
                }
                b_cWarrior = false;
                b_cMage = false;
                b_cBattleMage = false;
                b_cArcher = true;
                m_createStat[STAT_MAG] = 10;
                m_createStat[STAT_INT] = 10;
                m_createStat[STAT_CHR] = 14;
                m_createStat[STAT_STR] = 10;
                m_createStat[STAT_VIT] = 12;
                m_createStat[STAT_DEX] = 14;
                iPoint = m_createStat[STAT_STR] + m_createStat[STAT_VIT] + m_createStat[STAT_DEX] + m_createStat[STAT_INT] + m_createStat[STAT_MAG] + m_createStat[STAT_CHR];
                iPoint = 70 - iPoint;
                break;

                //case 28: // BMage
                //	if (m_cCurFocus != 6) 
                //	{
                //		m_cCurFocus = 6;
                //		return;
                //	}

                //	m_ccMag = 12 ;
                //	m_ccInt = 14 ;
                //	m_ccChr = 10 ;
                //	m_ccStr = 14 ;
                //	m_ccVit = 10 ;
                //	m_ccDex = 10 ;
                //	iPoint = m_ccStr + m_ccVit + m_ccDex + m_ccInt + m_ccMag + m_ccChr;
                //	iPoint  = 70 - iPoint;
                //	break;

                //case 29: // Archer
                //	if (m_cCurFocus != 7) 
                //	{
                //		m_cCurFocus = 7;
                //		return;
                //	}

                //	m_ccMag = 10 ;
                //	m_ccInt = 10 ;
                //	m_ccChr = 14 ;
                //	m_ccStr = 14 ;
                //	m_ccVit = 10 ;
                //	m_ccDex = 12 ;
                //	iPoint = m_ccStr + m_ccVit + m_ccDex + m_ccInt + m_ccMag + m_ccChr;
                //	iPoint  = 70 - iPoint;
                //	break;
        }


    }
    // 800x600 Fixed xRisenx

    if ((msX >= 239) && (msX <= 347) && (msY >= 153) && (msY <= 172))
    {			// "Enter Character Name"
        PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER1);
    }
    else
        if ((msX >= 281) && (msX <= 336) && (msY >= 205) && (msY <= 218))
        {			// "Select Characters Gender"
            PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER2);
        }
        else
            if ((msX >= 309) && (msX <= 346) && (msY >= 220) && (msY <= 232))
            {			// "Select Characters Skin Color."
                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER3);
            }
            else
                if ((msX >= 309) && (msX <= 346) && (msY >= 235) && (msY <= 247))
                {			// "Select Characters Hairstyle."
                    PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER4);
                }
                else
                    if ((msX >= 309) && (msX <= 346) && (msY >= 250) && (msY <= 262))
                    {			// "Select Characters hair color."
                        PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER5);
                    }
                    else
                        if ((msX >= 309) && (msX <= 346) && (msY >= 265) && (msY <= 277))
                        {			// "Select Characters Underwear color."
                            PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER6);
                        }
                        else
                            if ((msX >= 310) && (msX <= 346) && (msY >= 324) && (msY <= 338))
                            {
                                // Str
                                i = 0;
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER7);
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER8);
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER9);
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER10);
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER11);
                            }
                            else if ((msX >= 310) && (msX <= 346) && (msY >= 341) && (msY <= 355))
                            {
                                // Vit
                                i = 0;
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER12);//"
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER13);//"
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER14);//"
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER15);//"
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER16);//"
                            }
                            else if ((msX >= 310) && (msX <= 346) && (msY >= 357) && (msY <= 371))
                            {
                                // Dex
                                i = 0;
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER17);//"
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER18);//"
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER19);//"
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER20);//"
                            }
                            else if ((msX >= 310) && (msX <= 346) && (msY >= 373) && (msY <= 386))
                            {
                                // Int
                                i = 0;
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER21);//"
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER22);//"
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER23);//"
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER24);
                            }
                            else if ((msX >= 310) && (msX <= 346) && (msY >= 389) && (msY <= 403))
                            {
                                // Mag
                                i = 0;
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER25);
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER26);
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER27);
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER28);
                            }
                            else if ((msX >= 310) && (msX <= 346) && (msY >= 405) && (msY <= 419))
                            {
                                // Char
                                i = 0;
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER29);
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER30);
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER31);
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER32);
                            }

    // 640 Resolution Setup Below xRisenx
    //if ((msX >= 65+4-127+80) && (msX <= 275+4+80) && (msY >= 65+45+120) && (msY <= 82+45+120)) {
    //	PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER1);
    //}
    //else
    //if ((msX >= 261+4-212) && (msX <= 289+4) && (msY >= 111+45) && (msY <= 124+45)) {
    //	PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER2);
    //}
    //else
    //if ((msX >= 261+4-212) && (msX <= 289+4) && (msY >= 126+45) && (msY <= 139+45)) {
    //	PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER3);
    //}
    //else
    //if ((msX >= 261+4-212) && (msX <= 289+4) && (msY >= 141+45) && (msY <= 154+45)) {
    //	PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER4);
    //}
    //else
    //if ((msX >= 261+4-212) && (msX <= 289+4) && (msY >= 156+45) && (msY <= 169+45)) {
    //	PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER5);
    //}
    //else
    //if ((msX >= 261+4-212) && (msX <= 289+4) && (msY >= 171+45) && (msY <= 184+45)) {
    //	PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER6);
    //}
    //else
    //if ((msX >= 240+4-175) && (msX <= 268+4) && (msY >= 231+45) && (msY <= 244+45)) {
    //	// Str
    //	i= 0 ;
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER7);
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER8);
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER9);
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER10);
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER11);
    //}
    //else if ((msX >= 240+4-175) && (msX <= 268+4) && (msY >= 246+45) && (msY <= 259+45)) {

    //	i= 0 ;
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER12);//"
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER13);//"
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER14);//"
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER15);//"
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER16);//"
    //}
    //else if ((msX >= 240+4-175) && (msX <= 268+4) && (msY >= 261+45) && (msY <= 274+45)) {
    //	// Dex
    //	i= 0 ;
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER17);//"
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER18);//"
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER19);//"
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER20);//"
    //}
    //else if ((msX >= 240+4-175) && (msX <= 268+4) && (msY >= 276+45) && (msY <= 289+45)) {
    //	// Int
    //	i= 0 ;
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER21);//"
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER22);//"
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER23);//"
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER24);
    //}
    //else if ((msX >= 240+4-175) && (msX <= 268+4) && (msY >= 291+45) && (msY <= 304+45)) {
    //	// Mag
    //	i= 0 ;
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER25);
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER26);
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER27);
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER28);
    //}
    //else if ((msX >= 240+4-175) && (msX <= 268+4) && (msY >= 306+45) && (msY <= 319+45)) {
    //	// Charisma
    //	i= 0 ;
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER29);
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER30);
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER31);
    //	PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER32);
    //}

                            else if ((msX >= 704) && (msX <= 795) && (msY >= 558) && (msY <= 595))
                            {
                                m_cCurFocus = 2;
                                if (strlen(cName) <= 0)
                                {
                                    i = 0;
                                    PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER35);//"Please enter a character name."
                                }
                                else if (iPoint > 0)
                                {
                                    i = 0;
                                    PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER36);//"You need to select your character class."
                                }
                                else if (m_Misc.bCheckValidName(cName) == false)
                                {
                                    i = 0;
                                    PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER39);//"Cannot use special characters "
                                    PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER40);//"in your character's name. Please"
                                    PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER41);//"type another name."
                                                                                                                                                //PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER39);//"Cannot use special characters "
                                                                                                                                                //PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER40);//"in your character's name. Please"
                                                                                                                                                //PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER41);//"type another name."
                                }
                                else
                                {
                                    i = 0;
                                    PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER44);//"
                                    PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER45);//"
                                    PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER46);//"
                                    PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER47);//"
                                    PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 - 5 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER48);//"
                                                                                                                                                //PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER44);//"
                                                                                                                                                //PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER45);//"
                                                                                                                                                //PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER46);//"
                                                                                                                                                //PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER47);//"
                                                                                                                                                //PutAlignedString(370, 580, 345 + 16*i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER48);//"
                                }
                                //}else if ((msX >= 500+80) && (msX <= 500+72+80) && (msY >= 445+120) && (msY <= 445+15+120))
                                //{	m_cCurFocus = 3;
                                //	PutAlignedString(370+80, 580+80, 345+120, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER49);
                                //}

                                //if ((msX >= 180) && (msX <= 250) && (msY >= 515) && (msY <= 530)) 
                                //{
                                //	m_cCurFocus = 4;
                                //	PutAlignedString(370+80, 580+80, 345+120, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER50);
                                //}

                                //if ((msX >= 260) && (msX <= 330) && (msY >= 515) && (msY <= 530))
                                //{
                                //	m_cCurFocus = 5;
                                //	PutAlignedString(370+80, 580+80, 345+120, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER51);
                                //}
                                // //Battle Mage
                                //if ((msX >= 180) && (msX <= 250) && (msY >= 540) && (msY <= 555)) 
                                //{
                                //	m_cCurFocus = 6;
                                //	PutAlignedString(370+80, 580+80, 345+120, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER52);
                                //}
                                // //Archer
                                //if ((msX >= 260) && (msX <= 330) && (msY >= 540) && (msY <= 555))
                                //{
                                //	m_cCurFocus = 7;
                                //	PutAlignedString(370+80, 580+80, 345+120, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER53);
                                //}

                                //640 Resolution Below

                            }
                            else if ((msX >= 6) && (msX <= 97) && (msY >= 558) && (msY <= 595))			// Cancel
                            {
                                m_cCurFocus = 3;
                                PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER49);
                            }// Difference: 30			30
                             //			87				159				
    if ((msX >= 87) && (msX <= 159) && (msY >= 447) && (msY <= 467))
    {				// Warrior
        m_cCurFocus = 4;
        PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER50);
    }

    if ((msX >= 164) && (msX <= 236) && (msY >= 447) && (msY <= 467))
    {				// Mage
        m_cCurFocus = 5;
        PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER51);
    }

    if ((msX >= 241) && (msX <= 313) && (msY >= 447) && (msY <= 467))
    {				// Battle Mage
        m_cCurFocus = 6;
        PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER52);
    }

    if (isArcherEnabled == false)
    {
        if ((msX >= 318) && (msX <= 390) && (msY >= 447) && (msY <= 467))
        {
            PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER54);
        }
    }
    else
    {
        if ((msX >= 318) && (msX <= 390) && (msY >= 447) && (msY <= 467))
        {				// Archer
            m_cCurFocus = 7;
            PutAlignedString(370 + 80 + 15, 580 + 80 + 15, 345 + 120 - 125, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER53);
        }
    }

    //	if (m_cGameModeCount < 6) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
    //	if (m_cGameModeCount < 2) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);

    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

#ifdef MAKE_ACCOUNT

void CGame::UpdateScreen_OnAgreement()
{
    short sX, sY, msX, msY, msZ;
    char  cLB, cRB, cMB;
    char  cMIresult;
    int i, iTotalLines, iPointerLoc;
    uint32_t dwTime = unixtime();
    double d1, d2, d3;
    int iMIbuttonNum;

    msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


    sX = 121;
    sY = 22;

    if (m_cGameModeCount == 0)
    {
        m_iAgreeView = 0;
        _LoadAgreementTextContents(0);

    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;
    if (m_bEnterPressed == true)
    {
        PlaySound('E', 14, 5);
        ChangeGameMode(GAMEMODE_ONCREATENEWACCOUNT);
        ClearContents_OnCreateNewAccount();
        return;
    }
    if (m_bEscPressed == true)
    {
        PlaySound('E', 14, 5);
        isItemLoaded = false;
        ChangeGameMode(GAMEMODE_ONMAINMENU);
        m_bEscPressed = false;
        return;
    }
    ////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX

    if (cMIresult)
    {
        switch (iMIbuttonNum)
        {
            case 1: // Agree
                PlaySound('E', 14, 5);
                ChangeGameMode(GAMEMODE_ONCREATENEWACCOUNT);
                ClearContents_OnCreateNewAccount();
                return;

            case 2:	// Disagree
                PlaySound('E', 14, 5);
                isItemLoaded = false;
                ChangeGameMode(GAMEMODE_ONMAINMENU);
                return;
        }
    }

    DrawNewDialogBox(SPRID_INTERFACE_ND_AGREEMENT, 0, 0, 0, true);
    DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, 43, 38, 12, false);

    iTotalLines = 0;
    for (i = 0; i < TEXTDLGMAXLINES; i++)
        if (m_pAgreeMsgTextList[i] != 0) iTotalLines++;

    d1 = (double)m_iAgreeView;
    d2 = (double)(iTotalLines - 20);
    d3 = (double)d1 / d2;
    d1 = 338.0f * d3;
    iPointerLoc = (int)d1;
    m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 361 - 112, sY + 37 + 13 + iPointerLoc, 7, dwTime);

    for (i = 0; i < 20; i++)
        if (m_pAgreeMsgTextList[i + m_iAgreeView] != 0)
        {
            PutAlignedString(60, 360, sY + 65 + i * 13, m_pAgreeMsgTextList[i + m_iAgreeView]->m_pMsg, video::SColor(255, 45, 25, 25));
        }

    if (msZ != 0)
    {
        m_iAgreeView = m_iAgreeView - msZ / 60;
        ////DIRECTX m_dInput.m_sZ = 0;//DIRECTX
    }
    if (cLB != 0 && iTotalLines > 20)
    {
        if ((msX >= sX + 345 - 112) && (msX <= sX + 380 - 112) && (msY >= sY + 50) && (msY <= sY + 395))
        {
            d1 = (double)(msY - (sY + 37 + 13));
            d2 = (double)(iTotalLines - 17);
            d3 = (double)(d1 * d2) / (338.0f);
            m_iAgreeView = (int)d3;
            m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutTransSprite(sX + 361 - 112, sY + 37 + 13 + iPointerLoc, 4, dwTime);
        }
    }
    if (m_iAgreeView < 0) m_iAgreeView = 0;
    if (iTotalLines > 20 && m_iAgreeView > iTotalLines - 20) m_iAgreeView = iTotalLines - 20;

    if ((msX > sX + 82 - 105) && (msX < sX + 131 - 105) && (msY > sY + 355 - 3) && (msY < sY + 374 + 3))
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 43 + 20 - 23 + 45 - 105, sY + 265 + 90, 13);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 43 + 20 - 23 + 45 - 105, sY + 265 + 90, 12);

    if ((msX > sX + 235 - 105) && (msX < sX + 303 - 105) && (msY > sY + 355 - 3) && (msY < sY + 375 + 3))
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 158 + 57 - 23 + 45 - 105, sY + 265 + 90, 15);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 158 + 57 - 23 + 45 - 105, sY + 265 + 90, 14);

    DrawVersion();
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;

    //	if (m_cGameModeCount < 6) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
    //	if (m_cGameModeCount < 2) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);

    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void CGame::UpdateScreen_OnCreateNewAccount()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    char cMIresult;
    int  iMIbuttonNum;
    static char cName[12], cPassword[12], cConfirm[12], cPrevFocus, cSSN_A[8], cSSN_B[8], cQuiz[44], cAnswer[20], cTempQuiz[44];
    uint32_t dwTime = unixtime();
    int iFlag = 0;

    msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


    if (m_cGameModeCount == 0)
    {
        cPrevFocus = 1;
        m_cCurFocus = 1;
        m_cMaxFocus = 9; //12
        m_bEnterPressed = false;
        m_cArrowPressed = 0;
        ZeroMemory(m_cEmailAddr, sizeof(m_cEmailAddr));
        ZeroMemory(cName, sizeof(cName));
        ZeroMemory(cPassword, sizeof(cPassword));
        ZeroMemory(cConfirm, sizeof(cConfirm));
        ZeroMemory(cSSN_A, sizeof(cSSN_A));
        ZeroMemory(cSSN_B, sizeof(cSSN_B));
        ZeroMemory(m_cAccountSSN, sizeof(m_cAccountSSN));
        ZeroMemory(cQuiz, sizeof(cQuiz));
        ZeroMemory(cTempQuiz, sizeof(cTempQuiz));
        ZeroMemory(cAnswer, sizeof(cAnswer));
        StartInputString(336, 70 + 157, 11, cName);
        ClearInputString();
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;
    if (m_cArrowPressed != 0)
    {
        switch (m_cArrowPressed)
        {
            case 1:
                m_cCurFocus--;
                if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
                break;
            case 3:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;
        }
        m_cArrowPressed = 0;
    }

    if (m_bEscPressed == true)
    {
        isItemLoaded = false;
        ChangeGameMode(GAMEMODE_ONMAINMENU);
        m_bEscPressed = false;
        return;
    }

    if (cPrevFocus != m_cCurFocus)
    {
        EndInputString();
        switch (m_cCurFocus)
        {
            case 1:
                StartInputString(336, 70 + 157, 11, cName);
                break;
            case 2:
                StartInputString(336, 70 + 157 + 20, 11, cPassword);
                break;
            case 3:
                StartInputString(336, 70 + 157 + 20 * 2, 11, cConfirm);
                break;
            case 4:
                StartInputString(336, 70 + 157 + 20 * 3, 31, m_cEmailAddr);
                break;
            case 5:
                StartInputString(336, 70 + 157 + 20 * 4, 44, cQuiz);
                break;
            case 6:
                StartInputString(336, 70 + 157 + 20 * 5, 19, cAnswer);
                break;

                /*case 1:
                StartInputString(427, 84, 11, cName);
                break;
                case 2:
                StartInputString(427, 106, 11, cPassword);
                break;
                case 3:
                StartInputString(427, 129, 11, cConfirm);
                break;
                case 4:
                StartInputString(311, 215, 31, m_cEmailAddr);
                break;
                case 5:
                StartInputString(311, 253, 44, cQuiz);
                break;
                case 6:
                StartInputString(311, 291 , 19, cAnswer);
                break;*/
        }
        cPrevFocus = m_cCurFocus;
    }

    //DIRECTX m_DDraw.ClearBackB4();
    DrawNewDialogBox(SPRID_INTERFACE_ND_NEWACCOUNT, 0, 0, 0, true);

    PutString2(281, 70 + 157, "Account:", 255, 0, 0);
    PutString2(281, 70 + 157 + 20, "Password:", 255, 0, 0);
    PutString2(281, 70 + 157 + 20 * 2, "(confirm)", 255, 0, 0);
    PutString2(281, 70 + 157 + 20 * 3, "eMail:", 255, 0, 0);
    PutString2(281, 70 + 157 + 20 * 4, "Quiz:", 255, 0, 0);
    PutString2(281, 70 + 157 + 20 * 5, "Answer:", 255, 0, 0);

    if ((m_cCurFocus == 2) || (m_cCurFocus == 3))
        ShowReceivedString(true);
    else if ((m_cCurFocus == 1) || (m_cCurFocus == 4) || (m_cCurFocus == 5) || (m_cCurFocus == 6))
        ShowReceivedString();

    if (m_cCurFocus != 1)
    {
        if (m_Misc.bCheckValidName(cName) != false)
            PutString2(336, 70 + 157, cName, 100, 200, 100);
        else PutString2(336, 70 + 157, cName, 200, 100, 100);
    }
    if (m_cCurFocus != 2)
    {
        if (m_Misc.bCheckValidName(cPassword) != false)
            PutString(336, 70 + 157 + 20, cPassword, video::SColor(255, 100, 200, 100), true, 1);
        else PutString(336, 70 + 157 + 20, cPassword, video::SColor(255, 200, 100, 100), true, 1);
    }

    if (m_cCurFocus != 3)
    {
        if (memcmp(cPassword, cConfirm, 10) == 0)
            PutString(336, 70 + 157 + 20 * 2, cConfirm, video::SColor(255, 100, 200, 100), true, 1);
        else PutString(336, 70 + 157 + 20 * 2, cConfirm, video::SColor(255, 200, 100, 100), true, 1);
    }
    if (memcmp(cPassword, cConfirm, 10) != 0) iFlag = 9;

    if (m_cCurFocus != 4)
    {
        if (m_Misc.bIsValidEmail(m_cEmailAddr))
            PutString2(336, 70 + 157 + 20 * 3, m_cEmailAddr, 100, 200, 100);
        else PutString2(336, 70 + 157 + 20 * 3, m_cEmailAddr, 200, 100, 100);
    }

    /*//DIRECTX m_DDraw.ClearBackB4();
    DrawNewDialogBox(SPRID_INTERFACE_ND_NEWACCOUNT, 0,0,0, TRUE);
    PutString(377, 84,  "Account:",video::SColor(255,100,100,200));
    PutString(372, 106, "Password:",video::SColor(255,100,100,200));
    PutString(372, 129, "(confirm)",video::SColor(255,100,100,200));
    PutString(271, 215, "eMail:",video::SColor(255,100,100,200));
    PutString(276, 253, "Quiz:",video::SColor(255,100,100,200));
    PutString(266, 291, "Answer:",video::SColor(255,100,100,200));

    if ((m_cCurFocus == 2) || (m_cCurFocus == 3))
    ShowReceivedString(TRUE);
    else if ((m_cCurFocus == 1) || (m_cCurFocus == 4) || (m_cCurFocus == 5) || (m_cCurFocus == 6))
    ShowReceivedString();

    if (m_cCurFocus != 1) {
    if (m_Misc.bCheckValidName(cName) != FALSE)
    PutString2(427, 84, cName, 100,200,100);
    else PutString2(427, 84, cName, 200,100,100);
    }
    if (m_cCurFocus != 2) {
    if (m_Misc.bCheckValidName(cPassword) != FALSE)
    PutString(427, 106, cPassword,video::SColor(255,100,200,100), TRUE, 1);
    else PutString(427, 106, cPassword,video::SColor(255,200,100,100), TRUE, 1);
    }

    if (m_cCurFocus != 3) {
    if (memcmp(cPassword, cConfirm, 10) == 0)
    PutString(427, 129, cConfirm,video::SColor(255,100,200,100), TRUE, 1);
    else PutString(427, 129, cConfirm,video::SColor(255,200,100,100), TRUE, 1);
    }
    if (memcmp(cPassword, cConfirm, 10) != 0) iFlag = 9;

    if (m_cCurFocus != 4) {
    if( m_Misc.bIsValidEmail(m_cEmailAddr) )
    PutString2(311, 48 + 190 -25 +2, m_cEmailAddr, 100,200,100);
    else PutString2(311, 48 + 190 -25 +2, m_cEmailAddr, 200,100,100);
    }*/

    wsprintfA(cTempQuiz, "%s", cQuiz);
    m_Misc.ReplaceString(cTempQuiz, ' ', '_');

    if (m_cCurFocus != 5)
    {
        //if(m_Misc.bCheckValidName(cTempQuiz) != FALSE)
        PutString2(336, 70 + 157 + 20 * 4, cQuiz, 100, 200, 100);
        //else PutString2(311, 48 + 226 -25 +4, cQuiz, 200,100,100);
    }

    if (m_cCurFocus != 6)
    {
        //if (m_Misc.bCheckValidName(cAnswer) != FALSE)
        PutString2(336, 70 + 157 + 20 * 5, cAnswer, 100, 200, 100);
        //else PutString2(311, 291, cAnswer, 200,100,100);
    }

    //if (m_cCurFocus != 5) {
    //	//if(m_Misc.bCheckValidName(cTempQuiz) != FALSE)
    //		PutString2(311, 48 + 226 -25 +4, cQuiz, 100,200,100);
    //	//else PutString2(311, 48 + 226 -25 +4, cQuiz, 200,100,100);
    //}

    //if (m_cCurFocus != 6) {
    //	//if (m_Misc.bCheckValidName(cAnswer) != FALSE)
    //		 PutString2(311, 291, cAnswer, 100,200,100);
    //	//else PutString2(311, 291, cAnswer, 200,100,100);
    //}

    //if (m_Misc.bCheckValidName(cAnswer) == FALSE)		iFlag = 13;
    //if (m_Misc.bCheckValidName(cTempQuiz) == FALSE)		iFlag = 12;
    if (strlen(cAnswer) == 0)							iFlag = 11;
    if (strlen(cTempQuiz) == 0)							iFlag = 10;
    if (m_Misc.bCheckValidName(cPassword) == false)		iFlag = 7;
    if (m_Misc.bCheckValidName(cName) == false)			iFlag = 6;
    if (m_Misc.bIsValidEmail(m_cEmailAddr) == false)	iFlag = 5;
    if (strlen(cConfirm) == 0)							iFlag = 3;
    if (strlen(cPassword) == 0)							iFlag = 2;
    if ((strlen(cName) == 0))							iFlag = 1;


    switch (m_cCurFocus)
    {
        case 1:
            PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 200 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT1);
            PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT2);
            break;

        case 2:
            PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT4);
            break;

        case 3:
            PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT8);
            break;

        case 4:
            PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 200 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT21);
            PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT22);
            PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 + 12 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT23);
            break;

        case 5:
            PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 200 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT25);
            PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT26);
            break;

        case 6:
            PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT29);
            break;

        case 7:
            switch (iFlag)
            {
                case 0:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT33);
                    break;

                case 1:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT35);
                    break;

                case 2:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT38);
                    break;

                case 3:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT42);
                    break;

                case 5:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT50);
                    break;

                case 6:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 200 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT52);
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT53);
                    break;

                case 7:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 200 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT56);
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT57);
                    break;

                case 9:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 200 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT63);
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT64);
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 + 12 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT65);
                    break;
                case 10:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT67);
                    break;
                case 11:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT69);
                    break;
                case 12:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 200 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT73);
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT74);
                    break;

                case 13:
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 200 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT77);
                    PutAlignedString(404 - 20 - 125, 637 - 20 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT78);
                    break;
                    /*case 1:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT1);
                    PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT2);
                    break;

                    case 2:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT4);
                    break;

                    case 3:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT8);
                    break;

                    case 4:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT21);
                    PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT22);
                    PutAlignedString(290, 575, 360, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT23);
                    break;

                    case 5:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT25);
                    PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT26);
                    break;

                    case 6:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT29);
                    break;

                    case 7:
                    switch (iFlag) {
                    case 0:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT33);
                    break;

                    case 1:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT35);
                    break;

                    case 2:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT38);
                    break;

                    case 3:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT42);
                    break;

                    case 5:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT50);
                    break;

                    case 6:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT52);
                    PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT53);
                    break;

                    case 7:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT56);
                    PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT57);
                    break;

                    case 9:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT63);
                    PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT64);
                    PutAlignedString(290, 575, 360, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT65);
                    break;
                    case 10:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT67);
                    break;
                    case 11:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT69);
                    break;
                    case 12:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT73);
                    PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT74);
                    break;

                    case 13:
                    PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT77);
                    PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT78);
                    break;*/

            }
            break;

        case 8:
            PutAlignedString(404 - 125, 637 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT80);
            break;

        case 9:
            PutAlignedString(404 - 125, 637 - 125, 212 + 157 - 20, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT81);
            break;
    }

    //case 8:
    //	PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT80);
    //	break;

    //case 9:
    //	PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT81);
    //	break;
    //}


    if ((iFlag == 0) && (m_cCurFocus == 7))
        m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(262 - 5, 224 - 3 + 157, 25, dwTime);
    else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(262 - 5, 224 - 3 + 157, 24, dwTime);

    if (m_cCurFocus == 8)
        m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(262 - 5 + 84, 224 - 3 + 157, 27, dwTime);
    else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(262 - 5 + 84, 224 - 3 + 157, 26, dwTime);

    if (m_cCurFocus == 9)
        m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(262 - 5 + 84 * 2, 224 - 3 + 157, 17, dwTime);
    else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(262 - 5 + 84 * 2, 224 - 3 + 157, 16, dwTime);

    /*if ( (iFlag == 0) && (m_cCurFocus == 7) )
    m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(199+98, 398, 25, dwTime);
    else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(199+98, 398, 24, dwTime);

    if (m_cCurFocus == 8)
    m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(294+98, 398, 27, dwTime);
    else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(294+98, 398, 26, dwTime);

    if (m_cCurFocus == 9)
    m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(390+98, 398, 17, dwTime);
    else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(390+98, 398, 16, dwTime);*/

    DrawVersion(true);
    ////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    if (m_bEnterPressed == true)
    {
        PlaySound('E', 14, 5);
        switch (m_cCurFocus)
        {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;

            case 7:
                if (iFlag != 0) return;
                ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
                ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));

                ZeroMemory(m_cAccountQuiz, sizeof(m_cAccountQuiz));
                ZeroMemory(m_cAccountAnswer, sizeof(m_cAccountAnswer));

                strcpy(m_cAccountName, cName);
                strcpy(m_cAccountPassword, cPassword);

                strcpy(m_cAccountQuiz, cTempQuiz);
                strcpy(m_cAccountAnswer, cAnswer);
                m_cAccountQuiz[45] = ' ';
                m_cAccountAnswer[20] = ' ';

                ZeroMemory(m_cAccountSSN, sizeof(m_cAccountSSN));
                wsprintfA(m_cAccountSSN, "%s-%s", cSSN_A, cSSN_B);

                if (memcmp(cPassword, cConfirm, 10) != 0)
                {
                    ChangeGameMode(GAMEMODE_ONMSG);
                    ZeroMemory(m_cMsg, sizeof(m_cMsg));
                    strcpy(m_cMsg, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT82);
                    //"Cannot create account! - password not match!"
                    return;
                }

                ChangeGameMode(GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_CREATENEWACCOUNT;
                ZeroMemory(m_cMsg, sizeof(m_cMsg));
                strcpy(m_cMsg, "00");
                return;

            case 8:
                ZeroMemory(cName, sizeof(cName));
                ZeroMemory(cPassword, sizeof(cPassword));
                ZeroMemory(cConfirm, sizeof(cConfirm));
                ZeroMemory(m_cAccountAge, sizeof(m_cAccountAge));
                ZeroMemory(m_cAccountCountry, sizeof(m_cAccountCountry));
                ZeroMemory(m_cAccountSSN, sizeof(m_cAccountSSN));
                ZeroMemory(m_cEmailAddr, sizeof(m_cEmailAddr));
                ZeroMemory(cSSN_A, sizeof(cSSN_A));
                ZeroMemory(cSSN_B, sizeof(cSSN_B));
                ZeroMemory(cQuiz, sizeof(cQuiz));
                ZeroMemory(cTempQuiz, sizeof(cTempQuiz));
                ZeroMemory(cAnswer, sizeof(cAnswer));

                break;

            case 9:
                isItemLoaded = false;
                ChangeGameMode(GAMEMODE_ONMAINMENU);
                return;
        }
        m_bEnterPressed = false;
    }

    if (cMIresult)
    {
        PlaySound('E', 14, 5);

        switch (iMIbuttonNum)
        {
            default:
                m_cCurFocus = iMIbuttonNum;
                break;

            case 7:
                if (iFlag != 0) return;
                ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
                ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));

                ZeroMemory(m_cAccountQuiz, sizeof(m_cAccountQuiz));
                ZeroMemory(m_cAccountAnswer, sizeof(m_cAccountAnswer));


                strcpy(m_cAccountName, cName);
                strcpy(m_cAccountPassword, cPassword);
                strcpy(m_cAccountQuiz, cTempQuiz);
                strcpy(m_cAccountAnswer, cAnswer);

                ZeroMemory(m_cAccountSSN, sizeof(m_cAccountSSN));
                wsprintfA(m_cAccountSSN, "%s-%s", cSSN_A, cSSN_B);

                if (memcmp(cPassword, cConfirm, 10) != 0)
                {
                    ChangeGameMode(GAMEMODE_ONMSG);
                    ZeroMemory(m_cMsg, sizeof(m_cMsg));
                    strcpy(m_cMsg, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT82);
                    //"Cannot create account! - password not match!"
                    return;
                }
                ChangeGameMode(GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_CREATENEWACCOUNT;
                ZeroMemory(m_cMsg, sizeof(m_cMsg));
                strcpy(m_cMsg, "00");
                return;

            case 8:
                ZeroMemory(cName, sizeof(cName));
                ZeroMemory(cPassword, sizeof(cPassword));
                ZeroMemory(cConfirm, sizeof(cConfirm));
                ZeroMemory(m_cAccountAge, sizeof(m_cAccountAge));
                ZeroMemory(m_cAccountCountry, sizeof(m_cAccountCountry));
                ZeroMemory(m_cAccountSSN, sizeof(m_cAccountSSN));
                ZeroMemory(m_cEmailAddr, sizeof(m_cEmailAddr));
                ZeroMemory(cSSN_A, sizeof(cSSN_A));
                ZeroMemory(cSSN_B, sizeof(cSSN_B));
                ZeroMemory(cQuiz, sizeof(cQuiz));
                ZeroMemory(cTempQuiz, sizeof(cTempQuiz));
                ZeroMemory(cAnswer, sizeof(cAnswer));
                break;

            case 9:
                isItemLoaded = false;
                ChangeGameMode(GAMEMODE_ONMAINMENU);
                return;
        }
    }

    if ((msX >= 262 - 5) && (msX <= 262 - 5 + 84) && (msY >= 224 - 3 + 157) && (msY <= 224 - 3 + 25 + 157))  m_cCurFocus = 7;  //12
    if ((msX >= 262 - 5 + 84) && (msX <= 262 - 5 + 84 * 2) && (msY >= 224 - 3 + 157) && (msY <= 224 - 3 + 25 + 157)) m_cCurFocus = 8;  //13
    if ((msX >= 262 - 5 + 84 * 2) && (msX <= 262 - 5 + 84 * 3) && (msY >= 224 - 3 + 157) && (msY <= 224 - 3 + 25 + 157)) m_cCurFocus = 9; //14

                                                                                                                                          //if ((msX >= 297) && (msX <= 370) && (msY >= 396) && (msY <= 417))  m_cCurFocus = 7;  //12
                                                                                                                                          //if ((msX >= 392) && (msX <= 465) && (msY >= 396) && (msY <= 417)) m_cCurFocus = 8;  //13
                                                                                                                                          //if ((msX >= 488) && (msX <= 561) && (msY >= 396) && (msY <= 417)) m_cCurFocus = 9; //14

                                                                                                                                          //	if (m_cGameModeCount < 6) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
                                                                                                                                          //	if (m_cGameModeCount < 2) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);

                                                                                                                                          //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

#endif


void CGame::UpdateScreen_OnQuit()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    char cMIresult;
    int  iMIbuttonNum;

    uint32_t dwTime = unixtime();

    if (m_cGameModeCount == 0)
    {
        if (_socket)
        {
            _socket->stop();
        }
        m_bEscPressed = false;
        m_bEnterPressed = false;
        m_bEnterPressed = false;
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 120) m_cGameModeCount = 120;

    //DIRECTX m_DDraw.ClearBackB4();

    if (m_bEscPressed == true || m_bEnterPressed == true)
    {
        m_bEscPressed = false;
        m_bEnterPressed = false;
        ChangeGameMode(GAMEMODE_NULL);
        SendMessage(*(HWND*)m_hWnd, WM_DESTROY, 0, 0);
        return;
    }
    DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 0, 0, 0, true);
    DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 328, 83, 1, true);

    /*if (m_cGameModeCount > 20) DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 255,123,1, TRUE); // Removed xRisenx
    else if ((m_cGameModeCount >= 15) && (m_cGameModeCount <= 20)) m_pSprite[SPRID_INTERFACE_ND_QUIT]->PutTransSprite25(255,123,1, TRUE);*/
    DrawVersion(true);
    //if(m_cGameModeCount == 1000) // Remember to add back xRisenx
    //{	ChangeGameMode(GAMEMODE_NULL);
    //	delete pMI;
    //	SendMessage(m_hWnd, WM_DESTROY, NULL, NULL);
    //	//GoHomepage();
    //	return;
    //}
    //DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;

    if ((cMIresult) && (iMIbuttonNum == 1))
    {
        ChangeGameMode(GAMEMODE_NULL);
        //SendMessage(*(HWND*)m_hWnd, WM_DESTROY, 0, 0);
        device->closeDevice();
        io_service_.stop();
        return;
    }

    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void CGame::UpdateScreen_OnQueryForceLogin()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    char cMIresult;
    int  iMIbuttonNum;

    static uint32_t dwCTime;
    uint32_t dwTime = unixtime();

    if (m_cGameModeCount == 0)
    {
        m_bEnterPressed = false;
        m_bEscPressed = false;
        m_cArrowPressed = 0;

        dwCTime = unixtime();

        PlaySound('E', 25, 0);
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {
        ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
        m_bEscPressed = false;
        return;
    }

    //DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);
    UpdateScreen_OnSelectCharacter(0, 0, 0, 0);
    if ((m_cGameModeCount >= 0) && (m_cGameModeCount < 6))
    {
        ////DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
        //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx
    }
    else if (m_cGameModeCount >= 6)
    {
        ////DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
        //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx
        ////DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
        //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx
    }

    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162 + 80, 130 + 80, 2);

    PutString_SprFont(172 + 86 + 80, 160 + 80, "Account in use", 7, 0, 0);
    PutAlignedString(178 + 80, 453 + 80, 195 + 80, UPDATE_SCREEN_ON_QUERY_FORCE_LOGIN1);
    PutAlignedString(178 + 80, 453 + 80, 215 + 80, UPDATE_SCREEN_ON_QUERY_FORCE_LOGIN2);

    if ((msX >= 200 + 80) && (msX <= 200 + 80 + BTNSZX) && (msY >= 244 + 80) && (msY <= 244 + 80 + BTNSZY))
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 200 + 80, 244 + 80, 19);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 200 + 80, 244 + 80, 18);

    if ((msX >= 370 + 80) && (msX <= 370 + 80 + BTNSZX) && (msY >= 244 + 80) && (msY <= 244 + 80 + BTNSZY))
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 370 + 80, 244 + 80, 3);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 370 + 80, 244 + 80, 2);

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    if (cMIresult)
    {
        PlaySound('E', 14, 5);
        switch (iMIbuttonNum)
        {
            case 1:
                ChangeGameMode(GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                m_wEnterGameType = ENTERGAMEMSGTYPE_NOENTER_FORCEDISCONN;
                ZeroMemory(m_cMsg, sizeof(m_cMsg));
                strcpy(m_cMsg, "33");
                return;

            case 2:
                ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
                break;
        }
    }
    DrawVersion();
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;
    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void CGame::UpdateScreen_OnSelectCharacter(short sX, short sY, short msX, short msY, bool bIgnoreFocus)
{
    int iYear, iMonth, iDay, iHour, iMinute;
    __int64 iTemp1, iTemp2;
    char cTotalChar = 0;
    uint32_t dwTime = unixtime();
    sY = 10;
    //DIRECTX m_DDraw.ClearBackB4();
    //DrawNewDialogBox(SPRID_INTERFACE_ND_SELECTCHAR, 0, 0, 0);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 50);

    iTemp1 = 0;
    iTemp2 = 0;
    iYear = iMonth = iDay = iHour = iMinute = 0;

    if (selectedchar)

    {
        // 		if ((m_cCurFocus - 1 == i) && (bIgnoreFocus == false))
        // 			 m_pSprite[SPRID_INTERFACE_ND_SELECTCHAR]->PutTransSpriteRGB(sX+92+i*138+36, 63+77, 6, 0, 0, 0, dwTime);		// Character HOVER
        cTotalChar++;
        switch (selectedchar->m_sSex)
        {
            case 1:	_tmp_sOwnerType = 1; break;
            case 2:	_tmp_sOwnerType = 4; break;
        }
        _tmp_sOwnerType += selectedchar->m_sSkinCol - 1;
        _tmp_cDir = m_cMenuDir;
        _tmp_sAppr1 = selectedchar->m_sAppr1;
        _tmp_sAppr2 = selectedchar->m_sAppr2;
        _tmp_sAppr3 = selectedchar->m_sAppr3;
        _tmp_sAppr4 = selectedchar->m_sAppr4;
        _tmp_iApprColor = selectedchar->m_iApprColor;
        _tmp_sHeadApprValue = selectedchar->m_sHeadApprValue; // Re-Coding Sprite xRisenx
        _tmp_sBodyApprValue = selectedchar->m_sBodyApprValue; // Re-Coding Sprite xRisenx
        _tmp_sArmApprValue = selectedchar->m_sArmApprValue; // Re-Coding Sprite xRisenx
        _tmp_sLegApprValue = selectedchar->m_sLegApprValue; // Re-Coding Sprite xRisenx
        ZeroMemory(_tmp_cName, sizeof(_tmp_cName));
        memcpy(_tmp_cName, selectedchar->m_cName.c_str(), 10);
        // CLEROTH - NO USE
        _tmp_cAction = OBJECTMOVE;
        _tmp_cFrame = m_cMenuFrame;

        /*
        ****************TODO******************
        Design a top banner that will give
        the character name, and level, aswell
        as the experience on that said char.
        **************************************
        */

        if (selectedchar->m_sSex != 0)
        {
            if (m_Misc.bCheckValidString(selectedchar->m_cName.c_str()) == true)
            {
                m_pEffectSpr[0]->PutTransSprite(sX + 36, sY + 50 + 28, 1, dwTime);
                driver->setRenderTarget(charselect, true, true, video::SColor(0, 0, 0, 0));
                //DrawObject_OnMove_ForMenu(0, 0, sX + 156 + 36, sY + 138 + 118 + 28, false, dwTime, 0, 0);
                DrawObject_OnMove_ForMenu(0, 0, sX + 36, sY + 50 + 28, false, dwTime, 0, 0);
                driver->setRenderTarget(0);
                int	_sLevel = selectedchar->m_sLevel;
                wsprintfA(G_cTxt, "Name: %s", selectedchar->m_cName.c_str());
                PutString(sX + 46 + 96, sY + 160, G_cTxt, video::SColor(255, 255, 255, 255));//25,35,25);	// sY = 14

                wsprintfA(G_cTxt, "Level: %d", _sLevel);
                PutString(sX + 46 + 96, sY + 160 + 17, G_cTxt, video::SColor(255, 255, 255, 255)); //25,35,25);

                wsprintfA(G_cTxt, "%d", selectedchar->m_iExp);
                PutString(sX - 10 + 96, sY + 179 - 9 + 118 + 19 - 2, G_cTxt, video::SColor(255, 255, 255, 255)); //25,35,25);
            }
            iTemp2 = selectedchar->m_iYear * 1000000 + selectedchar->m_iMonth * 60000 + selectedchar->m_iDay * 1700 + selectedchar->m_iHour * 70 + selectedchar->m_iMinute;
            if (iTemp1 < iTemp2)
            {
                iYear = selectedchar->m_iYear;
                iMonth = selectedchar->m_iMonth;
                iDay = selectedchar->m_iDay;
                iHour = selectedchar->m_iHour;
                iMinute = selectedchar->m_iMinute;
                iTemp1 = iTemp2;
            }
        }
    }

    int iTempMon, iTempDay, iTempHour, iTempMin;
    iTempMon = iTempDay = iTempHour = iTempMin = 0;

    /*if (m_iAccntYear != 0)
    {	 iTempMin =  (m_iTimeLeftSecAccount / 60 ) ;
    wsprintfA(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER37, m_iAccntYear, m_iAccntMonth, m_iAccntDay, iTempMin);
    }else
    {	if (m_iTimeLeftSecAccount > 0)
    {	iTempDay =   (m_iTimeLeftSecAccount /(60*60*24)) ;
    iTempHour =  (m_iTimeLeftSecAccount /(60*60)) % 24;
    iTempMin =   (m_iTimeLeftSecAccount / 60 ) % 60;
    wsprintfA(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER38, iTempDay, iTempHour, iTempMin);
    }else strcpy(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER39);
    }
    PutAlignedString(98, 357, 385 +10, G_cTxt);*/


    if (m_iIpYear != 0)
    {
        iTempHour = (m_iTimeLeftSecIP / (60 * 60));
        iTempMin = (m_iTimeLeftSecIP / 60) % 60;
        wsprintfA(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER40, m_iIpYear, m_iIpMonth, m_iIpDay, iTempHour, iTempMin);
    }
    else
    {
        if (m_iTimeLeftSecIP > 0)
        {
            iTempDay = (m_iTimeLeftSecIP / (60 * 60 * 24));
            iTempHour = (m_iTimeLeftSecIP / (60 * 60)) % 24;
            iTempMin = (m_iTimeLeftSecIP / 60) % 60;
            wsprintfA(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER41, iTempDay, iTempHour, iTempMin);
        }
        else
            strcpy(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER42);
    }
    PutAlignedString(98 + 80 - 40, 357 + 80 - 40, 400 + 10 + 120 - 40, G_cTxt);
    if (iYear != 0)
    {
        wsprintfA(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER43, iYear, iMonth, iDay, iHour, iMinute);
        PutAlignedString(98 + 80 - 40, 357 + 80 - 40, 415 + 10 + 120 - 40, G_cTxt);
    }

#ifdef _DEBUG
    PutAlignedString(122, 315, 456, UPDATE_SCREEN_ON_SELECT_CHARACTER36);//"Test Server"
#else

    if (strcmp(m_cWorldServerName, NAME_WORLDNAME1) == 0)
        PutAlignedString(129 + 80, 321 + 80, 456 + 120, MSG_WORLDNAME1); //"ABADDON Server"
    else if (strcmp(m_cWorldServerName, "WS2") == 0)
        PutAlignedString(129, 321, 456, MSG_WORLDNAME2);//"APOCALYPSE Server"

#endif
}

void CGame::UpdateScreen_OnWaitingResponse()
{
    short sX, sY, msX, msY, msZ;
    char cLB, cRB, cMB;

    uint32_t dwTime = unixtime();
    static uint32_t dwCTime;


    if (m_cGameModeCount == 0)
    {
        m_bEnterPressed = false;
        m_bEscPressed = false;
        dwCTime = unixtime();
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {
        if ((dwTime - m_dwTime) > 7000)
        {
            isItemLoaded = false;
            ChangeGameMode(GAMEMODE_ONMAINMENU);
            gamemode = 0;
            if (_socket)
            {
                _socket->stop();
            }
        }
        m_bEscPressed = false;
        return;
    }

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    m_bIsHideLocalCursor = true;
    //DIRECTX m_DDraw.ClearBackB4();
    switch (m_cMsg[0])
    {
        case '0':
            _Draw_UpdateScreen_OnCreateNewAccount();
            break;
        case '1':
            sX = 146;
            sY = 114;
            _Draw_OnLogin(m_cAccountName, m_cAccountPassword, 0, 0);
            break;
        case '2':
            _bDraw_OnCreateNewCharacter(m_cPlayerName, 0, 0, 0);
            break;
        case '3':
            UpdateScreen_OnSelectCharacter(0, 0, 0, 0);
            break;
        case '4':// Change Password
            UpdateScreen_OnSelectCharacter(0, 0, 0, 0, true);
            break;
        case '5':
            //DIRECTX m_DDraw.ClearBackB4();
            break;
    }
    m_bIsHideLocalCursor = false;

    ////DIRECTX m_DDraw.DrawShadowBox(0,0, 639,479);
    //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx

    //TODO: replace with cegui connecting ui
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162 + 80, 125 + 80, 2);
    PutString_SprFont(172 + 44 - 17 + 80, 190 + 80, "Connected. Waiting for response...", 7, 0, 0);

    if ((dwTime - m_dwTime) > 7000)
    {
        PutAlignedString(180 + 80, 463 + 80, 195 + 30 + 80, UPDATE_SCREEN_ON_WATING_RESPONSE1);
        PutAlignedString(180 + 80, 463 + 80, 195 + 45 + 80, UPDATE_SCREEN_ON_WATING_RESPONSE2);
    }
    else PutAlignedString(180 + 80, 463 + 80, 195 + 30 + 80, UPDATE_SCREEN_ON_WATING_RESPONSE3);

    DrawVersion();
    //DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 8, dwTime);
    m_stMCursor.sCursorFrame = 8;

    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void CGame::UpdateScreen_OnQueryDeleteCharacter()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    char cMIresult;
    int  iMIbuttonNum;

    static uint32_t dwCTime;
    uint32_t dwTime = unixtime();


    if (m_cGameModeCount == 0)
    {
        //m_cCurFocus = 1;
        //m_cMaxFocus = 2;
        m_bEnterPressed = false;
        m_cArrowPressed = 0;

        dwCTime = unixtime();

        PlaySound('E', 25, 0);
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {
        ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
        m_bEscPressed = false;
        return;
    }

    //DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);
    UpdateScreen_OnSelectCharacter(0, 0, 500, 70);
    if ((m_cGameModeCount >= 0) && (m_cGameModeCount < 6))
    {	////DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
        //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx
    }
    else if (m_cGameModeCount >= 6)
    {	////DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
        //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx
        ////DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
        //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx
    }

    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162 + 80, 125 + 80, 2);

    PutString_SprFont(172 + 86 + 80, 160 + 80, "Delete Character", 7, 0, 0);
    PutString(215 + 80, 195 + 80, UPDATE_SCREEN_ON_QUERY_DELETE_CHARACTER1, video::SColor(255, 5, 5, 5));//"Character Name"
                                                                                                         //"Character Name"
    PutString(335 + 80, 199 + 80, "__________", video::SColor(255, 5, 5, 5));
    PutString(335 + 80, 195 + 80, m_pCharList[m_wEnterGameType - 1]->m_cName.c_str(), video::SColor(255, 25, 35, 25));
    PutAlignedString(178 + 80, 453 + 80, 220 + 80, UPDATE_SCREEN_ON_QUERY_DELETE_CHARACTER2);//"Do you want to delete the character above?"


    if ((msX >= 200 + 80) && (msX <= 200 + 80 + BTNSZX) && (msY >= 244 + 80) && (msY <= 244 + 80 + BTNSZY))
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 200 + 80, 244 + 80, 19);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 200 + 80, 244 + 80, 18);

    if ((msX >= 370 + 80) && (msX <= 370 + 80 + BTNSZX) && (msY >= 244 + 80) && (msY <= 244 + 80 + BTNSZY))
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 370 + 80, 244 + 80, 3);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 370 + 80, 244 + 80, 2);

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    if (cMIresult)
    {
        PlaySound('E', 14, 5);
        switch (iMIbuttonNum)
        {
            case 1:
                ChangeGameMode(GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_DELETECHARACTER;
                ZeroMemory(m_cMsg, sizeof(m_cMsg));
                strcpy(m_cMsg, "33");
                return;

            case 2:
                ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
                break;
        }
    }
    DrawVersion();
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;
    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void CGame::UpdateScreen_OnLogResMsg()
{
    short msX, msY, msZ, sX, sY;
    char  cLB, cRB, cMB;
    uint32_t dwTime = unixtime();
    static uint32_t dwCTime;
    int   iMIbuttonNum;
    char  cMIresult;


    if (m_cGameModeCount == 0)
    {
        m_bEnterPressed = false;
        m_bEscPressed = false;
        m_cArrowPressed = 0;
        dwCTime = unixtime();
        //DIRECTX		if (m_bSoundFlag) m_pESound[38]->bStop();
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true || m_bEnterPressed)
    {
        switch (m_cMsg[0])
        {
            case '0':
                ChangeGameMode(GAMEMODE_ONCREATENEWACCOUNT);
                break;
            case '1':
                isItemLoaded = false;
                ChangeGameMode(GAMEMODE_ONMAINMENU);
                break;
            case '2':
                ChangeGameMode(GAMEMODE_ONCREATENEWCHARACTER);
                break;
            case '3':
                ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
                break;
            case '4':
                ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
                break;
            case '5':
                isItemLoaded = false;
                ChangeGameMode(GAMEMODE_ONMAINMENU);
                break;
            case '6':
                switch (m_cMsg[1])
                {
                    case 'B':
                        isItemLoaded = false;
                        ChangeGameMode(GAMEMODE_ONMAINMENU);
                        break;
                    case 'C': ChangeGameMode(GAMEMODE_ONCHANGEPASSWORD); break;
                    case 'M': ChangeGameMode(GAMEMODE_ONCHANGEPASSWORD); break;
                }
                break;
            case '7':
            case '8':
                isItemLoaded = false;
                ChangeGameMode(GAMEMODE_ONMAINMENU);
                break;
        }

        m_bEscPressed = false;
        return;
    }

    //DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);

    //DIRECTX m_DDraw.ClearBackB4();

    switch (m_cMsg[0])
    {
        case '0':
        case '5':
            _Draw_UpdateScreen_OnCreateNewAccount();
            break;

        case '1':
        case '7':
            sX = 146;
            sY = 114;

            _Draw_OnLogin(m_cAccountName, m_cAccountPassword, 0, 0);
            break;

        case '2':
        case '4':
            _bDraw_OnCreateNewCharacter(m_cPlayerName, 0, 0, 0);
            break;

        case '3':
            sX = 0;
            sY = 0;
            UpdateScreen_OnSelectCharacter(sX, sY, 0, 0);
            break;

        case '6':
            sX = 146;
            sY = 114;

            UpdateScreen_OnSelectCharacter(0, 0, 0, 0, true);
            break;
        case '8':
            DrawNewDialogBox(SPRID_INTERFACE_ND_MAINMENU, -1, -1, 0, true);
            break;
    }

    ////DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
    //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx

    int dlgX = 243, dlgY = 215;

    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, dlgX, dlgY, 2); //162,125

    if ((msX >= 451) && (msX <= 451 + BTNSZX) && (msY >= 334) && (msY <= 334 + BTNSZY))
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 451, 334, 1);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 451, 334, 0);

    switch (m_cMsg[1])
    {
        case '1':
            PutString_SprFont(dlgX + 72, dlgY + 43, "Password is not correct!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG5);//"
            break;

        case '2':
            PutString_SprFont(dlgX + 80, dlgY + 43, "Not existing account!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG6);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG7);//"
            break;

        case '3':
            PutString_SprFont(dlgX + 54, dlgY + 43, "Can not connect to game server!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG8);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG9);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20 + 20, UPDATE_SCREEN_ON_LOG_MSG10);//"
            break;

        case '4':
            PutString_SprFont(dlgX + 81, dlgY + 43, "New account created.", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG11);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG12);//"
            break;

        case '5':
            PutString_SprFont(dlgX + 56, dlgY + 43, "Can not create new account!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG13);//"
            break;

        case '6':
            PutString_SprFont(dlgX + 55, dlgY + 43, "Can not create new account!", 7, 0, 0);
            PutString_SprFont(dlgX + 53, dlgY + 43 + 15, "Already existing account name.", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG14);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG15);//"
            break;

        case '7':
            PutString_SprFont(dlgX + 77, dlgY + 43, "New character created.", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG16);//"
            break;

        case '8':
            PutString_SprFont(dlgX + 55, dlgY + 43, "Can not create new character!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG17);//"
            break;

        case '9':
            PutString_SprFont(dlgX + 54, dlgY + 43, "Can not create new character!", 7, 0, 0);
            PutString_SprFont(dlgX + 53, dlgY + 43 + 15, "Already existing character name.", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG18);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG19);//"
            break;

        case 'A':
            PutString_SprFont(dlgX + 85, dlgY + 43, "Character deleted.", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG20);//"

            break;
        case 'B':
            PutString_SprFont(dlgX + 85, dlgY + 43, "Password changed.", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG21);//"
            break;
        case 'C':
            PutString_SprFont(dlgX + 76, dlgY + 43, "Can not change password!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG22);//"
            break;

        case 'D':
            PutString_SprFont(dlgX + 53, dlgY + 43, "Can not connect to game server!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG23);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG24);//"
            break;

        case 'E':
            PutString_SprFont(dlgX + 53, dlgY + 43, "Can not connect to game server!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG25);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG26);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20 + 20, UPDATE_SCREEN_ON_LOG_MSG27);//"
            break;

        case 'F':
            PutString_SprFont(dlgX + 53, dlgY + 43, "Can not connect to game server!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG28);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG29);//"
            break;

        case 'G':
            PutString_SprFont(dlgX + 53, dlgY + 43, "Can not connect to game server!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG30);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG31);//"
            break;

        case 'H':
            PutString_SprFont(dlgX + 78, dlgY + 43, "Connection Rejected!", 7, 0, 0);
            if (m_iBlockYear == 0)
            {
                PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG32);//"
                PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG33);//"
            }
            else
            {
                PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG34);//"
                wsprintfA(G_cTxt, UPDATE_SCREEN_ON_LOG_MSG35, m_iBlockYear, m_iBlockMonth, m_iBlockDay);//"
                PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, G_cTxt);
            }
            break;

        case 'I': //
            PutString_SprFont(dlgX + 77, dlgY + 43, "Not Enough Point!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, "ÂI¼Æ¨Ï¥Î´Á­­¤wµ²§ô, ½Ð¦ÜGD2S.gamania.com©µªø¨Ï¥Î´Á­­");

            break;

        case 'J':
            PutString_SprFont(dlgX + 77, dlgY + 43, "World Server Full", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, "Please ! Try Other World Server");
            break;

        case 'M':
            PutString_SprFont(dlgX + 70, dlgY + 43, "Your password expired", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, "Please! Change password");
            break;


        case 'U':
            PutString_SprFont(dlgX + 69, dlgY + 43, "Keycode input Success!", 7, 0, 0);
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, "Keycode Registration successed.");

            break;

        case 'X':
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG38);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG39);//"
            break;

        case 'Y':
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG40);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG41);//"
            break;

        case 'Z':
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, UPDATE_SCREEN_ON_LOG_MSG42);//"
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74 + 20, UPDATE_SCREEN_ON_LOG_MSG41);//"
            break;
    }

    if (cMIresult)
    {
        switch (iMIbuttonNum)
        {
            case 1:
                switch (m_cMsg[0])
                {
                    case '0':
                        ChangeGameMode(GAMEMODE_ONCREATENEWACCOUNT);
                        break;
                    case '1':
                        isItemLoaded = false;
                        ChangeGameMode(GAMEMODE_ONMAINMENU);
                        break;
                    case '2':
                        ChangeGameMode(GAMEMODE_ONCREATENEWCHARACTER);
                        break;
                    case '3':
                        ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
                        break;
                    case '4':
                        ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
                        break;
                    case '5':
                        isItemLoaded = false;
                        ChangeGameMode(GAMEMODE_ONMAINMENU);
                        break;
                    case '6':
                        switch (m_cMsg[1])
                        {
                            case 'B':
                                isItemLoaded = false;
                                ChangeGameMode(GAMEMODE_ONMAINMENU);
                                break;
                            case 'C': ChangeGameMode(GAMEMODE_ONCHANGEPASSWORD); break;
                            case 'M': ChangeGameMode(GAMEMODE_ONCHANGEPASSWORD); break;
                        }
                        break;
                    case '7':
                    case '8':
                        isItemLoaded = false;
                        ChangeGameMode(GAMEMODE_ONMAINMENU);
                        break;
                }
                return;
        }
    }

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;
    DrawVersion();
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;
    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void CGame::UpdateScreen_OnChangePassword()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    char cMIresult;
    int  iMIbuttonNum;
    static char  cName[12], cPassword[12], cNewPassword[12], cNewPassConfirm[12], cPrevFocus;
    static uint32_t dwCTime;
    uint32_t dwTime = unixtime();
    bool bFlag = true;

    msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


    if (m_cGameModeCount == 0)
    {
        EndInputString();

        cPrevFocus = 2; //1
        m_cCurFocus = 2; //1
        m_cMaxFocus = 6;
        m_bEnterPressed = false;
        m_cArrowPressed = 0;

        ZeroMemory(cName, sizeof(cName));
        ZeroMemory(cPassword, sizeof(cPassword));
        ZeroMemory(cNewPassword, sizeof(cNewPassword));
        ZeroMemory(cNewPassConfirm, sizeof(cNewPassConfirm));

        strcpy(cName, m_cAccountName);
        //StartInputString(314, 155, 11, cName);
        StartInputString(314, 179, 11, cPassword);
        ClearInputString();
        dwCTime = dwTime;
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    if (m_cArrowPressed != 0)
    {
        switch (m_cArrowPressed)
        {
            case 1:
                m_cCurFocus--;
                if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
                break;

            case 2:
                if (m_cCurFocus == 3) m_cCurFocus = 4;
                else if (m_cCurFocus == 4) m_cCurFocus = 3;
                break;

            case 3:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;

            case 4:
                if (m_cCurFocus == 3) m_cCurFocus = 4;
                else if (m_cCurFocus == 4) m_cCurFocus = 3;
                break;
        }
        m_cArrowPressed = 0;
    }

    if (m_bEnterPressed == true)
    {
        PlaySound('E', 14, 5);
        switch (m_cCurFocus)
        {
            case 1:
            case 2:
            case 3:
            case 4:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;

            case 5:	// Connect
                if ((m_Misc.bCheckValidString(cPassword) == false) || (strlen(cPassword) == 0) ||
                    (m_Misc.bCheckValidName(cNewPassword) == false) || (m_Misc.bCheckValidName(cNewPassConfirm) == false) ||
                    (strlen(cNewPassword) == 0) || (memcmp(cNewPassword, cNewPassConfirm, 10) != 0)) break;

                ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
                ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
                ZeroMemory(m_cNewPassword, sizeof(m_cNewPassword));
                ZeroMemory(m_cNewPassConfirm, sizeof(m_cNewPassConfirm));
                strcpy(m_cAccountName, cName);
                strcpy(m_cAccountPassword, cPassword);
                strcpy(m_cNewPassword, cNewPassword);
                strcpy(m_cNewPassConfirm, cNewPassConfirm);
                ChangeGameMode(GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_CHANGEPASSWORD;
                ZeroMemory(m_cMsg, sizeof(m_cMsg));
                strcpy(m_cMsg, "41");
                return;

            case 6:	// Cancel
                ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
                //m_dwLoginMode = MSGID_REQUEST_LOGIN;
                return;
        }
        m_bEnterPressed = false;
    }

    if (m_bEscPressed == true)
    {
        isItemLoaded = false;
        ChangeGameMode(GAMEMODE_ONMAINMENU);
        m_bEscPressed = false;
        return;
    }

    if (cPrevFocus != m_cCurFocus)
    {
        EndInputString();
        switch (m_cCurFocus)
        {
            case 1:
                StartInputString(314, 155, 11, cName);
                break;
            case 2:
                StartInputString(314, 179, 11, cPassword);
                break;
            case 3:
                StartInputString(314, 203, 11, cNewPassword);
                break;
            case 4:
                StartInputString(314, 227, 11, cNewPassConfirm);
                break;
        }
        cPrevFocus = m_cCurFocus;
    }

    //DIRECTX m_DDraw.ClearBackB4();

    UpdateScreen_OnSelectCharacter(0, 0, 0, 0, true);
    ////DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);//SelectCharacter
    //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx

    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 153, 112, 0);
    //DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT , 153, 112, 13);
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 152 + 157, 112 + 109, 7);//

    PutString(206, 155, UPDATE_SCREEN_ON_CHANGE_PASSWORD1, video::SColor(255, 25, 35, 25));
    PutString(206, 179, UPDATE_SCREEN_ON_CHANGE_PASSWORD2, video::SColor(255, 25, 35, 25));
    PutString(206, 203, UPDATE_SCREEN_ON_CHANGE_PASSWORD3, video::SColor(255, 25, 35, 25));
    PutString(206, 227, UPDATE_SCREEN_ON_CHANGE_PASSWORD4, video::SColor(255, 25, 35, 25));

    if (m_cCurFocus != 1)
    {
        if (m_Misc.bCheckValidString(cName) != false)
            PutString(314, 155, cName, video::SColor(255, 25, 35, 25));
        else PutString(314, 155, cName, video::SColor(255, 55, 18, 13));
    }
    if ((m_Misc.bCheckValidString(cName) == false) || (strlen(cName) == 0)) bFlag = false;

    if (m_cCurFocus != 2)
    {
        if ((m_Misc.bCheckValidString(cPassword) != false))
            PutString(314, 179, cPassword, video::SColor(255, 25, 35, 25), true, 3);
        else PutString(314, 179, cPassword, video::SColor(255, 55, 18, 13), true, 3);
    }

    if (m_cCurFocus != 3)
    {
        if ((m_Misc.bCheckValidName(cNewPassword) != false))
            PutString(314, 203, cNewPassword, video::SColor(255, 25, 35, 25), true, 3);
        else PutString(314, 203, cNewPassword, video::SColor(255, 55, 18, 13), true, 3);
    }

    if (m_cCurFocus != 4)
    {
        if ((m_Misc.bCheckValidName(cNewPassConfirm) != false))
            PutString(314, 227, cNewPassConfirm, video::SColor(255, 25, 35, 25), true, 3);
        else PutString(314, 227, cNewPassConfirm, video::SColor(255, 55, 18, 13), true, 3);
    }

    if ((m_Misc.bCheckValidString(cPassword) == false) || (strlen(cPassword) == 0) ||
        (strlen(cNewPassword) < 8) || (memcmp(cNewPassword, cNewPassConfirm, 10) != 0) ||
        (memcmp(cPassword, cNewPassword, 10) == 0)) bFlag = false;


    if (m_cCurFocus == 1) ShowReceivedString();
    else if ((m_cCurFocus == 2) || (m_cCurFocus == 3) || (m_cCurFocus == 4)) ShowReceivedString(true);

    PutAlignedString(153, 487, 258, UPDATE_SCREEN_ON_CHANGE_PASSWORD5);//"
    PutAlignedString(153, 487, 273, UPDATE_SCREEN_ON_CHANGE_PASSWORD6);//"
    PutAlignedString(153, 487, 288, UPDATE_SCREEN_ON_CHANGE_PASSWORD7);//"

    if ((bFlag == true) && (m_cCurFocus == 5))
        m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(197, 320, 21, dwTime);
    else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(197, 320, 20, dwTime);

    if (m_cCurFocus == 6)
        m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(370, 320, 17, dwTime);
    else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(370, 320, 16, dwTime);

    DrawVersion();
    //DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;
    if (cMIresult)
    {
        PlaySound('E', 14, 5);

        switch (iMIbuttonNum)
        {
            case 1:
            case 2:
            case 3:
            case 4:
                m_cCurFocus = iMIbuttonNum;
                break;

            case 5:
                if ((m_Misc.bCheckValidString(cPassword) == false) || (strlen(cPassword) == 0) ||
                    (m_Misc.bCheckValidName(cNewPassword) == false) || (m_Misc.bCheckValidName(cNewPassConfirm) == false) ||
                    (strlen(cNewPassword) == 0) || (memcmp(cNewPassword, cNewPassConfirm, 10) != 0)) break;

                EndInputString();
                ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
                ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
                ZeroMemory(m_cNewPassword, sizeof(m_cNewPassword));
                ZeroMemory(m_cNewPassConfirm, sizeof(m_cNewPassConfirm));
                strcpy(m_cAccountName, cName);
                strcpy(m_cAccountPassword, cPassword);
                strcpy(m_cNewPassword, cNewPassword);
                strcpy(m_cNewPassConfirm, cNewPassConfirm);
                ChangeGameMode(GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_CHANGEPASSWORD;
                ZeroMemory(m_cMsg, sizeof(m_cMsg));
                strcpy(m_cMsg, "41");
                return;

            case 6:
                // Cancel
                ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
                return;
        }
    }

    if ((msX >= 197) && (msX <= 197 + BTNSZX) && (msY >= 320) && (msY <= 320 + BTNSZY)) m_cCurFocus = 5;
    if ((msX >= 370) && (msX <= 370 + BTNSZX) && (msY >= 320) && (msY <= 320 + BTNSZY)) m_cCurFocus = 6;

    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void CGame::UpdateScreen_OnVersionNotMatch()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    char cMIresult;
    int  iMIbuttonNum;
    uint32_t dwTime = unixtime();

    msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


    if (m_cGameModeCount == 0)
    {
        if (_socket)
        {
            _socket->stop();
        }
        m_bEnterPressed = false;
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 120) m_cGameModeCount = 120;
    //DIRECTX m_DDraw.ClearBackB4();
    if (m_bEscPressed == true || m_bEnterPressed == true)
    {
        m_bEscPressed = false;
        m_bEnterPressed = false;
        ChangeGameMode(GAMEMODE_NULL);
        device->closeDevice();
        io_service_.stop();
        //SendMessage(*(HWND*)m_hWnd, WM_DESTROY, 0, 0);
        return;
    }
    DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 0, 0, 0, true);
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162, 125, 2);
    PutAlignedString(168, 474, 160, UPDATE_SCREEN_ON_VERSION_NO_MATCH1);
    PutAlignedString(168, 474, 180, UPDATE_SCREEN_ON_VERSION_NO_MATCH2);
    PutAlignedString(168, 474, 250, MSG_HOMEPAGE);
    DrawVersion();
    //DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;

    if ((cMIresult) && (iMIbuttonNum == 1))
    {
        ChangeGameMode(GAMEMODE_NULL);
        device->closeDevice();
        io_service_.stop();
        //SendMessage(*(HWND*)m_hWnd, WM_DESTROY, 0, 0);
        return;
    }
    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void CGame::UpdateScreen_OnGame()
{
    lock_guard<std::mutex> lock(screenupdate);

    short sVal, sDivX, sModX, sDivY, sModY, sPivotX, sPivotY, sVPXsave, sVPYsave;
    static int  iUpdateRet = -1;
    static uint32_t perfVar = 0, perfCnt = 0;
    short absX, absY, tX, tY;
    uint32_t ItemColor;
    int  i, iAmount;
    uint32_t dwTime = unixtime();
    static uint32_t dwPrevChatTime = 0;
    static int   imX = 0, imY = 0;
    iUpdateRet = 1;
    if (m_cGameModeCount == 0)
    {
        //DIRECTX m_DDraw.ClearBackB4();
        //DIRECTX m_DDraw.ResetFrameCnt();
        m_dwFPStime = m_dwCheckConnTime = m_dwCheckSprTime = m_dwCheckChatTime = dwTime;
        if (m_bMusicStat) StartBGM();
        //if(m_iLevel < 40) AddEventList(UPDATE_SCREEN_ONGAME12, 10);
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 20) m_cGameModeCount = 20;
    // ----------------------------------------------------

    //DIRECTX//DIRECTX m_dInput.UpdateMouseState();
    m_dwCurTime = unixtime();

    if (m_bEnterPressed)
    {
        m_bEnterPressed = false;

        if (iGetTopDialogBoxIndex() == 7 && m_bIsDialogEnabled[7] && m_dialogBoxes[7].GetMode() == 1)
        {
            EndInputString();

            if (strlen(m_cGuildName) == 0) return;
            if (strcmp(m_cGuildName, "NONE") != 0)
            {
                bSendCommand(MSGID_REQUEST_CREATENEWGUILD, MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
                m_dialogBoxes[7].SetMode(2);
            }
        }
        else
            if (iGetTopDialogBoxIndex() == 17 && m_bIsDialogEnabled[17] && m_dialogBoxes[17].GetMode() == 1)
            {
                // Drop Item Query
                EndInputString();

                if (m_bSkillUsingStatus == true)
                {
                    AddEventList(UPDATE_SCREEN_ONGAME1, 10);
                    return;
                }

                if ((m_bIsDialogEnabled[20] == true)
                    && ((m_dialogBoxes[20].GetMode() == 1) || (m_dialogBoxes[20].GetMode() == 2)))
                {
                    AddEventList(UPDATE_SCREEN_ONGAME1, 10);//"You can not give or drop before the actual Item transaction."
                    return;
                }

                if ((m_bIsDialogEnabled[4] == true) || (m_bIsDialogEnabled[23] == true) || (m_bIsDialogEnabled[26] == true))
                {
                    AddEventList(UPDATE_SCREEN_ONGAME1, 10);
                    return;
                }

                if (strlen(m_cAmountString) == 0) return;
                iAmount = atoi(m_cAmountString);

                if ((int)(m_pItemList[m_dialogBoxes[17].sView]->m_dwCount) < iAmount)
                {
                    iAmount = m_pItemList[m_dialogBoxes[17].sView]->m_dwCount;
                }

                if (iAmount != 0)
                {
                    if ((int)(m_pItemList[m_dialogBoxes[17].sView]->m_dwCount) >= iAmount)
                    {
                        if (m_dialogBoxes[17].sV1 != 0)
                        {
                            absX = abs(m_dialogBoxes[17].sV1 - m_sPlayerX);
                            absY = abs(m_dialogBoxes[17].sV2 - m_sPlayerY);

                            if ((absX == 0) && (absY == 0))
                                AddEventList(UPDATE_SCREEN_ONGAME5, 10);
                            else if ((absX <= 8) && (absY <= 8))
                            {
                                switch (m_dialogBoxes[17].sV3)
                                {
                                    case 1:
                                    case 2:
                                    case 3:
                                    case 4:
                                    case 5:
                                    case 6:
                                        EnableDialogBox(20, 1, m_dialogBoxes[17].sView, m_dialogBoxes[17].sV3);
                                        //m_dialogBoxes[20].CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
                                        m_dialogBoxes[20].sV3 = iAmount;
                                        m_dialogBoxes[20].sV4 = m_wCommObjectID;
                                        m_dialogBoxes[20].sV5 = m_dialogBoxes[17].sV1;
                                        m_dialogBoxes[20].sV6 = m_dialogBoxes[17].sV2;

                                        ZeroMemory(m_dialogBoxes[20].cStr, sizeof(m_dialogBoxes[20].cStr));
                                        strcpy(m_dialogBoxes[20].cStr, m_dialogBoxes[17].cStr);
                                        break;

                                    case 20:
                                        EnableDialogBox(20, 3, m_dialogBoxes[17].sView, m_dialogBoxes[17].sV3);
                                        //m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
                                        m_dialogBoxes[20].sV3 = iAmount;
                                        m_dialogBoxes[20].sV4 = m_wCommObjectID;
                                        m_dialogBoxes[20].sV5 = m_dialogBoxes[17].sV1;
                                        m_dialogBoxes[20].sV6 = m_dialogBoxes[17].sV2;

                                        ZeroMemory(m_dialogBoxes[20].cStr, sizeof(m_dialogBoxes[20].cStr));
                                        GetNpcName(m_dialogBoxes[17].sV3, m_dialogBoxes[20].cStr);
                                        break;

                                    case 15:
                                    case 24:
                                        EnableDialogBox(20, 2, m_dialogBoxes[17].sView, m_dialogBoxes[17].sV3);
                                        //m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
                                        m_dialogBoxes[20].sV3 = iAmount;
                                        m_dialogBoxes[20].sV4 = m_wCommObjectID;

                                        ZeroMemory(m_dialogBoxes[20].cStr, sizeof(m_dialogBoxes[20].cStr));
                                        GetNpcName(m_dialogBoxes[17].sV3, m_dialogBoxes[20].cStr);
                                        break;
                                    case NPC_PRINCESS:
                                        EnableDialogBox(20, 10, m_dialogBoxes[17].sView, m_dialogBoxes[17].sV3);
                                        //m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
                                        m_dialogBoxes[20].sV3 = iAmount;
                                        m_dialogBoxes[20].sV4 = m_wCommObjectID;
                                        m_dialogBoxes[20].sV5 = m_dialogBoxes[17].sV1;
                                        m_dialogBoxes[20].sV6 = m_dialogBoxes[17].sV2;

                                        ZeroMemory(m_dialogBoxes[20].cStr, sizeof(m_dialogBoxes[20].cStr));
                                        GetNpcName(m_dialogBoxes[17].sV3, m_dialogBoxes[20].cStr);
                                        break;
                                    case 1000: // Trade stackable items
                                               // hum, déjà affiché? , j'attends le retour et je désactive!
                                               /*m_stDialogBoxInfo[27].sV1 = m_pItemList[m_stDialogBoxInfo[17].sV4]->m_sSprite;
                                               m_stDialogBoxInfo[27].sV2 = m_pItemList[m_stDialogBoxInfo[17].sV4]->m_sSpriteFrame;
                                               m_stDialogBoxInfo[27].sV3 = iAmount;
                                               m_stDialogBoxInfo[27].sV4 = m_pItemList[m_stDialogBoxInfo[17].sV4]->m_cItemColor;*/

                                        if (m_stDialogBoxExchangeInfo[0].sV1 == -1)			m_stDialogBoxExchangeInfo[0].sItemID = m_dialogBoxes[17].sV4;
                                        else if (m_stDialogBoxExchangeInfo[1].sV1 == -1)	m_stDialogBoxExchangeInfo[1].sItemID = m_dialogBoxes[17].sV4;
                                        else if (m_stDialogBoxExchangeInfo[2].sV1 == -1)	m_stDialogBoxExchangeInfo[2].sItemID = m_dialogBoxes[17].sV4;
                                        else if (m_stDialogBoxExchangeInfo[3].sV1 == -1)	m_stDialogBoxExchangeInfo[3].sItemID = m_dialogBoxes[17].sV4;
                                        else return; // Impossible case, tested at function beginning

                                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SETEXCHANGEITEM, 0, m_dialogBoxes[17].sV4, iAmount, 0, 0);
                                        break;

                                    case 1001:
                                        // Sell List
                                        for (i = 0; i < MAXSELLLIST; i++)
                                            if (m_stSellItemList[i].iIndex == -1)
                                            {
                                                m_stSellItemList[i].iIndex = m_dialogBoxes[17].sV4;
                                                m_stSellItemList[i].iAmount = iAmount;
                                                m_bIsItemDisabled[m_dialogBoxes[17].sV4] = true;
                                                break;
                                            }
                                        if (i == MAXSELLLIST) AddEventList(UPDATE_SCREEN_ONGAME6, 10);
                                        break;

                                    case 1002:
                                        if (_iGetBankItemCount() < MAXBANKITEMS)
                                            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, m_dialogBoxes[39].sV1, iAmount, m_dialogBoxes[39].sV5, m_dialogBoxes[39].sV6, m_pItemList[m_dialogBoxes[39].sV1]->m_cName, m_dialogBoxes[39].sV4); //v1.4
                                        else
                                            AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);
                                        break;

                                    case 1003: // guild bank
                                        if (m_guildBankMap.size() < maxGWHItems[m_guild.m_upgrades[GU_WAREHOUSE]])
                                            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, m_dialogBoxes[39].sV1, iAmount, m_dialogBoxes[39].sV5, m_dialogBoxes[39].sV6, m_pItemList[m_dialogBoxes[39].sV1]->m_cName, m_dialogBoxes[39].sV4);
                                        else
                                            AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);
                                        break;

                                    case 1004: // mail item send
                                        if (m_dialogBoxes[DIALOG_MAILBOX].vvec.size() < MAX_MAIL_ITEMS)
                                        {
                                            MailItemSend item;
                                            item.index = m_dialogBoxes[17].sView;
                                            item.count = iAmount;
                                            m_dialogBoxes[DIALOG_MAILBOX].vvec.push_back(item);
                                        }
                                        else
                                        {
                                            AddEventList(DLGBOX_CLICK_NPCACTION_QUERY10, 10);
                                        }
                                        break;

                                    default:
                                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, (char)(m_dialogBoxes[17].sView), iAmount, m_dialogBoxes[17].sV1, m_dialogBoxes[17].sV2, m_pItemList[m_dialogBoxes[17].sView]->m_cName);
                                        break;
                                }
                                m_bIsItemDisabled[m_dialogBoxes[17].sView] = true;
                            }
                            else AddEventList(UPDATE_SCREEN_ONGAME7, 10);
                        }
                        else
                        {
                            if (iAmount <= 0)
                            {
                                AddEventList(UPDATE_SCREEN_ONGAME8, 10);
                            }
                            else
                            {
                                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_ITEMDROP, 0, m_dialogBoxes[17].sView, iAmount, 0, m_pItemList[m_dialogBoxes[17].sView]->m_cName);
                                m_bIsItemDisabled[m_dialogBoxes[17].sView] = true;

                            }
                        }
                    }
                    else
                    {
                        AddEventList(UPDATE_SCREEN_ONGAME9, 10);//"You entered more quantity that you can carry."
                    }
                }
                DisableDialogBox(17);
            }
            else if (iGetTopDialogBoxIndex() == 58 && m_bIsDialogEnabled[58] && m_dialogBoxes[58].GetMode() == 1)
            {

                // contribute gold/maj to guild

                EndInputString();

                if (m_bSkillUsingStatus)
                {
                    AddEventList(UPDATE_SCREEN_ONGAME1, 10);
                    return;
                }

                if ((m_bIsDialogEnabled[20] == true)
                    && ((m_dialogBoxes[20].GetMode() == 1) || (m_dialogBoxes[20].GetMode() == 2)))
                {
                    AddEventList(UPDATE_SCREEN_ONGAME1, 10);//"You can not give or drop before the actual Item transaction."
                    return;
                }

                if ((m_bIsDialogEnabled[4] == true) || (m_bIsDialogEnabled[23] == true) || (m_bIsDialogEnabled[26] == true))
                {
                    AddEventList(UPDATE_SCREEN_ONGAME1, 10);
                    return;
                }

                if (strlen(m_cAmountString) == 0) return;
                iAmount = atoi(m_cAmountString);

                switch (m_dialogBoxes[58].sView)
                {
                    case 0: // Gold
                        if (GetGold() < iAmount)
                            iAmount = GetGold();
                        break;
                    case 1: // Maj
                        if (m_iGizonItemUpgradeLeft < iAmount)
                            iAmount = m_iGizonItemUpgradeLeft;
                        break;
                    case 2: // Cont
                        if (m_iContribution < iAmount)
                            iAmount = m_iContribution;
                        break;
                }

                if (iAmount <= 0)
                {
                    AddEventList(UPDATE_SCREEN_ONGAME8, 10);
                }
                else
                {
                    bSendCommand(MSGID_GUILDCONTRIBUTE, 0, 0, m_dialogBoxes[58].sView, iAmount, 0, m_pItemList[m_dialogBoxes[58].sView]->m_cName);
                }
                DisableDialogBox(58);
            }
            else if (iGetTopDialogBoxIndex() == DIALOG_GUILD && m_bIsDialogEnabled[DIALOG_GUILD] &&
                     m_dialogBoxes[DIALOG_GUILD].GetMode() == 10 && m_pInputBuffer != m_cChatMsg)
            {
                EndInputString();
            }
            else if (iGetTopDialogBoxIndex() == DIALOG_MAILBOX && m_bIsDialogEnabled[DIALOG_MAILBOX] &&
                     m_dialogBoxes[DIALOG_MAILBOX].GetMode() >= 1 && m_dialogBoxes[DIALOG_MAILBOX].GetMode() < 5 &&
                     m_pInputBuffer != m_cChatMsg)
            {
                EndInputString();
            }
            else
            {
                if (!m_bInputStatus)
                {
                    switch (m_cBackupChatMsg[0])
                    {
                        case '!':
                        case '@':
                        case '#':
                        case '$':
                        case '^':
                            ZeroMemory(m_cChatMsg, sizeof(m_cChatMsg));
                            m_cChatMsg[0] = m_cBackupChatMsg[0];
                            //StartInputString(10, 414, sizeof(m_cChatMsg), m_cChatMsg);
                            StartInputString(10, 530, sizeof(m_cChatMsg), m_cChatMsg); // 800x600 Resolution xRisenx 534 is right / 530 fits?
                            break;
                        default:
                            //StartInputString(10, 414, sizeof(m_cChatMsg), m_cChatMsg);
                            StartInputString(10, 530, sizeof(m_cChatMsg), m_cChatMsg); // 800x600 Resolution xRisenx 534 is right / 530 fits?
                            ClearInputString();
                            break;
                    }
                }
                else
                {
                    EndInputString();
                    ZeroMemory(G_cTxt, sizeof(G_cTxt));
                    ReceiveString((char *)G_cTxt);
                    ZeroMemory(m_cBackupChatMsg, sizeof(m_cBackupChatMsg));
                    strcpy(m_cBackupChatMsg, G_cTxt);
                    if ((G_cTxt[0] != '!' && m_dwCurTime - dwPrevChatTime > 400) ||
                        (G_cTxt[0] == '!' && m_dwCurTime - dwPrevChatTime > 700))
                    {
                        dwPrevChatTime = m_dwCurTime;
                        m_curse.ConvertString(G_cTxt, strlen(G_cTxt));
                        if (strlen(G_cTxt) > 0)
                        {
                            if ((G_cTxt[0] == '!') || (G_cTxt[0] == '~'))
                            {
                                if (m_Misc.bCheckIMEString(G_cTxt) == false) return;
                            }
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, G_cTxt);

                        }
                    }
                }
            }
    }
    // Gladiator Arena xRisenx
    //	if(strcmp(m_cMapName, ARENAMAP) == 0)
    //{
    //   char cEntry[64];
    //   int iEntry = 0;
    //
    //   wsprintfA(cEntry, "Name      Kills      Deaths");
    //   PutString( 10, 100, cEntry,video::SColor(255,255,255,255) );
    //
    //   for (i = 0; i < MAXARENAPLAYERS; i++)
    //   {
    //      //if(iEntry >= 5) break;  // only show top 5
    //	   if(iEntry >= 20) break;  // Showing top 20
    //      if(strlen(m_stArenaPlayers[i].cCharName) != 0)
    //      {
    //         iEntry++;
    //         wsprintfA(cEntry, "%s      %i     %i", m_stArenaPlayers[i].cCharName, m_stArenaPlayers[i].iKills, m_stArenaPlayers[i].iDeaths);
    //         PutString( 10, 100 + (iEntry*15), cEntry,video::SColor(255,255,255,255) );
    //      }
    //   }
    //}
    // Gladiator Arena xRisenx
    sVPXsave = m_sViewPointX;
    sVPYsave = m_sViewPointY;

    if ((m_iCameraShakingDegree > 0) && (iUpdateRet != 0))
    {
        m_sViewPointX += m_iCameraShakingDegree - (rand() % m_iCameraShakingDegree * 2);
        m_sViewPointY += m_iCameraShakingDegree - (rand() % m_iCameraShakingDegree * 2);
        m_iCameraShakingDegree--;
        if (m_iCameraShakingDegree <= 0) m_iCameraShakingDegree = 0;
    }
    sPivotX = m_pMapData->m_sPivotX;
    sPivotY = m_pMapData->m_sPivotY;
    sVal = m_sViewPointX - (sPivotX * 32);
    sDivX = sVal / 32;
    sModX = sVal % 32;
    sVal = m_sViewPointY - (sPivotY * 32);
    sDivY = sVal / 32;
    sModY = sVal % 32;
    if (fullscreenswap) iUpdateRet = 1;
    if (iUpdateRet != 0)
        DrawBackground(sDivX, sModX, sDivY, sModY);

    if (iUpdateRet != 0)
        DrawEffectLights();
    if (iUpdateRet != 0)
        DrawObjects(sPivotX, sPivotY, sDivX, sDivY, sModX, sModY, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);

    if (iUpdateRet != 0)
    {
        DrawEffects();
        DrawWeatherEffects();
        DrawChatMsgs(-100, 0, GetWidth(), GetHeight());
        WeatherObjectFrameCounter();
    }

    if (m_cMapIndex == 26)	//Snoopy: Add Apocalypse map effect (fires in inferniaA)
    {
        m_pEffectSpr[89]->PutTransSprite(1296 - m_sViewPointX, 1283 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[89]->PutTransSprite(1520 - m_sViewPointX, 1123 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[89]->PutTransSprite(1488 - m_sViewPointX, 3971 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[93]->PutTransSprite(2574 - m_sViewPointX, 3677 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[93]->PutTransSprite(3018 - m_sViewPointX, 3973 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
    }
    else if (m_cMapIndex == 27)	//Add Apocalypse map effect (fires in inferniaB)
    {
        m_pEffectSpr[89]->PutTransSprite(1293 - m_sViewPointX, 3657 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[89]->PutTransSprite(944 - m_sViewPointX, 3881 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[89]->PutTransSprite(1325 - m_sViewPointX, 4137 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[89]->PutTransSprite(1648 - m_sViewPointX, 3913 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
    }
    //Snoopy: Add Apocalypse Gate and apocalypse map effects (if no Gate, m_iGatePositX will be -1...
    if ((m_iGatePositX >= m_sViewPointX / 32) && (m_iGatePositX <= m_sViewPointX / 32 + 20)
        && (m_iGatePositY >= m_sViewPointY / 32) && (m_iGatePositY <= m_sViewPointY / 32 + 15))
    {
        m_pEffectSpr[101]->PutTransSprite(m_iGatePositX * 32 - m_sViewPointX - 96, m_iGatePositY * 32 - m_sViewPointY - 69, _tmp_iEffectFrame % 30, dwTime);
    }
    if (iUpdateRet != 0)
        DrawDialogBoxes();

    if ((iUpdateRet != 0) && m_bInputStatus)
    {
        if ((m_bIsDialogEnabled[7] && m_dialogBoxes[7].GetMode() == 1) ||
            (m_bIsDialogEnabled[17] && m_dialogBoxes[17].GetMode() == 1) ||
            (m_bIsDialogEnabled[58] && m_dialogBoxes[58].GetMode() == 1) ||
            (m_bIsDialogEnabled[DIALOG_MAILBOX] && m_dialogBoxes[DIALOG_MAILBOX].GetMode() == 1) ||
            (m_bIsDialogEnabled[DIALOG_GUILD] && m_dialogBoxes[DIALOG_GUILD].GetMode() == 10)
            )
            //{}else //DIRECTX m_DDraw.DrawShadowBox(0, 413, 639, 429);
        {
        }
        else //DIRECTX m_DDraw.DrawShadowBox(0, 529, 799, 546); // 800x600 Resolution xRisenx
            ShowReceivedString();
    }

    if (iUpdateRet != 0)
        ShowEventList(m_dwCurTime);

    if ((iUpdateRet != 0) && (m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_ITEM) &&
        (m_pItemList[m_stMCursor.sSelectedObjectID] != 0))
    {
        ItemColor = m_pItemList[m_stMCursor.sSelectedObjectID]->m_ItemColor;
        if (ItemColor != 0)
        {
            if ((m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos == EQUIPPOS_LHAND) ||
                (m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos == EQUIPPOS_RHAND) ||
                (m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos == EQUIPPOS_TWOHAND))
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->PutSpriteRGB(G_pGame->m_stMCursor.sX - m_stMCursor.sDistX, G_pGame->m_stMCursor.sY - m_stMCursor.sDistY,
                                                                                                                           m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame,
                                                                                                                           ItemColor,
                                                                                                                           dwTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->PutSpriteRGB(G_pGame->m_stMCursor.sX - m_stMCursor.sDistX, G_pGame->m_stMCursor.sY - m_stMCursor.sDistY,
                                                                                                                           m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame,
                                                                                                                           ItemColor,
                                                                                                                           dwTime);
            }
        }
        else m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->PutSpriteFast(G_pGame->m_stMCursor.sX - m_stMCursor.sDistX, G_pGame->m_stMCursor.sY - m_stMCursor.sDistY,
                                                                                                                         m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame, dwTime);

        if (isItemLoaded == false)
        {
            _LoadItemContents();		// Item Display - ShadowEvil
            isItemLoaded = true;
        }
        isCommon = isUncommon = isRare = isEpic = isLegendary = false;

        int  iLoc;
        std::vector<string> * lines = GetItemName(m_pItemList[m_stMCursor.sSelectedObjectID]);


        iLoc = 0;

        std::vector<string>::iterator iter = lines->begin(), end = lines->end();

        uint32_t maxWidth = 0;

        while (iter != end)
        {
            if (iter->size() > maxWidth)
                maxWidth = iter->size();

            ++iter;
        }

        //DIRECTX m_DDraw.DrawShadowBox(G_pGame->m_stMCursor.sX - 3, G_pGame->m_stMCursor.sY + 25 - 1, G_pGame->m_stMCursor.sX + maxWidth * 5.5, G_pGame->m_stMCursor.sY + 25+1 + 13*lines->size());

        iter = lines->begin();

        PutFontString(font[FONT_TREBMS8PX], G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY + 25, iter->c_str(), m_itemColor, false, 1);
        iLoc += 13;
        ++iter;

        while (iter != end)
        {
            PutFontString(font[FONT_TREBMS8PX], G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY + 25 + iLoc, iter->c_str(), video::SColor(255, 150, 150, 150), false, 1);
            iLoc += 13;
            ++iter;
        }

        // This is handled inside the GetItemName() function.
        //if ( iLoc == 13 )
        //{
        //	iLoc = 0;
        //	for ( int iTmp = 0 ; iTmp < MAXITEMS ; iTmp++ )
        //	{
        //		if( m_pItemList[iTmp] != NULL )
        //		{
        //			if( strcmp(m_pItemList[iTmp]->m_cName, m_pItemList[m_stMCursor.sSelectedObjectID]->m_cName ) == 0 )
        //				iLoc++;
        //		}
        //	}
        //	if( iLoc > 1 )
        //	{
        //		wsprintfA(G_cTxt, MSG_TOTAL_NUMBER, iLoc);
        //		PutString(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY + 40, G_cTxt,video::SColor(255,150,150,150), FALSE, 1);
        //	}
        //}
    }

    //Snoopy: Add Apocalypse map effect (druncncity bubbles)
    if (iUpdateRet && m_cMapIndex == 25)
        //{	bAddNewEffect(13, m_sViewPointX + rand() %640, m_sViewPointY + rand() %480, 0, 0, -1*(rand() %80), 1);
    {
        bAddNewEffect(13, m_sViewPointX + rand() % GetWidth(), m_sViewPointY + rand() % GetHeight(), 0, 0, -1 * (rand() % 80), 1); // 800x600 Resolution xRisenx
        DrawDruncncity();
    }

    if (iUpdateRet)
    {
        if (m_displayQuest && m_stQuest.sQuestType)
            DrawQuestHelper();

        if (m_bIsHeldenianMode && m_bIsHeldenianMap)
            DrawHeldenianStats();

        if (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)
            DrawAstoriaStats();

        if (m_bSoccerMode && memcmp(m_cCurLocation, "fight", 5) == 0)
            DrawSoccerInfo();

        DrawTopMsg();
    }

#ifdef _DEBUG
#ifdef MOUSEDEBUG
    if (iUpdateRet)
    {
        wsprintfA(G_cTxt, "M(%d,%d) T(%d,%d)", G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY, (m_sViewPointX + G_pGame->m_stMCursor.sX) / 32, (m_sViewPointY + G_pGame->m_stMCursor.sY) / 32);
        PutString(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY + 30, G_cTxt, video::SColor(255, 255, 255, 255));
    }
#endif
#endif

    // LogOut process
    if (m_cLogOutCount > 0)
    {
        if ((dwTime - m_dwLogOutCountTime) > 1000) // Change to 1000 before releasing xRisenx
        {
            m_cLogOutCount--;
            m_dwLogOutCountTime = dwTime;
            wsprintfA(G_cTxt, UPDATE_SCREEN_ONGAME13, m_cLogOutCount);
            AddEventList(G_cTxt, 10);
        }
    }
    // ShadowEvil - Close Party Logout Process
    if (m_cLogOutCount <= 1 && m_cLogOutCount > 0)
    {
        m_iPartyStatus = 0;
        m_iTotalPartyMember = 0;
        ClearPartyMembers();
    }
    // Logout
    if (m_cLogOutCount == 0)
    {
        gamemode = 0;
        isItemLoaded = false;
        _socket->stop();
        m_bEscPressed = false;

        PlaySound('E', 14, 5);
        //DIRECTX		if (m_bSoundFlag) m_pESound[38]->bStop();
        if ((m_bSoundFlag) && (m_bMusicStat == true))
        {
            //DIRECTX			if (m_pBGM != NULL) m_pBGM->bStop();
        }
        isItemLoaded = false;
        ChangeGameMode(GAMEMODE_ONMAINMENU);
        return;
    }

    // Restart Process
    if (m_cRestartCount > 0)
    {
        if ((dwTime - m_dwRestartCountTime) > 1000)
        {
            m_cRestartCount--;
            m_dwRestartCountTime = dwTime;
            wsprintfA(G_cTxt, UPDATE_SCREEN_ONGAME14, m_cRestartCount); // "Restarting game...%d"
            AddEventList(G_cTxt, 10);
        }
    }
    // Restart
    if (m_cRestartCount == 0)
    {
        m_cRestartCount = -1;
        bSendCommand(MSGID_REQUEST_RESTART);
        return;
    }

    //Snoopy: Added diag "LEVEL UP" right down
    if (iUpdateRet && m_iLU_Point >0) PutString_SprFont(GetWidth() - 80, GetHeight() - 90, "Level Up", 250, 250, 0); // 800x600 Resolution xRisenx added x+160 and y+120
                                                                                                                     //if (m_cGameModeCount < 6) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
    if (m_cGameModeCount < 6) //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx
                              //if (m_cGameModeCount < 2) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
        if (m_cGameModeCount < 2) //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx

                                  //if( iUpdateRet == 0 ) m_iFrameCount++;
                                  //else m_iFrameCount+=256;


            if (iUpdateRet != 0 && (dwTime - lastFriendUpdate) < 6000 && (dwTime - lastFriendUpdate) > 4000)
                for (int f = 0; f < 13; f++)
                    if (friendsList[f].updated == false)
                        friendsList[f].online = false;

    perfVar++;
    if (iUpdateRet != 0)
    {
        if (m_adminLevel > 1)
        {
            //PutAlignedString(0, 639, 0, "Ethereal", (m_GMFlags & GMFLAG_ETHEREAL) ?video::SColor(255,50,220,50) :video::SColor(255,220,50,50));
            PutAlignedString(0, GetWidth() - 1, 0, "Ethereal", (m_GMFlags & GMFLAG_ETHEREAL) ? video::SColor(255, 50, 220, 50) : video::SColor(255, 220, 50, 50)); // 800x600 Resolution xRisenx

            if (m_adminLevel >= 4)
            {
                //PutAlignedString(0, 639, 15, "No aggro", (m_GMFlags & GMFLAG_NOAGGRO) ?video::SColor(255,50,220,50) :video::SColor(255,220,50,50));
                //PutAlignedString(0, 639, 30,  "Invincible", (m_GMFlags & GMFLAG_INVINCIBLE) ?video::SColor(255,50,220,50) :video::SColor(255,220,50,50));
                PutAlignedString(0, GetWidth() - 1, 15, "No aggro", (m_GMFlags & GMFLAG_NOAGGRO) ? video::SColor(255, 50, 220, 50) : video::SColor(255, 220, 50, 50)); // 800x600 Resolution xRisenx
                PutAlignedString(0, GetWidth() - 1, 30, "Invincible", (m_GMFlags & GMFLAG_INVINCIBLE) ? video::SColor(255, 50, 220, 50) : video::SColor(255, 220, 50, 50)); // 800x600 Resolution xRisenx
            }
        }

        if (m_eventPot != -1)
        {
            wsprintfA(G_cTxt, "Total pot: %d", m_eventPot);
            //PutAlignedString(0, 639, 45,  G_cTxt,video::SColor(255,50,220,50));
            PutAlignedString(0, GetWidth() - 1, 45, G_cTxt, video::SColor(255, 50, 220, 50)); // 800x600 Resolution xRisenx
        }

        if (m_showTime)
        {
            wsprintfA(G_cTxt, "  %d", unixtime() / 1000 % 60);
            PutString(10, 100, G_cTxt, video::SColor(255, 255, 255, 255), false, 1);
        }

        // 		if( m_bShowFPS )
        // 		{
        // 			wsprintfA( G_cTxt, "FPS: %d", m_iFPS);
        // 			PutString( 10, 115, G_cTxt,video::SColor(255,255,255,255), FALSE, 1 );
        // 			wsprintfA( G_cTxt, "PC: %dk", perfCnt/1000 );
        // 			PutString( 10, 130, G_cTxt,video::SColor(255,255,255,255), FALSE, 1 );
        // 		}

#ifdef TitleClient
        // notify of title level up
        if (dwTime - dwTitleLevelUpTime < 5000)
        {
            //PutString_SprFont3(250, 200, "NEW TITLE ACQUIRED!", 200, 250, 2);
            //PutString_SprFont3(320-((strlen(cTitleLevelUpSubject)*7)/2), 215, cTitleLevelUpSubject, 2, 200, 250);
            PutString_SprFont3(300, 280, "NEW TITLE ACQUIRED!", 200, 250, 2);
            PutString_SprFont3(380 - ((strlen(cTitleLevelUpSubject) * 7) / 2), 295, cTitleLevelUpSubject, 2, 200, 250);
        }
#endif

        if (m_bIsObserverMode == true)
        {	//DIRECTX m_DDraw.PutPixel(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY,   255,255,255);
            //DIRECTX m_DDraw.PutPixel(G_pGame->m_stMCursor.sX+1, G_pGame->m_stMCursor.sY, 255,255,255);
            //DIRECTX m_DDraw.PutPixel(G_pGame->m_stMCursor.sX-1, G_pGame->m_stMCursor.sY, 255,255,255);
            //DIRECTX m_DDraw.PutPixel(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY+1, 255,255,255);
            //DIRECTX m_DDraw.PutPixel(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY-1, 255,255,255);
        }
        //else m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY, m_stMCursor.sCursorFrame, dwTime);

        //if( //DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST ) RestoreSprites();
    }

    if (dwTime - m_dwFPStime > 1000)
    {
        m_dwFPStime = dwTime;
        m_iFPS = //DIRECTX m_DDraw.GetFrameCnt();
                 //DIRECTX m_DDraw.ResetFrameCnt();
            perfCnt = perfVar;
        perfVar = 0;
    }

    //if( dwTime - m_dwFPStime > 1000 )
    //{
    //	m_dwFPStime = dwTime;
    //	//m_iFPS = //DIRECTX m_DDraw.GetFrameCnt();
    //	m_iFPS = perfVar>>7;
    //	if ( m_iFPS < 10 ) m_iFPS += 6;
    //	//DIRECTX m_DDraw.ResetFrameCnt();
    //	perfCnt = perfVar;
    //	perfVar = 0;
    //}

    //if((dwTime - lastScreenUpdate) > 1)
    {
        iUpdateRet = m_pMapData->iObjectFrameCounter(m_cPlayerName, m_sViewPointX, m_sViewPointY);
        lastScreenUpdate = dwTime;
    }
    //else iUpdateRet = 0;
    // Magic Icons xRisenx
    if (dwTime - m_dwAuraTime > 1000)
    {

        if (m_sZerk > 0)
        {
            m_sZerk--;
            if (m_sZerk == 0) m_bZerk = false;
        }
        if (m_sInv > 0)
        {
            m_sInv--;
            if (m_sInv == 0) m_bInv = false;
        }
        if (m_sPfm > 0)
        {
            m_sPfm--;
            if (m_sPfm == 0) m_bPfm = false;
        }
        if (m_sPfa > 0)
        {
            m_sPfa--;
            if (m_sPfa == 0) m_bPfa = false;
        }
        if (m_sShield > 0)
        {
            m_sShield--;
            if (m_sShield == 0) m_bShield = false;
        }
#ifdef MonsterBarClient
        if (m_sNpcBar == 0)
            bNpcBar = false;
        else m_sNpcBar--;
#endif
        m_dwAuraTime = dwTime;
    }

    if ((bEffectFrameCounter() == true) && (iUpdateRet == 0)) iUpdateRet = -1;
    if (iUpdateRet == 2)
    {
        m_bCommandAvailable = true;
        m_dwCommandTime = 0;
    }
    CommandProcessor(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY, ((sDivX + sPivotX) * 32 + sModX + G_pGame->m_stMCursor.sX - 17) / 32 + 1, ((sDivY + sPivotY) * 32 + sModY + G_pGame->m_stMCursor.sY - 17) / 32 + 1, G_pGame->m_stMCursor.LB, G_pGame->m_stMCursor.RB, G_pGame->m_stMCursor.MB);

    m_sViewPointX = sVPXsave;
    m_sViewPointY = sVPYsave;

    if (iUpdateRet > 0) CalcViewPoint();

    if (m_bIsObserverMode)
    {
        if ((dwTime - m_dwObserverCamTime) > 25)
        {
            m_dwObserverCamTime = dwTime;
            CalcViewPoint();
            iUpdateRet = -1;
        }
    }

    if (iUpdateRet > 0)
    {
        if (m_bDrawFlagDir == false)
        {
            m_iDrawFlag++;
            if (m_iDrawFlag >= 25)
            {
                m_iDrawFlag = 25;
                m_bDrawFlagDir = true;
            }
        }
        else
        {
            m_iDrawFlag--;
            if (m_iDrawFlag < 0)
            {
                m_iDrawFlag = 0;
                m_bDrawFlagDir = false;
            }
        }
    }
    // iUpdateRet
}


void CGame::UpdateScreen_OnSelectServer()
{
    short msX, msY, msZ, sX, sY;
    char cLB, cRB, cMB;
    char cMIresult;
    int  iMIbuttonNum;
    static char  cPrevFocus;
    uint32_t dwTime = unixtime();
    bool bFlag = true;


    sX = 146;
    sY = 114;
    if (m_cGameModeCount == 0)
    {
        EndInputString();

        cPrevFocus = 1;
        m_cCurFocus = 1;
        m_cMaxFocus = 3;

        m_bEnterPressed = false;
        m_cArrowPressed = 0;
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_cArrowPressed != 0)
    {
        switch (m_cArrowPressed)
        {
            case 1:
                m_cCurFocus--;
                if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
                break;

            case 3:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;
        }
        m_cArrowPressed = 0;
    }

    if (m_bEnterPressed == true)
    {
        m_bEnterPressed = false;
        PlaySound('E', 14, 5);
        switch (m_cCurFocus)
        {
            case 1:
                if (strlen(m_cWorldServerName) == 0)
                    ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));
                strcpy(m_cWorldServerName, NAME_WORLDNAME1);
                ChangeGameMode(GAMEMODE_ONLOGIN);
                return;

            case 2:
                ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));
                strcpy(m_cWorldServerName, "WS2");
                ChangeGameMode(GAMEMODE_ONLOGIN);
                return;

            case 3:	// Cancel
                isItemLoaded = false;
                ChangeGameMode(GAMEMODE_ONMAINMENU);
                return;
        }
    }

    if (m_bEscPressed == true)
    {
        isItemLoaded = false;
        ChangeGameMode(GAMEMODE_ONMAINMENU);
        m_bEscPressed = false;
        return;
    }
    if (cPrevFocus != m_cCurFocus)
    {
        cPrevFocus = m_cCurFocus;
    }
    //DIRECTX m_DDraw.ClearBackB4();
    DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 0, 0, 0, true);
    if (m_cGameModeCount > 20) DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 40, 121, 1, true);
    else if ((m_cGameModeCount >= 15) && (m_cGameModeCount <= 20)) m_pSprite[SPRID_INTERFACE_ND_LOGIN]->PutTransSprite25(40, 121, 1, true);

    if (m_cGameModeCount > 20)
    {
        if (m_cCurFocus == 1) DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 138, 177, 5, true);
        if (m_cCurFocus == 2) DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 130, 205, 6, true);
        if (m_cCurFocus == 3) DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 256, 282, 4, true);
    }
    DrawVersion();
    //DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;

    if (cMIresult)
    {
        PlaySound('E', 14, 5);

        switch (iMIbuttonNum)
        {
            case 1:
                if (m_cCurFocus == 1)
                {
                    ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));
                    strcpy(m_cWorldServerName, NAME_WORLDNAME1);
                    ChangeGameMode(GAMEMODE_ONLOGIN);
                    return;
                }
                else m_cCurFocus = 1;
                break;

            case 2:
                if (m_cCurFocus == 2)
                {
                    ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));
                    strcpy(m_cWorldServerName, "WS2");
                    ChangeGameMode(GAMEMODE_ONLOGIN);
                    return;
                }
                else m_cCurFocus = 2;
                break;

            case 3:
                isItemLoaded = false;
                ChangeGameMode(GAMEMODE_ONMAINMENU);
                return;
        }
    }

    if ((msX >= 130) && (msX <= 295) && (msY >= 175) && (msY <= 198)) m_cCurFocus = 1;
    if ((msX >= 130) && (msX <= 295) && (msY >= 199) && (msY <= 225)) m_cCurFocus = 2;
    if ((msX >= 256) && (msX <= 331) && (msY >= 279) && (msY <= 308)) m_cCurFocus = 3;

    //	if (m_cGameModeCount < 6) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
    //	if (m_cGameModeCount < 2) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);

    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}
