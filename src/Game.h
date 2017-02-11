// Game.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__0089D9E3_74E6_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_GAME_H__0089D9E3_74E6_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>
#include <vector>
#include <functional>
#include <mutex>

#include <process.h>
#include <direct.h>
#include <set>

#include "shared/common.h"
#include "shared/NetMessages.h"
#include "shared/npcType.h"
#include "shared/maths.h"

#include "GlobalDef.h"
#include "engine/Sprite.h"
#include "engine/SpriteID.h"
#include "Misc.h"
#include "StrTok.h"
#include "network/Msg.h"
#include "engine/Effect.h"
#include "map/MapData.h"
#include "char/ActionID.h"
#include "char/CharInfo.h"
#include "char/item/Item.h"
#include "char/Magic.h"
#include "char/Skill.h"
#include "char/DynamicObjectID.h"
#include "char/item/BuildItem.h"
#include "char/item/ItemName.h"
#include "char/Curse.h"
#include "char/guild.h"
#include "char/mail.h"
#include "ui/DialogBox.h"
#include "ui/HTMLUI.h"

#include "char/Title.h" // Titles xRisenx

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "network/streams.h"

#define BTNSZX				74
#define BTNSZY				20
#define LBTNPOSX			30
#define RBTNPOSX			154
#define BTNPOSY			292

#define MAXDIALOGBOX		9999			// ShadowEvil - Was 67


#define INDEX4_MSGID		0
#define INDEX2_MSGTYPE		4

#define SOCKETBLOCKLIMIT	300

#define MAXSPRITES			25000	// 20000 // Snoopy: Adjusted!
#define MAXTILES			1000	// 800 // Snoopy: Adjusted!
#define MAXEFFECTSPR		300
#define MAXSOUNDEFFECTS		200		// 110   //Snoopy: Adjusted!
#define MAXCHATMSGS			500
#define MAXWHISPERMSG		5
#define MAXCHATSCROLLMSGS	80
#define MAXEFFECTS			300	//600 <- original
#define CHATTIMEOUT_A		4000
#define CHATTIMEOUT_B		500
#define CHATTIMEOUT_C		2000
#define MAXITEMS			75	// 50->75 xRisenx
#define MAXMENUITEMS		140  //v2.15  120 ->140
#define TEXTDLGMAXLINES		300 //v2.18 3000->300

#define MAXMAGICTYPE		100
#define MAXWEATHEROBJECTS	600
#define MAXBUILDITEMS		100
#define MAXGAMEMSGS			300
#define MAXITEMNAMES		1000
#define MAXGUILDNAMES		100
#define MAXSELLLIST			12

#define WM_USER_GAMESOCKETEVENT	WM_USER + 2000
#define WM_USER_LOGSOCKETEVENT	WM_USER + 2001
#define WM_USER_VOTESOCKETEVENT	WM_USER + 2002

#define GAMEMODE_NULL					-2
#define GAMEMODE_ONQUIT					-1
#define GAMEMODE_ONMAINMENU				0
#define GAMEMODE_ONCONNECTING			1
#define GAMEMODE_ONLOADING				2
#define GAMEMODE_ONWAITINGINITDATA		3
#define GAMEMODE_ONMAINGAME				4
#define GAMEMODE_ONCONNECTIONLOST		5
#define GAMEMODE_ONMSG					6
#define GAMEMODE_ONCREATENEWACCOUNT		7
#define GAMEMODE_ONLOGIN				8
#define GAMEMODE_ONQUERYFORCELOGIN		9
#define GAMEMODE_ONSELECTCHARACTER		10
#define GAMEMODE_ONCREATENEWCHARACTER	11
#define GAMEMODE_ONWAITINGRESPONSE		12
#define GAMEMODE_ONQUERYDELETECHARACTER 13
#define GAMEMODE_ONLOGRESMSG			14
#define GAMEMODE_ONCHANGEPASSWORD		15
#define GAMEMODE_ONVERSIONNOTMATCH		17
#define GAMEMODE_ONINTRODUCTION			18
#define GAMEMODE_ONAGREEMENT			19
#define GAMEMODE_ONSELECTSERVER			20
#define GAMEMODE_ONINPUTKEYCODE			21

#define SERVERTYPE_GAME			1
#define SERVERTYPE_LOG			2

#define CURSORSTATUS_NULL		0
#define CURSORSTATUS_PRESSED	1
#define CURSORSTATUS_SELECTED	2
#define CURSORSTATUS_DRAGGING	3

#define SELECTEDOBJTYPE_DLGBOX	1
#define SELECTEDOBJTYPE_ITEM	2

#define DOUBLECLICKTIME			300
#define MAXCRUSADESTRUCTURES	300

#define NONE						-1
#define STAT_STR					0
#define STAT_VIT					1
#define STAT_DEX					2
#define STAT_INT					3
#define STAT_MAG					4
#define STAT_CHR					5

#define MAP_PING_DURATION		12

#define Stats_Limit				700 // Added for easy change xRisenx (Max Stats)
//#define TOTALLEVELUPPOINT		7  // Number of additional stat points player gets each level

//#define GAMEFUNCT(x) std::bind1st(std::mem_fun(&CGame::x), this)
#define GAMEFUNCT(x) std::bind(std::mem_fun(&CGame::x), this)

// ShadowEvil Defines
#define MAXITEMSTATS		1000

extern void * G_hWnd;

class connection;
typedef boost::shared_ptr<connection> connection_ptr;

#define FONT_BUILTIN 0
#define FONT_TREBMS6PX 1
#define FONT_TREBMS8PX 2
#define FONT_TREBMS10PX 3
#define FONT_TREBMS12PX 4
#define FONT_TREBMS14PX 5
#define FONT_TREBMS16PX 6


enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_WINDOW_BUTTON,
	GUI_ID_FILE_OPEN_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR
};

class FPS
{
public:
    FPS() : mFrame(0), mFps(0) {}
    void update()
    {
        if (mClock.getElapsedTime().asSeconds() >= 1.f)
        {
            mFps = mFrame;
            mFrame = 0;
            mClock.restart();
        }

        ++mFrame;
    }
    const unsigned int getFPS() const { return mFps; }

private:
    unsigned int mFrame;
    unsigned int mFps;
    sf::Clock mClock;
};

class CGame
{
public:
    FPS fps;
    struct UIMsgQueueEntry
    {
        JSValue obj;
        string message;
    };

    typedef std::deque<shared_ptr<UIMsgQueueEntry>> UIMsgQueue;
    UIMsgQueue uiqueue;
    void PutUIMsgQueue(shared_ptr<UIMsgQueueEntry> msg);
    shared_ptr<UIMsgQueueEntry> GetUIMsgQueue();

    int viewdstxvar = 0;
    int viewdstyvar = 0;
    int viewdstxcharvar = 0;
    int viewdstycharvar = 0;

	void DrawStatusText(int sX, int sY);
    void StartLogin();
    void OnEvent(sf::Event event);
    void CalcViewPointOld();
    bool calcoldviewport = true;

    bool autologin = false;
    bool autoresolution = false;
    bool autovresolution = false;

	struct MsgQueueEntry
	{
		char * data;
		uint32_t size;
	};
	typedef std::list<std::shared_ptr<MsgQueueEntry>> MsgQueue;
	MsgQueue socketpipe;
	void PutMsgQueue(MsgQueue & q, char * data, uint32_t size);
	void PutMsgQueue(std::shared_ptr<MsgQueueEntry>, MsgQueue & q);
	std::shared_ptr<MsgQueueEntry> GetMsgQueue();

    MsgQueue loginpipe;
    std::shared_ptr<MsgQueueEntry> GetLoginMsgQueue();


	connection_ptr _socket;
	void start(connection_ptr c);
	void stop(connection_ptr c);
	void handle_stop();
	void handle_connect(const boost::system::error_code& e);
	boost::asio::io_service io_service_;
	boost::asio::signal_set signals_;
	connection_ptr new_connection_;
	request_handler request_handler_;
    bool loggedin;

	std::mutex uimtx;
	std::mutex screenupdate;
    std::mutex socketmut;
    std::mutex uimut;

    void CreateSocket();

    void ProcessUI(shared_ptr<UIMsgQueueEntry>);

	boost::shared_ptr<boost::thread> socketthread;


    sf::RenderTexture visible;
    sf::RenderTexture bg;
    sf::RenderTexture charselect;
    sf::RenderTexture htmlRTT;

    uint8_t drawState = 0;

    void draw(const Drawable& drawable, const RenderStates& states = RenderStates::Default)
    {
        if (drawState == 0)
        {
            //render to visible
            visible.draw(drawable, states);
        }
        else if (drawState == 1)
        {
            //render to bg
            bg.draw(drawable, states);
        }
        else if (drawState == 2)
        {
            //render to charselect
            charselect.draw(drawable, states);
        }
    }

    void setRenderTarget(uint8_t s, bool clearbuffer = false, Color color = Color(0, 0, 0))
    {
        drawState = s;
        if (s == DS_VISIBLE)
        {
            if (clearbuffer)
                visible.clear(color);
        }
        else if (s == DS_BG)
        {
            if (clearbuffer)
                bg.clear(color);
        }
        else if (s == DS_CS)
        {
            if (clearbuffer)
                charselect.clear(color);
        }
    };

	HTMLUI* htmlUI;

	bool gamemode;

	int16_t lastchar;
	bool capslock;
	bool fullscreen;
	char oldmode;
	bool fullscreenswap;
	bool vsync;
	uint64_t foregroundfpstarget;
	uint64_t foregroundfps;
	uint64_t foregroundframetime;
	uint64_t backgroundfpstarget;
	uint64_t backgroundfps;
	uint64_t backgroundframetime;
	uint64_t time1;
	uint64_t time2;

	bool wasinactive;

    string _renderer;

    uint16_t charselectx = 0, charselecty = 0;

    sf::RenderWindow window;

    unsigned char* uibuffer;
    sf::Texture uitex;
    sf::Sprite uispr;
    char winName[256];

    bool CreateRenderer(bool fs = false)
	{
		fullscreen = fs;
		//when streaming, vsync on = screen capture nogo
		//has to use "game capture" (render hook)
		//vsync better for production though - include option for players to choose                                    \/
// 		device = createDevice(driverType,irr::core::dimension2d<uint32_t>(screenwidth, screenheight), 32, fullscreen, false, vsync, this);
// 		if (device == 0)
// 		{
// 			MessageBox(0, "Cannot create video device!", "ERROR!", MB_OK);
// 			return false; // could not create selected driver.
// 		}
		//device->setEventReceiver(this);

        sprintf(winName, "Helbreath Xtreme %u.%u.%u Renderer: %s", HBF_MAJOR, HBF_MINOR, HBF_LOWER, _renderer.c_str());

        //Style::Fullscreen
        window.create(sf::VideoMode(screenwidth, screenheight), winName, (fullscreen ? Style::Fullscreen : (Style::Resize | Style::Close)));

        if (vsync)
            window.setVerticalSyncEnabled(true);
        else
            window.setVerticalSyncEnabled(false);

        visible.create(screenwidth_v, screenheight_v);
        bg.create(screenwidth_v + 300, screenheight_v + 300);
        charselect.create(256, 256);
        htmlRTT.create(screenwidth_v, screenheight_v);

		htmlUI = new HTMLUI(this);
		htmlUI->Init();

        handle = window.getSystemHandle();
        


        //create some fonts

        sf::Font s;
        s.loadFromFile(workingdirectory + "fonts/Arya-Regular.ttf");
        _font.insert(pair<string, sf::Font>("arya", s));

        s.loadFromFile(workingdirectory + "fonts/OpenSans-Regular.ttf");
        _font.insert(pair<string, sf::Font>("default", s));

        s.loadFromFile(workingdirectory + "fonts/PoetsenOne-Regular.ttf");
        _font.insert(pair<string, sf::Font>("test", s));

        uibuffer = new unsigned char[screenwidth_v * screenheight_v * 4];
        uitex.create(screenwidth_v, screenheight_v);
        uispr.setTexture(uitex);
        
        return true;
	}
    sf::WindowHandle handle;
    void PutFontStringSize(string fontname, int iX, int iY, string text, Color color, int size);

    std::map<string, sf::Font> _font;
    sf::Text _text;

    string workingdirectory = "";


	bool IsKeyDown(sf::Keyboard::Key keyCode) const { return KeyIsDown[keyCode]; }

    bool KeyIsDown[sf::Keyboard::Key::KeyCount];

	shared_ptr<CCharInfo> selectedchar;
	
	
	bool clipmousegame = true;
	bool clipmousewindow;
	bool isactive;
	uint16_t screenwidth;
	uint16_t screenheight;
	uint16_t screenwidth_v;
	uint16_t screenheight_v;
    void SetResolution(uint16_t width, uint16_t height) { screenwidth = width; screenheight = height; }
    void SetVirtualResolution(uint16_t width, uint16_t height) { screenwidth_v = width; screenheight_v = height; }
	__forceinline uint16_t GetWidth() { return screenwidth_v; }
	__forceinline uint16_t GetHeight() { return screenheight_v; }

	void DrawScene(uint64_t time);
	//void DrawFPS2();//debug func
	void DrawVersion2();
	uint64_t mtime;


	void emptyfunc(void){};
	void ShowSoccerVictory(short sSide);
	void NotifyMsg_Soccer(char * pData);
	void DrawSoccerInfo();

	void DrawDialogBox_QuestList();
	void DlgBoxClick_QuestList();

	void NotifyMsg_Heldenian(char * pData);
	void DrawHeldenianStats();
	void TimeStamp(char * pString);
	void DrawDialogBox_GuildBank();//14
	void DlgBoxClick_GuildBank();
	void bItemDrop_GuildBank();
	
	void ReceiveGuildInfo(char * data);
	void ReceiveGuildsmanStatus(char * data);
	void GuildContributeRsp(char * data);
	void NotifyMsg_ItemToGuildBank(char * pData);
	void InitItemList_GuildBank(char * data);
	void RetrieveItemHandler_GuildBank(char * pData);

	bool m_bHappyHour;
	void DrawQuestHelper();
	void LoadFriendList();
	void SaveFriendList();
	void DrawDialogBox_FriendList();
	void DlgBoxClick_FriendList();

	uint32_t m_adminLevel;
	uint8_t m_GMFlags;
	uint32_t m_eventPot;

	uint32_t lastScreenUpdate;

	int m_iTotalFriends;
	int m_iFriendIndex;
	uint32_t lastFriendUpdate;
	struct {
		bool online;
		char friendName[13];
		bool updated;
	} friendsList[13];

	// CLEROTH - AURAS
	void CheckActiveAura(short sX, short sY, uint64_t dwTime, short sOwnerType);
	void CheckActiveAura2(short sX, short sY, uint64_t dwTime, short sOwnerType);

	// MJ Stats Change Related functions - Alastor
	void DrawDialogBox_ChangeStatsMajestic();// Change stats using majestic - Alastor
	void DlgBoxClick_ChangeStatsMajestic();			// Change stats using majestic - Alastor

	//char m_cStatChange[TOTALLEVELUPPOINT]; // Stats System xRisenx

	int m_iTeleportMapCount;
	void ResponseTeleportList(char * pData);
	void ResponseChargedTeleport(char * pData);
	void ReceiveModifyTile(char * pData);

	int m_iQuestCount;
	void ResponseQuestList(char * pData);

	void * operator new (size_t size)
	{
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
	};

	void operator delete(void * mem)
	{
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, mem);
	};

	void ItemEquipHandler(char cItemID);
	void ReleaseEquipHandler(char cEquipPos);

	void ReadSettings();
	uint32_t ReadSettingsVar(const char * var);
	void WriteSettings();
	void WriteSettingsVar(const char * var, uint32_t val);

    void DrawNewDialogBox(char cType, int sX, int sY, int iFrame, bool bIsNoColorKey = false, bool bIsTrans = false);
    int  iGetManaCost(int iMagicNo);
	void UseMagic(int iMagicNo);
	bool _bCheckMoveable( short sx, short sy );
	bool FindGuildName(char* pName, int* ipIndex);
	void bItemDrop_SkillDialog();
	void bItemDrop_IconPanel();
	void bItemDrop_TopPanel(); // New Top Bar xRisenx
	void bItemDrop_Character();
	void bItemDrop_Inventory();
	void bItemDrop_ItemUpgrade();
	void bItemDrop_SellList();
	void bItemDrop_ExchangeDialog();
	void bItemDrop_Bank();
	void bItemDrop_ExternalScreen(char cItemID, short msX, short msY);
	void CreateScreenShot();
	void CrusadeWarResult(int iWinnerSide);
	void CrusadeContributionResult(int iWarContribution);
	void CannotConstruct(int iCode);
	void DrawTopMsg();
	void SetTopMsg(const char * pString, unsigned char iLastSec);
	void DrawAstoriaStats();
	void DrawObjectFOE(int ix, int iy, int iFrame);
	void GrandMagicResult(char * pMapName, int iV1, int iV2, int iV3, int iV4, int iHP1, int iHP2, int iHP3, int iHP4) ;
	void MeteorStrikeComing(int iCode);
	void AddMapStatusInfo(char * pData, bool bIsLastData);
	void _RequestMapStatus(char * pMapName, int iMode);
	int  GetCharKind(char *str, int index);
	bool GetText(HWND hWnd,UINT msg,WPARAM wparam, LPARAM lparam);
	
	bool bReadItemNameConfigFile();

	void DisplayCommaNumber_G_cTxt(int iGold);
	
	void DrawDialogBox_ConfirmExchange();//41
	void DlgBoxClick_ConfirmExchange();
	void DrawDialogBox_YesNo();
	void DlgBoxClick_YesNo();

	void DrawDialogBox_DKMenuWeapons();// 53
	void DlgBoxClick_DKMenuWeapons();

    void DrawDialogBox_Map();


	void EnableChat(ChatType t, bool enable);
	
	void ReceiveGuildBoard(char * data);
	void ReceiveGuildBoardPost(char * data);

	void ReceiveMailbox(char * data);
	void ReceiveMailData(char * data);

	void NotifyMsgHandler(char * pData);
	void NotifyMsg_GlobalAttackMode(char * pData);
	void NotifyMsg_QuestReward(char * pData);
	void NotifyMsg_QuestContents(char * pData);
	void NotifyMsg_ItemColorChange(char * pData);
	void NotifyMsg_ItemSocketChange(char * pData);
	void NotifyMsg_DropItemFin_CountChanged(char * pData);
	void NotifyMsg_CannotGiveItem(char * pData);
	void NotifyMsg_GiveItemFin_CountChanged(char * pData);
	void NotifyMsg_SetExchangeItem(char * pData);
	void NotifyMsg_OpenExchageWindow(char * pData);
	void NotifyMsg_DownSkillIndexSet(char * pData);
	void NotifyMsg_AdminInfo(char * pData);
	void NotifyMsg_WeatherChange(char * pData);
	void NotifyMsg_FishChance(char * pData);
	void NotifyMsg_EventFishMode(char * pData);
	void NotifyMsg_NoticeMsg(char * pData);
	void NotifyMsg_RatingPlayer(char * pData);
	void NotifyMsg_CannotRating(char * pData);
	void NotifyMsg_PlayerShutUp(char * pData);
	void NotifyMsg_TimeChange(char * pData);
	void NotifyMsg_Hunger(char * pData);
	void NotifyMsg_PlayerProfile(char * pData);
	void NotifyMsg_WhisperMode(bool bActive, char * pData);
	void NotifyMsg_UnitStatus(bool bOnGame, char * pData);
	void NotifyMsg_Charisma(char * pData);
	void NotifyMsg_ItemRepaired(char * pData);
	void NotifyMsg_RepairItemPrice(char * pData);
	void NotifyMsg_CannotRepairItem(char * pData);
	void NotifyMsg_CannotSellItem(char * pData);
	void NotifyMsg_SellItemPrice(char * pData);
	void NotifyMsg_ShowMap(char * pData);
	void NotifyMsg_SkillUsingEnd(char * pData);
	void NotifyMsg_TotalUsers(char * pData);
	void NotifyMsg_EventStart(char * pData);
	void NotifyMsg_EventStarting(char * pData);
	void NotifyMsg_Casualties(char * pData);	
	void NotifyMsg_ShieldHP(char * pData);	
	void NotifyMsg_RelicInAltar(char * pData);
	void NotifyMsg_RelicGrabbed(char * pData);
	void NotifyMsg_EventWinner(char * pData);
	void NotifyMsg_MagicEffectOff(char * pData);
	void NotifyMsg_MagicEffectOn(char * pData);
	void NotifyMsg_SetItemCount(char * pData);
	void NotifyMsg_ItemDepleted_EraseItem(char * pData);
	void NotifyMsg_ServerChange(char * pData);
	void NotifyMsg_Skill(char * pData);
	void NotifyMsg_DropItemFin_EraseItem(char * pData);
	void NotifyMsg_GiveItemFin_EraseItem(char * pData);
	void NotifyMsg_EnemyKillReward(char * pData);
	void NotifyMsg_PKcaptured(char * pData);
	void NotifyMsg_PKpenalty(char * pData);
	void NotifyMsg_ItemToBank(char * pData);
	void NotifyMsg_ItemLifeSpanEnd(char * pData);
	void NotifyMsg_ItemReleased(char * pData);
	void NotifyMsg_LevelUp(char * pData);
	void NotifyMsg_SettingSuccess(char * pData); // CLEROTH - LU
	void NotifyMsg_MP(char * pData);
	void NotifyMsg_SP(char * pData);
	void NotifyMsg_SkillTrainSuccess(char * pData);
	void NotifyMsg_MagicStudyFail(char * pData);
	void NotifyMsg_MagicStudySuccess(char * pData);
	void NotifyMsg_DismissGuildsMan(char * pData);
	void NotifyMsg_NewGuildsMan(char * pData);
	void NotifyMsg_CannotJoinMoreGuildsMan(char * pData);
	void NotifyMsg_GuildDisbanded(char * pData);
	void NotifyMsg_Exp(char * pData);
	void NotifyMsg_Killed(char * pData);
	void NotifyMsg_HP(char * pData);
	void NotifyMsg_ItemPurchased(char * pData);
	void NotifyMsg_DismissGuildReject(char * pData);
	void NotifyMsg_DismissGuildApprove(char * pData);
	void NotifyMsg_JoinGuildReject(char * pData);
	void NotifyMsg_JoinGuildApprove(char * pData);
	void NotifyMsg_QueryDismissGuildPermission(char * pData);
	void NotifyMsg_QueryJoinGuildPermission(char * pData);
	void NotifyMsg_ItemObtained(char * pData);
	void NotifyMsg_ForceDisconn(char * pData);
	void NotifyMsg_FriendOnGame(char * pData);
	void NotifyMsg_RepairAllPrice(char * pData);

	void ResponsePanningHandler(char * pData);
	void UpdateScreen_OnSelectServer();
	void _SetIlusionEffect(int iOwnerH);
	int _iGetFOE(int iStatus);
	void NoticementHandler(char * pData);
	void GetItemName(char * cItemName, uint32_t dwAttribute, char *pStr1, char *pStr2, char *pStr3);
	void GetItemName(CItem * pItem, char * pStr1, char * pStr2, char * pStr3);
	std::vector<string> * GetItemName(char * cItemName, uint32_t attr, uint8_t sockets[MAXITEMSOCKETS], uint32_t count);
	std::vector<string> * GetItemName(CItem * pItem, bool isWH = false);
	void _InitOnCreateNewCharacter();
	bool _bCheckCurrentBuildItemStatus();
	bool _bCheckBuildItemStatus();
	bool _bDecodeBuildItemContents();
	void GetNpcName(short sType, char * pName);


#ifdef MAKE_ACCOUNT
	int m_iAgreeView;
	void _LoadAgreementTextContents(char cType);
	void UpdateScreen_OnAgreement();
	void UpdateScreen_OnCreateNewAccount();
#endif

	void UpdateFriendsStatus();
	void UseShortCut( int num );
	void UpdateScreen();
	void UpdateScreen_OnMainMenu();
	void UpdateScreen_OnGame();
	void UpdateScreen_OnConnecting();
	void UpdateScreen_OnWaitInitData();
// 	void MakeSprite( char* FileName, short sStart, short sCount, bool bAlphaEffect = TRUE);
// 	void MakeLegionSprite( char* FileName, short sStart, short sCount, bool bAlphaEffect = TRUE);
// 	void MakeTileSpr( char* FileName, short sStart, short sCount, bool bAlphaEffect = TRUE);
// 	void MakeLegionTileSpr( char* FileName, short sStart, short sCount, bool bAlphaEffect = TRUE);
// 	void MakeEffectSpr( char* FileName, short sStart, short sCount, bool bAlphaEffect = TRUE);
// 	void MakeLegionEffectSpr( char* FileName, short sStart, short sCount, bool bAlphaEffect = TRUE);
	void UpdateScreen_OnLoading(bool bActive);
	void UpdateScreen_OnConnectionLost();
	void UpdateScreen_OnLogin();
	void UpdateScreen_OnMsg();
	void UpdateScreen_OnQuit();
	void UpdateScreen_OnQueryForceLogin();
	void UpdateScreen_OnQueryDeleteCharacter();
	void UpdateScreen_OnWaitingResponse();
	void UpdateScreen_OnCreateNewCharacter();
	void UpdateScreen_OnSelectCharacter();
	void UpdateScreen_OnLogResMsg();
	void UpdateScreen_OnSelectCharacter(short sX, short sY, short msX, short msY, bool bIgnoreFocus = false);
	void UpdateScreen_OnChangePassword();
	void UpdateScreen_OnLoading_Progress();
	void UpdateScreen_OnVersionNotMatch();
	void NpcTalkHandler(char * pData);
	int  _iGetWeaponSkillType();
	void SetCameraShakingEffect(short sDist, int iMul = 0);
	void ClearSkillUsingStatus();
	bool bCheckItemOperationEnabled(char cItemID);
	void _DrawThunderEffect(int sX, int sY, int dX, int dY, int rX, int rY, char cType);
	void DrawLine2(int x0, int y0, int x1, int y1, int iR, int iG, int iB);
    void DrawLine(int x0, int y0, int x1, int y1, int iR, int iG, int iB, int iA);
    void DrawLine(int x0, int y0, int x1, int y1, Color color = Color(255, 255, 255, 255));
	void SetWeatherStatus(Weather type);
	void WeatherObjectFrameCounter();
	void DrawWeatherEffects();
	bool bCheckLocalChatCommand(char const * const pMsg);
	char GetOfficialMapName(char const * const pMapName, char * pName);
	int iGetLevelExp(int iLevel);
	int _iCalcTotalWeight();
	void DrawVersion(bool bAuthor = false);
	bool _bIsItemOnHand();
	void DynamicObjectHandler(char * pData);
	bool _bCheckItemByType(char cType);
	void _DrawBlackRect(int iSize);
	void DrawNpcName(   short sX, short sY, short sOwnerType, int iStatus);
	void DrawObjectName(short sX, short sY, char * pName, int iStatus);
	void PlaySound(char cType, int iNum, int iDist, long lPan = 0);
	void _RemoveChatMsgListByObjectID(int iObjectID);
	void _LoadTextDlgContents(int cType);
	int  _iLoadTextDlgContents2(int iType);
	void DrawChatMsgs(short sX, short sY, short dX, short dY);
	void RequestFullObjectData(uint16_t wObjectID);
	bool bInitSkillCfgList();
	bool bCheckImportantFile();
	void DlgBoxDoubleClick_Inventory();
	void DlgBoxDoubleClick_Character();
	void DlgBoxDoubleClick_GuideMap();
	Point GetGuideMapPos(uint32_t x, uint32_t y);
	void EraseItem(char cItemID);
	void RetrieveItemHandler(char * pData);
	void CivilRightAdmissionHandler(char * pData);
	void _Draw_CharacterBody(short sX, short sY, short sType);
	void ClearContents_OnSelectCharacter();
	void ClearContents_OnCreateNewAccount();
	void _Draw_UpdateScreen_OnCreateNewAccount();
	bool _bGetIsStringIsNumber(char * pStr);
	bool bInitMagicCfgList();
	bool __bDecodeContentsAndBuildItemForSaleList(char * pBuffer);
	void _LoadShopMenuContents(char cType);
	void PutChatScrollList(char * pMsg, char cType);
	void RequestTeleportAndWaitData();
	void DrawEffectLights();
	void PointCommandHandler(int indexX, int indexY, char cItemID = -1);
	void DrawEffects();
	void bAddNewEffect(short sType, int sX, int sY, int dX, int dY, char cStartFrame, int iV1 = 1);
	void AddEventList(char const * const pTxt, char cColor = 0, bool bDupAllow = true);
	void ShowEventList(uint64_t dwTime);
	void SetItemCount(char * pItemName, uint32_t dwCount);
	uint32_t GetItemCount(char * pItemName);
	void _ShiftGuildOperationList();
	void _PutGuildOperationList(char * pName, char cOpMode);
	void DisbandGuildResponseHandler(char * pData);
	void InitPlayerCharacteristics(char * pData);
	void CreateNewGuildResponseHandler(char * pData);
	void _GetHairColorRGB(int iColorType , int * pR, int * pG, int * pB);
	void InitGameSettings();
	void CommonEventHandler(char * pData, uint32_t size);
	bool _bCheckDraggingItemRelease(char dlgID);
	void _SetItemOrder(char cWhere, char cItemID);
	int iGetTopDialogBoxIndex();
	void DisableDialogBox(int iBoxID);
	void EnableDialogBox(int iBoxID, int cType = 0, int sV1 = 0, int sV2 = 0, const char * pString = 0);

	void InitItemList(char * pData);
	int _iCheckDlgBoxFocus(char cButtonSide);
	void GetPlayerTurn();
	bool __fastcall DrawObject_OnDead(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool __fastcall DrawObject_OnDying(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool __fastcall DrawObject_OnMagic(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool __fastcall DrawObject_OnAttack(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool __fastcall DrawObject_OnAttackMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool __fastcall DrawObject_OnStop(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool __fastcall DrawObject_OnMove_ForMenu(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool __fastcall DrawObject_OnMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool __fastcall DrawObject_OnDamageMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool __fastcall DrawObject_OnRun(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool __fastcall DrawObject_OnDamage(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool __fastcall DrawObject_OnGetItem(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY);
	bool bEffectFrameCounter();
	void ClearGuildNameList();
	void DrawBackground(short sDivX, short sModX, short sDivY, short sModY);
	void DrawChatMsgBox(short sX, short sY, int iChatIndex, bool bIsPreDC);
	void ReleaseTimeoverChatMsg();
	void ChatMsgHandler(char * pData);
	void ReleaseUnusedSprites();
	bool bReadIp();
	void OnKeyUp(uint32_t wParam);
	void OnSysKeyDown(WPARAM wParam);
	void OnSysKeyUp(WPARAM wParam);
	void ChangeGameMode(char cMode);
	void PutFontString(string fontname, int iX, int iY, string pString, Color color = Color(255, 255, 255, 255));
	void PutChatString(int iX, int iY, char * pString, Color color = Color(255, 255, 255, 255));
    void PutString(int iX, int iY, string pString, Color color = Color(255, 255, 255, 255), bool bHide = false, char cBGtype = 2);
    void PutString2(int iX, int iY, string pString, int r = 255, int g = 255, int b = 255)
    {//TODO: remove
        PutString(iX, iY, pString, Color(r, g, b, 255));
    }
	void PutAlignedString(int iX1, int iX2, int iY, string text, Color color = Color(255,255,255,255));
    __inline sf::Font & GetFont(string font = "default")
    {
        try
        {
            return _font.at(font);
        }
        catch (std::out_of_range & oor)
        {
            //TODO: error
        }
        return sf::Font();
    }
	void PutString_SprFont(int iX, int iY, char * pStr, short sR, short sG, short sB);
	void PutString_SprFont2(int iX, int iY, char * pStr, short sR, short sG, short sB);
	void PutString_SprFont3(int iX, int iY, char * pStr, short sR, short sG, short sB, bool bTrans = false, int iType = 0);
	void PutString_SprNum(int iX, int iY, char * pStr, short sR, short sG, short sB);
	void LogResponseHandler(uint32_t size, char * pData);
	void OnTimer();
	void _ReadMapData(short sPivotX, short sPivotY, char * pData);
	void MotionEventHandler(char * pData);
	void InitDataResponseHandler(char * pData);
	void InitPlayerResponseHandler(char * pData);
	void ConnectionEstablishHandler(char cWhere);
	void MotionResponseHandler(char * pData);
	void GameRecvMsgHandler(uint32_t dwMsgSize, char * pData);
	void DrawObjects(short sPivotX, short sPivotY, short sDivX, short sDivY, short sModX, short sModY, short msX, short msY);
	bool bSendCommand(uint32_t dwMsgID, uint16_t wCommand = 0, char cDir = 0, int iV1 = 0, int iV2 = 0, int iV3 = 0, char const * const pString = 0, int iV4 = 0); 
	bool SendLoginCommand(uint32_t dwMsgID); 
	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, bool bMoveCheck = false, bool isMIM = false);
	void CommandProcessor(short msX, short msY, short indexX, short indexY, char cLB, char cRB, char cMB);
	void CalcViewPoint(uint64_t dwTime);
	void OnKeyDown(uint32_t wParam);
	void Quit();
	bool bInit(void * hWnd, void * hInst, char * pCmdLine);

	void ReserveFightzoneResponseHandler(char * pData);
	int _iGetAttackType();
	bool __bDecodeBuildItemContents(char * pBuffer);
	int _iGetBankItemCount();
	int _iGetTotalItemNum();
	uint32_t GetGold() const;
	void SetGold(uint32_t val);
	LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	void GoHomepage(bool _web);
	void StartBGM();

	//Snoopy: added function:
	void DebugLog(char * cStr);
	//int bHasHeroSet( short Appr3, short Appr4, char OwnerType);
	int bHasHeroSet(short HeadApprValue, short BodyApprValue, short ArmApprValue, short LegApprValue, char OwnerType);
	void ShowHeldenianVictory(short sSide);
	void ResponseHeldenianTeleportList(char *pData);
	void DKGlare(int iWeaponColor, int iWeaponIndex, int *iWeaponGlare);
	void DrawDruncncity();
	void Abaddon_corpse(int sX, int sY);
	void DrawAngel(int iSprite, short sX, short sY, char cFrame, uint64_t dwTime);

	void LoadMuteList();
	void SaveMuteList();

	char MouseOverDialog();
	void SetupDialogBox(int dialog,  short X, short Y, short background, int backFrame, int titleTxtFrame = -1, bool trans = false);

	void ClearPartyMembers();

	bool _ItemDropHistory(char * ItemName);
	CGame();
	virtual ~CGame();

	struct {
		short sX;
		short sY;
		float sZ;
		bool LB;
		bool RB;
		bool MB;
		short sCursorFrame;
		char  cPrevStatus;
		char  cSelectedObjectType;
		short sSelectedObjectID;
		short sPrevX, sPrevY, sDistX, sDistY;
		uint64_t dwSelectClickTime;
		short sClickX, sClickY;
	} m_stMCursor;

	CDialogBox m_dialogBoxes[MAXDIALOGBOX];

	char m_cDialogBoxOrder[MAXDIALOGBOX];
	bool m_bIsDialogEnabled[MAXDIALOGBOX];//was BOOL
	ItemMapIter m_guildBankIt;
	
	struct {
		int   sV1, sV2, sV3, sV4, sV5, sV6, sV7, sItemID;
		uint32_t dwV1;
		uint8_t sockets[MAXITEMSOCKETS];
		char  cStr1[32], cStr2[32];
	} m_stDialogBoxExchangeInfo[8];

	struct {
		int iIndex;
		int iAmount;
	} m_stSellItemList[MAXSELLLIST];

	struct {
		char cName[22];
		char cOpMode;
	} m_stGuildOpList[100];

	struct {
		uint64_t dwTime;
		char  cColor;
		char  cTxt[96];
	} m_stEventHistory[6];

	struct {
		uint64_t dwTime;
		char  cColor;
		char  cTxt[96];
	} m_stEventHistory2[6];

	struct {
		short sX, sY, sBX;
		char cStep;
	} m_stWeatherObject[MAXWEATHEROBJECTS];

	struct MapPing{
		Point pos;
		time_t t;

		MapPing(uint32_t x, uint32_t y)
		{
			pos.x = x;
			pos.y = y;
			t = time(0);
		}

		static bool Expired(const MapPing ping)
		{
			return (time(0) - ping.t >= MAP_PING_DURATION) ? true : false;
		}
	};

	std::vector<MapPing> m_mapPings;
	void AddMapPing(MapPing ping);

	struct {
		bool bIsQuestCompleted;
		short sWho, sQuestType, sContribution, sTargetType, /*sTargetCount,*/ sX, sY, sRange;
		//int sWho, sQuestType, sContribution, sTargetType, /*sTargetCount,*/ sX, sY, sRange;
		//short sCurrentCount; // by Snoopy
		int sCurrentCount, sTargetCount; // Changed to int xRisenx
		char cTargetName[22];
	} m_stQuest;

	struct partyMember{
		short sX, sY;
		string cName;
		partyMember(char * inName) : cName(inName), sX(0), sY(0) {} 
	}; 

	std::vector<partyMember *> m_stPartyMember;
	
	std::vector<Mail*> m_mails;
	std::vector<Mail*> m_gldBoard;

	struct {
		short sX, sY;
		char cType;
		char cSide;
	} m_stCrusadeStructureInfo[MAXCRUSADESTRUCTURES];

	struct {
		uint64_t dwRefTime;
		int iGuildRank;
		char cCharName[12];
		char cGuildName[24];
	} m_stGuildName[MAXGUILDNAMES];

	struct {
		int iIndex;
		char mapname[12];
		int iX;
		int iY;
		int iCost;
		bool donateEvent;
	} m_stTeleportList[20];

	struct {
		int iIndex;
		int Type;
		int RewardAmount;
		int Contribution;
		int TargetType;
		int MaxCount;
		int sX;
		int sY;
		int Range;
		char TargetName[22];
	} m_stQuestList[50];

    sf::Sound m_pCSound[30];
    sf::Sound m_pMSound[160];
    sf::Sound m_pESound[55];
    sf::SoundBuffer CSoundBuffer[30];
    sf::SoundBuffer MSoundBuffer[160];
    sf::SoundBuffer ESoundBuffer[55];
    sf::SoundBuffer bgmbuffer;
    sf::Sound m_pBGM;
	class CMisc      m_Misc;
	class CSprite  * m_pSprite[MAXSPRITES];
	class CSprite  * m_pTileSpr[MAXTILES];
	class CSprite  * m_pEffectSpr[MAXEFFECTSPR];
	class CMapData * m_pMapData;
	class CMsg    * m_pChatMsgList[MAXCHATMSGS];

	std::vector<CMsg*> * m_chatDisplay;
	std::vector<CMsg*> m_chatMsgs[CHAT_MAX];
	bool m_chatToggle[CHAT_MAX];

	class CMsg    * m_pWhisperMsg[MAXWHISPERMSG];
	class CEffect * m_pEffectList[MAXEFFECTS];
	class CItem   * m_pItemList[MAXITEMS];
	class CItem   * m_pBankList[MAXBANKITEMS];
	ItemMap m_guildBankMap;
	uint32_t m_guildBankVer;
	class CMagic * m_pMagicCfgList[MAXMAGICTYPE];
	class CSkill * m_pSkillCfgList[MAXSKILLTYPE];
	class CMsg * m_pMsgTextList[TEXTDLGMAXLINES];
	class CMsg * m_pMsgTextList2[TEXTDLGMAXLINES];
	class CMsg * m_pAgreeMsgTextList[TEXTDLGMAXLINES];
	class CBuildItem * m_pBuildItemList[MAXBUILDITEMS];
	class CBuildItem * m_pDispBuildItemList[MAXBUILDITEMS];


	class CItem * m_pItemForSaleList[MAXMENUITEMS];
	vector<shared_ptr<CCharInfo>> m_pCharList;
	//class CCharInfo * m_pCharList[10];
	class CItemName * m_pItemNameList[MAXITEMNAMES];
	class CCurse m_curse;

    uint64_t G_dwGlobalTime;
    uint64_t m_dwCommandTime; //v2.15 SpeedHack
    uint64_t m_dwConnectMode;
    uint64_t m_dwTime;
    uint64_t m_dwCurTime;
	uint64_t m_dwCheckConnTime, m_dwCheckSprTime, m_dwCheckChatTime;
    uint64_t m_dwDialogCloseTime;
    uint64_t  m_dwLogOutCountTime;//was DWORD
    uint64_t m_dwRestartCountTime;
    uint64_t m_dwWOFtime; //v1.4
    uint64_t m_dwObserverCamTime;
    uint64_t m_dwDamagedTime;
    uint64_t m_dwSpecialAbilitySettingTime;
    uint64_t m_dwCommanderCommandRequestedTime;
    uint64_t m_dwTopMsgTime;
    uint64_t m_dwEnvEffectTime;
    uint64_t viewporttime = 0;

	//v2.2
    uint64_t m_dwMonsterEventTime;
	short m_sMonsterID;
	short m_sEventX, m_sEventY;

	bool m_shieldDisabled;
	bool m_armorDisabled;
	bool m_magicDisabled[MAXMAGICTYPE];

	Side m_side;

	bool m_bZoomMap;
	bool m_bIsProgramActive;
	int m_bCommandAvailable;//was BOOL
	bool m_bSoundFlag;
	bool m_bSoundStat, m_bMusicStat; // On/Off
	bool m_bIsItemEquipped[MAXITEMS];//was BOOL
	bool m_bIsItemDisabled[MAXITEMS];//was BOOL
	bool m_bIsGetPointingMode;//was BOOL
	bool m_bEnterPressed, m_bEscPressed, m_bCtrlPressed, m_bRunningMode, m_bShiftPressed, m_altPressed;

	bool m_bDialogTrans;
	bool m_bIsCombatMode;
	bool m_bIsSafeAttackMode;
	bool m_bSkillUsingStatus;//was BOOL
	bool m_bItemUsingStatus;//was BOOL
	bool m_bSuperAttackMode;	//
	bool m_bIsObserverMode, m_bIsObserverCommanded;
	bool m_bIsPoisoned;//was BOOL
	bool m_bIsFirstConn;
	bool m_bIsConfusion;
	bool m_bIsRedrawPDBGS;
	bool m_bDrawFlagDir;
	bool m_bIsCrusadeMode;
	bool m_bIsAstoriaMode;
	Casualties m_astoriaStats[MAXSIDES];
	//uint16 m_astoriaShieldsHP[MAXSIDES]; // Shield xRisenx
	uint64_t m_astoriaShieldsHP[MAXSIDES]; // Shield xRisenx
	uint64_t m_relicOwnedTime;
	Side m_relicOwnedSide;
	bool m_bIsSpecialAbilityEnabled;//was BOOL
	bool m_bToggleScreen;
	bool m_bIsSpecial;
	Color m_itemColor;

	bool m_bIsF1HelpWindowEnabled;
	bool m_bIsTeleportRequested;//was BOOL
	bool m_bIsPrevMoveBlocked;
	bool m_bIsHideLocalCursor;

	bool m_bForceDisconn;//was BOOL
	bool m_targetNeutral;
	bool m_bForceAttack;
	bool m_bParalyze;

	int m_iFPS;
	uint64_t m_dwFPStime;
	bool  m_bShowFPS;
	bool  m_showTime;
	bool  m_showGrid;
	bool  m_showAllDmg;
	bool  m_bigItems;
	bool  m_showTimeStamp;
	bool  m_windowsKey;
	bool  m_displayQuest;

	int m_iFightzoneNumber;
	int m_iFightzoneNumberTemp;
	int m_iPlayerApprColor; 
	int m_iHP;//was int			// Hit Point
	int m_iMP;//was int			// Mana Point
	int m_iSP;//was int			// Staminar Point
	//int m_iAC;						// Armour Class
	int m_iTHAC0;					// To Hit Armour Class 0
	int m_iDefenseRatio; // Auto updates defense ingame xRisenx

    uint64_t m_iExp;
	int m_iLevel, m_iContribution, m_iLucky;
	int m_stat[6];
	//int m_angelStat[STAT_STR], m_angelStat[STAT_INT], m_angelStat[STAT_DEX], m_angelStat[STAT_MAG];
	int m_angelStat[6];

	char m_createStat[6];

	//int m_luStat[6];

	short m_luStat[6];

	int m_iLU_Point;

	int m_iEnemyKillCount;
	int m_iEnemyKillTotalCount;
	int m_iPKCount;
	int m_iRewardGold;
	CGuild m_guild;
	GuildRank m_iGuildRank;
	int m_iTotalGuildsMan;
	int m_iPointCommandType;
	int m_iTotalChar;
//	int m_iAccountStatus;
	short m_sMagicShortCut;
	int m_iCameraShakingDegree;
	int m_iSuperAttackLeft;
	int m_iAccntYear, m_iAccntMonth, m_iAccntDay;
	int m_iIpYear, m_iIpMonth, m_iIpDay;
	int m_iDownSkillIndex;

	int m_iIlusionOwnerH;
	int m_iApprColor_IE;
	int m_iPDBGSdivX, m_iPDBGSdivY;			   // Pre-Draw Background Surface
	short m_sRecentShortCut;
	short m_sShortCut[6]; // Snoopy: 6 shortcuts
	uint64_t	m_iSpecialAbilityTimeLeftSec;
	int m_iDrawFlag;
	int m_iSpecialAbilityType;
	uint64_t m_iTimeLeftSecAccount, m_iTimeLeftSecIP;
	int m_iCrusadeDuty;
	int m_iLogServerPort;
	int m_iRating; //Rating

	int m_iPrevMoveX, m_iPrevMoveY;
	int m_iBlockYear, m_iBlockMonth, m_iBlockDay;
	unsigned char m_iTopMsgLastSec;
	int m_iConstructionPoint;
	int m_iWarContribution;
	int m_iConstructLocX, m_iConstructLocY;
	int m_iNetLagCount;
	int m_iTeleportLocX, m_iTeleportLocY;
	int m_iTotalPartyMember;
	int m_iPartyStatus;
	int m_iGizonItemUpgradeLeft;
	//int m_iFeedBackCardIndex; // removed by Snoopy

	short m_sItemEquipmentStatus[MAXITEMEQUIPPOS];
	short m_sPlayerX, m_sPlayerY;
	short m_sPlayerObjectID;
	short m_sPlayerType;
	short m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4;
	short m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue; // Re-Coding Sprite xRisenx
	UnitStatus m_iPlayerStatus;
	short m_sMCX, m_sMCY;
	short m_sCommX, m_sCommY;
	int   m_iCastingMagicType;
	short m_sDamageMove;
	#ifdef RaiseCriticalLimit
	int   m_iDamageMoveAmount;
	#else
	short m_sDamageMoveAmount;
	#endif
	short m_sAppr1_IE, m_sAppr2_IE, m_sAppr3_IE, m_sAppr4_IE;
	short m_sHeadApprValue_IE, m_sBodyApprValue_IE, m_sArmApprValue_IE, m_sLegApprValue_IE; // Re-Coding Sprite xRisenx
	int m_iStatus_IE;
	short m_sViewDstX, m_sViewDstY;
	short m_sViewPointX, m_sViewPointY;
	short m_sVDL_X, m_sVDL_Y;

	uint16_t m_wCommObjectID;
	uint16_t m_wEnterGameType;
#ifdef MoreColors
	uint16_t m_wR[16], m_wG[16], m_wB[16];
	uint16_t m_wWR[21], m_wWG[21], m_wWB[21];
#else
	uint16_t m_wR[16], m_wG[16], m_wB[16];
	uint16_t m_wWR[16], m_wWG[16], m_wWB[16];
#endif
	char m_cEdit[4];
	char G_cTxt[128];
	char G_cTxt2[MAX_MAIL_MSG_LENGTH+1];
	char m_cGameModeCount;
	char m_cBGMmapName[12];
	char m_cItemOrder[MAXITEMS];
	char m_cAmountString[12];
	char m_cLogOutCount;
	char m_cRestartCount;
	char m_cGameMode;
	char m_cWhisperIndex;
	char m_cWhisperName[12];
	string m_cAccountName;
	string m_cAccountPassword;
	char m_cAccountAge[12];
	char m_cNewPassword[12];
	char m_cNewPassConfirm[12];
	char m_cAccountCountry[18];
	char m_cAccountSSN[32];
	char m_cEmailAddr[52];
	char m_cAccountQuiz[46];// Quiz
	char m_cAccountAnswer[22];
	char m_cPlayerName[12];
	char m_cPlayerDir;
	char m_cMsg[200];
	char m_cLocation[12];
	char m_cCurLocation[12];
	char m_cGuildName[22];
	char m_cMCName[12];
	char m_cMapName[12];
	char m_cMapMessage[32];
	char m_cMapIndex;
	char m_cPlayerTurn;
	char m_cCommand;
	char m_cCurFocus, m_cMaxFocus;
	char m_cEnterCheck, m_cTabCheck, m_cLeftArrowCheck;
	char m_cArrowPressed;
	char m_cLogServerAddr[16];
	char m_cChatMsg[64];
	char m_cBackupChatMsg[64];
	char m_cGender, m_cSkinCol, m_cHairStyle, m_cHairCol, m_cUnderCol;

	char m_cMagicMastery[MAXMAGICTYPE];
	unsigned char m_cSkillMastery[MAXSKILLTYPE]; 
	uint32_t m_cash;

	char m_cWorldServerName[32];
	char m_cDetailLevel;
	char m_cMenuDir, m_cMenuDirCnt, m_cMenuFrame;
	char m_cSoundVolume, m_cMusicVolume;
	Weather m_weather;
	char m_cIlusionOwnerType;
	char m_cName_IE[12];
	char m_sViewDX, m_sViewDY;
	char m_cCommandCount;
	//char m_cLoading;
	int m_cLoading;	// ShadowEvil - Changed to int cause it should be -.-
	char m_cDiscount;

	char m_cStatusMapName[12];
	char m_cTopMsg[128];
	char m_cTeleportMapName[12];
	char m_cConstructMapName[12];
	char m_cGameServerName[22]; //  Gateway

	//char m_cItemDrop[25][25];

	RECT m_rcPlayerRect, m_rcBodyRect;
	void * m_hWnd;

	void * m_hPakFile;

	bool m_bWhisper;
	bool m_bShout;

	#define NUM_ITEMDROPSLOTS		120
	char m_cItemDrop[NUM_ITEMDROPSLOTS][21];//m_cItemDrop[25][25];//besk itemdrop - holds names of items we have tried to drop before
	bool m_bItemDrop;//besk itemdrop - if TRUE, only show confirm screen the first time an item is dropped
    int  m_iItemDropCnt;//besk itemdrop - number of item names listed in m_cItemDrop

	//bool m_bItemDrop;
    //int  m_iItemDropCnt;

	// Snoopy: Apocalypse Gate
	char m_cGateMapName[12];
	int  m_iGatePositX, m_iGatePositY;
	int m_iHeldenianAresdenLeftTower;
	int m_iHeldenianElvineLeftTower;
	int m_iHeldenianAresdenFlags;
	int m_iHeldenianElvineFlags;
	int m_iHeldenianAresdenDead;
	int m_iHeldenianElvineDead;
	int m_iHeldenianAresdenKill;
	int m_iHeldenianElvineKill;
	bool m_bIsHeldenianMode;
	bool m_bIsHeldenianMap;

	bool m_bIllusionMVT;
	int m_iGameServerMode;
	bool m_bIsXmas;
	bool m_bUsingSlate;


	//Snoopy: Avatar
	bool m_bIsAvatarMode;
	bool m_bIsAvatarMessenger;

	//Snoopy: Crafting
	//bool _bDecodeCraftItemContents();
	//bool __bDecodeCraftItemContents(char *pBuffer);	
	//bool _bCheckCraftItemStatus();
	//bool _bCheckCurrentCraftItemStatus();

	class CBuildItem * m_pCraftItemList[MAXBUILDITEMS];
	class CBuildItem * m_pDispCraftItemList[MAXBUILDITEMS];
	int   m_iContributionPrice;

	char m_cTakeHeroItemName[100]; //Drajwer - hero item str

	int m_levelExpTable[MAXLEVEL +20];

	std::set<string> m_MuteList;

	bool m_partyAutoAccept;

    boost::asio::ssl::context ctx;

	bool m_ekScreenshot;
	uint64_t m_ekSSTime;

	bool m_tabbedNotification;
	int m_relicX, m_relicY;

	bool m_manuAutoFill;

	uint32_t m_weight;

	bool m_droppedOnIconbar;
	bool m_showBtns;

	int m_SoccerAresdenGoals, m_SoccerElvineGoals;
	bool m_bSoccerMode;

	bool m_bZerk, m_bInv, m_bPfm, m_bPfa, m_bShield; // Magic Icons xRisenx
    short m_sZerk, m_sInv, m_sPfm, m_sPfa, m_sShield; // Magic Icons xRisenx

	uint64_t m_dwAuraTime, m_dwResurTime, m_dwVentanaTime; // Magic Icons xRisenx

#ifdef MonsterBarClient
	void NotifyMsg_NpcBar(char * pData);
	//void NotifyMsg_NpcBar2(char * pData);
	short m_sNpcBar;
	int iNpcBar_HP/*, iNpcBar_MP*/;
#endif
	
#ifdef TitleClient
	void NotifyMsg_AddTitle(char * pData);
	void NotifyMsg_TitleLevelUp(char * pData);
	void GetTitleName(char * cSubject,int iRank, char *pStr1);
	int iGetNextTitle(char * cSubject, uint32_t iNumber);
	int getTitleIndexFromSubject(char * cSubject);
	void ClearPlayerTitle(char * cName);
	bool FindTitleName(char* pName, int* ipIndex);
	int m_iTitleIndex;	// this is the index to the player's active title (?) -1 means none selected
						//	The player's currently displayed title in m_pTitles[]
	uint64_t dwTitleLevelUpTime;
	char cTitleLevelUpSubject[120]; // this seems to hold the most recently leveled up title (if any)
									//		For displaying it on top of your character (informing/congratulating the player on the new title)

	struct {// these are the titles being worn, for displaying under the name of each player
		uint64_t dwRefTime;
		int iCrusadeT;//1=soldier, 2=constructor, 3=commander
		int iRank;
		int iCurrentNo;
		char cCharName[12];
		char cSubject[21];
	} m_stTitles[MAXGUILDNAMES];
	
	class Title * m_pTitles[MAXMENUITEMS];//besk: these are the player's own titles
	uint64_t m_dwReqTitlesTime;

	void DrawDialogBox_Titles();
	void DlgBoxClick_Titles(); // 
#endif
	bool bCheckItemEquiped(char itemName[]); // Beholder Necklace Fix xRisenx
	// Gladiator Arena xRisenx
	/*#define MAXARENAPLAYERS      200
struct {
   int iKills;
   int iDeaths;
   char cCharName[12];
} 
m_stArenaPlayers[MAXARENAPLAYERS];*/
	// Gladiator Arena xRisenx
	// ShadowEvil Declarations
	bool b_cRemember;
	bool b_cWarrior, b_cMage, b_cBattleMage, b_cArcher;
	char m_cUserName[13];
	void WriteUsername(const char* cName, bool RemUser);
	void ReadUsername();
	void _LoadItemContents();
	bool __bDecodeContentsAndBuildItemList(char * pBuffer);
	CItem * m_pBuildItemListStats[MAXITEMSTATS];
	bool isCommon, isUncommon, isRare, isEpic, isLegendary;
	void isValue(uint32_t value);
	bool isItemLoaded;
	void HandleItemDescription(CItem * item);

	// Monster kill event xRisenx
	void NotifyMsg_NpcHuntingON();
	void NotifyMsg_NpcHuntingOFF();
	void NotifyMsg_NpcHuntingWinner(char * pData);
	// Monster kill event xRisenx

	bool m_bHackMoveBlocked; // Anti Hack xRisenx
	//bool CheckHackProcesses(); // Anti Hack xRisenx
	//bool CheckHackDLLProcesses(); // Anti Hack xRisenx
	//bool CheckHackProcessesMulti(); // Anti Hack xRisenx

};

#endif // !defined(AFX_GAME_H__0089D9E3_74E6_11D2_A8E6_00001C7030A6__INCLUDED_)