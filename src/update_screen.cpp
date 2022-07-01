//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "helbreath.h"
#include "lan_eng.h"

// todo fix this shitfest
extern char G_cSpriteAlphaDegree;

extern char G_cCmdLine[256], G_cCmdLineTokenA[120], G_cCmdLineTokenA_Lowercase[120], G_cCmdLineTokenB[120], G_cCmdLineTokenC[120], G_cCmdLineTokenD[120], G_cCmdLineTokenE[120];

extern char _cDrawingOrder[];
extern char _cMantleDrawingOrder[];
extern char _cMantleDrawingOrderOnRun[];

extern short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;
extern int _tmp_iStatus;
extern short _tmp_sHeadApprValue, _tmp_sBodyApprValue, _tmp_sArmApprValue, _tmp_sLegApprValue;
extern char _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
extern int _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY;
extern uint16_t _tmp_wObjectID;
extern char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
extern uint16_t wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int iFocuiStatus;
extern int iFocusApprColor;
extern short sFocusHeadApprValue, sFocusBodyApprValue, sFocusArmApprValue, sFocusLegApprValue;

void helbreath::MakeSprite(char * FileName, int iStart, short sCount, bool bAlphaEffect)
{
    for (short i = 0; i < sCount; i++)
    {
        m_pSprite[i + iStart] = sprite::CreateSprite(FileName, i, bAlphaEffect);
    }
}
void helbreath::MakeTileSpr(char * FileName, short sStart, short sCount, bool bAlphaEffect)
{
    for (short i = 0; i < sCount; i++)
    {
        m_pTileSpr[i + sStart] = sprite::CreateSprite(FileName, i, bAlphaEffect);
    }
}
void helbreath::MakeEffectSpr(char * FileName, short sStart, short sCount, bool bAlphaEffect)
{
    for (short i = 0; i < sCount; i++)
    {
        m_pEffectSpr[i + sStart] = sprite::CreateSprite(FileName, i, bAlphaEffect);
    }
}

void helbreath::UpdateScreen()
{
	G_dwGlobalTime = unixtime();
	visible.clear();
	static int64_t counter = 0;

	switch (m_cGameMode)
	{
		case GAMEMODE_ONSELECTCHARACTER:
			UpdateScreen_OnSelectCharacter();
			break;

		case GAMEMODE_ONCONNECTING:
			m_stMCursor.sCursorFrame = 8;
			UpdateScreen_OnConnecting();
			break;

		case GAMEMODE_ONLOADING:
			UpdateScreen_OnLoading(true);
			break;

		case GAMEMODE_ONMAINGAME:
			UpdateScreen_OnGame();
			break;

		case GAMEMODE_ONWAITINGINITDATA:
			UpdateScreen_OnWaitInitData();
			break;

		case GAMEMODE_ONMSG:
			UpdateScreen_OnMsg();
			break;

		case GAMEMODE_ONQUIT:
			UpdateScreen_OnQuit();
			break;

		case GAMEMODE_ONLOGRESMSG:
			m_stMCursor.sCursorFrame = 0;
            UpdateScreen_OnLogResMsg();
            break;

		case GAMEMODE_ONCONNECTIONLOST:
			if (m_bSoundFlag) m_pESound[38].stop();
			if (m_bMusicStat) m_pBGM.stop();
			isItemLoaded = false;
			m_iPartyStatus = 0;
			m_iTotalPartyMember = 0;
			ClearPartyMembers();
			m_stMCursor.sCursorFrame = 0;
			break;
	}

	setRenderTarget(DS_VISIBLE);

	// Things rendered over the UI are here

	if (isloaded)
	{
		// movable window character select test
/*
		static int dir = 0;
		_tmp_sOwnerType = 1;
		_tmp_cDir = int((G_dwGlobalTime % 8000) / 1000) + 1;
		_tmp_cFrame = m_cMenuFrame = ((G_dwGlobalTime % 800) / 100);
		if (G_dwGlobalTime % 100)
		{
			uint8_t oldTarget = getRenderTarget();
			setRenderTarget(DS_CS, true, Color(0, 0, 0, 0));
			DrawObject_OnMove_ForMenu(0, 0, 127, 127, false, G_dwGlobalTime, 0, 0);
			sf::Image img = charselect.getTexture().copyToImage();
			std::vector<unsigned char> out;
			img.flipVertically();
			img.saveToMemory(out, "jpg");
			send_message_to_ui("charsprite", { { "data", base64_encode(out.data(), out.size()) }, { "dir", (int)_tmp_cDir }, { "key", G_dwGlobalTime % 100 } });
			setRenderTarget(oldTarget);
			dir = _tmp_cDir;
		}*/
	}

	if (rendering_character)
	{
		{
			uint8_t oldTarget = getRenderTarget();
			m_pEffectSpr[0]->PutTransSprite(m_stMCursor.sX + 36, m_stMCursor.sY + 50 + 28, 1, G_dwGlobalTime);
			setRenderTarget(DS_CS, true, Color(0, 0, 0, 0));
			DrawObject_OnMove_ForMenu(0, 0, m_stMCursor.sX + 36, m_stMCursor.sY + 50 + 28, false, G_dwGlobalTime, 0, 0);
			charselect.display();
			setRenderTarget(oldTarget);
		}

		auto size = charselect.getSize();
		const Color col = Color(255, 255, 255, 255);
		const sf::Texture & t = charselect.getTexture();
		sf::Sprite sprite = sf::Sprite(t);
		sprite.setPosition(charselectx, charselecty);
		sprite.setScale(2, 2);
		visible.draw(sprite);
	}

	//     if (m_cGameMode == GAMEMODE_ONSELECTCHARACTER)
	//     {
	//
	//         auto size = charselect.getSize();
	//         const Color col = Color(255, 255, 255, 255);
	//         const Texture & t = charselect.getTexture();
	//         sf::Sprite sprite = sf::Sprite(t);
	//         sprite.setPosition(charselectx, charselecty);
	//         sprite.setScale(2, 2);
	//         visible.draw(sprite);
	//     }

	float diffx = static_cast<float>(screenwidth) / screenwidth_v;
	float diffy = static_cast<float>(screenheight) / screenheight_v;
	int mx = int(m_stMCursor.sX * diffx);
	int my = int(m_stMCursor.sY * diffy);

	//test code
	std::string cfps;
	cfps = fmt::format("FPS: {}", fps.getFPS());

	//sf::Font & f = GetFont("arya");

	_text.setString(cfps);
	_text.setPosition(5, 5);
	_text.setFillColor(Color(255, 255, 255, 255));
	_text.setCharacterSize(14);

	std::string pingstr;

	pingstr = fmt::format("Ping: {}", (ping / (1000i64 * 1000)) / 2);

	if ((m_pBGM.Stopped || m_pBGM.getBuffer() == nullptr) && get_game_mode() == GAMEMODE_ONMAINGAME)
	{
		StartBGM();
	}

	if (get_game_mode() == GAMEMODE_ONMAINGAME)
	{
		sf::Sprite sprite = sf::Sprite(bg.getTexture());
		visible.draw(sprite);
	}

	if (draw_version) DrawVersion();

	visible.display();

	sf::Sprite sprite = sf::Sprite(visible.getTexture());
	sprite.setPosition(0, 0);
	sprite.setScale(static_cast<float>(screenwidth) / screenwidth_v, static_cast<float>(screenheight) / screenheight_v);

	window.clear();

	window.draw(sprite);

	// draw mouse over everything - this draws it full size at every virtual resolution.
	// this needs to go above `window.draw(sprite);` and draw to `visible` above to scale with virtual resolution
	if (m_bIsObserverMode == true)
	{
		DrawLine(mx - 5, my, mx + 5, my, Color(255, 0, 0, 255));
		DrawLine(mx, my, mx, my + 5, Color(255, 0, 0, 255));

		DrawLine(mx - 5, my - 1, mx + 5, my - 1, Color(255, 0, 0, 127));
		DrawLine(mx - 1, my - 5, mx - 1, my + 5, Color(255, 0, 0, 127));

		DrawLine(mx - 5, my + 1, mx + 5, my + 1, Color(255, 0, 0, 127));
		DrawLine(mx + 1, my - 5, mx + 1, my + 5, Color(255, 0, 0, 127));
	}
	else
		m_pSprite[SPRID_MOUSECURSOR]->draw_to(mx, my, m_stMCursor.sCursorFrame, unixseconds(), Color(255, 255, 255, 255), DS_WIN);

	m_stMCursor.sZ = 0;


	sf::Text TESTTEXT;

	TESTTEXT.setFont(_font.at("arya"));

	int drawcursorx = mx;
	int drawcursory = my - 40;

	if (mx > GetWidth() - 110)
		drawcursorx = mx - 120;
	if (my < 55)
		drawcursory = my + 50;

	TESTTEXT.setString(fmt::format("R({},{})", mx, my));
	TESTTEXT.setOutlineColor(Color(0, 0, 0));
	TESTTEXT.setOutlineThickness(2.f);
	TESTTEXT.setFillColor(Color(255, 255, 255, 255));
	TESTTEXT.setCharacterSize(20);
	TESTTEXT.setColor(Color(255, 255, 255, 255));
	TESTTEXT.setPosition((float)drawcursorx, (float)drawcursory);
    draw_to(TESTTEXT, DS_WIN);

    TESTTEXT.setPosition((float)drawcursorx, (float)drawcursory - 20);
    TESTTEXT.setString(fmt::format("V({},{})", m_stMCursor.sX, m_stMCursor.sY));

	draw_to(TESTTEXT, DS_WIN);

	window.draw(_text);

	_text.setPosition(5, 25);
	_text.setString(pingstr);
	window.draw(_text);

    _text.setPosition(5, 40);
    _text.setString(fmt::format("x: {} - y: {}", xtest, ytest));
    window.draw(_text);

}


void helbreath::UpdateScreen_OnMainMenu()
{
	short msX, msY, msZ;
	char cLB, cRB, cMB, cMIresult;
	int  iMIbuttonNum;
	static class CMouseInterface * pMI;
	uint64_t dwTime = G_dwGlobalTime;

	msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
	cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;

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
		pMI = new class CMouseInterface;

		// ShadowEvil Updated location of buttons
		pMI->AddRect(27, 148, 202, 170);
		pMI->AddRect(26, 174, 202, 195);
		pMI->AddRect(25, 200, 202, 222);
		pMI->AddRect(26, 226, 202, 248);
		pMI->AddRect(26, 254, 202, 276);
		m_stMCursor.sX = GetWidth() / 2;
		////DIRECTX m_dInput.m_sY = 240;
		m_stMCursor.sY = GetHeight() / 2; // 800x600 Resolution xRisenx

		// CLEROTH - INTERFACE
		/*pMI->AddRect(384,177,548,198);
		pMI->AddRect(384,215,548,236);
		pMI->AddRect(384,254,548,275);
		//DIRECTX m_dInput.m_sX = 400;
		//DIRECTX m_dInput.m_sY = 240;*/

		m_cCurFocus = 1;
		m_cMaxFocus = 3;

		m_bEnterPressed = false;
		m_cArrowPressed = 0;
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	////DIRECTX m_DDraw.ClearBackB4();//DIRECTX

	DrawNewDialogBox(SPRID_INTERFACE_ND_MAINMENU, -1, -1, 0, true);

	// CLEROTH
	////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX


	/*if ((msX >= 384) && (msY >= 177) && (msX <= 548) && (msY <= 198)) m_cCurFocus = 1;
	if ((msX >= 384) && (msY >= 215) && (msX <= 548) && (msY <= 236)) m_cCurFocus = 2;
	if ((msX >= 384) && (msY >= 254) && (msX <= 548) && (msY <= 275)) m_cCurFocus = 3;*/

	if ((msX >= 25) && (msX <= 202))
	{
		if ((msY >= 148) && (msY <= 170))
			m_cCurFocus = 1;
		if ((msY >= 174) && (msY <= 195))
			m_cCurFocus = 2;
		if ((msY >= 200) && (msY <= 222))
			m_cCurFocus = 3;
		if ((msY >= 226) && (msY <= 248))
			m_cCurFocus = 4;
		if ((msY >= 254) && (msY <= 276))
			m_cCurFocus = 5;
	}
	else
	{
		m_cCurFocus = 1; // Locked to Login button when client starts xRisenx
	}

	/*switch (m_cCurFocus) {

	case 1:
		m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->PutSpriteFast(384 , 177, 1, dwTime);
		break;
	case 2:
		m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->PutSpriteFast(384, 215, 2, dwTime);
		break;

	case 3:
		m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->PutSpriteFast(384, 254, 3, dwTime);
		break;
	}*/
	// New Buttons fixed by xRisenx
	switch (m_cCurFocus)
	{
		case 0:
			break;
		case 1:
			m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->PutSpriteFast(27, 150, 1, dwTime);
			break;
		case 2:
			m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->PutSpriteFast(27, 175, 2, dwTime);
			break;

		case 3:
			m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->PutSpriteFast(27, 200, 3, dwTime);
			break;

		case 4:
			m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->PutSpriteFast(28, 228, 4, dwTime);
			break;

		case 5:
			m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->PutSpriteFast(28, 256, 5, dwTime);
			break;
	}

	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	//m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSpriteRGB(msX, msY, 1, 0, 0, 0, dwTime);

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
		// Enter
		PlaySound('E', 14, 5);
		m_bEnterPressed = false;
		switch (m_cCurFocus)
		{
			case 1:
				delete pMI;
				ChangeGameMode(GAMEMODE_ONSELECTSERVER);
				return;
			case 2:
#ifdef MAKE_ACCOUNT
				ClearContents_OnSelectCharacter();
				delete pMI;
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
				delete pMI;
				ChangeGameMode(GAMEMODE_ONQUIT);
				return;
		}
	}

	DrawVersion(true);

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
	{
		PlaySound('E', 14, 5);
		m_cCurFocus = iMIbuttonNum;
		switch (iMIbuttonNum)
		{
			case 1:
				ChangeGameMode(GAMEMODE_ONSELECTSERVER);
				delete pMI;
				break;

			case 2:
#ifdef MAKE_ACCOUNT
				ClearContents_OnSelectCharacter();
				delete pMI;
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
				delete pMI;
				ChangeGameMode(GAMEMODE_ONQUIT);
				return;
		}
	}
	//	if ((msX >= 112) && (msY >= 150) && (msX <= 247) && (msY <= 191)) m_cCurFocus = 1;
	//	if ((msX >= 73) && (msY >= 192) && (msX <= 281) && (msY <= 230)) m_cCurFocus = 2;
	//	if ((msX >= 120) && (msY >= 231) && (msX <= 239) && (msY <= 262)) m_cCurFocus = 3;

	//	if (m_cGameModeCount < 6) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
	//	if (m_cGameModeCount < 2) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);

		//if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

struct load_data
{
    uint32_t id;
    uint32_t sprite_type;
    std::string name;
    uint32_t num;
    bool alpha;
    std::string label;
};

#define SPRITETYPE_SPRITE 1
#define SPRITETYPE_TILE 2
#define SPRITETYPE_EFFECT 3

std::queue<load_data> data_list;
int data_max = 0;

void helbreath::create_load_list()
{
	uint32_t i = 0;
	data_list.push({ SPRID_INTERFACE_NEWMAPS1, SPRITETYPE_SPRITE, "Newmaps", 0, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_NEWMAPS2, SPRITETYPE_SPRITE, "Newmaps", 1, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_NEWMAPS3, SPRITETYPE_SPRITE, "Newmaps", 2, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_NEWMAPS4, SPRITETYPE_SPRITE, "Newmaps", 3, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_NEWMAPS5, SPRITETYPE_SPRITE, "Newmaps", 4, false, "Loading interface" });

	data_list.push({ SPRID_INTERFACE_ND_GAME1, SPRITETYPE_SPRITE, "GameDialog", 0, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_ND_GAME2, SPRITETYPE_SPRITE, "GameDialog", 1, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_ND_GAME3, SPRITETYPE_SPRITE, "GameDialog", 2, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_ND_GAME4, SPRITETYPE_SPRITE, "GameDialog", 3, false, "Loading interface" });

	data_list.push({ SPRID_INTERFACE_ND_CRUSADE, SPRITETYPE_SPRITE, "GameDialog", 4, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_ND_ICONPANNEL, SPRITETYPE_SPRITE, "GameDialog", 6, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_ND_INVENTORY, SPRITETYPE_SPRITE, "GameDialog", 7, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_ND_SELECTCHAR, SPRITETYPE_SPRITE, "GameDialog", 8, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_ND_NEWCHAR, SPRITETYPE_SPRITE, "GameDialog", 9, false, "Loading interface" });
	//TODO current sprite does not contain newer interface
	//data_list.push({ SPRID_INTERFACE_ND_NEWEXCHANGE, SPRITETYPE_SPRITE, "GameDialog", 10, false, "Loading interface" });

	data_list.push({ SPRID_INTERFACE_ND_TEXT, SPRITETYPE_SPRITE, "DialogText", 0, false, "Loading interface" });
	data_list.push({ SPRID_INTERFACE_ND_BUTTON, SPRITETYPE_SPRITE, "DialogText", 1, false, "Loading interface" });

	auto make_sprite = [&](std::string FileName, uint32_t iStart, short sCount, bool bAlphaEffect, std::string label)
	{
		for (uint32_t i = 0; i < sCount; i++)
			data_list.push({ i + iStart, SPRITETYPE_SPRITE, FileName, i, bAlphaEffect, label });
	};

	auto make_tile_sprite = [&](std::string FileName, uint32_t iStart, short sCount, bool bAlphaEffect, std::string label)
	{
		for (uint32_t i = 0; i < sCount; i++)
			data_list.push({ i + iStart, SPRITETYPE_TILE, FileName, i, bAlphaEffect, label });
	};

	auto make_effect_sprite = [&](std::string FileName, uint32_t iStart, short sCount, bool bAlphaEffect, std::string label)
	{
		for (uint32_t i = 0; i < sCount; i++)
			data_list.push({ i + iStart, SPRITETYPE_EFFECT, FileName, i, bAlphaEffect, label });
	};

	make_sprite("Telescope", SPRID_INTERFACE_GUIDEMAP, 34, false, "Loading interface");
	make_sprite("Telescope2", SPRID_INTERFACE_GUIDEMAP + 35, 4, false, "Loading interface");
	make_sprite("monster", SPRID_INTERFACE_MONSTER, 1, false, "Loading interface");

	make_tile_sprite("maptiles1", 0, 32, true, "Loading map data");
	make_tile_sprite("structures1", 50, 20, true, "Loading map data");
	make_tile_sprite("Sinside1", 70, 27, false, "Loading map data");
	make_tile_sprite("Trees1", 100, 46, true, "Loading map data");
	make_tile_sprite("TreeShadows", 150, 46, true, "Loading map data");
	make_tile_sprite("objects1", 200, 10, true, "Loading map data");
	make_tile_sprite("objects2", 211, 5, true, "Loading map data");
	make_tile_sprite("objects3", 216, 4, true, "Loading map data");
	make_tile_sprite("objects4", 220, 2, true, "Loading map data");

	make_tile_sprite("Tile223-225", 223, 3, true, "Loading map data");
	make_tile_sprite("Tile226-229", 226, 4, true, "Loading map data");
	make_tile_sprite("objects5", 230, 9, true, "Loading map data");
	make_tile_sprite("objects6", 238, 4, true, "Loading map data");
	make_tile_sprite("objects7", 242, 7, true, "Loading map data");
	make_tile_sprite("maptiles2", 300, 15, true, "Loading map data");
	make_tile_sprite("maptiles4", 320, 10, true, "Loading map data");
	make_tile_sprite("maptiles5", 330, 19, true, "Loading map data");
	make_tile_sprite("maptiles6", 349, 4, true, "Loading map data");
	make_tile_sprite("maptiles353-361", 353, 9, true, "Loading map data");
	make_tile_sprite("Tile363-366", 363, 4, true, "Loading map data");
	make_tile_sprite("Tile367-367", 367, 1, true, "Loading map data");
	make_tile_sprite("Tile370-381", 370, 12, true, "Loading map data"); // Tile370~381
	make_tile_sprite("Tile382-387", 382, 6, true, "Loading map data");
	make_tile_sprite("Tile388-402", 388, 15, true, "Loading map data");

	make_tile_sprite("Tile403-405", 403, 3, true, "Loading map data");
	make_tile_sprite("Tile406-421", 406, 16, true, "Loading map data");
	make_tile_sprite("Tile422-429", 422, 8, true, "Loading map data");
	make_tile_sprite("Tile430-443", 430, 14, true, "Loading map data");
	make_tile_sprite("Tile444-444", 444, 1, true, "Loading map data");
	make_tile_sprite("Tile445-461", 445, 17, true, "Loading map data");
	make_tile_sprite("Tile462-473", 462, 12, true, "Loading map data");
	make_tile_sprite("Tile474-478", 474, 5, true, "Loading map data");
	make_tile_sprite("Tile479-488", 479, 10, true, "Loading map data");
	make_tile_sprite("Tile489-522", 489, 34, true, "Loading map data"); // Drunken City
	make_tile_sprite("Tile523-530", 523, 8, true, "Loading map data");  // Rampart
	make_tile_sprite("Tile531-540", 531, 10, true, "Loading map data"); // GodH + Pont
	make_tile_sprite("Tile541-545", 541, 5, true, "Loading map data");  // GodH

	make_sprite("itempack", SPRID_ITEMPACK_PIVOTPOINT + 1, 27, false, "Loading items");

	data_list.push({ SPRID_ITEMPACK_PIVOTPOINT + 20, SPRITETYPE_SPRITE, "itempack", 17, false, "Loading items" });
	data_list.push({ SPRID_ITEMPACK_PIVOTPOINT + 21, SPRITETYPE_SPRITE, "itempack", 18, false, "Loading items" });
	data_list.push({ SPRID_ITEMPACK_PIVOTPOINT + 22, SPRITETYPE_SPRITE, "itempack", 19, false, "Loading items" });

	make_sprite("itemground", SPRID_ITEMGROUND_PIVOTPOINT + 1, 19, false, "Loading items");

	data_list.push({ SPRID_ITEMGROUND_PIVOTPOINT + 20, SPRITETYPE_SPRITE, "itemground", 17, false, "Loading items" });
	data_list.push({ SPRID_ITEMGROUND_PIVOTPOINT + 21, SPRITETYPE_SPRITE, "itemground", 18, false, "Loading items" });
	data_list.push({ SPRID_ITEMGROUND_PIVOTPOINT + 22, SPRITETYPE_SPRITE, "itemground", 19, false, "Loading items" });

	make_sprite("item-dynamic", SPRID_ITEMDYNAMIC_PIVOTPOINT, 3, false, "Loading items");

	// MALE
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 0, SPRITETYPE_SPRITE, "itemequipM", 0, false, "Loading items" });   // body
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 1, SPRITETYPE_SPRITE, "itemequipM", 1, false, "Loading items" });   // 1-swords
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 2, SPRITETYPE_SPRITE, "itemequipM", 2, false, "Loading items" });   // 2-bows
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 3, SPRITETYPE_SPRITE, "itemequipM", 3, false, "Loading items" });   // 3-shields
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 4, SPRITETYPE_SPRITE, "itemequipM", 4, false, "Loading items" });   // 4-tunics
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 5, SPRITETYPE_SPRITE, "itemequipM", 5, false, "Loading items" });   // 5-shoes
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 7, SPRITETYPE_SPRITE, "itemequipM", 6, false, "Loading items" });   // 6-berk
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 8, SPRITETYPE_SPRITE, "itemequipM", 7, false, "Loading items" });   // 7-hoses
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 9, SPRITETYPE_SPRITE, "itemequipM", 8, false, "Loading items" });   // 8-bodyarmor
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 15, SPRITETYPE_SPRITE, "itemequipM", 11, false, "Loading items" }); // Axe hammer
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 17, SPRITETYPE_SPRITE, "itemequipM", 12, false, "Loading items" }); // Wands
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 18, SPRITETYPE_SPRITE, "itemequipM", 9, false, "Loading items" });  // hair
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 19, SPRITETYPE_SPRITE, "itemequipM", 10, false, "Loading items" }); // undies
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 20, SPRITETYPE_SPRITE, "itemequipM", 13, false, "Loading items" }); // capes
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 21, SPRITETYPE_SPRITE, "itemequipM", 14, false, "Loading items" }); // helm

	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 16, SPRITETYPE_SPRITE, "itempack", 15, false, "Loading items" }); // Necks, Angels, Pendants
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 22, SPRITETYPE_SPRITE, "itempack", 19, false, "Loading items" }); // Angels

	// FEMALE
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 40, SPRITETYPE_SPRITE, "itemequipW", 0, false, "Loading items" });  // body
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 41, SPRITETYPE_SPRITE, "itemequipW", 1, false, "Loading items" });  // 1-swords
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 42, SPRITETYPE_SPRITE, "itemequipW", 2, false, "Loading items" });  // 2-bows
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 43, SPRITETYPE_SPRITE, "itemequipW", 3, false, "Loading items" });  // 3-shields
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 45, SPRITETYPE_SPRITE, "itemequipW", 4, false, "Loading items" });  // 4-tunics
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 50, SPRITETYPE_SPRITE, "itemequipW", 5, false, "Loading items" });  // 5-shoes
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 51, SPRITETYPE_SPRITE, "itemequipW", 6, false, "Loading items" });  // 6-berk
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 52, SPRITETYPE_SPRITE, "itemequipW", 7, false, "Loading items" });  // 7-hoses
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 53, SPRITETYPE_SPRITE, "itemequipW", 8, false, "Loading items" });  // 8-bodyarmor
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 55, SPRITETYPE_SPRITE, "itemequipW", 11, false, "Loading items" }); // Axe hammer
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 57, SPRITETYPE_SPRITE, "itemequipW", 12, false, "Loading items" }); // Wands
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 58, SPRITETYPE_SPRITE, "itemequipW", 9, false, "Loading items" });  // hair
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 59, SPRITETYPE_SPRITE, "itemequipW", 10, false, "Loading items" }); // undies
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 60, SPRITETYPE_SPRITE, "itemequipW", 13, false, "Loading items" }); // capes
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 61, SPRITETYPE_SPRITE, "itemequipW", 14, false, "Loading items" }); // helm

	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 56, SPRITETYPE_SPRITE, "itempack", 15, false, "Loading items" }); // Necks, Angels, Pendants
	data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 62, SPRITETYPE_SPRITE, "itempack", 19, false, "Loading items" }); // Angels

	make_sprite("Bm", 500 + 15 * 8 * 0, 96, true, "Loading characters"); // Black Man (Type: 1)
	make_sprite("Wm", 500 + 15 * 8 * 1, 96, true, "Loading characters"); // White Man (Type: 2)
	make_sprite("Ym", 500 + 15 * 8 * 2, 96, true, "Loading characters"); // Yellow Man (Type: 3)
	make_sprite("Bw", 500 + 15 * 8 * 3, 96, true, "Loading characters"); // Black Woman (Type: 4)
	make_sprite("Ww", 500 + 15 * 8 * 4, 96, true, "Loading characters"); // White Woman (Type: 5)
	make_sprite("Yw", 500 + 15 * 8 * 5, 96, true, "Loading characters"); // Yellow Woman (Type: 6)

	make_sprite("TutelarAngel1", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 0, 48, false, "Loading angels"); //(STR)
	make_sprite("TutelarAngel2", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 1, 48, false, "Loading angels"); //(DEX)
	make_sprite("TutelarAngel3", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 2, 48, false, "Loading angels"); //(INT)
	make_sprite("TutelarAngel4", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 3, 48, false, "Loading angels"); //(MAG)

	make_sprite("slm", SPRID_MOB + 7 * 8 * 0, 40, true, "Loading npcs");    // Slime (Type: 10)
	make_sprite("ske", SPRID_MOB + 7 * 8 * 1, 40, true, "Loading npcs");    // Skeleton (Type: 11)
	make_sprite("Gol", SPRID_MOB + 7 * 8 * 2, 40, true, "Loading npcs");    // Stone-Golem (Type: 12)
	make_sprite("Cyc", SPRID_MOB + 7 * 8 * 3, 40, true, "Loading npcs");    // Cyclops (Type: 13)
	make_sprite("Orc", SPRID_MOB + 7 * 8 * 4, 40, true, "Loading npcs");    // Orc (Type: 14)
	make_sprite("Shopkpr", SPRID_MOB + 7 * 8 * 5, 8, true, "Loading npcs"); // ShopKeeper-Woman (Type: 15)
	make_sprite("Ant", SPRID_MOB + 7 * 8 * 6, 40, true, "Loading npcs");    //  Giant-Ant (Type: 16)
	make_sprite("Scp", SPRID_MOB + 7 * 8 * 7, 40, true, "Loading npcs");    //  Scorpion (Type: 17)
	make_sprite("Zom", SPRID_MOB + 7 * 8 * 8, 40, true, "Loading npcs");    //  Zombie (Type: 18)
	make_sprite("Gandlf", SPRID_MOB + 7 * 8 * 9, 8, true, "Loading npcs");
	make_sprite("Howard", SPRID_MOB + 7 * 8 * 10, 8, true, "Loading npcs");
	make_sprite("Guard", SPRID_MOB + 7 * 8 * 11, 40, true, "Loading npcs");  // Guard (Type: 21)
	make_sprite("Amp", SPRID_MOB + 7 * 8 * 12, 40, true, "Loading npcs");    // Amphis (Type: 22)
	make_sprite("Cla", SPRID_MOB + 7 * 8 * 13, 40, true, "Loading npcs");    // Clay-Golem (Type: 23)
	make_sprite("tom", SPRID_MOB + 7 * 8 * 14, 8, true, "Loading npcs");     // Tom (Type: 24)
	make_sprite("William", SPRID_MOB + 7 * 8 * 15, 8, true, "Loading npcs"); // William (Type: 25)

	make_sprite("Kennedy", SPRID_MOB + 7 * 8 * 16, 8, true, "Loading npcs");        // Kennedy (Type: 26)
	make_sprite("Helb", SPRID_MOB + 7 * 8 * 17, 40, true, "Loading npcs");          // Hellbound (Type: 27)
	make_sprite("Troll", SPRID_MOB + 7 * 8 * 18, 40, true, "Loading npcs");         // Troll (Type: 28)
	make_sprite("EnragedTroll", SPRID_MOB + 7 * 8 * 112, 40, true, "Loading npcs"); // Enraged Troll (Type: 122)
	make_sprite("Orge", SPRID_MOB + 7 * 8 * 19, 40, true, "Loading npcs");          // Orge (Type: 29)
	make_sprite("Liche", SPRID_MOB + 7 * 8 * 20, 40, true, "Loading npcs");         // Liche (Type: 30)
	make_sprite("Demon", SPRID_MOB + 7 * 8 * 21, 40, true, "Loading npcs");         // Demon (Type: 31)

	//TODO some dumbass made this - missing 40th (39) sprite
	//make_sprite("BlackDemon", SPRID_MOB + 7 * 8 * 95, 40, true, "Loading npcs");// Black Demon (Type: 105)

	make_sprite("Unicorn", SPRID_MOB + 7 * 8 * 22, 40, true, "Loading npcs");     // Unicorn (Type: 32)
	make_sprite("DarkUnicorn", SPRID_MOB + 7 * 8 * 94, 40, true, "Loading npcs"); // DarkUnicorn (Type: 104)
	make_sprite("WereWolf", SPRID_MOB + 7 * 8 * 23, 40, true, "Loading npcs");    // WereWolf (Type: 33)
	make_sprite("Dummy", SPRID_MOB + 7 * 8 * 24, 40, true, "Loading npcs");       // Dummy (Type: 34)

	for (i = 0; i < 40; i++)
		data_list.push({ SPRID_MOB + i + 7 * 8 * 25, SPRITETYPE_SPRITE, "Effect5", 0, false, "Loading npcs" });

	make_sprite("GT-Arrow", SPRID_MOB + 7 * 8 * 26, 40, true, "Loading npcs");        // Arrow-GuardTower (Type: 36)
	make_sprite("GT-Cannon", SPRID_MOB + 7 * 8 * 27, 40, true, "Loading npcs");       // Cannon-GuardTower (Type: 37)
	make_sprite("ManaCollector", SPRID_MOB + 7 * 8 * 28, 40, true, "Loading npcs");   // Mana Collector (Type: 38)
	make_sprite("Detector", SPRID_MOB + 7 * 8 * 29, 40, true, "Loading npcs");        // Detector (Type: 39)
	make_sprite("ESG", SPRID_MOB + 7 * 8 * 30, 40, true, "Loading npcs");             // ESG (Type: 40)
	make_sprite("GMG", SPRID_MOB + 7 * 8 * 31, 40, true, "Loading npcs");             // GMG (Type: 41)
	make_sprite("ManaStone", SPRID_MOB + 7 * 8 * 32, 40, true, "Loading npcs");       // ManaStone (Type: 42)
	make_sprite("LWB", SPRID_MOB + 7 * 8 * 33, 40, true, "Loading npcs");             // Light War Beetle (Type: 43)
	make_sprite("GHK", SPRID_MOB + 7 * 8 * 34, 40, true, "Loading npcs");             // God's Hand Knight (Type: 44)
	make_sprite("GHKABS", SPRID_MOB + 7 * 8 * 35, 40, true, "Loading npcs");          // God's Hand Knight with Armored Battle Steed (Type: 45)
	make_sprite("TK", SPRID_MOB + 7 * 8 * 36, 40, true, "Loading npcs");              // Temple Knight (Type: 46)
	make_sprite("BG", SPRID_MOB + 7 * 8 * 37, 40, true, "Loading npcs");              // Battle Golem (Type: 47)
	make_sprite("Stalker", SPRID_MOB + 7 * 8 * 38, 40, true, "Loading npcs");         // Stalker (Type: 48)
	make_sprite("Hellclaw", SPRID_MOB + 7 * 8 * 39, 40, true, "Loading npcs");        // Hellclaw (Type: 49)
	make_sprite("Tigerworm", SPRID_MOB + 7 * 8 * 40, 40, true, "Loading npcs");       // Tigerworm (Type: 50)
	make_sprite("Catapult", SPRID_MOB + 7 * 8 * 41, 40, true, "Loading npcs");        // Catapult (Type: 51)
	make_sprite("Gagoyle", SPRID_MOB + 7 * 8 * 42, 40, true, "Loading npcs");         // Gargoyle (Type: 52)
	make_sprite("EnragedGagoyle", SPRID_MOB + 7 * 8 * 115, 40, true, "Loading npcs"); // Enraged Gagoyle (Type: 125)
	make_sprite("Beholder", SPRID_MOB + 7 * 8 * 43, 40, true, "Loading npcs");        // Beholder (Type: 53)
	make_sprite("DarkElf", SPRID_MOB + 7 * 8 * 44, 40, true, "Loading npcs");         // Dark-Elf (Type: 54)
	make_sprite("Bunny", SPRID_MOB + 7 * 8 * 45, 40, true, "Loading npcs");           // Bunny (Type: 55)
	make_sprite("Cat", SPRID_MOB + 7 * 8 * 46, 40, true, "Loading npcs");             // Cat (Type: 56)
	make_sprite("GiantFrog", SPRID_MOB + 7 * 8 * 47, 40, true, "Loading npcs");       // GiantFrog (Type: 57)
	make_sprite("MTGiant", SPRID_MOB + 7 * 8 * 48, 40, true, "Loading npcs");         // Mountain Giant (Type: 58)
	make_sprite("Ettin", SPRID_MOB + 7 * 8 * 49, 40, true, "Loading npcs");           // Ettin (Type: 59)
	make_sprite("CanPlant", SPRID_MOB + 7 * 8 * 50, 40, true, "Loading npcs");        // Cannibal Plant (Type: 60)
	make_sprite("Rudolph", SPRID_MOB + 7 * 8 * 51, 40, true, "Loading npcs");         // Rudolph (Type: 61)
	make_sprite("DireBoar", SPRID_MOB + 7 * 8 * 52, 40, true, "Loading npcs");        // Boar (Type: 62)
	make_sprite("frost", SPRID_MOB + 7 * 8 * 53, 40, true, "Loading npcs");           // Frost (Type: 63)
	make_sprite("Crop", SPRID_MOB + 7 * 8 * 54, 40, true, "Loading npcs");            // Crop(Type: 64)
	make_sprite("IceGolem", SPRID_MOB + 7 * 8 * 55, 40, true, "Loading npcs");        // IceGolem (Type: 65)
	make_sprite("Wyvern", SPRID_MOB + 7 * 8 * 56, 24, true, "Loading npcs");          // Wyvern (Type: 66)
	make_sprite("McGaffin", SPRID_MOB + 7 * 8 * 57, 16, true, "Loading npcs");        // McGaffin (Type: 67)
	make_sprite("Perry", SPRID_MOB + 7 * 8 * 58, 16, true, "Loading npcs");           // Perry (Type: 68)
	make_sprite("Devlin", SPRID_MOB + 7 * 8 * 59, 16, true, "Loading npcs");          // Devlin (Type: 69)
	make_sprite("Barlog", SPRID_MOB + 7 * 8 * 60, 40, true, "Loading npcs");          // Barlog (Type: 70)
	make_sprite("Centaurus", SPRID_MOB + 7 * 8 * 61, 40, true, "Loading npcs");       // Centaurus (Type: 71)
	make_sprite("ClawTurtle", SPRID_MOB + 7 * 8 * 62, 40, true, "Loading npcs");      // Claw-Turtle (Type: 72)
	make_sprite("FireWyvern", SPRID_MOB + 7 * 8 * 63, 24, true, "Loading npcs");      // Fire-Wyvern (Type: 73)
	make_sprite("GiantCrayfish", SPRID_MOB + 7 * 8 * 64, 40, true, "Loading npcs");   // Giant-Crayfish (Type: 74)
	make_sprite("GiantLizard", SPRID_MOB + 7 * 8 * 65, 40, true, "Loading npcs");     // Giant-Lizard (Type: 75)
	make_sprite("DarkWyvern", SPRID_MOB + 7 * 8 * 98, 24, true, "Loading npcs");      // Dark-Wyvern (Type: 108)
	make_sprite("EarthWyvern", SPRID_MOB + 7 * 8 * 96, 24, true, "Loading npcs");     // Earth-Wyvern (Type: 106)
	make_sprite("GrassWyvern", SPRID_MOB + 7 * 8 * 99, 24, true, "Loading npcs");     // Grass-Wyvern (Type: 109)
	make_sprite("MetalWyvern", SPRID_MOB + 7 * 8 * 102, 24, true, "Loading npcs");    // Metal-Wyvern (Type: 112)
	make_sprite("SeaWyvern", SPRID_MOB + 7 * 8 * 101, 24, true, "Loading npcs");      // Sea-Wyvern (Type: 111)
	make_sprite("SkyWyvern", SPRID_MOB + 7 * 8 * 100, 24, true, "Loading npcs");      // Sky-Wyvern (Type: 110)
	make_sprite("VoidWyvern", SPRID_MOB + 7 * 8 * 103, 24, true, "Loading npcs");     // Void-Wyvern (Type: 113)

	//TODO: another retard made this (missing last sprite)
	//make_sprite("WindWyvern", SPRID_MOB + 7 * 8 * 97, 24, true, "Loading npcs");// Wind-Wyvern (Type: 107)

	make_sprite("GiantPlant", SPRID_MOB + 7 * 8 * 66, 40, true, "Loading npcs");       // Giant-Plant (Type: 76)
	make_sprite("MasterMageOrc", SPRID_MOB + 7 * 8 * 67, 40, true, "Loading npcs");    // MasterMage-Orc (Type: 77)
	make_sprite("Minotaurs", SPRID_MOB + 7 * 8 * 68, 40, true, "Loading npcs");        // Minotaurs (Type: 78)
	make_sprite("Nizie", SPRID_MOB + 7 * 8 * 69, 40, true, "Loading npcs");            // Nizie (Type: 79)
	make_sprite("Tentocle", SPRID_MOB + 7 * 8 * 70, 40, true, "Loading npcs");         // Tentocle (Type: 80)
	make_sprite("yspro", SPRID_MOB + 7 * 8 * 71, 32, true, "Loading npcs");            // Abaddon (Type: 81)
	make_sprite("Sorceress", SPRID_MOB + 7 * 8 * 72, 40, true, "Loading npcs");        // Sorceress (Type: 82)
	make_sprite("TPKnight", SPRID_MOB + 7 * 8 * 73, 40, true, "Loading npcs");         // TPKnight (Type: 83)
	make_sprite("ElfMaster", SPRID_MOB + 7 * 8 * 74, 40, true, "Loading npcs");        // ElfMaster (Type: 84)
	make_sprite("DarkKnight", SPRID_MOB + 7 * 8 * 75, 40, true, "Loading npcs");       // DarkKnight (Type: 85)
	make_sprite("HBTank", SPRID_MOB + 7 * 8 * 76, 32, true, "Loading npcs");           // HeavyBattleTank (Type: 86)
	make_sprite("CBTurret", SPRID_MOB + 7 * 8 * 77, 32, true, "Loading npcs");         // CBTurret (Type: 87)
	make_sprite("Babarian", SPRID_MOB + 7 * 8 * 78, 40, true, "Loading npcs");         // Babarian (Type: 88)
	make_sprite("ACannon", SPRID_MOB + 7 * 8 * 79, 32, true, "Loading npcs");          // ACannon (Type: 89)
	make_sprite("EternalDragon", SPRID_MOB + 7 * 8 * 104, 32, true, "Loading npcs");   // Eternal dragon (Type: 114)
	make_sprite("EnragedCyclops", SPRID_MOB + 7 * 8 * 113, 40, true, "Loading npcs");  // Enraged Cyclops (Type: 123)
	make_sprite("EnragedStalker", SPRID_MOB + 7 * 8 * 114, 40, true, "Loading npcs");  // Enraged Stalker (Type: 124)
	make_sprite("EnragedHellclaw", SPRID_MOB + 7 * 8 * 116, 40, true, "Loading npcs"); // Enraged Hellclaw (Type: 126)
	make_sprite("EnragedTW", SPRID_MOB + 7 * 8 * 117, 40, true, "Loading npcs");       // Enraged Tigerworm (Type: 127)

	make_sprite("Gail", SPRID_MOB + 7 * 8 * 80, 8, true, "Loading npcs");  // Gail (Type: 90)
	make_sprite("Gate", SPRID_MOB + 7 * 8 * 81, 24, true, "Loading npcs"); // Heldenian Gate (Type: 91)/**/
	make_sprite("Scarecrow", SPRID_MOB + 7 * 8 * 82, 40, true, "Loading npcs");
	make_sprite("Princess", SPRID_MOB + 7 * 8 * 92, 8, true, "Loading npcs"); // Princess 102

	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_M + i + 15 * 0, SPRITETYPE_SPRITE, "Mpt", i + 12 * 0, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_M + i + 15 * 1, SPRITETYPE_SPRITE, "Mpt", i + 12 * 1, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_M + i + 15 * 2, SPRITETYPE_SPRITE, "Mpt", i + 12 * 2, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_M + i + 15 * 3, SPRITETYPE_SPRITE, "Mpt", i + 12 * 3, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_M + i + 15 * 4, SPRITETYPE_SPRITE, "Mpt", i + 12 * 4, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_M + i + 15 * 5, SPRITETYPE_SPRITE, "Mpt", i + 12 * 5, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_M + i + 15 * 6, SPRITETYPE_SPRITE, "Mpt", i + 12 * 6, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_M + i + 15 * 7, SPRITETYPE_SPRITE, "Mpt", i + 12 * 7, false, "Loading items" });

	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_M + i + 15 * 0, SPRITETYPE_SPRITE, "Mhr", i + 12 * 0, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_M + i + 15 * 1, SPRITETYPE_SPRITE, "Mhr", i + 12 * 1, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_M + i + 15 * 2, SPRITETYPE_SPRITE, "Mhr", i + 12 * 2, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_M + i + 15 * 3, SPRITETYPE_SPRITE, "Mhr", i + 12 * 3, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_M + i + 15 * 4, SPRITETYPE_SPRITE, "Mhr", i + 12 * 4, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_M + i + 15 * 5, SPRITETYPE_SPRITE, "Mhr", i + 12 * 5, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_M + i + 15 * 6, SPRITETYPE_SPRITE, "Mhr", i + 12 * 6, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_M + i + 15 * 7, SPRITETYPE_SPRITE, "Mhr", i + 12 * 7, false, "Loading items" });

	make_sprite("MLArmor", SPRID_BODYARMOR_M + 15 * 1, 12, true, "Loading items");
	make_sprite("MCMail", SPRID_BODYARMOR_M + 15 * 2, 12, true, "Loading items");
	make_sprite("MSMail", SPRID_BODYARMOR_M + 15 * 3, 12, true, "Loading items");
	make_sprite("MPMail", SPRID_BODYARMOR_M + 15 * 4, 12, true, "Loading items");
	//make_sprite( "MPMail",	SPRID_BODYARMOR_M + 15*25, 12, true, "Loading items");
	make_sprite("Mtunic", SPRID_BODYARMOR_M + 15 * 5, 12, true, "Loading items");
	make_sprite("MRobe1", SPRID_BODYARMOR_M + 15 * 6, 12, true, "Loading items");
	make_sprite("MSanta", SPRID_BODYARMOR_M + 15 * 7, 12, true, "Loading items");
	make_sprite("MHPMail1", SPRID_BODYARMOR_M + 15 * 8, 12, true, "Loading items");       //hero
	make_sprite("MHPMail2", SPRID_BODYARMOR_M + 15 * 9, 12, true, "Loading items");       //hero
	make_sprite("MHRobe1", SPRID_BODYARMOR_M + 15 * 10, 12, true, "Loading items");       //hero
	make_sprite("MHRobe2", SPRID_BODYARMOR_M + 15 * 11, 12, true, "Loading items");       //hero
	make_sprite("AncHeroArmorM", SPRID_BODYARMOR_M + 15 * 12, 12, true, "Loading items"); //hero // Black Ancient Hero Armor M
	make_sprite("AncHeroRobeM", SPRID_BODYARMOR_M + 15 * 13, 12, true, "Loading items");  //hero // Black Ancient Hero Robe M
	make_sprite("ALegRobeM", SPRID_BODYARMOR_M + 15 * 14, 12, true, "Loading items");     // Aresden Ancient Hero Robe M
	make_sprite("ELegRobeM", SPRID_BODYARMOR_M + 15 * 15, 12, true, "Loading items");     // Elvine Ancient Hero Robe M
	make_sprite("ALegArmorM", SPRID_BODYARMOR_M + 15 * 16, 12, true, "Loading items");    // Aresden Ancient Hero Armor M
	make_sprite("ELegArmorM", SPRID_BODYARMOR_M + 15 * 17, 12, true, "Loading items");    // Elvine Ancient Hero Armor M
	make_sprite("MShirt", SPRID_BERK_M + 15 * 1, 12, true, "Loading items");
	make_sprite("MHauberk", SPRID_BERK_M + 15 * 2, 12, true, "Loading items");
	//make_sprite( "MHauberk",	  SPRID_BERK_M + 15*25, 12, true, "Loading items");
	make_sprite("MHHauberk1", SPRID_BERK_M + 15 * 3, 12, true, "Loading items");
	make_sprite("MHHauberk2", SPRID_BERK_M + 15 * 4, 12, true, "Loading items");
	make_sprite("AncHeroHauberkM", SPRID_BERK_M + 15 * 5, 12, true, "Loading items"); // Black Ancient Hero Hauberk M
	make_sprite("ALegHauberkM", SPRID_BERK_M + 15 * 6, 12, true, "Loading items");    // Aresden Ancient Hero Hauberk M
	make_sprite("ELegHauberkM", SPRID_BERK_M + 15 * 7, 12, true, "Loading items");    // Elvine Ancient Hero Hauberk M

	make_sprite("MTrouser", SPRID_LEGG_M + 15 * 1, 12, true, "Loading items");
	make_sprite("MHTrouser", SPRID_LEGG_M + 15 * 2, 12, true, "Loading items");
	make_sprite("MCHoses", SPRID_LEGG_M + 15 * 3, 12, true, "Loading items");
	make_sprite("MLeggings", SPRID_LEGG_M + 15 * 4, 12, true, "Loading items");
	//make_sprite( "MLeggings",SPRID_LEGG_M + 15*25, 12, true, "Loading items");
	make_sprite("MHLeggings1", SPRID_LEGG_M + 15 * 5, 12, true, "Loading items");   // hero
	make_sprite("MHLeggings2", SPRID_LEGG_M + 15 * 6, 12, true, "Loading items");   // hero
	make_sprite("AncHeroLegM", SPRID_LEGG_M + 15 * 7, 12, true, "Loading items");   // hero // Black Ancient Hero Leggings M
	make_sprite("ALegLeggingsM", SPRID_LEGG_M + 15 * 8, 12, true, "Loading items"); // Aresden Ancient Leggings M
	make_sprite("ELegLeggingsM", SPRID_LEGG_M + 15 * 9, 12, true, "Loading items"); // Elvine Ancient Leggings M

	make_sprite("MShoes", SPRID_BOOT_M + 15 * 1, 12, true, "Loading items");
	make_sprite("MLBoots", SPRID_BOOT_M + 15 * 2, 12, true, "Loading items");
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 1, SPRITETYPE_SPRITE, "Msw", i + 56 * 0, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 2, SPRITETYPE_SPRITE, "Msw", i + 56 * 1, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 3, SPRITETYPE_SPRITE, "Msw", i + 56 * 2, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 4, SPRITETYPE_SPRITE, "Msw", i + 56 * 3, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 5, SPRITETYPE_SPRITE, "Msw", i + 56 * 4, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 6, SPRITETYPE_SPRITE, "Msw", i + 56 * 5, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 7, SPRITETYPE_SPRITE, "Msw", i + 56 * 6, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 8, SPRITETYPE_SPRITE, "Msw", i + 56 * 7, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 9, SPRITETYPE_SPRITE, "Msw", i + 56 * 8, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 10, SPRITETYPE_SPRITE, "Msw", i + 56 * 9, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 11, SPRITETYPE_SPRITE, "Msw", i + 56 * 10, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 12, SPRITETYPE_SPRITE, "Msw", i + 56 * 11, false, "Loading items" });

	make_sprite("Mswx", SPRID_WEAPON_M + 64 * 5, 56, true, "Loading items");
	make_sprite("Msw2", SPRID_WEAPON_M + 64 * 13, 56, true, "Loading items");
	make_sprite("Msw3", SPRID_WEAPON_M + 64 * 14, 56, true, "Loading items");
	make_sprite("MStormBringer", SPRID_WEAPON_M + 64 * 15, 56, true, "Loading items");
	make_sprite("MDarkExec", SPRID_WEAPON_M + 64 * 16, 56, true, "Loading items");
	make_sprite("MKlonessBlade", SPRID_WEAPON_M + 64 * 17, 56, true, "Loading items");
	make_sprite("MKlonessAstock", SPRID_WEAPON_M + 64 * 18, 56, true, "Loading items");
	make_sprite("MDebastator", SPRID_WEAPON_M + 64 * 19, 56, true, "Loading items");
	make_sprite("MAxe1", SPRID_WEAPON_M + 64 * 20, 56, true, "Loading items"); // Axe
	make_sprite("MAxe2", SPRID_WEAPON_M + 64 * 21, 56, true, "Loading items");
	make_sprite("MAxe3", SPRID_WEAPON_M + 64 * 22, 56, true, "Loading items");
	make_sprite("MAxe4", SPRID_WEAPON_M + 64 * 23, 56, true, "Loading items");
	make_sprite("MAxe5", SPRID_WEAPON_M + 64 * 24, 56, true, "Loading items");
	make_sprite("MPickAxe1", SPRID_WEAPON_M + 64 * 25, 56, true, "Loading items");
	make_sprite("MAxe6", SPRID_WEAPON_M + 64 * 26, 56, true, "Loading items");
	make_sprite("Mhoe", SPRID_WEAPON_M + 64 * 27, 56, true, "Loading items");
	make_sprite("MKlonessAxe", SPRID_WEAPON_M + 64 * 28, 56, true, "Loading items");
	make_sprite("MLightBlade", SPRID_WEAPON_M + 64 * 29, 56, true, "Loading items");
	make_sprite("MHammer", SPRID_WEAPON_M + 64 * 30, 56, true, "Loading items");
	make_sprite("MBHammer", SPRID_WEAPON_M + 64 * 31, 56, true, "Loading items");
	make_sprite("MBabHammer", SPRID_WEAPON_M + 64 * 32, 56, true, "Loading items");
	make_sprite("MBShadowSword", SPRID_WEAPON_M + 64 * 33, 56, true, "Loading items");
	make_sprite("MBerserkWand", SPRID_WEAPON_M + 64 * 34, 56, true, "Loading items");
	make_sprite("Mstaff1", SPRID_WEAPON_M + 64 * 35, 56, true, "Loading items"); // Staff
	make_sprite("Mstaff2", SPRID_WEAPON_M + 64 * 36, 56, true, "Loading items");
	make_sprite("MStaff3", SPRID_WEAPON_M + 64 * 37, 56, true, "Loading items");
	make_sprite("MReMagicWand", SPRID_WEAPON_M + 64 * 38, 56, true, "Loading items");
	make_sprite("MKlonessWand", SPRID_WEAPON_M + 64 * 39, 56, true, "Loading items");
	make_sprite("Staff4M", SPRID_WEAPON_M + 64 * 40, 56, true, "Loading items");
	// Bows 40 41 below
	make_sprite("MDirectBow", SPRID_WEAPON_M + 64 * 43, 56, true, "Loading items");
	make_sprite("MFireBow", SPRID_WEAPON_M + 64 * 44, 56, true, "Loading items");
	make_sprite("Mbo", SPRID_WEAPON_M + 64 * 41, 56, true, "Loading items");

	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_M + i + 64 * 42, SPRITETYPE_SPRITE, "Mbo", i + 56 * 1, false, "Loading items" });

	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_M + i + 8 * 1, SPRITETYPE_SPRITE, "Msh", i + 7 * 0, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_M + i + 8 * 2, SPRITETYPE_SPRITE, "Msh", i + 7 * 1, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_M + i + 8 * 3, SPRITETYPE_SPRITE, "Msh", i + 7 * 2, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_M + i + 8 * 4, SPRITETYPE_SPRITE, "Msh", i + 7 * 3, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_M + i + 8 * 5, SPRITETYPE_SPRITE, "Msh", i + 7 * 4, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_M + i + 8 * 6, SPRITETYPE_SPRITE, "Msh", i + 7 * 5, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_M + i + 8 * 7, SPRITETYPE_SPRITE, "Msh", i + 7 * 6, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_M + i + 8 * 8, SPRITETYPE_SPRITE, "Msh", i + 7 * 7, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_M + i + 8 * 9, SPRITETYPE_SPRITE, "Msh", i + 7 * 8, false, "Loading items" });

	make_sprite("Mmantle01", SPRID_MANTLE_M + 15 * 1, 12, true, "Loading items");
	make_sprite("Mmantle02", SPRID_MANTLE_M + 15 * 2, 12, true, "Loading items");
	make_sprite("Mmantle03", SPRID_MANTLE_M + 15 * 3, 12, true, "Loading items");
	make_sprite("Mmantle04", SPRID_MANTLE_M + 15 * 4, 12, true, "Loading items");
	make_sprite("Mmantle05", SPRID_MANTLE_M + 15 * 5, 12, true, "Loading items");
	make_sprite("Mmantle06", SPRID_MANTLE_M + 15 * 6, 12, true, "Loading items");
	make_sprite("Mmantle07", SPRID_MANTLE_M + 15 * 7, 12, true, "Loading items");
	make_sprite("Mmantle08", SPRID_MANTLE_M + 15 * 8, 12, true, "Loading items");
	make_sprite("AAncHeroCapeM", SPRID_MANTLE_M + 15 * 9, 12, true, "Loading items");  // Aresden Black Ancient Hero Cape M
	make_sprite("EAncHeroCapeM", SPRID_MANTLE_M + 15 * 10, 12, true, "Loading items"); // Elvine Black Ancient Hero Cape M
	make_sprite("MHelm1", SPRID_HEAD_M + 15 * 1, 12, true, "Loading items");
	make_sprite("MHelm2", SPRID_HEAD_M + 15 * 2, 12, true, "Loading items");
	make_sprite("MHelm3", SPRID_HEAD_M + 15 * 3, 12, true, "Loading items");
	make_sprite("MHelm4", SPRID_HEAD_M + 15 * 4, 12, true, "Loading items");
	make_sprite("NMHelm1", SPRID_HEAD_M + 15 * 5, 12, true, "Loading items");
	//make_sprite( "NMHelm1", SPRID_HEAD_M + 15*25, 12, true, "Loading items");
	make_sprite("NMHelm2", SPRID_HEAD_M + 15 * 6, 12, true, "Loading items");
	make_sprite("NMHelm3", SPRID_HEAD_M + 15 * 7, 12, true, "Loading items");
	make_sprite("NMHelm4", SPRID_HEAD_M + 15 * 8, 12, true, "Loading items");
	make_sprite("MHHelm1", SPRID_HEAD_M + 15 * 9, 12, true, "Loading items");
	make_sprite("MHHelm2", SPRID_HEAD_M + 15 * 10, 12, true, "Loading items");
	make_sprite("MHCap1", SPRID_HEAD_M + 15 * 11, 12, true, "Loading items");
	make_sprite("MHCap2", SPRID_HEAD_M + 15 * 12, 12, true, "Loading items");
	make_sprite("AncHeroHelmM", SPRID_HEAD_M + 15 * 13, 12, true, "Loading items"); // Black Ancient Hero Helm M
	make_sprite("AncHeroCapM", SPRID_HEAD_M + 15 * 14, 12, true, "Loading items");  // Black Ancient Hero Cap M
	make_sprite("ALegHelmM", SPRID_HEAD_M + 15 * 15, 12, true, "Loading items");    // Aresden Ancient Helm M
	make_sprite("ELegHelmM", SPRID_HEAD_M + 15 * 16, 12, true, "Loading items");    // Elvine Ancient Helm M
	make_sprite("ALegCapM", SPRID_HEAD_M + 15 * 17, 12, true, "Loading items");     // Aresden Ancient Cap M
	make_sprite("ELegCapM", SPRID_HEAD_M + 15 * 18, 12, true, "Loading items");     // Elvine Ancient Cap M

	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_W + i + 15 * 0, SPRITETYPE_SPRITE, "Mpt", i + 12 * 0, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_W + i + 15 * 1, SPRITETYPE_SPRITE, "Mpt", i + 12 * 1, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_W + i + 15 * 2, SPRITETYPE_SPRITE, "Mpt", i + 12 * 2, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_W + i + 15 * 3, SPRITETYPE_SPRITE, "Mpt", i + 12 * 3, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_W + i + 15 * 4, SPRITETYPE_SPRITE, "Mpt", i + 12 * 4, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_W + i + 15 * 5, SPRITETYPE_SPRITE, "Mpt", i + 12 * 5, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_W + i + 15 * 6, SPRITETYPE_SPRITE, "Mpt", i + 12 * 6, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_UNDIES_W + i + 15 * 7, SPRITETYPE_SPRITE, "Mpt", i + 12 * 7, false, "Loading items" });

	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_W + i + 15 * 0, SPRITETYPE_SPRITE, "Mhr", i + 12 * 0, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_W + i + 15 * 1, SPRITETYPE_SPRITE, "Mhr", i + 12 * 1, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_W + i + 15 * 2, SPRITETYPE_SPRITE, "Mhr", i + 12 * 2, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_W + i + 15 * 3, SPRITETYPE_SPRITE, "Mhr", i + 12 * 3, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_W + i + 15 * 4, SPRITETYPE_SPRITE, "Mhr", i + 12 * 4, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_W + i + 15 * 5, SPRITETYPE_SPRITE, "Mhr", i + 12 * 5, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_W + i + 15 * 6, SPRITETYPE_SPRITE, "Mhr", i + 12 * 6, false, "Loading items" });
	for (i = 0; i < 12; i++)
		data_list.push({ SPRID_HAIR_W + i + 15 * 7, SPRITETYPE_SPRITE, "Mhr", i + 12 * 7, false, "Loading items" });

	make_sprite("WBodice1", SPRID_BODYARMOR_W + 15 * 1, 12, true, "Loading items");
	make_sprite("WBodice2", SPRID_BODYARMOR_W + 15 * 2, 12, true, "Loading items");
	make_sprite("WLArmor", SPRID_BODYARMOR_W + 15 * 3, 12, true, "Loading items");
	make_sprite("WCMail", SPRID_BODYARMOR_W + 15 * 4, 12, true, "Loading items");
	make_sprite("WSMail", SPRID_BODYARMOR_W + 15 * 5, 12, true, "Loading items");
	make_sprite("WPMail", SPRID_BODYARMOR_W + 15 * 6, 12, true, "Loading items");
	make_sprite("WRobe1", SPRID_BODYARMOR_W + 15 * 7, 12, true, "Loading items");
	make_sprite("WSanta", SPRID_BODYARMOR_W + 15 * 8, 12, true, "Loading items");
	make_sprite("WHPMail1", SPRID_BODYARMOR_W + 15 * 9, 12, true, "Loading items");       //hero
	make_sprite("WHPMail2", SPRID_BODYARMOR_W + 15 * 10, 12, true, "Loading items");      //hero
	make_sprite("WHRobe1", SPRID_BODYARMOR_W + 15 * 11, 12, true, "Loading items");       // hero
	make_sprite("WHRobe2", SPRID_BODYARMOR_W + 15 * 12, 12, true, "Loading items");       // hero
	make_sprite("AncHeroArmorW", SPRID_BODYARMOR_W + 15 * 13, 12, true, "Loading items"); //hero // Black Ancient Hero Armor W
	make_sprite("AncHeroRobeW", SPRID_BODYARMOR_W + 15 * 14, 12, true, "Loading items");  // hero // Black Ancient Hero Robe W
	make_sprite("ALegRobeW", SPRID_BODYARMOR_W + 15 * 15, 12, true, "Loading items");     // Aresden Ancient Robe W
	make_sprite("ELegRobeW", SPRID_BODYARMOR_W + 15 * 16, 12, true, "Loading items");     // Elvine Ancient Robe W
	make_sprite("ALegArmorW", SPRID_BODYARMOR_W + 15 * 17, 12, true, "Loading items");    // Aresden Ancient Armor W
	make_sprite("ELegArmorW", SPRID_BODYARMOR_W + 15 * 18, 12, true, "Loading items");    // Elvine Ancient Armor W

	make_sprite("WChemiss", SPRID_BERK_W + 15 * 1, 12, true, "Loading items");
	make_sprite("WShirt", SPRID_BERK_W + 15 * 2, 12, true, "Loading items");
	make_sprite("WHauberk", SPRID_BERK_W + 15 * 3, 12, true, "Loading items");
	make_sprite("WHHauberk1", SPRID_BERK_W + 15 * 4, 12, true, "Loading items");
	make_sprite("WHHauberk2", SPRID_BERK_W + 15 * 5, 12, true, "Loading items");
	make_sprite("AncHeroHauberkW", SPRID_BERK_W + 15 * 6, 12, true, "Loading items"); // Black Ancient Hero Hauberk W
	make_sprite("ALegHauberkW", SPRID_BERK_W + 15 * 7, 12, true, "Loading items");    // Aresden Ancient Hauberk W
	make_sprite("ELegHauberkW", SPRID_BERK_W + 15 * 8, 12, true, "Loading items");    // Elvine Ancient Hauberk W
	make_sprite("WSkirt", SPRID_LEGG_W + 15 * 1, 12, true, "Loading items");
	make_sprite("WTrouser", SPRID_LEGG_W + 15 * 2, 12, true, "Loading items");
	make_sprite("WHTrouser", SPRID_LEGG_W + 15 * 3, 12, true, "Loading items");
	make_sprite("WCHoses", SPRID_LEGG_W + 15 * 4, 12, true, "Loading items");
	make_sprite("WLeggings", SPRID_LEGG_W + 15 * 5, 12, true, "Loading items");
	make_sprite("WHLeggings1", SPRID_LEGG_W + 15 * 6, 12, true, "Loading items");
	make_sprite("WHLeggings2", SPRID_LEGG_W + 15 * 7, 12, true, "Loading items");
	make_sprite("AncHeroLegW", SPRID_LEGG_W + 15 * 8, 12, true, "Loading items");    // Black Ancient Hero Leggings W
	make_sprite("ALegLeggingsW", SPRID_LEGG_W + 15 * 9, 12, true, "Loading items");  // Aresden Ancient Leggings W
	make_sprite("ELegLeggingsW", SPRID_LEGG_W + 15 * 10, 12, true, "Loading items"); // Elvine Ancient Leggings W
	make_sprite("WShoes", SPRID_BOOT_W + 15 * 1, 12, true, "Loading items");
	make_sprite("WLBoots", SPRID_BOOT_W + 15 * 2, 12, true, "Loading items");

	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 1, SPRITETYPE_SPRITE, "Wsw", i + 56 * 0, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 2, SPRITETYPE_SPRITE, "Wsw", i + 56 * 1, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 3, SPRITETYPE_SPRITE, "Wsw", i + 56 * 2, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 4, SPRITETYPE_SPRITE, "Wsw", i + 56 * 3, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 5, SPRITETYPE_SPRITE, "Wsw", i + 56 * 4, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 6, SPRITETYPE_SPRITE, "Wsw", i + 56 * 5, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 7, SPRITETYPE_SPRITE, "Wsw", i + 56 * 6, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 8, SPRITETYPE_SPRITE, "Wsw", i + 56 * 7, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 9, SPRITETYPE_SPRITE, "Wsw", i + 56 * 8, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 10, SPRITETYPE_SPRITE, "Wsw", i + 56 * 9, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 11, SPRITETYPE_SPRITE, "Wsw", i + 56 * 10, false, "Loading items" });
	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 12, SPRITETYPE_SPRITE, "Wsw", i + 56 * 11, false, "Loading items" });

	make_sprite("Wswx", SPRID_WEAPON_W + 64 * 5, 56, true, "Loading items");
	make_sprite("Wsw2", SPRID_WEAPON_W + 64 * 13, 56, true, "Loading items");
	make_sprite("Wsw3", SPRID_WEAPON_W + 64 * 14, 56, true, "Loading items"); // TheVampire
	make_sprite("WStormBringer", SPRID_WEAPON_W + 64 * 15, 56, true, "Loading items");
	make_sprite("WDarkExec", SPRID_WEAPON_W + 64 * 16, 56, true, "Loading items");
	make_sprite("WKlonessBlade", SPRID_WEAPON_W + 64 * 17, 56, true, "Loading items");
	make_sprite("WKlonessAstock", SPRID_WEAPON_W + 64 * 18, 56, true, "Loading items");
	make_sprite("WDebastator", SPRID_WEAPON_W + 64 * 19, 56, true, "Loading items");
	make_sprite("WAxe1", SPRID_WEAPON_W + 64 * 20, 56, true, "Loading items"); // Axe
	make_sprite("WAxe2", SPRID_WEAPON_W + 64 * 21, 56, true, "Loading items");
	make_sprite("WAxe3", SPRID_WEAPON_W + 64 * 22, 56, true, "Loading items");
	make_sprite("WAxe4", SPRID_WEAPON_W + 64 * 23, 56, true, "Loading items");
	make_sprite("WAxe5", SPRID_WEAPON_W + 64 * 24, 56, true, "Loading items");
	make_sprite("WpickAxe1", SPRID_WEAPON_W + 64 * 25, 56, true, "Loading items");
	make_sprite("WAxe6", SPRID_WEAPON_W + 64 * 26, 56, true, "Loading items");
	make_sprite("Whoe", SPRID_WEAPON_W + 64 * 27, 56, true, "Loading items");
	make_sprite("WKlonessAxe", SPRID_WEAPON_W + 64 * 28, 56, true, "Loading items");
	make_sprite("WLightBlade", SPRID_WEAPON_W + 64 * 29, 56, true, "Loading items");
	make_sprite("WHammer", SPRID_WEAPON_W + 64 * 30, 56, true, "Loading items");
	make_sprite("WBHammer", SPRID_WEAPON_W + 64 * 31, 56, true, "Loading items");
	make_sprite("WBabHammer", SPRID_WEAPON_W + 64 * 32, 56, true, "Loading items");
	make_sprite("WBShadowSword", SPRID_WEAPON_W + 64 * 33, 56, true, "Loading items");
	make_sprite("WBerserkWand", SPRID_WEAPON_W + 64 * 34, 56, true, "Loading items");
	make_sprite("Wstaff1", SPRID_WEAPON_W + 64 * 35, 56, true, "Loading items");
	make_sprite("Wstaff2", SPRID_WEAPON_W + 64 * 36, 56, true, "Loading items");
	make_sprite("WStaff3", SPRID_WEAPON_W + 64 * 37, 56, true, "Loading items");
	make_sprite("WKlonessWand", SPRID_WEAPON_W + 64 * 39, 56, true, "Loading items");
	make_sprite("WReMagicWand", SPRID_WEAPON_W + 64 * 38, 56, true, "Loading items");
	make_sprite("Staff4W", SPRID_WEAPON_W + 64 * 40, 56, true, "Loading items");
	// bows 40 41 below
	make_sprite("WDirectBow", SPRID_WEAPON_W + 64 * 43, 56, true, "Loading items");
	make_sprite("WFireBow", SPRID_WEAPON_W + 64 * 44, 56, true, "Loading items");
	make_sprite("Wmantle01", SPRID_MANTLE_W + 15 * 1, 12, true, "Loading items");
	make_sprite("Wmantle02", SPRID_MANTLE_W + 15 * 2, 12, true, "Loading items");
	make_sprite("Wmantle03", SPRID_MANTLE_W + 15 * 3, 12, true, "Loading items");
	make_sprite("Wmantle04", SPRID_MANTLE_W + 15 * 4, 12, true, "Loading items");
	make_sprite("Wmantle05", SPRID_MANTLE_W + 15 * 5, 12, true, "Loading items");
	make_sprite("Wmantle06", SPRID_MANTLE_W + 15 * 6, 12, true, "Loading items");
	make_sprite("Wmantle07", SPRID_MANTLE_W + 15 * 7, 12, true, "Loading items");
	make_sprite("Wmantle08", SPRID_MANTLE_W + 15 * 8, 12, true, "Loading items");
	make_sprite("AAncHeroCapeW", SPRID_MANTLE_W + 15 * 9, 12, true, "Loading items");  // Aresden Black Ancient Hero Cape W
	make_sprite("EAncHeroCapeW", SPRID_MANTLE_W + 15 * 10, 12, true, "Loading items"); // Elvine Black Ancient Hero Cape W
	make_sprite("WHelm1", SPRID_HEAD_W + 15 * 1, 12, true, "Loading items");
	make_sprite("WHelm4", SPRID_HEAD_W + 15 * 4, 12, true, "Loading items");
	make_sprite("NWHelm1", SPRID_HEAD_W + 15 * 5, 12, true, "Loading items");
	make_sprite("NWHelm2", SPRID_HEAD_W + 15 * 6, 12, true, "Loading items");
	make_sprite("NWHelm3", SPRID_HEAD_W + 15 * 7, 12, true, "Loading items");
	make_sprite("NWHelm4", SPRID_HEAD_W + 15 * 8, 12, true, "Loading items");
	make_sprite("WHHelm1", SPRID_HEAD_W + 15 * 9, 12, true, "Loading items");
	make_sprite("WHHelm2", SPRID_HEAD_W + 15 * 10, 12, true, "Loading items");
	make_sprite("WHCap1", SPRID_HEAD_W + 15 * 11, 12, true, "Loading items");
	make_sprite("WHCap2", SPRID_HEAD_W + 15 * 12, 12, true, "Loading items");
	make_sprite("AncHeroHelmW", SPRID_HEAD_W + 15 * 13, 12, true, "Loading items"); // Black Ancient Hero Helm W
	make_sprite("AncHeroCapW", SPRID_HEAD_W + 15 * 14, 12, true, "Loading items");  // Black Ancient Hero Cap W
	make_sprite("ALegHelmW", SPRID_HEAD_W + 15 * 15, 12, true, "Loading items");    // Aresden Ancient Helm M
	make_sprite("ELegHelmW", SPRID_HEAD_W + 15 * 16, 12, true, "Loading items");    // Elvine Ancient Helm M
	make_sprite("ALegCapW", SPRID_HEAD_W + 15 * 17, 12, true, "Loading items");     // Aresden Ancient Cap M
	make_sprite("ELegCapW", SPRID_HEAD_W + 15 * 18, 12, true, "Loading items");     // Elvine Ancient Cap M
	make_sprite("Wbo", SPRID_WEAPON_W + 64 * 41, 56, true, "Loading items");        // Bow

	for (i = 0; i < 56; i++)
		data_list.push({ SPRID_WEAPON_W + i + 64 * 42, SPRITETYPE_SPRITE, "Wbo", i + 56 * 1, false, "Loading items" });

	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_W + i + 8 * 1, SPRITETYPE_SPRITE, "Wsh", i + 7 * 0, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_W + i + 8 * 2, SPRITETYPE_SPRITE, "Wsh", i + 7 * 1, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_W + i + 8 * 3, SPRITETYPE_SPRITE, "Wsh", i + 7 * 2, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_W + i + 8 * 4, SPRITETYPE_SPRITE, "Wsh", i + 7 * 3, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_W + i + 8 * 5, SPRITETYPE_SPRITE, "Wsh", i + 7 * 4, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_W + i + 8 * 6, SPRITETYPE_SPRITE, "Wsh", i + 7 * 5, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_W + i + 8 * 7, SPRITETYPE_SPRITE, "Wsh", i + 7 * 6, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_W + i + 8 * 8, SPRITETYPE_SPRITE, "Wsh", i + 7 * 7, false, "Loading items" });
	for (i = 0; i < 7; i++)
		data_list.push({ SPRID_SHIELD_W + i + 8 * 9, SPRITETYPE_SPRITE, "Wsh", i + 7 * 8, false, "Loading items" });

	make_effect_sprite("effect", 0, 10, false, "Loading effects");
	make_effect_sprite("effect2", 10, 3, false, "Loading effects");
	make_effect_sprite("effect3", 13, 6, false, "Loading effects");
	make_effect_sprite("effect4", 19, 5, false, "Loading effects");
	for (i = 0; i <= 6; i++)
		data_list.push({ i + 24, SPRITETYPE_EFFECT, "effect5", i + 1, false, "Loading effects" });
	make_effect_sprite("CruEffect1", 31, 9, false, "Loading effects");
	make_effect_sprite("effect6", 40, 5, false, "Loading effects");
	make_effect_sprite("effect7", 45, 12, false, "Loading effects");
	make_effect_sprite("effect8", 57, 9, false, "Loading effects");
	make_effect_sprite("effect9", 66, 21, false, "Loading effects");

	make_effect_sprite("effect10", 87, 2, false, "Loading effects");  // Effects Hero items
	make_effect_sprite("effect11", 89, 14, false, "Loading effects"); // Cancel, stormBlade, resu, GateHeldenian....etc
	make_effect_sprite("effect11s", 104, 1, false, "Loading effects");
	make_effect_sprite("yseffect2", 140, 8, false, "Loading effects"); // Abaddon's death
	make_effect_sprite("effect12", 148, 4, false, "Loading effects");  // Slates auras
	make_effect_sprite("yseffect3", 152, 16, false, "Loading effects");
	make_effect_sprite("yseffect4", 133, 7, false, "Loading effects"); // Abaddon's map thunder.
	make_effect_sprite("effects", 168, 1, false, "Loading effects");   // minimap ping

	//progressLabel = "Loading sounds";
	std::string path = {};
	for (i = 1; i <= 8; i++)
	{
		path = fmt::format("data\\sounds\\C{}.wav", i);
		CSoundBuffer[i].loadFromFile(path);
		m_pCSound[i].setBuffer(CSoundBuffer[i]);
	}
	for (i = 1; i <= 50; i++)
	{
		path = fmt::format("data\\sounds\\M{}.wav", i);
		MSoundBuffer[i].loadFromFile(path);
		m_pMSound[i].setBuffer(MSoundBuffer[i]);
	}
	for (i = 1; i <= 15; i++)
	{
		path = fmt::format("data\\sounds\\E{}.wav", i);
		ESoundBuffer[i].loadFromFile(path);
		m_pESound[i].setBuffer(ESoundBuffer[i]);
	}
	for (i = 9; i <= 16; i++)
	{
		path = fmt::format("data\\sounds\\C{}.wav", i);
		CSoundBuffer[i].loadFromFile(path);
		m_pCSound[i].setBuffer(CSoundBuffer[i]);
	}
	for (i = 51; i <= 100; i++)
	{
		path = fmt::format("data\\sounds\\M{}.wav", i);
		MSoundBuffer[i].loadFromFile(path);
		m_pMSound[i].setBuffer(MSoundBuffer[i]);
	}
	for (i = 16; i <= 30; i++)
	{
		path = fmt::format("data\\sounds\\E{}.wav", i);
		ESoundBuffer[i].loadFromFile(path);
		m_pESound[i].setBuffer(ESoundBuffer[i]);
	}
	for (i = 17; i <= 24; i++)
	{
		path = fmt::format("data\\sounds\\C{}.wav", i);
		CSoundBuffer[i].loadFromFile(path);
		m_pCSound[i].setBuffer(CSoundBuffer[i]);
	}
	for (i = 101; i <= 156; i++)
	{
		path = fmt::format("data\\sounds\\M{}.wav", i);
		MSoundBuffer[i].loadFromFile(path);
		m_pMSound[i].setBuffer(MSoundBuffer[i]);
	}
	for (i = 31; i <= 54; i++)
	{
		path = fmt::format("data\\sounds\\E{}.wav", i);
		ESoundBuffer[i].loadFromFile(path);
		m_pESound[i].setBuffer(ESoundBuffer[i]);
	}
	data_max = data_list.size();
}

void helbreath::UpdateScreen_OnLoading(bool bActive)
{
	draw_version = true;
	if (data_list.empty())
	{
		ChangeGameMode(GAMEMODE_ONCONNECTING);
		return;
	}

	UpdateScreen_OnLoading_Progress();

	std::string progress_label;

	int perform = 300; // (game->fps.getFPS() + 20) / 20;

	while (--perform)
	{
		if (!data_list.empty())
		{
			load_data item = data_list.front();
			data_list.pop();
			progress_label = item.label;
			if (item.sprite_type == SPRITETYPE_SPRITE)
			{
				m_pSprite[item.id] = sprite::CreateSprite(item.name, item.num, item.alpha);
			}
			else if (item.sprite_type == SPRITETYPE_TILE)
			{
				m_pTileSpr[item.id] = sprite::CreateSprite(item.name, item.num, item.alpha);
			}
			else if (item.sprite_type == SPRITETYPE_EFFECT)
			{
				m_pEffectSpr[item.id] = sprite::CreateSprite(item.name, item.num, item.alpha);
			}
		}
	}

	double percent = ((double(data_max) - data_list.size()) / double(data_max)) * 118;

	m_cLoading = percent;


	/*
	// mass convert
	mkdir("dump");
	for (int i = 0; i < MAXSPRITES; ++i)
	{
		if (m_pSprite[i])
		{
			m_pSprite[i]->_iOpenSprite();
			if (m_pSprite[i]->_localimage.getSize().x == 0)
			{
				std::cout << fmt::format("Failed to load: {} - {}\n", i, m_pSprite[i]->m_cPakFileName);
			}
			mkdir(fmt::format("dump/{}", i).c_str());
			for (int x = 0; x < m_pSprite[i]->m_iTotalFrame; ++x)
			{
				//mkdir(fmt::format("dump/{}/{}", i, x).c_str());
				std::string save = fmt::format("dump/{}/{}.png", i, x);
				//IntRect(m_stBrush[i].sx, m_stBrush[i].sy, m_stBrush[i].szx, m_stBrush[i].szy)
				sf::RenderTexture temp;
				temp.create(m_pSprite[i]->m_stBrush[x].szx, m_pSprite[i]->m_stBrush[x].szy);
				m_pSprite[i]->sprite[x].setPosition(m_pSprite[i]->m_stBrush[x].pvx, m_pSprite[i]->m_stBrush[x].pvy);
				temp.draw(m_pSprite[i]->sprite[x]);
				temp.display();
				//std::cout << fmt::format("{} Size: {} x {} - Pivot: ({}, {}) - Source: ({}, {})\n", save, m_pSprite[i]->m_stBrush[x].szx, m_pSprite[i]->m_stBrush[x].szy, m_pSprite[i]->m_stBrush[x].pvx, m_pSprite[i]->m_stBrush[x].pvy, m_pSprite[i]->m_stBrush[x].sx, m_pSprite[i]->m_stBrush[x].sy);
				temp.getTexture().copyToImage().saveToFile(save);
			}
			m_pSprite[i]->_localimage.copyToImage().saveToFile(fmt::format("dump/{}/master.png", i));
		}
	}*/
}

void helbreath::UpdateScreen_OnLoading_Progress()
{
    m_pSprite[SPRID_INTERFACE_ND_LOADING]->_iOpenSprite();
    int16_t bgx = screenwidth_v / 2 - m_pSprite[SPRID_INTERFACE_ND_LOADING]->brush[0].szx / 2;
    int16_t bgy = screenheight_v / 2 - m_pSprite[SPRID_INTERFACE_ND_LOADING]->brush[0].szy / 2;
    DrawNewDialogBox(SPRID_INTERFACE_ND_LOADING, bgx, bgy, 0, true);
    int16_t fgx = screenwidth_v / 2 - m_pSprite[SPRID_INTERFACE_ND_LOADING]->brush[1].szx / 2;
    int16_t fgy = screenheight_v / 2 - m_pSprite[SPRID_INTERFACE_ND_LOADING]->brush[1].szy / 2;
    m_pSprite[SPRID_INTERFACE_ND_LOADING]->PutSpriteFastWidth(fgx + 426 / 2, fgy + 424 / 2, 1, m_cLoading, G_dwGlobalTime);
}

void helbreath::UpdateScreen_OnMsg()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    uint64_t dwTime = G_dwGlobalTime;

    msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


    ////DIRECTX m_DDraw.ClearBackB4();//DIRECTX
    PutString(10, 10, m_cMsg, Color(255, 255, 155, 155), false, 1);
    DrawVersion();
    ////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
    m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();//DIRECTX
    if ((G_dwGlobalTime - m_dwTime) > 1500)
    {
        isItemLoaded = false;
        ChangeGameMode(GAMEMODE_ONMAINMENU);
    }
}


void helbreath::UpdateScreen_OnSelectCharacter()
{
    short sX, sY, msX, msY, msZ;
    char cLB, cRB, cMB, cTotalChar;
    char cMIresult;
    uint64_t dwTime;
    static uint64_t dwCTime;

    int iMIbuttonNum = 0;

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

		m_cCurFocus = 1;
		m_cMaxFocus = m_pCharList.size();
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 100)
        m_cGameModeCount = 100;

    if (m_cArrowPressed != 0)
    {
        switch (m_cArrowPressed)
        {
            case 2:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus)
                    m_cCurFocus = 1;
				selectedchar = m_pCharList[m_cCurFocus - 1];
                break;
            case 4:
                m_cCurFocus--;
                if (m_cCurFocus <= 0)
                    m_cCurFocus = m_cMaxFocus;
                selectedchar = m_pCharList[m_cCurFocus - 1];
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

        if (m_pCharList[m_cCurFocus - 1] != nullptr)
        {
            if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
            {
                player_name = m_pCharList[m_cCurFocus - 1]->m_cName;
                m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;
				// todo - who cares if it's valid if it exists??
                //if (m_Misc.bCheckValidString(player_name.c_str()) == true)
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
    UpdateScreen_OnSelectCharacter(sX + GetVirtualWidth() / 2 - 640 / 2, sY + GetVirtualHeight() / 2 - 480 / 2);

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
    if (m_cMenuDir > 8)
        m_cMenuDir = 1;

    m_pSprite[SPRID_INTERFACE_ND_SELECTCHAR]->DrawRGB(msX, msY, 0, dwTime);
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
                    if (m_pCharList[m_cCurFocus - 1] != nullptr)
                    {
                        if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
                        {
                            player_name = m_pCharList[m_cCurFocus - 1]->m_cName;
                            m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;
                            //if (m_Misc.bCheckValidString(player_name.c_str()) == true)
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
                if (m_pCharList[m_cCurFocus - 1] != nullptr)
                {
                    if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
                    {
                        player_name = m_pCharList[m_cCurFocus - 1]->m_cName;
                        m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;

                        //if (m_Misc.bCheckValidString(player_name.c_str()) == true)
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
                if ((m_pCharList[m_cCurFocus - 1] != nullptr) && (m_pCharList[m_cCurFocus - 1]->m_sLevel <= MAXDELETELEVEL))
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
}

// void helbreath::UpdateScreen_OnConnecting()
// {
// 	short sX, sY, msX, msY, msZ;
// 	char cLB, cRB, cMB;
// 	uint64_t dwTime = unixtime();
// 	static class CMouseInterface * pMI;
// 	static uint32_t dwMTime, dwCTime;
// 
// 	msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
// 	cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;
// 
// 
// 	if (m_cGameModeCount == 0)
// 	{
// 		m_bEnterPressed = false;
// 		m_bEscPressed = false;
// 		dwCTime = dwMTime = unixtime();
// 	}
// 	m_cGameModeCount++;
// 	if (m_cGameModeCount > 100) m_cGameModeCount = 100;
// 
// 	if (m_bEscPressed == true)
// 	{
// 
// 		if ((dwTime - m_dwTime) > 1000)
// 		{
// 			isItemLoaded = false;
// 			ChangeGameMode(GAMEMODE_ONMAINMENU);
// 			gamemode = 0;
// 			if (_socket)
// 			{
// 				_socket->stop();
// 			}
// 		}
// 		m_bEscPressed = false;
// 		return;
// 	}
// 
// 	if ((dwTime - dwMTime) > 150) dwMTime = dwTime;
// 
// 	if ((dwTime - dwCTime) > 100)
// 	{
// 		m_cMenuFrame++;
// 		dwCTime = dwTime;
// 	}
// 	if (m_cMenuFrame >= 8)
// 	{
// 		m_cMenuDirCnt++;
// 		if (m_cMenuDirCnt > 8)
// 		{
// 			m_cMenuDir++;
// 			m_cMenuDirCnt = 1;
// 		}
// 		m_cMenuFrame = 0;
// 	}
// 	if (m_cMenuDir > 8) m_cMenuDir = 1;
// 
// 	//DIRECTX m_DDraw.ClearBackB4();
// 
// 	m_bIsHideLocalCursor = true;
// 	switch (m_cMsg[0])
// 	{
// 		case '0':
// 			_Draw_UpdateScreen_OnCreateNewAccount();
// 			break;
// 
// 		case '1':
// 			sX = 146;
// 			sY = 114;
// 			_Draw_OnLogin(m_cAccountName, m_cAccountPassword, 0, 0);
// 			break;
// 
// 		case '2':
// 			_bDraw_OnCreateNewCharacter(m_cPlayerName, 0, 0, 0);
// 			break;
// 
// 		case '3':
// 			UpdateScreen_OnSelectCharacter(0, 0, 0, 0);
// 			break;
// 
// 		case '4':
// 			// Change Password
// 			UpdateScreen_OnSelectCharacter(0, 0, 0, 0, true);
// 			break;
// 
// 		case '5':
// 			//DIRECTX m_DDraw.ClearBackB4();
// 			break;
// 	}
// 	m_bIsHideLocalCursor = false;
// 
// 	////DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
// 	////DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx//DIRECTX
// 	DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162 + 80, 125 + 80, 2); // xRisenx added x+80 and y+80
// 	fmt::format_to(G_cTxt, "Connecting to Server... %3dSec", (dwTime - m_dwTime) / 1000);
// 	PutString_SprFont(172 + 35 + 80, 190 + 80, G_cTxt, 7, 0, 0); // xRisenx added x+80 and y+80
// 
// 	if ((dwTime - m_dwTime) > 7000)
// 	{
// 		PutAlignedString(180 + 80, 463 + 80, 195 + 30 + 80, UPDATE_SCREEN_ON_CONNECTING1);//"Press ESC key during long time of no"  // xRisenx added x+80 and y+80
// 		PutAlignedString(180 + 80, 463 + 80, 195 + 45 + 80, UPDATE_SCREEN_ON_CONNECTING2);//"connection and return to the main menu."  // xRisenx added x+80 and y+80
// 	}
// 	else PutAlignedString(180 + 80, 463 + 80, 195 + 30 + 80, UPDATE_SCREEN_ON_CONNECTING3);//"  Connecting to server. Please wait..."  // xRisenx added x+80 and y+80
// 	DrawVersion();
// 	////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
// 	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 8, dwTime);
// 
// 	//	if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
// }

void helbreath::UpdateScreen_OnConnecting()
{
    short sX, sY, msX, msY, msZ;
    char cLB, cRB, cMB;
    uint64_t dwTime = unixtime();
    static uint32_t dwMTime, dwCTime;

    if (m_cGameModeCount == 0)
    {
        draw_version = true;
        m_bEnterPressed = false;
        m_bEscPressed = false;
        dwCTime = dwMTime = unixtime();

        if (is_closed())
        {
            perform_connect();
        }
        else if (is_connected())
        {
            ConnectionEstablishHandler(SERVERTYPE_LOG);
            return;
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
            if (!is_closed())
            {
                close(ix::WebSocketCloseConstants::kNormalClosureCode, "cancel");
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

    m_bIsHideLocalCursor = true;
    switch (m_cMsg[0])
    {
        //         case '2':
        //             _bDraw_OnCreateNewCharacter(m_cPlayerName, 0, 0, 0);
        //             break;

        case '3':
            UpdateScreen_OnSelectCharacter(GetVirtualWidth() / 2 - 640 / 2, GetVirtualHeight() / 2 - 480 / 2);
            break;

        case '4':
            // Change Password
            UpdateScreen_OnSelectCharacter(GetVirtualWidth() / 2 - 640 / 2, GetVirtualHeight() / 2 - 480 / 2, true);
            break;

        case '5':
            //DIRECTX m_DDraw.ClearBackB4();
            break;
    }
    m_bIsHideLocalCursor = false;

    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162 + 80, 125 + 80, 2); // xRisenx added x+80 and y+80
    fmt::format_to(G_cTxt, "Connecting to Server... (%d)", (dwTime - m_dwTime) / 1000);
    PutString_SprFont(172 + 35 + 80, 190 + 80, G_cTxt, 7, 0, 0); // xRisenx added x+80 and y+80

    // 	if ((dwTime - m_dwTime) > 7000)
    // 	{
    // 		PutAlignedString(180+80, 463+80, 195+30+80, UPDATE_SCREEN_ON_CONNECTING1);//"Press ESC key during long time of no"  // xRisenx added x+80 and y+80
    // 		PutAlignedString(180+80, 463+80, 195+45+80, UPDATE_SCREEN_ON_CONNECTING2);//"connection and return to the main menu."  // xRisenx added x+80 and y+80
    // 	}
    // 	else PutAlignedString(180+80, 463+80, 195+30+80, UPDATE_SCREEN_ON_CONNECTING3);//"  Connecting to server. Please wait..."  // xRisenx added x+80 and y+80
    //DrawVersion();
    ////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 8, dwTime);
    m_stMCursor.sCursorFrame = 8;

    //	if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void helbreath::UpdateScreen_OnWaitInitData()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    uint64_t dwTime = unixtime();

    msX = m_stMCursor.sX;
    msY = m_stMCursor.sY;
    msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0;
    cRB = (m_stMCursor.RB == true) ? 1 : 0;
    cMB = (m_stMCursor.MB == true) ? 1 : 0;

    if (m_cGameModeCount == 0)
    {
        m_bEnterPressed = false;
        m_bEscPressed = false;
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100)
        m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {
        if ((dwTime - m_dwTime) > 7000)
        {
            isItemLoaded = false;
            ChangeGameMode(GAMEMODE_ONMAINMENU);
            socketmode(0);
        }
        m_bEscPressed = false;
        return;
    }

    ////DIRECTX m_DDraw.ClearBackB4();//DIRECTX
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162 + 80, 125 + 80, 2); // xRisenx added x+80 and y+80

    fmt::format_to(G_cTxt, "Waiting for response... %dsec", (dwTime - m_dwTime) / 1000);
    PutString_SprFont(172 + 44 + 80, 190 + 80, G_cTxt, 7, 0, 0); // xRisenx added x+80 and y+80
    if ((dwTime - m_dwTime) > 7000)
    {
        PutAlignedString(174 + 80, 467 + 80, 190 + 30 + 80, UPDATE_SCREEN_ON_WAIT_INIT_DATA1); //"Press ESC key during long time of no" // xRisenx added x+80 and y+80
        PutAlignedString(174 + 80, 467 + 80, 190 + 45 + 80, UPDATE_SCREEN_ON_WAIT_INIT_DATA2); //"connection and return to the main menu." // xRisenx added x+80 and y+80
    }
    else
        PutAlignedString(174 + 80, 467 + 80, 195 + 30 + 80, UPDATE_SCREEN_ON_WAIT_INIT_DATA3); //  Connecting to server. Please wait..." // xRisenx added x+80 and y+80

    m_stMCursor.sCursorFrame = 8;
}

void helbreath::UpdateScreen_OnConnectionLost()
{
	short msX, msY, msZ;
	char cLB, cRB, cMB;
	static uint64_t dwTime;

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
	PutString(172 + 50 + 80, 180 + 30 + 80, UPDATE_SCREEN_ON_CONNECTION_LOST, Color(255, 0, 0, 0));//" // xRisenx added x+80 and y+80
	DrawVersion();
	////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);

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

bool helbreath::_bDraw_OnCreateNewCharacter(std::string pName, short msX, short msY, int iPoint)		// DrawCreateCharacter
{
	bool bFlag = true;
	uint64_t dwTime = unixtime();
	int i = 0;

	////DIRECTX m_DDraw.ClearBackB4();//DIRECTX

	DrawNewDialogBox(SPRID_INTERFACE_ND_NEWCHAR, 0, 0, 0, true);
	//DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, 0, 0, 69, TRUE);
	PutString(124, 139 - 5, _BDRAW_ON_CREATE_NEW_CHARACTER1, Color(255, 255, 255, 255));//"
	PutString(96, 159 - 5, MSG_CHARACTERNAME, Color(255, 255, 255, 255));//"Character Name"
	if (m_cCurFocus != 1) PutString(239 + 3, 153 + 3, pName, Color(255, 25, 35, 25));
	PutString(119, 189 - 5, _BDRAW_ON_CREATE_NEW_CHARACTER2, Color(255, 255, 255, 255));//"
	PutString(113, 209 - 5, MSG_GENDER, Color(255, 255, 255, 255));//"Gender"
	PutString(113, 224 - 5, MSG_SKINCOLOR, Color(255, 255, 255, 255));//"Skin Color"
	PutString(113, 239 - 5, MSG_HAIRSTYLE, Color(255, 255, 255, 255));//"Hair Style"
	PutString(113, 254 - 5, MSG_HAIRCOLOR, Color(255, 255, 255, 255));//"Hair Color"
	PutString(113, 269 - 5, MSG_UNDERWEARCOLOR, Color(255, 255, 255, 255));//"Underwear Color"
	//PutAlignedString(64, 282, 245, _BDRAW_ON_CREATE_NEW_CHARACTER3, 5,5,5);
	//fmt::format_to(G_cTxt, _BDRAW_ON_CREATE_NEW_CHARACTER4,  iPoint);//" %d points"
	//PutAlignedString(64, 282, 260, G_cTxt, 15,10,10);
	PutString(113, 324, MSG_STRENGTH, Color(255, 255, 255, 255));//"Strength"
	PutString(113, 341, MSG_VITALITY, Color(255, 255, 255, 255));//"Vitality"
	PutString(113, 358, MSG_DEXTERITY, Color(255, 255, 255, 255));//"Dexterity"
	PutString(113, 375, MSG_INTELLIGENCE, Color(255, 255, 255, 255));//"Intelligence"
	PutString(113, 392, MSG_MAGIC, Color(255, 255, 255, 255));//"Magic"
	PutString(113, 409, MSG_CHARISMA, Color(255, 255, 255, 255));//"Agility"

	//DrawNewDialogBox(SPRID_INTERFACE_ND_NEWCHAR, 0, 0, 0, TRUE);
	//DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 69, TRUE);
	//PutAlignedString(64, 282, 90, _BDRAW_ON_CREATE_NEW_CHARACTER1, 5,5,5);//"
	//PutAlignedString(57, 191, 110, MSG_CHARACTERNAME, 5,5,5);//"Character Name"
	//if (m_cCurFocus != 1) PutString(197, 112, pName,Color(255,25,35,25));
	//PutAlignedString(64, 282, 140, _BDRAW_ON_CREATE_NEW_CHARACTER2, 5,5,5);//"
	//PutString(100, 160, MSG_GENDER,Color(255,5,5,5));//"Gender"
	//PutString(100, 175, MSG_SKINCOLOR,Color(255,5,5,5));//"Skin Color"
	//PutString(100, 190, MSG_HAIRSTYLE,Color(255,5,5,5));//"Hair Style"
	//PutString(100, 205, MSG_HAIRCOLOR,Color(255,5,5,5));//"Hair Color"
	//PutString(100, 220, MSG_UNDERWEARCOLOR,Color(255,5,5,5));//"Underwear Color"
	////PutAlignedString(64, 282, 245, _BDRAW_ON_CREATE_NEW_CHARACTER3, 5,5,5);
	////fmt::format_to(G_cTxt, _BDRAW_ON_CREATE_NEW_CHARACTER4,  iPoint);//" %d points"
	////PutAlignedString(64, 282, 260, G_cTxt, 15,10,10);
	//PutString(100, 275, MSG_STRENGTH,Color(255,5,5,5));//"Strength"
	//PutString(100, 292, MSG_VITALITY,Color(255,5,5,5));//"Vitality"
	//PutString(100, 309, MSG_DEXTERITY,Color(255,5,5,5));//"Dexterity"
	//PutString(100, 326, MSG_INTELLIGENCE,Color(255,5,5,5));//"Intelligence"
	//PutString(100, 343, MSG_MAGIC,Color(255,5,5,5));//"Magic"
	//PutString(100, 360, MSG_CHARISMA,Color(255,5,5,5));//"Charisma"

	for (int i = 1; i <= 6; i++)
	{
		switch (i)
		{
			case 1:
				fmt::format_to(G_cTxt, "%d", m_createStat[STAT_STR]);
				PutAlignedString(269 + 3, 307 - 3, 308 + i * 16, G_cTxt, Color(255, 255, 255, 255));
				break;
			case 2:
				fmt::format_to(G_cTxt, "%d", m_createStat[STAT_VIT]);
				PutAlignedString(269 + 3, 307 - 3, 308 + i * 16, G_cTxt, Color(255, 255, 255, 255));
				break;
			case 3:
				fmt::format_to(G_cTxt, "%d", m_createStat[STAT_DEX]);
				PutAlignedString(269 + 3, 307 - 3, 308 + i * 16, G_cTxt, Color(255, 255, 255, 255));
				break;
			case 4:
				fmt::format_to(G_cTxt, "%d", m_createStat[STAT_INT]);
				PutAlignedString(269 + 3, 307 - 3, 308 + i * 16, G_cTxt, Color(255, 255, 255, 255));
				break;
			case 5:
				fmt::format_to(G_cTxt, "%d", m_createStat[STAT_MAG]);
				PutAlignedString(269 + 3, 307 - 3, 308 + i * 16, G_cTxt, Color(255, 255, 255, 255));
				break;
			case 6:
				fmt::format_to(G_cTxt, "%d", m_createStat[STAT_CHR]);
				PutAlignedString(269 + 3, 307 - 3, 308 + i * 16, G_cTxt, Color(255, 255, 255, 255));
				break;
		}
	}

	if (pName.empty()) bFlag = false;
	if (iPoint > 0) bFlag = false;
	//if (m_Misc.bCheckValidName(pName) == false) bFlag = false;

	if (bFlag == true)																			// Create Character
	{ // pMI->AddRect(704, 558, 795, 595);			// Create
		if ((msX >= 704) && (msX <= 795) && (msY >= 558) && (msY <= 595))
		{
			m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(703, 558, 16, dwTime);
		}
	} //else {				// Add for disabled create button
//		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(0,0,18,dwTime);
//	}

	if ((msX >= 6) && (msX <= 97) && (msY >= 558) && (msY <= 595))														// Cancel
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(5, 558, 15, dwTime);

	if ((msX >= 87) && (msX <= 159) && (msY >= 447) && (msY <= 467) && (b_cWarrior == false))							// Warrior
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(87, 446, 14, dwTime);
	if ((b_cWarrior == true) && (b_cArcher == false) && (b_cMage == false) && (b_cBattleMage == false))
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(87, 446, 13, dwTime);

	if ((msX >= 164) && (msX <= 236) && (msY >= 447) && (msY <= 467) && (b_cMage == false))								// Mage
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(164, 446, 12, dwTime);
	if ((b_cMage == true) && (b_cArcher == false) && (b_cWarrior == false) && (b_cBattleMage == false))
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(164, 446, 11, dwTime);

	if ((msX >= 241) && (msX <= 313) && (msY >= 447) && (msY <= 467) && (b_cBattleMage == false))						// Battle Mage
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(241, 446, 10, dwTime);
	if ((b_cBattleMage == true) && (b_cArcher == false) && (b_cMage == false) && (b_cWarrior == false))
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(241, 446, 9, dwTime);

	if (isArcherEnabled == true)
	{
		if ((msX >= 318) && (msX <= 390) && (msY >= 447) && (msY <= 467) && (b_cArcher == false))						// Archer
			m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(318, 446, 19, dwTime);
		if ((b_cArcher == true) && (b_cBattleMage == false) && (b_cMage == false) && (b_cWarrior == false))
			m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(318, 446, 17, dwTime);
	}
	else
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(318, 446, 18, dwTime);
	}

	if ((msX >= 281) && (msX <= 297) && (msY >= 205) && (msY <= 218))
	{
		if (m_cGender == 1)
		{
			m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(282, 205, 6, dwTime);
		}
	}

	if ((msX >= 320) && (msX <= 336) && (msY >= 205) && (msY <= 218))
	{
		if (m_cGender == 2)
		{
			m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(321, 205, 4, dwTime);
		}
	}

	if (m_cGender == 2)
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(282, 205, 5, dwTime);
	}
	if (m_cGender == 1)
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(321, 205, 3, dwTime);
	}
	// Frame 1 = Left
	// Frame 2 = Right
	if ((msX >= 309) && (msY >= 220) && (msX <= 321) && (msY <= 232))		// Skin Color Left
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(309, 221, 1, dwTime);
	}
	if ((msX >= 334) && (msY >= 220) && (msX <= 346) && (msY <= 232))		// Skin Color Right
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(334, 221, 2, dwTime);
	}

	if ((msX >= 309) && (msY >= 235) && (msX <= 321) && (msY <= 247))		// Hair Style Left
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(309, 236, 1, dwTime);
	}
	if ((msX >= 334) && (msY >= 235) && (msX <= 346) && (msY <= 247))		// Hair Style Right
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(334, 236, 2, dwTime);
	}

	if ((msX >= 309) && (msY >= 250) && (msX <= 321) && (msY <= 262))		// Hair Color Left
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(309, 251, 1, dwTime);
	}
	if ((msX >= 334) && (msY >= 250) && (msX <= 346) && (msY <= 262))		// Hair Color Right
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(334, 251, 2, dwTime);
	}

	if ((msX >= 309) && (msY >= 265) && (msX <= 321) && (msY <= 277))		// Underwear Color Left
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(309, 266, 1, dwTime);
	}
	if ((msX >= 334) && (msY >= 265) && (msX <= 346) && (msY <= 277))		// Underwear Color Right
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(334, 266, 2, dwTime);
	}

	// Frame 7 = Plus
	// Frame 8 = Minus
	if ((msX >= 310) && (msY >= 324) && (msX <= 326) && (msY <= 338))		// Strength Plus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(310, 324, 7, dwTime);
	}
	if ((msX >= 330) && (msY >= 324) && (msX <= 346) && (msY <= 338))		// Strength Minus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(330, 324, 8, dwTime);
	}

	if ((msX >= 310) && (msY >= 341) && (msX <= 326) && (msY <= 355))		// Vitality Plus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(310, 341, 7, dwTime);
	}
	if ((msX >= 330) && (msY >= 341) && (msX <= 346) && (msY <= 355))		// Vitality Minus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(330, 341, 8, dwTime);
	}

	if ((msX >= 310) && (msY >= 357) && (msX <= 326) && (msY <= 371))		// Dexterity Plus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(310, 357, 7, dwTime);
	}
	if ((msX >= 330) && (msY >= 357) && (msX <= 346) && (msY <= 371))		// Dexterity Minus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(330, 357, 8, dwTime);
	}

	if ((msX >= 310) && (msY >= 373) && (msX <= 326) && (msY <= 386))		// Intelligence Plus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(310, 373, 7, dwTime);
	}
	if ((msX >= 330) && (msY >= 373) && (msX <= 346) && (msY <= 386))		// Intelligence Minus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(330, 373, 8, dwTime);
	}

	if ((msX >= 310) && (msY >= 389) && (msX <= 326) && (msY <= 403))		// Magic Plus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(310, 389, 7, dwTime);
	}
	if ((msX >= 330) && (msY >= 389) && (msX <= 346) && (msY <= 403))		// Magic Minus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(330, 389, 8, dwTime);
	}

	if ((msX >= 310) && (msY >= 405) && (msX <= 326) && (msY <= 419))		// Charisma Plus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(310, 405, 7, dwTime);
	}
	if ((msX >= 330) && (msY >= 405) && (msX <= 346) && (msY <= 419))		// Charisma Minus
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(330, 405, 8, dwTime);
	}

	/*if (m_cCurFocus == 4)
		 m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(60, 245, 68, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(60, 245, 67, dwTime);
	if (m_cCurFocus == 5)
		 m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(145, 245, 66, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(145, 245, 65, dwTime);
	if (m_cCurFocus == 6)
		 m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(230, 245, 64, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(230, 245, 63, dwTime);*/

	ShowReceivedString();

	switch (m_cGender)
	{
		case 1:	_tmp_sOwnerType = 1; break;
		case 2:	_tmp_sOwnerType = 4; break; //@@@@@@@@@@@@@@@@@!!!!!!!!!!!!!!!!!
	}
	_tmp_sOwnerType += m_cSkinCol - 1;
	_tmp_cDir = m_cMenuDir;
	_tmp_sAppr1 = 0;
	_tmp_sAppr1 = _tmp_sAppr1 | (m_cUnderCol);
	_tmp_sAppr1 = _tmp_sAppr1 | (m_cHairStyle << 8);
	_tmp_sAppr1 = _tmp_sAppr1 | (m_cHairCol << 4);
	_tmp_sAppr2 = 0;
	_tmp_sAppr3 = 0;
	_tmp_sAppr4 = 0;
	_tmp_sHeadApprValue = 0; // Re-Coding Sprite xRisenx
	_tmp_sBodyApprValue = 0; // Re-Coding Sprite xRisenx
	_tmp_sArmApprValue = 0; // Re-Coding Sprite xRisenx
	_tmp_sLegApprValue = 0; // Re-Coding Sprite xRisenx
	ZeroMemory(_tmp_cName, sizeof(_tmp_cName));
	memcpy(_tmp_cName, player_name.c_str(), 10);
	_tmp_cAction = OBJECTMOVE;
	_tmp_cFrame = m_cMenuFrame;

	_Draw_CharacterBody(611, 277, _tmp_sOwnerType);

	DrawObject_OnMove_ForMenu(0, 0, 500 + 80 + 58, 174 + 120 - 56, false, dwTime, msX, msY);

	i = 0;

	PutString(572, 129, MSG_HITPOINT, Color(255, 255, 255, 255));//"Hit Point"
	//fmt::format_to(G_cTxt, "%d", m_createStat[STAT_VIT]*3 + 2 + m_createStat[STAT_STR]/2);
	fmt::format_to(G_cTxt, "%d", m_createStat[STAT_VIT] * 8 + m_createStat[STAT_STR] * 2 + m_createStat[STAT_INT] * 2 + 8); // HP System xRisenx
	PutString(572 + 120, 129 + 16 * i++, G_cTxt, Color(255, 255, 255, 255));

	PutString(572, 145, MSG_MANAPOINT, Color(255, 255, 255, 255));//"Mana Point"
	fmt::format_to(G_cTxt, "%d", m_createStat[STAT_MAG] * 3 + 2 + m_createStat[STAT_INT] * 2); // MP System xRisenx
	PutString(572 + 120, 129 + 16 * i++, G_cTxt, Color(255, 255, 255, 255));

	PutString(572, 161, MSG_STAMINARPOINT, Color(255, 255, 255, 255));//"Stamina Point"
	fmt::format_to(G_cTxt, "%d", m_createStat[STAT_STR] + 17); // SP System xRisenx
	PutString(572 + 120, 129 + 16 * i++, G_cTxt, Color(255, 255, 255, 255));

	return bFlag;
}

void helbreath::UpdateScreen_OnCreateNewCharacter()		// DrawCreateCharacter
{
	static class CMouseInterface * pMI;
	int i = 0;
	int iMIbuttonNum;
	static int iPoint;
	char cLB, cRB, cMB, cMIresult;
	static char cName[12];
	static char cPrevFocus;
	short msX, msY, msZ;
	bool bFlag;
	static uint32_t dwMTime;
	uint64_t dwTime = unixtime();

	msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
	cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


	if (m_cGameModeCount == 0)
	{
		pMI = new class CMouseInterface;			// pMI->AddRect(xMin, yMin, xMax, yMax);
		pMI->AddRect(239, 153, 347, 172);		// Character Name Input Field

		pMI->AddRect(281, 205, 297, 218);		// Male Female Left
		pMI->AddRect(320, 205, 336, 218);		// Male Female Right

		pMI->AddRect(309, 220, 321, 232);		// Skin Color Left
		pMI->AddRect(334, 220, 346, 232);		// Skin Color Right

		pMI->AddRect(309, 235, 321, 247);		// Hair Style Left
		pMI->AddRect(334, 235, 346, 247);		// Hair Style Right

		pMI->AddRect(309, 250, 321, 262);			// Hair Color Left
		pMI->AddRect(334, 250, 346, 262);			// Hair Color Right

		pMI->AddRect(309, 265, 321, 277);			// Underwear Color Left
		pMI->AddRect(334, 265, 346, 277);			// Underwear Color Right


		pMI->AddRect(310, 324, 326, 338);			// Strength Plus
		pMI->AddRect(330, 324, 346, 338);			// Strength Minus

		pMI->AddRect(310, 341, 326, 355);			// Vitality Plus
		pMI->AddRect(330, 341, 346, 355);			// Vitality Minus

		pMI->AddRect(310, 357, 326, 371);			// Dexterity Plus
		pMI->AddRect(330, 357, 346, 371);			// Dexterity Minus

		pMI->AddRect(310, 373, 326, 386);			// Intelligence Plus
		pMI->AddRect(330, 373, 346, 386);			// Intelligence Minus

		pMI->AddRect(310, 389, 326, 403);			// Magic Plus
		pMI->AddRect(330, 389, 346, 403);			// Magic Minus

		pMI->AddRect(310, 405, 326, 419);			// Charisma Plus
		pMI->AddRect(330, 405, 346, 419);			// Charisma Minus

		pMI->AddRect(704, 558, 795, 595);			// Create
		pMI->AddRect(6, 558, 97, 595);				// Cancel

		pMI->AddRect(87, 447, 159, 467);			// Warrior
		pMI->AddRect(164, 447, 236, 467);			// Mage
		pMI->AddRect(241, 447, 313, 467);			// Battle Mage
		pMI->AddRect(318, 447, 390, 467);			// Archer

		//pMI->AddRect(180, 515, 250, 535); // 4
		//pMI->AddRect(260, 515, 330, 535); // 5
		//pMI->AddRect(180, 540, 250, 560); // 6 Battle Mage
		//pMI->AddRect(260, 540, 330, 560); // 7 Archer

		// 640 Resolution Setup Below xRisenx
		/*pMI->AddRect(65+4+80, 65+45+120, 275+4+80, 82+45+120);

		pMI->AddRect(232+4+80, 111+45+120, 274+4 -21+80, 124+45+120);
		pMI->AddRect(255+4+80, 111+45+120, 289+4 -13+80, 124+45+120);

		pMI->AddRect(232+4+80, 126+45, 274+4 -21, 139+45);
		pMI->AddRect(255+4+80, 126+45, 289+4 -13, 139+45);

		pMI->AddRect(232+4+80, 141+45, 274+4 -21, 154+45);
		pMI->AddRect(255+4+80, 141+45, 289+4 -13, 154+45);

		pMI->AddRect(232+4+80, 156+45, 274+4 -21, 169+45);
		pMI->AddRect(255+4+80, 156+45, 289+4 -13, 169+45);

		pMI->AddRect(232+4+80, 171+45, 274+4 -21, 184+45);
		pMI->AddRect(255+4+80, 171+45, 289+4 -13, 184+45);


		pMI->AddRect(232+4, 231+45, 253+4, 244+45);
		pMI->AddRect(255+4, 231+45, 276+4, 244+45);

		pMI->AddRect(232+4, 246+45, 253+4, 259+45);
		pMI->AddRect(255+4, 246+45, 276+4, 259+45);

		pMI->AddRect(232+4, 261+45, 253+4, 274+45);
		pMI->AddRect(255+4, 261+45, 276+4, 274+45);

		pMI->AddRect(232+4, 276+45, 253+4, 289+45);
		pMI->AddRect(255+4, 276+45, 276+4, 289+45);

		pMI->AddRect(232+4, 291+45, 253+4, 304+45);
		pMI->AddRect(255+4, 291+45, 276+4, 304+45);

		pMI->AddRect(232+4, 306+45, 253+4, 319+45);
		pMI->AddRect(255+4, 306+45, 276+4, 319+45);

		pMI->AddRect(384, 445, 384+72, 445+15);
		pMI->AddRect(500, 445, 500+72, 445+15);

		pMI->AddRect(60, 445, 60+72, 445+15);
		pMI->AddRect(145, 445, 145+72, 445+15);
		pMI->AddRect(230, 445, 230+72, 445+15);*/

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
		delete pMI;
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
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
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
				player_name = cName;
				ChangeGameMode(GAMEMODE_ONCONNECTING);
				m_dwConnectMode = MSGID_REQUEST_CREATENEWCHARACTER;
				ZeroMemory(m_cMsg, sizeof(m_cMsg));
				strcpy(m_cMsg, "22");
				delete pMI;
				// todo - does this need to be here
// 				if (_socket == nullptr)
// 				{
// 					boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
// 					new_connection_->socket().async_connect(endpoint,
// 						boost::bind(&helbreath::handle_connect, this,
// 							boost::asio::placeholders::error));
// 				}
// 				else
				{
					ConnectionEstablishHandler(SERVERTYPE_LOG);
				}
				return;


			case 25:
				if (m_cCurFocus != 3)
				{
					m_cCurFocus = 3;
					return;
				}
				ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
				delete pMI;
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

// todo - make login screen show a single button to Reconnect to the server
void helbreath::UpdateScreen_OnLogin()
{
	short msX, msY, msZ, sX, sY;
	char cLB, cRB, cMB;
	char cMIresult;
	int  iMIbuttonNum;
	static class CMouseInterface * pMI;
	static char  cPassword[12], cPrevFocus;
	static char cName[12];

	msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
	cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


	sX = 146;
	sY = 114;
	if (m_cGameModeCount == 0)
	{
		EndInputString();
		pMI = new class CMouseInterface;
		pMI->AddRect(328, 267, 472, 287);		// Username field
		pMI->AddRect(328, 307, 472, 327);		// Password Field
		pMI->AddRect(353, 359, 446, 397);		// Login button
		pMI->AddRect(715, 560, 795, 595);		// Exit button
		pMI->AddRect(6, 558, 146, 596);			// Create Account Button
		pMI->AddRect(154, 558, 247, 596);		// Forums Button
		pMI->AddRect(255, 558, 348, 596);		// Website Button
		pMI->AddRect(322, 337, 341, 356);		// Remember Username
		cPrevFocus = 1;
		m_cCurFocus = 1;
		m_cMaxFocus = 2;
		m_bEnterPressed = false;
		m_cArrowPressed = 0;
		ZeroMemory(cName, sizeof(cName));
		ZeroMemory(cPassword, sizeof(cPassword));
		if (b_cRemember == true)
		{
			//ReadUsername();
			strcpy(cName, m_cUserName);
			StartInputString(330, 269, 11, cName);
			if (m_cCurFocus == 1)
			{
				m_cCurFocus = 2;
				StartInputString(330, 309, 11, cPassword, true);
			}
		}
		else
		{
			StartInputString(330, 269, 11, cName);
			ClearInputString();
		}
		//ClearInputString();
	}

	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;
	/*//DIRECTX m_DDraw.ClearBackB4();
	DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 0,0,0, TRUE);*/

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

			case 3:	// Login
	/*			m_cCurFocus++;
				if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
				break;
	*/
			case 4:	// Exit Button
	/*			if (m_cCurFocus == 3) m_cCurFocus = 4;
				else if (m_cCurFocus == 4) m_cCurFocus = 3;
				break;*/
			case 5:	// Create Account Button
			case 6:	// Forums Button
			case 7: // Website Button
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
				m_cCurFocus++;
				if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
				break;
			case 2:
			case 3:
				// 			if(CheckCheating()) {
				// 				MessageBoxA(*(HWND*)m_hWnd, "Error Code: 1600\n\nClient.exe has detected an illegal program or modification.\n\nGame Closing.", "Hack detected!", MB_OK | MB_ICONERROR);
				// 				exit(1600);
				// 			}
				if ((strlen(cName) == 0) || (strlen(cPassword) == 0)) break;
// 				ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
// 				ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
// 				strcpy(m_cAccountName, cName);
// 				strcpy(m_cAccountPassword, cPassword);
				ChangeGameMode(GAMEMODE_ONCONNECTING);
				m_dwConnectMode = MSGID_REQUEST_LOGIN;
				ZeroMemory(m_cMsg, sizeof(m_cMsg));
				strcpy(m_cMsg, "11");
				delete pMI;
// 				if (_socket == nullptr)
// 				{
// 					boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
// 					new_connection_->socket().async_connect(endpoint,
// 						boost::bind(&helbreath::handle_connect, this,
// 							boost::asio::placeholders::error));
// 				}
// 				else
				{
					ConnectionEstablishHandler(SERVERTYPE_LOG);
				}
				return;
			case 4:	// Exit
	/*#ifdef SELECTSERVER
				ChangeGameMode(GAMEMODE_ONSELECTSERVER);
	#else
				ChangeGameMode(GAMEMODE_ONMAINMENU);
	#endif

				delete pMI;
				ChangeGameMode(GAMEMODE_ONQUIT);*/
				return;
			case 5: //Create Account Button
				return;
			case 6: // Forums Button
				return;
			case 7: // Website Button
				return;
		}
	}

	/* ShadowEvil - Don't need if it's main menu already.
		  Will need to add a comformation dialog to ask if user wants to exit.

		if (m_bEscPressed == TRUE)
		{	EndInputString();
			ChangeGameMode(GAMEMODE_ONMAINMENU);
			delete pMI;
			m_bEscPressed = FALSE;
			return;
		}*/

	if (cPrevFocus != m_cCurFocus)
	{
		EndInputString();
		switch (m_cCurFocus)
		{
			case 1:
				StartInputString(330, 269, 11, cName);
				break;
			case 2:
				StartInputString(330, 309, 11, cPassword, true);
				break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				break;
		}
		cPrevFocus = m_cCurFocus;
	}

	//DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);
	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
	{
		PlaySound('E', 14, 5);
		switch (iMIbuttonNum)
		{
			case 1:
				m_cCurFocus = 1;
				break;

			case 2:
				m_cCurFocus = 2;
				break;

			case 3:
				// 			if(CheckCheating()) {
				// 				MessageBoxA(*(HWND*)m_hWnd, "Error Code: 1600\n\nClient.exe has detected an illegal program or modification.\n\nGame Closing.", "Hack detected!", MB_OK | MB_ICONERROR);
				// 				exit(1600);
				// 			}
				if ((strlen(cName) == 0) || (strlen(cPassword) == 0)) break;
				EndInputString();
// 				ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
// 				ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
// 				strcpy(m_cAccountName, cName);
// 				strcpy(m_cAccountPassword, cPassword);
				ChangeGameMode(GAMEMODE_ONCONNECTING);
				m_dwConnectMode = MSGID_REQUEST_LOGIN;
				ZeroMemory(m_cMsg, sizeof(m_cMsg));
				strcpy(m_cMsg, "11");
				delete pMI;
// 				if (_socket == nullptr)
// 				{
// 					boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
// 					new_connection_->socket().async_connect(endpoint,
// 						boost::bind(&helbreath::handle_connect, this,
// 							boost::asio::placeholders::error));
// 				}
// 				else
				{
					ConnectionEstablishHandler(SERVERTYPE_LOG);
				}
				return;

			case 4: // Exit button
	/*#ifdef SELECTSERVER
				ChangeGameMode(GAMEMODE_ONSELECTSERVER); // ONMAINMENU
	#else
				ChangeGameMode(GAMEMODE_ONMAINMENU);
	#endif*/
				delete pMI;
				ChangeGameMode(GAMEMODE_ONQUIT);
				return;
			case 5:	// Create Account Button
#ifdef MAKE_ACCOUNT
				ClearContents_OnSelectCharacter();
				delete pMI;
				//ChangeGameMode(GAMEMODE_ONAGREEMENT);
				ChangeGameMode(GAMEMODE_ONCREATENEWACCOUNT);
#else
				GoHomepage();
#endif
				return;
			case 6:	// Forums Button
				GoHomepage(false);
				return;
			case 7: // Website Button
				GoHomepage(true);
				return;
			case 8:	// Check Button
// 				if (b_cRemember == true)
// 				{
// 					b_cRemember = false;
// 					WriteUsername(cName, true);
// 					return;
// 				}
// 				else
// 				{
// 					b_cRemember = true;
// 					if (sizeof(cName) > 0)
// 					{
// 						WriteUsername(cName, false);
// 					}
// 					return;
// 				}
				return;
		}
	}

	if ((msX >= 353) && (msX <= 446) && (msY >= 359) && (msY <= 397))
	{
		m_cCurFocus = 3;
	}
	else if ((msX >= 715) && (msX <= 795) && (msY >= 560) && (msY <= 595))
	{
		m_cCurFocus = 4;
	}
	else if ((msX >= 6) && (msX <= 146) && (msY >= 558) && (msY <= 596))
	{
		m_cCurFocus = 5;
	}
	else if ((msX >= 154) && (msX <= 247) && (msY >= 558) && (msY <= 596))
	{
		m_cCurFocus = 6;
	}
	else if ((msX >= 255) && (msX <= 348) && (msY >= 558) && (msY <= 596))
	{
		m_cCurFocus = 7;
	}

	_Draw_OnLogin(cName, cPassword, msX, msY, m_cGameModeCount);
}

void helbreath::UpdateScreen_OnSelectServer()
{
	short msX, msY, msZ, sX, sY;
	char cLB, cRB, cMB;
	char cMIresult;
	int  iMIbuttonNum;
	static class CMouseInterface * pMI;
	static char  cPrevFocus;
	uint64_t dwTime = unixtime();
	bool bFlag = true;

	msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
	cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


	sX = 146;
	sY = 114;
	if (m_cGameModeCount == 0)
	{
		EndInputString();

		pMI = new class CMouseInterface;
		pMI->AddRect(130, 177, 270, 198);
		pMI->AddRect(130, 199, 270, 225);
		pMI->AddRect(256, 279, 331, 308);

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
				delete pMI;
				return;

			case 2:
				ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));
				strcpy(m_cWorldServerName, "WS2");
				ChangeGameMode(GAMEMODE_ONLOGIN);
				delete pMI;
				return;

			case 3:	// Cancel
				isItemLoaded = false;
				ChangeGameMode(GAMEMODE_ONMAINMENU);
				delete pMI;
				return;
		}
	}

	if (m_bEscPressed == true)
	{
		isItemLoaded = false;
		ChangeGameMode(GAMEMODE_ONMAINMENU);
		delete pMI;
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
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
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
					delete pMI;
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
					delete pMI;
					return;
				}
				else m_cCurFocus = 2;
				break;

			case 3:
				isItemLoaded = false;
				ChangeGameMode(GAMEMODE_ONMAINMENU);
				delete pMI;
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


void helbreath::UpdateScreen_OnQuit()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    char cMIresult;
    int iMIbuttonNum;

    uint64_t dwTime = unixtime();

    if (m_cGameModeCount == 0)
    {
        m_bEscPressed = false;
        m_bEnterPressed = false;
        m_bEnterPressed = false;
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 120)
        m_cGameModeCount = 120;

    if (m_bEscPressed == true || m_bEnterPressed == true)
    {
        m_bEscPressed = false;
        m_bEnterPressed = false;
        ChangeGameMode(GAMEMODE_NULL);
        close(ix::WebSocketCloseConstants::kNormalClosureCode, "quit");
        window.close();
        return;
    }
    DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 0, 0, 0, true);
    DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 328, 83, 1, true);

    m_stMCursor.sCursorFrame = 0;

    if ((cMIresult) && (iMIbuttonNum == 1))
    {
        ChangeGameMode(GAMEMODE_NULL);
        close(ix::WebSocketCloseConstants::kNormalClosureCode, "quit");
        window.close();
        return;
    }
}

void helbreath::UpdateScreen_OnQueryForceLogin()
{
	short msX, msY, msZ;
	char cLB, cRB, cMB;
	char cMIresult;
	int  iMIbuttonNum;

	static class CMouseInterface * pMI;
	static uint32_t dwCTime;
	uint64_t dwTime = unixtime();

	msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
	cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


	if (m_cGameModeCount == 0)
	{
		pMI = new class CMouseInterface;
		pMI->AddRect(200 + 80, 244 + 80, 200 + 80 + BTNSZX, 244 + 80 + BTNSZY);	// ShadowEvil added x+80 and y+80
		pMI->AddRect(370 + 80, 244 + 80, 370 + 80 + BTNSZX, 244 + 80 + BTNSZY);	// ShadowEvil added x+80 and y+80
		/*pMI->AddRect(200,244,200+BTNSZX,244+BTNSZY);
		pMI->AddRect(370,244,370+BTNSZX,244+BTNSZY);*/
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
		delete pMI;
		m_bEscPressed = false;
		return;
	}

	//DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);
	UpdateScreen_OnSelectCharacter(GetVirtualWidth() / 2 - 640 / 2, GetVirtualHeight() / 2 - 480 / 2);
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

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
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
				delete pMI;
// 				if (_socket == nullptr)
// 				{
// 					boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
// 					new_connection_->socket().async_connect(endpoint,
// 						boost::bind(&helbreath::handle_connect, this,
// 							boost::asio::placeholders::error));
// 				}
//				else
				{
					ConnectionEstablishHandler(SERVERTYPE_LOG);
				}
				return;

			case 2:
				ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
				delete pMI;
				break;
		}
	}
	DrawVersion();
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	//if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void helbreath::UpdateScreen_OnSelectCharacter(short sX, short sY, bool bIgnoreFocus)
{
	int i;
	int iYear, iMonth, iDay, iHour, iMinute;
	__int64 iTemp1, iTemp2;
	char cTotalChar = 0;
	uint64_t dwTime = unixtime();
	DrawNewDialogBox(SPRID_INTERFACE_ND_SELECTCHAR, sX, sY, 0);
	//DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 50);

	iTemp1 = 0;
	iTemp2 = 0;
	iYear = iMonth = iDay = iHour = iMinute = 0;
	for (i = 0; i < m_pCharList.size(); i++)
	{
// 		if ((m_cCurFocus - 1 == i) && (bIgnoreFocus == false))
// 			m_pSprite[SPRID_INTERFACE_ND_SELECTCHAR]->PutTransSpriteRGB(sX + 92 + i * 138 + 36, 63 + 77, 6, 0, 0, 0, dwTime);		// Character HOVER

		if (m_pCharList[i] != nullptr)
		{
			cTotalChar++;
			switch (m_pCharList[i]->m_sSex)
			{
				case 1:	_tmp_sOwnerType = 1; break;
				case 2:	_tmp_sOwnerType = 4; break;
			}
			_tmp_sOwnerType += m_pCharList[i]->m_sSkinCol - 1;
			_tmp_cDir = m_cMenuDir;
			_tmp_sAppr1 = m_pCharList[i]->m_sAppr1;
			_tmp_sAppr2 = m_pCharList[i]->m_sAppr2;
			_tmp_sAppr3 = m_pCharList[i]->m_sAppr3;
			_tmp_sAppr4 = m_pCharList[i]->m_sAppr4;
			_tmp_iApprColor = m_pCharList[i]->m_iApprColor;
			_tmp_sHeadApprValue = m_pCharList[i]->m_sHeadApprValue; // Re-Coding Sprite xRisenx
			_tmp_sBodyApprValue = m_pCharList[i]->m_sBodyApprValue; // Re-Coding Sprite xRisenx
			_tmp_sArmApprValue = m_pCharList[i]->m_sArmApprValue; // Re-Coding Sprite xRisenx
			_tmp_sLegApprValue = m_pCharList[i]->m_sLegApprValue; // Re-Coding Sprite xRisenx
			ZeroMemory(_tmp_cName, sizeof(_tmp_cName));
			memcpy(_tmp_cName, m_pCharList[i]->m_cName.c_str(), 10);
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

			if (m_pCharList[i]->m_sSex != 0)
			{
				//if (m_Misc.bCheckValidString(m_pCharList[i]->m_cName) == true)
				{
					m_pEffectSpr[0]->PutTransSprite(sX + 156 + i * 110, sY + 145, 1, dwTime);
					DrawObject_OnMove_ForMenu(0, 0, sX + 156 + i * 110, sY + 145, false, dwTime, 0, 0);
// 					m_pEffectSpr[0]->PutTransSprite(sX + 156 + i * 138 + 36 + GetWidth() / 2 - 640 / 2, sY + 138 + 118 + 28, 1, dwTime);
// 					DrawObject_OnMove_ForMenu(0, 0, sX + 156 + i * 138 + 36 + GetWidth() / 2 - 640 / 2, sY + 138 + 118 + 28, false, dwTime, 0, 0);
					std::string name = m_pCharList[i]->m_cName;
					std::string level = fmt::format("{}", m_pCharList[i]->m_sLevel);
					std::string exp = fmt::format("{}", m_pCharList[i]->m_iExp);

                    PutFontStringSize("default", sX + 136 + i * 110, sY + 180, name, Color(255, 255, 255), 10);
                    PutFontStringSize("default", sX + 136 + 30 + i * 110, sY + 180 + 17, level, Color(255, 255, 255), 10);
                    PutFontStringSize("default", sX + 136 + 30 + i * 110, sY + 180 + 32, exp, Color(255, 255, 255), 10);

//                     switch (i)
//                     {
//                         case 0:	// Position 1
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180, name, Color(255, 255, 255), 8);
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180 + 17, level, Color(255, 255, 255), 8);
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180 + 34, exp, Color(255, 255, 255), 8);
//                             break;
//                         case 1:	// Position 2
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180, name, Color(255, 255, 255), 8);
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180 + 17, level, Color(255, 255, 255), 8);
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180 + 34, exp, Color(255, 255, 255), 8);
//                             break;
//                         case 2:	// Position 3
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180, name, Color(255, 255, 255), 8);
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180 + 17, level, Color(255, 255, 255), 8);
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180 + 34, exp, Color(255, 255, 255), 8);
//                             break;
//                         case 3:	// Position 4
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180, name, Color(255, 255, 255), 8);
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180 + 17, level, Color(255, 255, 255), 8);
//                             PutFontStringSize("default", sX + 136 + i * 110, sY + 180 + 34, exp, Color(255, 255, 255), 8);
//                             break;
//                     }
					/*fmt::format_to(G_cTxt, "%d", m_pCharList[i]->m_iExp);
					PutString(sX-10+i*161+96, sY +179 -9+118+19-2, G_cTxt,Color(255,255,255,255)); //25,35,25);*/
				}
				iTemp2 = m_pCharList[i]->m_iYear * 1000000 + m_pCharList[i]->m_iMonth * 60000 + m_pCharList[i]->m_iDay * 1700 + m_pCharList[i]->m_iHour * 70 + m_pCharList[i]->m_iMinute;
				if (iTemp1 < iTemp2)
				{
					iYear = m_pCharList[i]->m_iYear;
					iMonth = m_pCharList[i]->m_iMonth;
					iDay = m_pCharList[i]->m_iDay;
					iHour = m_pCharList[i]->m_iHour;
					iMinute = m_pCharList[i]->m_iMinute;
					iTemp1 = iTemp2;
				}
			}
		}
	}
	i = 0;

	/*DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0,0,51);
	DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0,0,52);
	DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0,0,53);
	DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0,0,54);
	DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0,0,55);*/

	// 364,289 - 539,311
	if ((m_stMCursor.sX > 364 + sX) && (m_stMCursor.sY >= 289 + sY) && (m_stMCursor.sX < 539 + sX) && (m_stMCursor.sY <= 311 + sY))
	{
		// todo - fix hover states of "buttons"
		//DrawNewDialogBox(SPRID_INTERFACE_ND_SELECTCHAR, 715, 560, 1);		// Start Game rollover
		PutAlignedString(110 + sX, 230, 290 + (0 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER1);//"
		PutAlignedString(110 + sX, 230, 290 + (1 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER2);//"
		PutAlignedString(110 + sX, 230, 290 + (2 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER3);//"
		PutAlignedString(110 + sX, 230, 290 + (3 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER4);//"
	}
	else if ((m_stMCursor.sX > 364 + sX) && (m_stMCursor.sY >= 319 + sY) && (m_stMCursor.sX < 539 + sX) && (m_stMCursor.sY <= 340 + sY))
	{
		//DrawNewDialogBox(SPRID_INTERFACE_ND_SELECTCHAR, 254, 558, 4);		// Create Rollover
		PutAlignedString(110 + sX, 230, 290 + (0 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER5);//"

	}
	else if ((m_stMCursor.sX > 364 + sX) && (m_stMCursor.sY >= 349 + sY) && (m_stMCursor.sX < 539 + sX) && (m_stMCursor.sY <= 370 + sY))
	{

		//DrawNewDialogBox(SPRID_INTERFACE_ND_SELECTCHAR, 354, 558, 3);	// Delete Rollover
		PutAlignedString(110 + sX, 230, 290 + (0 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER6);//"
		PutAlignedString(110 + sX, 230, 290 + (1 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER7);//"
		PutAlignedString(110 + sX, 230, 290 + (2 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER8);//"
		PutAlignedString(110 + sX, 230, 290 + (3 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER9);//"
		PutAlignedString(110 + sX, 230, 290 + (4 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER10);//"
		PutAlignedString(110 + sX, 230, 290 + (5 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER11);//"

	}
	else if ((m_stMCursor.sX > 364 + sX) && (m_stMCursor.sY >= 379 + sY) && (m_stMCursor.sX < 539 + sX) && (m_stMCursor.sY <= 400 + sY))
	{
		//DrawNewDialogBox(SPRID_INTERFACE_ND_SELECTCHAR, 454, 558, 2);			// Change Password Rollover
		PutAlignedString(110 + sX, 230, 290 + (0 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER12);//"
	}
	else if ((m_stMCursor.sX > 364 + sX) && (m_stMCursor.sY >= 409 + sY) && (m_stMCursor.sX < 539 + sX) && (m_stMCursor.sY <= 430 + sY))
	{
		//DrawNewDialogBox(SPRID_INTERFACE_ND_SELECTCHAR, 4, 558, 5);			// Logout
		PutAlignedString(110 + sX, 230, 290 + (0 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER13);//"
	}
	else
	{
		if (cTotalChar == 0)
		{
			PutAlignedString(110 + sX, 230, 290 + (0 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER14);//"
			PutAlignedString(110 + sX, 230, 290 + (1 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER15);//"
			PutAlignedString(110 + sX, 230, 290 + (2 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER16);//"
			PutAlignedString(110 + sX, 230, 290 + (3 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER17);//"New Character
			PutAlignedString(110 + sX, 230, 290 + (4 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER18);//"
		}
		else if (cTotalChar < 4)
		{
			PutAlignedString(110 + sX, 230, 290 + (0 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER19);//"
			PutAlignedString(110 + sX, 230, 290 + (1 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER20);
			PutAlignedString(110 + sX, 230, 290 + (2 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER21);//"
			PutAlignedString(110 + sX, 230, 290 + (3 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER22);//"
			PutAlignedString(110 + sX, 230, 290 + (4 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER23);//"Delete Character
			PutAlignedString(110 + sX, 230, 290 + (5 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER24);//"
		}
		if (cTotalChar == 4)
		{
			PutAlignedString(110 + sX, 230, 290 + (0 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER25);//"
			PutAlignedString(110 + sX, 230, 290 + (1 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER26);//"Play
			PutAlignedString(110 + sX, 230, 290 + (2 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER27);//"Delete Character
			PutAlignedString(110 + sX, 230, 290 + (3 * 15) + sY, UPDATE_SCREEN_ON_SELECT_CHARACTER28);//"
		}
	}
	int iTempMon, iTempDay, iTempHour, iTempMin;
	iTempMon = iTempDay = iTempHour = iTempMin = 0;

	//life[gm] removed hbusa dont use,
	/*if (m_iAccntYear != 0)
	{	 iTempMin =  (m_iTimeLeftSecAccount / 60 ) ;
		 fmt::format_to(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER37, m_iAccntYear, m_iAccntMonth, m_iAccntDay, iTempMin);
	}else
	{	if (m_iTimeLeftSecAccount > 0)
		{	iTempDay =   (m_iTimeLeftSecAccount /(60*60*24)) ;
			iTempHour =  (m_iTimeLeftSecAccount /(60*60)) % 24;
			iTempMin =   (m_iTimeLeftSecAccount / 60 ) % 60;
			fmt::format_to(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER38, iTempDay, iTempHour, iTempMin);
		}else strcpy(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER39);
	}
	PutAlignedString(98, 357, 385 +10, G_cTxt);*/


	if (m_iIpYear != 0)
	{
		iTempHour = (m_iTimeLeftSecIP / (60 * 60));
		iTempMin = (m_iTimeLeftSecIP / 60) % 60;
		fmt::format_to(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER40, m_iIpYear, m_iIpMonth, m_iIpDay, iTempHour, iTempMin);
	}
	else
	{
		if (m_iTimeLeftSecIP > 0)
		{
			iTempDay = (m_iTimeLeftSecIP / (60 * 60 * 24));
			iTempHour = (m_iTimeLeftSecIP / (60 * 60)) % 24;
			iTempMin = (m_iTimeLeftSecIP / 60) % 60;
			fmt::format_to(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER41, iTempDay, iTempHour, iTempMin);
		}
		else strcpy(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER42);
	}
	PutAlignedString(98 + 80 - 40, 357 + 80 - 40, 400 + 10 + 120 - 40, G_cTxt);
	if (iYear != 0)
	{
		fmt::format_to(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER43, iYear, iMonth, iDay, iHour, iMinute);
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

void helbreath::UpdateScreen_OnWaitingResponse()
{
	short sX, sY, msX, msY, msZ;
	char cLB, cRB, cMB;

	uint64_t dwTime = unixtime();
	//static class CMouseInterface * pMI;
	static uint32_t dwCTime;

	msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
	cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


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
            if (!is_closed())
            {
                socketmode(false);
                close(ix::WebSocketCloseConstants::kNormalClosureCode, "cancel");
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

		// logging in via client will not be supported
// 		case '1':
// 			sX = 146;
// 			sY = 114;
// 			_Draw_OnLogin(m_cAccountName, m_cAccountPassword, 0, 0);
// 			break;
		case '2':
			_bDraw_OnCreateNewCharacter(player_name, 0, 0, 0);
			break;
		case '3':
			UpdateScreen_OnSelectCharacter(GetVirtualWidth() / 2 - 640 / 2, GetVirtualHeight() / 2 - 480 / 2);
			break;
		case '4':// Change Password
			UpdateScreen_OnSelectCharacter(GetVirtualWidth() / 2 - 640 / 2, GetVirtualHeight() / 2 - 480 / 2, true);
			break;
		case '5':
			//DIRECTX m_DDraw.ClearBackB4();
			break;
	}
	m_bIsHideLocalCursor = false;

	////DIRECTX m_DDraw.DrawShadowBox(0,0, 639,479);
	//DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx
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
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 8, dwTime);

	//if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void helbreath::UpdateScreen_OnQueryDeleteCharacter()
{
	short msX, msY, msZ;
	char cLB, cRB, cMB;
	char cMIresult;
	int  iMIbuttonNum;

	static class CMouseInterface * pMI;
	static uint32_t dwCTime;
	uint64_t dwTime = unixtime();

	msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
	cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


	if (m_cGameModeCount == 0)
	{
		pMI = new class CMouseInterface;
		pMI->AddRect(200 + 80, 244 + 80, 200 + 80 + BTNSZX, 244 + 80 + BTNSZY);
		pMI->AddRect(370 + 80, 244 + 80, 370 + 80 + BTNSZX, 244 + 80 + BTNSZY);
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
		delete pMI;
		m_bEscPressed = false;
		return;
	}

	//DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);
	UpdateScreen_OnSelectCharacter(GetVirtualWidth() / 2 - 640 / 2, GetVirtualHeight() / 2 - 480 / 2);
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
	PutString(215 + 80, 195 + 80, UPDATE_SCREEN_ON_QUERY_DELETE_CHARACTER1, Color(255, 5, 5, 5));//"Character Name"
					   //"Character Name"
	PutString(335 + 80, 199 + 80, "__________", Color(255, 5, 5, 5));
	PutString(335 + 80, 195 + 80, m_pCharList[m_wEnterGameType - 1]->m_cName, Color(255, 25, 35, 25));
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

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
	{
		PlaySound('E', 14, 5);
		switch (iMIbuttonNum)
		{
			case 1:
				ChangeGameMode(GAMEMODE_ONCONNECTING);
				m_dwConnectMode = MSGID_REQUEST_DELETECHARACTER;
				ZeroMemory(m_cMsg, sizeof(m_cMsg));
				strcpy(m_cMsg, "33");
				delete pMI;
// 				if (_socket == nullptr)
// 				{
// 					boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
// 					new_connection_->socket().async_connect(endpoint,
// 						boost::bind(&helbreath::handle_connect, this,
// 							boost::asio::placeholders::error));
// 				}
// 				else
				{
					ConnectionEstablishHandler(SERVERTYPE_LOG);
				}
				return;

			case 2:
				ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
				delete pMI;
				break;
		}
	}
	DrawVersion();
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	//if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}


void helbreath::UpdateScreen_OnLogResMsg()
{
    short msX, msY, msZ, sX, sY;
    char  cLB, cRB, cMB;
    uint64_t dwTime = unixtime();
    static uint32_t dwCTime;
    static class CMouseInterface * pMI;
    int   iMIbuttonNum;
    char  cMIresult;

    msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;



    if (m_cGameModeCount == 0)
    {
        pMI = new class CMouseInterface;
        pMI->AddRect(451, 334, 451 + BTNSZX, 334 + BTNSZY);
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

        delete pMI;
        m_bEscPressed = false;
        return;
    }

    //DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);

    //DIRECTX m_DDraw.ClearBackB4();

    switch (m_cMsg[0])
    {
		// login not supported in client
//         case '1':
//         case '7':
//             sX = 146;
//             sY = 114;
// 
//             _Draw_OnLogin(m_cAccountName, m_cAccountPassword, 0, 0);
//             break;

        case '2':
        case '4':
            _bDraw_OnCreateNewCharacter(player_name, 0, 0, 0);
            break;

        case '3':
            sX = 0;
            sY = 0;
            UpdateScreen_OnSelectCharacter(sX, sY);
            break;

        case '6':
            sX = 146;
            sY = 114;

            UpdateScreen_OnSelectCharacter(GetVirtualWidth() / 2 - 640 / 2, GetVirtualHeight() / 2 - 480 / 2, true);
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

	if (!response.empty())
	{
        PutString_SprFont(dlgX + 72, dlgY + 43, "Error!", 7, 0, 0);
        PutAlignedString(dlgX + 15, dlgX + 300, dlgY + 74, response);

	}
	else

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
                fmt::format_to(G_cTxt, UPDATE_SCREEN_ON_LOG_MSG35, m_iBlockYear, m_iBlockMonth, m_iBlockDay);//"
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

    iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
    if (cMIresult == MIRESULT_CLICK)
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
                delete pMI;
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
    m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}


void helbreath::UpdateScreen_OnVersionNotMatch()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    char cMIresult;
    int  iMIbuttonNum;
    static class CMouseInterface * pMI;
    uint64_t dwTime = unixtime();

    msX = m_stMCursor.sX; msY = m_stMCursor.sY; msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0; cRB = (m_stMCursor.RB == true) ? 1 : 0; cMB = (m_stMCursor.MB == true) ? 1 : 0;


    if (m_cGameModeCount == 0)
    {
        if (!is_closed())
        {
            socketmode(false);
            close(ix::WebSocketCloseConstants::kNormalClosureCode, "cancel");
        }
        pMI = new class CMouseInterface;
        //pMI->AddRect(0,0,640,480);
        pMI->AddRect(0, 0, GetWidth(), GetHeight()); // 800x600 Resolution xRisenx
        m_bEnterPressed = false;
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 120) m_cGameModeCount = 120;
    //DIRECTX m_DDraw.ClearBackB4();
    if (m_bEscPressed == true || m_bEnterPressed == true)
    {
        m_bEscPressed = false;
        m_bEnterPressed = false;
        delete pMI;
        ChangeGameMode(GAMEMODE_NULL);
        SendMessage(*(HWND *)m_hWnd, WM_DESTROY, 0, 0);
        return;
    }
    DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 0, 0, 0, true);
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162, 125, 2);
    PutAlignedString(168, 474, 160, UPDATE_SCREEN_ON_VERSION_NO_MATCH1);
    PutAlignedString(168, 474, 180, UPDATE_SCREEN_ON_VERSION_NO_MATCH2);
    PutAlignedString(168, 474, 250, MSG_HOMEPAGE);
    DrawVersion();
    //DIRECTX//DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);
    m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);

    iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
    if ((cMIresult == MIRESULT_CLICK) && (iMIbuttonNum == 1))
    {
        ChangeGameMode(GAMEMODE_NULL);
        delete pMI;
        SendMessage(*(HWND *)m_hWnd, WM_DESTROY, 0, 0);
        return;
    }
    //if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}


void helbreath::UpdateScreen_OnGame()
{
	short sVal, sDivX, sModX, sDivY, sModY, sPivotX, sPivotY, sVPXsave, sVPYsave;
	static int  iUpdateRet = -1;
	static uint32_t perfVar = 0, perfCnt = 0;
	short absX, absY, tX, tY;
	uint32_t ItemColor;
	int  i, iAmount;
	uint64_t dwTime = unixtime();
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
										//m_dialogBoxes[20].CentreOverPoint(m_stMCursor.sX, m_stMCursor.sY);
										m_dialogBoxes[20].sV3 = iAmount;
										m_dialogBoxes[20].sV4 = m_wCommObjectID;
										m_dialogBoxes[20].sV5 = m_dialogBoxes[17].sV1;
										m_dialogBoxes[20].sV6 = m_dialogBoxes[17].sV2;

										ZeroMemory(m_dialogBoxes[20].cStr, sizeof(m_dialogBoxes[20].cStr));
										strcpy(m_dialogBoxes[20].cStr, m_dialogBoxes[17].cStr);
										break;

									case 20:
										EnableDialogBox(20, 3, m_dialogBoxes[17].sView, m_dialogBoxes[17].sV3);
										//m_dialogBoxes[20].//CentreOverPoint(m_stMCursor.sX, m_stMCursor.sY);
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
										//m_dialogBoxes[20].//CentreOverPoint(m_stMCursor.sX, m_stMCursor.sY);
										m_dialogBoxes[20].sV3 = iAmount;
										m_dialogBoxes[20].sV4 = m_wCommObjectID;

										ZeroMemory(m_dialogBoxes[20].cStr, sizeof(m_dialogBoxes[20].cStr));
										GetNpcName(m_dialogBoxes[17].sV3, m_dialogBoxes[20].cStr);
										break;
									case NPC_PRINCESS:
										EnableDialogBox(20, 10, m_dialogBoxes[17].sView, m_dialogBoxes[17].sV3);
										//m_dialogBoxes[20].//CentreOverPoint(m_stMCursor.sX, m_stMCursor.sY);
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
//   fmt::format_to(cEntry, "Name      Kills      Deaths");
//   PutString( 10, 100, cEntry,Color(255,255,255,255) );
//
//   for (i = 0; i < MAXARENAPLAYERS; i++)
//   {
//      //if(iEntry >= 5) break;  // only show top 5
//	   if(iEntry >= 20) break;  // Showing top 20
//      if(strlen(m_stArenaPlayers[i].cCharName) != 0)
//      {
//         iEntry++;
//         fmt::format_to(cEntry, "%s      %i     %i", m_stArenaPlayers[i].cCharName, m_stArenaPlayers[i].iKills, m_stArenaPlayers[i].iDeaths);
//         PutString( 10, 100 + (iEntry*15), cEntry,Color(255,255,255,255) );
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
		DrawObjects(sPivotX, sPivotY, sDivX, sDivY, sModX, sModY, m_stMCursor.sX, m_stMCursor.sY);

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
				m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->PutSpriteRGB(m_stMCursor.sX - m_stMCursor.sDistX, m_stMCursor.sY - m_stMCursor.sDistY,
					m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame,
					ItemColor,
					dwTime);
			}
			else
			{
				m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->PutSpriteRGB(m_stMCursor.sX - m_stMCursor.sDistX, m_stMCursor.sY - m_stMCursor.sDistY,
					m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame,
					ItemColor,
					dwTime);
			}
		}
		else m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->PutSpriteFast(m_stMCursor.sX - m_stMCursor.sDistX, m_stMCursor.sY - m_stMCursor.sDistY,
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

		//DIRECTX m_DDraw.DrawShadowBox(m_stMCursor.sX - 3, m_stMCursor.sY + 25 - 1, m_stMCursor.sX + maxWidth * 5.5, m_stMCursor.sY + 25+1 + 13*lines->size());

		iter = lines->begin();

		PutString(m_stMCursor.sX, m_stMCursor.sY + 25, *iter, m_itemColor, false, 1);
		iLoc += 13;
		++iter;

		while (iter != end)
		{
			PutString(m_stMCursor.sX, m_stMCursor.sY + 25 + iLoc, *iter, Color(255, 150, 150, 150), false, 1);
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
		//		fmt::format_to(G_cTxt, MSG_TOTAL_NUMBER, iLoc);
		//		PutString(m_stMCursor.sX, m_stMCursor.sY + 40, G_cTxt,Color(255,150,150,150), FALSE, 1);
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
		fmt::format_to(G_cTxt, "M(%d,%d) T(%d,%d)", m_stMCursor.sX, m_stMCursor.sY, (m_sViewPointX + m_stMCursor.sX) / 32, (m_sViewPointY + m_stMCursor.sY) / 32);
		PutString(m_stMCursor.sX, m_stMCursor.sY + 30, G_cTxt, Color(255, 255, 255, 255));
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
			fmt::format_to(G_cTxt, UPDATE_SCREEN_ONGAME13, m_cLogOutCount);
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
		ChangeGameMode(GAMEMODE_ONMAINMENU);
        if (!is_closed())
        {
			socketmode(false);
            close(ix::WebSocketCloseConstants::kNormalClosureCode, "cancel");
        }
		isItemLoaded = false;
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
			fmt::format_to(G_cTxt, UPDATE_SCREEN_ONGAME14, m_cRestartCount); // "Restarting game...%d"
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
	if (iUpdateRet && m_iLU_Point > 0) PutString_SprFont(GetWidth() - 80, GetHeight() - 90, "Level Up", 250, 250, 0); // 800x600 Resolution xRisenx added x+160 and y+120
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
			//PutAlignedString(0, 639, 0, "Ethereal", (m_GMFlags & GMFLAG_ETHEREAL) ?Color(255,50,220,50) :Color(255,220,50,50));
			PutAlignedString(0, GetWidth() - 1, 0, "Ethereal", (m_GMFlags & GMFLAG_ETHEREAL) ? Color(255, 50, 220, 50) : Color(255, 220, 50, 50)); // 800x600 Resolution xRisenx

			if (m_adminLevel >= 4)
			{
				//PutAlignedString(0, 639, 15, "No aggro", (m_GMFlags & GMFLAG_NOAGGRO) ?Color(255,50,220,50) :Color(255,220,50,50));
				//PutAlignedString(0, 639, 30,  "Invincible", (m_GMFlags & GMFLAG_INVINCIBLE) ?Color(255,50,220,50) :Color(255,220,50,50));
				PutAlignedString(0, GetWidth() - 1, 15, "No aggro", (m_GMFlags & GMFLAG_NOAGGRO) ? Color(255, 50, 220, 50) : Color(255, 220, 50, 50)); // 800x600 Resolution xRisenx
				PutAlignedString(0, GetWidth() - 1, 30, "Invincible", (m_GMFlags & GMFLAG_INVINCIBLE) ? Color(255, 50, 220, 50) : Color(255, 220, 50, 50)); // 800x600 Resolution xRisenx
			}
		}

		if (m_eventPot != -1)
		{
			fmt::format_to(G_cTxt, "Total pot: %d", m_eventPot);
			//PutAlignedString(0, 639, 45,  G_cTxt,Color(255,50,220,50));
			PutAlignedString(0, GetWidth() - 1, 45, G_cTxt, Color(255, 50, 220, 50)); // 800x600 Resolution xRisenx
		}

		if (m_showTime)
		{
			fmt::format_to(G_cTxt, "  %d", unixtime() / 1000 % 60);
			PutString(10, 100, G_cTxt, Color(255, 255, 255, 255), false, 1);
		}

		// 		if( m_bShowFPS )
		// 		{
		// 			fmt::format_to( G_cTxt, "FPS: %d", m_iFPS);
		// 			PutString( 10, 115, G_cTxt,Color(255,255,255,255), FALSE, 1 );
		// 			fmt::format_to( G_cTxt, "PC: %dk", perfCnt/1000 );
		// 			PutString( 10, 130, G_cTxt,Color(255,255,255,255), FALSE, 1 );
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
		{	//DIRECTX m_DDraw.PutPixel(m_stMCursor.sX, m_stMCursor.sY,   255,255,255);
			//DIRECTX m_DDraw.PutPixel(m_stMCursor.sX+1, m_stMCursor.sY, 255,255,255);
			//DIRECTX m_DDraw.PutPixel(m_stMCursor.sX-1, m_stMCursor.sY, 255,255,255);
			//DIRECTX m_DDraw.PutPixel(m_stMCursor.sX, m_stMCursor.sY+1, 255,255,255);
			//DIRECTX m_DDraw.PutPixel(m_stMCursor.sX, m_stMCursor.sY-1, 255,255,255);
		}
		else m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(m_stMCursor.sX, m_stMCursor.sY, m_stMCursor.sCursorFrame, dwTime);

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
		iUpdateRet = m_pMapData->iObjectFrameCounter(player_name, m_sViewPointX, m_sViewPointY);
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
		if (m_sNpcBar == 0)
			bNpcBar = false;
		else m_sNpcBar--;
		m_dwAuraTime = dwTime;
	}

	if ((bEffectFrameCounter() == true) && (iUpdateRet == 0)) iUpdateRet = -1;
	if (iUpdateRet == 2)
	{
		m_bCommandAvailable = true;
		m_dwCommandTime = 0;
	}
	CommandProcessor(m_stMCursor.sX, m_stMCursor.sY, ((sDivX + sPivotX) * 32 + sModX + m_stMCursor.sX - 17) / 32 + 1, ((sDivY + sPivotY) * 32 + sModY + m_stMCursor.sY - 17) / 32 + 1, m_stMCursor.LB, m_stMCursor.RB, m_stMCursor.MB);

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
