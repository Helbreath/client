

#include "newsocket.h"
#include "Game.h"
#include "buffer.h"
#include <algorithm>
#include <iostream>

#include "lan_eng.h"
#include <asio/ssl.hpp>

extern void MakeSprite(char * FileName, int iStart, short sCount, bool bAlphaEffect);
extern void MakeTileSpr(char * FileName, short sStart, short sCount, bool bAlphaEffect);
extern void MakeEffectSpr(char * FileName, short sStart, short sCount, bool bAlphaEffect);

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
extern int iHungerStatus; // Hunger Bar xRisenx
extern bool m_bStuckHack; // Anti Hack xRisenx


void CGame::UpdateScreen_OnMainMenu()
{
    uint64_t dwTime = G_dwGlobalTime;

    m_iItemDropCnt = 0;
    m_bItemDrop = false;

    if (m_cGameModeCount == 0)
    {
        if (m_pSprite[SPRID_INTERFACE_ND_LOADING] != 0)
        {
            delete m_pSprite[SPRID_INTERFACE_ND_LOADING];
            m_pSprite[SPRID_INTERFACE_ND_LOADING] = 0;
        }

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
    /*if( bActive )*/ //UpdateScreen_OnLoading_Progress();

	std::string progressLabel;
	bool progressComplete = false;
	int progress = 0;

    switch (m_cLoading)
    {
        case 0:
        {
            new_connection_ = std::make_shared<connection>(io_service_, *this, request_handler_, ctx);

			progressLabel = "Loading interface";
            m_pSprite[SPRID_MOUSECURSOR] = CSprite::CreateSprite("interface", 0, false);
            m_pSprite[SPRID_INTERFACE_SPRFONTS] = CSprite::CreateSprite("interface", 1, false);

            m_pSprite[SPRID_INTERFACE_NEWMAPS1] = CSprite::CreateSprite("Newmaps", 0, false);
            m_pSprite[SPRID_INTERFACE_NEWMAPS2] = CSprite::CreateSprite("Newmaps", 1, false);
            m_pSprite[SPRID_INTERFACE_NEWMAPS3] = CSprite::CreateSprite("Newmaps", 2, false);
            m_pSprite[SPRID_INTERFACE_NEWMAPS4] = CSprite::CreateSprite("Newmaps", 3, false);
            m_pSprite[SPRID_INTERFACE_NEWMAPS5] = CSprite::CreateSprite("Newmaps", 4, false);

            m_pSprite[SPRID_INTERFACE_ND_LOGIN] = CSprite::CreateSprite("LoginDialog", 0, false);
      
            m_pSprite[SPRID_INTERFACE_ND_MAINMENU] = CSprite::CreateSprite("New-Dialog", 1, false);
            m_pSprite[SPRID_INTERFACE_ND_QUIT] = CSprite::CreateSprite("New-Dialog", 2, false);

            m_pSprite[SPRID_INTERFACE_ND_GAME1] = CSprite::CreateSprite("GameDialog", 0, false);
            m_pSprite[SPRID_INTERFACE_ND_GAME2] = CSprite::CreateSprite("GameDialog", 1, false);
            m_pSprite[SPRID_INTERFACE_ND_GAME3] = CSprite::CreateSprite("GameDialog", 2, false);
            m_pSprite[SPRID_INTERFACE_ND_GAME4] = CSprite::CreateSprite("GameDialog", 3, false);
            m_pSprite[SPRID_INTERFACE_ND_CRUSADE] = CSprite::CreateSprite("GameDialog", 4, false);
            m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL] = CSprite::CreateSprite("GameDialog", 6, false);
            m_pSprite[SPRID_INTERFACE_ND_INVENTORY] = CSprite::CreateSprite("GameDialog", 7, false);
            m_pSprite[SPRID_INTERFACE_ND_SELECTCHAR] = CSprite::CreateSprite("GameDialog", 8, false);
            m_pSprite[SPRID_INTERFACE_ND_NEWCHAR] = CSprite::CreateSprite("GameDialog", 9, false);
            m_pSprite[SPRID_INTERFACE_ND_NEWEXCHANGE] = CSprite::CreateSprite("GameDialog", 10, false);

            m_pSprite[SPRID_INTERFACE_ND_TEXT] = CSprite::CreateSprite("DialogText", 0, false);
            m_pSprite[SPRID_INTERFACE_ND_BUTTON] = CSprite::CreateSprite("DialogText", 1, false);
            
            MakeSprite("Telescope", SPRID_INTERFACE_GUIDEMAP, 34, false);
            MakeSprite("Telescope2", SPRID_INTERFACE_GUIDEMAP + 35, 4, false);
            MakeSprite("monster", SPRID_INTERFACE_MONSTER, 1, false);
            m_cLoading = 2;
        }
        break;
        case 2:
        {
			progressLabel = "Loading map data";
            MakeTileSpr("maptiles1", 0, 32, true);
            MakeTileSpr("structures1", 50, 20, true);
            MakeTileSpr("Sinside1", 70, 27, false);
            MakeTileSpr("Trees1", 100, 46, true);
            MakeTileSpr("TreeShadows", 150, 46, true);
            MakeTileSpr("objects1", 200, 10, true);
            MakeTileSpr("objects2", 211, 5, true);
            MakeTileSpr("objects3", 216, 4, true);
            MakeTileSpr("objects4", 220, 2, true);
            m_cLoading = 4;
        }
        break;
        case 4:
        {
			progressLabel = "Loading map data.";
            MakeTileSpr("Tile223-225", 223, 3, true);
            MakeTileSpr("Tile226-229", 226, 4, true);
            MakeTileSpr("objects5", 230, 9, true);
            MakeTileSpr("objects6", 238, 4, true);
            MakeTileSpr("objects7", 242, 7, true);
            MakeTileSpr("maptiles2", 300, 15, true);
            MakeTileSpr("maptiles4", 320, 10, true);
            MakeTileSpr("maptiles5", 330, 19, true);
            MakeTileSpr("maptiles6", 349, 4, true);
            MakeTileSpr("maptiles353-361", 353, 9, true);
            MakeTileSpr("Tile363-366", 363, 4, true);
            MakeTileSpr("Tile367-367", 367, 1, true);
            MakeTileSpr("Tile370-381", 370, 12, true);// Tile370~381
            MakeTileSpr("Tile382-387", 382, 6, true);
            MakeTileSpr("Tile388-402", 388, 15, true);
            m_cLoading = 6;
        }
        break;
        case 6:
        {
			progressLabel = "Loading map data..";
            MakeTileSpr("Tile403-405", 403, 3, true);
            MakeTileSpr("Tile406-421", 406, 16, true);
            MakeTileSpr("Tile422-429", 422, 8, true);
            MakeTileSpr("Tile430-443", 430, 14, true);
            MakeTileSpr("Tile444-444", 444, 1, true);
            MakeTileSpr("Tile445-461", 445, 17, true);
            MakeTileSpr("Tile462-473", 462, 12, true);
            MakeTileSpr("Tile474-478", 474, 5, true);
            MakeTileSpr("Tile479-488", 479, 10, true);
            MakeTileSpr("Tile489-522", 489, 34, true);	// Drunken City
            MakeTileSpr("Tile523-530", 523, 8, true);	// Rampart
            MakeTileSpr("Tile531-540", 531, 10, true);	// GodH + Pont
            MakeTileSpr("Tile541-545", 541, 5, true);	// GodH

            MakeSprite("itempack", SPRID_ITEMPACK_PIVOTPOINT + 1, 27, false);
            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + 20] = CSprite::CreateSprite("itempack", 17, false);
            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + 21] = CSprite::CreateSprite("itempack", 18, false);
            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + 22] = CSprite::CreateSprite("itempack", 19, false); // Angels

            MakeSprite("itemground", SPRID_ITEMGROUND_PIVOTPOINT + 1, 19, false);
            m_pSprite[SPRID_ITEMGROUND_PIVOTPOINT + 20] = CSprite::CreateSprite("itemground", 17, false);
            m_pSprite[SPRID_ITEMGROUND_PIVOTPOINT + 21] = CSprite::CreateSprite("itemground", 18, false);
            m_pSprite[SPRID_ITEMGROUND_PIVOTPOINT + 22] = CSprite::CreateSprite("itemground", 19, false);//Angels
            MakeSprite("item-dynamic", SPRID_ITEMDYNAMIC_PIVOTPOINT, 3, false);
            m_cLoading = 8;
        }
        break;
        case 8:
        {
			progressLabel = "Loading items";
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 0] = CSprite::CreateSprite("itemequipM", 0, false);	// body
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 1] = CSprite::CreateSprite("itemequipM", 1, false);	// 1-swords
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 2] = CSprite::CreateSprite("itemequipM", 2, false);	// 2-bows
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 3] = CSprite::CreateSprite("itemequipM", 3, false);	// 3-shields
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 4] = CSprite::CreateSprite("itemequipM", 4, false);	// 4-tunics
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 5] = CSprite::CreateSprite("itemequipM", 5, false);	// 5-shoes
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 7] = CSprite::CreateSprite("itemequipM", 6, false);	// 6-berk
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 8] = CSprite::CreateSprite("itemequipM", 7, false);	// 7-hoses
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 9] = CSprite::CreateSprite("itemequipM", 8, false);	// 8-bodyarmor
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 15] = CSprite::CreateSprite("itemequipM", 11, false); // Axe hammer
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 17] = CSprite::CreateSprite("itemequipM", 12, false); // Wands
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 18] = CSprite::CreateSprite("itemequipM", 9, false);  // hair
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 19] = CSprite::CreateSprite("itemequipM", 10, false); // undies
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 20] = CSprite::CreateSprite("itemequipM", 13, false); // capes
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 21] = CSprite::CreateSprite("itemequipM", 14, false); // helm

            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 16] = CSprite::CreateSprite("itempack", 15); // Necks, Angels, Pendants
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 22] = CSprite::CreateSprite("itempack", 19); // Angels

            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 40] = CSprite::CreateSprite("itemequipW", 0, false); // body
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 41] = CSprite::CreateSprite("itemequipW", 1, false); // 1-swords
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 42] = CSprite::CreateSprite("itemequipW", 2, false); // 2-bows
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 43] = CSprite::CreateSprite("itemequipW", 3, false); // 3-shields
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 45] = CSprite::CreateSprite("itemequipW", 4, false); // 4-shoes
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 50] = CSprite::CreateSprite("itemequipW", 5, false); // 5-Soustif
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 51] = CSprite::CreateSprite("itemequipW", 6, false); // 6 berk
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 52] = CSprite::CreateSprite("itemequipW", 7, false); // 7 hose
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 53] = CSprite::CreateSprite("itemequipW", 8, false); // 8-hoses
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 55] = CSprite::CreateSprite("itemequipW", 11, false); // Axe hammer
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 57] = CSprite::CreateSprite("itemequipW", 12, false); // Wands
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 58] = CSprite::CreateSprite("itemequipW", 9, false); // hair
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 59] = CSprite::CreateSprite("itemequipW", 10, false);// undies
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 60] = CSprite::CreateSprite("itemequipW", 13, false);// capes
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 61] = CSprite::CreateSprite("itemequipW", 14, false);// helm

            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 56] = CSprite::CreateSprite("itempack", 15);// necks, angels, pendants
            m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 62] = CSprite::CreateSprite("itempack", 19); // Angels
            MakeSprite("Bm", 500 + 15 * 8 * 0, 96, true);// Black Man (Type: 1)
            MakeSprite("Wm", 500 + 15 * 8 * 1, 96, true);// White Man (Type: 2)
            MakeSprite("Ym", 500 + 15 * 8 * 2, 96, true);// Yellow Man (Type: 3)
            m_cLoading = 10;
        }
        break;
        case 10:
        {
			progressLabel = "Loading player sprites";
            MakeSprite("TutelarAngel1", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 0, 48, false);//(STR)
            MakeSprite("TutelarAngel2", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 1, 48, false);//(DEX)
            MakeSprite("TutelarAngel3", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 2, 48, false);//(INT)
            MakeSprite("TutelarAngel4", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 3, 48, false);//(MAG)
            MakeSprite("Bw", 500 + 15 * 8 * 3, 96, true);// Black Woman (Type: 4)
            MakeSprite("Ww", 500 + 15 * 8 * 4, 96, true);// White Woman (Type: 5)
            MakeSprite("Yw", 500 + 15 * 8 * 5, 96, true);// Yellow Woman (Type: 6)
            m_cLoading = 12;
        }
        break;
        case 12:
        {
			progressLabel = "Loading NPC sprites";
            MakeSprite("slm", SPRID_MOB + 7 * 8 * 0, 40, true);// Slime (Type: 10)
            MakeSprite("ske", SPRID_MOB + 7 * 8 * 1, 40, true);// Skeleton (Type: 11)
            MakeSprite("Gol", SPRID_MOB + 7 * 8 * 2, 40, true);// Stone-Golem (Type: 12)
            MakeSprite("Cyc", SPRID_MOB + 7 * 8 * 3, 40, true);// Cyclops (Type: 13)
            MakeSprite("Orc", SPRID_MOB + 7 * 8 * 4, 40, true);// Orc (Type: 14)
            MakeSprite("Shopkpr", SPRID_MOB + 7 * 8 * 5, 8, true);		// ShopKeeper-Woman (Type: 15)
            MakeSprite("Ant", SPRID_MOB + 7 * 8 * 6, 40, true);//  Giant-Ant (Type: 16)
            MakeSprite("Scp", SPRID_MOB + 7 * 8 * 7, 40, true);//  Scorpion (Type: 17)
            MakeSprite("Zom", SPRID_MOB + 7 * 8 * 8, 40, true);//  Zombie (Type: 18)
            MakeSprite("Gandlf", SPRID_MOB + 7 * 8 * 9, 8, true);
            MakeSprite("Howard", SPRID_MOB + 7 * 8 * 10, 8, true);
            MakeSprite("Guard", SPRID_MOB + 7 * 8 * 11, 40, true);// Guard (Type: 21)
            MakeSprite("Amp", SPRID_MOB + 7 * 8 * 12, 40, true);// Amphis (Type: 22)
            MakeSprite("Cla", SPRID_MOB + 7 * 8 * 13, 40, true);// Clay-Golem (Type: 23)
            MakeSprite("tom", SPRID_MOB + 7 * 8 * 14, 8, true);// Tom (Type: 24)
            MakeSprite("William", SPRID_MOB + 7 * 8 * 15, 8, true);// William (Type: 25)
            m_cLoading = 14;
        }
        break;
        case 14:
        {
			progressLabel = "Loading NPC sprites.";
            MakeSprite("Kennedy", SPRID_MOB + 7 * 8 * 16, 8, true);// Kennedy (Type: 26)
            MakeSprite("Helb", SPRID_MOB + 7 * 8 * 17, 40, true);// Hellbound (Type: 27)
            MakeSprite("Troll", SPRID_MOB + 7 * 8 * 18, 40, true);// Troll (Type: 28)
            MakeSprite("EnragedTroll", SPRID_MOB + 7 * 8 * 112, 40, true);// Enraged Troll (Type: 122)
            MakeSprite("Orge", SPRID_MOB + 7 * 8 * 19, 40, true);// Orge (Type: 29)
            MakeSprite("Liche", SPRID_MOB + 7 * 8 * 20, 40, true);// Liche (Type: 30)
            MakeSprite("Demon", SPRID_MOB + 7 * 8 * 21, 40, true);// Demon (Type: 31)
            MakeSprite("BlackDemon", SPRID_MOB + 7 * 8 * 95, 40, true);// Black Demon (Type: 105)
            MakeSprite("Unicorn", SPRID_MOB + 7 * 8 * 22, 40, true);// Unicorn (Type: 32)
            MakeSprite("DarkUnicorn", SPRID_MOB + 7 * 8 * 94, 40, true);// DarkUnicorn (Type: 104)
            MakeSprite("WereWolf", SPRID_MOB + 7 * 8 * 23, 40, true);// WereWolf (Type: 33)
            MakeSprite("Dummy", SPRID_MOB + 7 * 8 * 24, 40, true);// Dummy (Type: 34)
            //m_hPakFile = CreateFileA("sprites\\Effect5.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL); // Energy-Ball (Type: 35)
            for (i = 0; i < 40; i++)
                m_pSprite[SPRID_MOB + i + 7 * 8 * 25] = CSprite::CreateSprite("Effect5", 0, true);
            m_cLoading = 16;
        }
        break;
        case 16:
        {
			progressLabel = "Loading NPC sprites...";
            MakeSprite("GT-Arrow", SPRID_MOB + 7 * 8 * 26, 40, true);// Arrow-GuardTower (Type: 36)
            MakeSprite("GT-Cannon", SPRID_MOB + 7 * 8 * 27, 40, true);// Cannon-GuardTower (Type: 37)
            MakeSprite("ManaCollector", SPRID_MOB + 7 * 8 * 28, 40, true);// Mana Collector (Type: 38)
            MakeSprite("Detector", SPRID_MOB + 7 * 8 * 29, 40, true);// Detector (Type: 39)
            MakeSprite("ESG", SPRID_MOB + 7 * 8 * 30, 40, true);// ESG (Type: 40)
            MakeSprite("GMG", SPRID_MOB + 7 * 8 * 31, 40, true);// GMG (Type: 41)
            MakeSprite("ManaStone", SPRID_MOB + 7 * 8 * 32, 40, true);// ManaStone (Type: 42)
            MakeSprite("LWB", SPRID_MOB + 7 * 8 * 33, 40, true);// Light War Beetle (Type: 43)
            MakeSprite("GHK", SPRID_MOB + 7 * 8 * 34, 40, true);// God's Hand Knight (Type: 44)
            MakeSprite("GHKABS", SPRID_MOB + 7 * 8 * 35, 40, true);// God's Hand Knight with Armored Battle Steed (Type: 45)
            MakeSprite("TK", SPRID_MOB + 7 * 8 * 36, 40, true);// Temple Knight (Type: 46)
            MakeSprite("BG", SPRID_MOB + 7 * 8 * 37, 40, true);// Battle Golem (Type: 47)
            m_cLoading = 18;
        }
        break;
        case 18:
        {
			progressLabel = "Loading NPC sprites";
            MakeSprite("Stalker", SPRID_MOB + 7 * 8 * 38, 40, true);// Stalker (Type: 48)
            MakeSprite("Hellclaw", SPRID_MOB + 7 * 8 * 39, 40, true);// Hellclaw (Type: 49)
            MakeSprite("Tigerworm", SPRID_MOB + 7 * 8 * 40, 40, true);// Tigerworm (Type: 50)
            MakeSprite("Catapult", SPRID_MOB + 7 * 8 * 41, 40, true);// Catapult (Type: 51)
            MakeSprite("Gagoyle", SPRID_MOB + 7 * 8 * 42, 40, true);// Gargoyle (Type: 52)
            MakeSprite("EnragedGagoyle", SPRID_MOB + 7 * 8 * 115, 40, true);// Enraged Gagoyle (Type: 125)
            MakeSprite("Beholder", SPRID_MOB + 7 * 8 * 43, 40, true);// Beholder (Type: 53)
            MakeSprite("DarkElf", SPRID_MOB + 7 * 8 * 44, 40, true);// Dark-Elf (Type: 54)
            MakeSprite("Bunny", SPRID_MOB + 7 * 8 * 45, 40, true);// Bunny (Type: 55)
            MakeSprite("Cat", SPRID_MOB + 7 * 8 * 46, 40, true);// Cat (Type: 56)
            MakeSprite("GiantFrog", SPRID_MOB + 7 * 8 * 47, 40, true);// GiantFrog (Type: 57)
            MakeSprite("MTGiant", SPRID_MOB + 7 * 8 * 48, 40, true);// Mountain Giant (Type: 58)
            m_cLoading = 20;
        }
        break;
        case 20:
        {
			progressLabel = "Loading NPC sprites.";
            MakeSprite("Ettin", SPRID_MOB + 7 * 8 * 49, 40, true);// Ettin (Type: 59)
            MakeSprite("CanPlant", SPRID_MOB + 7 * 8 * 50, 40, true);// Cannibal Plant (Type: 60)
            MakeSprite("Rudolph", SPRID_MOB + 7 * 8 * 51, 40, true);// Rudolph (Type: 61)
            MakeSprite("DireBoar", SPRID_MOB + 7 * 8 * 52, 40, true);// Boar (Type: 62)
            MakeSprite("frost", SPRID_MOB + 7 * 8 * 53, 40, true);// Frost (Type: 63)
            MakeSprite("Crop", SPRID_MOB + 7 * 8 * 54, 40, true);// Crop(Type: 64)
            MakeSprite("IceGolem", SPRID_MOB + 7 * 8 * 55, 40, true);// IceGolem (Type: 65)
            MakeSprite("Wyvern", SPRID_MOB + 7 * 8 * 56, 24, true);// Wyvern (Type: 66)
            MakeSprite("McGaffin", SPRID_MOB + 7 * 8 * 57, 16, true);// McGaffin (Type: 67)
            MakeSprite("Perry", SPRID_MOB + 7 * 8 * 58, 16, true);// Perry (Type: 68)
            MakeSprite("Devlin", SPRID_MOB + 7 * 8 * 59, 16, true);// Devlin (Type: 69)
            MakeSprite("Barlog", SPRID_MOB + 7 * 8 * 60, 40, true);// Barlog (Type: 70)
            MakeSprite("Centaurus", SPRID_MOB + 7 * 8 * 61, 40, true);// Centaurus (Type: 71)
            MakeSprite("ClawTurtle", SPRID_MOB + 7 * 8 * 62, 40, true);// Claw-Turtle (Type: 72)
            MakeSprite("FireWyvern", SPRID_MOB + 7 * 8 * 63, 24, true);// Fire-Wyvern (Type: 73)
            MakeSprite("GiantCrayfish", SPRID_MOB + 7 * 8 * 64, 40, true);// Giant-Crayfish (Type: 74)
            MakeSprite("GiantLizard", SPRID_MOB + 7 * 8 * 65, 40, true);// Giant-Lizard (Type: 75)
            MakeSprite("DarkWyvern", SPRID_MOB + 7 * 8 * 98, 24, true);// Dark-Wyvern (Type: 108)
            MakeSprite("EarthWyvern", SPRID_MOB + 7 * 8 * 96, 24, true);// Earth-Wyvern (Type: 106)
            MakeSprite("GrassWyvern", SPRID_MOB + 7 * 8 * 99, 24, true);// Grass-Wyvern (Type: 109)
            MakeSprite("MetalWyvern", SPRID_MOB + 7 * 8 * 102, 24, true);// Metal-Wyvern (Type: 112)
            MakeSprite("SeaWyvern", SPRID_MOB + 7 * 8 * 101, 24, true);// Sea-Wyvern (Type: 111)
            MakeSprite("SkyWyvern", SPRID_MOB + 7 * 8 * 100, 24, true);// Sky-Wyvern (Type: 110)
            MakeSprite("VoidWyvern", SPRID_MOB + 7 * 8 * 103, 24, true);// Void-Wyvern (Type: 113)
            MakeSprite("WindWyvern", SPRID_MOB + 7 * 8 * 97, 24, true);// Wind-Wyvern (Type: 107)
            m_cLoading = 22;
        }
        break;
        case 22:
        {
			progressLabel = "Loading NPC sprites..";
            MakeSprite("GiantPlant", SPRID_MOB + 7 * 8 * 66, 40, true);// Giant-Plant (Type: 76)
            MakeSprite("MasterMageOrc", SPRID_MOB + 7 * 8 * 67, 40, true);// MasterMage-Orc (Type: 77)
            MakeSprite("Minotaurs", SPRID_MOB + 7 * 8 * 68, 40, true);// Minotaurs (Type: 78)
            MakeSprite("Nizie", SPRID_MOB + 7 * 8 * 69, 40, true);// Nizie (Type: 79)
            MakeSprite("Tentocle", SPRID_MOB + 7 * 8 * 70, 40, true);// Tentocle (Type: 80)
            MakeSprite("yspro", SPRID_MOB + 7 * 8 * 71, 32, true);// Abaddon (Type: 81)
            MakeSprite("Sorceress", SPRID_MOB + 7 * 8 * 72, 40, true);// Sorceress (Type: 82)
            MakeSprite("TPKnight", SPRID_MOB + 7 * 8 * 73, 40, true);// TPKnight (Type: 83)
            MakeSprite("ElfMaster", SPRID_MOB + 7 * 8 * 74, 40, true);// ElfMaster (Type: 84)
            MakeSprite("DarkKnight", SPRID_MOB + 7 * 8 * 75, 40, true);// DarkKnight (Type: 85)
            MakeSprite("HBTank", SPRID_MOB + 7 * 8 * 76, 32, true);// HeavyBattleTank (Type: 86)
            MakeSprite("CBTurret", SPRID_MOB + 7 * 8 * 77, 32, true);// CBTurret (Type: 87)
            MakeSprite("Babarian", SPRID_MOB + 7 * 8 * 78, 40, true);// Babarian (Type: 88)
            MakeSprite("ACannon", SPRID_MOB + 7 * 8 * 79, 32, true);// ACannon (Type: 89)
            MakeSprite("EternalDragon", SPRID_MOB + 7 * 8 * 104, 32, true);// Eternal dragon (Type: 114) 
            MakeSprite("EnragedCyclops", SPRID_MOB + 7 * 8 * 113, 40, true);// Enraged Cyclops (Type: 123)
            MakeSprite("EnragedStalker", SPRID_MOB + 7 * 8 * 114, 40, true);// Enraged Stalker (Type: 124)
            MakeSprite("EnragedHellclaw", SPRID_MOB + 7 * 8 * 116, 40, true);// Enraged Hellclaw (Type: 126)
            MakeSprite("EnragedTW", SPRID_MOB + 7 * 8 * 117, 40, true);// Enraged Tigerworm (Type: 127)
            m_cLoading = 24;
        }
        break;
        case 24:
        {
			progressLabel = "Loading NPC sprites...";
            MakeSprite("Gail", SPRID_MOB + 7 * 8 * 80, 8, true); // Gail (Type: 90)
            MakeSprite("Gate", SPRID_MOB + 7 * 8 * 81, 24, true);// Heldenian Gate (Type: 91)/**/
            MakeSprite("Scarecrow", SPRID_MOB + 7 * 8 * 82, 40, true);
            MakeSprite("Princess", SPRID_MOB + 7 * 8 * 92, 8, true);// Princess 102
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 0] = CSprite::CreateSprite("Mpt", i + 12 * 0, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 1] = CSprite::CreateSprite("Mpt", i + 12 * 1, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 2] = CSprite::CreateSprite("Mpt", i + 12 * 2, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 3] = CSprite::CreateSprite("Mpt", i + 12 * 3, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 4] = CSprite::CreateSprite("Mpt", i + 12 * 4, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 5] = CSprite::CreateSprite("Mpt", i + 12 * 5, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 6] = CSprite::CreateSprite("Mpt", i + 12 * 6, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 7] = CSprite::CreateSprite("Mpt", i + 12 * 7, true);
            m_cLoading = 26;
        }
        break;

        case 26:
        {
			progressLabel = "Creating item sprites";
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 0] = CSprite::CreateSprite("Mhr", i + 12 * 0, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 1] = CSprite::CreateSprite("Mhr", i + 12 * 1, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 2] = CSprite::CreateSprite("Mhr", i + 12 * 2, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 3] = CSprite::CreateSprite("Mhr", i + 12 * 3, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 4] = CSprite::CreateSprite("Mhr", i + 12 * 4, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 5] = CSprite::CreateSprite("Mhr", i + 12 * 5, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 6] = CSprite::CreateSprite("Mhr", i + 12 * 6, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 7] = CSprite::CreateSprite("Mhr", i + 12 * 7, true);
            MakeSprite("MLArmor", SPRID_BODYARMOR_M + 15 * 1, 12, true);
            MakeSprite("MCMail", SPRID_BODYARMOR_M + 15 * 2, 12, true);
            MakeSprite("MSMail", SPRID_BODYARMOR_M + 15 * 3, 12, true);
            MakeSprite("MPMail", SPRID_BODYARMOR_M + 15 * 4, 12, true);
            //MakeSprite( "MPMail",	SPRID_BODYARMOR_M + 15*25, 12, TRUE);
            MakeSprite("Mtunic", SPRID_BODYARMOR_M + 15 * 5, 12, true);
            MakeSprite("MRobe1", SPRID_BODYARMOR_M + 15 * 6, 12, true);
            MakeSprite("MSanta", SPRID_BODYARMOR_M + 15 * 7, 12, true);
            MakeSprite("MHPMail1", SPRID_BODYARMOR_M + 15 * 8, 12, true); //hero
            MakeSprite("MHPMail2", SPRID_BODYARMOR_M + 15 * 9, 12, true); //hero
            MakeSprite("MHRobe1", SPRID_BODYARMOR_M + 15 * 10, 12, true); //hero
            MakeSprite("MHRobe2", SPRID_BODYARMOR_M + 15 * 11, 12, true); //hero
            MakeSprite("AncHeroArmorM", SPRID_BODYARMOR_M + 15 * 12, 12, true); //hero // Black Ancient Hero Armor M
            MakeSprite("AncHeroRobeM", SPRID_BODYARMOR_M + 15 * 13, 12, true); //hero // Black Ancient Hero Robe M
            MakeSprite("ALegRobeM", SPRID_BODYARMOR_M + 15 * 14, 12, true); // Aresden Ancient Hero Robe M
            MakeSprite("ELegRobeM", SPRID_BODYARMOR_M + 15 * 15, 12, true); // Elvine Ancient Hero Robe M
            MakeSprite("ALegArmorM", SPRID_BODYARMOR_M + 15 * 16, 12, true); // Aresden Ancient Hero Armor M
            MakeSprite("ELegArmorM", SPRID_BODYARMOR_M + 15 * 17, 12, true); // Elvine Ancient Hero Armor M
            MakeSprite("MShirt", SPRID_BERK_M + 15 * 1, 12, true);
            MakeSprite("MHauberk", SPRID_BERK_M + 15 * 2, 12, true);
            //MakeSprite( "MHauberk",	  SPRID_BERK_M + 15*25, 12, TRUE);
            MakeSprite("MHHauberk1", SPRID_BERK_M + 15 * 3, 12, true);
            MakeSprite("MHHauberk2", SPRID_BERK_M + 15 * 4, 12, true);
            MakeSprite("AncHeroHauberkM", SPRID_BERK_M + 15 * 5, 12, true); // Black Ancient Hero Hauberk M
            MakeSprite("ALegHauberkM", SPRID_BERK_M + 15 * 6, 12, true); // Aresden Ancient Hero Hauberk M
            MakeSprite("ELegHauberkM", SPRID_BERK_M + 15 * 7, 12, true); // Elvine Ancient Hero Hauberk M
            m_cLoading = 28;
        }
        break;
        case 28:
        {
			progressLabel = "Creating item sprites.";
            MakeSprite("MTrouser", SPRID_LEGG_M + 15 * 1, 12, true);
            MakeSprite("MHTrouser", SPRID_LEGG_M + 15 * 2, 12, true);
            MakeSprite("MCHoses", SPRID_LEGG_M + 15 * 3, 12, true);
            MakeSprite("MLeggings", SPRID_LEGG_M + 15 * 4, 12, true);
            //MakeSprite( "MLeggings",SPRID_LEGG_M + 15*25, 12, TRUE);
            MakeSprite("MHLeggings1", SPRID_LEGG_M + 15 * 5, 12, true); // hero
            MakeSprite("MHLeggings2", SPRID_LEGG_M + 15 * 6, 12, true); // hero
            MakeSprite("AncHeroLegM", SPRID_LEGG_M + 15 * 7, 12, true); // hero // Black Ancient Hero Leggings M
            MakeSprite("ALegLeggingsM", SPRID_LEGG_M + 15 * 8, 12, true); // Aresden Ancient Leggings M 
            MakeSprite("ELegLeggingsM", SPRID_LEGG_M + 15 * 9, 12, true); // Elvine Ancient Leggings M

            MakeSprite("MShoes", SPRID_BOOT_M + 15 * 1, 12, true);
            MakeSprite("MLBoots", SPRID_BOOT_M + 15 * 2, 12, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 1] = CSprite::CreateSprite("Msw", i + 56 * 0, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 2] = CSprite::CreateSprite("Msw", i + 56 * 1, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 3] = CSprite::CreateSprite("Msw", i + 56 * 2, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 4] = CSprite::CreateSprite("Msw", i + 56 * 3, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 6] = CSprite::CreateSprite("Msw", i + 56 * 5, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 7] = CSprite::CreateSprite("Msw", i + 56 * 6, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 8] = CSprite::CreateSprite("Msw", i + 56 * 7, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 9] = CSprite::CreateSprite("Msw", i + 56 * 8, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 10] = CSprite::CreateSprite("Msw", i + 56 * 9, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 11] = CSprite::CreateSprite("Msw", i + 56 * 10, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 12] = CSprite::CreateSprite("Msw", i + 56 * 11, true);
            m_cLoading = 30;
        }
        break;
        case 30:
        {
			progressLabel = "Creating item sprites..";
            MakeSprite("Mswx", SPRID_WEAPON_M + 64 * 5, 56, true);
            MakeSprite("Msw2", SPRID_WEAPON_M + 64 * 13, 56, true);
            MakeSprite("Msw3", SPRID_WEAPON_M + 64 * 14, 56, true);
            MakeSprite("MStormBringer", SPRID_WEAPON_M + 64 * 15, 56, true);
            MakeSprite("MDarkExec", SPRID_WEAPON_M + 64 * 16, 56, true);
            MakeSprite("MKlonessBlade", SPRID_WEAPON_M + 64 * 17, 56, true);
            MakeSprite("MKlonessAstock", SPRID_WEAPON_M + 64 * 18, 56, true);
            MakeSprite("MDebastator", SPRID_WEAPON_M + 64 * 19, 56, true);
            MakeSprite("MAxe1", SPRID_WEAPON_M + 64 * 20, 56, true);// Axe
            MakeSprite("MAxe2", SPRID_WEAPON_M + 64 * 21, 56, true);
            MakeSprite("MAxe3", SPRID_WEAPON_M + 64 * 22, 56, true);
            MakeSprite("MAxe4", SPRID_WEAPON_M + 64 * 23, 56, true);
            MakeSprite("MAxe5", SPRID_WEAPON_M + 64 * 24, 56, true);
            MakeSprite("MPickAxe1", SPRID_WEAPON_M + 64 * 25, 56, true);
            MakeSprite("MAxe6", SPRID_WEAPON_M + 64 * 26, 56, true);
            MakeSprite("Mhoe", SPRID_WEAPON_M + 64 * 27, 56, true);
            MakeSprite("MKlonessAxe", SPRID_WEAPON_M + 64 * 28, 56, true);
            MakeSprite("MLightBlade", SPRID_WEAPON_M + 64 * 29, 56, true);
            m_cLoading = 32;
        }
        break;
        case 32:
        {
			progressLabel = "Creating item sprites...";
            MakeSprite("MHammer", SPRID_WEAPON_M + 64 * 30, 56, true);
            MakeSprite("MBHammer", SPRID_WEAPON_M + 64 * 31, 56, true);
            MakeSprite("MBabHammer", SPRID_WEAPON_M + 64 * 32, 56, true);
            MakeSprite("MBShadowSword", SPRID_WEAPON_M + 64 * 33, 56, true);
            MakeSprite("MBerserkWand", SPRID_WEAPON_M + 64 * 34, 56, true);
            MakeSprite("Mstaff1", SPRID_WEAPON_M + 64 * 35, 56, true);// Staff
            MakeSprite("Mstaff2", SPRID_WEAPON_M + 64 * 36, 56, true);
            MakeSprite("MStaff3", SPRID_WEAPON_M + 64 * 37, 56, true);
            MakeSprite("MReMagicWand", SPRID_WEAPON_M + 64 * 38, 56, true);
            MakeSprite("MKlonessWand", SPRID_WEAPON_M + 64 * 39, 56, true);
            MakeSprite("Staff4M", SPRID_WEAPON_M + 64 * 40, 56, true);
            // Bows 40 41 below
            MakeSprite("MDirectBow", SPRID_WEAPON_M + 64 * 43, 56, true);
            MakeSprite("MFireBow", SPRID_WEAPON_M + 64 * 44, 56, true);
            m_cLoading = 34;
        }
        break;
        case 34:
        {
			progressLabel = "Creating item sprites";
            MakeSprite("Mbo", SPRID_WEAPON_M + 64 * 41, 56, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 42] = CSprite::CreateSprite("Mbo", i + 56 * 1, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 1] = CSprite::CreateSprite("Msh", i + 7 * 0, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 2] = CSprite::CreateSprite("Msh", i + 7 * 1, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 3] = CSprite::CreateSprite("Msh", i + 7 * 2, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 4] = CSprite::CreateSprite("Msh", i + 7 * 3, true);
            for (i = 0; i < 7; i++)	m_pSprite[SPRID_SHIELD_M + i + 8 * 5] = CSprite::CreateSprite("Msh", i + 7 * 4, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 6] = CSprite::CreateSprite("Msh", i + 7 * 5, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 7] = CSprite::CreateSprite("Msh", i + 7 * 6, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 8] = CSprite::CreateSprite("Msh", i + 7 * 7, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 9] = CSprite::CreateSprite("Msh", i + 7 * 8, true);
            m_cLoading = 36;
        }
        break;
        case 36:
        {
			progressLabel = "Creating item sprites..";
            MakeSprite("Mmantle01", SPRID_MANTLE_M + 15 * 1, 12, true);
            MakeSprite("Mmantle02", SPRID_MANTLE_M + 15 * 2, 12, true);
            MakeSprite("Mmantle03", SPRID_MANTLE_M + 15 * 3, 12, true);
            MakeSprite("Mmantle04", SPRID_MANTLE_M + 15 * 4, 12, true);
            MakeSprite("Mmantle05", SPRID_MANTLE_M + 15 * 5, 12, true);
            MakeSprite("Mmantle06", SPRID_MANTLE_M + 15 * 6, 12, true);
            MakeSprite("Mmantle07", SPRID_MANTLE_M + 15 * 7, 12, true);
            MakeSprite("Mmantle08", SPRID_MANTLE_M + 15 * 8, 12, true);
            MakeSprite("AAncHeroCapeM", SPRID_MANTLE_M + 15 * 9, 12, true); // Aresden Black Ancient Hero Cape M
            MakeSprite("EAncHeroCapeM", SPRID_MANTLE_M + 15 * 10, 12, true); // Elvine Black Ancient Hero Cape M
            MakeSprite("MHelm1", SPRID_HEAD_M + 15 * 1, 12, true);
            MakeSprite("MHelm2", SPRID_HEAD_M + 15 * 2, 12, true);
            MakeSprite("MHelm3", SPRID_HEAD_M + 15 * 3, 12, true);
            MakeSprite("MHelm4", SPRID_HEAD_M + 15 * 4, 12, true);
            MakeSprite("NMHelm1", SPRID_HEAD_M + 15 * 5, 12, true);
            //MakeSprite( "NMHelm1", SPRID_HEAD_M + 15*25, 12, TRUE);
            MakeSprite("NMHelm2", SPRID_HEAD_M + 15 * 6, 12, true);
            MakeSprite("NMHelm3", SPRID_HEAD_M + 15 * 7, 12, true);
            MakeSprite("NMHelm4", SPRID_HEAD_M + 15 * 8, 12, true);
            MakeSprite("MHHelm1", SPRID_HEAD_M + 15 * 9, 12, true);
            MakeSprite("MHHelm2", SPRID_HEAD_M + 15 * 10, 12, true);
            MakeSprite("MHCap1", SPRID_HEAD_M + 15 * 11, 12, true);
            MakeSprite("MHCap2", SPRID_HEAD_M + 15 * 12, 12, true);
            MakeSprite("AncHeroHelmM", SPRID_HEAD_M + 15 * 13, 12, true); // Black Ancient Hero Helm M
            MakeSprite("AncHeroCapM", SPRID_HEAD_M + 15 * 14, 12, true); // Black Ancient Hero Cap M
            MakeSprite("ALegHelmM", SPRID_HEAD_M + 15 * 15, 12, true); // Aresden Ancient Helm M
            MakeSprite("ELegHelmM", SPRID_HEAD_M + 15 * 16, 12, true); // Elvine Ancient Helm M
            MakeSprite("ALegCapM", SPRID_HEAD_M + 15 * 17, 12, true); // Aresden Ancient Cap M
            MakeSprite("ELegCapM", SPRID_HEAD_M + 15 * 18, 12, true); // Elvine Ancient Cap M
            m_cLoading = 38;
        }
        break;
        case 38:
        {
			progressLabel = "Creating item sprites...";
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 0] = CSprite::CreateSprite("Wpt", i, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 1] = CSprite::CreateSprite("Wpt", i + 12, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 2] = CSprite::CreateSprite("Wpt", i + 12 * 2, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 3] = CSprite::CreateSprite("Wpt", i + 12 * 3, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 4] = CSprite::CreateSprite("Wpt", i + 12 * 4, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 5] = CSprite::CreateSprite("Wpt", i + 12 * 5, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 6] = CSprite::CreateSprite("Wpt", i + 12 * 6, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 7] = CSprite::CreateSprite("Wpt", i + 12 * 7, true);

            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 0] = CSprite::CreateSprite("Whr", i + 0, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 1] = CSprite::CreateSprite("Whr", i + 12, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 2] = CSprite::CreateSprite("Whr", i + 12 * 2, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 3] = CSprite::CreateSprite("Whr", i + 12 * 3, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 4] = CSprite::CreateSprite("Whr", i + 12 * 4, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 5] = CSprite::CreateSprite("Whr", i + 12 * 5, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 6] = CSprite::CreateSprite("Whr", i + 12 * 6, true);
            for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 7] = CSprite::CreateSprite("Whr", i + 12 * 7, true);
            m_cLoading = 40;
        }
        break;
        case 40:
        {
			progressLabel = "Creating item sprites";
            MakeSprite("WBodice1", SPRID_BODYARMOR_W + 15 * 1, 12, true);
            MakeSprite("WBodice2", SPRID_BODYARMOR_W + 15 * 2, 12, true);
            MakeSprite("WLArmor", SPRID_BODYARMOR_W + 15 * 3, 12, true);
            MakeSprite("WCMail", SPRID_BODYARMOR_W + 15 * 4, 12, true);
            MakeSprite("WSMail", SPRID_BODYARMOR_W + 15 * 5, 12, true);
            MakeSprite("WPMail", SPRID_BODYARMOR_W + 15 * 6, 12, true);
            MakeSprite("WRobe1", SPRID_BODYARMOR_W + 15 * 7, 12, true);
            MakeSprite("WSanta", SPRID_BODYARMOR_W + 15 * 8, 12, true);
            MakeSprite("WHPMail1", SPRID_BODYARMOR_W + 15 * 9, 12, true); //hero
            MakeSprite("WHPMail2", SPRID_BODYARMOR_W + 15 * 10, 12, true); //hero
            MakeSprite("WHRobe1", SPRID_BODYARMOR_W + 15 * 11, 12, true); // hero
            MakeSprite("WHRobe2", SPRID_BODYARMOR_W + 15 * 12, 12, true); // hero
            MakeSprite("AncHeroArmorW", SPRID_BODYARMOR_W + 15 * 13, 12, true); //hero // Black Ancient Hero Armor W
            MakeSprite("AncHeroRobeW", SPRID_BODYARMOR_W + 15 * 14, 12, true); // hero // Black Ancient Hero Robe W
            MakeSprite("ALegRobeW", SPRID_BODYARMOR_W + 15 * 15, 12, true); // Aresden Ancient Robe W
            MakeSprite("ELegRobeW", SPRID_BODYARMOR_W + 15 * 16, 12, true); // Elvine Ancient Robe W
            MakeSprite("ALegArmorW", SPRID_BODYARMOR_W + 15 * 17, 12, true); // Aresden Ancient Armor W
            MakeSprite("ELegArmorW", SPRID_BODYARMOR_W + 15 * 18, 12, true); // Elvine Ancient Armor W

            MakeSprite("WChemiss", SPRID_BERK_W + 15 * 1, 12, true);
            MakeSprite("WShirt", SPRID_BERK_W + 15 * 2, 12, true);
            MakeSprite("WHauberk", SPRID_BERK_W + 15 * 3, 12, true);
            MakeSprite("WHHauberk1", SPRID_BERK_W + 15 * 4, 12, true);
            MakeSprite("WHHauberk2", SPRID_BERK_W + 15 * 5, 12, true);
            MakeSprite("AncHeroHauberkW", SPRID_BERK_W + 15 * 6, 12, true); // Black Ancient Hero Hauberk W
            MakeSprite("ALegHauberkW", SPRID_BERK_W + 15 * 7, 12, true); // Aresden Ancient Hauberk W
            MakeSprite("ELegHauberkW", SPRID_BERK_W + 15 * 8, 12, true); // Elvine Ancient Hauberk W
            MakeSprite("WSkirt", SPRID_LEGG_W + 15 * 1, 12, true);
            MakeSprite("WTrouser", SPRID_LEGG_W + 15 * 2, 12, true);
            MakeSprite("WHTrouser", SPRID_LEGG_W + 15 * 3, 12, true);
            MakeSprite("WCHoses", SPRID_LEGG_W + 15 * 4, 12, true);
            MakeSprite("WLeggings", SPRID_LEGG_W + 15 * 5, 12, true);
            MakeSprite("WHLeggings1", SPRID_LEGG_W + 15 * 6, 12, true);
            MakeSprite("WHLeggings2", SPRID_LEGG_W + 15 * 7, 12, true);
            MakeSprite("AncHeroLegW", SPRID_LEGG_W + 15 * 8, 12, true); // Black Ancient Hero Leggings W
            MakeSprite("ALegLeggingsW", SPRID_LEGG_W + 15 * 9, 12, true); // Aresden Ancient Leggings W
            MakeSprite("ELegLeggingsW", SPRID_LEGG_W + 15 * 10, 12, true); // Elvine Ancient Leggings W
            MakeSprite("WShoes", SPRID_BOOT_W + 15 * 1, 12, true);
            MakeSprite("WLBoots", SPRID_BOOT_W + 15 * 2, 12, true);
            m_cLoading = 42;
        }
        break;
        case 42:
        {
			progressLabel = "Creating item sprites.";
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 1] = CSprite::CreateSprite("Wsw", i + 56 * 0, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 2] = CSprite::CreateSprite("Wsw", i + 56 * 1, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 3] = CSprite::CreateSprite("Wsw", i + 56 * 2, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 4] = CSprite::CreateSprite("Wsw", i + 56 * 3, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 6] = CSprite::CreateSprite("Wsw", i + 56 * 5, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 7] = CSprite::CreateSprite("Wsw", i + 56 * 6, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 8] = CSprite::CreateSprite("Wsw", i + 56 * 7, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 9] = CSprite::CreateSprite("Wsw", i + 56 * 8, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 10] = CSprite::CreateSprite("Wsw", i + 56 * 9, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 11] = CSprite::CreateSprite("Wsw", i + 56 * 10, true);
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 12] = CSprite::CreateSprite("Wsw", i + 56 * 11, true);
            MakeSprite("Wswx", SPRID_WEAPON_W + 64 * 5, 56, true);
            MakeSprite("Wsw2", SPRID_WEAPON_W + 64 * 13, 56, true);
            MakeSprite("Wsw3", SPRID_WEAPON_W + 64 * 14, 56, true); // TheVampire
            MakeSprite("WStormBringer", SPRID_WEAPON_W + 64 * 15, 56, true);
            MakeSprite("WDarkExec", SPRID_WEAPON_W + 64 * 16, 56, true);
            MakeSprite("WKlonessBlade", SPRID_WEAPON_W + 64 * 17, 56, true);
            MakeSprite("WKlonessAstock", SPRID_WEAPON_W + 64 * 18, 56, true);
            MakeSprite("WDebastator", SPRID_WEAPON_W + 64 * 19, 56, true);
            m_cLoading = 44;
        }
        break;
        case 44:
        {
			progressLabel = "Creating item sprites..";
            MakeSprite("WAxe1", SPRID_WEAPON_W + 64 * 20, 56, true);// Axe
            MakeSprite("WAxe2", SPRID_WEAPON_W + 64 * 21, 56, true);
            MakeSprite("WAxe3", SPRID_WEAPON_W + 64 * 22, 56, true);
            MakeSprite("WAxe4", SPRID_WEAPON_W + 64 * 23, 56, true);
            MakeSprite("WAxe5", SPRID_WEAPON_W + 64 * 24, 56, true);
            MakeSprite("WpickAxe1", SPRID_WEAPON_W + 64 * 25, 56, true);
            MakeSprite("WAxe6", SPRID_WEAPON_W + 64 * 26, 56, true);
            MakeSprite("Whoe", SPRID_WEAPON_W + 64 * 27, 56, true);
            MakeSprite("WKlonessAxe", SPRID_WEAPON_W + 64 * 28, 56, true);
            MakeSprite("WLightBlade", SPRID_WEAPON_W + 64 * 29, 56, true);
            MakeSprite("WHammer", SPRID_WEAPON_W + 64 * 30, 56, true);
            MakeSprite("WBHammer", SPRID_WEAPON_W + 64 * 31, 56, true);
            MakeSprite("WBabHammer", SPRID_WEAPON_W + 64 * 32, 56, true);
            MakeSprite("WBShadowSword", SPRID_WEAPON_W + 64 * 33, 56, true);
            MakeSprite("WBerserkWand", SPRID_WEAPON_W + 64 * 34, 56, true);
            MakeSprite("Wstaff1", SPRID_WEAPON_W + 64 * 35, 56, true);
            MakeSprite("Wstaff2", SPRID_WEAPON_W + 64 * 36, 56, true);
            MakeSprite("WStaff3", SPRID_WEAPON_W + 64 * 37, 56, true);
            MakeSprite("WKlonessWand", SPRID_WEAPON_W + 64 * 39, 56, true);
            MakeSprite("WReMagicWand", SPRID_WEAPON_W + 64 * 38, 56, true);
            MakeSprite("Staff4W", SPRID_WEAPON_W + 64 * 40, 56, true);
            // bows 40 41 below
            MakeSprite("WDirectBow", SPRID_WEAPON_W + 64 * 43, 56, true);
            MakeSprite("WFireBow", SPRID_WEAPON_W + 64 * 44, 56, true);
            m_cLoading = 46;
        }
        break;
		case 46:
        {
			progressLabel = "Creating item sprites...";
            MakeSprite("Wmantle01", SPRID_MANTLE_W + 15 * 1, 12, true);
            MakeSprite("Wmantle02", SPRID_MANTLE_W + 15 * 2, 12, true);
            MakeSprite("Wmantle03", SPRID_MANTLE_W + 15 * 3, 12, true);
            MakeSprite("Wmantle04", SPRID_MANTLE_W + 15 * 4, 12, true);
            MakeSprite("Wmantle05", SPRID_MANTLE_W + 15 * 5, 12, true);
            MakeSprite("Wmantle06", SPRID_MANTLE_W + 15 * 6, 12, true);
            MakeSprite("Wmantle07", SPRID_MANTLE_W + 15 * 7, 12, true);
            MakeSprite("Wmantle08", SPRID_MANTLE_W + 15 * 8, 12, true);
            MakeSprite("AAncHeroCapeW", SPRID_MANTLE_W + 15 * 9, 12, true); // Aresden Black Ancient Hero Cape W
            MakeSprite("EAncHeroCapeW", SPRID_MANTLE_W + 15 * 10, 12, true); // Elvine Black Ancient Hero Cape W
            MakeSprite("WHelm1", SPRID_HEAD_W + 15 * 1, 12, true);
            MakeSprite("WHelm4", SPRID_HEAD_W + 15 * 4, 12, true);
            MakeSprite("NWHelm1", SPRID_HEAD_W + 15 * 5, 12, true);
            MakeSprite("NWHelm2", SPRID_HEAD_W + 15 * 6, 12, true);
            MakeSprite("NWHelm3", SPRID_HEAD_W + 15 * 7, 12, true);
            MakeSprite("NWHelm4", SPRID_HEAD_W + 15 * 8, 12, true);
            MakeSprite("WHHelm1", SPRID_HEAD_W + 15 * 9, 12, true);
            MakeSprite("WHHelm2", SPRID_HEAD_W + 15 * 10, 12, true);
            MakeSprite("WHCap1", SPRID_HEAD_W + 15 * 11, 12, true);
            MakeSprite("WHCap2", SPRID_HEAD_W + 15 * 12, 12, true);
            MakeSprite("AncHeroHelmW", SPRID_HEAD_W + 15 * 13, 12, true); // Black Ancient Hero Helm W
            MakeSprite("AncHeroCapW", SPRID_HEAD_W + 15 * 14, 12, true); // Black Ancient Hero Cap W
            MakeSprite("ALegHelmW", SPRID_HEAD_W + 15 * 15, 12, true); // Aresden Ancient Helm M
            MakeSprite("ELegHelmW", SPRID_HEAD_W + 15 * 16, 12, true); // Elvine Ancient Helm M
            MakeSprite("ALegCapW", SPRID_HEAD_W + 15 * 17, 12, true); // Aresden Ancient Cap M
            MakeSprite("ELegCapW", SPRID_HEAD_W + 15 * 18, 12, true); // Elvine Ancient Cap M
            m_cLoading = 48;
        }
        break;
        case 48:
        {
			progressLabel = "Creating item sprites";
            MakeSprite("Wbo", SPRID_WEAPON_W + 64 * 41, 56, true);// Bow
            for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 42] = CSprite::CreateSprite("Wbo", i + 56 * 1, true);
            
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 1] = CSprite::CreateSprite("Wsh", i + 7 * 0, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 2] = CSprite::CreateSprite("Wsh", i + 7 * 1, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 3] = CSprite::CreateSprite("Wsh", i + 7 * 2, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 4] = CSprite::CreateSprite("Wsh", i + 7 * 3, true);
            for (i = 0; i < 7; i++)	m_pSprite[SPRID_SHIELD_W + i + 8 * 5] = CSprite::CreateSprite("Wsh", i + 7 * 4, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 6] = CSprite::CreateSprite("Wsh", i + 7 * 5, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 7] = CSprite::CreateSprite("Wsh", i + 7 * 6, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 8] = CSprite::CreateSprite("Wsh", i + 7 * 7, true);
            for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 9] = CSprite::CreateSprite("Wsh", i + 7 * 8, true);
            m_cLoading = 50;
        }
        break;
        case 50:
		{
			progressLabel = "Creating effects";
			MakeEffectSpr("effect", 0, 10, false);
			MakeEffectSpr("effect2", 10, 3, false);
			MakeEffectSpr("effect3", 13, 6, false);
			MakeEffectSpr("effect4", 19, 5, false);
			for (i = 0; i <= 6; i++)
				m_pEffectSpr[i + 24] = CSprite::CreateSprite("effect5", i + 1, false);
			MakeEffectSpr("CruEffect1", 31, 9, false);
			MakeEffectSpr("effect6", 40, 5, false);
			MakeEffectSpr("effect7", 45, 12, false);
			MakeEffectSpr("effect8", 57, 9, false);
			MakeEffectSpr("effect9", 66, 21, false);

			MakeEffectSpr("effect10", 87, 2, false); // Effects Hero items
			MakeEffectSpr("effect11", 89, 14, false); // Cancel, stormBlade, resu, GateHeldenian....etc
			MakeEffectSpr("effect11s", 104, 1, false);
			MakeEffectSpr("yseffect2", 140, 8, false); // Abaddon's death
			MakeEffectSpr("effect12", 148, 4, false); // Slates auras
			MakeEffectSpr("yseffect3", 152, 16, false);
			MakeEffectSpr("yseffect4", 133, 7, false); // Abaddon's map thunder.
			MakeEffectSpr("effects", 168, 1, false); // minimap ping
			m_cLoading = 52;
		}
		break;
		case 52: 
		{
			progressLabel = "Loading sounds";
			if (m_bSoundFlag)
			{
				for (i = 1; i <= 8; i++)
				{
					sprintf(G_cTxt, "data\\sounds\\C%d.wav", i);
					CSoundBuffer[i].loadFromFile(G_cTxt);
					m_pCSound[i].setBuffer(CSoundBuffer[i]);
					progress = m_cLoading + (int)(i / 2);
				}
			}
			m_cLoading = 60;
		}
		break;
		case 60:
		{
			progressLabel = "Loading sounds.";
			if (m_bSoundFlag)
			{
				for (i = 1; i <= 50; i++)
				{
					sprintf(G_cTxt, "data\\sounds\\M%d.wav", i);
					MSoundBuffer[i].loadFromFile(G_cTxt);
					m_pMSound[i].setBuffer(MSoundBuffer[i]);
					progress = m_cLoading + (int)(i / 5);
				}
			}
			m_cLoading = 68;
		}
		break;
		case 68:
		{
			progressLabel = "Loading sounds..";
			if (m_bSoundFlag)
			{
				for (i = 1; i <= 15; i++)
				{
					sprintf(G_cTxt, "data\\sounds\\E%d.wav", i);
					ESoundBuffer[i].loadFromFile(G_cTxt);
					m_pESound[i].setBuffer(ESoundBuffer[i]);
					progress = m_cLoading + (int)(i / 6);
				}
			}
            m_cLoading = 76;
		}
		break;
        case 76:
        {
            progressLabel = "Loading sounds";
            if (m_bSoundFlag)
            {
                for (i = 9; i <= 16; i++)
                {
                    sprintf(G_cTxt, "data\\sounds\\C%d.wav", i);
                    CSoundBuffer[i].loadFromFile(G_cTxt);
                    m_pCSound[i].setBuffer(CSoundBuffer[i]);
                    progress = m_cLoading + (int)(i / 2);
                }
            }
            m_cLoading = 84;
        }
        break;
        case 84:
        {
            progressLabel = "Loading sounds.";
            if (m_bSoundFlag)
            {
                for (i = 51; i <= 100; i++)
                {
                    sprintf(G_cTxt, "data\\sounds\\M%d.wav", i);
                    MSoundBuffer[i].loadFromFile(G_cTxt);
                    m_pMSound[i].setBuffer(MSoundBuffer[i]);
                    progress = m_cLoading + (int)(i / 5);
                }
            }
            m_cLoading = 88;
        }
        break;
        case 88:
        {
            progressLabel = "Loading sounds..";
            if (m_bSoundFlag)
            {
                for (i = 16; i <= 30; i++)
                {
                    sprintf(G_cTxt, "data\\sounds\\E%d.wav", i);
                    ESoundBuffer[i].loadFromFile(G_cTxt);
                    m_pESound[i].setBuffer(ESoundBuffer[i]);
                    progress = m_cLoading + (int)(i / 6);
                }
            }
            m_cLoading = 92;
        }
        break;
        case 92:
        {
            progressLabel = "Loading sounds";
            if (m_bSoundFlag)
            {
                for (i = 17; i <= 24; i++)
                {
                    sprintf(G_cTxt, "data\\sounds\\C%d.wav", i);
                    CSoundBuffer[i].loadFromFile(G_cTxt);
                    m_pCSound[i].setBuffer(CSoundBuffer[i]);
                    progress = m_cLoading + (int)(i / 2);
                }
            }
            m_cLoading = 95;
        }
        break;
        case 95:
        {
            progressLabel = "Loading sounds.";
            if (m_bSoundFlag)
            {
                for (i = 101; i <= 156; i++)
                {
                    sprintf(G_cTxt, "data\\sounds\\M%d.wav", i);
                    MSoundBuffer[i].loadFromFile(G_cTxt);
                    m_pMSound[i].setBuffer(MSoundBuffer[i]);
                    progress = m_cLoading + (int)(i / 5);
                }
            }
            m_cLoading = 98;
        }
        break;
        case 98:
        {
            progressLabel = "Loading sounds..";
            if (m_bSoundFlag)
            {
                for (i = 31; i <= 54; i++)
                {
                    sprintf(G_cTxt, "data\\sounds\\E%d.wav", i);
                    ESoundBuffer[i].loadFromFile(G_cTxt);
                    m_pESound[i].setBuffer(ESoundBuffer[i]);
                    progress = m_cLoading + (int)(i / 6);
                }
            }
            m_cLoading = 100;
        }
        break;
		case 100:
		{
			progressLabel = "Finalizing";
            isItemLoaded = false;
            //ChangeGameMode(GAMEMODE_ONMAINMENU);

			// Let the UI know we're done loading
            if (autologin)
            {
                ChangeGameMode(GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_LOGIN;
                ZeroMemory(m_cMsg, sizeof(m_cMsg));
                strcpy(m_cMsg, "11");

                asio::ip::tcp::endpoint endpoint(asio::ip::make_address_v4(m_cLogServerAddr), m_iLogServerPort);
                new_connection_->socket().async_connect(endpoint,
                                                        std::bind(&CGame::handle_connect, this,
                                                                    std::placeholders::_1));
                break;
            }
            ChangeGameMode(oldmode);
        }
        break;
    }

	// Update the UI with the loading progress
	JSObject obj;
	progress = !progress ? m_cLoading : progress;
	bool async = !(m_cLoading == 52 || m_cLoading == 60 || m_cLoading == 92 || m_cLoading == 100);
	obj.SetProperty(WSLit("progress"), JSValue(progress));
	obj.SetProperty(WSLit("progressLabel"), WSLit(progressLabel.c_str()));
	obj.SetProperty(WSLit("complete"), JSValue(m_cLoading == 100 ? true : false));
	htmlUI->EmitObject("progressUpdate", obj, async);

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
    uint64_t dwTime;
    static uint64_t dwCTime;

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
    }

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
    uint64_t dwTime = unixtime();
    static uint64_t dwMTime, dwCTime;

    if (m_cGameModeCount == 0)
    {
        m_bEnterPressed = false;
        m_bEscPressed = false;
        dwCTime = dwMTime = unixtime();

        if ((_socket != nullptr) && (_socket->handshake_complete))
        {
            printf("UpdateScreen_OnConnecting()");
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
            break;

        case '2':
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
    uint64_t dwTime = unixtime();

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
    static uint64_t dwTime;

    msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


    if (m_cGameModeCount == 0)
    {
		dwTime = unixtime();
		if (m_bSoundFlag) m_pESound[38].stop();
		if ((m_bSoundFlag) && (m_bMusicStat == TRUE))
		{
			m_pBGM.stop();
		}
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162 + 80, 125 + 80, 2);
    PutString_SprFont(172 + 54 + 80, 180 + 80, "Connection Lost!", 7, 0, 0);
    PutString(172 + 50 + 80, 180 + 30 + 80, UPDATE_SCREEN_ON_CONNECTION_LOST, Color(0, 0, 0));
    DrawVersion();
    m_stMCursor.sCursorFrame = 0;


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

void CGame::UpdateScreen_OnQuit()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    char cMIresult;
    int  iMIbuttonNum;

    uint64_t dwTime = unixtime();

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

    if (m_bEscPressed == true || m_bEnterPressed == true)
    {
        m_bEscPressed = false;
        m_bEnterPressed = false;
        ChangeGameMode(GAMEMODE_NULL);
        io_service_.stop();
        window.close();
        return;
    }
    DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 0, 0, 0, true);
    DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 328, 83, 1, true);

    DrawVersion(true);
    m_stMCursor.sCursorFrame = 0;

    if ((cMIresult) && (iMIbuttonNum == 1))
    {
        ChangeGameMode(GAMEMODE_NULL);
        io_service_.stop();
        window.close();
        return;
    }
}

void CGame::UpdateScreen_OnSelectCharacter(short sX, short sY, short msX, short msY, bool bIgnoreFocus)
{
    int iYear, iMonth, iDay, iHour, iMinute;
    __int64 iTemp1, iTemp2;
    char cTotalChar = 0;
    uint64_t dwTime = unixtime();
    sY = 10;

    iTemp1 = 0;
    iTemp2 = 0;
    iYear = iMonth = iDay = iHour = iMinute = 0;

    if (selectedchar)

    {
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
        _tmp_cAction = OBJECTMOVE;
        _tmp_cFrame = m_cMenuFrame;

        if (selectedchar->m_sSex != 0)
        {
            if (m_Misc.bCheckValidString(selectedchar->m_cName.c_str()) == true)
            {
                m_pEffectSpr[0]->PutTransSprite(sX + 36, sY + 50 + 28, 1, dwTime);
                setRenderTarget(DS_CS, true, Color(0, 0, 0, 0));
                DrawObject_OnMove_ForMenu(0, 0, sX + 36, sY + 50 + 28, false, dwTime, 0, 0);
                charselect.display();
                setRenderTarget(DS_VISIBLE);
                int	_sLevel = selectedchar->m_sLevel;
                sprintf(G_cTxt, "Name: %s", selectedchar->m_cName.c_str());
                PutString(sX + 46 + 96, sY + 160, G_cTxt, Color(255, 255, 255));//25,35,25);	// sY = 14

                sprintf(G_cTxt, "Level: %d", _sLevel);
                PutString(sX + 46 + 96, sY + 160 + 17, G_cTxt, Color(255, 255, 255)); //25,35,25);

                sprintf(G_cTxt, "%I64d", selectedchar->m_iExp);//TODO: won't be here for ui eventually
                PutString(sX - 10 + 96, sY + 179 - 9 + 118 + 19 - 2, G_cTxt, Color(255, 255, 255)); //25,35,25);
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

    if (iYear != 0)
    {
        wsprintfA(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER43, iYear, iMonth, iDay, iHour, iMinute);
        PutAlignedString(98 + 80 - 40, 357 + 80 - 40, 415 + 10 + 120 - 40, G_cTxt);
    }

    if (strcmp(m_cWorldServerName, NAME_WORLDNAME1) == 0)
        PutAlignedString(129 + 80, 321 + 80, 456 + 120, MSG_WORLDNAME1); //"ABADDON Server"
    else if (strcmp(m_cWorldServerName, "WS2") == 0)
        PutAlignedString(129, 321, 456, MSG_WORLDNAME2);//"APOCALYPSE Server"
}

void CGame::UpdateScreen_OnLogResMsg()
{
    short msX, msY, msZ, sX, sY;
    char  cLB, cRB, cMB;
    uint64_t dwTime = unixtime();
    static uint64_t dwCTime;
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

            break;

        case '2':
        case '4':
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
            PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, "�I�ƨϥδ����w����, �Ц�GD2S.gamania.com�����ϥδ���");

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

void CGame::UpdateScreen_OnGame()
{
    lock_guard<std::mutex> lock(screenupdate);

    short sVal, sDivX, sModX, sDivY, sModY, sPivotX, sPivotY, sVPXsave, sVPYsave;
    static int  iUpdateRet = -1;
    static uint32_t perfVar = 0, perfCnt = 0;
    short absX, absY, tX, tY;
    uint32_t ItemColor;
    int  i, iAmount;
    uint64_t dwTime = unixtime();
    static uint64_t dwPrevChatTime = 0;
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
                                               // hum, d�j� affich�? , j'attends le retour et je d�sactive!
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
                     m_dialogBoxes[DIALOG_GUILD].GetMode() == 10)
            {
            }
            else if (iGetTopDialogBoxIndex() == DIALOG_MAILBOX && m_bIsDialogEnabled[DIALOG_MAILBOX] &&
                     m_dialogBoxes[DIALOG_MAILBOX].GetMode() >= 1 && m_dialogBoxes[DIALOG_MAILBOX].GetMode() < 5)
            {
            }
            else
            {
//                 if (!m_bInputStatus)
//                 {
//                     switch (m_cBackupChatMsg[0])
//                     {
//                         case '!':
//                         case '@':
//                         case '#':
//                         case '$':
//                         case '^':
//                             ZeroMemory(m_cChatMsg, sizeof(m_cChatMsg));
//                             m_cChatMsg[0] = m_cBackupChatMsg[0];
//                             //StartInputString(10, 414, sizeof(m_cChatMsg), m_cChatMsg);
//                             StartInputString(10, 530, sizeof(m_cChatMsg), m_cChatMsg); // 800x600 Resolution xRisenx 534 is right / 530 fits?
//                             break;
//                         default:
//                             //StartInputString(10, 414, sizeof(m_cChatMsg), m_cChatMsg);
//                             StartInputString(10, 530, sizeof(m_cChatMsg), m_cChatMsg); // 800x600 Resolution xRisenx 534 is right / 530 fits?
//                             ClearInputString();
//                             break;
//                     }
//                 }
//                 else
//                 {
//                     EndInputString();
//                     ZeroMemory(G_cTxt, sizeof(G_cTxt));
//                     ReceiveString((char *)G_cTxt);
//                     ZeroMemory(m_cBackupChatMsg, sizeof(m_cBackupChatMsg));
//                     strcpy(m_cBackupChatMsg, G_cTxt);
//                     if ((G_cTxt[0] != '!' && m_dwCurTime - dwPrevChatTime > 400) ||
//                         (G_cTxt[0] == '!' && m_dwCurTime - dwPrevChatTime > 700))
//                     {
//                         dwPrevChatTime = m_dwCurTime;
//                         m_curse.ConvertString(G_cTxt, strlen(G_cTxt));
//                         if (strlen(G_cTxt) > 0)
//                         {
//                             if ((G_cTxt[0] == '!') || (G_cTxt[0] == '~'))
//                             {
//                                 if (m_Misc.bCheckIMEString(G_cTxt) == false) return;
//                             }
//                             bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, G_cTxt);
// 
//                         }
//                     }
//                 }
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

    bg.display();
    const Texture & t = bg.getTexture();
    sf::Sprite sprite = sf::Sprite(t);
    sprite.setPosition(0 - sModX, 0 - sModY);
    visible.draw(sprite);



    if (iUpdateRet != 0)
        DrawEffectLights();
    if (iUpdateRet != 0)
        DrawObjects(sPivotX, sPivotY, sDivX, sDivY, sModX, sModY, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);



    if (m_showGrid)
    {
        int16_t ix, iy, iyMax;
        int16_t indexY = sDivY + m_pMapData->m_sPivotY;
        for (iy = -sModY; iy < GetHeight() + 32; iy += 32)
        {
            int16_t indexX = sDivX + m_pMapData->m_sPivotX;
            for (ix = -sModX; ix < GetWidth() + 32; ix += 32)
            {
                {
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(ix - 16, iy - 16), Color(127, 127, 0, 127)),
                        sf::Vertex(sf::Vector2f(ix - 16, iy + 16), Color(127, 127, 0, 127)) };
                    visible.draw(line, 2, sf::Lines);
                }
                {
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(ix - 16, iy - 16), Color(127, 127, 0, 127)),
                        sf::Vertex(sf::Vector2f(ix + 16, iy - 16), Color(127, 127, 0, 127)) };
                    visible.draw(line, 2, sf::Lines);
                }

//                 char text[20];
//                 sprintf(text, "(%d,%d)\n%d-%d", (m_sViewPointX + ix) / 32, (m_sViewPointY + iy) / 32, m_pMapData->m_tile[indexX][indexY].m_sObjectSprite, m_pMapData->m_tile[indexX][indexY].m_sObjectSpriteFrame);
// 
//                 _text.setFont(_font.at("default"));
//                 _text.setString(text);
//                 _text.setFillColor(Color(255, 0, 0, 180));
//                 _text.setPosition(ix - 16, iy - 8);
//                 _text.setOutlineColor(Color(0, 0, 0, 128));
//                 _text.setOutlineThickness(1);
//                 _text.setCharacterSize(10);
//                 visible.draw(_text);
                indexX++;
            }
            indexY++;
        }
    }

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

    if ((iUpdateRet != 0))
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
//         else //DIRECTX m_DDraw.DrawShadowBox(0, 529, 799, 546); // 800x600 Resolution xRisenx
//             ShowReceivedString();
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

        PutString(m_stMCursor.sX, m_stMCursor.sY + 25, iter->c_str(), m_itemColor, false, 1);
        iLoc += 13;
        ++iter;

        while (iter != end)
        {
            PutString(m_stMCursor.sX, m_stMCursor.sY + 25 + iLoc, iter->c_str(), Color(150, 150, 150), false, 1);
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
            PutAlignedString(0, GetWidth() - 1, 0, "Ethereal", (m_GMFlags & GMFLAG_ETHEREAL) ? Color(50, 220, 50) : Color( 220, 50, 50)); // 800x600 Resolution xRisenx

            if (m_adminLevel >= 4)
            {
                //PutAlignedString(0, 639, 15, "No aggro", (m_GMFlags & GMFLAG_NOAGGRO) ?video::SColor(255,50,220,50) :video::SColor(255,220,50,50));
                //PutAlignedString(0, 639, 30,  "Invincible", (m_GMFlags & GMFLAG_INVINCIBLE) ?video::SColor(255,50,220,50) :video::SColor(255,220,50,50));
                PutAlignedString(0, GetWidth() - 1, 15, "No aggro", (m_GMFlags & GMFLAG_NOAGGRO) ? Color(50, 220, 50) : Color(220, 50, 50)); // 800x600 Resolution xRisenx
                PutAlignedString(0, GetWidth() - 1, 30, "Invincible", (m_GMFlags & GMFLAG_INVINCIBLE) ? Color(50, 220, 50) : Color(220, 50, 50)); // 800x600 Resolution xRisenx
            }
        }

        if (m_eventPot != -1)
        {
            wsprintfA(G_cTxt, "Total pot: %d", m_eventPot);
            //PutAlignedString(0, 639, 45,  G_cTxt,video::SColor(255,50,220,50));
            PutAlignedString(0, GetWidth() - 1, 45, G_cTxt, Color(50, 220, 50)); // 800x600 Resolution xRisenx
        }

        if (m_showTime)
        {
            wsprintfA(G_cTxt, "  %d", unixtime() / 1000 % 60);
            PutString(10, 100, G_cTxt, Color(255, 255, 255), false, 1);
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
     
    if (calcoldviewport)
    {
        if (iUpdateRet > 0)
            CalcViewPointOld();
    }
    else
        CalcViewPoint(dwTime);

    if (m_bIsObserverMode)
    {
        if ((dwTime - m_dwObserverCamTime) > 25)
        {
            m_dwObserverCamTime = dwTime;
            CalcViewPoint(dwTime);
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