// Game.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__0089D9E3_74E6_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_GAME_H__0089D9E3_74E6_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mmsystem.h> 
#include <winbase.h>
#include <cstring>
#include <vector>
#include <functional>

#include <process.h>
#include <direct.h>
#include <set>

#include "common.h"
#include "NetMessages.h"
#include "npcType.h"
#include "maths.h"

#include "GlobalDef.h"
// #include "directx\DXC_ddraw.h"
// #include "directx\DXC_dinput.h"
// #include "directx\DSound.h"
// #include "directx\SoundBuffer.h"
// #include "directx\YWSound.h"
#include "net\XSocket.h"
#include "directx\Sprite.h"
#include "directx\SpriteID.h"
#include "Misc.h"
#include "StrTok.h"
#include "net\Msg.h"
#include "directx\Effect.h"
#include "map\MapData.h"
#include "char\ActionID.h"
#include "ui\MouseInterface.h"
#include "char\CharInfo.h"
#include "char\item\Item.h"
#include "char\Magic.h"
#include "char\Skill.h"
#include "char\DynamicObjectID.h"
#include "char\item\BuildItem.h"
#include "char\item\ItemName.h"
#include "char\Curse.h"
#include "char\guild.h"
#include "char\mail.h"
#include "ui\DialogBox.h"

#include "titles\Title.h" // Titles xRisenx
#include "tlhelp32.h" // Anti Hack xRisenx



#include <irrlicht.h>


#ifdef USING_WIN_IME
	#include <RICHEDIT.H>
#endif

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

extern HWND G_hWnd;

extern video::E_DRIVER_TYPE driverType;

#define FONT_BUILTIN 0
#define FONT_TREBMS6PX 1
#define FONT_TREBMS8PX 2
#define FONT_TREBMS10PX 3
#define FONT_TREBMS12PX 4
#define FONT_TREBMS14PX 5
#define FONT_TREBMS16PX 6


class CGame : public irr::IEventReceiver
{
public:
	void DrawStatusText(int sX, int sY);

	IrrlichtDevice * device;
	video::IVideoDriver * driver;
	scene::ISceneManager* smgr;
	gui::IGUIFont * font[100];
	gui::IGUIEnvironment* env;

	video::ITexture* bg;

	bool gamemode;

	int16_t lastchar;
	bool capslock;
	bool CreateRenderer()
	{
		//when streaming, vsync on = screen capture nogo
		//has to use "game capture" (render hook)
		//vsync better for production though - include option for players to choose                                \/
		device = createDevice(driverType,irr::core::dimension2d<u32>(GetWidth(), GetHeight()), 32, false, false, false, this);
		if (device == 0)
		{
			MessageBox(NULL, L"Cannot create video device!", L"ERROR!", MB_OK);
			return false; // could not create selected driver.
		}
		//device->setEventReceiver(this);



		wchar_t winName[256];
		wsprintfW(winName, L"Helbreath Xtreme %u.%u.%u", HBF_MAJOR, HBF_MINOR, HBF_LOWER);
		device->setWindowCaption(winName);

		driver = device->getVideoDriver();
		driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
		smgr = device->getSceneManager();
		env = device->getGUIEnvironment();

		irr::video::SExposedVideoData vdata = driver->getExposedVideoData();
		G_hWnd = reinterpret_cast<HWND>(vdata.D3D9.HWnd);

		driver->getMaterial2D().TextureLayer[0].BilinearFilter=false;
		driver->getMaterial2D().AntiAliasing=video::EAAM_OFF;


		if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET))
		{
			bg = driver->addRenderTargetTexture(core::dimension2d<u32>(GetWidth()+100,GetHeight()+100), "RTT1");
		}
		else
		{
			MessageBoxA(0, "Unable to RTT - background will not render", "RTT", MB_OK);
		}

		return true;
	}
	virtual bool OnEvent(const irr::SEvent& event);
	virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const { return KeyIsDown[keyCode]; }

	bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];


	bool clipmousegame;
	bool clipmousewindow;
	bool isactive;
	uint16_t screenwidth;
	uint16_t screenheight;
	void SetResolution(uint16_t width, uint16_t height) { screenwidth = width; screenheight = height; }
	__forceinline uint16_t GetWidth() { return screenwidth; }
	__forceinline uint16_t GetHeight() { return screenheight; }

	void DrawScene(u32 time);
	//void DrawFPS2();//debug func
	void DrawMouse();
	void DrawVersion2();
	uint64_t mtime;


	void emptyfunc(void){};
	void ShowSoccerVictory(short sSide);
	void NotifyMsg_Soccer(char * pData);
	void DrawSoccerInfo();

	void DrawDialogBox_QuestList();
	void DlgBoxClick_QuestList();

	void TopsiteVote();
	void OnVoteSocketEvent(WPARAM wParam, LPARAM lParam);
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

	BOOL m_bHappyHour;
	void DrawQuestHelper();
	void LoadFriendList();
	void SaveFriendList();
	void DrawDialogBox_FriendList();
	void DlgBoxClick_FriendList();

	uint32 m_adminLevel;
	uint8 m_GMFlags;
	uint32 m_eventPot;

	Vector<string> m_voteUrls;
	int m_voteState;
	XSocket * m_voteSocket;
	DWORD lastScreenUpdate;

	int m_iTotalFriends;
	int m_iFriendIndex;
	DWORD lastFriendUpdate;
	struct {
		bool online;
		char friendName[13];
		bool updated;
	} friendsList[13];

	// CLEROTH - AURAS
	void CheckActiveAura(short sX, short sY, DWORD dwTime, short sOwnerType);
	void CheckActiveAura2(short sX, short sY, DWORD dwTime, short sOwnerType);

	// MJ Stats Change Related functions - Alastor
	void DrawDialogBox_ChangeStatsMajestic();// Change stats using majestic - Alastor
	void DlgBoxClick_ChangeStatsMajestic();			// Change stats using majestic - Alastor

	//char m_cStatChange[TOTALLEVELUPPOINT]; // Stats System xRisenx

	int m_iTeleportMapCount;
	void ResponseTeleportList(char * pData);
	void ResponseChargedTeleport(char * pData);

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
	uint32 ReadSettingsVar(const char * var);
	void WriteSettings();
	void WriteSettingsVar(const char * var, uint32 val);

	int  iGetManaCost(int iMagicNo);
	void UseMagic(int iMagicNo);
	BOOL _bCheckMoveable( short sx, short sy );
	BOOL FindGuildName(char* pName, int* ipIndex);
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
	void _Draw_OnLogin(char * pAccount, char * pPassword, int msX, int msY, int iFrame = 60000);
	void DrawNewDialogBox(char cType, int sX, int sY, int iFrame, BOOL bIsNoColorKey = FALSE, BOOL bIsTrans = FALSE);
	void AddMapStatusInfo(char * pData, BOOL bIsLastData);
	void _RequestMapStatus(char * pMapName, int iMode);
	int  GetCharKind(char *str, int index);
	void ReceiveString(char * pString);
	void EndInputString();
	void ClearInputString();
	void ShowReceivedString(BOOL bIsHide = FALSE);
	bool GetText(HWND hWnd,UINT msg,WPARAM wparam, LPARAM lparam);
	
	BOOL bReadItemNameConfigFile();
	void DrawDialogBoxes();
	void DrawDialogBox_Character();//1
	void DrawDialogBox_Inventory();//2
	void DrawDialogBox_Magic();//3
	void DrawDialogBox_GuildMenu();//7
	void DrawDialogBox_GuildOperation();//8
	void DrawDialogBox_GuideMap();//9
	void DrawDialogBox_Chat();//10
	void DrawDialogBox_Shop();//11
	void DrawDialogBox_LevelUpSetting();//12
	void DrawDialogBox_CityHallMenu();//13
	void DrawDialogBox_Bank();//14
	void DrawDialogBox_Skill();//15
	void DrawDialogBox_MagicShop();//16
	void DrawDialogBox_QueryDropItemAmount();//17
	void DrawDialogBox_Text();//18
	void DrawDialogBox_SysMenu();//19
	void DrawDialogBox_NpcActionQuery();//20
	void DrawDialogBox_NpcTalk();//21
	void DrawDialogBox_Map();//22
	void DrawDialogBox_SellorRepairItem();//23
	void DrawDialogBox_Fishing();//24
	void DrawDialogBox_ShutDownMsg();//25
	void DrawDialogBox_SkillDlg();//26
	void DrawDialogBox_Exchange();//27
	void DrawDialogBox_Quest();//28
	void DrawDialogBox_GaugePanel();//29
	void DrawDialogBox_IconPanel();//30
	void DrawDialogBox_SellList();//31
	void DrawDialogBox_Party();//32
	void DrawDialogBox_CrusadeJob();//33
	void DrawDialogBox_ItemUpgrade();//34
	void DrawDialogBox_Help();//35
	void DrawDialogBox_Commander();//36
	void DrawDialogBox_Constructor();//37
	void DrawDialogBox_Soldier();//38
   void DrawDialogBox_ItemDrop();//4
	void DrawDialogBox_WarningMsg();//6
	void DrawDialogBox_15AgeMsg();//5
	void DrawDialogBox_FeedBackCard();//40
	void DrawDialogBox_Guild();//57
	void DlgBoxClick_Guild(); //57
   void DrawDialogBox_GuildContribute();//58
   void DlgBoxClick_GuildContribute();//58
	void DrawDialogBox_ExtendedSysMenu();//59
	void DlgBoxClick_ExtendedSysMenu();//59
	void DrawDialogBox_MuteList();//60
	void DlgBoxClick_MuteList();//60
	void DrawDialogBox_TopPanel(); //62
	void DrawDialogBox_Mailbox();
	//void DrawDialogBox_Titles(/*char cLB*/); // [70] Titles xRisenx
	//void DlgBoxClick_Titles(); // [70] Titles xRisenx

	void DisplayCommaNumber_G_cTxt(int iGold);
	
	void DrawDialogBox_ConfirmExchange();//41
	void DlgBoxClick_ConfirmExchange();
	void DrawDialogBox_YesNo();
	void DlgBoxClick_YesNo();

	void DrawDialogBox_DKMenuWeapons();// 53
	void DlgBoxClick_DKMenuWeapons();

	// Slates - Alastor
	void bItemDrop_Slates();
	void DlgBoxClick_Slates();
	void DrawDialogBox_Slates();//40

	void DlgBoxClick_WarningMsg();
	void DlgBoxClick_15AgeMsg();
	void DlgBoxClick_ItemDrop();
	void DlgBoxClick_Character();
	void DlgBoxClick_Inventory();
	void DlgBoxClick_Chat();
	void DlgBoxDoubleClick_Chat();

	void EnableChat(ChatType t, bool enable);
	void DlgBoxClick_Magic();
	void DlgBoxClick_GuildMenu();
	void DlgBoxClick_GuildOp();
	void DlgBoxClick_GuideMap();
	void DlgBoxClick_Shop();
	void DlgBoxClick_LevelUpSettings();
	void DlgBoxClick_CityhallMenu();
	void DlgBoxClick_Bank();
	void DlgBoxClick_Skill();
	void DlgBoxClick_MagicShop();
	void DlgBoxClick_FeedBackCard();
	void DlgBoxClick_Text();
	void DlgBoxClick_SysMenu();
	void DlgBoxClick_NpcActionQuery();
	void DlgBoxClick_NpcTalk();
	void DlgBoxClick_ItemSellorRepair();
	void DlgBoxClick_Fish();
	void DlgBoxClick_ShutDownMsg();
	void DlgBoxClick_SkillDlg();
	void DlgBoxClick_Exchange();

	void DlgBoxClick_Quest();
	void DlgBoxClick_SellList();
	void DlgBoxClick_IconPanel();
	void DlgBoxClick_Party();
	void DlgBoxClick_CrusadeJob();
	void DlgBoxClick_ItemUpgrade();
	void DlgBoxClick_Help();
	void DlgBoxClick_Commander();
	void DlgBoxClick_Constructor();
	void DlgBoxClick_Soldier();
	void DlgBoxClick_Mailbox();
	void DlgBoxClick_TopPanel(); // New Top Bar xRisenx
	void ItemDrop_Mailbox();
	
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
	void NotifyMsg_WhisperMode(BOOL bActive, char * pData);
	void NotifyMsg_UnitStatus(BOOL bOnGame, char * pData);
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
	void _CalcSocketClosed();
	void UpdateScreen_OnSelectServer();
	void StartInputString(int left, int top, uint32 len, char * pBuffer, BOOL bIsHide = FALSE, int right = 0);
	void _SetIlusionEffect(int iOwnerH);
	int _iGetFOE(int iStatus);
	void NoticementHandler(char * pData);
	void GetItemName(char * cItemName, DWORD dwAttribute, char *pStr1, char *pStr2, char *pStr3);
	void GetItemName(CItem * pItem, char * pStr1, char * pStr2, char * pStr3);
	Vector<string> * GetItemName(char * cItemName, DWORD attr, uint8 sockets[MAXITEMSOCKETS], uint32 count);
	Vector<string> * GetItemName(CItem * pItem, bool isWH = false);
	void _InitOnCreateNewCharacter();
	BOOL _bCheckCurrentBuildItemStatus();
	BOOL _bCheckBuildItemStatus();
	BOOL _bDecodeBuildItemContents();
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
	void UpdateScreen_OnSelectCharacter(short sX, short sY, short msX, short msY, BOOL bIgnoreFocus = FALSE);
	void UpdateScreen_OnChangePassword();
	void UpdateScreen_OnLoading_Progress();
	void UpdateScreen_OnVersionNotMatch();
	void NpcTalkHandler(char * pData);
	int  _iGetWeaponSkillType();
	void SetCameraShakingEffect(short sDist, int iMul = 0);
	BOOL DlgBoxPress_SkillDlg();
	BOOL DlgBoxPress_Inventory();
	BOOL DlgBoxPress_Character();
	void ClearSkillUsingStatus();
	BOOL bCheckItemOperationEnabled(char cItemID);
	void _DrawThunderEffect(int sX, int sY, int dX, int dY, int rX, int rY, char cType);
	void DrawLine2(int x0, int y0, int x1, int y1, int iR, int iG, int iB);
	void DrawLine(int x0, int y0, int x1, int y1, int iR, int iG, int iB);
	void SetWeatherStatus(Weather type);
	void WeatherObjectFrameCounter();
	void DrawWeatherEffects();
	BOOL bCheckLocalChatCommand(char const * const pMsg);
	char GetOfficialMapName(char const * const pMapName, char * pName);
	int iGetLevelExp(int iLevel);
	int _iCalcTotalWeight();
	void DrawVersion(BOOL bAuthor = FALSE);
	BOOL _bIsItemOnHand();
	void DynamicObjectHandler(char * pData);
	BOOL _bCheckItemByType(char cType);
	void _DrawBlackRect(int iSize);
	void DrawNpcName(   short sX, short sY, short sOwnerType, int iStatus);
	void DrawObjectName(short sX, short sY, char * pName, int iStatus);
	void PlaySound(char cType, int iNum, int iDist, long lPan = 0);
	void _RemoveChatMsgListByObjectID(int iObjectID);
	void _LoadTextDlgContents(int cType);
	int  _iLoadTextDlgContents2(int iType);
	void DrawChatMsgs(short sX, short sY, short dX, short dY);
	void RequestFullObjectData(WORD wObjectID);
	BOOL bInitSkillCfgList();
	BOOL bCheckImportantFile();
	void DlgBoxDoubleClick_Inventory();
	void DlgBoxDoubleClick_Character();
	void DlgBoxDoubleClick_GuideMap();
	Point GetGuideMapPos(uint32 x, uint32 y);
	void EraseItem(char cItemID);
	void RetrieveItemHandler(char * pData);
	void CivilRightAdmissionHandler(char * pData);
	void _Draw_CharacterBody(short sX, short sY, short sType);
	void ClearContents_OnSelectCharacter();
	void ClearContents_OnCreateNewAccount();
	void _Draw_UpdateScreen_OnCreateNewAccount();
	BOOL _bDraw_OnCreateNewCharacter(char * pName, short msX, short msY, int iPoint);
	BOOL _bGetIsStringIsNumber(char * pStr);
	BOOL bInitMagicCfgList();
	BOOL __bDecodeContentsAndBuildItemForSaleList(char * pBuffer);
	void _LoadShopMenuContents(char cType);
	void PutChatScrollList(char * pMsg, char cType);
	void RequestTeleportAndWaitData();
	void DrawEffectLights();
	void PointCommandHandler(int indexX, int indexY, char cItemID = -1);
	void DrawEffects();
	void bAddNewEffect(short sType, int sX, int sY, int dX, int dY, char cStartFrame, int iV1 = 1);
	void AddEventList(char const * const pTxt, char cColor = 0, BOOL bDupAllow = TRUE);
	void ShowEventList(DWORD dwTime);
	void SetItemCount(char * pItemName, DWORD dwCount);
	uint32 GetItemCount(char * pItemName);
	void _ShiftGuildOperationList();
	void _PutGuildOperationList(char * pName, char cOpMode);
	void DisbandGuildResponseHandler(char * pData);
	void InitPlayerCharacteristics(char * pData);
	void CreateNewGuildResponseHandler(char * pData);
	void _GetHairColorRGB(int iColorType , int * pR, int * pG, int * pB);
	void InitGameSettings();
	void CommonEventHandler(char * pData);
	BOOL _bCheckDraggingItemRelease(char dlgID);
	void _SetItemOrder(char cWhere, char cItemID);
	int iGetTopDialogBoxIndex();
	void DisableDialogBox(int iBoxID);
	void EnableDialogBox(int iBoxID, int cType = 0, int sV1 = 0, int sV2 = 0, const char * pString = NULL);

	void InitItemList(char * pData);
	int _iCheckDlgBoxFocus(char cButtonSide);
	void GetPlayerTurn();
	BOOL __fastcall DrawObject_OnDead(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL __fastcall DrawObject_OnDying(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL __fastcall DrawObject_OnMagic(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL __fastcall DrawObject_OnAttack(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL __fastcall DrawObject_OnAttackMove(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL __fastcall DrawObject_OnStop(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL __fastcall DrawObject_OnMove_ForMenu(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL __fastcall DrawObject_OnMove(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL __fastcall DrawObject_OnDamageMove(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL __fastcall DrawObject_OnRun(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL __fastcall DrawObject_OnDamage(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL __fastcall DrawObject_OnGetItem(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	BOOL bEffectFrameCounter();
	void ClearGuildNameList();
	void DrawBackground(short sDivX, short sModX, short sDivY, short sModY);
	void DrawChatMsgBox(short sX, short sY, int iChatIndex, BOOL bIsPreDC);
	void ReleaseTimeoverChatMsg();
	void ChatMsgHandler(char * pData);
	void ReleaseUnusedSprites();
	BOOL bReadIp();
	void OnKeyUp(WPARAM wParam);
	void OnSysKeyDown(WPARAM wParam);
	void OnSysKeyUp(WPARAM wParam);
	void ChangeGameMode(char cMode);
	void PutFontString(gui::IGUIFont * font, int iX, int iY, char * pString, video::SColor color);
	void PutFontString(gui::IGUIFont * font, int iX, int iY, char const * pString, video::SColor color, BOOL bHide, char cBGtype, BOOL bIsPreDC = FALSE);
	void PutChatString(int iX, int iY, char * pString, video::SColor color);
	void PutString(int iX, int iY, char const * pString, video::SColor color);
	void PutString(int iX, int iY, char const * pString, video::SColor color, BOOL bHide, char cBGtype, BOOL bIsPreDC = FALSE);
	void PutString2(int iX, int iY, char * pString, short sR, short sG, short sB);
	void PutString3(int iX, int iY, char const * pString, ::SColor color);
	void PutAlignedString(int iX1, int iX2, int iY, char const * const pString, video::SColor color = video::SColor(255,255,255,255));
	void PutAlignedString(int iX1, int iX2, int iY, char const * const pString, short sR, short sG, short sB)
	{
		//finish replacing eventually
		PutAlignedString(iX1, iX2, iY, pString, video::SColor(255, sR, sG, sB));
	}
	//void ButtonString(CDialogBox & dlg, int button, char const * const pString, Alignment pos = POS_CENTER);
	//void ButtonStringToggle(CDialogBox & dlg, int button, char const * const pString, bool enabled = true, Alignment pos = POS_CENTER);
	void PutString_SprFont(int iX, int iY, char * pStr, short sR, short sG, short sB);
	void PutString_SprFont2(int iX, int iY, char * pStr, short sR, short sG, short sB);
	void PutString_SprFont3(int iX, int iY, char * pStr, short sR, short sG, short sB, BOOL bTrans = FALSE, int iType = 0);
	void PutString_SprNum(int iX, int iY, char * pStr, short sR, short sG, short sB);
	void LogRecvMsgHandler(char * pData);
	void LogResponseHandler(char * pData);
	void OnLogSocketEvent(WPARAM wParam, LPARAM lParam);
	void OnTimer();
	void _ReadMapData(short sPivotX, short sPivotY, char * pData);
	void MotionEventHandler(char * pData);
	void InitDataResponseHandler(char * pData);
	void InitPlayerResponseHandler(char * pData);
	void ConnectionEstablishHandler(char cWhere);
	void MotionResponseHandler(char * pData);
	void GameRecvMsgHandler(DWORD dwMsgSize, char * pData);
	void DrawObjects(short sPivotX, short sPivotY, short sDivX, short sDivY, short sModX, short sModY, short msX, short msY);
	BOOL bSendCommand(DWORD dwMsgID, WORD wCommand = NULL, char cDir = NULL, int iV1 = NULL, int iV2 = NULL, int iV3 = NULL, char const * const pString = NULL, int iV4 = NULL); 
	BOOL SendLoginCommand(DWORD dwMsgID); 
	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, BOOL bMoveCheck = FALSE, BOOL isMIM = FALSE);
	void RestoreSprites();
	void CommandProcessor(short msX, short msY, short indexX, short indexY, char cLB, char cRB, char cMB);
	void OnGameSocketEvent(WPARAM wParam, LPARAM lParam);
	void CalcViewPoint();
	void OnKeyDown(WPARAM wParam);
	void Quit();
	BOOL bInit(HWND hWnd, HINSTANCE hInst, char * pCmdLine);

	void ReserveFightzoneResponseHandler(char * pData);
	int _iGetAttackType();
	BOOL __bDecodeBuildItemContents(char * pBuffer);
	int _iGetBankItemCount();
	int _iGetTotalItemNum();
	uint32 GetGold() const;
	void SetGold(uint32 val);
	LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	void GoHomepage(bool _web);
	void StartBGM();

	//Snoopy: added function:
	void DebugLog(char * cStr);
	BOOL bReadLoginConfigFile(char * cFn);
	//int bHasHeroSet( short Appr3, short Appr4, char OwnerType);
	int bHasHeroSet(short HeadApprValue, short BodyApprValue, short ArmApprValue, short LegApprValue, char OwnerType);
	void ShowHeldenianVictory(short sSide);
	void DrawDialogBox_Resurect();
	void DlgBoxClick_Resurect();
	void DrawDialogBox_GuildSummons();
	void DlgBoxClick_GuildSummons();
	void DrawDialogBox_GuildQuery();
	void DlgBoxClick_GuildQuery();
	void DrawDialogBox_CMDHallMenu();
	void DlgBoxClick_CMDHallMenu();
	void ResponseHeldenianTeleportList(char *pData);
	void DKGlare(int iWeaponColor, int iWeaponIndex, int *iWeaponGlare);
	void DrawDruncncity();
	void Abaddon_corpse(int sX, int sY);
	void DrawAngel(int iSprite, short sX, short sY, char cFrame, DWORD dwTime);

	void LoadMuteList();
	void SaveMuteList();

	char MouseOverDialog();
	void SetupDialogBoxes();
	void SetupDialogBox(int dialog,  short X, short Y, short background, int backFrame, int titleTxtFrame = -1, bool trans = false);

	void ClearPartyMembers();

	BOOL _ItemDropHistory(char * ItemName);
	CGame();
	virtual ~CGame();

	struct {
		short sX;
		short sY;
		short sZ;
		bool LB;
		bool RB;
		bool MB;
		short sCursorFrame;
		char  cPrevStatus;
		char  cSelectedObjectType;
		short sSelectedObjectID;
		short sPrevX, sPrevY, sDistX, sDistY;
		DWORD dwSelectClickTime;
		short sClickX, sClickY;
	} m_stMCursor;

	CDialogBox m_dialogBoxes[MAXDIALOGBOX];

	char m_cDialogBoxOrder[MAXDIALOGBOX];
	int m_bIsDialogEnabled[MAXDIALOGBOX];//was BOOL
	ItemMapIter m_guildBankIt;
	
	struct {
		int   sV1, sV2, sV3, sV4, sV5, sV6, sV7, sItemID;
		DWORD dwV1;
		uint8 sockets[MAXITEMSOCKETS];
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
		DWORD dwTime;
		char  cColor;
		char  cTxt[96];
	} m_stEventHistory[6];

	struct {
		DWORD dwTime;
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

		MapPing(uint32 x, uint32 y)
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

	Vector<MapPing> m_mapPings;
	void AddMapPing(MapPing ping);

	struct {
		BOOL bIsQuestCompleted;
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

	Vector<partyMember *> m_stPartyMember;
	typedef Vector<partyMember *>::iterator partyIterator;
	
	MailVec m_mails;
	MailVec m_gldBoard;

	struct {
		short sX, sY;
		char cType;
		char cSide;
	} m_stCrusadeStructureInfo[MAXCRUSADESTRUCTURES];

	struct {
		DWORD dwRefTime;
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

//	class YWSound m_DSound;
// 	class CSoundBuffer *	m_pCSound[MAXSOUNDEFFECTS];
// 	class CSoundBuffer *	m_pMSound[MAXSOUNDEFFECTS];
// 	class CSoundBuffer *	m_pESound[MAXSOUNDEFFECTS];
// 	class CSoundBuffer *    m_pBGM;
//	class DXC_ddraw  m_DDraw;
//	class DXC_dinput m_DInput;
	class CMisc      m_Misc;
	class CSprite  * m_pSprite[MAXSPRITES];
	class CSprite  * m_pTileSpr[MAXTILES];
	class CSprite  * m_pEffectSpr[MAXEFFECTSPR];
	class CMapData * m_pMapData;
	class XSocket * m_pGSock;
	class XSocket * m_pLSock;
	class CMsg    * m_pChatMsgList[MAXCHATMSGS];

	typedef Vector<CMsg*> MsgVec;
	MsgVec * m_chatDisplay;
	MsgVec m_chatMsgs[CHAT_MAX];
	bool m_chatToggle[CHAT_MAX];

	class CMsg    * m_pWhisperMsg[MAXWHISPERMSG];
	class CEffect * m_pEffectList[MAXEFFECTS];
	class CItem   * m_pItemList[MAXITEMS];
	class CItem   * m_pBankList[MAXBANKITEMS];
	ItemMap m_guildBankMap;
	uint32 m_guildBankVer;
	class CMagic * m_pMagicCfgList[MAXMAGICTYPE];
	class CSkill * m_pSkillCfgList[MAXSKILLTYPE];
	class CMsg * m_pMsgTextList[TEXTDLGMAXLINES];
	class CMsg * m_pMsgTextList2[TEXTDLGMAXLINES];
	class CMsg * m_pAgreeMsgTextList[TEXTDLGMAXLINES];
	class CBuildItem * m_pBuildItemList[MAXBUILDITEMS];
	class CBuildItem * m_pDispBuildItemList[MAXBUILDITEMS];


	class CItem * m_pItemForSaleList[MAXMENUITEMS];
	class CCharInfo * m_pCharList[4];
	class CItemName * m_pItemNameList[MAXITEMNAMES];
	class CCurse m_curse;

	char * m_pInputBuffer;

	DWORD G_dwGlobalTime;
	DWORD m_dwCommandTime; //v2.15 SpeedHack
	DWORD m_dwConnectMode;
	DWORD m_dwTime;
	DWORD m_dwCurTime;
	DWORD m_dwCheckConnTime, m_dwCheckSprTime, m_dwCheckChatTime;
	DWORD m_dwDialogCloseTime;
	int  m_dwLogOutCountTime;//was DWORD
	DWORD m_dwRestartCountTime;
	DWORD m_dwWOFtime; //v1.4
	DWORD m_dwObserverCamTime;
	DWORD m_dwDamagedTime;
	DWORD m_dwSpecialAbilitySettingTime;
	DWORD m_dwCommanderCommandRequestedTime;
	DWORD m_dwTopMsgTime;
	DWORD m_dwEnvEffectTime;

	//v2.2
	DWORD m_dwMonsterEventTime;
	short m_sMonsterID;
	short m_sEventX, m_sEventY;

	bool m_shieldDisabled;
	bool m_armorDisabled;
	bool m_magicDisabled[MAXMAGICTYPE];

	Side m_side;

	BOOL m_bZoomMap;
	BOOL m_bIsProgramActive;
	int m_bCommandAvailable;//was BOOL
	BOOL m_bSoundFlag;
	BOOL m_bSoundStat, m_bMusicStat; // On/Off
	int m_bIsItemEquipped[MAXITEMS];//was BOOL
	int m_bIsItemDisabled[MAXITEMS];//was BOOL
	int m_bIsGetPointingMode;//was BOOL
	BOOL m_bEnterPressed, m_bEscPressed, m_bCtrlPressed, m_bRunningMode, m_bShiftPressed, m_altPressed;

	BOOL m_bDialogTrans;
	BOOL m_bIsCombatMode;
	BOOL m_bIsSafeAttackMode;
	int m_bSkillUsingStatus;//was BOOL
	int m_bItemUsingStatus;//was BOOL
	BOOL m_bSuperAttackMode;	//
	BOOL m_bIsObserverMode, m_bIsObserverCommanded;
	int m_bIsPoisoned;//was BOOL
	BOOL m_bIsFirstConn;
	BOOL m_bIsConfusion;
	BOOL m_bIsRedrawPDBGS;
	BOOL m_bDrawFlagDir;
	BOOL m_bIsCrusadeMode;
	BOOL m_bIsAstoriaMode;
	Casualties m_astoriaStats[MAXSIDES];
	//uint16 m_astoriaShieldsHP[MAXSIDES]; // Shield xRisenx
	uint32 m_astoriaShieldsHP[MAXSIDES]; // Shield xRisenx
	DWORD m_relicOwnedTime;
	Side m_relicOwnedSide;
	int m_bIsSpecialAbilityEnabled;//was BOOL
	BOOL m_bInputStatus;
	BOOL m_bToggleScreen;
	BOOL m_bIsSpecial;
	video::SColor m_itemColor;

	BOOL m_bIsF1HelpWindowEnabled;
	int m_bIsTeleportRequested;//was BOOL
	BOOL m_bIsPrevMoveBlocked;
	BOOL m_bIsHideLocalCursor;

	int m_bForceDisconn;//was BOOL
	BOOL m_targetNeutral;
	BOOL m_bForceAttack;
	BOOL m_bParalyze;

	int m_iFPS;
	DWORD m_dwFPStime;
	BOOL  m_bShowFPS;
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

	int m_iLevel, m_iExp, m_iContribution, m_iLucky;
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
	int m_iInputX, m_iInputY, m_iInputX2;
	int m_iPDBGSdivX, m_iPDBGSdivY;			   // Pre-Draw Background Surface
	short m_sRecentShortCut;
	short m_sShortCut[6]; // Snoopy: 6 shortcuts
	int	m_iSpecialAbilityTimeLeftSec;
	int m_iDrawFlag;
	int m_iSpecialAbilityType;
	int m_iTimeLeftSecAccount, m_iTimeLeftSecIP;
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

	WORD m_wCommObjectID;
	WORD m_wEnterGameType;
#ifdef MoreColors
	WORD m_wR[16], m_wG[16], m_wB[16];
	WORD m_wWR[21], m_wWG[21], m_wWB[21];
#else
	WORD m_wR[16], m_wG[16], m_wB[16];
	WORD m_wWR[16], m_wWG[16], m_wWB[16];
#endif
	uint32 m_inputMaxLen;
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
	char m_cAccountName[12];
	char m_cAccountPassword[12];
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
	uint32 m_cash;

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
	HWND m_hWnd;

	HANDLE m_hPakFile;

	BOOL m_bWhisper;
	BOOL m_bShout;

	#define NUM_ITEMDROPSLOTS		120
	char m_cItemDrop[NUM_ITEMDROPSLOTS][21];//m_cItemDrop[25][25];//besk itemdrop - holds names of items we have tried to drop before
	BOOL m_bItemDrop;//besk itemdrop - if TRUE, only show confirm screen the first time an item is dropped
    int  m_iItemDropCnt;//besk itemdrop - number of item names listed in m_cItemDrop

	//BOOL m_bItemDrop;
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

	BOOL m_bIllusionMVT;
	int m_iGameServerMode;
	BOOL m_bIsXmas;
	BOOL m_bUsingSlate;


	//Snoopy: Avatar
	BOOL m_bIsAvatarMode;
	BOOL m_bIsAvatarMessenger;

	//Snoopy: Crafting
	//BOOL _bDecodeCraftItemContents();
	//BOOL __bDecodeCraftItemContents(char *pBuffer);	
	//BOOL _bCheckCraftItemStatus();
	//BOOL _bCheckCurrentCraftItemStatus();

	class CBuildItem * m_pCraftItemList[MAXBUILDITEMS];
	class CBuildItem * m_pDispCraftItemList[MAXBUILDITEMS];
	int   m_iContributionPrice;

	char m_cTakeHeroItemName[100]; //Drajwer - hero item str

	int m_levelExpTable[MAXLEVEL +20];

	std::set<string> m_MuteList;

	BOOL m_partyAutoAccept;

	bool m_ekScreenshot;
	DWORD m_ekSSTime;

	bool m_tabbedNotification;
	int m_relicX, m_relicY;

	bool m_manuAutoFill;

	DWORD m_weight;

	bool m_droppedOnIconbar;
	bool m_showBtns;

	int m_SoccerAresdenGoals, m_SoccerElvineGoals;
	bool m_bSoccerMode;

	bool m_bZerk, m_bInv, m_bPfm, m_bPfa, m_bShield; // Magic Icons xRisenx
    short m_sZerk, m_sInv, m_sPfm, m_sPfa, m_sShield; // Magic Icons xRisenx

	DWORD m_dwAuraTime, m_dwResurTime, m_dwVentanaTime; // Magic Icons xRisenx

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
	int iGetNextTitle(char * cSubject, DWORD iNumber);
	int getTitleIndexFromSubject(char * cSubject);
	void ClearPlayerTitle(char * cName);
	BOOL FindTitleName(char* pName, int* ipIndex);
	int m_iTitleIndex;	// this is the index to the player's active title (?) -1 means none selected
						//	The player's currently displayed title in m_pTitles[]
	DWORD dwTitleLevelUpTime;
	char cTitleLevelUpSubject[120]; // this seems to hold the most recently leveled up title (if any)
									//		For displaying it on top of your character (informing/congratulating the player on the new title)

	struct {// these are the titles being worn, for displaying under the name of each player
		DWORD dwRefTime;
		int iCrusadeT;//1=soldier, 2=constructor, 3=commander
		int iRank;
		int iCurrentNo; // ShadowEvil Updated DWORD to int
		char cCharName[12];
		char cSubject[21];
	} m_stTitles[MAXGUILDNAMES];
	
	class Title * m_pTitles[MAXMENUITEMS];//besk: these are the player's own titles
	DWORD m_dwReqTitlesTime;

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
	void WriteUsername(char* cName, bool RemUser);
	void ReadUsername();
	void _LoadItemContents();
	BOOL __bDecodeContentsAndBuildItemList(char * pBuffer);
	CItem * m_pBuildItemListStats[MAXITEMSTATS];
	bool isCommon, isUncommon, isRare, isEpic, isLegendary;
	void isValue(uint32 value);
	bool isItemLoaded;
	void HandleItemDescription(CItem * item);

	// Monster kill event xRisenx
	void NotifyMsg_NpcHuntingON();
	void NotifyMsg_NpcHuntingOFF();
	void NotifyMsg_NpcHuntingWinner(char * pData);
	// Monster kill event xRisenx

	bool m_bHackMoveBlocked; // Anti Hack xRisenx
	bool CheckHackProcesses(); // Anti Hack xRisenx
	bool CheckHackDLLProcesses(); // Anti Hack xRisenx
	bool CheckHackProcessesMulti(); // Anti Hack xRisenx

};

#endif // !defined(AFX_GAME_H__0089D9E3_74E6_11D2_A8E6_00001C7030A6__INCLUDED_)
