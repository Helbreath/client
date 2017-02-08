// Game.cpp: implementation of the CGame class.
//
//////////////////////////////////////////////////////////////////////


#include "newsocket.h"
#include "Game.h"
#include "..\shared\buffer.h"
#include <algorithm>
#include <iostream>

#include "lan_eng.h"
#include <boost/asio/ssl.hpp>
#include "Awesomium\WebKeyboardEvent.h"
#include <vector2d.h>

using namespace Awesomium;

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

char _cDrawingOrder[]            = {0, 1, 0, 0, 0, 0, 0, 1, 1};
char _cMantleDrawingOrder[]      = {0, 1, 1, 1, 0, 0, 0, 2, 2};
char _cMantleDrawingOrderOnRun[] = {0, 1, 1, 1, 1, 1, 1, 1, 1};


short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;
int _tmp_iStatus;
short _tmp_sHeadApprValue, _tmp_sBodyApprValue, _tmp_sArmApprValue, _tmp_sLegApprValue; // Re-Coding Sprite xRisenx
char  _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
int   _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY; // 21.171 2002-6-14
uint16_t  _tmp_wObjectID;
char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
uint16_t  wFocusObjectID;
short sFocus_dX, sFocus_dY;
char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
int iFocuiStatus;
int   iFocusApprColor;
short sFocusHeadApprValue, sFocusBodyApprValue, sFocusArmApprValue, sFocusLegApprValue; // Re-Coding Sprite xRisenx
int iHungerStatus = 100; // Hunger Bar xRisenx
bool m_bStuckHack = false; // Anti Hack xRisenx


#ifdef MonsterBarClient
bool bNpcBar = false;
#endif

std::unordered_map<uint8_t, const char*> g_socketMap;

uint64_t unixtime()
{
#ifdef WIN32
	struct __timeb64 tstruct;
	_ftime64_s(&tstruct);
#else
	struct timeb tstruct;
	ftime(&tstruct);
#endif
	return tstruct.millitm + tstruct.time * 1000;
}

uint32_t unixseconds()
{
#ifdef WIN32
	struct __timeb64 tstruct;
	_ftime64_s(&tstruct);
#else
	struct timeb tstruct;
	ftime(&tstruct);
#endif
	return tstruct.time;
}

bool CGame::OnEvent(const irr::SEvent& event)
{
	if (G_pGame->htmlUI) {
		G_pGame->htmlUI->view->Focus();
	}
    if (event.MouseInput.Event != irr::EMIE_MOUSE_MOVED)
    {
        //AddEventList("Irrlicht Injected Successfully.", 10);
    }
    // Move out of if(true) at a later date when all UI elements are updated
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
        {
			htmlUI->view->InjectMouseDown(Awesomium::MouseButton::kMouseButton_Left);
            if (wasinactive)
            {
                wasinactive = false;
                return false;
            }
            //context.injectMouseButtonDown(MouseButton::LeftButton);
            m_stMCursor.LB = true;
        }
        else if (event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
		{
			htmlUI->view->InjectMouseDown(Awesomium::MouseButton::kMouseButton_Right);
            if (wasinactive)
            {
                wasinactive = false;
                return false;
            }
            //context.injectMouseButtonDown(MouseButton::RightButton);
            m_stMCursor.RB = true;
        }
        else if (event.MouseInput.Event == irr::EMIE_MMOUSE_PRESSED_DOWN)
		{
			htmlUI->view->InjectMouseDown(Awesomium::MouseButton::kMouseButton_Middle);
            if (wasinactive)
            {
                wasinactive = false;
                return false;
            }
            //context.injectMouseButtonDown(MouseButton::MiddleButton);
            m_stMCursor.MB = true;
        }
        else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP)
        {
            m_stMCursor.LB = false;
			htmlUI->view->InjectMouseUp(Awesomium::MouseButton::kMouseButton_Left);
            //context.injectMouseButtonUp(MouseButton::LeftButton);
        }
        else if (event.MouseInput.Event == irr::EMIE_RMOUSE_LEFT_UP)
		{
			htmlUI->view->InjectMouseDown(Awesomium::MouseButton::kMouseButton_Right);
            m_stMCursor.RB = false;
            //context.injectMouseButtonUp(MouseButton::RightButton);
        }
        else if (event.MouseInput.Event == irr::EMIE_MMOUSE_LEFT_UP)
		{
			htmlUI->view->InjectMouseDown(Awesomium::MouseButton::kMouseButton_Middle);
            m_stMCursor.MB = false;
            //context.injectMouseButtonUp(MouseButton::MiddleButton);
        }
        else if (event.MouseInput.Event == irr::EMIE_LMOUSE_DOUBLE_CLICK)
        {
            //context.injectMouseButtonDoubleClick(MouseButton::LeftButton);
        }
        else if (event.MouseInput.Event == irr::EMIE_LMOUSE_TRIPLE_CLICK)
        {
            //context.injectMouseButtonTripleClick(MouseButton::LeftButton);
        }
        else if (event.MouseInput.Event == irr::EMIE_RMOUSE_DOUBLE_CLICK)
        {
            //context.injectMouseButtonDoubleClick(MouseButton::RightButton);
        }
        else if (event.MouseInput.Event == irr::EMIE_RMOUSE_TRIPLE_CLICK)
        {
            //context.injectMouseButtonTripleClick(MouseButton::RightButton);
        }
        else if (event.MouseInput.Event == irr::EMIE_MOUSE_WHEEL)
        {
            // TODO: get values?
			m_stMCursor.sZ = event.MouseInput.Wheel;
			htmlUI->view->InjectMouseWheel(event.MouseInput.Wheel * 16, 0);
            //mouse wheel for dialogs?
        }
        else if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED)
		{
			htmlUI->view->InjectMouseMove(event.MouseInput.X, event.MouseInput.Y);
            //context.injectMousePosition(event.MouseInput.X, event.MouseInput.Y);
		}
        return false;
    }
    else
    {
        // Only processed if a UI element does not accept input from keypress

        // Remember whether each key is down or up
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
        {
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
            if (event.KeyInput.PressedDown)
            {
                //if (GetText(0, WM_CHAR, event.KeyInput.Key, 0))
                //	return true;
				//context.injectKeyDown((Key::Scan)event.KeyInput.Key);
				WebKeyboardEvent keyEvent = WebKeyboardEvent();
				keyEvent.type = WebKeyboardEvent::kTypeKeyDown;
				keyEvent.virtual_key_code = event.KeyInput.Key;
				keyEvent.native_key_code = event.KeyInput.Key;

				char* buf = new char[20];
				GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buf);
				strcpy(keyEvent.key_identifier, buf);
				delete[] buf;

				if (event.KeyInput.Control) {
					keyEvent.modifiers |= WebKeyboardEvent::kModControlKey;
				}
				else if (event.KeyInput.Shift) {
					keyEvent.modifiers |= WebKeyboardEvent::kModShiftKey;
				}

				G_pGame->htmlUI->view->InjectKeyboardEvent(keyEvent);


				if (event.KeyInput.Char && event.KeyInput.Key != KEY_TAB) {
					WebKeyboardEvent keyEvent2 = WebKeyboardEvent();
					keyEvent2.type = WebKeyboardEvent::kTypeChar;
					keyEvent2.text[0] = event.KeyInput.Char;

					if (event.KeyInput.Control) {
						keyEvent2.modifiers |= WebKeyboardEvent::kModControlKey;
					}
					else if (event.KeyInput.Shift) {
						keyEvent2.modifiers |= WebKeyboardEvent::kModShiftKey;
					}

					G_pGame->htmlUI->view->InjectKeyboardEvent(keyEvent2);
				}
				
                OnKeyDown(event.KeyInput.Key);
                OnSysKeyDown(event.KeyInput.Key);
                //lastchar = event.KeyInput.Key;
                //context.injectChar((Key::Scan)event.KeyInput.Key);
            }
            else
            {
				//context.injectKeyUp((Key::Scan)event.KeyInput.Key);

				if (event.KeyInput.Char) {
					WebKeyboardEvent keyEvent = WebKeyboardEvent();
					keyEvent.type = WebKeyboardEvent::kTypeKeyUp;
					keyEvent.virtual_key_code = event.KeyInput.Key; // native_key_code = event.KeyInput.Key;
					G_pGame->htmlUI->view->InjectKeyboardEvent(keyEvent);

					if (event.KeyInput.Control) {
						keyEvent.modifiers = WebKeyboardEvent::kModControlKey;
					}
					else if (event.KeyInput.Shift) {
						keyEvent.modifiers = WebKeyboardEvent::kModShiftKey;
					}
				}

                OnKeyUp(event.KeyInput.Key);
                OnSysKeyUp(event.KeyInput.Key);
			}
            return false;
        }
	}
	return false;
}

void CGame::DrawScene(uint32_t ptime)
{
	//driver->beginScene(true, true, video::SColor(255,120,102,136));
	driver->beginScene(true, true);
	mtime = ptime;
	//drawfunc();
	//m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->DrawSprite(-1, -1, 0);
	//DrawFPS2();
	//DrawMouse();

	driver->endScene();
}
void CGame::DrawVersion2()
{
	//sprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->DrawRGB(14, 463, 19, 140, 140, 140, mtime);
	// Upper Version
	//char version[20];
	//sprintf(version, "V. %d.%.2d", UPPER_VERSION, LOWER_VERSION);
	//font[TESTFONT]->draw(version, rect<s32>(36, 463, 100, 475), SColor(255, 140, 140, 140));
	//sprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->DrawRGB(42, 463, 18, 140, 140, 140, mtime);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CGame::ReadSettings()
{
	m_sMagicShortCut = -1;
	m_sRecentShortCut = -1;
	for( int i=0 ; i<6 ; i++ ) m_sShortCut[i] = -1;

	uint32_t val;
	if((val = ReadSettingsVar("ShowFPS")) == -1)
		m_bShowFPS = false;
	else
		m_bShowFPS = val;

	if((val = ReadSettingsVar("ShowTime")) == -1)
		m_bShowFPS = false;
	else
		m_bShowFPS = val;

	if((val = ReadSettingsVar("ShowGrid")) == -1)
		m_showGrid = false;
	else
		m_showGrid = val;

	if((val = ReadSettingsVar("ShowAllDmg")) == -1)
		m_showAllDmg = true;
	else
		m_showAllDmg = val;

	if((val = ReadSettingsVar("BigItems")) == -1)
		m_bigItems = false;
	else
		m_bigItems = val;

	if((val = ReadSettingsVar("PartyAutoAccept")) == -1)
		m_partyAutoAccept = false;
	else
		m_partyAutoAccept = val;

	if((val = ReadSettingsVar("DetailLevel")) == -1)
		m_cDetailLevel = false;
	else
		m_cDetailLevel = val;

	if((val = ReadSettingsVar("Music")) == -1)
		m_bMusicStat = true;
	else
		m_bMusicStat = val;

	if((val = ReadSettingsVar("Sound")) == -1)
		m_bSoundStat = true;
	else
		m_bSoundStat = val;

	if((val = ReadSettingsVar("EKScreenshot")) == -1)
		m_ekScreenshot = false;
	else
		m_ekScreenshot = val;

	if((val = ReadSettingsVar("TabbedNotification")) == -1)
		m_tabbedNotification = false;
	else
		m_tabbedNotification = val;

	if((val = ReadSettingsVar("ManuAutoFill")) == -1)
		m_manuAutoFill = false;
	else
		m_manuAutoFill = val;

	if((val = ReadSettingsVar("TimeStamp")) == -1)
		m_showTimeStamp = false;
	else
		m_showTimeStamp = val;

	if((val = ReadSettingsVar("WindowsKey")) == -1) 
		m_windowsKey = true;
	else
		m_windowsKey = val;

	if((val = ReadSettingsVar("ShowQuest")) == -1)
		m_displayQuest = false;
	else
		m_displayQuest = val;

	if((val = ReadSettingsVar("Remember")) == -1)
		b_cRemember = false;
	else
		b_cRemember = val;

	uint32_t ret;
	ret = ReadSettingsVar("Magic");

	if( ret > 0 && ret < 101 ) m_sMagicShortCut = ret - 1;
	else m_sMagicShortCut = -1;

	ret = ReadSettingsVar("ShortCut0");

	if( ret > 0 && ret < 201 ) m_sShortCut[0] = ret - 1;
	else m_sShortCut[0] = -1;

	ret = ReadSettingsVar("ShortCut1");

	if( ret > 0 && ret < 201 ) m_sShortCut[1] = ret - 1;
	else m_sShortCut[1] = -1;

	ret = ReadSettingsVar("ShortCut2");

	if( ret > 0 && ret < 201 ) m_sShortCut[2] = ret - 1;
	else m_sShortCut[2] = -1;

	ret = ReadSettingsVar("ShortCut3");

	if( ret > 0 && ret < 201 ) m_sShortCut[3] = ret - 1;
	else m_sShortCut[3] = -1;

	ret = ReadSettingsVar("ShortCut4");

	if( ret > 0 && ret < 201 ) m_sShortCut[4] = ret - 1;
	else m_sShortCut[4] = -1;

}

uint32_t CGame::ReadSettingsVar(const char * var)
{
// 	HKEY key;
// 	unsigned long dwDisp;
// 
// 	unsigned long val;
// 	unsigned long size = sizeof(unsigned long);
// 	if( RegCreateKeyExA( HKEY_CURRENT_USER, "Software\\Siementech\\Helbreath\\Settings", 0, 0,
// 		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &key, &dwDisp ) != ERROR_SUCCESS )
// 		return -1;
// 
// 	if( RegQueryValueExA(key, var, 0, 0, (LPBYTE)&val, &size) != ERROR_SUCCESS )
// 	{
// 		RegCloseKey(key);
// 		return -1;
// 	}
// 
// 	RegCloseKey(key);
// 	return val;
	return 0;
}

void CGame::WriteSettings()
{
	WriteSettingsVar("ShowFPS", m_bShowFPS);
	WriteSettingsVar("ShowTime", m_showTime);
	WriteSettingsVar("ShowGrid", m_showGrid);
	WriteSettingsVar("ShowAllDmg", m_showAllDmg);
	WriteSettingsVar("BigItems", m_bigItems);
	WriteSettingsVar("PartyAutoAccept", m_partyAutoAccept);
	WriteSettingsVar("DetailLevel", m_cDetailLevel);
	WriteSettingsVar("Music", m_bMusicStat);
	WriteSettingsVar("Sound", m_bSoundStat);
	WriteSettingsVar("EKScreenshot", m_ekScreenshot);
	WriteSettingsVar("TabbedNotification", m_tabbedNotification);
	WriteSettingsVar("ManuAutoFill", m_manuAutoFill);
	WriteSettingsVar("TimeStamp", m_showTimeStamp);
	WriteSettingsVar("WindowsKey", m_windowsKey);
	WriteSettingsVar("ShowQuest", m_displayQuest);
	WriteSettingsVar("Remember", b_cRemember);
	if( m_sMagicShortCut >= 0 && m_sMagicShortCut < 100 )
		WriteSettingsVar("Magic", m_sMagicShortCut + 1);

	if( m_sShortCut[0] >= 0 && m_sShortCut[0] < 200 )
		WriteSettingsVar("ShortCut0", m_sShortCut[0] + 1);

	if( m_sShortCut[1] >= 0 && m_sShortCut[1] < 200 )
		WriteSettingsVar("ShortCut1", m_sShortCut[1] + 1);

	if( m_sShortCut[2] >= 0 && m_sShortCut[2] < 200 )
		WriteSettingsVar("ShortCut2", m_sShortCut[2] + 1);

	if( m_sShortCut[3] >= 0 && m_sShortCut[3] < 200 )
		WriteSettingsVar("ShortCut3", m_sShortCut[3] + 1);

	if( m_sShortCut[4] >= 0 && m_sShortCut[4] < 200 )
		WriteSettingsVar("ShortCut4", m_sShortCut[4] + 1);
}

void CGame::WriteSettingsVar(const char * var, uint32_t val)
{
// 	HKEY key;
// 	unsigned long dwDisp;
// 
// 	unsigned long size = sizeof(unsigned long);
// 	if( RegCreateKeyExA( HKEY_CURRENT_USER, "Software\\Siementech\\Helbreath\\Settings", 0, 0,
// 		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &key, &dwDisp ) != ERROR_SUCCESS )
// 		return;
// 
// 	RegSetValueExA(key, var, 0, REG_DWORD, (LPBYTE)&val, sizeof(UINT) );
// 
// 	RegCloseKey(key);
}

void CGame::WriteUsername(const char* cName, bool RemUser)
{
	int i;
	FILE *fp = fopen("user.dat", "w+");

	if(fp != 0)
	{
		if(RemUser == true)
		{
			fprintf(fp, cName);
		} else {
			fprintf(fp, "");
		}
		fclose(fp);
	} else {
		return;
	}
}

void CGame::ReadUsername()
{
	FILE *pFile = fopen("user.dat", "r");
	if(pFile != 0)
	{
		fgets(m_cUserName, 12, pFile);
		puts(m_cUserName);
		fclose(pFile);
	}
}

void CGame::PutUIMsgQueue(shared_ptr<UIMsgQueueEntry> msg)
{
    uiqueue.push_back(msg);
}


shared_ptr<CGame::UIMsgQueueEntry> CGame::GetUIMsgQueue()
{
    shared_ptr<UIMsgQueueEntry> msg = uiqueue.front();
    uiqueue.pop_front();
    return msg;
}

shared_ptr<CGame::MsgQueueEntry> CGame::GetLoginMsgQueue()
{
    shared_ptr<CGame::MsgQueueEntry> msg = loginpipe.front();
    loginpipe.pop_front();
    return msg;
}

void CGame::PutMsgQueue(MsgQueue & q, char * data, uint32_t size)
{
	//poco_information(*logger, "PutMsgQueue()");
	shared_ptr<MsgQueueEntry> msg(new MsgQueueEntry);

	msg->data = data;
	msg->size = size;

	q.push_back(msg);
}

void CGame::PutMsgQueue(shared_ptr<CGame::MsgQueueEntry> msg, MsgQueue & q)
{
	q.push_back(msg);
}

shared_ptr<CGame::MsgQueueEntry> CGame::GetMsgQueue()
{
	shared_ptr<CGame::MsgQueueEntry> msg = socketpipe.front();
    socketpipe.pop_front();
	return msg;
}

void CGame::start(connection_ptr c)
{
	_socket = c;
	c->start();
}

void CGame::stop(connection_ptr c)
{
	if (c == nullptr)
	{
		//consoleLogger->trace("Invalid socket being closed");
		return;
	}
	try
	{
		c->stop();
		ChangeGameMode(GAMEMODE_ONCONNECTIONLOST);
        _socket.reset();
        gamemode = 0;
		//post socket closing
	}
	catch (std::exception& e)
	{
		printf("exception: %s", e.what());
	}
}

void CGame::handle_stop()
{
	stop(_socket);
	stop(new_connection_);
	io_service_.stop();
}
/*

char itoh(int num)
{
    switch (num)
    {
        case 1:
            return 49;
        case 2:
            return 50;
        case 3:
            return 51;
        case 4:
            return 52;
        case 5:
            return 53;
        case 6:
            return 54;
        case 7:
            return 55;
        case 8:
            return 56;
        case 9:
            return 57;
        case 0:
            return 48;
        case 10://a
            return 97;
        case 11://b
            return 98;
        case 12://c
            return 99;
        case 13://d
            return 100;
        case 14://e
            return 101;
        case 15://f
            return 102;
    }
    return 0;
}*/

void CGame::CreateSocket()
{
    loggedin = false;
    new_connection_ = boost::make_shared<connection>(io_service_, *this, request_handler_, ctx);
}


CGame::CGame()
	: io_service_(),
	signals_(io_service_),
    ctx(io_service_, boost::asio::ssl::context::sslv23)
{



/*
    char test[] = "-----BEGIN CERTIFICATE-----\n"
    "MIICETCCAXoCCQCmcAwBEbBNIDANBgkqhkiG9w0BAQsFADBNMQswCQYDVQQGEwJV\n"
    "UzELMAkGA1UECAwCUEExGDAWBgNVBAoMD1hhbmRpdW0gU3R1ZGlvczEXMBUGA1UE\n"
    "AwwOaGVsYnJlYXRoeC5uZXQwHhcNMTcwMTE4MDU0MDEwWhcNMjcwMTE2MDU0MDEw\n"
    "WjBNMQswCQYDVQQGEwJVUzELMAkGA1UECAwCUEExGDAWBgNVBAoMD1hhbmRpdW0g\n"
    "U3R1ZGlvczEXMBUGA1UEAwwOaGVsYnJlYXRoeC5uZXQwgZ8wDQYJKoZIhvcNAQEB\n"
    "BQADgY0AMIGJAoGBANRft0+jr0+mezGUXuqx1tQMqye6N27BkTQXvbD/pJrnCM2q\n"
    "l55d9K14D6i00FJ8q59S/PdTK2ocC0npqkLE1DdwEa1BHRvJ7IOMnhBGfFIrpYC7\n"
    "FaHwNgdMhxZBfJLXrJY00W/siUVg8visnWkW5MvnaYJYiYhFPDJV5PIc+PVPAgMB\n"
    "AAEwDQYJKoZIhvcNAQELBQADgYEAKeY/qUS30N393j7udFVh3nn+SpfQaHb7UuuK\n"
    "/AQ4+Tw0QGDz3g0N8MfLcmk36MWtR/48s/oVEZTpUd8NfFfC45oZDYcCVfiY95V2\n"
    "2emWMi3F6Z0N1Chk7b2VBxkN+IGNEBVTwyyWFPIqy9xywu/TBsYBQFWlrJNMZeD/\n"
    "0t2XnBg=\n"
    "-----END CERTIFICATE-----\n";*/
    char test[] = {
        0x78, 0x66, 0x64, 0x66, 0x70, 0x4d, 0x4c, 0x4c, 0x54, 0x59, 0x6b, 0x48, 0xa8, 0x5d, 0x5f, 0xaa,
        0xa1, 0xa2, 0xae, 0xaa, 0x5f, 0xae, 0xb4, 0xb6, 0xb8, 0xb6, 0xb4, 0x91, 0x40, 0x4a, 0x48, 0xb0,
        0x80, 0x9f, 0x8e, 0x88, 0x8c, 0x9b, 0xb2, 0x80, 0x8e, 0x9a, 0x8e, 0xb6, 0x8e, 0xb2, 0x7a, 0xb1,
        0xa0, 0x8d, 0xa9, 0xa5, 0x94, 0xaf, 0xa0, 0x95, 0xad, 0x70, 0x76, 0x7a, 0x6b, 0x64, 0x68, 0x8c,
        0x74, 0x30, 0x7b, 0x49, 0x4c, 0x52, 0x3e, 0x4d, 0x4c, 0x4f, 0x49, 0x55, 0x20, 0x22, 0x7e, 0x7c,
        0x2e, 0x5a, 0x50, 0x2f, 0x59, 0x52, 0x50, 0x2c, 0x50, 0x60, 0x51, 0x5d, 0x05, 0x5e, 0x79, 0x4e,
        0x07, 0x06, 0x08, 0x20, 0x0a, 0x02, 0xf0, 0x1e, 0x10, 0x0c, 0x08, 0x3c, 0x2e, 0x1e, 0x2c, 0x2e,
        0x73, 0x20, 0x2f, 0x2a, 0x1a, 0x2d, 0x0a, 0x15, 0x01, 0x2d, 0x28, 0x74, 0x00, 0x0f, 0x20, 0x6b,
        0xa3, 0xad, 0xa4, 0x59, 0xb3, 0xaf, 0x5a, 0x73, 0xb2, 0x5e, 0x7e, 0x59, 0xbc, 0x55, 0xaa, 0x47,
        0x71, 0x4c, 0x71, 0xae, 0x5b, 0x56, 0xa1, 0x5e, 0x52, 0xaa, 0xb8, 0x5e, 0x48, 0x85, 0xb0, 0x7c,
        0xb2, 0x98, 0xa8, 0x8c, 0x99, 0xbc, 0x98, 0xb5, 0x95, 0xb7, 0xa0, 0xa3, 0x9d, 0x80, 0x8c, 0xb0,
        0xb0, 0x7e, 0x91, 0x93, 0x9c, 0x78, 0xab, 0x8b, 0x8e, 0x99, 0x94, 0x9f, 0x6e, 0x78, 0x84, 0x9f,
        0x40, 0x7f, 0x44, 0x4f, 0x58, 0x73, 0x54, 0x4f, 0x50, 0x00, 0x22, 0x33, 0x4e, 0x21, 0x24, 0x49,
        0x4e, 0x70, 0x24, 0x5f, 0x28, 0x3d, 0x54, 0x2f, 0x40, 0x3b, 0x54, 0x2f, 0x48, 0x78, 0x09, 0x5c,
        0x25, 0x0d, 0x05, 0x06, 0x1c, 0x3c, 0x3a, 0x00, 0x1c, 0x22, 0x0f, 0x1d, 0x1c, 0x22, 0x2a, 0x2e,
        0x72, 0x25, 0x1d, 0x1e, 0x65, 0x2e, 0x17, 0x16, 0x2c, 0x74, 0x12, 0x2a, 0x3c, 0x1e, 0x0c, 0x30,
        0x4c, 0xb0, 0x4e, 0x5e, 0x48, 0x4e, 0xbb, 0x4c, 0x4f, 0x4a, 0xa2, 0x49, 0x4a, 0xa1, 0x5d, 0xb1,
        0xac, 0x78, 0xa4, 0xaf, 0xbc, 0x4b, 0x4b, 0x41, 0x78, 0x5d, 0x7b, 0x4f, 0x5a, 0xa3, 0x6a, 0x89,
        0x90, 0xfc, 0x99, 0xfa, 0xa5, 0x88, 0xb7, 0xbd, 0xae, 0x45, 0x8c, 0xa3, 0xa0, 0xad, 0x9c, 0xac,
        0xac, 0xba, 0x9c, 0xae, 0xac, 0x78, 0x7a, 0x94, 0x8c, 0x90, 0x9d, 0x78, 0x94, 0x61, 0x89, 0x67,
        0x54, 0x53, 0x59, 0x24, 0x28, 0x4c, 0x7c, 0x3e, 0x25, 0x23, 0x58, 0x3c, 0x4a, 0x55, 0x3b, 0x7c,
        0x2f, 0x5a, 0x50, 0x21, 0x56, 0x70, 0x59, 0x2a, 0x73, 0x61, 0x4e, 0x55, 0x4c, 0x42, 0x4c, 0x31,
        0xc5, 0x0d, 0x18, 0x0a, 0x0f, 0x28, 0x18, 0xf3, 0x0c, 0x16, 0x10, 0x0c, 0x25, 0x32, 0x02, 0x2c,
        0x2d, 0x2a, 0x25, 0x19, 0x09, 0x7f, 0x33, 0xc8, 0x75, 0x7d, 0x3b, 0x30, 0x60, 0x6e, 0x79, 0x0c,
        0x50, 0x53, 0xbc, 0xba, 0xbb, 0x72, 0xbf, 0x52, 0x58, 0xba, 0x80, 0xae, 0xb9, 0xa1, 0x81, 0xbc,
        0xad, 0x44, 0x5f, 0x5a, 0x5b, 0x79, 0x41, 0xaf, 0xba, 0x7b, 0x51, 0x79, 0x61, 0x4c, 0x44, 0xa1,
        0xbc, 0xc5, 0xa7, 0xfe, 0xf8, 0xaf, 0xf8, 0x88, 0xfc, 0xff, 0x8f, 0xfd, 0x84, 0xc3, 0xc3, 0xad,
        0xa5, 0xb3, 0x78, 0xbe, 0xa4, 0x9c, 0xb2, 0x93, 0x8f, 0x9f, 0x96, 0xb9, 0x62, 0x6c, 0x8e, 0xa3,
        0x39, 0x3b, 0x38, 0x20, 0x57, 0x4e, 0x70, 0x4f, 0x2f, 0x00, 0x4c, 0x2a, 0x3c, 0x2d, 0x2b, 0x21,
        0x71, 0x25, 0x32, 0x22, 0x52, 0x56, 0x75, 0x4b, 0x2d, 0x50, 0x4d, 0x2d, 0x44, 0x7d, 0x63, 0x5a,
        0x0e, 0xf0, 0xc1, 0x0d, 0x2c, 0x0b, 0x3a, 0x15, 0x32, 0x2f, 0x14, 0x33, 0x7b, 0x15, 0x31, 0x0d,
        0x25, 0x27, 0x13, 0x79, 0x15, 0x1a, 0x33, 0x33, 0x02, 0x38, 0x7e, 0x72, 0x18, 0x19, 0x6a, 0x3b,
        0xb1, 0xa2, 0xbe, 0xa5, 0x5a, 0xb4, 0x5a, 0x7e, 0x58, 0x81, 0xb5, 0xaa, 0x54, 0xa5, 0x58, 0x4a,
        0x54, 0x43, 0xad, 0x5b, 0xaf, 0x55, 0x5d, 0xbe, 0x5b, 0x52, 0x4e, 0xb0, 0x43, 0x59, 0x43, 0xb2,
        0xa2, 0x86, 0x89, 0xc1, 0x8c, 0x82, 0x8c, 0xbc, 0x8f, 0x9a, 0xa0, 0x91, 0xa6, 0x80, 0x99, 0xaa,
        0x83, 0x71, 0x8e, 0xa5, 0xac, 0x92, 0xac, 0x97, 0xad, 0x9a, 0xa8, 0xaf, 0x6a, 0x9a, 0x8c, 0xb2,
        0x46, 0x2e, 0x50, 0x64, 0x3c, 0x5e, 0x5e, 0x70, 0x7b, 0x59, 0x7e, 0x42, 0x3e, 0x45, 0x3a, 0x7e,
        0x4f, 0x21, 0x5d, 0x43, 0x3e, 0x71, 0x75, 0x08, 0x5e, 0x7b, 0x4d, 0x5a, 0x6c, 0x4b, 0x51, 0x3c,
        0x10, 0x3e, 0x3c, 0x00, 0xd5, 0xf0, 0x00, 0x12, 0xff, 0xf4, 0x1f, 0x3c, 0xc3, 0x22, 0x2a, 0x2f,
        0x75, 0x3c, 0x02, 0x3b, 0x11, 0x3b, 0x14, 0x0d, 0x17, 0x0c, 0x74, 0x70, 0x3e, 0x39, 0x04, 0x1c,
        0xbf, 0x5d, 0x7a, 0x7f, 0x7b, 0xbc, 0x72, 0xb4, 0xa1, 0x4e, 0xa1, 0x5f, 0x43, 0x5e, 0x4f, 0xbb,
        0x59, 0x41, 0xad, 0x4d, 0xae, 0xbf, 0xbc, 0x74, 0x45, 0xaf, 0x40, 0x48, 0x4e, 0x59, 0x6d, 0x6a,
        0x94, 0xf2, 0xfc, 0x9d, 0xfd, 0xd5, 0xf1, 0xae, 0xb8, 0xa0, 0x94, 0xb2, 0xbe, 0xa9, 0xbd, 0x99,
        0xbb, 0x99, 0xb8, 0xa8, 0x8b, 0x74, 0xba, 0x81, 0xbd, 0x81, 0xa9, 0x63, 0x66, 0x81, 0xa6, 0x8a,
        0x42, 0x59, 0x4c, 0x49, 0x59, 0x5f, 0x3a, 0x3a, 0x04, 0x20, 0x4d, 0x5b, 0x24, 0x42, 0x78, 0x3a,
        0x73, 0x72, 0x72, 0x7e, 0x32, 0x5f, 0x21, 0x70, 0x44, 0x2d, 0x58, 0x2d, 0x5a, 0x67, 0x61, 0x49,
        0x19, 0x06, 0x11, 0x2e, 0x0f, 0xf0, 0xc9, 0xf3, 0x3f, 0xfd, 0x23, 0x35, 0x2d, 0x08, 0x34, 0xe9,
        0x38, 0xc6, 0xc4, 0xc6, 0x30, 0x2e, 0x15, 0x2f, 0xcb, 0x2c, 0x2c, 0x19, 0x1f, 0x0a, 0x0d, 0x0a,
        0x4e, 0x4a, 0x5f, 0x4e, 0x70, 0x76, 0x74, 0x76, 0x78, 0x15, 0x0b
    };
    int isize = 779;// sizeof(test);

/*
    char dh[] = "-----BEGIN DH PARAMETERS-----\n"
        "MIGHAoGBAPxoRNblo3SD2aIKqtxGAMGAiB/dT1pZOd0cwFQ0+tn4WBUJLW/yE/pX\n"
        "BUEEZUPkeXd6D8kEy+jLQhoI/bdiO9ELiszFgv9so6q05beYSsA4vkEqfoZfGcPy\n"
        "rOxvSLvl3mIjnM0BC4jySAUnl4wU6OEUH/DBTrOo8Wx77TE1AYS7AgEC\n"
        "-----END DH PARAMETERS-----\n";*/
    char dh[] = "-----BEGIN DH PARAMETERS-----\n"
        "MIICCAKCAgEArOGgxAq72jKvDwwU4PqVTpEUAJqFboyaXUu/E9p7dE1BLfgpG59o\n"
        "xYPI3iS18aXnZL+v7J8kDsLj0tbAh0H8VC0GesgUSOprv8AlwErrk6H1PGNQEIhR\n"
        "v7RLq2TXp4hJMyMjuQ+m7oKNCZ910Igxa185qTN7cNs6WCiFVEgXVki5Lb4F+Jn4\n"
        "9Q3BvofDuDCUX3xOEGgBQemfnlaEtaJyd2zz7JCGMZ4AY0lPXMktxrY6MGEcFema\n"
        "PU3SxRdlIdBfqLn2+tE0smPRuNf3zAJrbp9SsPevwuxRbrPd+z86SpAvbBB93tAB\n"
        "1J8L1+dy2DvdR7+MAJX+IxSToF6j0Kk1I8vugCV2Sn7p6BNrjAw3OUP24pWBi1I/\n"
        "6otETlb2OhLvcy2D/KUxYP56XLu4SqSEI1EnPSYfNUto870Ze0c94gLaIKrpVi5B\n"
        "TLKN+Lkt5bO2GONSKdBhCKpisxHAZr0dYizEDE1kLhTC9xrDQUEoOC32ZXY8HH2e\n"
        "7kX+/uPDm21OwmY0Qput4AHhe5aeMgrX//A3mHHMhlwzzDI+4x74lELxt4hsOSme\n"
        "bw7N6vl1s4ECHrL/hPnlubW/g2T0WrMFJVHiToPMN9x0Cl6h79PTyYZIVLSzxkLq\n"
        "oOs10FJt0IBvSDSCpaLTKIUCwLHPzqfSoeaenKl0wrRvuwyqdPu2qdsCAQI=\n"
        "-----END DH PARAMETERS-----\n";

                    /*
                    cout << endl;
                    for (int i = 0; i < isize; i++)
                    {
                    test[i] += (i ^ 163);
                    test[i] = test[i] ^ (163 ^ (isize - i));
                    cout << "0x" << itoh((test[i] & 0xF0) >> 4) << itoh(test[i] & 0x0F) << ", ";
                    }*/


    for (int i = 0; i < isize; i++)
    {
        test[i] = test[i] ^ (163 ^ (isize - i));
        test[i] -= (i ^ 163);
    }
    string str = test;
    string dhstr = dh;
    boost::asio::const_buffer buffer_(str.c_str(), str.length());
    boost::asio::const_buffer dh_buff(dhstr.c_str(), dhstr.length());
    ctx.set_options(
        boost::asio::ssl::context::default_workarounds
        | boost::asio::ssl::context::no_sslv2
        | boost::asio::ssl::context::single_dh_use);
    ctx.set_verify_mode(boost::asio::ssl::verify_peer);
    ctx.add_certificate_authority(buffer_);
    ctx.use_tmp_dh(dh_buff);
    //ctx.load_verify_file("server.crt");

	selectedchar = nullptr;
	autologin = false;
	_socket = nullptr;

	wasinactive = false;
	fullscreenswap = false;
	vsync = false;
	oldmode = gamemode = 0;
	int i;
	srand( (unsigned)time( 0 ) );
	ReadSettings();

	foregroundfpstarget = 120;
	backgroundfpstarget = 45;
	foregroundframetime = 1000 / foregroundfpstarget;
	backgroundframetime = 1000 / backgroundfpstarget;


//	screenwidth = 800;
//	screenheight = 600;
    //SetResolution(640, 480);
	//SetResolution(800,600);
	SetResolution(1024,768);
	//SetResolution(1280, 1024);
    //SetResolution(1920, 1200);
    
    //SetResolution(1280, 720);
    //SetResolution(1366, 768);
    //SetResolution(1600, 900);
    //SetResolution(1920, 1080);

#ifdef _DEBUG
	m_bToggleScreen = true;
#else
	m_bToggleScreen = false;
#endif

	m_cLoading = 0;
	m_bZoomMap = false;
	m_bIsFirstConn = true;
	m_iItemDropCnt = 0;
	m_bItemDrop = false;
	m_bIsSpecial = false;
	m_cGameMode = GAMEMODE_ONLOADING;
	m_cWhisperIndex = MAXWHISPERMSG;
	m_cGameModeCount = 0;
	memset(m_cMapName, 0, sizeof(m_cMapName));
	_socket = nullptr;
//	m_pGSock   = 0;
	m_pMapData = 0;
	m_cCommandCount  = 0;
	m_dwCommandTime = 0; //v2.15 SpeedHack
	m_sPlayerX = 0;
	m_sPlayerY = 0;
	m_sViewDX  = 0;
	m_sViewDY  = 0;
	m_cCommand = OBJECTSTOP;
	m_bIsObserverMode = false;
	m_ekSSTime = 0;
	m_showBtns = false;
	m_weight = 0;
	m_droppedOnIconbar = false;

	//m_voteUrls.push_back("hbtop50.com/button.php?u=Legion"); // Voting ? xRisenx

	for (i = 0; i < MAXSPRITES; i++) m_pSprite[i] = 0;
	for (i = 0; i < MAXTILES; i++) m_pTileSpr[i] = 0;
	for (i = 0; i < MAXEFFECTSPR; i++) m_pEffectSpr[i] = 0;
	m_pBGM = nullptr;
	for (i = 0; i < MAXSOUNDEFFECTS; i++)
	{
        m_pCSound[i]  = nullptr;
		m_pESound[i]  = nullptr;
		m_pMSound[i]  = nullptr;
	}

	for (i = 0; i < 13; i++){
		strcpy(friendsList[i].friendName, "");
		friendsList[i].online = false;
	}
	for (i = 0; i < MAXCHATMSGS; i++) m_pChatMsgList[i] = 0;

	for (i = 0; i < MAXEFFECTS; i++) m_pEffectList[i] = 0;

	for (i = 0; i < MAXITEMS; i++) m_pItemList[i] = 0;

	for (i = 0; i < MAXBANKITEMS; i++) m_pBankList[i] = 0;

	for (i = 0; i < MAXDIALOGBOX; i++) m_cDialogBoxOrder[i] = 0;

	for (i = 0; i < MAXMAGICTYPE; i++) m_pMagicCfgList[i] = 0;

	for (i = 0; i < MAXSKILLTYPE; i++) m_pSkillCfgList[i] = 0;

	for (i = 0; i < TEXTDLGMAXLINES; i++) {
		m_pMsgTextList[i] = 0;
		m_pMsgTextList2[i] = 0;
		m_pAgreeMsgTextList[i] = 0;
	}

	for (i = 0; i < MAXBUILDITEMS; i++) m_pBuildItemList[i] = 0;

	for (i = 0; i < MAXBUILDITEMS; i++) m_pDispBuildItemList[i] = 0;
	// Gladiator Arena xRisenx
	/*for (i = 0; i < MAXARENAPLAYERS; i++)
{
   m_stArenaPlayers[i].iKills = 0;
   m_stArenaPlayers[i].iDeaths = 0;
   ZeroMemory(m_stArenaPlayers[i].cCharName, sizeof(m_stArenaPlayers[i].cCharName));
}*/
	// Gladiator Arena xRisenx
	// Crafting:
	for (i = 0; i < MAXBUILDITEMS; i++) m_pCraftItemList[i] = 0;
	for (i = 0; i < MAXBUILDITEMS; i++) m_pDispCraftItemList[i] = 0;

	for (i = 0; i < MAXITEMNAMES; i++) m_pItemNameList[i] = 0;

	for (i = 0; i < MAXLEVEL +20; i++) m_levelExpTable[i] = iGetLevelExp(i);

	m_stMCursor.cPrevStatus       = CURSORSTATUS_NULL;
	m_stMCursor.dwSelectClickTime = 0;

	memset(m_cLogServerAddr, 0, sizeof(m_cLogServerAddr));
	m_iGameServerMode = 2; // Snoopy: Default is INTERNET

	for (i = 0; i < MAXMENUITEMS; i++)
		m_pItemForSaleList[i] = 0;

	m_cash = 0;

	m_bCtrlPressed  = false;
	m_bShiftPressed = false;
	m_bEnterPressed = false;
	m_bEscPressed	= false;
	m_bSoundFlag = true;
	m_dwDialogCloseTime = 0;
	m_iTimeLeftSecAccount = 0;
	m_iTimeLeftSecIP      = 0;
	m_bWhisper = true;
	m_bShout   = true;

	signals_.add(SIGINT);
	signals_.add(SIGTERM);
#if defined(SIGQUIT)
	signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
	signals_.async_wait(boost::bind(&CGame::handle_stop, this));
}

CGame::~CGame()
{}

void CGame::SetupDialogBox(int dialog,  short X, short Y, short background, int backFrame, int titleTxtFrame, bool trans)
{
	short sizeX = m_pSprite[background]->m_stBrush[backFrame].szx;
	short sizeY = m_pSprite[background]->m_stBrush[backFrame].szy;
	m_dialogBoxes[dialog].SetupDialog(dialog, X, Y, sizeX, sizeY, background, backFrame, titleTxtFrame, trans);
}

void CGame::SetupDialogBoxes()
{
	//Character-Info Dialog(F5)
	SetupDialogBox(DIALOG_CHARACTER, 50, 78, SPRID_INTERFACE_ND_TEXT, 0, -1, true);
	//m_dialogBoxes[1].SetupDialog(DIALOG_CHARACTER, 30, 30, 270, 376);
	m_dialogBoxes[1].SetupHandlers(GAMEFUNCT(DrawDialogBox_Character), GAMEFUNCT(DlgBoxClick_Character), 
		GAMEFUNCT(DlgBoxDoubleClick_Character), GAMEFUNCT(bItemDrop_Character));

	//Inventory Dialog(F6)
	SetupDialogBox(DIALOG_INVENTORY, 472, 297, SPRID_INTERFACE_ND_INVENTORY, 0, -1, true);
	//m_dialogBoxes[2].SetupDialog(DIALOG_INVENTORY, 380, 210, 225, 185);
	m_dialogBoxes[2].SetupHandlers(GAMEFUNCT(DrawDialogBox_Inventory), GAMEFUNCT(DlgBoxClick_Inventory),
		GAMEFUNCT(DlgBoxDoubleClick_Inventory), GAMEFUNCT(bItemDrop_Inventory));

	//Magic Circle Dialog(F7)
	SetupDialogBox(DIALOG_MAGIC, 429, 113, SPRID_INTERFACE_ND_GAME1, 1, 7, true);
	//m_dialogBoxes[3].SetupDialog(DIALOG_MAGIC, 337, 57, 258, 328);
	m_dialogBoxes[3].SetupHandlers(GAMEFUNCT(DrawDialogBox_Magic), GAMEFUNCT(DlgBoxClick_Magic),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	// Item drop confirmation
	SetupDialogBox(DIALOG_ITEMDROP, 0, 0, SPRID_INTERFACE_ND_GAME1, 2);
	//m_dialogBoxes[4].SetupDialog(DIALOG_ITEMDROP, 0, 0, 270, 105);
	m_dialogBoxes[4].SetupHandlers(GAMEFUNCT(DrawDialogBox_ItemDrop), GAMEFUNCT(DlgBoxClick_ItemDrop),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	// Age <15 box !?!?!?
	m_dialogBoxes[5].SetupDialog(DIALOG_15AGEMSG, 0, 0, 310, 170);
	m_dialogBoxes[5].SetupHandlers(GAMEFUNCT(DrawDialogBox_15AgeMsg), GAMEFUNCT(DlgBoxClick_15AgeMsg),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	// ** This is a battle area **
	SetupDialogBox(DIALOG_WARNINGMSG, 160, 2, SPRID_INTERFACE_ND_GAME4, 2);
	//m_dialogBoxes[6].SetupDialog(DIALOG_WARNINGMSG, 160, 2, 310, 170);
	m_dialogBoxes[6].SetupHandlers(GAMEFUNCT(DrawDialogBox_WarningMsg), GAMEFUNCT(DlgBoxClick_WarningMsg),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Guild Menu Dialog
	SetupDialogBox(DIALOG_GUILDMENU, 337, 57, SPRID_INTERFACE_ND_GAME2, 2,  19);
//	m_dialogBoxes[7].SetupDialog(DIALOG_GUILDMENU, 337, 57, 258, 339);
	m_dialogBoxes[7].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildMenu), GAMEFUNCT(DlgBoxClick_GuildMenu),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Guild Operation Dialog
	SetupDialogBox(DIALOG_GUILDOPERATION, 337, 57, SPRID_INTERFACE_ND_GAME2, 0,  19);
//	m_dialogBoxes[8].SetupDialog(DIALOG_GUILDOPERATION, 337, 57, 295, 346);
	m_dialogBoxes[8].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildOperation), GAMEFUNCT(DlgBoxClick_GuildOp),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Guide Map Dialog
	m_dialogBoxes[9].SetupDialog(DIALOG_GUIDEMAP, 672, 0, 128, 128);
	m_dialogBoxes[9].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuideMap), GAMEFUNCT(DlgBoxClick_GuideMap),
		GAMEFUNCT(DlgBoxDoubleClick_GuideMap),GAMEFUNCT(emptyfunc));

	//Chatting History Dialog(F9)
	SetupDialogBox(DIALOG_CHAT, 211, 374, SPRID_INTERFACE_ND_GAME2, 4,  22, true);
//	m_dialogBoxes[10].SetupDialog(DIALOG_CHAT, 135, 273, 364, 162);
	m_dialogBoxes[10].SetupHandlers(GAMEFUNCT(DrawDialogBox_Chat), 
		GAMEFUNCT(DlgBoxClick_Chat), GAMEFUNCT(DlgBoxDoubleClick_Chat),GAMEFUNCT(emptyfunc));

	//Sale Menu Dialog
	SetupDialogBox(DIALOG_SHOP, 70, 50, SPRID_INTERFACE_ND_GAME2, 2,  11);
//	m_dialogBoxes[11].SetupDialog(DIALOG_SHOP, 70, 50, 258, 339);
	m_dialogBoxes[11].SetupHandlers(GAMEFUNCT(DrawDialogBox_Shop), GAMEFUNCT(DlgBoxClick_Shop),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Level-Up Setting Dialog
	SetupDialogBox(DIALOG_LEVELUPSETTING, 0, 0, SPRID_INTERFACE_ND_GAME2, 0,  2);
//	m_dialogBoxes[12].SetupDialog(DIALOG_LEVELUPSETTING, 0, 0, 258, 339);
	m_dialogBoxes[12].SetupHandlers(GAMEFUNCT(DrawDialogBox_LevelUpSetting), GAMEFUNCT(DlgBoxClick_LevelUpSettings),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//City Hall Menu Dialog
	SetupDialogBox(DIALOG_CITYHALLMENU, 337, 57, SPRID_INTERFACE_ND_GAME2, 2,  18);
//	m_dialogBoxes[13].SetupDialog(DIALOG_CITYHALLMENU, 337, 57, 258, 339);
	m_dialogBoxes[13].SetupHandlers(GAMEFUNCT(DrawDialogBox_CityHallMenu), GAMEFUNCT(DlgBoxClick_CityhallMenu),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Bank Dialog
	SetupDialogBox(DIALOG_BANK , 60, 50, SPRID_INTERFACE_ND_GAME2, 2, 21);
//	m_dialogBoxes[14].SetupDialog(DIALOG_BANK , 60, 50, 258, 339);
	m_dialogBoxes[14].SetupHandlers(GAMEFUNCT(DrawDialogBox_Bank), GAMEFUNCT(DlgBoxClick_Bank), GAMEFUNCT(emptyfunc),
		GAMEFUNCT(bItemDrop_Bank));
	m_dialogBoxes[14].sV1 = 13;

	//Skill Menu(F8)
	SetupDialogBox(DIALOG_SKILL, 440, 81, SPRID_INTERFACE_ND_GAME2, 0,  1);
//	m_dialogBoxes[15].SetupDialog(DIALOG_SKILL, 337, 57, 258, 339);
	m_dialogBoxes[15].SetupHandlers(GAMEFUNCT(DrawDialogBox_Skill), GAMEFUNCT(DlgBoxClick_Skill),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Magic Shop Menu
	SetupDialogBox(DIALOG_MAGICSHOP, 30, 30, SPRID_INTERFACE_ND_GAME4, 1,  14);
//	m_dialogBoxes[16].SetupDialog(DIALOG_MAGICSHOP, 30, 30, 304, 328);
	m_dialogBoxes[16].SetupHandlers(GAMEFUNCT(DrawDialogBox_MagicShop), GAMEFUNCT(DlgBoxClick_MagicShop),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Dialog items drop external screen
	SetupDialogBox(DIALOG_QUERYDROPITEMAMOUNT, 0, 0, SPRID_INTERFACE_ND_GAME2, 5);
//	m_dialogBoxes[17].SetupDialog(DIALOG_QUERYDROPITEMAMOUNT, 0, 0, 215, 87);
	m_dialogBoxes[17].SetupHandlers(GAMEFUNCT(DrawDialogBox_QueryDropItemAmount),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Text Dialog
	SetupDialogBox(DIALOG_TEXT, 20, 65, SPRID_INTERFACE_ND_GAME2, 0);
	m_dialogBoxes[18].SetupHandlers(GAMEFUNCT(DrawDialogBox_Text), GAMEFUNCT(DlgBoxClick_Text),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//System Menu Dialog(F12)
	SetupDialogBox(DIALOG_SYSMENU, 452, 131, SPRID_INTERFACE_ND_GAME1, 0,  6);
//	m_dialogBoxes[19].SetupDialog(DIALOG_SYSMENU, 337, 107, 258, 268);
	m_dialogBoxes[19].SetupHandlers(GAMEFUNCT(DrawDialogBox_SysMenu), GAMEFUNCT(DlgBoxClick_SysMenu),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//NpcActionQuery Dialog
	SetupDialogBox(DIALOG_NPCACTIONQUERY, 237, 57, SPRID_INTERFACE_ND_GAME2, 5);
//	m_dialogBoxes[20].SetupDialog(DIALOG_NPCACTIONQUERY, 237, 57, 252, 87);
	m_dialogBoxes[20].SetupHandlers(GAMEFUNCT(DrawDialogBox_NpcActionQuery), GAMEFUNCT(DlgBoxClick_NpcActionQuery),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//NpcTalk Dialog
	SetupDialogBox(DIALOG_NPCTALK, 337, 57, SPRID_INTERFACE_ND_GAME2, 2);
//	m_dialogBoxes[21].SetupDialog(DIALOG_NPCTALK, 337, 57, 258, 339);
	m_dialogBoxes[21].SetupHandlers(GAMEFUNCT(DrawDialogBox_NpcTalk), GAMEFUNCT(DlgBoxClick_NpcTalk),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Map
	m_dialogBoxes[22].SetupDialog(DIALOG_MAP, 336+120, 88, 270, 346); // Map Fixed xRisenx?
	m_dialogBoxes[22].SetupHandlers(GAMEFUNCT(DrawDialogBox_Map),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//ItemSellorRepair Dialog
	SetupDialogBox(DIALOG_SELLORREPAIRITEM, 337, 57, SPRID_INTERFACE_ND_GAME2, 2);
//	m_dialogBoxes[23].SetupDialog(DIALOG_SELLORREPAIRITEM, 337, 57, 258, 339);
	m_dialogBoxes[23].SetupHandlers(GAMEFUNCT(DrawDialogBox_SellorRepairItem), GAMEFUNCT(DlgBoxClick_ItemSellorRepair),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Fishing Dialog
	SetupDialogBox(DIALOG_FISHING, 193, 241, SPRID_INTERFACE_ND_GAME1, 2);
//	m_dialogBoxes[24].SetupDialog(DIALOG_FISHING, 193, 241, 263, 100);
	m_dialogBoxes[24].SetupHandlers(GAMEFUNCT(DrawDialogBox_Fishing), GAMEFUNCT(DlgBoxClick_Fish),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//ShutDownMsg Dialog
	SetupDialogBox(DIALOG_SHUTDOWNMSG, 162, 40, SPRID_INTERFACE_ND_GAME4, 2);
//	m_dialogBoxes[25].SetupDialog(DIALOG_SHUTDOWNMSG, 162, 40, 315, 171);
	m_dialogBoxes[25].SetupHandlers(GAMEFUNCT(DrawDialogBox_ShutDownMsg), GAMEFUNCT(DlgBoxClick_ShutDownMsg),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//SkillDlg Dialog
	m_dialogBoxes[26].SetupDialog(DIALOG_SKILLDLG, 100, 60, 258, 339);
	m_dialogBoxes[26].SetupHandlers(GAMEFUNCT(DrawDialogBox_SkillDlg), GAMEFUNCT(DlgBoxClick_SkillDlg), GAMEFUNCT(emptyfunc),
		GAMEFUNCT(bItemDrop_SkillDialog));

	//Exchange Dialog
	SetupDialogBox(DIALOG_EXCHANGE, 100, 30, SPRID_INTERFACE_ND_NEWEXCHANGE, 0);
//	m_dialogBoxes[27].SetupDialog(DIALOG_EXCHANGE, 100, 30, 520, 357);
	m_dialogBoxes[27].SetupHandlers(GAMEFUNCT(DrawDialogBox_Exchange), GAMEFUNCT(DlgBoxClick_Exchange), GAMEFUNCT(emptyfunc),
		GAMEFUNCT(bItemDrop_ExchangeDialog));

	//Quest Dialog
	SetupDialogBox(DIALOG_QUEST, 0, 0, SPRID_INTERFACE_ND_GAME2, 2,  4);
//	m_dialogBoxes[28].SetupDialog(DIALOG_QUEST, 0, 0, 258, 339);
	m_dialogBoxes[28].SetupHandlers(GAMEFUNCT(DrawDialogBox_Quest), GAMEFUNCT(DlgBoxClick_Quest),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Gauge Pannel
	//m_dialogBoxes[29].SetupDialog(DIALOG_GAUGEPANEL, 0, 434, 157, 53);
	m_dialogBoxes[29].SetupDialog(DIALOG_GAUGEPANEL, 0, GetHeight() - 3, 157, 53);
	m_dialogBoxes[29].SetupHandlers(GAMEFUNCT(DrawDialogBox_GaugePanel),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc),GAMEFUNCT(emptyfunc));

	//Icon Pannel
	//m_dialogBoxes[30].SetupDialog(DIALOG_ICONPANEL, 0, 427, 640, 53);
	m_dialogBoxes[30].SetupDialog(DIALOG_ICONPANEL, 0, GetHeight() - 53, GetWidth(), 53); // 800x600 Resolution xRisenx
	m_dialogBoxes[30].SetupHandlers(GAMEFUNCT(DrawDialogBox_IconPanel), GAMEFUNCT(DlgBoxClick_IconPanel), GAMEFUNCT(emptyfunc),
		GAMEFUNCT(bItemDrop_IconPanel));

	//Sell List Dialog
	SetupDialogBox(DIALOG_SELLLIST, 170, 70, SPRID_INTERFACE_ND_GAME2, 2,  11);
//	m_dialogBoxes[31].SetupDialog(DIALOG_SELLLIST, 170, 70, 258, 339);
	m_dialogBoxes[31].SetupHandlers(GAMEFUNCT(DrawDialogBox_SellList), GAMEFUNCT(DlgBoxClick_SellList), GAMEFUNCT(emptyfunc),
		GAMEFUNCT(bItemDrop_SellList));

	//Party Dialog
	SetupDialogBox(DIALOG_PARTY, 0, 0, SPRID_INTERFACE_ND_GAME2, 0,  3);
//	m_dialogBoxes[32].SetupDialog(DIALOG_PARTY, 0, 0, 258, 339);
	m_dialogBoxes[32].SetupHandlers(GAMEFUNCT(DrawDialogBox_Party), GAMEFUNCT(DlgBoxClick_Party), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	//Crusade Job Dialog
	SetupDialogBox(DIALOG_CRUSADEJOB, 360, 65, SPRID_INTERFACE_ND_GAME2, 0);
//	m_dialogBoxes[33].SetupDialog(DIALOG_CRUSADEJOB, 360, 65, 258, 339);
	m_dialogBoxes[33].SetupHandlers(GAMEFUNCT(DrawDialogBox_CrusadeJob), GAMEFUNCT(DlgBoxClick_CrusadeJob), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	//Item Upgrade Dialog
	SetupDialogBox(DIALOG_ITEMUPGRADE, 60, 50, SPRID_INTERFACE_ND_GAME2, 0,  5);
//	m_dialogBoxes[34].SetupDialog(DIALOG_ITEMUPGRADE, 60, 50, 258, 339);
	m_dialogBoxes[34].SetupHandlers(GAMEFUNCT(DrawDialogBox_ItemUpgrade), GAMEFUNCT(DlgBoxClick_ItemUpgrade), GAMEFUNCT(emptyfunc),
		GAMEFUNCT(bItemDrop_ItemUpgrade));

	//Help Menu Dialog(F1)
	SetupDialogBox(DIALOG_HELP, 358, 65, SPRID_INTERFACE_ND_GAME2, 2);
//	m_dialogBoxes[35].SetupDialog(DIALOG_HELP, 358, 65, 258, 339);
	m_dialogBoxes[35].SetupHandlers(GAMEFUNCT(DrawDialogBox_Help), GAMEFUNCT(DlgBoxClick_Help), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	//Crusade Commander Dialog
	//SetupDialogBox(DIALOG_COMMANDER, 20, 20, SPRID_INTERFACE_ND_CRUSADE, 0,  15, true);
	m_dialogBoxes[36].SetupDialog(DIALOG_COMMANDER, 20, 20, 310, 400);
	m_dialogBoxes[36].SetupHandlers(GAMEFUNCT(DrawDialogBox_Commander), GAMEFUNCT(DlgBoxClick_Commander), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	//Crusade Constructor Dialog
	//SetupDialogBox(DIALOG_CONSTRUCTOR, 20, 20, SPRID_INTERFACE_ND_CRUSADE, 0,  16, true);
	m_dialogBoxes[37].SetupDialog(DIALOG_CONSTRUCTOR, 20, 20, 310, 400);
	m_dialogBoxes[37].SetupHandlers(GAMEFUNCT(DrawDialogBox_Constructor), GAMEFUNCT(DlgBoxClick_Constructor), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	//Crusade Soldier Dialog
	//SetupDialogBox(DIALOG_SOLDIER, 20, 20, SPRID_INTERFACE_ND_CRUSADE, 0,  17, true);
	m_dialogBoxes[38].SetupDialog(DIALOG_SOLDIER, 20, 20, 310, 400);
	m_dialogBoxes[38].SetupHandlers(GAMEFUNCT(DrawDialogBox_Soldier), GAMEFUNCT(DlgBoxClick_Soldier), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	// Give item ???
	m_dialogBoxes[39].SetupDialog(39, 0, 0, 291, 413);

	// 3.51 Slates Dialog - Diuuude
	m_dialogBoxes[40].SetupDialog(DIALOG_SLATES, 100, 60, 258, 339);
	m_dialogBoxes[40].SetupHandlers(GAMEFUNCT(DrawDialogBox_Slates), GAMEFUNCT(DlgBoxClick_Slates), GAMEFUNCT(emptyfunc),
		GAMEFUNCT(bItemDrop_Slates));

	// Snoopy: Item exchange confirmation
	SetupDialogBox(DIALOG_CONFIRMEXCHANGE, 285, 200, SPRID_INTERFACE_ND_GAME1, 2);
//	m_dialogBoxes[41].SetupDialog(DIALOG_CONFIRMEXCHANGE, 285, 200, 270, 105);
	m_dialogBoxes[41].SetupHandlers(GAMEFUNCT(DrawDialogBox_ConfirmExchange), GAMEFUNCT(DlgBoxClick_ConfirmExchange), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	// MJ Stats Change DialogBox - Diuuude
	SetupDialogBox(DIALOG_CHANGESTATSMAJESTIC, 0, 0, SPRID_INTERFACE_ND_GAME2, 0,  2);
//	m_dialogBoxes[42].SetupDialog(DIALOG_CHANGESTATSMAJESTIC, 0, 0, 258, 339);
	m_dialogBoxes[42].SetupHandlers(GAMEFUNCT(DrawDialogBox_ChangeStatsMajestic), GAMEFUNCT(DlgBoxClick_ChangeStatsMajestic), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

  // friends
	SetupDialogBox(DIALOG_FRIENDLIST, 0, 0, SPRID_INTERFACE_ND_GAME2, 2);
//	m_dialogBoxes[43].SetupDialog(DIALOG_FRIENDLIST, 0, 0, 258, 339);
	m_dialogBoxes[43].SetupHandlers(GAMEFUNCT(DrawDialogBox_FriendList), GAMEFUNCT(DlgBoxClick_FriendList), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	//guildquery (assign rank)
	SetupDialogBox(DIALOG_GUILDQUERY, 237, 57, SPRID_INTERFACE_ND_GAME2, 6);
//	m_dialogBoxes[44].SetupDialog(DIALOG_GUILDQUERY, 237, 57, 252, 87);
	m_dialogBoxes[44].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildQuery), GAMEFUNCT(DlgBoxClick_GuildQuery), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	// guild teleport request
	SetupDialogBox(DIALOG_GUILDSUMMONS, 185, 20, SPRID_INTERFACE_ND_GAME1, 2);
//	m_dialogBoxes[49].SetupDialog(DIALOG_GUILDSUMMONS, 185, 20, 270, 105);
	m_dialogBoxes[49].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildSummons), GAMEFUNCT(DlgBoxClick_GuildSummons), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	// Resurection
	SetupDialogBox(DIALOG_RESURECT, 185, 20, SPRID_INTERFACE_ND_GAME1, 2);
//	m_dialogBoxes[50].SetupDialog(DIALOG_RESURECT, 185, 20, 270, 105);
	m_dialogBoxes[50].SetupHandlers(GAMEFUNCT(DrawDialogBox_Resurect), GAMEFUNCT(DlgBoxClick_Resurect), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	//Guild Hall Menu Dialog
	SetupDialogBox(DIALOG_CMDHALLMENU, 337, 57, SPRID_INTERFACE_ND_GAME2, 2);
//	m_dialogBoxes[51].SetupDialog(DIALOG_CMDHALLMENU, 337, 57, 258, 339);
	m_dialogBoxes[51].SetupHandlers(GAMEFUNCT(DrawDialogBox_CMDHallMenu), GAMEFUNCT(DlgBoxClick_CMDHallMenu), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	// DK Weapons Dialog
	SetupDialogBox(DIALOG_DKMENUWEAPONS, 358, 65, SPRID_INTERFACE_ND_GAME2, 2);
//	m_dialogBoxes[53].SetupDialog(DIALOG_DKMENUWEAPONS, 358, 65, 258, 339);
	m_dialogBoxes[53].SetupHandlers(GAMEFUNCT(DrawDialogBox_DKMenuWeapons), GAMEFUNCT(DlgBoxClick_DKMenuWeapons), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	//Quest Helper
	m_dialogBoxes[55].SetupDialog(55, 530, 130, 110, 36);

	//GuildBank Dialog
	SetupDialogBox(DIALOG_GUILDBANK, 60, 50, SPRID_INTERFACE_ND_GAME2, 2,  21);
//	m_dialogBoxes[56].SetupDialog(DIALOG_GUILDBANK, 60, 50, 258, 339);
	m_dialogBoxes[56].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildBank), GAMEFUNCT(DlgBoxClick_GuildBank), GAMEFUNCT(emptyfunc),
		GAMEFUNCT(bItemDrop_GuildBank));
	m_dialogBoxes[56].sV1 = 13;

	//Guild Dialog
	SetupDialogBox(DIALOG_GUILD, 30, 30, SPRID_INTERFACE_ND_GAME2, 2,  19);
//	m_dialogBoxes[57].SetupDialog(DIALOG_GUILD, 30, 30, 258, 339);
	m_dialogBoxes[57].SetupHandlers(GAMEFUNCT(DrawDialogBox_Guild), GAMEFUNCT(DlgBoxClick_Guild), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	// Contribute to guild
	SetupDialogBox(DIALOG_GUILDCONTRIBUTE, 0, 0, SPRID_INTERFACE_ND_GAME2, 5);
//	m_dialogBoxes[58].SetupDialog(DIALOG_GUILDCONTRIBUTE, 0, 0, 215, 87);
	m_dialogBoxes[58].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildContribute), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	SetupDialogBox(DIALOG_EXTENDEDSYSMENU, 300, 220, SPRID_INTERFACE_ND_GAME4, 3);
	m_dialogBoxes[59].SetupHandlers(GAMEFUNCT(DrawDialogBox_ExtendedSysMenu), GAMEFUNCT(DlgBoxClick_ExtendedSysMenu), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	//SetupDialogBox(DIALOG_FRIENDLIST, 0, 0, SPRID_INTERFACE_ND_GAME2, 2);
//	m_dialogBoxes[43].SetupDialog(DIALOG_FRIENDLIST, 0, 0, 258, 339);
	//m_dialogBoxes[60].SetupHandlers(GAMEFUNCT(DrawDialogBox_FriendList), GAMEFUNCT(DlgBoxClick_FriendList));

	// Titles xRisenx
	//SetupDialogBox(DIALOG_TITLE, 0, 0, SPRID_INTERFACE_ND_GAME2, 2);
	//m_dialogBoxes[70].SetupHandlers(GAMEFUNCT(DrawDialogBox_Titles), GAMEFUNCT(DlgBoxClick_Titles));
	// Titles xRisenx

	SetupDialogBox(DIALOG_MUTELIST, 0, 0, SPRID_INTERFACE_ND_GAME2, 2);
	m_dialogBoxes[DIALOG_MUTELIST].SetupHandlers(GAMEFUNCT(DrawDialogBox_MuteList), GAMEFUNCT(DlgBoxClick_MuteList), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	//Quest Menu Dialog
	SetupDialogBox(DIALOG_QUESTS, 337, 57, SPRID_INTERFACE_ND_GAME2, 0,  4);
	m_dialogBoxes[DIALOG_QUESTS].SetupHandlers(GAMEFUNCT(DrawDialogBox_QuestList), GAMEFUNCT(DlgBoxClick_QuestList), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));
#ifdef TitleClient
	// Title
	SetupDialogBox(DIALOG_TITLE, 74, 133, SPRID_INTERFACE_ND_GAME2, 2);
	m_dialogBoxes[63].SetupHandlers(GAMEFUNCT(DrawDialogBox_Titles), GAMEFUNCT(DlgBoxClick_Titles), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));
#endif
	SetupDialogBox(DIALOG_MAILBOX, 30, 65, SPRID_INTERFACE_ND_GAME2, 2);
	m_dialogBoxes[DIALOG_MAILBOX].SetupHandlers(GAMEFUNCT(DrawDialogBox_Mailbox), GAMEFUNCT(DlgBoxClick_Mailbox),
		GAMEFUNCT(emptyfunc), GAMEFUNCT(ItemDrop_Mailbox));
	
	SetupDialogBox(DIALOG_YESNO, 184, 188, SPRID_INTERFACE_ND_GAME1, 2);
	m_dialogBoxes[DIALOG_YESNO].
		SetupHandlers(GAMEFUNCT(DrawDialogBox_YesNo), GAMEFUNCT(DlgBoxClick_YesNo), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

	//New Top Bar xRisenx
	//m_dialogBoxes[30].SetupDialog(DIALOG_ICONPANEL, 0, 427, 640, 53);
	m_dialogBoxes[62].SetupDialog(DIALOG_TOPPANEL, 2, 2, 165, 48); // 800x600 Resolution xRisenx
	m_dialogBoxes[62].SetupHandlers(GAMEFUNCT(DrawDialogBox_TopPanel), GAMEFUNCT(DlgBoxClick_TopPanel), GAMEFUNCT(emptyfunc),
		GAMEFUNCT(bItemDrop_TopPanel));

	//Quest Menu Dialog

	for(int i = 0; i < MAXDIALOGBOX; i++)
	{		
		m_dialogBoxes[i].SetMode(0); //Setup default buttons
	}
}

bool CGame::bInit(void * hWnd, void * hInst, char * pCmdLine)
{
	int iIndex;

	int i;
	class CStrTok * pStrTok;
	char seps[] = "&= ,\t\n";
	char * token;
 // CLEROTH - BUG
	for (i = 0; i < MAXSPRITES; i++)
		m_pSprite[i] = 0;
	if (pCmdLine != 0)
	{
		memset(G_cCmdLine, 0, sizeof(G_cCmdLine));
		memset(G_cCmdLineTokenA, 0, sizeof(G_cCmdLineTokenA));
		memset(G_cCmdLineTokenB, 0, sizeof(G_cCmdLineTokenB));
		memset(G_cCmdLineTokenC, 0, sizeof(G_cCmdLineTokenC));
		memset(G_cCmdLineTokenD, 0, sizeof(G_cCmdLineTokenD));
		memset(G_cCmdLineTokenE, 0, sizeof(G_cCmdLineTokenE));

		strcpy(G_cCmdLine, pCmdLine);

		iIndex = 0;
		pStrTok = new class CStrTok(pCmdLine, seps);
		token = pStrTok->pGet();
		while( token != 0 )
		{
			switch (iIndex)
			{
				case 0:	strcpy(G_cCmdLineTokenA, token); break;
				case 1: strcpy(G_cCmdLineTokenB, token); break;
				case 2: strcpy(G_cCmdLineTokenC, token); break;
				case 3: strcpy(G_cCmdLineTokenD, token); break;
				case 4: strcpy(G_cCmdLineTokenE, token); break;
			}
			token = pStrTok->pGet();
			iIndex++;
		}
		delete pStrTok;
	}

// #ifdef UPDATER
// 	if(strcmp(G_cCmdLineTokenA, ENCRYPT_CODE) != 0)
// 	{
// 		MessageBoxA(hWnd, LNG_STARTLAUNCHER, LNG_STARTLAUNCHER, MB_OK);
// 		return false;
// 	}
// #endif

	string str = G_cCmdLineTokenA;
	transform(str.begin(), str.end(), str.begin(), ::tolower);


	if (memcmp(str.c_str(), "/egparam", 8) == 0)
	{
		memset(G_cCmdLineTokenA, 0, sizeof(G_cCmdLineTokenA));
		memcpy(G_cCmdLineTokenA,"dataq",5);
	}
	m_hWnd = hWnd;
	m_bCommandAvailable = true;
	m_dwTime = G_dwGlobalTime;
    klang = createIrrKlangDevice();
    if (!klang)
        m_bSoundFlag = m_bSoundStat = m_bMusicStat = false;
	m_bIsHideLocalCursor = false;
	m_cEnterCheck = m_cTabCheck = m_cLeftArrowCheck = 0;

	memset(m_cLogServerAddr, 0, sizeof(m_cLogServerAddr));
	strcpy(m_cLogServerAddr, SERVER_IP);
	m_iLogServerPort = SERVER_PORT;

	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	if ((SysTime.wMonth == 12) || (SysTime.wMonth == 1)) 
		m_bIsXmas = true;
	else m_bIsXmas = false;

	if (bCheckImportantFile() == false)
	{
//fix windows
		MessageBoxA(*(HWND*)m_hWnd, MSG_NOTIFY_CHECKSUM, "ERROR1", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	if (_bDecodeBuildItemContents() == false)
	{
//fix windows
		MessageBoxA(*(HWND*)m_hWnd, MSG_NOTIFY_CHECKSUM, "ERROR2", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}


	if(bReadItemNameConfigFile() == false)
	{
//fix windows
		MessageBoxA(*(HWND*)m_hWnd, MSG_NOTIFY_ITEMNAME, "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	for(int i = 0; i < SG_MAX; i++)
	{
		g_socketMap[ i ] = SocketGems[i];
	}

	if (bInitMagicCfgList() == false)
	{
//fix windows
		MessageBoxA(*(HWND*)m_hWnd, MSG_NOTIFY_MAGICCFG, "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	// Skill
	if (bInitSkillCfgList() == false)
	{
//fix windows
		MessageBoxA(*(HWND*)m_hWnd, MSG_NOTIFY_SKILLCFG, "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

// 	if (//DIRECTX m_DDraw.bInit(m_hWnd) == FALSE)
// 	{
// 		MessageBoxA(m_hWnd, MSG_NOTIFY_DIRECTX7,"ERROR",MB_ICONEXCLAMATION | MB_OK);
// 		return FALSE;
// 	}

// 	if (//DIRECTX m_dInput.bInit(hWnd, hInst) == FALSE)
// 	{
// 		MessageBoxA(m_hWnd, MSG_NOTIFY_DIRECTX7,"ERROR",MB_ICONEXCLAMATION | MB_OK);
// 		return FALSE;
// 	}


	////m_hPakFile = CreateFileA("sprites\\New-Dialog.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	m_pSprite[SPRID_INTERFACE_ND_LOADING] = CSprite::CreateSprite(L"New-Dialog", 0, false);
	////CloseHandle(m_hPakFile);

	////m_hPakFile = CreateFileA("sprites\\interface2.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	m_pSprite[SPRID_INTERFACE_ADDINTERFACE] = CSprite::CreateSprite(L"interface2", 0, false);
	m_pSprite[SPRID_INTERFACE_CRAFTING] = CSprite::CreateSprite(L"interface2", 3, false);
	////CloseHandle(m_hPakFile);

	// CLEROTH - LOAD FONTS BEFORE MAIN LOADINGl
	////m_hPakFile = CreateFileA("sprites\\interface2.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( m_hPakFile != INVALID_HANDLE_VALUE )
	{
		m_pSprite[SPRID_INTERFACE_SPRFONTS2] = CSprite::CreateSprite(L"interface2", 1, false);
		m_pSprite[SPRID_INTERFACE_F1HELPWINDOWS] = CSprite::CreateSprite(L"interface2", 2, false);
		////CloseHandle(m_hPakFile);
	}

	////m_hPakFile = CreateFileA("sprites\\sprfonts.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( m_hPakFile != INVALID_HANDLE_VALUE )
	{
		m_pSprite[SPRID_INTERFACE_FONT1] = CSprite::CreateSprite(L"sprfonts", 0, false);
		m_pSprite[SPRID_INTERFACE_FONT2] = CSprite::CreateSprite(L"sprfonts", 1, false);
		////CloseHandle(m_hPakFile);
	}

	m_stMCursor.sX = 0;
	m_stMCursor.sY = 0;
	m_pMapData = new class CMapData(this);
	memset(m_cPlayerName, 0, sizeof(m_cPlayerName));
	//memset(m_cAccountName, 0, sizeof(m_cAccountName));
	//memset(m_cAccountPassword, 0, sizeof(m_cAccountPassword));

	m_sPlayerType = 2;
	m_cPlayerTurn = 0;

	m_cDialogBoxOrder[MAXDIALOGBOX - 1] = 29;
	m_cDialogBoxOrder[MAXDIALOGBOX - 2] = 30; 

	m_cMenuDir    = 4;
	m_cMenuDirCnt = 0;
	m_cMenuFrame  = 0;

	m_cSoundVolume = 100;
	m_cMusicVolume = 100;

	m_wWR[1] = 70; m_wWG[1] = 70; m_wWB[1] = 80;
	m_wWR[2] = 70; m_wWG[2] = 70; m_wWB[2] = 80;
	m_wWR[3] = 70; m_wWG[3] = 70; m_wWB[3] = 80;
	m_wWR[4] = 70; m_wWG[4] = 100; m_wWB[4] = 70;
	m_wWR[5] = 130; m_wWG[5] = 90; m_wWB[5] = 10;
	m_wWR[6] = 42; m_wWG[6] = 53; m_wWB[6] = 111;
	m_wWR[7] = 145; m_wWG[7] = 145; m_wWB[7] = 145;
	m_wWR[8] = 120; m_wWG[8] = 100; m_wWB[8] = 120;
	m_wWR[9] = 75; m_wWG[9] = 10; m_wWB[9] = 10;
	m_wWR[10] = 135; m_wWG[10] = 104; m_wWB[10] = 30;


	m_wR[0] = 100;	m_wWG[0] = 100;	m_wWB[0] = 100;
	m_wR[1] = 0;	m_wWG[1] = 0;	m_wWB[1] = 64;
	m_wR[2] = 61;	m_wWG[2] = 41;	m_wWB[2] = 24;
	m_wR[3] = 42;	m_wWG[3] = 85;	m_wWB[3] = 53;
	m_wR[4] = 64;	m_wWG[4] = 0;	m_wWB[4] = 0;
	m_wR[5] = 0;	m_wWG[5] = 64;	m_wWB[5] = 0;
	m_wR[6] = 18;	m_wWG[6] = 0;	m_wWB[6] = 32;
	m_wR[7] = 0;	m_wWG[7] = 64;	m_wWB[7] = 64;
	m_wR[8] = 127;	m_wWG[8] = 10;	m_wWB[8] = 73;
	m_wR[9] = 64;	m_wWG[9] = 0;	m_wWB[9] = 64;
	m_wR[10] = 175;	m_wWG[10] = 175;	m_wWB[10] = 175;
	m_wR[11] = 127;	m_wWG[11] = 82;		m_wWB[11] = 0;
	m_wR[12] = 135;	m_wWG[12] = 64;		m_wWB[12] = 84;
	m_wR[13] = 127;	m_wWG[13] = 107;	m_wWB[13] = 0;
	m_wR[14] = 64;	m_wWG[14] = 0;		m_wWB[14] = 0;
	m_wR[15] = 40;	m_wWG[15] = 40;		m_wWB[15] = 40;

	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 200/2,  200/2,  200/2),  &m_wR[0], &m_wG[0], &m_wB[0]); // Normal Dont Change
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x00/4, 0x00/4, 0xff/4),  &m_wR[1], &m_wG[1], &m_wB[1]); // Blue
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xf4/4, 0xa4/4, 0x60/4),  &m_wR[2], &m_wG[2], &m_wB[2]); // Sandy Brown 
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x7f/3, 0xff/3, 0xd4/3),  &m_wR[3], &m_wG[3], &m_wB[3]); // AquaMarine
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/4, 0x00/4, 0x00/4),  &m_wR[4], &m_wG[4], &m_wB[4]); // Red
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x00/4, 0xff/4, 0x00/4),  &m_wR[5], &m_wG[5], &m_wB[5]); // Green
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x4b/4, 0x00/4, 0x82/4),  &m_wR[6], &m_wG[6], &m_wB[6]); // Indigo
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x00/4, 0xff/4, 0xff/4),  &m_wR[7], &m_wG[7], &m_wB[7]); // Aqua
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/2, 0x14/2, 0x93/2),  &m_wR[8], &m_wG[8], &m_wB[8]); // Pink
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/4, 0x00/4, 0xff/4),  &m_wR[9], &m_wG[9], &m_wB[9]); // Violet
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 175, 175, 175),  &m_wR[10], &m_wG[10], &m_wB[10]); // White
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/2, 0xa5/2, 0x00/2),  &m_wR[11], &m_wG[11], &m_wB[11]); // Orange
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x0f/3, 0xc0/3, 0xfc/3),  &m_wR[12], &m_wG[12], &m_wB[12]); // Disco Blue
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/2, 0xd7/2, 0x00/2),  &m_wR[13], &m_wG[13], &m_wB[13]); // Yellow Damage Monster Txt
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/4, 0x00/4, 0x00/4),  &m_wR[14], &m_wG[14], &m_wB[14]); // Red Casting Txt
	// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 40, 40, 40),  &m_wR[15], &m_wG[15], &m_wB[15]); // Black








	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,  70,  70,  80), &m_wWR[1], &m_wWG[1], &m_wWB[1]); // Light-blue
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,  70,  70,  80), &m_wWR[2], &m_wWG[2], &m_wWB[2]); // light-blue
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,  70,  70,  80), &m_wWR[3], &m_wWG[3], &m_wWB[3]); // light-blue
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,  70, 100,  70), &m_wWR[4], &m_wWG[4], &m_wWB[4]); // Green
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 130,  90,  10), &m_wWR[5], &m_wWG[5], &m_wWB[5]); // Critical
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,  42,  53, 111), &m_wWR[6], &m_wWG[6], &m_wWB[6]); // Heavy-blue
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 145, 145, 145), &m_wWR[7], &m_wWG[7], &m_wWB[7]); // White
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 120, 100, 120), &m_wWR[8], &m_wWG[8], &m_wWB[8]);
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,  75,  10,  10), &m_wWR[9], &m_wWG[9], &m_wWB[9]); // Heavy-Red
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 135, 104,  30), &m_wWR[10],&m_wWG[10],&m_wWB[10]);	// Gold

	// Weapon's
	// 
	// 
	//TODO: need to remap all colors into irrlicht capable shit
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x7f/3, 0xff/3, 0xd4/3), &m_wWR[1], &m_wWG[1], &m_wWB[1]); // Agile
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x0f/3, 0xc0/3, 0xfc/3), &m_wWR[2], &m_wWG[2], &m_wWB[2]); // Light
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x00/4, 0xff/4, 0xff/4), &m_wWR[3], &m_wWG[3], &m_wWB[3]); // Strong
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x00/4, 0xff/4, 0x00/4), &m_wWR[4], &m_wWG[4], &m_wWB[4]); // Poison
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/2, 0xa5/2, 0x00/2), &m_wWR[5], &m_wWG[5], &m_wWB[5]); // Critical
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x00/4, 0x00/4, 0xff/4), &m_wWR[6], &m_wWG[6], &m_wWB[6]); // Sharp
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 145, 145, 145), &m_wWR[7], &m_wWG[7], &m_wWB[7]); // Rightous
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 120, 100, 120), &m_wWR[8], &m_wWG[8], &m_wWB[8]); // Ancient
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,  75,  10,  10), &m_wWR[9], &m_wWG[9], &m_wWB[9]); // Heavy-Red
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 175, 175, 175), &m_wWR[10], &m_wWG[10], &m_wWB[10]); // White
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/2, 0xd7/2, 0x00/2), &m_wWR[11], &m_wWG[11], &m_wWB[11]); // Gold
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 40, 40, 40), &m_wWR[12], &m_wWG[12], &m_wWB[12]); // Black
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/4, 0x00/4, 0xff/4), &m_wWR[13], &m_wWG[13], &m_wWB[13]); // Violet
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xf4/4, 0xa4/4, 0x60/4), &m_wWR[14], &m_wWG[14], &m_wWB[14]); // Sandy Brown
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x4b/4, 0x00/4, 0x82/4), &m_wWR[15], &m_wWG[15], &m_wWB[15]); // Indigo
// #ifdef MoreColors
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x00, 0xff, 0x00), &m_wWR[16], &m_wWG[16], &m_wWB[16]); // Test 1 Green
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff, 0xff, 0x00), &m_wWR[17], &m_wWG[17], &m_wWB[17]); // Test 2 Yellow
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff, 0xff, 0xff), &m_wWR[18], &m_wWG[18], &m_wWB[18]); // Test 3 White
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff, 0x00, 0x00), &m_wWR[19], &m_wWG[19], &m_wWB[19]); // Test 4 Red
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff, 0x00, 0xff), &m_wWR[20], &m_wWG[20], &m_wWB[20]); // Test 5 PINK
// #endif
// 	// Armor's
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 200/2,  200/2,  200/2),  &m_wR[0], &m_wG[0], &m_wB[0]); // Normal Dont Change
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x00/4, 0x00/4, 0xff/4),  &m_wR[1], &m_wG[1], &m_wB[1]); // Blue
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xf4/4, 0xa4/4, 0x60/4),  &m_wR[2], &m_wG[2], &m_wB[2]); // Sandy Brown 
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x7f/3, 0xff/3, 0xd4/3),  &m_wR[3], &m_wG[3], &m_wB[3]); // AquaMarine
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/4, 0x00/4, 0x00/4),  &m_wR[4], &m_wG[4], &m_wB[4]); // Red
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x00/4, 0xff/4, 0x00/4),  &m_wR[5], &m_wG[5], &m_wB[5]); // Green
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x4b/4, 0x00/4, 0x82/4),  &m_wR[6], &m_wG[6], &m_wB[6]); // Indigo
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x00/4, 0xff/4, 0xff/4),  &m_wR[7], &m_wG[7], &m_wB[7]); // Aqua
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/2, 0x14/2, 0x93/2),  &m_wR[8], &m_wG[8], &m_wB[8]); // Pink
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/4, 0x00/4, 0xff/4),  &m_wR[9], &m_wG[9], &m_wB[9]); // Violet
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 175, 175, 175),  &m_wR[10], &m_wG[10], &m_wB[10]); // White
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/2, 0xa5/2, 0x00/2),  &m_wR[11], &m_wG[11], &m_wB[11]); // Orange
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0x0f/3, 0xc0/3, 0xfc/3),  &m_wR[12], &m_wG[12], &m_wB[12]); // Disco Blue
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/2, 0xd7/2, 0x00/2),  &m_wR[13], &m_wG[13], &m_wB[13]); // Yellow Damage Monster Txt
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 0xff/4, 0x00/4, 0x00/4),  &m_wR[14], &m_wG[14], &m_wB[14]); // Red Casting Txt
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 40, 40, 40),  &m_wR[15], &m_wG[15], &m_wB[15]); // Black

	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 200/2,  200/2,  200/2),  &m_wR[0], &m_wG[0], &m_wB[0]);
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,0x50/2, 0x50/2, 0xC0/2),  &m_wR[1], &m_wG[1], &m_wB[1]); // Indigo Blue
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,    87,   87,     62),  &m_wR[2], &m_wG[2], &m_wB[2]); // Custom-Weapon Color
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,   135,    104,     30),  &m_wR[3], &m_wG[3], &m_wB[3]); // Gold
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255, 255/2,   36/2,      0),  &m_wR[4], &m_wG[4], &m_wB[4]); // Crimson
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,    10,     60,     10),  &m_wR[5], &m_wG[5], &m_wB[5]); // Green
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,0x50/2, 0x50/2, 0x50/2),  &m_wR[6], &m_wG[6], &m_wB[6]); // Gray
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,0x5F/2, 0x9E/2, 0xA0/2),  &m_wR[7], &m_wG[7], &m_wB[7]); // Aqua
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,0xFF/2, 0x69/2, 0xB4/2),  &m_wR[8], &m_wG[8], &m_wB[8]); // Pink
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,    90,     60,     90),  &m_wR[9], &m_wG[9], &m_wB[9]);
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,     60,     60,     80),  &m_wR[10], &m_wG[10], &m_wB[10]); // Blue
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,0xD2/2, 0xB4/2, 0x8C/2),  &m_wR[11], &m_wG[11], &m_wB[11]); // Tan
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,0xBD/2, 0xB7/2, 0x6B/2),  &m_wR[12], &m_wG[12], &m_wB[12]); // Khaki
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,    85,     85,      8),  &m_wR[13], &m_wG[13], &m_wB[13]); // Yellow
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,    75,     10,     10),  &m_wR[14], &m_wG[14], &m_wB[14]); // Red
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,  12,   20,   30),  &m_wR[15], &m_wG[15], &m_wB[15]); // Black

	memset(m_cWorldServerName, 0, sizeof(m_cWorldServerName));
	socketthread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));

	return true;
}

void CGame::Quit()
{
	int i;
	WriteSettings();
	ChangeGameMode(GAMEMODE_NULL);

	for (i = 0; i < MAXSPRITES; i++)
	if (m_pSprite[i] != 0) delete m_pSprite[i];
	for (i = 0; i < MAXTILES; i++)
	if (m_pTileSpr[i] != 0) delete m_pTileSpr[i];
	for (i = 0; i < MAXEFFECTSPR; i++)
	if (m_pEffectSpr[i] != 0) delete m_pEffectSpr[i];

	for (i = 0; i < MAXSOUNDEFFECTS; i++)
    {
		if (m_pCSound[i] != nullptr) m_pCSound[i]->drop();
		if (m_pMSound[i] != nullptr) m_pMSound[i]->drop();
		if (m_pESound[i] != nullptr) m_pESound[i]->drop();
	}

    if (m_pBGM != nullptr) m_pBGM->drop();

	for (i = 0; i < MAXITEMS; i++)
	if (m_pItemList[i] != 0)	delete m_pItemList[i];

	for (i = 0; i < MAXBANKITEMS; i++)
	if (m_pBankList[i] != 0)	delete m_pBankList[i];

	for (i = 0; i < MAXEFFECTS; i++)
	if (m_pEffectList[i] != 0) delete m_pEffectList[i];

	for (i = 0; i < MAXCHATMSGS; i++)
		if (m_pChatMsgList[i] != 0) delete m_pChatMsgList[i];

	for(int i = 0; i < CHAT_MAX; i++)
	{
		for (std::vector<CMsg*>::iterator it = m_chatMsgs[i].begin();
			it != m_chatMsgs[i].end();
			++it)
		{
			delete (*it);
		}
		m_chatMsgs[i].clear();
	}
	if(m_chatDisplay)
		delete m_chatDisplay;

	for (i = 0; i < MAXWHISPERMSG; i++)
		if (m_pWhisperMsg[i] != 0) delete m_pWhisperMsg[i];

	for (i = 0; i < MAXMENUITEMS; i++)
		if (m_pItemForSaleList[i] != 0) delete m_pItemForSaleList[i];

	for (i = 0; i < MAXMAGICTYPE; i++)
		if (m_pMagicCfgList[i] != 0) delete m_pMagicCfgList[i];

	for (i = 0; i < MAXSKILLTYPE; i++)
		if (m_pSkillCfgList[i] != 0) delete m_pSkillCfgList[i];

	for (i = 0; i < TEXTDLGMAXLINES; i++) {
		if (m_pMsgTextList[i] != 0)  delete m_pMsgTextList[i];
		if (m_pMsgTextList2[i] != 0) delete m_pMsgTextList2[i];
		if (m_pAgreeMsgTextList[i] != 0) delete m_pAgreeMsgTextList[i];
	}

	for (i = 0; i < MAXBUILDITEMS; i++)
		if (m_pBuildItemList[i] != 0) delete m_pBuildItemList[i];

	for (i = 0; i < MAXBUILDITEMS; i++)
		if (m_pDispBuildItemList[i] != 0) delete m_pDispBuildItemList[i];

	for (i=0; i<MAXITEMSTATS; i++)
		if(m_pBuildItemListStats[i] != 0) delete m_pBuildItemListStats[i];

	// Crafting:
	for (i = 0; i < MAXBUILDITEMS; i++)
		if (m_pCraftItemList[i] != 0) delete m_pCraftItemList[i];
	for (i = 0; i < MAXBUILDITEMS; i++)
		if (m_pDispCraftItemList[i] != 0) delete m_pDispCraftItemList[i];


	for (i = 0; i < MAXITEMNAMES; i++)
		if (m_pItemNameList[i] != 0) delete m_pItemNameList[i];

	delete m_pMapData;

	//if (m_pGSock != NULL) delete m_pGSock;
	if (_socket != nullptr)
		_socket = nullptr;
	if (new_connection_ != nullptr)
		new_connection_ = nullptr;
	io_service_.stop();
}

void CGame::UpdateScreen()
{
	G_pGame->driver->beginScene(true, true);
	G_dwGlobalTime = unixtime();

	switch (m_cGameMode) {
#ifdef MAKE_ACCOUNT
	case GAMEMODE_ONAGREEMENT:
		// UpdateScreen_OnAgreement(); //unused by HBx server..
		break;

	case GAMEMODE_ONCREATENEWACCOUNT:
		UpdateScreen_OnCreateNewAccount();
		break;
#endif

	case GAMEMODE_ONVERSIONNOTMATCH:
		UpdateScreen_OnVersionNotMatch();
		break;

	case GAMEMODE_ONCONNECTING:
		UpdateScreen_OnConnecting();
		break;

	case GAMEMODE_ONMAINMENU:
		//UpdateScreen_OnMainMenu();
		UpdateScreen_OnLogin();	// ShadowEvil - Added a skip main menu
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

	case GAMEMODE_ONCONNECTIONLOST:
		UpdateScreen_OnConnectionLost();
		break;

	case GAMEMODE_ONMSG:
		UpdateScreen_OnMsg();
		break;

	case GAMEMODE_ONLOGIN:
		UpdateScreen_OnLogin();
		break;

	case GAMEMODE_ONSELECTSERVER:
		UpdateScreen_OnSelectServer();
		break;

	case GAMEMODE_ONQUIT:
		UpdateScreen_OnQuit();
		break;

	case GAMEMODE_ONQUERYFORCELOGIN:
		UpdateScreen_OnQueryForceLogin();
		break;

	case GAMEMODE_ONSELECTCHARACTER:
		b_cWarrior = false;
		b_cMage = false;
		b_cBattleMage = false;
		b_cArcher = false;
		UpdateScreen_OnSelectCharacter();
		break;

	case GAMEMODE_ONCREATENEWCHARACTER:
		UpdateScreen_OnCreateNewCharacter();
		break;

	case GAMEMODE_ONWAITINGRESPONSE:
		UpdateScreen_OnWaitingResponse();
		break;

	case GAMEMODE_ONQUERYDELETECHARACTER:
		UpdateScreen_OnQueryDeleteCharacter();
		break;

	case GAMEMODE_ONLOGRESMSG:
		UpdateScreen_OnLogResMsg();
		break;

	case GAMEMODE_ONCHANGEPASSWORD:
		UpdateScreen_OnChangePassword();
		break;
	}

#ifdef USING_WIN_IME

	if (GetAsyncKeyState(VK_RETURN) != 0) m_cEnterCheck = 1;
	if ((m_cEnterCheck == 1) && (GetAsyncKeyState(VK_RETURN) == 0))
	{
		m_bEnterPressed = true;
		m_cEnterCheck = 0;
	}
	if (GetAsyncKeyState(VK_TAB) != 0) m_cTabCheck = 1;
	if ((m_cTabCheck == 1) && (GetAsyncKeyState(VK_TAB) == 0))
	{
		m_cCurFocus++;
		if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
		if (m_cGameMode == GAMEMODE_ONMAINGAME) bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);
		m_cTabCheck = 0;
	}
	if (m_bInputStatus)
	{
		if (GetAsyncKeyState(VK_LEFT) != 0) m_cLeftArrowCheck = 1;
		if ((m_cLeftArrowCheck == 1) && (GetAsyncKeyState(VK_LEFT) == 0))
		{
			m_cLeftArrowCheck = 0;
			if (G_hEditWnd != 0)
			{
				int iStrLen = strlen(m_pInputBuffer);
				SendMessage(G_hEditWnd, EM_SETSEL, iStrLen, iStrLen);
			}
		}
	}
#endif
    static uint64_t fpstime = unixtime();
	static uint64_t uitime = unixtime();
	if (htmlUI->surface && G_dwGlobalTime - uitime > 50) {
        if (G_dwGlobalTime - fpstime > 1000)
        {
            char cfps[20];
            sprintf(cfps, "%d", driver->getFPS());
            htmlUI->jsData.SetProperty(WSLit("fps"), WSLit(cfps));
            fpstime = G_dwGlobalTime;
            htmlUI->surface->set_is_dirty(true);
        }

		htmlUI->view->Focus();
        WebCore::instance()->Update();
        uitime = G_dwGlobalTime;
	}

	// Render HTML ui
	if (htmlUI->isDirty())
	{
		uitime = G_dwGlobalTime;
		int width = htmlUI->surface->width();
		int height = htmlUI->surface->height();
		if (htmlRTT) {
			driver->removeTexture(htmlRTT);
		}
		// ui->surface->SaveToPNG(WSLit("./ui-debug.png"), true);
		IImage *img = driver->createImageFromData(ECF_A8R8G8B8, irr::core::dimension2d<u32>(GetWidth(), GetHeight()), (unsigned char*)htmlUI->surface->buffer(), false, false);
		htmlRTT = driver->addTexture("ui-html.png", img);
		img->drop();
		htmlUI->surface->set_is_dirty(false);
	}

	driver->draw2DImage(htmlRTT, core::vector2d<s32>(0, 0), core::rect<s32>(0, 0, GetWidth(), GetHeight()), 0, video::SColor(255, 255, 255, 255), true);

    // Things rendered over the UI are here

    if (m_cGameMode == GAMEMODE_ONSELECTCHARACTER)
    {
        auto size = charselect->getSize();
        driver->draw2DImage(charselect, core::position2d<s32>(charselectx, charselecty),
                            core::rect<s32>(0, 0, size.Width, size.Height), 0,
                            video::SColor(255, 255, 255, 255), true);
    }


	/*
	char cfps[20];
	sprintf(cfps, "FPS: %d", driver->getFPS());

	font[0]->draw(cfps,
		core::rect<s32>(5,5,40,10),
		video::SColor(255,255,255,255));

    font[0]->draw("Arrow keys = Map Pivot Change - Shift = ViewDest Change - Control = Player Offset Change",
                  core::rect<s32>(5, 15, 40, 10),
                  video::SColor(255, 255, 255, 255));


    char cdata[100];
    sprintf(cdata, "m_cCommandCount: %d", (int)m_cCommandCount);

    font[0]->draw(cdata,
                  core::rect<s32>(5, 70, 40, 10),
                  video::SColor(255, 255, 255, 255));



	stringstream ts;
	
	ts << "Pivot: " << m_pMapData->m_sPivotX << " : " << m_pMapData->m_sPivotY << "\n"
		<< "ViewPoint: " << m_sViewPointX << " : " << m_sViewPointY << "\n"
		<< "ViewDst: " << m_sViewDstX << " : " << m_sViewDstY;

	font[0]->draw(ts.str().c_str(),
		core::rect<s32>(5, 25, 40, 10),
		video::SColor(255, 255, 255, 255));


    short sPivotX, sPivotY, sVal, sDivX, sModX, sDivY, sModY;
    sPivotX = m_pMapData->m_sPivotX;
    sPivotY = m_pMapData->m_sPivotY;
    sVal = m_sViewPointX - (sPivotX * 32);
    sDivX = sVal / 32;
    sModX = sVal % 32;
    sVal = m_sViewPointY - (sPivotY * 32);
    sDivY = sVal / 32;
    sModY = sVal % 32;

    ts.str(string());

    ts  << "sPivotX:sPivotY " << m_pMapData->m_sPivotX << " : " << m_pMapData->m_sPivotY << "\n"
        << "sDivX:sModX     " << sDivX << " : " << sModX << "\n"
        << "sDivY:sModY     " << sDivY << " : " << sModY;


    font[0]->draw(ts.str().c_str(),
                  core::rect<s32>(5, 80, 80, 10),
                  video::SColor(255, 255, 255, 255));


    char cdata2[500];
    sprintf(cdata2, 
            "DrawBackground(sDivX, sModX, sDivY, sModY)\n"
            "DrawBackground(%d, %d, %d, %d)\n"
            "DrawObjects(sPivotX, sPivotY, sDivX, sDivY, sModX, sModY, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY)\n"
            "DrawObjects(%d, %d, %d, %d, %d, %d, %d, %d)\n\n"
            "m_sViewDstX = (m_sPlayerX - 24) * 32 | m_sViewDstY = (m_sPlayerY - 16) * 32\n"
            "%d = (%d - %d) * 32 | %d = (%d - %d) * 32",
        (int)sDivX, (int)sModX, (int)sDivY, (int)sModY, (int)sPivotX, (int)sPivotY, (int)sDivX, (int)sDivY, (int)sModX, (int)sModY, (int)m_stMCursor.sX, (int)m_stMCursor.sY,
            (m_sPlayerX - viewdstxcharvar) * 32, m_sPlayerX, viewdstxcharvar, (m_sPlayerY - viewdstycharvar) * 32, m_sPlayerY, viewdstycharvar);
    


    font[0]->draw(cdata2,
                  core::rect<s32>(5, 120, 80, 10),
                  video::SColor(255, 255, 255, 255));

                  */

//     static uint64_t vptime = unixtime();
//     if (G_dwGlobalTime - vptime > 100)
//     {
//         m_bIsRedrawPDBGS = true;
//         CalcViewPoint();
//         vptime = G_dwGlobalTime;
//     }

// 	sprintf(cfps, "Mouse: (%d,%d)", m_stMCursor.sX, m_stMCursor.sY);
// 
// 	font[0]->draw(cfps,
// 		core::rect<s32>(5,15,40,20),
// 		video::SColor(255,255,255,255));

//	char text[200];

// 	sprintf(text, "fgtargetfps: %d\nbgtargetfps: %d\ntime1: %I64d\ntime2: %I64d\nfgframetime: %d\nbgframetime: %d", foregroundfpstarget, backgroundfpstarget, time1, time2, foregroundframetime, backgroundframetime);
// 	font[0]->draw(text,
// 		core::rect<s32>(5, 35, 40, 60),
// 		video::SColor(255, 255, 255, 255));

// 	if (device->isWindowActive())
// 	{
// 		font[0]->draw("Active",
// 			core::rect<s32>(5,25,40,30),
// 			video::SColor(255,255,255,255));
// 	}

	env->drawAll();

	//draw gui

	{
		lock_guard<std::mutex> lock(uimtx);
		// CEGUI::System::getSingleton().renderAllGUIContexts();
	}

	//m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(m_stMCursor.sX, m_stMCursor.sY, m_stMCursor.sCursorFrame, unixseconds());

    if (m_bIsObserverMode == true)
    {
        driver->draw2DLine(irr::core::vector2d<s32>(m_stMCursor.sX - 5, G_pGame->m_stMCursor.sY), irr::core::vector2d<s32>(G_pGame->m_stMCursor.sX + 5, G_pGame->m_stMCursor.sY), irr::video::SColor(255, 255, 0, 0));
        driver->draw2DLine(irr::core::vector2d<s32>(m_stMCursor.sX, G_pGame->m_stMCursor.sY - 5), irr::core::vector2d<s32>(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY + 5), irr::video::SColor(255, 255, 0, 0));

        driver->draw2DLine(irr::core::vector2d<s32>(m_stMCursor.sX - 5, G_pGame->m_stMCursor.sY - 1), irr::core::vector2d<s32>(G_pGame->m_stMCursor.sX + 5, G_pGame->m_stMCursor.sY - 1), irr::video::SColor(127, 255, 255, 0));
        driver->draw2DLine(irr::core::vector2d<s32>(m_stMCursor.sX - 1, G_pGame->m_stMCursor.sY - 5), irr::core::vector2d<s32>(G_pGame->m_stMCursor.sX - 1, G_pGame->m_stMCursor.sY + 5), irr::video::SColor(127, 255, 255, 0));

        driver->draw2DLine(irr::core::vector2d<s32>(m_stMCursor.sX - 5, G_pGame->m_stMCursor.sY + 1), irr::core::vector2d<s32>(G_pGame->m_stMCursor.sX + 5, G_pGame->m_stMCursor.sY + 1), irr::video::SColor(127, 255, 255, 0));
        driver->draw2DLine(irr::core::vector2d<s32>(m_stMCursor.sX + 1, G_pGame->m_stMCursor.sY - 5), irr::core::vector2d<s32>(G_pGame->m_stMCursor.sX + 1, G_pGame->m_stMCursor.sY + 5), irr::video::SColor(127, 255, 255, 0));
    }
    //else m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY, m_stMCursor.sCursorFrame, dwTime);
    else m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(m_stMCursor.sX, m_stMCursor.sY, m_stMCursor.sCursorFrame, unixseconds());

	m_stMCursor.sZ = 0;
	driver->endScene();
}

void CGame::CalcViewPoint()
{
	short dX, dY;
	dX = m_sViewPointX - m_sViewDstX;
	dY = m_sViewPointY - m_sViewDstY;
	if (abs(dX) < abs(m_sViewDX))
	{
		m_sViewPointX = m_sViewDstX;
		m_sViewDX = 0;
	}
	else
	{
		if (dX > 0) m_sViewDX--;
		if (dX < 0) m_sViewDX++;
		if (dX == 0) m_sViewDX = 0;
		if (abs(dX) < 40) {
			if (m_sViewDX > 4)  m_sViewDX = 4;
			else if (m_sViewDX < -4) m_sViewDX = -4;
		}
		m_sViewPointX += m_sViewDX;
	}

	if (abs(dY) < abs(m_sViewDY))
	{
		m_sViewPointY = m_sViewDstY;
		m_sViewDY = 0;
	}
	else
	{
		if (dY > 0) m_sViewDY--;
		if (dY < 0) m_sViewDY++;
		if (dY == 0) m_sViewDY = 0;
		if (abs(dY) < 40) {
			if (m_sViewDY > 4)  m_sViewDY = 4;
			else if (m_sViewDY < -4) m_sViewDY = -4;
		}
		m_sViewPointY += m_sViewDY;
	}
}

void CGame::handle_connect(const boost::system::error_code& e)
{
	if (!e)
	{
		_socket = new_connection_;

		start(new_connection_);
		new_connection_.reset(new connection(io_service_, *this, request_handler_, ctx));
		ConnectionEstablishHandler(SERVERTYPE_LOG);
	}
	else
	{
		new_connection_->stop();
	    new_connection_.reset(new connection(io_service_, *this, request_handler_, ctx));
        _socket = nullptr;
/*
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
		new_connection_->socket().async_connect(endpoint,
			boost::bind(&CGame::handle_connect, this,
			boost::asio::placeholders::error));*/
	}
}

void CGame::RestoreSprites()
{
	//for (int i = 0; i < MAXSPRITES; i++)
	//if (m_pSprite[i] != 0) m_pSprite[i]->iRestore();
}

char _tmp_cTmpDirX[9] = { 0,0,1,1,1,0,-1,-1,-1 };
char _tmp_cTmpDirY[9] = { 0,-1,-1,0,1,1,1,0,-1 };
char CGame::cGetNextMoveDir(short sX, short sY, short dstX, short dstY, bool bMoveCheck, bool isMIM)
{
 char  cDir, cTmpDir;
 //int   aX, aY, aX2, aY2, dX, dY;
 int   aX, aY, dX, dY;
 int   i;
	if ((sX == dstX) && (sY == dstY)) return 0;
	dX = sX;
	dY = sY;

	if (isMIM == false)
		 cDir = m_Misc.cGetNextMoveDir(dX, dY, dstX, dstY);
	else cDir = m_Misc.cGetNextMoveDir(dstX, dstY, dX, dY);

	if (m_cPlayerTurn == 0)
	for (i = cDir; i <= cDir + 2;i++)
	{	cTmpDir = i;
		if (cTmpDir > 8) cTmpDir -= 8;
		aX = _tmp_cTmpDirX[cTmpDir];
		aY = _tmp_cTmpDirY[cTmpDir];
		if (((dX + aX) == m_iPrevMoveX) && ((dY + aY) == m_iPrevMoveY) && (m_bIsPrevMoveBlocked == true) && (bMoveCheck == true))
		{	m_bIsPrevMoveBlocked = false;
		}else if (m_pMapData->bGetIsLocateable(dX + aX, dY + aY) == true)
		{	if( m_pMapData->bIsTeleportLoc(dX + aX, dY + aY) == true )
			{	if( _bCheckMoveable(dX + aX, dY + aY) == true ) return cTmpDir;
				else
				{	SetTopMsg( MSG_GETNEXTMOVEDIR, 5 );
				}
			}else return cTmpDir;
	}	}

	if (m_cPlayerTurn == 1)
	for (i = cDir; i >= cDir - 2;i--)
	{	cTmpDir = i;
		if (cTmpDir < 1) cTmpDir += 8;
		aX = _tmp_cTmpDirX[cTmpDir];
		aY = _tmp_cTmpDirY[cTmpDir];
		if (((dX + aX) == m_iPrevMoveX) && ((dY + aY) == m_iPrevMoveY) && (m_bIsPrevMoveBlocked == true) && (bMoveCheck == true))
		{	m_bIsPrevMoveBlocked = false;
		}else if (m_pMapData->bGetIsLocateable(dX + aX, dY + aY) == true)
		{	if( m_pMapData->bIsTeleportLoc(dX + aX, dY + aY) == true )
			{	if( _bCheckMoveable(dX + aX, dY + aY) == true ) return cTmpDir;
				else
				{	SetTopMsg( MSG_GETNEXTMOVEDIR, 5 );
				}
			}else return cTmpDir;
	}	}
	return 0;
}

bool CGame::_bCheckMoveable( short sx, short sy )
{	// Snoopy: This function prevents the client from asking at TP from some maps to
	// ML or PL if not citizen
	// Incomplete function and useless (the server will handle such cases)
/*	if( m_bIsCrusadeMode ) return TRUE;
	if( m_bHunter == FALSE ) return TRUE;
	if( m_side == NEUTRAL ) return TRUE;
	if( m_side == ARESDEN && m_cMapIndex == 11 )
	{	if( sy == 20 )
		{	if( (sx>=39) && (sx<=47) ) return FALSE;
			if( (sx>=360) && (sx<=368) ) return FALSE;
		}
	}
	if( m_side == ELVINE && m_cMapIndex == 3 )
	{
		if( sy == 373 )
		{	if( (sx>=27) && (sx<=32) ) return FALSE;
		}
		if( sy == 374 )
		{	if( (sx>=298) && (sx<=305) ) return FALSE;
		}
	}*/
	return true;
}

bool CGame::SendLoginCommand(uint32_t dwMsgID)
{
	char	*cp, cMsg[300], macAddress[24], cKey;

	StreamWrite sw;

	if (_socket == nullptr) return false;
	memset(cMsg, 0, sizeof(cMsg));
	cKey = (char)(rand() % 255) +1;

	cp = cMsg;

	//TODO: future servers opened under same bin will need a different name
	strcpy(m_cWorldServerName, "Xtreme");

	sw.WriteInt(dwMsgID);
	sw.WriteString("", 12);
	sw.WriteString(m_cAccountName, 10);
	sw.WriteString(m_cAccountPassword, 10);
	//XSocket::GetMACaddress(macAddress);

	switch(dwMsgID)
	{
	case MSGID_GETMINIMUMLOADGATEWAY:
	case MSGID_REQUEST_LOGIN:
		sw.WriteString(m_cWorldServerName, 30);
		break;

	case MSGID_REQUEST_ENTERGAME:
		sw.WriteString(m_cPlayerName, 10);
		sw.WriteShort(m_wEnterGameType);
		sw.WriteString(m_cWorldServerName, 30);
		sw.WriteString(G_cCmdLineTokenA, 120);
		break;

	case MSGID_REQUEST_CHANGEPASSWORD:
		sw.WriteString(m_cNewPassword, 10);
		sw.WriteString(m_cNewPassConfirm, 10);
		break;

	case MSGID_REQUEST_CREATENEWACCOUNT:
		sw.WriteString(m_cEmailAddr, 50);
		sw.WriteString(m_cAccountAge, 10);
		sw.WriteString(m_cAccountCountry, 17);
		sw.WriteString(m_cAccountSSN, 28);
		sw.WriteString(m_cAccountQuiz, 45);
		sw.WriteString(m_cAccountAnswer, 20);
		sw.WriteString(G_cCmdLineTokenA_Lowercase, 50);
		break;

	case MSGID_REQUEST_CREATENEWCHARACTER:
		sw.WriteString(m_cPlayerName, 10);
		sw.WriteString(m_cWorldServerName, 30);
		sw.WriteByte(m_cGender);
		sw.WriteByte(m_cSkinCol);
		sw.WriteByte(m_cHairStyle);
		sw.WriteByte(m_cHairCol);
		sw.WriteByte(m_cUnderCol);

		sw.WriteByte(m_createStat[STAT_STR]);
		sw.WriteByte(m_createStat[STAT_VIT]);
		sw.WriteByte(m_createStat[STAT_DEX]);
		sw.WriteByte(m_createStat[STAT_INT]);
		sw.WriteByte(m_createStat[STAT_MAG]);
		sw.WriteByte(m_createStat[STAT_CHR]);
		break;

	case MSGID_REQUEST_DELETECHARACTER:
		sw.WriteString(m_pCharList[m_wEnterGameType - 1]->m_cName, 10);
		sw.WriteString(m_cWorldServerName, 30);
		break;
	}

	_socket->write(sw);
	return true;
}

bool CGame::bSendCommand(uint32_t dwMsgID, uint16_t wCommand, char cDir, int iV1, int iV2, int iV3, char const * const pString, int iV4)
{
	char  * cp, cMsg[300 + MAX_MAIL_MSG_LENGTH], cTxt[256], cKey;
    uint32_t * dwp;
    uint64_t dwTime;
	int   iRet, i, * fightzonenum ;
	CDialogBox * dlg;

	StreamWrite sw;

	if (_socket == nullptr) return false;
	dwTime = unixtime();

	cKey = (char)(rand() % 255) +1;

	sw.WriteInt(dwMsgID);

	if(dwMsgID & MSGIDTYPE_MOTION)
	{
		if(m_bIsTeleportRequested)
			return false;
		
		sw.WriteShort(m_sPlayerX);
		sw.WriteShort(m_sPlayerY);
		sw.WriteByte(cDir);
		sw.WriteInt64(dwTime);
		sw.WriteShort(iV1);

        switch (dwMsgID)
        {
            case MSGID_MOTION_GETITEM:
            case MSGID_MOTION_RUN:
            case MSGID_MOTION_MOVE:
            case MSGID_MOTION_DAMAGEMOVE:
                break;
            case MSGID_MOTION_MAGIC:
                sw.WriteShort(iV2);//magic type
                break;
            case MSGID_MOTION_ATTACK:
            case MSGID_MOTION_ATTACKMOVE:
                sw.WriteShort(iV2);//m_sCommX, m_sCommY, wType, 0, m_wCommObjectID
                sw.WriteShort(iV3);
                sw.WriteShort(iV4);
                break;
        }
/*
		if(wCommand != MSGID_MOTION_GETITEM && wCommand != MSGID_MOTION_RUN && 
			wCommand != MSGID_MOTION_MOVE && wCommand != MSGID_MOTION_DAMAGEMOVE)
		{
            //bSendCommand(MSGID_MOTION_GETITEM, 0, m_cPlayerDir, 0, 0, 0, 0);
            sw.WriteShort(iV2);
			if(wCommand != MSGID_MOTION_MAGIC)
			{
                //bSendCommand(MSGID_MOTION_MAGIC, 0, m_cPlayerDir, m_iCastingMagicType, 0, 0, 0);
                sw.WriteShort(iV3);
				if(wCommand == MSGID_MOTION_ATTACK || wCommand == MSGID_MOTION_ATTACKMOVE) 
					sw.WriteShort(iV4);
                //bSendCommand(MSGID_MOTION_ATTACK, 0, cDir, m_sCommX, m_sCommY, wType, 0, m_wCommObjectID);
                //bSendCommand(MSGID_MOTION_ATTACKMOVE, 0, cDir, m_sCommX, m_sCommY, wType, 0, m_wCommObjectID);
            }
		}*/
		
		_socket->write(sw);

		m_cCommandCount++;
	} 
	else 
	{
		sw.WriteShort(wCommand);

		switch (dwMsgID)
		{
		case MSGID_PINGMAP:
			sw.WriteShort(iV1);
			sw.WriteShort(iV2);
			_socket->write(sw);
			break;

		case MSGID_REQ_GUILDBOARD:
			sw.WriteInt((m_gldBoard.size() > 0) ? m_gldBoard[m_gldBoard.size() - 1]->id : 0);
			_socket->write(sw);
			break;

		case MSGID_REQ_MAILBOX:
			sw.WriteInt((m_mails.size() > 0) ? m_mails[m_mails.size() - 1]->id : 0);
			_socket->write(sw);
			break;
		
		case MSGID_REQ_DELETEMAIL:
		case MSGID_REQ_DELETEGUILDPOST:
			sw.WriteInt(iV1);
			_socket->write(sw);
			break;

// 		case MSGID_REQ_SENDMAIL:
// 			dlg = &m_dialogBoxes[DIALOG_MAILBOX];
// 			Push(cp, dlg->cStr); // to
// 			if(dlg->cStr2[0])
// 				Push(cp, dlg->cStr2); // title
// 			else
// 				Push(cp, "Untitled");
// 			Push(cp, dlg->txt); // msg
// 
// 			Push(cp, (uint8_t)dlg->vvec.size());
// 			for(i = 0; i < dlg->vvec.size(); i++)
// 			{
// 				Push(cp, dlg->vvec[i].index);
// 				Push(cp, dlg->vvec[i].count);
// 			}
// 
// 			_socket->write(cMsg, cp - cMsg);
// 			break;
// 
// 		case MSGID_REQ_RETRIEVEMAILITEM:
// 			dlg = &m_dialogBoxes[DIALOG_MAILBOX];
// 			Push(cp, m_mails[dlg->sV1]->id);
// 			Push(cp, m_mails[dlg->sV1]->items[dlg->sV2]->ItemUniqueID);
// 
// 			_socket->write(cMsg, cp - cMsg);
// 			break;
// 
// 		case MSGID_REQ_MAILDATA:
// 		case MSGID_REQ_GUILDPOSTDATA:
// 			Push(cp, (uint32_t)iV1);
// 
// 			_socket->write(cMsg, cp - cMsg);
// 			break;
// 
// 		case MSGID_REQ_POSTGUILDBOARD:
// 			dlg = &m_dialogBoxes[DIALOG_GUILD];
// 			if(dlg->cStr2[0])
// 				Push(cp, dlg->cStr2); // title
// 			else
// 				Push(cp, "Untitled");
// 			Push(cp, dlg->txt); // msg
// 
// 			_socket->write(cMsg, cp - cMsg);
// 			break;
// 
// 		case MSGID_GUILDSMANCHANGE:
// 			Push(cp, (uint8_t)iV1);
// 			memset(cp, 0, 10);
// 			strcpy(cp, pString);
// 			cp += 10;
// 			_socket->write(cMsg, cp - cMsg);
// 			break;
// 
// 		case MSGID_GUILDUPGRADE:
// 			Push(cp, (uint8_t)iV1);
// 			_socket->write(cMsg, cp - cMsg);
// 			break;
// 
// 		case MSGID_GUILDCONTRIBUTE:
// 			Push(cp, (uint8_t)iV1);
// 			Push(cp, (uint32_t)iV2);
// 			_socket->write(cMsg, cp - cMsg);
// 			break;
// 
// 		case MSGID_REQUEST_GUILDBANK:
// 			Push(cp, m_guildBankVer);
// 			_socket->write(cMsg, cp - cMsg);
// 			break;
// 
// 		case MSGID_REQUEST_ANGEL:	// to Game Server
// 			cp = (char*)(cMsg + 6);
// 			memset( cp, 0, 20 );
// 			memcpy((char *)cp, pString, strlen(pString) + 1);
// 			cp += 20;
// 			ip = (int *)cp;
// 			*ip = iV1; // Angel ID
// 			_socket->write(cMsg, 30);
// 			break;
// 
// 		case MSGID_REQUEST_DKITEM:	// to Game Server
// 			cp = (char*)(cMsg + 6);
// 			memset( cp, 0, 20 );
// 			memcpy((char *)cp, pString, strlen(pString) + 1);
// 			cp += 20;
// 			ip = (int *)cp;
// 			*ip = iV1; // DK Item ID
// 			_socket->write(cMsg, 30);
// 			break;
// 
// 		case MSGID_REQUEST_RESURRECTPLAYER_YES: // By snoopy
// 		case MSGID_REQUEST_RESURRECTPLAYER_NO:  // By snoopy
// 		case MSGID_REQUEST_CRITICALS: // CTRL + C Criticals xRisenx
// 			_socket->write(cMsg, 6);
// 			break;
// 
// 		case MSGID_REQUEST_HELDENIAN_SCROLL:// By snoopy
// 			cp = (char*)(cMsg + 6);
// 			memset( cp, 0, 20 );
// 			memcpy((char *)cp, pString, strlen(pString) + 1);
// 			cp += 20;
// 			wp = (uint16_t *)cp;
// 			*wp = wCommand; // Item ID
// 			_socket->write(cMsg, 28);
// 			break;
// 
// 		case MSGID_REQUEST_TELEPORT_LIST:
// 			cp = (char*)(cMsg + 6);
// 			memset( cp, 0, 20 );
// 			strcpy( cp, pString);
// 			_socket->write(cMsg, 26);
// 			break;
// 
// 		case MSGID_REQUEST_QUEST_LIST:
// 			cp = (char*)(cMsg + 6);
// 			memset( cp, 0, 20 );
// 			strcpy( cp, pString);
// 			_socket->write(cMsg, 26);
// 			break;
// 
// 		case MSGID_REQUEST_CHARGED_TELEPORT:
// 			ip  = (int *)(cMsg + INDEX2_MSGTYPE + 2);
// 			*ip = iV1;
// 			_socket->write(cMsg, 10);
// 			break;
// 
// 		case MSGID_REQUEST_ACCEPT_QUEST:
// 			ip  = (int *)(cMsg + INDEX2_MSGTYPE + 2);
// 			*ip = iV1;
// 			_socket->write(cMsg, 10);
// 			break;
// 	
// 		case MSGID_REQUEST_SELLITEMLIST:
// 			cp = (char *)(cMsg + INDEX2_MSGTYPE + 2);
// 			for (i = 0; i < MAXSELLLIST; i++)
// 			{	*cp = m_stSellItemList[i].iIndex;
// 				cp++;
// 				ip = (int *)cp;
// 				*ip = m_stSellItemList[i].iAmount;
// 				cp += 4;
// 			}
// 
// 			_socket->write(cMsg, 70);
// 			break;

		case MSGID_REQUEST_RESTART:
			_socket->write(sw);
			break;

		case MSGID_REQUEST_PANNING:
			sw.WriteByte(cDir);

			_socket->write(sw);
			break;

		case MSGID_REQUEST_SETITEMPOS:
			// to Game Server
			sw.WriteByte(cDir);
			sw.WriteShort(iV1);
			sw.WriteShort(iV2);

			_socket->write(sw);
			break;

		case MSGID_COMMAND_CHECKCONNECTION:
			sw.WriteInt(dwTime);
			_socket->write(sw);

			break;

		case MSGID_REQUEST_INITDATA:
		case MSGID_REQUEST_INITPLAYER:
			// to Game Server
			sw.WriteString(m_cPlayerName, 10);
			sw.WriteString(m_cAccountName, 10);
			sw.WriteString(m_cAccountPassword, 10);
			sw.WriteByte(m_bIsObserverMode);
			sw.WriteString(m_cGameServerName, 20);


			_socket->write(sw);

			//m_bIsObserverMode = FALSE;
			break;
		case MSGID_LEVELUPSETTINGS:

			sw.WriteShort(m_luStat[STAT_STR]);
			sw.WriteShort(m_luStat[STAT_VIT]);
			sw.WriteShort(m_luStat[STAT_DEX]);
			sw.WriteShort(m_luStat[STAT_INT]);
			sw.WriteShort(m_luStat[STAT_MAG]);
			sw.WriteShort(m_luStat[STAT_CHR]);

			_socket->write(sw);
			break;

		case MSGID_COMMAND_CHATMSG:
			if (m_bIsTeleportRequested == true) return false;

			sw.WriteShort(m_sPlayerX);
			sw.WriteShort(m_sPlayerY);
			sw.WriteString(m_cPlayerName, 10);//TODO: fix potential bugs (make std::string)
			sw.WriteByte(iV1);

			if (bCheckLocalChatCommand(pString) == true) return false;
			sw.WriteString(pString, strlen(pString));//TODO: fix potential bugs (make std::string)

			_socket->write(sw);
			break;

		case MSGID_COMMAND_COMMON:
			if (m_bIsTeleportRequested == true) return false;
			sw.WriteShort(m_sPlayerX);
			sw.WriteShort(m_sPlayerY);
			sw.WriteByte(cDir);
			switch (wCommand) {
			case COMMONTYPE_BUILDITEM:
				sw.WriteString(pString, strlen(pString));//TODO: fix potential bugs (make std::string)
				sw.WriteByte(m_dialogBoxes[26].sV1);//TODO: integers
				sw.WriteByte(m_dialogBoxes[26].sV2);
				sw.WriteByte(m_dialogBoxes[26].sV3);
				sw.WriteByte(m_dialogBoxes[26].sV4);
				sw.WriteByte(m_dialogBoxes[26].sV5);
				sw.WriteByte(m_dialogBoxes[26].sV6);
				_socket->write(sw);
				break;

			case COMMONTYPE_REQ_CREATEPOTION:
				sw.WriteByte(m_dialogBoxes[26].sV1);//TODO: integers
				sw.WriteByte(m_dialogBoxes[26].sV2);
				sw.WriteByte(m_dialogBoxes[26].sV3);
				sw.WriteByte(m_dialogBoxes[26].sV4);
				sw.WriteByte(m_dialogBoxes[26].sV5);
				sw.WriteByte(m_dialogBoxes[26].sV6);
				_socket->write(sw);

				break;

			//Crafting
			case COMMONTYPE_CRAFTITEM:
				sw.WriteString("                    ", 20);//TODO: fix - old code
				sw.WriteByte(m_dialogBoxes[26].sV1);//TODO: integers
				sw.WriteByte(m_dialogBoxes[26].sV2);
				sw.WriteByte(m_dialogBoxes[26].sV3);
				sw.WriteByte(m_dialogBoxes[26].sV4);
				sw.WriteByte(m_dialogBoxes[26].sV5);
				sw.WriteByte(m_dialogBoxes[26].sV6);
				_socket->write(sw);

				break;

			case COMMONTYPE_REQ_CREATESLATE:
				sw.WriteByte(m_dialogBoxes[40].sV1);//TODO: integers
				sw.WriteByte(m_dialogBoxes[40].sV2);
				sw.WriteByte(m_dialogBoxes[40].sV3);
				sw.WriteByte(m_dialogBoxes[40].sV4);
				sw.WriteByte(m_dialogBoxes[40].sV5);
				sw.WriteByte(m_dialogBoxes[40].sV6);
				_socket->write(sw);
				break;

			default:
				if (pString == 0)
				{
					sw.WriteInt(iV1);
					sw.WriteInt(iV2);
					sw.WriteInt(iV3);
					sw.WriteInt(dwTime);
					_socket->write(sw);
				}
				else
				{
					sw.WriteInt(iV1);
					sw.WriteInt(iV2);
					sw.WriteInt(iV3);
					sw.WriteString(pString, strlen(pString));//TODO: fix potential bugs (make std::string)
					sw.WriteInt(iV4);
					_socket->write(sw);
				}
				break;
			}

			break;

		case MSGID_REQUEST_CREATENEWGUILD:
		case MSGID_REQUEST_DISBANDGUILD:
			// to Game Server
// 			*wp = MSGTYPE_CONFIRM;
// 
// 			cp = (char *)(cMsg + INDEX2_MSGTYPE + 2);
// 
// 			memset(cTxt, 0, sizeof(cTxt));
// 			memcpy(cTxt, m_cPlayerName, 10);
// 			memcpy(cp, cTxt, 10);
// 			cp += 10;
// 
// 			memset(cTxt, 0, sizeof(cTxt));
// 			memcpy(cTxt, m_cAccountName, 10);
// 			memcpy(cp, cTxt, 10);
// 			cp += 10;
// 
// 			memset(cTxt, 0, sizeof(cTxt));
// 			memcpy(cTxt, m_cAccountPassword, 10);
// 			memcpy(cp, cTxt, 10);
// 			cp += 10;
// 			char cTemp[21];
// 			memset(cTemp, 0, sizeof(cTemp));
// 			memcpy(cTemp, m_cGuildName, 20);
// 			m_Misc.ReplaceString(cTemp, ' ', '_');
// 			memcpy(cp, cTemp, 20);
// 			cp += 20;
// 
// 			_socket->write(cMsg, 56);
			break;

		case MSGID_REQUEST_TELEPORT:
			sw.WriteShort(MSGTYPE_CONFIRM);

			_socket->write(sw);

			m_bIsTeleportRequested = true;
			break;

		case MSGID_REQUEST_CIVILRIGHT:
			sw.WriteShort(MSGTYPE_CONFIRM);

			_socket->write(sw);
			break;

		case MSGID_REQGUILDSUMMONS:
			cp = (char *)(cMsg + INDEX2_MSGTYPE);
			Push(cp, (char*)pString, 10);

			_socket->write(cMsg, cp - cMsg);
			break;

		case MSGID_RSPGUILDSUMMONS:
			cp = (char *)(cMsg + INDEX2_MSGTYPE);
			Push(cp, (uint8_t)iV1);

			_socket->write(cMsg, cp - cMsg);
			break;

		case MSGID_REQUEST_RETRIEVEITEM_GUILDBANK:
			cp = (char *)(cMsg + INDEX2_MSGTYPE);
			Push(cp, (uint32_t)iV1);

			_socket->write(cMsg, cp - cMsg);
			break;
// 
// 		case MSGID_REQUEST_RETRIEVEITEM:
// 			*wp = MSGTYPE_CONFIRM;
// 
// 			cp = (char *)(cMsg + INDEX2_MSGTYPE + 2);
// 			*cp = (char)iV1;
// 
// 			_socket->write(cMsg, 7);
// 			break;
// 
// 		case MSGID_REQUEST_NOTICEMENT:
// 			cp = (char *)(cMsg + INDEX2_MSGTYPE + 2);
// 			ip = (int *)cp;
// 			*ip = iV1;
// 			cp += 4;
// 
// 			_socket->write(cMsg, 10);
// 			break;
// 
// 		case MSGID_REQUEST_FIGHTZONE_RESERVE:
// 			cp = (char *)(cMsg + INDEX2_MSGTYPE + 2);
// 			fightzonenum = (int *)cp;
// 			*fightzonenum = iV1;
// 			cp += 4;
// 
// 			_socket->write(cMsg, 10);
// 			break;

		case MSGID_STATECHANGEPOINT:

			sw.WriteShort((uint16_t)-m_luStat[STAT_STR]);
			sw.WriteShort((uint16_t)-m_luStat[STAT_VIT]);
			sw.WriteShort((uint16_t)-m_luStat[STAT_DEX]);
			sw.WriteShort((uint16_t)-m_luStat[STAT_INT]);
			sw.WriteShort((uint16_t)-m_luStat[STAT_MAG]);
			sw.WriteShort((uint16_t)-m_luStat[STAT_CHR]);

			_socket->write(sw);
			break;
			
// #ifdef TitleClient
// 		case MSGID_SETTITLE:
// 			//BESK in our client - send 4byte integer instead of 21 byte char array
// 			dwp = (uint32_t *)(cMsg + INDEX4_MSGID);
// 			*dwp = dwMsgID;
// 			wp  = (uint16_t *)(cMsg + INDEX2_MSGTYPE);
// 			*wp = 0;
// 			cp = (char*)(cMsg + 6);
// 			//memset( cp, 0, 21 );
// 			//memcpy((char *)cp, pString, strlen(pString) + 1);
// 			//cp += 21;
// 			ip = (int *)cp;
// 			*ip = iV1;
// 			cp += 4;
// 
// 			_socket->write(cMsg, 10/*31*/);//BESK NOTE: we are sending cKey here...
// 			break;
// #endif

		case MSGID_REQUEST_UPDATE_FRIENDS:

			sw.WriteString(pString, strlen(pString) + 1);

			_socket->write(sw);
			break;

		case MSGID_REQUEST_LGNPTS:
		case MSGID_REQUEST_LGNSVC:

			_socket->write(sw);
			break;

		case MSGID_REQUEST_SETRECALLPNT:
			dwp = (uint32_t *)(cMsg + INDEX4_MSGID);
			*dwp = dwMsgID;
			cp = (char *)(cMsg + INDEX2_MSGTYPE + 2);
			*cp = (char)iV1;

			_socket->write(cMsg, 7);
			break;

		default:
			if (m_bIsTeleportRequested == true) return false;

			sw.WriteShort(m_sPlayerX);
			sw.WriteShort(m_sPlayerY);
			sw.WriteByte(cDir);
			sw.WriteShort(iV1);
			sw.WriteShort(iV2);
			sw.WriteShort(iV3);

			if (wCommand == OBJECTATTACK || wCommand == OBJECTATTACKMOVE) {
				sw.WriteShort(iV3);
				sw.WriteInt(dwTime);

				_socket->write(sw);
			}
			else {
				sw.WriteInt(dwTime);
				_socket->write(sw);
			}

			m_cCommandCount++;
			break;
		}
	}

	return true;
}

void CGame::DrawObjects(short sPivotX, short sPivotY, short sDivX, short sDivY, short sModX, short sModY, short msX, short msY)
{
	//int ix, iy, indexX, indexY, dX, dY, iDvalue;
	int ix, iy, indexX, indexY, dX, dY, iDvalue, iItemDropPosX[21+10][15+10], iItemDropPosY[21+10][15+10], iTempX = 0, iTempY = 0; // Show Item On Ground xRisenx
	//char cItemColor;
	int i;
	char cItemColor, cItemDropList[21+10][15+10][21], cStr1[64], cStr2[64], cStr3[64]; // Show Item On Ground xRisenx
	bool bIsPlayerDrawed = false;
	bool bContact = false;
	bool bRet = false;
	uint32_t dwItemAttribute, dwItemDropAttr[21+10][15+10]; // Show Item On Ground xRisenx
	short sItemSprite, sItemSpriteFrame, sObjSpr, sObjSprFrame, sDynamicObject, sDynamicObjectFrame;
	static uint32_t dwMCAnimTime = G_dwGlobalTime;
	static short sMCAnimFrame = 1;
	// Xmas
	static int ix1[100];
	static int iy2[100];
	static int iXmasTreeBulbDelay = 76;
	int idelay = 75;
	uint32_t groundPivotPoint = (m_bigItems) ? SPRID_ITEMPACK_PIVOTPOINT : SPRID_ITEMGROUND_PIVOTPOINT;

	if( sDivY < 0 || sDivX < 0) return ;
	m_sMCX = 0;
	m_sMCY = 0;
	memset(m_cMCName, 0, sizeof(m_cMCName));

	//dwTime = G_dwGlobalTime;
	uint32_t dwTime = m_dwCurTime;
	m_stMCursor.sCursorFrame = 0;
	// Show Item On Ground xRisenx
	/*for(iy = 0; iy < 15+10; iy++)
	{	for(ix = 0; ix < 21+10; ix++)
		{	iItemDropPosX[ix][iy] = -1;
			iItemDropPosY[ix][iy] = -1;
			dwItemDropAttr[ix][iy] = NULL;
			ZeroMemory(cItemDropList[ix][iy], sizeof(cItemDropList[ix][iy]));
	}	}*/

	indexY = sDivY + sPivotY - 7;//BESK TODO: keep this?
	//for (iy = -sModY-224; iy <= 427+352; iy += 32)
	for (iy = -sModY-224; iy <= GetHeight()+100; iy += 32) // 800x600 xRisenx
	{
		indexX = sDivX + sPivotX-4;
		//for (ix = -sModX-128 ; ix <= 640 + 128; ix += 32)
		for (ix = -sModX-128 ; ix <= GetWidth() + 128; ix += 32) // 800x600 Resolution xRisenx
		{
			sDynamicObject = 0;
			bRet = false;
			//if ((ix >= -sModX) && (ix <= 640+16) && (iy >= -sModY) && (iy <= 427+32+16))
			if ((ix >= -sModX) && (ix <= GetWidth()+16) && (iy >= -sModY) && (iy <= GetHeight()+100)) // 800x600 Resolution xRisenx
			{
				_tmp_wObjectID = _tmp_sOwnerType = _tmp_sAppr1 = _tmp_sAppr2 = _tmp_sAppr3 = _tmp_sAppr4 = _tmp_sHeadApprValue = _tmp_sBodyApprValue = _tmp_sArmApprValue = _tmp_sLegApprValue = _tmp_iStatus = 0;
				_tmp_cDir = _tmp_cFrame = 0;
				_tmp_iEffectType = _tmp_iEffectFrame = _tmp_iChatIndex = 0;
				memset(_tmp_cName, 0, sizeof(_tmp_cName));
				if ((indexX < m_pMapData->m_sPivotX) || (indexX > m_pMapData->m_sPivotX + MAPDATASIZEX) ||
					(indexY < m_pMapData->m_sPivotY) || (indexY > m_pMapData->m_sPivotY + MAPDATASIZEY))
				{
					sItemSprite = 0;
					sItemSpriteFrame = 0;
					bRet = false;
					cItemColor = 0;
					//dwItemAttribute = NULL; // 1234
				}
				else
				{
					_tmp_dX = dX = indexX - m_pMapData->m_sPivotX;
					_tmp_dY = dY = indexY - m_pMapData->m_sPivotY; // 296x17 = CRASH POINT. BAD HEADAPPRVALUE
					_tmp_wObjectID  = m_pMapData->m_pData[dX][dY].m_wDeadObjectID;
					_tmp_sOwnerType = m_pMapData->m_pData[dX][dY].m_sDeadOwnerType;
					if(_tmp_sOwnerType == NPC_AS)
						_tmp_sOwnerType = NPC_ESG;

					_tmp_cDir       = m_pMapData->m_pData[dX][dY].m_cDeadDir;
					_tmp_sAppr1     = m_pMapData->m_pData[dX][dY].m_sDeadAppr1;
					_tmp_sAppr2     = m_pMapData->m_pData[dX][dY].m_sDeadAppr2;
					_tmp_sAppr3     = m_pMapData->m_pData[dX][dY].m_sDeadAppr3;
					_tmp_sAppr4     = m_pMapData->m_pData[dX][dY].m_sDeadAppr4;
					_tmp_iApprColor = m_pMapData->m_pData[dX][dY].m_iDeadApprColor;
					
					// Variable gets naughty value
					_tmp_sHeadApprValue = m_pMapData->m_pData[dX][dY].m_sDeadHeadApprValue; // Re-Coding Sprite xRisenx
					// Bad value is patched to ensure client operation.
					//if(_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0; // this is the ACTUAL max? so that math works? No i can add upto 65k with short :D, we just need a safe value for sprite ID
					// there aren't 65k sprites NO lemme find out :D
					// u so pro even when i do noob stuff to u xD 18 is max
					_tmp_sBodyApprValue = m_pMapData->m_pData[dX][dY].m_sDeadBodyApprValue; // Re-Coding Sprite xRisenx
					_tmp_sArmApprValue  = m_pMapData->m_pData[dX][dY].m_sDeadArmApprValue; // Re-Coding Sprite xRisenx
					_tmp_sLegApprValue  = m_pMapData->m_pData[dX][dY].m_sDeadLegApprValue; // Re-Coding Sprite xRisenx // hmm? no trace beyond dY... :S
					_tmp_cFrame     = m_pMapData->m_pData[dX][dY].m_cDeadOwnerFrame;
					_tmp_iChatIndex = m_pMapData->m_pData[dX][dY].m_iDeadChatMsg;
					_tmp_iStatus    = m_pMapData->m_pData[dX][dY].m_iDeadStatus;
					strcpy(_tmp_cName, m_pMapData->m_pData[dX][dY].m_cDeadOwnerName);
					sItemSprite      = m_pMapData->m_pData[dX][dY].m_sItemSprite;
					sItemSpriteFrame = m_pMapData->m_pData[dX][dY].m_sItemSpriteFrame;
					//dwItemAttribute = m_pMapData->m_pData[dX][dY].m_dwItemAttribute; // 1234
					cItemColor       = m_pMapData->m_pData[dX][dY].m_cItemColor;
					sDynamicObject      = m_pMapData->m_pData[dX][dY].m_sDynamicObjectType;
					sDynamicObjectFrame = (short)m_pMapData->m_pData[dX][dY].m_cDynamicObjectFrame;
					cDynamicObjectData1 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData1;
					cDynamicObjectData2 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData2;
					cDynamicObjectData3 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData3;
					cDynamicObjectData4 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData4;
					bRet = true;
			 	}

				if ((bRet == true) && (sItemSprite != 0) && !m_altPressed)
				{
                    if (cItemColor == 0)
						 m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteFast(ix, iy, sItemSpriteFrame, dwTime);
					else
					{
                        switch (sItemSprite)
                        {
						    case 1: // Swds
						    case 2: // Bows
						    case 3: // Shields
						    case 15: // Axes hammers
							    m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
								    , sItemSpriteFrame, m_wWR[cItemColor] -m_wR[0], m_wWG[cItemColor] -m_wG[0], m_wWB[cItemColor] -m_wB[0], dwTime);
							    break;
						    default:
							    m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
								    , sItemSpriteFrame, m_wR[cItemColor] -m_wR[0], m_wG[cItemColor] -m_wG[0], m_wB[cItemColor] -m_wB[0], dwTime);
							    break;
					    }
                    }

					if ((ix - 13 < msX)	&& (ix + 13 > msX) && (iy - 13 < msY) && (iy + 13 > msY))
					{
                        if ((dwTime - dwMCAnimTime)	> 200)
						{
                            dwMCAnimTime = dwTime;
							if (sMCAnimFrame == 1)
								 sMCAnimFrame = 2;
							else sMCAnimFrame = 1;
						}
						m_stMCursor.sCursorFrame  = sMCAnimFrame;
				    }
                }

				if ((bRet == true) && (_tmp_wObjectID != 0))
				{
					bContact = DrawObject_OnDead(indexX, indexY, ix, iy, false, dwTime, msX, msY);
					if(!m_bShiftPressed)
						bContact = false;
				}

				//if ((bRet == TRUE) && (sItemSprite != NULL)/* && m_altPressed*/)
				//{	if (cItemColor == 0) 
				//		m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteFast(ix, iy, sItemSpriteFrame, dwTime);
				//	else
				//	{	switch (sItemSprite) {
				//		case 1: // Swds
				//		case 2: // Bows
				//		case 3: // Shields
				//		case 15: // Axes hammers
				//		case 17: // Wands
				//			m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
				//				, sItemSpriteFrame, m_wWR[cItemColor] -m_wR[0], m_wWG[cItemColor] -m_wG[0], m_wWB[cItemColor] -m_wB[0], dwTime);
				//			break;
				//		default:
				//			m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
				//				, sItemSpriteFrame, m_wR[cItemColor] -m_wR[0], m_wG[cItemColor] -m_wG[0], m_wB[cItemColor] -m_wB[0], dwTime);
				//			break;
				//	}	}

				//	if (m_bShiftPressed && ((sItemSprite == 6 && sItemSpriteFrame > 8 && !(sItemSpriteFrame >= 17 && sItemSpriteFrame <= 54) && !(sItemSpriteFrame >= 93 && sItemSpriteFrame <= 107)) || sItemSprite != 6/* || memcmp(m_pItemConfigList[sIDnum]->m_cName, "Power", 5) == 0 || memcmp(m_pItemConfigList[sIDnum]->m_cName, "Dilution", 8) == 0*/))
				//	{	iItemDropPosX[iTempX-1][iTempY-1] = ix;
				//		iItemDropPosY[iTempX-1][iTempY-1] = iy;
				//		dwItemDropAttr[iTempX-1][iTempY-1] = dwItemAttribute;
				//		strcpy(cItemDropList[iTempX-1][iTempY-1],  m_pBuildItemListStats[i]->m_cName);
				//	}
				//	
				//	if ((ix - 13 < msX)	&& (ix + 13 > msX) && (iy - 13 < msY) && (iy + 13 > msY))
				//	{	if ((dwTime - dwMCAnimTime)	> 200)
				//		{	dwMCAnimTime = dwTime;
				//			if (sMCAnimFrame == 1)
				//				 sMCAnimFrame = 2;
				//			else sMCAnimFrame = 1;
				//		}
				//		m_stMCursor.sCursorFrame  = sMCAnimFrame;

				//		if (m_bCtrlPressed && !m_bShiftPressed)
				//		{	iItemDropPosX[iTempX-1][iTempY-1] = msX;
				//			iItemDropPosY[iTempX-1][iTempY-1] = msY+30;
				//			dwItemDropAttr[iTempX-1][iTempY-1] = dwItemAttribute;
				//			strcpy(cItemDropList[iTempX-1][iTempY-1], m_pBuildItemListStats[i]->m_cName);
				//		}
				//}	}

				if ((bRet == true) && (sItemSprite != 0) && m_altPressed)
				{
                    if (cItemColor == 0)
						 m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteFast(ix, iy, sItemSpriteFrame, dwTime);
					else
					{
                        switch (sItemSprite)
                        {
						    case 1: // Swds
						    case 2: // Bows
						    case 3: // Shields
						    case 15: // Axes hammers
							    m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
								    , sItemSpriteFrame, m_wWR[cItemColor] -m_wR[0], m_wWG[cItemColor] -m_wG[0], m_wWB[cItemColor] -m_wB[0], dwTime);
							    break;
						    default:
							    m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
								    , sItemSpriteFrame, m_wR[cItemColor] -m_wR[0], m_wG[cItemColor] -m_wG[0], m_wB[cItemColor] -m_wB[0], dwTime);
							    break;
					    }
                    }

					if ((ix - 13 < msX)	&& (ix + 13 > msX) && (iy - 13 < msY) && (iy + 13 > msY))
					{
                        if ((dwTime - dwMCAnimTime)	> 200)
						{
                            dwMCAnimTime = dwTime;
							if (sMCAnimFrame == 1)
								 sMCAnimFrame = 2;
							else sMCAnimFrame = 1;
						}
						m_stMCursor.sCursorFrame  = sMCAnimFrame;
				    }
                }

				//if ((bContact == TRUE) && (msY <= 431))
				if ((bContact == true) && (msY <= GetHeight()-49)) // Resolution Limit, Decides how long Down(y) you can see npcs ( Fixed xRisenx )
				{
                    m_sMCX = indexX;
					m_sMCY = indexY;
					sFocusX = ix;
					sFocusY = iy;
					sFocusOwnerType = _tmp_sOwnerType;
					cFocusAction = OBJECTDEAD;
					wFocusObjectID = _tmp_wObjectID;
					cFocusFrame  = _tmp_cFrame;
					cFocusDir    = _tmp_cDir;
					sFocusAppr1  = _tmp_sAppr1;
					sFocusAppr2  = _tmp_sAppr2;
					sFocusAppr3  = _tmp_sAppr3;
					sFocusAppr4  = _tmp_sAppr4;
					iFocusApprColor = _tmp_iApprColor;
					sFocusHeadApprValue = _tmp_sHeadApprValue; // Re-Coding Sprite xRisenx
					sFocusBodyApprValue = _tmp_sBodyApprValue; // Re-Coding Sprite xRisenx
					sFocusArmApprValue = _tmp_sArmApprValue; // Re-Coding Sprite xRisenx
					sFocusLegApprValue = _tmp_sLegApprValue; // Re-Coding Sprite xRisenx
					iFocuiStatus = _tmp_iStatus;
					memset(cFocusName, 0, sizeof(cFocusName));
					strcpy(cFocusName, _tmp_cName);
					memset(m_cMCName, 0, sizeof(m_cMCName));
					strcpy(m_cMCName,  _tmp_cName);
					sFocus_dX = _tmp_dX;
					sFocus_dY = _tmp_dY;
					bContact = false;
				}

				_tmp_wObjectID = _tmp_sOwnerType = _tmp_sAppr1 = _tmp_sAppr2 = _tmp_sAppr3 = _tmp_sAppr4 = _tmp_sHeadApprValue = _tmp_sBodyApprValue = _tmp_sArmApprValue = _tmp_sLegApprValue = _tmp_iStatus = 0; // Re-Coding Sprite xRisenx
				_tmp_cFrame = _tmp_cDir = 0;
				_tmp_iEffectType = _tmp_iEffectFrame = _tmp_iApprColor = _tmp_iChatIndex = 0;
				memset(_tmp_cName, 0, sizeof(_tmp_cName));

				if ((indexX < m_pMapData->m_sPivotX) || (indexX > m_pMapData->m_sPivotX + MAPDATASIZEX) ||
					(indexY < m_pMapData->m_sPivotY) || (indexY > m_pMapData->m_sPivotY + MAPDATASIZEY))
				{	sItemSprite = 0;
					bRet = false;
				}
                else
				{
                    _tmp_dX = dX = indexX - m_pMapData->m_sPivotX;
					_tmp_dY = dY = indexY - m_pMapData->m_sPivotY;
					_tmp_wObjectID  = m_pMapData->m_pData[dX][dY].m_wObjectID;
					_tmp_sOwnerType = m_pMapData->m_pData[dX][dY].m_sOwnerType;
					if(_tmp_sOwnerType == NPC_AS)
						_tmp_sOwnerType = NPC_ESG;

					_tmp_cAction    = m_pMapData->m_pData[dX][dY].m_cOwnerAction;
					_tmp_iStatus    = m_pMapData->m_pData[dX][dY].m_iStatus;
					_tmp_cDir       = m_pMapData->m_pData[dX][dY].m_cDir;
					_tmp_sAppr1     = m_pMapData->m_pData[dX][dY].m_sAppr1;
					_tmp_sAppr2     = m_pMapData->m_pData[dX][dY].m_sAppr2;
					_tmp_sAppr3     = m_pMapData->m_pData[dX][dY].m_sAppr3;
					_tmp_sAppr4     = m_pMapData->m_pData[dX][dY].m_sAppr4;
					_tmp_iApprColor = m_pMapData->m_pData[dX][dY].m_iApprColor;
					_tmp_sHeadApprValue = m_pMapData->m_pData[dX][dY].m_sHeadApprValue; // Re-Coding Sprite xRisenx
					_tmp_sBodyApprValue = m_pMapData->m_pData[dX][dY].m_sBodyApprValue; // Re-Coding Sprite xRisenx
					_tmp_sArmApprValue  = m_pMapData->m_pData[dX][dY].m_sArmApprValue; // Re-Coding Sprite xRisenx
					_tmp_sLegApprValue  = m_pMapData->m_pData[dX][dY].m_sLegApprValue; // Re-Coding Sprite xRisenx
					_tmp_cFrame     = m_pMapData->m_pData[dX][dY].m_cOwnerFrame;
					_tmp_iChatIndex = m_pMapData->m_pData[dX][dY].m_iChatMsg;
					_tmp_iEffectType  = m_pMapData->m_pData[dX][dY].m_iEffectType;
					_tmp_iEffectFrame = m_pMapData->m_pData[dX][dY].m_iEffectFrame;

					strcpy(_tmp_cName, m_pMapData->m_pData[dX][dY].m_cOwnerName);
					bRet = true;

					if (m_iIlusionOwnerH != 0)
					{
						if ((strcmp(_tmp_cName, m_cPlayerName) != 0) && (_tmp_sOwnerType < 10))
						{
							_tmp_sOwnerType = m_cIlusionOwnerType;
							if(_tmp_iStatus & STATUS_INVISIBILITY)
							{
								_tmp_iStatus = m_iStatus_IE;
								_tmp_iStatus |= STATUS_INVISIBILITY;
							}
							else
							{
								_tmp_iStatus    = m_iStatus_IE;
							}
							_tmp_sAppr1     = m_sAppr1_IE;
							_tmp_sAppr2     = m_cIlusionOwnerType ? m_sAppr2_IE : 0xF000;
							_tmp_sAppr3     = m_sAppr3_IE;
							_tmp_sAppr4     = m_sAppr4_IE;
							_tmp_iApprColor = m_iApprColor_IE;
							_tmp_sHeadApprValue    = m_sHeadApprValue_IE; // Re-Coding Sprite xRisenx
							_tmp_sBodyApprValue    = m_sBodyApprValue_IE; // Re-Coding Sprite xRisenx
							_tmp_sArmApprValue     = m_sArmApprValue_IE; // Re-Coding Sprite xRisenx
							_tmp_sLegApprValue     = m_sLegApprValue_IE; // Re-Coding Sprite xRisenx
			 			}
					}
				}

				if ((bRet == true) && (strlen(_tmp_cName) > 0))
				{	
					_tmp_dx = 0;
					_tmp_dy = 0;
					switch (_tmp_cAction) {
					case OBJECTSTOP:
						bContact = DrawObject_OnStop(indexX, indexY, ix, iy, false, dwTime, msX, msY);
						break;

					case OBJECTMOVE:
						bContact = DrawObject_OnMove(indexX, indexY, ix, iy, false, dwTime, msX, msY);
						break;

					case OBJECTDAMAGEMOVE:
						bContact = DrawObject_OnDamageMove(indexX, indexY, ix, iy, false, dwTime, msX, msY);
						break;

					case OBJECTRUN:
						bContact = DrawObject_OnRun(indexX, indexY, ix, iy, false, dwTime, msX, msY);
						break;

					case OBJECTATTACK:
						bContact = DrawObject_OnAttack(indexX, indexY, ix, iy, false, dwTime, msX, msY);
						break;

					case OBJECTATTACKMOVE:
						bContact = DrawObject_OnAttackMove(indexX, indexY, ix, iy, false, dwTime, msX, msY);
						break;

					case OBJECTMAGIC:
						bContact = DrawObject_OnMagic(indexX, indexY, ix, iy, false, dwTime, msX, msY);
						break;

					case OBJECTGETITEM:
						bContact = DrawObject_OnGetItem(indexX, indexY, ix, iy, false, dwTime, msX, msY);
						break;

					case OBJECTDAMAGE:
						bContact = DrawObject_OnDamage(indexX, indexY, ix, iy, false, dwTime, msX, msY);
						break;

					case OBJECTDYING:
						bContact = DrawObject_OnDying(indexX, indexY, ix, iy, false, dwTime, msX, msY);
						if(!m_bShiftPressed)
							bContact = false;
						break;
					}

					//if(bContact && msY <= 431)
					if(bContact && msY <= GetHeight()-49/*551*/) // Resolution Limit, Decides how long Down(y) you can see npcs ( Fixed xRisenx )
					{
						m_sMCX = indexX;
						m_sMCY = indexY;
						sFocusX = ix;
						sFocusY = iy;
						wFocusObjectID = _tmp_wObjectID;
						sFocusOwnerType = _tmp_sOwnerType;
						cFocusAction = _tmp_cAction;
						cFocusFrame  = _tmp_cFrame;
						cFocusDir    = _tmp_cDir;
						sFocusAppr1  = _tmp_sAppr1;
						sFocusAppr2  = _tmp_sAppr2;
						sFocusAppr3  = _tmp_sAppr3;
						sFocusAppr4  = _tmp_sAppr4;
						iFocusApprColor = _tmp_iApprColor;
						sFocusHeadApprValue = _tmp_sHeadApprValue; // Re-Coding Sprite xRisenx
						sFocusBodyApprValue = _tmp_sBodyApprValue; // Re-Coding Sprite xRisenx
						sFocusArmApprValue  = _tmp_sArmApprValue; // Re-Coding Sprite xRisenx
						sFocusLegApprValue  = _tmp_sLegApprValue; // Re-Coding Sprite xRisenx
						iFocuiStatus = _tmp_iStatus;
						memset(cFocusName, 0, sizeof(cFocusName));
						strcpy(cFocusName, _tmp_cName);
						memset(m_cMCName, 0, sizeof(m_cMCName));
						strcpy(m_cMCName,  _tmp_cName);
						sFocus_dX = _tmp_dX;
						sFocus_dY = _tmp_dY;
						bContact = false;
					}

					if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0)
					{
						if (m_bIsObserverMode == false)
						{
							//m_sViewDstX = (indexX*32) - 288 - 32;
							//m_sViewDstY = (indexY*32) - 224;
							//m_sViewDstX = (indexX*32) - 288 - 32-32-32; // 800x600 Resolution xRisenx Center Char
							//m_sViewDstY = (indexY*32) - 224-32-32; // 800x600 Resolution xRisenx Center Char
                             m_sViewDstX = (m_sPlayerX - viewdstxcharvar) * 32; // 800x600 Resolution xRisenx Center Char
                             m_sViewDstY = (m_sPlayerY - viewdstycharvar) * 32; // 800x600 Resolution xRisenx Center Char

                             m_sViewDstX = (m_sPlayerX - (GetWidth() / 32) / 2) * 32;
                             m_sViewDstY = (m_sPlayerY - ((GetHeight() - 60) / 32) / 2) * 32;


                        }
						SetRect(&m_rcPlayerRect, m_rcBodyRect.left, m_rcBodyRect.top, m_rcBodyRect.right, m_rcBodyRect.bottom);
						bIsPlayerDrawed = true;
		   			}
				}
			}

			// CLEROTH
			sObjSpr      = m_pMapData->m_tile[indexX][indexY].m_sObjectSprite;
			sObjSprFrame = m_pMapData->m_tile[indexX][indexY].m_sObjectSpriteFrame;

			if (sObjSpr != 0)
			{
				if ((sObjSpr < 100) || (sObjSpr >= 200))
				{
					switch (sObjSpr)
					{
					case 200:
					case 223:
						m_pTileSpr[sObjSpr]->PutShadowSprite(ix - 16, iy - 16, sObjSprFrame, dwTime);
						break;

					case 224:
						switch (sObjSprFrame)
						{
                            case 24:
                            case 34:
                            case 35:
                            case 36:
                            case 37:
                            case 38:
                                break;
                            default:
                                m_pTileSpr[sObjSpr]->PutShadowSprite(ix - 16, iy - 16, sObjSprFrame, dwTime);
                                break;
						}
					}
					if (m_cDetailLevel == 0) // Special Grass & Flowers
					{
						if ((sObjSpr != 6) && (sObjSpr != 9))
							m_pTileSpr[sObjSpr]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
					}
					else
					{
						m_pTileSpr[sObjSpr]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
					}

					switch (sObjSpr)
					{
					case 200:
						if (sObjSprFrame == 32)
						{
							if (G_cSpriteAlphaDegree == 2)
							{
								int iDvalue1 = -1*(rand() % 5);
								m_pEffectSpr[0]->PutTransSpriteRGB(ix-2,  iy - 75, 1, iDvalue1, iDvalue1, iDvalue1, dwTime);
							}
						}
						break;
					case 223:
						if (sObjSprFrame == 4)
						{
							if (G_cSpriteAlphaDegree == 2) //nuit
							{
								int iDvalue1 = -1*(rand() % 5);
								int iDvalue2 = -1*(rand() % 5);
								int iDvalue3 = -1*(rand() % 5);
								m_pEffectSpr[0]->PutTransSpriteRGB(ix-2,  iy - 150, 1, iDvalue1, iDvalue1, iDvalue1, dwTime);
								m_pEffectSpr[0]->PutTransSpriteRGB(ix+16, iy - 96,  1, iDvalue2, iDvalue2, iDvalue2, dwTime);
								m_pEffectSpr[0]->PutTransSpriteRGB(ix-19, iy - 126, 1, iDvalue3, iDvalue3, iDvalue3, dwTime);
							}
						}
						break;

					case 370: // nuit
						if ( ((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 9) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix -16 +30, m_sViewPointY + iy -16 -334, 0, 0, 0, 0);
						if ( ((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 11) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX +ix -16 +17, m_sViewPointY + iy -16 -300, 0, 0, 0, 0);
						break;

					case 374: // nuit
						if ( ((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 2) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix -7, m_sViewPointY + iy -122, 0, 0, 0, 0);
						if ( ((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 6) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix -14, m_sViewPointY + iy -321, 0, 0, 0, 0);
						if ( ((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 7) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX +ix +7, m_sViewPointY + iy -356, 0, 0, 0, 0);
						break;

					case 376: // nuit
						if ( ((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 12) && (G_cSpriteAlphaDegree == 2))
						{
							bAddNewEffect(65, m_sViewPointX + ix -16, m_sViewPointY + iy -346, 0, 0, 0, 0);
							bAddNewEffect(65, m_sViewPointX + ix +11, m_sViewPointY + iy -308, 0, 0, 0, 0);
						}
						break;

					case 378: // nuit
						if ( ((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 11) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix, m_sViewPointY + iy -91, 0, 0, 0, 0);
						break;

					case 382: // nuit
						if ( ((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 9) && (G_cSpriteAlphaDegree == 2))
						{
							bAddNewEffect(65, m_sViewPointX + ix +73, m_sViewPointY + iy -264, 0, 0, 0, 0);
							bAddNewEffect(65, m_sViewPointX + ix +23, m_sViewPointY + iy -228, 0, 0, 0, 0);
						}
						break;

					case 429:
						if ( ((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 2)) bAddNewEffect(65, m_sViewPointX + ix -15, m_sViewPointY + iy -224, 0, 0, 0, 0);
						break;
					}
				}
				else // sprites 100..199: Trees and tree shadows
				{	
					m_pTileSpr[sObjSpr]->_GetSpriteRect(ix - 16, iy - 16, sObjSprFrame);
					//if (m_cDetailLevel==0)
					//{	
						if( sObjSpr < 100 + 11 ) m_pTileSpr[100 + 4]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
						else if( sObjSpr < 100 + 23 ) m_pTileSpr[100 + 9]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
						else if( sObjSpr < 100 + 32 ) m_pTileSpr[100 + 23]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
						else m_pTileSpr[100 + 32]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
					//}else
					//{	
					//	//Druncncity crash fixed
					//	if ((bIsPlayerDrawed == TRUE) && (m_pTileSpr[sObjSpr] != 0) && (m_pTileSpr[sObjSpr]->m_rcBound.top <= m_rcPlayerRect.top) && (m_pTileSpr[sObjSpr]->m_rcBound.bottom >= m_rcPlayerRect.bottom) &&
					//		(m_cDetailLevel >= 2) && (m_pTileSpr[sObjSpr]->m_rcBound.left <= m_rcPlayerRect.left) && (m_pTileSpr[sObjSpr]->m_rcBound.right >= m_rcPlayerRect.right))
					//	{	
					//		m_pTileSpr[sObjSpr + 50]->PutFadeSprite(ix , iy , sObjSprFrame, dwTime);
					//		m_pTileSpr[sObjSpr]->PutTransSprite2(ix - 16, iy - 16, sObjSprFrame, dwTime);
					//	}else
					//	{	
					//		m_pTileSpr[sObjSpr + 50]->PutSpriteFast(ix , iy , sObjSprFrame, dwTime);
					//		m_pTileSpr[sObjSpr]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
					//	}

					//	if(m_bIsXmas && G_cSpriteAlphaDegree == 2) // nuit
					//	{	
					//		if( iXmasTreeBulbDelay < 0 || iXmasTreeBulbDelay > idelay + 1) iXmasTreeBulbDelay = 0;
					//		if( iXmasTreeBulbDelay > idelay )
					//		{	
					//			for (int i = 0; i < 100; i++) 
					//			{
					//				ix1[i] = 1*(rand() % 400)-200;
					//				iy2[i] = -1*(rand() % 300);
					//			}
					//			iXmasTreeBulbDelay = 0;
					//		}	
					//		else 
					//			iXmasTreeBulbDelay++;

					//		for (int j = 0; j < 100; j++)
					//		{	
					//			if( m_pTileSpr[sObjSpr]->_bCheckCollison(ix-16, iy-16, sObjSprFrame, ix + ix1[j], iy + iy2[j]) )
					//			{	
					//				m_pEffectSpr[66+(j%6)]->PutTransSprite(ix + ix1[j], iy + iy2[j], (iXmasTreeBulbDelay>>2), dwTime);
					//			}	
					//		}	
					//	}
					//}	
				}	
			}

			// Dynamic Object
			if ( (bRet == true) && (sDynamicObject != 0) )
			{
                switch (sDynamicObject) {
				case DYNAMICOBJECT_PCLOUD_BEGIN:	// 10
					if (sDynamicObjectFrame >= 0)
						m_pEffectSpr[23]->PutTransSprite50_NoColorKey(ix+(rand() % 2), iy+(rand() % 2), sDynamicObjectFrame, dwTime);
					break;

				case DYNAMICOBJECT_PCLOUD_LOOP:		// 11
					m_pEffectSpr[23]->PutTransSprite50_NoColorKey(ix+(rand() % 2), iy+(rand() % 2), sDynamicObjectFrame+8, dwTime);
					break;

				case DYNAMICOBJECT_PCLOUD_END:		// 12
					m_pEffectSpr[23]->PutTransSprite50_NoColorKey(ix+(rand() % 2), iy+(rand() % 2), sDynamicObjectFrame+16, dwTime);
					break;

				case DYNAMICOBJECT_ICESTORM:		// 8
					iDvalue = (rand() % 5)*(-1);
					m_pEffectSpr[0]->PutTransSpriteRGB(ix, iy, 1, iDvalue, iDvalue, iDvalue, dwTime);
					m_pEffectSpr[13]->PutTransSprite70_NoColorKey(ix, iy, sDynamicObjectFrame, dwTime);
					break;

				case DYNAMICOBJECT_FIRE:			// 1
				case DYNAMICOBJECT_FIRE3:			// 14
					switch (rand() % 3)
                    {
                        case 0: m_pEffectSpr[0]->PutTransSprite25_NoColorKey(ix, iy, 1, dwTime); break;
                        case 1: m_pEffectSpr[0]->PutTransSprite50_NoColorKey(ix, iy, 1, dwTime); break;
                        case 2: m_pEffectSpr[0]->PutTransSprite70_NoColorKey(ix, iy, 1, dwTime); break;
					}
					m_pEffectSpr[9]->PutTransSprite70_NoColorKey(ix, iy, sDynamicObjectFrame/3, dwTime);
					break;

				case DYNAMICOBJECT_FIRE2:			// 13
					switch (rand() % 3)
                    {
                        case 0: m_pEffectSpr[0]->PutTransSprite25_NoColorKey(ix, iy, 1, dwTime); break;
                        case 1: m_pEffectSpr[0]->PutTransSprite50_NoColorKey(ix, iy, 1, dwTime); break;
                        case 2: m_pEffectSpr[0]->PutTransSprite70_NoColorKey(ix, iy, 1, dwTime); break;
					}
					break;

				case DYNAMICOBJECT_FISH:			// 2
					{
                    char cTmpDOdir, cTmpDOframe;
						cTmpDOdir   = m_Misc.cCalcDirection(cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData1 + cDynamicObjectData3, cDynamicObjectData2 + cDynamicObjectData4);
						cTmpDOframe = ((cTmpDOdir-1) * 4) + (rand() % 4);
						m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+0]->PutTransSprite2(ix + cDynamicObjectData1, iy + cDynamicObjectData2, cTmpDOframe, dwTime);
					}
					break;

				case DYNAMICOBJECT_MINERAL1:		// 4
					if (m_cDetailLevel != 0) m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->PutShadowSprite(ix, iy, 0, dwTime);
					m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->PutSpriteFast(ix, iy, 0, dwTime);
					if (    (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->m_rcBound.top != -1)
						&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->m_rcBound.top < msY)
						&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->m_rcBound.bottom > msY)
						&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->m_rcBound.left < msX)
						&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->m_rcBound.right > msX) )
					{
                        m_sMCX = indexX;
						m_sMCY = indexY;
						iFocuiStatus = 0;
						memset(cFocusName, 0, sizeof(cFocusName));
						memset(m_cMCName, 0, sizeof(m_cMCName));
					}
					break;

				case DYNAMICOBJECT_MINERAL2:		// 5
					if (m_cDetailLevel != 0) m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->PutShadowSprite(ix, iy, 1, dwTime);
					m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->PutSpriteFast(ix, iy, 1, dwTime);
					if (   (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->m_rcBound.top != -1)
						&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->m_rcBound.top < msY)
						&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->m_rcBound.bottom > msY)
						&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->m_rcBound.left < msX)
						&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+1]->m_rcBound.right > msX) )
					{
                        m_sMCX = indexX;
						m_sMCY = indexY;
						iFocuiStatus = 0;
						memset(cFocusName, 0, sizeof(cFocusName));
						memset(m_cMCName, 0, sizeof(m_cMCName));
					}
					break;

				case DYNAMICOBJECT_SPIKE:			// 9
					m_pEffectSpr[17]->PutTransSprite70_NoColorKey(ix, iy, sDynamicObjectFrame, dwTime);
					break;

				case DYNAMICOBJECT_ARESDENFLAG:  // 6
				case DYNAMICOBJECT_ELVINEFLAG: // 7
					m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT+2]->PutSpriteFast(ix, iy, sDynamicObjectFrame, dwTime);
					break;
			    }
            }
			indexX++;
		}
		indexY++;
	}

	if ((dwTime - m_dwEnvEffectTime) > 400) m_dwEnvEffectTime = dwTime;

	if (m_sMCX != 0)	// CLEROTH - STATUS
	{	if( _iGetFOE(iFocuiStatus) < 0 ) m_stMCursor.sCursorFrame = 3;
		else m_stMCursor.sCursorFrame = 6;

		_tmp_wObjectID  = wFocusObjectID;
		_tmp_sOwnerType = sFocusOwnerType;

		if(_tmp_sOwnerType == NPC_AS)
			_tmp_sOwnerType = NPC_ESG;

		_tmp_cAction    = cFocusAction;
		_tmp_cFrame     = cFocusFrame;
		_tmp_cDir       = cFocusDir;
		_tmp_sAppr1     = sFocusAppr1;
		_tmp_sAppr2     = sFocusAppr2;
		_tmp_sAppr3     = sFocusAppr3;
		_tmp_sAppr4     = sFocusAppr4;
		_tmp_iApprColor = iFocusApprColor;
		//if (_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0;
		_tmp_sHeadApprValue = sFocusHeadApprValue; // Re-Coding Sprite xRisenx // Maybe something like this to patch the crash
		_tmp_sBodyApprValue = sFocusBodyApprValue; // Re-Coding Sprite xRisenx
		_tmp_sArmApprValue = sFocusArmApprValue; // Re-Coding Sprite xRisenx
		_tmp_sLegApprValue = sFocusLegApprValue; // Re-Coding Sprite xRisenx
		_tmp_iStatus    = iFocuiStatus;
		strcpy(_tmp_cName, cFocusName);
		_tmp_dX = sFocus_dX;
		_tmp_dY = sFocus_dY;

		if ( (_tmp_cAction != OBJECTDEAD) && (_tmp_cFrame < 0) ) return;
		switch (_tmp_cAction) {
		case OBJECTSTOP:
			DrawObject_OnStop(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
			break;
		case OBJECTMOVE:
			switch (_tmp_sOwnerType) {
			case 1:
			case 2:
			case 3: // Human M
			case 4:
			case 5:
			case 6: // Human F

			case 28: // Troll.
			case 29: // Ogre
			case 30: // Liche
			case 31: // DD
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
			case 51: // CP
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
			case 91: // Gate
			//case 103: // Orc Lord
			case 104: // Dark Unicorn
			case 105: // Black Demon
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
			//case 115: // Mother Dragon
			//case 116: // Hell Dragon
			//case 117: // Alligator
			//case 118: // Puma
			//case 119: // Zombie King
			//case 120: // Slime King
			//case 121: // Slime Priness
			//case 122: // Enraged Troll
			case 123: // Enraged Cyclops
			case 124: // Enraged Stalker
			case 125: // Enraged Gagoyle
			case 126: // Enraged Hellclaw
			case 127: // Enraged Tigerworm

		/*	case 15: // ShopKeeper // Ils ont 8 sprites Stop
			case 19: // Gandalf
			case 20: // Howard
			case 24: // Tom
			case 25: // William
			case 26: // Kenedy
			case 90: // Gail*/
				break;

			default: // 10..27
				_tmp_cFrame = _tmp_cFrame * 2; //
				break;
			}

			DrawObject_OnMove(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
			break;

		case OBJECTDAMAGEMOVE:
			DrawObject_OnDamageMove(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
			break;

		case OBJECTRUN:
			DrawObject_OnRun(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
			break;

		case OBJECTATTACK:
			DrawObject_OnAttack(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
			break;

		case OBJECTATTACKMOVE:
			DrawObject_OnAttackMove(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
			break;

		case OBJECTMAGIC:
			DrawObject_OnMagic(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
			break;

		case OBJECTDAMAGE:
			DrawObject_OnDamage(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
			break;

		case OBJECTDYING: //10
			DrawObject_OnDying(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
			break;

		case OBJECTDEAD: //101
			DrawObject_OnDead(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
			break;
	}	}
	// Show Gr
	//if (!bObjSprRange) bSetSprRangeObj(-1, true);
	
	/*for(iy = 0; iy < 15+10; iy++)
	{	for(ix = 0; ix < 21+10; ix++)
		{	if (iItemDropPosX[ix][iy] != -1 && iItemDropPosY[ix][iy] != -1) {
				iTempX = iTempY = 0;
				GetItemName(cItemDropList[ix][iy], dwItemDropAttr[ix][iy], cStr1, cStr2, cStr3);
				if (strlen(cStr2) > 0) {
					iTempX = strlen(cStr2);
					iTempY++;
				}
				if (strlen(cStr3) > 0) {
					if (strlen(cStr3) > iTempX) iTempX = strlen(cStr3);
					iTempY++;
				}
				//DIRECTX m_DDraw.DrawShadowBox(iItemDropPosX[ix][iy]+2, iItemDropPosY[ix][iy]+2, iItemDropPosX[ix][iy]+4+iTempX*11/5, iItemDropPosY[ix][iy]+4+iTempY*15);
				iTempY = 0;
				PutString(iItemDropPosX[ix][iy]+4, iItemDropPosY[ix][iy]+4, cStr1,video::SColor(255,250,0,0));
				if (strlen(cStr2) > 0) {
					PutString(iItemDropPosX[ix][iy]+6, iItemDropPosY[ix][iy]+4+15, cStr2,video::SColor(255,0,250,0));
					iTempY = 1;
				}
				if (strlen(cStr3) > 0) PutString(iItemDropPosX[ix][iy]+6, iItemDropPosY[ix][iy]+4+15+15*iTempY, cStr3,video::SColor(255,0,250,0));
			}
	}	}*/

	if (m_bIsGetPointingMode == true)
	{	if ( (m_iPointCommandType >= 100) && (m_iPointCommandType < 200) ) // spell
		{	if (m_bCommandAvailable == true)
			{	if( m_sMCX != 0 )
				{	if( _iGetFOE(iFocuiStatus) < 0 )
						m_stMCursor.sCursorFrame = 5;   // Red enemi for spell
					else m_stMCursor.sCursorFrame = 4;  // Blue friend for spell
				}else m_stMCursor.sCursorFrame = 4;     // Blue friend for spell
			}else m_stMCursor.sCursorFrame = 8;
		}else if ( (m_iPointCommandType >= 0) && (m_iPointCommandType < 50) ) // item
		{	m_stMCursor.sCursorFrame = 10;				// hand to grap item
	}	}
}

void CGame::GameRecvMsgHandler(uint32_t dwMsgSize, char * pData)
{
	uint32_t * dwpMsgID;
	dwpMsgID = (uint32_t *)(pData + INDEX4_MSGID);

	if(*dwpMsgID & MSGIDTYPE_MOTION)
	{
		MotionEventHandler(pData);
		return;
	}


	uint8_t v;

	switch (*dwpMsgID)
	{
	case MSGID_MODIFYTILE:
		ReceiveModifyTile(pData);
		break;

	case MSGID_RESPONSE_CHARGED_TELEPORT:
		ResponseChargedTeleport(pData);
		break;

	case MSGID_RESPONSE_TELEPORT_LIST:
		ResponseTeleportList(pData);
		break;

	case MSGID_RESPONSE_QUEST_LIST:
		ResponseQuestList(pData);
		break;

	case MSGID_RESPONSE_NOTICEMENT:
		NoticementHandler(pData);
		break;

	case MSGID_DYNAMICOBJECT:
		DynamicObjectHandler(pData);
		break;

	case MSGID_RESPONSE_INITPLAYER:
		InitPlayerResponseHandler(pData);
		break;

	case MSGID_RESPONSE_INITDATA:
		InitDataResponseHandler(pData);
		break;

	case MSGID_RESPONSE_MOTION:
		MotionResponseHandler(pData);
		break;

	case MSGID_EVENT_COMMON:
		CommonEventHandler(pData, dwMsgSize);
		break;
		
	case MSGID_COMMAND_CHATMSG:
		ChatMsgHandler(pData);
		break;

	case MSGID_PLAYERITEMLISTCONTENTS:
		InitItemList(pData);
		break;

	case MSGID_GUILDINFO:
		ReceiveGuildInfo(pData+4);
		break;

	case MSGID_GUILDSMANSTATUS:
		ReceiveGuildsmanStatus(pData+4);
		break;

	case MSGID_REQGUILDSUMMONS:
		pData += 4;
		Pop(pData, (uint8_t&)v);
		EnableDialogBox(49, 0, v, 0);
		break;

	case MSGID_RESPONSE_GUILDBANK:
		InitItemList_GuildBank(pData+4);
		break;
		
	case MSGID_RSP_GUILDBOARD:
		ReceiveGuildBoard(pData + 8);
		break;

	case MSGID_RSP_MAILBOX:
		ReceiveMailbox(pData + 8);
		break;
		
	case MSGID_RSP_MAILDATA:
		ReceiveMailData(pData + 12);
		break;

	case MSGID_RSP_GUILDBOARDPOST:
		ReceiveGuildBoardPost(pData + 8);
		break;

	case MSGID_RSP_RETRIEVEMAILITEM:
		m_mails[ m_dialogBoxes[DIALOG_MAILBOX].sV1 ]->
			items.erase(m_mails[ m_dialogBoxes[DIALOG_MAILBOX].sV1 ]->items.begin() + m_dialogBoxes[DIALOG_MAILBOX].sV2);

		if(m_mails[ m_dialogBoxes[DIALOG_MAILBOX].sV1 ]->items.size() == 0)
			m_dialogBoxes[DIALOG_MAILBOX].SetMode(2);
		else
			m_dialogBoxes[DIALOG_MAILBOX].SetMode(4);
		break;

	case MSGID_RSP_SENDMAIL:
		m_dialogBoxes[DIALOG_MAILBOX].bFlag = false;

		if(*(uint16_t*)(pData + 4) == MSGTYPE_CONFIRM)
		{
			AddEventList(NOTIFY_MSG_HANDLER80, CHAT_GM);

			std::vector<MailItemSend> & vvec = m_dialogBoxes[DIALOG_MAILBOX].vvec;
			for(std::vector<MailItemSend>::iterator it = vvec.begin();
				it != vvec.end();
				++it)
			{
				int index = (*it).index;
				int count = (*it).count;

				m_bIsItemDisabled[index] = false;
				m_pItemList[index]->m_dwCount -= count;

				if(m_pItemList[index]->m_dwCount == 0)
				{
					if(m_bIsItemEquipped[index])
					{
						m_sItemEquipmentStatus[	m_pItemList[index]->m_cEquipPos ] = -1;
						m_bIsItemEquipped[index] = false;
					}
					EraseItem(index);
				}
			}

			_iCalcTotalWeight();
			
			m_dialogBoxes[DIALOG_MAILBOX].SetMode(0);
		} else {
			sprintf(G_cTxt, NOTIFY_MSG_HANDLER81, m_dialogBoxes[62].cStr);
			AddEventList(G_cTxt, CHAT_GM);
		}
		break;

	case MSGID_NOTIFY:
		NotifyMsgHandler(pData);
		break;

	case MSGID_RESPONSE_CREATENEWGUILD:
		CreateNewGuildResponseHandler(pData);
		break;

	case MSGID_RESPONSE_DISBANDGUILD:
		DisbandGuildResponseHandler(pData);
		break;

	case MSGID_PLAYERCHARACTERCONTENTS:
		InitPlayerCharacteristics(pData);
		break;

	case MSGID_RESPONSE_CIVILRIGHT:
		CivilRightAdmissionHandler(pData);
		break;

	case MSGID_RESPONSE_RETRIEVEITEM:
		RetrieveItemHandler(pData);
		break;

	case MSGID_RESPONSE_RETRIEVEITEM_GUILDBANK:
		RetrieveItemHandler_GuildBank(pData+4);
		break;

	case MSGID_RESPONSE_PANNING:
		ResponsePanningHandler(pData);
		break;

	case MSGID_RESPONSE_FIGHTZONE_RESERVE:
		ReserveFightzoneResponseHandler(pData);
		break;
	}
}

void CGame::ConnectionEstablishHandler(char cWhere)
{
	ChangeGameMode(GAMEMODE_ONWAITINGRESPONSE);

	switch (cWhere) {
	case SERVERTYPE_GAME:
		bSendCommand(MSGID_REQUEST_INITPLAYER);
		break;

	case SERVERTYPE_LOG:
		switch (m_dwConnectMode) {
		case MSGID_REQUEST_LOGIN:
			SendLoginCommand(MSGID_REQUEST_LOGIN);
			break;
		case MSGID_REQUEST_CREATENEWACCOUNT:
			SendLoginCommand(MSGID_REQUEST_CREATENEWACCOUNT);
			break;
		case MSGID_REQUEST_CREATENEWCHARACTER:
			SendLoginCommand(MSGID_REQUEST_CREATENEWCHARACTER);
			break;
		case MSGID_REQUEST_ENTERGAME:
			SendLoginCommand(MSGID_REQUEST_ENTERGAME);
			break;
		case MSGID_REQUEST_DELETECHARACTER:
			SendLoginCommand(MSGID_REQUEST_DELETECHARACTER);
			break;
		case MSGID_REQUEST_CHANGEPASSWORD:
			SendLoginCommand(MSGID_REQUEST_CHANGEPASSWORD);
			break;
		case MSGID_REQUEST_INPUTKEYCODE:
			SendLoginCommand(MSGID_REQUEST_INPUTKEYCODE);
			break;
		}
		break;
	}
}

void CGame::InitPlayerResponseHandler(char * pData)
{
	uint16_t * wp;
	wp = (uint16_t *)(pData + INDEX2_MSGTYPE);
	switch (*wp) {
	case MSGTYPE_CONFIRM:
		//bSendCommand(MSGID_REQUEST_INITDATA);
		LoadMuteList();
		ChangeGameMode(GAMEMODE_ONWAITINGINITDATA);
		break;

	case MSGTYPE_REJECT:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		memset(m_cMsg, 0, sizeof(m_cMsg));
		strcpy(m_cMsg, "3J");
		break;
	}
}

void MakeSprite( wchar_t * FileName, int iStart, short sCount, bool bAlphaEffect )
{
	for( short i=0 ; i < sCount ; i++ )
	{
		G_pGame->m_pSprite[i+iStart] = CSprite::CreateSprite(FileName, i, bAlphaEffect);
	}
}
void MakeTileSpr( wchar_t * FileName, short sStart, short sCount, bool bAlphaEffect )
{
	for( short i=0 ; i < sCount ; i++ )
	{
		G_pGame->m_pTileSpr[i+sStart] = CSprite::CreateSprite(FileName, i, bAlphaEffect);
	}
}
void MakeEffectSpr( wchar_t * FileName, short sStart, short sCount, bool bAlphaEffect )
{
	for( short i=0 ; i < sCount ; i++ )
	{
		G_pGame->m_pEffectSpr[i+sStart] = CSprite::CreateSprite(FileName, i, bAlphaEffect);
	}
}

/*
void CGame::MakeSprite( Lchar* FileName, short sStart, short sCount, bool bAlphaEffect )
{
	int iTotalimage;
	uint32_t nCount;
	char PathName[50];
	std::vector<int> framePositions;

	wsprintfA( PathName, "sprites\\%s.pak", FileName );
	HANDLE //m_hPakFile = CreateFileA(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	ReadFramePositions(m_hPakFile, framePositions, sCount);
	if( m_hPakFile == INVALID_HANDLE_VALUE ) {	//return;
	//besk: the default pak is only useful if the client fills m_stFrame[mobnum][] with some nonzero values, 
		//		otherwise client will still crash when the mob appears.
		strcpy(PathName, "sprites\\SKE.pak");//besk add: default pak
		//m_hPakFile = CreateFileA(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
		if (m_hPakFile == INVALID_HANDLE_VALUE) return;
		char cTempFileName[64];
		strcpy(cTempFileName, "SKE");
		SetFilePointer(m_hPakFile, 20, NULL, FILE_BEGIN);
		ReadFile(m_hPakFile, (char *)&iTotalimage, 4, &nCount, NULL);

		for( short i=0 ; i < sCount && i < iTotalimage; i++ ){
			if( i < iTotalimage ) m_pSprite[i+sStart] = CSprite::CreateSprite(LcTempFileName, i, bAlphaEffect);
			//m_pSprite[i+sStart] = CSprite::CreateSprite(LcTempFileName, i, bAlphaEffect);
		}
		//CloseHandle(m_hPakFile);
	} else {
	SetFilePointer(m_hPakFile, 20, NULL, FILE_BEGIN);
	ReadFile(m_hPakFile, (char *)&iTotalimage, 4, &nCount, NULL);
	for( short i=0 ; i < sCount && i < iTotalimage; i++ ) {
		m_pSprite[i+sStart] = CSprite::CreateSprite(LFileName, i, bAlphaEffect);
	}
	//CloseHandle(m_hPakFile);
	}
}

void CGame::MakeLegionSprite( char* FileName, short sStart, short sCount, bool bAlphaEffect )
{
	int iTotalimage;
	uint32_t nCount;
	char PathName[50];
	std::vector<int> framePositions;

	wsprintfA( PathName, "sprites\\%s.lpk", FileName );
	HANDLE //m_hPakFile = CreateFileA(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( m_hPakFile == INVALID_HANDLE_VALUE ) return;
	ReadFramePositions(m_hPakFile, framePositions, sCount);
	SetFilePointer(m_hPakFile, 20, NULL, FILE_BEGIN);
	ReadFile(m_hPakFile, (char *)&iTotalimage, 4, &nCount, NULL);
	iTotalimage ^= 712345;
	for( short i=0 ; i < sCount && i < iTotalimage; i++ ) {
		m_pSprite[i+sStart] = CSprite::CreateSprite(LFileName, i, bAlphaEffect);
	}
	//CloseHandle(m_hPakFile);
}

void CGame::MakeTileSpr( Lchar* FileName, short sStart, short sCount, bool bAlphaEffect )
{
	int iTotalimage;
	uint32_t nCount;
	char PathName[28];
	std::vector<int> framePositions;

	wsprintfA( PathName, "sprites\\%s.pak", FileName );
	HANDLE //m_hPakFile = CreateFileA(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( m_hPakFile == INVALID_HANDLE_VALUE ) return;
	ReadFramePositions(m_hPakFile, framePositions, sCount);
	SetFilePointer(m_hPakFile, 20, NULL, FILE_BEGIN);
	ReadFile(m_hPakFile, (char *)&iTotalimage, 4, &nCount, NULL);
	for( short i=0 ; i < sCount && i < iTotalimage; i++ ) {
		m_pTileSpr[i+sStart] = CSprite::CreateSprite(LFileName, i, bAlphaEffect);
	}
	//CloseHandle(m_hPakFile);
}

void CGame::MakeLegionTileSpr( char* FileName, short sStart, short sCount, bool bAlphaEffect )
{
	int iTotalimage;
	uint32_t nCount;
	char PathName[28];
	std::vector<int> framePositions;

	wsprintfA( PathName, "sprites\\%s.lpk", FileName );
	HANDLE //m_hPakFile = CreateFileA(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( m_hPakFile == INVALID_HANDLE_VALUE ) return;
	ReadFramePositions(m_hPakFile, framePositions, sCount);
	SetFilePointer(m_hPakFile, 20, NULL, FILE_BEGIN);
	ReadFile(m_hPakFile, (char *)&iTotalimage, 4, &nCount, NULL);
	iTotalimage ^= 712345;
	for( short i=0 ; i < sCount && i < iTotalimage ; i++ ) {
		m_pTileSpr[i+sStart] = CSprite::CreateSprite(LFileName, i, bAlphaEffect);
	}
	//CloseHandle(m_hPakFile);
}

void CGame::MakeEffectSpr( Lchar* FileName, short sStart, short sCount, bool bAlphaEffect )
{
	int iTotalimage;
	uint32_t nCount;
	char PathName[28];
	std::vector<int> framePositions;

	wsprintfA( PathName, "sprites\\%s.pak", FileName );
	HANDLE //m_hPakFile = CreateFileA(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( m_hPakFile == INVALID_HANDLE_VALUE ) return;
	ReadFramePositions(m_hPakFile, framePositions, sCount);
	SetFilePointer(m_hPakFile, 20, NULL, FILE_BEGIN);
	ReadFile(m_hPakFile, (char *)&iTotalimage, 4, &nCount, NULL);
	for( short i=0 ; i < sCount ; i++ ) {
		if( i < iTotalimage ) m_pEffectSpr[i+sStart] = CSprite::CreateSprite(LFileName, i, bAlphaEffect);
	}
	//CloseHandle(m_hPakFile);
}

void CGame::MakeLegionEffectSpr( char* FileName, short sStart, short sCount, bool bAlphaEffect )
{
	int iTotalimage;
	uint32_t nCount;
	char PathName[28];
	std::vector<int> framePositions;

	wsprintfA( PathName, "sprites\\%s.lpk", FileName );
	HANDLE //m_hPakFile = CreateFileA(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( m_hPakFile == INVALID_HANDLE_VALUE ) return;
	ReadFramePositions(m_hPakFile, framePositions, sCount);
	SetFilePointer(m_hPakFile, 20, NULL, FILE_BEGIN);
	ReadFile(m_hPakFile, (char *)&iTotalimage, 4, &nCount, NULL);
	iTotalimage ^= 712345;
	for( short i=0 ; i < sCount ; i++ ) {
		if( i < iTotalimage ) m_pEffectSpr[i+sStart] = CSprite::CreateSprite(LFileName, i, bAlphaEffect);
	}
	//CloseHandle(m_hPakFile);
}
*/

void CGame::ProcessUI(shared_ptr<UIMsgQueueEntry> msg)
{
    WebString method_name = msg->obj.ToObject().GetProperty(WSLit("method_name")).ToString();
    JSArray args = msg->obj.ToObject().GetProperty(WSLit("args")).ToArray();



    if (method_name == WSLit("log"))
    {
        std::string buffer = "";
        int i = 0;
        while (i < args.size())
        {
            JSValue entry = args.At(i);
            buffer = buffer + Awesomium::ToString(entry.ToString()) + " ";
            i++;
        }
        printf("[JS] > %s\n", buffer.c_str());
        return;
    }
    else if (method_name == WSLit("renderCharacter"))
    {
        if (args.size() < 2)
        {
            htmlUI->Emit("renderCharacter", false, "Invalid data");
            return;
        }
        else
        {
            charselectx = args.At(0).ToInteger();
            charselecty = args.At(1).ToInteger();
        }
    }
    else if (method_name == WSLit("cancelLogin"))
    {
        if (_socket)
        {
            _socket->stop();
            _socket = nullptr;
            ChangeGameMode(GAMEMODE_ONLOGIN);
        }
    }
    else if (method_name == WSLit("login"))
    {
        if (args.size() < 2)
        {
            htmlUI->Emit("login", false, "Invalid login information");
            return;
        }
        else
        {
            WebString username = args.At(0).ToString();
            WebString password = args.At(1).ToString();

            if (username.IsEmpty() || password.IsEmpty())
            {
                htmlUI->Emit("login", false, "Username and password cannot be empty");
                return;
            }

            m_cAccountName = ToString(username);
            m_cAccountPassword = ToString(password);
            StartLogin();
            ChangeGameMode(GAMEMODE_ONCONNECTING);
            m_dwConnectMode = MSGID_REQUEST_LOGIN;
            return;
        }
    }
    else if (method_name == WSLit("selectcharacter"))
    {
        if (args.size() == 0)
        {
            htmlUI->Emit("selectcharacter", false, "Invalid character");
            return;
        }
        else
        {
            int16_t selectedchar = static_cast<int16_t>(args.At(0).ToInteger());

            if (m_pCharList.size() > selectedchar)
            {
                m_cCurFocus = selectedchar;
                htmlUI->Emit("selectcharacter", true, "");
            }
            return;
        }
    }
    else if (method_name == WSLit("entergame"))
    {
        if (m_pCharList[m_cCurFocus] != 0)
        {
            if (m_pCharList[m_cCurFocus]->m_sSex != 0)
            {
                ZeroMemory(m_cPlayerName, sizeof(m_cPlayerName));
                strcpy(m_cPlayerName, m_pCharList[m_cCurFocus]->m_cName.c_str());
                m_iLevel = (int)m_pCharList[m_cCurFocus]->m_sLevel;
                if (m_Misc.bCheckValidString(m_cPlayerName) == true)
                {
                    m_pSprite[SPRID_INTERFACE_ND_LOGIN]->_iCloseSprite();
                    m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->_iCloseSprite();
                    ChangeGameMode(GAMEMODE_ONCONNECTING);
                    m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                    m_wEnterGameType = ENTERGAMEMSGTYPE_NEW;
                    ZeroMemory(m_cMsg, sizeof(G_pGame->m_cMsg));
                    strcpy(m_cMsg, "33");
                    ZeroMemory(m_cMapName, sizeof(m_cMapName));
                    memcpy(m_cMapName, m_pCharList[m_cCurFocus]->m_cMapName.c_str(), 10);
                    htmlUI->Emit("entergame", true, "");
                    return;
                }
                htmlUI->Emit("entergame", false, "Invalid character name");
                return;
            }
        }
    }
}

void CGame::OnTimer()
{
	if( m_cGameMode < 0 ) return;
	uint64_t dwTime = unixtime();


    {
        std::lock_guard<std::mutex> lock(uimut);
        while (uiqueue.size() > 0)
        {
            shared_ptr<UIMsgQueueEntry> entry = GetUIMsgQueue();
            ProcessUI(entry);
        }
    }
    {
        std::lock_guard<std::mutex> lock(socketmut);
        while (loginpipe.size() > 0)
        {
            shared_ptr<CGame::MsgQueueEntry> entry = GetLoginMsgQueue();
            LogResponseHandler(entry->size, entry->data);
        }
    }
    {
        std::lock_guard<std::mutex> lock(socketmut);
        while (socketpipe.size() > 0)
        {
            shared_ptr<CGame::MsgQueueEntry> entry = GetMsgQueue();
            GameRecvMsgHandler(entry->size, entry->data);
        }
    }

	if (m_cGameMode != GAMEMODE_ONLOADING)
	{
		if (((int64_t)dwTime - m_dwCheckSprTime) > 8000 )
		{
			m_dwCheckSprTime = dwTime;
			if( m_bIsProgramActive ) ReleaseUnusedSprites();
			if (_socket != nullptr && loggedin)
				bSendCommand(MSGID_COMMAND_CHECKCONNECTION, MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
		}
	}

	if (m_cGameMode == GAMEMODE_ONMAINGAME)
	{
		if ((dwTime - m_dwCheckConnTime) > 5000)
		{
			m_dwCheckConnTime = dwTime;
			if ((m_bIsCrusadeMode) && (m_iCrusadeDuty == 0)) EnableDialogBox(33, 1, 0, 0);
		}

		if (m_ekSSTime != 0 && dwTime > m_ekSSTime){
			m_ekSSTime = 0;
			CreateScreenShot();
		}


		if ((dwTime - m_dwCheckChatTime) > 2000)
		{
//#ifndef _DEBUG
// 			if( CheckCheating() /*|| CheckHackProgram()*/ )
// 				{
// 				MessageBoxA(*(HWND*)m_hWnd, "Error Code: 1600\n\nClient.exe has detected an illegal program or modification.\n\nGame Closing.", "Hack detected!", MB_OK | MB_ICONERROR);
// 				ChangeGameMode(GAMEMODE_ONQUIT);
// 				_socket->stop();
// 				m_bEscPressed = false;
// 				PlaySound('E', 14, 5);
// // 				if (m_bSoundFlag) m_pESound[38]->bStop();
// // 				if ((m_bSoundFlag) && (m_bMusicStat == TRUE))
// // 				{
// // 					if (m_pBGM != NULL) m_pBGM->bStop();
// // 				}//DIRECTX
// 				exit(1600);
// 			}
//#endif

			m_dwCheckChatTime = dwTime;
			ReleaseTimeoverChatMsg();
			if (m_cCommandCount >= 6)
			{
				m_iNetLagCount++;
				if (m_iNetLagCount >= 7)
				{
					ChangeGameMode(GAMEMODE_ONCONNECTIONLOST);
					gamemode = 0;
					_socket->stop();
					return;
				}
			}
			else m_iNetLagCount = 0;
		}
	}
}

void CGame::_SetItemOrder(char cWhere, char cItemID)
{
 int i;

	switch (cWhere) {
	case 0:
		for (i = 0; i < MAXITEMS; i++)
		if (m_cItemOrder[i] == cItemID)
			m_cItemOrder[i] = -1;

		for (i = 1; i < MAXITEMS; i++)
		if ((m_cItemOrder[i-1] == -1) && (m_cItemOrder[i] != -1)) {
			m_cItemOrder[i-1] = m_cItemOrder[i];
			m_cItemOrder[i]   = -1;
		}

		for (i = 0; i < MAXITEMS; i++)
		if (m_cItemOrder[i] == -1) {
			m_cItemOrder[i] = cItemID;
			return;
		}
		break;
	}
}

bool CGame::_bCheckDraggingItemRelease(char dlgID)
{
	//char cDlgID = MouseOverDialog();

	if(dlgID)
	{
		EnableDialogBox(dlgID, 0, 0, 0);
		m_dialogBoxes[dlgID].OnDropItem();
			return true;
	}

	bItemDrop_ExternalScreen((char)m_stMCursor.sSelectedObjectID, G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
	return false;
}

void CGame::bItemDrop_ExternalScreen(char cItemID, short msX, short msY)
{
	char  cName[21];
	short sType, tX, tY;
	int iStatus, dlgType = 0;

	if (bCheckItemOperationEnabled(cItemID) == false) return;

	//if ((m_sMCX != 0) && (m_sMCY != 0) && (abs(m_sPlayerX - m_sMCX) <= 8) && (abs(m_sPlayerY - m_sMCY) <= 8))
	if ((m_sMCX != 0) && (m_sMCY != 0) && (abs(m_sPlayerX - m_sMCX) <= 11) && (abs(m_sPlayerY - m_sMCY) <= 9))//besk resolution
	{	
		memset(cName, 0, sizeof(cName));
		m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sType, &iStatus, &m_wCommObjectID);
		if (memcmp(m_cPlayerName, cName, 10) != 0)
		{	
			if (   ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_ARROW))
				&& (m_pItemList[cItemID]->m_dwCount > 1))
			{	
				m_dialogBoxes[17].m_X  = msX - 140;
				m_dialogBoxes[17].m_Y  = msY - 70;
				if (m_dialogBoxes[17].m_Y < 0) m_dialogBoxes[17].m_Y = 0;
				m_dialogBoxes[17].sV1 = m_sMCX;
				m_dialogBoxes[17].sV2 = m_sMCY;
				m_dialogBoxes[17].sV3 = sType;
				m_dialogBoxes[17].sV4 = m_wCommObjectID;
				memset(m_dialogBoxes[17].cStr, 0, sizeof(m_dialogBoxes[17].cStr));
				if (sType < 10)
					memcpy(m_dialogBoxes[17].cStr, cName, 10);
				else
				{	
					GetNpcName(sType, m_dialogBoxes[17].cStr);
				}
				EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
			}else
			{	
				switch (sType) {
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
					if(!dlgType) dlgType = 1;

				case 20: // Howard
					if(!dlgType) dlgType = 3;

				case 15: // ShopKeeper-W
		  		case 24: // Tom
					if(!dlgType) dlgType = 2;

				case NPC_PRINCESS: // Princess
					if(!dlgType) dlgType = 10;

					EnableDialogBox(20, dlgType, cItemID, sType);
					//m_dialogBoxes[20].//CentreOverPoint(msX, msY);
					m_dialogBoxes[20].sV3 = 1;
					m_dialogBoxes[20].sV4 = m_wCommObjectID; 
					m_dialogBoxes[20].sV5 = m_sMCX;
					m_dialogBoxes[20].sV6 = m_sMCY;

					memset(m_dialogBoxes[20].cStr, 0, sizeof(m_dialogBoxes[20].cStr));
					GetNpcName(sType, m_dialogBoxes[20].cStr);
					break;

				default:
					bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, cItemID, 1, m_sMCX, m_sMCY, m_pItemList[cItemID]->m_cName);
					break;
				}
				//m_bIsItemDisabled[cItemID] = TRUE;
			}
			m_bIsItemDisabled[cItemID] = true;
		}
	}else
	{	
		if (   ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_ARROW)) 
			&& (m_pItemList[cItemID]->m_dwCount > 1))
		{	
			m_dialogBoxes[17].m_X  = msX - 140;
			m_dialogBoxes[17].m_Y  = msY - 70;
			if (m_dialogBoxes[17].m_Y < 0)		m_dialogBoxes[17].m_Y = 0;
			m_dialogBoxes[17].sV1 = 0;
			m_dialogBoxes[17].sV2 = 0;
			m_dialogBoxes[17].sV3 = 0;
			m_dialogBoxes[17].sV4 = 0;
			memset(m_dialogBoxes[17].cStr, 0, sizeof(m_dialogBoxes[17].cStr));
			EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
		}else
		{	
			if(_ItemDropHistory(m_pItemList[cItemID]->m_cName))
			{	
				m_dialogBoxes[4].m_X  = msX - 140;
				m_dialogBoxes[4].m_Y  = msY - 70;
				if (m_dialogBoxes[4].m_Y < 0)	m_dialogBoxes[4].m_Y = 0;
    			m_dialogBoxes[4].sV1 = 0;
				m_dialogBoxes[4].sV2 = 0;
				m_dialogBoxes[4].sV3 = 1;
				m_dialogBoxes[4].sV4 = 0;
				m_dialogBoxes[4].sV5 = cItemID;
				memset(m_dialogBoxes[4].cStr, 0, sizeof(m_dialogBoxes[4].cStr));
				EnableDialogBox(4, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
			}
			else
			{	
				bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_ITEMDROP, 0, cItemID, 1, 0, m_pItemList[cItemID]->m_cName);
			}	
		}
		m_bIsItemDisabled[cItemID] = true;
	}
}

void CGame::CommonEventHandler(char * pData, uint32_t size)
{
	uint16_t wEventType;
	short sX, sY;
	int32_t sV1, sV2, sV3, sV4;

	StreamRead sr = StreamRead(pData, size);

	sr.ReadInt();
	wEventType = sr.ReadShort();

	sX = sr.ReadShort();
	sY = sr.ReadShort();
	sV1 = sr.ReadInt();
	sV2 = sr.ReadInt();
	sV3 = sr.ReadInt();
	sV4 = sr.ReadInt();

	switch (wEventType) {
	case COMMONTYPE_ITEMDROP:
		if ((sV1 == 6) && (sV2 == 0)) {
			bAddNewEffect(4, sX, sY, 0, 0, 0);
		}
		m_pMapData->bSetItem(sX, sY, sV1, sV2, (char)sV3); // 1234 Added NULL LIEs.. wrong!
		break;

	case COMMONTYPE_SETITEM:
		m_pMapData->bSetItem(sX, sY, sV1, sV2, (char)sV3, false);
		break;

	case COMMONTYPE_MAGIC:
		bAddNewEffect(sV3, sX, sY, sV1, sV2, 0, sV4);
		break;

	case COMMONTYPE_CLEARGUILDNAME:
		ClearGuildNameList();
		break;
	}
}

void CGame::ClearGuildNameList()
{
	for (int i = 0; i < MAXGUILDNAMES; i++) {
		m_stGuildName[i].dwRefTime = 0;
		m_stGuildName[i].iGuildRank = -1;
		memset(m_stGuildName[i].cCharName, 0, sizeof(m_stGuildName[i].cCharName));
		memset(m_stGuildName[i].cGuildName, 0, sizeof(m_stGuildName[i].cGuildName));
	}
}

void CGame::InitGameSettings()
{
	int i;

	m_bHackMoveBlocked = false; // Anti Hack xRisenx

	m_adminLevel = 0;
	m_GMFlags = 0;
	m_eventPot = -1;

	m_bIllusionMVT = false;
	m_bForceAttack = false;
	m_dwCommandTime = 0;

	m_bInputStatus = false;
	m_pInputBuffer = 0;

	m_iPDBGSdivX = 0;
	m_iPDBGSdivY = 0;
	m_bIsRedrawPDBGS = true;

	m_iCameraShakingDegree = 0;

	m_cCommand = OBJECTSTOP;
	m_cCommandCount = 0;

	m_bIsGetPointingMode = false;
	m_iPointCommandType  = -1;

	m_bIsCombatMode = false;
	m_bRunningMode = false;

	m_stMCursor.cPrevStatus       = CURSORSTATUS_NULL;
	m_stMCursor.dwSelectClickTime = 0;

	m_bSkillUsingStatus = false;
	m_bItemUsingStatus  = false;
	m_bUsingSlate = false;

	m_weather = WEATHER_SUNNY;

	m_iDownSkillIndex = -1;
	m_dialogBoxes[15].bFlag = false;

	m_bIsConfusion = false;

	m_iIlusionOwnerH = 0;
	m_cIlusionOwnerType = 0;

	m_iDrawFlag = 0;
	m_bDrawFlagDir = false;
	m_bIsAstoriaMode = false;
	m_relicOwnedTime = 0;
	for(int i=0; i < MAXSIDES; i++)
	{
		m_astoriaStats[i].deaths = 0;
		m_astoriaStats[i].kills = 0;
	}
	for(int i=0; i < MAXSIDES; i++)
	{
		m_astoriaShieldsHP[i] = 0;
		m_astoriaShieldsHP[i] = 0;
	}
	m_bIsCrusadeMode = false;
	m_iCrusadeDuty = 0;
	m_bIsAvatarMode  = false;
	m_bIsAvatarMessenger = false;

	m_iNetLagCount = 0;

	m_dwEnvEffectTime = unixtime();

	for (i = 0; i < MAXGUILDNAMES; i++) {
		m_stGuildName[i].dwRefTime = 0;
		m_stGuildName[i].iGuildRank = -1;
		memset(m_stGuildName[i].cCharName, 0, sizeof(m_stGuildName[i].cCharName));
		memset(m_stGuildName[i].cGuildName, 0, sizeof(m_stGuildName[i].cGuildName));
	}

	m_guildBankVer = 0;

#ifdef TitleClient
	for (i = 0; i < MAXGUILDNAMES; i++) {
		m_stTitles[i].dwRefTime = 0;
		m_stTitles[i].iCurrentNo = -1;
		m_stTitles[i].iRank = 0;
		memset(m_stTitles[i].cCharName, 0, sizeof(m_stTitles[i].cCharName));
		memset(m_stTitles[i].cSubject, 0, sizeof(m_stTitles[i].cSubject));
		strcpy(m_stTitles[i].cCharName, "NONE");
		strcpy(m_stTitles[i].cSubject, "NONE");
	}
	for (i = 0; i < MAXMENUITEMS; i++) {
		if (m_pTitles[i] != 0) delete m_pTitles[i];
		m_pTitles[i] = 0;
	}
	m_iTitleIndex = -1;
#endif

	//Snoopy: 61
	for (i = 0; i < MAXDIALOGBOX; i++)
		m_bIsDialogEnabled[i] = false;

	for (i = 0; i < MAXDIALOGBOX - 2; i++)
		m_cDialogBoxOrder[i] = 0;

	for (i = 0; i < MAXEFFECTS; i++) {
		if (m_pEffectList[i] != 0) delete m_pEffectList[i];
		m_pEffectList[i] = 0;
	}

	for (i = 0; i < MAXCHATMSGS; i++)
	{
		if (m_pChatMsgList[i])
		{
			delete m_pChatMsgList[i];
		}
		m_pChatMsgList[i] = 0;
	}

	for(int i = 0; i < CHAT_MAX; i++)
	{
		for (std::vector<CMsg*>::iterator it = m_chatMsgs[i].begin();
			it != m_chatMsgs[i].end();
			++it)
		{
			delete (*it);
		}
		m_chatMsgs[i].clear();
	}
	if(m_chatDisplay)
		delete m_chatDisplay;

	m_chatDisplay = new std::vector<CMsg*>;

	for(int i = 0; i < CHAT_MAX; i++)
	{
		m_chatToggle[i] = true;
	}

	for (i = 0; i < MAXWHISPERMSG; i++) {
		if (m_pWhisperMsg[i] != 0) delete m_pWhisperMsg[i];
		m_pWhisperMsg[i] = 0;
	}

	ZeroMemory(m_cLocation, sizeof(m_cLocation));

	ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
	m_iGuildRank = GUILDRANK_NONE;
	m_iTotalGuildsMan = 0;
	m_guild.Reset();

	for (i = 0; i < 100; i++) {
		m_stGuildOpList[i].cOpMode = 0;
		ZeroMemory(m_stGuildOpList[i].cName, sizeof(m_stGuildOpList[i].cName));
	}

	for (i = 0; i < 6; i++) {
		ZeroMemory(m_stEventHistory[i].cTxt, sizeof(m_stEventHistory[i].cTxt));
		m_stEventHistory[i].dwTime = G_dwGlobalTime;

		ZeroMemory(m_stEventHistory2[i].cTxt, sizeof(m_stEventHistory2[i].cTxt));
		m_stEventHistory2[i].dwTime = G_dwGlobalTime;
	}

    for (i = 0; i < MAXMENUITEMS; i++) {
		if (m_pItemForSaleList[i] != 0) delete m_pItemForSaleList[i];
		m_pItemForSaleList[i] = 0;
	}

	for (i = 0; i<MAXITEMSTATS; i++)
	{
		if(m_pBuildItemListStats[i] != 0) delete m_pBuildItemListStats[i];
		m_pBuildItemListStats[i] = 0;
	}

	for (i = 0; i < 61; i++) {
		m_dialogBoxes[i].bFlag = false;
		m_dialogBoxes[i].sView = 0;
		m_dialogBoxes[i].bIsScrollSelected = false;
	}

	for (i = 0; i < MAXITEMS; i++)
	if (m_pItemList[i] != 0) {
		delete m_pItemList[i];
		m_pItemList[i] = 0;
	}

	for (i = 0; i < MAXSELLLIST; i++) {
		m_stSellItemList[i].iIndex = -1;
		m_stSellItemList[i].iAmount = 0;
	}

	for (i = 0; i < MAXBANKITEMS; i++)
	if (m_pBankList[i] != 0) {
		delete m_pBankList[i];
		m_pBankList[i] = 0;
	}
	
	m_mails.clear();
	m_gldBoard.clear();

	m_guildBankMap.clear();
	m_guildBankIt = m_guildBankMap.end();

	for (i = 0; i < MAXMAGICTYPE; i++)
		m_cMagicMastery[i] = 0;

	for (i = 0; i < MAXSKILLTYPE; i++)
		m_cSkillMastery[i] = 0;

	for (i = 0; i < TEXTDLGMAXLINES; i++) {
		if (m_pMsgTextList[i] != 0)
			delete m_pMsgTextList[i];
		m_pMsgTextList[i] = 0;

		if (m_pMsgTextList2[i] != 0)
			delete m_pMsgTextList2[i];
		m_pMsgTextList2[i] = 0;

		if (m_pAgreeMsgTextList[i] != 0)
			delete m_pAgreeMsgTextList[i];
		m_pAgreeMsgTextList[i] = 0;
	}

	ClearPartyMembers();
	m_stPartyMember.reserve(MAXPARTYMEMBERS);

	m_iLU_Point = 0;
	m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;
	m_bDialogTrans   = false;
	m_cLogOutCount = -1;
	m_dwLogOutCountTime = 0;
	m_iSuperAttackLeft = 0;
	m_bSuperAttackMode = false;
	m_iFightzoneNumber = 0 ;
	ZeroMemory(m_cBGMmapName, sizeof(m_cBGMmapName));
	m_dwWOFtime = 0;
	m_stQuest.sWho          = 0;
	m_stQuest.sQuestType    = 0;
	m_stQuest.sContribution = 0;
	m_stQuest.sTargetType   = 0;
	m_stQuest.sTargetCount  = 0;
	m_stQuest.sCurrentCount = 0;
	m_stQuest.sX            = 0;
	m_stQuest.sY            = 0;
	m_stQuest.sRange        = 0;
	m_stQuest.bIsQuestCompleted = false;
	ZeroMemory(m_stQuest.cTargetName, sizeof(m_stQuest.cTargetName));
	m_bIsObserverMode = false;
	m_bIsObserverCommanded = false;
	m_bIsPoisoned = false;
	m_bIsPrevMoveBlocked = false;
	m_iPrevMoveX = m_iPrevMoveY = -1;
	m_sDamageMove = 0;
	#ifdef RaiseCriticalLimit
	m_iDamageMoveAmount = 0; // Removed Critical Limit xRisenx
	#else
	m_sDamageMoveAmount = 0;
	#endif
	m_bForceDisconn = false;
	m_bIsSpecialAbilityEnabled = false;
	m_iSpecialAbilityType = 0;
	m_dwSpecialAbilitySettingTime = 0;
	m_iSpecialAbilityTimeLeftSec = 0;
	m_stMCursor.cSelectedObjectType = 0;
	m_bIsF1HelpWindowEnabled = false;
	m_bIsTeleportRequested = false;
	for (i = 0; i < MAXCRUSADESTRUCTURES; i++)
	{	m_stCrusadeStructureInfo[i].cType = 0;
		m_stCrusadeStructureInfo[i].cSide = 0;
		m_stCrusadeStructureInfo[i].sX = 0;
		m_stCrusadeStructureInfo[i].sY = 0;
	}
	ZeroMemory(m_cStatusMapName, sizeof(m_cStatusMapName));
	m_dwCommanderCommandRequestedTime = 0;
	ZeroMemory(m_cTopMsg, sizeof(m_cTopMsg));
	m_iTopMsgLastSec = 0;
	m_dwTopMsgTime   = 0;
	m_iConstructionPoint = 0;
	m_iWarContribution   = 0;
	ZeroMemory(m_cTeleportMapName, sizeof(m_cTeleportMapName));
	m_iTeleportLocX = m_iTeleportLocY = -1;
	ZeroMemory(m_cConstructMapName, sizeof(m_cConstructMapName));
	m_iConstructLocX = m_iConstructLocY = -1;

	//Snoopy: Apocalypse Gate
	ZeroMemory(m_cGateMapName, sizeof(m_cGateMapName));
	m_iGatePositX = m_iGatePositY = -1;

	m_bIsHeldenianMode 				= false;
	m_bIsHeldenianMap 				= false;
	m_iHeldenianAresdenLeftTower	= -1;
	m_iHeldenianElvineLeftTower		= -1;
	m_iHeldenianAresdenFlags		= -1;
	m_iHeldenianElvineFlags			= -1;
	m_iHeldenianAresdenDead			= -1;
	m_iHeldenianElvineDead			= -1;
	m_iHeldenianAresdenKill			= -1;

	m_iPartyStatus = 0;

	m_iGizonItemUpgradeLeft = 0;
	// Stats System xRisenx
	/*for (i=0; i<TOTALLEVELUPPOINT; i++) {
		m_cStatChange[i] =0;
	}*/

	EnableDialogBox(9, 0, 0, 0 );

	m_bSoccerMode			= false;
	m_SoccerAresdenGoals	= 0;
	m_SoccerElvineGoals		= 0;
}

/*
void CGame::_GetHairColorRGB(int iColorType, int * pR, int * pG, int * pB)
{	switch (iColorType) {
	case 0: // rouge fonc�
		*pR = 14; *pG = -5; *pB = -5; break;
	case 1: // Orange
		*pR = 20; *pG = 0; *pB = 0; break;
	case 2: // marron tres clair
		*pR = 22; *pG = 13; *pB = -10; break;
	case 3:
		*pR = 0; *pG = 10; *pB = 0; break;
	case 4: // Bleu flashy
		*pR = 0; *pG = 0; *pB = 22; break;
	case 5: // Bleu fonc�
		*pR = -5; *pG = -5; *pB = 15; break;
	case 6: //Mauve
		*pR = 15; *pG = -5; *pB = 16; break;
	case 7: // Noir
		*pR = -6; *pG = -6; *pB = -6; break;
	case 8:
		*pR = 10; *pG = 3; *pB = 10; break;
	case 9:
		*pR = 10; *pG = 3; *pB = -10; break;
	case 10:
		*pR = -10; *pG = 3; *pB = 10; break;
	case 11:
		*pR = 10; *pG = 3; *pB = 20; break;
	case 12:
		*pR = 21; *pG = 3; *pB = 3; break;
	case 13:
		*pR = 3; *pG = 3; *pB = 25; break;
	case 14:
		*pR = 3; *pG = 11; *pB = 3; break;
	case 15:
		*pR = 6; *pG = 8; *pB = 0; break;
	}
}*/
void CGame::_GetHairColorRGB(int iColorType, int * pR, int * pG, int * pB)//TODO: proper hair colors
{
	switch (iColorType) {
	case 0: // Dark Red
		*pR = 127; *pG = 0; *pB = 0; break;
	case 1: // Orange
		*pR = 255; *pG = 127; *pB = 0; break;
	case 2: // Light Brown
		*pR = 22; *pG = 13; *pB = -10; break;
	case 3:
		*pR = 0; *pG = 10; *pB = 0; break;
	case 4: // Bleu flashy
		*pR = 0; *pG = 0; *pB = 22; break;
	case 5: // Bleu fonc�
		*pR = -5; *pG = -5; *pB = 15; break;
	case 6: //Mauve
		*pR = 15; *pG = -5; *pB = 16; break;
	case 7: // Noir
		*pR = -6; *pG = -6; *pB = -6; break;
	case 8:
		*pR = 10; *pG = 3; *pB = 10; break;
	case 9:
		*pR = 10; *pG = 3; *pB = -10; break;
	case 10:
		*pR = -10; *pG = 3; *pB = 10; break;
	case 11:
		*pR = 10; *pG = 3; *pB = 20; break;
	case 12:
		*pR = 21; *pG = 3; *pB = 3; break;
	case 13:
		*pR = 3; *pG = 3; *pB = 25; break;
	case 14:
		*pR = 3; *pG = 11; *pB = 3; break;
	case 15:
		*pR = 6; *pG = 8; *pB = 0; break;
	}
}

void CGame::CreateNewGuildResponseHandler(char * pData)
{
	uint16_t * wpResult;
 	wpResult = (uint16_t *)(pData + INDEX2_MSGTYPE);

	switch (*wpResult) {
	case MSGTYPE_CONFIRM:
		m_iGuildRank = GUILDRANK_MASTER;
		m_dialogBoxes[7].SetMode(3);
		m_dialogBoxes[7].SetMode(3);
		break;
	case MSGTYPE_REJECT:
		m_iGuildRank = GUILDRANK_NONE;
		m_dialogBoxes[7].SetMode(4);
		break;
	}
}

void CGame::InitPlayerCharacteristics(char * pData)
{
 int  * ip;
 char * cp;
 uint16_t * wp;

	for(int i=0;i < 6; i++)
		m_angelStat[i] = 0;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	ip   = (int *)cp;
	m_iHP = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_iMP = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_iSP = *ip;
	cp += 4;

	/*ip   = (int *)cp;
	m_iAC = *ip;
	cp += 4;*/

	ip   = (int *)cp;
	m_iDefenseRatio = *ip; // Defense Display in Action Bar xRisenx
	cp += 4;

	ip   = (int *)cp;
	m_iTHAC0 = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_iLevel = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_createStat[STAT_STR] = m_stat[STAT_STR] = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_createStat[STAT_INT] = m_stat[STAT_INT] = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_createStat[STAT_VIT] = m_stat[STAT_VIT] = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_createStat[STAT_DEX] = m_stat[STAT_DEX] = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_createStat[STAT_MAG] = m_stat[STAT_MAG] = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_createStat[STAT_CHR] = m_stat[STAT_CHR] = *ip;
	cp += 4;

	wp   = (uint16_t *)cp;
	m_iLU_Point = (*wp - 3); // Comment out - 3 ? xRisenx
	cp += 2; // Change to 7? xRisenx

	m_iExp = *(uint64_t *)cp;
	cp += 8;

	ip   = (int *)cp;
	m_iEnemyKillCount = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_iPKCount = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_iRewardGold = *ip;
	cp += 4;

	memcpy(m_cLocation, cp, 10);
	cp += 10;

	if (memcmp(m_cLocation, "are", 3) == 0)
		m_side = ARESDEN;
	else if (memcmp(m_cLocation, "elv", 3) == 0)
		m_side = ELVINE;
	else if (memcmp(m_cLocation, "ist", 3) == 0)
		m_side = ISTRIA;
	else
		m_side = NEUTRAL;

	cp = (char *)cp;
	memcpy(m_cGuildName, cp, 20);
	cp += 20;

	if (strcmp(m_cGuildName, "NONE") == 0)
		ZeroMemory(m_cGuildName, sizeof(m_cGuildName));

	m_Misc.ReplaceString(m_cGuildName, '_', ' ');

	ip   = (int *)cp;
	m_iGuildRank = (GuildRank)*ip;
	cp += 4;

	m_iSuperAttackLeft = (int)*cp;
	cp++;

	ip   = (int *)cp;
	m_iFightzoneNumber = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_iEnemyKillTotalCount = *ip;
	cp += 4;
}

void CGame::DisbandGuildResponseHandler(char * pData)
{
	uint16_t * wpResult;
 	wpResult = (uint16_t *)(pData + INDEX2_MSGTYPE);

	switch (*wpResult) {
	case MSGTYPE_CONFIRM:
		ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
		m_iGuildRank = GUILDRANK_NONE;
		m_guild.Reset();
		DisableDialogBox(57);
		m_dialogBoxes[7].SetMode(7);
		break;
	case MSGTYPE_REJECT:
		m_dialogBoxes[7].SetMode(8);
		break;
	}
}

void CGame::_PutGuildOperationList(char * pName, char cOpMode)
{int i;
	for (i = 0; i < 100; i++)
	if (m_stGuildOpList[i].cOpMode == 0)
	{	m_stGuildOpList[i].cOpMode = cOpMode;
		ZeroMemory(m_stGuildOpList[i].cName, sizeof(m_stGuildOpList[i].cName));
		memcpy(m_stGuildOpList[i].cName, pName, 20);
		return;
	}
}

void CGame::_ShiftGuildOperationList()
{int i;
	ZeroMemory(m_stGuildOpList[0].cName ,sizeof(m_stGuildOpList[0].cName));
	m_stGuildOpList[0].cOpMode = 0;

	for (i = 1; i < 100; i++)
	if ((m_stGuildOpList[i-1].cOpMode == 0) && (m_stGuildOpList[i].cOpMode != 0)) {
		m_stGuildOpList[i-1].cOpMode = m_stGuildOpList[i].cOpMode;
		ZeroMemory(m_stGuildOpList[i-1].cName, sizeof(m_stGuildOpList[i-1].cName));
		memcpy(m_stGuildOpList[i-1].cName, m_stGuildOpList[i].cName, 20);

		ZeroMemory(m_stGuildOpList[i].cName ,sizeof(m_stGuildOpList[i].cName));
		m_stGuildOpList[i].cOpMode = 0;
	}
}

void CGame::SetItemCount(char * pItemName, uint32_t dwCount)
{
	char cTmpName[21];

	ZeroMemory(cTmpName, sizeof(cTmpName));
	strcpy(cTmpName, pItemName);

	for(int i = 0; i < MAXITEMS; i++)
	{
		if ((m_pItemList[i] != 0) && (memcmp(m_pItemList[i]->m_cName, cTmpName, 20) == 0))
		{	
			m_pItemList[i]->m_dwCount = dwCount;
			return;
		}
	}
}

uint32_t CGame::GetItemCount(char * pItemName)
{
	char cTmpName[21];

	ZeroMemory(cTmpName, sizeof(cTmpName));
	strcpy(cTmpName, pItemName);

	for(int i = 0; i < MAXITEMS; i++)
	{
		if ((m_pItemList[i] != 0) && (memcmp(m_pItemList[i]->m_cName, cTmpName, 20) == 0))
		{	
			return m_pItemList[i]->m_dwCount;
		}
	}

	return 0;
}

void CGame::AddEventList(char const * const pTxt, char cColor, bool bDupAllow)
{
	int i;
	if ((bDupAllow == false) && (strcmp(m_stEventHistory[5].cTxt, pTxt) == 0)) return;
	if (cColor == 10)
	{
		for (i = 1; i < 6; i++)
		{
			strcpy(m_stEventHistory2[i-1].cTxt, m_stEventHistory2[i].cTxt);
			m_stEventHistory2[i-1].cColor = m_stEventHistory2[i].cColor;
			m_stEventHistory2[i-1].dwTime = m_stEventHistory2[i].dwTime;
		}
		ZeroMemory(m_stEventHistory2[5].cTxt, sizeof(m_stEventHistory2[5].cTxt));
		strcpy(m_stEventHistory2[5].cTxt, pTxt);
		m_stEventHistory2[5].cColor = cColor;
		m_stEventHistory2[5].dwTime = m_dwCurTime;
	}
	else
	{
		for (i = 1; i < 6; i++)
		{
			strcpy(m_stEventHistory[i-1].cTxt, m_stEventHistory[i].cTxt);
			m_stEventHistory[i-1].cColor = m_stEventHistory[i].cColor;
			m_stEventHistory[i-1].dwTime = m_stEventHistory[i].dwTime;
		}
		ZeroMemory(m_stEventHistory[5].cTxt, sizeof(m_stEventHistory[5].cTxt));
		strcpy(m_stEventHistory[5].cTxt, pTxt);
		m_stEventHistory[5].cColor = cColor;
		m_stEventHistory[5].dwTime = m_dwCurTime;
	}
}

int _iAttackerHeight[] = {0, 35, 35,35,35,35,35, 0,0,0,
5,  // Slime
35, // Skeleton
40, // Stone-Golem
45, // Cyclops
35,// OrcMage
35,// ShopKeeper
5, // GiantAnt
8, // Scorpion
35,// Zombie
35,// Gandalf
35,// Howard
35,// Guard
10,// Amphis
38,// Clay-Golem
35,// Tom
35,// William
35,// Kennedy
35,// Hellhound
50,// Troll
45,// Orge
55,// Liche
65,// Demon
46,// Unicorn
49,// WereWolf
55,// Dummy
35,// Energysphere
75,// Arrow Guard Tower
75,// Cannon Guard Tower
50,// Mana Collector
50,// Detector
50,// Energy Shield Generator
50,// Grand Magic Generator
50,// ManaStone 42
40,// Light War Beetle
35,// GHK
40,// GHKABS
35,// TK
60,// BG
40,// Stalker
70,// HellClaw
85,// Tigerworm
50,// Catapult
85,// Gargoyle
70,// Beholder
40,// Dark-Elf
20,// Bunny
20,// Cat
40,// Giant-Frog
80,// Mountain-Giant
85,// Ettin
50,// Cannibal-Plant
50, // Rudolph 61 //Snoopy....
80, // Direboar 62
90, // Frost 63
40, // Crops 64
80, // IceGolem 65
190, // Wyvern 66
35, // npc 67
35, // npc 68
35, // npc 69
100, // Dragon 70
90, // Centaur 71
75, // ClawTurtle 72
200, // FireWyvern 73
80, // GiantCrayfish 74
120, // Gi Lizard 75
100, // Gi Tree 76
100, // Master Orc 77
80, // Minaus 78
100, // Nizie 79
25,  // Tentocle 80
200, // Abaddon	 81
60, // Sorceress 82
60, // ATK 83
70, // MasterElf 84
60, // DSK 85
50, // HBT 86
60, // CT 87
60, // Barbarian 88
60, // AGC 89
35, // ncp 90 Gail
35,  // Gate 91
50 // Check xRisenx
};

void CGame::bAddNewEffect(short sType, int sX, int sY, int dX, int dY, char cStartFrame, int iV1)
{
	int i;
	short sAbsX, sAbsY, sDist;
	long lPan;
	int  iV2 = 0;
	if (m_cDetailLevel == 0) // Detail Level Low
	{	
		switch (sType)
		{
		case 11:
		case 15:
			return;
		}	
	}
//	if( m_bIsProgramActive == false ) return;
  	//sAbsX = abs(((m_sViewPointX / 32) + 10) - dX);
	//sAbsY = abs(((m_sViewPointY / 32) + 7) - dY);
	sAbsX = abs(((m_sViewPointX / 32) + 12) - dX); // 800x600 Resolution xRisenx Center Char xRisenx
	sAbsY = abs(((m_sViewPointY / 32) + 9) - dY); // 800x600 Resolution xRisenx Center Char xRisenx
	if (sAbsX > sAbsY) sDist = sAbsX;
	else sDist = sAbsY;

	//cout << "magic: " << sType << "," << sX << "," << sY << "," << dX << "," << dY << "," << iV1 << "\n";

	for (i = 0; i < MAXEFFECTS; i++)
	if (m_pEffectList[i] == 0)
	{
		m_pEffectList[i] = new class CEffect;
		m_pEffectList[i]->m_sType  = sType;
		m_pEffectList[i]->m_sX     = sX;
		m_pEffectList[i]->m_sY     = sY;
		m_pEffectList[i]->m_dX     = dX;
		m_pEffectList[i]->m_dY     = dY;
		m_pEffectList[i]->m_iV1    = iV1;
		m_pEffectList[i]->m_cFrame = cStartFrame;
		m_pEffectList[i]->m_dwTime = m_dwCurTime;

		switch (sType) {
		case 1: // coup normal
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - _iAttackerHeight[iV1];
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 2:	// Fl�che qui vole
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - _iAttackerHeight[iV1];
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 10;
			m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
			PlaySound('C', 4, sDist);
			break;

		case 4: // Gold
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32;
			m_pEffectList[i]->m_cMaxFrame   = 12;
			m_pEffectList[i]->m_dwFrameTime = 100;
			//sAbsX = abs(((m_sViewPointX / 32) + 10) - sX);
			//sAbsY = abs(((m_sViewPointY / 32) + 7)  - sY);
			sAbsX = abs(((m_sViewPointX / 32) + 12) - sX); // 800x600 Resolution xRisenx Center Char xRisenx
			sAbsY = abs(((m_sViewPointY / 32) + 9)  - sY); // 800x600 Resolution xRisenx Center Char xRisenx
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			lPan = -(((m_sViewPointX / 32) + 10) - sX)*1000;
			PlaySound('E', 12, sDist, lPan);
			break;

		case 5: // FireBall Fire Explosion
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 11;
			m_pEffectList[i]->m_dwFrameTime = 10;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - sX)*1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist);
			break;

		case 6:	 // Energy Bolt
		case 10: // Lightning Arrow
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 14;
			m_pEffectList[i]->m_dwFrameTime = 10;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX))*1000;
			PlaySound('E', 2, sDist, lPan);
			SetCameraShakingEffect(sDist);
			break;

		case 7: // Magic Missile Explosion
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 5;
			m_pEffectList[i]->m_dwFrameTime = 50;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX))*1000;
			PlaySound('E', 3, sDist, lPan);
			break;

		case 8: // Burst
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 4;
			m_pEffectList[i]->m_dwFrameTime = 30;
			break;

		case 9: // Burst
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_rX     =  6 - (rand() % 12);
			m_pEffectList[i]->m_rY     = -8 - (rand() % 6);
			m_pEffectList[i]->m_cMaxFrame   = 14;
			m_pEffectList[i]->m_dwFrameTime = 30;
			break;

		case 11:
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_rX     =  6 - (rand() % 12);
			if (iV2 == 0)
				 m_pEffectList[i]->m_rY     = -2 - (rand() % 4);
			else m_pEffectList[i]->m_rY     = -2 - (rand() % 10);
			m_pEffectList[i]->m_cMaxFrame   = 8;
			m_pEffectList[i]->m_dwFrameTime = 30;
			break;

		case 12: // Burst
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_rX    =  8 - (rand() % 16);
			m_pEffectList[i]->m_rY    =  4 - (rand() % 12);
			m_pEffectList[i]->m_cMaxFrame   = 10;
			m_pEffectList[i]->m_dwFrameTime = 30;
			break;

		case 13: // Bulles druncncity
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 18;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 14: // Traces de pas ou Tremor (pas en low detail)
			m_pEffectList[i]->m_mX    = sX;
			if (m_pEffectList[i]->m_iV1 > 0) // Case if hit by an arrow
			{	m_pEffectList[i]->m_mY = sY - (_iAttackerHeight[m_pEffectList[i]->m_iV1]/4 + rand()%(_iAttackerHeight[m_pEffectList[i]->m_iV1]/2) );
				m_pEffectList[i]->m_mX = sX + (rand()%5) -2;
			}else m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 4;
			m_pEffectList[i]->m_dwFrameTime = 100;
			m_pEffectList[i]->m_iV1 = iV1;
			break;

		case 15: //
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 16;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 16: //
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - 40;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 17:
			m_pEffectList[i]->m_mX     = sX + (rand() % 20) - 40;
			m_pEffectList[i]->m_mY     = sY + (rand() % 20) - 40;
			m_pEffectList[i]->m_rX     =  8 - (rand() % 16);
			m_pEffectList[i]->m_rY     =  4 - (rand() % 12);
			m_pEffectList[i]->m_mX3    = sX;
			m_pEffectList[i]->m_mY3    = sY;
			m_pEffectList[i]->m_iV1    = 0;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 18:
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 10;
			m_pEffectList[i]->m_dwFrameTime = 50;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;

		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27: // Critical strike with a weapon
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - 40;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 10;
			m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
			break;

		case 30: // Mass-Fire-Strike (called 1 time)
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 9;
			m_pEffectList[i]->m_dwFrameTime = 40;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX))*1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist*2);
			break;

		case 31: // Mass-Fire-Strike (called 3 times)
		case 252: //
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 8;
			m_pEffectList[i]->m_dwFrameTime = 40;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX))*1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist);
			break;

		case 32: //
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 4;
			m_pEffectList[i]->m_dwFrameTime = 100;
			break;

		case 33: //
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 16;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 34: //
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - 40;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 20;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;

		case 35: // Snoopy: rajout (pour Mass Magic-Missile)
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 18;
			m_pEffectList[i]->m_dwFrameTime = 40;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX))*1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist*2);
			break;

		case 36: // Snoopy: Rajout (pour Mass Magic-Missile)
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 15;
			m_pEffectList[i]->m_dwFrameTime = 40;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX))*1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist);
			break;

		case 40: //
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 15;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX)-320)*30;
			PlaySound('E', 45, sDist, lPan);
			break;

		case 41: // Large Type 1, 2, 3, 4
		case 42:
		case 43:
		case 44:
		case 45: // Small Type 1, 2
		case 46:
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY -220;
			m_pEffectList[i]->m_cMaxFrame   = 14;
			m_pEffectList[i]->m_dwFrameTime = 20;
			m_pEffectList[i]->m_iV1 = 20;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX)-320)*30;
			PlaySound('E', 46, sDist, lPan);
			break;

		case 47: // Blizzard
		case 48: // Blizzard
		case 49: // Blizzard
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY -220;
			m_pEffectList[i]->m_cMaxFrame   = 12;
			m_pEffectList[i]->m_dwFrameTime = 20;
			m_pEffectList[i]->m_iV1 = 20;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX)-320)*30;
			PlaySound('E', 46, sDist, lPan);
			break;

		case 50: //
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 12;
			m_pEffectList[i]->m_dwFrameTime = 50;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX)-320)*30;
			if ((rand()%4) == 1) SetCameraShakingEffect(sDist);
			PlaySound('E', 47, sDist, lPan);
			break;

		case 51:
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 9; //15;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 52: // Protect ring
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 15;
			m_pEffectList[i]->m_dwFrameTime = 80;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX)-320)*30;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 53: // Hold twist
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 15;
			m_pEffectList[i]->m_dwFrameTime = 80;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX)-320)*30;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 54: // star twingkling (effect armes brillantes)
		case 55: // Unused
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 10;
			m_pEffectList[i]->m_dwFrameTime = 15;
			break;

		case 56: //  Mass-Chill-Wind
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 14;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX)-320)*30;
			PlaySound('E', 45, sDist, lPan);
			break;

		case 57: //
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 16;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 60: //
			m_pEffectList[i]->m_mX    = sX +300;
			m_pEffectList[i]->m_mY    = sY -460;
			m_pEffectList[i]->m_cMaxFrame   = 10;
			m_pEffectList[i]->m_dwFrameTime = 50;
			break;

		case 61: //
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 16;
			m_pEffectList[i]->m_dwFrameTime = 10;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - sX)*1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist, 2);
			break;

		case 62: //
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 6;
			m_pEffectList[i]->m_dwFrameTime = 100;
			break;

		case 63: //
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 16;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 64: //
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 15;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 65: // Crusade's MS
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 30;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 66: // Crusade MS explosion
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 14;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - sX)*1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist, 2);
			break;

		case 67: // Crusade's MS fire + smoke ?
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 27;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 68: // worm-bite
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 17;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - sX)*1000;
			PlaySound('E', 4, sDist, lPan);
			m_pEffectList[i]->m_iV1 = sDist;
			//SetCameraShakingEffect(sDist, 2);
			break;

		case 69: // identique au cas 70
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 11;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - (sX/32))*1000;
			PlaySound('E', 42, sDist, lPan);
			break;

		case 70: // identtique au cas 69
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 11;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - (sX/32))*1000;
			PlaySound('E', 42, sDist, lPan);
			break;

		case 71: //
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 20;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;

		case 72: // Blizzard
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 15;
			m_pEffectList[i]->m_dwFrameTime = 20;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX)-320)*30;
			if ((rand()%4) == 1) SetCameraShakingEffect(sDist);
			PlaySound('E', 47, sDist, lPan);
			break;

		case 73:
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 15;
			m_pEffectList[i]->m_dwFrameTime = 60;
			break;

		case 74:
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 19;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 75: //ice golem
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_dX = dX;
			m_pEffectList[i]->m_dY = dY;
			m_pEffectList[i]->m_cMaxFrame   = 16;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 76: //ice golem
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_dX = dX;
			m_pEffectList[i]->m_dY = dY;
			m_pEffectList[i]->m_cMaxFrame   = 16;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 77: //ice golem
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_dX = dX;
			m_pEffectList[i]->m_dY = dY;
			m_pEffectList[i]->m_cMaxFrame   = 16;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 80: // Snoopy: rajou�, impl�ment� en dernier ds la v351
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_iV1    = 20;
			m_pEffectList[i]->m_cMaxFrame   = 30;
			m_pEffectList[i]->m_dwFrameTime = 25;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;

		case 81: //  Snoopy: Rajout (StormBlade)
			m_pEffectList[i]->m_mX		= sX*32;
			m_pEffectList[i]->m_mY		= sY*32;
			m_pEffectList[i]->m_iErr	= 0;
			m_pEffectList[i]->m_cMaxFrame   = 27;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 82: //  Snoopy: Rajout (Gate Apocalypse)
			m_pEffectList[i]->m_cMaxFrame   = 30;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 100: // MagicMissile is Flying
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - 40;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 20;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 101: // Heal
		case 111: // Staminar-Drain
		case 121: // Great Heal
		case 123: // Staminar-Recovery
		case 128: // Great-Staminar-Recovery
			m_pEffectList[i]->m_cMaxFrame   = 14;
			m_pEffectList[i]->m_dwFrameTime = 80;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 102: // CreateFood
		case 122: // Recall
		case 126: // Possession
		case 127: // Poison
		case 134: // DetectInvi
		case 136: // Cure
		case 142: // Confuse language
		case 152: // Polymorph
		case 153: // Mass-Poison
		case 162: // Confusion
		case 171: // Mass-Confusion
			m_pEffectList[i]->m_cMaxFrame   = 13;
			m_pEffectList[i]->m_dwFrameTime = 120;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 110: // Energy-Bolt
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - 40;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 20;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 112: // Recall
		case 131: // Summon
		case 132: // Invi
			m_pEffectList[i]->m_cMaxFrame   = 12;
			m_pEffectList[i]->m_dwFrameTime = 80;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 113: // Defense-Shield
		case 144: // Great-Defense-Shield
			m_pEffectList[i]->m_cMaxFrame   = 12;
			m_pEffectList[i]->m_dwFrameTime = 120;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 114: // Celebrating Light
			bAddNewEffect(69 +(rand()%2), dX*32 +20 - (rand() % 40), dY*32 +20 - (rand() % 40), 0, 0, -12);
			bAddNewEffect(69 +(rand()%2), dX*32 +20 - (rand() % 40), dY*32 +20 - (rand() % 40), 0, 0, -9);
			bAddNewEffect(69 +(rand()%2), dX*32 +20 - (rand() % 40), dY*32 +20 - (rand() % 40), 0, 0, -6);
			bAddNewEffect(69 +(rand()%2), dX*32 +20 - (rand() % 40), dY*32 +20 - (rand() % 40), 0, 0, -3);
			bAddNewEffect(69 +(rand()%2), dX*32 +20 - (rand() % 40), dY*32 +20 - (rand() % 40), 0, 0, 0);
			delete m_pEffectList[i];
			m_pEffectList[i] = 0;
			break;

		case 120: // Fire Ball
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - 40;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 20;
			m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 124: // Protect form N.M
		case 133: // Protection from Magic
 			bAddNewEffect(52, dX*32, dY*32, 0, 0, 0, 0);
			delete m_pEffectList[i];
			m_pEffectList[i] = 0;
			break;

		case 125: // Hold Person
		case 135: // Paralyze
			bAddNewEffect(53, dX*32, dY*32, 0, 0, 0, 0);
			delete m_pEffectList[i];
			m_pEffectList[i] = 0;
			break;

		case 130: // Fire Strike
		case 137: // Lightning Arrow
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - 40;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 20;
			m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 138: // Tremor.
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist, 2);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);

			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			m_pEffectList[i]->m_cMaxFrame   = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 143: // Lightning
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - 50;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_rX     = 5 - (rand() % 10);
			m_pEffectList[i]->m_rY	   = 5 - (rand() % 10);
			m_pEffectList[i]->m_cMaxFrame   = 7;
			m_pEffectList[i]->m_dwFrameTime = 10;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 40, sDist, lPan);
			break;

		case 145: // ChillWind
			m_pEffectList[i]->m_cMaxFrame   = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 147: // Triple-Energy-Bolt
			m_pEffectList[i]->m_cMaxFrame   = 0 ;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 150: // Berserk : Cirlcle 6 magic
		case 177: // Illusion-Movement
		case 180: // Illusion
		case 183: // Inhibition-Casting
		case 190: // Mass-Illusion
		case 195: // Mass-Illusion-Movement
			m_pEffectList[i]->m_cMaxFrame   = 11;
			m_pEffectList[i]->m_dwFrameTime = 100;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 151: // LightningBolt
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - 50;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_rX     = 5 - (rand() % 10);
			m_pEffectList[i]->m_rY	   = 5 - (rand() % 10);
			m_pEffectList[i]->m_cMaxFrame   = 10;
			m_pEffectList[i]->m_dwFrameTime = 10;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 40, sDist, lPan);
			break;

		case 156: // Mass-Ligtning-Arrow
			m_pEffectList[i]->m_cMaxFrame   = 3;
			m_pEffectList[i]->m_dwFrameTime = 130;
			break;

		case 157: // Ice-Strike
			m_pEffectList[i]->m_cMaxFrame   = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 160: // Energy-Strike
			m_pEffectList[i]->m_cMaxFrame   = 7;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 161: // Mass-Fire-Strike
		case 251: //
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - 40;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 20;
			m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 163: // Mass-Chill-Wind
			m_pEffectList[i]->m_cMaxFrame   = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 164: // worm-bite
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 4, sDist, lPan);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);

			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			bAddNewEffect(14, dX*32 + (rand() % 120) - 60, dY*32 + (rand() % 80) - 40, 0, 0, 0, 0);
			m_pEffectList[i]->m_cMaxFrame   = 1;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 165: // Absolute-Magic-Protection
			m_pEffectList[i]->m_cMaxFrame   = 21;
			m_pEffectList[i]->m_dwFrameTime = 70;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 166: // Armor Break
			m_pEffectList[i]->m_cMaxFrame   = 13;
			m_pEffectList[i]->m_dwFrameTime = 80;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 170: // Bloody-Shock-Wave
			m_pEffectList[i]->m_cMaxFrame   = 7;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 172: // Mass-Ice-Strike
			m_pEffectList[i]->m_cMaxFrame   = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 174: // Lightning-Strike
			m_pEffectList[i]->m_cMaxFrame   = 5;
			m_pEffectList[i]->m_dwFrameTime = 120;
			break;

		case 176: // Snoopy: Ajout Cancellation
			m_pEffectList[i]->m_cMaxFrame   = 23;
			m_pEffectList[i]->m_dwFrameTime = 60;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - sX)*1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 181: // MS
			m_pEffectList[i]->m_mX    = dX*32 +300;
			m_pEffectList[i]->m_mY    = dY*32 -460;
			m_pEffectList[i]->m_cMaxFrame   = 10;
			m_pEffectList[i]->m_dwFrameTime = 25;
			break;

		case 182: // Snoopy: Ajout Mass-Magic-Missile
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32 - 40;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 20;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 244: // Snoopy: d�plac� pour nvx sorts: Aura du casteur de Mass MagicMissile
		//case 184: // effet sur le caster pour MassMM
			m_pEffectList[i]->m_cMaxFrame   = 29;
			m_pEffectList[i]->m_dwFrameTime = 80;
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			break;

		case 191: // Blizzard
			m_pEffectList[i]->m_cMaxFrame   = 7;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		//case 192: // Hero set Effect
		case 242: // Hero set Effect
			m_pEffectList[i]->m_cMaxFrame   = 30;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		//case 193: // Hero set Effect
		case 243: // Hero set Effect
			m_pEffectList[i]->m_cMaxFrame   = 19;
			m_pEffectList[i]->m_dwFrameTime = 18;
			break;

		case 194: // Resurrection
			m_pEffectList[i]->m_cMaxFrame   = 30;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 196: // Snoopy: Ajout de Earth-Shock-Wave
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 30;
			m_pEffectList[i]->m_dwFrameTime = 25;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;
		case 200: //
		case 201: //
		case 202: //
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 15;
			m_pEffectList[i]->m_dwFrameTime = 25;
			break;

		case 203: //
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 18;
			m_pEffectList[i]->m_dwFrameTime = 70;
			break;

		case 204: //
		case 205: //
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 12;
			m_pEffectList[i]->m_dwFrameTime = 70;
			break;

		case 206: //
			m_pEffectList[i]->m_mX     = sX;
			m_pEffectList[i]->m_mY     = sY;
			m_pEffectList[i]->m_cMaxFrame   = 3;
			m_pEffectList[i]->m_dwFrameTime = 70;
			break;

		case 250: //
			m_pEffectList[i]->m_mX     = sX * 32;
			m_pEffectList[i]->m_mY     = sY * 32 -40;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame   = 0;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 197: //Fiery-Shock-Wave xRisenx
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 8;
			m_pEffectList[i]->m_dwFrameTime = 25;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;

		case 198: //Mass-Blizzard xRisenx
			m_pEffectList[i]->m_cMaxFrame   = 15;
			m_pEffectList[i]->m_dwFrameTime = 100;
			break;

		case 192: //Call-Of-The-Gods xRisenx
		    m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 8;
			m_pEffectList[i]->m_dwFrameTime = 25;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
      		break;

		case 149:
		case 187: //Strike-Of-The-Ghosts xRisenx
			m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 8;
			m_pEffectList[i]->m_dwFrameTime = 25;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			break;

		case 245: //Ghosts 1 xRisenx
			m_pEffectList[i]->m_mX    = sX;
			m_pEffectList[i]->m_mY    = sY;
			m_pEffectList[i]->m_cMaxFrame   = 17;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - sX)*1000;
			PlaySound('E', 4, sDist, lPan);
			m_pEffectList[i]->m_iV1 = sDist;
			break;

		case 193: //Hurricane-Blast xRisenx
				  
		    m_pEffectList[i]->m_mX     = sX*32;
			m_pEffectList[i]->m_mY     = sY*32;
			m_pEffectList[i]->m_iErr   = 0;
			m_pEffectList[i]->m_cMaxFrame   = 8;
			m_pEffectList[i]->m_dwFrameTime = 25;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
      		break;

		case 184: //Magic-Shield xRisenx
			m_pEffectList[i]->m_cMaxFrame   = 21;
			m_pEffectList[i]->m_dwFrameTime = 70;
			lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
			PlaySound('E', 5, sDist, lPan);
			break;

       case 188: // Lightning-Storm xRisenx
            m_pEffectList[i]->m_mX = sX*32;
			m_pEffectList[i]->m_mY = sY*32;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = 20;
			m_pEffectList[i]->m_dwFrameTime = 25;
			sAbsX = abs(400 - (sX - m_sViewPointX));
			sAbsY = abs(300 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;

		//case 104: // Thunder Storm xRisenx
		//	m_pEffectList[i]->m_mX     = sX*32;
		//	m_pEffectList[i]->m_mY     = sY*32;
		//	m_pEffectList[i]->m_iErr   = 0;
		//	m_pEffectList[i]->m_cMaxFrame   = 30;
		//	m_pEffectList[i]->m_dwFrameTime = 25;
		//	sAbsX = abs(320 - (sX - m_sViewPointX));
		//	sAbsY = abs(240 - (sY - m_sViewPointY));

		//	if (sAbsX > sAbsY) sDist = sAbsX;
		//	else sDist = sAbsY;

		//	sDist = sDist / 32;
		//	SetCameraShakingEffect(sDist);
		//	break;

		//case 105: // Inferno xRisenx
		//	m_pEffectList[i]->m_mX  = sX*32;
  //          m_pEffectList[i]->m_mY  = sY*32;
		//	m_pEffectList[i]->m_iErr   = 0;
		//	m_pEffectList[i]->m_cMaxFrame   = 20;
		//	m_pEffectList[i]->m_dwFrameTime = 25;
		//	sAbsX = abs(320 - (sX - m_sViewPointX));
		//	sAbsY = abs(240 - (sY - m_sViewPointY));

		//	if (sAbsX > sAbsY) sDist = sAbsX;
		//	else sDist = sAbsY;

		//	sDist = sDist / 32;
		//	SetCameraShakingEffect(sDist);
		//	break;
		//	// This is where u set Coords and Max Time / Frames on spell animation
		//case 106: // Hell-Fire xRisenx
		//	m_pEffectList[i]->m_mX    = dX*32 +300;
		//	m_pEffectList[i]->m_mY    = dY*32 -460;
		//	m_pEffectList[i]->m_cMaxFrame   = 10;
		//	m_pEffectList[i]->m_dwFrameTime = 25;
		//	/*m_pEffectList[i]->m_mX     = sX*32;
		//	m_pEffectList[i]->m_mY     = sY*32 - 40;
		//	m_pEffectList[i]->m_iErr   = 0;
		//	m_pEffectList[i]->m_cMaxFrame   = NULL;
		//	m_pEffectList[i]->m_dwFrameTime = 20;
		//	m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);*/

		//	lPan = -(((m_sViewPointX / 32) + 10) - dX)*1000;
		//	PlaySound('E', 1, sDist, lPan);
		//	break;

		default:
			delete m_pEffectList[i];
			m_pEffectList[i] = 0;
			break;
		}
		if (m_pEffectList[i] != 0)
		{	m_pEffectList[i]->m_mX2 = m_pEffectList[i]->m_mX;
			m_pEffectList[i]->m_mY2 = m_pEffectList[i]->m_mY;
		}
		return;
	}
}

void CGame::DrawEffects()
{
	int i, dX, dY, iDvalue,  tX, tY, rX, rY, rX2, rY2, rX3, rY3, rX4, rY4, rX5, rY5, iErr;
	char  cTempFrame;
	uint32_t dwTime = m_dwCurTime;

 	for (i = 0;	i < MAXEFFECTS; i++)
	if ((m_pEffectList[i] != 0) && (m_pEffectList[i]->m_cFrame >= 0))
	{
		switch (m_pEffectList[i]->m_sType) {
		case 1: // Normal hit
			if (m_pEffectList[i]->m_cFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[8]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 2: // Arrow flying
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = (m_pEffectList[i]->m_cDir-1)*2;
			if (cTempFrame < 0) break;
			m_pEffectSpr[7]->PutSpriteFast(dX, dY, cTempFrame, dwTime);
			break;

		case 4: // gold
			/* 1.5
			if (m_pEffectList[i]->m_cFrame < 9) break;
			cTempFrame = m_pEffectList[i]->m_cFrame - 9;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[1]->PutSpriteFast(dX, dY-40, cTempFrame, dwTime);
			*/
			break;

		case 5: // FireBall Fire Explosion
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			iDvalue = (cTempFrame - 8)*(-5);
			if (cTempFrame < 7)
				 m_pEffectSpr[3]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			else m_pEffectSpr[3]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);
			break;

		case 6:	 // Energy Bolt
		case 10: // Lightning Arrow
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			iDvalue = (cTempFrame - 7)*(-6);
			if (cTempFrame < 6)
				 m_pEffectSpr[6]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			else m_pEffectSpr[6]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 7: // Magic Missile Explosion
			cTempFrame = m_pEffectList[i]->m_cFrame;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			iDvalue = (cTempFrame - 4)*(-3);
			if (cTempFrame < 4)
				 m_pEffectSpr[6]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			else m_pEffectSpr[6]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 8: // Burst
			cTempFrame = m_pEffectList[i]->m_cFrame;
			cTempFrame = 4 - cTempFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 9: // Burst
			cTempFrame = (rand() % 5);
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 11: // pt grenat
			cTempFrame = (rand() % 5) + 5;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite2(dX, dY, cTempFrame, dwTime);
			break;

		case 12: // Burst
			cTempFrame = (rand() % 6) + 10;
			if (cTempFrame < 0) break;
			iDvalue = (m_pEffectList[i]->m_cFrame - 4)*(-3);
			dX  = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			if (cTempFrame < 4)
				 m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			else //m_pEffectSpr[11]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			//
			m_pEffectSpr[11]->PutTransSprite(dX, dY, cTempFrame, dwTime);
			break;

		case 13:
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			if (cTempFrame < 13)
			{	m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, 25 + (cTempFrame / 5), dwTime);
			}else
			{	m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, (8 + cTempFrame), dwTime);
			}
			break;

		case 14: // Traces de pas (terrain sec)
			if (m_pEffectList[i]->m_cFrame < 0) break;
			dX  = m_pEffectList[i]->m_mX - m_sViewPointX;
			dY  = m_pEffectList[i]->m_mY - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite50_NoColorKey(dX, dY, (28 + m_pEffectList[i]->m_cFrame), dwTime);
			break;

		case 15: // petits nuages rouges
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = m_pEffectList[i]->m_mX - m_sViewPointX;
			dY  = m_pEffectList[i]->m_mY - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite50_NoColorKey(dX, dY, (33 + cTempFrame), dwTime);
			break;

		case 16: //
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[0]->PutTransSprite_NoColorKey(dX, dY, 0, dwTime);
			break;

		case 17: //test
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			cTempFrame = 39 + (rand() % 3)*3 + (rand() % 3);
			if (cTempFrame < 0) break;
			m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			dX  = (m_pEffectList[i]->m_mX2)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY2)  - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite50_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 18: //
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			m_pEffectSpr[18]->PutTransSprite70_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 20: // critical hit
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27: // Critical strike with a weapon
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[8]->PutTransSprite_NoColorKey(dX, dY, 1, dwTime);
			break;

		case 30: // Mass-Fire-Strike
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			m_pEffectSpr[14]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 31: // Mass-Fire-Strike
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			m_pEffectSpr[15]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 32: // Trace de pas  (raining weather)
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame + 20;
			if (cTempFrame < 0) break;
			m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 33: //
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			iDvalue = 0;
			m_pEffectSpr[19]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 34: // absent (220 et 351)
			break;

		case 35: // Snoopy: Ajout
			if (m_pEffectList[i]->m_cFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			m_pEffectSpr[6]->PutTransSprite_NoColorKey(dX-30, dY-18, cTempFrame, dwTime);
			break;

		case 36: // Snoopy: Ajout
			if (m_pEffectList[i]->m_cFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			m_pEffectSpr[97]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 40:
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[20]->PutTransSprite50_NoColorKey(dX, dY, cTempFrame, dwTime); // 20
			break;

		case 41: // Large Type 1, 2, 3, 4
		case 42:
		case 43:
		case 44:
		case 45: // Small Type 1, 2
		case 46:
			dX  = (m_pEffectList[i]->m_sX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_sY)  - m_sViewPointY;
			m_pEffectSpr[21]->PutFadeSprite(dX, dY, 48, dwTime);
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			if ((8*(m_pEffectList[i]->m_sType-41) +cTempFrame) < (8*(m_pEffectList[i]->m_sType-41) +7))
			{	iDvalue = -8*(6 - cTempFrame);
				m_pEffectSpr[21]->PutTransSpriteRGB(dX, dY, 8*(m_pEffectList[i]->m_sType-41) +cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			}else
			{	if ((cTempFrame - 5) >= 8) cTempFrame = ((cTempFrame - 5) - 8) + 5;
				m_pEffectSpr[21]->PutSpriteFast(dX, dY, 8*(m_pEffectList[i]->m_sType-41) + (cTempFrame - 5), dwTime);
			}
			break;

		case 47:
		case 48:
		case 49: // Blizzard
			dX  = (m_pEffectList[i]->m_sX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_sY)  - m_sViewPointY;
			m_pEffectSpr[m_pEffectList[i]->m_sType-1]->PutRevTransSprite(dX, dY, 0, dwTime);
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			//PutString(dX, dY, "*",video::SColor(255,255,255,255));
			if (cTempFrame < 7) {
				iDvalue = -8*(6 - cTempFrame);
				m_pEffectSpr[m_pEffectList[i]->m_sType-1]->PutTransSpriteRGB(dX, dY, cTempFrame+1, iDvalue, iDvalue, iDvalue, dwTime);
			}
			else {
				if (cTempFrame >= 8) cTempFrame = cTempFrame % 8;
				m_pEffectSpr[m_pEffectList[i]->m_sType-1]->PutSpriteFast(dX, dY, cTempFrame+1, dwTime);
			}
			break;

		case 50:
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;

			if (cTempFrame <= 6) {
				iDvalue = 0;
				m_pEffectSpr[22]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);	// RGB2
			}
			else {
				iDvalue = -5*(cTempFrame - 6);
				m_pEffectSpr[22]->PutTransSpriteRGB(dX, dY, 6, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			}
			break;

		case 51: //
			cTempFrame = m_pEffectList[i]->m_cFrame + 11; //15
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[28]->PutTransSprite25(dX, dY, cTempFrame, dwTime); //20
			break;


		case 52: // Protection Ring commente par siementec, a voir
		/*			cTempFrame = m_pEffectList[i]->m_cFrame;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[24]->PutTransSprite(dX, dY, cTempFrame, dwTime);
		*/
			break;


		case 53: // Hold Twist
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			if (cTempFrame < 0) cTempFrame = 0;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[25]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime); //25
			break;

		case 54: //  star twingkling (effect armes brillantes)
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) cTempFrame = 0;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[28]->PutTransSprite(dX, dY, cTempFrame, dwTime);
			break;

		case 55: //
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) cTempFrame = 0;
			dX  = (m_pEffectList[i]->m_mX);
			dY  = (m_pEffectList[i]->m_mY);
			m_pEffectSpr[28]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 56: // Mass-Chill-Wind
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) cTempFrame = 0;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[29]->PutTransSprite50_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 57:  // absent (220 et 351)
			break;

		case 60:  //
		case 181: // MS
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			if (cTempFrame > 4)
			{	cTempFrame = cTempFrame / 4;
			}
			if (cTempFrame >= 0)
			{	dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
				dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
				m_pEffectSpr[31]->PutSpriteFast(dX, dY, 15 + cTempFrame, dwTime);
				m_pEffectSpr[31]->PutTransSprite(dX, dY, cTempFrame, dwTime);
			}
			break;

		case 61: // Fire aura on ground (crueffect1, 1)
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[32]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 62: // MS strike
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			if (cTempFrame > 0)
			{	cTempFrame = cTempFrame - 1;
				dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
				dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
				m_pEffectSpr[31]->PutRevTransSprite(dX, dY, 20 + cTempFrame, dwTime, cTempFrame/3);
			}
			break;

		case 63: // Fire explosion (crueffect1, 2)
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[33]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 64: // effet halo blancchatre
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[34]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 65: // MS from crusade striking
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			cTempFrame = cTempFrame / 6;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[31]->PutRevTransSprite(dX, dY, 20 + cTempFrame, dwTime, cTempFrame >> 2);
			break;

		case 66: // MS explodes on the ground
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[39]->PutRevTransSprite(dX, dY, cTempFrame, dwTime);
			m_pEffectSpr[39]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 67: // MS fire with smoke
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			switch (rand() % 3) {
			case 0: m_pEffectSpr[0]->PutTransSprite25_NoColorKey(dX, dY +20, 1, dwTime); break;
			case 1: m_pEffectSpr[0]->PutTransSprite50_NoColorKey(dX, dY +20, 1, dwTime); break;
			case 2: m_pEffectSpr[0]->PutTransSprite70_NoColorKey(dX, dY +20, 1, dwTime); break;
			}
			m_pEffectSpr[35]->PutTransSprite70_NoColorKey(dX, dY, cTempFrame/3, dwTime);
			break;

		case 68: // worm-bite
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			if (cTempFrame <= 11)
			{	m_pEffectSpr[40]->PutSpriteFast(dX, dY, cTempFrame, dwTime);
				m_pEffectSpr[41]->PutTransSprite50_NoColorKey(dX, dY, cTempFrame, dwTime);
				m_pEffectSpr[44]->PutRevTransSprite(dX-2, dY-3, cTempFrame, dwTime);
				m_pEffectSpr[44]->PutTransSprite_NoColorKey(dX-4, dY-3, cTempFrame, dwTime);
			}else
			{	switch (cTempFrame) {
				case 12:
				case 13:
				case 14: m_pEffectSpr[40]->PutSpriteFast(dX, dY, 11, dwTime); break;
				case 15: m_pEffectSpr[40]->PutTransSprite70_NoColorKey(dX, dY, 11, dwTime); break;
				case 16: m_pEffectSpr[40]->PutTransSprite50_NoColorKey(dX, dY, 11, dwTime); break;
				case 17: m_pEffectSpr[40]->PutTransSprite25_NoColorKey(dX, dY, 11, dwTime); break;
			}	}
			break;

		case 69: // identique au cas 70
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[42]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 70: // identique au cas 69
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[43]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 71: // absent v220 et v351
			break;

		case 72: // Blizzard
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			if (cTempFrame <= 8)
			{	iDvalue = 0;
				m_pEffectSpr[51]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);
			}else
			{	iDvalue = -1*(cTempFrame - 8);
				m_pEffectSpr[51]->PutTransSpriteRGB(dX, dY, 8, iDvalue, iDvalue, iDvalue, dwTime);	// RGB2
			}
			break;

		case 73: // absent v220 et v351
		case 74: // absent v220 et v351
		case 75: // absent v220 et v351
		case 76: // absent v220 et v351
		case 77: // absent v220 et v351
			break;

		case 80:
		case 196: // Earth-Shock-Wave
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[91]->PutSpriteFast(dX, dY, cTempFrame, dwTime); //Nbe d'arguments modifi�s ds la 351....
			m_pEffectSpr[92]->PutTransSprite(dX, dY, cTempFrame, dwTime);
			break;

		case 81: // Snoopy: Ajout StormBlade
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			m_pEffectSpr[100]->PutTransSprite_NoColorKey(dX+70, dY+70, cTempFrame, dwTime);
			break;

		case 82: // Gate (apocalypse)
			cTempFrame = m_pEffectList[i]->m_cFrame;
			//m_pEffectSpr[101]->PutTransSprite_NoColorKey(320, 480, cTempFrame, dwTime);
			m_pEffectSpr[101]->PutTransSprite_NoColorKey(GetWidth()/2, GetHeight(), cTempFrame, dwTime); // 800x600 xRisenx
			break;

		case 100: // Magic Missile
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[0]->PutTransSprite_NoColorKey(dX, dY, 3, dwTime);
			break;

		case 101: // Heal
		case 121: // Great-Heal
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			m_pEffectSpr[50]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 102: // Create Food
		case 124: // Protection from N.M
		case 125: // Hold-Person
		case 126: // Possession
		case 127: // Poison
		case 133: // Protect-From-Magic
		case 134: // Detect-Invisibility
		case 135: // Paralyze
		case 136: // Cure
		case 142: // Confuse Language
		case 152: // Polymorph
		case 153: // Mass-Poison
		case 162: // Confusion
		case 171: // Mass-Confusion
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			if (cTempFrame < 5)
				 m_pEffectSpr[4]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[4]->PutTransSpriteRGB(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 110: // Energy-Bolt
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[0]->PutTransSprite_NoColorKey(dX, dY, 2 + (rand() % 4), dwTime);
			break;

		case 111: // Staminar Drain
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			m_pEffectSpr[49]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 112: // Recall
		case 131: // Summon-Creature
		case 132: // Invisibility
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			m_pEffectSpr[52]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		//case 132: // Invisibility
		//	cTempFrame = m_pEffectList[i]->m_cFrame;
		//	if (cTempFrame < 0) break;
		//	dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
		//	dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
		//	iDvalue = (cTempFrame - 5)*(-5);
		//	m_pEffectSpr[172]->PutTransSprite_NoColorKey(dX-10, dY-100, m_pEffectList[i]->m_cFrame, dwTime);
		//	break;

		case 113: // Defense Shield
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			if (cTempFrame < 6)
				 m_pEffectSpr[62]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[62]->PutTransSpriteRGB(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 120: // Fire Ball
		case 130: // Fire Strike
		case 161: // Mass-Fire-Strike
		case 251: //
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			cTempFrame = (m_pEffectList[i]->m_cDir-1)*4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[5]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 122: // Absent v220 et 351
			break;

		case 123: // Staminar-Recovery
		case 128: // Great-Staminar-Recovery
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			m_pEffectSpr[56]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 137: // Lightning Arrow
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			tX  = (m_pEffectList[i]->m_mX2)  - m_sViewPointX;
			tY  = (m_pEffectList[i]->m_mY2)  - m_sViewPointY;
			iErr = 0;
			m_Misc.GetPoint(dX, dY, tX, tY, &rX, &rY, &iErr, 15);
			m_Misc.GetPoint(dX, dY, tX, tY, &rX2, &rY2, &iErr, 30);
			m_Misc.GetPoint(dX, dY, tX, tY, &rX3, &rY3, &iErr, 45);
			m_Misc.GetPoint(dX, dY, tX, tY, &rX4, &rY4, &iErr, 60);
			m_Misc.GetPoint(dX, dY, tX, tY, &rX5, &rY5, &iErr, 75);
			cTempFrame = (m_pEffectList[i]->m_cDir-1)*4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite25_NoColorKey(rX5, rY5, cTempFrame, dwTime);
			cTempFrame = (m_pEffectList[i]->m_cDir-1)*4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite25_NoColorKey(rX4, rY4, cTempFrame, dwTime);
			cTempFrame = (m_pEffectList[i]->m_cDir-1)*4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite50_NoColorKey(rX3, rY3, cTempFrame, dwTime);
			cTempFrame = (m_pEffectList[i]->m_cDir-1)*4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite50_NoColorKey(rX2, rY2, cTempFrame, dwTime);
			cTempFrame = (m_pEffectList[i]->m_cDir-1)*4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite70_NoColorKey(rX,  rY, cTempFrame, dwTime);
			cTempFrame = (m_pEffectList[i]->m_cDir-1)*4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite(dX, dY, cTempFrame, dwTime);
			break;

		case 143: // Lightning
			_DrawThunderEffect(m_pEffectList[i]->m_dX*32 - m_sViewPointX, m_pEffectList[i]->m_dY*32 - m_sViewPointY - GetWidth(),
				                m_pEffectList[i]->m_dX*32 - m_sViewPointX, m_pEffectList[i]->m_dY*32 - m_sViewPointY,
								m_pEffectList[i]->m_rX, m_pEffectList[i]->m_rY, 1);
			_DrawThunderEffect(m_pEffectList[i]->m_dX*32 - m_sViewPointX, m_pEffectList[i]->m_dY*32 - m_sViewPointY - GetWidth(),
				                m_pEffectList[i]->m_dX*32 - m_sViewPointX, m_pEffectList[i]->m_dY*32 - m_sViewPointY,
								m_pEffectList[i]->m_rX+4, m_pEffectList[i]->m_rY+2, 2);
			_DrawThunderEffect(m_pEffectList[i]->m_dX*32 - m_sViewPointX, m_pEffectList[i]->m_dY*32 - m_sViewPointY - GetWidth(),
				                m_pEffectList[i]->m_dX*32 - m_sViewPointX, m_pEffectList[i]->m_dY*32 - m_sViewPointY,
								m_pEffectList[i]->m_rX-2, m_pEffectList[i]->m_rY-2, 2);
			break;

		case 144: // Great-Defense-Shield
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			if (cTempFrame < 9)
				 m_pEffectSpr[63]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[63]->PutTransSpriteRGB(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 151: // Lightning Bolt
			_DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
				                m_pEffectList[i]->m_dX*32 - m_sViewPointX, m_pEffectList[i]->m_dY*32 - m_sViewPointY,
								m_pEffectList[i]->m_rX, m_pEffectList[i]->m_rY, 1);

			_DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
				                m_pEffectList[i]->m_dX*32 - m_sViewPointX, m_pEffectList[i]->m_dY*32 - m_sViewPointY,
								m_pEffectList[i]->m_rX+2, m_pEffectList[i]->m_rY-2, 2);

			_DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
				                m_pEffectList[i]->m_dX*32 - m_sViewPointX, m_pEffectList[i]->m_dY*32 - m_sViewPointY,
								m_pEffectList[i]->m_rX-2, m_pEffectList[i]->m_rY-2, 2);
			break;

		case 165: // Absolute-Magic-Protect
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY; // 53 = APFM buble
			m_pEffectSpr[53]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 166: // Armor-Break
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			m_pEffectSpr[55]->PutRevTransSprite(dX, dY+35, m_pEffectList[i]->m_cFrame, dwTime);
			m_pEffectSpr[54]->PutTransSprite50(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 176: // Cancellation
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			m_pEffectSpr[90]->PutTransSprite_NoColorKey(dX+50, dY+85, cTempFrame, dwTime);
			break;

		case 177: // Illusion-Movement
		case 180: // Illusion
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-3);
			if (cTempFrame < 9)	m_pEffectSpr[60]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[60]->PutTransSpriteRGB(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 182: //Mass-Magic-Missile
			cTempFrame = m_pEffectList[i]->m_cFrame;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[98]->PutTransSprite(dX, dY, cTempFrame, dwTime, 0);
		break;

		case 183: // Inhibition-Casting
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-3);
			if (cTempFrame < 9) m_pEffectSpr[94]->PutTransSprite_NoColorKey(dX, dY + 40, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[94]->PutTransSpriteRGB(dX, dY + 40, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime);
			break;

		case 184: //Magic-Shield xRisenx
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY; // 53 = APFM buble
			m_pEffectSpr[53]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 245: // Strike-Of-The-Ghosts
		case 187:
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			
			m_pEffectSpr[158]->PutTransSprite70(dX, dY , cTempFrame, dwTime);
			m_pEffectSpr[154]->PutTransSprite70(dX, dY	, cTempFrame, dwTime);

			break;

		case 188: // Lightning Storm xRisenx
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			/*m_pEffectSpr[91]->PutSpriteFast(dX, dY, cTempFrame, dwTime); 
			m_pEffectSpr[92]->PutTransSprite(dX, dY, cTempFrame, dwTime);*/
			break;

		//case 104: // Thunder Storm xRisenx
		//	cTempFrame = m_pEffectList[i]->m_cFrame;
		//	if (cTempFrame < 0) break;
		//	dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
		//	dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
		//	m_pEffectSpr[169]->PutSpriteFast(dX, dY, cTempFrame, dwTime); //effect11.pak [2]
		//	m_pEffectSpr[177]->PutTransSprite(dX, dY, cTempFrame, dwTime); //effect11.pak [3]
		//	break;

		//case 105: // Inferno xRisenx
		//	cTempFrame = m_pEffectList[i]->m_cFrame;
		//	if (cTempFrame < 0) break;
		//	dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
		//	dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
		//	/*m_pEffectSpr[91]->PutSpriteFast(dX, dY, cTempFrame, dwTime);
		//	m_pEffectSpr[92]->PutTransSprite(dX, dY, cTempFrame, dwTime);*/
		//	break;

		// From what i understand this is where you make the spell animation when its in the air.

		//case 106: // Hell-Fire xRisenx
		//	cTempFrame = m_pEffectList[i]->m_cFrame;
		//	if (cTempFrame < 0) break;
		//	if (cTempFrame > 4)
		//	{	cTempFrame = cTempFrame / 4;
		//	}
		//	if (cTempFrame >= 0)
		//	{	dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
		//		dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
		//		m_pEffectSpr[135]->PutSpriteFast(dX, dY, 15 + cTempFrame, dwTime);
		//		m_pEffectSpr[135]->PutSpriteFast(dX, dY+65, 15 + cTempFrame, dwTime);
		//		m_pEffectSpr[135]->PutSpriteFast(dX, dY-65, 15 + cTempFrame, dwTime);
		//		//m_pEffectSpr[31]->PutSpriteFast(dX+65, dY+65, 15 + cTempFrame, dwTime);
		//		m_pEffectSpr[135]->PutTransSprite(dX, dY, cTempFrame, dwTime);
		//		m_pEffectSpr[135]->PutTransSprite(dX, dY+65, cTempFrame, dwTime);
		//		m_pEffectSpr[135]->PutTransSprite(dX, dY-65, cTempFrame, dwTime);
		//		//m_pEffectSpr[31]->PutTransSprite(dX+65, dY+65, cTempFrame, dwTime);
		//		m_pEffectSpr[5]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
		//	}
		//	//dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
		//	//dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
		//	//cTempFrame = (m_pEffectList[i]->m_cDir-1)*4 + (rand() % 4);

		//	//if (cTempFrame < 0) break;
		//	//m_pEffectSpr[5]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
		//	break;

		case 244: // Snoopy: d�plac� pour nvx sorts: Aura du casteur de Mass MagicMissile
		//case 184: // Aura du casteur de Mass MagicMissile
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = m_pEffectList[i]->m_mX - m_sViewPointX;
			dY  = m_pEffectList[i]->m_mY - m_sViewPointY;
			m_pEffectSpr[96]->PutTransSprite(dX, dY, m_pEffectList[i]->m_cFrame, dwTime, 0);
			break;

		case 190: // Mass-Illusion
		case 195: // Mass-Illusion-Movement
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-3);
			if (cTempFrame < 9) m_pEffectSpr[61]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[61]->PutTransSpriteRGB(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		//case 192: // Mage Hero set effect
		case 242:
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			m_pEffectSpr[87]->PutTransSprite_NoColorKey(dX+50, dY+57, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		//case 193: // War Hero set effect
		case 243:
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			m_pEffectSpr[88]->PutTransSprite_NoColorKey(dX+65, dY+80, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 194: // Resurrection
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			m_pEffectSpr[99]->PutTransSprite(dX, dY, m_pEffectList[i]->m_cFrame, dwTime, 0);
			break;

		case 200: // shotstar fall on ground
			dX  = m_pEffectList[i]->m_mX;
			dY  = m_pEffectList[i]->m_mY;
			m_pEffectSpr[133]->PutTransSprite_NoColorKey(dX, dY, (rand() %15), dwTime);
			break;

		case 201: // shotstar fall on ground
			dX  = m_pEffectList[i]->m_mX;
			dY  = m_pEffectList[i]->m_mY;
			m_pEffectSpr[134]->PutTransSprite_NoColorKey(dX, dY, (rand() %15), dwTime);
			break;

		case 202: // shotstar fall on ground
			dX  = m_pEffectList[i]->m_mX;
			dY  = m_pEffectList[i]->m_mY;
			m_pEffectSpr[135]->PutTransSprite_NoColorKey(dX, dY, (rand() %15), dwTime);
			break;

		case 203: // explosion feu apoc
			dX  = m_pEffectList[i]->m_mX;
			dY  = m_pEffectList[i]->m_mY;
			m_pEffectSpr[136]->PutTransSprite_NoColorKey(dX, dY, (rand() %18), dwTime);
			break;

		case 204: // Faille oblique
			dX  = m_pEffectList[i]->m_mX;
			dY  = m_pEffectList[i]->m_mY;
			m_pEffectSpr[137]->PutTransSprite_NoColorKey(dX, dY, (rand() %12), dwTime);
			break;

		case 205: // Faille horizontale
			dX  = m_pEffectList[i]->m_mX;
			dY  = m_pEffectList[i]->m_mY;
			m_pEffectSpr[138]->PutTransSprite_NoColorKey(dX, dY, (rand() %12), dwTime);
			break;

		case 206: // steams
			dX  = m_pEffectList[i]->m_mX;
			dY  = m_pEffectList[i]->m_mY;
			m_pEffectSpr[139]->PutTransSprite_NoColorKey(dX, dY, (rand() %20), dwTime);
			break;

		case 250: // Gate (round one)
			dX  = m_pEffectList[i]->m_mX - m_sViewPointX;
			dY  = m_pEffectList[i]->m_mY - m_sViewPointY;
			//m_pEffectSpr[103]->PutTransSprite_NoColorKey(dX, dY, (rand() %3), dwTime);
			break;

		case 252: // burst (lisgt salmon color)
			dX  = m_pEffectList[i]->m_mX - m_sViewPointX;
			dY  = m_pEffectList[i]->m_mY - m_sViewPointY;
			m_pEffectSpr[104]->PutTransSprite_NoColorKey(dX, dY, (rand() %3), dwTime);
			break;
		}
	}
}

void CGame::bItemDrop_TopPanel()
{
	if (m_bIsItemDisabled[m_stMCursor.sSelectedObjectID] == true) return;
	if (m_cCommand < 0) return;

	char onButton = m_dialogBoxes[30].OnButton();
	if (onButton == 4)
	{	
		m_droppedOnIconbar = true;
		bItemDrop_Inventory();
	}
	else if (onButton == 3)
	{	
		bItemDrop_Character();
	}
}

void CGame::bItemDrop_IconPanel()
{
	if (m_bIsItemDisabled[m_stMCursor.sSelectedObjectID] == true) return;
	if (m_cCommand < 0) return;

	char onButton = m_dialogBoxes[30].OnButton();
	if (onButton == 4)
	{	
		m_droppedOnIconbar = true;
		bItemDrop_Inventory();
	}
	else if (onButton == 3)
	{	
		bItemDrop_Character();
	}
}

void CGame::DrawEffectLights()
{
	int i, dX, dY, iDvalue;
 uint32_t dwTime = m_dwCurTime;
 char  cTempFrame;

	for (i = 0;	i < MAXEFFECTS; i++)
	if (m_pEffectList[i] != 0) {
		switch (m_pEffectList[i]->m_sType) {
		case 1:
			break;

		case 2:
			break;

		case 4:
			break;

		case 5:	// Fire Explosion
			if (m_pEffectList[i]->m_cFrame >= 0)
			{	dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
				dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
				iDvalue = (m_pEffectList[i]->m_cFrame - 7)*(-1);
				if (m_pEffectList[i]->m_cFrame < 6)
					 m_pEffectSpr[0]->PutTransSprite_NoColorKey(dX, dY+30, 1, dwTime);
				else m_pEffectSpr[0]->PutTransSpriteRGB(dX, dY+30, 1, iDvalue, iDvalue, iDvalue, dwTime);
			}
			break;

		case 6:	 // Energy Bolt
		case 10: // Lightning Arrow
			if (m_pEffectList[i]->m_cFrame >= 0)
			{	dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
				dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
				iDvalue = (m_pEffectList[i]->m_cFrame - 9)*(-1);
				if (m_pEffectList[i]->m_cFrame < 8)
					 m_pEffectSpr[0]->PutTransSprite_NoColorKey(dX, dY+30, 1, dwTime);
				else m_pEffectSpr[0]->PutTransSpriteRGB(dX, dY+30, 1, iDvalue, iDvalue, iDvalue, dwTime);
			}
			break;
		case 7: // Magic Missile Explosion
			if (m_pEffectList[i]->m_cFrame >= 0)
			{	dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
				dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
				iDvalue = (m_pEffectList[i]->m_cFrame - 2)*(-1);
				if (m_pEffectList[i]->m_cFrame < 2)
					 m_pEffectSpr[0]->PutTransSprite_NoColorKey(dX, dY+30, 1, dwTime);
				else m_pEffectSpr[0]->PutTransSpriteRGB(dX, dY+30, 1, iDvalue, iDvalue, iDvalue, dwTime);
			}
			break;

		case 16:
		case 61:
		case 66:
		case 100:
		case 110:
		case 120:
		case 130:
		case 137: // Lightning arrow
		//case 165: // APFM fixed green light
			// Light on ground below the flying effect
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			iDvalue = -5;
			m_pEffectSpr[0]->PutTransSpriteRGB(dX, dY+30, 1, iDvalue, iDvalue, iDvalue, dwTime);
			break;

		case 69:
		case 70:
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[0]->PutTransSprite25(dX, dY+30, 1, dwTime);
			break;

		case 33: //
			/*
			if (m_pEffectList[i]->m_cFrame >= 0) {
				dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
				dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
				iDvalue = (m_pEffectList[i]->m_cFrame - 9)*(-1);
				if (m_pEffectList[i]->m_cFrame < 8)
					 m_pEffectSpr[0]->PutTransSprite(dX, dY, 1, dwTime);
				else m_pEffectSpr[0]->PutTransSpriteRGB(dX, dY, 1, iDvalue, iDvalue, iDvalue, dwTime);
			}
			*/
			break;

		case 40: //
		case 56:
			if (m_pEffectList[i]->m_cFrame >= 0)
			{	dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
				dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
				iDvalue = (m_pEffectList[i]->m_cFrame - 7)*(-1);
				if (m_pEffectList[i]->m_cFrame < 6)
					 m_pEffectSpr[0]->PutTransSprite(dX, dY, 1, dwTime);
				else m_pEffectSpr[0]->PutTransSpriteRGB(dX, dY, 1, iDvalue, iDvalue, iDvalue, dwTime);
			}
			break;

		case 52: // Protection Ring
			if (m_pEffectList[i]->m_cFrame >= 0)
			{				dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
				dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
				m_pEffectSpr[24]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			}
			break;

		case 57: // Spell Load Animation xRisenx
			if (m_pEffectList[i]->m_cFrame >= 0) {
				dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
				dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
				m_pEffectSpr[30]->PutTransSprite_NoColorKey(dX-35, dY-52, m_pEffectList[i]->m_cFrame, dwTime);
			}
			break;

		case 73:
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[74]->PutTransSprite(dX, dY-34, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 74:
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[75]->PutTransSprite(dX, dY+35, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 75: // Icegolem
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[76]->PutTransSprite25(dX+m_pEffectList[i]->m_dX*m_pEffectList[i]->m_cFrame, dY+m_pEffectList[i]->m_dY*m_pEffectList[i]->m_cFrame, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 76:// Icegolem
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[77]->PutTransSprite25(dX+m_pEffectList[i]->m_dX*m_pEffectList[i]->m_cFrame, dY+m_pEffectList[i]->m_dY*m_pEffectList[i]->m_cFrame, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 77:// Icegolem
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[78]->PutTransSprite25(dX+m_pEffectList[i]->m_dX*m_pEffectList[i]->m_cFrame, dY+m_pEffectList[i]->m_dY*m_pEffectList[i]->m_cFrame, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 150: // Berserk : Cirlcle 6 magic
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			m_pEffectSpr[58]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 180: // Ilusion
		case 190: // Mass Illusion
			cTempFrame = m_pEffectList[i]->m_cFrame;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			m_pEffectSpr[59]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			//if( cTempFrame < 9 ) m_pEffectSpr[59]->PutTransSprite_NoColorKey( dX, dY, cTempFrame, dwTime );
			//else m_pEffectSpr[59]->PutSpriteFast( dX, dY, cTempFrame, dwTime );
			break;

		case 177: // Illusion mvt
		case 195: // Mass Illusion mvt
			cTempFrame = m_pEffectList[i]->m_cFrame;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			m_pEffectSpr[102]->PutTransSprite_NoColorKey(dX, dY+30, cTempFrame, dwTime);
			break;

		case 183: // Inhibition casting
			cTempFrame = m_pEffectList[i]->m_cFrame;
			dX  = (m_pEffectList[i]->m_dX*32)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_dY*32)  - m_sViewPointY;
			m_pEffectSpr[95]->PutTransSprite_NoColorKey(dX, dY+40, cTempFrame, dwTime);
			break;
		}
	}
}

void CGame::_LoadShopMenuContents(char cType)
{
 char cFileName[255], cTemp[255];
 HANDLE hFile;
 FILE * pFile;
 uint32_t  dwFileSize;
 char * pBuffer;

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));
	wsprintfA(cTemp, "contents%d", cType);
	strcat(cFileName, "data\\shops" );
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, "Shop" );
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cTemp);
	strcat(cFileName, ".txt");

	hFile = CreateFileA(cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	dwFileSize = GetFileSize(hFile, 0);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFileName, "rt");
	if (pFile == 0) return;
	else {
		pBuffer = new char[dwFileSize+1];
		ZeroMemory(pBuffer, dwFileSize+1);
		fread(pBuffer, dwFileSize, 1, pFile);

		__bDecodeContentsAndBuildItemForSaleList(pBuffer);
		delete[] pBuffer;
	}
	fclose(pFile);
}

bool CGame::__bDecodeContentsAndBuildItemForSaleList(char * pBuffer)
{
 char * pContents, * token;
 char seps[] = "= ,\t\n";
 char cReadModeA = 0;
 char cReadModeB = 0;
 int  iItemForSaleListIndex = 0;
 class CStrTok * pStrTok;

	pContents = pBuffer;

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while( token != 0 ) {
		if (cReadModeA != 0) {
			//
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					ZeroMemory(m_pItemForSaleList[iItemForSaleListIndex]->m_cName, sizeof(m_pItemForSaleList[iItemForSaleListIndex]->m_cName));
					memcpy(m_pItemForSaleList[iItemForSaleListIndex]->m_cName, token, strlen(token));
					cReadModeB = 2;
					break;
				case 2:	// m_cItemType
					m_pItemForSaleList[iItemForSaleListIndex]->m_cItemType = atoi(token);
					cReadModeB = 3;
					break;
				case 3: // m_cEquipPos
					m_pItemForSaleList[iItemForSaleListIndex]->m_cEquipPos = atoi(token);
					cReadModeB = 4;
					break;
				case 4: // m_sItemEffectType
					//m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectType = atoi(token);
					cReadModeB = 5;
					break;
				case 5:	// m_sItemEffectValue1
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue1 = atoi(token);
					cReadModeB = 6;
					break;
				case 6: // m_sItemEffectValue2
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue2 = atoi(token);
					cReadModeB = 7;
					break;
				case 7: // m_sItemEffectValue3
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue3 = atoi(token);
					cReadModeB = 8;
					break;
				case 8: // m_sItemEffectValue4
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue4 = atoi(token);
					cReadModeB = 9;
					break;
				case 9: // m_sItemEffectValue5
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue5 = atoi(token);
					cReadModeB = 10;
					break;
				case 10: // m_sItemEffectValue6
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue6 = atoi(token);
					cReadModeB = 11;
					break;
				case 11: // m_wMaxLifeSpan
					m_pItemForSaleList[iItemForSaleListIndex]->m_wMaxLifeSpan = (uint16_t)atoi(token);
					cReadModeB = 12;
					break;
				case 12: // m_sMaxFixCount
					//m_pItemForSaleList[iItemForSaleListIndex]->m_sMaxFixCount = atoi(token);
					cReadModeB = 13;
					break;
				case 13: // m_sSprite
					m_pItemForSaleList[iItemForSaleListIndex]->m_sSprite = atoi(token);
					cReadModeB = 14;
					break;
				case 14: // m_sSpriteFrame
					m_pItemForSaleList[iItemForSaleListIndex]->m_sSpriteFrame = atoi(token);
					cReadModeB = 15;
					break;
				case 15: // m_wPrice
					m_pItemForSaleList[iItemForSaleListIndex]->m_wPrice = atoi(token);
					cReadModeB = 16;
					break;
				case 16: // m_wWeight
					m_pItemForSaleList[iItemForSaleListIndex]->m_wWeight = atoi(token);
					cReadModeB = 17;
					break;
				case 17: // Appr Value
					//m_pItemForSaleList[iItemForSaleListIndex]->m_cApprValue = atoi(token);
					cReadModeB = 18;
					break;
				case 18: // m_cSpeed
					m_pItemForSaleList[iItemForSaleListIndex]->m_cSpeed = atoi(token);
					cReadModeB = 19;
					break;
				case 19: // Level Limit
					m_pItemForSaleList[iItemForSaleListIndex]->m_sLevelLimit = atoi(token);
					m_pItemForSaleList[iItemForSaleListIndex]->m_dwCount = 1;
					cReadModeA = 0;
					cReadModeB = 0;
					iItemForSaleListIndex++;
					break;
				}
				break;

			default:
				break;
			}
		}else
		{	if (memcmp(token, "ItemForSale", 4) == 0)
			{	if (iItemForSaleListIndex >= MAXMENUITEMS)
				{	delete pStrTok;
					return false;
				}
				cReadModeA = 1;
				cReadModeB = 1;
				m_pItemForSaleList[iItemForSaleListIndex] = new class CItem;
		}	}
		token = pStrTok->pGet();
	}
	delete pStrTok;
	if ((cReadModeA != 0) || (cReadModeB != 0)) return false;
	return true;
}

static char __cSpace[] = {8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8, 8,6,8,7,8,8,9,10,9,7, 8,8,8,8,8, 8,8,
                          15,16,12,17,14,15,14,16,10,13, 19,10,17,17,15,14,15,16,13,17, 16,16,20,17,16,14,
	       			      8,8,8,8,8,8,	8,6,7,8,7,7,7,7,4,7,7,  4,11,7,8,8,7,8,6,5,8,9,14,8,9,8, 8,8,8,8,
				          8,8,8,8,8,8,8};
void CGame::PutString_SprFont(int iX, int iY, char * pStr, short sR, short sG, short sB)
{
 int iXpos;
 uint32_t iCnt;
 uint32_t dwTime = G_dwGlobalTime;
 char  cTmpStr[100];

	ZeroMemory(cTmpStr, sizeof(cTmpStr));
	strcpy(cTmpStr, pStr);
	iXpos = iX;
	for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
		if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122)) {
			m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteRGB(iXpos+1, iY, cTmpStr[iCnt] - 33, sR+11, sG+7, sB+6, dwTime);
			if ((sR == 0) && (sG == 0) && (sB == 0))
				 m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 33, dwTime);
			else m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 33, sR, sG, sB, dwTime);
			 iXpos += __cSpace[cTmpStr[iCnt] - 33];
		}
		else iXpos += 5;
	}
}

void CGame::PutString_SprFont2(int iX, int iY, char * pStr, short sR, short sG, short sB)
{
 int iXpos, iR, iG, iB;
 uint32_t iCnt;
 uint32_t dwTime = G_dwGlobalTime;
 char  cTmpStr[200];

	//DIRECTX m_DDraw.ColorTransferRGB(RGB(sR, sG, sB), &iR, &iG, &iB);

	ZeroMemory(cTmpStr, sizeof(cTmpStr));
	strcpy(cTmpStr, pStr);

	iXpos = iX;
	for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
		if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122)) {
			m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos+1, iY, cTmpStr[iCnt] - 33, dwTime);
			m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos+1, iY+1, cTmpStr[iCnt] - 33, dwTime);
			if ((sR == 0) && (sG == 0) && (sB == 0))
				 m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 33, dwTime);
			else m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 33, iR, iG, iB, dwTime);
			 iXpos += __cSpace[cTmpStr[iCnt] - 33];
		}
		else iXpos += 5;
	}
}

void CGame::PutString_SprFont3(int iX, int iY, char * pStr, short sR, short sG, short sB, bool bTrans, int iType)
{
 int iXpos, iAdd;
 uint32_t iCnt;
 uint32_t dwTime = G_dwGlobalTime;
 char  cTmpStr[128];

	ZeroMemory(cTmpStr, sizeof(cTmpStr));
	strcpy(cTmpStr, pStr);

	if (iType != -1) {
		iAdd = 95*iType;
		iXpos = iX;
		for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
			if ((cTmpStr[iCnt] >= 32) && (cTmpStr[iCnt] <= 126)) {

				if (bTrans == false) {
					m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteFast(iXpos, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
					m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteFast(iXpos + 1, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
					if ((sR == 0) && (sG == 0) && (sB == 0))
						 m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 32 +iAdd, dwTime);
					else m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 +iAdd, sR, sG, sB, dwTime);

				}
				else m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutTransSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 +iAdd, sR, sG, sB, dwTime);

				iXpos += (m_pSprite[SPRID_INTERFACE_SPRFONTS2]->m_stBrush[cTmpStr[iCnt] - 32 +iAdd].szx);
			}
			else iXpos += 5;
		}
	}
	else {
		iAdd = 0;
		iXpos = iX;
		for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
			if ((cTmpStr[iCnt] >= 32) && (cTmpStr[iCnt] <= 126)) {

				if (bTrans == false) {
					m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteFast(iXpos, iY+1, cTmpStr[iCnt] - 32 +iAdd, dwTime);
					m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteFast(iXpos+1, iY+1, cTmpStr[iCnt] - 32 +iAdd, dwTime);
					if ((sR == 0) && (sG == 0) && (sB == 0))
						 m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 32 +iAdd, dwTime);
					else m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 +iAdd, sR, sG, sB, dwTime);

				}
				else m_pSprite[SPRID_INTERFACE_FONT2]->PutTransSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 +iAdd, sR, sG, sB, dwTime);

				iXpos += (m_pSprite[SPRID_INTERFACE_FONT2]->m_stBrush[cTmpStr[iCnt] - 32 +iAdd].szx);
			}
			else iXpos += 5;
		}
	}
}

static char __cSpace2[] = {6,4,6,6,6,6,6,6,6,6,6}; //{8,6,9,8,8,9,8,8,8,8};
void CGame::PutString_SprNum(int iX, int iY, char * pStr, short sR, short sG, short sB)
{
	int iXpos;
	unsigned char iCnt;
	uint32_t dwTime = G_dwGlobalTime;
	char  cTmpStr[200];
	ZeroMemory(cTmpStr, sizeof(cTmpStr));
	strcpy(cTmpStr, pStr);
	//m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,sR, sG, sB), &wR, &wG, &wB);
	iXpos = iX;
	for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
	{
		if ((cTmpStr[iCnt] >= 0x30) && (cTmpStr[iCnt] <= 0x39))
		{
			m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSprite(iXpos+2, iY, cTmpStr[iCnt] - 0x30 +6, dwTime);
			m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSprite(iXpos+1, iY+1, cTmpStr[iCnt] - 0x30 +6, dwTime);
			if ((sR == 0) && (sG == 0) && (sB == 0))
				 m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSprite(iXpos, iY, cTmpStr[iCnt] - 0x30 +6, dwTime);
			else m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 0x30 +6, sR, sG, sB, dwTime);
			iXpos += __cSpace2[cTmpStr[iCnt] - 0x30];
		}
	}
}

void CGame::PutString(int iX, int iY, char const * pString, video::SColor color, bool bHide, char cBGtype, bool bIsPreDC)
{
	char * pTmp;
 int i;
	if (strlen(pString) == 0) return;
	//if (bIsPreDC == FALSE) //DIRECTX m_DDraw._GetBackBufferDC();
	if (bHide == false)
	{
		switch (cBGtype)
		{
		case 0:
			PutString(iX+1, iY, (char*)pString, color);
			//DIRECTX m_DDraw.TextOut(iX+1, iY, pString, color);
			break;
		case 1:
			PutString(iX, iY+1, (char*)pString, video::SColor(255,5,5,5));
			PutString(iX+1, iY+1, (char*)pString, video::SColor(255,5,5,5));
			PutString(iX+1, iY, (char*)pString, video::SColor(255,5,5,5));
			//DIRECTX m_DDraw.TextOut(iX, iY+1, pString,video::SColor(255,5,5,5));
			//DIRECTX m_DDraw.TextOut(iX+1, iY+1, pString,video::SColor(255,5,5,5));
			//DIRECTX m_DDraw.TextOut(iX+1, iY, pString,video::SColor(255,5,5,5));
			break;
		}
		PutString(iX, iY, (char*)pString, color);
		//DIRECTX m_DDraw.TextOut(iX, iY, pString, color);
	}
	else
	{
		pTmp = new char[strlen(pString)+2];
		ZeroMemory(pTmp, strlen(pString)+2);
		strcpy(pTmp, pString);
		for (i = 0; i < (int)strlen(pString); i++)
			if (pTmp[i] != 0) pTmp[i] = '*';

		switch (cBGtype)
		{
		case 0:
			PutString(iX+1, iY, (char*)pString, color);
			//DIRECTX m_DDraw.TextOut(iX+1, iY, pString, color);
			break;
		case 1:
			PutString(iX, iY+1, (char*)pString, video::SColor(255,5,5,5));
			PutString(iX+1, iY+1, (char*)pString, video::SColor(255,5,5,5));
			PutString(iX+1, iY, (char*)pString, video::SColor(255,5,5,5));
			//DIRECTX m_DDraw.TextOut(iX, iY+1, pString,video::SColor(255,5,5,5));
			//DIRECTX m_DDraw.TextOut(iX+1, iY+1, pString,video::SColor(255,5,5,5));
			//DIRECTX m_DDraw.TextOut(iX+1, iY, pString,video::SColor(255,5,5,5));
			break;
		}
		PutString(iX, iY, (char*)pString, color);
		//DIRECTX m_DDraw.TextOut(iX, iY, pTmp, color);
		delete[] pTmp;
	}
	//if (bIsPreDC == FALSE) //DIRECTX m_DDraw._ReleaseBackBufferDC();
}
void CGame::PutFontString(gui::IGUIFont * font, int iX, int iY, char const * pString, video::SColor color, bool bHide, char cBGtype, bool bIsPreDC)
{
	char * pTmp;
	int i;
	if (strlen(pString) == 0) return;
	if (bHide == false)
	{
		switch (cBGtype)
		{
		case 0:
			PutFontString(font, iX+1, iY, (char*)pString, color);
			break;
		case 1:
			PutFontString(font, iX, iY+1, (char*)pString, video::SColor(255,5,5,5));
			PutFontString(font, iX+1, iY+1, (char*)pString, video::SColor(255,5,5,5));
			PutFontString(font, iX+1, iY, (char*)pString, video::SColor(255,5,5,5));
			break;
		}
		PutFontString(font, iX, iY, (char*)pString, color);
	}
	else
	{
		pTmp = new char[strlen(pString)+2];
		ZeroMemory(pTmp, strlen(pString)+2);
		strcpy(pTmp, pString);
		for (i = 0; i < (int)strlen(pString); i++)
			if (pTmp[i] != 0) pTmp[i] = '*';

		switch (cBGtype)
		{
		case 0:
			PutFontString(font, iX+1, iY, (char*)pString, color);
			break;
		case 1:
			PutFontString(font, iX, iY+1, (char*)pString, video::SColor(255,5,5,5));
			PutFontString(font, iX+1, iY+1, (char*)pString, video::SColor(255,5,5,5));
			PutFontString(font, iX+1, iY, (char*)pString, video::SColor(255,5,5,5));
			break;
		}
		PutFontString(font, iX, iY, (char*)pString, color);
		delete[] pTmp;
	}
}
void CGame::PutChatString(int iX, int iY, char * pString, video::SColor color)
{
	font[FONT_TREBMS8PX]->draw(pString,core::rect<s32>(iX,iY,iX+40,iY+10),video::SColor(color));
//	PutFontString(font[1], iX, iY, pString, color);
}
void CGame::PutFontString(gui::IGUIFont * font, int iX, int iY, char * pString, video::SColor color)
{
	font->draw(pString,core::rect<s32>(iX,iY,iX+40,iY+10),(color));
}
void CGame::PutString(int iX, int iY, char const * pString, video::SColor color)
{
	font[FONT_TREBMS8PX]->draw(pString,core::rect<s32>(iX,iY,iX+40,iY+10),video::SColor(color));

	//DIRECTX m_DDraw._GetBackBufferDC();
	//DIRECTX m_DDraw.TextOut(iX, iY, pString, color);
	//DIRECTX m_DDraw._ReleaseBackBufferDC();
}

void CGame::PutString2(int iX, int iY, char * pString, short sR, short sG, short sB)
{
	font[FONT_TREBMS8PX]->draw(pString,core::rect<s32>(iX+1,iY+1,iX+40+1,iY+10+1),video::SColor(255, 0, 0, 0));
	font[FONT_TREBMS8PX]->draw(pString,core::rect<s32>(iX,iY,iX+40,iY+10),video::SColor(255,sR,sG,sB));
	//DIRECTX m_DDraw._GetBackBufferDC();
	//DIRECTX m_DDraw.TextOut(iX+1, iY, pString,video::SColor(255,0, 0, 0));
	//DIRECTX m_DDraw.TextOut(iX, iY+1, pString,video::SColor(255,0, 0, 0));
	//DIRECTX m_DDraw.TextOut(iX+1, iY+1, pString,video::SColor(255,0, 0, 0));
	//DIRECTX m_DDraw.TextOut(iX, iY, pString,video::SColor(255,sR, sG, sB));
	//DIRECTX m_DDraw._ReleaseBackBufferDC();
}

void CGame::PutString3(int iX, int iY, char const * pString, video::SColor color)
{
	font[FONT_TREBMS8PX]->draw(pString,core::rect<s32>(iX,iY,iX+40,iY+10),color);
	//DIRECTX m_DDraw._GetBackBufferDC();
	//DIRECTX m_DDraw.TextOut(iX, iY, pString, color);
	//DIRECTX m_DDraw.TextOut(iX+1, iY, pString, color);
	//DIRECTX m_DDraw._ReleaseBackBufferDC();
}

void CGame::PutAlignedString(int iX1, int iX2, int iY, char const * const pString, video::SColor color)
{
	font[FONT_TREBMS8PX]->draw(pString,core::rect<s32>(iX1,iY,iX2+40,iY+10),color);
// 	RECT rt;
// 	m_DDraw._GetBackBufferDC();
// 	SetRect(&rt, iX1, iY, iX2, iY+15);
// 	m_DDraw.DrawText(&rt, pString, RGB(sR, sG, sB));
// 	m_DDraw._ReleaseBackBufferDC();
}

bool CGame::bInitMagicCfgList()
{
 char cFn[255], cTemp[255];
 char * pContents, * token;
 char seps[] = "= ,\t\n";
 char cReadModeA = 0;
 char cReadModeB = 0;
 int  iMagicCfgListIndex = 0;
 class CStrTok * pStrTok;
 HANDLE hFile;
 FILE * pFile;
 uint32_t  dwFileSize;

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFn, sizeof(cFn));

	// CLEROTH - MAGIC CFG
	strcpy(cTemp, "magiccfg.txt");

	strcat(cFn, "data\\shops");
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	strcat(cFn, cTemp);

	hFile = CreateFileA(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	dwFileSize = GetFileSize(hFile, 0);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFn, "rt");
	if (pFile == 0) return false;
	else {
		pContents = new char[dwFileSize+1];
		ZeroMemory(pContents, dwFileSize+1);
		fread(pContents, dwFileSize, 1, pFile);
		fclose(pFile);
	}

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while( token != 0 ) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					if (_bGetIsStringIsNumber(token) == false)
					{	delete[] pContents;
						delete pStrTok;
						return false;
					}
					if (m_pMagicCfgList[atoi(token)] != 0)
					{	delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicCfgList[atoi(token)] = new class CMagic;
					iMagicCfgListIndex = atoi(token);

					cReadModeB = 2;
					break;

				case 2:
					ZeroMemory(m_pMagicCfgList[iMagicCfgListIndex]->m_cName, sizeof(m_pMagicCfgList[iMagicCfgListIndex]->m_cName));
					memcpy(m_pMagicCfgList[iMagicCfgListIndex]->m_cName, token, strlen(token));
					cReadModeB = 3;
					break;

				case 3: // m_sValue1
					if (_bGetIsStringIsNumber(token) == false) {
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicCfgList[iMagicCfgListIndex]->m_sValue1 = atoi(token);
					cReadModeB = 4;
					break;

				case 4: // m_sValue2	// INT
					if (_bGetIsStringIsNumber(token) == false) {
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicCfgList[iMagicCfgListIndex]->m_sValue2 = atoi(token);
					cReadModeB = 5;
					break;

				case 5: // m_sValue3	// COST
					if (_bGetIsStringIsNumber(token) == false) {
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicCfgList[iMagicCfgListIndex]->m_sValue3 = atoi(token);
					cReadModeB = 6;
					break;

				// CLEROTH MAGIC CFG
				case 6: // m_sValue4	// STR
					if (_bGetIsStringIsNumber(token) == false)
					{	delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pMagicCfgList[iMagicCfgListIndex]->m_sValue4 = atoi(token);
					cReadModeB = 7;
					break;

				case 7: // m_sValue5
					cReadModeB = 8;
					break;

				case 8: // m_sValue6
					cReadModeB = 9;
					break;

				case 9: // m_bIsVisible
					if (_bGetIsStringIsNumber(token) == false)
					{	delete[] pContents;
						delete pStrTok;
						return false;
					}
					if( atoi(token) == 0 ) m_pMagicCfgList[iMagicCfgListIndex]->m_bIsVisible = false;
					else m_pMagicCfgList[iMagicCfgListIndex]->m_bIsVisible = true;
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				}
				break;

			default:
				break;
			}
		}
		else {
			if (memcmp(token, "magic", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		return false;
	}

	return true;
}

bool CGame::bCheckImportantFile()
{	HANDLE hFile;

	hFile = CreateFileA("SPRITES\\TREES1.PAK", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if( hFile == INVALID_HANDLE_VALUE ) return false;

	// FileSize : 1846406.... Anti Tree1.pak hack....inutile ca peut se modifier sans changer la taille!
	if( GetFileSize( hFile, 0 ) != 2945524 )//2,949,120 (2,945,524 b
	{	
		CloseHandle( hFile );
		return false;
	}

	CloseHandle( hFile );
	return true;
}

bool CGame::bInitSkillCfgList()
{
 char cFn[255], cTemp[255];
 char * pContents, * token;
 char seps[] = "= ,\t\n";
 char cReadModeA = 0;
 char cReadModeB = 0;
 int  iSkillCfgListIndex = 0;
 class CStrTok * pStrTok;
 HANDLE hFile;
 FILE * pFile;
 uint32_t  dwFileSize;

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFn, sizeof(cFn));

	strcpy(cTemp, "Skillcfg.txt");
	strcat(cFn, "data\\shops");
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	strcat(cFn, cTemp);

	hFile = CreateFileA(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	dwFileSize = GetFileSize(hFile, 0);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFn, "rt");
	if (pFile == 0) return false;
	else {
		pContents = new char[dwFileSize+1];
		ZeroMemory(pContents, dwFileSize+1);
		fread(pContents, dwFileSize, 1, pFile);
		fclose(pFile);
	}

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while( token != 0 ) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					if (_bGetIsStringIsNumber(token) == false)
					{	delete[] pContents;
						delete pStrTok;
						return false;
					}
					if (m_pSkillCfgList[atoi(token)] != 0)
					{	delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pSkillCfgList[atoi(token)] = new class CSkill;
					iSkillCfgListIndex = atoi(token);
					cReadModeB = 2;
					break;

				case 2:
					ZeroMemory(m_pSkillCfgList[iSkillCfgListIndex]->m_cName, sizeof(m_pSkillCfgList[iSkillCfgListIndex]->m_cName));
					memcpy(m_pSkillCfgList[iSkillCfgListIndex]->m_cName, token, strlen(token));
					cReadModeB = 3;
					break;

				case 3: // m_bIsUseable
					if (_bGetIsStringIsNumber(token) == false) {
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pSkillCfgList[iSkillCfgListIndex]->m_bIsUseable = (bool)atoi(token);
					cReadModeB = 4;
					break;

				case 4: // m_cUseMethod
					if (_bGetIsStringIsNumber(token) == false) {
						delete[] pContents;
						delete pStrTok;
						return false;
					}
					m_pSkillCfgList[iSkillCfgListIndex]->m_cUseMethod = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				}
				break;

			default:
				break;
			}
		}
		else {
			if (memcmp(token, "skill", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		return false;
	}

	return true;
}

bool CGame::_bGetIsStringIsNumber(char * pStr)
{
 int i;
	for (i = 0; i < (int)strlen(pStr); i++)
	if ((pStr[i] != '-') && ((pStr[i] < (char)'0') || (pStr[i] > (char)'9'))) return false;

	return true;
}

void CGame::RequestFullObjectData(uint16_t wObjectID)
{
 char    cMsg[256];
 int     iRet;
 uint32_t * dwp;
 uint16_t  * wp;

	ZeroMemory(cMsg, sizeof(cMsg));

	dwp = (uint32_t *)(cMsg + INDEX4_MSGID);
	*dwp = MSGID_REQUEST_FULLOBJECTDATA;
	wp  = (uint16_t *)(cMsg + INDEX2_MSGTYPE);
	*wp = wObjectID;

	_socket->write(cMsg, 6);
}

void CGame::DrawStatusText(int sX, int sY)
{
	if (m_altPressed)
	{
		wsprintfA(G_cTxt,"Appr1: 0x%.4X ",(uint16_t)_tmp_sAppr1);
		PutString2(sX+70, sY+(14*1), G_cTxt, 30,255,30);
		wsprintfA(G_cTxt,"Appr2: 0x%.4X ",(uint16_t)_tmp_sAppr2);
		PutString2(sX+70, sY+(14*2), G_cTxt, 30,255,30);
		wsprintfA(G_cTxt,"Appr3: 0x%.4X ",(uint16_t)_tmp_sAppr3);
		PutString2(sX+70, sY+(14*3), G_cTxt, 30,255,30);
		wsprintfA(G_cTxt,"Appr4: 0x%.4X ",(uint16_t)_tmp_sAppr4);
		PutString2(sX+70, sY+(14*4), G_cTxt, 30,255,30);
		wsprintfA(G_cTxt,"HeadAppr: 0x%.4X ",(uint16_t)_tmp_sHeadApprValue); // Re-Coding Sprite xRisenx
		PutString2(sX+70, sY+(14*5), G_cTxt, 30,255,30);
		wsprintfA(G_cTxt,"BodyAppr: 0x%.4X ",(uint16_t)_tmp_sBodyApprValue); // Re-Coding Sprite xRisenx
		PutString2(sX+70, sY+(14*6), G_cTxt, 30,255,30);
		wsprintfA(G_cTxt,"ArmAppr: 0x%.4X ",(uint16_t)_tmp_sArmApprValue); // Re-Coding Sprite xRisenx
		PutString2(sX+70, sY+(14*7), G_cTxt, 30,255,30);
		wsprintfA(G_cTxt,"LegAppr: 0x%.4X ",(uint16_t)_tmp_sLegApprValue); // Re-Coding Sprite xRisenx
		PutString2(sX+70, sY+(14*8), G_cTxt, 30,255,30);
	}
}

void CGame::_ReadMapData(short sPivotX, short sPivotY, char * pData)
{
	int i;
	char  * cp, ucHeader, cDir, cName[12], cItemColor;
	short * sp, sTotal, sX, sY, sType, sAppr1, sAppr2, sAppr3, sAppr4, sSprite, sSpriteFrame, sItemSpr, sItemSprFrame, sDynamicObjectType, sHeadApprValue, sBodyApprValue, sArmApprValue, sLegApprValue; // Re-Coding Sprite xRisenx
	int iStatus;
	int   * ip, iApprColor;
	uint16_t    wObjectID;
	uint16_t  * wp, wDynamicObjectID;

	cp = pData;
	m_sVDL_X = sPivotX;
	m_sVDL_Y = sPivotY;
	sp = (short *)cp;
	sTotal = *sp;
	cp += 2;
	for (i = 1;	i <= sTotal; i++)
	{
		sp = (short *)cp;
		sX = *sp;
		cp += 2;
		sp = (short *)cp;
		sY = *sp;
		cp += 2;
		ucHeader = *cp;
		cp++;
		if (ucHeader & 0x01) // object ID
		{
			wp  = (uint16_t *)cp;
			wObjectID = *wp;
			cp += 2;
			sp  = (short *)cp;
			sType = *sp;// object type
			cp += 2;
			// dir
			cDir = *cp;
			cp++;
			if (wObjectID < 10000)
			{	
				sp  = (short *)cp;
				sAppr1 = *sp;// Appearance1
				cp += 2;
				sp  = (short *)cp;
				sAppr2 = *sp;// Appearance2
				cp += 2;
				sp  = (short *)cp;
				sAppr3 = *sp;// Appearance3
				cp += 2;
				sp  = (short *)cp;
				sAppr4 = *sp;// Appearance4
				cp += 2;
				ip = (int *)cp;
				iApprColor = *ip;
				cp += 4;
				sp  = (short *)cp;
				sHeadApprValue = *sp; // Re-Coding Sprite xRisenx
				cp += 2;
				sp  = (short *)cp;
				sBodyApprValue = *sp; // Re-Coding Sprite xRisenx
				cp += 2;
				sp  = (short *)cp;
				sArmApprValue = *sp; // Re-Coding Sprite xRisenx
				cp += 2;
				sp  = (short *)cp;
				sLegApprValue = *sp; // Re-Coding Sprite xRisenx
				cp += 2;
				ip  = (int *)cp;
				iStatus = *ip;
				cp += 4;
				// Name
				ZeroMemory(cName, sizeof(cName));
				memcpy(cName, cp, 10);
				cp    += 10;
			}
			else // NPC
			{	
				//sAppr1 = sAppr3 = sAppr4 = 0;
				sAppr1 = sAppr3 = sAppr4 = sHeadApprValue = sBodyApprValue = sArmApprValue = sLegApprValue = 0; // Re-Coding Sprite xRisenx
				sp  = (short *)cp;
				sAppr2 = *sp;// Appearance2
				cp += 2;
				ip  = (int *)cp;
				iStatus = *ip;
				cp += 4;
				ZeroMemory(cName, sizeof(cName));
				sprintf(cName, "npc%d", wObjectID - 10000);
			}
			m_pMapData->bSetOwner(wObjectID, /*sPivotX + */sX, /*sPivotY + */sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, sHeadApprValue, sBodyApprValue, sArmApprValue, sLegApprValue, iStatus, cName, OBJECTSTOP, 0, 0, 0);
		}
		if (ucHeader & 0x02) // object ID
		{	
			wp  = (uint16_t *)cp;
			wObjectID = *wp;
			cp += 2;
			sp  = (short *)cp;
			sType = *sp;	// object type
			cp += 2;
			cDir = *cp;	// dir
			cp++;
			if (wObjectID < 10000)
			{	
				sp  = (short *)cp;
				sAppr1 = *sp;// Appearance1
				cp += 2;
				sp  = (short *)cp;
				sAppr2 = *sp;// Appearance2
				cp += 2;
				sp  = (short *)cp;
				sAppr3 = *sp;// Appearance3
				cp += 2;
				sp  = (short *)cp;
				sAppr4 = *sp;// Appearance4
				cp += 2;
				ip = (int *)cp;
				iApprColor = *ip;
				cp += 4;
				sp  = (short *)cp;
				sHeadApprValue = *sp; // Re-Coding Sprite xRisenx
				cp += 2;
				sp  = (short *)cp;
				sBodyApprValue = *sp; // Re-Coding Sprite xRisenx
				cp += 2;
				sp  = (short *)cp;
				sArmApprValue = *sp; // Re-Coding Sprite xRisenx
				cp += 2;
				sp  = (short *)cp;
				sLegApprValue = *sp; // Re-Coding Sprite xRisenx
				cp += 2;
				// CLEROTH - CRASH BUG ( STATUS )
				// Status
				ip  = (int *)cp;
				iStatus = *ip;
				cp += 4;
				// Name
				ZeroMemory(cName, sizeof(cName));
				memcpy(cName, cp, 10);
				cp    += 10;
			}
			else 	// NPC
			{	
				//sAppr1 = sAppr3 = sAppr4 = 0;
				sAppr1 = sAppr3 = sAppr4 = sHeadApprValue = sBodyApprValue = sArmApprValue = sLegApprValue = 0; // Re-Coding Sprite xRisenx
				sp  = (short *)cp;
				sAppr2 = *sp;// Appearance2
				cp += 2;
				ip  = (int *)cp;
				iStatus = *ip;// Status
				cp += 4;
				ZeroMemory(cName, sizeof(cName));
				sprintf(cName, "npc%d", wObjectID - 10000);
			}
			m_pMapData->bSetDeadOwner(wObjectID, /*sPivotX + */sX, /*sPivotY + */sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, sHeadApprValue, sBodyApprValue, sArmApprValue, sLegApprValue, iStatus, cName);
		}
		if (ucHeader & 0x04)
		{
			sp  = (short *)cp;
			sItemSpr = *sp;
			cp += 2;
			sp  = (short *)cp;
			sItemSprFrame = *sp;
			cp += 2;
			ip = (int *)cp;
			cItemColor = *ip;
			cp += 4;
			m_pMapData->bSetItem(/*sPivotX + */sX, /*sPivotY + */sY, sItemSpr, sItemSprFrame, cItemColor, false);
		}
		if (ucHeader & 0x08) // Dynamic object
		{
			wp = (uint16_t *)cp;
			wDynamicObjectID = *wp;
			cp += 2;
			sp  = (short *)cp;
			sDynamicObjectType = *sp;
			cp += 2;
			m_pMapData->bSetDynamicObject(/*sPivotX + */sX, /*sPivotY + */sY, wDynamicObjectID, sDynamicObjectType, false);
		}
	}
}

void CGame::LogResponseHandler(uint32_t size, char * pData)
{
	StreamRead sr(pData, size);

	uint16_t wResponse;
	uint16_t wServerUpperVersion, wServerLowerVersion;
	string charname;

	sr.ReadInt();
	wResponse = sr.ReadShort();

	switch (wResponse)
	{
	case LOGRESMSGTYPE_CHARACTERDELETED:
		/*m_iAccountStatus = (int)*/ sr.ReadByte();
		m_iTotalChar = (int)sr.ReadByte();
		m_pCharList.clear();

		if (m_iTotalChar > 10) //error
		{
			m_iTotalChar = 10;//
		}

		for (int i = 0; i < m_iTotalChar; i++)
		{
			shared_ptr<CCharInfo> character(new CCharInfo());
			character->m_cName = sr.ReadString(10);
			if (sr.ReadByte() == 0)
			{
				character->m_sSex = 0;
				sr.position += 39;
			}
			else
			{
				character->m_sAppr1 = sr.ReadShort();
				character->m_sAppr2 = sr.ReadShort();
				character->m_sAppr3 = sr.ReadShort();
				character->m_sAppr4 = sr.ReadShort();
				character->m_sHeadApprValue = sr.ReadShort();
				character->m_sBodyApprValue = sr.ReadShort();
				character->m_sArmApprValue = sr.ReadShort();
				character->m_sLegApprValue = sr.ReadShort();
				character->m_sSex = sr.ReadShort();
				character->m_sSkinCol = sr.ReadShort();
				character->m_sLevel = sr.ReadShort();
				character->m_iExp = sr.ReadInt64();
				character->m_sStr = sr.ReadShort();
				character->m_sVit = sr.ReadShort();
				character->m_sDex = sr.ReadShort();
				character->m_sInt = sr.ReadShort();
				character->m_sMag = sr.ReadShort();
				character->m_sChr = sr.ReadShort();
				character->m_iApprColor = sr.ReadInt();
				character->m_iYear = sr.ReadShort();
				character->m_iMonth = sr.ReadShort();
				character->m_iDay = sr.ReadShort();
				character->m_iHour = sr.ReadShort();
				character->m_iMinute = sr.ReadShort();
				character->m_cMapName = sr.ReadString(10);
			}
			m_pCharList.push_back(character);
		}
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "3A");
		break;

	case LOGRESMSGTYPE_CONFIRM:
        htmlUI->Emit("login", true, "");
        loggedin = true;
		wServerUpperVersion = sr.ReadShort();
		wServerLowerVersion = sr.ReadShort();
		/*m_iAccountStatus = (int)*/ sr.ReadByte();
		m_iAccntYear = sr.ReadShort();
		m_iAccntMonth = sr.ReadShort();
		m_iAccntDay = sr.ReadShort();
		m_iIpYear = sr.ReadShort();
		m_iIpMonth = sr.ReadShort();
		m_iIpDay = sr.ReadShort();
		m_iTotalChar = (int)sr.ReadByte();
		m_pCharList.clear();

		if (m_iTotalChar > 10) //error
		{
			m_iTotalChar = 10;//
		}

		for (int i = 0; i < m_iTotalChar; i++)
		{
			shared_ptr<CCharInfo> character(new CCharInfo());
			character->m_cName = sr.ReadString(10);
			if (sr.ReadByte() == 0)
			{
				character->m_sSex = 0;
				sr.position += 39;
			}
			else
			{
				character->m_sAppr1 = sr.ReadShort();
				character->m_sAppr2 = sr.ReadShort();
				character->m_sAppr3 = sr.ReadShort();
				character->m_sAppr4 = sr.ReadShort();
				character->m_sHeadApprValue = sr.ReadShort();
				character->m_sBodyApprValue = sr.ReadShort();
				character->m_sArmApprValue = sr.ReadShort();
				character->m_sLegApprValue = sr.ReadShort();
				character->m_sSex = sr.ReadShort();
				character->m_sSkinCol = sr.ReadShort();
				character->m_sLevel = sr.ReadShort();
				character->m_iExp = sr.ReadInt64();
				character->m_sStr = sr.ReadShort();
				character->m_sVit = sr.ReadShort();
				character->m_sDex = sr.ReadShort();
				character->m_sInt = sr.ReadShort();
				character->m_sMag = sr.ReadShort();
				character->m_sChr = sr.ReadShort();
				character->m_iApprColor = sr.ReadInt();
				character->m_iYear = sr.ReadShort();
				character->m_iMonth = sr.ReadShort();
				character->m_iDay = sr.ReadShort();
				character->m_iHour = sr.ReadShort();
				character->m_iMinute = sr.ReadShort();
				character->m_cMapName = sr.ReadString(10);
			}
			m_pCharList.push_back(character);
		}
		m_iTimeLeftSecAccount = sr.ReadInt();
		m_iTimeLeftSecIP = sr.ReadInt();
		ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
		ClearContents_OnSelectCharacter();

        htmlUI->SetCharacters();

// #ifndef _DEBUG
// 		if ( (wServerUpperVersion!=UPPER_VERSION) || (wServerLowerVersion!=LOWER_VERSION) )
// 			ChangeGameMode(GAMEMODE_ONVERSIONNOTMATCH);
// #endif
		break;

	case LOGRESMSGTYPE_REJECT:
		m_iBlockYear = sr.ReadInt();
		m_iBlockMonth = sr.ReadInt();
		m_iBlockDay = sr.ReadInt();
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "7H");
		break;

	case LOGRESMSGTYPE_NOTENOUGHPOINT:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "7I");
		break;

	case LOGRESMSGTYPE_ACCOUNTLOCKED:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "7K");
		break;

	case LOGRESMSGTYPE_SERVICENOTAVAILABLE:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "7L");
		break;

	case LOGRESMSGTYPE_PASSWORDCHANGESUCCESS:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "6B");
		break;

	case LOGRESMSGTYPE_PASSWORDCHANGEFAIL:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "6C");
		break;

	case LOGRESMSGTYPE_PASSWORDMISMATCH:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "11");
		break;

	case LOGRESMSGTYPE_NOTEXISTINGACCOUNT:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "12");
		break;

	case LOGRESMSGTYPE_NEWACCOUNTCREATED:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "54");
		break;

	case LOGRESMSGTYPE_NEWACCOUNTFAILED:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "05");
		break;

	case LOGRESMSGTYPE_ALREADYEXISTINGACCOUNT:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "06");
		break;

	case LOGRESMSGTYPE_NOTEXISTINGCHARACTER:
		ChangeGameMode(GAMEMODE_ONMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "Not existing character!");
		break;

	case LOGRESMSGTYPE_NEWCHARACTERCREATED:
	{
		string cCharName = sr.ReadString(10);

		m_iTotalChar = (int)sr.ReadByte();
		m_pCharList.clear();

		if (m_iTotalChar > 10) //error
		{
			m_iTotalChar = 10;//
		}

		for (int i = 0; i < m_iTotalChar; i++)
		{
			shared_ptr<CCharInfo> character(new CCharInfo());
			character->m_cName = sr.ReadString(10);
			if (sr.ReadByte() == 0)
			{
				character->m_sSex = 0;
				sr.position += 39;
			}
			else
			{
				character->m_sAppr1 = sr.ReadShort();
				character->m_sAppr2 = sr.ReadShort();
				character->m_sAppr3 = sr.ReadShort();
				character->m_sAppr4 = sr.ReadShort();
				character->m_sHeadApprValue = sr.ReadShort();
				character->m_sBodyApprValue = sr.ReadShort();
				character->m_sArmApprValue = sr.ReadShort();
				character->m_sLegApprValue = sr.ReadShort();
				character->m_sSex = sr.ReadShort();
				character->m_sSkinCol = sr.ReadShort();
				character->m_sLevel = sr.ReadShort();
				character->m_iExp = sr.ReadInt64();
				character->m_sStr = sr.ReadShort();
				character->m_sVit = sr.ReadShort();
				character->m_sDex = sr.ReadShort();
				character->m_sInt = sr.ReadShort();
				character->m_sMag = sr.ReadShort();
				character->m_sChr = sr.ReadShort();
				character->m_iApprColor = sr.ReadInt();
				character->m_iYear = sr.ReadShort();
				character->m_iMonth = sr.ReadShort();
				character->m_iDay = sr.ReadShort();
				character->m_iHour = sr.ReadShort();
				character->m_iMinute = sr.ReadShort();
				character->m_cMapName = sr.ReadString(10);
			}
			m_pCharList.push_back(character);
		}
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "47");
		break;
	}

	case LOGRESMSGTYPE_NEWCHARACTERFAILED:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "28");
		break;

	case LOGRESMSGTYPE_ALREADYEXISTINGCHARACTER:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "29");
		break;

	case ENTERGAMERESTYPE_PLAYING:
		ChangeGameMode(GAMEMODE_ONQUERYFORCELOGIN);
		break;

	case ENTERGAMERESTYPE_CONFIRM:
		{
			int iGameServerPort;
			char cGameServerAddr[16];
			ZeroMemory(cGameServerAddr, sizeof(cGameServerAddr));
			string gameserveraddress = sr.ReadString(16);
			memcpy(cGameServerAddr, gameserveraddress.c_str(), (gameserveraddress.length() < 16) ? gameserveraddress.length() : 16);
			iGameServerPort = sr.ReadShort();
			ZeroMemory(m_cGameServerName, sizeof(m_cGameServerName));
			string gameservername= sr.ReadString(20);
			memcpy(m_cGameServerName, gameservername.c_str(), (gameservername.length() < 20) ? gameservername.length() : 20);
			gamemode = 1;
			ConnectionEstablishHandler(SERVERTYPE_GAME);
		}
		break;

	case ENTERGAMERESTYPE_REJECT:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		switch (sr.ReadByte()) {
		case 1:	strcpy(m_cMsg, "3E"); break;
		case 2:	strcpy(m_cMsg, "3F"); break;
		case 3:	strcpy(m_cMsg, "33"); break;
		case 4: strcpy(m_cMsg, "3D"); break;
		case 5: strcpy(m_cMsg, "3G"); break;
		case 6: strcpy(m_cMsg, "3Z"); break;
		case 7: strcpy(m_cMsg, "3J"); break;
		}
		break;

	case ENTERGAMERESTYPE_FORCEDISCONN:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "3X");
		break;

	case LOGRESMSGTYPE_NOTEXISTINGWORLDSERVER:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "1Y");
		break;

	case LOGRESMSGTYPE_INPUTKEYCODE:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		sr.position += 6;
		switch (sr.ReadByte()) {
		case 1:	strcpy(m_cMsg, "8U"); break; //MainMenu, Keycode registration success
		case 2:	strcpy(m_cMsg, "82"); break; //MainMenu, Not existing Account
		case 3:	strcpy(m_cMsg, "81"); break; //MainMenu, Password wrong
		case 4: strcpy(m_cMsg, "8V"); break; //MainMenu, Invalid Keycode
		case 5: strcpy(m_cMsg, "8W"); break; //MainMenu, Already Used Keycode
		}
		break;


	case LOGRESMSGTYPE_FORCECHANGEPASSWORD:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "6M");
		break;

	case LOGRESMSGTYPE_INVALIDKOREANSSN:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "1a");
		break;

	case LOGRESMSGTYPE_LESSTHENFIFTEEN:
		ChangeGameMode(GAMEMODE_ONLOGRESMSG);
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
		strcpy(m_cMsg, "1b");
		break;

	}
// 	delete m_pLSock;
// 	gamemode = 0;
// 	m_pGSock = m_pLSock = NULL;
}

void CGame::UpdateScreen_OnMsg()
{short msX, msY, msZ;
 char cLB, cRB, cMB;
 uint32_t dwTime = G_dwGlobalTime;

 msX = m_stMCursor.sX;msY = m_stMCursor.sY;msZ = m_stMCursor.sZ;
 cLB = (m_stMCursor.LB==true)?1:0;cRB = (m_stMCursor.RB==true)?1:0;cMB = (m_stMCursor.MB==true)?1:0;


	////DIRECTX m_DDraw.ClearBackB4();//DIRECTX
	PutString(10, 10, m_cMsg,video::SColor(255,255,155,155), false, 1);
	DrawVersion();
	////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
	//m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	m_stMCursor.sCursorFrame = 0;
	//if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();//DIRECTX
	if ((G_dwGlobalTime - m_dwTime) > 1500)
	{
		isItemLoaded = false;
		ChangeGameMode(GAMEMODE_ONMAINMENU);
	}
}

void CGame::_InitOnCreateNewCharacter()
{	m_cGender    = rand() % 2 + 1;
	m_cSkinCol   = rand() % 3 + 1;
	m_cHairStyle = rand() % 8;
	m_cHairCol   = rand() % 16;
	m_cUnderCol  = rand() % 8;
	m_createStat[STAT_STR] = 10;
	m_createStat[STAT_VIT] = 10;
	m_createStat[STAT_DEX] = 10;
	m_createStat[STAT_INT] = 10;
	m_createStat[STAT_MAG] = 10;
	m_createStat[STAT_CHR] = 10;
}

void CGame::ClearContents_OnCreateNewAccount()
{
//	ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
	ZeroMemory(m_cAccountAge, sizeof(m_cAccountAge));
	ZeroMemory(m_cAccountCountry, sizeof(m_cAccountCountry));
	ZeroMemory(m_cAccountSSN, sizeof(m_cAccountSSN));
	ZeroMemory(m_cEmailAddr, sizeof(m_cEmailAddr));
	//v1.4334
	ZeroMemory(m_cAccountQuiz, sizeof(m_cAccountQuiz));
	ZeroMemory(m_cAccountAnswer, sizeof(m_cAccountAnswer));
}

void CGame::ChangeGameMode(char cMode)
{
	m_cGameMode = cMode;
	m_cGameModeCount = 0;
	m_dwTime = G_dwGlobalTime;

#ifndef SELECTSERVER
	if( cMode == GAMEMODE_ONSELECTSERVER )
	{
		ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));
		strcpy(m_cWorldServerName, NAME_WORLDNAME1);
		m_cGameMode = GAMEMODE_ONLOGIN;
	}
#endif
}

bool CGame::bReadIp()
{
	ZeroMemory(m_cLogServerAddr, sizeof(m_cLogServerAddr));
	strcpy(m_cLogServerAddr, SERVER_IP);
	m_iLogServerPort = SERVER_PORT;
	return true;
}

void CGame::ReleaseUnusedSprites()
{	int i;
	for (i = 0; i < MAXSPRITES; i++)
	if (m_pSprite[i] != 0)
	{	if( (m_pSprite[i]->m_bIsSurfaceEmpty == false) && (m_pSprite[i]->m_bOnCriticalSection == false) )
		{	if ((G_dwGlobalTime - m_pSprite[i]->m_dwRefTime) > 60000 ) m_pSprite[i]->_iCloseSprite();

	}	}
	for (i = 0; i < MAXTILES; i++)
	if (m_pTileSpr[i] != 0)
	{	if( (m_pTileSpr[i]->m_bIsSurfaceEmpty == false) && (m_pTileSpr[i]->m_bOnCriticalSection == false) )
		{	if ((G_dwGlobalTime - m_pTileSpr[i]->m_dwRefTime) > 60000 ) m_pTileSpr[i]->_iCloseSprite();
	}	}
	for (i = 0; i < MAXEFFECTSPR; i++)
	if (m_pEffectSpr[i] != 0)
	{	if( (m_pEffectSpr[i]->m_bIsSurfaceEmpty == false) && (m_pEffectSpr[i]->m_bOnCriticalSection == false) )
		{	if ((G_dwGlobalTime - m_pEffectSpr[i]->m_dwRefTime) > 60000 ) m_pEffectSpr[i]->_iCloseSprite();
	}	}

// 	for (i = 0; i < MAXSOUNDEFFECTS; i++)
// 	{	if (m_pCSound[i] != NULL)
// 		{	if (((G_dwGlobalTime - m_pCSound[i]->m_dwTime) > 30000) && (m_pCSound[i]->m_bIsLooping == FALSE)) m_pCSound[i]->_ReleaseSoundBuffer();
// 		}
// 		if (m_pMSound[i] != NULL)
// 		{	if (((G_dwGlobalTime - m_pMSound[i]->m_dwTime) > 30000) && (m_pMSound[i]->m_bIsLooping == FALSE)) m_pMSound[i]->_ReleaseSoundBuffer();
// 		}
// 		if (m_pESound[i] != NULL)
// 		{	if (((G_dwGlobalTime - m_pESound[i]->m_dwTime) > 30000) && (m_pESound[i]->m_bIsLooping == FALSE)) m_pESound[i]->_ReleaseSoundBuffer();
// 	}	}//DIRECTX
}

void CGame::PutChatScrollList(char * txt, char cType)
{
	CMsg * msg = new CMsg(1, txt, cType);

	m_chatMsgs[cType].push_back( msg );

	if(m_chatToggle[cType])
		m_chatDisplay->push_back( msg );
	
	if(m_chatDisplay->size() >= MAXCHATSCROLLMSGS + MAXCHATSCROLLMSGS/20)
	{
		m_chatDisplay->erase( m_chatDisplay->begin(), m_chatDisplay->begin() + MAXCHATSCROLLMSGS/20);
	}

	if(m_chatMsgs[cType].size() >= MAXCHATSCROLLMSGS + MAXCHATSCROLLMSGS/20)
	{
		m_chatMsgs[cType].erase( m_chatMsgs[cType].begin(), m_chatMsgs[cType].begin() + MAXCHATSCROLLMSGS/20);
	}

	if(m_tabbedNotification && !m_bIsProgramActive) 
	{
		if(strstr(txt, m_cPlayerName))
			FlashWindow(*(HWND*)m_hWnd,true);
	}
}

void CGame::ChatMsgHandler(char * pData)
{
	int i, iObjectID, iLoc;
	short * sp, sX, sY;
	char * cp, cMsgType, cName[21], cTemp[100], cMsg[100], cTxt1[100], cTxt2[100];
	uint32_t dwTime;
	uint16_t * wp;
	bool bFlag;

	dwTime = m_dwCurTime;

	ZeroMemory(cTxt1, sizeof(cTxt1));
	ZeroMemory(cTxt2, sizeof(cTxt2));
	ZeroMemory(cMsg, sizeof(cMsg));

	wp = (uint16_t *)(pData + INDEX2_MSGTYPE);
	iObjectID = (int)*wp;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	sp = (short *)cp;
	sX = *sp;
	cp += 2;

	sp = (short *)cp;
	sY = *sp;
	cp += 2;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, (char *)cp, 10);
	cp += 10;

	cMsgType = *cp;
	cp++;

	ZeroMemory(cTemp, sizeof(cTemp));
	strcpy(cTemp, cp);

	if((m_MuteList.find(string(cName)) != m_MuteList.end()) ||
		(!m_bWhisper && cMsgType == CHAT_WHISPER) ||
		(!m_bShout && (cMsgType == CHAT_SHOUT || cMsgType == CHAT_NATIONSHOUT) ) ||
		((cMsgType==0 || cMsgType==2 || cMsgType==3) && !m_Misc.bCheckIMEString(cTemp)) )
	{
		return;
	}

	//if (cMsgType == 2) { // GM Helper Chat - determine if the player speaking in global is a GM Helper
	//	if(strcmp(cName, "GM1lole") == 0 )
	//	{
	//		cMsgType = 36;
	//	}
	//}

	//if (cMsgType == 2) { // ViP Chat - determine if the player speaking in global is a VIP Player
	//	if(strcmp(cName, "VipPlayer") == 0 )
	//	{
	//		cMsgType = 37;
	//	}
	//}

	//if (cMsgType == 2) { // Alpha Tester Chat - determine if the player speaking in global is a Alpha Tester
	//	if(strcmp(cName, "Alpha") == 0 || strcmp(cName, "Alpha1") == 0 || strcmp(cName, "Alpha2") == 0 )
	//	{
	//		cMsgType = 38;
	//	}
	//}

	char timeStamp[50];
	TimeStamp(timeStamp);

	ZeroMemory(cMsg, sizeof(cMsg));
	if(cMsgType == CHAT_WHISPER && (memcmp(m_cPlayerName, cName, 10) == 0))
	{
		if (m_showTimeStamp)
			wsprintfA(cMsg, "[%s](%s) %s: %s", timeStamp, m_cWhisperName, cName, cTemp);
		else
			wsprintfA(cMsg, "(%s) %s: %s", m_cWhisperName, cName, cTemp);
	}
	else
	{
		if (m_showTimeStamp)
			wsprintfA(cMsg, "[%s] %s: %s", timeStamp, cName, cTemp);
		else
			wsprintfA(cMsg, "%s: %s", cName, cTemp);
	}

	//DIRECTX m_DDraw._GetBackBufferDC();
	bFlag = false;
	short sCheckByte = 0;
	while (bFlag == false)
	{
		//DIRECTX iLoc = m_Misc.iGetTextLengthLoc(m_DDraw.m_hDC, cMsg, 305);
		iLoc = 0;
		for( int i=0 ; i<iLoc ; i++ )
			if( cMsg[i] < 0 ) sCheckByte ++;
		if (iLoc == 0)
		{
			PutChatScrollList(cMsg, cMsgType);
			bFlag = true;
		}
		else
		{
			if ((sCheckByte%2)==0)
			{
				ZeroMemory(cTemp, sizeof(cTemp));
				memcpy(cTemp, cMsg, iLoc);
				PutChatScrollList(cTemp, cMsgType);
				ZeroMemory(cTemp, sizeof(cTemp));
				strcpy(cTemp, cMsg +iLoc );
				ZeroMemory(cMsg, sizeof(cMsg));
				strcpy(cMsg, " ");
				strcat(cMsg, cTemp);
			}
			else
			{	ZeroMemory(cTemp, sizeof(cTemp));
				memcpy(cTemp, cMsg, iLoc+1);
				PutChatScrollList(cTemp, cMsgType);
				ZeroMemory(cTemp, sizeof(cTemp));
				strcpy(cTemp, cMsg +iLoc+1);
				ZeroMemory(cMsg, sizeof(cMsg));
				strcpy(cMsg, " ");
				strcat(cMsg, cTemp);
			}
		}
	}

	//DIRECTX m_DDraw._ReleaseBackBufferDC();

	_RemoveChatMsgListByObjectID(iObjectID);

	for (i = 1; i < MAXCHATMSGS; i++)
	if (m_pChatMsgList[i] == 0) {
		m_pChatMsgList[i] = new class CMsg(1, (char *)(cp), dwTime);
		m_pChatMsgList[i]->m_iObjectID = iObjectID;

		if (m_pMapData->bSetChatMsgOwner(iObjectID, sX, sY, i) == false) {
			delete m_pChatMsgList[i];
			m_pChatMsgList[i] = 0;
		}

		char cHeadMsg[200];

		if(cMsgType != 0 && (!m_bIsDialogEnabled[10] || !m_chatToggle[cMsgType]) )
		{
			ZeroMemory(cHeadMsg, sizeof(cHeadMsg));
			if(cMsgType == CHAT_WHISPER && (memcmp(m_cPlayerName, cName, 10) == 0))
			{
				if (m_showTimeStamp)
					wsprintfA(cHeadMsg, "[%s](%s) %s: %s", timeStamp, m_cWhisperName, cName, cp);
				else
					wsprintfA(cHeadMsg, "(%s) %s: %s", m_cWhisperName, cName, cp);
			}else{
				if (m_showTimeStamp)
					wsprintfA(cHeadMsg, "[%s] %s: %s", timeStamp, cName, cp);
				else
					wsprintfA(cHeadMsg, "%s: %s", cName, cp);
			}
			AddEventList(cHeadMsg, cMsgType);
		}
		return;
	}
}

void CGame::ReleaseTimeoverChatMsg()
{
	int i;
	uint32_t dwTime;

	dwTime = G_dwGlobalTime;
	for ( i = 1; i < MAXCHATMSGS; i++)
	if (m_pChatMsgList[i] != 0) {

		if ((m_pChatMsgList[i]->m_cType >= 1) && (m_pChatMsgList[i]->m_cType <= 20)) {
			if ((dwTime - m_pChatMsgList[i]->m_dwTime) > CHATTIMEOUT_A) {
				delete m_pChatMsgList[i];
				m_pChatMsgList[i] = 0;
			}
		}
		else
		if ((m_pChatMsgList[i]->m_cType >= 21) && (m_pChatMsgList[i]->m_cType <= 40)) {
			if ((dwTime - m_pChatMsgList[i]->m_dwTime) > CHATTIMEOUT_B) {
				delete m_pChatMsgList[i];
				m_pChatMsgList[i] = 0;
			}
		}
		else
		if ((m_pChatMsgList[i]->m_cType >= 41) && (m_pChatMsgList[i]->m_cType <= 60)) {
			if ((dwTime - m_pChatMsgList[i]->m_dwTime) > CHATTIMEOUT_C) {
				delete m_pChatMsgList[i];
				m_pChatMsgList[i] = 0;
			}
		}
		else if ((dwTime - m_pChatMsgList[i]->m_dwTime) > CHATTIMEOUT_A) {
			delete m_pChatMsgList[i];
			m_pChatMsgList[i] = 0;
		}
	}
}

void CGame::DrawBackground(short sDivX, short sModX, short sDivY, short sModY)
{
	int indexX, indexY, ix, iy, iyMax;
	short sSpr, sSprFrame;

// 	if (sDivX < 0 || sDivY < 0) return ;

	//iyMax = m_bIsObserverMode ? 480+48 : 427+48;
	iyMax = m_bIsObserverMode ? GetHeight()+48 : GetHeight()-3; // 800x600 Resolution xRisenx
	if ((m_bIsRedrawPDBGS == true) || (m_iPDBGSdivX != sDivX) || (m_iPDBGSdivY != sDivY)) {
		// Pre-Draw Background Surface
		m_bIsRedrawPDBGS = false;
		m_iPDBGSdivX = sDivX;
		m_iPDBGSdivY = sDivY;
		//SetRect(&//DIRECTX m_DDraw.m_rcClipArea, 0,0, GetHeight()+32, 480+32);
		//SetRect(&//DIRECTX m_DDraw.m_rcClipArea, 0,0, GetWidth()+32, 600+32); // 800x600 Resolution xRisenx
		indexY = sDivY+m_pMapData->m_sPivotY;
		//for (iy = -sModY; iy < 427+48 ; iy += 32)
		driver->setRenderTarget(bg, true, true, video::SColor(0,0,0,0));
		for (iy = -sModY; iy < GetHeight()-3 +32; iy += 32) // 800x600 xRisenx
		//for (iy = -sModY; iy < iyMax ; iy += 32) Resolution Need to check? Core code changed?
		{
			indexX = sDivX+m_pMapData->m_sPivotX;
			//for (ix = -sModX; ix < 640+48 ; ix += 32)
			for (ix = -sModX; ix < GetWidth()+48 +32; ix += 32) // 800x600 Resolution xRisenx
			{
                if (indexX >= 0 && indexY >= 0)
                {
                    sSpr = m_pMapData->m_tile[indexX][indexY].m_sTileSprite;
                    sSprFrame = m_pMapData->m_tile[indexX][indexY].m_sTileSpriteFrame;
                    if (m_pTileSpr[sSpr])
                        //m_pTileSpr[sSpr]->PutSpriteFastNoColorKeyDst((LPDIRECTDRAWSURFACE7)0, ix - 16 +sModX, iy - 16 +sModY, sSprFrame, m_dwCurTime);
                    {
                        // 					if ((indexY % 2) + (indexX % 2) == 1)
                        // 						m_pTileSpr[sSpr]->PutSpriteRGB(ix - 16 + sModX, iy - 16 + sModY, sSprFrame, (uint32_t)irr::video::SColor(255, 200, 0, 0).color, m_dwCurTime);//color ground
                        // 					else
                        m_pTileSpr[sSpr]->DrawSpriteNCK(ix - 16 + sModX, iy - 16 + sModY, sSprFrame, m_dwCurTime);
                    }
                }
				indexX++;
			}
			indexY++;
		}
		driver->setRenderTarget(0);
		//SetRect(&//DIRECTX m_DDraw.m_rcClipArea, 0,0, GetHeight(), 480);
		//SetRect(&//DIRECTX m_DDraw.m_rcClipArea, 0,0, GetWidth(), 600); // 800x600 Resolution xRisenx
	}
	RECT rcRect;
	//SetRect(&rcRect, sModX, sModY, 640+sModX, 480+sModY); // our fictitious sprite bitmap is
	SetRect(&rcRect, sModX, sModY, GetWidth()+sModX, GetHeight()+sModY); // 800x600 Resolution xRisenx
	//DIRECTX m_DDraw.m_lpBackB4->BltFast( 0, 0, //DIRECTX m_DDraw.m_lpPDBGS, &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);

	driver->draw2DImage(bg, core::vector2d<s32>(0 - sModX, 0 - sModY));

// 
// 	char cfps[20];
// 	sprintf(cfps, "sModX: %d sModY: %d", sModX, sModY);
// 
// 	font[0]->draw(cfps,
// 		core::rect<s32>(5,100,40,110),
// 		video::SColor(255,255,255,255));



	if (m_showGrid)
	{
		indexY = sDivY+m_pMapData->m_sPivotY;
		for (iy = -sModY; iy < iyMax ; iy += 32)
		{
			indexX = sDivX+m_pMapData->m_sPivotX;
			//for (ix = -sModX; ix < 640+48 ; ix += 32)
			for (ix = -sModX; ix < GetWidth()+48 ; ix += 32) // 800x600 Resolution xRisenx
			{
				DrawLine(ix - 16 , iy - 16 , ix + 16 , iy - 16 , 6,13,13);
				DrawLine(ix - 16 , iy - 16 , ix - 16 , iy + 16 , 6,13,13);

                char cfps[20];
                sprintf(cfps, "(%d,%d)", (m_sViewPointX+ix)/32, (m_sViewPointY+iy)/32);

                font[0]->draw(cfps,
                              core::rect<s32>(ix-16, iy, 15, 10),
                              video::SColor(128, 255, 255, 255));

				indexX++;
			}
			indexY++;
		}
	}

	if( m_bIsCrusadeMode )
	{
		if(m_iConstructLocX != -1) DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, m_iConstructLocX*32 - m_sViewPointX, m_iConstructLocY*32 - m_sViewPointY, 41);
		if( m_iTeleportLocX != -1) DrawNewDialogBox(SPRID_INTERFACE_ND_CRUSADE, m_iTeleportLocX*32 - m_sViewPointX, m_iTeleportLocY*32 - m_sViewPointY, 42);
	}
}

bool CGame::bEffectFrameCounter()
{
	int i, x;
	uint32_t dwTime;
	bool bRet = false;
	short sAbsX, sAbsY, sDist;
	char  cDir;
	long lPan;
	dwTime = m_dwCurTime;
	dwTime += m_pMapData->m_dwFrameAdjustTime;
	for (i = 0; i < MAXEFFECTS; i++)
	if (m_pEffectList[i] != 0) {
		if ((dwTime - m_pEffectList[i]->m_dwTime) > m_pEffectList[i]->m_dwFrameTime)
		{	m_pEffectList[i]->m_dwTime = dwTime;
			m_pEffectList[i]->m_cFrame++;
			bRet = true;
			m_pEffectList[i]->m_mX2 = m_pEffectList[i]->m_mX;
			m_pEffectList[i]->m_mY2 = m_pEffectList[i]->m_mY;
			switch (m_pEffectList[i]->m_sType) {
			case 1: // coup normal
				if (m_pEffectList[i]->m_cFrame == 1)
				{	for (int j = 1; j <= m_pEffectList[i]->m_iV1; j++) bAddNewEffect(11, m_pEffectList[i]->m_mX + 15 - (rand() % 30), m_pEffectList[i]->m_mY + 15 - (rand() % 30), 0, 0, -1*(rand() % 2));
				}
				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 2:	// (Arrow missing target ?)
				m_Misc.GetPoint(m_pEffectList[i]->m_mX,    m_pEffectList[i]->m_mY,
					            m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32 - 40,
								&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
								&m_pEffectList[i]->m_iErr, 70);
				if (   (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX*32) <= 2)
					&& (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY*32 - 40)) <= 2) )
				{	//bAddNewEffect(14, m_pEffectList[i]->m_mX +(rand() % 5) - 2, m_pEffectList[i]->m_mY +(rand() % 5) - 2, NULL, NULL, 0, 0);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 4: // Gold Drop ,33,69,70
			case 33: //
			case 69:
			case 70:
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 5:
			case 30:
			case 31: // Fire Explosion
			case 252:
				if (m_pEffectList[i]->m_cFrame == 1)
				{	bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
				}
				if (m_pEffectList[i]->m_cFrame == 7)
				{	bAddNewEffect(15, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, 0);
					bAddNewEffect(15, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, 0);
					bAddNewEffect(15, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, 0);
				}
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 6: // Lightning Bolt Burst
				if (m_pEffectList[i]->m_cFrame == 1)
				{	bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
				}
				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 7: // Magic Missile Burst
				if (m_pEffectList[i]->m_cFrame == 1)
				{	bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
				}
				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 9:  // Burst Type 2
			case 11: // Burst Type 3
				m_pEffectList[i]->m_mX += m_pEffectList[i]->m_rX;
				m_pEffectList[i]->m_mY += m_pEffectList[i]->m_rY;
				m_pEffectList[i]->m_rY++;
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 10: // Lightning Arrow Burst
				if (m_pEffectList[i]->m_cFrame == 1)
				{	bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
				}
				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 12: // Burst Type 4
				m_pEffectList[i]->m_mX += m_pEffectList[i]->m_rX;
				m_pEffectList[i]->m_mY += m_pEffectList[i]->m_rY;
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 13: // Bulles druncncity
				if (m_pEffectList[i]->m_cFrame < 15)
				{	if ((rand() % 2) == 0)
						 m_pEffectList[i]->m_mX++;
					else m_pEffectList[i]->m_mX--;
					m_pEffectList[i]->m_mY--;
				}
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 16: //
				m_Misc.GetPoint(m_pEffectList[i]->m_mX,    m_pEffectList[i]->m_mY,
					            m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY,
								&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
								&m_pEffectList[i]->m_iErr, 40);
				bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1*(rand() % 4));
				if (   (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX) <= 2)
					&& (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY)) <= 2))
				{	bAddNewEffect(18, m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY, 0, 0, 0); // testcode 0111 18
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1*(rand() % 2));
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 17: // Ice-Storm
				cDir = m_Misc.cGetNextMoveDir(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, m_pEffectList[i]->m_mX3, m_pEffectList[i]->m_mY3);
				switch (cDir) {
				case 1:
					m_pEffectList[i]->m_rY -= 2;
					break;
				case 2:
					m_pEffectList[i]->m_rY -= 2;
					m_pEffectList[i]->m_rX += 2;
					break;
				case 3:
					m_pEffectList[i]->m_rX += 2;
					break;
				case 4:
					m_pEffectList[i]->m_rX += 2;
					m_pEffectList[i]->m_rY += 2;
					break;
				case 5:
					m_pEffectList[i]->m_rY += 2;
					break;
				case 6:
					m_pEffectList[i]->m_rX -= 2;
					m_pEffectList[i]->m_rY += 2;
					break;
				case 7:
					m_pEffectList[i]->m_rX -= 2;
					break;
				case 8:
					m_pEffectList[i]->m_rX -= 2;
					m_pEffectList[i]->m_rY -= 2;
					break;
				}
				if (m_pEffectList[i]->m_rX < -10) m_pEffectList[i]->m_rX = -10;
				if (m_pEffectList[i]->m_rX >  10) m_pEffectList[i]->m_rX =  10;
				if (m_pEffectList[i]->m_rY < -10) m_pEffectList[i]->m_rY = -10;
				if (m_pEffectList[i]->m_rY >  10) m_pEffectList[i]->m_rY =  10;
				m_pEffectList[i]->m_mX += m_pEffectList[i]->m_rX;
				m_pEffectList[i]->m_mY += m_pEffectList[i]->m_rY;
				m_pEffectList[i]->m_mY3--;
				if (m_pEffectList[i]->m_cFrame > 10)
				{	m_pEffectList[i]->m_cFrame = 0;
					if (abs(m_pEffectList[i]->m_sY - m_pEffectList[i]->m_mY3) > 100)
					{	delete m_pEffectList[i];
						m_pEffectList[i] = 0;
				}	}
				break;

			case 20: // Critical strike with a weapon
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27: // Critical strike with a weapon
				m_Misc.GetPoint(m_pEffectList[i]->m_mX,    m_pEffectList[i]->m_mY,
					            m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32 - 40,
								&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
								&m_pEffectList[i]->m_iErr, 50);
				bAddNewEffect(8, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0);//-1*(rand() % 4));
				bAddNewEffect(8, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0);//-1*(rand() % 4));
				bAddNewEffect(8, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0);//-1*(rand() % 4));
				bAddNewEffect(8, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0);//-1*(rand() % 4));
				bAddNewEffect(8, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0);//-1*(rand() % 4));
				if ( (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX*32) <= 2) &&
					 (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY*32 - 40)) <= 2) )
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 34: //
				m_Misc.GetPoint(m_pEffectList[i]->m_mX,    m_pEffectList[i]->m_mY,
					            m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY,
								&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
								&m_pEffectList[i]->m_iErr, 50);
				bAddNewEffect(33, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, 0, 0, -1*(rand() % 4));
				if ( (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX) <= 2) &&
					 (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY)) <= 2) )
				{	bAddNewEffect(33, m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY, 0, 0, 0); //7
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;


			case 40:
			case 56:
				if (m_pEffectList[i]->m_cFrame == 9)
				{	bAddNewEffect(51, m_pEffectList[i]->m_mX +((rand() % 100)-50), m_pEffectList[i]->m_mY +((rand() % 70)-35), 0, 0, 0, 0);
					bAddNewEffect(51, m_pEffectList[i]->m_mX +((rand() % 100)-50), m_pEffectList[i]->m_mY +((rand() % 70)-35), 0, 0, 0, 0);
					bAddNewEffect(51, m_pEffectList[i]->m_mX +((rand() % 100)-50), m_pEffectList[i]->m_mY +((rand() % 70)-35), 0, 0, 0, 0);
					bAddNewEffect(51, m_pEffectList[i]->m_mX +((rand() % 100)-50), m_pEffectList[i]->m_mY +((rand() % 70)-35), 0, 0, 0, 0);
					bAddNewEffect(51, m_pEffectList[i]->m_mX +((rand() % 100)-50), m_pEffectList[i]->m_mY +((rand() % 70)-35), 0, 0, 0, 0);
				}
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 41: //Large Type 1, 2, 3, 4
			case 42:
			case 43:
			case 44:
			case 45: // Small Type 1, 2
			case 46:
				if (m_pEffectList[i]->m_cFrame >= 7)
				{	m_pEffectList[i]->m_mX--;
					m_pEffectList[i]->m_mY += m_pEffectList[i]->m_iV1;
					m_pEffectList[i]->m_iV1++;
				}

				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	if ((m_pEffectList[i]->m_sType != 45) && (m_pEffectList[i]->m_sType != 46))
					{	bAddNewEffect(50, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0);
						bAddNewEffect(14, m_pEffectList[i]->m_mX +((rand() % 20)-10), m_pEffectList[i]->m_mY +((rand() % 20)-10), 0, 0, 0, 0);
						bAddNewEffect(14, m_pEffectList[i]->m_mX +((rand() % 20)-10), m_pEffectList[i]->m_mY +((rand() % 20)-10), 0, 0, 0, 0);
						bAddNewEffect(14, m_pEffectList[i]->m_mX +((rand() % 20)-10), m_pEffectList[i]->m_mY +((rand() % 20)-10), 0, 0, 0, 0);
						if (m_cDetailLevel != 0){
							bAddNewEffect(51, m_pEffectList[i]->m_mX +((rand() % 20)-10), m_pEffectList[i]->m_mY +((rand() % 20)-10), 0, 0, 0, 0);
							bAddNewEffect(51, m_pEffectList[i]->m_mX +((rand() % 20)-10), m_pEffectList[i]->m_mY +((rand() % 20)-10), 0, 0, 0, 0);
						}
					}
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 47: // Blizzard
			case 48:
			case 49:
				if (m_pEffectList[i]->m_cFrame >= 7)
				{	m_pEffectList[i]->m_mX--;
					m_pEffectList[i]->m_mY += m_pEffectList[i]->m_iV1;
					m_pEffectList[i]->m_iV1 += 4;
				}
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	if (m_pEffectList[i]->m_sType == 49)
						 bAddNewEffect(72, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0);
					else bAddNewEffect(50, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0);
					bAddNewEffect(14, m_pEffectList[i]->m_mX +((rand() % 20)-10), m_pEffectList[i]->m_mY +((rand() % 20)-10), 0, 0, 0, 0);
					bAddNewEffect(14, m_pEffectList[i]->m_mX +((rand() % 20)-10), m_pEffectList[i]->m_mY +((rand() % 20)-10), 0, 0, 0, 0);
					bAddNewEffect(14, m_pEffectList[i]->m_mX +((rand() % 20)-10), m_pEffectList[i]->m_mY +((rand() % 20)-10), 0, 0, 0, 0);
					if (m_cDetailLevel != 0){
						bAddNewEffect(51, m_pEffectList[i]->m_mX +((rand() % 20)-10), m_pEffectList[i]->m_mY +((rand() % 20)-10), 0, 0, 0, 0);
						bAddNewEffect(51, m_pEffectList[i]->m_mX +((rand() % 20)-10), m_pEffectList[i]->m_mY +((rand() % 20)-10), 0, 0, 0, 0);
					}
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 60: //
			case 181: // Meteor-Strike
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	bAddNewEffect(61, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);
					bAddNewEffect(63, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);
					bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1*(rand() % 2));
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else if (m_pEffectList[i]->m_cFrame >= 0)
				{	m_pEffectList[i]->m_mX -= 30;
					m_pEffectList[i]->m_mY += 46;
					bAddNewEffect(62, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);
				}
				break;

			case 62:
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else if (m_pEffectList[i]->m_cFrame >= 0)
				{	m_pEffectList[i]->m_mX += (rand() % 3) -1;
					m_pEffectList[i]->m_mY += (rand() % 3) -1;
				}
				break;

			case 65: // Building fire after MS (crusade) 65 & 67
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else if (m_pEffectList[i]->m_cFrame >= 0)
				{	m_pEffectList[i]->m_mX += (rand() % 3) -1;
					m_pEffectList[i]->m_mY -= 4 +(rand() % 2);
				}
				break;

			case 66:
			case 203:
			case 204:
			case 205:
			case 206:
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 68:
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else if (m_pEffectList[i]->m_cFrame == 11)
				{	SetCameraShakingEffect(m_pEffectList[i]->m_iV1, 2);
				}
				break;

			case 71:
				m_Misc.GetPoint(m_pEffectList[i]->m_mX,    m_pEffectList[i]->m_mY,
					            m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY,
								&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
								&m_pEffectList[i]->m_iErr, 50);
				bAddNewEffect(48, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, 0, 0, 0);
				if (m_cDetailLevel != 0) bAddNewEffect(51, m_pEffectList[i]->m_mX +((rand() % 20)-10), m_pEffectList[i]->m_mY +((rand() % 20)-10), 0, 0, 0, 0);
				if ( (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX) <= 2) &&
					 (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY)) <= 2) )
				{	bAddNewEffect(49, m_pEffectList[i]->m_mX/* + (rand() % 30) - 15*/, m_pEffectList[i]->m_mY/* + (rand() % 30) - 15*/, 0, 0, 0);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 81: // Snoopy: Ajout StromBlade
				m_Misc.GetPoint(m_pEffectList[i]->m_mX
					, m_pEffectList[i]->m_mY
					, m_pEffectList[i]->m_dX*32
					, m_pEffectList[i]->m_dY*32
					, &m_pEffectList[i]->m_mX
					, &m_pEffectList[i]->m_mY
					, &m_pEffectList[i]->m_iErr
					, 10);
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 100: // Magic Missile
				m_Misc.GetPoint(m_pEffectList[i]->m_mX,    m_pEffectList[i]->m_mY,
					            m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32/* - 40*/,
								&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
								&m_pEffectList[i]->m_iErr, 50);
				bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1*(rand() % 4));

				if ( (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX*32) <= 2) &&
					 (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY*32/* - 40*/)) <= 2) )
				{	bAddNewEffect(7, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 110: // Enegy-Bolt
				m_Misc.GetPoint(m_pEffectList[i]->m_mX,    m_pEffectList[i]->m_mY,
					            m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32/* - 40*/,
								&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
								&m_pEffectList[i]->m_iErr, 50);
				bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1*(rand() % 4));
				bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1*(rand() % 4));
				if (    (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX*32) <= 2)
					 && (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY*32) <= 2) )
				{	bAddNewEffect(6, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0); // 6 testcode 0111
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 120: // Fire Ball
				m_Misc.GetPoint(m_pEffectList[i]->m_mX,    m_pEffectList[i]->m_mY,
					            m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32/* - 40*/,
								&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
								&m_pEffectList[i]->m_iErr, 50);
				if (    (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX*32) <= 2)
					 && (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY*32) <= 2) )
				{	bAddNewEffect(5, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 130: // Fire Strike
				m_Misc.GetPoint(m_pEffectList[i]->m_mX,    m_pEffectList[i]->m_mY,
					            m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32/* - 40*/,
								&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
								&m_pEffectList[i]->m_iErr, 50);
				if (    (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX*32) <= 2)
					 && (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY*32) <= 2) )
				{	bAddNewEffect(5, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0);
					bAddNewEffect(5, m_pEffectList[i]->m_dX*32 -30, m_pEffectList[i]->m_dY*32 -15, 0, 0, -7);
					bAddNewEffect(5, m_pEffectList[i]->m_dX*32 +35, m_pEffectList[i]->m_dY*32 -30, 0, 0, -5);
					bAddNewEffect(5, m_pEffectList[i]->m_dX*32 +20, m_pEffectList[i]->m_dY*32 +30, 0, 0, -3);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 137: // Lightning Arrow
				m_Misc.GetPoint(m_pEffectList[i]->m_mX,    m_pEffectList[i]->m_mY,
					            m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32/* - 40*/,
								&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
								&m_pEffectList[i]->m_iErr, 50);
				bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1*(rand() % 4));
				bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1*(rand() % 4));
				bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1*(rand() % 4));
				if (   (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX*32) <= 2)
					&& (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY*32) <= 2) )
				{	bAddNewEffect(10, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 143: // Lightning
			case 151: // Lightning-Bolt
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	bAddNewEffect(10, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	m_pEffectList[i]->m_rX = 5 - (rand() % 10);
					m_pEffectList[i]->m_rY = 5 - (rand() % 10);
				}
				break;

			case 145: // Chill-Wind
				bAddNewEffect(40, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0);
				bAddNewEffect(40, m_pEffectList[i]->m_dX*32 -30, m_pEffectList[i]->m_dY*32 -15, 0, 0, -10);
				bAddNewEffect(40, m_pEffectList[i]->m_dX*32 +35, m_pEffectList[i]->m_dY*32 -30, 0, 0, -6);
				bAddNewEffect(40, m_pEffectList[i]->m_dX*32 +20, m_pEffectList[i]->m_dY*32 +30, 0, 0, -3);
				delete m_pEffectList[i];
				m_pEffectList[i] = 0;
				break;

			case 147:  // Triple-Energy-Bolt
				bAddNewEffect(110, m_pEffectList[i]->m_sX , m_pEffectList[i]->m_sY ,
						               m_pEffectList[i]->m_dX -1, m_pEffectList[i]->m_dY-1, 0);
				bAddNewEffect(110, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						               m_pEffectList[i]->m_dX +1, m_pEffectList[i]->m_dY -1, 0);
				bAddNewEffect(110, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						               m_pEffectList[i]->m_dX +1, m_pEffectList[i]->m_dY +1, 0);
				bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1*(rand() % 4));
				//sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
				//sAbsY = abs(((m_sViewPointY / 32) + 7)  - m_pEffectList[i]->m_dY);
				sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX); // 800x600 Resolution xRisenx Center Char xRisenx
				sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY); // 800x600 Resolution xRisenx Center Char xRisenx
				if (sAbsX > sAbsY) sDist = sAbsX;
					else sDist = sAbsY;
				lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX)*1000;
				PlaySound('E', 1, sDist, lPan);
				bAddNewEffect(7, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0);
				delete m_pEffectList[i];
				m_pEffectList[i] = 0;
				break ;

			case 156: // Mass-Lightning-Arrow
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	bAddNewEffect(137, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						               m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY, 0);
					//sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					//sAbsY = abs(((m_sViewPointY / 32) + 7)  - m_pEffectList[i]->m_dY);
					sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX); // 800x600 Resolution xRisenx Center Char xRisenx
					sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY); // 800x600 Resolution xRisenx Center Char xRisenx
					if (sAbsX > sAbsY) sDist = sAbsX;
					else sDist = sAbsY;
					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX)*1000;
					PlaySound('E', 1, sDist, lPan);
				}
				break;

			case 157: // Ice-Strike
				bAddNewEffect(41, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0);
				for (x = 0; x < 14; x++)
				{	bAddNewEffect(41 + (rand() % 3), m_pEffectList[i]->m_dX*32 +(rand() % 100) - 50 +10, m_pEffectList[i]->m_dY*32 + (rand() % 90) - 45, 0, 0, -1*x-1);
				}
				for (x = 0; x < 6; x++)
				{	bAddNewEffect(45 + (rand() % 2), m_pEffectList[i]->m_dX*32 +(rand() % 100) - 50 +10, m_pEffectList[i]->m_dY*32 + (rand() % 90) - 45, 0, 0, -1*x-1 -10);
				}
				delete m_pEffectList[i];
				m_pEffectList[i] = 0;
				break;

			case 160: // Energy-Strike
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	bAddNewEffect(16, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						          m_pEffectList[i]->m_dX*32 + 50 - (rand() % 100), m_pEffectList[i]->m_dY*32 + 50 - (rand() % 100), 0);
					//sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					//sAbsY = abs(((m_sViewPointY / 32) + 7)  - m_pEffectList[i]->m_dY);
					sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX); // 800x600 Resolution xRisenx Center Char xRisenx
					sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY); // 800x600 Resolution xRisenx Center Char xRisenx
					if (sAbsX > sAbsY) sDist = sAbsX;
					else sDist = sAbsY;
					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					PlaySound('E', 1, sDist, lPan);
				}
				break;

			case 161: // Mass-Fire-Strike
				m_Misc.GetPoint(m_pEffectList[i]->m_mX,    m_pEffectList[i]->m_mY,
					            m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32/* - 40*/,
								&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
								&m_pEffectList[i]->m_iErr, 50);
				if (    (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX*32) <= 2)
					 && (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY*32) <= 2) )
				{	bAddNewEffect(30, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0);
					bAddNewEffect(31, m_pEffectList[i]->m_dX*32 -30, m_pEffectList[i]->m_dY*32 -15, 0, 0, -7);
					bAddNewEffect(31, m_pEffectList[i]->m_dX*32 +35, m_pEffectList[i]->m_dY*32 -30, 0, 0, -5);
					bAddNewEffect(31, m_pEffectList[i]->m_dX*32 +20, m_pEffectList[i]->m_dY*32 +30, 0, 0, -3);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 163: // Mass-Chill-Wind Chill-Wind
				bAddNewEffect(56, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0);
				bAddNewEffect(56, m_pEffectList[i]->m_dX*32 -30, m_pEffectList[i]->m_dY*32 -15, 0, 0, -10);
				bAddNewEffect(56, m_pEffectList[i]->m_dX*32 +35, m_pEffectList[i]->m_dY*32 -30, 0, 0, -6);
				bAddNewEffect(56, m_pEffectList[i]->m_dX*32 +20, m_pEffectList[i]->m_dY*32 +30, 0, 0, -3);
				bAddNewEffect(56, m_pEffectList[i]->m_dX*32 +(rand()%100)-50, m_pEffectList[i]->m_dY*32 +(rand()%70)-35, 0, 0, -1*(rand()%10));
				bAddNewEffect(56, m_pEffectList[i]->m_dX*32 +(rand()%100)-50, m_pEffectList[i]->m_dY*32 +(rand()%70)-35, 0, 0, -1*(rand()%10));
				bAddNewEffect(56, m_pEffectList[i]->m_dX*32 +(rand()%100)-50, m_pEffectList[i]->m_dY*32 +(rand()%70)-35, 0, 0, -1*(rand()%10));
				bAddNewEffect(56, m_pEffectList[i]->m_dX*32 +(rand()%100)-50, m_pEffectList[i]->m_dY*32 +(rand()%70)-35, 0, 0, -1*(rand()%10));
				delete m_pEffectList[i];
				m_pEffectList[i] = 0;
				break;

			case 164: // worm-bite
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	bAddNewEffect(68, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0); // testcode 0111 18
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 170: // Bloody-Shock-Wave
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else if ((m_pEffectList[i]->m_cFrame % 2) == 0)
				{	bAddNewEffect(34, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						          m_pEffectList[i]->m_dX*32 + 30 - (rand() % 60), m_pEffectList[i]->m_dY*32 + 30 - (rand() % 60), 0);
					//sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					//sAbsY = abs(((m_sViewPointY / 32) + 7)  - m_pEffectList[i]->m_dY);
					sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX); // 800x600 Resolution xRisenx Center Char xRisenx
					sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY); // 800x600 Resolution xRisenx Center Char xRisenx
					if (sAbsX > sAbsY) sDist = sAbsX;
					else sDist = sAbsY;
					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					PlaySound('E', 1, sDist, lPan);
				}
				break;

			case 172: // Mass-Ice-Strike
				bAddNewEffect(44, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, 0, 0, 0);
				bAddNewEffect(44, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, 0, 0, -1*(rand() % 3));
				bAddNewEffect(44, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, 0, 0, -1*(rand() % 3));
				bAddNewEffect(44, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, 0, 0, -1*(rand() % 3));
				bAddNewEffect(44, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, 0, 0, -1*(rand() % 3));
				bAddNewEffect(44, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, 0, 0, -1*(rand() % 3));
				bAddNewEffect(44, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, 0, 0, -1*(rand() % 3));
				bAddNewEffect(44, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, 0, 0, -1*(rand() % 3));
				bAddNewEffect(44, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, 0, 0, -1*(rand() % 3));
				for (x = 0; x < 16; x++)
				{	bAddNewEffect(44, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, 0, 0, -1*x-1);
				}
				for (x = 0; x < 8; x++)
				{	bAddNewEffect(45 + (rand() % 2), m_pEffectList[i]->m_dX*32 +(rand() % 100) - 50 +10, m_pEffectList[i]->m_dY*32 + (rand() % 90) - 45, 0, 0, -1*x-1 -10);
				}
				delete m_pEffectList[i];
				m_pEffectList[i] = 0;
				break;

			case 174: // Lightning-Strike
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	bAddNewEffect(151, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						          m_pEffectList[i]->m_dX +(rand()%3) -1, m_pEffectList[i]->m_dY +(rand()%3) -1, 0);
					//sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					//sAbsY = abs(((m_sViewPointY / 32) + 7)  - m_pEffectList[i]->m_dY);
					sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX); // 800x600 Resolution xRisenx Center Char xRisenx
					sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY); // 800x600 Resolution xRisenx Center Char xRisenx
					if (sAbsX > sAbsY) sDist = sAbsX;
					else sDist = sAbsY;
					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					PlaySound('E', 1, sDist, lPan);
				}
				break;

			case 182: // Mass-Magic-Missile
				m_Misc.GetPoint(m_pEffectList[i]->m_mX
					, m_pEffectList[i]->m_mY
					, m_pEffectList[i]->m_dX*32
					, m_pEffectList[i]->m_dY*32
					, &m_pEffectList[i]->m_mX
					, &m_pEffectList[i]->m_mY
					, &m_pEffectList[i]->m_iErr
					, 50);
				bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1*(rand() % 4));
				if (    (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX*32) <= 2)
					 && (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY*32) <= 2) )
				{	// JLE 0043132A
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	bAddNewEffect(35, m_pEffectList[i]->m_dX*32 +22, m_pEffectList[i]->m_dY*32 -15, 0, 0, -7, 1);
					bAddNewEffect(36, m_pEffectList[i]->m_dX*32 -22, m_pEffectList[i]->m_dY*32 -7,  0, 0, -7, 1);
					bAddNewEffect(36, m_pEffectList[i]->m_dX*32 +30, m_pEffectList[i]->m_dY*32 -22, 0, 0, -5, 1);
					bAddNewEffect(36, m_pEffectList[i]->m_dX*32 +12, m_pEffectList[i]->m_dY*32 +22, 0, 0, -3, 1);
				}
				break;

			case 191: // Blizzard
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else /*if (m_pEffectList[i]->m_cFrame == 1)*/
				{	bAddNewEffect(71, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						          m_pEffectList[i]->m_dX*32 +(rand()%120)-60, m_pEffectList[i]->m_dY*32 +(rand()%120)-60, 0);
					//sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					//sAbsY = abs(((m_sViewPointY / 32) + 7)  - m_pEffectList[i]->m_dY);
					sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX); // 800x600 Resolution xRisenx Center Char xRisenx
					sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY); // 800x600 Resolution xRisenx Center Char xRisenx
					if (sAbsX > sAbsY) sDist = sAbsX;
					else sDist = sAbsY;
					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					PlaySound('E', 1, sDist, lPan);
				}
				break;

			case 196: // Earth-Shock-Wave
				m_Misc.GetPoint(m_pEffectList[i]->m_mX
					, m_pEffectList[i]->m_mY
					, m_pEffectList[i]->m_dX*32
					, m_pEffectList[i]->m_dY*32
					, &m_pEffectList[i]->m_mX
					, &m_pEffectList[i]->m_mY
					, &m_pEffectList[i]->m_iErr
					, 40);
				bAddNewEffect(80, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, 0, 0, 0, 1);
				bAddNewEffect(80, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, 0, 0);
				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	//sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					//sAbsY = abs(((m_sViewPointY / 32) + 7)  - m_pEffectList[i]->m_dY);
					sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX); // 800x600 Resolution xRisenx Center Char xRisenx
					sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY); // 800x600 Resolution xRisenx Center Char xRisenx
					if (sAbsX > sAbsY) sDist = sAbsX -10;
					else sDist = sAbsY -10;
					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					PlaySound('E', 1, sDist, lPan);
				}
				break;

			case 193: //Hurricane-Blast xRisenx
				m_Misc.GetPoint(m_pEffectList[i]->m_mX
					, m_pEffectList[i]->m_mY
					, m_pEffectList[i]->m_dX*32
					, m_pEffectList[i]->m_dY*32
					, &m_pEffectList[i]->m_mX
					, &m_pEffectList[i]->m_mY
					, &m_pEffectList[i]->m_iErr
					, 40);
				// 30 - looks ok, good fps, bit amaturish
				// 31 - looks good, good fps
				// 67 - looks pants, best fps tho
				// 66 - very FPS intensive
				// 5 - looks ok, bit gappy
				// 18 - ENERGY SHOCK WAVE-looking sprite :P
				bAddNewEffect(10, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, 0, 0);
				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX);
					sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY);
					if (sAbsX > sAbsY) sDist = sAbsX -10;
					else sDist = sAbsY -10;
					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					PlaySound('E', 1, sDist, lPan);
				}
				break;

			case 198: // xRisenx - Mass-Blizzard
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else /*if (m_pEffectList[i]->m_cFrame == 1)*/
				{	bAddNewEffect(71, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
					m_pEffectList[i]->m_dX*32 +(rand()%120)-60, m_pEffectList[i]->m_dY*32 +(rand()%120)-60, 0);
					sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX);
					sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY);
					if (sAbsX > sAbsY) sDist = sAbsX;
					else sDist = sAbsY;
					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					PlaySound('E', 1, sDist, lPan);
				}
				break;

			case 197: // xRisenx - Fiery-Shock-Wave
				m_Misc.GetPoint(m_pEffectList[i]->m_mX
					, m_pEffectList[i]->m_mY
					, m_pEffectList[i]->m_dX*32
					, m_pEffectList[i]->m_dY*32
					, &m_pEffectList[i]->m_mX
					, &m_pEffectList[i]->m_mY
					, &m_pEffectList[i]->m_iErr
					, 40);
				// 30 - looks ok, good fps, bit amaturish
				// 31 - looks good, good fps
				// 67 - looks pants, best fps tho
				// 66 - very FPS intensive
				// 5 - looks ok, bit gappy
				// 18 - ENERGY SHOCK WAVE-looking sprite :P
				bAddNewEffect(66, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, 0, 0);
				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX);
					sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY);
					if (sAbsX > sAbsY) sDist = sAbsX -10;
					else sDist = sAbsY -10;
					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					PlaySound('E', 1, sDist, lPan);
				}
				break;

			case 192: // xRisenx - Call-Of-The-Gods
				m_Misc.GetPoint(m_pEffectList[i]->m_mX
					, m_pEffectList[i]->m_mY
					, m_pEffectList[i]->m_dX*32
					, m_pEffectList[i]->m_dY*32
					, &m_pEffectList[i]->m_mX
					, &m_pEffectList[i]->m_mY
					, &m_pEffectList[i]->m_iErr
					, 40);
				if(m_pEffectList[i]->m_cFrame % 2 == 0)
				bAddNewEffect(68, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, 0, 0, 0, 1);

				//bAddNewEffect(44, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, NULL, NULL, 0, 0);*/
				//bAddNewEffect(71, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						          //m_pEffectList[i]->m_dX*32 +(rand()%120)-60, m_pEffectList[i]->m_dY*32 +(rand()%120)-60, 0);
				bAddNewEffect(71, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						          m_pEffectList[i]->m_dX*32 +(rand()%120)-60, m_pEffectList[i]->m_dY*32 +(rand()%120)-60, 0);
				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX);
					sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY);
					if (sAbsX > sAbsY) sDist = sAbsX -10;
					else sDist = sAbsY -10;
					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					PlaySound('E', 1, sDist, lPan);
				}
			
				break;

			case 187: // xRisenx - Strike-Of-The-Ghosts
				m_Misc.GetPoint(m_pEffectList[i]->m_mX
					, m_pEffectList[i]->m_mY
					, m_pEffectList[i]->m_dX*32
					, m_pEffectList[i]->m_dY*32
					, &m_pEffectList[i]->m_mX
					, &m_pEffectList[i]->m_mY
					, &m_pEffectList[i]->m_iErr
					, 40);

				bAddNewEffect(245, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, 0, 0, 0, 1);

				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX);
					sAbsY = abs(((m_sViewPointY / 32) + 9)  - m_pEffectList[i]->m_dY);
					if (sAbsX > sAbsY) sDist = sAbsX -10;
					else sDist = sAbsY -10;
					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					PlaySound('E', 1, sDist, lPan);
				}
			
				break;

			case 188: // Lightning Storm xRisenx 
				m_Misc.GetPoint(m_pEffectList[i]->m_mX
				, m_pEffectList[i]->m_mY
				, m_pEffectList[i]->m_dX*32
				, m_pEffectList[i]->m_dY*32
				, &m_pEffectList[i]->m_mX
				, &m_pEffectList[i]->m_mY
				, &m_pEffectList[i]->m_iErr
				, 40);

				bAddNewEffect(151, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
				m_pEffectList[i]->m_dX +(rand()%5) -1, m_pEffectList[i]->m_dY +(rand()%5) -1, 0);
				bAddNewEffect(151, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
				m_pEffectList[i]->m_dX +(rand()%5) -1, m_pEffectList[i]->m_dY +(rand()%5) -1, 0);
				sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX); // Resolution ? 10 to 12? xRisenx Changed everyone in this function
				sAbsY = abs(((m_sViewPointY / 32) + 9) - m_pEffectList[i]->m_dY); // Resolution ? 7 to 9? xRisenx Changed everyone in this function
				bAddNewEffect(151, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
				m_pEffectList[i]->m_dX +(rand()%5) -1, m_pEffectList[i]->m_dY +(rand()%5) -1, 0);
				bAddNewEffect(151, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
				m_pEffectList[i]->m_dX +(rand()%5) -1, m_pEffectList[i]->m_dY +(rand()%5) -1, 0);
				sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX);
				sAbsY = abs(((m_sViewPointY / 32) + 9) - m_pEffectList[i]->m_dY);

				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{ 
				delete m_pEffectList[i];
				m_pEffectList[i] = 0;
				}
				else
				{ 
				sAbsX = abs(((m_sViewPointX / 32) + 12) - m_pEffectList[i]->m_dX);
				sAbsY = abs(((m_sViewPointY / 32) + 9) - m_pEffectList[i]->m_dY);
				if (sAbsX > sAbsY) sDist = sAbsX -10;
				else sDist = sAbsY -10;
				lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX); // Is this linked to Resolution? Change +10 to +12?
				PlaySound('E', 1, sDist, lPan);
				}
				break;

			case 104: // Thunder Storm xRisenx
				m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32
					, &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY, &m_pEffectList[i]->m_iErr, 40);

				bAddNewEffect(151, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
					m_pEffectList[i]->m_dX +(rand()%7) -6, m_pEffectList[i]->m_dY +(rand()%7) -6, 0);

				bAddNewEffect(151, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
					m_pEffectList[i]->m_dX +(rand()%10) -6, m_pEffectList[i]->m_dY +(rand()%10) -6, 0);

				//bAddNewEffect(6, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, NULL, NULL, 0, 0);

				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{       
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				else
				{       
					sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					sAbsY = abs(((m_sViewPointY / 32) + 7)  - m_pEffectList[i]->m_dY);

					if (sAbsX > sAbsY) sDist = sAbsX -10;
					else sDist = sAbsY -10;

					lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
					PlaySound('E', 1, sDist, lPan);
				}
				break;

			//case 105: // Inferno xRisenx
			//	m_Misc.GetPoint(m_pEffectList[i]->m_mX
			//		, m_pEffectList[i]->m_mY
			//		, m_pEffectList[i]->m_dX*32
			//		, m_pEffectList[i]->m_dY*32
			//		, &m_pEffectList[i]->m_mX
			//		, &m_pEffectList[i]->m_mY
			//		, &m_pEffectList[i]->m_iErr
			//		, 40);

			//	
			//	bAddNewEffect(66, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, NULL, NULL, 0, 1);
			//	
			//	if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
			//	{	
			//		delete m_pEffectList[i];
			//		m_pEffectList[i] = NULL;
			//	}
			//	else
			//	{	
			//		sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
			//		sAbsY = abs(((m_sViewPointY / 32) + 7)  - m_pEffectList[i]->m_dY);
			//		if (sAbsX > sAbsY) sDist = sAbsX -10;
			//		else sDist = sAbsY -10;
			//		lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
			//		PlaySound('E', 1, sDist, lPan);
			//	}
			//	break;
			//	// From what i understand this is where you make the animation for the spell when it hits the ground.
			//case 106: // Hell-Fire xRisenx
			//	if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
			//	{	bAddNewEffect(61, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, NULL, NULL, 0, 0);
			//		bAddNewEffect(63, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, NULL, NULL, 0, 0);
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		//bAddNewEffect(62, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, NULL, NULL, 0);

			//		bAddNewEffect(61, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY+65, NULL, NULL, 0, 0);
			//		bAddNewEffect(63, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY+65, NULL, NULL, 0, 0);
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 +65 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 +65 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 +65 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 +65 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 +65 - (rand() % 10), NULL, NULL, -1*(rand() % 2));

			//		bAddNewEffect(61, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY-65, NULL, NULL, 0, 0);
			//		bAddNewEffect(63, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY-65, NULL, NULL, 0, 0);
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 -65 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 -65 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 -65 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 -65 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 -65 - (rand() % 10), NULL, NULL, -1*(rand() % 2));

			//		bAddNewEffect(62, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, NULL, NULL, 0);
			//	/*bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(61, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, NULL, NULL, 0, 0);
			//		bAddNewEffect(63, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, NULL, NULL, 0, 0);
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//	for (x = 0; x < 16; x++)
			//	{
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*x-1);
			//	}
			//	for (x = 0; x < 8; x++)
			//	{
			//	bAddNewEffect(62 + (rand() % 2), m_pEffectList[i]->m_dX*32 +(rand() % 100) - 50 +10, m_pEffectList[i]->m_dY*32 + (rand() % 90) - 45, NULL, NULL, -1*x-1 -10);		}
			//	for (x = 0; x < 8; x++)
			//	{
			//	bAddNewEffect(62 + (rand() % 2), m_pEffectList[i]->m_dX*32 +(rand() % 100) - 60 +10, m_pEffectList[i]->m_dY*32 + (rand() % 90) - 45, NULL, NULL, -1*x-1 -10);
			//	}*/
			//		delete m_pEffectList[i];
			//		m_pEffectList[i] = NULL;
			//	}else if (m_pEffectList[i]->m_cFrame >= 0)
			//	{	m_pEffectList[i]->m_mX -= 30;
			//		m_pEffectList[i]->m_mY += 46;
			//		bAddNewEffect(62, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, NULL, NULL, 0, 0);
			//	}		
			//	/*bAddNewEffect(62, m_pEffectList[i]->m_dX*32, m_pEffectList[i]->m_dY*32, NULL, NULL, 0);
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*(rand() % 3));
			//	bAddNewEffect(61, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, NULL, NULL, 0, 0);
			//		bAddNewEffect(63, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, NULL, NULL, 0, 0);
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//		bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), NULL, NULL, -1*(rand() % 2));
			//	for (x = 0; x < 16; x++)
			//	{
			//	bAddNewEffect(62, m_pEffectList[i]->m_dX*32 +(rand() % 110) - 55 +10, m_pEffectList[i]->m_dY*32 + (rand() % 100) - 50, NULL, NULL, -1*x-1);
			//	}
			//	for (x = 0; x < 8; x++)
			//	{
			//	bAddNewEffect(62 + (rand() % 2), m_pEffectList[i]->m_dX*32 +(rand() % 100) - 50 +10, m_pEffectList[i]->m_dY*32 + (rand() % 90) - 45, NULL, NULL, -1*x-1 -10);		}
			//	for (x = 0; x < 8; x++)
			//	{
			//	bAddNewEffect(62 + (rand() % 2), m_pEffectList[i]->m_dX*32 +(rand() % 100) - 60 +10, m_pEffectList[i]->m_dY*32 + (rand() % 90) - 45, NULL, NULL, -1*x-1 -10);
			//	}

			//	delete m_pEffectList[i];
			//	m_pEffectList[i] = NULL;*/
			//				
			//	break;

			case 200:
				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	bAddNewEffect(203, m_pEffectList[i]->m_sX + 40, m_pEffectList[i]->m_sY + 120, 0, 0, 0, 0);
					bAddNewEffect(204, m_pEffectList[i]->m_sX -10, m_pEffectList[i]->m_sY +70, 0, 0, 0, 0);
					bAddNewEffect(205, m_pEffectList[i]->m_sX -10, m_pEffectList[i]->m_sY +75, 0, 0, 0, 0);
					bAddNewEffect(206, m_pEffectList[i]->m_sX -7, m_pEffectList[i]->m_sY +27, 0, 0, 0, 0);
					//bAddNewEffect(201, (rand() % 160) + 320, (rand() % 120) + 240, NULL, NULL, 0, 1);
					//bAddNewEffect(202, (rand() % 160) + 320, (rand() % 120) + 240, NULL, NULL, 0, 1);
					bAddNewEffect(201, (rand() % 160) + 400, (rand() % 120) + 300, 0, 0, 0, 1);
					bAddNewEffect(202, (rand() % 160) + 400, (rand() % 120) + 300, 0, 0, 0, 1);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 201:
				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	bAddNewEffect(203, m_pEffectList[i]->m_sX + 110, m_pEffectList[i]->m_sY + 120, 0, 0, 0, 0);
					bAddNewEffect(204, m_pEffectList[i]->m_sX -10, m_pEffectList[i]->m_sY +70, 0, 0, 0, 0);
					bAddNewEffect(205, m_pEffectList[i]->m_sX -10, m_pEffectList[i]->m_sY +75, 0, 0, 0, 0);
					//bAddNewEffect(202, (rand() % 160) + 320, (rand() % 120) + 240, NULL, NULL, 0, 1);
					bAddNewEffect(202, (rand() % 160) + 400, (rand() % 120) + 300, 0, 0, 0, 1);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 202:
				if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	bAddNewEffect(203, m_pEffectList[i]->m_sX + 65, m_pEffectList[i]->m_sY + 120, 0, 0, 0, 0);
					bAddNewEffect(204, m_pEffectList[i]->m_sX -10, m_pEffectList[i]->m_sY +70, 0, 0, 0, 0);
					bAddNewEffect(205, m_pEffectList[i]->m_sX -10, m_pEffectList[i]->m_sY +75, 0, 0, 0, 0);
					bAddNewEffect(206, m_pEffectList[i]->m_sX -7, m_pEffectList[i]->m_sY +27, 0, 0, 0, 0);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 250: // Gate round
				m_Misc.GetPoint(m_pEffectList[i]->m_mX
					, m_pEffectList[i]->m_mY
					, m_pEffectList[i]->m_dX*32
					, m_pEffectList[i]->m_dY*32 -40
					, &m_pEffectList[i]->m_mX
					, &m_pEffectList[i]->m_mY
					, &m_pEffectList[i]->m_iErr
					, 10);
				if (    (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX*32) <= 2)
					 && (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY*32 - 40)) <= 2) )
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 251: // Salmon burst (effect11s)
				m_Misc.GetPoint(m_pEffectList[i]->m_mX
					, m_pEffectList[i]->m_mY
					, m_pEffectList[i]->m_dX*32
					, m_pEffectList[i]->m_dY*32
					, &m_pEffectList[i]->m_mX
					, &m_pEffectList[i]->m_mY
					, &m_pEffectList[i]->m_iErr
					, 50);
				if (    (abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX*32) <= 2)
					 && (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY*32 - 40)) <= 2) )
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}else
				{	bAddNewEffect(252, m_pEffectList[i]->m_dX*32     , m_pEffectList[i]->m_dY*32     , 0, 0, 0, 1);
					bAddNewEffect(252, m_pEffectList[i]->m_dX*32 - 30, m_pEffectList[i]->m_dY*32 - 15, 0, 0, -7, 1);
					bAddNewEffect(252, m_pEffectList[i]->m_dX*32 - 35, m_pEffectList[i]->m_dY*32 - 30, 0, 0, -5, 1);
					bAddNewEffect(252, m_pEffectList[i]->m_dX*32 + 20, m_pEffectList[i]->m_dY*32 + 30, 0, 0, -3, 1);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;

			case 8:
			case 14:
			case 15:
			case 18:
			case 32:
			case 35: //
			case 36: //
			case 50:
			case 51:
			case 52:
			case 53:
			case 54:
			case 55:
			case 57:
			case 61:
			case 63:
			case 64:
			case 67:
			case 72:
			case 73:
			case 74:
			case 75:
			case 76:
			case 77:
			case 80: //
			case 82: //

			case 101:
			case 102:
			case 111:
			case 112:
			case 113:
			case 121:
			case 122:
			case 123: // Stamina Rec
			case 124:
			case 125:
			case 126:
			case 127:
			case 128: // Gr Stamina Rec
			case 131:
			case 132:
			case 133:
			case 134:
			case 135:
			case 136:
			case 142:
			case 144:
			case 150: // Berserk : Cirlcle 6 magic
			case 152: // Polymorph
			case 153:
			case 162:
			case 165:
			case 166:
			case 171:
			case 176: //
			case 177: //

			case 180:
			case 183: //
			//case 184: // EP's Magic Drain
			case 184: // xRisenx - Magic Shield
			case 185: // xRisenx - Mass Heal
			case 245: // Ghosts 1
			case 190:
			//case 192:
			//case 193:
			case 194:
			case 195:
			case 242: // Mage hero effect
			case 243: // War hero effect
			case 244: // Snoopy: d�plac� pour nvx sorts: Aura du casteur de Mass MagicMissile
				if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
				{	delete m_pEffectList[i];
					m_pEffectList[i] = 0;
				}
				break;
	}	}	}
	return bRet;
}

void CGame::GetPlayerTurn()
{
 char cDir;
 short sX, sY, sCnt1, sCnt2;
 int   iError;

	sX = m_sPlayerX;
	sY = m_sPlayerY;
	sCnt1 = 0;
	m_cPlayerTurn = 0;
	iError = 0;
	while (1) {
		cDir = cGetNextMoveDir(sX, sY, m_sCommX, m_sCommY);
		if (cDir == 0) break;
		switch (cDir) {
		case 1: sY--;       break;
		case 2: sX++; sY--; break;
		case 3: sX++;       break;
		case 4: sX++; sY++; break;
		case 5: sY++;       break;
		case 6: sX--; sY++; break;
		case 7: sX--;       break;
		case 8: sX--; sY--; break;
		}
		sCnt1++;
		if (sCnt1 > 30) break;
	}

	sX = m_sPlayerX;
	sY = m_sPlayerY;
	sCnt2 = 0;
	m_cPlayerTurn = 1;
	iError = 0;
	while (1) {
		cDir = cGetNextMoveDir(sX, sY, m_sCommX, m_sCommY);
		if (cDir == 0) break;
		switch (cDir) {
		case 1: sY--;       break;
		case 2: sX++; sY--; break;
		case 3: sX++;       break;
		case 4: sX++; sY++; break;
		case 5: sY++;       break;
		case 6: sX--; sY++; break;
		case 7: sX--;       break;
		case 8: sX--; sY--; break;
		}
		sCnt2++;
		if (sCnt2 > 30) break;
	}

	if (sCnt1 > sCnt2)
		 m_cPlayerTurn = 0;
	else m_cPlayerTurn = 1;
}

int CGame::_iCheckDlgBoxFocus(char cButtonSide)
{
	int i;
	char         cDlgID;
	short        sX, sY;
	short msX = G_pGame->m_stMCursor.sX;
	short msY = G_pGame->m_stMCursor.sY;
	uint32_t		  dwTime = m_dwCurTime;
	cDlgID = MouseOverDialog();

	if (cDlgID == 0) return 0;

	if (cButtonSide == 1) 
	{
		EnableDialogBox(cDlgID, 0, 0, 0);

		m_stMCursor.sPrevX = msX;
		m_stMCursor.sPrevY = msY;
		m_stMCursor.sDistX = msX - m_dialogBoxes[cDlgID].m_X;
		m_stMCursor.sDistY = msY - m_dialogBoxes[cDlgID].m_Y;

		switch (cDlgID)
		{
		case 1:
			if (DlgBoxPress_Character() == false) {
				m_stMCursor.cSelectedObjectType	= SELECTEDOBJTYPE_DLGBOX;
				m_stMCursor.sSelectedObjectID   = cDlgID;
			}
			break;

		case 2:	// (Sell Item)
			if (DlgBoxPress_Inventory() == false)
			{	
				m_stMCursor.cSelectedObjectType	= SELECTEDOBJTYPE_DLGBOX;
				m_stMCursor.sSelectedObjectID   = cDlgID;
			}
			break;

		case 26:
			if (DlgBoxPress_SkillDlg() == false) 
			{	
				m_stMCursor.cSelectedObjectType	= SELECTEDOBJTYPE_DLGBOX;
				m_stMCursor.sSelectedObjectID   = cDlgID;
			}
			break;

		default:
			if (m_dialogBoxes[cDlgID].IsScrollable())
			{
				if (m_dialogBoxes[cDlgID].IsOnScrollBar()) 
				{
					m_dialogBoxes[cDlgID].bIsScrollSelected = true;
				}

				if (m_dialogBoxes[cDlgID].bIsScrollSelected)
				{
					return -1;
				}
				else 
				{
					m_stMCursor.cSelectedObjectType	= SELECTEDOBJTYPE_DLGBOX;
					m_stMCursor.sSelectedObjectID   = cDlgID;
				}
			}
			else
			{
				m_stMCursor.cSelectedObjectType	= SELECTEDOBJTYPE_DLGBOX;
				m_stMCursor.sSelectedObjectID   = cDlgID;
			}
		}
	}
	else if (cButtonSide == 2)
	{	
		if ((dwTime - m_dwDialogCloseTime) < 300) return 0;

		char mode = m_dialogBoxes[cDlgID].GetMode();
		

		if(cDlgID == DIALOG_GUIDEMAP && m_altPressed)
		{
			Point p = GetGuideMapPos( G_pGame->m_stMCursor.sX,  G_pGame->m_stMCursor.sY );
			AddMapPing( MapPing(p.x, p.y) );
			bSendCommand(MSGID_PINGMAP, PINGMAP_PARTY | PINGMAP_GUILD, 0, p.x, p.y);
		} else if(cDlgID != 5 && cDlgID != 6 && cDlgID != 8 && cDlgID != 12 && 
			(cDlgID != 23 || mode < 3) && 
			cDlgID != 24 && cDlgID != 27 && cDlgID != 34 && cDlgID != 33 && 
			!(cDlgID == 32 && (mode == 1 || mode == 3)) &&
			!(cDlgID == DIALOG_MAILBOX && mode == 5)
			)
		{
			DisableDialogBox(cDlgID);
		}
		
		m_dwDialogCloseTime = dwTime;
	}
	
	return 1;
}

char CGame::MouseOverDialog()
{
	int i;
	int onDialog = 0;
	for (i = MAXDIALOGBOX - 1; i >= 0; i--)
	{
		if (m_cDialogBoxOrder[i] != 0 &&
			m_dialogBoxes[m_cDialogBoxOrder[i]].OnDialogBox())
		{
			onDialog = m_cDialogBoxOrder[i];
			break;
		}
	}
	return onDialog;
}

void CGame::InitItemList(char * pData)
{
char    cTotalItems;
 int     i, iAngelValue;
 short * sp;
 uint32_t * dwp;
 uint16_t  * wp;
 char  * cp;

	m_weight = 0;

	for (i = 0; i < MAXITEMS; i++)
		m_cItemOrder[i] = -1;

	for (i = 0; i < MAXITEMEQUIPPOS; i++)
		m_sItemEquipmentStatus[i] = -1;

	for (i = 0; i < MAXITEMS; i++)
		m_bIsItemDisabled[i] = false;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	cTotalItems = *cp;
	cp++;

	for (i = 0; i < MAXITEMS; i++)
	if (m_pItemList[i] != 0)
	{	delete m_pItemList[i];
		m_pItemList[i] = 0;
	}

	for (i = 0; i < MAXBANKITEMS; i++)
	if (m_pBankList[i] != 0)
	{	delete m_pBankList[i];
		m_pBankList[i] = 0;
	}

	for (i = 0; i < cTotalItems; i++)
	{
		m_pItemList[i] = new class CItem;
		memcpy(m_pItemList[i]->m_cName, cp, 20);
		cp += 20;
		dwp = (uint32_t *)cp;
		m_pItemList[i]->m_dwCount = *dwp;
		m_pItemList[i]->m_sX      =	40;
		m_pItemList[i]->m_sY      =	30;
		cp += 4;
		m_pItemList[i]->m_cItemType = *cp;
		cp++;
		m_pItemList[i]->m_cEquipPos = *cp;
		cp++;
		if( *cp == 0 ) m_bIsItemEquipped[i] = false;
		else m_bIsItemEquipped[i] = true;
		cp++;
		if (m_bIsItemEquipped[i] == true)
		{	m_sItemEquipmentStatus[m_pItemList[i]->m_cEquipPos] = i;
		}
		sp = (short *)cp;
		m_pItemList[i]->m_sLevelLimit = *sp;
		cp += 2;
		m_pItemList[i]->m_cGenderLimit = *cp;
		cp++;
		wp =(uint16_t *)cp;
		m_pItemList[i]->m_wCurLifeSpan = *wp;
		cp += 2;
		wp =(uint16_t *)cp;
		m_pItemList[i]->m_wWeight = *wp;
		cp += 2;
		sp = (short *)cp;
		m_pItemList[i]->m_sSprite = *sp;
		cp += 2;
		sp = (short *)cp;
		m_pItemList[i]->m_sSpriteFrame = *sp;
		cp += 2;
		dwp = (uint32_t *)cp;
		m_pItemList[i]->m_ItemColor = *dwp;
		cp += 4;
		m_pItemList[i]->m_sItemSpecEffectValue2 = (short)*cp;
		cp++;
		dwp = (uint32_t *)cp;
		m_pItemList[i]->m_dwAttribute = *dwp;
		cp += 4;
		for(int j = 0; j < MAXITEMSOCKETS; j++)
		{
			Pop(cp, m_pItemList[i]->m_sockets[j]);
		}
		m_cItemOrder[i] = i;
		// Snoopy: Add Angelic Stats
		if (   (m_pItemList[i]->m_cItemType == 1)
			&& (m_bIsItemEquipped[i] == true)
			&& (m_pItemList[i]->m_cEquipPos >= 11))
		{	if(memcmp(m_pItemList[i]->m_cName, "AngelicPendant(STR)", 19) == 0)
			{	iAngelValue = (m_pItemList[i]->m_dwAttribute & 0xF0000000) >> 28;
				//m_angelStat[STAT_STR] = 1 + iAngelValue;
				m_angelStat[STAT_STR] = 12 * iAngelValue; // Angel Stats xRisenx
			}else if(memcmp(m_pItemList[i]->m_cName, "AngelicPendant(DEX)", 19) == 0)
			{	iAngelValue = (m_pItemList[i]->m_dwAttribute & 0xF0000000) >> 28;
				//m_angelStat[STAT_DEX] = 1 + iAngelValue;
				m_angelStat[STAT_DEX] = 12 * iAngelValue; // Angel Stats xRisenx
			}else if(memcmp(m_pItemList[i]->m_cName, "AngelicPendant(INT)", 19) == 0)
			{	iAngelValue = (m_pItemList[i]->m_dwAttribute & 0xF0000000) >> 28;
				//m_angelStat[STAT_INT] = 1 + iAngelValue;
				m_angelStat[STAT_INT] = 12 * iAngelValue; // Angel Stats xRisenx
			}else if(memcmp(m_pItemList[i]->m_cName, "AngelicPendant(MAG)", 19) == 0)
			{	iAngelValue = (m_pItemList[i]->m_dwAttribute & 0xF0000000) >> 28;
				//m_angelStat[STAT_MAG] = 1 + iAngelValue;
				m_angelStat[STAT_MAG] = 12 * iAngelValue; // Angel Stats xRisenx
	}	}	}

	cTotalItems = *cp;
	_iCalcTotalWeight();
	cp++;

	for (i = 0; i < MAXBANKITEMS; i++)
	if (m_pBankList[i] != 0)
	{	delete m_pBankList[i];
		m_pBankList[i] = 0;
	}

	for (i = 0; i < cTotalItems; i++)
	{	m_pBankList[i] = new class CItem;
		memcpy(m_pBankList[i]->m_cName, cp, 20);
		cp += 20;

		dwp = (uint32_t *)cp;
		m_pBankList[i]->m_dwCount = *dwp;
		cp += 4;

		m_pBankList[i]->m_sX = 40;
		m_pBankList[i]->m_sY = 30;

		m_pBankList[i]->m_cItemType = *cp;
		cp++;

		m_pBankList[i]->m_cEquipPos = *cp;
		cp++;

		sp = (short *)cp;
		m_pBankList[i]->m_sLevelLimit = *sp;
		cp += 2;

		m_pBankList[i]->m_cGenderLimit = *cp;
		cp++;

		wp =(uint16_t *)cp;
		m_pBankList[i]->m_wCurLifeSpan = *wp;
		cp += 2;

		wp =(uint16_t *)cp;
		m_pBankList[i]->m_wWeight = *wp;
		cp += 2;

		sp = (short *)cp;
		m_pBankList[i]->m_sSprite = *sp;
		cp += 2;

		sp = (short *)cp;
		m_pBankList[i]->m_sSpriteFrame = *sp;
		cp += 2;

		dwp = (uint32_t*)cp;
		m_pBankList[i]->m_ItemColor = *dwp;
		cp += 4;

		m_pBankList[i]->m_sItemSpecEffectValue2 = (short)*cp;
		cp++;

		dwp = (uint32_t *)cp;
		m_pBankList[i]->m_dwAttribute = *dwp;
		cp += 4;
		for(int j = 0; j < MAXITEMSOCKETS; j++)
		{
			Pop(cp, m_pBankList[i]->m_sockets[j]);
		}
	}

	// Magic, Skill Mastery
	for (i = 0; i < MAXMAGICTYPE; i++)
	{	m_cMagicMastery[i] = *cp;
		cp++;
	}

	for (i = 0; i < MAXSKILLTYPE; i++)
	{	m_cSkillMastery[i] = (unsigned char)*cp;
		if (m_pSkillCfgList[i] != 0)
			m_pSkillCfgList[i]->m_iLevel = (int)*cp;
		cp++;
	}
}

void CGame::EnableDialogBox(int iBoxID, int cType, int sV1, int sV2, const char * pString)
{
 int i;
 short sX, sY;

	switch (iBoxID) {
	case 11:
		if (m_bIsDialogEnabled[11] == false) {
			switch (cType) {
			case 0:
				break;
			default:
				_LoadShopMenuContents(cType);
				m_dialogBoxes[11].sV1 = cType;
				m_dialogBoxes[11].SetMode(0);
				m_dialogBoxes[11].sView = 0;
				m_dialogBoxes[11].bFlag = true;
				m_dialogBoxes[11].sV3   = 1; 
				break;
			}
		}
		break;

	case 12: // levelup diag
		if (m_bIsDialogEnabled[12] == false)
		{	m_dialogBoxes[12].m_X = m_dialogBoxes[1].m_X + 20;
			m_dialogBoxes[12].m_Y = m_dialogBoxes[1].m_Y + 20;
			//m_iLU_Point = m_iLevel*3 - (
			m_iLU_Point = m_iLevel*7 - (
				(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
				- 70)
				//- 3 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
				- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			m_dialogBoxes[12].sV1 = m_iLU_Point;
		}
		break;

	case 3: // Magic Dialog
		break;

	case 4:
		if (m_bIsDialogEnabled[4] == false) {
			m_dialogBoxes[4].sView = cType;
		}
		break;

	case 5:
		if (m_bIsDialogEnabled[5] == false) {
			m_dialogBoxes[5].sView = cType;
		}
		break;

	case 6:
		if (m_bIsDialogEnabled[6] == false) {
			m_dialogBoxes[6].sView = cType;
		}
		break;

	case 7:
		if (m_dialogBoxes[7].GetMode() == 1) {
			sX = m_dialogBoxes[7].m_X;
			sY = m_dialogBoxes[7].m_Y;
			EndInputString();
			StartInputString(sX + 75, sY + 140, 21, m_cGuildName);
		}
		break;

	case 17: // demande quantit�
		if (m_bIsDialogEnabled[17] == false)
		{	
			m_dialogBoxes[iBoxID].SetMode(1);
			m_dialogBoxes[17].sView	= cType;
			EndInputString();
			ZeroMemory(m_cAmountString, sizeof(m_cAmountString));
			wsprintfA( m_cAmountString, "%d", sV1 );
			sX = m_dialogBoxes[17].m_X;
			sY = m_dialogBoxes[17].m_Y;
			StartInputString(sX + 40, sY + 57, 11, m_cAmountString, false);
		}else
		{	if (m_dialogBoxes[17].GetMode() == 1)
			{	sX = m_dialogBoxes[17].m_X;
				sY = m_dialogBoxes[17].m_Y;
				EndInputString();
				StartInputString(sX + 40, sY + 57, 11, m_cAmountString, false);
		}	}
		break;

	case 18:
		if (m_bIsDialogEnabled[18] == false)
		{	switch (cType) {
			case 0:
				m_dialogBoxes[18].SetMode(0);
				m_dialogBoxes[18].sView = 0;
				break;
			default:
				_LoadTextDlgContents(cType);
				m_dialogBoxes[18].SetMode(0);
				m_dialogBoxes[18].sView = 0;
				break;
		} 	}
		break;

	case 19:
		break;

	case 120:
		break;

	case 20: // Talk to npc or unicorn
		m_bIsItemDisabled[m_dialogBoxes[20].sV1] = false;
		if (m_bIsDialogEnabled[20] == false)
		{	m_dialogBoxes[11].sV1 = m_dialogBoxes[11].sV2 = m_dialogBoxes[11].sV3 =
			m_dialogBoxes[11].sV4 = m_dialogBoxes[11].sV5 = m_dialogBoxes[11].sV6 = 0;
			m_dialogBoxes[20].SetMode(cType);
			m_dialogBoxes[20].sView = 0;
			m_dialogBoxes[20].sV1 = sV1;
			m_dialogBoxes[20].sV2 = sV2;
		}
		break;

	case 21:
		if (m_bIsDialogEnabled[21] == false)
		{	m_dialogBoxes[21].SetMode(cType);
			m_dialogBoxes[21].sView = 0;
			m_dialogBoxes[21].sV1 = _iLoadTextDlgContents2(sV1+20);
			m_dialogBoxes[21].sV2 = sV1+20;
		}
		break;

	case 22:
		if (m_bIsDialogEnabled[22] == false) {
			m_dialogBoxes[22].sV1 = sV1;
			m_dialogBoxes[22].sV2 = sV2;

			m_dialogBoxes[22].sSizeX = 290;
			m_dialogBoxes[22].sSizeY = 290;
		}
		break;

	case 23:
		if (m_bIsDialogEnabled[23] == false) {
			m_dialogBoxes[23].SetMode(cType);
			m_dialogBoxes[23].sV1   = sV1;		// ItemID
			m_dialogBoxes[23].sV2   = sV2;
			if( cType == 2 ) {
				m_dialogBoxes[23].m_X = m_dialogBoxes[11].m_X;
				m_dialogBoxes[23].m_Y = m_dialogBoxes[11].m_Y;
			}
		}
		break;

	case 15:
		break;

	case 24:
		if (m_bIsDialogEnabled[24] == false)
		{	m_dialogBoxes[24].SetMode(cType);
			m_dialogBoxes[24].sV1   = sV1;
			m_dialogBoxes[24].sV2   = sV2;
			m_bSkillUsingStatus = true;
		}
		break;

	case 25:
		if (m_bIsDialogEnabled[25] == false) {
			m_dialogBoxes[25].SetMode(cType);
			m_dialogBoxes[25].sV1   = sV1;
			m_dialogBoxes[25].sV2   = sV2;
		}
		break;

	case 26:
		switch (cType) {
		case 1:
		case 2: //
			if (m_bIsDialogEnabled[26] == false)
			{	
				m_dialogBoxes[26].sV1   = -1;
				m_dialogBoxes[26].sV2   = -1;
				m_dialogBoxes[26].sV3   = -1;
				m_dialogBoxes[26].sV4   = -1;
				m_dialogBoxes[26].sV5   = -1;
				m_dialogBoxes[26].sV6   = -1;
				m_dialogBoxes[26].cStr[0] = 0;
				m_bSkillUsingStatus = true;
				m_dialogBoxes[26].sSizeX = 195;
				m_dialogBoxes[26].sSizeY = 215;
				m_dialogBoxes[26].SetMode(cType);
				DisableDialogBox(17);
				DisableDialogBox(20);
				DisableDialogBox(23);
			}
			break;

		case 3:	//
			if (m_bIsDialogEnabled[26] == false)
			{	
				m_dialogBoxes[26].sView = 0;
				m_dialogBoxes[26].sV1   = -1;
				m_dialogBoxes[26].sV2   = -1;
				m_dialogBoxes[26].sV3   = -1;
				m_dialogBoxes[26].sV4   = -1;
				m_dialogBoxes[26].sV5   = -1;
				m_dialogBoxes[26].sV6   = -1;
				m_dialogBoxes[26].cStr[0] = 0;
				m_dialogBoxes[26].cStr[1] = 0;
				m_dialogBoxes[26].cStr[4] = 0;
				m_bSkillUsingStatus = true;
				_bCheckBuildItemStatus();
				//m_stDialogBoxInfo[26].sX = 0;
				//m_stDialogBoxInfo[26].sY = 0;
				m_dialogBoxes[26].sSizeX = 270;
				m_dialogBoxes[26].sSizeY = 381;
				m_dialogBoxes[26].SetMode(cType);
				DisableDialogBox(17);
				DisableDialogBox(20);
				DisableDialogBox(23);
			}
			break;

		case 6:
			if (m_bIsDialogEnabled[26] == false)
			{	
				m_dialogBoxes[26].cStr[2] = sV1;
				m_dialogBoxes[26].cStr[3] = sV2;
				m_dialogBoxes[26].sSizeX = 270;
				m_dialogBoxes[26].sSizeY = 381;
				m_dialogBoxes[26].SetMode(cType);
				m_bSkillUsingStatus = true;
				_bCheckBuildItemStatus();
				DisableDialogBox(17);
				DisableDialogBox(20);
				DisableDialogBox(23);
			}
			break;
		// Crafting
		case 7:
		case 8:
			if (m_bIsDialogEnabled[26] == false)
			{	
				m_dialogBoxes[26].sV1   = -1;
				m_dialogBoxes[26].sV2   = -1;
				m_dialogBoxes[26].sV3   = -1;
				m_dialogBoxes[26].sV4   = -1;
				m_dialogBoxes[26].sV5   = -1;
				m_dialogBoxes[26].sV6   = -1;
				m_dialogBoxes[26].cStr[0] = 0;
				m_dialogBoxes[26].cStr[1] = 0;
				m_bSkillUsingStatus = true;
				//_bCheckCraftItemStatus();
				m_dialogBoxes[26].sSizeX = 195;
				m_dialogBoxes[26].sSizeY = 215;
				m_dialogBoxes[26].SetMode(cType);
				DisableDialogBox(17);
				DisableDialogBox(20);
				DisableDialogBox(23);
			}
			break;
		}
		break;

	case 27: // Snoopy: 7 mar 06 (multitrade) case rewriten
		if (m_bIsDialogEnabled[27] == false)
		{	m_dialogBoxes[27].SetMode(cType);
			for (i=0; i<8;i++)
			{	ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr1, sizeof(m_stDialogBoxExchangeInfo[i].cStr1));
				ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr2, sizeof(m_stDialogBoxExchangeInfo[i].cStr2));
				m_stDialogBoxExchangeInfo[i].sV1 = -1;
				m_stDialogBoxExchangeInfo[i].sV2 = -1;
				m_stDialogBoxExchangeInfo[i].sV3 = -1;
				m_stDialogBoxExchangeInfo[i].sV4 = -1;
				m_stDialogBoxExchangeInfo[i].sV5 = -1;
				m_stDialogBoxExchangeInfo[i].sV6 = -1;
				m_stDialogBoxExchangeInfo[i].sV7 = -1;
				m_stDialogBoxExchangeInfo[i].dwV1 = 0;
			}
			DisableDialogBox(17);
			DisableDialogBox(20);
			DisableDialogBox(23);
			DisableDialogBox(26);
		}
		break;

	case 41: // Snoopy: 7 mar 06 (MultiTrade) Confirmation dialog
		break;

	case 28:
		if (m_bIsDialogEnabled[28] == false) {
			m_dialogBoxes[28].m_X = m_dialogBoxes[1].m_X + 20;
			m_dialogBoxes[28].m_Y = m_dialogBoxes[1].m_Y + 20;
			m_dialogBoxes[28].SetMode(cType);
		}
		break;

	case 32:
		if (m_bIsDialogEnabled[32] == false) {
			m_dialogBoxes[32].m_X = m_dialogBoxes[1].m_X + 20;
			m_dialogBoxes[32].m_Y = m_dialogBoxes[1].m_Y + 20;
			m_dialogBoxes[32].SetMode(cType);
		}
		break;

	case 33:
		if ((m_iHP <= 0) || m_side == NEUTRAL) return;
		if (m_bIsDialogEnabled[33] == false)
		{	
			m_dialogBoxes[33].m_X  = 360;
			m_dialogBoxes[33].m_Y  = 65;
			m_dialogBoxes[33].sV1 = sV1;
			m_dialogBoxes[33].SetMode(cType);
		}
		break;

	case 34:
		if (m_bIsDialogEnabled[iBoxID] == false)
		{
			m_dialogBoxes[iBoxID].sV1  = -1;
			m_dialogBoxes[iBoxID].dwV1 = 0;
			m_dialogBoxes[iBoxID].SetMode(5);
		}
		break;

	case 16:
		if (m_bIsDialogEnabled[iBoxID]  == false) {
			if (m_cSkillMastery[SKILL_MAGIC] == 0) {
				 DisableDialogBox(16);
				 EnableDialogBox(21, 0, 480, 0);
				 return;
			}
			else {
				m_dialogBoxes[iBoxID].SetMode(0);
				m_dialogBoxes[iBoxID].sView = 0;
			}
		}
		break;

	case 14:
		EndInputString();
		if (m_bIsDialogEnabled[iBoxID]  == false) {
			m_dialogBoxes[iBoxID].SetMode(0);
			m_dialogBoxes[iBoxID].sView = 0;
			EnableDialogBox(2);
			DisableDialogBox(56);
		}
		break;

	case 40: // Slates
		if (m_bIsDialogEnabled[40] == false) {
			m_dialogBoxes[40].sView = 0;
			m_dialogBoxes[40].sV1   = -1;
			m_dialogBoxes[40].sV2   = -1;
			m_dialogBoxes[40].sV3   = -1;
			m_dialogBoxes[40].sV4   = -1;
			m_dialogBoxes[40].sV5   = -1;
			m_dialogBoxes[40].sV6   = -1;
			m_dialogBoxes[40].cStr[0] = 0;
			m_dialogBoxes[40].cStr[1] = 0;
			m_dialogBoxes[40].cStr[4] = 0;

			m_dialogBoxes[40].sSizeX = 180;
			m_dialogBoxes[40].sSizeY = 183;
			m_dialogBoxes[40].SetMode(cType);
			DisableDialogBox(17);
			DisableDialogBox(20);
			DisableDialogBox(23);
			DisableDialogBox(26);
		}
		break;

	case 42: // Diuuude: Change stats window
		if (m_bIsDialogEnabled[42] == false) {
			m_dialogBoxes[42].m_X = m_dialogBoxes[12].m_X+10;
			m_dialogBoxes[42].m_Y = m_dialogBoxes[12].m_Y+10;
			m_dialogBoxes[42].SetMode(0);
			m_dialogBoxes[42].sView = 0;
			m_bSkillUsingStatus = false;
		}
		break;

	case 43:
		if (m_bIsDialogEnabled[43] == false)
		{
			m_iFriendIndex = -1;
			m_dialogBoxes[43].sV1= 0;
		}
		break;

	case 44: // guildquery
		if (m_bIsDialogEnabled[44])
			break;
		m_dialogBoxes[44].SetMode(cType);
		m_dialogBoxes[44].sV1 = sV1; // current rank
		if (pString)
			strcpyn(m_dialogBoxes[44].cStr, pString);
		break;

	case 49: // guild teleport request
		if(!m_bIsDialogEnabled[49])
		{
			m_dialogBoxes[49].m_X = 185;
			m_dialogBoxes[49].m_Y = 20;
			m_dialogBoxes[49].SetMode(0);
			m_dialogBoxes[49].sView = 0;
			GetOfficialMapName(GetMapName(sV1), m_dialogBoxes[49].cStr);
		}
		break;

	case 50: // Resurection
		if (m_bIsDialogEnabled[50] == false)
		{
			m_dialogBoxes[50].m_X = 185;
			m_dialogBoxes[50].m_Y = 20;
			m_dialogBoxes[50].SetMode(0);
			m_dialogBoxes[50].sView = 0;
			m_bSkillUsingStatus = false;
		}
		break;

	case 52: //New Shop
		break;
	case 53: //new bsmith
		break;

	//GuildBank
	case 56:
		EndInputString();

		if (m_bIsDialogEnabled[iBoxID]  == false) {
			m_dialogBoxes[iBoxID].SetMode(0);
			m_dialogBoxes[iBoxID].sView = 0;
			if(m_guildBankVer == 0)
				m_guildBankIt = m_guildBankMap.end();
			else
				m_guildBankIt = m_guildBankMap.begin();
			EnableDialogBox(2);
			bSendCommand(MSGID_REQUEST_GUILDBANK);
			DisableDialogBox(14);
		}
		break;

	case 58:
		if(!m_bIsDialogEnabled[iBoxID])
		{
			m_dialogBoxes[iBoxID].SetMode(1);
			m_dialogBoxes[iBoxID].sView	= cType;

			EndInputString();
			sX = m_dialogBoxes[iBoxID].m_X;
			sY = m_dialogBoxes[iBoxID].m_Y;
			ZeroMemory(m_cAmountString, sizeof(m_cAmountString));
			StartInputString(sX + 40, sY + 57, 11, m_cAmountString);
		}
		else
		{
			// moving dialog
			if (m_dialogBoxes[iBoxID].GetMode() == 1)
			{
				sX = m_dialogBoxes[iBoxID].m_X;
				sY = m_dialogBoxes[iBoxID].m_Y;
				EndInputString();
				StartInputString(sX + 40, sY + 57, 11, m_cAmountString);
			}
		}
		break;

	case DIALOG_MAILBOX:
		EndInputString();

		if(!m_bIsDialogEnabled[iBoxID])
		{
			m_dialogBoxes[iBoxID].SetMode(0);
			m_dialogBoxes[iBoxID].sView = 0;
			bSendCommand(MSGID_REQ_MAILBOX);
		}
		break;

	case DIALOG_YESNO:
		if(!m_bIsDialogEnabled[iBoxID])
			m_dialogBoxes[iBoxID].SetMode(cType);
		break;

	default:
		EndInputString();
		if (m_bIsDialogEnabled[iBoxID]  == false) {
			m_dialogBoxes[iBoxID].SetMode(0);
			m_dialogBoxes[iBoxID].sView = 0;
		}
		break;
	}
	if( iBoxID != 30 )
	{	if (m_bIsDialogEnabled[iBoxID]  == false)
		//{	if( m_dialogBoxes[iBoxID].m_Y > 400 ) m_dialogBoxes[iBoxID].m_Y = 410;
		//	if( m_dialogBoxes[iBoxID].m_X > 620 ) m_dialogBoxes[iBoxID].m_X = 620;
			{	if( m_dialogBoxes[iBoxID].m_Y > 520 ) m_dialogBoxes[iBoxID].m_Y = 530;
			if( m_dialogBoxes[iBoxID].m_X > 780 ) m_dialogBoxes[iBoxID].m_X = 780;
			if( (m_dialogBoxes[iBoxID].m_X+m_dialogBoxes[iBoxID].sSizeX) < 10 ) m_dialogBoxes[iBoxID].m_X += 20;
			if( (m_dialogBoxes[iBoxID].m_Y+m_dialogBoxes[iBoxID].sSizeY) < 10 ) m_dialogBoxes[iBoxID].m_Y += 20;
	}	}
	m_bIsDialogEnabled[iBoxID] = true;
	if (pString != 0) strcpy(m_dialogBoxes[iBoxID].cStr, pString);

	for (i = 0; i < MAXDIALOGBOX -2; i++)
	if (m_cDialogBoxOrder[i] == iBoxID) m_cDialogBoxOrder[i] = 0;

	for (i = 1; i < MAXDIALOGBOX -2; i++)
	if ((m_cDialogBoxOrder[i-1] == 0) && (m_cDialogBoxOrder[i] != 0)) {
		m_cDialogBoxOrder[i-1] = m_cDialogBoxOrder[i];
		m_cDialogBoxOrder[i] = 0;
	}

	for (i = 0; i < MAXDIALOGBOX -2; i++)
	if (m_cDialogBoxOrder[i] == 0) {
		m_cDialogBoxOrder[i] = iBoxID;
		return;
	}
}

void CGame::DisableDialogBox(int iBoxID)
{
 int i;

	switch (iBoxID) {
	case 4:
		m_bIsItemDisabled[m_dialogBoxes[4].sView] = false;
		break;

	case 5:
		m_bIsItemDisabled[m_dialogBoxes[5].sView] = false;
		break;

	case 6:
		m_bIsItemDisabled[m_dialogBoxes[6].sView] = false;
		break;

	case 7:
		if (m_dialogBoxes[7].GetMode() == 1)
			EndInputString();
		m_dialogBoxes[7].SetMode(0);
		break;

	case 11:
		for (i = 0; i < MAXMENUITEMS; i++)
		if (m_pItemForSaleList[i] != 0) {
			delete m_pItemForSaleList[i];
			m_pItemForSaleList[i] = 0;
		}
		m_dialogBoxes[39].sV3 = 0;
		m_dialogBoxes[39].sV4 = 0; 
		m_dialogBoxes[39].sV5 = 0;
		m_dialogBoxes[39].sV6 = 0;
		break;

	case 14:
		if (m_dialogBoxes[14].GetMode() < 0) return;
		break;

	case 17:
		if (m_dialogBoxes[17].GetMode() == 1) {
			EndInputString();
			if(m_dialogBoxes[17].sV3 != 1004) // leave item disabled if sending mail
				m_bIsItemDisabled[m_dialogBoxes[17].sView] = false;
		}
		break;

	case 20:
		m_bIsItemDisabled[m_dialogBoxes[20].sV1] = false;
		break;

	case 21:
		if (m_dialogBoxes[21].sV2 == 500)
		{	bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GETMAGICABILITY, 0, 0, 0, 0, 0);
		}
		break;

	case 24:
		m_bSkillUsingStatus = false;
		break;

	case 26:
		if (m_dialogBoxes[26].sV1 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV1] = false;
		if (m_dialogBoxes[26].sV2 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV2] = false;
		if (m_dialogBoxes[26].sV3 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV3] = false;
		if (m_dialogBoxes[26].sV4 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV4] = false;
		if (m_dialogBoxes[26].sV5 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV5] = false;
		if (m_dialogBoxes[26].sV6 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV6] = false;
		m_bSkillUsingStatus = false;
		break;

	case 27: //Snoopy: 7 mar 06 (multiTrade) case rewriten
		for (i=0; i<8;i++)
		{	ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr1, sizeof(m_stDialogBoxExchangeInfo[i].cStr1));
			ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr2, sizeof(m_stDialogBoxExchangeInfo[i].cStr2));
			m_stDialogBoxExchangeInfo[i].sV1 = -1;
			m_stDialogBoxExchangeInfo[i].sV2 = -1;
			m_stDialogBoxExchangeInfo[i].sV3 = -1;
			m_stDialogBoxExchangeInfo[i].sV4 = -1;
			m_stDialogBoxExchangeInfo[i].sV5 = -1;
			m_stDialogBoxExchangeInfo[i].sV6 = -1;
			m_stDialogBoxExchangeInfo[i].sV7 = -1;
			m_stDialogBoxExchangeInfo[i].dwV1 = 0;
			if (m_bIsItemDisabled[m_stDialogBoxExchangeInfo[i].sItemID] == true)
				m_bIsItemDisabled[m_stDialogBoxExchangeInfo[i].sItemID] = false;
		}
		break;


	case 31:
		for (i = 0; i < MAXSELLLIST; i++)
		{	if (m_stSellItemList[i].iIndex != -1) m_bIsItemDisabled[m_stSellItemList[i].iIndex] = false;
			m_stSellItemList[i].iIndex = -1;
			m_stSellItemList[i].iAmount = 0;
		}
		break;

	case 34:
		if(m_dialogBoxes[34].sV1 != -1)
		m_bIsItemDisabled[m_dialogBoxes[34].sV1] = false;
		break;

	case 40:
		m_bIsItemDisabled[m_dialogBoxes[40].sV1] = false;
		m_bIsItemDisabled[m_dialogBoxes[40].sV2] = false;
		m_bIsItemDisabled[m_dialogBoxes[40].sV3] = false;
		m_bIsItemDisabled[m_dialogBoxes[40].sV4] = false;

		ZeroMemory(m_dialogBoxes[40].cStr, sizeof(m_dialogBoxes[40].cStr));
		ZeroMemory(m_dialogBoxes[40].cStr2, sizeof(m_dialogBoxes[40].cStr2));
		ZeroMemory(m_dialogBoxes[40].cStr3, sizeof(m_dialogBoxes[40].cStr3));
		ZeroMemory(m_dialogBoxes[40].cStr4, sizeof(m_dialogBoxes[40].cStr4));
		m_dialogBoxes[40].sV1   = -1;
		m_dialogBoxes[40].sV2   = -1;
		m_dialogBoxes[40].sV3   = -1;
		m_dialogBoxes[40].sV4   = -1;
		m_dialogBoxes[40].sV5   = -1;
		m_dialogBoxes[40].sV6   = -1;
		m_dialogBoxes[40].dwV1   = 0;
		m_dialogBoxes[40].dwV2   = 0;
		break;

	case 42:
		m_luStat[STAT_STR] = 0;
		m_luStat[STAT_VIT] = 0;
		m_luStat[STAT_DEX] = 0;
		m_luStat[STAT_INT] = 0;
		m_luStat[STAT_MAG] = 0;
		m_luStat[STAT_CHR] = 0;
		// Stats System xRisenx
		/*for (int i=0; i<TOTALLEVELUPPOINT; i++) {
			m_cStatChange[i] = 0;
		}
		break;*/
	//GuildBank
	case 56:
		if (m_dialogBoxes[56].GetMode() < 0) return;
		break;

	case DIALOG_GUILD:
		m_dialogBoxes[iBoxID].SetMode(0);
		if (m_dialogBoxes[iBoxID].GetMode() == 10) {
			EndInputString();
		}
		break;
		
	case DIALOG_GUILDCONTRIBUTE:
		if (m_dialogBoxes[iBoxID].GetMode() == 1) {
			EndInputString();
		}
		break;

	case DIALOG_MAILBOX:
		if (m_dialogBoxes[iBoxID].GetMode() == 1) {
			EndInputString();
		}
		break;
 	}

	m_bIsDialogEnabled[iBoxID] = false;

	for (i = 0; i < MAXDIALOGBOX -2; i++)
	if (m_cDialogBoxOrder[i] == iBoxID)
		m_cDialogBoxOrder[i] = 0;

	for (i = 1; i < MAXDIALOGBOX -2; i++)
	if ((m_cDialogBoxOrder[i-1] == 0) && (m_cDialogBoxOrder[i] != 0))
	{	m_cDialogBoxOrder[i-1] = m_cDialogBoxOrder[i];
		m_cDialogBoxOrder[i] = 0;
	}
}

int CGame::iGetTopDialogBoxIndex()
{
 int i;

	for (i = MAXDIALOGBOX -3; i >= 0; i--) //ignore top 2
	if (m_cDialogBoxOrder[i] != 0)
		return m_cDialogBoxOrder[i];

	return 0;
}

void CGame::DrawChatMsgs(short sX, short sY, short dX, short dY)
{
 int i;

	for (i = 0; i < MAXCHATMSGS; i++)
	if (m_pChatMsgList[i] != 0)
	if ( (m_pChatMsgList[i]->m_sX >= sX) && (m_pChatMsgList[i]->m_sX <= dX) &&
	     (m_pChatMsgList[i]->m_sY >= sY) && (m_pChatMsgList[i]->m_sY <= dY) ) {

		switch (m_pChatMsgList[i]->m_cType) {
		case 41:
		case 42:
		case 21:
		case 22:
		case 23:
			DrawChatMsgBox(m_pChatMsgList[i]->m_sX, m_pChatMsgList[i]->m_sY, i, false);
			break;
		}
	}

	//DIRECTX m_DDraw._GetBackBufferDC();
	for (i = 0; i < MAXCHATMSGS; i++)
	if (m_pChatMsgList[i] != 0)
	if ( (m_pChatMsgList[i]->m_sX >= sX) && (m_pChatMsgList[i]->m_sX <= dX) &&
	     (m_pChatMsgList[i]->m_sY >= sY) && (m_pChatMsgList[i]->m_sY <= dY) ) {

		switch (m_pChatMsgList[i]->m_cType) {
		case 41:
		case 42:
		case 21:
		case 22:
		case 23:
			break;

		case 20:
		default:
			DrawChatMsgBox(m_pChatMsgList[i]->m_sX, m_pChatMsgList[i]->m_sY, i, true);
			break;
		}
	}
	//DIRECTX m_DDraw._ReleaseBackBufferDC();
}

void CGame::_LoadTextDlgContents(int cType)
{char * pContents, * token, cTemp[120], cFileName[120];
 char   seps[] = "\n";
 int    iIndex = 0, i;
 class  CStrTok * pStrTok;
 uint32_t  dwFileSize;
 HANDLE hFile;
 FILE * pFile;
	for (i = 0; i < TEXTDLGMAXLINES; i++)
	{	if (m_pMsgTextList[i] != 0)
			delete m_pMsgTextList[i];
		m_pMsgTextList[i] = 0;
	}
	// cType
	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));

	wsprintfA(cTemp, "contents%d", cType);
	strcat(cFileName, "data\\shops");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cTemp);
	strcat(cFileName, ".txt");

	hFile = CreateFileA(cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	dwFileSize = GetFileSize(hFile, 0);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFileName, "rt");
	if (pFile == 0) return;
	else
	{ 	pContents = new char[dwFileSize+1];
		ZeroMemory(pContents, dwFileSize+1);
		fread(pContents, dwFileSize, 1, pFile);
	}
	fclose(pFile);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while( token != 0 )
	{	m_pMsgTextList[iIndex] = new class CMsg(0, token, 0);
		token = pStrTok->pGet();
		iIndex++;
	}
	delete pStrTok;
	delete[] pContents;
}

int CGame::_iLoadTextDlgContents2(int iType)
{char * pContents, * token, cTemp[120], cFileName[120];
 char   seps[] = "\n";
 int    iIndex = 0, i;
 class  CStrTok * pStrTok;
 uint32_t  dwFileSize;
 HANDLE hFile;
 FILE * pFile;
	for (i = 0; i < TEXTDLGMAXLINES; i++)
	{	if (m_pMsgTextList2[i] != 0)
			delete m_pMsgTextList2[i];
		m_pMsgTextList2[i] = 0;
	}
	// cType
	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));

	wsprintfA(cTemp, "contents%d", iType);

	strcat(cFileName, "data\\shops");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cTemp);
	strcat(cFileName, ".txt");
	hFile = CreateFileA(cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	dwFileSize = GetFileSize(hFile, 0);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFileName, "rt");
	if (pFile == 0) return -1;
	else
	{	pContents = new char[dwFileSize+1];
		if (pContents == 0) return -1;
		ZeroMemory(pContents, dwFileSize+1);
		fread(pContents, dwFileSize, 1, pFile);
	}
	fclose(pFile);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while( token != 0 )
	{	m_pMsgTextList2[iIndex] = new class CMsg(0, token, 0);
		token = pStrTok->pGet();
		iIndex++;
	}
	delete pStrTok;
	delete[] pContents;
	return iIndex;
}

void CGame::_RequestMapStatus(char * pMapName, int iMode)
{
	bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_MAPSTATUS, 0, iMode, 0, 0, pMapName);
}

void CGame::AddMapStatusInfo(char * pData, bool bIsLastData)
{
 char * cp, cTotal;
 short * sp, sIndex;
 int i;

	ZeroMemory(m_cStatusMapName, sizeof(m_cStatusMapName));

	cp = (char *)(pData + 6);
	memcpy(m_cStatusMapName, cp, 10);
	cp += 10;

	sp = (short *)cp;
	sIndex = *sp;
	cp += 2;

	cTotal = *cp;
	cp++;

	for (i = 1; i <= cTotal; i++) {
		m_stCrusadeStructureInfo[sIndex].cType = *cp;
		cp++;
		sp = (short *)cp;
		m_stCrusadeStructureInfo[sIndex].sX = *sp;
		cp += 2;
		sp = (short *)cp;
		m_stCrusadeStructureInfo[sIndex].sY = *sp;
		cp += 2;
		m_stCrusadeStructureInfo[sIndex].cSide = *cp;
		cp++;

		sIndex++;
	}

	if (bIsLastData == true) {
		while (sIndex < MAXCRUSADESTRUCTURES) {
			m_stCrusadeStructureInfo[sIndex].cType = 0;
			m_stCrusadeStructureInfo[sIndex].sX = 0;
			m_stCrusadeStructureInfo[sIndex].sY = 0;
			m_stCrusadeStructureInfo[sIndex].cSide = 0;
			sIndex++;
		}
	}
}

bool CGame::GetText(HWND hWnd,UINT msg,WPARAM wparam, LPARAM lparam)
{
	int len;
	HIMC hIMC=0;

	if (m_pInputBuffer == 0) return false;
	switch (msg)
	{
		/*case WM_IME_COMPOSITION:
			ZeroMemory(m_cEdit, sizeof(m_cEdit));
			if (lparam & GCS_RESULTSTR)
			{
				hIMC = ImmGetContext(hWnd);
				len = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);
				if( len > 4 ) len = 4;
				ImmGetCompositionString(hIMC, GCS_RESULTSTR, m_cEdit, len);
				ImmReleaseContext(hWnd, hIMC);
				len = strlen(m_pInputBuffer) + strlen(m_cEdit);
				if (len < m_inputMaxLen) strcpy(m_pInputBuffer+strlen(m_pInputBuffer),m_cEdit);
				ZeroMemory(m_cEdit, sizeof(m_cEdit));
			}
			else if (lparam & GCS_COMPSTR)
			{
				hIMC = ImmGetContext(hWnd);
				len = ImmGetCompositionString(hIMC, GCS_COMPSTR, NULL, 0);
				if( len > 4 ) len = 4;
				ImmGetCompositionString(hIMC, GCS_COMPSTR, m_cEdit, len);
				ImmReleaseContext(hWnd, hIMC);
				len = strlen(m_pInputBuffer) + strlen(m_cEdit);
				if (len >= m_inputMaxLen) ZeroMemory(m_cEdit, sizeof(m_cEdit));
			}
			return TRUE;*/

		case WM_CHAR:
			if(wparam == 8)
			{
				if(strlen(m_pInputBuffer) > 0)
				{
					len = strlen(m_pInputBuffer);

					switch (GetCharKind(m_pInputBuffer, len-1))
					{
					case 1:
						m_pInputBuffer[len-1] = 0;
						break;
					case 2:
					case 3:
						m_pInputBuffer[len-2]  = 0;
						m_pInputBuffer[len-1]  = 0;
						break;
					}
					ZeroMemory(m_cEdit, sizeof(m_cEdit));
				}
			}
			else if ((wparam != 9) && (wparam != 13) && (wparam != 27)) {
				len = strlen(m_pInputBuffer);
				if (len >= m_inputMaxLen-1) return false;
				m_pInputBuffer[len] = wparam & 0xff;
				m_pInputBuffer[len+1] = 0;
			}

			return true;
	}
	return false;
}

int CGame::GetCharKind(char *str, int index)
{	int kind=1;
    do
	{	if(kind==2) kind=3;
		else
		{	if((unsigned char) *str < 128) kind=1;
	    	else kind=2;
		}
		str++;
		index--;
    }
	while(index>=0);
    return kind;
}

void CGame::ShowReceivedString(bool bIsHide)
{
	ZeroMemory(G_cTxt2, sizeof(G_cTxt2));

#ifdef USING_WIN_IME
	if( G_hEditWnd != 0 ) GetWindowText(G_hEditWnd, m_pInputBuffer, (int)m_inputMaxLen);
	strcpy(G_cTxt2, m_pInputBuffer);
#else
	strcpy(G_cTxt2, m_pInputBuffer);
	if( (m_cEdit[0] != 0) && ( strlen(m_pInputBuffer)+strlen(m_cEdit)+1 <= m_inputMaxLen ) )
	{
		strcpy(G_cTxt2 + strlen(m_pInputBuffer), m_cEdit);
	}
#endif
	if (bIsHide == true)
	{
		for (unsigned char i = 0; i < strlen(G_cTxt2); i++)
			if (G_cTxt2[i] != 0) G_cTxt2[i] = '*';
	}

	if( (G_dwGlobalTime%400) < 210 ) 
		G_cTxt2[strlen(G_cTxt2)] = '_';

	if(m_iInputX2 == 0)
	{
		PutString(m_iInputX+1, m_iInputY+1, G_cTxt2,video::SColor(255,0,0,0));
		PutString(m_iInputX, m_iInputY+1, G_cTxt2,video::SColor(255,0,0,0));
		PutString(m_iInputX+1, m_iInputY, G_cTxt2,video::SColor(255,0,0,0));
		PutString(m_iInputX, m_iInputY, G_cTxt2,video::SColor(255,255,255,255));
	} else {
		PutAlignedString(m_iInputX+1, m_iInputX2+1, m_iInputY+1, G_cTxt2,video::SColor(255,0,0,0));
		PutAlignedString(m_iInputX, m_iInputX2, m_iInputY+1, G_cTxt2,video::SColor(255,0,0,0));
		PutAlignedString(m_iInputX+1, m_iInputX2+1, m_iInputY, G_cTxt2,video::SColor(255,0,0,0));
		PutAlignedString(m_iInputX, m_iInputX2, m_iInputY, G_cTxt2,video::SColor(255,255,255,255));
	}
}

void CGame::ClearInputString()
{
	if (m_pInputBuffer != 0)	ZeroMemory(m_pInputBuffer, sizeof(m_pInputBuffer));
	ZeroMemory(m_cEdit, sizeof(m_cEdit));
#ifdef USING_WIN_IME
	if (G_hEditWnd != 0)	SetWindowText(G_hEditWnd, "");
#endif
}

void CGame::StartInputString(int left, int top, uint32_t len, char * pBuffer, bool bIsHide, int right)
{
	m_bInputStatus = true;
	m_iInputX = left;
	m_iInputY = top;
	m_iInputX2 = right;
	m_pInputBuffer = pBuffer;
	ZeroMemory(m_cEdit, sizeof(m_cEdit));
	m_inputMaxLen = len;
#ifdef USING_WIN_IME
	if (bIsHide == false) G_hEditWnd = CreateWindow( RICHEDIT_CLASS, 0, WS_POPUP | ES_SELFIME, sX-5 , sY -1, len*12, 16, G_hWnd, (HMENU)0, G_hInstance, 0);
	else G_hEditWnd = CreateWindow( RICHEDIT_CLASS, 0, WS_POPUP | ES_PASSWORD | ES_SELFIME, sX-5 , sY -1, len*12, 16, G_hWnd, (HMENU)0, G_hInstance, 0);
	SetWindowText(G_hEditWnd, m_pInputBuffer);
	SendMessage(G_hEditWnd, EM_EXLIMITTEXT, 0, len-1 );
	SendMessage(G_hEditWnd, EM_SETLANGOPTIONS, 0, ~IMF_AUTOFONT);
	COMPOSITIONFORM composform;
	composform.dwStyle = CFS_POINT;
	composform.ptCurrentPos.x = sX;
	composform.ptCurrentPos.y = sY;
	HIMC hImc = ImmGetContext(G_hWnd);
	ImmSetCompositionWindow( hImc, &composform );
	int StrLen = strlen( m_pInputBuffer );
	SendMessage(G_hEditWnd, EM_SETSEL, StrLen, StrLen);
#endif
}

void CGame::EndInputString()
{	m_bInputStatus = false;
#ifdef USING_WIN_IME
	if (G_hEditWnd != 0)
	{	GetWindowText(G_hEditWnd, m_pInputBuffer, (int)m_inputMaxLen);
		CANDIDATEFORM candiform;
		SendMessage(G_hEditWnd, WM_IME_CONTROL, IMC_GETCANDIDATEPOS, (LPARAM)&candiform);
		DestroyWindow(G_hEditWnd);
		G_hEditWnd = 0;
	}
#else
	int len = strlen(m_cEdit);
	if (len > 0)
	{	m_cEdit[len] = 0;
		strcpy(m_pInputBuffer+strlen(m_pInputBuffer),m_cEdit);
		ZeroMemory( m_cEdit, sizeof(m_cEdit) );
	}
#endif
}

void CGame::ReceiveString(char *pString)
{
	strcpy(pString, m_pInputBuffer);

#ifdef USING_WIN_IME
	if (G_hEditWnd != 0) GetWindowText(G_hEditWnd, pString, (int)m_inputMaxLen);
#endif
}

void CGame::SetCameraShakingEffect(short sDist, int iMul)
{
 int iDegree;

	iDegree = 5 - sDist;
	if (iDegree <= 0) iDegree = 0;
	iDegree *= 2;

	if (iMul != 0) iDegree *= iMul;

	if (iDegree <= 2) return;

	m_iCameraShakingDegree = iDegree;
}

void CGame::MeteorStrikeComing(int iCode)
{
	switch (iCode) {
	case 1: //
		SetTopMsg(CRUSADE_MESSAGE0, 5);
		break;
	case 2: //
		SetTopMsg(CRUSADE_MESSAGE10, 10);
		break;
	case 3: //
		SetTopMsg(CRUSADE_MESSAGE90, 5);
		break;
	case 4: //
		SetTopMsg(CRUSADE_MESSAGE11, 10);
		break;
	}
}

void CGame::DrawObjectFOE(int ix, int iy, int iFrame)
{	if( _iGetFOE(_tmp_iStatus) < 0 ) // red crusade circle
	{	if (iFrame <= 4 ) m_pEffectSpr[38]->PutTransSprite(ix, iy, iFrame, G_dwGlobalTime);
	}
}

void CGame::SetTopMsg(const char *pString, unsigned char iLastSec)
{
	ZeroMemory(m_cTopMsg, sizeof(m_cTopMsg));
	strcpy(m_cTopMsg, pString);

	m_iTopMsgLastSec = iLastSec;
	m_dwTopMsgTime   = G_dwGlobalTime;
}

void CGame::DrawTopMsg()
{
	if (strlen(m_cTopMsg) == 0) return;
	////DIRECTX m_DDraw.DrawShadowBox(0, 0, 639, 30);
	//DIRECTX m_DDraw.DrawShadowBox(0, 20, 799, 55); // 800x600 Resolution xRisenx

	if ((((G_dwGlobalTime - m_dwTopMsgTime)/250) % 2) == 0)
		//PutAlignedString(0, 639, 10, m_cTopMsg, 255,255,255);
		PutAlignedString(0, GetWidth()-1, 30, m_cTopMsg, video::SColor(255,255,255,255)); // 800x600 Resolution xRisenx

	if ( G_dwGlobalTime > (m_iTopMsgLastSec*CLOCKS_PER_SEC+m_dwTopMsgTime) ) {
		ZeroMemory(m_cTopMsg, sizeof(m_cTopMsg));
	}
}

void CGame::DrawAstoriaStats()
{
	//wsprintfA( G_cTxt, "Kills");
	//PutString_SprFont3(30, 150, G_cTxt, m_wR[14]*4, m_wG[14]*4, m_wB[14]*4, TRUE, 2);
	
	wsprintfA( G_cTxt, "%s: %d", sideName[ARESDEN], m_astoriaStats[ARESDEN].kills);
	PutString_SprFont3(10, 160, G_cTxt, m_wR[14]*4, m_wG[14]*4, m_wB[14]*4, true, 2);
	wsprintfA( G_cTxt, "%s: %d", sideName[ELVINE], m_astoriaStats[ELVINE].kills);
	PutString_SprFont3(10, 175, G_cTxt, m_wR[14]*4, m_wG[14]*4, m_wB[14]*4, true, 2);

	if(m_astoriaShieldsHP[ARESDEN] != 0 && m_astoriaShieldsHP[ELVINE] != 0)
	{
		wsprintfA( G_cTxt, "%s Shield: %d", sideName[ARESDEN], m_astoriaShieldsHP[ARESDEN]);
		PutString_SprFont3(10, 200, G_cTxt, m_wR[14]*4, m_wG[14]*4, m_wB[14]*4, true, 2);
		wsprintfA( G_cTxt, "%s Shield: %d", sideName[ELVINE], m_astoriaShieldsHP[ELVINE]);
		PutString_SprFont3(10, 215, G_cTxt, m_wR[14]*4, m_wG[14]*4, m_wB[14]*4, true, 2);
	}

	if(m_relicOwnedTime != 0)
	{
		uint32_t currTime = unixtime();
		wsprintfA( G_cTxt, "Time left for %s to win: %u:%.2u", sideName[m_relicOwnedSide],
			((m_relicOwnedTime + RELICVICTORYTIME - currTime) / 1000) / 60,
			((m_relicOwnedTime + RELICVICTORYTIME - currTime) / 1000) % 60
			);
		PutString_SprFont3(190, 25, G_cTxt, m_wR[3]*4, m_wG[3]*4, m_wB[3]*4, true, 2);
	}
}

void CGame::TimeStamp(char * pString)
{
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	wsprintfA(pString, "%02d:%02d:%02d", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
}

void CGame::EnableChat(ChatType t, bool enable)
{
	if((enable == m_chatToggle[t]))
		return;

	m_chatToggle[t] = !m_chatToggle[t];

	if(m_chatMsgs[t].size() == 0)
		return;

	if(enable)
	{
		std::vector<CMsg*> * result = new std::vector<CMsg*>(m_chatDisplay->size() + m_chatMsgs[t].size());
		merge(m_chatDisplay->begin(), m_chatDisplay->end(), 
			m_chatMsgs[t].begin(), m_chatMsgs[t].end(), result->begin(), &CMsg::OrderPred);

		delete m_chatDisplay;
		m_chatDisplay = result;
	}
	else
	{
		CMsg::SetPred(t);
		std::vector<CMsg*>::iterator remIt = remove_if(m_chatDisplay->begin(), m_chatDisplay->end(), CMsg::RemovePred);
		m_chatDisplay->erase(remIt, m_chatDisplay->end());
	}
}

void CGame::CannotConstruct(int iCode)
{
	switch (iCode) {
	case 1:
		SetTopMsg(CRUSADE_MESSAGE18, 5);
		break;

	case 2:
		wsprintfA(G_cTxt, "%s XY(%d, %d)", CRUSADE_MESSAGE19, m_iConstructLocX, m_iConstructLocY);
		SetTopMsg(G_cTxt, 5);
		break;

	case 3:
		SetTopMsg(CRUSADE_MESSAGE20, 5);
		break;

	case 4:
		SetTopMsg(CRUSADE_MESSAGE20, 5);
		break;

	case 5:
		SetTopMsg(CRUSADE_MESSAGE20B, 5);
		break;
	}
}

void CGame::DisplayCommaNumber_G_cTxt(int iGold)
{char cGold[20];
 int iStrLen;
	ZeroMemory(cGold, sizeof(cGold));
	ZeroMemory(G_cTxt, sizeof(G_cTxt));
	_itoa(iGold, cGold, 10);
#ifdef COMMA_GOLD
	iStrLen = strlen(cGold);
	iStrLen--;
	int cnt = 0;
	for (int i = 0 ; i < iStrLen+1 ; i++)
	{	if( (cnt != 0) && ((cnt+1)%4 == 0) )
		{	G_cTxt[cnt] = ',';
			i--;
		}else G_cTxt[cnt] = cGold[iStrLen-i];
		cnt++;
	}
	iStrLen = strlen(G_cTxt);
	G_cTxt[iStrLen] = '\0';
	_strrev(G_cTxt);
#else
	strcpy(G_cTxt, cGold);
#endif
}


void CGame::CrusadeContributionResult(int iWarContribution)
{int i;
 char cTemp[120];
	DisableDialogBox(18);
	for (i = 0; i < TEXTDLGMAXLINES; i++)
	{
        if (m_pMsgTextList[i] != 0)
			delete m_pMsgTextList[i];
		m_pMsgTextList[i] = 0;
	}
	if (iWarContribution > 0)
	{
        PlaySound('E', 23, 0, 0);
		PlaySound('C', 21, 0, 0);
		PlaySound('C', 22, 0, 0);
		m_pMsgTextList[0] = new class CMsg(0, CRUSADE_MESSAGE22, 0);
		m_pMsgTextList[1] = new class CMsg(0, CRUSADE_MESSAGE23, 0);
		m_pMsgTextList[2] = new class CMsg(0, " ", 0);
		m_pMsgTextList[3] = new class CMsg(0, CRUSADE_MESSAGE24, 0);
		m_pMsgTextList[4] = new class CMsg(0, CRUSADE_MESSAGE25, 0);
		m_pMsgTextList[5] = new class CMsg(0, CRUSADE_MESSAGE26, 0);
		m_pMsgTextList[6] = new class CMsg(0, " ", 0);
		m_pMsgTextList[7] = new class CMsg(0, CRUSADE_MESSAGE27, 0);
		m_pMsgTextList[8] = new class CMsg(0, " ", 0);
		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintfA(cTemp, "You Gained %dExp Points!", iWarContribution*35);
		m_pMsgTextList[9] = new class CMsg(0, cTemp, 0);
		for (i = 9; i < 18; i++)
		m_pMsgTextList[i] = new class CMsg(0, " ", 0);

	}
    else if (iWarContribution < 0)
	{
        PlaySound('E', 24, 0, 0);
		PlaySound('C', 12, 0, 0);
		PlaySound('C', 13, 0, 0);
		m_pMsgTextList[0] = new class CMsg(0, CRUSADE_MESSAGE28, 0);
		m_pMsgTextList[1] = new class CMsg(0, CRUSADE_MESSAGE29, 0);
		m_pMsgTextList[2] = new class CMsg(0, " ", 0);
		m_pMsgTextList[3] = new class CMsg(0, CRUSADE_MESSAGE30, 0);
		m_pMsgTextList[4] = new class CMsg(0, CRUSADE_MESSAGE31, 0);
		m_pMsgTextList[5] = new class CMsg(0, CRUSADE_MESSAGE32, 0);
		m_pMsgTextList[6] = new class CMsg(0, " ", 0);
		m_pMsgTextList[7] = new class CMsg(0, CRUSADE_MESSAGE33, 0);
		m_pMsgTextList[8] = new class CMsg(0, CRUSADE_MESSAGE34, 0);
		for (i = 9; i < 18; i++)
		m_pMsgTextList[i] = new class CMsg(0, " ", 0);
	}
    else if (iWarContribution == 0)
	{
        PlaySound('E', 25, 0, 0);
		m_pMsgTextList[0] = new class CMsg(0, CRUSADE_MESSAGE50, 0); // The battle that you have participated
		m_pMsgTextList[1] = new class CMsg(0, CRUSADE_MESSAGE51, 0); // is already finished;
		m_pMsgTextList[2] = new class CMsg(0, CRUSADE_MESSAGE52, 0); //
		m_pMsgTextList[3] = new class CMsg(0, " ", 0);
		m_pMsgTextList[4] = new class CMsg(0, CRUSADE_MESSAGE53, 0); // You must connect after finishing
		m_pMsgTextList[5] = new class CMsg(0, CRUSADE_MESSAGE54, 0); // the previous and before starting
		m_pMsgTextList[6] = new class CMsg(0, CRUSADE_MESSAGE55, 0); // the next battle so you can receive
		m_pMsgTextList[7] = new class CMsg(0, CRUSADE_MESSAGE56, 0); // the prize
		for (i = 8; i < 18; i++)
		m_pMsgTextList[i] = new class CMsg(0, " ", 0);
	}
	EnableDialogBox(18);
}


void CGame::CrusadeWarResult(int iWinnerSide)
{
	int i;

	DisableDialogBox(18);
	for (i = 0; i < TEXTDLGMAXLINES; i++)
	{	if (m_pMsgTextList[i] != 0)
			delete m_pMsgTextList[i];
		m_pMsgTextList[i] = 0;
	}

	if (m_side == 0)
	{	switch (iWinnerSide) {
		case 0:
			PlaySound('E', 25, 0, 0);
			m_pMsgTextList[0] = new class CMsg(0, CRUSADE_MESSAGE35, 0); // All out war finished!
			m_pMsgTextList[1] = new class CMsg(0, CRUSADE_MESSAGE36, 0); // There was a draw in the
			m_pMsgTextList[2] = new class CMsg(0, CRUSADE_MESSAGE37, 0); // battle
			m_pMsgTextList[3] = new class CMsg(0, " ", 0);
			break;
		case 1:
			PlaySound('E', 25, 0, 0);
			m_pMsgTextList[0] = new class CMsg(0, CRUSADE_MESSAGE35, 0); // All out war finished!
			m_pMsgTextList[1] = new class CMsg(0, CRUSADE_MESSAGE38, 0); // Aresden was victorious
			m_pMsgTextList[2] = new class CMsg(0, CRUSADE_MESSAGE39, 0); // and put an end to the war
			m_pMsgTextList[3] = new class CMsg(0, " ", 0);
			break;
		case 2:
			PlaySound('E', 25, 0, 0);
			m_pMsgTextList[0] = new class CMsg(0, CRUSADE_MESSAGE35, 0); // All out war finished!
			m_pMsgTextList[1] = new class CMsg(0, CRUSADE_MESSAGE40, 0); // Elvine was victorious
			m_pMsgTextList[2] = new class CMsg(0, CRUSADE_MESSAGE41, 0); // and put an end to the war
			m_pMsgTextList[3] = new class CMsg(0, " ", 0);
			break;
		}
		for (i = 4; i < 18; i++)
			m_pMsgTextList[i] = new class CMsg(0, " ", 0);
	}else
	{	if (iWinnerSide == 0)
		{	PlaySound('E', 25, 0, 0);
			m_pMsgTextList[0] = new class CMsg(0, CRUSADE_MESSAGE35, 0); // All out war finished!
			m_pMsgTextList[1] = new class CMsg(0, CRUSADE_MESSAGE36, 0); // There was a draw in the
			m_pMsgTextList[2] = new class CMsg(0, CRUSADE_MESSAGE37, 0); // battle
			m_pMsgTextList[3] = new class CMsg(0, " ", 0);
			for (i = 4; i < 18; i++)
				m_pMsgTextList[i] = new class CMsg(0, " ", 0);
		}else
		{ 	if (iWinnerSide == m_side)
			{	PlaySound('E', 23, 0, 0);
				PlaySound('C', 21, 0, 0);
				PlaySound('C', 22, 0, 0);
				switch (iWinnerSide) {
				case 1:
					m_pMsgTextList[0] = new class CMsg(0, CRUSADE_MESSAGE35, 0); // All out war finished!;
					m_pMsgTextList[1] = new class CMsg(0, CRUSADE_MESSAGE38, 0); // Aresden was victorious;
					m_pMsgTextList[2] = new class CMsg(0, CRUSADE_MESSAGE39, 0); // and put an end to the war
					m_pMsgTextList[3] = new class CMsg(0, " ", 0);
					m_pMsgTextList[4] = new class CMsg(0, CRUSADE_MESSAGE42, 0); // Congratulations!
					m_pMsgTextList[5] = new class CMsg(0, CRUSADE_MESSAGE43, 0); // As a victorious citizen
					m_pMsgTextList[6] = new class CMsg(0, CRUSADE_MESSAGE44, 0); // You will receive
					m_pMsgTextList[7] = new class CMsg(0, CRUSADE_MESSAGE45, 0); // a prize
					break;
				case 2:
					m_pMsgTextList[0] = new class CMsg(0, CRUSADE_MESSAGE35, 0); // All out war finished!
					m_pMsgTextList[1] = new class CMsg(0, CRUSADE_MESSAGE40, 0); // Elvine was victorious
					m_pMsgTextList[2] = new class CMsg(0, CRUSADE_MESSAGE41, 0); // and put an end to the war
					m_pMsgTextList[3] = new class CMsg(0, " ", 0);
					m_pMsgTextList[4] = new class CMsg(0, CRUSADE_MESSAGE42, 0); // Congratulations!
					m_pMsgTextList[5] = new class CMsg(0, CRUSADE_MESSAGE43, 0); // As a victorious citizen
					m_pMsgTextList[6] = new class CMsg(0, CRUSADE_MESSAGE44, 0); // You will receive
					m_pMsgTextList[7] = new class CMsg(0, CRUSADE_MESSAGE45, 0); // a prize
					break;
				}
				for (i = 8; i < 18; i++)
					m_pMsgTextList[i] = new class CMsg(0, " ", 0);
			}else if (iWinnerSide != m_side)
			{	PlaySound('E', 24, 0, 0);
				PlaySound('C', 12, 0, 0);
				PlaySound('C', 13, 0, 0);
				switch (iWinnerSide) {
				case 1:
					m_pMsgTextList[0] = new class CMsg(0, CRUSADE_MESSAGE35, 0); // All out war finished!
					m_pMsgTextList[1] = new class CMsg(0, CRUSADE_MESSAGE38, 0); // Aresden was victorious;
					m_pMsgTextList[2] = new class CMsg(0, CRUSADE_MESSAGE39, 0); // and put an end to the war
					m_pMsgTextList[3] = new class CMsg(0, " ", 0);
					m_pMsgTextList[4] = new class CMsg(0, CRUSADE_MESSAGE46, 0); // Unfortunately,
					m_pMsgTextList[5] = new class CMsg(0, CRUSADE_MESSAGE47, 0); // As a losser citizen
					m_pMsgTextList[6] = new class CMsg(0, CRUSADE_MESSAGE48, 0); // the prize that accomplishes
					m_pMsgTextList[7] = new class CMsg(0, CRUSADE_MESSAGE49, 0); // will not be given.
					break;
				case 2:
					m_pMsgTextList[0] = new class CMsg(0, CRUSADE_MESSAGE35, 0); // All out war finished!
					m_pMsgTextList[1] = new class CMsg(0, CRUSADE_MESSAGE40, 0); // Elvine was victorious
					m_pMsgTextList[2] = new class CMsg(0, CRUSADE_MESSAGE41, 0); // and put an end to the war
					m_pMsgTextList[3] = new class CMsg(0, " ", 0);
					m_pMsgTextList[4] = new class CMsg(0, CRUSADE_MESSAGE46, 0); // Unfortunately,
					m_pMsgTextList[5] = new class CMsg(0, CRUSADE_MESSAGE47, 0); // As a losser citizen
					m_pMsgTextList[6] = new class CMsg(0, CRUSADE_MESSAGE48, 0); // the prize that accomplishes
					m_pMsgTextList[7] = new class CMsg(0, CRUSADE_MESSAGE49, 0); // will not be given.
					break;
				}
				for (i = 8; i < 18; i++)
					m_pMsgTextList[i] = new class CMsg(0, " ", 0);
	}	}	}
	EnableDialogBox(18);
	DisableDialogBox(36);
	DisableDialogBox(37);
	DisableDialogBox(38);
}

void CGame::_Draw_UpdateScreen_OnCreateNewAccount()
{
	//DIRECTX m_DDraw.ClearBackB4();
/*
	DrawNewDialogBox(SPRID_INTERFACE_ND_NEWACCOUNT, 0,0,0, true);
	PutString2((800/2)-80, (600/2)-120, m_cAccountName, 200,200,200);
	PutString( (800/2)-80, (600/2)-120+20, m_cAccountPassword,video::SColor(255,200,200,200), true, 1);
	PutString( (800/2)-80, (600/2)-120+20*2, m_cAccountPassword,video::SColor(255,200,200,200), true, 1);
	PutString2((800/2)-80, (600/2)-120+20*3, m_cAccountCountry, 200,200,200);
	PutString2((800/2)-80, (600/2)-120+20*4, m_cAccountSSN, 200,200,200);
	PutString2((800/2)-80, (600/2)-120+20*5, m_cEmailAddr, 200,200,200);*/
	/*//DIRECTX m_DDraw.ClearBackB4();
	DrawNewDialogBox(SPRID_INTERFACE_ND_NEWACCOUNT, 0,0,0, TRUE);
	PutString2(329, 110, m_cAccountName, 200,200,200);
	PutString( 329, 125, m_cAccountPassword,video::SColor(255,200,200,200), TRUE, 1);
	PutString( 329, 140, m_cAccountPassword,video::SColor(255,200,200,200), TRUE, 1);
	PutString2(300, 202, m_cAccountCountry, 200,200,200);
	PutString2(300, 218, m_cAccountSSN, 200,200,200);
	PutString2(194, 257, m_cEmailAddr, 200,200,200);*/
}

void CGame::DrawChatMsgBox(short sX, short sY, int iChatIndex, bool bIsPreDC)
{
 char cMsg[200], cMsgA[22], cMsgB[22], cMsgC[22], * cp;
 int  iRet, iLines, i, iSize, iSize2, iLoc, iFontSize;
 uint32_t dwTime;
 video::SColor rgb;
 bool bIsTrans;
 RECT rcRect;
 SIZE Size;

	ZeroMemory(cMsg, sizeof(cMsg));
	ZeroMemory(cMsgA, sizeof(cMsgA));
	ZeroMemory(cMsgB, sizeof(cMsgB));
	ZeroMemory(cMsgC, sizeof(cMsgC));

	if(iChatIndex >= MAXCHATMSGS)
	{
#ifdef _DEBUG
		MessageBoxA(0, "check _tmp_cName (must be a buffer overflow)", "ASSERTION FAILED", MB_OK);
#else
		return;
#endif
	}

	dwTime = m_pChatMsgList[iChatIndex]->m_dwTime;
	strncpy(cMsg, m_pChatMsgList[iChatIndex]->m_pMsg, sizeof(cMsg) -1);
	cp = (char *)cMsg;
	iLines = 0;

	rgb =video::SColor(255,255,255,255);
	switch (m_pChatMsgList[iChatIndex]->m_cType) {
	case 1:
		rgb =video::SColor(255,255,255,255);
		break;
	case 20:
		rgb =video::SColor(255,255,255,20);
				if ((m_dwCurTime - dwTime) < 650) return;
		else dwTime += 650;
		break;
	case 41:
		rgb =video::SColor(255,255,80,80);
		break;

	case 42:
		rgb =video::SColor(255,255,80,80);
		if ((m_dwCurTime - dwTime) < 650) return;
		else dwTime += 650;
		break;
	}

	if (strlen(cp) != 0) {
		memcpy(cMsgA, cp, 20);

		iRet = GetCharKind(cMsgA, 19);
		if (iRet == CODE_HAN1) {
			cMsgA[20] = cp[20];
			cp++;
		}
		cp += 20;
		iLines = 1;
	}

	if (strlen(cp) != 0) {
		memcpy(cMsgB, cp, 20);

		iRet = GetCharKind(cMsgB, 19);
		if (iRet == CODE_HAN1) {
			cMsgB[20] = cp[20];
			cp++;
		}
		cp += 20;
		iLines = 2;
	}

	if (strlen(cp) != 0) {
		memcpy(cMsgC, cp, 20);

		iRet = GetCharKind(cMsgC, 19);
		if (iRet == CODE_HAN1) {
			cMsgC[20] = cp[20];
			cp++;
		}
		cp += 20;
		iLines = 3;
	}

	iSize = 0;
	for (i = 0; i < 20; i++)
	if (cMsgA[i] != 0)

	if ((unsigned char)cMsgA[i] >= 128) {
		iSize += 5;	//6
		i++;
	}
	else iSize += 4;

	iLoc = m_dwCurTime - dwTime;
	switch (m_pChatMsgList[iChatIndex]->m_cType) {
	case 21:
	case 22:
	case 23://...
		if( iLoc > 80 ) iLoc = 10;
		else iLoc = iLoc>>3;
		break;
	default://
		if( iLoc > 352 ) iLoc = 9;
		else if( iLoc > 320 ) iLoc = 10;
		else iLoc = iLoc>>5;
		break;
	}

	//TODO: remove low quality text, this isn't 1999 anymore
// 	if (m_cDetailLevel == 0)
// 		 bIsTrans = false;
// 	else bIsTrans = true;

	switch (m_pChatMsgList[iChatIndex]->m_cType) {
	case 41:
	case 42:
		iSize2 = 0;
		for (i = 0; i < 100; i++)
		if (cMsg[i] != 0)
		if ((unsigned char)cMsg[i] >= 128) {
			iSize2 += 5;
			i++;
		}
		else iSize2 += 4;
		if( m_Misc.bCheckIMEString(cMsg) == false )
		{
			PutString3(sX - iSize2, sY - 65 - iLoc, cMsg,video::SColor(255,180, 30, 30));
		}
		else PutString_SprFont3(sX - iSize2, sY - 65 - iLoc, cMsg, m_wR[14]*4, m_wG[14]*4, m_wB[14]*4, false, 0);
		break;

	case 21:
	case 22:
	case 23:
		iFontSize = 23 - (int)m_pChatMsgList[iChatIndex]->m_cType;
		switch (iLines) {
		case 1:
			PutString_SprFont3(sX - iSize, sY - 65 - iLoc, cMsgA, 255, 255, 0, bIsTrans, iFontSize);
			break;
		case 2:
			PutString_SprFont3(sX - iSize, sY - 81 - iLoc, cMsgA, 255, 255, 0, bIsTrans, iFontSize);
			PutString_SprFont3(sX - iSize, sY - 65 - iLoc, cMsgB, 255, 255, 0, bIsTrans, iFontSize);
			break;
		case 3:
			PutString_SprFont3(sX - iSize, sY - 97 - iLoc, cMsgA, 255, 255, 0, bIsTrans, iFontSize);
			PutString_SprFont3(sX - iSize, sY - 81 - iLoc, cMsgB, 255, 255, 0, bIsTrans, iFontSize);
			PutString_SprFont3(sX - iSize, sY - 65 - iLoc, cMsgC, 255, 255, 0, bIsTrans, iFontSize);
			break;
		}
		break;

	case 20:
	default:
		//if (bIsPreDC == FALSE)
			//DIRECTX m_DDraw._GetBackBufferDC();

		//GetTextExtentPoint32(//DIRECTX m_DDraw.m_hDC, cMsg, strlen(cMsg), &Size);

// 		switch (Size.cx / 160) {
// 		case 0:
		PutChatString(sX - 80 + 1, sY - 65 - iLoc, (char*)cMsg, video::SColor(255, 0, 0, 0));
			//SetRect(&rcRect, sX - 80 + 1, sY - 65 - iLoc, sX + 80 + 1, sY - iLoc);
			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg,video::SColor(255,0,0,0));

		PutChatString(sX - 80, sY - 65 - iLoc + 1, (char*)cMsg, video::SColor(255, 0, 0, 0));
			//SetRect(&rcRect, sX-80, sY-65 -iLoc +1, sX+80, sY -iLoc +1);
			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg,video::SColor(255,0,0,0));

		PutChatString(sX - 80, sY - 65 - iLoc, (char*)cMsg, rgb);
			//SetRect(&rcRect, sX-80, sY-65 -iLoc, sX+80, sY -iLoc);
			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg, rgb);
// 			break;
// 
// 		case 1:
// 			//SetRect(&rcRect, sX-80 +1, sY-83 -iLoc, sX+80 +1, sY -iLoc);
// 			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg,video::SColor(255,0,0,0));
// 
// 			//SetRect(&rcRect, sX-80, sY-83 -iLoc +1, sX+80, sY -iLoc +1);
// 			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg,video::SColor(255,0,0,0));
// 
// 			//SetRect(&rcRect, sX-80, sY-83 -iLoc, sX+80, sY -iLoc);
// 			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg, rgb);
// 			break;
// 
// 		case 2:
// 			//SetRect(&rcRect, sX-80 +1, sY-101 -iLoc, sX+80 +1, sY -iLoc);
// 			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg,video::SColor(255,0,0,0));
// 
// 			//SetRect(&rcRect, sX-80, sY-101 -iLoc +1, sX+80, sY -iLoc +1);
// 			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg,video::SColor(255,0,0,0));
// 
// 			//SetRect(&rcRect, sX-80, sY-101 -iLoc, sX+80, sY -iLoc);
// 			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg, rgb);
// 			break;
// 
// 		case 3:
// 			//SetRect(&rcRect, sX-80 +1, sY-119 -iLoc, sX+80 +1, sY -iLoc);
// 			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg,video::SColor(255,0,0,0));
// 
// 			//SetRect(&rcRect, sX-80, sY-119 -iLoc +1, sX+80, sY -iLoc +1);
// 			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg,video::SColor(255,0,0,0));
// 
// 			//SetRect(&rcRect, sX-80, sY-119 -iLoc, sX+80, sY -iLoc);
// 			//DIRECTX m_DDraw.DrawText(&rcRect, cMsg, rgb);
// 			break;
// 		}

		//if (bIsPreDC == FALSE)
			//DIRECTX m_DDraw._ReleaseBackBufferDC();
		break;
	}
}

void CGame::ClearContents_OnSelectCharacter()
{
	m_cCurFocus = 1;
}



void CGame::CivilRightAdmissionHandler(char *pData)
{uint16_t * wp, wResult;
 char * cp;

	wp   = (uint16_t *)(pData + INDEX2_MSGTYPE);
	wResult = *wp;

	switch (wResult) {
	case 0:
		m_dialogBoxes[13].SetMode(4);
		break;

	case 1:
		m_dialogBoxes[13].SetMode(3);
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		ZeroMemory(m_cLocation, sizeof(m_cLocation));
		memcpy(m_cLocation, cp, 10);

		if (memcmp(m_cLocation, "are", 3) == 0)
			m_side = ARESDEN;
		else if (memcmp(m_cLocation, "elv", 3) == 0)
			m_side = ELVINE;
		else if (memcmp(m_cLocation, "ist", 3) == 0)
			m_side = ISTRIA;
		else
			m_side = NEUTRAL;
		break;
	}
}

void CGame::_RemoveChatMsgListByObjectID(int iObjectID)
{
 int i;

	for (i = 1; i < MAXCHATMSGS; i++)
	if ((m_pChatMsgList[i] != 0) && (m_pChatMsgList[i]->m_iObjectID == iObjectID)) {
		delete m_pChatMsgList[i];
		m_pChatMsgList[i] = 0;
	}
}

void CGame::PlaySound(char cType, int iNum, int iDist, long lPan)
{
 int iVol;

	if (m_bSoundFlag == false) return;
	if (m_bSoundStat == false) return;

	if (iDist > 10) iDist = 10;

	iVol = (m_cSoundVolume - 100)*30;
	iVol += -200 * iDist;

	if (iVol > 0) iVol = 0;
	if (iVol < -10000) iVol = -10000;

    //int width = GetWidth();
    //int pan = lPan

	switch (cType) {
		case 'C':
			if (m_pCSound[iNum] == nullptr) return;
            //m_pCSound[iNum]->Play(FALSE, lPan, iVol);
            m_pCSound[iNum]->setPan((float)lPan / 5000);
            m_pCSound[iNum]->setVolume((iVol + 10000)/10000);
            klang->play2D(m_pCSound[iNum]->getSoundSource());
			break;

		case 'M':
			if (m_pMSound[iNum] == nullptr) return;
			//m_pMSound[iNum]->Play(FALSE, lPan, iVol);
            m_pMSound[iNum]->setPan((float)lPan / 5000);
            m_pMSound[iNum]->setVolume((iVol + 10000) / 10000);
            klang->play2D(m_pMSound[iNum]->getSoundSource());
            break;

		case 'E':
			if (m_pESound[iNum] == nullptr) return;
			//m_pESound[iNum]->Play(FALSE, lPan, iVol);
            m_pESound[iNum]->setPan((float)lPan / 5000);
            m_pESound[iNum]->setVolume((iVol + 10000) / 10000);
            klang->play2D(m_pESound[iNum]->getSoundSource());
            break;
	}
}

void CGame::_DrawBlackRect(int iSize)
{
 int ix, iy, sx, sy, dcx, dcy;
 uint32_t dwTime;

	dwTime = unixtime();

	dcx = 40 - iSize*2;
	dcy = 30 - iSize*2;

	sx = iSize*16;
	sy = iSize*16;

	for (ix = 0; ix < dcx; ix++) {
		m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(ix*16 + sx, iSize*16,       12, dwTime);
		m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(ix*16 + sx, 464 - iSize*16, 12, dwTime);
	}

	for (iy = 0; iy < dcy; iy++) {
		m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(iSize*16,       iy*16 + sy, 12, dwTime);
		m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(624 - iSize*16, iy*16 + sy, 12, dwTime);
	}
}

bool CGame::_bCheckItemByType(char cType)
{
 int i;

	for (i = 0; i < MAXITEMS; i++)
	if ( (m_pItemList[i] != 0) && (m_pItemList[i]->m_cItemType == cType) ) return true;

	return false;
}


void CGame::DynamicObjectHandler(char * pData)
{
 uint16_t * wp;
 char * cp;
 short * sp, sX, sY, sV1, sV2, sV3;

	cp = (char *)(pData + INDEX2_MSGTYPE);
	wp = (uint16_t *)cp;
	cp += 2;

	sp = (short *)cp;
	sX = *sp;
	cp += 2;

	sp = (short *)cp;
	sY = *sp;
	cp += 2;

	sp = (short *)cp;
	sV1 = *sp;
	cp += 2;

	sp = (short *)cp;
	sV2 = *sp;		   // Dyamic Object Index
	cp += 2;

	sp = (short *)cp;
	sV3 = *sp;
	cp += 2;

	switch (*wp) {
	case MSGTYPE_CONFIRM:// Dynamic Object
		m_pMapData->bSetDynamicObject(sX, sY, sV2, sV1, true);
		break;

	case MSGTYPE_REJECT:// Dynamic object
		m_pMapData->bSetDynamicObject(sX, sY, sV2, 0, true);
		break;
	}
}

bool CGame::_bIsItemOnHand() // Snoopy: Fixed to remove ShieldCast
{int i;
 uint16_t wWeaponType;
	for (i = 0; i < MAXITEMS; i++)
	if ((m_pItemList[i] != 0) && (m_bIsItemEquipped[i] == true))
	{	if (   (m_pItemList[i]->m_cEquipPos == EQUIPPOS_LHAND)
			|| /*(*/(m_pItemList[i]->m_cEquipPos == EQUIPPOS_TWOHAND))/* && !(wWeaponType == 35)))*/ // Battle Mages xRisenx
			//|| (m_pItemList[i]->m_cEquipPos == EQUIPPOS_TWOHAND))
			return true;
	}
	for (i = 0; i < MAXITEMS; i++)
	if ((m_pItemList[i] != 0) && (m_bIsItemEquipped[i] == true))
	{	if (m_pItemList[i]->m_cEquipPos == EQUIPPOS_RHAND)
		{	wWeaponType = ((m_sPlayerAppr2 & 0x0FF0) >> 4);
			// Snoopy 34 for all wands.
			if ((wWeaponType >= 34) && (wWeaponType < 41)) return false;
			//else if( wWeaponType == 27 ) return FALSE; // Farming's hoe !
			else return true;
	}	}
	return false;
}

int CGame::_iCalcTotalWeight()
{
 int i, iWeight, iCnt, iTemp;
	iCnt = 0;
	iWeight = 0;
	for (i = 0; i < MAXITEMS; i++)
	if (m_pItemList[i] != 0)
	{	if (   (m_pItemList[i]->m_cItemType == ITEMTYPE_CONSUME)
			|| (m_pItemList[i]->m_cItemType == ITEMTYPE_ARROW) )
		{	iTemp = m_pItemList[i]->m_wWeight * m_pItemList[i]->m_dwCount;
			if ((strcmp(m_pItemList[i]->m_cName, "Gold") == 0) || (strcmp(m_pItemList[i]->m_cName, "XtremeTokens") == 0)) iTemp = iTemp / 20;
			iWeight += iTemp;
		}else iWeight += m_pItemList[i]->m_wWeight;
		iCnt++;
	}
	m_weight = iWeight/100;
	return iWeight;
}



int CGame::iGetLevelExp(int iLevel)
{int iRet;
	if (iLevel == 0) return 0;
	iRet = iGetLevelExp(iLevel - 1) + iLevel * ( 50 + (iLevel * (iLevel / 17) * (iLevel / 17) ) );
	return iRet;
}

int CGame::_iGetTotalItemNum()
{
	int i, iCnt;
	iCnt = 0;
	for (i = 0; i < MAXITEMS; i++)
	if (m_pItemList[i] != 0) iCnt++;
	return iCnt;
}

uint32_t CGame::GetGold() const
{
	for(int i = 0; i < MAXITEMS; i++)
	{
		if(m_pItemList[i] && strcmp(m_pItemList[i]->m_cName, "Gold") == 0)
			return m_pItemList[i]->m_dwCount;
	}

	return 0;
}

void CGame::SetGold(uint32_t val)
{
	for(int i = 0; i < MAXITEMS; i++)
	{
		if(m_pItemList[i] && strcmp(m_pItemList[i]->m_cName, "Gold") == 0)
		{
			m_pItemList[i]->m_dwCount = val;
			_iCalcTotalWeight();
			return;
		}
	}
}

void CGame::DrawWeatherEffects()
{
#define MAXNUM 1000
 static int ix1[MAXNUM];
 static int iy2[MAXNUM];
 static int iFrame[MAXNUM];
 static int iNum = 0;
 int i;
 short dX, dY, sCnt;
 char cTempFrame;
 uint32_t dwTime = m_dwCurTime;

	switch (m_weather) {
	case WEATHER_LIGHTRAIN:
	case WEATHER_MEDIUMRAIN:
	case WEATHER_HEAVYRAIN:
		switch (m_weather) {
		case WEATHER_LIGHTRAIN:		sCnt = MAXWEATHEROBJECTS / 5; break;
		case WEATHER_MEDIUMRAIN:	sCnt = MAXWEATHEROBJECTS / 2; break;
		case WEATHER_HEAVYRAIN:		sCnt = MAXWEATHEROBJECTS;     break;
		}

		for (i = 0; i < sCnt; i++)
		{	if ((m_stWeatherObject[i].cStep >= 0) && (m_stWeatherObject[i].cStep < 20) && (m_stWeatherObject[i].sX != 0))
			{	dX = m_stWeatherObject[i].sX - m_sViewPointX;
				dY = m_stWeatherObject[i].sY - m_sViewPointY;
				cTempFrame = 16 + (m_stWeatherObject[i].cStep / 6);
				m_pEffectSpr[11]->PutTransSprite(dX, dY, cTempFrame, dwTime);
 			}else if ((m_stWeatherObject[i].cStep >= 20) && (m_stWeatherObject[i].cStep < 25) && (m_stWeatherObject[i].sX != 0))
			{	dX = m_stWeatherObject[i].sX - m_sViewPointX;
				dY = m_stWeatherObject[i].sY - m_sViewPointY;
				m_pEffectSpr[11]->PutTransSprite(dX, dY, m_stWeatherObject[i].cStep, dwTime);
		}	}
		break;

	case WEATHER_LIGHTSNOW:
	case WEATHER_MEDIUMSNOW:
	case WEATHER_HEAVYSNOW:
		switch (m_weather) {
		case WEATHER_LIGHTSNOW: sCnt = MAXWEATHEROBJECTS / 5; break;
		case WEATHER_MEDIUMSNOW:	sCnt = MAXWEATHEROBJECTS / 2; break;
		case WEATHER_HEAVYSNOW:	sCnt = MAXWEATHEROBJECTS;     break;
		}
		for (i = 0; i < sCnt; i++)
		{	if ((m_stWeatherObject[i].cStep >= 0) && (m_stWeatherObject[i].cStep < 80))
			{	dX = m_stWeatherObject[i].sX - m_sViewPointX;
				dY = m_stWeatherObject[i].sY - m_sViewPointY;

				// Snoopy: Snow on lower bar		
				if (dY >= 460)
				{	cTempFrame = 39 + (m_stWeatherObject[i].cStep / 20)*3;
					dX = m_stWeatherObject[i].sBX;
					dY = 426;// 426
				}else cTempFrame = 39 + (m_stWeatherObject[i].cStep / 20)*3 + (rand() % 3);

				m_pEffectSpr[11]->PutTransSprite(dX, dY, cTempFrame, dwTime);

			if (m_bIsXmas == true)
			{
					if( dY == 478 - 53 )
					{	ix1[iNum] = dX;
						iy2[iNum] = dY + (rand()%5);
						iFrame[iNum] = cTempFrame;
						iNum++;
					}
					if( iNum >= MAXNUM ) iNum = 0;
			}	
		
		}	}
		if (m_bIsXmas == true)
		{
			for (i = 0; i < MAXNUM; i++)
			{	if( iy2[i] > 10 ) m_pEffectSpr[11]->PutTransSprite(ix1[i], iy2[i], iFrame[i], dwTime);
		}	
		}
		break;
	}
}

void CGame::WeatherObjectFrameCounter()
{
 int i;
 short sCnt;
 char  cAdd;
 uint32_t dwTime = m_dwCurTime;

	if ((dwTime - m_dwWOFtime) < 30) return;
	m_dwWOFtime = dwTime;

	switch (m_weather) {
	case WEATHER_LIGHTRAIN:
	case WEATHER_MEDIUMRAIN:
	case WEATHER_HEAVYRAIN:
		switch (m_weather) {
		case WEATHER_LIGHTRAIN:		sCnt = MAXWEATHEROBJECTS / 5; break;
		case WEATHER_MEDIUMRAIN:	sCnt = MAXWEATHEROBJECTS / 2; break;
		case WEATHER_HEAVYRAIN:		sCnt = MAXWEATHEROBJECTS;     break;
		}
		for (i = 0; i < sCnt; i++)
		{	m_stWeatherObject[i].cStep++;
			if ((m_stWeatherObject[i].cStep >= 0) && (m_stWeatherObject[i].cStep < 20))
			{	cAdd = (40 - m_stWeatherObject[i].cStep);
				if (cAdd < 0) cAdd = 0;
				m_stWeatherObject[i].sY = m_stWeatherObject[i].sY + cAdd;
				if (cAdd != 0)
					m_stWeatherObject[i].sX = m_stWeatherObject[i].sX - 1;
			}else if (m_stWeatherObject[i].cStep >= 25)
			{	if (m_weather == WEATHER_SUNNY)
				{	m_stWeatherObject[i].sX    = 0;
					m_stWeatherObject[i].sY    = 0;
					m_stWeatherObject[i].cStep = 30;
				}else
				{	m_stWeatherObject[i].sX    = (m_pMapData->m_sPivotX*32) + ((rand() % 940) - 200) + 300;
					m_stWeatherObject[i].sY    = (m_pMapData->m_sPivotY*32) + ((rand() % 800) - 600) + 240;
					m_stWeatherObject[i].cStep = -1*(rand() % 10);
		}	}	}
		break;
		
	case WEATHER_LIGHTSNOW:
	case WEATHER_MEDIUMSNOW:
	case WEATHER_HEAVYSNOW:
		switch (m_weather) {
		case WEATHER_LIGHTSNOW: sCnt = MAXWEATHEROBJECTS / 5; break;
		case WEATHER_MEDIUMSNOW:	sCnt = MAXWEATHEROBJECTS / 2; break;
		case WEATHER_HEAVYSNOW:	sCnt = MAXWEATHEROBJECTS;     break;
		}
		for (i = 0; i < sCnt; i++)
		{	m_stWeatherObject[i].cStep++;
			if ((m_stWeatherObject[i].cStep >= 0) && (m_stWeatherObject[i].cStep < 80))
			{	cAdd = (80 - m_stWeatherObject[i].cStep)/10;
				if (cAdd < 0) cAdd = 0;
				m_stWeatherObject[i].sY = m_stWeatherObject[i].sY + cAdd;

				//Snoopy: Snow on lower bar
				if (m_stWeatherObject[i].sY > (426 + m_sViewPointY))
				{	m_stWeatherObject[i].sY = 470 + m_sViewPointY;
					if ((rand() % 10) !=2) m_stWeatherObject[i].cStep--;
					if (m_stWeatherObject[i].sBX == 0) m_stWeatherObject[i].sBX = m_stWeatherObject[i].sX - m_sViewPointX;


				}else m_stWeatherObject[i].sX += 1 - (rand() % 3);
			}else if (m_stWeatherObject[i].cStep >= 80)
			{	if (m_weather == WEATHER_SUNNY)
				{	m_stWeatherObject[i].sX    = 0;
					m_stWeatherObject[i].sY    = 0;
					m_stWeatherObject[i].sBX   = 0;
					m_stWeatherObject[i].cStep = 80;
				}else
				{	m_stWeatherObject[i].sX    = (m_pMapData->m_sPivotX*32) + ((rand() % 940) - 200) + 300;
					m_stWeatherObject[i].sY    = (m_pMapData->m_sPivotY*32) + ((rand() % 800) - 600) + 600;
					m_stWeatherObject[i].cStep = -1*(rand() % 10);
					m_stWeatherObject[i].sBX   = 0;
		}	}	}
		break;
	}
}

void CGame::SetWeatherStatus(Weather type)
{
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);

	if(type != WEATHER_SUNNY)
	{	
		m_weather = type;

		if(type >= WEATHER_LIGHTRAIN && type <= WEATHER_HEAVYRAIN)
		{
            if (m_bSoundStat && m_bSoundFlag)
            {
                //m_pESound[38]->Play(true);
                klang->play2D(m_pESound[38]->getSoundSource(), true);
            }

			if(m_bMusicStat) 
				StartBGM();
		}

		for (int i = 0; i < MAXWEATHEROBJECTS; i++)
		{	m_stWeatherObject[i].sX    = 1;
			m_stWeatherObject[i].sBX    = 1;
			m_stWeatherObject[i].sY    = 1;
			m_stWeatherObject[i].cStep = -1*(rand() % 40);
		}
	} else {
		m_weather = WEATHER_SUNNY;
        if ((m_bSoundStat == TRUE) && (m_bSoundFlag)) m_pESound[38]->stop();
	}
}

void CGame::DrawLine(int x0, int y0, int x1, int y1, int iR, int iG, int iB)
{
    int dx, dy, x_inc, y_inc, error, index, dstR, dstG, dstB;
    int iResultX, iResultY;
    uint16_t * pDst;
    uint32_t * pDwDst;

	if ((x0 == x1) && (y0 == y1)) return;
    driver->draw2DLine(irr::core::vector2d<s32>(x0, y0), irr::core::vector2d<s32>(x1, y1), irr::video::SColor(255, 255, 128, 128));
    return;
    error = 0;
	iResultX = x0;
	iResultY = y0;
	dx = x1-x0;
	dy = y1-y0;
	if(dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx = -dx;
	}
	if(dy>=0)
	{
		y_inc = 1;
	}
	else
	{
		y_inc = -1;
		dy = -dy;
	}
	if(dx>dy)
	{
		for(index = 0; index <= dx; index++)
		{
			error += dy;
			if(error > dx)
			{
				error -= dx;
				iResultY += y_inc;
			}
			iResultX += x_inc;
			//if ((iResultX >= 0) && (iResultX < 640) && (iResultY >= 0) && (iResultY < 480)) {
//			if ((iResultX >= 0) && (iResultX < GetWidth()) && (iResultY >= 0) && (iResultY < GetHeight()))
			{ // 800x600 Resolution xRisenx
                driver->draw2DLine(irr::core::vector2d<s32>(x0, y0), irr::core::vector2d<s32>(x1, y1), irr::video::SColor(255, 255, 128, 255));
//				pDst = (uint16_t *)//DIRECTX m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*//DIRECTX m_DDraw.m_sBackB4Pitch);
// 				switch (//DIRECTX m_DDraw.m_cPixelFormat) {
// 				case 1:
// 					dstR = (int)//DIRECTX m_DDraw.m_lTransRB100[(pDst[0]&0xF800)>>11][iR];
// 					dstG = (int)//DIRECTX m_DDraw.m_lTransG100[(pDst[0]&0x7E0)>>5][iG];
// 					dstB = (int)//DIRECTX m_DDraw.m_lTransRB100[(pDst[0]&0x1F)][iB];
// 					*pDst = (WORD)((dstR<<11) | (dstG<<5) | dstB);
// 					break;
// 
// 				case 2:
// 					dstR = (int)//DIRECTX m_DDraw.m_lTransRB100[(pDst[0]&0x7C00)>>10][iR];
// 					dstG = (int)//DIRECTX m_DDraw.m_lTransG100[(pDst[0]&0x3E0)>>5][iG];
// 					dstB = (int)//DIRECTX m_DDraw.m_lTransRB100[(pDst[0]&0x1F)][iB];
// 					*pDst = (WORD)((dstR<<10) | (dstG<<5) | dstB);
// 					break;
// 				case 4:
// 					pDwDst = (uint32_t *)//DIRECTX m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*//DIRECTX m_DDraw.m_sBackB4Pitch);
// 					*pDwDst = (DWORD)((iR<<16) | (iG<<8) | iB);
// 					break;
// 				}	
			}	
		}
	}
	else
	{
		for(index = 0; index <= dy; index++)
		{
			error += dx;
			if(error > dy)
			{
				error -= dy;
				iResultX += x_inc;
			}
			iResultY += y_inc;
			//if ((iResultX >= 0) && (iResultX < 640) && (iResultY >= 0) && (iResultY < 480)) {
//			if ((iResultX >= 0) && (iResultX < GetWidth()) && (iResultY >= 0) && (iResultY < GetHeight()))
			{ // 800x600 Resolution xRisenx
                driver->draw2DLine(irr::core::vector2d<s32>(x0, y0), irr::core::vector2d<s32>(x1, y1), irr::video::SColor(255, 255, 128, 255));
//				pDst = (uint16_t *)//DIRECTX m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*//DIRECTX m_DDraw.m_sBackB4Pitch);
// 				switch (//DIRECTX m_DDraw.m_cPixelFormat) {
// 				case 1:
// 					dstR = (int)//DIRECTX m_DDraw.m_lTransRB100[(pDst[0]&0xF800)>>11][iR];
// 					dstG = (int)//DIRECTX m_DDraw.m_lTransG100[(pDst[0]&0x7E0)>>5][iG];
// 					dstB = (int)//DIRECTX m_DDraw.m_lTransRB100[(pDst[0]&0x1F)][iB];
// 					*pDst = (WORD)((dstR<<11) | (dstG<<5) | dstB);
// 					break;
// 
// 				case 2:
// 					dstR = (int)//DIRECTX m_DDraw.m_lTransRB100[(pDst[0]&0x7C00)>>10][iR];
// 					dstG = (int)//DIRECTX m_DDraw.m_lTransG100[(pDst[0]&0x3E0)>>5][iG];
// 					dstB = (int)//DIRECTX m_DDraw.m_lTransRB100[(pDst[0]&0x1F)][iB];
// 					*pDst = (WORD)((dstR<<10) | (dstG<<5) | dstB);
// 					break;
// 				case 4:
// 					pDwDst = (uint32_t *)//DIRECTX m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*//DIRECTX m_DDraw.m_sBackB4Pitch);
// 					*pDwDst = (DWORD)((iR<<16) | (iG<<8) | iB);
// 					break;
// 	
// 				}
			}
		}	
	}
}


void CGame::DrawLine2(int x0, int y0, int x1, int y1, int iR, int iG, int iB)
{
	int dx, dy, x_inc, y_inc, error, index, dstR, dstG, dstB;
	int iResultX, iResultY;
	uint16_t * pDst;
	if ((x0 == x1) && (y0 == y1)) return;

	error = 0;
	iResultX = x0;
	iResultY = y0;
	dx = x1-x0;
	dy = y1-y0;
	if(dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx = -dx;
	}
	if(dy>=0)
	{
		y_inc = 1;
	}
	else
	{
		y_inc = -1;
		dy = -dy;
	}
	if(dx>dy)
	{
		for(index = 0; index <= dx; index++)
		{
			error += dy;
			if(error > dx)
			{
				error -= dx;
				iResultY += y_inc;
			}
			iResultX += x_inc;
			//if ((iResultX >= 0) && (iResultX < 640) && (iResultY >= 0) && (iResultY < 480)) {
			if ((iResultX >= 0) && (iResultX < GetWidth()) && (iResultY >= 0) && (iResultY < GetHeight())) { // 800x600 Resolution xRisenx
//				pDst = (uint16_t *)//DIRECTX m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*//DIRECTX m_DDraw.m_sBackB4Pitch);
// 				switch (//DIRECTX m_DDraw.m_cPixelFormat) {
// 				case 1:
// 					dstR = (int)//DIRECTX m_DDraw.m_lTransRB50[(pDst[0]&0xF800)>>11][iR];
// 					dstG = (int)//DIRECTX m_DDraw.m_lTransG50[(pDst[0]&0x7E0)>>5][iG];
// 					dstB = (int)//DIRECTX m_DDraw.m_lTransRB50[(pDst[0]&0x1F)][iB];
// 					*pDst = (WORD)((dstR<<11) | (dstG<<5) | dstB);
// 					break;
// 
// 				case 2:
// 					dstR = (int)//DIRECTX m_DDraw.m_lTransRB50[(pDst[0]&0x7C00)>>10][iR];
// 					dstG = (int)//DIRECTX m_DDraw.m_lTransG50[(pDst[0]&0x3E0)>>5][iG];
// 					dstB = (int)//DIRECTX m_DDraw.m_lTransRB50[(pDst[0]&0x1F)][iB];
// 					*pDst = (WORD)((dstR<<10) | (dstG<<5) | dstB);
// 					break;
// 				}
			}
		}
	}
	else
	{
		for(index = 0; index <= dy; index++)
		{
			error += dx;
			if(error > dy)
			{
				error -= dy;
				iResultX += x_inc;
			}
			iResultY += y_inc;
			//if ((iResultX >= 0) && (iResultX < 640) && (iResultY >= 0) && (iResultY < 480)) {
			if ((iResultX >= 0) && (iResultX < GetWidth()) && (iResultY >= 0) && (iResultY < GetHeight())) { // 800x600 Resolution xRisenx
//				pDst = (uint16_t *)//DIRECTX m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*//DIRECTX m_DDraw.m_sBackB4Pitch);
// 				switch (//DIRECTX m_DDraw.m_cPixelFormat) {
// 				case 1:
// 					dstR = (int)//DIRECTX m_DDraw.m_lTransRB50[(pDst[0]&0xF800)>>11][iR];
// 					dstG = (int)//DIRECTX m_DDraw.m_lTransG50[(pDst[0]&0x7E0)>>5][iG];
// 					dstB = (int)//DIRECTX m_DDraw.m_lTransRB50[(pDst[0]&0x1F)][iB];
// 					*pDst = (WORD)((dstR<<11) | (dstG<<5) | dstB);
// 					break;
// 
// 				case 2:
// 					dstR = (int)//DIRECTX m_DDraw.m_lTransRB50[(pDst[0]&0x7C00)>>10][iR];
// 					dstG = (int)//DIRECTX m_DDraw.m_lTransG50[(pDst[0]&0x3E0)>>5][iG];
// 					dstB = (int)//DIRECTX m_DDraw.m_lTransRB50[(pDst[0]&0x1F)][iB];
// 					*pDst = (WORD)((dstR<<10) | (dstG<<5) | dstB);
// 					break;
// 	
// 				}
			}	
		}
	}
}

void CGame::_DrawThunderEffect(int sX, int sY, int dX, int dY, int rX, int rY, char cType)
{int j, iErr, pX1, pY1, iX1, iY1, tX, tY;
 char cDir;
 uint32_t dwTime;
 uint16_t  wR1, wG1, wB1, wR2, wG2, wB2, wR3, wG3, wB3, wR4, wG4, wB4;
	dwTime = m_dwCurTime;
	sX = pX1 = iX1 = tX = sX;
	sY = pY1 = iY1 = tY = sY;
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,50, 50, 100), &wR1, &wG1, &wB1);
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,30, 30, 100), &wR2, &wG2, &wB2);
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,0, 0, 30), &wR3, &wG3, &wB3);
// 	m_Misc.ColorTransfer(//DIRECTX m_DDraw.m_cPixelFormat,video::SColor(255,50, 50, 200), &wR4, &wG4, &wB4);

	for (j = 0; j < 100; j++)
	{	switch (cType) {
		case 1:
			DrawLine(pX1, pY1, iX1, iY1, 15, 15, 20);
			DrawLine(pX1-1, pY1, iX1-1, iY1, wR1, wG1, wB1);
			DrawLine(pX1+1, pY1, iX1+1, iY1, wR1, wG1, wB1);
			DrawLine(pX1, pY1-1, iX1, iY1-1, wR1, wG1, wB1);
			DrawLine(pX1, pY1+1, iX1, iY1+1, wR1, wG1, wB1);

			DrawLine(pX1-2, pY1, iX1-2, iY1, wR2, wG2, wB2);
			DrawLine(pX1+2, pY1, iX1+2, iY1, wR2, wG2, wB2);
			DrawLine(pX1, pY1-2, iX1, iY1-2, wR2, wG2, wB2);
			DrawLine(pX1, pY1+2, iX1, iY1+2, wR2, wG2, wB2);

			DrawLine(pX1-1, pY1-1, iX1-1, iY1-1, wR3, wG3, wB3);
			DrawLine(pX1+1, pY1-1, iX1+1, iY1-1, wR3, wG3, wB3);
			DrawLine(pX1+1, pY1-1, iX1+1, iY1-1, wR3, wG3, wB3);
			DrawLine(pX1-1, pY1+1, iX1-1, iY1+1, wR3, wG3, wB3);
			break;

		case 2:
			DrawLine2(pX1, pY1, iX1, iY1, wR4, wG4, wB4);
			break;
		}
		iErr = 0;
		m_Misc.GetPoint(sX, sY, dX, dY, &tX, &tY, &iErr, j*10);
		pX1 = iX1;
		pY1 = iY1;
		cDir = m_Misc.cGetNextMoveDir(iX1, iY1, tX, tY);
		switch (cDir) {
		case 1:	rY -= 5; break;
		case 2: rY -= 5; rX += 5; break;
		case 3:	rX += 5; break;
		case 4: rX += 5; rY += 5; break;
		case 5: rY += 5; break;
		case 6: rX -= 5; rY += 5; break;
		case 7: rX -= 5; break;
		case 8: rX -= 5; rY -= 5; break;
		}
		if (rX < -20) rX = -20;
		if (rX >  20) rX =  20;
		if (rY < -20) rY = -20;
		if (rY >  20) rY =  20;
		iX1 = iX1 + rX;
		iY1 = iY1 + rY;
		if ((abs(tX - dX) < 5) && (abs(tY - dY) < 5)) break;
	}
	switch (cType) {
	case 1:
		m_pEffectSpr[6]->PutTransSprite(iX1, iY1, (rand() % 2), dwTime);
		break;
	}
}

int CGame::_iGetAttackType()
{
	uint16_t wWeaponType;
	int i; // Battle Mages xRisenx
	wWeaponType = ((m_sPlayerAppr2 & 0x0FF0) >> 4);
	if (wWeaponType == 0)
	{
		if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) &&
			(m_cSkillMastery[SKILL_HANDATTACK] >= 100))
			return 20;
		else return 1;		// Boxe
	}else if ((wWeaponType >= 1) && (wWeaponType <= 2))
	{
		if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) &&
			(m_cSkillMastery[SKILL_SHORTSWORD] >= 100))
			return 21;
		else return 1;		//Dag, SS
	}else if ((wWeaponType > 2) && (wWeaponType < 20))
	{
		if ((wWeaponType == 7)||(wWeaponType == 18)) // Added Kloness Esterk
		{
			if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) &&
				(m_cSkillMastery[SKILL_FENCING] >= 100))
				return 22;
			else return 1;  // Esterk
		}else if (wWeaponType == 15)
		{
			if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) &&
				(m_cSkillMastery[SKILL_LONGSWORD] >= 100))
				return 30;
			else return 5;  // StormBlade
		}else
		{
			if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) &&
				(m_cSkillMastery[SKILL_LONGSWORD] >= 100))
				return 23;
			else return 1;	// LongSwords
		}
	}else if ((wWeaponType >= 20) && (wWeaponType < 29))
	{
		if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) &&
			(m_cSkillMastery[SKILL_AXE] >= 100))
			return 24;
		else return 1;		// Haches
	}else if ((wWeaponType >= 30) && (wWeaponType < 33))
	{
		if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) &&
			(m_cSkillMastery[SKILL_HAMMER] >= 100))
			return 26;
		else return 1;		// Hammers
	}
	// Battle Mages xRisenx
	else if ((wWeaponType >= 34) && (wWeaponType < 41))
	{	
		for (i = 0; i < MAXITEMS; i++)
		if ((m_pItemList[i] != 0) && (m_bIsItemEquipped[i] == true))	{
			if ((m_pItemList[i]->m_cEquipPos == EQUIPPOS_TWOHAND) && (wWeaponType == 35)) {
				if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[21] >= 100)) return 30;
				else return 5;
			}
		}
		if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[21] >= 100)) return 27;
		else return 1;		// Wands
		// Battle Mages xRisenx
	//else if ((wWeaponType >= 34) && (wWeaponType < 41))
	//{
	//	if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE) &&
	//		(m_cSkillMastery[SKILL_STAFF] >= 100))
	//		return 27;
	//	else return 1;		// Wands
	}else if (wWeaponType >= 41)
	{
		if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) &&
			(m_cSkillMastery[SKILL_ARCHERY] >= 100))
			return 25;
		else return 2;		// Bows
	}else if ((wWeaponType == 29) || (wWeaponType == 33))
	{
		if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) &&
			(m_cSkillMastery[SKILL_LONGSWORD] >= 100))
			return 23;
		else return 1;		// LS
	}
	return 0;
}

int CGame::_iGetWeaponSkillType()
{
	uint16_t wWeaponType = ((m_sPlayerAppr2 & 0x0FF0) >> 4);

	if (wWeaponType == 0)
	{
		return SKILL_HANDATTACK;
	}else if ((wWeaponType >= 1) && (wWeaponType < 3))
	{
		return SKILL_SHORTSWORD;
	}else if ((wWeaponType >= 3) && (wWeaponType < 20))
	{
		if ((wWeaponType == 7)||(wWeaponType == 18)) // Esterk or KlonessEsterk
			 return SKILL_FENCING;
		else return SKILL_LONGSWORD;
	}else if ((wWeaponType >= 20) && (wWeaponType < 29))
	{
		return SKILL_AXE;
	}else if ((wWeaponType >= 30) && (wWeaponType < 33))
	{
		return SKILL_HAMMER;
	}else if ((wWeaponType >= 34) && (wWeaponType < 41))
	{
		return SKILL_STAFF;
	}else if (wWeaponType >= 41)
	{
		return SKILL_ARCHERY;
	}else if ((wWeaponType == 29) || (wWeaponType == 33))
	{
		return SKILL_LONGSWORD;  // LS LightingBlade || BlackShadow
	}
	return SKILL_FISHING;
}

void CGame::NotifyMsg_AdminInfo(char *pData)
{
	char * cp, cStr[256];
	int  * ip, iV1, iV2, iV3, iV4, iV5;

	cp = (char *)(pData + 6);

	ip = (int *)cp;
	iV1 = *ip;
	cp += 4;

	ip = (int *)cp;
	iV2 = *ip;
	cp += 4;

	ip = (int *)cp;
	iV3 = *ip;
	cp += 4;

	ip = (int *)cp;
	iV4 = *ip;
	cp += 4;

	ip = (int *)cp;
	iV5 = *ip;
	cp += 4;

	ZeroMemory(cStr, sizeof(cStr));
	wsprintfA(cStr, "%d %d %d %d %d", iV1, iV2, iV3, iV4, iV5);
	AddEventList(cStr);
}

/*********************************************************************************************************************
**  void CGame::bItemDrop_ExchangeDialog()										**
**  description			:: modifyed for MultiTrade																	**
**********************************************************************************************************************/
void CGame::bItemDrop_ExchangeDialog()
{
	char cItemID;
	if (m_cCommand < 0) return;
	if (m_stDialogBoxExchangeInfo[3].sV1 != -1) return; //Do not accept item's drop if already 4 items.

	short msY = G_pGame->m_stMCursor.sY;
	short msX = G_pGame->m_stMCursor.sX;
	cItemID = (char)m_stMCursor.sSelectedObjectID;

	if (m_stDialogBoxExchangeInfo[0].sV1 == cItemID || m_stDialogBoxExchangeInfo[1].sV1 == cItemID || m_stDialogBoxExchangeInfo[2].sV1 == cItemID)
		return;

	if ( ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_ARROW)) &&
		 (m_pItemList[cItemID]->m_dwCount > 1) )
	{	
		m_dialogBoxes[17].m_X  = msX - 140;
		m_dialogBoxes[17].m_Y  = msY - 70;
		if (m_dialogBoxes[17].m_Y < 0) m_dialogBoxes[17].m_Y = 0;
		m_dialogBoxes[17].sV1 = m_sPlayerX+1;
		m_dialogBoxes[17].sV2 = m_sPlayerY+1;
		m_dialogBoxes[17].sV3 = 1000;
		m_dialogBoxes[17].sV4 = cItemID;
		//m_stDialogBoxInfo[27].sView = cItemID;
		if (m_stDialogBoxExchangeInfo[0].sV1 == -1)			m_stDialogBoxExchangeInfo[0].sItemID = cItemID;
		else if (m_stDialogBoxExchangeInfo[1].sV1 == -1)	m_stDialogBoxExchangeInfo[1].sItemID = cItemID;
		else if (m_stDialogBoxExchangeInfo[2].sV1 == -1)	m_stDialogBoxExchangeInfo[2].sItemID = cItemID;
		else if (m_stDialogBoxExchangeInfo[3].sV1 == -1)	m_stDialogBoxExchangeInfo[3].sItemID = cItemID;
		else return; // Impossible case, tested at function beginning
		ZeroMemory(m_dialogBoxes[17].cStr, sizeof(m_dialogBoxes[17].cStr));
		EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
		return;
	}else // hum? d�j� on affiche? , bon je d�sactive, ca devrait plut�t s'afficher lors du retour du serveur.
	{	/*m_stDialogBoxInfo[27].sV1 = m_pItemList[cItemID]->m_sSprite;
		m_stDialogBoxInfo[27].sV2 = m_pItemList[cItemID]->m_sSpriteFrame;
		m_stDialogBoxInfo[27].sV3 = 1;
		m_stDialogBoxInfo[27].sV4 = m_pItemList[cItemID]->m_cItemColor;
		m_stDialogBoxInfo[27].sView = cItemID;*/
		if (m_stDialogBoxExchangeInfo[0].sV1 == -1)			m_stDialogBoxExchangeInfo[0].sItemID = cItemID;
		else if (m_stDialogBoxExchangeInfo[1].sV1 == -1)	m_stDialogBoxExchangeInfo[1].sItemID = cItemID;
		else if (m_stDialogBoxExchangeInfo[2].sV1 == -1)	m_stDialogBoxExchangeInfo[2].sItemID = cItemID;
		else if (m_stDialogBoxExchangeInfo[3].sV1 == -1)	m_stDialogBoxExchangeInfo[3].sItemID = cItemID;
		else return; // Impossible case, tested at function beginning
		m_bIsItemDisabled[cItemID] = true;
		bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SETEXCHANGEITEM, 0, cItemID, 1, 0, 0);
//	   :bSendCommand(DWORD dwMsgID,        WORD wCommand,             char cDir, int iV1, int iV2, int iV3, char * pString, int iV4)
		return;
	}
}

int CGame::_iGetBankItemCount()
{
 int i, iCnt;

	iCnt = 0;
	for (i = 0; i < MAXBANKITEMS; i++)
		if (m_pBankList[i] != 0) iCnt++;

	return iCnt;
}

bool CGame::_bDecodeBuildItemContents()
{char cFileName[255], cTemp[255];
 HANDLE hFile;
 FILE * pFile;
 uint32_t  dwFileSize;
 char * pBuffer;
 bool   bRet;
 int    i;

	for (i = 0; i < MAXBUILDITEMS; i++)
	if (m_pBuildItemList[i] != 0)
	{	delete m_pBuildItemList[i];
		m_pBuildItemList[i] = 0;
	}

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));

	strcpy(cTemp, "BItemcfg");
	strcat(cFileName, "data\\shops");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cTemp);
	strcat(cFileName, ".txt");

	hFile = CreateFileA(cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	dwFileSize = GetFileSize(hFile, 0);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFileName, "rt");
	if (pFile == 0) return false;
	else
	{	pBuffer = new char[dwFileSize+1];
		ZeroMemory(pBuffer, dwFileSize+1);
		fread(pBuffer, dwFileSize, 1, pFile);
		bRet = __bDecodeBuildItemContents(pBuffer);
		delete[] pBuffer;
	}
	fclose(pFile);
	return bRet;
}

bool CGame::_bCheckBuildItemStatus()
{int iIndex, i, j, iMatch, iCount;
 char cTempName[21];
 int  iItemCount[MAXITEMS];

	for (i = 0; i < MAXBUILDITEMS; i++)
	if (m_pDispBuildItemList[i] != 0)
	{	delete m_pDispBuildItemList[i];
		m_pDispBuildItemList[i] = 0;
	}
	iIndex = 0;
#ifdef TitleClient
	int iBSTitleRank = 0;
	for (i=0; i<MAXMENUITEMS; i++) {
		if (m_pTitles[i] != 0) {
			if (strcmp(m_pTitles[i]->m_cSubject, "Blacksmithing") == 0) {
				iBSTitleRank = m_pTitles[i]->m_iRank;
				break;
			}
		}
	}
#endif
	for (i = 0; i < MAXBUILDITEMS; i++)
	if (m_pBuildItemList[i] != 0)
	{	// Skill-Limit
#ifdef TitleClient
		if (iBSTitleRank >= m_pBuildItemList[i]->m_iSkillLimit) // Titles xRisenx		
#else
		if (m_cSkillMastery[SKILL_MANUFACTURING] >= m_pBuildItemList[i]->m_iSkillLimit)
#endif
		{	iMatch = 0;
			m_pDispBuildItemList[iIndex] = new class CBuildItem;
			memcpy(m_pDispBuildItemList[iIndex]->m_cName, m_pBuildItemList[i]->m_cName, 20);

			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName1, m_pBuildItemList[i]->m_cElementName1, 20);
			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName2, m_pBuildItemList[i]->m_cElementName2, 20);
			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName3, m_pBuildItemList[i]->m_cElementName3, 20);
			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName4, m_pBuildItemList[i]->m_cElementName4, 20);
			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName5, m_pBuildItemList[i]->m_cElementName5, 20);
			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName6, m_pBuildItemList[i]->m_cElementName6, 20);

			m_pDispBuildItemList[iIndex]->m_iElementCount[1] = m_pBuildItemList[i]->m_iElementCount[1];
			m_pDispBuildItemList[iIndex]->m_iElementCount[2] = m_pBuildItemList[i]->m_iElementCount[2];
			m_pDispBuildItemList[iIndex]->m_iElementCount[3] = m_pBuildItemList[i]->m_iElementCount[3];
			m_pDispBuildItemList[iIndex]->m_iElementCount[4] = m_pBuildItemList[i]->m_iElementCount[4];
			m_pDispBuildItemList[iIndex]->m_iElementCount[5] = m_pBuildItemList[i]->m_iElementCount[5];
			m_pDispBuildItemList[iIndex]->m_iElementCount[6] = m_pBuildItemList[i]->m_iElementCount[6];

			m_pDispBuildItemList[iIndex]->m_iSprH       = m_pBuildItemList[i]->m_iSprH;
			m_pDispBuildItemList[iIndex]->m_iSprFrame   = m_pBuildItemList[i]->m_iSprFrame;
			m_pDispBuildItemList[iIndex]->m_iMaxSkill   = m_pBuildItemList[i]->m_iMaxSkill;
			m_pDispBuildItemList[iIndex]->m_iSkillLimit = m_pBuildItemList[i]->m_iSkillLimit;

			// ItemCount
			for (j = 0; j < MAXITEMS; j++)
			if (m_pItemList[j] != 0)
				 iItemCount[j] = m_pItemList[j]->m_dwCount;
			else iItemCount[j] = 0;

			// Element1
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName1, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[1];
			if (iCount == 0) iMatch++;
			else
			{	for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != 0) {
					if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
						(iItemCount[j] > 0))
					{	iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[1] = true;
						iItemCount[j] -= iCount;
						goto CBIS_STEP2;
			}	}	}

CBIS_STEP2:;
			// Element2
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName2, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[2];
			if (iCount == 0) iMatch++;
			else
			{	for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != 0)
				{	if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
						(iItemCount[j] > 0))
					{	iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[2] = true;
						iItemCount[j] -= iCount;
						goto CBIS_STEP3;
			}	}	}

CBIS_STEP3:;
			// Element3
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName3, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[3];
			if (iCount == 0) iMatch++;
			else
			{	for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != 0)
				{	if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
						(iItemCount[j] > 0))
					{	iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[3] = true;
						iItemCount[j] -= iCount;
						goto CBIS_STEP4;
			}	}	}

CBIS_STEP4:;

			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName4, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[4];
			if (iCount == 0) iMatch++;
			else
			{	for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != 0)
				{	if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
						(iItemCount[j] > 0))
					{	iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[4] = true;
						iItemCount[j] -= iCount;
						goto CBIS_STEP5;
			}	}	}

CBIS_STEP5:;

			// Element5
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName5, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[5];
			if (iCount == 0) iMatch++;
			else
			{	for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != 0)
				{	if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
						(iItemCount[j] > 0))
					{	iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[5] = true;
						iItemCount[j] -= iCount;
						goto CBIS_STEP6;
			}	}	}

CBIS_STEP6:;

			// Element6
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName6, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[6];
			if (iCount == 0) iMatch++;
			else
			{	for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != 0)
				{	if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
						(iItemCount[j] > 0))
					{	iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[6] = true;
						iItemCount[j] -= iCount;
						goto CBIS_STEP7;
			}	}	}

CBIS_STEP7:;

			if (iMatch == 6) m_pDispBuildItemList[iIndex]->m_bBuildEnabled = true;
			iIndex++;
	}	}
	return true;
}

bool CGame::_ItemDropHistory(char * ItemName)
{
	/*bool bFlag = FALSE;
	if (m_iItemDropCnt == 0 )
	{	strcpy(m_cItemDrop[m_iItemDropCnt], ItemName);
		m_iItemDropCnt++;
		return TRUE;
	}
	if ( (1 <= m_iItemDropCnt) && (20 >= m_iItemDropCnt) )
	{	for (int i = 0; i < m_iItemDropCnt; i++)
		{	if (strcmp(m_cItemDrop[i], ItemName) == 0)
			{	bFlag = TRUE;
	            break;
		}	}
		if (bFlag)
		{	if (m_bItemDrop)
				return FALSE;
			else
				return TRUE;
		}

		if( 20 < m_iItemDropCnt )
		{	for (int i = 0; i < m_iItemDropCnt ; i++)
			strcpy(m_cItemDrop[i-1], ItemName);
			strcpy(m_cItemDrop[20], ItemName);
			m_iItemDropCnt = 21;
		}else
		{	strcpy(m_cItemDrop[m_iItemDropCnt], ItemName);
			m_iItemDropCnt++;
	}	}
	return TRUE;*/
	//BESK item drop rewritten
	bool bFound = false;
	for (int i=0; i<m_iItemDropCnt; i++) {
		if (strcmp(m_cItemDrop[i], ItemName) == 0) {
			bFound = true;
			break;
		}
	}

	if (bFound == true) {//the item was already listed
		if (m_bItemDrop == true)//user doesnt want to be bothered for item names he already dropped
			return false;//dont show the user the confirm dlg
		else
			return true;
	} else {
		if (m_iItemDropCnt < NUM_ITEMDROPSLOTS) {
			strcpy(m_cItemDrop[m_iItemDropCnt], ItemName);
			m_iItemDropCnt++;
		}
		return true;
	}
}


bool CGame::__bDecodeBuildItemContents(char *pBuffer)
{char * pContents, * token;
 char seps[] = "= ,\t\n";
 char cReadModeA = 0;
 char cReadModeB = 0;
 int  iIndex = 0;
 class CStrTok * pStrTok;
	pContents = pBuffer;
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();

	while( token != 0 )
	{	if (cReadModeA != 0)
		{	switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					ZeroMemory(m_pBuildItemList[iIndex]->m_cName, sizeof(m_pBuildItemList[iIndex]->m_cName));
					memcpy(m_pBuildItemList[iIndex]->m_cName, token, strlen(token));
					cReadModeB = 2;
					break;
				case 2:
					m_pBuildItemList[iIndex]->m_iSkillLimit = atoi(token);
					cReadModeB = 3;
					break;
				case 3: // m_cElementName1
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName1, sizeof(m_pBuildItemList[iIndex]->m_cElementName1));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName1, token, strlen(token));
					cReadModeB = 4;
					break;
				case 4: // m_iElementCount1
					m_pBuildItemList[iIndex]->m_iElementCount[1] = atoi(token);
					cReadModeB = 5;
					break;
				case 5: // m_cElementName2
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName2, sizeof(m_pBuildItemList[iIndex]->m_cElementName2));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName2, token, strlen(token));
					cReadModeB = 6;
					break;
				case 6: // m_iElementCount2
					m_pBuildItemList[iIndex]->m_iElementCount[2] = atoi(token);
					cReadModeB = 7;
					break;
				case 7: // m_cElementName3
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName3, sizeof(m_pBuildItemList[iIndex]->m_cElementName3));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName3, token, strlen(token));
					cReadModeB = 8;
					break;
				case 8: // m_iElementCount3
					m_pBuildItemList[iIndex]->m_iElementCount[3] = atoi(token);
					cReadModeB = 9;
					break;
				case 9: // m_cElementName4
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName4, sizeof(m_pBuildItemList[iIndex]->m_cElementName4));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName4, token, strlen(token));
					cReadModeB = 10;
					break;
				case 10: // m_iElementCount4
					m_pBuildItemList[iIndex]->m_iElementCount[4] = atoi(token);
					cReadModeB = 11;
					break;
				case 11: // m_cElementName5
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName5, sizeof(m_pBuildItemList[iIndex]->m_cElementName5));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName5, token, strlen(token));
					cReadModeB = 12;
					break;
				case 12: // m_iElementCount5
					m_pBuildItemList[iIndex]->m_iElementCount[5] = atoi(token);
					cReadModeB = 13;
					break;
				case 13: // m_cElementName6
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName6, sizeof(m_pBuildItemList[iIndex]->m_cElementName6));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName6, token, strlen(token));
					cReadModeB = 14;
					break;
				case 14: // m_iElementCount6
					m_pBuildItemList[iIndex]->m_iElementCount[6] = atoi(token);
					cReadModeB = 15;
					break;

				case 15:
					m_pBuildItemList[iIndex]->m_iSprH = atoi(token);
					cReadModeB = 16;
					break;

				case 16:
					m_pBuildItemList[iIndex]->m_iSprFrame = atoi(token);
					cReadModeB = 17;
					break;

				case 17:
					m_pBuildItemList[iIndex]->m_iMaxSkill = atoi(token);

					cReadModeA = 0;
					cReadModeB = 0;
					iIndex++;
					break;
				}
				break;

			default:
				break;
			}
		}else
		{	if (memcmp(token, "BuildItem", 9) == 0)
			{	cReadModeA = 1;
				cReadModeB = 1;
				m_pBuildItemList[iIndex] = new class CBuildItem;
		}	}
		token = pStrTok->pGet();
	}
	delete pStrTok;
	if ((cReadModeA != 0) || (cReadModeB != 0)) return false;
	return true;
}


bool CGame::_bCheckCurrentBuildItemStatus()
{
 int i, iCount2, iMatch, iIndex, iItemIndex[7];
 int iCount;
 int iItemCount[7];
 char cTempName[21];
 bool bItemFlag[7];

	iIndex = m_dialogBoxes[26].cStr[0];

	if (m_pBuildItemList[iIndex] == 0) return false;

	iItemIndex[1] = m_dialogBoxes[26].sV1;
	iItemIndex[2] = m_dialogBoxes[26].sV2;
	iItemIndex[3] = m_dialogBoxes[26].sV3;
	iItemIndex[4] = m_dialogBoxes[26].sV4;
	iItemIndex[5] = m_dialogBoxes[26].sV5;
	iItemIndex[6] = m_dialogBoxes[26].sV6;

	for (i = 1; i <= 6; i++)
	if (iItemIndex[i] != -1)
		 iItemCount[i] = m_pItemList[iItemIndex[i]]->m_dwCount;
	else iItemCount[i] = 0;
	iMatch = 0;
	for (i = 1; i <= 6; i++) bItemFlag[i] = false;

	// Element1
	ZeroMemory(cTempName, sizeof(cTempName));
	memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName1, 20);
	iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[1];
	if (iCount == 0) iMatch++;
	else
	{	for (i = 1; i <= 6; i++)
		{	if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
				(m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
				(iItemCount[i] > 0) && (bItemFlag[i] == false))
			{	iMatch++;
				iItemCount[i] -= iCount;
				bItemFlag[i] = true;
				goto CCBIS_STEP2;
	}	}	}

CCBIS_STEP2:;

	// Element2
	ZeroMemory(cTempName, sizeof(cTempName));
	memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName2, 20);
	iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[2];
	if (iCount == 0) iMatch++;
	else
	{	for (i = 1; i <= 6; i++)
		{	if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
				(m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
				(iItemCount[i] > 0) && (bItemFlag[i] == false))
			{	iMatch++;
				iItemCount[i] -= iCount;
				bItemFlag[i] = true;
				goto CCBIS_STEP3;
	}	}	}

CCBIS_STEP3:;


	// Element3
	ZeroMemory(cTempName, sizeof(cTempName));
	memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName3, 20);
	iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[3];
	if (iCount == 0) iMatch++;
	else
	{	for (i = 1; i <= 6; i++)
		{	if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
				(m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
				(iItemCount[i] > 0) && (bItemFlag[i] == false))
			{	iMatch++;
				iItemCount[i] -= iCount;
				bItemFlag[i] = true;
				goto CCBIS_STEP4;
	}	}	}

CCBIS_STEP4:;

	// Element4
	ZeroMemory(cTempName, sizeof(cTempName));
	memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName4, 20);
	iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[4];
	if (iCount == 0) iMatch++;
	else
	{	for (i = 1; i <= 6; i++)
		{	if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
				(m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
				(iItemCount[i] > 0) && (bItemFlag[i] == false))
			{	iMatch++;
				iItemCount[i] -= iCount;
				bItemFlag[i] = true;
				goto CCBIS_STEP5;
	}	}	}

CCBIS_STEP5:;

	// Element5
	ZeroMemory(cTempName, sizeof(cTempName));
	memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName5, 20);
	iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[5];
	if (iCount == 0) iMatch++;
	else
	{	for (i = 1; i <= 6; i++)
		{	if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
				(m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
				(iItemCount[i] > 0) && (bItemFlag[i] == false))
			{	iMatch++;
				iItemCount[i] -= iCount;
				bItemFlag[i] = true;
				goto CCBIS_STEP6;
	}	}	}

CCBIS_STEP6:;

	// Element6
	ZeroMemory(cTempName, sizeof(cTempName));
	memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName6, 20);
	iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[6];
	if (iCount == 0) iMatch++;
	else
	{	for (i = 1; i <= 6; i++)
		{	if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
				(m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
				(iItemCount[i] > 0) && (bItemFlag[i] == false))
			{	iMatch++;
				iItemCount[i] -= iCount;
				bItemFlag[i] = true;
				goto CCBIS_STEP7;
	}	}	}

CCBIS_STEP7:;

	iCount = 0;
	for (i = 1; i <= 6; i++)
	if (m_pDispBuildItemList[iIndex]->m_iElementCount[i] != 0) iCount++;
	iCount2 = 0;
	for (i = 1; i <= 6; i++)
	if (iItemIndex[i] != -1) iCount2++;
	if ((iMatch == 6) && (iCount == iCount2)) return true;
	return false;
}

void CGame::NoticementHandler(char * pData)
{
 char * cp;
 FILE * pFile;
 uint16_t * wp;
	wp = (uint16_t *)(pData + INDEX2_MSGTYPE);
	switch (*wp) {
	case MSGTYPE_CONFIRM:
		break;
	case MSGTYPE_REJECT:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		pFile = fopen("data\\shops\\contents1000.txt", "wt");
		if (pFile == 0) return;
		fwrite(cp, strlen(cp), 1, pFile);
		fclose(pFile);
		m_dialogBoxes[18].m_X  =  20;
		m_dialogBoxes[18].m_Y  =  65;
		EnableDialogBox(18, 1000, 0, 0);
		break;
	}
	AddEventList(MSG_NOTIFY_HELP, 10);
	if (m_iLevel < 42) EnableDialogBox(35, 0, 0, 0);

}

int CGame::_iGetFOE(int iStatus)
{
	if(m_adminLevel > 1) return 1;	// GMHelper

	Side foeSide = (Side)(iStatus >> 28);

	if( m_iPKCount != 0 ) return -1;
	if( iStatus & STATUS_PK ) return -2;
	if( m_iIlusionOwnerH != 0) return -1;

	if(foeSide == HOSTILE)
		return -1;
	else if(foeSide == NEUTRAL || m_side == NEUTRAL)
		return 0;
	else if(foeSide == m_side)
		return 1;

	return -1;
}

void CGame::_SetIlusionEffect(int iOwnerH)
{
 char cDir;

	m_iIlusionOwnerH = iOwnerH;

	ZeroMemory(m_cName_IE, sizeof(m_cName_IE));
	m_pMapData->GetOwnerStatusByObjectID(iOwnerH, &m_cIlusionOwnerType, &cDir, &m_sAppr1_IE, &m_sAppr2_IE, &m_sAppr3_IE, &m_sAppr4_IE, &m_sHeadApprValue_IE, &m_sBodyApprValue_IE, &m_sArmApprValue_IE, &m_sLegApprValue_IE, &m_iStatus_IE, &m_iApprColor_IE, m_cName_IE); // Re-Coding Sprite xRisenx

	if (!m_cIlusionOwnerType){
		m_cIlusionOwnerType = 1;
	}
	if (m_iStatus_IE & STATUS_INVISIBILITY){
		m_iStatus_IE = m_iStatus_IE & (STATUS_ALL - STATUS_INVISIBILITY);
	}
}

void CGame::ResponsePanningHandler(char *pData)
{
 char * cp, cDir;
 short * sp, sX, sY;

	cp = (char *)(pData + INDEX2_MSGTYPE +2);

	sp = (short *)cp;
	sX = *sp;
	cp += 2;

	sp = (short *)cp;
	sY = *sp;
	cp += 2;

	cDir = *cp;
	cp++;

	switch (cDir) {
	case 1: m_sViewDstY -= 32; m_sPlayerY--; break;
	case 2: m_sViewDstY -= 32; m_sPlayerY--; m_sViewDstX += 32; m_sPlayerX++; break;
	case 3: m_sViewDstX += 32; m_sPlayerX++; break;
	case 4: m_sViewDstY += 32; m_sPlayerY++; m_sViewDstX += 32; m_sPlayerX++; break;
	case 5: m_sViewDstY += 32; m_sPlayerY++;break;
	case 6: m_sViewDstY += 32; m_sPlayerY++; m_sViewDstX -= 32; m_sPlayerX--; break;
	case 7: m_sViewDstX -= 32; m_sPlayerX--; break;
	case 8: m_sViewDstY -= 32; m_sPlayerY--; m_sViewDstX -= 32; m_sPlayerX--; break;
	}

	m_pMapData->ShiftMapData(cDir);
	_ReadMapData(sX, sY, cp);

	m_bIsRedrawPDBGS = true;

	m_bIsObserverCommanded = false;
}

bool CGame::bReadItemNameConfigFile()
{
 FILE * pFile;
 HANDLE hFile;
 uint32_t  dwFileSize;
 char * cp, * token, cReadModeA, cReadModeB;
 char seps[] = "=\n";
 int iIndex;

	cReadModeA = 0;
	cReadModeB = 0;
	iIndex = 0;

	hFile = CreateFileA("data\\shops\\ItemName.cfg", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	dwFileSize = GetFileSize(hFile, 0);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen("data\\shops\\ItemName.cfg", "rt");
	if (pFile == 0) return false;
	else {
		cp = new char[dwFileSize+2];
		ZeroMemory(cp, dwFileSize+2);
		fread(cp, dwFileSize, 1, pFile);

		token = strtok( cp, seps );
		while( token != 0 )   {

			if (cReadModeA != 0) {
				switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
					case 1:
						m_pItemNameList[iIndex] = new class CItemName;
						strcpy(m_pItemNameList[iIndex]->m_cOriginName, token);
						cReadModeB = 2;
						break;

					case 2:
						strcpy(m_pItemNameList[iIndex]->m_cName, token);
						cReadModeA = 0;
						cReadModeB = 0;
						iIndex++;
						break;
					}
				}
			}
			else {
				if (memcmp(token, "Item", 4) == 0) {
					cReadModeA = 1;
					cReadModeB = 1;
				}
			}
			token = strtok( 0, seps );
		}
		delete[] cp;
	}
	if (pFile != 0) fclose(pFile);

	return true;
}

void CGame::DrawDialogBox_Map()
{
 short sX, sY;
 uint32_t dwTime = m_dwCurTime;
 double dV1, dV2, dV3;
 int    tX, tY, szX, szY, dX, dY;

	sX = m_dialogBoxes[22].m_X;
	sY = m_dialogBoxes[22].m_Y;

	szX = 0;
	szY = 0;

	switch (m_dialogBoxes[22].sV1) {
	case 1:
		switch (m_dialogBoxes[22].sV2) {
		case 0: // aresden
			if (m_bDialogTrans)
				 m_pSprite[SPRID_INTERFACE_NEWMAPS1]->PutTransSprite2(sX, sY, 0, dwTime);
			else m_pSprite[SPRID_INTERFACE_NEWMAPS1]->PutSpriteFast(sX, sY, 0, dwTime);
			dX = 19;
			dY = 20;
			szX = 260;
			szY = 260;
			break;

		case 1: // elvine
			if (m_bDialogTrans)
				 m_pSprite[SPRID_INTERFACE_NEWMAPS1]->PutTransSprite2(sX, sY, 1, dwTime);
			else m_pSprite[SPRID_INTERFACE_NEWMAPS1]->PutSpriteFast(sX, sY, 1, dwTime);
			dX = 20;
			dY = 18;
			szX = 260;
			szY = 260;
			break;

		case 2: // middleland
			if (m_bDialogTrans)
				 m_pSprite[SPRID_INTERFACE_NEWMAPS2]->PutTransSprite2(sX, sY, 0, dwTime);
			else m_pSprite[SPRID_INTERFACE_NEWMAPS2]->PutSpriteFast(sX, sY, 0, dwTime);
			dX = 11;
			dY = 31;
			szX = 280;
			szY = 253;
			break;

		case 3: // default
			if (m_bDialogTrans)
				 m_pSprite[SPRID_INTERFACE_NEWMAPS2]->PutTransSprite2(sX, sY, 1, dwTime);
			else m_pSprite[SPRID_INTERFACE_NEWMAPS2]->PutSpriteFast(sX, sY, 1, dwTime);
			dX = 52;
			dY = 42;
			szX = 200;
			szY = 200;
			break;

		case 4:
			if (m_bDialogTrans)
				 m_pSprite[SPRID_INTERFACE_NEWMAPS3]->PutTransSprite2(sX, sY, 0, dwTime);
			else m_pSprite[SPRID_INTERFACE_NEWMAPS3]->PutSpriteFast(sX, sY, 0, dwTime);
			dX = 40;
			dY = 40;
			szX = 220;
			szY = 220;
			break;

		case 5:
			if (m_bDialogTrans)
				 m_pSprite[SPRID_INTERFACE_NEWMAPS3]->PutTransSprite2(sX, sY, 1, dwTime);
			else m_pSprite[SPRID_INTERFACE_NEWMAPS3]->PutSpriteFast(sX, sY, 1, dwTime);
			dX = 40;
			dY = 40;
			szX = 220;
			szY = 220;
			break;

		case 6: // aresden
			if (m_bDialogTrans)
				 m_pSprite[SPRID_INTERFACE_NEWMAPS4]->PutTransSprite2(sX, sY, 0, dwTime);
			else m_pSprite[SPRID_INTERFACE_NEWMAPS4]->PutSpriteFast(sX, sY, 0, dwTime);
			dX = 40;
			dY = 40;
			szX = 220;
			szY = 220;
			break;

		case 7: // elvine
			if (m_bDialogTrans)
				 m_pSprite[SPRID_INTERFACE_NEWMAPS4]->PutTransSprite2(sX, sY, 1, dwTime);
			else m_pSprite[SPRID_INTERFACE_NEWMAPS4]->PutSpriteFast(sX, sY, 1, dwTime);
			dX = 40;
			dY = 40;
			szX = 220;
			szY = 220;
			break;
		case 8: // aresden
			if (m_bDialogTrans)
				 m_pSprite[SPRID_INTERFACE_NEWMAPS5]->PutTransSprite2(sX, sY, 0, dwTime);
			else m_pSprite[SPRID_INTERFACE_NEWMAPS5]->PutSpriteFast(sX, sY, 0, dwTime);
			dX = 40;
			dY = 32;
			szX = 220;
			szY = 220;
			break;

		case 9: // elvine
			if (m_bDialogTrans)
				 m_pSprite[SPRID_INTERFACE_NEWMAPS5]->PutTransSprite2(sX, sY, 1, dwTime);
			else m_pSprite[SPRID_INTERFACE_NEWMAPS5]->PutSpriteFast(sX, sY, 1, dwTime);
			dX = 40;
			dY = 38;
			szX = 220;
			szY = 220;
			break;

		}

		dV1 = (double)m_pMapData->m_sMapSizeX;
		dV2 = (double)m_sPlayerX;
		dV3 = (dV2*(double)szX)/dV1;
		tX  = (int)dV3 +dX;

		dV1 = (double)m_pMapData->m_sMapSizeY;
		if( dV1 == 752 ) dV1 = 680;
		dV2 = (double)m_sPlayerY;
		dV3 = (dV2*(double)szY)/dV1;
		tY  = (int)dV3 +dY;


		//m_pSprite[SPRID_INTERFACE_MAPS1]->PutSpriteFast(sX +tX, sY +tY, 4, dwTime);
		DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, sX +tX, sY +tY, 43);
		wsprintfA(G_cTxt, "%d,%d", m_sPlayerX, m_sPlayerY);
		PutString_SprFont3(sX + 10 +tX -5, sY + 10 + tY -6, G_cTxt, m_wR[13]*4, m_wG[13]*4, m_wB[13]*4, false, 2);
		break;
   	}
}
/*********************************************************************************************************************
**  void CGame::NotifyMsg_SetExchangeItem(char *pData)		(snoopy)												**
**  description			:: Recieve a msg from gserver and sets the item												**
**********************************************************************************************************************/
void CGame::NotifyMsg_SetExchangeItem(char *pData)
{short * sp, sDir, sSprite, sSpriteFrame, sCurLife, sMaxLife, sPerformance;
 int * ip, iAmount, i;
 char * cp, cColor, cItemName[24], cCharName[12];
 uint32_t * dwp, dwAttribute;
	ZeroMemory(cItemName, sizeof(cItemName));
	ZeroMemory(cCharName, sizeof(cCharName));

	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
	sp = (short *)cp;
	sDir = *sp;
	cp += 2;
	sp = (short *)cp;
	sSprite = *sp;
	cp += 2;
	sp = (short *)cp;
	sSpriteFrame = *sp;
	cp += 2;
	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;
	cColor = *cp;
	cp++;
	sp = (short *)cp;
	sCurLife = *sp;
	cp += 2;
	sp = (short *)cp;
	sMaxLife = *sp;
	cp += 2;
	sp = (short *)cp;
	sPerformance = *sp;
	cp += 2;
	memcpy(cItemName, cp, 20);
	cp += 20;
	memcpy(cCharName, cp, 10);
	cp += 10;
	dwp = (uint32_t *)cp;
	dwAttribute = *dwp;
	cp += 4;

	uint8_t sockets[MAXITEMSOCKETS];
	for(int j = 0; j < MAXITEMSOCKETS; j++)
		Pop(cp, sockets[j]);

	if (sDir >= 1000)  // Set the item I want to exchange
	{	i = 0;
		while (m_stDialogBoxExchangeInfo[i].sV1 !=-1)
		{	i++;
			if (i>=4) return; // Error situation
		}
	}else // Set the item he proposes me.
	{	i = 4;
		while (m_stDialogBoxExchangeInfo[i].sV1 !=-1)
		{	i++;
			if (i>=8) return; // Error situation
	}	}
	m_stDialogBoxExchangeInfo[i].sV1 = sSprite;
	m_stDialogBoxExchangeInfo[i].sV2 = sSpriteFrame;
	m_stDialogBoxExchangeInfo[i].sV3 = iAmount;
	m_stDialogBoxExchangeInfo[i].sV4 = cColor;
	m_stDialogBoxExchangeInfo[i].sV5 = (int)sCurLife;
	m_stDialogBoxExchangeInfo[i].sV6 = (int)sMaxLife;
	m_stDialogBoxExchangeInfo[i].sV7 = (int)sPerformance;
	memcpy(m_stDialogBoxExchangeInfo[i].cStr1, cItemName, 20);
	memcpy(m_stDialogBoxExchangeInfo[i].cStr2, cCharName, 10);
	m_stDialogBoxExchangeInfo[i].dwV1 = dwAttribute;
	//if (i<4) m_stDialogBoxExchangeInfo[i].sItemID = sDir -1000;

	for(int j = 0; j < MAXITEMSOCKETS; j++)
		m_stDialogBoxExchangeInfo[i].sockets[j] = sockets[j];
}

void CGame::NotifyMsg_DismissGuildApprove(char * pData)
{
	char * cp, cName[24];
	ZeroMemory(cName, sizeof(cName));

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	Pop(cp, cName, 20);

	ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
	m_guild.Reset();
	DisableDialogBox(57);
	m_iGuildRank = GUILDRANK_NONE;

	EnableDialogBox(8, 0, 0, 0);
	_PutGuildOperationList(cName, 5);
}

void CGame::NotifyMsg_DismissGuildReject(char * pData)
{
	char * cp, cName[21];
	ZeroMemory(cName, sizeof(cName));
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	Pop(cp, cName, 20);

	EnableDialogBox(8, 0, 0, 0);
	_PutGuildOperationList(cName, 6);
}

void CGame::NotifyMsg_DownSkillIndexSet(char *pData)
{
 uint16_t * wp;
 short sSkillIndex;
 char * cp;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	sSkillIndex = (short)*wp;
	cp += 2;
	m_iDownSkillIndex = sSkillIndex;
	m_dialogBoxes[15].bFlag = false;
}

void CGame::NotifyMsg_FishChance(char * pData)
{
 int iFishChance;
 char * cp;
 uint16_t * wp;
	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	iFishChance = (int)*wp;
	cp += 2;
	m_dialogBoxes[24].sV1 = iFishChance;
}

void CGame::NotifyMsg_GuildDisbanded(char * pData)
{
	char * cp, cName[24], cLocation[12];
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	m_Misc.ReplaceString(cName, '_', ' ');


	EnableDialogBox(8, 0, 0, 0);
	_PutGuildOperationList(cName, 7);

	ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
	m_guild.Reset();
	DisableDialogBox(57);
	m_iGuildRank = GUILDRANK_NONE;
}

void CGame::NotifyMsg_WeatherChange(char * pData)
{
 char * cp;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	SetWeatherStatus((Weather)*cp);
}

void CGame::NotifyMsg_TimeChange(char * pData)
{ 
	char * cp;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	G_cSpriteAlphaDegree = *cp;

	switch (G_cSpriteAlphaDegree) 
	{
	case 1:	PlaySound('E', 32, 0); break;
	case 2: PlaySound('E', 31, 0); break;
	case 3: // Special night with christmas bulbs
		if (m_weather > WEATHER_LIGHTSNOW && m_weather <= WEATHER_HEAVYSNOW) 
			m_bIsXmas = true;
		else
			m_bIsXmas = false;

		PlaySound('E', 31, 0);
		G_cSpriteAlphaDegree = 2;
		break;
	}

	m_cGameModeCount = 1;
	m_bIsRedrawPDBGS = true;
}

void CGame::NotifyMsg_RepairItemPrice(char * pData)
{char * cp, cName[21];
 uint32_t * dwp, wV1, wV2, wV3, wV4;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	dwp = (uint32_t *)cp;
  	wV1 = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
  	wV2 = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
  	wV3 = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
  	wV4 = *dwp;
	cp += 4;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;
	EnableDialogBox(23, 2, wV1, wV2);
	m_dialogBoxes[23].sV3 = wV3;
}

void CGame::NotifyMsg_SellItemPrice(char * pData)
{char * cp, cName[21];
 uint32_t * dwp, wV1, wV2, wV3, wV4;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	dwp = (uint32_t *)cp;
  	wV1 = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
  	wV2 = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
  	wV3 = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
  	wV4 = *dwp;
	cp += 4;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;
	EnableDialogBox(23, 1, wV1, wV2);
	m_dialogBoxes[23].sV3 = wV3;
	m_dialogBoxes[23].sV4 = wV4;
}

void CGame::NotifyMsg_QueryDismissGuildPermission(char * pData)
{char * cp, cName[12];
	ZeroMemory(cName, sizeof(cName));
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	memcpy(cName, cp, 10);
	cp += 10;
	EnableDialogBox(8, 0, 0, 0);
	_PutGuildOperationList(cName, 2);
}


void CGame::NotifyMsg_QueryJoinGuildPermission(char * pData)
{char * cp, cName[12];
	ZeroMemory(cName, sizeof(cName));
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	memcpy(cName, cp, 10);
	cp += 10;
	EnableDialogBox(8, 0, 0, 0);
	_PutGuildOperationList(cName, 1);
}


void CGame::NotifyMsg_QuestContents(char *pData)
{short * sp;
 char  * cp;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	sp = (short *)cp;
	m_stQuest.sWho = *sp;
	cp += 2;
	sp = (short *)cp;
	m_stQuest.sQuestType = *sp;
	cp += 2;
	sp = (short *)cp;
	m_stQuest.sContribution = *sp;
	cp += 2;
	sp = (short *)cp;
	m_stQuest.sTargetType = *sp;
	cp += 2;
	sp = (short *)cp;
	m_stQuest.sTargetCount = *sp;
	cp += 2;
	sp = (short *)cp;
	m_stQuest.sX = *sp;
	cp += 2;
	sp = (short *)cp;
	m_stQuest.sY = *sp;
	cp += 2;
	sp = (short *)cp;
	m_stQuest.sRange = *sp;
	cp += 2;
	sp = (short *)cp;
	m_stQuest.bIsQuestCompleted = (bool)*sp;
	cp += 2;
	ZeroMemory(m_stQuest.cTargetName, sizeof(m_stQuest.cTargetName));
	memcpy(m_stQuest.cTargetName, cp, 20);
	cp += 20;

	//AddEventList(NOTIFYMSG_QUEST_STARTED, 10);
}

void CGame::NotifyMsg_PlayerProfile(char * pData)
{char * cp;
 char cTemp[500];
 int i;
	ZeroMemory(cTemp, sizeof(cTemp));
	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
	strcpy(cTemp, cp);
	for (i = 0; i < 500; i++)
	if (cTemp[i] == '_') cTemp[i] = ' ';
	AddEventList(cTemp, 10);
}

void CGame::NotifyMsg_NoticeMsg(char * pData)
{char * cp, cMsg[1000];
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	strcpy(cMsg, cp);
	AddEventList(cMsg, 10);
}
/*********************************************************************************************************************
**  void CGame::NotifyMsg_OpenExchageWindow(char *pData)		(snoopy)											**
**  description			:: Recieve a msg from gserver and sets the item	and opens trade windows						**
**********************************************************************************************************************/
void CGame::NotifyMsg_OpenExchageWindow(char *pData)
{short * sp, sDir, sSprite, sSpriteFrame, sCurLife, sMaxLife, sPerformance;
 int * ip, iAmount;
 char * cp, cColor, cItemName[24], cCharName[12];
 uint32_t * dwp, dwAttribute;
	ZeroMemory(cItemName, sizeof(cItemName));
	ZeroMemory(cCharName, sizeof(cCharName));

	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
	sp = (short *)cp;
	sDir = *sp;
	cp += 2;
	sp = (short *)cp;
	sSprite = *sp;
	cp += 2;
	sp = (short *)cp;
	sSpriteFrame = *sp;
	cp += 2;
	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;
	cColor = *cp;
	cp++;
	sp = (short *)cp;
	sCurLife = *sp;
	cp += 2;
	sp = (short *)cp;
	sMaxLife = *sp;
	cp += 2;
	sp = (short *)cp;
	sPerformance = *sp;
	cp += 2;
	memcpy(cItemName, cp, 20);
	cp += 20;
	memcpy(cCharName, cp, 10);
	cp += 10;
	dwp = (uint32_t *)cp;
	dwAttribute = *dwp;
	cp += 4;

	uint8_t sockets[MAXITEMSOCKETS];
	for(int j = 0; j < MAXITEMSOCKETS; j++)
		Pop(cp, sockets[j]);

	EnableDialogBox(27, 1, 0, 0, 0);
	int i;
	if (sDir >= 1000)  // Set the item I want to exchange
	{	i = 0;
		while (m_stDialogBoxExchangeInfo[i].sV1 !=-1)
		{	i++;
			if (i>=4) return; // Error situation
		}
		if ((sDir >1000) && (i == 0))
		{	m_bIsItemDisabled[sDir -1000] = true;
			m_stDialogBoxExchangeInfo[0].sItemID = sDir -1000;
		}
	}else // Set the item he proposes me.
	{	i = 4;
		while (m_stDialogBoxExchangeInfo[i].sV1 !=-1)
		{	i++;
			if (i>=8) return; // Error situation
	}	}
	m_stDialogBoxExchangeInfo[i].sV1 = sSprite;
	m_stDialogBoxExchangeInfo[i].sV2 = sSpriteFrame;
	m_stDialogBoxExchangeInfo[i].sV3 = iAmount;
	m_stDialogBoxExchangeInfo[i].sV4 = cColor;
	m_stDialogBoxExchangeInfo[i].sV5 = (int)sCurLife;
	m_stDialogBoxExchangeInfo[i].sV6 = (int)sMaxLife;
	m_stDialogBoxExchangeInfo[i].sV7 = (int)sPerformance;
	memcpy(m_stDialogBoxExchangeInfo[i].cStr1, cItemName, 20);
	memcpy(m_stDialogBoxExchangeInfo[i].cStr2, cCharName, 10);
	m_stDialogBoxExchangeInfo[i].dwV1 = dwAttribute;

	for(int j = 0; j < MAXITEMSOCKETS; j++)
		m_stDialogBoxExchangeInfo[i].sockets[j] = sockets[j];
}

void CGame::NotifyMsg_JoinGuildApprove(char * pData)
{
	char * cp, cName[21];
	ZeroMemory(cName, sizeof(cName));
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	memcpy(cName, cp, 20);
	cp += 20;
	short * sp = (short *)cp;
	m_iGuildRank = (GuildRank)*sp;
	cp += 2;

	ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
	strcpy(m_cGuildName, cName);
	EnableDialogBox(8, 0, 0, 0);
	_PutGuildOperationList(cName, 3);
}


void CGame::NotifyMsg_JoinGuildReject(char * pData)
{
	char * cp, cName[21];
	ZeroMemory(cName, sizeof(cName));
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	Pop(cp, cName, 20);

	EnableDialogBox(8, 0, 0, 0);
	_PutGuildOperationList(cName, 4);
}

void CGame::ReceiveGuildBoard(char * data)
{
	uint16_t nposts;

	Pop(data, nposts);
	for(int i = 0; i < nposts; i++)
	{
		uint32_t id;
		Pop(data, id);

		string title;
		Pop(data, title);

		string sender;
		Pop(data, sender);

		Date d;
		Pop(data, d.month);
		Pop(data, d.day);

		m_gldBoard.push_back( new Mail(id, title, sender, d) );
	}
}

void CGame::ReceiveMailbox(char * data)
{
	uint16_t nmails;

	Pop(data, nmails);
	for(int i = 0; i < nmails; i++)
	{
		uint32_t id;
		Pop(data, id);

		string title;
		Pop(data, title);

		string sender;
		Pop(data, sender);

		Date d;
		Pop(data, d.month);
		Pop(data, d.day);

		m_mails.push_back( new Mail(id, title, sender, d) );
	}
}

void CGame::ReceiveMailData(char * data)
{
	uint32_t mailid;

	Pop(data, mailid);

	int idx = -1;

	for(int i = 0; i < m_mails.size(); i++)
	{
		if(m_mails[i]->id == mailid)
		{
			idx = i;
			break;
		}
	}

	if(idx != -1 && !m_mails[idx]->msg)
	{
		m_mails[idx]->msg = new string;
		Pop(data, *m_mails[idx]->msg);
	}

	uint8_t nitems;
	Pop(data, nitems);

	for(int i = 0; i < nitems; i++)
	{
		CItem * item = new CItem;
		
		Pop(data, item->m_cName);
		Pop(data, item->m_dwCount);
		Pop(data, (uint8_t&)item->m_cItemType);
		Pop(data, (uint8_t&)item->m_cEquipPos);
		Pop(data, (uint16_t&)item->m_sLevelLimit);
		Pop(data, (uint8_t&)item->m_cGenderLimit);
		Pop(data, item->m_wCurLifeSpan);
		Pop(data, item->m_wWeight);
		Pop(data, (uint16_t&)item->m_sSprite);
		Pop(data, (uint16_t&)item->m_sSpriteFrame);
		Pop(data, (uint32_t&)item->m_ItemColor);
		Pop(data, (uint16_t&)item->m_sItemSpecEffectValue2);
		Pop(data, item->m_dwAttribute);
		for(int i = 0; i < MAXITEMSOCKETS; i++)
			Pop(data, item->m_sockets[i]);
		Pop(data, item->ItemUniqueID);

		item->m_sX = 40;
		item->m_sY = 30;

		m_mails[idx]->items.push_back( item );
	}
}

void CGame::ReceiveGuildBoardPost(char * data)
{
	uint32_t mailid;

	Pop(data, mailid);

	int idx = -1;

	for(int i = 0; i < m_gldBoard.size(); i++)
	{
		if(m_gldBoard[i]->id == mailid)
		{
			idx = i;
			break;
		}
	}

	if(idx != -1 && !m_gldBoard[idx]->msg)
	{
		m_gldBoard[idx]->msg = new string;
		Pop(data, *m_gldBoard[idx]->msg);
	}
}
/*********************************************************************************************************************
** void CGame::CreateScreenShot()										(snoopy)									**
**  description			:: Fixed Screen Shots																		**
**********************************************************************************************************************/
void CGame::CreateScreenShot()
{
	int i;
	FILE * pFile;
	char cFn[256];
	char ServerName[128];
	char SStime[32], SStime2[32], SStime3[32]; // Added SStime2,3 xRisenx
	SYSTEMTIME SysTime; // Added New Details xRisenx
	GetLocalTime(&SysTime);
	ZeroMemory(ServerName, sizeof(ServerName));
	ZeroMemory(SStime, sizeof(SStime));
	wsprintfA(SStime, "Helbreath Xtreme");
	wsprintfA(SStime2, "%02d / %02d / %02d", SysTime.wMonth, SysTime.wDay, SysTime.wYear);
	wsprintfA(SStime3, "%02d : %02d : %02d", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);

	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );

	driver->beginScene(false, false);
	PutAlignedString(GetWidth()-180, GetWidth(), 10, SStime, video::SColor(255,255, 255, 255)); //ScreenShot time
	PutAlignedString(GetWidth()-180, GetWidth(), 30, SStime2, video::SColor(255,255, 255, 255)); //ScreenShot time
	PutAlignedString(GetWidth()-180, GetWidth(), 50, SStime3, video::SColor(255,255, 255, 255)); //ScreenShot time
	driver->endScene();
	IImage * screenshot = driver->createScreenShot();

	_mkdir("screenshots");
	for (i = 0; i < 1000; i++)
	{
		ZeroMemory(cFn, sizeof(cFn));
		wchar_t tempstr[300];
		wsprintfW(tempstr, L"screenshots\\HelShot%04d%02d%02d_%02d%02d%02d.jpg",
			(now->tm_year + 1900), (now->tm_mon + 1), now->tm_mday,
			now->tm_hour, now->tm_min, now->tm_sec);
		wsprintfA(cFn, "screenshots\\HelShot%04d%02d%02d_%02d%02d%02d.jpg",
			(now->tm_year + 1900), (now->tm_mon + 1), now->tm_mday,
			now->tm_hour, now->tm_min, now->tm_sec);
		//		   wsprintfA(cFn, "SAVE\\Helbreath Xtreme SS #%d.bmp", i);

		if(_access(cFn, 0 ) == -1){

			driver->writeImageToFile(screenshot, tempstr);

// 			video::IImage* image = driver->createImageFromData (
// 				bg->getColorFormat(),
// 				bg->getSize(),
// 				bg->lock(),
// 				false  //copy mem
// 				);
// 
// 			bg->unlock();
// 
// 			driver->writeImageToFile(image, "drawbackground.png");
			screenshot->drop();
			//				pFile = fopen(cFn, "rb");
			//				//DIRECTX m_DDraw.Screenshot(cFn, //DIRECTX m_DDraw.m_lpBackB4);

			wsprintfA(G_cTxt, NOTIFYMSG_CREATE_SCREENSHOT1, cFn);
			AddEventList(G_cTxt, 10);
			return;
		}
	}
	AddEventList(NOTIFYMSG_CREATE_SCREENSHOT2, 10);
}


bool CGame::_bDraw_OnCreateNewCharacter(char * pName, short msX, short msY, int iPoint)		// DrawCreateCharacter
{
 bool bFlag = true;
 uint32_t dwTime = unixtime();
 int i=0;

	////DIRECTX m_DDraw.ClearBackB4();//DIRECTX

	DrawNewDialogBox(SPRID_INTERFACE_ND_NEWCHAR, 0, 0, 0, true);
	//DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, 0, 0, 69, TRUE);
	PutString(124, 139-5, _BDRAW_ON_CREATE_NEW_CHARACTER1,video::SColor(255,255,255,255));//"
	PutString(96, 159-5, MSG_CHARACTERNAME,video::SColor(255,255,255,255));//"Character Name"
	if (m_cCurFocus != 1) PutString(239+3, 153+3, pName,video::SColor(255,25,35,25));
	PutString(119, 189-5, _BDRAW_ON_CREATE_NEW_CHARACTER2,video::SColor(255,255,255,255));//"
	PutString(113, 209-5, MSG_GENDER,video::SColor(255,255,255,255));//"Gender"
	PutString(113, 224-5, MSG_SKINCOLOR,video::SColor(255,255,255,255));//"Skin Color"
	PutString(113, 239-5, MSG_HAIRSTYLE,video::SColor(255,255,255,255));//"Hair Style"
	PutString(113, 254-5, MSG_HAIRCOLOR,video::SColor(255,255,255,255));//"Hair Color"
	PutString(113, 269-5, MSG_UNDERWEARCOLOR,video::SColor(255,255,255,255));//"Underwear Color"
	//PutAlignedString(64, 282, 245, _BDRAW_ON_CREATE_NEW_CHARACTER3, 5,5,5);
	//wsprintfA(G_cTxt, _BDRAW_ON_CREATE_NEW_CHARACTER4,  iPoint);//" %d points"
	//PutAlignedString(64, 282, 260, G_cTxt, 15,10,10);
	PutString(113, 324, MSG_STRENGTH,video::SColor(255,255,255,255));//"Strength"
	PutString(113, 341, MSG_VITALITY,video::SColor(255,255,255,255));//"Vitality"
	PutString(113, 358, MSG_DEXTERITY,video::SColor(255,255,255,255));//"Dexterity"
	PutString(113, 375, MSG_INTELLIGENCE,video::SColor(255,255,255,255));//"Intelligence"
	PutString(113, 392, MSG_MAGIC,video::SColor(255,255,255,255));//"Magic"
	PutString(113, 409, MSG_CHARISMA,video::SColor(255,255,255,255));//"Agility"

	//DrawNewDialogBox(SPRID_INTERFACE_ND_NEWCHAR, 0, 0, 0, TRUE);
	//DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 69, TRUE);
	//PutAlignedString(64, 282, 90, _BDRAW_ON_CREATE_NEW_CHARACTER1, 5,5,5);//"
	//PutAlignedString(57, 191, 110, MSG_CHARACTERNAME, 5,5,5);//"Character Name"
	//if (m_cCurFocus != 1) PutString(197, 112, pName,video::SColor(255,25,35,25));
	//PutAlignedString(64, 282, 140, _BDRAW_ON_CREATE_NEW_CHARACTER2, 5,5,5);//"
	//PutString(100, 160, MSG_GENDER,video::SColor(255,5,5,5));//"Gender"
	//PutString(100, 175, MSG_SKINCOLOR,video::SColor(255,5,5,5));//"Skin Color"
	//PutString(100, 190, MSG_HAIRSTYLE,video::SColor(255,5,5,5));//"Hair Style"
	//PutString(100, 205, MSG_HAIRCOLOR,video::SColor(255,5,5,5));//"Hair Color"
	//PutString(100, 220, MSG_UNDERWEARCOLOR,video::SColor(255,5,5,5));//"Underwear Color"
	////PutAlignedString(64, 282, 245, _BDRAW_ON_CREATE_NEW_CHARACTER3, 5,5,5);
	////wsprintfA(G_cTxt, _BDRAW_ON_CREATE_NEW_CHARACTER4,  iPoint);//" %d points"
	////PutAlignedString(64, 282, 260, G_cTxt, 15,10,10);
	//PutString(100, 275, MSG_STRENGTH,video::SColor(255,5,5,5));//"Strength"
	//PutString(100, 292, MSG_VITALITY,video::SColor(255,5,5,5));//"Vitality"
	//PutString(100, 309, MSG_DEXTERITY,video::SColor(255,5,5,5));//"Dexterity"
	//PutString(100, 326, MSG_INTELLIGENCE,video::SColor(255,5,5,5));//"Intelligence"
	//PutString(100, 343, MSG_MAGIC,video::SColor(255,5,5,5));//"Magic"
	//PutString(100, 360, MSG_CHARISMA,video::SColor(255,5,5,5));//"Charisma"

	for(int i=1;i<=6;i++)
	{
		switch(i)
		{
		case 1:
			wsprintfA(G_cTxt, "%d", m_createStat[STAT_STR]);
			PutAlignedString(269+3, 307-3, 308+i*16, G_cTxt,video::SColor(255,255,255,255));
			break;
		case 2:
			wsprintfA(G_cTxt, "%d", m_createStat[STAT_VIT]);
			PutAlignedString(269+3, 307-3, 308+i*16, G_cTxt,video::SColor(255,255,255,255));
			break;
		case 3:
			wsprintfA(G_cTxt, "%d", m_createStat[STAT_DEX]);
			PutAlignedString(269+3, 307-3, 308+i*16, G_cTxt,video::SColor(255,255,255,255));
			break;
		case 4:
			wsprintfA(G_cTxt, "%d", m_createStat[STAT_INT]);
			PutAlignedString(269+3, 307-3, 308+i*16, G_cTxt,video::SColor(255,255,255,255));
			break;
		case 5:
			wsprintfA(G_cTxt, "%d", m_createStat[STAT_MAG]);
			PutAlignedString(269+3, 307-3, 308+i*16, G_cTxt,video::SColor(255,255,255,255));
			break;
		case 6:
			wsprintfA(G_cTxt, "%d", m_createStat[STAT_CHR]);
			PutAlignedString(269+3, 307-3, 308+i*16, G_cTxt,video::SColor(255,255,255,255));
			break;
		}
	}

	if (strlen(pName) <= 0) bFlag = false;
	if (iPoint > 0) bFlag = false;
	if (m_Misc.bCheckValidName(pName) == false) bFlag = false;

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
	if((b_cWarrior == true) && (b_cArcher == false) && (b_cMage == false) && (b_cBattleMage == false))
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(87, 446, 13, dwTime);

	if ((msX >= 164) && (msX <= 236) && (msY >= 447) && (msY <= 467) && (b_cMage == false))								// Mage
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(164, 446, 12, dwTime);
	if((b_cMage == true) && (b_cArcher == false) && (b_cWarrior == false) && (b_cBattleMage == false))
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(164, 446, 11, dwTime);

	if ((msX >= 241) && (msX <= 313) && (msY >= 447) && (msY <= 467) && (b_cBattleMage == false))						// Battle Mage
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(241, 446, 10, dwTime);
	if((b_cBattleMage == true) && (b_cArcher == false) && (b_cMage == false) && (b_cWarrior == false))
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(241, 446, 9, dwTime);

	if(isArcherEnabled == true)
	{
		if ((msX >= 318) && (msX <= 390) && (msY >= 447) && (msY <= 467) && (b_cArcher == false))						// Archer
			m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(318, 446, 19, dwTime);
		if((b_cArcher == true) && (b_cBattleMage == false) && (b_cMage == false) && (b_cWarrior == false))
			m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(318, 446, 17, dwTime);
	} else {
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(318, 446, 18, dwTime);
	}

	if ((msX >= 281) && (msX <= 297) && (msY >= 205) && (msY <= 218))
	{
		if(m_cGender == 1)
		{
			m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(282, 205, 6, dwTime);
		}
	}

	if ((msX >= 320) && (msX <= 336) && (msY >= 205) && (msY <= 218))
	{
		if(m_cGender == 2)
		{
			m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(321, 205, 4, dwTime);
		}
	}

	if(m_cGender == 2)
	{
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR]->PutSpriteFast(282, 205, 5, dwTime);
	}
	if(m_cGender == 1)
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

	switch (m_cGender) {
	case 1:	_tmp_sOwnerType = 1; break;
	case 2:	_tmp_sOwnerType = 4; break; //@@@@@@@@@@@@@@@@@!!!!!!!!!!!!!!!!!
	}
	_tmp_sOwnerType += m_cSkinCol - 1;
	_tmp_cDir   = m_cMenuDir;
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
	memcpy(_tmp_cName, m_cPlayerName,10);
	_tmp_cAction = OBJECTMOVE;
	_tmp_cFrame = m_cMenuFrame;

	_Draw_CharacterBody(611, 277, _tmp_sOwnerType);

	DrawObject_OnMove_ForMenu(0, 0, 500+80+58, 174+120-56, false, dwTime, msX, msY);

    i = 0 ;

	PutString(572, 129, MSG_HITPOINT,video::SColor(255,255,255,255));//"Hit Point"
	//wsprintfA(G_cTxt, "%d", m_createStat[STAT_VIT]*3 + 2 + m_createStat[STAT_STR]/2);
	wsprintfA(G_cTxt, "%d", m_createStat[STAT_VIT]*8 + m_createStat[STAT_STR]*2 + m_createStat[STAT_INT]*2 + 8); // HP System xRisenx
	PutString(572+120, 129 + 16*i++, G_cTxt,video::SColor(255,255,255,255));

	PutString(572, 145, MSG_MANAPOINT,video::SColor(255,255,255,255));//"Mana Point"
	wsprintfA(G_cTxt, "%d", m_createStat[STAT_MAG]*3 + 2 + m_createStat[STAT_INT]*2); // MP System xRisenx
	PutString(572+120, 129 + 16*i++, G_cTxt,video::SColor(255,255,255,255));

	PutString(572, 161, MSG_STAMINARPOINT,video::SColor(255,255,255,255));//"Stamina Point"
	wsprintfA(G_cTxt, "%d", m_createStat[STAT_STR] + 17); // SP System xRisenx
	PutString(572+120, 129 + 16*i++, G_cTxt,video::SColor(255,255,255,255));

	return bFlag;
}

#ifdef MAKE_ACCOUNT
void CGame::_LoadAgreementTextContents(char cType)
{
 char * pContents, * token, cTemp[120], cFileName[120];
 char   seps[] = "\n";
 int    iIndex = 0, i;
 class  CStrTok * pStrTok;
 uint32_t  dwFileSize;
 HANDLE hFile;
 FILE * pFile;

	for (i = 0; i < TEXTDLGMAXLINES; i++) {
		if (m_pAgreeMsgTextList[i] != 0)
			delete m_pAgreeMsgTextList[i];
		m_pAgreeMsgTextList[i] = 0;
	}

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));

	wsprintfA(cTemp, "contents%d", cType);

	strcat(cFileName, "data\\shops");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cTemp);
	strcat(cFileName, ".txt");

	hFile = CreateFileA(cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	dwFileSize = GetFileSize(hFile, 0);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFileName, "rt");
	if (pFile == 0) return;
	else {
		pContents = new char[dwFileSize+1];
		ZeroMemory(pContents, dwFileSize+1);
		fread(pContents, dwFileSize, 1, pFile);
	}

	fclose(pFile);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while( token != 0 ) {
		m_pAgreeMsgTextList[iIndex] = new class CMsg(0, token, 0);
		token = pStrTok->pGet();
		iIndex++;
	}
	delete pStrTok;
	delete[] pContents;
}


#endif //endif from #ifdef MAKE_ACCOUNT

void CGame::StartLogin()
{
    CreateSocket();
    if (_socket == nullptr)
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
        new_connection_->socket().async_connect(endpoint,
                                                boost::bind(&CGame::handle_connect, this,
                                                            boost::asio::placeholders::error));
    }
    else
    {
        ConnectionEstablishHandler(SERVERTYPE_LOG);
    }
}

void CGame::UpdateScreen_OnLogin()
{
	short msX, msY, msZ, sX, sY;
	char cLB, cRB, cMB;
	char cMIresult;
	int  iMIbuttonNum;
	static char  cPassword[12], cPrevFocus;
	static char cName[12];

	sX = 146;
	sY = 114;
	if (m_cGameModeCount == 0)
	{
		EndInputString();
		m_stMCursor.sCursorFrame = 0;

		if (_socket)
		{
			_socket->stop();
		}
		if (new_connection_)
		{
			new_connection_->stop();
		}
	}

	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;
	
	/*
	if (m_bEnterPressed == true)
	{
		m_bEnterPressed = false;
		PlaySound('E', 14, 5);

		switch (m_cCurFocus) {
		case 1:
			m_cCurFocus++;
			if( m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
			break;
		case 2:
		case 3:
// 			if(CheckCheating()) {
// 				MessageBoxA(*(HWND*)m_hWnd, "Error Code: 1600\n\nClient.exe has detected an illegal program or modification.\n\nGame Closing.", "Hack detected!", MB_OK | MB_ICONERROR);
// 				exit(1600);
// 			}
			if ((strlen(cName) == 0) || (strlen(cPassword) == 0)) break;
			ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
			ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
			strcpy(m_cAccountName, cName);
			strcpy(m_cAccountPassword, cPassword);
			ChangeGameMode(GAMEMODE_ONCONNECTING);
			m_dwConnectMode = MSGID_REQUEST_LOGIN;
			ZeroMemory(m_cMsg, sizeof(m_cMsg));
			strcpy(m_cMsg, "11");
			delete pMI;
			if (_socket == nullptr)
			{
				boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
				new_connection_->socket().async_connect(endpoint,
					boost::bind(&CGame::handle_connect, this,
					boost::asio::placeholders::error));
			}
			else
			{
				ConnectionEstablishHandler(SERVERTYPE_LOG);
			}
			return;
		case 4:	// Exit
// #ifdef SELECTSERVER
// 			ChangeGameMode(GAMEMODE_ONSELECTSERVER);
// #else
// 			ChangeGameMode(GAMEMODE_ONMAINMENU);
// #endif
// 			
// 			delete pMI;
// 			ChangeGameMode(GAMEMODE_ONQUIT);
			return;
		case 5: //Create Account Button
			return;
		case 6: // Forums Button
			return;
		case 7: // Website Button
			return;
		}
	}

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
	{
		PlaySound('E', 14, 5);
		switch (iMIbuttonNum) {
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
			ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
			ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
			strcpy(m_cAccountName, cName);
			strcpy(m_cAccountPassword, cPassword);
			ChangeGameMode(GAMEMODE_ONCONNECTING);
			m_dwConnectMode = MSGID_REQUEST_LOGIN;
			ZeroMemory(m_cMsg, sizeof(m_cMsg));
			strcpy(m_cMsg, "11");
			delete pMI;
			if (_socket == nullptr)
			{
				boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
				new_connection_->socket().async_connect(endpoint,
					boost::bind(&CGame::handle_connect, this,
					boost::asio::placeholders::error));
			}
			else
			{
				ConnectionEstablishHandler(SERVERTYPE_LOG);
			}
			return;

		case 4: // Exit button
/ *#ifdef SELECTSERVER
			ChangeGameMode(GAMEMODE_ONSELECTSERVER); // ONMAINMENU
#else
			ChangeGameMode(GAMEMODE_ONMAINMENU);
#endif* /
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
			if(b_cRemember == true)
			{
				b_cRemember = false;
				WriteUsername(cName, true);
				return;
			} else {
				b_cRemember = true;
				if(sizeof(cName) > 0)
				{
					WriteUsername(cName, false);
				}
				return;
			}
			return;
		}
	}*/

	_Draw_OnLogin(cName, cPassword, msX, msY, m_cGameModeCount);
}

void CGame::OnSysKeyDown(WPARAM wParam)
{
	switch( wParam )
	{
	case VK_SHIFT:
		m_bShiftPressed = true;
		break;
	case VK_CONTROL:
		m_bCtrlPressed = true;
		break;
	case VK_RETURN:
		if (m_altPressed)
		{
			for (int i = 0; i < MAXSPRITES; ++i)
			{
				delete m_pSprite[i];
				m_pSprite[i] = 0;
			}
			for (int i = 0; i < MAXTILES; ++i)
			{
				delete m_pTileSpr[i];
				m_pTileSpr[i] = 0;
			}
			
			for (int i = 0; i < MAXEFFECTSPR; ++i)
			{
				delete m_pEffectSpr[i];
				m_pEffectSpr[i] = 0;
			}

			//fullscreen swap?
			driver->endScene();
			device->closeDevice();
			if (fullscreen)
			{
				this->screenwidth = 800;
				this->screenheight = 600;
			}
			else
			{
				this->screenwidth = 1920;
				this->screenheight = 1080;
			}
			oldmode = m_cGameMode;
			CreateRenderer(!fullscreen);
			m_hWnd = G_hWnd;
			//m_cGameMode = GAMEMODE_ONLOADING;
			ChangeGameMode(GAMEMODE_ONLOADING);
			fullscreenswap = true;
			driver->beginScene();

			m_pSprite[SPRID_INTERFACE_ND_LOADING] = CSprite::CreateSprite(L"New-Dialog", 0, false);
			m_pSprite[SPRID_INTERFACE_ADDINTERFACE] = CSprite::CreateSprite(L"interface2", 0, false);
			m_pSprite[SPRID_INTERFACE_CRAFTING] = CSprite::CreateSprite(L"interface2", 3, false);
			m_pSprite[SPRID_INTERFACE_SPRFONTS2] = CSprite::CreateSprite(L"interface2", 1, false);
			m_pSprite[SPRID_INTERFACE_F1HELPWINDOWS] = CSprite::CreateSprite(L"interface2", 2, false);
			m_pSprite[SPRID_INTERFACE_FONT1] = CSprite::CreateSprite(L"sprfonts", 0, false);
			m_pSprite[SPRID_INTERFACE_FONT2] = CSprite::CreateSprite(L"sprfonts", 1, false);

			m_cLoading = 0;
		}
		m_bEnterPressed = true;
		break;
	case VK_MENU:
		m_altPressed = true;
		break;
	case VK_F10:
		if(!m_bIsDialogEnabled[57] && m_iGuildRank != GUILDRANK_NONE)
			EnableDialogBox(57, 0, 0, 0);
		else
			DisableDialogBox(57);
		break;
	}
}

void CGame::OnSysKeyUp(WPARAM wParam)
{
	switch( wParam )
	{
	case VK_SHIFT:
		m_bShiftPressed = false;
		break;
	case VK_CONTROL:
		m_bCtrlPressed = false;
		break;
	case VK_RETURN:
		m_bEnterPressed = false;
		if( m_bToggleScreen == true )
		{
			m_bIsRedrawPDBGS = true;
			////DIRECTX m_DDraw.ChangeDisplayMode(G_hWnd);
		}
		break;
	case VK_ESCAPE:
		m_bEscPressed = false;
	case VK_MENU:
		m_altPressed = false;
		break;

	}
}

void CGame::OnKeyUp(WPARAM wParam)
{
 int i=0;
 uint32_t dwTime = unixtime();
 static uint32_t dwPrevTabTime = 0;

	switch (wParam) {
	case VK_ADD:
		foregroundfpstarget++;
		backgroundfpstarget++;
		break;
	case VK_SUBTRACT:
		foregroundfpstarget--;
		backgroundfpstarget--;
		break;
	case VK_MENU:
		m_altPressed = false;
		break;
	case VK_SHIFT:
	case 160://temporary (irrlicht shift code)
		m_bShiftPressed = false;
		break;
	case VK_CONTROL:
	case 162://temporary
		m_bCtrlPressed = false;
		break;

	case 65://'A'
		if (m_bCtrlPressed && m_cGameMode == GAMEMODE_ONMAINGAME && (!m_bInputStatus) )
		{	if( m_bForceAttack )
			{	m_bForceAttack = false;
				AddEventList( MSG_FORCEATTACK_OFF, 10 );
			}else
			{	m_bForceAttack = true;
				AddEventList( MSG_FORCEATTACK_ON, 10 );
		}	}
		break;

	case 67://'C' // Crit Command / CTRL + C xRisenx
		if (m_bCtrlPressed && m_cGameMode == GAMEMODE_ONMAINGAME && (!m_bInputStatus) )
		{
			bSendCommand(MSGID_REQUEST_CRITICALS, 0, 0, 0, 0, 0, 0, 0);
		}
		break;

	case 68://'D'
		if (m_bCtrlPressed == true && m_cGameMode == GAMEMODE_ONMAINGAME && (!m_bInputStatus) )
		{
            m_cDetailLevel++;
			if( m_cDetailLevel > 2 ) m_cDetailLevel = 0;
			switch( m_cDetailLevel )
            {
			case 0:
				AddEventList( NOTIFY_MSG_DETAIL_LEVEL_LOW, 10 );
				break;
			case 1:
				AddEventList( NOTIFY_MSG_DETAIL_LEVEL_MEDIUM, 10 );
				break;
			case 2:
				AddEventList( NOTIFY_MSG_DETAIL_LEVEL_HIGH, 10 );
				break;
		    }
        }
		break;

	case 70: //'F'
		if( m_bCtrlPressed )
		{
            LoadFriendList();
			UpdateFriendsStatus();
            EnableDialogBox(43, 0, 0, 0);
        }
        break;

	case 72: // 'H' // Snoopy: Mimics VK_F1
		if (m_bCtrlPressed && m_cGameMode == GAMEMODE_ONMAINGAME && (!m_bInputStatus) )
		{
            if (m_bIsDialogEnabled[35] == false)
				EnableDialogBox(35, 0, 0, 0);
			else
			{
                DisableDialogBox(35);
				DisableDialogBox(18);
		    }
        }
		break;

	case 87: // 'W' // Snoopy: mimics VK_F11 Togles transparency
		if (m_bCtrlPressed && m_cGameMode == GAMEMODE_ONMAINGAME && (!m_bInputStatus) )
		{
            m_bDialogTrans = !m_bDialogTrans;
		}
		break;

	case 88: // 'X' // Snoopy: mimics VK_F12 Logout Window
		if (m_bCtrlPressed && m_cGameMode == GAMEMODE_ONMAINGAME && (!m_bInputStatus) )
		{
            if (m_bIsDialogEnabled[19] == false)
				EnableDialogBox(19, 0, 0, 0);
			else DisableDialogBox(19);
		}
		break;

	case 77://'M'
		if( m_cGameMode == GAMEMODE_ONMAINGAME )
		{
            if( m_bCtrlPressed )
			{
                if( m_bIsDialogEnabled[9] == true ) DisableDialogBox(9);
				else EnableDialogBox(9, 0, 0, 0, 0);
		    }
        }
		break;

	case 78://'N'
		if(m_bCtrlPressed && m_cGameMode == GAMEMODE_ONMAINGAME && !m_bInputStatus ) {	
			if( m_targetNeutral ) {	
				m_targetNeutral = false;
				AddEventList( MSG_TARGETNEUTRAL_OFF, 10 );
			} else {	
				m_targetNeutral = true;
				AddEventList( MSG_TARGETNEUTRAL_ON, 10 );
			}
		}
		break;

	case 80://'P'
		if( ( m_bCtrlPressed == true ) && ( m_cGameMode == GAMEMODE_ONMAINGAME ) && (!m_bInputStatus) )
		{
			if( _tmp_sOwnerType < 7 && (strlen(_tmp_cName)>0) && (m_iIlusionOwnerH==0)
				&& (strcmp(m_cMCName, m_cPlayerName) != 0) && m_cMCName[0] != '_')
            {
                m_dialogBoxes[32].SetMode(3);
                PlaySound('E', 14, 5);
                ZeroMemory(m_dialogBoxes[32].cStr, sizeof(m_dialogBoxes[32].cStr));
                strcpy(m_dialogBoxes[32].cStr, _tmp_cName);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_JOINPARTY, 0, 1, 0, 0, _tmp_cName);
			}
			else {
				m_dialogBoxes[32].SetMode(0);
				PlaySound('E', 14, 5);

				AddEventList(POINT_COMMAND_HANDLER1, 10);
			}
		}
		break;

	case 82://'R'
		if (m_bCtrlPressed == true && m_cGameMode == GAMEMODE_ONMAINGAME && (!m_bInputStatus) )
		{
            if( m_bRunningMode )
			{
            m_bRunningMode = false;
				AddEventList( NOTIFY_MSG_CONVERT_WALKING_MODE, 10 );
			}
            else
			{
            m_bRunningMode = true;
				AddEventList( NOTIFY_MSG_CONVERT_RUNNING_MODE, 10 );
		    }
        }
		break;

	case 83://'S'
		if (m_bCtrlPressed == true && m_cGameMode == GAMEMODE_ONMAINGAME && (!m_bInputStatus) )
		{
			if (m_bMusicStat == true) // Music Off
			{
				m_bMusicStat = false;
				if (m_bSoundFlag)
				{
					if (m_pBGM != nullptr)
					{
						m_pBGM->stop();
					}
				}
				AddEventList( NOTIFY_MSG_MUSIC_OFF, 10 );
				break;
			}
			else if( m_bSoundStat == true )
			{
				m_pESound[38]->stop();
				m_bSoundStat = false;
				AddEventList( NOTIFY_MSG_SOUND_OFF, 10 );
				break;
			}
            else 	// Music On
			{
				if( m_bSoundFlag )
				{
					m_bMusicStat = true;
					AddEventList( NOTIFY_MSG_MUSIC_ON, 10 );
				}
				if( m_bSoundFlag )
				{
					m_bSoundStat = true;
					AddEventList( NOTIFY_MSG_SOUND_ON, 10 );
				}
				StartBGM();
			}
		}
		break;

	case 84: //'T'
		if(m_bCtrlPressed && m_cGameMode == GAMEMODE_ONMAINGAME && (!m_bInputStatus) )
		{	
			char tempid[100];
			uint8_t onButton = m_dialogBoxes[DIALOG_CHAT].OnButton();

			ZeroMemory( tempid, sizeof( tempid ) );

			if( m_bIsDialogEnabled[10] == true && onButton >= 10 &&  onButton <= 17)
			{
				CStrTok *pStrTok;
				char   * token, cBuff[128];
				char   seps[] = "]: ";

				int i = abs(onButton - 17);
				if( i + m_dialogBoxes[10].sView >= m_chatDisplay->size() ) return;
				CMsg * msg = *(m_chatDisplay->rbegin() + m_dialogBoxes[10].sView + i);

				if(msg->m_pMsg[0] == ' ' ) i++;
				strcpy(cBuff, msg->m_pMsg);

				pStrTok = new CStrTok(cBuff, seps);
				token = pStrTok->pGet();
				if(cBuff[0] == '[') // Time stamped msg
				{
					token = pStrTok->pGet();
					token = pStrTok->pGet();
					token = pStrTok->pGet();
				}
				if(token != 0){
					wsprintfA( tempid, "/to %s", token );
					bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
				}
				delete pStrTok;
			}
            else if( _tmp_sOwnerType < 7 && (strlen(_tmp_cName)>0) && (m_iIlusionOwnerH==0)
						&& ((m_bIsCrusadeMode == false) || _iGetFOE(_tmp_iStatus) >= 0))
			{
                wsprintfA( tempid, "/to %s", _tmp_cName );
				bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
			}
            else
			{
                EndInputString();
				wsprintfA( m_cChatMsg, "/to " );
				//StartInputString(10, 414, sizeof(m_cChatMsg), m_cChatMsg);
				StartInputString(10, 530, sizeof(m_cChatMsg), m_cChatMsg); // 800x600 Resolution xRisenx 534 is right / 530 fits?
		    }
        }
		break;
// 	case 107: //'+'
// 		if(m_bInputStatus == FALSE) m_bZoomMap = TRUE;
// 		break;
// 	case 109: //'-'
// 		if(m_bInputStatus == FALSE) m_bZoomMap = FALSE;
// 		break;

	case VK_F2:
		UseShortCut( 1 );
		break;

	case VK_F3:
		UseShortCut( 2 );
		break;

	case VK_INSERT:
		if (m_iHP <= 0) return;
		if (m_bItemUsingStatus == true)
		{
            AddEventList(USE_RED_POTION1, 10);
			return;
		}
		if (m_bIsDialogEnabled[27] == true)
		{
            AddEventList(USE_RED_POTION2, 10);
			return;
		}
		for (i = 0; i < MAXITEMS; i++)
		if ( (m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
			 (m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 1))
		{
            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);
			m_bIsItemDisabled[i] = true;
			m_bItemUsingStatus = true;
			return;
		}

		for (i = 0; i < MAXITEMS; i++)
		if ( (m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
			 (m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 2))
		{
            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);
			m_bIsItemDisabled[i] = true;
			m_bItemUsingStatus = true;
			return;
		}
		break;

	case VK_DELETE:
		if (m_iHP <= 0) return;
		if (m_bItemUsingStatus == true)
		{
            AddEventList(USE_BLUE_POTION1, 10);
			return;
		}
		if (m_bIsDialogEnabled[27] == true)
		{
            AddEventList(USE_BLUE_POTION2, 10);
			return;
		}

		for (i = 0; i < MAXITEMS; i++)
		if ( (m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
			 (m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 3))
		{
            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);
			m_bIsItemDisabled[i] = true;
			m_bItemUsingStatus = true;
			return;
		}

		for (i = 0; i < MAXITEMS; i++)
		if ( (m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
			 (m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 4))
		{
            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);
			m_bIsItemDisabled[i] = true;
			m_bItemUsingStatus = true;
			return;
		}
		break;

	case VK_END:
		if(!(m_bIsDialogEnabled[7] && m_dialogBoxes[7].GetMode() == 1 && iGetTopDialogBoxIndex() == 7) &&
			!(m_bIsDialogEnabled[17] && m_dialogBoxes[17].GetMode() == 1 && iGetTopDialogBoxIndex() == 17) &&
			!(m_bIsDialogEnabled[58] && m_dialogBoxes[58].GetMode() == 1 && iGetTopDialogBoxIndex() == 58) &&
			!(m_bIsDialogEnabled[62] && m_dialogBoxes[62].GetMode() == 1 && iGetTopDialogBoxIndex() == 62) &&
			!m_bInputStatus && m_cBackupChatMsg[0] != '!' && m_cBackupChatMsg[0] != '~' &&
			m_cBackupChatMsg[0] != '^' && m_cBackupChatMsg[0] != '@')
		{
			ZeroMemory(m_cChatMsg, sizeof(m_cChatMsg));
			strcpy(m_cChatMsg, m_cBackupChatMsg);
			//StartInputString(10, 414, sizeof(m_cChatMsg), m_cChatMsg);
			StartInputString(10, 530, sizeof(m_cChatMsg), m_cChatMsg); // 800x600 Resolution xRisenx 534 is right / 530 fits?
		}
		break;

	case VK_F4:
		if(m_cGameMode != GAMEMODE_ONMAINGAME) return;
		UseMagic(m_sMagicShortCut);
		break;

	case VK_F5:
		if (m_bIsDialogEnabled[1] == false)
			EnableDialogBox(1, 0, 0, 0);
		else DisableDialogBox(1);
		break;

	case VK_F6:
		if (m_bIsDialogEnabled[2] == false)
			EnableDialogBox(2, 0, 0, 0);
		else DisableDialogBox(2);
		break;

	case VK_F7:
		if (m_bIsDialogEnabled[3] == false)
			EnableDialogBox(3, 0, 0, 0);
		else DisableDialogBox(3);
		break;
//#ifdef TitleClient
	/*case VK_F8:
		if (m_bIsDialogEnabled[63] == FALSE)
			EnableDialogBox(63, NULL, NULL, NULL);
		else DisableDialogBox(63);
		break;*/
//#else
		case VK_F8:
		if (m_bIsDialogEnabled[15] == false)
			EnableDialogBox(15, 0, 0, 0);
		else DisableDialogBox(15);
		break;
//#endif
	case VK_F9:
		if (m_bIsDialogEnabled[10] == false)
			EnableDialogBox(10, 0, 0, 0);
		else DisableDialogBox(10);
		break;

	case VK_F11:
		m_bDialogTrans = !m_bDialogTrans;
		break;

	case VK_F12:
		if(m_bInputStatus) return;
		if (m_bIsDialogEnabled[19] == false)
			EnableDialogBox(19, 0, 0, 0);
		else DisableDialogBox(19);
		CreateScreenShot();
		break;

	case VK_F1:
		if (m_bInputStatus) return;
		if (m_bIsDialogEnabled[35] == false) // 35 CLEROTH
			EnableDialogBox(35, 0, 0, 0);
		else
		{	DisableDialogBox(35);
			DisableDialogBox(18);
		}
		break;

	case VK_UP:
		m_cArrowPressed	= 1;
		if( m_cGameMode == GAMEMODE_ONMAINGAME )
		{	int iTotalMsg=0;
			for( int i=MAXWHISPERMSG-1 ; i>=0 ; i-- )
			{	if( m_pWhisperMsg[i] != 0 )
				{	iTotalMsg = i;
					break;
			}	}
			m_cWhisperIndex ++;
			if( m_cWhisperIndex > iTotalMsg ) m_cWhisperIndex = 0;
			if( m_cWhisperIndex < 0 ) m_cWhisperIndex = iTotalMsg;
			if( m_pWhisperMsg[m_cWhisperIndex] != 0 ) {
			EndInputString();
			wsprintfA( m_cChatMsg, "/to %s", m_pWhisperMsg[m_cWhisperIndex]->m_pMsg );
			//StartInputString(10, 414, sizeof(m_cChatMsg), m_cChatMsg);
			StartInputString(10, 530, sizeof(m_cChatMsg), m_cChatMsg); // 800x600 Resolution xRisenx 534 is right / 530 fits?
		}	}
		break;

	case VK_RIGHT:
		m_cArrowPressed	= 2;
		break;

	case VK_DOWN:
		m_cArrowPressed	= 3;
		if( m_cGameMode == GAMEMODE_ONMAINGAME )
		{	int iTotalMsg=0;
			for( int i=MAXWHISPERMSG-1 ; i>=0 ; i-- )
			{	if( m_pWhisperMsg[i] != 0 )
				{	iTotalMsg = i;
					break;
			}	}
			m_cWhisperIndex --;
			if( m_cWhisperIndex < 0 ) m_cWhisperIndex = iTotalMsg;
			if( m_cWhisperIndex > iTotalMsg ) m_cWhisperIndex = 0;
			if( m_pWhisperMsg[m_cWhisperIndex] != 0 ) {
			EndInputString();
			wsprintfA( m_cChatMsg, "/to %s", m_pWhisperMsg[m_cWhisperIndex]->m_pMsg );
			//StartInputString(10, 414, sizeof(m_cChatMsg), m_cChatMsg);
			StartInputString(10, 530, sizeof(m_cChatMsg), m_cChatMsg); // 800x600 Resolution xRisenx 534 is right / 530 fits?
		}	}
		break;

	case VK_LEFT:
		m_cArrowPressed	= 4;
		break;

	case VK_SNAPSHOT:
		CreateScreenShot();
		break;

#ifndef USING_WIN_IME
	case VK_TAB:
		if( m_bShiftPressed )
		{	m_cCurFocus--;
			if( m_cCurFocus < 1 ) m_cCurFocus = m_cMaxFocus;
		}else
		{	m_cCurFocus++;
			if( m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
		}
		if ((dwTime - dwPrevTabTime) > 500 && m_cGameMode == GAMEMODE_ONMAINGAME)
		{
			dwPrevTabTime = dwTime;
			bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);
		}
		break;

	case VK_RETURN:
		m_bEnterPressed = true;
		//if (m_altPressed)
			//change screen mode
		break;
#endif

	case VK_HOME:
		if (m_cGameMode == GAMEMODE_ONMAINGAME) {
			bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TOGGLESAFEATTACKMODE, 0, 0, 0, 0, 0);
		}
		break;

	case VK_ESCAPE:
		m_bEscPressed = true;
		if (m_cGameMode == GAMEMODE_ONMAINGAME)
		{
			if ((m_bIsObserverMode == true) && (m_bShiftPressed))
			{ //ObserverMode Shift+Esc
				// Log Out
				if (m_cLogOutCount == -1) m_cLogOutCount = 1;
				DisableDialogBox(19);
				PlaySound('E', 14, 5);
			}
			else if(m_cLogOutCount != -1)
			{
				if (m_bForceDisconn == false)
				{ //Esc
					m_cLogOutCount = -1;
					AddEventList(DLGBOX_CLICK_SYSMENU2, 10);
				}
			}
			if (m_bIsGetPointingMode == true)
			{
				m_bIsGetPointingMode = false;
				AddEventList(COMMAND_PROCESSOR1, 10);
			}
			m_bIsF1HelpWindowEnabled = false;
		}
		if (!clipmousewindow)
		{
			RECT trect;
			GetWindowRect(*(HWND*)&G_hWnd, &trect);
			ClipCursor( &trect );
			clipmousegame = true;
			clipmousewindow = true;
		}
		else 
		{
			clipmousewindow = clipmousegame = false;
			ClipCursor( 0 );
		}
		break;

	case 33:
		if (m_cGameMode != GAMEMODE_ONMAINGAME) return;
		if (m_bInputStatus) return;
		if (m_bIsSpecialAbilityEnabled == true)
		{	if (m_iSpecialAbilityType != 0) {
				bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_ACTIVATESPECABLTY, 0, 0, 0, 0, 0);
				m_bIsSpecialAbilityEnabled = false;
			}
			else AddEventList(ON_KEY_UP26, 10);
		}
		else {
			if (m_iSpecialAbilityType == 0) AddEventList(ON_KEY_UP26, 10);
			else {
				if ((m_sPlayerAppr4 & 0x00F0) != 0) {
					AddEventList(ON_KEY_UP28, 10);
					return;
				}

				i = (dwTime - m_dwSpecialAbilitySettingTime)/1000;
				i = m_iSpecialAbilityTimeLeftSec - i;
				if (i < 0) i = 0;

				ZeroMemory(G_cTxt, sizeof(G_cTxt));
				if (i < 60) {
					switch (m_iSpecialAbilityType) {
					case 1: wsprintfA(G_cTxt, ON_KEY_UP29, i); break;//"
					case 2: wsprintfA(G_cTxt, ON_KEY_UP30, i); break;//"
					case 3: wsprintfA(G_cTxt, ON_KEY_UP31, i); break;//"
					case 4: wsprintfA(G_cTxt, ON_KEY_UP32, i); break;//"
					case 5: wsprintfA(G_cTxt, ON_KEY_UP33, i); break;//"
					case 50:wsprintfA(G_cTxt, ON_KEY_UP34, i); break;//"
					case 51:wsprintfA(G_cTxt, ON_KEY_UP35, i); break;//"
					case 52:wsprintfA(G_cTxt, ON_KEY_UP36, i); break;//"
					}
				}
				else {
					switch (m_iSpecialAbilityType) {
					case 1: wsprintfA(G_cTxt, ON_KEY_UP37, i/60); break;//"
					case 2: wsprintfA(G_cTxt, ON_KEY_UP38, i/60); break;//"
					case 3: wsprintfA(G_cTxt, ON_KEY_UP39, i/60); break;//"
					case 4: wsprintfA(G_cTxt, ON_KEY_UP40, i/60); break;//"
					case 5: wsprintfA(G_cTxt, ON_KEY_UP41, i/60); break;//"
					case 50:wsprintfA(G_cTxt, ON_KEY_UP42, i/60); break;//"
					case 51:wsprintfA(G_cTxt, ON_KEY_UP43, i/60); break;//"
					case 52:wsprintfA(G_cTxt, ON_KEY_UP44, i/60); break;//"
					}
				}
				AddEventList(G_cTxt, 10);
			}
		}
		break;
	}
}

void CGame::OnKeyDown(WPARAM wParam)
{
    if (m_bShiftPressed)
    {
        switch (wParam)
        {
            case 39:
                viewdstxvar--;
                m_sViewDstX = (m_sPlayerX - viewdstxvar) * 32;
                m_sViewDstY = (m_sPlayerY - viewdstyvar) * 32;
                break;
            case 40:
                viewdstyvar--;
                m_sViewDstX = (m_sPlayerX - viewdstxvar) * 32;
                m_sViewDstY = (m_sPlayerY - viewdstyvar) * 32;
                break;
            case 37:
                viewdstxvar++;
                m_sViewDstX = (m_sPlayerX - viewdstxvar) * 32;
                m_sViewDstY = (m_sPlayerY - viewdstyvar) * 32;
                break;
            case 38:
                viewdstyvar++;
                m_sViewDstX = (m_sPlayerX - viewdstxvar) * 32;
                m_sViewDstY = (m_sPlayerY - viewdstyvar) * 32;
                break;
        }
        return;
    }
    if (m_bCtrlPressed)
    {
        switch (wParam)
        {
            case 39:
                viewdstxcharvar--;
                break;
            case 40:
                viewdstycharvar--;
                break;
            case 37:
                viewdstxcharvar++;
                break;
            case 38:
                viewdstycharvar++;
                break;
        }
        return;
    }
	switch (wParam)
    {
//         case 39://right arrow
//             m_pMapData->m_sPivotX += 1;
//             break;
//         case 40://down arrow
//             m_pMapData->m_sPivotY += 1;
//             break;
//         case 37://left arrow
//             m_pMapData->m_sPivotX -= 1;
//             break;
//         case 38://up arrow
//             m_pMapData->m_sPivotY -= 1;
//             break;
        case 39:
            viewdstxvar--;
            m_sViewDstX = (m_sPlayerX - viewdstxvar) * 32;
            m_sViewDstY = (m_sPlayerY - viewdstyvar) * 32;
            break;
        case 40:
            viewdstyvar--;
            m_sViewDstX = (m_sPlayerX - viewdstxvar) * 32;
            m_sViewDstY = (m_sPlayerY - viewdstyvar) * 32;
            break;
        case 37:
            viewdstxvar++;
            m_sViewDstX = (m_sPlayerX - viewdstxvar) * 32;
            m_sViewDstY = (m_sPlayerY - viewdstyvar) * 32;
            break;
        case 38:
            viewdstyvar++;
            m_sViewDstX = (m_sPlayerX - viewdstxvar) * 32;
            m_sViewDstY = (m_sPlayerY - viewdstyvar) * 32;
            break;
        case VK_CONTROL:
	    case 162://temporary
		    m_bCtrlPressed = true;
		    break;
	    case VK_SHIFT:
	    case 160://temporary (irrlicht shift code)
		    m_bShiftPressed = true;
		    break;
	    case VK_INSERT:
	    case VK_DELETE:
		    break;
	    case VK_TAB:
		    break;
	    case VK_RETURN:
	    case VK_ESCAPE:
	    case VK_END:
	    case VK_HOME:
		    break;
	    case VK_F1:
		    break;
	    case VK_F2:
	    case VK_F3:
	    case VK_F4:
	    case VK_F5:
	    case VK_F6:
	    case VK_F7:
	    case VK_F8:
	    case VK_F9:
	    case VK_F10:
	    case VK_F11:
	    case VK_F12:
	    case VK_PRIOR: // Page-Up
	    case VK_NEXT: // Page-Down
	    case VK_LWIN:
	    case VK_RWIN:
	    case VK_MULTIPLY:
	    case VK_ADD: //'+'
	    case VK_SEPARATOR:
	    case VK_SUBTRACT: //'-'
	    case VK_DECIMAL:
	    case VK_DIVIDE:
	    case VK_NUMLOCK:
	    case VK_SCROLL:
		    break;

	    default:
		    if (m_cGameMode == GAMEMODE_ONMAINGAME)
		    {	if (m_bCtrlPressed)
			    {	switch (wParam) {
				    case 48: EnableDialogBox(3, 0, 0, 0); m_dialogBoxes[3].sView = 9; break; // 0
				    case 49: EnableDialogBox(3, 0, 0, 0); m_dialogBoxes[3].sView = 0; break; // 1
				    case 50: EnableDialogBox(3, 0, 0, 0); m_dialogBoxes[3].sView = 1; break; // 2
				    case 51: EnableDialogBox(3, 0, 0, 0); m_dialogBoxes[3].sView = 2; break; // 3
				    case 52: EnableDialogBox(3, 0, 0, 0); m_dialogBoxes[3].sView = 3; break; // 4
				    case 53: EnableDialogBox(3, 0, 0, 0); m_dialogBoxes[3].sView = 4; break; // 5
				    case 54: EnableDialogBox(3, 0, 0, 0); m_dialogBoxes[3].sView = 5; break; // 6
				    case 55: EnableDialogBox(3, 0, 0, 0); m_dialogBoxes[3].sView = 6; break; // 7
				    case 56: EnableDialogBox(3, 0, 0, 0); m_dialogBoxes[3].sView = 7; break; // 8
				    case 57: EnableDialogBox(3, 0, 0, 0); m_dialogBoxes[3].sView = 8; break; // 9
				    }
			    }else if ((m_bInputStatus == false) && (GetAsyncKeyState(VK_MENU)>>15 == false))
			    {	//StartInputString(10, 414, sizeof(m_cChatMsg), m_cChatMsg);
				    StartInputString(10, 530, sizeof(m_cChatMsg), m_cChatMsg); // 800x600 Resolution xRisenx 534 is right / 530 fits?
				    ClearInputString();
			    }
		    }
		    break;
	}
// 	if (m_pInputBuffer == NULL) return;
// 	if(wParam == 8)
// 	{
// 		if(strlen(m_pInputBuffer) > 0)
// 		{
// 			int len = strlen(m_pInputBuffer);
// 
// 			switch (GetCharKind(m_pInputBuffer, len-1))
// 			{
// 			case 1:
// 				m_pInputBuffer[len-1] = NULL;
// 				break;
// 			case 2:
// 			case 3:
// 				m_pInputBuffer[len-2]  = NULL;
// 				m_pInputBuffer[len-1]  = NULL;
// 				break;
// 			}
// 			ZeroMemory(m_cEdit, sizeof(m_cEdit));
// 		}
// 	}
// 	else if ((wParam != 9) && (wParam != 13) && (wParam != 27)) {
// 		int len = strlen(m_pInputBuffer);
// 		if (len >= m_inputMaxLen-1) return;
// 		m_pInputBuffer[len] = wParam & 0xff;
// 		m_pInputBuffer[len+1] = 0;
// 	}
}

void CGame::NotifyMsgHandler(char * pData)
{
	uint32_t * dwp, dwTime, dwTemp;
 uint16_t  * wp, wEventType;
 char  * cp, cTemp[510], cTxt[120], cTempName[21];
 short * sp, sX, sY, sV1 = 0, sV2, sV3, sV4, sV5, sV6, sV7, sV8, sV9;
 int   * ip, i, iV1, iV2, iV3, iV4;
 string str;

	dwTime = unixtime();

	wp   = (uint16_t *)(pData + INDEX2_MSGTYPE);
	wEventType = *wp;

	switch (wEventType)
	{
	case NOTIFY_SOCCERBEGIN:
		m_bSoccerMode = true;
		SetTopMsg("Soccer event has been started.", 10);
		break;

	case NOTIFY_SOCCERVICTORY:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp  = (short *)cp;
		sV1 = *sp;
		cp+=2;
		ShowSoccerVictory(sV1);
		m_SoccerAresdenGoals	= 0;
		m_SoccerElvineGoals		= 0;
		m_bSoccerMode = false;
		SetTopMsg("Soccer event has been closed.", 10);
		break;

	case NOTIFY_SOCCER:
		NotifyMsg_Soccer(pData);
		break;

	case NOTIFY_GUILDCONTRIBUTERSP:
		GuildContributeRsp(pData+6);
		break;

	case NOTIFY_LGNPTS:
		dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
		m_cash = *dwp;
		break;

	case NOTIFY_SLATE_BERSERK:		// reversed by Snoopy: 0x0BED
		AddEventList( MSG_NOTIFY_SLATE_BERSERK, 10 );//"Berserk magic casted!"
		m_bUsingSlate = true;
		break;

	case NOTIFY_LOTERY_LOST:		// reversed by Snoopy: 0x0BEE:
		AddEventList( MSG_NOTIFY_LOTERY_LOST, 10 );//"You draw a blank. Please try again next time.."
		break;

	case NOTIFY_TELEPORT_REJECTED:
		sp = (short *)(pData + INDEX2_MSGTYPE + 2);
		sV1 = *sp; sp++;
		sV2 = *sp; sp++;
		m_pMapData->m_tile[sV1][sV2].m_bIsTeleport = false;
		m_bIsTeleportRequested = false;
		ChangeGameMode(GAMEMODE_ONMAINGAME);
		AddEventList(NOTIFYMSG_TELEPORT_REJECTED, 10);
		break;

	case NOTIFY_0BEF:				// 0x0BEF: // Snoopy: Crash or closes the client? (Calls SE entry !)
		// I'm noot sure at all of this function's result, so let's quit game...

		break;
	case NOTIFY_EVENTILLUSION:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sV1 = *cp;
		cp++;
		if(sV1 == 1) AddEventList(NOTIFYMSG_EVENTILLUSIONON, 10);
		else AddEventList(NOTIFYMSG_EVENTILLUSIONOFF, 10);
		break;

	case NOTIFY_EVENTTP:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);

		sV1 = *cp;
		cp++;
		sV2 = *cp;
		cp++;

		if(sV2)
		{
			// donate event
			if(sV1 == 1) 
				SetTopMsg(NOTIFYMSG_DONATEEVENTTPOPEN, 10);
			else 
				SetTopMsg(NOTIFYMSG_DONATEEVENTTPCLOSE, 10);
		} else {
			if(sV1 == 1) 
				AddEventList(NOTIFYMSG_EVENTTPOPEN, 10);
			else 
				AddEventList(NOTIFYMSG_EVENTTPCLOSE, 10);
		}
		break;

	case NOTIFY_EVENTSPELL:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sV1 = *cp;
		cp++;
		sV2 = *cp;
		cp++;
		sV3 = *cp;
		cp++;
		if(sV2 < MAXMAGICTYPE && sV2 >= 0){
			if(sV1 == 1){
				if(!sV3) m_magicDisabled[sV2] = true;
				wsprintfA(cTemp, NOTIFYMSG_SPELLDISABLED, m_pMagicCfgList[sV2]->m_cName);
			}
			else{
				m_magicDisabled[sV2] = false;
				wsprintfA(cTemp, NOTIFYMSG_SPELLENABLED, m_pMagicCfgList[sV2]->m_cName);
			}
			AddEventList(cTemp, 10 );
		}
		break;

	case NOTIFY_EVENTARMOR:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sV1 = *cp;
		cp++;
		sV2 = *cp;
		cp++;
		if(sV1 == 1){
			if(!sV2) m_armorDisabled = true;
			AddEventList(NOTIFYMSG_ARMORDISABLED, 10);
		}
		else {
			m_armorDisabled = false;
			AddEventList(NOTIFYMSG_ARMORENABLED, 10);
		}
		break;

	case NOTIFY_EVENTSHIELD:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sV1 = *cp;
		cp++;
		sV2 = *cp;
		cp++;
		if(sV1 == 1){
			AddEventList(NOTIFYMSG_SHIELDDISABLED, 10);
			if(!sV2) m_shieldDisabled = true;
		}
		else {
			AddEventList(NOTIFYMSG_SHIELDENABLED, 10);
			m_shieldDisabled = false;
		}
		break;

	case NOTIFY_EVENTCHAT:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sV1 = *cp;
		cp++;
		sV2 = *cp;
		cp++;
		if(sV1 == 1) AddEventList(NOTIFYMSG_CHATDISABLED, 10);
		else AddEventList(NOTIFYMSG_CHATENABLED, 10);
		break;

	case NOTIFY_EVENTPARTY:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sV1 = *cp;
		cp++;
		sV2 = *cp;
		cp++;
		if(sV1 == 1) AddEventList(NOTIFYMSG_PARTYDISABLED, 10);
		else AddEventList(NOTIFYMSG_PARTYENABLED, 10);
		break;

	case NOTIFY_EVENTRESET:
		m_shieldDisabled = false;
		m_armorDisabled = false;
		for(i = 0; i < MAXMAGICTYPE; i++)
			m_magicDisabled[i] = false;
		AddEventList(NOTIFYMSG_EVENTRESET, 10 );
		break;

	case NOTIFY_CRAFTING_SUCCESS:	//reversed by Snoopy: 0x0BF0:
		m_iContribution -= m_iContributionPrice;
		m_iContributionPrice = 0;
		DisableDialogBox(25);
		AddEventList(NOTIFY_MSG_HANDLER42, 10);		// "Item manufacture success!"
		PlaySound('E', 23, 5);
		switch (m_sPlayerType) {
		case 1:
		case 2:
		case 3:
			PlaySound('C', 21, 0);
			break;
		case 4:
		case 5:
		case 6:
			PlaySound('C', 22, 0);
			break;
		}
		break;

	case NOTIFY_CRAFTING_FAIL:
		AddEventList(MSG_NOTIFY_CRAFTING_FAILED, 10);		// "Crafting failed"
		PlaySound('E', 24, 5);
		break;

	case NOTIFY_NOMATCHINGCRAFTING:
		AddEventList(MSG_NOTIFY_CRAFTING_NO_PART, 10);		// "There is not enough material"
		PlaySound('E', 24, 5);
		break;

	case NOTIFY_NO_CRAFT_CONTRIB:
		AddEventList(MSG_NOTIFY_CRAFTING_NO_CONTRIB, 10);	// "There is not enough Contribution Point"
		PlaySound('E', 24, 5);
		break;

	case NOTIFY_ANGELIC_STATS:		// reversed by Snoopy: 0x0BF2
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		ip = (int *)cp;
		m_angelStat[STAT_STR] = *ip;  // m_angelStat[STAT_STR]
		cp +=4;
		ip = (int *)cp;
		m_angelStat[STAT_INT] = *ip;  // m_angelStat[STAT_INT]
		cp +=4;
		ip = (int *)cp;
		m_angelStat[STAT_DEX] = *ip;  // m_angelStat[STAT_DEX]
		cp +=4;
		ip = (int *)cp;
		m_angelStat[STAT_MAG] = *ip;  // m_angelStat[STAT_MAG]
		break;

	case NOTIFY_ITEM_CANT_RELEASE:	// reversed by Snoopy: 0x0BF3
		AddEventList(MSG_NOTIFY_NOT_RELEASED , 10 );//"Item cannot be released"
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		ItemEquipHandler(*cp);
		break;

	case NOTIFY_ANGEL_FAILED:		// reversed by Snoopy: 0x0BF4
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		ip = (int *)cp;
		iV1 = *ip; // Error reason
		switch (iV1) {
		case 1: // "BFB9BBF3C4A120BECAC0BA20B9F6B1D7C0D4B4CFB4D92E20A4D02E2EA4D0" (Stolen bytes ?)
			AddEventList(MSG_NOTIFY_ANGEL_FAILED , 10 ); //"Impossible to get a Tutelary Angel." // Invented by Snoopy.
			break;
		case 2: //
			AddEventList(MSG_NOTIFY_ANGEL_MAJESTIC , 10 );//"You need additional Majesty Points."
			break;
		case 3: //
			AddEventList(MSG_NOTIFY_ANGEL_LOW_LVL , 10 ); //"Only Majesty characters can receive Tutelary Angel"
			break;
		}
		break;

	case NOTIFY_ANGEL_RECEIVED:		// reversed by Snoopy: 0x0BF5:
		AddEventList( "Gold has been decreased by 100.000", 10 );
		AddEventList( "Majestic points has been decreased by 100 points.", 10 );
		AddEventList(MSG_NOTIFY_ANGEL_RECEIVED, 10 );// "You have received the Tutelary Angel."
		break;

	case NOTIFY_SPELL_SKILL:		// reversed by Snoopy: 0x0BF6
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		for (i = 0; i < MAXMAGICTYPE; i++)
		{	m_cMagicMastery[i] = *cp;
			cp++;
		}
		for (i = 0; i < MAXSKILLTYPE; i++)
		{	m_cSkillMastery[i] = (unsigned char)*cp;
			if (m_pSkillCfgList[i] != 0)
				m_pSkillCfgList[i]->m_iLevel = (int)*cp;
			cp++;
		}
		break;

	case NOTIFY_APOCGATESTARTMSG: // Snoopy 0x0BD2
		SetTopMsg("The portal to the Apocalypse is opened.", 10);
		break;

	case NOTIFY_APOCGATEENDMSG: // Snoopy 0x0BD3
		SetTopMsg("The portal to the Apocalypse is closed.", 10);
		break;

	case NOTIFY_APOCGATEOPEN: // Snoopy ;  Case BD4 of switch 00454077
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		ip  = (int *)cp;
		m_iGatePositX = *ip;
		cp += 4;
		ip  = (int *)cp;
		m_iGatePositY = *ip;
		cp += 4;
		ZeroMemory(m_cGateMapName, sizeof(m_cGateMapName));
		memcpy(m_cGateMapName, cp, 10);
		cp += 10;
		break;

	case NOTIFY_QUESTCOUNTER: // Snoopy;  Case BE2 of switch 00454077
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		ip  = (int *)cp;
		m_stQuest.sCurrentCount = (short)*ip;
		cp += 4;
		break;
	//	// Defense Display in Action Bar xRisenx
	case NOTIFY_REQDEF: // Notify Defense xRisenx
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		ip  = (int *)cp;
		m_iDefenseRatio = *ip;
		cp += 4;
		break;

	case NOTIFY_MONSTERCOUNT: // Snoopy ;  Case BE3 of switch 00454077
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp  = (short *)cp;

		sV1 = *sp;
		sp++;
		sV2 = *sp;
		sp++;
		sV3 = *sp;

		cp+=6;
		wsprintfA(cTxt,"Rest Monster :%d, Pos: %d, %d", sV1, sV2, sV3) ;
		AddEventList(cTxt, 10);
		break;

	case NOTIFY_APOCGATECLOSE: // Snoopy ;  Case BD5 of switch 00454077
		m_iGatePositX = m_iGatePositY = -1;
		ZeroMemory(m_cGateMapName, sizeof(m_cGateMapName));
		break;

	case NOTIFY_APOCFORCERECALLPLAYERS: // Snoopy ;  Case BD7 of switch 00454077
		AddEventList( "You are recalled by force, because the Apocalypse is started.", 10 );
		break;

	case NOTIFY_ABADDONKILLED: // Snoopy ;  Case BD6 of switch 00454077
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		ZeroMemory(cTxt, sizeof(cTxt));
		memcpy(cTxt, cp, 10);
		cp += 10;
		wsprintfA(G_cTxt, "Abaddon is destroyed by %s", cTxt);
		AddEventList(G_cTxt, 10);
		break;

	case NOTIFY_RESURRECTPLAYER: // Case BE9 of switch 00454077
		EnableDialogBox(50, 0, 0, 0);
		break;

	case NOTIFY_HELDENIANEND:
		m_bIsHeldenianMode = false;
		SetTopMsg("Heldenian holy war has been closed.", 10);
		break;

	case NOTIFY_HELDENIANSTART:
		m_bIsHeldenianMode = true;
		m_bIsHeldenianMap  = ((m_cMapIndex == 35) || (m_cMapIndex == 36) || (m_cMapIndex == 37));
		SetTopMsg("Heldenian has been started! Teleport to Heldenian field is available", 10);
		break;

	case NOTIFY_HELDENIANVICTORY:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp  = (short *)cp;
		sV1 = *sp;
		cp+=2;
		ShowHeldenianVictory(sV1);
		m_iHeldenianAresdenLeftTower	= 0;
		m_iHeldenianElvineLeftTower		= 0;
		m_iHeldenianAresdenFlags		= 0;
		m_iHeldenianElvineFlags			= 0;
		m_iHeldenianAresdenDead			= 0;
		m_iHeldenianElvineDead			= 0;
		m_iHeldenianAresdenKill			= 0;
		break;

	case NOTIFY_HELDENIANCOUNT:
		NotifyMsg_Heldenian(pData);
		break;


	// Slates - Diuuude
	case NOTIFY_SLATE_CREATESUCCESS:	// 0x0BC1
		AddEventList( MSG_NOTIFY_SLATE_CREATESUCCESS, 10 );
		break;
	case NOTIFY_SLATE_CREATEFAIL:		// 0x0BC2
		AddEventList( MSG_NOTIFY_SLATE_CREATEFAIL, 10 );
		break;
	case NOTIFY_SLATE_INVINCIBLE:		// 0x0BD8
		AddEventList( MSG_NOTIFY_SLATE_INVINCIBLE, 10 );
		m_bUsingSlate = true;
		break;
	case NOTIFY_SLATE_MANA:				// 0x0BD9
		AddEventList( MSG_NOTIFY_SLATE_MANA, 10 );
		m_bUsingSlate = true;
		break;
	case NOTIFY_SLATE_EXP:				// 0x0BE0
		AddEventList( MSG_NOTIFY_SLATE_EXP, 10 );
		m_bUsingSlate = true;
		break;
	case NOTIFY_SLATE_STATUS:			// 0x0BE1
		AddEventList( MSG_NOTIFY_SLATECLEAR, 10 ); // "The effect of the prophecy-slate is disappeared."
		m_bUsingSlate = false;
		break;

	// MJ Stats Change - Diuuude: Erreur, ici il s'agit de sorts et skills, le serveur comme la v351 sont aussi bugu�s !
	case NOTIFY_STATECHANGE_SUCCESS:	// 0x0BB5
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		for (i = 0; i < MAXMAGICTYPE; i++)
		{	m_cMagicMastery[i] = *cp;
			cp++;
		}
		for (i = 0; i < MAXSKILLTYPE; i++)
		{	m_cSkillMastery[i] = (unsigned char)*cp;
			if (m_pSkillCfgList[i] != 0)
				m_pSkillCfgList[i]->m_iLevel = (int)*cp;
			//else m_pSkillCfgList[i]->m_iLevel = 0;
			cp++;
		}
		// MJ Stats Change - Diuuude
		m_stat[STAT_STR] += m_luStat[STAT_STR];
		m_stat[STAT_VIT] += m_luStat[STAT_VIT];
		m_stat[STAT_DEX] += m_luStat[STAT_DEX];
		m_stat[STAT_INT] += m_luStat[STAT_INT];
		m_stat[STAT_MAG] += m_luStat[STAT_MAG];
		m_stat[STAT_CHR] += m_luStat[STAT_CHR];
		m_iLU_Point = m_iLevel*7 - (
		//m_iLU_Point = m_iLevel*TOTALLEVELUPPOINT - ( // Stats System xRisenx
			(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
			- 70)
			- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			// Stats System xRisenx
			//- TOTALLEVELUPPOINT + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
		m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;
		AddEventList( "Your stat has been changed.", 10 ); // "Your stat has been changed."
		break;

	case NOTIFY_LEVELUP: // 0x0B16
		NotifyMsg_LevelUp(pData);
		break;

	case NOTIFY_STATECHANGE_FAILED:		// 0x0BB6
		m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;
		m_iLU_Point = m_iLevel*7 - (
		//m_iLU_Point = m_iLevel*TOTALLEVELUPPOINT - ( // Stats System xRisenx
			(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
			- 70)
			- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			// Stats System xRisenx
			//- TOTALLEVELUPPOINT + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
		AddEventList( "Your stat has not been changed.", 10 );
		break;

	case NOTIFY_SETTING_FAILED: // 0x0BB4 -  Case BB4 of switch 00454077
		AddEventList( "Your stat has not been changed.", 10 );
		m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;
		m_iLU_Point = m_iLevel*7 - (
		//m_iLU_Point = m_iLevel*TOTALLEVELUPPOINT - ( // Stats System xRisenx
			(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
			- 70)
			- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			// Stats System xRisenx
			//- TOTALLEVELUPPOINT + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
		break;

	// CLEROTH - LU
	case NOTIFY_SETTING_SUCCESS: // 0x0BB3 - envoie le niv et les stats
		NotifyMsg_SettingSuccess(pData);
		break;

	case NOTIFY_AGRICULTURENOAREA:		// 0x0BB2
		AddEventList( MSG_NOTIFY_AGRICULTURENOAREA, 10 );
		break;
	case NOTIFY_AGRICULTURESKILLLIMIT:	// 0x0BB1
		AddEventList( MSG_NOTIFY_AGRICULTURESKILLLIMIT, 10 );
		break;

	case NOTIFY_NOMOREAGRICULTURE:		// 0x0BB0
		AddEventList( MSG_NOTIFY_NOMOREAGRICULTURE, 10 );
		break;
	case NOTIFY_MONSTEREVENT_POSITION:				// 0x0BAA
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		m_sMonsterID = (short)(*cp);
		cp++;
		sp  = (short *)cp;
		m_sEventX = *sp;
		cp+=2;
		sp  = (short *)cp;
		m_sEventY = *sp;
		cp+=2;
		m_dwMonsterEventTime = dwTime;
		break;

	case NOTIFY_RESPONSE_HUNTMODE:			// 0x0BA9
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		memcpy(m_cLocation, cp, 10);
		cp += 10;

		if (memcmp(m_cLocation, "are", 3) == 0)
			m_side = ARESDEN;
		else if (memcmp(m_cLocation, "elv", 3) == 0)
			m_side = ELVINE;
		/*else if (memcmp(m_cLocation, "ist", 3) == 0)
			m_side = ISTRIA;*/
		else
			m_side = NEUTRAL;

		AddEventList( MSG_GAMEMODE_CHANGED, 10 );
		break;

	case NOTIFY_REQGUILDNAMEANSWER:	 //   0x0BA6
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp  = (short *)cp;
		sV1 = *sp;
		cp += 2;
		sp  = (short *)cp;
		sV2 = *sp;
		cp += 2;
		ZeroMemory(cTemp, sizeof(cTemp));
		memcpy(cTemp, cp, 20);
		cp += 20;

		ZeroMemory( m_stGuildName[sV2].cGuildName, sizeof(m_stGuildName[sV2].cGuildName) );
		strcpy(m_stGuildName[sV2].cGuildName, cTemp);
		m_stGuildName[sV2].iGuildRank = sV1;
		for (i = 0; i < 20; i++) if (m_stGuildName[sV2].cGuildName[i] == '_') m_stGuildName[sV2].cGuildName[i] = ' ';
		break;

	case NOTIFY_FORCERECALLTIME: // 0x0BA7
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp  = (short *)cp;
		sV1 = *sp;
		cp += 2;
		if ( (int)(sV1/20) > 0)
			wsprintfA(G_cTxt,NOTIFY_MSG_FORCERECALLTIME1,(int) (sV1/20)) ;
		else
			wsprintfA(G_cTxt,NOTIFY_MSG_FORCERECALLTIME2) ;
		AddEventList(G_cTxt, 10);
		break;

	case NOTIFY_GIZONITEMUPGRADELEFT: // 0x0BA4// Item upgrade is possible.
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp  = (short *)cp;
		sV1 = *sp;
		cp += 2;
		m_iGizonItemUpgradeLeft = sV1;
		dwp = (uint32_t *)cp;
		switch (*dwp) {
		case 1: //
			AddEventList(NOTIFY_MSG_HANDLER_GIZONITEMUPGRADELEFT1, 10);
			break;
		}
		//wsprintfA(G_cTxt,"majesty: %d", m_iGizonItemUpgradeLeft);
		//DebugLog(G_cTxt);
		cp += 4;
		break;

	case NOTIFY_GIZONEITEMCHANGE: // 0x0BA5
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp  = (short *)cp;
		sV1 = *sp;
		cp += 2;
		m_pItemList[sV1]->m_cItemType = *cp ;
		cp++ ;
		wp  = (uint16_t *)cp;
		m_pItemList[sV1]->m_wCurLifeSpan = *wp;
		cp += 2;
		sp  = (short *)cp;
		m_pItemList[sV1]->m_sSprite = *sp;
		cp += 2;
		sp  = (short *)cp;
		m_pItemList[sV1]->m_sSpriteFrame = *sp;
		cp += 2;
		dwp = (uint32_t *) cp ;
		m_pItemList[sV1]->m_ItemColor = *dwp ;
		cp += 4 ;
		m_pItemList[sV1]->m_sItemSpecEffectValue2 = *cp ;
		cp++ ;
		dwp = (uint32_t *) cp ;
		m_pItemList[sV1]->m_dwAttribute =  *dwp ;
		cp +=4 ;
		ZeroMemory( m_pItemList[sV1]->m_cName, sizeof(m_pItemList[sV1]->m_cName) );
		memcpy(m_pItemList[sV1]->m_cName,cp,20) ;
		cp += 20 ;
		if (m_bIsDialogEnabled[34] == true)
		{	m_dialogBoxes[34].SetMode(3); // succes
		}
		PlaySound('E', 23, 5);
		switch (m_sPlayerType) {
		case 1:
		case 2:
		case 3:
			PlaySound('C', 21, 0);
			break;

		case 4:
		case 5:
		case 6:
			PlaySound('C', 22, 0);
			break;
		}
		break;

	case NOTIFY_ITEMATTRIBUTECHANGE: // 0x0BA3
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp  = (short *)cp;
		sV1 = *sp;
		cp += 2;
		dwTemp = m_pItemList[sV1]->m_dwAttribute;
		dwp  = (uint32_t *)cp;
		m_pItemList[sV1]->m_dwAttribute = *dwp;
		cp += 4;
		dwp  = (uint32_t *)cp;
		if (*dwp != 0) m_pItemList[sV1]->m_sItemSpecEffectValue1 = (short)*dwp;
		cp += 4;
		dwp  = (uint32_t *)cp;
		if (*dwp != 0) m_pItemList[sV1]->m_sItemSpecEffectValue2 = (short)*dwp;
		cp += 4;
		if (dwTemp == m_pItemList[sV1]->m_dwAttribute)
		{	if (m_bIsDialogEnabled[34] == true)
			{	m_dialogBoxes[34].SetMode(4);// Failed
			}
			PlaySound('E', 24, 5);
		}else
		{	if (m_bIsDialogEnabled[34] == true)
			{	m_dialogBoxes[34].SetMode(3); // Success
			}
			PlaySound('E', 23, 5);
			switch (m_sPlayerType) {
			case 1:
			case 2:
			case 3:
				PlaySound('C', 21, 0);
				break;
			case 4:
			case 5:
			case 6:
				PlaySound('C', 22, 0);
				break;
		}	}
		break;

	case NOTIFY_ITEMUPGRADEFAIL:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp  = (short *)cp;
		sV1 = *sp;
		cp += 2;
		if (m_bIsDialogEnabled[34] == false) return ;
		PlaySound('E', 24, 5);
		switch(sV1){
		case 1:
			m_dialogBoxes[34].SetMode(8) ; // Failed
			break ;
		case 2:
			m_dialogBoxes[34].SetMode(9) ; // Failed
			break ;
		case 3:
			m_dialogBoxes[34].SetMode(10) ; // Failed
			break ;
		}
		break;

	case NOTIFY_PARTY:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp  = (short *)cp;
		sV1 = *sp;
		cp += 2;
		sp  = (short *)cp;
		sV2 = *sp;
		cp += 2;
		sp  = (short *)cp;
		sV3 = *sp;
		cp += 2;
		switch (sV1) {
		case 1: //Create Party
			switch (sV2) {
			case 0: //Create party failure
				EnableDialogBox(32, 0, 0, 0);
				m_dialogBoxes[32].SetMode(9);
				break;

			case 1: //Create party success
				m_iPartyStatus = 1;
				EnableDialogBox(32, 0, 0, 0);
				m_dialogBoxes[32].SetMode(8);
				ClearPartyMembers();
				break;
			}
			break;

		case 2: //Delete party
			m_iPartyStatus = 0;
			EnableDialogBox(32, 0, 0, 0);
			m_dialogBoxes[32].SetMode(10);
			ClearPartyMembers();
			break;

		case 4: //Join party
			ZeroMemory(cTxt, sizeof(cTxt));
			memcpy(cTxt, cp, 10);
			cp += 10;

			switch (sV2) {
			case 0: //Join party failure
				EnableDialogBox(32, 0, 0, 0);
				m_dialogBoxes[32].SetMode(9);
				break;

			case 1: //Join party success
				if (strcmp(cTxt, m_cPlayerName) == 0) {
					m_iPartyStatus = 2;
					EnableDialogBox(32, 0, 0, 0);
					m_dialogBoxes[32].SetMode(8);
				}
				else {
					wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER1, cTxt);
					AddEventList(G_cTxt, 10);
				}

				m_stPartyMember.push_back(new partyMember(cTxt));
				break;
			}
			break;

		case 5: //Get list of names
			m_iTotalPartyMember = sV3;
			for (i = 0; i < sV3; i++) {
				ZeroMemory(cTxt, sizeof(cTxt));
				memcpy(cTxt, cp, 10);
				for (sV6 = 0, sV5 = false; sV6 < m_stPartyMember.size(); sV6++)
					if(m_stPartyMember[sV6]->cName.compare(cTxt) == 0){ sV5 = true; break;}

				if(sV5 == false){
					m_stPartyMember.push_back(new partyMember(cTxt));
				}
				cp+= 11;
			}
			break;

		default:
			sp  = (short *)cp;
			sV4 = *sp;
			cp += 2;
			break;

		case 6: //Dismissed party member
			ZeroMemory(cTxt, sizeof(cTxt));
			memcpy(cTxt, cp, 10);
			cp += 10;

			switch (sV2) {
			case 0: //
				EnableDialogBox(32, 0, 0, 0);
				m_dialogBoxes[32].SetMode(7);
				break;

			case 1: //Party member successfully dismissed
				if (strcmp(cTxt, m_cPlayerName) == 0) {
					m_iPartyStatus = 0;
					EnableDialogBox(32, 0, 0, 0);
					m_dialogBoxes[32].SetMode(6);
				}
				else {
					wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER2 , cTxt);
					AddEventList(G_cTxt, 10);
				}
				for (std::vector<partyMember*>::iterator it = m_stPartyMember.begin(); it < m_stPartyMember.end(); ++it)
				{
					if ((*it)->cName.compare(cTxt) == 0) {
						delete *it;
						m_stPartyMember.erase(it);
						break;
					}
				}
				break;
			}
			break;

		case 7: //Party join rejected
			EnableDialogBox(32, 0, 0, 0);
			m_dialogBoxes[32].SetMode(9);
			break;

		case 8: //You left the party
			m_iPartyStatus = 0;
			m_iTotalPartyMember = 0;
			ClearPartyMembers();
			break;
		}
		break;

	case NOTIFY_PARTY_COORDS:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		ZeroMemory(cTxt, sizeof(cTxt));
		memcpy(cTxt, cp, 10);
		cp += 10;

		sp  = (short *)cp;
		sV4 = *sp;
		cp += 2;

		sp  = (short *)cp;
		sV5 = *sp;  //
		cp += 2;
		for (i = 0; i < m_stPartyMember.size(); i++){
			if(m_stPartyMember[i]->cName.compare(cTxt) ==0){
				m_stPartyMember[i]->sX = sV4;
				m_stPartyMember[i]->sY = sV5;
			}
		}
		break;

	case NOTIFY_CANNOTCONSTRUCT:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);

		sp  = (short *)cp;
		sV1 = *sp;
		cp += 2;

		CannotConstruct(sV1);
		PlaySound('E', 25, 0, 0);
		break;

	case NOTIFY_TCLOC:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);

		sp  = (short *)cp;
		m_iTeleportLocX = *sp;
		cp += 2;

		sp  = (short *)cp;
		m_iTeleportLocY = *sp;
		cp += 2;

		ZeroMemory(m_cTeleportMapName, sizeof(m_cTeleportMapName));
		memcpy(m_cTeleportMapName, cp, 10);
		cp += 10;

		sp  = (short *)cp;
		m_iConstructLocX = *sp;
		cp += 2;

		sp  = (short *)cp;
		m_iConstructLocY = *sp;
		cp += 2;

		ZeroMemory(m_cConstructMapName, sizeof(m_cConstructMapName));
		memcpy(m_cConstructMapName, cp, 10);
		cp += 10;
		break;

	case NOTIFY_CONSTRUCTIONPOINT:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);

		sp  = (short *)cp;
		sV1 = *sp;
		cp += 2;

		sp  = (short *)cp;
		sV2 = *sp;
		cp += 2;

		sp  = (short *)cp;
		sV3 = *sp;
		cp += 2;

		if (sV3 == 0) {
			if ((sV1 > m_iConstructionPoint) && (sV2 > m_iWarContribution)) {
				wsprintfA(G_cTxt, "%s +%d, %s +%d", CRUSADE_MESSAGE13, (sV1 - m_iConstructionPoint), CRUSADE_MESSAGE21, (sV2 - m_iWarContribution));
				SetTopMsg(G_cTxt, 5);
				PlaySound('E', 23, 0, 0);
			}

			if ((sV1 > m_iConstructionPoint) && (sV2 == m_iWarContribution)) {
				if (m_iCrusadeDuty == 3) {
					wsprintfA(G_cTxt, "%s +%d", CRUSADE_MESSAGE13, sV1 - m_iConstructionPoint);
					SetTopMsg(G_cTxt, 5);
					PlaySound('E', 23, 0, 0);
				}
			}

			if ((sV1 == m_iConstructionPoint) && (sV2 > m_iWarContribution)) {
				wsprintfA(G_cTxt, "%s +%d", CRUSADE_MESSAGE21, sV2 - m_iWarContribution);
				SetTopMsg(G_cTxt, 5);
				PlaySound('E', 23, 0, 0);
			}

			if (sV1 < m_iConstructionPoint) {
				if (m_iCrusadeDuty == 3) {
					wsprintfA(G_cTxt, "%s -%d", CRUSADE_MESSAGE13, m_iConstructionPoint - sV1);
					SetTopMsg(G_cTxt, 5);
					PlaySound('E', 25, 0, 0);
				}
			}

			if (sV2 < m_iWarContribution) {
				wsprintfA(G_cTxt, "%s -%d", CRUSADE_MESSAGE21, m_iWarContribution - sV2);
				SetTopMsg(G_cTxt, 5);
				PlaySound('E', 24, 0, 0);
			}
		}

		m_iConstructionPoint = sV1;
		m_iWarContribution   = sV2;
		break;

	case NOTIFY_NOMORECRUSADESTRUCTURE:
		SetTopMsg(CRUSADE_MESSAGE12, 5);
		PlaySound('E', 25, 0, 0);
		break;

	case NOTIFY_GRANDMAGICRESULT:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);

		wp  = (uint16_t *)cp;
		sV1 = *wp;
		cp += 2;

		wp  = (uint16_t *)cp;
		sV2 = *wp;
		cp += 2;

		wp  = (uint16_t *)cp;
		sV3 = *wp;
		cp += 2;

		ZeroMemory(cTxt, sizeof(cTxt));
		memcpy(cTxt, cp, 10);
		cp += 10;

		wp  = (uint16_t *)cp;
		sV4 = *wp;
		cp += 2;

		wp  = (uint16_t *)cp;
		sV5 = *wp;  //
		cp += 2;

		if (sV5  > 0 ) {
			wp  = (uint16_t *)cp;
			sV6 = *wp;
			cp += 2;
			sV5-- ;
		}
		else sV6 = 0 ;

		if ( sV5  > 0 ) {
			wp  = (uint16_t *)cp;
			sV7 = *wp;
			cp += 2;
			sV5-- ;
		}
		else sV7 = 0 ;

		if ( sV5  > 0 ) {
			wp  = (uint16_t *)cp;
			sV8 = *wp;
			cp += 2;
			sV5-- ;
		}
		else sV8 = 0 ;

		if ( sV5  > 0 ) {
			wp  = (uint16_t *)cp;
			sV9 = *wp;
			cp += 2;
			sV5-- ;
		}
		else sV9 = 0 ;

		GrandMagicResult(cTxt, sV1, sV2, sV3, sV4, sV6, sV7, sV8, sV9);
		break;

	case NOTIFY_METEORSTRIKECOMING:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		wp  = (uint16_t *)cp;
		sV1 = *wp;
		cp += 2;
		MeteorStrikeComing(sV1);
		PlaySound('E', 25, 0, 0);
		break;

	case NOTIFY_METEORSTRIKEHIT:
		SetTopMsg(CRUSADE_MESSAGE17, 5);
		//StartMeteorStrikeEffect
		//for( i=0 ; i<36 ; i++ ) bAddNewEffect(60, m_sViewPointX +(rand() % 640), m_sViewPointY +(rand() % 480), NULL, NULL, -(rand() % 80));
		for( i=0 ; i<36 ; i++ ) bAddNewEffect(60, m_sViewPointX +(rand() % GetWidth()), m_sViewPointY +(rand() % GetHeight()), 0, 0, -(rand() % 80)); // 800x600 Resolution xRisenx
		break;

	case NOTIFY_MAPSTATUSNEXT:
		AddMapStatusInfo(pData, false);
		break;

	case NOTIFY_MAPSTATUSLAST:
		AddMapStatusInfo(pData, true);
		break;

	case NOTIFY_LOCKEDMAP:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp = (short *)cp;
		sV1 = *sp;
		cp += 2;

		ZeroMemory(cTemp, sizeof(cTemp));
		ZeroMemory(cTxt, sizeof(cTxt));
		memcpy(cTxt, cp, 10);
		cp += 10;

		GetOfficialMapName(cTxt, cTemp);
		wsprintfA( G_cTxt, NOTIFY_MSG_HANDLER3, sV1, cTemp );
		SetTopMsg(G_cTxt, 10);
		PlaySound('E', 25, 0, 0);
		break;

	case NOTIFY_CRUSADE: // Crusade msg
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		ip = (int *)cp;
		iV1 = *ip; // Crusademode
		cp += 4;
		ip = (int *)cp;
		iV2 = *ip; // crusade duty
		cp += 4;
		ip = (int *)cp;
		iV3 = *ip;
		cp += 4;
		ip = (int *)cp;
		iV4 = *ip;
		cp += 4;
		if(!m_bIsCrusadeMode)
		{	
			if (iV1 != 0) // begin crusade
			{	
				m_bIsCrusadeMode = true;
				m_iCrusadeDuty = iV2;

				if( (m_iCrusadeDuty != 3) && m_side == NEUTRAL )
					_RequestMapStatus("2ndmiddle", 3);

				if (m_iCrusadeDuty != 0)
					 EnableDialogBox(33, 2, iV2);
				else 
					EnableDialogBox(33, 1);

				if( m_side == NEUTRAL ) EnableDialogBox(18, 800, 0, 0);
				else if( m_side == ARESDEN ) EnableDialogBox(18, 801, 0, 0);
				else if( m_side == ELVINE ) EnableDialogBox(18, 802, 0, 0);
				if (m_side == NEUTRAL) SetTopMsg(NOTIFY_MSG_CRUSADESTART_NONE, 10);
				else SetTopMsg(CRUSADE_MESSAGE9, 10);
				PlaySound('E', 25, 0, 0);
			}
			if (iV3 != 0) // Crusade finished, show XP result screen
			{	
				CrusadeContributionResult(iV3);
			}
			if (iV4 == -1) // The crusade you played in was finished.
			{	
				CrusadeContributionResult(0); // You connect in this crusade, but did not connect after previous one => no XP....
			}
		}else
		{	
			if (iV1 == 0) // crusade finished show result (1st result: winner)
			{	
				m_bIsCrusadeMode = false;
				m_iCrusadeDuty   = 0;
				CrusadeWarResult(iV4);
				SetTopMsg(CRUSADE_MESSAGE57, 8);
			}else
			{	
				if(iV2 == -1)
				{
					AddEventList(NOTIFY_MSG_HANDLER89, CHAT_GM);
					return;
				} 
				else if (m_iCrusadeDuty != iV2)
				{	
					m_iCrusadeDuty = iV2;
					EnableDialogBox(33, 2, iV2, 0);
					PlaySound('E', 25, 0, 0);
				}	
			}
			if (iV4 == -1)
			{	
				CrusadeContributionResult(0); // You connect in this crusade, but did not connect after previous one => no XP....
			}	
		}
		break;

	case NOTIFY_SPECIALABILITYSTATUS:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp = (short *)cp;
		sV1 = *sp;
		cp += 2;
		sp = (short *)cp;
		sV2 = *sp;
		cp += 2;
		sp = (short *)cp;
		sV3 = *sp;
		cp += 2;
		if (sV1 == 1) // Use SA
		{	PlaySound('E', 35, 0);
			AddEventList(NOTIFY_MSG_HANDLER4, 10); // "Use special ability!"
			switch (sV2) {
			case 1: wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER5,sV3); break;//"You are untouchable for %d seconds!"
			case 2: wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER6, sV3); break;//"
			case 3: wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER7, sV3); break;//"
			case 4: wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER8, sV3); break;//"
			case 5: wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER9, sV3); break;//"
			case 50:wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER10, sV3); break;//"
			case 51:wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER11, sV3); break;//"
			case 52:wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER12, sV3); break;//"
			case 55: // Spell effect
				if (sV3 >90)
					wsprintfA(G_cTxt, "You cast a powerful incantation, you can't use it again before %d minutes.", sV3/60);
				else
					wsprintfA(G_cTxt, "You cast a powerful incantation, you can't use it again before %d seconds.", sV3);
				break;
			}
			AddEventList(G_cTxt, 10);
		}else if (sV1 == 2) // Finished using
		{	if (m_iSpecialAbilityType != (int)sV2)
			{	PlaySound('E', 34, 0);
				AddEventList(NOTIFY_MSG_HANDLER13, 10);//"Special ability has been set!"
				if (sV3 >= 60)
				{	switch (sV2) {
					case 1: wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER14, sV3/60); AddEventList(G_cTxt, 10); break;//"Ability that decreases enemy's HP by 50%: Can use after %dMin"
					case 2: wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER15, sV3/60); AddEventList(G_cTxt, 10); break;//"
					case 3: wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER16, sV3/60); AddEventList(G_cTxt, 10); break;//"
					case 4: wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER17, sV3/60); AddEventList(G_cTxt, 10); break;//"
					case 5: wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER18, sV3/60); AddEventList(G_cTxt, 10); break;//"
					case 50:wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER19, sV3/60); AddEventList(G_cTxt, 10); break;//"
					case 51:wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER20, sV3/60); AddEventList(G_cTxt, 10); break;//"
					case 52:wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER21, sV3/60); AddEventList(G_cTxt, 10); break;//"
					}
				}else
				{	switch (sV2) {
					case 1: wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER22, sV3); AddEventList(G_cTxt, 10); break;//"
					case 2: wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER23, sV3); AddEventList(G_cTxt, 10); break;//"
					case 3: wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER24, sV3); AddEventList(G_cTxt, 10); break;//"
					case 4: wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER25, sV3); AddEventList(G_cTxt, 10); break;//"
					case 5: wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER26, sV3); AddEventList(G_cTxt, 10); break;//"
					case 50:wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER27, sV3); AddEventList(G_cTxt, 10); break;//"
					case 51:wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER28, sV3); AddEventList(G_cTxt, 10); break;//"
					case 52:wsprintfA(G_cTxt,  NOTIFY_MSG_HANDLER29, sV3); AddEventList(G_cTxt, 10); break;//""Ability that makes character untouchable: Can use after %dSec"
			}	}	}
			m_iSpecialAbilityType = (int)sV2;
			m_dwSpecialAbilitySettingTime = dwTime;
			m_iSpecialAbilityTimeLeftSec  = (int)sV3;
		}else if (sV1 == 3)  // End of using time
		{	m_bIsSpecialAbilityEnabled = false;
			m_dwSpecialAbilitySettingTime = dwTime;
			if (sV3 == 0)
			{	m_iSpecialAbilityTimeLeftSec  = 1200;
				AddEventList(NOTIFY_MSG_HANDLER30, 10);//"Special ability has run out! Will be available in 20 minutes."
			}else
			{	m_iSpecialAbilityTimeLeftSec  = (int)sV3;
				if (sV3 >90)
					 wsprintfA(G_cTxt, "Special ability has run out! Will be available in %d minutes." , sV3/60);
				else wsprintfA(G_cTxt, "Special ability has run out! Will be available in %d seconds." , sV3);
				AddEventList(G_cTxt, 10);
			}
		}else if (sV1 == 4) // Unequiped the SA item
		{	AddEventList(NOTIFY_MSG_HANDLER31, 10);//"Special ability has been released."
			m_iSpecialAbilityType = 0;
		}else if (sV1 == 5) // Angel
		{	PlaySound('E', 52, 0); // Angel
		}
		break;

	case NOTIFY_SPECIALABILITYENABLED:
		if (m_bIsSpecialAbilityEnabled == false) {
			PlaySound('E', 30, 5);
			AddEventList(NOTIFY_MSG_HANDLER32, 10);//"
		}
		m_bIsSpecialAbilityEnabled = true;
		break;

	case NOTIFY_ENERGYSPHEREGOALIN:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp = (short *)cp;
		sV1 = *sp;
		cp += 2;
		sp = (short *)cp;
		sV2 = *sp;
		cp += 2;
		sp = (short *)cp;
		sV3 = *sp;
		cp += 2;
		ZeroMemory(cTxt, sizeof(cTxt));
		memcpy(cTxt, cp, 20);

		if (sV2 == sV3)
		{	PlaySound('E', 24, 0);
			if (strcmp(cTxt, m_cPlayerName) == 0)
			{	AddEventList(NOTIFY_MSG_HANDLER33, 10);//You pushed energy sphere to enemy's energy portal! Contribution point will be decreased by 10 points."
				m_iContribution += sV1; // fixed, server must match...
				m_iContributionPrice = 0;
				if (m_iContribution < 0) m_iContribution = 0;
			}
			else {
				ZeroMemory(G_cTxt, sizeof(G_cTxt));
				if( m_side == ARESDEN ) wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER34, cTxt);//"%s(Aresden) pushed energy sphere to enemy's portal!!..."
				else if (m_side == ELVINE) wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER34_ELV, cTxt);//"%s(Elvine) pushed energy sphere to enemy's portal!!..."
				AddEventList(G_cTxt, 10);
			}
		}else
		{	PlaySound('E', 23, 0);
			if (strcmp(cTxt, m_cPlayerName) == 0)
			{	switch (m_sPlayerType) {
				case 1:
				case 2:
				case 3:	PlaySound('C', 21, 0); break;
				case 4:
				case 5:
				case 6:	PlaySound('C', 22, 0); break;
				}
				AddEventList(NOTIFY_MSG_HANDLER35, 10);//"Congulaturations! You brought energy sphere to energy portal and earned experience and prize gold!"

				m_iContribution += 5;
				if (m_iContribution < 0) m_iContribution = 0;
			}else
			{	ZeroMemory(G_cTxt, sizeof(G_cTxt));
				if (sV3 == 1)
				{	wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER36, cTxt);//"Elvine %s : Goal in!"
					AddEventList(G_cTxt, 10);
				}else if (sV3 == 2)
				{	wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER37, cTxt);//"Aresden %s : Goal in!"
					AddEventList(G_cTxt, 10);
		}	}	}
		break;

	case NOTIFY_ENERGYSPHERECREATED:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp = (short *)cp;
		sV1 = *sp;
		cp += 2;
		sp = (short *)cp;
		sV2 = *sp;
		cp += 2;
		ZeroMemory(cTempName, sizeof(cTempName));
		memcpy(cTempName, cp, 20);
		cp += 20;

		ZeroMemory(G_cTxt, sizeof(G_cTxt));
		wsprintfA(G_cTxt, NOTIFY_MSG_HANDLER38, sV1, sV2, cTempName);
		AddEventList(G_cTxt, 10);
		//AddEventList(NOTIFY_MSG_HANDLER39, 10);
		break;

	case NOTIFY_QUERY_JOINPARTY:
		ZeroMemory(m_dialogBoxes[32].cStr, sizeof(m_dialogBoxes[32].cStr));
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		strcpy(m_dialogBoxes[32].cStr, cp);

		if(m_partyAutoAccept) {
			bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_ACCEPTJOINPARTY, 0, 1, 0, 0, m_dialogBoxes[32].cStr);
		}
		else {
			EnableDialogBox(32, 0, 0, 0);
			m_dialogBoxes[32].SetMode(1);
		}
		break;

	case NOTIFY_DAMAGEMOVE:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		sp = (short *)cp;
		m_sDamageMove = *sp;
		cp += 2;
		#ifdef RaiseCriticalLimit
		ip = (int *)cp;
		m_iDamageMoveAmount = *ip;
		cp += 2; // Maybe change back to 2 xRisenx
		#else
		sp = (short *)cp;
		m_sDamageMoveAmount = *sp;
		cp += 2;
		#endif
		break;

	case NOTIFY_OBSERVERMODE:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		sp = (short *)cp;
		if (*sp == 1)
		{	AddEventList(NOTIFY_MSG_HANDLER40);//"Observer Mode On. Press 'SHIFT + ESC' to Log Out..."
			m_bIsObserverMode = true;
			m_dwObserverCamTime = unixtime();
			char cName[12];
			ZeroMemory(cName, sizeof(cName));
			memcpy(cName, m_cPlayerName, 10);
			m_pMapData->bSetOwner(m_sPlayerObjectID, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cName, 0, 0, 0, 0);
		}else
		{	AddEventList(NOTIFY_MSG_HANDLER41);//"Observer Mode Off"
			m_bIsObserverMode = false;
			m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir, m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor, m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue, m_iPlayerStatus, m_cPlayerName, OBJECTSTOP, 0, 0, 0); // Re-Coding Sprite xRisenx
		}
		break;

	case NOTIFY_BUILDITEMSUCCESS:
		DisableDialogBox(26);
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp = (short *)cp;
		sV1 = *sp;
		cp += 2;
		sp = (short *)cp;
		sV2 = *sp;
		cp += 2;
		if (sV1 < 10000)
		{	EnableDialogBox(26, 6, 1, sV1, 0);
			m_dialogBoxes[26].sV1 = sV2;
		}else
		{	EnableDialogBox(26, 6, 1, -1*(sV1 - 10000), 0);
			m_dialogBoxes[26].sV1 = sV2;
		}
		AddEventList(NOTIFY_MSG_HANDLER42, 10);
		PlaySound('E', 23, 5);
		switch (m_sPlayerType) {
		case 1:
		case 2:
		case 3:
			PlaySound('C', 21, 0);
			break;

		case 4:
		case 5:
		case 6:
			PlaySound('C', 22, 0);
			break;
		}
		break;

	case NOTIFY_BUILDITEMFAIL:
		DisableDialogBox(26);
		EnableDialogBox(26, 6, 0, 0);
		AddEventList(NOTIFY_MSG_HANDLER43, 10);
		PlaySound('E', 24, 5);
		break;

	case NOTIFY_QUESTREWARD:
		NotifyMsg_QuestReward(pData);
		break;

	case NOTIFY_QUESTCOMPLETED:
		m_stQuest.bIsQuestCompleted = true;
		DisableDialogBox(28);
		EnableDialogBox(28, 1, 0, 0);
		switch (m_sPlayerType) {
		case 1:
		case 2:
		case 3:	PlaySound('C', 21, 0); break;
		case 4:
		case 5:
		case 6:	PlaySound('C', 22, 0); break;
		}
		PlaySound('E', 23, 0);
		AddEventList(NOTIFY_MSG_HANDLER44, 10);
		break;

	case NOTIFY_QUESTABORTED:
		m_stQuest.sQuestType = 0;
		DisableDialogBox(28);
		EnableDialogBox(28, 2, 0, 0);
		break;

	case NOTIFY_QUESTCONTENTS:
		NotifyMsg_QuestContents(pData);
		break;

	case NOTIFY_ITEMCOLORCHANGE:
		NotifyMsg_ItemColorChange(pData);
		break;

	case NOTIFY_ITEMSOCKETCHANGE:
		NotifyMsg_ItemSocketChange(pData);
		break;

	case NOTIFY_DROPITEMFIN_COUNTCHANGED:
		NotifyMsg_DropItemFin_CountChanged(pData);
		break;

	case NOTIFY_CANNOTGIVEITEM:
		NotifyMsg_CannotGiveItem(pData);
		break;

	case NOTIFY_GIVEITEMFIN_COUNTCHANGED:
		NotifyMsg_GiveItemFin_CountChanged(pData);
		break;

	case NOTIFY_EXCHANGEITEMCOMPLETE:
		AddEventList(NOTIFYMSG_EXCHANGEITEM_COMPLETE1, 10);
		DisableDialogBox(27);
		//Snoopy: MultiTrade
		DisableDialogBox(41);
		PlaySound('E', 23, 5);
		break;

	case NOTIFY_CANCELEXCHANGEITEM:
		PlaySound('E', 24, 5);
		AddEventList(NOTIFYMSG_CANCEL_EXCHANGEITEM1, 10);
		AddEventList(NOTIFYMSG_CANCEL_EXCHANGEITEM2, 10);
		//Snoopy: MultiTrade
		DisableDialogBox(41);
		DisableDialogBox(27);
		break;

	case NOTIFY_SETEXCHANGEITEM:
		NotifyMsg_SetExchangeItem(pData);
		break;

	case NOTIFY_OPENEXCHANGEWINDOW:
		NotifyMsg_OpenExchageWindow(pData);
		break;

	case NOTIFY_NOTFLAGSPOT:
		AddEventList(NOTIFY_MSG_HANDLER45, 10);
		break;

	case NOTIFY_ITEMPOSLIST:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		for (i = 0; i < MAXITEMS; i++) {
			sp = (short *)cp;
			sX = *sp;
			cp += 2;
			sp = (short *)cp;
			sY = *sp;
			cp += 2;
			if (m_pItemList[i] != 0) {
				if (sY < -10) sY = -10;
				if (sX < 0)   sX = 0;
				if (sX > 170) sX = 170;
				if (sY > 95)  sY = 95;

				m_pItemList[i]->m_sX = sX;
				m_pItemList[i]->m_sY = sY;
			}
		}
		break;

	case NOTIFY_ENEMYKILLS:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		ip = (int *)cp;
		m_iEnemyKillCount = *ip;
		ip++;
		m_iEnemyKillTotalCount = *ip;
		break;
		
	case NOTIFY_DOWNSKILLINDEXSET:
		NotifyMsg_DownSkillIndexSet(pData);
		break;

	case NOTIFY_ADMINIFO:
		NotifyMsg_AdminInfo(pData);
		break;

	case NOTIFY_NPCTALK:
		NpcTalkHandler(pData);
		break;

	case NOTIFY_PORTIONSUCCESS:
		AddEventList(NOTIFY_MSG_HANDLER46, 10);
		break;

	case NOTIFY_PORTIONFAIL:
		AddEventList(NOTIFY_MSG_HANDLER47, 10);
		break;

	case NOTIFY_LOWPORTIONSKILL:
		AddEventList(NOTIFY_MSG_HANDLER48, 10);
		break;

	case NOTIFY_NOMATCHINGPORTION:
		AddEventList(NOTIFY_MSG_HANDLER49, 10);
		break;

	case NOTIFY_SUPERATTACKLEFT:
		sp = (short *)(pData + INDEX2_MSGTYPE + 2);
		m_iSuperAttackLeft = (int)*sp;
		break;

	case NOTIFY_SAFEATTACKMODE:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		switch (*cp) {
		case 1:
			if(!m_bIsSafeAttackMode) AddEventList(NOTIFY_MSG_HANDLER50, 10);//"
			m_bIsSafeAttackMode = true;
			break;
		case 0:
			if(m_bIsSafeAttackMode) AddEventList(NOTIFY_MSG_HANDLER51, 10);//"
			m_bIsSafeAttackMode = false;
			break;
		}
		break;

	case NOTIFY_IPACCOUNTINFO:
		ZeroMemory(cTemp, sizeof(cTemp));
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		strcpy(cTemp, cp);
		AddEventList(cTemp);
		break;

	case NOTIFY_REWARDGOLD:
		dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
		m_iRewardGold = *dwp;
		break;

	case NOTIFY_SERVERSHUTDOWN:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		if (m_bIsDialogEnabled[25] == false)
			 EnableDialogBox(25, *cp, 0, 0);
		else m_dialogBoxes[25].SetMode(*cp);
		PlaySound('E', 27, 0);
		break;

	case NOTIFY_GLOBALATTACKMODE:
		NotifyMsg_GlobalAttackMode(pData);
		break;

	case NOTIFY_WEATHERCHANGE:
		NotifyMsg_WeatherChange(pData);
		break;

	case NOTIFY_FISHCANCELED:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		wp = (uint16_t *)cp;
		switch (*wp) {
		case 0:
			AddEventList(NOTIFY_MSG_HANDLER52, 10);
			DisableDialogBox(24);
			break;

		case 1:
			AddEventList(NOTIFY_MSG_HANDLER53, 10);
			DisableDialogBox(24);
			break;

		case 2:
			AddEventList(NOTIFY_MSG_HANDLER54, 10);
			DisableDialogBox(24);
			break;
		}
		break;

	case NOTIFY_FISHSUCCESS:
		AddEventList(NOTIFY_MSG_HANDLER55, 10);
		PlaySound('E', 23, 5);
		PlaySound('E', 17, 5);
		switch (m_sPlayerType) {
		case 1:
		case 2:
		case 3:
			PlaySound('C', 21, 0);
			break;

		case 4:
		case 5:
		case 6:
			PlaySound('C', 22, 0);
			break;
		}
		break;

	case NOTIFY_FISHFAIL:
		AddEventList(NOTIFY_MSG_HANDLER56, 10);
		PlaySound('E', 24, 5);
		break;

	case NOTIFY_FISHCHANCE:
		NotifyMsg_FishChance(pData);
		break;

	case NOTIFY_EVENTFISHMODE:
		NotifyMsg_EventFishMode(pData);
		break;

	case NOTIFY_NOTICEMSG:
		NotifyMsg_NoticeMsg(pData);
		break;

	case NOTIFY_RATINGPLAYER:
		NotifyMsg_RatingPlayer(pData);
		break;

	case NOTIFY_CANNOTRATING:
		NotifyMsg_CannotRating(pData);
		break;

	case NOTIFY_ADMINUSERLEVELLOW:
		AddEventList(NOTIFY_MSG_HANDLER58, 10);
		break;

	case NOTIFY_NOGUILDMASTERLEVEL:
		AddEventList(NOTIFY_MSG_HANDLER59, 10);
		break;
	case NOTIFY_SUCCESSBANGUILDMAN:
		AddEventList(NOTIFY_MSG_HANDLER60, 10);
		break;
	case NOTIFY_CANNOTBANGUILDMAN:
		AddEventList(NOTIFY_MSG_HANDLER61, 10);
		break;

	case NOTIFY_PLAYERSHUTUP:
		NotifyMsg_PlayerShutUp(pData);
		break;

	case NOTIFY_TIMECHANGE:
		NotifyMsg_TimeChange(pData);
		break;

	case NOTIFY_TOBERECALLED:
		AddEventList(NOTIFY_MSG_HANDLER62, 10);
		break;

	case NOTIFY_HUNGER:
		NotifyMsg_Hunger(pData);
		break;

	case NOTIFY_PLAYERPROFILE:
		NotifyMsg_PlayerProfile(pData);
		break;

	case NOTIFY_WHISPERMODEON:
		NotifyMsg_WhisperMode(true, pData);
		break;

	case NOTIFY_WHISPERMODEOFF:
		NotifyMsg_WhisperMode(false, pData);
		break;

	case NOTIFY_PLAYERONGAME:
		NotifyMsg_UnitStatus(true, pData);
		break;

	case NOTIFY_PLAYERNOTONGAME:
		NotifyMsg_UnitStatus(false, pData);
		break;

	case NOTIFY_CHARISMA:
		NotifyMsg_Charisma(pData);
		break;

	case NOTIFY_ITEMSOLD:
		DisableDialogBox(23);
		break;

	case NOTIFY_ITEMREPAIRED:
		DisableDialogBox(23);
		NotifyMsg_ItemRepaired(pData);
		break;

	case NOTIFY_CANNOTREPAIRITEM:
		NotifyMsg_CannotRepairItem(pData);
		break;

	case NOTIFY_CANNOTSELLITEM:
		NotifyMsg_CannotSellItem(pData);
		break;

	case NOTIFY_REPAIRITEMPRICE:
		NotifyMsg_RepairItemPrice(pData);
		break;

	case NOTIFY_SELLITEMPRICE:
		NotifyMsg_SellItemPrice(pData);
		break;

	case NOTIFY_SHOWMAP:
		NotifyMsg_ShowMap(pData);
		break;

	case NOTIFY_SKILLUSINGEND:
		NotifyMsg_SkillUsingEnd(pData);
		break;

	case NOTIFY_TOTALUSERS:
		NotifyMsg_TotalUsers(pData);
		break;

	case NOTIFY_EVENTSTART:
		NotifyMsg_EventStart(pData);
		break;

	case NOTIFY_EVENTSTARTING:
	case NOTIFY_EVENTSTARTING2:
	case NOTIFY_EVENTSTARTING3:
		NotifyMsg_EventStarting(pData);
		break;

	case NOTIFY_CASUALTIES:
		NotifyMsg_Casualties(pData);
		break;
		
	case NOTIFY_SHIELDHP: // Shield xRisenx
		NotifyMsg_ShieldHP(pData);
		break;

	case NOTIFY_RELICINALTAR:
		NotifyMsg_RelicInAltar(pData);
		break;

	case NOTIFY_RELICGRABBED:
		NotifyMsg_RelicGrabbed(pData);
		break;

	case NOTIFY_EVENTWINNER:
		NotifyMsg_EventWinner(pData);
		break;

	case NOTIFY_RELICPOSITION:
		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		sp = (short *)cp;
		m_relicX = *sp;
		sp++;
		m_relicY = *sp;
		break;

	case NOTIFY_MAGICEFFECTOFF:
		NotifyMsg_MagicEffectOff(pData);
		break;

	case NOTIFY_MAGICEFFECTON:
		NotifyMsg_MagicEffectOn(pData);
		break;

	case NOTIFY_CANNOTITEMTOBANK:
		AddEventList(NOTIFY_MSG_HANDLER63, 10);
		break;

	case NOTIFY_SERVERCHANGE:
		NotifyMsg_ServerChange(pData);
		break;

	case NOTIFY_SKILL:
		NotifyMsg_Skill(pData);
		break;

	case NOTIFY_SETITEMCOUNT:
		NotifyMsg_SetItemCount(pData);
		break;

	case NOTIFY_ITEMDEPLETED_ERASEITEM:
		NotifyMsg_ItemDepleted_EraseItem(pData);
		break;

	case NOTIFY_DROPITEMFIN_ERASEITEM:
		NotifyMsg_DropItemFin_EraseItem(pData);
		break;

	case NOTIFY_GIVEITEMFIN_ERASEITEM:
		NotifyMsg_GiveItemFin_EraseItem(pData);
		break;

	case NOTIFY_ENEMYKILLREWARD:
		NotifyMsg_EnemyKillReward(pData);
		break;

	case NOTIFY_PKCAPTURED:
		NotifyMsg_PKcaptured(pData);
		break;

	case NOTIFY_PKPENALTY:
		NotifyMsg_PKpenalty(pData);
		break;

	case NOTIFY_ITEMTOBANK:
		NotifyMsg_ItemToBank(pData);
		break;

	case NOTIFY_ITEMTOGUILDBANK:
		NotifyMsg_ItemToGuildBank(pData);
		break;

	case NOTIFY_TRAVELERLIMITEDLEVEL:
		AddEventList(NOTIFY_MSG_HANDLER64, 10);
		break;

	case NOTIFY_LIMITEDLEVEL:
		AddEventList(NOTIFYMSG_LIMITED_LEVEL1, 10);
		break;

	case NOTIFY_ITEMLIFESPANEND:
		NotifyMsg_ItemLifeSpanEnd(pData);
		break;

	case NOTIFY_ITEMRELEASED:
		NotifyMsg_ItemReleased(pData);
		break;

	case NOTIFY_ITEMOBTAINED:
		NotifyMsg_ItemObtained(pData);
		break;

	case NOTIFY_ITEMPURCHASED:
		NotifyMsg_ItemPurchased(pData);
		break;

	case NOTIFY_QUERY_JOINGUILDREQPERMISSION:
		NotifyMsg_QueryJoinGuildPermission(pData);
		break;

	case NOTIFY_QUERY_DISMISSGUILDREQPERMISSION:
		NotifyMsg_QueryDismissGuildPermission(pData);
		break;

	case COMMONTYPE_JOINGUILDAPPROVE:
		NotifyMsg_JoinGuildApprove(pData);
		break;

	case COMMONTYPE_JOINGUILDREJECT:
		NotifyMsg_JoinGuildReject(pData);
		break;

	case COMMONTYPE_DISMISSGUILDAPPROVE:
		NotifyMsg_DismissGuildApprove(pData);
		break;

	case COMMONTYPE_DISMISSGUILDREJECT:
		NotifyMsg_DismissGuildReject(pData);
		break;

	case NOTIFY_CANNOTCARRYMOREITEM:
		AddEventList(NOTIFY_MSG_HANDLER65, 10);//"
		AddEventList(NOTIFY_MSG_HANDLER66, 10);//"
		// Bank dialog Box
		m_dialogBoxes[14].SetMode(0);
		//GuildBank
		m_dialogBoxes[56].SetMode(0);
		break;

	case NOTIFY_NOTENOUGHGOLD:
		DisableDialogBox(23);
		AddEventList(NOTIFY_MSG_HANDLER67, 10);//"Gold
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		if (*cp >= 0) {
			m_bIsItemDisabled[*cp] = false;
		}
		break;

	case NOTIFY_HP:
		NotifyMsg_HP(pData);
		break;
	case NOTIFY_MP:
		NotifyMsg_MP(pData);
		break;
	case NOTIFY_SP:
		NotifyMsg_SP(pData);
		break;
	case NOTIFY_KILLED:
		NotifyMsg_Killed(pData);
		break;
	case NOTIFY_EXP:
		NotifyMsg_Exp(pData);
		break;
	case NOTIFY_GUILDDISBANDED:
		NotifyMsg_GuildDisbanded(pData);
		break;
	case NOTIFY_CANNOTJOINMOREGUILDSMAN:
		NotifyMsg_CannotJoinMoreGuildsMan(pData);
		break;
	case NOTIFY_NEWGUILDSMAN:
		NotifyMsg_NewGuildsMan(pData);
		break;
	case NOTIFY_DISMISSGUILDSMAN:
		NotifyMsg_DismissGuildsMan(pData);
		break;
	case NOTIFY_MAGICSTUDYSUCCESS:
		NotifyMsg_MagicStudySuccess(pData);
		break;
	case NOTIFY_MAGICSTUDYFAIL:
		NotifyMsg_MagicStudyFail(pData);
		break;
	case NOTIFY_SKILLTRAINSUCCESS:
		NotifyMsg_SkillTrainSuccess(pData);
		break;
	case NOTIFY_SKILLTRAINFAIL:
		break;
	case NOTIFY_FORCEDISCONN:
		NotifyMsg_ForceDisconn(pData);
		break;
	case NOTIFY_FRIENDONGAME:
		NotifyMsg_FriendOnGame(pData);
		break;
	case NOTIFY_CANNOTRECALL:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		Pop(cp, (uint8_t&)sV1);
		switch(sV1)
		{
		case 0:
			AddEventList(NOTIFY_MSG_HANDLER74, 10); //"You can't recall within 10 seconds of taking damage."
			break;
		case 1:
			AddEventList(NOTIFY_MSG_HANDLER74B, 10); //"You cannot recall in this map."
			break;
		}
		break;

	case NOTIFY_CANNOTCRUSADE:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		Pop(cp, (uint8_t&)sV1);
		switch(sV1)
		{
		case 0:
			AddEventList(NOTIFY_MSG_HANDLER77, 10); 
			break;
		case 1:
			AddEventList(NOTIFY_MSG_HANDLER78, 10); 
			break;
		case 2:
			AddEventList(NOTIFY_MSG_HANDLER79, 10); 
			break;
		}
		break;
	case NOTIFY_FIGHTZONERESERVE:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		ip = (int *)cp;
		switch (*ip) {
		case -5:
			AddEventList(NOTIFY_MSG_HANDLER68, 10);
			break;
		case -4:
			AddEventList(NOTIFY_MSG_HANDLER69, 10);
			break;
		case -3:
			AddEventList(NOTIFY_MSG_HANDLER70, 10);
			break;
		case -2:
			m_iFightzoneNumber = 0;
			AddEventList(NOTIFY_MSG_HANDLER71, 10);
			break;
		case -1:
			m_iFightzoneNumber = m_iFightzoneNumber * -1 ;
			AddEventList(NOTIFY_MSG_HANDLER72, 10);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			wsprintfA(cTxt, NOTIFY_MSG_HANDLER73, *ip);//"
			AddEventList(cTxt, 10);
			break;
		}
		break;

#ifdef TitleClient
	case NOTIFY_CHANGETITLE:
		char cCharName[11];//TODO TITLES: change this to 11 in my code...?
		ZeroMemory(cCharName, sizeof(cCharName));

		cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
		memcpy(cCharName, cp, 11);
		cp += 11;

		ClearPlayerTitle(cCharName);

		break;

	case NOTIFY_ADDTITLE:
		NotifyMsg_AddTitle(pData);
		break;

	case NOTIFY_TITLELEVELUP:
		NotifyMsg_TitleLevelUp(pData);
		break;

	case NOTIFY_REQTITLEANSWER:
		{
			cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);

			ip = (int *)cp;
			int iReturnIndex = *ip;
			cp += 4;

			ip = (int *)cp;
			int iNumPoints = *ip;
			cp += 4;

			ip = (int *)cp;
			int iRank = *ip;
			cp += 4;

			ip = (int *)cp;
			int iCrusadeJob = *ip;
			cp += 4;

			//dwp  = (uint32_t *)cp;
			//uint32_t dwV1 = *dwp;//iCurrentNo
			//cp += 4;

			//ip  = (int *)cp;
			//sV2 = *ip;//iReturnIndex (for m_stTitles[..])
			//cp += 4;

			//ip  = (int *)cp;
			//sV3 = *ip;//iCrusadeT
			//cp += 4;

			ZeroMemory(cTemp, sizeof(cTemp) );
			memcpy(cTemp, cp, 21);
			cp += 21;

			//ZeroMemory( m_stTitles[sV2].cSubject, sizeof(m_stTitles[sV2].cSubject) );
			//strcpy(m_stTitles[sV2].cSubject, cTemp);

			//m_stTitles[sV2].iCurrentNo = dwV1;
			//m_stTitles[sV2].iCrusadeT = sV3;
			ZeroMemory( m_stTitles[iReturnIndex].cSubject, sizeof(m_stTitles[iReturnIndex].cSubject) );
			strcpy(m_stTitles[iReturnIndex].cSubject, cTemp);

			m_stTitles[iReturnIndex].iCurrentNo = iNumPoints;
			m_stTitles[iReturnIndex].iCrusadeT = iCrusadeJob;
			m_stTitles[iReturnIndex].iRank = iRank;
			break;
		}
#endif

	case NOTIFY_REPAIRALLPRICE:
		NotifyMsg_RepairAllPrice(pData);
		break;
		
	case NOTIFY_MAILRECEIVED:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		Pop(cp, str);

		sprintf(cTxt, NOTIFY_MSG_HANDLER82, str.c_str());
		AddEventList(cTxt, 10);

		PlaySound('E', 54, 5);

		if(m_bIsDialogEnabled[DIALOG_MAILBOX])
		{
			bSendCommand(MSGID_REQ_MAILBOX);
		}
		break;

	case NOTIFY_GUILDBOARDPOST:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		Pop(cp, str);

		if(strcmp(str.c_str(), m_cPlayerName) == 0)
		{
			sprintf(cTxt, NOTIFY_MSG_HANDLER86, str.c_str());
			m_dialogBoxes[DIALOG_GUILD].SetMode(8);
		}
		else {
			sprintf(cTxt, NOTIFY_MSG_HANDLER85, str.c_str());
			PlaySound('E', 54, 5);
		}

		AddEventList(cTxt, 10);
		
		if(m_bIsDialogEnabled[DIALOG_GUILD])
		{
			bSendCommand(MSGID_REQ_GUILDBOARD);
		}
		break;
		
	case NOTIFY_ADMININFO:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		Pop(cp, m_adminLevel);
		Pop(cp, m_GMFlags);
		break;

	case NOTIFY_DONATEEVENTINFO:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		Pop(cp, m_eventPot);
		break;

	case NOTIFY_PINGMAP:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		uint8_t type;
		uint16_t x,y;
		
		Pop(cp, type);
		Pop(cp, x);
		Pop(cp, y);
		
		AddMapPing(MapPing(x, y));
		break;
		// Gladiator Arena xRisenx
	//case NOTIFY_ARENASTATUS:
	//	cp = (char *)(pData   + INDEX2_MSGTYPE + 2);
	//	   ip  = (int *)cp;
	//	   sV1 = *ip;
	//	   cp += 4;

	//	   ip  = (int *)cp;
	//	   sV2 = *ip;
	//	   cp += 4;

	//	   ip  = (int *)cp;
	//	   sV3 = *ip;
	//	   cp += 4;

	//	   ZeroMemory(cTemp, sizeof(cTemp) );
	//	   memcpy(cTemp, cp, 21);
	//	   cp += 21;

	//	   if(sV3 > 0)
	//	   {
	//		  bool bFound = false;

	//		  for (i = 0; i < MAXARENAPLAYERS; i++)
	//		  {
	//			 if(bFound) break;
	//			 if(strcmp(m_stArenaPlayers[i].cCharName, cTemp) == 0)
	//			 {
	//				m_stArenaPlayers[i].iKills = sV1;
	//				m_stArenaPlayers[i].iDeaths = sV2;
	//				bFound = true;
	//			 }
	//		  }
	//		  if(bFound == false)
	//		  {
	//			 for (i = 0; i < MAXARENAPLAYERS; i++)
	//			 {
	//				if(strlen(m_stArenaPlayers[i].cCharName) == 0)
	//				{
	//				   m_stArenaPlayers[i].iKills = sV1;
	//				   m_stArenaPlayers[i].iDeaths = sV2;
	//				   ZeroMemory(m_stArenaPlayers[i].cCharName, sizeof(m_stArenaPlayers[i].cCharName) );
	//				   strcpy(m_stArenaPlayers[i].cCharName, cTemp);
	//				   break;
	//				}
	//			 }
	//		  }
	//	   }
	//	   else // player DC/Logout/Recall, remove from list
	//	   {
	//		  for (i = 0; i < MAXARENAPLAYERS; i++)
	//		  {
	//			 if(strcmp(m_stArenaPlayers[i].cCharName, cTemp) == 0)
	//			 {
	//				m_stArenaPlayers[i].iKills = 0;
	//				m_stArenaPlayers[i].iDeaths = 0;
	//				ZeroMemory(m_stArenaPlayers[i].cCharName, sizeof(m_stArenaPlayers[i].cCharName));
	//			 }
	//		  }
	//	   }
		// Gladiator Arena xRisenx
#ifdef MonsterBarClient
	case NOTIFY_NPCBAR:
		NotifyMsg_NpcBar(pData);
		break;

	case NOTIFY_NPCBAR2:
		NotifyMsg_NpcBar2(pData);
		break;
#endif

	// Monster kill event xRisenx
	case NOTIFY_NPCHUNTON:
		NotifyMsg_NpcHuntingON();
		break;
	case NOTIFY_NPCHUNTOFF:
		NotifyMsg_NpcHuntingOFF();
		break;
	case NOTIFY_NPCHUNTWINNER:
		AddEventList( "You Gained 50 Reputation Points", 10 );
		AddEventList( "You Gained 50 Contribution Points", 10 );
		NotifyMsg_NpcHuntingWinner(pData);
		break;
	// Monster kill event xRisenx
	}
}

void CGame::ReserveFightzoneResponseHandler(char * pData)
{
 	uint16_t * wpResult;
	char * cp ;
	int * ip ;
 	wpResult = (uint16_t *)(pData + INDEX2_MSGTYPE);
	switch (*wpResult) {
	case MSGTYPE_CONFIRM:
		AddEventList(RESERVE_FIGHTZONE_RESPONSE_HANDLER1, 10);
		m_dialogBoxes[7].SetMode(14);
		m_iFightzoneNumber = m_iFightzoneNumberTemp ;
		break;

	case MSGTYPE_REJECT:
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		ip   = (int *)cp;
		cp += 4;
		AddEventList(RESERVE_FIGHTZONE_RESPONSE_HANDLER2, 10);
		m_iFightzoneNumberTemp = 0 ;

		if (*ip == 0) {
		 	m_dialogBoxes[7].SetMode(15);
		}else if (*ip == -1){
			m_dialogBoxes[7].SetMode(16);
		} else if (*ip == -2) {
			m_dialogBoxes[7].SetMode(17);
		}else if (*ip == -3) {
			m_dialogBoxes[7].SetMode(21);
		}else if (*ip == -4) {
			m_dialogBoxes[7].SetMode(22);
		}
		break;
	}
}

void CGame::RetrieveItemHandler(char *pData)
{
	char * cp, /*cBankItemIndex,*/ cItemIndex, cTxt[120];
	short cBankItemIndex;
	uint16_t * wp;
	int j;

	wp = (uint16_t *)(pData + INDEX2_MSGTYPE);
	if (*wp != MSGTYPE_REJECT)
	{
		cp = (char *)(pData + INDEX2_MSGTYPE + 2);
		cBankItemIndex = *cp;
		cp++;
		cItemIndex = *cp;
		cp++;

		if (m_pBankList[cBankItemIndex] != 0) {

			char cStr1[64], cStr2[64], cStr3[64];
			GetItemName(m_pBankList[cBankItemIndex], cStr1, cStr2, cStr3);

			ZeroMemory(cTxt, sizeof(cTxt));
			wsprintfA(cTxt, RETIEVE_ITEM_HANDLER4, cStr1);//""You took out %s."
			AddEventList(cTxt, 10);

			if(m_pBankList[cBankItemIndex]->m_cItemType == ITEMTYPE_CONSUME ||
				m_pBankList[cBankItemIndex]->m_cItemType == ITEMTYPE_ARROW)
			{
				if (!m_pItemList[cItemIndex]) goto RIH_STEP2;
				delete m_pBankList[cBankItemIndex];
				m_pBankList[cBankItemIndex] = 0;

				for( j = 0; j < MAXBANKITEMS - 1; j++)
				{
					if(m_pBankList[j+1] && !m_pBankList[j])
					{
						m_pBankList[j] = m_pBankList[j+1];
						m_pBankList[j+1] = 0;
					}
				}
			}else{
RIH_STEP2:;
				if(m_pItemList[cItemIndex]) return;
				short nX, nY;
				nX = 40;
				nY = 30;
				for (j = 0; j < MAXITEMS; j++)
				{
					if(m_pItemList[j] && memcmp(m_pItemList[j]->m_cName, cStr1, 20) == 0)
					{
						nX = m_pItemList[j]->m_sX+1;
						nY = m_pItemList[j]->m_sY+1;
						break;
					}
				}
				m_pItemList[cItemIndex] = m_pBankList[cBankItemIndex];
				m_pItemList[cItemIndex]->m_sX =	nX;
				m_pItemList[cItemIndex]->m_sY =	nY;
                bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, cItemIndex, nX, nY, 0, 0);

				for (j = 0; j < MAXITEMS; j++)
				if (m_cItemOrder[j] == -1)
				{
					m_cItemOrder[j] = cItemIndex;
					break;
				}
				m_bIsItemEquipped[cItemIndex] = false;
				m_bIsItemDisabled[cItemIndex] = false;
				m_pBankList[cBankItemIndex] = 0;
				for ( j = 0; j < MAXBANKITEMS - 1; j++)
				{
					if ((m_pBankList[j+1] != 0) && (m_pBankList[j] == 0))
					{
						m_pBankList[j] = m_pBankList[j+1];
						m_pBankList[j+1] = 0;
					}
				}
			}
		}
	}
	m_dialogBoxes[14].SetMode(0);
}

void CGame::EraseItem(char cItemID)
{int i;
 char cStr1[64], cStr2[64], cStr3[64];
	ZeroMemory(cStr1, sizeof(cStr1));
	ZeroMemory(cStr2, sizeof(cStr2));
	ZeroMemory(cStr3, sizeof(cStr3));
	for( i=0 ; i<6 ; i++ )
	{	if (m_sShortCut[i] == cItemID)
		{	GetItemName(m_pItemList[cItemID], cStr1, cStr2, cStr3);
			if( i < 3 ) wsprintfA(G_cTxt, ERASE_ITEM, cStr1, cStr2, cStr3, i+1);
			else wsprintfA(G_cTxt, ERASE_ITEM, cStr1, cStr2, cStr3, i+7);
			AddEventList(G_cTxt, 10);
			m_sShortCut[i] = -1;
	}	}

	if (cItemID == m_sRecentShortCut)
		m_sRecentShortCut = -1;
	// ItemOrder
	for (i = 0; i < MAXITEMS; i++)
	if (m_cItemOrder[i] == cItemID)
			m_cItemOrder[i] = -1;
	for (i = 1; i < MAXITEMS; i++)
	if ((m_cItemOrder[i-1] == -1) && (m_cItemOrder[i] != -1))
	{	m_cItemOrder[i-1] = m_cItemOrder[i];
		m_cItemOrder[i]   = -1;
	}
	// ItemList
	delete m_pItemList[cItemID];
	m_pItemList[cItemID] = 0;
	m_bIsItemEquipped[cItemID] = false;
	m_bIsItemDisabled[cItemID] = false;
	_iCalcTotalWeight();
}

void CGame::AddMapPing(MapPing ping)
{
	PlaySound('E', 14, 5);
	m_mapPings.push_back( ping );
}

Point CGame::GetGuideMapPos(uint32_t x, uint32_t y)
{	
	Point p;
	short sX = m_dialogBoxes[9].m_X;
	short sY = m_dialogBoxes[9].m_Y;
	short szX = m_dialogBoxes[9].sSizeX;
	short szY = m_dialogBoxes[9].sSizeY;

	if( sX < 20 ) sX = 0;
	if( sY < 20 ) sY = 0;
	//if( sX > 640-128-20 ) sX = 640-128;
	if( sX > GetWidth()-128-20 ) sX = GetWidth()-128; // 800x600 Resolution xRisenx
	//if( sY > 427-128-20 ) sY = 427-128;
	if( sY > GetHeight()-53-128-20 ) sY = GetHeight()-53-128; // 800x600 Resolution xRisenx
	if( m_bZoomMap )
	{	
		p.x = m_sPlayerX-64;
		p.y = m_sPlayerY-64;
		if( p.x < 0 ) p.x = 0;
		if( p.y < 0 ) p.y = 0;
		if( p.x > m_pMapData->m_sMapSizeX-128 ) p.x = m_pMapData->m_sMapSizeX-128;
		if( p.y > m_pMapData->m_sMapSizeY-128 ) p.y = m_pMapData->m_sMapSizeY-128;
		p.x = p.x + x - sX;
		p.y = p.y + y - sY;
	}else
	{	
		p.x = (m_pMapData->m_sMapSizeX*(x-sX))/128;
		p.y = (m_pMapData->m_sMapSizeX*(y-sY))/128;
	}

	return p;
}


void CGame::DrawNpcName(short sX, short sY, short sOwnerType, int iStatus)
{
	char cTxt[42], cTxt2[64];
	ZeroMemory(cTxt, sizeof(cTxt));
	ZeroMemory(cTxt2, sizeof(cTxt2));
	int iAdd = 0;
#ifdef MonsterDetails
	int iNameLength = 0;
#endif

	GetNpcName(sOwnerType, cTxt);

	if ((iStatus & STATUS_BERSERK) != 0) strcat(cTxt, DRAW_OBJECT_NAME50);//" Berserk"
	if ((iStatus & STATUS_FROZEN) != 0) strcat(cTxt, DRAW_OBJECT_NAME51);//" Frozen"
	PutString2(sX, sY, cTxt, 255,255,255);

	iAdd += 14;

#ifdef MonsterDetails
	iNameLength = sizeof(cTxt);

	//PutString2(sX + (iNameLength * 2), sY, "Test Text", 255,255,255);

	//iAdd += 14;

	switch (sOwnerType)
	{
		case 10: // slime
		//case 97: // slime king
		//case 98: // slime prince
			PutString2(sX, sY + iAdd, "Slime-Type", 130,130,130);
			iAdd += 14;
			break;
		case 31: // demon
		//case 95: // black demon
		case 49: // hellclaw
		case 27: // hellhound
		case 48: // Stalker
		case 62: // Dire Boar
		case 71: // Centaurus
		case 78: // Minotaur
			PutString2(sX, sY + iAdd, "Demon-Type", 130,130,130);
			iAdd += 14;
			break;
		case 66: // wyvern
		case 70: // barlog
		case 73: // fire wyvern
		case 114: // Eternal Dragon
			PutString2(sX, sY + iAdd, "Dragon-Type", 130,130,130);
			iAdd += 14;
			break;
		case 14: // Orc / Magic Orc
		case 77: // Master Mage Orc
			PutString2(sX, sY + iAdd, "Orc-Type", 130,130,130);
			iAdd += 14;
			break;
		case 12: // stone golem
		case 23: // clay golem
		case 65: // ice golem
			PutString2(sX, sY + iAdd, "Golem-Type", 130,130,130);
			iAdd += 14;
			break;
		case 60: // Cannibal plant
		case 76: // giant plant
		case 80: // tentocle
			PutString2(sX, sY + iAdd, "Plant-Type", 130,130,130);
			iAdd += 14;
			break;
		case 13: // cyclops
		case 29: // ogre
		case 59: // ettin
		case 58: // mountain giant
		case 16: // Giant Ant
		case 57: // Giant Frog
		case 17: // Giant Scorpion
		case 74: // Giant Crayfish
		case 75: // Giant Lizard
		case 81: // Abby
		case 72: // claw-turtle
			PutString2(sX, sY + iAdd, "Giant-Type", 130,130,130);
			iAdd += 14;
			break;
		case 18: // zombie
		case 11: // skeleton
		case 30: // liche
		case 33: // Werewolf
		case 54: // Dark-Elf
		case 52: // Gargoyle
			PutString2(sX, sY + iAdd, "Undead-Type", 130,130,130);
			iAdd += 14;
			break;
		case 61: // Rudolph
		case 63: // Frost
		case 53: // Beholder
		case 79: // Nizie
		case 32: // Unicorn
		case 56: // Cat
		case 55: // Rabbit
		case 50: // Tigerworm
			PutString2(sX, sY + iAdd, "Mystic-Type", 130,130,130);
			iAdd += 14;
			break;
	}
#endif

	if (m_bIsObserverMode == true) PutString2(sX, sY+iAdd, cTxt, 50,50,255);
	else if (m_bIsConfusion || (m_iIlusionOwnerH != 0)) {
		ZeroMemory(cTxt, sizeof(cTxt));
		strcpy(cTxt, DRAW_OBJECT_NAME87);
		PutString2(sX, sY+iAdd, cTxt, 150,150,150);
	}
	else
	{
		switch( _iGetFOE(iStatus) )
		{
		case -2:
			PutString2(sX, sY+iAdd, DRAW_OBJECT_NAME90, 255, 0, 0);
			break;
		case -1:
			PutString2(sX, sY+iAdd, DRAW_OBJECT_NAME90, 255, 0, 0);
			break;
		case 0:
			PutString2(sX, sY+iAdd, DRAW_OBJECT_NAME88, 50,50,255);
			break;
		case 1:
			PutString2(sX, sY+iAdd, DRAW_OBJECT_NAME89, 30,255,30);
			break;
		}
	}

#ifdef MonsterDetails
	//iAdd += 14;
#endif

	iAdd += 14;

	switch ((iStatus & 0x0F00) >> 8) {
	case 0: break;
	case 1: strcpy(cTxt2, DRAW_OBJECT_NAME52); break;//"Clairvoyant"
	case 2: strcpy(cTxt2, DRAW_OBJECT_NAME53); break;//"Destruction of Magic Protection"
	case 3: strcpy(cTxt2, DRAW_OBJECT_NAME54); break;//"Anti-Physical Damage"
	case 4: strcpy(cTxt2, DRAW_OBJECT_NAME55); break;//"Anti-Magic Damage"
	case 5: strcpy(cTxt2, DRAW_OBJECT_NAME56); break;//"Poisonous"
	case 6: strcpy(cTxt2, DRAW_OBJECT_NAME57); break;//"Critical Poisonous"
	case 7: strcpy(cTxt2, DRAW_OBJECT_NAME58); break;//"Explosive"
	case 8: strcpy(cTxt2, DRAW_OBJECT_NAME59); break;//"Critical Explosive"
	case 9: strcpy(cTxt2, DRAW_OBJECT_NAME59B); break;//"Highly Trained"
	case 10: strcpy(cTxt2, DRAW_OBJECT_NAME59G); break;//"Elite"
	case 11: strcpy(cTxt2, DRAW_OBJECT_NAME59D); break;//"Mighty"
	case 12: strcpy(cTxt2, DRAW_OBJECT_NAME59E); break;//"Crippled"
	case 13: strcpy(cTxt2, DRAW_OBJECT_NAME59F); break;//"Shaman"
	case 14: strcpy(cTxt2, DRAW_OBJECT_NAME59C); break;//"Swift"
	}
	if( m_Misc.bCheckIMEString(cTxt2) ) PutString_SprFont3(sX, sY + iAdd, cTxt2, m_wR[13]*4, m_wG[13]*4, m_wB[13]*4, false, 2);
	else PutString2(sX, sY + iAdd, cTxt2, 240,240,70);

#ifdef _DEBUG
	wsprintfA(G_cTxt,"Status: 0x%.8X ",iStatus);
	PutString2(sX+70, sY+(14*0), G_cTxt, 30,255,30);
#endif
}

void CGame::DrawObjectName(short sX, short sY, char * pName, int iStatus)
{
	char cTxt[64], cTxt2[64];
	short sR, sG, sB;
	int i, iGuildIndex, iFOE, iAddY=0;
	bool bPK;

	iFOE = _iGetFOE(iStatus);
	Side side = (Side)((iStatus & STATUS_SIDE) >> 28);
	if( iFOE < 0 )
	{
		sR = 255; sG = 0; sB = 0;
	}
	else if( iFOE == 0 )
	{
		sR = 50; sG = 50; sB = 255;
	}
	else
	{
		sR = 30; sG = 200; sB = 30;
	}

	ZeroMemory(cTxt, sizeof(cTxt));
	ZeroMemory(cTxt2, sizeof(cTxt2));

	if (m_iIlusionOwnerH == 0)
	{
		if(m_bIsCrusadeMode && m_adminLevel == 0) 
		{
			if (_tmp_wObjectID >= 10000) strcpy(cTxt, NPC_NAME_MERCENARY);
			else
			{
				if( iFOE == -1 ) wsprintfA(cTxt, "%d", _tmp_wObjectID);
				else strcpy(cTxt, pName);
			}
		}
		else
		{
			wsprintfA(cTxt, "%s", pName);
		}

		if(m_iPartyStatus)
		{
			for (i = 0; i < m_stPartyMember.size(); i++)
			{
				if (m_stPartyMember[i]->cName.compare(pName) == 0)
				{
					strcat(cTxt, BGET_NPC_NAME23);
					break;
				}
			}
		}
	}
	else strcpy(cTxt, "?????");

	if ((iStatus & STATUS_BERSERK) != 0) strcat(cTxt, DRAW_OBJECT_NAME50);//" Berserk"
	if ((iStatus & STATUS_FROZEN) != 0) strcat(cTxt, DRAW_OBJECT_NAME51);//" Frozen"

	PutString2(sX, sY, cTxt, 255,255,255);
	ZeroMemory(cTxt, sizeof(cTxt));

	if( memcmp(m_cPlayerName, pName, 10) == 0 )
	{
		if(m_iGuildRank != GUILDRANK_NONE)
		{
			wsprintfA( G_cTxt, "%s %s", m_cGuildName, GetGuildRankName(m_iGuildRank) );
			PutString2(sX, sY+14, G_cTxt, 180,180,180);
			iAddY = 14;
		}
		if( m_iPKCount != 0 )
		{
			bPK = true;
			//sR = 255; sG = 0; sB = 0;
		}
		else
		{
			bPK = false;
			//sR = 30; sG = 200; sB = 30;
		}

		side = m_side;
	}
	else
	{
		side = (Side)( (iStatus & STATUS_SIDE) >> 28 );

		if( iStatus & STATUS_PK) bPK = true;
		else bPK = false;

		if( (m_bIsCrusadeMode==false || iFOE>=0) && m_iIlusionOwnerH == 0 )
		{
			if( FindGuildName(pName, &iGuildIndex) == true )
			{
				if (m_stGuildName[iGuildIndex].cGuildName[0] != 0)
				{
					if( strcmp(m_stGuildName[iGuildIndex].cGuildName, "NONE" )!=0 )
					{
						if( m_stGuildName[iGuildIndex].iGuildRank != GUILDRANK_NONE )
						{
							wsprintfA( G_cTxt, "%s %s", m_stGuildName[iGuildIndex].cGuildName,
								GetGuildRankName(m_stGuildName[iGuildIndex].iGuildRank) );
							PutString2(sX, sY+14, G_cTxt, 180,180,180);
							m_stGuildName[iGuildIndex].dwRefTime = m_dwCurTime;
							iAddY = 14;
						}
					}
					else
					{
						m_stGuildName[iGuildIndex].dwRefTime = 0;
					}
				}
			}
			else bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQGUILDNAME, 0, _tmp_wObjectID, iGuildIndex, 0, 0);
		}
	}

	strcpy(cTxt, sideName[side]);

	if(side != NEUTRAL && !bPK)
		strcat(cTxt, MSG_COMBATANT);

	if(bPK)
		strcat(cTxt, MSG_PK);

	PutString2(sX, sY+14 +iAddY, cTxt, sR, sG, sB);

	iAddY += 14;
	// Owner Rank
	if(strcmp(pName, "xRisenx") == 0 )
	{
		sR = 255; sG = 153; sB = 0;
		strcpy(cTxt, "Owner");

		PutString2(sX, sY+14 +iAddY, cTxt, sR, sG, sB);

		iAddY += 14;
	}
	// GM Helper Rank
	if(strcmp(pName, "ShadowWarr") == 0 || strcmp(pName, "ShadowEvil") == 0 ||
		strcmp(pName, "Cracka") == 0 || strcmp(pName, "dub") == 0)
	{
		sR = 255; sG = 153; sB = 0;
		strcpy(cTxt, "GM Helper");

		PutString2(sX, sY+14 +iAddY, cTxt, sR, sG, sB);

		iAddY += 14;
	}

#ifdef TitleClient
	//iAddY += 14;

	int iTitleIndex;
	if( FindTitleName(pName, &iTitleIndex) == true ) {
		if (m_stTitles[iTitleIndex].cSubject[0] != 0) 
		{//BESK: maybe we should be using strlen here instead of checking the first char for nonzero value?
			if( strcmp(m_stTitles[iTitleIndex].cSubject, "NONE" )!=0 ) 
			{
				GetTitleName(m_stTitles[iTitleIndex].cSubject, m_stTitles[iTitleIndex].iRank, cTxt);
				ZeroMemory(G_cTxt, sizeof(G_cTxt));
				wsprintfA( G_cTxt, "%s", cTxt);
				PutString2(sX, sY+14 +iAddY, G_cTxt, 255,255,204);
				m_stTitles[iTitleIndex].dwRefTime = m_dwCurTime;
				iAddY += 14;
			}
			if ((m_bIsCrusadeMode == true)) {
				ZeroMemory(G_cTxt, sizeof(G_cTxt));
				switch(m_stTitles[iTitleIndex].iCrusadeT) {
					case 1: wsprintfA(G_cTxt, "Soldier"); break;
					case 2: wsprintfA(G_cTxt, "Constructor"); break;
					case 3: wsprintfA(G_cTxt, "Commander"); break;
					default: ZeroMemory(G_cTxt, sizeof(G_cTxt)); break;
				}
				PutString2(sX, sY+14 +iAddY, G_cTxt, 0,200,200);
				iAddY += 14;
			}
			//else if (_tmp_cFlag == 30 || _tmp_cFlag == 31) {
			//
			//	wsprintfA(G_cTxt, "Flag Carrier");
			//	PutString2(sX, sY+14 +iAddY, G_cTxt, 0,200,200);
			//	iAddY += 14;
			//}
			//else if (_tmp_cFlag > 0 && _tmp_cFlag <= 20)
			//{
			//	if (_tmp_cFlag > 0 && _tmp_cFlag < 5)
			//		wsprintfA(G_cTxt, "Wanted (1)");
			//	else if (_tmp_cFlag >= 5 && _tmp_cFlag < 10)
			//		wsprintfA(G_cTxt, "Wanted (2)");
			//	else if (_tmp_cFlag >= 10 && _tmp_cFlag < 15)
			//		wsprintfA(G_cTxt, "Wanted (3)");
			//	else if (_tmp_cFlag >= 15 && _tmp_cFlag < 20)
			//		wsprintfA(G_cTxt, "Wanted (4)");
			//	else if (_tmp_cFlag == 20)
			//		wsprintfA(G_cTxt, "Wanted (5)");
			//	PutString2(sX, sY+14 +iAddY, G_cTxt, 0,200,200);
			//	iAddY += 14;
			//}
		}
	}
	else
	{
		//AddEventList("request title", 10);
		bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQTITLE, 0, _tmp_wObjectID, iTitleIndex, 0, 0);
	}
#endif

	//if(strcpy(m_cMapName, ARENAMAP) == 0)
	//{
	//   strcpy(cTxt, "Enemy");
	//   PutString2(sX, sY+14 +iAddY, cTxt, 255, 0, 0);
	//}
	//else
	//{
	//   PutString2(sX, sY+14 +iAddY, cTxt, sR, sG, sB);
	//   iAddY += 14;
	//}
	// Add m_adminLevel
	/*if(m_adminLevel == 1)
	{
		sR = 0;
		sG = 255;
		sB = 115;
		strcpy(cTxt, "GM Helper");
		PutString2(sX, sY+14 +iAddY, cTxt, sR, sG, sB);
		iAddY += 14;
	}*/

#ifdef _DEBUG
	wsprintfA(G_cTxt,"Status: 0x%.8X ",iStatus);
	PutString2(sX+70, sY+(14*0), G_cTxt, 30,255,30);
#endif
}

bool CGame::FindGuildName(char* pName, int* ipIndex)
{
	int i, iRet = 0;
	uint32_t dwTmpTime;
	for( i=0 ; i < MAXGUILDNAMES ; i++ )
	{
		if( memcmp(m_stGuildName[i].cCharName, pName, 10) == 0 )
		{
			m_stGuildName[i].dwRefTime = m_dwCurTime;
			*ipIndex = i;
			return true;
		}
	}
	dwTmpTime = m_stGuildName[0].dwRefTime;
	for( i=0 ; i < MAXGUILDNAMES ; i++ )
	{
		if( m_stGuildName[i].dwRefTime < dwTmpTime )
		{
			iRet = i;
			dwTmpTime = m_stGuildName[i].dwRefTime;
		}
	}
	ZeroMemory( m_stGuildName[iRet].cGuildName, sizeof(m_stGuildName[iRet].cGuildName) );
	memcpy( m_stGuildName[iRet].cCharName, pName, 10 );
	m_stGuildName[iRet].dwRefTime = m_dwCurTime;
	m_stGuildName[iRet].iGuildRank = -1;
	*ipIndex = iRet;
	return false;
}

void CGame::DrawVersion(bool bAuthor)
{
	wsprintfA(G_cTxt, "Version %d.%d%d", HBF_MAJOR, HBF_MINOR, HBF_LOWER);
	PutFontString(font[FONT_TREBMS8PX], 34, 463 + 63, G_cTxt, SColor(255, 255, 255, 255));
}

char CGame::GetOfficialMapName(char const * const pMapName, char * pName)
{	// MapIndex
	if (strcmp(pMapName, "middleland") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME28);	// Middleland
		return 4;
	}else if (strcmp(pMapName, "huntzone3") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME31);	// Death Valley
		return 0;
	}else if (strcmp(pMapName, "huntzone1") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME29);	// Rocky Highland
		return 1;
	}else if (strcmp(pMapName, "elvuni") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME57);	// Eldiniel Garden
		return 2;
	}else if (strcmp(pMapName, "elvine") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME24);	// Elvine City
		return 3;
	}else if (strcmp(pMapName, "elvfarm") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME2);	// Elvine Farm
		return 5;
	}else if (strcmp(pMapName, "arefarm") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME1);	// Aresden Farm
		return 6;
	}else if (strcmp(pMapName, "default") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME3);	// Beginner Zone
		return 7;
	}else if (strcmp(pMapName, "huntzone4") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME32);	// Silent Wood
		return 8;
	}else if (strcmp(pMapName, "huntzone2") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME30);	// Eternal Field
		return 9;
	}else if (strcmp(pMapName, "areuni") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME56);	// Aresien Garden
		return 10;
	}else if (strcmp(pMapName, "aresden") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME22);	// Aresden City
		return 11;
	}else if (strcmp(pMapName, "dglv2") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME25);	// Dungeon L2
		return 12;
	}else if (strcmp(pMapName, "dglv3") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME26);	// Dungeon L3
		return 13;
	}else if (strcmp(pMapName, "dglv4") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME53);	// Dungeon L4
		return 14;
	}else if (strcmp(pMapName, "elvined1") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME23);	// Elvine Dungeon
		return 15;
	}else if (strcmp(pMapName, "aresdend1") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME21);	// Aresden Dungeon
		return 16;
	}else if (strcmp(pMapName, "bisle") == 0) {
		strcpy(pName, GET_OFFICIAL_MAP_NAME27);	// Bleeding Island
		return 17;
	}else if (strcmp(pMapName, "toh1") == 0) {
		strcpy(pName, GET_OFFICIAL_MAP_NAME60);	// Tower of Hell 1
		return 18;
	}else if (strcmp(pMapName, "toh2") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME61);	// Tower of Hell 2
		return 19;
	}else if (strcmp(pMapName, "toh3") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME62);	// Tower of Hell 3
		return 20;
	}else if (strcmp(pMapName, "middled1x") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME58);	// Middleland Mine
		return 21;
	}else if (strcmp(pMapName, "middled1n") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME59);	// Middleland Dungeon
		return 22;
	}else if (strcmp(pMapName, "2ndmiddle") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME65);	// Promiseland
		return 23;
	}else if (strcmp(pMapName, "icebound") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME66);	// Ice Map
		return 24;
	// Snoopy:
	}else if (strcmp(pMapName, "druncncity") == 0) // Snoopy: Apocalypse maps
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME70);
		return 25;
	}else if (strcmp(pMapName, "inferniaA") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME71);
		return 26;
	}else if (strcmp(pMapName, "inferniaB") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME72);
		return 27;
	}else if (strcmp(pMapName, "maze") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME73);
		return -1; //28;
	}else if (strcmp(pMapName, "procella") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME74);
		return 29;
	}else if (strcmp(pMapName, "abaddon") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME75);
		return 30;
	}else if (strcmp(pMapName, "BtField") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME76);
		return 35;
	}else if (strcmp(pMapName, "GodH") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME77);
		return 36;
	}else if (strcmp(pMapName, "HRampart") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME78);
		return 37;
	}else if (strcmp(pMapName, "cityhall_1") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME35);	// Aresden Cityhall
		return -1;
	}else if (strcmp(pMapName, "cityhall_2") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME36);	// Elvine Cityhall
		return -1;
	}else if (strcmp(pMapName, "gldhall_1") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME37);	// Aresden Guildhall
		return -1;
	}else if (strcmp(pMapName, "gldhall_2") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME38);	// Elvine Guildhall
		return -1;
	}else if (memcmp(pMapName, "bsmith_1", 8) == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME33);	// Aresden Blacksmith
		return -1;
	}else if (memcmp(pMapName, "bsmith_2", 8) == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME34);	// Elvine Blacksmith
		return -1;
	}else if (memcmp(pMapName, "gshop_1", 7) == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME39);	// Aresden Shop
		return -1;
	}else if (memcmp(pMapName, "gshop_2", 7) == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME40);	// Elvine Shop
		return -1;
	}else if (memcmp(pMapName, "wrhus_1", 7) == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME43);	// Aresden Warehouse
		return -1;
	}else if (memcmp(pMapName, "wrhus_2", 7) == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME44);	// Elvine Warehouse
		return -1;
	}else if (strcmp(pMapName, "arewrhus") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME45);	// Aresden Warehouse
		return -1;
	}else if (strcmp(pMapName, "elvwrhus") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME46);	// Elvine Warehouse
		return -1;
	}
	else if (strcmp(pMapName, "wzdtwr_1") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME41);	// Magic Tower
		return -1;
	}else if (strcmp(pMapName, "wzdtwr_2") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME42);	// Magic Tower
		return -1;
	}else if (strcmp(pMapName, "cath_1") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME47);	// Aresien Church
		return -1;
	}else if (strcmp(pMapName, "cath_2") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME48);	// Eldiniel Church
		return -1;
	}
	else if (memcmp(pMapName, "resurr", 6) == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME54);	// Revival Zone
		return -1;
	}
	else if (strcmp(pMapName, "arebrk11") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME4);	// Aresden Barrack 1
		return -1;
	}else if (strcmp(pMapName, "arebrk12") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME5);	// Aresden Barrack 1
		return -1;
	}else if (strcmp(pMapName, "arebrk21") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME6);	// Aresden Barrack 2
		return -1;
	}else if (strcmp(pMapName, "arebrk22") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME7);	// Aresden Barrack 2
		return -1;
	}else if (strcmp(pMapName, "elvbrk11") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME8);	// Elvine Barrack 1
		return -1;
	}else if (strcmp(pMapName, "elvbrk12") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME9);	// Elvine Barrack 1
		return -1;
	}else if (strcmp(pMapName, "elvbrk21") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME10);	// Elvine Barrack 2
		return -1;
	}else if (strcmp(pMapName, "elvbrk22") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME11);	// Elvine Barrack 2
		return -1;
	}
	else if (strcmp(pMapName, "fightzone1") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME12);	// Arena 1
		return -1;
	}else if (strcmp(pMapName, "fightzone2") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME13);	// Arena 2
		return -1;
	}else if (strcmp(pMapName, "fightzone3") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME14);	// Arena 3
		return -1;
	}else if (strcmp(pMapName, "fightzone4") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME15);	// Arena 4
		return -1;
	}else if (strcmp(pMapName, "fightzone5") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME16);	// Arena 5
		return -1;
	}else if (strcmp(pMapName, "fightzone6") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME17);	// Arena 6
		return -1;
	}else if (strcmp(pMapName, "fightzone7") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME18);	// Arena 7
		return -1;
	}else if (strcmp(pMapName, "fightzone8") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME19);	// Arena 8
		return -1;
	}else if (strcmp(pMapName, "fightzone9") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME20);	// Arena 9
		return -1;
	}else if (strcmp(pMapName, "arejail") == 0)
	{	strcpy(pName, GET_OFFICIAL_MAP_NAME63);	// Aresden Jail
		return -1;
	}else if (strcmp(pMapName, "elvjail") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME64);	// Elvine Jail
		return -1;
	}
	else if (memcmp(pMapName, "CmdHall", 7) == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME79);
		return -1;
	}
	else if (strcmp(pMapName, "Barracks1") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME80);
		return -1;
	}else if (strcmp(pMapName, "Barracks2") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME81);
		return -1;
	}else if (strcmp(pMapName, "Bsmith") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME82);
		return -1;
	}
	else if (strcmp(pMapName, "default2") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME83);
		return -1;
	}
	else if (strcmp(pMapName, "astoria") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME84);
		return 44;
	}
	// Fantasy Maps xRisenx
	else if (strcmp(pMapName, "safeshop") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME85);
		return -1;
	}
	//else if (strcmp(pMapName, "pvpbarrack") == 0)
	//{
		//strcpy(pName, GET_OFFICIAL_MAP_NAME86);
		//return -1;
	//}
	//else if (strcmp(pMapName, "darkcrypt") == 0)
	//{
		//strcpy(pName, GET_OFFICIAL_MAP_NAME87);
		//return -1;
	//}
	//else if (strcmp(pMapName, "pvpbar2") == 0)
	//{
		//strcpy(pName, GET_OFFICIAL_MAP_NAME88);
		//return -1;
	//}
	else if (strcmp(pMapName, "arebar") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME86);
		return -1;
	}
	else if (strcmp(pMapName, "elvbar") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME87);
		return -1;
	}
	else if (strcmp(pMapName, "outlands") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME88);
		return 31;
	}
	else if (strcmp(pMapName, "catacombs") == 0)
	{
		strcpy(pName, GET_OFFICIAL_MAP_NAME89);
		return 32;
	}
	//else if (strcmp(pMapName, "GladArena") == 0)
	//{
	//	strcpy(pName, GET_OFFICIAL_MAP_NAME90); // Gladiator Arena xRisenx
	//	return 32;
	//}
	else
	{
		strcpy(pName, pMapName);
		toupper(pName[0]);
		return -1;
	}
}

bool CGame::bCheckLocalChatCommand(char const * const pMsg)
{
	class  CStrTok * pStrTok = 0;
	char   * token, cBuff[256], cTxt[120], cName[12];
	char   seps[] = " \t\n";

	ZeroMemory(cBuff, sizeof(cBuff));
	ZeroMemory(cName, sizeof(cName));
	strcpy(cBuff, pMsg);
	if (memcmp(cBuff, "/showframe", 10)==0 || memcmp(cBuff, "/fps", 4)==0)
	{
		m_bShowFPS = !m_bShowFPS;
		return true;
	}else if (memcmp(cBuff, "/showtime", 9)==0)
	{
		m_showTime = !m_showTime;
		return true;
	}else if (memcmp(cBuff, "/timestamp", 10)==0)
	{
		if(!m_showTimeStamp)
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND10, 10);
		else
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND11, 10);
		m_showTimeStamp = !m_showTimeStamp;
		return true;
	}else if (memcmp(cBuff, "/showgrid", 9)==0 || memcmp(cBuff, "/grid", 5)==0)
	{
		m_showGrid = !m_showGrid;
		return true;
	}else if (memcmp(cBuff, "/showalldmg", 11)==0)
	{
		if(!m_showAllDmg)
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND12, 10);
		else
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND13, 10);
		m_showAllDmg = !m_showAllDmg;
		return true;
	}else if (memcmp(cBuff, "/bigitems", 9)==0)
	{
		if(!m_bigItems)
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND14, 10);
		else
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND15, 10);
		m_bigItems = !m_bigItems;
		return true;
	}else if (memcmp(cBuff, "/ekscreenshot", 13)==0)
	{
		if(!m_ekScreenshot)
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND16, 10);
		else
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND17, 10);
		m_ekScreenshot = !m_ekScreenshot;
		return true;
	}else if (memcmp(cBuff, "/notifyme", 9)==0)
	{
		if(!m_tabbedNotification)
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND18, 10);
		else
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND19, 10);
		m_tabbedNotification = !m_tabbedNotification;
		return true;
	}else if (memcmp(cBuff, "/manufill", 9)==0)
	{
		if(!m_manuAutoFill)
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND20, 10);
		else
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND21, 10);
		m_manuAutoFill = !m_manuAutoFill;
		return true;
	}else if (memcmp(cBuff, "/enabletogglescreen", 19)==0)
	{	m_bToggleScreen = true;
		return true;
	}
	else if (memcmp(cBuff, "/whon", 5) == 0)
	{	m_bWhisper = true;
	    AddEventList(BCHECK_LOCAL_CHAT_COMMAND6, 10);// Enable to listen to whispers."
		return true;
	}else if (memcmp(cBuff, "/whoff", 6) == 0)
	{	m_bWhisper = false;
	    AddEventList(BCHECK_LOCAL_CHAT_COMMAND7, 10);//
		return true;
	}else if (memcmp(cBuff, "/shon", 5) == 0)
	{	m_bShout = true;
	    AddEventList(BCHECK_LOCAL_CHAT_COMMAND8, 10); //Enalbe to chat in public."
		return true;
	}else if (memcmp(cBuff, "/shoff", 6) == 0)
	{	m_bShout = false;
	    AddEventList(BCHECK_LOCAL_CHAT_COMMAND9, 10); //Unable to chat in public."
		return true;
	}
	if (memcmp(cBuff, "/tooff", 6) == 0)
	{	pStrTok = new class CStrTok(cBuff, seps);
		token = pStrTok->pGet();
		token = pStrTok->pGet();
		if (token != 0)
		{	if (strlen(token) <= 10)
			{	strcpy(cName, token);
				if (memcmp(m_cPlayerName, cName, 10) == 0) {
					AddEventList(BCHECK_LOCAL_CHAT_COMMAND2, 10);
					if (pStrTok != 0) delete pStrTok;
					return true;
				}
				else if(m_MuteList.find(string(cName)) == m_MuteList.end()){
					wsprintfA(cTxt, BCHECK_LOCAL_CHAT_COMMAND3, token);
					AddEventList(cTxt, 10);
					m_MuteList.insert(string(cName));
					SaveMuteList();
				}
   			}
			else AddEventList(BCHECK_LOCAL_CHAT_COMMAND5, 10);
		}
		if (pStrTok != 0) delete pStrTok;
		return true;
	}else if (memcmp(cBuff, "/toon", 5) == 0)
	{	pStrTok = new class CStrTok(cBuff, seps);
		token = pStrTok->pGet();
		token = pStrTok->pGet();
		if (token != 0) {
			if (strlen(token) <= 10) {
				if (m_MuteList.erase(string(token))) {
					wsprintfA(cTxt, BCHECK_LOCAL_CHAT_COMMAND1, token);
					AddEventList(cTxt, 10);
					SaveMuteList();
				}
   			}else AddEventList(BCHECK_LOCAL_CHAT_COMMAND5, 10);
		}
		if (pStrTok != 0) delete pStrTok;
		return true;
	}
	else if (memcmp(cBuff, "/showquest", 10)==0)
	{
		if(!m_displayQuest)
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND22, 10);
		else
			AddEventList(BCHECK_LOCAL_CHAT_COMMAND23, 10);
		m_displayQuest = !m_displayQuest;
		return true;
	}
#ifdef _DEBUG
	if (memcmp(cBuff, "/showdlg", 8) == 0)
	{	
		pStrTok = new CStrTok(cBuff, seps);
		token = pStrTok->pGet();
		token = pStrTok->pGet();
		if (token != 0)
		{	
			int dlg = atoi(token);
			token = pStrTok->pGet();
			if (token != 0) {
				EnableDialogBox(dlg, atoi(token), 0, 0);
				m_dialogBoxes[dlg].SetMode(atoi(token));
			}
		}
	if (pStrTok != 0) delete pStrTok;
		return true;
	}
	else if (memcmp(cBuff, "/showbtns", 9) == 0)
	{	
		m_showBtns = !m_showBtns;
		return true;
	}

#endif

	return false;
}

bool CGame::bCheckItemOperationEnabled(char cItemID)
{
	if (m_pItemList[cItemID] == 0) return false;
	if (m_cCommand < 0) return false;
	if (m_bIsTeleportRequested == true) return false;
	if (m_bIsItemDisabled[cItemID] == true) return false;

	if ((m_pItemList[cItemID]->m_sSpriteFrame == 155) && (m_bUsingSlate == true))
	{	if ((m_cMapIndex==35)||(m_cMapIndex==36)||(m_cMapIndex==37))
		{	AddEventList(MSG_NOTIFY_SLATE_WRONG_MAP, 10); // "You cannot use it right here."
			return false;
		}
		AddEventList(MSG_NOTIFY_SLATE_ALREADYUSING, 10); // Already Using Another Slate
		return false;
	}

	if ( m_bIsDialogEnabled[17] == true )
	{	AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
		return false;
	}

	if (m_bIsDialogEnabled[20] == true)
	{	AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
		return false;
	}

	if (m_bIsDialogEnabled[23] == true)
	{	AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
		return false;
	}

	if (m_bIsDialogEnabled[26] == true)
	{	AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
		return false;
	}

	if (m_bIsDialogEnabled[27] == true)
	{	AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
		return false;
	}

	if (m_bIsDialogEnabled[31] == true)
	{	AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
		return false;
	}

	if (m_bIsDialogEnabled[4] == true)
	{	AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
		return false;
	}

	return true;
}

void CGame::ClearSkillUsingStatus()
{
	if (m_bSkillUsingStatus == true)
	{	AddEventList(CLEAR_SKILL_USING_STATUS1, 10);//"
		DisableDialogBox(24);
		DisableDialogBox(26);
		if ((m_sPlayerType >= 1) && (m_sPlayerType <= 6)/* && ((m_sPlayerAppr2 & 0xF000) == 0)*/) {
			m_cCommand = OBJECTSTOP;
			m_sCommX = m_sPlayerX;
			m_sCommY = m_sPlayerY;
		}
	}
	m_bSkillUsingStatus = false;
}


void CGame::NpcTalkHandler(char *pData)
{
 char  * cp, cRewardName[21], cTargetName[21], cTemp[21], cTxt[250];
 short * sp, sType, sResponse;
 int     iAmount, iIndex, iContribution, iX, iY, iRange;
 int     iTargetType, iTargetCount, iQuestionType;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	sp = (short *)cp;
	sType = *sp;
	cp += 2;
	sp = (short *)cp;
	sResponse = *sp;
	cp += 2;
	sp = (short *)cp;
	iAmount = *sp;
	cp += 2;
	sp = (short *)cp;
	iContribution = *sp;
	cp += 2;
	sp = (short *)cp;
	iTargetType = *sp;
	cp += 2;
	sp = (short *)cp;
	iTargetCount = *sp;
	cp += 2;
	sp = (short *)cp;
	iX = *sp;
	cp += 2;
	sp = (short *)cp;
	iY = *sp;
	cp += 2;
	sp = (short *)cp;
	iRange = *sp;
	cp += 2;
	ZeroMemory(cRewardName, sizeof(cRewardName));
	memcpy(cRewardName, cp, 20);
	cp += 20;
	ZeroMemory(cTargetName, sizeof(cTargetName));
	memcpy(cTargetName, cp, 20);
	cp += 20;
	EnableDialogBox(21, sResponse, sType, 0);

	if ((sType >= 1) && (sType <= 100))
	{	
		iIndex = m_dialogBoxes[21].sV1;
		m_pMsgTextList2[iIndex] = new class CMsg(0, "  ", 0);
		iIndex++;
		iQuestionType = 0;
		switch (sType) {
		case 1: //Monster Hunt
			ZeroMemory(cTemp, sizeof(cTemp));
			GetNpcName(iTargetType, cTemp);
			ZeroMemory(cTxt, sizeof(cTxt));
			wsprintfA(cTxt, NPC_TALK_HANDLER16, iTargetCount, cTemp);
			m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
			iIndex++;

			ZeroMemory(cTxt, sizeof(cTxt));
			if (memcmp(cTargetName, "NONE", 4) == 0) {
				strcpy(cTxt, NPC_TALK_HANDLER17);//"
				m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
				iIndex++;
			}
			else {
				ZeroMemory(cTemp, sizeof(cTemp));
				GetOfficialMapName(cTargetName, cTemp);
				wsprintfA(cTxt, NPC_TALK_HANDLER18, cTemp);//"Map : %s"
				m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
				iIndex++;

				if (iX != 0) {
					ZeroMemory(cTxt, sizeof(cTxt));
					wsprintfA(cTxt, NPC_TALK_HANDLER19, iX, iY, iRange);//"Position: %d,%d within %d blocks"
					m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
					iIndex++;
				}

				ZeroMemory(cTxt, sizeof(cTxt));
				wsprintfA(cTxt, NPC_TALK_HANDLER20, iContribution);//"
				m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
				iIndex++;
			}
			iQuestionType = 1;
			break;

		case 7: //
			ZeroMemory(cTxt, sizeof(cTxt));
			m_pMsgTextList2[iIndex] = new class CMsg(0, NPC_TALK_HANDLER21, 0);
			iIndex++;

			ZeroMemory(cTxt, sizeof(cTxt));
			if (memcmp(cTargetName, "NONE", 4) == 0) {
				strcpy(cTxt, NPC_TALK_HANDLER22);
				m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
				iIndex++;
			}
			else {
				ZeroMemory(cTemp, sizeof(cTemp));
				GetOfficialMapName(cTargetName, cTemp);
				wsprintfA(cTxt, NPC_TALK_HANDLER23, cTemp);
				m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
				iIndex++;

				if (iX != 0) {
					ZeroMemory(cTxt, sizeof(cTxt));
					wsprintfA(cTxt, NPC_TALK_HANDLER24, iX, iY, iRange);
					m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
					iIndex++;
				}

				ZeroMemory(cTxt, sizeof(cTxt));
				wsprintfA(cTxt, NPC_TALK_HANDLER25, iContribution);
				m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
				iIndex++;
			}
			iQuestionType = 1;
			break;

		case 10: // Crusade
			ZeroMemory(cTxt, sizeof(cTxt));
			m_pMsgTextList2[iIndex] = new class CMsg(0, NPC_TALK_HANDLER26, 0);
			iIndex++;

			ZeroMemory(cTxt, sizeof(cTxt));
            strcpy(cTxt, NPC_TALK_HANDLER27);//"
			m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
			iIndex++;

			ZeroMemory(cTxt, sizeof(cTxt));
            strcpy(cTxt, NPC_TALK_HANDLER28);//"
			m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
			iIndex++;

			ZeroMemory(cTxt, sizeof(cTxt));
            strcpy(cTxt, NPC_TALK_HANDLER29);//"
			m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
			iIndex++;

			ZeroMemory(cTxt, sizeof(cTxt));
            strcpy(cTxt, NPC_TALK_HANDLER30);//"
			m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
			iIndex++;

			ZeroMemory(cTxt, sizeof(cTxt));
			strcpy(cTxt, " ");
			m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
			iIndex++;

			ZeroMemory(cTxt, sizeof(cTxt));
			if (memcmp(cTargetName, "NONE", 4) == 0) {
				strcpy(cTxt, NPC_TALK_HANDLER31);//"
				m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
				iIndex++;
			}
			else {
				ZeroMemory(cTemp, sizeof(cTemp));
				GetOfficialMapName(cTargetName, cTemp);
				wsprintfA(cTxt, NPC_TALK_HANDLER32, cTemp);//"
				m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
				iIndex++;
			}
			iQuestionType = 2;
			break;
		}

		switch (iQuestionType) {
		case 1:
			m_pMsgTextList2[iIndex] = new class CMsg(0, "  ", 0);
			iIndex++;
			m_pMsgTextList2[iIndex] = new class CMsg(0, NPC_TALK_HANDLER33, 0);//"
			iIndex++;
			m_pMsgTextList2[iIndex]  = new class CMsg(0, NPC_TALK_HANDLER34, 0);//"
			iIndex++;
			m_pMsgTextList2[iIndex] = new class CMsg(0, "  ", 0);
			iIndex++;
			break;

		case 2:
			m_pMsgTextList2[iIndex] = new class CMsg(0, "  ", 0);
			iIndex++;
			m_pMsgTextList2[iIndex] = new class CMsg(0, NPC_TALK_HANDLER35, 0);//"
			iIndex++;
			m_pMsgTextList2[iIndex] = new class CMsg(0, "  ", 0);
			iIndex++;
			break;

		default: break;
		}
	}
}

void CGame::GetNpcName(short sType, char *pName)
{
	switch (sType)
	{
	case 10: strcpy(pName, NPC_NAME_SLIME); break;
	case 11: strcpy(pName, NPC_NAME_SKELETON); break;
	case 12: strcpy(pName, NPC_NAME_STONEGOLEM); break;
	case 13: strcpy(pName, NPC_NAME_CYCLOPS); break;
	case 14: strcpy(pName, NPC_NAME_ORC); break;
	case 15: strcpy(pName, NPC_NAME_SHOP_KEEPER); break;
	case 16: strcpy(pName, NPC_NAME_GIANTANT); break;
	case 17: strcpy(pName, NPC_NAME_GIANTSCORPION); break;
	case 18: strcpy(pName, NPC_NAME_ZOMBIE); break;
	case 19: strcpy(pName, NPC_NAME_MAGICIAN); break;
	case 20: strcpy(pName, NPC_NAME_WAREHOUSE_KEEPER); break;
	case 21: strcpy(pName, NPC_NAME_GUARD); break;
	case 22: strcpy(pName, NPC_NAME_SNAKE); break;
	case 23: strcpy(pName, NPC_NAME_CLAYGOLEM); break;
	case 24: strcpy(pName, NPC_NAME_BLACKSMITH_KEEPER); break;
	case 25: strcpy(pName, NPC_NAME_CITYHALL_OFFICER); break;
	case 26: strcpy(pName, NPC_NAME_GUILDHALL_OFFICER); break;
	case 27: strcpy(pName, NPC_NAME_HELHOUND); break;
	case 28: strcpy(pName, NPC_NAME_TROLL); break;
	case 29: strcpy(pName, NPC_NAME_OGRE); break;
	case 30: strcpy(pName, NPC_NAME_LICHE); break;
	case 31: strcpy(pName, NPC_NAME_DEMON); break;
	case 32: strcpy(pName, NPC_NAME_UNICORN); break;
	case 33: strcpy(pName, NPC_NAME_WEREWOLF); break;
	case 34: strcpy(pName, NPC_NAME_DUMMY); break;
	case 35: strcpy(pName, NPC_NAME_ENERGYSPHERE); break;
	case 36:
		if (_tmp_sAppr2 != 0) strcpy(pName, NPC_NAME_ARROWGUARDTOWER_CK);
		else strcpy(pName, NPC_NAME_ARROWGUARDTOWER);
		break;
	case 37:
		if (_tmp_sAppr2 != 0) strcpy(pName, NPC_NAME_CANNONGUARDTOWER_CK);
		else strcpy(pName, NPC_NAME_CANNONGUARDTOWER);
		break;
	case 38:
		if (_tmp_sAppr2 != 0) strcpy(pName, NPC_NAME_MANACOLLECTOR_CK);
		else strcpy(pName, NPC_NAME_MANACOLLECTOR);
		break;
	case 39:
		if (_tmp_sAppr2 != 0) strcpy(pName, NPC_NAME_DETECTOR_CK);
		else strcpy(pName, NPC_NAME_DETECTOR);
		break;
	case 40: strcpy(pName, NPC_NAME_ENERGYSHIELD); break;
	case 41: strcpy(pName, NPC_NAME_GRANDMAGICGENERATOR); break;
	case 42: strcpy(pName, NPC_NAME_MANASTONE); break;
	case 43: strcpy(pName, NPC_NAME_LIGHTWARBEETLE); break;
	case 44: strcpy(pName, NPC_NAME_GODSHANDKNIGHT); break;
	case 45: strcpy(pName, NPC_NAME_GODSHANDKNIGHT_CK); break;
	case 46: strcpy(pName, NPC_NAME_TEMPLEKNIGHT); break;
	case 47: strcpy(pName, NPC_NAME_BATTLEGOLEM); break;
	case 48: strcpy(pName, NPC_NAME_STALKER); break;
	case 49: strcpy(pName, NPC_NAME_HELLCLAW); break;
	case 50: strcpy(pName, NPC_NAME_TIGERWORM); break;
	case 51: strcpy(pName, NPC_NAME_CATAPULT); break;
	case 52: strcpy(pName, NPC_NAME_GARGOYLE); break;
	case 53: strcpy(pName, NPC_NAME_BEHOLDER); break;
	case 54: strcpy(pName, NPC_NAME_DARKELF); break;
	case 55: strcpy(pName, NPC_NAME_RABBIT); break;
	case 56: strcpy(pName, NPC_NAME_CAT); break;
	case 57: strcpy(pName, NPC_NAME_FROG); break;
	case 58: strcpy(pName, NPC_NAME_MOUNTAIN_GIANT); break;
	case 59: strcpy(pName, NPC_NAME_ETTIN); break;
	case 60: strcpy(pName, NPC_NAME_CANNIBAL); break;
	case 61: strcpy(pName, NPC_NAME_RUDOLPH); break;
	case 62: strcpy(pName, NPC_NAME_DIREBOAR); break;
	case 63: strcpy(pName, NPC_NAME_FROST); break;
	case 64:
		{	switch((_tmp_sAppr2 & 0xFF00)>>8)	{
			case 1:	strcpy(pName, NPC_NAME_WATERMELON);	break;
			case 2: strcpy(pName, NPC_NAME_PUMPKIN); break;
			case 3: strcpy(pName, NPC_NAME_GARLIC); break;
			case 4: strcpy(pName, NPC_NAME_BARLEY); break;
			case 5:	strcpy(pName, NPC_NAME_CARROT); break;
			case 6: strcpy(pName, NPC_NAME_RADISH); break;
			case 7: strcpy(pName, NPC_NAME_CORN); break;
			case 8: strcpy(pName, NPC_NAME_BFLOWER); break;
			case 9: strcpy(pName, NPC_NAME_MELON); break;
			case 10: strcpy(pName, NPC_NAME_TOMATO); break;
			case 11: strcpy(pName, NPC_NAME_GRAPPE); break;
			case 12: strcpy(pName, NPC_NAME_BLUEGRAPPE); break;
			case 13: strcpy(pName, NPC_NAME_MUSHROM); break;
			case 14: strcpy(pName, NPC_NAME_GINSENG); break;
			default: strcpy(pName, NPC_NAME_CROP); break;
			}
		}
		break;
	case 65: strcpy(pName, NPC_NAME_ICEGOLEM); break;
	case 66: strcpy(pName, NPC_NAME_WYVERN); break;
	case 67: strcpy(pName, NPC_NAME_MCGAFFIN); break;
	case 68: strcpy(pName, NPC_NAME_PERRY); break;
	case 69: strcpy(pName, NPC_NAME_DEVLIN); break;

	case 70: strcpy(pName, NPC_NAME_DRAGON); break;
	case 71: strcpy(pName, NPC_NAME_CENTAUR); break;
	case 72: strcpy(pName, NPC_NAME_CLAWTUR); break;
	case 73: strcpy(pName, NPC_NAME_FIREWYV); break;
	case 74: strcpy(pName, NPC_NAME_GICRAYF); break;
	case 75: strcpy(pName, NPC_NAME_GILIZAR); break;
	case 76: strcpy(pName, NPC_NAME_GITREE); break;
	case 77: strcpy(pName, NPC_NAME_MASTORC); break;
	case 78: strcpy(pName, NPC_NAME_MINAUS); break;
	case 79: strcpy(pName, NPC_NAME_NIZIE); break;

	case 80: strcpy(pName, NPC_NAME_TENTOCL); break;
	case 81: strcpy(pName, NPC_NAME_ABADDON); break;
	case 82: strcpy(pName, NPC_NAME_SORCERS); break;
	case 83: strcpy(pName, NPC_NAME_ATK); break;
	case 84: strcpy(pName, NPC_NAME_MASTELF); break;
	case 85: strcpy(pName, NPC_NAME_DSK); break;
	case 86: strcpy(pName, NPC_NAME_HBT); break;
	case 87: strcpy(pName, NPC_NAME_CT); break;
	case 88: strcpy(pName, NPC_NAME_BARBAR); break;
	case 89: strcpy(pName, NPC_NAME_AGC); break;
	case 90: strcpy(pName, NPC_NAME_GAIL); break;
	case 91: strcpy(pName, NPC_NAME_GATE); break;
	case 102: strcpy(pName, NPC_NAME_GUILD_WAREHOUSE_KEEPER); break;
	case 103: strcpy(pName, NPC_NAME_ASTORIASHIELD); break;
	//case 95: strcpy(pName, NPC_NAME_ORCLORD); break; // Orc Lord
	case 104: strcpy(pName, NPC_NAME_DARKUNICORN); break; // DarkUnicorn
	case 105: strcpy(pName, NPC_NAME_BLACKDEMON); break;// Black Demon
	case 106: strcpy(pName, NPC_NAME_EARTHWYV); break;// Earth Wyvern
	case 107: strcpy(pName, NPC_NAME_WINDWYV); break;// Wind Wyvern
	case 108: strcpy(pName, NPC_NAME_DARKWYV); break;// Dark Wyvern
	case 109: strcpy(pName, NPC_NAME_GRASSWYV); break;// Grass Wyvern
	case 110: strcpy(pName, NPC_NAME_SKYWYV); break;// Sky Wyvern
	case 111: strcpy(pName, NPC_NAME_SEAWYV); break;// Sea Wyvern
	case 112: strcpy(pName, NPC_NAME_METALWYV); break;// Metal Wyvern
	case 113: strcpy(pName, NPC_NAME_VOIDWYV); break;// Void Wyvern
	case 114: strcpy(pName, NPC_NAME_ETERNALDRAGON); break;// Eternal Dragon
	case 122: strcpy(pName, NPC_NAME_ENRAGEDTROLL); break;// Enraged Troll
	case 123: strcpy(pName, NPC_NAME_ENRAGEDCYCLOPS); break;// Enraged Cyclops
	case 124: strcpy(pName, NPC_NAME_ENRAGEDSTALKER); break;// Enraged Stalker
	case 125: strcpy(pName, NPC_NAME_ENRAGEDGARGOYLE); break;// Enraged Gagoyle
	case 126: strcpy(pName, NPC_NAME_ENRAGEDHELLCLAW); break;// Enraged Hellclaw
	case 127: strcpy(pName, NPC_NAME_ENRAGEDTIGERWORM); break;// Enraged Tigerworm

	//case 110: strcpy(pName, NPC_NAME_AIRLEMENTAL); break;
	}
}
std::vector<string> * CGame::GetItemName(CItem * item, bool isWH)
{
	m_itemColor =video::SColor(255,255,255,255);

	static std::vector<string> lines;
	lines.clear();

	string str("");
	
	if(strcmp(item->m_cName, "ScrollofRenown") == 0)
	{
		bool foundName = false;
		for(int i = 0; i < MAXITEMNAMES; i++)
		{
			if(m_pItemNameList[i] && strcmp(m_pItemNameList[i]->m_cOriginName, item->m_cName) == 0)
			{
				foundName = true;
				str = m_pItemNameList[i]->m_cName;
				break;
			}
		}

		if(!foundName)
		{
			str = item->m_cName;
		}
		sprintf(G_cTxt, " (%d)", item->m_dwAttribute);
		str += G_cTxt;
		lines.push_back(str);

		sprintf(G_cTxt, GET_ITEM_NAME39, item->m_dwAttribute);
		lines.push_back(G_cTxt);
		return &lines;
	}

	if(item->m_dwCount > 1 && !item->IsManued())
	{
		wsprintfA(G_cTxt, "%u ", item->m_dwCount);
		str.append(G_cTxt);
	}

	if(item->IsVortexed())
	{
		str.append("Vortexed ");
		m_itemColor =video::SColor(255,132,132,180);
	}

	switch(GetNibble(item->m_dwAttribute, 5))
	{
	case 1: str.append( GET_ITEM_NAME3 );break; // Critical
	case 2: str.append( GET_ITEM_NAME4 );break; // Poisoning
	case 3: str.append( GET_ITEM_NAME5 );break; // Righteous
	case 4: break;
	case 5: str.append( GET_ITEM_NAME6 );break; // Agile
	case 6: str.append( GET_ITEM_NAME7 );break; // Light
	case 7: str.append( GET_ITEM_NAME8 );break; // Sharp
	case 8: str.append( GET_ITEM_NAME9 );break; // Strong
	case 9: str.append( GET_ITEM_NAME10 );break; // Ancient
	case 10: str.append( GET_ITEM_NAME11 );break; // Special
	case 11: str.append( GET_ITEM_NAME12 );break; // Mana Converting
	case 12: str.append( GET_ITEM_NAME13 );break; // Critical
	case 13: str.append( GET_ITEM_NAME40 );break; // Flawless Sharp
	case 14: str.append( GET_ITEM_NAME41 );break; // Flawless Ancient
	}

	bool foundName = false;
	for(int i = 0; i < MAXITEMNAMES; i++)
	{
		if(m_pItemNameList[i] && strcmp(m_pItemNameList[i]->m_cOriginName, item->m_cName) == 0)
		{
			foundName = true;
			str.append(m_pItemNameList[i]->m_cName);
			break;
		}
	}

	if(!foundName)
	{
		str.append(item->m_cName);
	}

	if(GetNibble(item->m_dwAttribute, 7))
	{
		sprintf(G_cTxt, "+%u", GetNibble(item->m_dwAttribute, 7));
		str.append(G_cTxt);
	}

	uint32_t nsockets = item->GetMaxSockets();
	if(nsockets > 0)
	{
		if(item->IsVortexed())
		{
			// don't take into consideration vortex gem
			nsockets--;
		}
		sprintf(G_cTxt, " (%u/%u)", item->GetNUsedSockets(), nsockets);
		str.append(G_cTxt);
	}

	lines.push_back(str);
	str.clear();

	G_cTxt[0] = 0;

	// Item Display - ShadowEvil
	HandleItemDescription(item);		// ShadowEvil - Handles item description so GetItemName isn't so clutter'd

	if(G_cTxt[0])
	{
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	}

	/*
EQUIPPOS_NONE
EQUIPPOS_HEAD
EQUIPPOS_BODY
EQUIPPOS_ARMS
EQUIPPOS_PANTS
EQUIPPOS_BOOTS
EQUIPPOS_NECK
EQUIPPOS_LHAND
EQUIPPOS_RHAND
EQUIPPOS_TWOHAND
EQUIPPOS_RFINGER
EQUIPPOS_LFINGER
EQUIPPOS_BACK
EQUIPPOS_FULLBODY
*/

	// Item Display - ShadowEvil
	int iTmp[15];
	ZeroMemory(iTmp, sizeof(iTmp));
	char m_cItemNameTemp1[22], m_cItemNameTemp2[22];
	ZeroMemory(m_cItemNameTemp2, sizeof(m_cItemNameTemp2));
	strcpy(m_cItemNameTemp2, item->m_cName);
	switch(item->m_cEquipPos)
	{
	case EQUIPPOS_HEAD:
	case EQUIPPOS_BODY:
	case EQUIPPOS_ARMS:
	case EQUIPPOS_PANTS:
	case EQUIPPOS_BOOTS:
	case EQUIPPOS_LHAND:
		for(int i=0;i<MAXITEMSTATS;i++)
		{
			ZeroMemory(m_cItemNameTemp1, sizeof(m_cItemNameTemp1));
			//m_cItemNameTemp1 = m_pBuildItemListStats[i]->m_cName;
			strcpy(m_cItemNameTemp1, m_pBuildItemListStats[i]->m_cName);
			if(strncmp(m_cItemNameTemp1, m_cItemNameTemp2, strlen(m_cItemNameTemp1)) == 0)
			{
				iTmp[0] = m_pBuildItemListStats[i]->m_sItemEffectValue1;	// Defense
				iTmp[1] = m_pBuildItemListStats[i]->m_sItemEffectValue2;	// Phsyical Absorb??
				break;
			}
		}
		wsprintfA(G_cTxt, ITEM_DESCRIPTION3, iTmp[0]);
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
		wsprintfA(G_cTxt, ITEM_DESCRIPTION4, iTmp[1]);
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
		break;
	case EQUIPPOS_RHAND:
	case EQUIPPOS_TWOHAND:
		for(int i=0;i<MAXITEMSTATS;i++)
		{
			ZeroMemory(m_cItemNameTemp1, sizeof(m_cItemNameTemp1));
			//m_cItemNameTemp1 = m_pBuildItemListStats[i]->m_cName;
			strcpy(m_cItemNameTemp1, m_pBuildItemListStats[i]->m_cName);
			if(strncmp(m_cItemNameTemp1, m_cItemNameTemp2, strlen(m_cItemNameTemp1)) == 0)
			{
				iTmp[0] = m_pBuildItemListStats[i]->m_sItemEffectValue1;	// Damage 1
				iTmp[1] = m_pBuildItemListStats[i]->m_sItemEffectValue2;	// Damage 2
				iTmp[2] = m_pBuildItemListStats[i]->m_sItemEffectValue3;	// Damage 3
				iTmp[3] = m_pBuildItemListStats[i]->m_sItemEffectValue4;	// Damage 4
				iTmp[4] = m_pBuildItemListStats[i]->m_sItemEffectValue5;	// Damage 5
				iTmp[5] = m_pBuildItemListStats[i]->m_sItemEffectValue6;	// Damage 6
				iTmp[6] = m_pBuildItemListStats[i]->m_wWeight / 100;		// Min Swing
				iTmp[7] = m_pBuildItemListStats[i]->m_cSpeed * 13;			// Max Swing
				break;
			}
		}
		wsprintfA(G_cTxt, ITEM_DESCRIPTION1, iTmp[0], iTmp[1], iTmp[2]);
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
		wsprintfA(G_cTxt, ITEM_DESCRIPTION2, iTmp[3], iTmp[4], iTmp[5]);
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
		wsprintfA(G_cTxt, ITEM_DESCRIPTION5, iTmp[6], iTmp[7]);
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
		break;
	case EQUIPPOS_NECK:
	case EQUIPPOS_RFINGER:
	case EQUIPPOS_LFINGER:
	case EQUIPPOS_BACK:
	case EQUIPPOS_FULLBODY:
	case EQUIPPOS_NONE:
		break;
	}
	// Item Display - ShadowEvil - END

	/*if(item->m_cItemType == ITEMTYPE_USE_DEPLETE_DEST)
	{
		if(!strcmp(item->m_cName, "VortexGem"))
		{
			str = GET_ITEM_NAME38;
			lines.push_back( str );
			str.clear();
		}
		else
		{
			for(int j = 0; j < MAXITEMNAMES; j++)
			{
				if(m_pItemNameList[j] && strcmp(m_pItemNameList[j]->m_cOriginName, item->m_cName) == 0)
				{
					item->GetGemAttr(G_cTxt);
					if(!G_cTxt[0]) break;

					str.append( G_cTxt );
					lines.push_back(str);
					str.clear();

					str.append( GET_ITEM_NAME37 );
					lines.push_back(str);
					str.clear();
					G_cTxt[0] = NULL;
					break;
				}
			}
		}
	}*/

	for(int i = 0; i < MAXITEMSOCKETS; i++)
	{
		if(item->m_sockets[i] == SG_NONE || item->m_sockets[i] == SG_VORTEXGEM)
			continue;

		std::unordered_map<uint8_t, const char*>::iterator iter = g_socketMap.find( item->m_sockets[i] );
		if(iter == g_socketMap.end())
			continue;

		for(int j = 0; j < MAXITEMNAMES; j++)
		{
			if(m_pItemNameList[j] && strcmp(m_pItemNameList[j]->m_cOriginName, iter->second) == 0)
			{
				str.append(m_pItemNameList[j]->m_cName);
				str.append(": ");
				item->GetGemAttr( G_cTxt, item->m_sockets[i] );
				str.append( G_cTxt );
				lines.push_back(str);
				str.clear();
				G_cTxt[0] = 0;
				break;
			}
		}
	}

	if(item->IsManued())
	{
		m_itemColor =video::SColor(255,172,95,95);
		if (item->m_cItemType == ITEMTYPE_MATERIAL)
		{
			wsprintfA(G_cTxt, GET_ITEM_NAME1, item->m_sItemSpecEffectValue2);		//"Purity: %d%%"
		} else{
			if (item->m_cEquipPos == EQUIPPOS_LFINGER)
			{
				wsprintfA(G_cTxt, GET_ITEM_NAME2, item->m_sItemSpecEffectValue2);	//"Completion:
			}else
			{
				wsprintfA(G_cTxt, GET_ITEM_NAME2, item->m_sItemSpecEffectValue2 +100);	//"Completion: +100
			}
		}
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	}

	G_cTxt[0] = 0;
	uint32_t value = GetNibble(item->m_dwAttribute, 4);

	if(GetNibble(item->m_dwAttribute, 5) && !item->IsVortexed())
	{
		m_itemColor =video::SColor(255,0,200,0);	// Solid green
	}

	switch(GetNibble(item->m_dwAttribute, 5))
	{
	case 1:
		wsprintfA(G_cTxt, GET_ITEM_NAME14, value);
		break;
	case 2:
		wsprintfA(G_cTxt, GET_ITEM_NAME15, value*5);
		break;
	case 3:
		break;
	case 4: break;
	case 5:
		strcpy(G_cTxt, GET_ITEM_NAME16);
		break;
	case 6:
		wsprintfA(G_cTxt, GET_ITEM_NAME17, value*4);
		break;
	case 7:
		strcpy(G_cTxt, GET_ITEM_NAME18);
		break;
	case 8:
		wsprintfA(G_cTxt, GET_ITEM_NAME19, value*7);
		break;
	case 9:
		strcpy(G_cTxt, GET_ITEM_NAME20);
		break;
	case 10:
		wsprintfA(G_cTxt, GET_ITEM_NAME21, value*3);
		break;
	case 11:
		wsprintfA(G_cTxt, GET_ITEM_NAME22, value);
		break;
	case 12:
		wsprintfA(G_cTxt, GET_ITEM_NAME23, value);
		break;
	}

	if(G_cTxt[0])
	{
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	}

	for(int i = 0; i < MAXITEMNAMES; i++)
	{
		if(m_pItemNameList[i] && strcmp(m_pItemNameList[i]->m_cOriginName, item->m_cName) == 0)
		{
			foundName = true;
			str.append(m_pItemNameList[i]->m_cName);
			break;
		}
	}

	G_cTxt[0] = 0;
	value = GetNibble(item->m_dwAttribute, 2);
	switch(GetNibble(item->m_dwAttribute, 3))
	{
	case 1:
		value *= 10;
		wsprintfA(G_cTxt, GET_ITEM_NAME24, value);
		isValue(value);
		value = 0;
		break;//PR
	case 2://Hit Prob
		value *= 10;
		wsprintfA(G_cTxt, GET_ITEM_NAME25, value);
		isValue(value);
		value = 0;
		break;
	case 3://DR
		value *= 10;
		wsprintfA(G_cTxt, GET_ITEM_NAME26, value);
		isValue(value);
		value = 0;
		break;
	case 4://HP rec
		value *= 10;
		wsprintfA(G_cTxt, GET_ITEM_NAME27, value);
		isValue(value);
		value = 0;
		break;
	case 5://SP rec
		value *= 10;
		wsprintfA(G_cTxt, GET_ITEM_NAME28, value);
		isValue(value);
		value = 0;
		break;
	case 6://MP rec
		value *= 10;
		wsprintfA(G_cTxt, GET_ITEM_NAME29, value);
		isValue(value);
		value = 0;
		break;
	case 7://MR
		value *= 10;
		wsprintfA(G_cTxt, GET_ITEM_NAME30, value);
		isValue(value);
		value = 0;
		break;
	case 8:  wsprintfA(G_cTxt, GET_ITEM_NAME31, value*3); value = 0; break;//PA
	case 9:  wsprintfA(G_cTxt, GET_ITEM_NAME32, value*2); value = 0; break;//Magic Abs Reduced from 39% to 26% MAX xRisenx
	case 10: wsprintfA(G_cTxt, GET_ITEM_NAME33, value); value = 0; break;//Rep
	case 11://Exp
		value *= 10;
		wsprintfA(G_cTxt, GET_ITEM_NAME34, value);
		isValue(value);
		value = 0;
		break;
	case 12://Gold +%
		value *= 10;
		wsprintfA(G_cTxt, GET_ITEM_NAME35, value);
		isValue(value);
		value = 0;
		break;
	}

	if(G_cTxt[0])
	{
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	}

	if(item->m_sLevelLimit != 0 && !item->IsManued())
	{
		wsprintfA(G_cTxt, "%s: %d", DRAW_DIALOGBOX_SHOP24, item->m_sLevelLimit);
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	}

	if(item->m_cEquipPos != EQUIPPOS_NONE && item->m_wWeight >= 1100)
	{
		int _wWeight = 0;
		if(item->m_wWeight % 100) _wWeight = 1;
		wsprintfA( G_cTxt, DRAW_DIALOGBOX_SHOP15, item->m_wWeight/100 + _wWeight);
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	}

	if((strcmp(item->m_cName, "ZemstoneofSacrifice") == 0) || (strcmp(item->m_cName, "AcientTablet") == 0))
	{
		m_itemColor =video::SColor(255,145,250,255);	// Light baby blue
		wsprintfA(G_cTxt, UPDATE_SCREEN_ONGAME15, item->m_wCurLifeSpan);
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	}
	else if(item->m_cItemType == ITEMTYPE_USE_DEPLETE_DEST)
	{
		wsprintfA(G_cTxt, UPDATE_SCREEN_ONGAME15, item->m_wCurLifeSpan);
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	} else if(item->m_cEquipPos != EQUIPPOS_NONE) {
		wsprintfA(G_cTxt, UPDATE_SCREEN_ONGAME10, item->m_wCurLifeSpan);
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	}

	if(isWH == false)
	{
		int iLoc = 0;
		for ( int iTmp = 0 ; iTmp < MAXITEMS ; iTmp++ )
		{
			if( m_pItemList[iTmp] != 0 )
			{
				if( strcmp(m_pItemList[iTmp]->m_cName, m_pItemList[m_stMCursor.sSelectedObjectID]->m_cName ) == 0 )
					iLoc++;
			}
		}
		if( iLoc > 1 )
		{
			wsprintfA(G_cTxt, MSG_TOTAL_NUMBER, iLoc);
			str = G_cTxt;
			lines.push_back(str);
			str.clear();
			//PutString(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY + 40, G_cTxt,video::SColor(255,150,150,150), FALSE, 1);
		}
	}
	return &lines;
}

std::vector<string> * CGame::GetItemName(char * cItemName, uint32_t attr, uint8_t sockets[MAXITEMSOCKETS], uint32_t count)
{
	m_itemColor =video::SColor(255,255,255,255);

	static std::vector<string> lines;
	lines.clear();

	string str("");

	if(strcmp(cItemName, "ScrollofRenown") == 0)
	{
		bool foundName = false;
		for(int i = 0; i < MAXITEMNAMES; i++)
		{
			if(m_pItemNameList[i] && strcmp(m_pItemNameList[i]->m_cOriginName, cItemName) == 0)
			{
				foundName = true;
				str = m_pItemNameList[i]->m_cName;
				break;
			}
		}

		if(!foundName)
		{
			str = cItemName;
		}
		
		sprintf(G_cTxt, " (%d)", attr);
		str += G_cTxt;
		lines.push_back(str);

		sprintf(G_cTxt, GET_ITEM_NAME39, attr);
		lines.push_back(G_cTxt);
		return &lines;
	}

	if(count > 1 && !(attr & 1))
	{
		wsprintfA(G_cTxt, "%u ", count);
		str.append(G_cTxt);
	}

	if(sockets[0] == SG_VORTEXGEM)
	{
		str.append("Vortexed ");
		m_itemColor =video::SColor(255,132,132,180);
	}

	switch(GetNibble(attr, 5))
	{
	case 1: str.append( GET_ITEM_NAME3 );   break;
	case 2: str.append( GET_ITEM_NAME4 );   break;
	case 3: str.append( GET_ITEM_NAME5 );   break;
	case 4: break;
	case 5: str.append( GET_ITEM_NAME6 );   break;
	case 6: str.append( GET_ITEM_NAME7 );   break;
	case 7: str.append( GET_ITEM_NAME8 );   break;
	case 8: str.append( GET_ITEM_NAME9 );   break;
	case 9: str.append( GET_ITEM_NAME10 );  break;
	case 10: str.append( GET_ITEM_NAME11 ); break;
	case 11: str.append( GET_ITEM_NAME12 ); break;
	case 12: str.append( GET_ITEM_NAME13 ); break;
	case 13: str.append( GET_ITEM_NAME40 ); break; // Flawless Sharp
	case 14: str.append( GET_ITEM_NAME41 ); break; // Flawless Ancient
	}

	bool foundName = false;
	for(int i = 0; i < MAXITEMNAMES; i++)
	{
		if(m_pItemNameList[i] && strcmp(m_pItemNameList[i]->m_cOriginName, cItemName) == 0)
		{
			foundName = true;
			str.append(m_pItemNameList[i]->m_cName);
			break;
		}
	}

	if(!foundName)
	{
		str.append(cItemName);
	}

	if(GetNibble(attr, 7))
	{
		sprintf(G_cTxt, "+%u", GetNibble(attr, 7));
		str.append(G_cTxt);
	}

	lines.push_back(str);
	str.clear();

	G_cTxt[0] = 0;

	if (strcmp(cItemName,"RingofOgrepower") == 0){
		wsprintfA(G_cTxt, "+%i %s", 40, LNG_DAMAGE);
	}else if (strcmp(cItemName,"RingofDemonpower") == 0){
		wsprintfA(G_cTxt, "+%i %s", 50, LNG_DAMAGE);
	}else if (strcmp(cItemName,"RingofDragonpower") == 0){
		wsprintfA(G_cTxt, "+%i %s", 60, LNG_DAMAGE);
	}else if (strcmp(cItemName,"RingoftheXelima") == 0){
		wsprintfA(G_cTxt, "+%i %s", 70, LNG_DAMAGE);
	}else if (strcmp(cItemName,"RingoftheAbaddon") == 0){
		wsprintfA(G_cTxt, "+%i %s", 100, LNG_DAMAGE);
	}else if (strcmp(cItemName,"RingofWizard") == 0){
		wsprintfA(G_cTxt, "+%i %s", 5, LNG_MAGICDAMAGE);
	}else if (strcmp(cItemName,"RingofMage") == 0){
		wsprintfA(G_cTxt, "+%i %s", 10, LNG_MAGICDAMAGE);
	}else if (strcmp(cItemName,"RingofGrandMage") == 0){
		wsprintfA(G_cTxt, "+%i %s", 15, LNG_MAGICDAMAGE);
	}else if (strcmp(cItemName,"RingofArchmage") == 0){
		wsprintfA(G_cTxt, "+%i %s", 20, LNG_MAGICDAMAGE);
	}

	if(G_cTxt[0])
	{
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	}

	/*if(!strcmp(cItemName, "VortexGem"))
	{
		str = GET_ITEM_NAME38;
		lines.push_back( str );
		str.clear();
	}
	else
	{
		for(int j = 0; j < MAXITEMNAMES; j++)
		{
			if(m_pItemNameList[j] && strcmp(m_pItemNameList[j]->m_cOriginName, cItemName) == 0)
			{
				CItem item;
				strcpy(item.m_cName, cItemName);
				item.GetGemAttr(G_cTxt);
				if(!G_cTxt[0]) break;

				str.append( G_cTxt );
				lines.push_back(str);
				str.clear();

				str.append( GET_ITEM_NAME37 );
				lines.push_back(str);
				str.clear();
				G_cTxt[0] = NULL;
				break;
			}
		}
	}*/

	for(int i = 0; i < MAXITEMSOCKETS; i++)
	{
		if(sockets[i] == SG_NONE || sockets[i] == SG_VORTEXGEM)
			continue;

		std::unordered_map<uint8_t, const char*>::iterator iter = g_socketMap.find( sockets[i] );
		if(iter == g_socketMap.end())
			continue;

		for(int j = 0; j < MAXITEMNAMES; j++)
		{
			if(m_pItemNameList[j] && strcmp(m_pItemNameList[j]->m_cOriginName, iter->second) == 0)
			{
				str.append(m_pItemNameList[j]->m_cName);
				str.append(": ");
				CItem item;
				item.GetGemAttr( G_cTxt, sockets[i] );
				str.append( G_cTxt );
				lines.push_back(str);
				str.clear();
				G_cTxt[0] = 0;
				break;
			}
		}
	}

	if(attr & 1)
	{
		m_itemColor =video::SColor(255,172,172,95);
	}

	G_cTxt[0] = 0;
	uint32_t value = GetNibble(attr, 4);

	if(GetNibble(attr, 5) && sockets[0] != SG_VORTEXGEM)
	{
		m_itemColor =video::SColor(255,90,220,90);
	}

	switch(GetNibble(attr, 5))
	{
	case 1: wsprintfA(G_cTxt, GET_ITEM_NAME14, value); break;
	case 2: wsprintfA(G_cTxt, GET_ITEM_NAME15, value*5); break;
	case 3: break;
	case 4: break;
	case 5: strcpy(G_cTxt, GET_ITEM_NAME16); break;
	case 6: wsprintfA(G_cTxt, GET_ITEM_NAME17, value*4); break;
	case 7: strcpy(G_cTxt, GET_ITEM_NAME18); break;
	case 8: wsprintfA(G_cTxt, GET_ITEM_NAME19, value*7); break;
	case 9: strcpy(G_cTxt, GET_ITEM_NAME20); break;
	case 10: wsprintfA(G_cTxt, GET_ITEM_NAME21, value*3); break;
	case 11: wsprintfA(G_cTxt, GET_ITEM_NAME22, value); break;
	case 12: wsprintfA(G_cTxt, GET_ITEM_NAME23, value); break;
	}

	if(G_cTxt[0])
	{
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	}

	G_cTxt[0] = 0;
	value = GetNibble(attr, 2);
	switch(GetNibble(attr, 3))
	{
	case 1:  wsprintfA(G_cTxt, GET_ITEM_NAME24, value*10); break;//PR
	case 2:  wsprintfA(G_cTxt, GET_ITEM_NAME25, value*10); break;//Hit Prob
	case 3:  wsprintfA(G_cTxt, GET_ITEM_NAME26, value*15); break;//DR // 130 -> 195 
	case 4:  wsprintfA(G_cTxt, GET_ITEM_NAME27, value*10); break;//HP rec
	case 5:  wsprintfA(G_cTxt, GET_ITEM_NAME28, value*10); break;//SP rec
	case 6:  wsprintfA(G_cTxt, GET_ITEM_NAME29, value*10); break;//MP rec
	case 7:  wsprintfA(G_cTxt, GET_ITEM_NAME30, value*15); break;//MR // 130 -> 195 
	case 8:  wsprintfA(G_cTxt, GET_ITEM_NAME31, value*3); break;//PA
	case 9:  wsprintfA(G_cTxt, GET_ITEM_NAME32, value*2); break;  // Magic Abs Reduced from 39% to 26% MAX xRisenx
	case 10: wsprintfA(G_cTxt, GET_ITEM_NAME33, value);   break;//Rep
	case 11: wsprintfA(G_cTxt, GET_ITEM_NAME34, value*10); break;//Exp
	case 12: wsprintfA(G_cTxt, GET_ITEM_NAME35, value*10); break;//Gold +%
	}

	if(G_cTxt[0])
	{
		str = G_cTxt;
		lines.push_back(str);
		str.clear();
	}

	return &lines;
}

void CGame::GetItemName(CItem *pItem, char *pStr1, char *pStr2, char *pStr3)
{int i;
 char cTxt[256], cTxt2[256], cName[51];
 uint32_t dwType1, dwType2, dwValue1, dwValue2, dwValue3;

	m_bIsSpecial = false;
	ZeroMemory(cName, sizeof(cName));
	ZeroMemory(pStr1, sizeof(pStr1));
	ZeroMemory(pStr2, sizeof(pStr2));
	ZeroMemory(pStr3, sizeof(pStr3));

	strcpy(cName, pItem->m_cName);
	for (i = 0; i < MAXITEMNAMES; i++)
	if ((m_pItemNameList[i] != 0) && (strcmp(m_pItemNameList[i]->m_cOriginName, pItem->m_cName) == 0))
	{	strcpy(cName, m_pItemNameList[i]->m_cName);
		break;
	}

     	/* if (0 == memcmp(pItem->m_cName,"AcientTablet", 12)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"NecklaceOf", 10)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"DarkElfBow", 10)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"DarkExecutor", 12)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"The_Devastator", 14)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"DemonSlayer", 10)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"LightingBlade", 12)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"RubyRing", 8)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"SapphireRing", 12)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"Ringof", 6)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"MagicNecklace", 13)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"MagicWand(M.Shield)", 19)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"MagicWand(MS30-LLF)", 19)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"Merien", 6)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"BerserkWand", 11)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"ResurWand", 9)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"Blood", 5)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"Swordof", 7)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"StoneOf", 7)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"ZemstoneofSacrifice", 19)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"StormBringer", 12)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"Aresden", 7)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"Elvine", 6)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"EmeraldRing", 11)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"Excaliber", 9)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"Xelima", 6)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"Kloness", 7)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"aHeroOf", 7)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(pItem->m_cName,"eHeroOf", 7)) m_bIsSpecial = TRUE;*/

	if ((pItem->m_dwAttribute & 0x00000001) != 0)
	{	m_bIsSpecial = true;
		strcpy(pStr1, cName);
		if (pItem->m_cItemType == ITEMTYPE_MATERIAL)
			wsprintfA(pStr2, GET_ITEM_NAME1, pItem->m_sItemSpecEffectValue2);		//"Purity: %d%%"
		else
		{	// Crafting Magins completion fix
			if (pItem->m_cEquipPos == EQUIPPOS_LFINGER)
			{	wsprintfA(pStr2, GET_ITEM_NAME2, pItem->m_sItemSpecEffectValue2);	//"Completion:
			}else
			{	wsprintfA(pStr2, GET_ITEM_NAME2, pItem->m_sItemSpecEffectValue2 +100);	//"Completion: +100
		}	}
	}else{
		if (pItem->m_dwCount == 1)
			wsprintfA(G_cTxt, "%s", cName);
		else
			wsprintfA(G_cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM1, pItem->m_dwCount, cName);//"%d %s"
		strcpy(pStr1, G_cTxt);
	}

	if ((pItem->m_dwAttribute & 0x00F0F000) != 0)
	{
		m_bIsSpecial = true;
		dwType1  = (pItem->m_dwAttribute & 0x00F00000) >> 20;
		dwValue1 = (pItem->m_dwAttribute & 0x000F0000) >> 16;
		dwType2  = (pItem->m_dwAttribute & 0x0000F000) >> 12;
		dwValue2 = (pItem->m_dwAttribute & 0x00000F00) >> 8;
		if (dwType1 != 0)
		{
			ZeroMemory(cTxt, sizeof(cTxt));
			switch (dwType1) {
			case 1: strcpy(cTxt, GET_ITEM_NAME3);   break;
			case 2: strcpy(cTxt, GET_ITEM_NAME4);   break; // "Poisoning "
			case 3: strcpy(cTxt, GET_ITEM_NAME5);   break; // "Righteous "
			case 4: break;
			case 5: strcpy(cTxt, GET_ITEM_NAME6);   break; // "Agile "
			case 6: strcpy(cTxt, GET_ITEM_NAME7);   break;
			case 7: strcpy(cTxt, GET_ITEM_NAME8);   break;
			case 8: strcpy(cTxt, GET_ITEM_NAME9);   break;
			case 9: strcpy(cTxt, GET_ITEM_NAME10);  break;
			case 10: strcpy(cTxt, GET_ITEM_NAME11); break;
			case 11: strcpy(cTxt, GET_ITEM_NAME12); break;
			case 12: strcpy(cTxt, GET_ITEM_NAME13); break;
			case 13: strcpy(cTxt, GET_ITEM_NAME40); break; // Flawless Sharp
			case 14: strcpy(cTxt, GET_ITEM_NAME41); break; // Flawless Ancient
			}
			strcat(cTxt, pStr1);
			ZeroMemory(pStr1, sizeof(pStr1));
			strcpy(pStr1, cTxt);

			ZeroMemory(cTxt, sizeof(cTxt));
			switch (dwType1) {
			case 1: wsprintfA(cTxt, GET_ITEM_NAME14, dwValue1); break; // "Critical Hit Damage+%d"
			case 2: wsprintfA(cTxt, GET_ITEM_NAME15, dwValue1*5); break; // "Poison Damage+%d"
			case 3: break;
			case 4: break;
			case 5: strcpy(cTxt, GET_ITEM_NAME16); break; // "Attack Speed -1"
			case 6: wsprintfA(cTxt, GET_ITEM_NAME17, dwValue1*4); break;
			case 7: strcpy(cTxt, GET_ITEM_NAME18); break;
			case 8: wsprintfA(cTxt, GET_ITEM_NAME19, dwValue1*7); break;
			case 9: strcpy(cTxt, GET_ITEM_NAME20); break;
			case 10: wsprintfA(cTxt, GET_ITEM_NAME21, dwValue1*3); break;
			case 11: wsprintfA(cTxt, GET_ITEM_NAME22, dwValue1); break;
			case 12: wsprintfA(cTxt, GET_ITEM_NAME23, dwValue1); break;
			}
			strcat(pStr2, cTxt);
		}

			if (dwType2 != 0) {
				ZeroMemory(cTxt, sizeof(cTxt));
			switch (dwType2)
			{
				case 1:  wsprintfA(cTxt, GET_ITEM_NAME24, dwValue2*10); break;//PR
				case 2:  wsprintfA(cTxt, GET_ITEM_NAME25, dwValue2*10); break;//Hit Prob
				case 3:  wsprintfA(cTxt, GET_ITEM_NAME26, dwValue2*15); break;//DR // 130 -> 195 
				case 4:  wsprintfA(cTxt, GET_ITEM_NAME27, dwValue2*10); break;//HP rec
				case 5:  wsprintfA(cTxt, GET_ITEM_NAME28, dwValue2*10); break;//SP rec
				case 6:  wsprintfA(cTxt, GET_ITEM_NAME29, dwValue2*10); break;//MP rec
				case 7:  wsprintfA(cTxt, GET_ITEM_NAME30, dwValue2*15); break;//MR // 130 -> 195 
				case 8:  wsprintfA(cTxt, GET_ITEM_NAME31, dwValue2*3); break;//PA
				case 9:  wsprintfA(cTxt, GET_ITEM_NAME32, dwValue2*2); break; // Magic Abs Reduced from 39% to 26% MAX xRisenx
				case 10: wsprintfA(cTxt, GET_ITEM_NAME33, dwValue2);   break;//Rep
				case 11: wsprintfA(cTxt, GET_ITEM_NAME34, dwValue2*10); break;//Exp
				case 12: wsprintfA(cTxt, GET_ITEM_NAME35, dwValue2*10); break;//Gold +%
				}
				strcpy(pStr3, cTxt);
		}
	}

	dwValue3 = (pItem->m_dwAttribute & 0xF0000000) >> 28;
	if (dwValue3 > 0)
	{
		if (pStr1[strlen(pStr1)-2] == '+')
		{
			dwValue3 = atoi((char *)(pStr1 + strlen(pStr1)-1)) + dwValue3;
			ZeroMemory(cTxt, sizeof(cTxt));
			memcpy(cTxt, pStr1, strlen(pStr1)-2);
			ZeroMemory(cTxt2, sizeof(cTxt2));
			wsprintfA(cTxt2, "%s+%d", cTxt, dwValue3);
			ZeroMemory(pStr1, sizeof(pStr1));
			strcpy(pStr1, cTxt2);
		}else
		{
			ZeroMemory(cTxt, sizeof(cTxt));
			wsprintfA(cTxt, "+%d", dwValue3);
			strcat(pStr1, cTxt);
}
	}
}

void CGame::GetItemName(char * cItemName, uint32_t dwAttribute, char *pStr1, char *pStr2, char *pStr3)
{
 int i;
 char cTxt[256], cTxt2[256], cName[51];
 uint32_t dwType1, dwType2, dwValue1, dwValue2, dwValue3;

 	m_bIsSpecial = false;
	ZeroMemory(cName, sizeof(cName));
	ZeroMemory(pStr1, sizeof(pStr1));
	ZeroMemory(pStr2, sizeof(pStr2));
	ZeroMemory(pStr3, sizeof(pStr3));

	strcpy(cName, cItemName);
	for (i = 0; i < MAXITEMNAMES; i++)
	if ((m_pItemNameList[i] != 0) && (strcmp(m_pItemNameList[i]->m_cOriginName, cItemName) == 0)) {
		strcpy(cName, m_pItemNameList[i]->m_cName);
		break;
	}

     	/* if (0 == memcmp(cItemName,"AcientTablet", 12)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"NecklaceOf", 10)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"DarkElfBow", 10)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"DarkExecutor", 12)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"The_Devastator", 14)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"DemonSlayer", 10)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"LightingBlade", 12)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"RubyRing", 8)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"SapphireRing", 12)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"Ringof", 6)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"MagicNecklace", 13)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"MagicWand(M.Shield)", 19)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"MagicWand(MS30-LLF)", 19)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"Merien", 6)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"BerserkWand", 11)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"ResurWand", 9)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"Blood", 5)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"Swordof", 7)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"StoneOf", 7)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"ZemstoneofSacrifice", 19)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"StormBringer", 12)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"Aresden", 7)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"Elvine", 6)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"EmeraldRing", 11)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"Excaliber", 9)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"Xelima", 6)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"Kloness", 7)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"aHeroOf", 7)) m_bIsSpecial = TRUE;
	else if (0 == memcmp(cItemName,"eHeroOf", 7)) m_bIsSpecial = TRUE;*/
	strcpy(pStr1, cName);

	if ((dwAttribute & 0x00F0F000) != 0)
	{	m_bIsSpecial = true;
		dwType1  = (dwAttribute & 0x00F00000) >> 20;
		dwValue1 = (dwAttribute & 0x000F0000) >> 16;
		dwType2  = (dwAttribute & 0x0000F000) >> 12;
		dwValue2 = (dwAttribute & 0x00000F00) >> 8;
		if (dwType1 != 0)
		{	ZeroMemory(cTxt, sizeof(cTxt));
			switch (dwType1) {
			case 1: strcpy(cTxt, GET_ITEM_NAME3); break;
			case 2: strcpy(cTxt, GET_ITEM_NAME4); break;
			case 3: strcpy(cTxt, GET_ITEM_NAME5); break;
			case 4: break;
			case 5: strcpy(cTxt, GET_ITEM_NAME6); break;
			case 6: strcpy(cTxt, GET_ITEM_NAME7); break;
			case 7: strcpy(cTxt, GET_ITEM_NAME8); break;
			case 8: strcpy(cTxt, GET_ITEM_NAME9); break;
			case 9: strcpy(cTxt, GET_ITEM_NAME10); break;
			case 10: strcpy(cTxt, GET_ITEM_NAME11); break;
			case 11: strcpy(cTxt, GET_ITEM_NAME12); break;
			case 12: strcpy(cTxt, GET_ITEM_NAME13); break;
			case 13: strcpy(cTxt, GET_ITEM_NAME40); break; // Flawless Sharp
			case 14: strcpy(cTxt, GET_ITEM_NAME41); break; // Flawless Ancient
			}
			strcat(cTxt, pStr1);
			ZeroMemory(pStr1, sizeof(pStr1));
			strcpy(pStr1, cTxt);

			ZeroMemory(cTxt, sizeof(cTxt));
			switch (dwType1) {
			case 1: wsprintfA(cTxt, GET_ITEM_NAME14, dwValue1); break;
			case 2: wsprintfA(cTxt, GET_ITEM_NAME15, dwValue1*5); break;
			case 3: break;
			case 4: break;
			case 5: strcpy(cTxt, GET_ITEM_NAME16); break;
			case 6: wsprintfA(cTxt, GET_ITEM_NAME17, dwValue1*4); break;
			case 7: strcpy(cTxt, GET_ITEM_NAME18); break;
			case 8: wsprintfA(cTxt, GET_ITEM_NAME19, dwValue1*7); break;
			case 9: strcpy(cTxt, GET_ITEM_NAME20); break;
			case 10: wsprintfA(cTxt, GET_ITEM_NAME21, dwValue1*3); break;
			case 11: wsprintfA(cTxt, GET_ITEM_NAME22, dwValue1); break;
			case 12: wsprintfA(cTxt, GET_ITEM_NAME23, dwValue1); break;
			}
			strcat(pStr2, cTxt);

			if (dwType2 != 0)
			{	ZeroMemory(cTxt, sizeof(cTxt));
				switch (dwType2) {
				case 1:  wsprintfA(cTxt, GET_ITEM_NAME24, dwValue2*10);  break;
				case 2:  wsprintfA(cTxt, GET_ITEM_NAME25, dwValue2*10);  break;
				case 3:  wsprintfA(cTxt, GET_ITEM_NAME26, dwValue2*15);  break; // 130 -> 195 
				case 4:  wsprintfA(cTxt, GET_ITEM_NAME27, dwValue2*10);  break;
				case 5:  wsprintfA(cTxt, GET_ITEM_NAME28, dwValue2*10);  break;
				case 6:  wsprintfA(cTxt, GET_ITEM_NAME29, dwValue2*10);  break;
				case 7:  wsprintfA(cTxt, GET_ITEM_NAME30, dwValue2*15);  break; // 130 -> 195 
				case 8:  wsprintfA(cTxt, GET_ITEM_NAME31, dwValue2*3);  break;
				case 9:  wsprintfA(cTxt, GET_ITEM_NAME32, dwValue2*2);  break; // Magic Abs Reduced from 39% to 26% MAX xRisenx
				case 10: wsprintfA(cTxt, GET_ITEM_NAME33, dwValue2);    break;
				case 11: wsprintfA(cTxt, GET_ITEM_NAME34, dwValue2*10); break;
				case 12: wsprintfA(cTxt, GET_ITEM_NAME35, dwValue2*10); break;
				}
				strcpy(pStr3, cTxt);
	}	}	}

	dwValue3 = (dwAttribute & 0xF0000000) >> 28;
	if (dwValue3 > 0)
	{	if (pStr1[strlen(pStr1)-2] == '+')
		{	dwValue3 = atoi((char *)(pStr1 + strlen(pStr1)-1)) + dwValue3;
			ZeroMemory(cTxt, sizeof(cTxt));
			memcpy(cTxt, pStr1, strlen(pStr1)-2);
			ZeroMemory(cTxt2, sizeof(cTxt2));
			wsprintfA(cTxt2, "%s+%d", cTxt, dwValue3);
			ZeroMemory(pStr1, sizeof(pStr1));
			strcpy(pStr1, cTxt2);
		}else
		{	ZeroMemory(cTxt, sizeof(cTxt));
			wsprintfA(cTxt, "+%d", dwValue3);
			strcat(pStr1, cTxt);
	}	}
}
void CGame::PointCommandHandler(int indexX, int indexY, char cItemID)
{
 char cTemp[31];
if ((m_iPointCommandType >= 100) && (m_iPointCommandType < 200)) {
		bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_MAGIC, 0, indexX, indexY, m_iPointCommandType, 0);
	}
	else if ((m_iPointCommandType >= 0) && (m_iPointCommandType < 50)) {
		bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_USEITEM, 0, m_iPointCommandType, indexX, indexY, cTemp, cItemID);

		if (m_pItemList[m_iPointCommandType]->m_cItemType == ITEMTYPE_USE_SKILL)
			m_bSkillUsingStatus = true;
		else if (m_pItemList[m_iPointCommandType]->m_cItemType == ITEMTYPE_USE_DEPLETE_DEST &&
			m_pItemList[m_iPointCommandType]->m_wCurLifeSpan > 1) // prevent exhausting by client
			m_pItemList[m_iPointCommandType]->m_wCurLifeSpan--;
	}
	else if (m_iPointCommandType == 200) {
		if ((strlen(m_cMCName) == 0) || (strcmp(m_cMCName, m_cPlayerName) == 0) || (m_cMCName[0] == '_')) {
			m_dialogBoxes[32].SetMode(0);
			PlaySound('E', 14, 5);

			AddEventList(POINT_COMMAND_HANDLER1, 10);
		}
		else {
			m_dialogBoxes[32].SetMode(3);
			PlaySound('E', 14, 5);
			ZeroMemory(m_dialogBoxes[32].cStr, sizeof(m_dialogBoxes[32].cStr));
			strcpy(m_dialogBoxes[32].cStr, m_cMCName);
			bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_JOINPARTY, 0, 1, 0, 0, m_cMCName);
			return;
		}
	}
	else if (m_iPointCommandType == 250) {
		for(int i = 0; i < m_iTotalFriends; i++) //Can't add friend twice
			if(strcmp(friendsList[i].friendName, m_cMCName) == 0){
				AddEventList("You cannot add the same player twice.",10);
				PlaySound('E', 24, 5);
				return;
			}

		if ((strlen(m_cMCName) == 0) || (strcmp(m_cMCName, m_cPlayerName) == 0) || (m_cMCName[0] == '_')) {
			PlaySound('E', 24, 5);
		}else
		{
			PlaySound('E', 14, 5);
			memcpy(friendsList[m_iTotalFriends].friendName,m_cMCName,10);
			m_iTotalFriends++;
			m_dialogBoxes[43].sV1=0;
			SaveFriendList();
			UpdateFriendsStatus();
		}
	}
}


void CGame::DrawHeldenianStats()
{
	wsprintfA( G_cTxt, "Towers Left | Flags | Kills | Deaths");
	PutString_SprFont3(5, 135, G_cTxt, m_wR[14]*4, m_wG[14]*4, m_wB[14]*4, true, 2);
	wsprintfA( G_cTxt, "%10s | %2d | %2d | %3d | %3d", sideName[ARESDEN], m_iHeldenianAresdenLeftTower, m_iHeldenianAresdenFlags, m_iHeldenianAresdenKill, m_iHeldenianAresdenDead);
	PutString_SprFont3(10, 155, G_cTxt, m_wR[14]*4, m_wG[14]*4, m_wB[14]*4, true, 2);
	wsprintfA( G_cTxt, "%10s | %2d | %2d | %3d | %3d", sideName[ELVINE], m_iHeldenianElvineLeftTower, m_iHeldenianElvineFlags, m_iHeldenianElvineKill, m_iHeldenianElvineDead);
	PutString_SprFont3(13, 170, G_cTxt, m_wR[14]*4, m_wG[14]*4, m_wB[14]*4, true, 2);
}
void CGame::NotifyMsg_Heldenian(char * pData)
{
	uint16_t *wp;
	wp = (uint16_t *)(pData + INDEX2_MSGTYPE + 2);
	m_iHeldenianAresdenLeftTower = *wp;
	wp++;
	m_iHeldenianAresdenFlags = *wp;
	wp++;
	m_iHeldenianAresdenKill = *wp;
	wp++;
	m_iHeldenianAresdenDead = *wp;
	wp++;
	m_iHeldenianElvineLeftTower = *wp;
	wp++;
	m_iHeldenianElvineFlags = *wp;
	wp++;
	m_iHeldenianElvineKill = *wp;
	wp++;
	m_iHeldenianElvineDead = *wp;
}

void CGame::StartBGM()
{
    if( m_bSoundFlag == false )
	{
		if (m_pBGM != nullptr)
		{
            m_pBGM->stop();
            m_pBGM->drop();
            //delete m_pBGM;
			m_pBGM = nullptr;
		}
		return;
	}
	char cWavFileName[32];
	ZeroMemory( cWavFileName, sizeof(cWavFileName) );
	if (m_bIsXmas == true) strcpy( cWavFileName, "data\\music\\Carol.wav" );
	else
	{
        if (memcmp(m_cCurLocation, "aresden", 7) == 0) strcpy( cWavFileName, "data\\music\\aresden.wav" );
		else if (memcmp(m_cCurLocation, "elvine", 6) == 0) strcpy( cWavFileName, "data\\music\\elvine.wav" );
		else if (memcmp(m_cCurLocation, "dglv", 4) == 0) strcpy( cWavFileName, "data\\music\\dungeon.wav" );
		else if (memcmp(m_cCurLocation, "middled1", 8) == 0) strcpy( cWavFileName, "data\\music\\dungeon.wav" );
		else if (memcmp(m_cCurLocation, "middleland", 10) == 0) strcpy( cWavFileName, "data\\music\\middleland.wav" );
		// Snoopy: new musics
		else if (memcmp(m_cCurLocation, "druncncity", 10) == 0) strcpy( cWavFileName, "data\\music\\druncncity.wav" );
		else if (memcmp(m_cCurLocation, "inferniaA", 9) == 0) strcpy( cWavFileName, "data\\music\\middleland.wav" );
		else if (memcmp(m_cCurLocation, "inferniaB", 9) == 0) strcpy( cWavFileName, "data\\music\\middleland.wav" );
		else if (memcmp(m_cCurLocation, "maze", 4) == 0) strcpy( cWavFileName, "data\\music\\dungeon.wav" );
		else if (memcmp(m_cCurLocation, "abaddon", 7) == 0) strcpy( cWavFileName, "data\\music\\abaddon.wav" );
		else if (strcmp(m_cCurLocation, "istria") == 0) strcpy( cWavFileName, "data\\music\\istria.wav" );
		else if (strcmp(m_cCurLocation, "astoria") == 0) strcpy( cWavFileName, "data\\music\\astoria.wav" );
		else strcpy( cWavFileName, "data\\music\\MainTm.wav" );
	}

	if (m_pBGM != nullptr)
	{
        m_pBGM->stop();
        m_pBGM->drop();
        m_pBGM = klang->play2D(cWavFileName, true, false, true, irrklang::ESM_AUTO_DETECT, false);
	}
	int iVolume = (m_cMusicVolume - 100)*20;
	if (iVolume > 0) iVolume = 0;
	if (iVolume < -10000) iVolume = -10000; //iVolume == Volume
    m_pBGM = klang->play2D(cWavFileName, true, false, true, irrklang::ESM_AUTO_DETECT, false);
}

void CGame::MotionResponseHandler(char * pData)
{
 uint16_t  * wp, wResponse;
 short * sp, sX, sY;
 char  * cp, cDir;
 int   * ip, iPreHP;
	//						          0 3        4 5						 6 7		8 9		   10	    11
	// Confirm Code(4) | MsgSize(4) | MsgID(4) | OBJECTMOVE_CONFIRM(2) | Loc-X(2) | Loc-Y(2) | Dir(1) | MapData ...
	// Confirm Code(4) | MsgSize(4) | MsgID(4) | OBJECTMOVE_REJECT(2)  | Loc-X(2) | Loc-Y(2)
	wp = (uint16_t *)(pData + INDEX2_MSGTYPE);
	wResponse = *wp;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	switch (wResponse) {
	case OBJECTMOTION_CONFIRM:
		m_cCommandCount--;
		break;

	case OBJECTMOTION_ATTACK_CONFIRM:
		m_cCommandCount--;
		break;

	case OBJECTMOTION_REJECT:
		bSendCommand(MSGID_CONFIRMBUMP);
		if (m_iHP <= 0) return;

		sp = (short *)cp;
		m_sPlayerX = *sp;
		cp += 2;

		sp = (short *)cp;
		m_sPlayerY = *sp;
		cp += 2;

		m_cCommand = OBJECTSTOP;
		m_sCommX = m_sPlayerX;
		m_sCommY = m_sPlayerY;

		m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
						                  m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
										  m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue, // Re-Coding Sprite xRisenx
										  m_iPlayerStatus, m_cPlayerName,
										  OBJECTSTOP, 0, 0, 0, 0, 5);
		m_cCommandCount = 0;
		m_bIsGetPointingMode = false;
		//m_sViewDstX = m_sViewPointX = (m_sPlayerX-10)*32;
		//m_sViewDstY = m_sViewPointY = (m_sPlayerY-7)*32;
        m_sViewDstX = m_sViewPointX = (m_sPlayerX-12)*32; // 800x600 Resolution xRisenx Center Char xRisenx
        m_sViewDstY = m_sViewPointY = (m_sPlayerY-9)*32; // 800x600 Resolution xRisenx Center Char xRisenx

        m_sViewDstX = m_sViewPointX = (m_sPlayerX - (GetWidth() / 32) / 2) * 32;
        m_sViewDstY = m_sViewPointY = (m_sPlayerY - ((GetHeight() - 60) / 32) / 2) * 32;
        //m_sViewDstX = m_sViewPointX = (m_sPlayerX-24)*32; // 800x600 Resolution xRisenx Center Char xRisenx
        //m_sViewDstY = m_sViewPointY = (m_sPlayerY-18)*32; // 800x600 Resolution xRisenx Center Char xRisenx

		m_bIsRedrawPDBGS = true;
		break;

	case OBJECTMOVE_CONFIRM:
		sp = (short *)cp;
		sX = *sp;
		cp += 2;
		sp = (short *)cp;
		sY = *sp;
		cp += 2;
		cDir = *cp;
		cp++;
		m_iSP = m_iSP - *cp;
		cp++;
		if( m_iSP < 0 ) m_iSP = 0;

		iPreHP = m_iHP;
		ip = (int *)cp;
		m_iHP = *ip;
		cp += 4;

		if (m_iHP != iPreHP)
		{	if (m_iHP < iPreHP)
			{	wsprintfA(G_cTxt, NOTIFYMSG_HP_DOWN, iPreHP - m_iHP);
				AddEventList(G_cTxt, 10);
				m_dwDamagedTime = unixtime();
				if ((m_cLogOutCount>0) && (m_bForceDisconn==false))
				{	m_cLogOutCount = -1;
					AddEventList(MOTION_RESPONSE_HANDLER2, 10);
				}
			}else
			{	wsprintfA(G_cTxt, NOTIFYMSG_HP_UP, m_iHP - iPreHP);
				AddEventList(G_cTxt, 10);
		}	}
		m_pMapData->ShiftMapData(cDir);
		_ReadMapData(sX, sY, cp);
		m_bIsRedrawPDBGS = true;
		m_cCommandCount--;
		break;

	case OBJECTMOVE_REJECT:
		if (m_iHP <= 0) return;
		wp = (uint16_t *)cp;
		if (m_sPlayerObjectID != *wp) return;
		cp += 2;
		sp  = (short *)cp;
		m_sPlayerX = *sp;
		cp += 2;
		sp  = (short *)cp;
		m_sPlayerY = *sp;
		cp += 2;
		sp  = (short *)cp;
		m_sPlayerType = *sp;
		cp += 2;
		m_cPlayerDir = *cp;
		cp++;
		//memcpy(cName, cp, 10);
		cp += 10;
		sp = (short *)cp;
		m_sPlayerAppr1 = *sp;
		cp += 2;
		sp = (short *)cp;
		m_sPlayerAppr2 = *sp;
		cp += 2;
		sp = (short *)cp;
		m_sPlayerAppr3 = *sp;
		cp += 2;
		sp = (short *)cp;
		m_sPlayerAppr4 = *sp;
		cp += 2;
		ip = (int *)cp;
		m_iPlayerApprColor = *ip;
		cp += 4;
		sp = (short *)cp; // This is right as i used no armor when i crashed..
		m_sPlayerHeadApprValue = *sp; // Re-Coding Sprite xRisenx
		cp += 2;
		sp = (short *)cp;
		m_sPlayerBodyApprValue = *sp; // Re-Coding Sprite xRisenx
		cp += 2;
		sp = (short *)cp;
		m_sPlayerArmApprValue = *sp; // Re-Coding Sprite xRisenx
		cp += 2;
		sp = (short *)cp;
		m_sPlayerLegApprValue = *sp; // Re-Coding Sprite xRisenx
		cp += 2;
		ip = (int *)cp;
		m_iPlayerStatus = *ip;
		cp += 4;
		m_cCommand = OBJECTSTOP;
		m_sCommX = m_sPlayerX;
		m_sCommY = m_sPlayerY;
		m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
										  m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
										  m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue, // Re-Coding Sprite xRisenx
										  m_iPlayerStatus, m_cPlayerName,
										  OBJECTSTOP, 0, 0, 0,
										  0, 11);
		m_cCommandCount = 0;
		m_bIsGetPointingMode = false;
		//m_sViewDstX = m_sViewPointX = (m_sPlayerX-10)*32;
		//m_sViewDstY = m_sViewPointY = (m_sPlayerY-7)*32;
		//m_sViewDstX = m_sViewPointX = (m_sPlayerX-12)*32; // 800x600 Resolution xRisenx Center Char xRisenx
		//m_sViewDstY = m_sViewPointY = (m_sPlayerY-9)*32; // 800x600 Resolution xRisenx Center Char xRisenx

        m_sViewDstX = m_sViewPointX = (m_sPlayerX - 24) * 32; // 800x600 Resolution xRisenx Center Char xRisenx
        m_sViewDstY = m_sViewPointY = (m_sPlayerY - 18) * 32; // 800x600 Resolution xRisenx Center Char xRisenx

        m_sViewDstX = m_sViewPointX = (m_sPlayerX - (GetWidth() / 32) / 2) * 32;
        m_sViewDstY = m_sViewPointY = (m_sPlayerY - ((GetHeight() - 60) / 32) / 2) * 32;

        m_bIsPrevMoveBlocked = true;
		switch (m_sPlayerType) {
		case 1:
		case 2:
		case 3:
			PlaySound('C', 12, 0);
			break;
		case 4:
		case 5:
		case 6:
			PlaySound('C', 13, 0);
			break;
		}
		//m_bCommandAvailable = TRUE;
		break;
	}
}


void CGame::CommandProcessor(short msX, short msY, short indexX, short indexY, char cLB, char cRB, char cMB)
{
	char   cDir, absX, absY, cName[12], dlgID;
	short  sX, sY, sObjectType, tX, tY, dynObjectType;
	int iObjectStatus;
	int    iRet;
	uint32_t  dwTime = unixtime();
	static uint32_t lastPanTime = 0;
	uint16_t   wType = 0;
	int i;//, iFOE;
	char   cTxt[120];
	#ifdef RemoveCritical
	uint32_t * dwp, dwDamage;
	#endif
	char  pDstName[21];
	short sDstOwnerType;
	int iDstOwnerStatus;

	#ifdef RemoveCritical
	dwDamage = 0;
	#endif
	bool  bGORet;
 // Fixed by Snoopy
	if ((m_bIsObserverCommanded == false) && (m_bIsObserverMode == true) && (dwTime - lastPanTime) > 100 )
	{	
		if ((msX == 0) && (msY == 0) && (m_sViewDstX > 32*21) && (m_sViewDstY > 32*16)) bSendCommand(MSGID_REQUEST_PANNING, 0, 8, 0, 0, 0, 0);
		else
		//if ((msX == 639) && (msY == 0) && (m_sViewDstX < 32*m_pMapData->m_sMapSizeX - 32*21) && (m_sViewDstY > 32*16)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 2, NULL, NULL, NULL, NULL);
		if ((msX == GetWidth()-1) && (msY == 0) && (m_sViewDstX < 32*m_pMapData->m_sMapSizeX - 32*21) && (m_sViewDstY > 32*16)) bSendCommand(MSGID_REQUEST_PANNING, 0, 2, 0, 0, 0, 0); // 800x600 Resolution xRisenx
		else
		//if ((msX == 639) && (msY == 479) && (m_sViewDstX < 32*m_pMapData->m_sMapSizeX - 32*21) && (m_sViewDstY < 32*m_pMapData->m_sMapSizeY - 32*16)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 4, NULL, NULL, NULL, NULL);
		if ((msX == GetWidth()-1) && (msY == GetHeight()-1) && (m_sViewDstX < 32*m_pMapData->m_sMapSizeX - 32*21) && (m_sViewDstY < 32*m_pMapData->m_sMapSizeY - 32*16)) bSendCommand(MSGID_REQUEST_PANNING, 0, 4, 0, 0, 0, 0); // 800x600 Resolution xRisenx
		else
		//if ((msX == 0) && (msY == 479) && (m_sViewDstX > 32*21) && (m_sViewDstY < 32*m_pMapData->m_sMapSizeY - 32*16) ) bSendCommand(MSGID_REQUEST_PANNING, NULL, 6, NULL, NULL, NULL, NULL);
		if ((msX == 0) && (msY == GetHeight()-1) && (m_sViewDstX > 32*21) && (m_sViewDstY < 32*m_pMapData->m_sMapSizeY - 32*16) ) bSendCommand(MSGID_REQUEST_PANNING, 0, 6, 0, 0, 0, 0); // 800x600 Resolution xRisenx
		else
		if ((msX == 0) && (m_sViewDstX > 32*21)) bSendCommand(MSGID_REQUEST_PANNING, 0, 7, 0, 0, 0, 0);
		else
		//if ((msX == 639) && (m_sViewDstX < 32*m_pMapData->m_sMapSizeX - 32*21)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 3, NULL, NULL, NULL, NULL);
		if ((msX == GetWidth()-1) && (m_sViewDstX < 32*m_pMapData->m_sMapSizeX - 32*21)) bSendCommand(MSGID_REQUEST_PANNING, 0, 3, 0, 0, 0, 0); // 800x600 Resolution xRisenx
		else
		if ((msY == 0) && (m_sViewDstY > 32*16)) bSendCommand(MSGID_REQUEST_PANNING, 0, 1, 0, 0, 0, 0);
		else
		//if ((msY == 479) && (m_sViewDstY < 32*m_pMapData->m_sMapSizeY - 32*16)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 5, NULL, NULL, NULL, NULL);
		if ((msY == GetHeight()-1) && (m_sViewDstY < 32*m_pMapData->m_sMapSizeY - 32*16)) bSendCommand(MSGID_REQUEST_PANNING, 0, 5, 0, 0, 0, 0); // 800x600 Resolution xRisenx
		else return;
		lastPanTime = dwTime;
		m_bIsObserverCommanded = true;
		m_cArrowPressed = 0;
		return;
	}

	if (m_bIsObserverMode == true) return;

	 m_altPressed = m_bSuperAttackMode = (GetAsyncKeyState(VK_MENU)>>15) ? true : false;

	switch (m_stMCursor.cPrevStatus) {
	case CURSORSTATUS_NULL:
		if (cLB != 0)
		{	
			iRet = _iCheckDlgBoxFocus(1);
			if (iRet == 1)
			{	
				m_stMCursor.cPrevStatus	= CURSORSTATUS_SELECTED;
				return;
			}else if (iRet == 0)
			{	
				m_stMCursor.cPrevStatus = CURSORSTATUS_PRESSED;
				// Snoopy: Added Golden LevelUp
				//if ((msX >560) && (msX <620) && (msY>390) && (msY<405) && (m_iLU_Point >0))
				if ((msX >= GetWidth()-90) && (msX <= GetWidth()) && (msY >= GetHeight()-1) && (msY <= GetHeight()-65) && (m_iLU_Point >0)) // 800x600 Resolution xRisenx new location for levelup button
				{	
					if (m_bIsDialogEnabled[12] != true)
					{
						EnableDialogBox(12, 0, 0, 0);
						PlaySound('E', 14, 5);
					}
					m_stMCursor.cPrevStatus = CURSORSTATUS_NULL;
					return;
				}
			}
			else if (iRet == -1)
			{	
				return;
			}
		}
		else if (cRB != 0)
		{	
			iRet = _iCheckDlgBoxFocus(2);
			if (iRet == 1) return;
		}
		break;
	case CURSORSTATUS_PRESSED:
		if (cLB == 0) // Normal Click
		{	
			m_stMCursor.cPrevStatus = CURSORSTATUS_NULL;
		}
		break;
	case CURSORSTATUS_SELECTED:
		if (cLB == 0)
		{	
			//DIRECTX//DIRECTX m_dInput.m_sZ = 0;
			dlgID = MouseOverDialog();

			if (dlgID != 0)
			{
				if(m_dialogBoxes[dlgID].HasDblClick() &&
					dwTime - m_stMCursor.dwSelectClickTime < DOUBLECLICKTIME
					&& abs(msX - m_stMCursor.sClickX) < 2 && abs(msY - m_stMCursor.sClickY) <2 )
				{	
					m_stMCursor.dwSelectClickTime = m_stMCursor.dwSelectClickTime;
					m_dialogBoxes[dlgID].OnDblClick();
				}
				else { //Need else?
					m_dialogBoxes[dlgID].OnClick();
				}
			}

				m_stMCursor.sClickX = msX;
				m_stMCursor.sClickY = msY;

			m_stMCursor.dwSelectClickTime = dwTime;
			m_stMCursor.cPrevStatus = CURSORSTATUS_NULL;
			if (m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_ITEM)
			{	
				_bCheckDraggingItemRelease(dlgID);
				m_stMCursor.cSelectedObjectType = 0;
				m_stMCursor.sSelectedObjectID   = 0;
			}
			return;
		}
		if (cLB != 0)
		{	
			if ((m_pMapData->bIsTeleportLoc(m_sPlayerX, m_sPlayerY) == true) && (m_cCommandCount == 0)) goto CP_SKIPMOUSEBUTTONSTATUS;

			if ((m_stMCursor.sPrevX != msX)	|| (m_stMCursor.sPrevY != msY))
			{	
				m_stMCursor.cPrevStatus = CURSORSTATUS_DRAGGING;
				m_stMCursor.sPrevX = msX;
				m_stMCursor.sPrevY = msY;
				if ( (m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_DLGBOX) &&
					 ((m_stMCursor.sSelectedObjectID == 30) || (m_stMCursor.sSelectedObjectID == 29)) )
				{	
					m_stMCursor.cPrevStatus = CURSORSTATUS_NULL;
				}
				if ((m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_DLGBOX) &&
					(m_stMCursor.sSelectedObjectID == 7) && (m_dialogBoxes[7].GetMode() == 1))
				{	EndInputString();
					m_dialogBoxes[7].SetMode(3);
				}
				// Query Drop Item Amount
				if ((m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_DLGBOX) &&
					(m_stMCursor.sSelectedObjectID == 17) && (m_dialogBoxes[17].GetMode() == 1))
					// Guild Menu
				{	EndInputString();
					m_dialogBoxes[17].SetMode(20);
				}
				// guild contribute amount
				if(m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_DLGBOX &&
					m_stMCursor.sSelectedObjectID == 58 && m_dialogBoxes[58].GetMode() == 1)
				{
					EndInputString();
					m_dialogBoxes[58].SetMode(20);
				}
				return;
			}
			if ((m_cCommand == OBJECTMOVE) || (m_cCommand == OBJECTRUN)) goto MOTION_COMMAND_PROCESS;
			return;
		}
		break;
	case CURSORSTATUS_DRAGGING:
		if (cLB != 0)
		{	
			if ((m_pMapData->bIsTeleportLoc(m_sPlayerX, m_sPlayerY) == true) && (m_cCommandCount == 0)) goto CP_SKIPMOUSEBUTTONSTATUS;
			
			if (m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_DLGBOX)
			{	m_dialogBoxes[m_stMCursor.sSelectedObjectID].m_X = msX - m_stMCursor.sDistX;
				m_dialogBoxes[m_stMCursor.sSelectedObjectID].m_Y = msY - m_stMCursor.sDistY;
			}
			m_stMCursor.sPrevX = msX;
			m_stMCursor.sPrevY = msY;

			if ((m_cCommand == OBJECTMOVE) || (m_cCommand == OBJECTRUN)) goto MOTION_COMMAND_PROCESS;
			return;
		}
		if (cLB == 0) {
			switch (m_stMCursor.cSelectedObjectType) {
			case SELECTEDOBJTYPE_DLGBOX:
				if ((m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_DLGBOX) &&
					(m_stMCursor.sSelectedObjectID == 7) && (m_dialogBoxes[7].GetMode() == 20))
				{	sX = m_dialogBoxes[7].m_X;
					sY = m_dialogBoxes[7].m_Y;
					StartInputString(sX + 75, sY + 140, 21, m_cGuildName);
					m_dialogBoxes[7].SetMode(1);
				}

				if ((m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_DLGBOX) &&
					(m_stMCursor.sSelectedObjectID == 17) && (m_dialogBoxes[17].GetMode() == 20))
				{	// Query Drop Item Amount
					sX = m_dialogBoxes[17].m_X;
					sY = m_dialogBoxes[17].m_Y;
					StartInputString(sX + 40, sY + 57, 11, m_cAmountString);
					m_dialogBoxes[17].SetMode(1);
				}

				// guild contribute amount
				if(m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_DLGBOX &&
					m_stMCursor.sSelectedObjectID == 58 && m_dialogBoxes[58].GetMode() == 20)
				{
					sX = m_dialogBoxes[58].m_X;
					sY = m_dialogBoxes[58].m_Y;
					StartInputString(sX + 40, sY + 57, 11, m_cAmountString);
					m_dialogBoxes[58].SetMode(1);
				}

				if ( m_stMCursor.sSelectedObjectID == 9 )
				{	
					//if( msX < 320 ) m_dialogBoxes[9].m_X = 0;
					if( msX < GetWidth()/2 ) m_dialogBoxes[9].m_X = 0; // 800x600 Resolution xRisenx
					//else m_dialogBoxes[9].m_X = 640 - m_dialogBoxes[9].sSizeX;
					else m_dialogBoxes[9].m_X = GetWidth() - m_dialogBoxes[9].sSizeX; // 800x600 Resolution xRisenx
					//if( msY < 213 ) m_dialogBoxes[9].m_Y = 0;
					if( msY < 273 ) m_dialogBoxes[9].m_Y = 0; // 800x600 Resolution xRisenx
					//else m_dialogBoxes[9].m_Y = 427 - m_dialogBoxes[9].sSizeY;
					else m_dialogBoxes[9].m_Y = GetHeight()-53 - m_dialogBoxes[9].sSizeY; // 800x600 Resolution xRisenx
				}

				m_stMCursor.cPrevStatus = CURSORSTATUS_NULL;
				m_stMCursor.cSelectedObjectType = 0;
				m_stMCursor.sSelectedObjectID   = 0;
				break;

			case SELECTEDOBJTYPE_ITEM:
				dlgID = MouseOverDialog();
				if(dlgID && m_dialogBoxes[dlgID].HasDblClick() &&
					abs(m_stMCursor.sPrevX - msX) < 2 && 
					abs(m_stMCursor.sPrevY - msY) < 2 &&
					(dwTime - m_stMCursor.dwSelectClickTime) < DOUBLECLICKTIME)
				{
					m_dialogBoxes[dlgID].OnDblClick();
				}
				else
				{			
					_bCheckDraggingItemRelease(dlgID);
				}
				m_stMCursor.cPrevStatus = CURSORSTATUS_NULL;
				m_stMCursor.cSelectedObjectType = 0;
				m_stMCursor.sSelectedObjectID   = 0;
				break;

			default:
				m_stMCursor.cPrevStatus = CURSORSTATUS_NULL;
				m_stMCursor.cSelectedObjectType = 0;
				m_stMCursor.sSelectedObjectID   = 0;
				break;
			}
			return;
		}
		break;
	}

CP_SKIPMOUSEBUTTONSTATUS:;
	if (m_bCommandAvailable == false) return;
	if ( (dwTime - m_dwCommandTime) < 300 )
	{
		gamemode = 0;
		_socket->stop();
		m_bEscPressed = false;
		PlaySound('E', 14, 5);
        if (m_bSoundFlag) m_pESound[38]->stop();
		if ((m_bSoundFlag) && (m_bMusicStat == true))
		{
    		if (m_pBGM != nullptr) m_pBGM->stop();
		}
		isItemLoaded = false;
		ChangeGameMode(GAMEMODE_ONMAINMENU);
		return;
	}
	if (m_iHP <= 0) return;

	if (m_sDamageMove != 0)
	{	m_cCommand = OBJECTDAMAGEMOVE;
		goto MOTION_COMMAND_PROCESS;
	}

	if (m_pMapData->bIsTeleportLoc(m_sPlayerX, m_sPlayerY) == true && m_cCommandCount == 0)
		RequestTeleportAndWaitData();

	// indexX, indexY
	if (cLB != 0) // Mouse Left button
	{	if (m_bIsGetPointingMode == true)
		{	if ((m_sMCX != 0) || (m_sMCY != 0))
				 PointCommandHandler(m_sMCX, m_sMCY);
			else PointCommandHandler(indexX, indexY);

			m_bCommandAvailable  = false;
			m_dwCommandTime = unixtime();
			m_bIsGetPointingMode = false;
			return;
		}

		m_pMapData->bGetOwner(m_sMCX, m_sMCY-1, cName, &sObjectType, &iObjectStatus, &m_wCommObjectID);
		//m_pMapData->m_pData[dX][dY].m_sItemSprite
		if (memcmp(m_cMCName, m_cPlayerName, 10) == 0 && ( sObjectType <= 6 || m_pMapData->m_pData[m_sPlayerX-m_pMapData->m_sPivotX][m_sPlayerY-m_pMapData->m_sPivotY].m_sItemSprite != 0 ))
		{//if (memcmp(m_cMCName, m_cPlayerName, 10) == 0 && ( sObjectType <= 6 || m_pMapData->m_pData[15][15].m_sItemSprite != 0 )) {
		 //if (memcmp(m_cMCName, m_cPlayerName, 10) == 0 && sObjectType <= 6){
			if ((m_sPlayerType >= 1) && (m_sPlayerType <= 6)/* && ((m_sPlayerAppr2 & 0xF000) == 0)*/)
			{	m_cCommand = OBJECTGETITEM;
				m_sCommX = m_sPlayerX;
				m_sCommY = m_sPlayerY;
			}
		}else
		{	if( memcmp(m_cMCName, m_cPlayerName, 10) == 0 ) m_sMCY -= 1;
			if ((m_sMCX != 0) && (m_sMCY != 0)) // m_sMCX, m_sMCY
			{
				if (m_bCtrlPressed == true)
				{	m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &iObjectStatus, &m_wCommObjectID);
					if ( (iObjectStatus & 0x10) != 0) return;
					if ((sObjectType == 15) || (sObjectType == 20) || (sObjectType == 24)) return;
					m_stMCursor.sCursorFrame = 3;
					absX = abs(m_sPlayerX - m_sMCX);
					absY = abs(m_sPlayerY - m_sMCY);
					if ((absX <= 1) && (absY <= 1))
					{	wType = _iGetAttackType();
						m_cCommand = OBJECTATTACK;
						m_sCommX = m_sMCX;
						m_sCommY = m_sMCY;
					}else if ( (absX <= 3) && (absY <= 2) // strike on Big mobs & gate from a range
							&& ((sObjectType == 66)||(sObjectType == 73)||(sObjectType == 81)||(sObjectType == 91)))
					{	wType = _iGetAttackType();
						m_cCommand = OBJECTATTACK;
						m_sCommX = m_sMCX;
						m_sCommY = m_sMCY;
					}else // Pas au corp � corp
					{	switch (_iGetWeaponSkillType()) {
						case 6: // Bow
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
							wType = _iGetAttackType();
							break;

						case 5: // OpenHand
						case 7: // SS
							if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0)))
							{	if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
								{	if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
									{	m_cCommand = OBJECTATTACKMOVE;
										wType = _iGetAttackType();
									}else
									{	m_cCommand = OBJECTRUN;
										GetPlayerTurn();
									}
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
								}else
								{	m_cCommand = OBJECTMOVE;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									GetPlayerTurn();
								}
							}else
							{	if (   (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0)
									&& (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
									 m_cCommand = OBJECTRUN;	// Staminar
								else m_cCommand = OBJECTMOVE;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
								GetPlayerTurn();
							}
							break;

						case 8: // LS
							if (   (absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
								&& (_iGetAttackType() != 30)) // Crit without StormBlade
							{	wType = _iGetAttackType();
								m_cCommand = OBJECTATTACK;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
							}else if ( (absX <= 5) && (absY <= 5) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
								&& (_iGetAttackType() == 30))  // Crit with StormBlade (by Snoopy)
							{	wType = _iGetAttackType();
								m_cCommand = OBJECTATTACK;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
							}else if ( (absX <= 3) && (absY <= 3)
								&& (_iGetAttackType() == 5))  // Normal hit with StormBlade (by Snoopy)
							{	wType = _iGetAttackType();
								m_cCommand = OBJECTATTACK;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
							}else // Swing
							{	if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0))
									&& (_iGetAttackType() != 5)) // no Dash possible with StormBlade
								{	if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
									{	if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
										{	m_cCommand = OBJECTATTACKMOVE;
											wType = _iGetAttackType();
										}else
										{	m_cCommand = OBJECTRUN;
											GetPlayerTurn();
										}
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
									}else
									{	m_cCommand = OBJECTMOVE;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										GetPlayerTurn();
									}
								}else
								{	if (   (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0)
										&& (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
										 m_cCommand = OBJECTRUN;
									else m_cCommand = OBJECTMOVE;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									GetPlayerTurn();
							}	}
							break;

						case 9: // Fencing
							if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
							{	m_cCommand = OBJECTATTACK;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
								wType = _iGetAttackType();
							}
							else {
								if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0))) {
									if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0)) {
										if (m_cSkillMastery[_iGetWeaponSkillType()] == 100) {
											m_cCommand = OBJECTATTACKMOVE;
											wType = _iGetAttackType();
										}
										else {
											m_cCommand = OBJECTRUN;
											GetPlayerTurn();
										}
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
									}
									else {
										m_cCommand = OBJECTMOVE;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										GetPlayerTurn();
									}
								}
								else {
									if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
										(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
										 m_cCommand = OBJECTRUN;
									else m_cCommand = OBJECTMOVE;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									GetPlayerTurn();
								}
							}
							break;

						case 10: // Axe
							if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
							{	m_cCommand = OBJECTATTACK;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
								wType = _iGetAttackType();
							}else
							{	if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0)))
								{	if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
									{	if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
										{	m_cCommand = OBJECTATTACKMOVE;
											wType = _iGetAttackType();
										}else
										{	m_cCommand = OBJECTRUN;
											GetPlayerTurn();
										}
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
									}else
									{	m_cCommand = OBJECTMOVE;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										GetPlayerTurn();
									}
								}else
								{	if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
										(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
										 m_cCommand = OBJECTRUN;
									else m_cCommand = OBJECTMOVE;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									GetPlayerTurn();
							}	}
							break;
						case 14: // Hammer
							if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)) {
								m_cCommand = OBJECTATTACK;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
								wType = _iGetAttackType();
							}
							else {
								if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0))) {
									if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0)) {
										if (m_cSkillMastery[_iGetWeaponSkillType()] == 100) {
											m_cCommand = OBJECTATTACKMOVE;
											wType = _iGetAttackType();
										}
										else {
											m_cCommand = OBJECTRUN;
											GetPlayerTurn();
										}
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
									}
									else {
										m_cCommand = OBJECTMOVE;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										GetPlayerTurn();
									}
								}
								else {
									if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
										(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
										 m_cCommand = OBJECTRUN;
									else m_cCommand = OBJECTMOVE;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									GetPlayerTurn();
							}	}
							break;
						case 21: // Wand
							if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)) {
								m_cCommand = OBJECTATTACK;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
								wType = _iGetAttackType();
							} // Battle Mages xRisenx
							else if ( (absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
								&& (_iGetAttackType() == 30))  // Crit with StormBlade Using SB as base.
							{	wType = _iGetAttackType();
								m_cCommand = OBJECTATTACK;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
							//
							}
							//else if ( (absX <= 1) && (absY <= 3)
							//	&& (_iGetAttackType() == 5))  // Normal hit with StormBlade Using SB as base.
							//{	wType = _iGetAttackType();
							//	m_cCommand = OBJECTATTACK;
							//	m_sCommX = m_sMCX;
							//	m_sCommY = m_sMCY;
							//} // Battle Mages xRisenx
							else {
								if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0)) && (_iGetAttackType() != 5)) { // && (_iGetAttackType() != 5) Battle Mages xRisenx
									if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0)) {
										if (m_cSkillMastery[_iGetWeaponSkillType()] == 100) {
											m_cCommand = OBJECTATTACKMOVE;
											wType = _iGetAttackType();
										}
										else {
											m_cCommand = OBJECTRUN;
											GetPlayerTurn();
										}
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
									}
									else {
										m_cCommand = OBJECTMOVE;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										GetPlayerTurn();
									}
								}
								else {
									if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
										(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
										 m_cCommand = OBJECTRUN;
									else m_cCommand = OBJECTMOVE;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									GetPlayerTurn();
								}
							}
							break;
						}
					}
				}else // CTRL not pressed
				{	m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &iObjectStatus, &m_wCommObjectID, &dynObjectType);
					if (sObjectType >= 10 || ((sObjectType >= 1) && (sObjectType <= 6)))
					{
						switch (sObjectType) { 	// CLEROTH - NPC TALK
						case 15: // Jehovah - Changed so the shopkeeper reads case 8.
							EnableDialogBox(20, 8, 0, 0);
							//m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
							m_dialogBoxes[20].sV3 = 15;
							break;

						case 19: // Gandlf
							EnableDialogBox(20, 0, 16, 0);
							//m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
							m_dialogBoxes[20].sV3 = 19;
							break;

						case 20: // Howard
							EnableDialogBox(20, 0, 14, 0);
							//m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
							m_dialogBoxes[20].sV3 = 20;
							m_dialogBoxes[39].sV3 = 20;
							m_dialogBoxes[39].sV4 = m_wCommObjectID;
							m_dialogBoxes[39].sV5 = m_sMCX;
							m_dialogBoxes[39].sV6 = m_sMCY;
							break;

						case NPC_PRINCESS:
							EnableDialogBox(20, 0, 56, 0);
							//m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
							m_dialogBoxes[20].sV3 = 20;
							m_dialogBoxes[39].sV3 = 20;
							m_dialogBoxes[39].sV4 = m_wCommObjectID;
							m_dialogBoxes[39].sV5 = m_sMCX;
							m_dialogBoxes[39].sV6 = m_sMCY;
							break;

						case 24:
							EnableDialogBox(20, 7, 0, 0);
							//m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
							m_dialogBoxes[20].sV3 = 24;
							m_dialogBoxes[39].sV3 = 24;
							m_dialogBoxes[39].sV4 = m_wCommObjectID;
							m_dialogBoxes[39].sV5 = m_sMCX;
							m_dialogBoxes[39].sV6 = m_sMCY;
							break;

						case 25:
							EnableDialogBox(20, 9, 0, 0);
							//m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
							m_dialogBoxes[20].sV3 = 25;
							break;

						case 26: // Kennedy
							EnableDialogBox(20, 0, 7, 0);
							//m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
							m_dialogBoxes[20].sV3 = 26;
							break;

						case 21: // Guard
							if ((_iGetFOE(iObjectStatus)>=0) && (!m_bIsCombatMode))
							{	
								EnableDialogBox(20, 4, 0, 0);
								//m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
								m_dialogBoxes[20].sV3 = 21;
							}
							break;
						case 67: // McGaffin
						case 68: // Perry
						case 69: // Devlin
							if (!m_bIsCombatMode)
							{	
								EnableDialogBox(20, 4, 0, 0);
								//m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
								m_dialogBoxes[20].sV3 = sObjectType;
							}
							break;

						case 90: // Snoopy: Gail
							EnableDialogBox(20, 6, 0, 0);
							//m_dialogBoxes[20].//CentreOverPoint(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
							m_dialogBoxes[20].sV3 = 90;
							break;

						default: // Other mobs
							if ( _iGetFOE(iObjectStatus) >= 0 && m_targetNeutral == false) break;
							if (iObjectStatus & STATUS_INVISIBILITY) break;
							if ( (sObjectType>=1) && (sObjectType<=6) && (m_bForceAttack==false) ) break;
							absX = abs(m_sPlayerX - m_sMCX);
							absY = abs(m_sPlayerY - m_sMCY);
							if ((absX <= 1) && (absY <= 1))
							{	wType = _iGetAttackType();
								m_cCommand = OBJECTATTACK;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
							}else if ( (absX <= 2) && (absY <= 2) // strike on Big mobs & gate from a range
							&& ((sObjectType == 66)||(sObjectType == 73)||(sObjectType == 81)||(sObjectType == 91)))
							{	wType = _iGetAttackType();
								m_cCommand = OBJECTATTACK;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
							}else // Normal hit from a range.
							{	switch (_iGetWeaponSkillType()) {
								case 6: // Bow
									m_cCommand = OBJECTATTACK;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									wType = _iGetAttackType();
									break;

								case 5: // Boxe
								case 7: // SS
									if (   (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0)
										&& (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
										m_cCommand = OBJECTRUN;
									else m_cCommand = OBJECTMOVE;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									GetPlayerTurn();
									break;

								case 8: // LS
									if (   (absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
										&& (_iGetAttackType() != 30)) // Crit without StormBlade by Snoopy
									{	if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
											 m_cCommand = OBJECTATTACKMOVE;
										else m_cCommand = OBJECTATTACK;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										wType = _iGetAttackType();
									}else if ((absX <= 5) && (absY <= 5) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
										   && (_iGetAttackType() == 30)) // Crit with StormBlade by Snoopy
									{	if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
											 m_cCommand = OBJECTATTACKMOVE;
										else m_cCommand = OBJECTATTACK;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										wType = _iGetAttackType();
									}else if ((absX <= 3) && (absY <= 3)
										   && (_iGetAttackType() == 5)) // Normal hit with StormBlade by Snoopy
									{	m_cCommand = OBJECTATTACK;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										wType = _iGetAttackType();
									}else
									{	if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
											(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
											m_cCommand = OBJECTRUN;
										else m_cCommand = OBJECTMOVE;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										GetPlayerTurn();
									}
									break;

								case 9: // Fencing
									if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
									{	if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
											 m_cCommand = OBJECTATTACKMOVE;
										else m_cCommand = OBJECTATTACK;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										wType = _iGetAttackType();
									}else
									{	if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
											(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
											m_cCommand = OBJECTRUN;
										else m_cCommand = OBJECTMOVE;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										GetPlayerTurn();
									}
									break;

								case 10: //
									if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)) {
										if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
											 m_cCommand = OBJECTATTACKMOVE;
										else m_cCommand = OBJECTATTACK;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										wType = _iGetAttackType();
									}
									else {
										if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
											(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
											m_cCommand = OBJECTRUN;
										else m_cCommand = OBJECTMOVE;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										GetPlayerTurn();
									}
									break;
								case 14: //
									if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)) {
										if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
											 m_cCommand = OBJECTATTACKMOVE;
										else m_cCommand = OBJECTATTACK;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										wType = _iGetAttackType();
									}
									else {
										if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
											(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
											m_cCommand = OBJECTRUN;
										else m_cCommand = OBJECTMOVE;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										GetPlayerTurn();
									}
									break;
								case 21: // Wands
									if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (_iGetAttackType() != 30)) { // && (_iGetAttackType() != 30) Battle Mages xRisenx
										if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
											 m_cCommand = OBJECTATTACKMOVE;
										else m_cCommand = OBJECTATTACK;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										wType = _iGetAttackType();
									} // Battle Mages xRisenx
									else if ((absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
										   && (_iGetAttackType() == 30)) // Crit with blatte staffs Using SB as base.
									{	if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
											 m_cCommand = OBJECTATTACKMOVE;
										else m_cCommand = OBJECTATTACK;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										wType = _iGetAttackType();
									}
									//else if ((absX <= 3) && (absY <= 3)
									//	   && (_iGetAttackType() == 5)) // Normal hit with battle staffs Using SB as base.
									//{	m_cCommand = OBJECTATTACK;
									//	m_sCommX = m_sMCX;
									//	m_sCommY = m_sMCY;
									//	wType = _iGetAttackType();
									//} // Battle Mages xRisenx
									else {
										if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
											(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
											m_cCommand = OBJECTRUN;
										else m_cCommand = OBJECTMOVE;
										m_sCommX = m_sMCX;
										m_sCommY = m_sMCY;
										GetPlayerTurn();
									}
									break;
								}
							}
							break;
						}
					}
					else if ((dynObjectType == DYNAMICOBJECT_MINERAL1 || dynObjectType == DYNAMICOBJECT_MINERAL2)
								&& ((m_sPlayerAppr2 & 0x0FF0) >> 4) == 25){ //Weapon = PickAxe
						if (abs(m_sPlayerX - m_sMCX) <= 1 && abs(m_sPlayerY - m_sMCY) <= 1) {
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
							wType = _iGetAttackType();
						}
						else {
							if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
								(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
								m_cCommand = OBJECTRUN;
							else m_cCommand = OBJECTMOVE;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
							GetPlayerTurn();
						}
					}
					else {
						if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
							(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
							 m_cCommand = OBJECTRUN;
						else m_cCommand = OBJECTMOVE;
						m_sCommX = m_sMCX;
						m_sCommY = m_sMCY;
						GetPlayerTurn();
					}
				}
			}else
			{	if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
					(m_sPlayerType >= 1) && (m_sPlayerType <= 6))
					 m_cCommand = OBJECTRUN;
				else m_cCommand = OBJECTMOVE;
				m_sCommX = indexX;
				m_sCommY = indexY;
				GetPlayerTurn();
			}
		}
	}else if (cRB != 0/* && !m_altPressed*/) // Mouse Right button
	{	m_cCommand = OBJECTSTOP;
		if (m_bIsGetPointingMode == true)
		{	m_bIsGetPointingMode = false;
			AddEventList(COMMAND_PROCESSOR1, 10);
		}
		if (m_bCommandAvailable == false) return;
		if (m_cCommandCount >= 6) return;

		if ((m_sMCX != 0) && (m_sMCY != 0))
		{	absX = abs(m_sPlayerX - m_sMCX);
			absY = abs(m_sPlayerY - m_sMCY);
			if( absX==0 && absY==0 ) return;

			if (m_bCtrlPressed == true)
			{	m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &iObjectStatus, &m_wCommObjectID);
				if ( (iObjectStatus & 0x10) != 0) return;
				if ((sObjectType == 15) || (sObjectType == 20) || (sObjectType == 24)) return;

				if ((absX <= 1) && (absY <= 1))
				{	wType = _iGetAttackType();
					m_cCommand = OBJECTATTACK;
					m_sCommX = m_sMCX;
					m_sCommY = m_sMCY;
				}else if ( (absX <= 2) && (absY <= 2) // strike on Big mobs & gate from a range
							&& ((sObjectType == 66)||(sObjectType == 73)||(sObjectType == 81)||(sObjectType == 91)))
				{	wType = _iGetAttackType();
					m_cCommand = OBJECTATTACK;
					m_sCommX = m_sMCX;
					m_sCommY = m_sMCY;
				}else
				{	switch (_iGetWeaponSkillType()) {
					case 6: // Bow
						m_cCommand = OBJECTATTACK;
						m_sCommX = m_sMCX;
						m_sCommY = m_sMCY;
						wType = _iGetAttackType();
						break;

					case 5: // Boxe
					case 7: // SS
						break;

					case 8: // LS
						if (   (absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
							&& (_iGetAttackType() != 30)) // without StormBlade by Snoopy
						{	wType = _iGetAttackType();
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
						}else if (   (absX <= 5) && (absY <= 5) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
							&& (_iGetAttackType() == 30)) // with stormBlade crit by Snoopy
						{	wType = _iGetAttackType();
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
						}else if (   (absX <= 3) && (absY <= 3)
							&& (_iGetAttackType() == 5)) // with stormBlade no crit by Snoopy
						{	wType = _iGetAttackType();
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
						}
						break;

					case 9: // Fencing
						if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)) {
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
							wType = _iGetAttackType();
						}
						break;

					case 10: //
						if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)) {
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
							wType = _iGetAttackType();
						}
						break;

					case 14: //
						if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)) {
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
							wType = _iGetAttackType();
						}
						break;
					case 21: // Wands
						if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (_iGetAttackType() != 30)) { // && (_iGetAttackType() != 30) Battle Mages xRisenx
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
							wType = _iGetAttackType();
						} // Battle Mages xRisenx
						else if (   (absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
							&& (_iGetAttackType() == 30)) // with stormBlade crit Using SB as base.
						{	wType = _iGetAttackType();
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
						}
						//else if (   (absX <= 3) && (absY <= 3)
						//	&& (_iGetAttackType() == 5)) // with BattleStaff no crit Using SB as base.
						//{	wType = _iGetAttackType();
						//	m_cCommand = OBJECTATTACK;
						//	m_sCommX = m_sMCX;
						//	m_sCommY = m_sMCY;
						//} // Battle Mages xRisenx
						break;
					}
				}
			}else // CTRL not pressed
			{	absX = abs(m_sPlayerX - m_sMCX);
				absY = abs(m_sPlayerY - m_sMCY);
				m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &iObjectStatus, &m_wCommObjectID, &dynObjectType);
				if (sObjectType >= 10 || ((sObjectType >= 1) && (sObjectType <= 6))) {
					switch (sObjectType) {
					case NPC_SHOPKEEPER_W:
					case NPC_GANDLF:
					case NPC_HOWARD:
					case NPC_TOM:
					case NPC_WILLIAM:
					case NPC_KENNEDY:
					case NPC_PRINCESS:
					case NPC_GUARD:
					case NPC_MCGAFFIN:
					case NPC_PERRY:
					case NPC_DEVLIN:
					case NPC_GAIL:
						break;

					default: // All "normal mobs"
						if ( _iGetFOE(iObjectStatus) >= 0 && m_targetNeutral == false ) break;
						if (iObjectStatus & STATUS_INVISIBILITY) break;
						if ( (sObjectType>=1) && (sObjectType<=6) && (m_bForceAttack==false) ) break;
						if ((absX <= 1) && (absY <= 1))
						{	wType = _iGetAttackType();
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
						}else if ( (absX <= 2) && (absY <= 2) // strike on Big mobs & gate from a range
							&& ((sObjectType == 66)||(sObjectType == 73)||(sObjectType == 81)||(sObjectType == 91)))
						{	wType = _iGetAttackType();
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
						}else //
						{	switch (_iGetWeaponSkillType()) {
							case 6: // Bow
								m_cCommand = OBJECTATTACK;
								m_sCommX = m_sMCX;
								m_sCommY = m_sMCY;
								wType = _iGetAttackType();
								break;

							case 5: // Boxe
							case 7: // SS
								break;

							case 8: // LS
								if (   (absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
									&& (_iGetAttackType() != 30)) // crit without StormBlade by Snoopy
								{	wType = _iGetAttackType();
									m_cCommand = OBJECTATTACK;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
								}else if (   (absX <= 5) && (absY <= 5) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
									&& (_iGetAttackType() == 30)) // with stormBlade crit by Snoopy
								{	wType = _iGetAttackType();
									m_cCommand = OBJECTATTACK;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
								}else if (   (absX <= 3) && (absY <= 3)
									&& (_iGetAttackType() == 5)) // with stormBlade no crit by Snoopy
								{	wType = _iGetAttackType();
									m_cCommand = OBJECTATTACK;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
								}
								break;

							case 9: // fencing
								if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)) {
									m_cCommand = OBJECTATTACK;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									wType = _iGetAttackType();
								}
								break;

							case 10: //
								if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)) {
									m_cCommand = OBJECTATTACK;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									wType = _iGetAttackType();
								}
								break;
							case 14: // hammer
								if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)) {
									m_cCommand = OBJECTATTACK;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									wType = _iGetAttackType();
								}
								break;
							case 21: // Wands
								if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (_iGetAttackType() != 30)) { // && (_iGetAttackType() != 30) Battle Mages xRisenx
									m_cCommand = OBJECTATTACK;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
									wType = _iGetAttackType();
								} // Battle Mages xRisenx
								else if (   (absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
									&& (_iGetAttackType() == 30)) // with battlesstaffs crit Magn0S:: Using SB as base.
								{	wType = _iGetAttackType();
									m_cCommand = OBJECTATTACK;
									m_sCommX = m_sMCX;
									m_sCommY = m_sMCY;
								}
								//else if (   (absX <= 3) && (absY <= 3)
								//	&& (_iGetAttackType() == 5)) // with Battlestafs no crit Magn0S:: Using SB as base.
								//{	wType = _iGetAttackType();
								//	m_cCommand = OBJECTATTACK;
								//	m_sCommX = m_sMCX;
								//	m_sCommY = m_sMCY;
								//} // Battle Mages xRisenx
								break;
						}	}
						break;
			}	}
				else if ((dynObjectType == DYNAMICOBJECT_MINERAL1 || dynObjectType == DYNAMICOBJECT_MINERAL2)
					&& ((m_sPlayerAppr2 & 0x0FF0) >> 4) == 25){ //Weapon = PickAxe
						if (absX <= 1 && absY <= 1) {
							m_cCommand = OBJECTATTACK;
							m_sCommX = m_sMCX;
							m_sCommY = m_sMCY;
							wType = _iGetAttackType();
						}
				}

			}
		}else
		{	cDir = m_Misc.cGetNextMoveDir(m_sPlayerX, m_sPlayerY, indexX, indexY);
			if (m_iHP <= 0) return;
			if (cDir == 0) return;
			if (m_cPlayerDir  == cDir) return;
			ClearSkillUsingStatus();
			m_cPlayerDir = cDir;
			bSendCommand(MSGID_MOTION_STOP, 0, m_cPlayerDir, 0, 0, 0, 0);

			m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
							                  m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
											  m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue, // Re-Coding Sprite xRisenx
											  m_iPlayerStatus, m_cPlayerName,
											  m_cCommand, 0, 0, 0, 0,
											  10);
			m_bCommandAvailable = false;
			m_dwCommandTime = unixtime();
			return;
	}	}
	else if(cMB != 0){

	}

MOTION_COMMAND_PROCESS:;

	if (m_cCommand != OBJECTSTOP)
	{
	    if (m_iHP <= 0) return;
		if (m_cCommandCount == 5) AddEventList(COMMAND_PROCESSOR2, 10, false);
		if (m_bCommandAvailable == false) return;
		if (m_cCommandCount >= 6) return;

		if (m_sPlayerType > 6)
		{
		    switch (m_cCommand) {
			case OBJECTRUN:
			case OBJECTMAGIC:
			case OBJECTGETITEM:
				m_cCommand = OBJECTSTOP;
				break;
		    }
		}

		ClearSkillUsingStatus();

		if (m_sDamageMove != 0)
		{
		    m_cCommand = OBJECTDAMAGEMOVE;
			m_sCommX = m_sPlayerX;
			m_sCommY = m_sPlayerY;

			switch (m_sDamageMove)
            {
                case 1: m_sCommY--; break;
                case 2: m_sCommX++; m_sCommY--; break;
                case 3: m_sCommX++; break;
                case 4: m_sCommX++; m_sCommY++; break;
                case 5: m_sCommY++; break;
                case 6: m_sCommX--; m_sCommY++; break;
                case 7: m_sCommX--; break;
                case 8: m_sCommX--; m_sCommY--; break;
			}

			#ifdef RemoveCritical
			for (i = 1; i < MAXCHATMSGS; i++)
			if (m_pChatMsgList[i] == 0)
			{	ZeroMemory(cTxt, sizeof(cTxt));
				if (dwDamage > 0)
					wsprintfA(cTxt, "-%dPts", dwDamage); //pts
				else strcpy(cTxt, "Critical!");

				int iFontType;
				if ((dwDamage >= 0) && (dwDamage < 500))		iFontType = 21;
				else if ((dwDamage >= 500) && (dwDamage < 1250)) iFontType = 22;
				else if ((dwDamage >= 1250) || (dwDamage < 0))	iFontType = 23;

				m_pChatMsgList[i] = new class CMsg(iFontType, cTxt, m_dwCurTime);
				m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;

				if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == false) {
					delete m_pChatMsgList[i];
					m_pChatMsgList[i] = 0;
				}
				break;
			}
			#else
			for (i = 1; i < MAXCHATMSGS; i++)
			if (m_pChatMsgList[i] == 0)
			{	ZeroMemory(cTxt, sizeof(cTxt));
				if (m_sDamageMoveAmount > 0)
					wsprintfA(cTxt, "-%d", m_sDamageMoveAmount); //pts
				else strcpy(cTxt, COMMAND_PROCESSOR6);

				int iFontType;
				if ((m_sDamageMoveAmount >= 0) && (m_sDamageMoveAmount < 12))		iFontType = 21;
				else if ((m_sDamageMoveAmount >= 12) && (m_sDamageMoveAmount < 40)) iFontType = 22;
				else if ((m_sDamageMoveAmount >= 40) || (m_sDamageMoveAmount < 0))	iFontType = 23;

				m_pChatMsgList[i] = new class CMsg(iFontType, cTxt, m_dwCurTime);
				m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;

				if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == false) {
					delete m_pChatMsgList[i];
					m_pChatMsgList[i] = 0;
				}
				break;
			}
		#endif
			m_sDamageMove = 0;
		}

		switch (m_cCommand) {
		case OBJECTRUN:
		case OBJECTMOVE:
		case OBJECTDAMAGEMOVE:

			if( m_bParalyze ) return;
			if( m_bHackMoveBlocked ) return; // Anti Hack xRisenx
			bGORet = m_pMapData->bGetOwner(m_sCommX, m_sCommY, pDstName, &sDstOwnerType, &iDstOwnerStatus, &m_wCommObjectID);

			if ((m_sPlayerX == m_sCommX) && (m_sPlayerY == m_sCommY))
				m_cCommand = OBJECTSTOP;
			else if ( (abs(m_sPlayerX - m_sCommX) <= 1) && (abs(m_sPlayerY - m_sCommY) <= 1) &&
				      (bGORet == true) && (sDstOwnerType != 0) )
				m_cCommand = OBJECTSTOP;
			else if((abs(m_sPlayerX - m_sCommX) <= 2) && (abs(m_sPlayerY - m_sCommY) <= 2) &&
				(m_pMapData->m_tile[m_sCommX][m_sCommY].m_bIsMoveAllowed == false))
				m_cCommand = OBJECTSTOP;
			else
			{	if( m_cCommand == OBJECTMOVE )
				{	if(m_bRunningMode || m_bShiftPressed) m_cCommand = OBJECTRUN;
				}
				if( m_cCommand == OBJECTRUN )
				{	if( (m_bRunningMode == false) && (m_bShiftPressed == false) ) m_cCommand = OBJECTMOVE;
					if( m_iSP < 1 ) m_cCommand = OBJECTMOVE;
				}

				cDir = cGetNextMoveDir(m_sPlayerX, m_sPlayerY, m_sCommX, m_sCommY, true, m_bIllusionMVT);

				if (cDir != 0)
				{
					if(m_cLogOutCount > 0 && !m_bForceDisconn)
					{
						m_cLogOutCount = -1;
						AddEventList(COMMAND_PROCESSOR7, 10);
					}
					m_cPlayerDir = cDir;
					bSendCommand(m_cCommand | MSGIDTYPE_MOTION, 0, cDir, 0, 0, 0, 0);
					
					switch (cDir) 
					{
					case 1:	m_sPlayerY--; break;
					case 2:	m_sPlayerY--; m_sPlayerX++;	break;
					case 3:	m_sPlayerX++; break;
					case 4:	m_sPlayerX++; m_sPlayerY++;	break;
					case 5:	m_sPlayerY++; break;
					case 6:	m_sPlayerX--; m_sPlayerY++;	break;
					case 7:	m_sPlayerX--; break;
					case 8:	m_sPlayerX--; m_sPlayerY--;	break;
					}

#ifdef _DEBUG
					static int timez = unixtime();
					//wsprintfA(G_cTxt, "Move: %i", unixtime() - timez);
					//AddEventList(G_cTxt);
					timez = unixtime();
#endif
					m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
						                  m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
										  m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue, // Re-Coding Sprite xRisenx
										  m_iPlayerStatus, m_cPlayerName,
										  m_cCommand, 0, 0, 0);
					m_bCommandAvailable = false;
					m_dwCommandTime = unixtime();
					m_iPrevMoveX = m_sPlayerX;
					m_iPrevMoveY = m_sPlayerY;
			}	}

			if (m_cCommand == OBJECTDAMAGEMOVE)
			{	m_bIsGetPointingMode = false;
				m_iPointCommandType	 = -1;
				m_stMCursor.sCursorFrame = 0;
				ClearSkillUsingStatus();
				m_cCommand = OBJECTSTOP;
			}
			break;

		case OBJECTATTACK:
			cDir = m_Misc.cGetNextMoveDir(m_sPlayerX, m_sPlayerY, m_sCommX, m_sCommY);
			// Snoopy: Illusion movement
			if (m_bIllusionMVT == true)
			{	cDir +=4;
				if (cDir >8) cDir -=8;
			}
			if (cDir != 0)
			{	if ((wType == 2) || (wType == 25))
				{	if (_bCheckItemByType(ITEMTYPE_ARROW) == false)
						wType = 0;
				}
				if (wType >= 20)
				{	m_iSuperAttackLeft--;
					if (m_iSuperAttackLeft < 0) m_iSuperAttackLeft = 0;
				}
				m_cPlayerDir = cDir;
				bSendCommand(MSGID_MOTION_ATTACK, 0, cDir, m_sCommX, m_sCommY, wType, 0, m_wCommObjectID);
				m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
					                  m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
									  m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue, // Re-Coding Sprite xRisenx
									  m_iPlayerStatus, m_cPlayerName,
									  OBJECTATTACK,
									  m_sCommX - m_sPlayerX, m_sCommY - m_sPlayerY, wType);
				m_bCommandAvailable = false;
				m_dwCommandTime = unixtime();
			}
			m_cCommand = OBJECTSTOP;
			break;

		case OBJECTATTACKMOVE:
			if( m_bParalyze ) return;
			if( m_bHackMoveBlocked ) return; // Anti Hack xRisenx
			bGORet = m_pMapData->bGetOwner(m_sCommX, m_sCommY, pDstName, &sDstOwnerType, &iDstOwnerStatus, &m_wCommObjectID);
			if ((m_sPlayerX == m_sCommX) && (m_sPlayerY == m_sCommY))
				m_cCommand = OBJECTSTOP;
			else if ( (abs(m_sPlayerX - m_sCommX) <= 1) && (abs(m_sPlayerY - m_sCommY) <= 1) &&
				      (bGORet == true) && (sDstOwnerType != 0) )
				m_cCommand = OBJECTSTOP;
			else
			{
				cDir = cGetNextMoveDir(m_sPlayerX, m_sPlayerY, m_sCommX, m_sCommY, true, m_bIllusionMVT);

				if (cDir != 0)
				{	m_cPlayerDir = cDir;
					bSendCommand(MSGID_MOTION_ATTACKMOVE, 0, cDir, m_sCommX, m_sCommY, wType, 0, m_wCommObjectID);
					switch (cDir) {
					case 1:	m_sPlayerY--; break;
					case 2:	m_sPlayerY--; m_sPlayerX++;	break;
					case 3:	m_sPlayerX++; break;
					case 4:	m_sPlayerX++; m_sPlayerY++;	break;
					case 5:	m_sPlayerY++; break;
					case 6:	m_sPlayerX--; m_sPlayerY++;	break;
					case 7:	m_sPlayerX--; break;
					case 8:	m_sPlayerX--; m_sPlayerY--;	break;
					}

					m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
						                  m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
										  m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue, // Re-Coding Sprite xRisenx
										  m_iPlayerStatus, m_cPlayerName,
										  m_cCommand, m_sCommX - m_sPlayerX, m_sCommY - m_sPlayerY, wType);
					m_bCommandAvailable = false;
					m_dwCommandTime = unixtime();
					m_iPrevMoveX = m_sPlayerX;
					m_iPrevMoveY = m_sPlayerY;
			}	}
			m_cCommand = OBJECTSTOP;
			break;

		case OBJECTGETITEM:
			bSendCommand(MSGID_MOTION_GETITEM, 0, m_cPlayerDir, 0, 0, 0, 0);
			m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
				                  m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
								  m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue, // Re-Coding Sprite xRisenx
								  m_iPlayerStatus, m_cPlayerName,
								  OBJECTGETITEM, 0, 0, 0);
			m_bCommandAvailable = false;
			m_cCommand = OBJECTSTOP;
			break;

		case OBJECTMAGIC:
			bSendCommand(MSGID_MOTION_MAGIC, 0, m_cPlayerDir, m_iCastingMagicType, 0, 0, 0);
			m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
				                  m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
								  m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue, // Re-Coding Sprite xRisenx
								  m_iPlayerStatus, m_cPlayerName,
								  OBJECTMAGIC, m_iCastingMagicType, 0, 0);
			m_bCommandAvailable = false;
			m_dwCommandTime = unixtime();
			m_bIsGetPointingMode = true;
			m_cCommand = OBJECTSTOP;
			_RemoveChatMsgListByObjectID(m_sPlayerObjectID);
			for (i = 1; i < MAXCHATMSGS; i++)
			if (m_pChatMsgList[i] == 0)
			{	ZeroMemory(cTxt, sizeof(cTxt));
				wsprintfA(cTxt, "%s!", m_pMagicCfgList[m_iCastingMagicType]->m_cName);
				m_pChatMsgList[i] = new class CMsg(41, cTxt, unixtime());
				m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;
				m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i);
				return;
			}
			break;

		default:
			break;
		}	
	}
	// Gladiator Arena xRisenx
	/*if( _iGetFOE(iFocuiStatus) < 0 )
{
   m_stMCursor.sCursorFrame = 3;
}
else if(strcpy(m_cMapName, ARENAMAP) == 0){
   m_stMCursor.sCursorFrame = 3;
}*/
// Gladiator Arena xRisenx
}

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

void CGame::bItemDrop_Character()
{	
	ItemEquipHandler((char)m_stMCursor.sSelectedObjectID);
}

void CGame::bItemDrop_Inventory()
{ 
	short sX, sY, dX, dY;
 char  cTxt[120];
	if (m_cCommand < 0) return;
	if (m_pItemList[m_stMCursor.sSelectedObjectID] == 0) return;
	if ((m_bSkillUsingStatus == true) && (m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] == true))
	{	AddEventList(BITEMDROP_INVENTORY1, 10);
		return;
	}
	if (m_bIsItemDisabled[m_stMCursor.sSelectedObjectID] == true) return;
	sY = m_dialogBoxes[2].m_Y;
	sX = m_dialogBoxes[2].m_X;
	dX = G_pGame->m_stMCursor.sX - sX - 32 - m_stMCursor.sDistX;
	dY = G_pGame->m_stMCursor.sY - sY - 44 - m_stMCursor.sDistY;
	if (dY < -10) dY = -10;
	if (dX < 0)   dX = 0;
	if (dX > 170) dX = 170;
	if (dY > 95) dY = 95;

	if(m_droppedOnIconbar) //Puts item back where it was rather than random
	{
		m_droppedOnIconbar = false; 
	}
	else
	{
	m_pItemList[m_stMCursor.sSelectedObjectID]->m_sX = dX;
	m_pItemList[m_stMCursor.sSelectedObjectID]->m_sY = dY;
	}

	short sTmpSpr, sTmpSprFrm;
	sTmpSpr = m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite;
	sTmpSprFrm = m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame;

	char cItemID;
	if (m_bShiftPressed)
	{	for (int i = 0; i < MAXITEMS; i++)
		{	if (m_cItemOrder[MAXITEMS - 1 - i] != -1)
			{	cItemID = m_cItemOrder[MAXITEMS - 1 - i];
				if (m_pItemList[cItemID] != 0 && memcmp(m_pItemList[cItemID]->m_cName, m_pItemList[m_stMCursor.sSelectedObjectID]->m_cName, 20) == 0 )
				{	m_pItemList[cItemID]->m_sX = dX;
					m_pItemList[cItemID]->m_sY = dY;
					bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, cItemID, dX, dY, 0, 0);
		}	}	}
    }else bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, (char)(m_stMCursor.sSelectedObjectID), dX, dY, 0, 0);

	if (m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] == true)
	{	char cStr1[64], cStr2[64], cStr3[64];
		GetItemName(m_pItemList[m_stMCursor.sSelectedObjectID], cStr1, cStr2, cStr3);
		wsprintfA(cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
		AddEventList(cTxt, 10);

		if(memcmp(m_pItemList[m_stMCursor.sSelectedObjectID]->m_cName, "AngelicPendant", 14) == 0) PlaySound('E', 53, 0);
		else PlaySound('E', 29, 0);

		// Remove Angelic Stats
		if (   (m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos >= 11)
			&& (m_pItemList[m_stMCursor.sSelectedObjectID]->m_cItemType == 1))
		{	char cItemID = m_stMCursor.sSelectedObjectID;
			if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(STR)", 19) == 0)
			{	m_angelStat[STAT_STR] = 0;
			}else if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(DEX)", 19) == 0)
			{	m_angelStat[STAT_DEX] = 0;
			}else if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(INT)", 19) == 0)
			{	m_angelStat[STAT_INT] = 0;
			}else if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(MAG)", 19) == 0)
			{	m_angelStat[STAT_MAG] = 0;
		}	}
		bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_RELEASEITEM, 0, m_stMCursor.sSelectedObjectID, 0, 0, 0);
		m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] = false;
		m_sItemEquipmentStatus[	m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos ] = -1;
	}
}

void CGame::bItemDrop_SellList()
{
 int i;
 char cItemID;

	cItemID = (char)m_stMCursor.sSelectedObjectID;

	if (m_pItemList[cItemID] == 0) return;
	if (m_bIsItemDisabled[cItemID] == true) return;
	if (m_cCommand < 0) return;
		for (i = 0; i < MAXSELLLIST; i++)
	if (m_stSellItemList[i].iIndex == cItemID)
	{	AddEventList(BITEMDROP_SELLLIST1, 10);
		return;
	}
	if (strcmp(m_pItemList[cItemID]->m_cName, "Gold") == 0) {
		AddEventList(BITEMDROP_SELLLIST2, 10);
		return;
	}
	if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
	{
		ZeroMemory( G_cTxt, sizeof(G_cTxt) );
		char cStr1[64], cStr2[64], cStr3[64];
		GetItemName(m_pItemList[cItemID], cStr1, cStr2, cStr3);
		wsprintfA( G_cTxt, NOTIFYMSG_CANNOT_SELL_ITEM2, cStr1 );
		AddEventList( G_cTxt, 10 );
		return;
	}

	if ( ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_ARROW)) &&
		 (m_pItemList[cItemID]->m_dwCount > 1) )
	{	m_dialogBoxes[17].m_X  = G_pGame->m_stMCursor.sX - 140;
		m_dialogBoxes[17].m_Y  = G_pGame->m_stMCursor.sY - 70;
		if (m_dialogBoxes[17].m_Y < 0) m_dialogBoxes[17].m_Y = 0;
		m_dialogBoxes[17].sV1 = m_sPlayerX+1;
		m_dialogBoxes[17].sV2 = m_sPlayerY+1;
		m_dialogBoxes[17].sV3 = 1001;
		m_dialogBoxes[17].sV4 = cItemID;
		ZeroMemory(m_dialogBoxes[17].cStr, sizeof(m_dialogBoxes[17].cStr));
		EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
		m_bIsItemDisabled[cItemID] = true;
	}else
	{	for (i = 0; i < MAXSELLLIST; i++)
		if (m_stSellItemList[i].iIndex == -1)
		{	m_stSellItemList[i].iIndex = cItemID;
			m_stSellItemList[i].iAmount = 1;
			m_bIsItemDisabled[cItemID] = true;
			return;
		}
		AddEventList(BITEMDROP_SELLLIST3, 10);
	}
}

void CGame::bItemDrop_ItemUpgrade()
{
 char cItemID;
	cItemID = (char)m_stMCursor.sSelectedObjectID;
	if (m_bIsItemDisabled[cItemID] == true) return;
	if (m_cCommand < 0) return;
	if (m_pItemList[cItemID]->m_cEquipPos == EQUIPPOS_NONE) return;

	switch (m_dialogBoxes[34].GetMode()) {
	case 1:
		m_bIsItemDisabled[m_dialogBoxes[34].sV1] = false;
		m_dialogBoxes[34].sV1 = cItemID;
		m_bIsItemDisabled[cItemID] = true;
		PlaySound('E', 29, 0);
		break;

	case 6:
		m_bIsItemDisabled[m_dialogBoxes[34].sV1] = false;
		m_dialogBoxes[34].sV1 = cItemID;
		m_bIsItemDisabled[cItemID] = true;
		PlaySound('E', 29, 0);
		break;

	case 13: // Upgrade Hero System xRisenx
		m_bIsItemDisabled[m_dialogBoxes[34].sV1] = false;
		m_dialogBoxes[34].sV1 = cItemID;
		m_bIsItemDisabled[cItemID] = true;
		PlaySound('E', 29, 0);
		break;
	}
}

void CGame::bItemDrop_Bank()
{	
	m_dialogBoxes[39].sV1 = m_stMCursor.sSelectedObjectID;
	if (m_cCommand < 0) return;
	if (m_pItemList[m_dialogBoxes[39].sV1] == 0) return;
	if (m_bIsItemDisabled[m_dialogBoxes[39].sV1] == true) return;
	if ( m_bIsDialogEnabled[17] == true )
	{	AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}
	if ( (m_bIsDialogEnabled[20] == true) && ((m_dialogBoxes[20].GetMode() == 1) || (m_dialogBoxes[20].GetMode() == 2)) )
	{	AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}
	if (m_bIsDialogEnabled[23] == true)
	{	AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}
	if (m_bIsDialogEnabled[4] == true)
	{	AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}
	if ( ((m_pItemList[m_dialogBoxes[39].sV1]->m_cItemType == ITEMTYPE_CONSUME) || (m_pItemList[m_dialogBoxes[39].sV1]->m_cItemType == ITEMTYPE_ARROW)) && (m_pItemList[m_dialogBoxes[39].sV1]->m_dwCount > 1) )
	{	m_dialogBoxes[17].m_X  = G_pGame->m_stMCursor.sX - 140;
		m_dialogBoxes[17].m_Y  = G_pGame->m_stMCursor.sY - 70;
		if (m_dialogBoxes[17].m_Y < 0) m_dialogBoxes[17].m_Y = 0;

		m_dialogBoxes[17].sV1 = m_sPlayerX+1;
		m_dialogBoxes[17].sV2 = m_sPlayerY+1;
		m_dialogBoxes[17].sV3 = 1002;// NPC
		m_dialogBoxes[17].sV4 = m_dialogBoxes[39].sV1;

		ZeroMemory(m_dialogBoxes[17].cStr, sizeof(m_dialogBoxes[17].cStr));
		EnableDialogBox(17, m_dialogBoxes[39].sV1, m_pItemList[m_dialogBoxes[39].sV1]->m_dwCount, 0);
	}else
	{	if (_iGetBankItemCount() >= MAXBANKITEMS) AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);
		else bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, m_dialogBoxes[39].sV1, 1, m_dialogBoxes[39].sV5, m_dialogBoxes[39].sV6, m_pItemList[m_dialogBoxes[39].sV1]->m_cName, m_dialogBoxes[39].sV4); //v1.4
	}
}

void CGame::bItemDrop_SkillDialog()
{
 int iConsumeNum;
 char cItemID;

	if (m_cCommand < 0) return;
	cItemID = (char)m_stMCursor.sSelectedObjectID;
	if (m_pItemList[cItemID] == 0) return;
	if (m_bIsItemDisabled[cItemID] == true) return;

	if ( m_bIsDialogEnabled[17] == true )
	{	AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}

	if (   (m_bIsDialogEnabled[20] == true)
		&& ((m_dialogBoxes[20].GetMode() == 1) || (m_dialogBoxes[20].GetMode() == 2)) ) 
	{	AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}

	if (m_bIsDialogEnabled[23] == true)
	{	AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}

	switch (m_dialogBoxes[26].GetMode()) {
	case 1:
		if (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME)
		{	iConsumeNum = 0;
			if (m_dialogBoxes[26].sV1 == cItemID) iConsumeNum++;
			if (m_dialogBoxes[26].sV2 == cItemID) iConsumeNum++;
			if (m_dialogBoxes[26].sV3 == cItemID) iConsumeNum++;
			if (m_dialogBoxes[26].sV4 == cItemID) iConsumeNum++;
			if (m_dialogBoxes[26].sV5 == cItemID) iConsumeNum++;
			if (m_dialogBoxes[26].sV6 == cItemID) iConsumeNum++;
			if (iConsumeNum >= (int)(m_pItemList[cItemID]->m_dwCount)) return;
		}
		if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_EAT) ||
			(m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) ||
			(m_pItemList[cItemID]->m_cItemType == ITEMTYPE_NONE))
		{}else return;

		if (m_dialogBoxes[26].sV1 == -1) 
		{	m_dialogBoxes[26].sV1 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV2 == -1) 
		{	m_dialogBoxes[26].sV2 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV3 == -1) 
		{	m_dialogBoxes[26].sV3 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV4 == -1) 
		{	m_dialogBoxes[26].sV4 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV5 == -1) 
		{	m_dialogBoxes[26].sV5 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV6 == -1)
		{	m_dialogBoxes[26].sV6 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}
		AddEventList(BITEMDROP_SKILLDIALOG4, 10);
		break;

	case 4:
		if (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME)
		{	iConsumeNum = 0;
			if (m_dialogBoxes[26].sV1 == cItemID) iConsumeNum++;
			if (m_dialogBoxes[26].sV2 == cItemID) iConsumeNum++;
			if (m_dialogBoxes[26].sV3 == cItemID) iConsumeNum++;
			if (m_dialogBoxes[26].sV4 == cItemID) iConsumeNum++;
			if (m_dialogBoxes[26].sV5 == cItemID) iConsumeNum++;
			if (m_dialogBoxes[26].sV6 == cItemID) iConsumeNum++;
			if (iConsumeNum >= (int)(m_pItemList[cItemID]->m_dwCount)) return;
		}

		if (m_dialogBoxes[26].sV1 == -1) 
		{	m_dialogBoxes[26].sV1 = cItemID;
			m_dialogBoxes[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV2 == -1) 
		{	m_dialogBoxes[26].sV2 = cItemID;
			m_dialogBoxes[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV3 == -1) 
		{	m_dialogBoxes[26].sV3 = cItemID;
			m_dialogBoxes[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV4 == -1)
		{	m_dialogBoxes[26].sV4 = cItemID;
			m_dialogBoxes[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV5 == -1) 
		{
			m_dialogBoxes[26].sV5 = cItemID;
			m_dialogBoxes[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV6 == -1)
		{	m_dialogBoxes[26].sV6 = cItemID;
			m_dialogBoxes[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}
		AddEventList(BITEMDROP_SKILLDIALOG4, 10); // "There is no more space for ingredients."
		break;

	// Crafting
	case 7:
		if (   (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_NONE)			// Merien Stone
			|| (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_EQUIP)		// Necks XXXMagins
			|| (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME)		// stones
			|| (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_MATERIAL)	// XXXwares
			|| m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_DEPLETE_DEST)	// gems
		{}else return;

		if (m_dialogBoxes[26].sV1 == -1) 
		{	m_dialogBoxes[26].sV1 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV2 == -1) 
		{	m_dialogBoxes[26].sV2 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV3 == -1) 
		{	m_dialogBoxes[26].sV3 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV4 == -1) 
		{	m_dialogBoxes[26].sV4 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV5 == -1) 
		{	m_dialogBoxes[26].sV5 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}else if (m_dialogBoxes[26].sV6 == -1)
		{	m_dialogBoxes[26].sV6 = cItemID;
			if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
			{}else m_bIsItemDisabled[cItemID] = true;
			return;
		}
		AddEventList(BITEMDROP_SKILLDIALOG4, 10); // "There is no more space for ingredients."
		break;

	default:
		break;
	}
}

// Slates Item Drag&Drop - Diuuude
void CGame::bItemDrop_Slates()
{
 char cItemID;
	if (m_cCommand < 0) return;
	cItemID = (char)m_stMCursor.sSelectedObjectID;
	if (m_pItemList[cItemID] == 0) return;
	if (m_bIsItemDisabled[cItemID] == true) return;
	if ( m_bIsDialogEnabled[17] == true ) {
		AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}

	if ( (m_bIsDialogEnabled[20] == true) &&
		 ((m_dialogBoxes[20].GetMode() == 1) || (m_dialogBoxes[20].GetMode() == 2)) ) {
		AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}

	if (m_bIsDialogEnabled[23] == true) {
		AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}

	switch (m_dialogBoxes[40].GetMode()) {
	case 1:
		if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX) && (m_pItemList[cItemID]->m_sSpriteFrame >= 151) && (m_pItemList[cItemID]->m_sSpriteFrame <= 154)) {
			char cItemIDText[20];
			switch(m_pItemList[cItemID]->m_sSpriteFrame){
				case 151:
					if (m_dialogBoxes[40].sV1 == -1){
						m_bIsItemDisabled[cItemID] = true;
						m_dialogBoxes[40].sV1 = cItemID;
						wsprintfA(cItemIDText, "Item ID : %d", cItemID);
						AddEventList(cItemIDText, 10);
					}
					break;
				case 152:
					if (m_dialogBoxes[40].sV2 == -1){
						m_bIsItemDisabled[cItemID] = true;
						m_dialogBoxes[40].sV2 = cItemID;
						wsprintfA(cItemIDText, "Item ID : %d", cItemID);
						AddEventList(cItemIDText, 10);
					}
					break;
				case 153:
					if (m_dialogBoxes[40].sV3 == -1){
						m_bIsItemDisabled[cItemID] = true;
						m_dialogBoxes[40].sV3 = cItemID;
						wsprintfA(cItemIDText, "Item ID : %d", cItemID);
						AddEventList(cItemIDText, 10);
					}
					break;
				case 154:
					if (m_dialogBoxes[40].sV4 == -1){
						m_bIsItemDisabled[cItemID] = true;
						m_dialogBoxes[40].sV4 = cItemID;
						wsprintfA(cItemIDText, "Item ID : %d", cItemID);
						AddEventList(cItemIDText, 10);
					}
					break;
			}
		}
		break;

	default:
		break;
	}
}

void CGame::ReceiveModifyTile(char * pData)
{
	//StreamRead sr(pData,);


}

void CGame::ResponseTeleportList(char *pData)
{	
	char *cp;
	int  *ip;

	cp = pData + 6;
	ip = (int*) cp;
	m_iTeleportMapCount = *ip;
	cp += 4;

	for(int i = 0 ; i < m_iTeleportMapCount ; i++)
	{	
		ip = (int*)cp;
		m_stTeleportList[i].iIndex = *ip;
		cp += 4;
		ZeroMemory(m_stTeleportList[i].mapname, sizeof(m_stTeleportList[i].mapname) );
		memcpy(m_stTeleportList[i].mapname, cp, 10);
		cp += 10;
		ip = (int*)cp;
		m_stTeleportList[i].iX = *ip;
		cp += 4;
		ip = (int*)cp;
		m_stTeleportList[i].iY = *ip;
		cp += 4;
		ip = (int*)cp;
		m_stTeleportList[i].iCost = *ip;
		cp += 4;
		m_stTeleportList[i].donateEvent = *cp;
		cp ++;
	}
}

void CGame::ResponseChargedTeleport(char *pData)
{	
	short *sp;
	char *cp;
	short sRejectReason = 0;
	cp = (char*)pData + INDEX2_MSGTYPE + 2;
	sp = (short*)cp;
	sRejectReason = *sp;

	switch( sRejectReason )	{
	case 1:
		AddEventList( RESPONSE_CHARGED_TELEPORT1, 10 );
		break;
	case 2:
		AddEventList( RESPONSE_CHARGED_TELEPORT2, 10 );
		break;
	case 3:
		AddEventList( RESPONSE_CHARGED_TELEPORT3, 10 );
		break;
	case 4:
		AddEventList( RESPONSE_CHARGED_TELEPORT4, 10 );
		break;
	case 5:
		AddEventList( RESPONSE_CHARGED_TELEPORT5, 10 );
		break;
	case 6:
		AddEventList( RESPONSE_CHARGED_TELEPORT6, 10 );
		break;
	case 7:
		AddEventList( RESPONSE_CHARGED_TELEPORT7, 10 );
		break;
	}
}

void CGame::NotifyMsg_CannotGiveItem(char *pData)
{
 char * cp, cName[21], cTxt[256];
 uint16_t * wp, wItemIndex;
 int  * ip, iAmount;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	wItemIndex = *wp;
	cp += 2;

	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[wItemIndex], cStr1, cStr2, cStr3);
	if( iAmount == 1 ) wsprintfA(cTxt, NOTIFYMSG_CANNOT_GIVE_ITEM2, cStr1, cName);
	else wsprintfA( cTxt, NOTIFYMSG_CANNOT_GIVE_ITEM1, iAmount, cStr1, cName);


	AddEventList(cTxt, 10);
}

void CGame::NotifyMsg_DropItemFin_CountChanged(char *pData)
{
 char * cp, cTxt[256];
 uint16_t * wp, wItemIndex;
 int  * ip, iAmount;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	wItemIndex = *wp;
	cp += 2;

	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[wItemIndex]->m_cName, m_pItemList[wItemIndex]->m_dwAttribute, cStr1, cStr2, cStr3);
	wsprintfA(cTxt, NOTIFYMSG_THROW_ITEM1, iAmount, cStr1);

	AddEventList(cTxt, 10);
}

void CGame::GuildContributeRsp(char * data)
{
// 	char cTxt[256];
// 
// 	uint8_t type;
// 	Pop(data, type);
// 	uint32_t amount;
// 	Pop(data, amount);
// 
// 	switch(type)
// 	{
// 	case 0: // gold
// 		wsprintfA(cTxt, DRAW_DIALOGBOX_GUILDCONTRIBUTE5, amount);
// 		SetGold( GetGold() - amount );
// 		break;
// 	case 1: // maj
// 		wsprintfA(cTxt, DRAW_DIALOGBOX_GUILDCONTRIBUTE6, amount);
// 		m_iGizonItemUpgradeLeft -= amount;
// 		break;
// 	case 2: // cont
// 		wsprintfA(cTxt, DRAW_DIALOGBOX_GUILDCONTRIBUTE9, amount);
// 		m_iContribution -= amount;
// 		break;
// 	}
// 
// 	AddEventList(cTxt, 10);
}

void CGame::NotifyMsg_CannotJoinMoreGuildsMan(char * pData)
{
 char * cp, cName[12], cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);

	wsprintfA(cTxt, NOTIFYMSG_CANNOT_JOIN_MOREGUILDMAN1, cName);
	AddEventList(cTxt, 10);
	AddEventList(NOTIFYMSG_CANNOT_JOIN_MOREGUILDMAN2, 10);
}



void CGame::NotifyMsg_DismissGuildsMan(char * pData)
{
 char * cp, cName[12], cTxt[120];
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);

	if( memcmp( m_cPlayerName, cName, 10 ) != 0 ) {
		wsprintfA(cTxt, NOTIFYMSG_DISMISS_GUILDMAN1, cName);
		AddEventList(cTxt, 10);
	}
	ClearGuildNameList();
}

void CGame::NotifyMsg_CannotRating(char * pData)
{
 char * cp, cTxt[120];
 uint16_t * wp, wTime;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	wTime = *wp;
	cp += 2;

	if (wTime == 0) wsprintfA(cTxt, NOTIFYMSG_CANNOT_RATING1, wTime*3);
	else wsprintfA(cTxt, NOTIFYMSG_CANNOT_RATING2, wTime*3);
	AddEventList(cTxt, 10);
}

void CGame::NotifyMsg_CannotRepairItem(char * pData)
{
 char * cp, cTxt[120], cStr1[64], cStr2[64], cStr3[64];
 uint16_t * wp, wV1, wV2;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
  	wV1 = *wp;
	cp += 2;
	wp = (uint16_t *)cp;
  	wV2 = *wp;
	cp += 2;
	ZeroMemory( cStr1, sizeof(cStr1) );
	ZeroMemory( cStr2, sizeof(cStr2) );
	ZeroMemory( cStr3, sizeof(cStr3) );
	GetItemName( m_pItemList[wV1], cStr1, cStr2, cStr3 );

	switch (wV2) {
	case 1:
		wsprintfA(cTxt, NOTIFYMSG_CANNOT_REPAIR_ITEM1, cStr1 );
		AddEventList(cTxt, 10);
 		break;
	case 2:
		wsprintfA(cTxt, NOTIFYMSG_CANNOT_REPAIR_ITEM2, cStr1 );
		AddEventList(cTxt, 10);
 		break;
	}
	m_bIsItemDisabled[wV1] = false;
}

void CGame::NotifyMsg_CannotSellItem(char * pData)
{
 char * cp, cTxt[120], cStr1[64], cStr2[64], cStr3[64];
 uint16_t * wp, wV1, wV2;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	wp = (uint16_t *)cp;
  	wV1 = *wp;
	cp += 2;

	wp = (uint16_t *)cp;
  	wV2 = *wp;
	cp += 2;

	ZeroMemory( cStr1, sizeof(cStr1) );
	ZeroMemory( cStr2, sizeof(cStr2) );
	ZeroMemory( cStr3, sizeof(cStr3) );
	GetItemName( m_pItemList[wV1], cStr1, cStr2, cStr3 );

	switch (wV2) {
	case 1:
		wsprintfA(cTxt, NOTIFYMSG_CANNOT_SELL_ITEM1, cStr1);//"
		AddEventList(cTxt, 10);
		break;

	case 2:
		wsprintfA(cTxt, NOTIFYMSG_CANNOT_SELL_ITEM2, cStr1);//"
		AddEventList(cTxt, 10);
		break;

	case 3:
		wsprintfA(cTxt, NOTIFYMSG_CANNOT_SELL_ITEM3, cStr1);//"
		AddEventList(cTxt, 10);
		AddEventList(NOTIFYMSG_CANNOT_SELL_ITEM4, 10);//"
		break;

	case 4:
		AddEventList(NOTIFYMSG_CANNOT_SELL_ITEM5, 10); // "
		AddEventList(NOTIFYMSG_CANNOT_SELL_ITEM6, 10); // "
		break;
	}
	m_bIsItemDisabled[wV1] = false;
}

void CGame::NotifyMsg_Charisma(char * pData)
{
 uint32_t * dwp;
 int  iPrevChar;
 char cTxt[120];

	iPrevChar = m_stat[STAT_CHR];
	dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
	m_stat[STAT_CHR] = (int)*dwp;

	if (m_stat[STAT_CHR] > iPrevChar)
	{	wsprintfA(cTxt, NOTIFYMSG_CHARISMA_UP, m_stat[STAT_CHR] - iPrevChar);//"
		AddEventList(cTxt, 10);
		PlaySound('E', 21, 0);
	}else
	{	wsprintfA(cTxt, NOTIFYMSG_CHARISMA_DOWN, iPrevChar - m_stat[STAT_CHR]);//"
		AddEventList(cTxt, 10);
	}
}

void CGame::NotifyMsg_DropItemFin_EraseItem(char *pData)
{
 char * cp;
 uint16_t * wp;
 int * ip, iAmount;
 short  sItemIndex;
 char   cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	wp = (uint16_t *)cp;
	sItemIndex = *wp;
	cp += 2;

	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[sItemIndex], cStr1, cStr2, cStr3);

	ZeroMemory(cTxt, sizeof(cTxt));
	if (m_bIsItemEquipped[sItemIndex] == true)
	{	wsprintfA(cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
		AddEventList(cTxt, 10);
		m_sItemEquipmentStatus[	m_pItemList[sItemIndex]->m_cEquipPos ] = -1;
		m_bIsItemEquipped[sItemIndex] = false;
	}
	if (m_iHP > 0)
	{	wsprintfA(cTxt, NOTIFYMSG_THROW_ITEM2, cStr1);
	}else
	{	if (iAmount < 2)
			wsprintfA(cTxt, NOTIFYMSG_DROPITEMFIN_ERASEITEM3, cStr1); // "You dropped a %s."
		else // Snoopy fix
		{	wsprintfA(cTxt, NOTIFYMSG_DROPITEMFIN_ERASEITEM5, cStr1); // "You dropped %s."
	}	}
	AddEventList(cTxt, 10);
	EraseItem((char)sItemIndex);
	_bCheckBuildItemStatus();
}


void CGame::NotifyMsg_EnemyKillReward(char *pData)
{
 uint32_t * dwp;
 short * sp, sGuildRank;
 char  * cp, cName[12], cGuildName[24], cTxt[120];
 int   iExp, iEnemyKillCount, iWarContribution;

	ZeroMemory(cName, sizeof(cName));
	ZeroMemory(cGuildName, sizeof(cGuildName));

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	dwp  = (uint32_t *)cp;
	iExp = *dwp;
	cp += 4;
	dwp  = (uint32_t *)cp;
	iEnemyKillCount = *dwp;
	cp += 4;
	memcpy(cName, cp, 10);
	cp += 10;
	memcpy(cGuildName, cp, 20);
	cp += 20;
	sp  = (short *)cp;
	sGuildRank = *sp;
	cp += 2;
	sp  = (short *)cp;
	iWarContribution = *sp;
	cp += 2;

	if (iWarContribution > m_iWarContribution+1)
	{	wsprintfA(G_cTxt, "%s +%d!", CRUSADE_MESSAGE21, iWarContribution - m_iWarContribution);
		SetTopMsg(G_cTxt, 5);
	}else if (iWarContribution < m_iWarContribution)
	{}
	m_iWarContribution = iWarContribution;

	if(sGuildRank == GUILDRANK_NONE)
	{
		wsprintfA(cTxt, NOTIFYMSG_ENEMYKILL_REWARD1, cName);
		AddEventList(cTxt, 10);
	}else{
		wsprintfA(cTxt, NOTIFYMSG_ENEMYKILL_REWARD2, GetGuildRankName(sGuildRank), cName, cGuildName);
		AddEventList(cTxt, 10);
	}

/*	if( m_iExp != iExp ) // removed by snoopy because too much msg hide victim's name
	{	if (m_iExp > iExp) wsprintfA(cTxt, EXP_DECREASED,m_iExp - iExp);
		else wsprintfA(cTxt, EXP_INCREASED,iExp - m_iExp);
		AddEventList(cTxt, 10);
	}*/

	int32_t ekchange = iEnemyKillCount - m_iEnemyKillCount;

	if (m_iEnemyKillCount != iEnemyKillCount)
	{	
		if (m_iEnemyKillCount > iEnemyKillCount)
		{	
			wsprintfA(cTxt, NOTIFYMSG_ENEMYKILL_REWARD5, -ekchange);
			AddEventList(cTxt, 10);
		}
		else
		{	
			wsprintfA(cTxt, NOTIFYMSG_ENEMYKILL_REWARD6, ekchange);
			AddEventList(cTxt, 10);
		}
	}

	if( iExp >= 0 ) m_iExp = iExp;
	if( iEnemyKillCount >= 0 )
	{
		m_iEnemyKillCount += ekchange;
		m_iEnemyKillTotalCount += ekchange;
	}
	PlaySound('E', 23, 0);

	if(m_ekScreenshot)
		m_ekSSTime = unixtime() + 650;
}

void CGame::NotifyMsg_EventFishMode(char * pData)
{
	short sSprite, sSpriteFrame;
	char * cp, cName[21];
	uint16_t * wp, wPrice;
	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);

	wp = (uint16_t *)cp;
	wPrice = *wp;
	cp += 2;

	wp = (uint16_t *)cp;
	sSprite = (short)*wp;
	cp += 2;

	wp = (uint16_t *)cp;
	sSpriteFrame = (short)*wp;
	cp += 2;

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	EnableDialogBox(24, 0, 0, wPrice, cName);
	m_dialogBoxes[24].sV3 = sSprite;
	m_dialogBoxes[24].sV4 = sSpriteFrame;

	AddEventList(NOTIFYMSG_EVENTFISHMODE1, 10);
}

void CGame::NotifyMsg_Exp(char * pData)
{
 uint32_t * dwp;
 int iPrevExp, * ip;
 char * cp, cTxt[120];

	iPrevExp = m_iExp;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	dwp = (uint32_t *)cp;
	m_iExp = (int)*dwp;
	cp += 4;

	ip = (int *)cp;
//	m_iRating = *ip;
	cp += 4;

	if (m_iExp > iPrevExp)
	{	wsprintfA(cTxt, EXP_INCREASED, m_iExp - iPrevExp);
		AddEventList(cTxt, 10);
	}else if (m_iExp < iPrevExp)
	{	wsprintfA(cTxt, EXP_DECREASED, iPrevExp - m_iExp);
		AddEventList(cTxt, 10);
	}
}


void CGame::NotifyMsg_ForceDisconn(char *pData)
{
	uint16_t * wpCount;
	wpCount = (uint16_t *)(pData + 6);
	m_bForceDisconn = true;
	//m_cLogOutCount = (char)*wpCount;
	if( m_bIsProgramActive )
	{	if( m_cLogOutCount < 0 || m_cLogOutCount > 5 ) m_cLogOutCount = 5;
		AddEventList(NOTIFYMSG_FORCE_DISCONN1, 10);
	}
	else
	{
		gamemode = 0;
		_socket->stop();
		m_bEscPressed = false;
        if (m_bSoundFlag) m_pESound[38]->stop();
		if ((m_bSoundFlag) && (m_bMusicStat == true))
		{
			if (m_pBGM != nullptr) m_pBGM->stop();
		}
		if (strlen(G_cCmdLineTokenA) != 0)
		{
			 ChangeGameMode(GAMEMODE_ONQUIT);
		} else {
			isItemLoaded = false;
			ChangeGameMode(GAMEMODE_ONMAINMENU);
		}
	}
}

void CGame::NotifyMsg_GiveItemFin_CountChanged(char *pData)
{
 char * cp, cName[21], cTxt[256];
 uint16_t * wp, wItemIndex;
 int  * ip, iAmount;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	wItemIndex = *wp;
	cp += 2;

	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[wItemIndex]->m_cName, m_pItemList[wItemIndex]->m_dwAttribute, cStr1, cStr2, cStr3);
	if( iAmount == 1 ) wsprintfA(cTxt, NOTIFYMSG_GIVEITEMFIN_COUNTCHANGED1, cStr1, cName);
	wsprintfA(cTxt, NOTIFYMSG_GIVEITEMFIN_COUNTCHANGED2, iAmount, cStr1, cName);
	AddEventList(cTxt, 10);
}

void CGame::NotifyMsg_GiveItemFin_EraseItem(char *pData)
{
 char * cp;
 uint16_t * wp;
 int  * ip, iAmount;
 short  sItemIndex;
 char cName[21], cTxt[250];


	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	wp = (uint16_t *)cp;
	sItemIndex = *wp;
	cp += 2;

	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[sItemIndex]->m_cName, m_pItemList[sItemIndex]->m_dwAttribute, cStr1, cStr2, cStr3);

	if (m_bIsItemEquipped[sItemIndex] == true) {
		wsprintfA(cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
		AddEventList(cTxt, 10);

		m_sItemEquipmentStatus[	m_pItemList[sItemIndex]->m_cEquipPos ] = -1;
		m_bIsItemEquipped[sItemIndex] = false;
	}
	if (strlen(cName) == 0) wsprintfA(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM2, iAmount, cStr1);
	else {
		if (strcmp(cName, "Howard") == 0)
			 wsprintfA(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM3, iAmount, cStr1);
		else if (strcmp(cName, "William") == 0)
			 wsprintfA(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM4, iAmount, cStr1);
		else if (strcmp(cName, "Kennedy") == 0)
			wsprintfA(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM5, iAmount, cStr1);
		else if (strcmp(cName, "Tom") == 0)
			wsprintfA(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM7, iAmount, cStr1);
		else if (strcmp(cName, "Princess") == 0)
			 wsprintfA(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM3, iAmount, cStr1);

		else wsprintfA(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM8, iAmount, cStr1, cName);
	}
	AddEventList(cTxt, 10);
	EraseItem((char)sItemIndex);
	_bCheckBuildItemStatus();
}

void CGame::NotifyMsg_GlobalAttackMode(char *pData)
{
 char * cp;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	switch (*cp) {
	case 0:
		AddEventList(NOTIFYMSG_GLOBAL_ATTACK_MODE1, 10);
		AddEventList(NOTIFYMSG_GLOBAL_ATTACK_MODE2, 10);
		break;

	case 1:
		AddEventList(NOTIFYMSG_GLOBAL_ATTACK_MODE3, 10);
		break;
	}
	cp++;
}


void CGame::NotifyMsg_HP(char * pData)
{
 uint32_t * dwp;
 int iPrevHP;
 char cTxt[120];
 int iPrevMP;

	iPrevHP = m_iHP;
	dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
	m_iHP = (int)*dwp;

	iPrevMP = m_iMP;
	dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 6);
	m_iMP = (int)*dwp;

	if (m_iHP > iPrevHP)
	{
		if ((m_iHP - iPrevHP) < 10) return;
		wsprintfA(cTxt, NOTIFYMSG_HP_UP, m_iHP - iPrevHP);
		AddEventList(cTxt, 10);
		PlaySound('E', 21, 0);
	}else if(iPrevHP > m_iHP)
	{
		if(m_cLogOutCount > 0 && !m_bForceDisconn)
		{
			m_cLogOutCount = -1;
			AddEventList(NOTIFYMSG_HP2, 10);
		}
		m_dwDamagedTime = unixtime();
		if (m_iHP < 20) AddEventList(NOTIFYMSG_HP3, 10);
		if ((iPrevHP - m_iHP) < 10) return;
		wsprintfA(cTxt, NOTIFYMSG_HP_DOWN, iPrevHP - m_iHP);
		AddEventList(cTxt, 10);
	}
}

void CGame::NotifyMsg_Hunger(char * pData)
{
 char * cp, cHLv;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	cHLv = *cp;
	iHungerStatus = cHLv; // Hunger Bar xRisenx
	if ((iHungerStatus >= 100)||(iHungerStatus<0)) // Hunger Bar xRisenx
		iHungerStatus = 100; // Hunger Bar xRisenx

	if ((cHLv <= 40) && (cHLv > 30)) AddEventList(NOTIFYMSG_HUNGER1, 10);//"
	if ((cHLv <= 25) && (cHLv > 20)) AddEventList(NOTIFYMSG_HUNGER2, 10);//"
	if ((cHLv <= 20) && (cHLv > 15)) AddEventList(NOTIFYMSG_HUNGER3, 10);//"
	if ((cHLv <= 15) && (cHLv > 10)) AddEventList(NOTIFYMSG_HUNGER4, 10);//"
	if ((cHLv <= 10) && (cHLv >= 0)) AddEventList(NOTIFYMSG_HUNGER5, 10);//"
}

#ifdef MonsterBarClient
void CGame::NotifyMsg_NpcBar(char * pData)
{
 char * cp, iHP;
 cp = (char *)(pData + INDEX2_MSGTYPE + 2);
 iHP = *cp;
 iNpcBar_HP = iHP;
}

void CGame::NotifyMsg_NpcBar2(char * pData) 
{
 char * cp, iMP;
 cp = (char *)(pData + INDEX2_MSGTYPE + 2);
 iMP = *cp;
 iNpcBar_MP = iMP;
 bNpcBar = true;
 m_sNpcBar = 5;
}
#endif

void CGame::NotifyMsg_ItemColorChange(char *pData)
{
 short * sp, sItemIndex, sItemColor;
 char * cp;
 char cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	sp = (short *)cp;
	sItemIndex = *sp;
	cp += 2;

	sp = (short *)cp;
	sItemColor = (short)*sp;
	cp += 2;

	if (m_pItemList[sItemIndex] != 0) {
		char cStr1[64], cStr2[64], cStr3[64];
		GetItemName( m_pItemList[sItemIndex], cStr1, cStr2, cStr3 );
		if (sItemColor != -1) {
			m_pItemList[sItemIndex]->m_ItemColor = (char)sItemColor;
			wsprintfA(cTxt, NOTIFYMSG_ITEMCOLOR_CHANGE1, cStr1);
			AddEventList(cTxt, 10);
		}
		else {
			wsprintfA(cTxt, NOTIFYMSG_ITEMCOLOR_CHANGE2, cStr1);
			AddEventList(cTxt, 10);
		}
	}
}

void CGame::NotifyMsg_ItemSocketChange(char *pData)
{
 short * sp, sItemIndex;
 char * cp;
 char cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	sp = (short *)cp;
	sItemIndex = *sp;
	cp += 2;

	if(!m_pItemList[sItemIndex])
		return;

	for(int i = 0; i < MAXITEMSOCKETS; i++)
	{
		Pop(cp, m_pItemList[sItemIndex]->m_sockets[i]);
	}

	if(m_pItemList[sItemIndex]->m_sockets[0] == SG_VORTEXGEM)
		m_pItemList[sItemIndex]->m_ItemColor = 10;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName( m_pItemList[sItemIndex], cStr1, cStr2, cStr3 );

	wsprintfA(cTxt, NOTIFYMSG_ITESOCKET_CHANGE1, cStr1);
	AddEventList(cTxt, 10);
}

void CGame::NotifyMsg_ItemDepleted_EraseItem(char * pData)
{
 char * cp;
 uint16_t * wp;
 short  sItemIndex;
 bool   bIsUseItemResult;
 char   cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	wp = (uint16_t *)cp;
	sItemIndex = *wp;
	cp += 2;

	bIsUseItemResult = (bool)*cp;
	cp += 2;

	ZeroMemory(cTxt, sizeof(cTxt));

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[sItemIndex], cStr1, cStr2, cStr3);

	if (m_bIsItemEquipped[sItemIndex] == true) {
		wsprintfA(cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
		AddEventList(cTxt, 10);

		m_sItemEquipmentStatus[	m_pItemList[sItemIndex]->m_cEquipPos ] = -1;
		m_bIsItemEquipped[sItemIndex] = false;
	}

	ZeroMemory(cTxt, sizeof(cTxt));

	switch(m_pItemList[sItemIndex]->m_cItemType)
	{
	case ITEMTYPE_CONSUME:
	case ITEMTYPE_ARROW:
		wsprintfA(cTxt, NOTIFYMSG_ITEMDEPlETED_ERASEITEM2, cStr1);
		break;
	case ITEMTYPE_USE_DEPLETE:
		if (bIsUseItemResult)
			wsprintfA(cTxt, NOTIFYMSG_ITEMDEPlETED_ERASEITEM3, cStr1);
		break;
	case ITEMTYPE_EAT:
		if (bIsUseItemResult)
		{
			wsprintfA(cTxt, NOTIFYMSG_ITEMDEPlETED_ERASEITEM4, cStr1);
			if ( (m_sPlayerType >= 1) && (m_sPlayerType <= 3) )
				PlaySound('C', 19, 0);
			if ( (m_sPlayerType >= 4) && (m_sPlayerType <= 6) )
				PlaySound('C', 20, 0);
		}
		break;
	case ITEMTYPE_USE_DEPLETE_DEST:
		if (bIsUseItemResult)
			wsprintfA(cTxt, NOTIFYMSG_ITEMDEPlETED_ERASEITEM3, cStr1);
		break;
	default:
		if (bIsUseItemResult)
		{
			wsprintfA(cTxt, NOTIFYMSG_ITEMDEPlETED_ERASEITEM6, cStr1);
			PlaySound('E', 10, 0);
		}
		break;
	}

	AddEventList(cTxt, 10);

	if (bIsUseItemResult == true) 	m_bItemUsingStatus = false;
	EraseItem((char)sItemIndex);
	_bCheckBuildItemStatus();
}

void CGame::NotifyMsg_ItemLifeSpanEnd(char * pData)
{
 char * cp;
 short * sp, sEquipPos, sItemIndex;
 char cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	sp = (short *)cp;
	sEquipPos = *sp;
	cp += 2;
	sp = (short *)cp;
	sItemIndex = *sp;
	cp += 2;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName( m_pItemList[sItemIndex], cStr1, cStr2, cStr3 );
	wsprintfA(cTxt, NOTIFYMSG_ITEMLIFE_SPANEND1, cStr1);
	AddEventList(cTxt, 10);
	m_sItemEquipmentStatus[	m_pItemList[sItemIndex]->m_cEquipPos ] = -1;
	m_bIsItemEquipped[sItemIndex] = false;
	m_pItemList[sItemIndex]->m_wCurLifeSpan = 0;

	PlaySound('E', 10, 0);
}

void CGame::NotifyMsg_ItemObtained(char * pData)
{
	char * cp;
	short * sp;
	uint32_t * dwp;

	uint32_t dwCount, dwAttribute;
	char  cName[21];
	bool  bIsEquipped;
	uint16_t sSprite, sSpriteFrame, sLevelLimit;
	int8_t sSpecialEV2;
	char  cTxt[120];
	uint8_t cGenderLimit, cItemType, cEquipPos;
	uint16_t  * wp, wWeight, wCurLifeSpan;
	uint32_t ItemColor;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	ZeroMemory(cName, sizeof(cName));
	Pop(cp, cName, 20);
	Pop(cp, dwCount);
	Pop(cp, cItemType);
	Pop(cp, cEquipPos);
	Pop(cp, (uint8_t&)bIsEquipped);
	Pop(cp, sLevelLimit);
	Pop(cp, cGenderLimit);
	Pop(cp, wCurLifeSpan);
	Pop(cp, wWeight);
	Pop(cp, sSprite);
	Pop(cp, sSpriteFrame);
	Pop(cp, ItemColor);
	Pop(cp, (uint8_t&)sSpecialEV2);
	Pop(cp, dwAttribute);

	uint8_t sockets[MAXITEMSOCKETS];

	for(int i = 0; i < MAXITEMSOCKETS; i++)
	{
		Pop(cp, sockets[i]);
	}

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(cName, dwAttribute, cStr1, cStr2, cStr3);

	ZeroMemory(cTxt, sizeof(cTxt));
	if( dwCount == 1 ) wsprintfA(cTxt, NOTIFYMSG_ITEMOBTAINED2, cStr1);
	else wsprintfA(cTxt, NOTIFYMSG_ITEMOBTAINED1, dwCount, cStr1);

	AddEventList(cTxt, 10);

	PlaySound('E', 20, 0);

	if(cItemType == ITEMTYPE_CONSUME || cItemType == ITEMTYPE_ARROW)
	{
		for (int i = 0; i < MAXITEMS; i++)
		if ((m_pItemList[i] != 0) && (memcmp(m_pItemList[i]->m_cName, cName, 20) == 0))
		{
			m_pItemList[i]->m_dwCount += dwCount;
			_iCalcTotalWeight();
			m_bIsItemDisabled[i] = false;
			return;
		}
	}

  	short nX = 40, nY = 30;
  	for (int i = 0; i < MAXITEMS; i++)
  	{
		if ( ( m_pItemList[i] != 0) && (memcmp(m_pItemList[i]->m_cName, cName, 20) == 0))
		{	nX = m_pItemList[i]->m_sX;
			nY = m_pItemList[i]->m_sY;
			break;
		}
	}


	for (int i = 0; i < MAXITEMS; i++)
	{
		if(m_pItemList[i])
			continue;

		m_pItemList[i] = new class CItem;
		memcpy(m_pItemList[i]->m_cName, cName, 20);
		m_pItemList[i]->m_dwCount = dwCount;
		m_pItemList[i]->m_sX      =	nX;
		m_pItemList[i]->m_sY      =	nY;
		bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, i, nX, nY, 0, 0);
		m_pItemList[i]->m_cItemType = cItemType;
		m_pItemList[i]->m_cEquipPos = cEquipPos;
		m_bIsItemDisabled[i]        = false;

		m_bIsItemEquipped[i] = false;
		m_pItemList[i]->m_sLevelLimit  = sLevelLimit;
		m_pItemList[i]->m_cGenderLimit = cGenderLimit;
		m_pItemList[i]->m_wCurLifeSpan = wCurLifeSpan;
		m_pItemList[i]->m_wWeight      = wWeight;
		m_pItemList[i]->m_sSprite      = sSprite;
		m_pItemList[i]->m_sSpriteFrame = sSpriteFrame;
		m_pItemList[i]->m_ItemColor   = ItemColor;
		m_pItemList[i]->m_sItemSpecEffectValue2 = sSpecialEV2;
		m_pItemList[i]->m_dwAttribute = dwAttribute;

		for(int j = 0; j < MAXITEMSOCKETS; j++)
		{
			m_pItemList[i]->m_sockets[j] = sockets[j];
		}

		_bCheckBuildItemStatus();
		_iCalcTotalWeight();

		for(int j = 0; j < MAXITEMS; j++)
		{
			if (m_cItemOrder[j] == -1)
			{
				m_cItemOrder[j] = i;
				return;
			}
		}
		return;
	}
}

void CGame::NotifyMsg_ItemPurchased(char * pData)
{
 char  * cp;
 short * sp;
 uint32_t * dwp;
 uint16_t  * wp;
 int i, j;

 uint32_t dwCount;
 char  cName[21], cItemType, cEquipPos, cGenderLimit;
 bool  bIsEquipped;
 short sSprite, sSpriteFrame, sLevelLimit;
 uint16_t  wCost, wWeight, wCurLifeSpan;
 char  cTxt[120], cItemColor;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	dwp = (uint32_t *)cp;
	dwCount = *dwp;
	cp += 4;

	cItemType = *cp;
	cp++;

	cEquipPos = *cp;
	cp++;

	bIsEquipped = (bool)*cp;
	cp++;

	sp = (short *)cp;
	sLevelLimit = *sp;
	cp += 2;

	cGenderLimit = *cp;
	cp++;

 	wp = (uint16_t *)cp;
	wCurLifeSpan = *wp;
	cp += 2;

	wp = (uint16_t *)cp;
	wWeight = *wp;
	cp += 2;

	sp = (short *)cp;
	sSprite = *sp;
	cp += 2;

	sp = (short *)cp;
	sSpriteFrame = *sp;
	cp += 2;

	cItemColor = *cp;
	cp++;

	wp = (uint16_t *)cp;
	wCost = *wp;
	ZeroMemory(cTxt, sizeof(cTxt));
	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName( cName, 0, cStr1, cStr2, cStr3 );
	wsprintfA(cTxt, NOTIFYMSG_ITEMPURCHASED, cStr1, wCost);
	AddEventList(cTxt, 10);

	if ( (cItemType == ITEMTYPE_CONSUME) || (cItemType == ITEMTYPE_ARROW))
	{	for (i = 0; i < MAXITEMS; i++)
		if ((m_pItemList[i] != 0) && (memcmp(m_pItemList[i]->m_cName, cName, 20) == 0))
		{	m_pItemList[i]->m_dwCount += dwCount;
			_iCalcTotalWeight();
			return;
	}	}

 short nX, nY;
 for (i = 0; i < MAXITEMS; i++)
  {	  if ( ( m_pItemList[i] != 0) && (memcmp(m_pItemList[i]->m_cName, cName, 20) == 0))
	  {	  nX = m_pItemList[i]->m_sX;
		  nY = m_pItemList[i]->m_sY;
		  break;
	  }else
	  {  nX = 40;
		  nY = 30;
  }  }

	for (i = 0; i < MAXITEMS; i++)
	if (m_pItemList[i] == 0)
	{	m_pItemList[i] = new class CItem;
		memcpy(m_pItemList[i]->m_cName, cName, 20);
		m_pItemList[i]->m_dwCount      = dwCount;
		//m_pItemList[i]->m_sX           = 40;
		//m_pItemList[i]->m_sY           = 30;
		m_pItemList[i]->m_sX           = nX;
		m_pItemList[i]->m_sY           = nY;
		bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, i, nX, nY, 0, 0);
		m_pItemList[i]->m_cItemType    = cItemType;
		m_pItemList[i]->m_cEquipPos    = cEquipPos;
		m_bIsItemDisabled[i]           = false;
		m_bIsItemEquipped[i]           = false;
		m_pItemList[i]->m_sLevelLimit  = sLevelLimit;
		m_pItemList[i]->m_cGenderLimit = cGenderLimit;
		m_pItemList[i]->m_wCurLifeSpan = wCurLifeSpan;
		m_pItemList[i]->m_wWeight      = wWeight;
		m_pItemList[i]->m_sSprite      = sSprite;
		m_pItemList[i]->m_sSpriteFrame = sSpriteFrame;
		m_pItemList[i]->m_ItemColor   = cItemColor;

		_iCalcTotalWeight();
		// fixed v1.11
		for (j = 0; j < MAXITEMS; j++)
		if (m_cItemOrder[j] == -1) {
			m_cItemOrder[j] = i;
			return;
		}

		return;
	}
}

void CGame::NotifyMsg_ItemReleased(char * pData)
{
 char * cp;
 short * sp, sEquipPos, sItemIndex;
 char cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	sp = (short *)cp;
	sEquipPos = *sp;
	cp += 2;
	sp = (short *)cp;
	sItemIndex = *sp;
	cp += 2;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[sItemIndex], cStr1, cStr2, cStr3);
	wsprintfA(cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
	AddEventList(cTxt, 10);
	m_bIsItemEquipped[sItemIndex] = false;
	m_sItemEquipmentStatus[	m_pItemList[sItemIndex]->m_cEquipPos ] = -1;

	if(memcmp(m_pItemList[sItemIndex]->m_cName, "AngelicPendant", 14) == 0) PlaySound('E', 53, 0);
	else PlaySound('E', 29, 0);
}

void CGame::NotifyMsg_ItemRepaired(char * pData)
{
 char * cp, cTxt[120];
 uint32_t * dwp, dwItemID, dwLife;

	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);

	dwp = (uint32_t *)cp;
	dwItemID = *dwp;
	cp += 4;

	dwp = (uint32_t *)cp;
	dwLife = *dwp;
	cp += 4;

	m_pItemList[dwItemID]->m_wCurLifeSpan = (uint16_t)dwLife;
	m_bIsItemDisabled[dwItemID] = false;
	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName( m_pItemList[dwItemID], cStr1, cStr2, cStr3 );

	if (m_pItemList[dwItemID]->m_wCurLifeSpan != m_pItemList[dwItemID]->m_wMaxLifeSpan){
		wsprintfA(cTxt, NOTIFYMSG_ITEMREPAIRED2, cStr1);
	}
	else {
	wsprintfA(cTxt, NOTIFYMSG_ITEMREPAIRED1, cStr1);
	}

	AddEventList(cTxt, 10);
}

void CGame::NotifyMsg_ItemToBank(char *pData)
{
 char * cp, cIndex;
 uint32_t * dwp, dwCount, dwAttribute;
 char  cName[21], cItemType, cEquipPos, cGenderLimit, cItemColor;
 bool  bIsEquipped;
 short * sp, sSprite, sSpriteFrame, sLevelLimit, sItemEffectValue2, sItemSpecEffectValue2;
 uint16_t  * wp, wWeight, wCurLifeSpan;
 char  cTxt[120];

	cp = (pData + INDEX2_MSGTYPE + 2);

	cIndex = *cp;
	cp++;

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	dwp = (uint32_t *)cp;
	dwCount = *dwp;
	cp += 4;

	cItemType = *cp;
	cp++;

	cEquipPos = *cp;
	cp++;

	bIsEquipped = (bool)*cp;
	cp++;

	sp = (short *)cp;
	sLevelLimit = *sp;
	cp += 2;

	cGenderLimit = *cp;
	cp++;

	wp = (uint16_t *)cp;
	wCurLifeSpan = *wp;
	cp += 2;

	wp = (uint16_t *)cp;
	wWeight = *wp;
	cp += 2;

	sp = (short *)cp;
	sSprite = *sp;
	cp += 2;

	sp = (short *)cp;
	sSpriteFrame = *sp;
	cp += 2;

	cItemColor = *cp;
	cp++;


	sp = (short *)cp;
	sItemEffectValue2 = *sp;
	cp += 2;

	dwp = (uint32_t *)cp;
	dwAttribute = *dwp;
	cp += 4;
	sItemSpecEffectValue2 = (short) *cp ;
	cp ++ ;

	uint8_t sockets[MAXITEMSOCKETS];
	for(int i = 0; i < MAXITEMSOCKETS; i++)
	{
		Pop(cp, sockets[i]);
	}

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(cName, dwAttribute, cStr1, cStr2, cStr3);


	if (m_pBankList[cIndex] == 0) {
		m_pBankList[cIndex] = new class CItem;

		memcpy(m_pBankList[cIndex]->m_cName, cName, 20);
		m_pBankList[cIndex]->m_dwCount = dwCount;

		m_pBankList[cIndex]->m_cItemType = cItemType;
		m_pBankList[cIndex]->m_cEquipPos = cEquipPos;

		m_pBankList[cIndex]->m_sLevelLimit  = sLevelLimit;
		m_pBankList[cIndex]->m_cGenderLimit = cGenderLimit;
		m_pBankList[cIndex]->m_wCurLifeSpan = wCurLifeSpan;
		m_pBankList[cIndex]->m_wWeight      = wWeight;
		m_pBankList[cIndex]->m_sSprite      = sSprite;
		m_pBankList[cIndex]->m_sSpriteFrame = sSpriteFrame;
		m_pBankList[cIndex]->m_ItemColor   = cItemColor;
		m_pBankList[cIndex]->m_sItemEffectValue2  = sItemEffectValue2;
		m_pBankList[cIndex]->m_dwAttribute        = dwAttribute;
		m_pBankList[cIndex]->m_sItemSpecEffectValue2 = sItemSpecEffectValue2;

		for(int i = 0; i < MAXITEMSOCKETS; i++)
		{
			m_pBankList[cIndex]->m_sockets[i] = sockets[i];
		}

		ZeroMemory(cTxt, sizeof(cTxt));
		if( dwCount == 1 ) wsprintfA(cTxt, NOTIFYMSG_ITEMTOBANK3, cStr1);
		else wsprintfA(cTxt, NOTIFYMSG_ITEMTOBANK2, dwCount, cStr1);

		if( m_bIsDialogEnabled[14] == true ) m_dialogBoxes[14].sView = MAXBANKITEMS-12;
		AddEventList(cTxt, 10);
	}
}


void CGame::NotifyMsg_Killed(char * pData)
{ char * cp, cAttackerName[21];
	m_bCommandAvailable = false;
	m_cCommand = OBJECTSTOP;
	m_iHP = 0;
	m_cCommand = -1;
	// Restart
	m_bItemUsingStatus = false;
	ClearSkillUsingStatus();
	ZeroMemory(cAttackerName, sizeof(cAttackerName));
	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
	memcpy(cAttackerName, cp, 20);
	cp += 20;
/*	if (strlen(cAttackerName) == 0) // removed in v2.20 (bug?) Many servers send the info themselves.
		AddEventList(NOTIFYMSG_KILLED1, 10);
	else
	{	wsprintfA(G_cTxt, NOTIFYMSG_KILLED2, cAttackerName);
		AddEventList(G_cTxt, 10);
	}*/
	// Snoopy: reduced 3 lines -> 2 lines
	AddEventList(NOTIFYMSG_KILLED1, 10);
	AddEventList(NOTIFYMSG_KILLED3, 10);
	//AddEventList(NOTIFYMSG_KILLED4, 10);//"Log Out
}

// This msg is sent by server when lvl-up
void CGame::NotifyMsg_LevelUp(char * pData)
{char * cp;
 int  * ip;
 int i, iPrevLevel;
 char cTxt[120];

	iPrevLevel = m_iLevel;

	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);

	ip  = (int *)cp;
	m_iLevel = *ip;
	cp += 4;

	ip   = (int *)cp;
	m_stat[STAT_STR] = *ip;
	cp  += 4;

	ip   = (int *)cp;
	m_stat[STAT_VIT] = *ip;
	cp  += 4;

	ip   = (int *)cp;
	m_stat[STAT_DEX] = *ip;
	cp  += 4;

	ip   = (int *)cp;
	m_stat[STAT_INT] = *ip;
	cp  += 4;

	ip   = (int *)cp;
	m_stat[STAT_MAG] = *ip;
	cp  += 4;

	ip   = (int *)cp;
	m_stat[STAT_CHR] = *ip;
	cp  += 4;

	// CLEROTH - LU
	m_iLU_Point = m_iLevel*7 - ( // Stats System xRisenx
	//m_iLU_Point = m_iLevel*TOTALLEVELUPPOINT - (
			(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
			- 70)
			- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			// Stats System xRisenx
			//- TOTALLEVELUPPOINT + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
	m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;

	wsprintfA(cTxt, NOTIFYMSG_LEVELUP1, m_iLevel);// "Level up!!! Level %d!"
	AddEventList(cTxt, 10);

	switch (m_sPlayerType) {
	case 1:
	case 2:
	case 3:
		PlaySound('C', 21, 0);
		break;

	case 4:
	case 5:
	case 6:
		PlaySound('C', 22, 0);
		break;
	}

	_RemoveChatMsgListByObjectID(m_sPlayerObjectID);

	for (i = 1; i < MAXCHATMSGS; i++)
	if (m_pChatMsgList[i] == 0) {
		ZeroMemory(cTxt, sizeof(cTxt));
		strcpy(cTxt, "Level up!");
		m_pChatMsgList[i] = new class CMsg(23, cTxt, m_dwCurTime);
		m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;

		if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == false) {
			delete m_pChatMsgList[i];
			m_pChatMsgList[i] = 0;
		}
		return;
	}
}

// CLEROTH - LU
void CGame::NotifyMsg_SettingSuccess(char * pData)
{char * cp;
 int  * ip;
 int iPrevLevel;
 char cTxt[120];
	iPrevLevel = m_iLevel;
	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
	ip  = (int *)cp;
	m_iLevel = *ip;
	cp += 4;
	ip   = (int *)cp;
	m_stat[STAT_STR] = *ip;
	cp  += 4;
	ip   = (int *)cp;
	m_stat[STAT_VIT] = *ip;
	cp  += 4;
	ip   = (int *)cp;
	m_stat[STAT_DEX] = *ip;
	cp  += 4;
	ip   = (int *)cp;
	m_stat[STAT_INT] = *ip;
	cp  += 4;
	ip   = (int *)cp;
	m_stat[STAT_MAG] = *ip;
	cp  += 4;
	ip   = (int *)cp;
	m_stat[STAT_CHR] = *ip;
	cp  += 4;
	wsprintfA(cTxt, "Your stat has been changed.");
	AddEventList(cTxt, 10);
	// LU
	m_iLU_Point = m_iLevel*7 - ( // Stats System xRisenx
	//m_iLU_Point = m_iLevel*TOTALLEVELUPPOINT - (
			(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
			- 70)
			- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			// Stats System xRisenx
			//- TOTALLEVELUPPOINT + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
	m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;
}

void CGame::NotifyMsg_MagicEffectOff(char * pData)
{char * cp;
 uint16_t * wp;
 short  sMagicType, sMagicEffect;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	sMagicType = (short)*wp;
	cp += 2;
	wp = (uint16_t *)cp;
	sMagicEffect = (short)*wp;
	cp += 2;
	switch (sMagicType) {
	case MAGICTYPE_PROTECT:
		switch (sMagicEffect) {
		case 1: // "Protection from arrows has vanished."
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF1, 10);
			m_bPfa = false; // Magic Icons xRisenx
			break;
		case 2:	// "Protection from magic has vanished."
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF2, 10);
			m_bPfm = false; // Magic Icons xRisenx
			break;
		case 3:	// "Defense shield effect has vanished."
		case 4:	// "Defense shield effect has vanished."
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF3, 10);
			m_bShield = false; // Magic Icons xRisenx
			break;
		case 5:	// "Absolute Magic Protection has been vanished."
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF14, 10);
			m_bPfm = false; // Magic Icons xRisenx
			break;
		}
		break;

	case MAGICTYPE_HOLDOBJECT:
		switch (sMagicEffect) {
		case 1:	// "Hold person magic effect has vanished."
			m_bParalyze = false;
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF4, 10);
			break;

		case 2:	// "Paralysis magic effect has vanished."
			m_bParalyze = false;
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF5, 10);
			break;
		}
		break;

	case MAGICTYPE_INVISIBILITY:
		switch (sMagicEffect) {
		case 1:	// "Invisibility magic effect has vanished."
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF6, 10);
			m_bInv = false; // Magic Icons xRisenx
			break;
		}
		break;

	case MAGICTYPE_CONFUSE:
		switch (sMagicEffect) {
		case 1:	// "Language confuse magic effect has vanished."
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF7, 10);
			break;
		case 2:	// "Confusion magic has vanished."
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF8, 10);
			m_bIsConfusion = false;
			break;
		case 3:	// "Illusion magic has vanished."
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF9, 10);
			m_iIlusionOwnerH = 0;
			break;
		case 4:	// "At last, you gather your senses." // snoopy
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF15, 10);
			m_bIllusionMVT = false;
			break;
		}
		break;

	case MAGICTYPE_POISON:
		if (m_bIsPoisoned) AddEventList(NOTIFYMSG_MAGICEFFECT_OFF10, 10);
		m_bIsPoisoned = false;
		break;

	case MAGICTYPE_BERSERK:
		switch (sMagicEffect) {
		case 1:
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF11, 10);
			m_bZerk = false; // Magic Icons xRisenx
			break;
		}
		break;

	case MAGICTYPE_POLYMORPH:
		switch (sMagicEffect) {
		case 1:
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF12, 10);
			break;
		}
		break;

	case MAGICTYPE_ICE:
	//case MAGICTYPE_ICE_MB:
	//case MAGICTYPE_ICE_COTG:
		AddEventList(NOTIFYMSG_MAGICEFFECT_OFF13, 10);
		break;
	}
}

void CGame::NotifyMsg_MagicEffectOn(char * pData)
{char * cp;
 uint32_t * dwp;
 uint16_t * wp;
 short  sMagicType, sMagicEffect, sOwnerH;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	sMagicType = (short)*wp;
	cp += 2;
	dwp = (uint32_t *)cp;
	sMagicEffect = (short)*dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	sOwnerH = (short)*dwp;
	cp += 4;
	switch (sMagicType) {
	case MAGICTYPE_PROTECT:
		switch (sMagicEffect) {
		case 1: // "You are completely protected from arrows!"
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON1, 10);
			m_bPfa = true; // Magic Icons xRisenx
			m_sPfa = 60; // Magic Icons Time Limit xRisenx
			break;
		case 2: // "You are protected from magic!"
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON2, 10);
			m_bPfm = true; // Magic Icons xRisenx
			m_sPfm = 60; // Magic Icons Time Limit xRisenx
			break;
		case 3: // "Defense ratio increased by a magic shield!"
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON3, 10);
			m_bShield = true; // Magic Icons xRisenx
			m_sShield = 60; // Magic Icons Time Limit xRisenx
			break;
		case 4: // "Defense ratio increased by a magic shield!"
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON3, 10);
			m_bShield = true; // Magic Icons xRisenx
			m_sShield = 60; // Magic Icons Time Limit xRisenx
			break;
		case 5: // "You are completely protected from magic!"
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON14, 10);
			m_bPfm = true; // Magic Icons xRisenx
			m_sPfm = 60; // Magic Icons Time Limit xRisenx
			break;
		}
		break;

	case MAGICTYPE_HOLDOBJECT:
		switch (sMagicEffect) {
		case 1: // "You were bounded by a Hold Person spell! Unable to move!"
			m_bParalyze = true;
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON4, 10);
			break;
		case 2: // "You were bounded by a Paralysis spell! Unable to move!"
			m_bParalyze = true;
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON5, 10);
			break;
		}
		break;

	case MAGICTYPE_INVISIBILITY:
		switch (sMagicEffect) {
		case 1: // "You are now invisible, no one can see you!"
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON6, 10);
			m_bInv = true; // Magic Icons xRisenx
			m_sInv = 60; // Magic Icons Time Limit xRisenx
			break;
		}
		break;

	case MAGICTYPE_CONFUSE:
		switch (sMagicEffect) {
		case 1:	// Confuse Language "No one understands you because of language confusion magic!"
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON7, 10);
			break;

		case 2: // Confusion "Confusion magic casted, impossible to determine player allegience."
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON8, 10);
			m_bIsConfusion = true;
			break;

		case 3:	// Illusion "Illusion magic casted, impossible to tell who is who!"
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON9, 10);
			_SetIlusionEffect(sOwnerH);
			break;

		case 4:	// IllusionMouvement "You are thrown into confusion, and you are flustered yourself." // snoopy
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON15, 10);
			m_bIllusionMVT = true;
			break;
		}
		break;

	case MAGICTYPE_POISON:
		AddEventList(NOTIFYMSG_MAGICEFFECT_ON10, 10);
		m_bIsPoisoned = true;
		break;

	case MAGICTYPE_BERSERK:
		switch (sMagicEffect) {
		case 1:
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON11, 10);
			m_bZerk = true; // Magic Icons xRisenx
			m_sZerk = 40; // Magic Icons Time Limit xRisenx
			break;

		case 2: // Greater Berserk
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON11, 10);
			m_bZerk = true; // Magic Icons xRisenx
			m_sZerk = 80; // Magic Icons Time Limit xRisenx

		case 3: // Ancient Berserk
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON11, 10);
			m_bZerk = true; // Magic Icons xRisenx
			m_sZerk = 300; // Magic Icons Time Limit xRisenx
			break;
		}
		break;

	case MAGICTYPE_POLYMORPH:
		switch (sMagicEffect) {
		case 1:
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON12, 10);
			break;
		}
		break;

	case MAGICTYPE_ICE:
	//case MAGICTYPE_ICE_MB:
	//case MAGICTYPE_ICE_COTG:
		AddEventList(NOTIFYMSG_MAGICEFFECT_ON13, 10);
		break;
	}
}

void CGame::NotifyMsg_MagicStudyFail(char * pData)
{
 char * cp, cMagicNum, cName[31], cFailCode;
 char cTxt[120];
 int  * ip, iCost, iReqInt;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	cFailCode = *cp;
	cp++;
	cMagicNum = *cp;
	cp++;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 30);
	cp += 30;
	ip = (int *)cp;
	iCost = *ip;
	cp += 4;
	ip = (int *)cp;
	iReqInt = *ip;
	cp += 4;
/*	// Snoopy: remove special CLEROTH's feature
	ip = (int *)cp;
	iReqStr = *ip;
	cp += 4;
	// CLEROTH
	wsprintfA(cTxt, NOTIFYMSG_MAGICSTUDY_FAIL4, cName, iCost, iReqInt, iReqStr);
	AddEventList(cTxt, 10);*/

	if (iCost > 0)
	{	wsprintfA(cTxt, NOTIFYMSG_MAGICSTUDY_FAIL1, cName);
		AddEventList(cTxt, 10);
	}else
	{	wsprintfA(cTxt, NOTIFYMSG_MAGICSTUDY_FAIL2,  cName);
		AddEventList(cTxt, 10);
		wsprintfA(cTxt, NOTIFYMSG_MAGICSTUDY_FAIL3, iReqInt);
		AddEventList(cTxt, 10);
	}
}

void CGame::NotifyMsg_MagicStudySuccess(char * pData)
{char * cp, cMagicNum, cName[31];
 char cTxt[120];
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	cMagicNum = *cp;
	cp++;
	m_cMagicMastery[cMagicNum] = 1;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 30);
	wsprintfA(cTxt, NOTIFYMSG_MAGICSTUDY_SUCCESS1, cName);
	AddEventList(cTxt, 10);
	PlaySound('E', 23, 0);
}

void CGame::NotifyMsg_MP(char * pData)
{uint32_t * dwp;
 int iPrevMP;
 char cTxt[120];
	iPrevMP = m_iMP;
	dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
	m_iMP = (int)*dwp;
	if (abs(m_iMP - iPrevMP) < 10) return;
	if (m_iMP > iPrevMP)
	{	wsprintfA(cTxt, NOTIFYMSG_MP_UP, m_iMP - iPrevMP);
		AddEventList(cTxt, 10);
		PlaySound('E', 21, 0);
	}else
	{	wsprintfA(cTxt, NOTIFYMSG_MP_DOWN, iPrevMP - m_iMP);
		AddEventList(cTxt, 10);
	}
}

void CGame::NotifyMsg_NewGuildsMan(char * pData)
{char * cp, cName[12], cTxt[120];
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);
	wsprintfA(cTxt, NOTIFYMSG_NEW_GUILDMAN1, cName);
	AddEventList(cTxt, 10);
	ClearGuildNameList();
}

void CGame::NotifyMsg_PKcaptured(char *pData)
{char  * cp;
 uint32_t * dwp;
 uint16_t  * wp;
 int     iPKcount, iLevel, iExp, iRewardGold;
 char cTxt[120], cName[12];
	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	iPKcount = *wp;
	cp += 2;
	wp = (uint16_t *)cp;
	iLevel = *wp;
	cp += 2;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);
	cp += 10;
	dwp = (uint32_t *)cp;
	iRewardGold = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iExp = *dwp;
	cp += 4;
	wsprintfA(cTxt, NOTIFYMSG_PK_CAPTURED1, iLevel, cName, iPKcount);
	AddEventList(cTxt, 10);
	wsprintfA(cTxt, EXP_INCREASED, iExp - m_iExp);
	AddEventList(cTxt, 10);
	wsprintfA(cTxt, NOTIFYMSG_PK_CAPTURED3, iExp - m_iExp);
	AddEventList(cTxt, 10);
}

void CGame::NotifyMsg_PKpenalty(char *pData)
{char  * cp;
 uint32_t * dwp;
 int     iPKcount, iExp, iStr, iVit, iDex, iInt, iMag, iChr;
	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
	dwp = (uint32_t *)cp;
	iExp = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iStr = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iVit = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iDex = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iInt = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iMag = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iChr = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iPKcount = *dwp;
	cp += 4;
	wsprintfA(G_cTxt, NOTIFYMSG_PK_PENALTY1, iPKcount);
	AddEventList(G_cTxt, 10);
	if (m_iExp > iExp)
	{	wsprintfA(G_cTxt, NOTIFYMSG_PK_PENALTY2, m_iExp - iExp);
		AddEventList(G_cTxt, 10);
	}
	m_iExp = iExp;
	m_stat[STAT_STR] = iStr;
	m_stat[STAT_VIT] = iVit;
	m_stat[STAT_DEX] = iDex;
	m_stat[STAT_INT] = iInt;
	m_stat[STAT_MAG] = iMag;
	m_stat[STAT_CHR] = iChr;
	m_iPKCount = iPKcount;
}

void CGame::NotifyMsg_PlayerShutUp(char * pData)
{char * cp, cName[12];
 uint16_t * wp, wTime;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	wTime = *wp;
	cp += 2;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);
	cp += 10;
	if (memcmp(m_cPlayerName, cName, 10) == 0)
		 wsprintfA(G_cTxt, NOTIFYMSG_PLAYER_SHUTUP1, wTime);
	else wsprintfA(G_cTxt, NOTIFYMSG_PLAYER_SHUTUP2, cName, wTime);

	AddEventList(G_cTxt, 10);
}

void CGame::NotifyMsg_UnitStatus(bool bOnGame, char * pData)
{char cName[12], cMapName[12], * cp;
 uint16_t * wp ;
 uint16_t  dx= 1 ,dy = 1;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);
	cp += 10;
	ZeroMemory(cMapName, sizeof(cMapName));
	memcpy(cMapName, cp, 10);
	cp += 10;
	wp = (uint16_t * ) cp ;
	dx = (uint16_t ) *wp ;
	cp += 2 ;
	wp = (uint16_t * ) cp ;
	dy = (uint16_t ) *wp ;
	cp += 2 ;
	ZeroMemory(G_cTxt, sizeof(G_cTxt));
	if (bOnGame == true) {
		if (strlen(cMapName) == 0)
			 wsprintfA(G_cTxt, NOTIFYMSG_PLAYER_STATUS1, cName);
		else wsprintfA(G_cTxt, NOTIFYMSG_PLAYER_STATUS2, cName, cMapName, dx, dy);
	}
	else wsprintfA(G_cTxt, NOTIFYMSG_PLAYER_STATUS3, cName);
	AddEventList(G_cTxt, 10);
}


void CGame::NotifyMsg_QuestReward(char *pData)
{short * sp, sWho, sFlag;
 char  * cp, cRewardName[21], cTxt[120];
 int   * ip, iAmount, iIndex, iPreCon;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	sp = (short *)cp;
	sWho = *sp;
	cp += 2;
	sp = (short *)cp;
	sFlag = *sp;
	cp += 2;
	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;
	ZeroMemory(cRewardName, sizeof(cRewardName));
	memcpy(cRewardName, cp, 20);
	cp += 20;
	iPreCon = m_iContribution;
	ip = (int *)cp;
	m_iContribution = *ip;
//	m_iContributionPrice = 0;
	cp += 4;

	if (sFlag == 1)
	{	m_stQuest.sWho          = 0;
		m_stQuest.sQuestType    = 0;
		m_stQuest.sContribution = 0;
		m_stQuest.sTargetType   = 0;
		m_stQuest.sTargetCount  = 0;
		m_stQuest.sX     = 0;
		m_stQuest.sY     = 0;
		m_stQuest.sRange = 0;
		m_stQuest.sCurrentCount = 0;
		m_stQuest.bIsQuestCompleted = false;
		ZeroMemory(m_stQuest.cTargetName, sizeof(m_stQuest.cTargetName));
		EnableDialogBox(21, 0, sWho+110, 0);
		iIndex = m_dialogBoxes[21].sV1;
		m_pMsgTextList2[iIndex] = new class CMsg(0, "  ", 0);
		iIndex++;
		ZeroMemory(cTxt, sizeof(cTxt));
		if (memcmp(cRewardName, "exp", 3) == 0)
		{	if (iAmount > 0) wsprintfA(cTxt, NOTIFYMSG_QUEST_REWARD1, iAmount);
		}else
		{	wsprintfA(cTxt, NOTIFYMSG_QUEST_REWARD2, iAmount, cRewardName);
		}
		m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
		iIndex++;
		m_pMsgTextList2[iIndex] = new class CMsg(0, "  ", 0);
		iIndex++;
		ZeroMemory(cTxt, sizeof(cTxt));
		if (iPreCon < m_iContribution)
			 wsprintfA(cTxt, NOTIFYMSG_QUEST_REWARD3, m_iContribution - iPreCon);
		else wsprintfA(cTxt, NOTIFYMSG_QUEST_REWARD4, iPreCon - m_iContribution);

		m_pMsgTextList2[iIndex] = new class CMsg(0, "  ", 0);
		iIndex++;
	}
	else EnableDialogBox(21, 0, sWho+120, 0);
}

void CGame::NotifyMsg_RatingPlayer(char * pData)
{//int * ip;
 char * cp, cName[12];
 uint16_t  cValue;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	cValue = *cp;
	cp++;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);
	cp += 10;
//	ip = (int *)cp;
//	m_iRating = *ip;
	cp += 4;
	ZeroMemory(G_cTxt, sizeof(G_cTxt));
	if (memcmp(m_cPlayerName, cName, 10) == 0)
	{	if (cValue == 1)
		{	 strcpy(G_cTxt, NOTIFYMSG_RATING_PLAYER1);
			 PlaySound('E', 23, 0);
 		}
	}else
	{	if (cValue == 1)
			 wsprintfA(G_cTxt, NOTIFYMSG_RATING_PLAYER2, cName);
		else wsprintfA(G_cTxt, NOTIFYMSG_RATING_PLAYER3, cName);
	}
	AddEventList(G_cTxt, 10);
}


void CGame::NotifyMsg_ServerChange(char * pData)
{
 char * cp, cWorldServerAddr[16];	//Snoopy: change names for better readability
 int * ip, iWorldServerPort;		//Snoopy: change names for better readability

	ZeroMemory(m_cMapName, sizeof(m_cMapName));
	ZeroMemory(m_cMapMessage, sizeof(m_cMapMessage));
	ZeroMemory(cWorldServerAddr, sizeof(cWorldServerAddr));


	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    memcpy(m_cMapName, cp, 10);

//	m_cMapIndex = GetOfficialMapName(m_cMapName, m_cMapMessage);
	cp += 10;

	memcpy(cWorldServerAddr, cp, 15);
	cp += 15;
	ip = (int *)cp;
	iWorldServerPort = *ip;
	cp += 4;
	gamemode = 0;
	if (_socket)
	{
		_socket->stop();
	}
	if (_socket == nullptr)
	{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
        CreateSocket();
        new_connection_->socket().async_connect(endpoint,
			boost::bind(&CGame::handle_connect, this,
			boost::asio::placeholders::error));
	}

	m_bIsPoisoned = false;

	ChangeGameMode(GAMEMODE_ONCONNECTING);
	m_dwConnectMode  = MSGID_REQUEST_ENTERGAME;
	//m_wEnterGameType = ENTERGAMEMSGTYPE_NEW; //Gateway
		m_wEnterGameType = ENTERGAMEMSGTYPE_CHANGINGSERVER;
		ZeroMemory(m_cMsg, sizeof(m_cMsg));
	strcpy(m_cMsg,"55");
}

void CGame::NotifyMsg_SetItemCount(char * pData)
{char  * cp;
 uint16_t  * wp;
 uint32_t * dwp;
 short  sItemIndex;
 uint32_t  dwCount;
 bool   bIsItemUseResponse;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	sItemIndex = *wp;
	cp += 2;
	dwp = (uint32_t *)cp;
	dwCount = *dwp;
	cp += 4;
	bIsItemUseResponse = (bool)*cp;
	cp++;
	if (m_pItemList[sItemIndex] != 0)
	{	m_pItemList[sItemIndex]->m_dwCount = dwCount;
		_iCalcTotalWeight();
		if (bIsItemUseResponse == true) m_bIsItemDisabled[sItemIndex] = false;
	}
}

void CGame::NotifyMsg_ShowMap(char * pData)
{char * cp;
 uint16_t * wp, w1, w2;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	w1 = *wp;
	cp += 2;
	wp = (uint16_t *)cp;
	w2 = *wp;
	cp += 2;
	if (w2 == 0) AddEventList(NOTIFYMSG_SHOW_MAP1, 10);
	else EnableDialogBox(22, 0, w1, w2 -1);
}

void CGame::NotifyMsg_Skill(char *pData)
{
	uint16_t * wp;
	short sSkillIndex, sValue;
	char * cp;
	char cTxt[120];
	int i;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	sSkillIndex = (short)*wp;
	cp += 2;
	wp = (uint16_t *)cp;
	sValue = (short)*wp;
	cp += 2;
	_RemoveChatMsgListByObjectID(m_sPlayerObjectID);
	if(!m_pSkillCfgList[sSkillIndex]) return;
	if (m_pSkillCfgList[sSkillIndex]->m_iLevel < sValue)
	{	wsprintfA(cTxt, NOTIFYMSG_SKILL1, m_pSkillCfgList[sSkillIndex]->m_cName, sValue - m_pSkillCfgList[sSkillIndex]->m_iLevel);
		AddEventList(cTxt, 10);
		PlaySound('E', 23, 0);
		for (i = 1; i < MAXCHATMSGS; i++)
		if (m_pChatMsgList[i] == 0)
		{	ZeroMemory(cTxt, sizeof(cTxt));
			wsprintfA(cTxt, "%s +%d%%", m_pSkillCfgList[sSkillIndex]->m_cName, sValue - m_pSkillCfgList[sSkillIndex]->m_iLevel);
			m_pChatMsgList[i] = new class CMsg(20, cTxt, m_dwCurTime);
			m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;
			if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == false)
			{	delete m_pChatMsgList[i];
				m_pChatMsgList[i] = 0;
			}
			break;
		}
	}else if (m_pSkillCfgList[sSkillIndex]->m_iLevel > sValue) {
		wsprintfA(cTxt, NOTIFYMSG_SKILL2, m_pSkillCfgList[sSkillIndex]->m_cName, m_pSkillCfgList[sSkillIndex]->m_iLevel - sValue);
		AddEventList(cTxt, 10);
		PlaySound('E', 24, 0);
		for (i = 1; i < MAXCHATMSGS; i++)
		if (m_pChatMsgList[i] == 0)
		{	ZeroMemory(cTxt, sizeof(cTxt));
			wsprintfA(cTxt, "%s -%d%%", m_pSkillCfgList[sSkillIndex]->m_cName, sValue - m_pSkillCfgList[sSkillIndex]->m_iLevel);
			m_pChatMsgList[i] = new class CMsg(20, cTxt, m_dwCurTime);
			m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;
			if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == false)
			{	delete m_pChatMsgList[i];
				m_pChatMsgList[i] = 0;
			}
			break;
	}	}
	m_pSkillCfgList[sSkillIndex]->m_iLevel = sValue;
	m_cSkillMastery[sSkillIndex] = (unsigned char)sValue;
}


void CGame::NotifyMsg_SkillTrainSuccess(char * pData)
{char * cp, cSkillNum, cSkillLevel;
 char cTemp[120];
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	cSkillNum = *cp;
	cp++;
	cSkillLevel = *cp;
	cp++;
	if(!m_pSkillCfgList[cSkillNum]) return;
	ZeroMemory(cTemp, sizeof(cTemp));
	wsprintfA(cTemp, NOTIFYMSG_SKILL_TRAIN_SUCCESS1, m_pSkillCfgList[cSkillNum]->m_cName, cSkillLevel);
	AddEventList(cTemp, 10);
	m_pSkillCfgList[cSkillNum]->m_iLevel = cSkillLevel;
	m_cSkillMastery[cSkillNum] = (unsigned char)cSkillLevel;
	PlaySound('E', 23, 0);
}

void CGame::NotifyMsg_SkillUsingEnd(char * pData)
{
	char * cp;
	uint16_t * wp, wResult;

	cp = (char *)(pData	+ INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	wResult = * wp;
	switch (wResult) {
	case 0:
		AddEventList(NOTIFYMSG_SKILL_USINGEND1, 10);
		break;
	case 1:
		AddEventList(NOTIFYMSG_SKILL_USINGEND2, 10);
		break;
	}
	m_bSkillUsingStatus = false;
}

void CGame::NotifyMsg_SP(char * pData)
{
	uint32_t * dwp;
	int iPrevSP;

	iPrevSP = m_iSP;
	dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
	m_iSP = (int)*dwp;
	if (abs(m_iSP - iPrevSP) < 10) return;
	if (m_iSP > iPrevSP)
	{
		wsprintfA(G_cTxt, NOTIFYMSG_SP_UP, m_iSP - iPrevSP);
		AddEventList(G_cTxt, 10);
		PlaySound('E', 21, 0);
	}
	else
	{
		wsprintfA(G_cTxt, NOTIFYMSG_SP_DOWN, iPrevSP - m_iSP);
		AddEventList(G_cTxt, 10);
	}
}

void CGame::NotifyMsg_TotalUsers(char * pData)
{
	uint16_t *wp;
	int iTotal;
	wp = (uint16_t *)(pData + INDEX2_MSGTYPE + 2);
	iTotal = (int)*wp;
	wsprintfA(G_cTxt, NOTIFYMSG_TOTAL_USER1, iTotal);
	AddEventList(G_cTxt, 10);
}

void CGame::NotifyMsg_RelicInAltar(char * pData)
{
	char *cp;
	Side altarSide;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	altarSide = (Side)*cp;
	wsprintfA(G_cTxt, NOTIFYMSG_RELICINALTAR, sideName[altarSide]);
	SetTopMsg(G_cTxt, 10);

	m_relicOwnedSide = altarSide;
	m_relicOwnedTime = unixtime() + 1 * 1000;
}

void CGame::NotifyMsg_RelicGrabbed(char * pData)
{
	char *cp;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	char playerName[12];
	ZeroMemory(playerName, sizeof(playerName));
	memcpy(playerName, cp, 10);

	wsprintfA(G_cTxt, NOTIFYMSG_RELICGRABBED, playerName);
	AddEventList(G_cTxt, 10);

	m_relicOwnedTime = 0;
}

void CGame::NotifyMsg_EventWinner(char * pData)
{
	char *cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	
	EventType eType = ET_NONE;
	Pop(cp, (uint8_t&)eType);
	
	Side winnerSide = NEUTRAL;
	Pop(cp, (uint8_t&)winnerSide);

	wsprintfA(G_cTxt, NOTIFYMSG_EVENTWINNER, sideName[winnerSide], eventName[eType]);
	SetTopMsg(G_cTxt, 10);

	m_bIsAstoriaMode = false;

	if(winnerSide != m_side)
		return;

	switch(eType)
	{
	case ET_CAPTURE:
		AddEventList(NOTIFYMSG_EVENTWINNERCTR, 10);
		break;
		
	case ET_DESTROY_SHIELD:
		AddEventList(NOTIFYMSG_EVENTWINNERDTS, 10);
		if(memcmp(m_cMapName, "astoria", 7) == 0)
		{
			AddEventList(NOTIFYMSG_EVENTWINNERDTS2, 10);
			m_iContribution += 30;
		} else {
			AddEventList(NOTIFYMSG_EVENTWINNERDTS2B, 10);
		}
		break;
		
	}
}

void CGame::NotifyMsg_EventStarting(char * pData)
{
	char *cp;
	EventType eType;

	uint16_t * wp = (uint16_t *)(pData + INDEX2_MSGTYPE);
	uint32_t msgID = *wp;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	eType = (EventType)*cp;

	sstream msg;
	msg.str().c_str();
	msg.flush();
	msg.str().c_str();

	switch(eType)
	{
	case ET_CAPTURE:
		msg << "A " << eventName[eType] << " event will be starting in ";
		break;
	case ET_DESTROY_SHIELD:
		msg << "The " << eventName[eType] << " will be starting in ";
		break;
	case ET_APOCALYPSE:
		msg << "The portal to " << eventName[eType] << " will be open in ";
		break;
	case ET_CRUSADE:
		msg << "The " << eventName[eType] << " will be starting in ";
		break;
	case ET_MONSTEREVENT: // Monster Kill Event xRisenx
		msg << "The " << eventName[eType] << " will be starting in ";
		break;
	default:
		msg << "An unknown event will be starting in ";
		break;
	}

	switch(msgID)
	{
	case NOTIFY_EVENTSTARTING:
		msg << "2 hours.";
		break;
	case NOTIFY_EVENTSTARTING2:
		msg << "30 minutes.";
		break;
	case NOTIFY_EVENTSTARTING3:
		msg << "10 minutes.";
		break;
	}

	SetTopMsg(msg.str().c_str(), 10);
}

void CGame::NotifyMsg_EventStart(char * pData)
{
	char *cp;
	EventType eType;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	eType = (EventType)*cp;

	for(int i=0; i < MAXSIDES; i++)
	{
		m_astoriaStats[i].deaths = 0;
		m_astoriaStats[i].kills = 0;
		m_astoriaShieldsHP[i] = 0;
	}
	m_bIsAstoriaMode = true;
	m_relicOwnedTime = 0;
	
	wsprintfA(G_cTxt, NOTIFYMSG_EVENTSTART_CTR, eventName[eType]);

	strcat(G_cTxt, NOTIFYMSG_EVENTSTART);
	SetTopMsg(G_cTxt, 10);
}

void CGame::NotifyMsg_Casualties(char * pData)
{
	uint16_t *wp;
	wp = (uint16_t *)(pData + INDEX2_MSGTYPE + 2);

	m_astoriaStats[ARESDEN].deaths = *wp;
	wp++;
	m_astoriaStats[ELVINE].deaths = *wp;
	wp++;
	m_astoriaStats[ISTRIA].deaths = *wp;
	wp++;
	m_astoriaStats[ARESDEN].kills = *wp;
	wp++;
	m_astoriaStats[ELVINE].kills = *wp;
	wp++;
	m_astoriaStats[ISTRIA].kills = *wp;

	m_bIsAstoriaMode = true;
}

void CGame::NotifyMsg_ShieldHP(char * pData) // Shield xRisenx
{	
	pData += 6;

	Side side = NEUTRAL;
	Pop(pData, (uint8_t&)side);

	uint32_t hp; // 16->32
	Pop(pData, (uint32_t&)hp); // 16->32

	m_astoriaShieldsHP[side] = hp;

	m_bIsAstoriaMode = true;
}

void CGame::NotifyMsg_WhisperMode(bool bActive, char * pData)
{
	char * cp;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(m_cWhisperName, sizeof(m_cWhisperName));
	memcpy(m_cWhisperName, cp, 10);
	if (bActive == true)
	{
		wsprintfA(G_cTxt, NOTIFYMSG_WHISPERMODE1, m_cWhisperName);
		if (m_pWhisperMsg[MAXWHISPERMSG - 1] != 0) {
			delete m_pWhisperMsg[MAXWHISPERMSG - 1];
			m_pWhisperMsg[MAXWHISPERMSG - 1] = 0;
		}
		for (int i = MAXWHISPERMSG - 2; i >= 0; i--) {
			m_pWhisperMsg[i+1] = m_pWhisperMsg[i];
			m_pWhisperMsg[i] = 0;
		}
		m_pWhisperMsg[0] = new class CMsg(0, m_cWhisperName, 0);
		m_cWhisperIndex = 0;
	}
	else wsprintfA(G_cTxt, NOTIFYMSG_WHISPERMODE2, m_cWhisperName);

	AddEventList(G_cTxt, 10);
}

void CGame::NotifyMsg_FriendOnGame(char * pData)
{
	char cName[12], * cp;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);

	for(int i = 0; i < m_iTotalFriends; i++)
		if(strcmp(cName,friendsList[i].friendName) == 0){
			friendsList[i].online = true;
			friendsList[i].updated = true;
		}
}

void CGame::DrawDialogBox_CrusadeJob()
{
	short sX, sY;

	CDialogBox & dlg = m_dialogBoxes[33];
	char onButton = dlg.OnButton();
	sX = dlg.m_X;
	sY = dlg.m_Y;
	//DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 0);

	switch (dlg.GetMode()) {
	case 1:
		PutAlignedString(sX +24, sX +246, sY +45+20, DRAWDIALOGBOX_CRUSADEJOB1);
		PutAlignedString(sX +24, sX +246, sY +60+20, DRAWDIALOGBOX_CRUSADEJOB2);
		PutAlignedString(sX +24, sX +246, sY +75+20, DRAWDIALOGBOX_CRUSADEJOB3);
		PutAlignedString(sX +24, sX +246, sY +90+20, DRAWDIALOGBOX_CRUSADEJOB4);

		if(m_side == ARESDEN || m_side == ELVINE)
		{
			if(m_iGuildRank != GUILDRANK_NONE && gldRankPerm[ m_iGuildRank ].crusadeCommander)
			{
//uncomment				ButtonString(dlg, 1, DRAWDIALOGBOX_CRUSADEJOB5);
			}
			
//uncomment			ButtonString(dlg, 2, DRAWDIALOGBOX_CRUSADEJOB7);

			if (m_iGuildRank != GUILDRANK_NONE)
			{
//uncomment				ButtonString(dlg, 3, DRAWDIALOGBOX_CRUSADEJOB9);
			}
		}

		PutAlignedString(sX +24, sX +246, sY +290 -40,   DRAWDIALOGBOX_CRUSADEJOB10);
		PutAlignedString(sX +24, sX +246, sY +305 -40,   DRAWDIALOGBOX_CRUSADEJOB17);

		if (onButton == 4)
			 PutString_SprFont(sX + 200, sY +296, "Help", 6,6,20);
		else PutString_SprFont(sX + 200, sY +296, "Help", 0, 0, 7);
		break;

	case 2:
		PutAlignedString(sX +24, sX +246, sY +90+20, DRAWDIALOGBOX_CRUSADEJOB18);
		switch (m_iCrusadeDuty) {
		case 1: PutAlignedString(sX +24, sX +246, sY+125, DRAWDIALOGBOX_CRUSADEJOB19); break;//"(Soldier)
		case 2: PutAlignedString(sX +24, sX +246, sY+125, DRAWDIALOGBOX_CRUSADEJOB20); break;//"(Constructor)
		case 3: PutAlignedString(sX +24, sX +246, sY+125, DRAWDIALOGBOX_CRUSADEJOB21); break;//"(Commander)
		}

		PutAlignedString(sX +24, sX +246, sY +145, DRAWDIALOGBOX_CRUSADEJOB22);
		if (onButton == 1)
			 PutAlignedString(sX +24, sX +246, sY+160, DRAWDIALOGBOX_CRUSADEJOB23, 255,255,255);
 		else PutAlignedString(sX +24, sX +246, sY+160, DRAWDIALOGBOX_CRUSADEJOB23, 4,0,50);

		PutAlignedString(sX +24, sX +246, sY +175, DRAWDIALOGBOX_CRUSADEJOB25);
		PutAlignedString(sX +24, sX +246, sY +190, DRAWDIALOGBOX_CRUSADEJOB26);

		if (onButton == 2)
			 DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 1);
		else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + RBTNPOSX, sY + BTNPOSY, 0);
		break;
	}
}

void CGame::_Draw_OnLogin(char *pAccount, char *pPassword, int msX, int msY, int iFrame)
{
	bool bFlag = true;
	uint64_t dwTime = unixtime();


	//DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 0,0,0, true);
	DrawVersion();
}

void CGame::ShowEventList(uint32_t dwTime)
{
	int i;

	//DIRECTX m_DDraw._GetBackBufferDC();
	for (i = 0; i < 6; i++)
	if ((dwTime - m_stEventHistory[i].dwTime) < 5000)
	{
		switch (m_stEventHistory[i].cColor) {
		case 0:
			PutFontString(font[FONT_TREBMS8PX], 10, 10 + i*15, m_stEventHistory[i].cTxt, video::SColor(255, 225,225,225), false, 1, true);
			break;
		case 1:
			PutFontString(font[FONT_TREBMS8PX], 10, 10 + i*15, m_stEventHistory[i].cTxt, video::SColor(255,130,255,130), false, 1, true);
			break;
		case 2:
			PutFontString(font[FONT_TREBMS8PX], 10, 10 + i*15, m_stEventHistory[i].cTxt, video::SColor(255,255,130,130), false, 1, true);
			break;
		case 3:
			PutFontString(font[FONT_TREBMS8PX], 10, 10 + i*15, m_stEventHistory[i].cTxt, video::SColor(255,130,130,255), false, 1, true);
			break;
		case 4:
			PutFontString(font[FONT_TREBMS8PX], 10, 10 + i*15, m_stEventHistory[i].cTxt, video::SColor(255,230, 230, 130), false, 1, true);
			break;
		case 10:
			PutFontString(font[FONT_TREBMS8PX], 10, 10 + i*15, m_stEventHistory[i].cTxt, video::SColor(255,180,255,180), false, 1, true);
			break;
		case 20:
			PutFontString(font[FONT_TREBMS8PX], 10, 10 + i*15, m_stEventHistory[i].cTxt, video::SColor(255,150,150,170), false, 1, true);
			break;
		//case 36: // GM Helper chat (Helper chatting in global)
		//	PutString(10, 10 + i*15, m_stEventHistory[i].cTxt,video::SColor(255,255,153,0), FALSE, 1, TRUE);
		//	break;
		//case 37: // Vip Player chat (Helper chatting in global)
		//	PutString(10, 10 + i*15, m_stEventHistory[i].cTxt,video::SColor(255,255,153,0), FALSE, 1, TRUE);
		//	break;
		//case 38: // Alpha Tester chat (Helper chatting in global)
		//	PutString(10, 10 + i*15, m_stEventHistory[i].cTxt,video::SColor(255,255,153,0), FALSE, 1, TRUE);
		//	break;
		}
	}

	for (i = 0; i < 6; i++)
	if ((dwTime - m_stEventHistory2[i].dwTime) < 5000)
	{
		switch (m_stEventHistory2[i].cColor) {
		case 0:
			PutFontString(font[FONT_TREBMS8PX], 10, 435 + i*15, m_stEventHistory2[i].cTxt, video::SColor(255,225,225,225), false, 1, true);
			break;
		case 1:
			PutFontString(font[FONT_TREBMS8PX], 10, 435 + i*15, m_stEventHistory2[i].cTxt, video::SColor(255,130,255,130), false, 1, true);
			break;
		case 2:
			PutFontString(font[FONT_TREBMS8PX], 10, 435 + i*15, m_stEventHistory2[i].cTxt, video::SColor(255,255,130,130), false, 1, true);
			break;
		case 3:
			PutFontString(font[FONT_TREBMS8PX], 10, 435 + i*15, m_stEventHistory2[i].cTxt, video::SColor(255,130,130,255), false, 1, true);
			break;
		case 4:
			PutFontString(font[FONT_TREBMS8PX], 10, 435 + i*15, m_stEventHistory2[i].cTxt, video::SColor(255,230, 230, 130), false, 1, true);
			break;
		case 10:
			PutFontString(font[FONT_TREBMS8PX], 10, 435 + i*15, m_stEventHistory2[i].cTxt, video::SColor(255,180,255,180), false, 1, true);
			break;
		case 20:
			PutFontString(font[FONT_TREBMS8PX], 10, /*322*/435 + i*15, m_stEventHistory2[i].cTxt, video::SColor(255,150,150,170), false, 1, true);
			break;
		//case 36: //besk: GM Helper chat (helper chatting in global)
		//	PutString(10, 435 + i*15, m_stEventHistory[i].cTxt,video::SColor(255,255,153,0), FALSE, 1, TRUE);
		//	break;
		//case 37: // Vip Player chat (helper chatting in global)
		//	PutString(10, 435 + i*15, m_stEventHistory[i].cTxt,video::SColor(255,255,153,0), FALSE, 1, TRUE);
		//	break;
		//case 38: //Alpha Tester chat (helper chatting in global)
		//	PutString(10, 435 + i*15, m_stEventHistory[i].cTxt,video::SColor(255,255,153,0), FALSE, 1, TRUE);
		//	break;
		}
	}
	if (m_bSkillUsingStatus	== true)
	{
		//PutString(280 -29, 280 -52, SHOW_EVENT_LIST1,video::SColor(255,235,235,235), FALSE, 1, TRUE);
		PutFontString(font[FONT_TREBMS8PX], 350, 330, SHOW_EVENT_LIST1, video::SColor(255,235,235,235), false, 1, true);//besk 800x600
	}
	//DIRECTX m_DDraw._ReleaseBackBufferDC();
}

void CGame::RequestTeleportAndWaitData()
{

	bSendCommand(MSGID_REQUEST_TELEPORT);
	ChangeGameMode(GAMEMODE_ONWAITINGINITDATA);
}

void CGame::InitDataResponseHandler(char * pData)
{
	lock_guard<std::mutex> lock(screenupdate);
	int * ip, i;
	short * sp, sX, sY;
	char  * cp, cMapFileName[32], cTxt[120], cPreCurLocation[12];
	bool  bIsObserverMode;
	HANDLE hFile;
	uint32_t  dwFileSize;

	ZeroMemory( cPreCurLocation, sizeof(cPreCurLocation) );
	m_bParalyze = false;

	m_pMapData->Init();

	m_sMonsterID = 0;
	m_dwMonsterEventTime = 0;
#ifdef TitleClient
	m_dwReqTitlesTime = m_dwCurTime + 30000;
#endif
	DisableDialogBox(7);
	DisableDialogBox(11);
	DisableDialogBox(13);
	DisableDialogBox(14);
	DisableDialogBox(16);
	DisableDialogBox(22);
	DisableDialogBox(20);
	DisableDialogBox(21);
	DisableDialogBox(23);
	DisableDialogBox(51); // Gail's diag

	m_bHackMoveBlocked = false; // Anti Hack xRisenx

	m_cCommand = OBJECTSTOP;
	//m_bCommandAvailable = TRUE;
	m_cCommandCount = 0;
	m_bIsGetPointingMode = false;
	m_iPointCommandType  = -1;
	m_iIlusionOwnerH = 0;
	m_cIlusionOwnerType = 0;
	m_bIsTeleportRequested = false;
	m_bIsConfusion = false;
	m_bSkillUsingStatus = false;

	m_bItemUsingStatus = false ;

	m_cRestartCount = -1;
	m_dwRestartCountTime = 0;

	m_relicX = m_relicY = 0;

	m_armorDisabled = false;
	m_shieldDisabled = false;

	for (i = 0; i < m_stPartyMember.size(); i++)
		m_stPartyMember[i]->sX = m_stPartyMember[i]->sY = 0;

	for (i = 0; i < MAXMAGICTYPE; i++)
		m_magicDisabled[i] = false;

	for (i = 0; i < MAXEFFECTS; i++)
	{	if (m_pEffectList[i] != 0) delete m_pEffectList[i];
		m_pEffectList[i] = 0;
	}

	for (i = 0; i < MAXWEATHEROBJECTS; i++)
	{	m_stWeatherObject[i].sX    = 0;
		m_stWeatherObject[i].sBX   = 0;
		m_stWeatherObject[i].sY    = 0;
		m_stWeatherObject[i].cStep = 0;
	}

	for (i = 0; i < MAXGUILDNAMES; i++)
	{	m_stGuildName[i].dwRefTime = 0;
		m_stGuildName[i].iGuildRank = -1;
		ZeroMemory(m_stGuildName[i].cCharName, sizeof(m_stGuildName[i].cCharName));
		ZeroMemory(m_stGuildName[i].cGuildName, sizeof(m_stGuildName[i].cGuildName));
	}

	for (i = 0; i < MAXCHATMSGS; i++) {
		if (m_pChatMsgList[i] != 0) delete m_pChatMsgList[i];
		m_pChatMsgList[i] = 0;
	}

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	// PlayerObjectID
	sp = (short *)cp;
	m_sPlayerObjectID = *sp;
	cp += 4;

	sp = (short *)cp;
	sX = *sp;
	cp += 2;

	sp = (short *)cp;
	sY = *sp;
	cp += 2;

	sp = (short *)cp;
	m_sPlayerType = *sp;
	cp += 2;

	sp = (short *)cp;
	m_sPlayerAppr1 = *sp;
	cp += 2;

	sp = (short *)cp;
	m_sPlayerAppr2 = *sp;
	cp += 2;

	sp = (short *)cp;
	m_sPlayerAppr3 = *sp;
	cp += 2;

	sp = (short *)cp;
	m_sPlayerAppr4 = *sp;
	cp += 2;

	ip = (int *)cp;
	m_iPlayerApprColor = *ip;
	cp += 4;

	sp = (short *)cp;
	m_sPlayerHeadApprValue = *sp; // Re-Coding Sprite xRisenx
	cp += 2;

	sp = (short *)cp;
	m_sPlayerBodyApprValue = *sp; // Re-Coding Sprite xRisenx
	cp += 2;

	sp = (short *)cp;
	m_sPlayerArmApprValue = *sp; // Re-Coding Sprite xRisenx
	cp += 2;

	sp = (short *)cp;
	m_sPlayerLegApprValue = *sp; // Re-Coding Sprite xRisenx
	cp += 2;

	// CLEROTH - BLACK FIX <- you mean my fix I made 13 years ago. tyvm
	ip = (int *)cp;
	m_iPlayerStatus = *ip;
	cp += 4;

	ZeroMemory(m_cMapName, sizeof(m_cMapName));
	ZeroMemory(m_cMapMessage, sizeof(m_cMapMessage));
	memcpy(m_cMapName, cp, 10);
	m_cMapIndex = GetOfficialMapName(m_cMapName, m_cMapMessage);
	if( m_cMapIndex < 0 )
	{
        m_dialogBoxes[9].sSizeX = -1;
		m_dialogBoxes[9].sSizeY = -1;
	}
    else
	{
        m_dialogBoxes[9].sSizeX = 128;
		m_dialogBoxes[9].sSizeY = 128;
	}
	cp += 10;

	strcpy( cPreCurLocation, m_cCurLocation );
	ZeroMemory(m_cCurLocation, sizeof(m_cCurLocation));
	memcpy(m_cCurLocation, cp, 10);
	cp += 10;

	G_cSpriteAlphaDegree = *cp;
	cp++;

	SetWeatherStatus((Weather)*cp);
	cp++;

	ip = (int *)cp;
	m_iContribution = *ip;
//	m_iContributionPrice = 0;
	cp += 4;
	bIsObserverMode = (bool)*cp;
	cp++;
	ip = (int *)cp;
//	m_iRating = *ip;
	cp += 4;
	ip = (int *)cp;
	m_iHP = *ip;
	cp += 4;
	ip = (int *)cp;
	m_iLucky = *ip;
	cp += 4;
    m_cDiscount = (char )*cp;
    cp++;


	ZeroMemory(cMapFileName, sizeof(cMapFileName));
	strcat(cMapFileName, "data\\mapdata\\");
	strcat(cMapFileName, m_cMapName);
	strcat(cMapFileName, ".amd");
	m_pMapData->OpenMapDataFile(cMapFileName);


//     if (GetWidth() == 800)
//     {
//         m_pMapData->m_sPivotX = sX - 19;
//         m_pMapData->m_sPivotY = sY - 17;
//     }
//     else if (GetWidth() == 1024)
//     {
//         m_pMapData->m_sPivotX = sX - 26;
//         m_pMapData->m_sPivotY = sY - 22;
//     }
//     else if (GetWidth() == 1280)
//     {
//         m_pMapData->m_sPivotX = sX - 35;
//         m_pMapData->m_sPivotY = sY - 29;
//     }
//     else if (GetWidth() == 1920)
//     {
//         m_pMapData->m_sPivotX = sX - 55;
//         m_pMapData->m_sPivotY = sY - 30;
//     }

	m_sPlayerX   = sX ;
	m_sPlayerY   = sY;

	m_cPlayerDir = 5;

	if (bIsObserverMode == false)
	{
        m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
							                  m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
											  m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue, // Re-Coding Sprite xRisenx
											  m_iPlayerStatus, m_cPlayerName,
											  OBJECTSTOP, 0, 0, 0);
	}

	//m_sViewDstX = m_sViewPointX = (sX+4+5)*32;
	//m_sViewDstY = m_sViewPointY = (sY+5+5)*32;
	//m_sViewDstX = m_sViewPointX = (sX+7)*32; // 800x600 Resolution xRisenx Center Char xRisenx
	//m_sViewDstY = m_sViewPointY = (sY+8)*32; // 800x600 Resolution xRisenx Center Char xRisenx

    //m_sViewDstX = m_sViewPointX = (sX ) * 32; // 800x600 Resolution xRisenx Center Char xRisenx
    //m_sViewDstY = m_sViewPointY = (sY ) * 32; // 800x600 Resolution xRisenx Center Char xRisenx

    m_sViewDstX = m_sViewPointX = (sX) * 32 - GetWidth()/2;// ((GetWidth() / 32 /*tiles across*/) / 2);
    m_sViewDstY = m_sViewPointY = (sY) * 32 - (GetHeight()-60)/2;// (((GetHeight() - 60) / 32 /*tiles across*/) / 2);

    viewdstxvar = (GetWidth() / 32) / 2;
    viewdstyvar = ((GetHeight()-60) / 32) / 2;

    m_sViewDstX = m_sViewPointX = (sX - (GetWidth() / 32) / 2) * 32;
    m_sViewDstY = m_sViewPointY = (sY - ((GetHeight() - 60) / 32) / 2) * 32;

    cout << "viewpoint " << m_sViewPointX << ":" << m_sViewPointY << endl;
	//_ReadMapData(sX + 4 + 5, sY + 5 + 5, cp);
	//_ReadMapData(sX + 7, sY + 8, cp); // 800x600 Resolution xRisenx Center Char xRisenx
    _ReadMapData(sX, sY, cp);
	//_ReadMapData(sX + 4 + 5 - 2, sY + 5 + 5 - 2, cp); // Maybe this insted ? xRisenx
	m_bIsRedrawPDBGS = true;
    // ------------------------------------------------------------------------+
	wsprintfA(cTxt, INITDATA_RESPONSE_HANDLER1, m_cMapMessage);
	AddEventList(cTxt, 10);

	/*if (  ( memcmp( m_cCurLocation, "middleland"	,10 ) == 0 )
		|| ( memcmp( m_cCurLocation, "dglv2"		, 5 ) == 0 )
		|| ( memcmp( m_cCurLocation, "middled1n"	, 9 ) == 0 ))
    	EnableDialogBox(6, NULL,NULL, NULL);*/

// Snoopy: removed for v351 compatibility. Maybe usefull later...
/*	bool bPrevSafe, bNowSafe;
	if( memcmp( cPreCurLocation, m_cLocation, 3 ) == 0 )
		bPrevSafe = TRUE;
	else bPrevSafe = FALSE;

	if( memcmp( m_cCurLocation, m_cLocation, 3 ) == 0 )
		bNowSafe = TRUE;
	else bNowSafe = FALSE;

	if( memcmp( m_cCurLocation, "2nd", 3 ) == 0 ) bNowSafe = TRUE;
	if( m_iPKCount != 0 ) bNowSafe = FALSE;

	if( bPrevSafe )
	{	if( bNowSafe == FALSE ) SetTopMsg(MSG_DANGERZONE, 5);
	}else
	{	if( bNowSafe ) SetTopMsg(MSG_SAFEZONE, 5);
	}*/

    // ------------------------------------------------------------------------+

	ChangeGameMode(GAMEMODE_ONMAINGAME);
	//DIRECTX m_DDraw.ClearBackB4();

	//v1.41
	if ((m_sPlayerAppr2 & 0xF000) != 0)
		 m_bIsCombatMode = true;
	else m_bIsCombatMode = false;

	//v1.42
	//if (m_bIsFirstConn == TRUE)
	//{	m_bIsFirstConn = FALSE;
	//	/*hFile = CreateFileA("contents\\contents1000.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	//	hFile = CreateFileA("contents\\contents1000.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);*/
	//	if (hFile == INVALID_HANDLE_VALUE)
	//		dwFileSize = 0;
	//	else
	//	{	dwFileSize = GetFileSize(hFile, NULL);
	//		CloseHandle(hFile);
	//	}
	//	bSendCommand(MSGID_REQUEST_NOTICEMENT, NULL, NULL, (int)dwFileSize, NULL, NULL, NULL);
	//}
	//cp += 2;

#ifdef TitleClient
	bSendCommand(MSGID_REQUEST_TITLES, 0, 0, 0, 0, 0, 0);
	m_cCommandCount--;
	m_dwReqTitlesTime = m_dwCurTime;
#endif
	// Gladiator Arena xRisenx
	// clear list if relog or log new char
	/*for (i = 0; i < MAXARENAPLAYERS; i++) 
	{ 
	   m_stArenaPlayers[i].iKills = 0; 
	   m_stArenaPlayers[i].iDeaths = 0; 
	   ZeroMemory(m_stArenaPlayers[i].cCharName, sizeof(m_stArenaPlayers[i].cCharName)); 
	}*/
	// Gladiator Arena xRisenx
}

void CGame::MotionEventHandler(char * pData)
{
	uint16_t  * wp, wObjectID;
	uint16_t * sp, sX, sY, sType, sAppr1, sAppr2, sAppr3, sAppr4, sSprite, sSpriteFrame, sV1, sV2, sV3, sPrevAppr2, sHeadApprValue, sBodyApprValue, sArmApprValue, sLegApprValue; // Re-Coding Sprite xRisenx
	UnitStatus status;
	Msgid eventType;
	uint8_t cDir, iLoc;
	char  * cp, cName[12];
	int   * ip, iApprColor;
	char    cTxt[300];
	int i;
	/*#ifdef RaiseCriticalLimit
	int iDamage = 0;
	#endif*/

	/*#ifdef RemoveCritical
	uint32_t * dwp, dwDamage;
	#endif*/

	ZeroMemory(cName, sizeof(cName));
	sV1 = sV2 = sV3 =
	/*#ifdef RemoveCritical
	dwDamage =
	#endif*/
	0;

	cp = pData;
	Pop(cp, eventType);
	Pop(cp, wObjectID);

	iLoc = 0;
	if (wObjectID < 30000)
	{	
		if (wObjectID < 10000) 	// Player
		{
			Pop(cp, sX);
			Pop(cp, sY);
			Pop(cp, sType);
			Pop(cp, cDir);
			Pop(cp, cName, 10);
			Pop(cp, sAppr1);
			Pop(cp, sAppr2);
			Pop(cp, sAppr3);
			Pop(cp, sAppr4);
			Pop(cp, (uint32_t&)iApprColor);
			Pop(cp, sHeadApprValue); // Re-Coding Sprite xRisenx
			Pop(cp, sBodyApprValue); // Re-Coding Sprite xRisenx
			Pop(cp, sArmApprValue); // Re-Coding Sprite xRisenx
			Pop(cp, sLegApprValue); // Re-Coding Sprite xRisenx
			Pop(cp, status);
			Pop(cp, iLoc);
		}else 	// Npc or mob
		{	
			sprintf(cName, "npc%d", wObjectID - 10000);
			Pop(cp, sX);
			Pop(cp, sY);
			Pop(cp, sType);
			Pop(cp, cDir);

			sAppr1 = sAppr3 = sAppr4 = sHeadApprValue = sBodyApprValue = sArmApprValue = sLegApprValue = 0; // Re-Coding Sprite xRisenx
			Pop(cp, sAppr2);
			Pop(cp, status);
			Pop(cp, iLoc);
		}
	}
	else
	{	
		switch (eventType) 
		{
		case MSGID_MOTION_MAGIC:
		#ifdef RaiseCriticalLimit
			Pop(cp, cDir);
			Pop(cp, (uint8_t&)sV1);
			Pop(cp, (uint8_t&)sV2);
			break;
		#endif
		case MSGID_MOTION_DAMAGEMOVE:
		case MSGID_MOTION_DAMAGE:
			Pop(cp, cDir);
			#ifdef RemoveCriticalClient
			Pop(cp, (uint16_t&)sV1);
			#else
			Pop(cp, (uint8_t&)sV1);
			#endif
			Pop(cp, (uint8_t&)sV2);
  			break;

		case MSGID_MOTION_DYING:
			Pop(cp, cDir);
			#ifdef RemoveCriticalClient
			Pop(cp, (uint16_t&)sV1);
			#else
			Pop(cp, (uint8_t&)sV1);
			#endif
			Pop(cp, (uint8_t&)sV2);
			Pop(cp, sX);
			Pop(cp, sY);
			break;

		case MSGID_MOTION_ATTACK:
			Pop(cp, cDir);
			#ifdef RemoveCriticalClient
			Pop(cp, (uint16_t&)sV1);
			#else
			Pop(cp, (uint8_t&)sV1);
			#endif
			Pop(cp, (uint8_t&)sV2);
			Pop(cp, sV3);
			break;

		default:
			Pop(cp, cDir);
			break;
	
		}	
	}

	if(eventType == MSGID_MOTION_EVENT_CONFIRM)
	{
		m_pMapData->bSetOwner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor,
			 sHeadApprValue, sBodyApprValue, sArmApprValue, sLegApprValue, status, cName, OBJECTSTOP, 0, 0, 0); // Re-Coding Sprite xRisenx
		switch (sType) {
		case 43: // LWB
		case 44: // GHK
		case 45: // GHKABS
		case 46: // TK
		case 47: // BG
			bAddNewEffect(64, (sX)*32 ,(sY)*32, 0, 0, 0);
			break;
		}

		_RemoveChatMsgListByObjectID(wObjectID);

		return;
	} 
	else if(eventType == MSGID_MOTION_EVENT_REJECT)
	{
		m_pMapData->bSetOwner(wObjectID, -1, -1, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, 
			iApprColor, sHeadApprValue, sBodyApprValue, sArmApprValue, sLegApprValue, status, cName, OBJECTSTOP, 0, 0, 0); // Re-Coding Sprite xRisenx

		_RemoveChatMsgListByObjectID(wObjectID);

		return;
	}


	if(eventType == MSGID_MOTION_NULL && memcmp(cName, m_cPlayerName, 10) == 0)
	{	
		m_sPlayerType   = sType;
		m_sPlayerAppr1  = sAppr1;
		sPrevAppr2      = m_sPlayerAppr2;
		m_sPlayerAppr2  = sAppr2;
		m_sPlayerAppr3  = sAppr3;
		m_sPlayerAppr4  = sAppr4;
		m_iPlayerApprColor = iApprColor;
		m_sPlayerHeadApprValue = sHeadApprValue; // Re-Coding Sprite xRisenx
		m_sPlayerBodyApprValue = sBodyApprValue; // Re-Coding Sprite xRisenx
		m_sPlayerArmApprValue  = sArmApprValue; // Re-Coding Sprite xRisenx
		m_sPlayerLegApprValue  = sLegApprValue; // Re-Coding Sprite xRisenx
		m_iPlayerStatus    = status;
		if ((sPrevAppr2 & 0xF000) == 0)
		{	
			if ((sAppr2 & 0xF000) != 0)
			{	
				AddEventList(MOTION_EVENT_HANDLER1, 10);
				m_bIsCombatMode = true;
			}
		}else
		{	
			if ((sAppr2 & 0xF000) == 0)
			{	
				AddEventList(MOTION_EVENT_HANDLER2, 10);
				m_bIsCombatMode = false;
			}	
		}

		if (m_cCommand != OBJECTRUN && m_cCommand != OBJECTMAGIC) 
		{
			m_pMapData->bSetOwner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4,
				iApprColor, sHeadApprValue, sBodyApprValue, sArmApprValue, sLegApprValue, status, cName, GET_MOTION_ACTIONID(eventType), sV1, sV2, sV3, iLoc); // Re-Coding Sprite xRisenx
		}
	}
	else if(!(wObjectID -30000 == m_sPlayerObjectID && 
		eventType == MSGID_MOTION_DYING && (m_cCommand == OBJECTRUN || m_cCommand == OBJECTMOVE)))
	{
		m_pMapData->bSetOwner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, 
			iApprColor, sHeadApprValue, sBodyApprValue, sArmApprValue, sLegApprValue, status, cName, GET_MOTION_ACTIONID(eventType), sV1, sV2, sV3, iLoc); // Re-Coding Sprite xRisenx
	}

	switch (eventType)
	{
	case MSGID_MOTION_MAGIC: // Casting
		_RemoveChatMsgListByObjectID(wObjectID - 30000);
// 		if (abs(sX - m_sPlayerX > 12) || abs(sX + m_sPlayerX) > 12 || abs(sY - m_sPlayerY) > 9 || abs(sY + m_sPlayerY) > 9)
// 		return;
		for (i = 1; i < MAXCHATMSGS; i++)
		if (m_pChatMsgList[i] == 0)
		{	ZeroMemory(cTxt, sizeof(cTxt));
		#ifdef RemoveCritical
			if (m_pMagicCfgList[dwDamage] != 0)
			wsprintfA(cTxt, "%s!", m_pMagicCfgList[dwDamage]->m_cName);
		#else
			wsprintfA(cTxt, "%s!", m_pMagicCfgList[sV1]->m_cName);
		#endif
			m_pChatMsgList[i] = new class CMsg(41, cTxt, m_dwCurTime);
			m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;
			if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == false)
			{	delete m_pChatMsgList[i];
				m_pChatMsgList[i] = 0;
			}
			return;
		}
		break;

	case MSGID_MOTION_DYING:
		/*#ifdef RaiseCriticalLimit
		for (i = 1; i < MAXCHATMSGS; i++)
        if (m_pChatMsgList[i] == NULL)
        {    ZeroMemory(cTxt, sizeof(cTxt));
            wsprintfA(cTxt, "-%dPts!", iDamage);
            int iFontType;
            if ((iDamage >= 0) && (iDamage < 12))        iFontType = 21;
            else if ((iDamage >= 12) && (iDamage < 40)) iFontType = 22;
            else if ((iDamage >= 40) || (iDamage < 0))    iFontType = 23;
            m_pChatMsgList[i] = new class CMsg(iFontType, cTxt, m_dwCurTime);
            m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;
            if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == FALSE)
            {    delete m_pChatMsgList[i];
                m_pChatMsgList[i] = NULL;
            }
            return;*/
		//#else
		//#ifdef RemoveCritical
		//_RemoveChatMsgListByObjectID(wObjectID - 30000);
		//for (i = 1; i < MAXCHATMSGS; i++)
		//if (m_pChatMsgList[i] == NULL)
		//{	ZeroMemory(cTxt, sizeof(cTxt));
		//	if (dwDamage > 0)
		//		wsprintfA(cTxt, "-%dPts!", dwDamage); //pts
		//	else strcpy(cTxt, "Critical!");
		//	int iFontType;
		//	if ((dwDamage >= 0) && (dwDamage < 500))		iFontType = 21;
		//	else if ((dwDamage >= 500) && (dwDamage < 1250)) iFontType = 22;
		//	else if ((dwDamage >= 1250) || (dwDamage < 0))	iFontType = 23;
		//	m_pChatMsgList[i] = new class CMsg(iFontType, cTxt, m_dwCurTime);
		//	m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;
		//	if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == FALSE)
		//	{	delete m_pChatMsgList[i];
		//		m_pChatMsgList[i] = NULL;
		//	}
		//	return;
		//#else
// 		if (abs(sX - m_sPlayerX > 12) || abs(sX + m_sPlayerX) > 12 || abs(sY - m_sPlayerY) > 9 || abs(sY + m_sPlayerY) > 9)
// 		return;
		//do it here to save on processing
		for (i = 1; i < MAXCHATMSGS; i++)
		{
			if (m_pChatMsgList[i] == 0)
			{
				int index = m_pMapData->getChatMsgIndex(wObjectID - 30000);
				if(m_showAllDmg && m_pChatMsgList[index] && strlen(m_pChatMsgList[index]->m_pMsg) < sizeof(cTxt)-30)
				{
					if(index != -1 && m_dwCurTime - m_pChatMsgList[index]->m_dwTime < 150 &&
						m_pChatMsgList[index]->m_cType >= 21 && m_pChatMsgList[index]->m_cType <= 23)
					{
						if (sV1 > 0)
							wsprintfA(cTxt, "%s-%d!", m_pChatMsgList[index]->m_pMsg, sV1);
						else
							wsprintfA(cTxt, "%s-Crit!", m_pChatMsgList[index]->m_pMsg, sV1);
					}
					else
					{
						if (sV1 > 0)
							wsprintfA(cTxt, "-%d!", sV1);
						else strcpy(cTxt, "Crit!");
					}
				}
				else
				{
					if (sV1 > 0)
						wsprintfA(cTxt, "-%d!", sV1);
					else strcpy(cTxt, COMMAND_PROCESSOR6);
				}
				int iFontType;
				if ((sV1 >= 0) && (sV1 < 500))		iFontType = 21;
				else if ((sV1 >= 500) && (sV1 < 1250)) iFontType = 22;
				else if ((sV1 >= 1250) || (sV1 < 0))	iFontType = 23;
				_RemoveChatMsgListByObjectID(wObjectID - 30000);
				m_pChatMsgList[i] = new class CMsg(iFontType, cTxt, m_dwCurTime);
				m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;
				if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == false)
				{
					delete m_pChatMsgList[i];
					m_pChatMsgList[i] = 0;
				}
			

				return;
			}
				//#endif
		}
		break;

	case MSGID_MOTION_DAMAGEMOVE:
	case MSGID_MOTION_DAMAGE:
		if (memcmp(cName, m_cPlayerName, 10) == 0)
		{
			m_bIsGetPointingMode = false;
			m_iPointCommandType	 = -1;
			m_stMCursor.sCursorFrame = 0;
			ClearSkillUsingStatus();
		}
		//#ifdef RemoveCritical
		//for (i = 1; i < MAXCHATMSGS; i++)
		//if (m_pChatMsgList[i] == NULL)
		//{	ZeroMemory(cTxt, sizeof(cTxt));
		//	if (dwDamage != 0)
		//	{	
		//		if (dwDamage >= 99999)
		//			wsprintfA(cTxt, "-%dPts", dwDamage-99999); //pts
		//		else if (dwDamage == 50000)
		//			wsprintfA(cTxt, "Broken!"); //pts
		//		else if (dwDamage > 0)
		//			wsprintfA(cTxt, "-%dPts", dwDamage); //pts
		//		else strcpy(cTxt, "Critical!");//
		//		int iFontType;
		//		if ((dwDamage >= 0) && (dwDamage < 500))		iFontType = 21;
		//		else if ((dwDamage >= 500) && (dwDamage < 1250)) iFontType = 22;
		//		else if ((dwDamage >= 99999) || (dwDamage == 50000)) iFontType = 25;
		//		else if ((dwDamage >= 1250) || (dwDamage < 0))	iFontType = 23;

		//		m_pChatMsgList[i] = new class CMsg(iFontType, cTxt, m_dwCurTime);
		//	}else
		//	{	strcpy(cTxt, " * Failed! *");
		//		m_pChatMsgList[i] = new class CMsg(22, cTxt, m_dwCurTime);
		//		PlaySound('C', 17, 0);
		//	}
		//	m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;
		//	if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == FALSE)
		//	{	delete m_pChatMsgList[i];
		//		m_pChatMsgList[i] = NULL;
		//	}
		//	return;
		//#else
// 		if (abs(sX - m_sPlayerX) > 12 || abs(sX + m_sPlayerX) > 12 || abs(sY - m_sPlayerY) > 9 || abs(sY + m_sPlayerY) > 9)
// 		return;
		for (i = 1; i < MAXCHATMSGS; i++)
		if (m_pChatMsgList[i] == 0)
		{
			ZeroMemory(cTxt, sizeof(cTxt));
			if (sV1 != 0)
			{
				int index = m_pMapData->getChatMsgIndex(wObjectID - 30000);
				if(index != -1 && m_showAllDmg && m_pChatMsgList[index] && strlen(m_pChatMsgList[index]->m_pMsg) < sizeof(cTxt)-30)
				{
					if(index != -1 && m_dwCurTime - m_pChatMsgList[index]->m_dwTime < 150 &&
						m_pChatMsgList[index]->m_cType >= 21 && m_pChatMsgList[index]->m_cType <= 23)
					{
						if (sV1 > 0)
							wsprintfA(cTxt, "%s-%d", m_pChatMsgList[index]->m_pMsg, sV1);
						else
							wsprintfA(cTxt, "%s-Crit", m_pChatMsgList[index]->m_pMsg, sV1);
					}
					else
					{
						if (sV1 > 0)
							wsprintfA(cTxt, "-%d", sV1);
						else strcpy(cTxt, "Crit");
					}
				}else{
					if (sV1 > 0)
						wsprintfA(cTxt, "-%d", sV1);
					else strcpy(cTxt, COMMAND_PROCESSOR6);
				}

				int iFontType;
				if ((sV1 >= 0) && (sV1 < 500))		iFontType = 21;
				else if ((sV1 >= 500) && (sV1 < 1250)) iFontType = 22;
				else if ((sV1 >= 1250) || (sV1 < 0))	iFontType = 23;

				_RemoveChatMsgListByObjectID(wObjectID - 30000);
				m_pChatMsgList[i] = new class CMsg(iFontType, cTxt, m_dwCurTime);
				}else{
				strcpy(cTxt, " * Failed! *");
				_RemoveChatMsgListByObjectID(wObjectID - 30000);
				m_pChatMsgList[i] = new class CMsg(22, cTxt, m_dwCurTime);
				PlaySound('C', 17, 0);
			}
			m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;
			if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == false)
			{
				delete m_pChatMsgList[i];
				m_pChatMsgList[i] = 0;
			}
			return;
		//#endif
		}
		break;
	}
}


void CGame::GrandMagicResult(char *pMapName, int iV1, int iV2, int iV3, int iV4, int iHP1, int iHP2, int iHP3, int iHP4)
{int i, iTxtIdx=0;
 char cTemp[120];

	for (i = 0; i < TEXTDLGMAXLINES; i++)
	{	if (m_pMsgTextList[i] != 0)
			delete m_pMsgTextList[i];
		m_pMsgTextList[i] = 0;
	}

	if (strcmp(pMapName, "aresden") == 0)
	{	m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE2, 0);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE3, 0);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, " ", 0);

		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintfA(cTemp, "%s %d", CRUSADE_MESSAGE4, iV1);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, cTemp, 0);

		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintfA(cTemp, "%s %d", CRUSADE_MESSAGE5, iV2);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, cTemp, 0);

		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintfA(cTemp, "%s %d", CRUSADE_MESSAGE6, iV3);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, cTemp, 0);

		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintfA(cTemp, "%s %d", CRUSADE_MESSAGE58, iV4);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, cTemp, 0);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, " ", 0);

		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintfA(cTemp, "%s %d %d %d %d", NOTIFY_MSG_STRUCTURE_HP, iHP1,iHP2,iHP3,iHP4);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, cTemp, 0);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, " ", 0);

		if (iV2 == 0) {
			PlaySound('E', 25, 0, 0);
			if(m_side == ELVINE)	{
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE59, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE60, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE61, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE62, 0);
				for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
			}
			else if (m_side == ARESDEN) {
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE69, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE70, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE71, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE72, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE73, 0);
				for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
			}
		}else
		{	if (iV1 != 0)
			{	if(m_side == ELVINE)
				{	PlaySound('E', 23, 0, 0);
					PlaySound('C', 21, 0, 0);
					PlaySound('C', 22, 0, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE63, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE64, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE65, 0);
					for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
				}else if (m_side == ARESDEN)
				{	PlaySound('E', 24, 0, 0);
					PlaySound('C', 12, 0, 0);
					PlaySound('C', 13, 0, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE74, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE75, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE76, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE77, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE78, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE79, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE80, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE81, 0);
					for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
				}else PlaySound('E', 25, 0, 0);
			}else
			{	if(m_side == ELVINE)
				{	PlaySound('E', 23, 0, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE66, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE67, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE68, 0);
					for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
				}else if (m_side == ARESDEN)
				{	PlaySound('E', 24, 0, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE82, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE83, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE84, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE85, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE86, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE87, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE88, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE89, 0);
					for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
				}else PlaySound('E', 25, 0, 0);
		}	}
	}else if (strcmp(pMapName, "elvine") == 0)
	{	m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE7, 0);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE8, 0);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, " ", 0);

		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintfA(cTemp, "%s %d", CRUSADE_MESSAGE4, iV1);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, cTemp, 0);

		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintfA(cTemp, "%s %d", CRUSADE_MESSAGE5, iV2);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, cTemp, 0);

		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintfA(cTemp, "%s %d", CRUSADE_MESSAGE6, iV3);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, cTemp, 0);

		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintfA(cTemp, "%s %d", CRUSADE_MESSAGE58, iV4);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, cTemp, 0);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, " ", 0);

		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintfA(cTemp, "%s %d %d %d %d", NOTIFY_MSG_STRUCTURE_HP, iHP1,iHP2,iHP3,iHP4);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, cTemp, 0);
		m_pMsgTextList[iTxtIdx++] = new class CMsg(0, " ", 0);

		if (iV2 == 0) {
			if (m_side == ARESDEN)
			{	PlaySound('E', 25, 0, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE59, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE60, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE61, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE62, 0);
				for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
			}else if (m_side == ELVINE)
			{	PlaySound('E', 25, 0, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE69, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE70, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE71, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE72, 0);
				m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE73, 0);
				for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
			}else PlaySound('E', 25, 0, 0);
		}else
		{	if (iV1 != 0) {
				if (m_side == ARESDEN)
				{	PlaySound('E', 23, 0, 0);
					PlaySound('C', 21, 0, 0);
					PlaySound('C', 22, 0, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE63, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE64, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE65, 0);
					for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
				}else if (m_side == ELVINE)
				{	PlaySound('E', 24, 0, 0);
					PlaySound('C', 12, 0, 0);
					PlaySound('C', 13, 0, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE74, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE75, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE76, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE77, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE78, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE79, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE80, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE81, 0);
					for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
				}else PlaySound('E', 25, 0, 0);
			}else
			{	if ( (true) && (m_side == ARESDEN) )
				{	PlaySound('E', 23, 0, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE66, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE67, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE68, 0);
					for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
				}
				else if ( (true) && (m_side == ELVINE) )
				{	PlaySound('E', 24, 0, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE82, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE83, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE84, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE85, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE86, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE87, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE88, 0);
					m_pMsgTextList[iTxtIdx++] = new class CMsg(0, CRUSADE_MESSAGE89, 0);
					for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new class CMsg(0, " ", 0);
				}else PlaySound('E', 25, 0, 0);
	}	}	}

	EnableDialogBox(18, 0, 0, 0);
}

void CGame::ItemDrop_Mailbox()
{
	if(m_dialogBoxes[DIALOG_MAILBOX].GetMode() != 1 &&
		m_dialogBoxes[DIALOG_MAILBOX].GetMode() != 3)
	{
		return;
	}

	uint32_t itemi = m_stMCursor.sSelectedObjectID;

	if (m_cCommand < 0 || !m_pItemList[itemi] || m_bIsItemDisabled[itemi]) 
		return;

	if(m_bIsDialogEnabled[DIALOG_QUERYDROPITEMAMOUNT] || m_bIsDialogEnabled[DIALOG_SELLORREPAIRITEM] || 
		m_bIsDialogEnabled[DIALOG_ITEMDROP] ||
		 (m_bIsDialogEnabled[DIALOG_NPCACTIONQUERY] && 
		 (m_dialogBoxes[DIALOG_NPCACTIONQUERY].GetMode() == 1 || m_dialogBoxes[DIALOG_NPCACTIONQUERY].GetMode() == 2))
		)
	{
		AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}

	if((m_pItemList[itemi]->m_cItemType == ITEMTYPE_CONSUME || 
		m_pItemList[itemi]->m_cItemType == ITEMTYPE_ARROW) && 
		m_pItemList[itemi]->m_dwCount > 1)
	{
		m_dialogBoxes[17].m_X  = G_pGame->m_stMCursor.sX - 140;
		m_dialogBoxes[17].m_Y  = G_pGame->m_stMCursor.sY - 70;
		if (m_dialogBoxes[17].m_Y < 0) m_dialogBoxes[17].m_Y = 0;

		m_dialogBoxes[17].sV1 = m_sPlayerX+1;
		m_dialogBoxes[17].sV2 = m_sPlayerY+1;
		m_dialogBoxes[17].sV3 = 1004;// NPC
		m_dialogBoxes[17].sV4 = itemi;

		ZeroMemory(m_dialogBoxes[17].cStr, sizeof(m_dialogBoxes[17].cStr));
		EnableDialogBox(17, itemi, m_pItemList[itemi]->m_dwCount, 0);
	}else
	{
		if (m_dialogBoxes[DIALOG_MAILBOX].vvec.size() < MAX_MAIL_ITEMS) 
		{
			m_bIsItemDisabled[itemi] = true;

			MailItemSend item;
			item.index = itemi;
			item.count = 1;
			m_dialogBoxes[DIALOG_MAILBOX].vvec.push_back( item );
		}
		else {
			AddEventList(DLGBOX_CLICK_NPCACTION_QUERY10, 10);
		}
	}
}

LONG CGame::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);
    if (retval == ERROR_SUCCESS)
	{
		long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, 0, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }
    return retval;
}

void CGame::GoHomepage(bool _web)
{
// 	wchar_t * url;
// 	if (_web)
// 	url = MSG_HOMEPAGE;
// 	else url = MSG_HOMEPAGE2; // Added Forum button xRisenx
// 	int		showcmd = SW_SHOW;
// 	wchar_t	key[MAX_PATH + MAX_PATH];
// 	SendMessage( G_hWnd, WM_ACTIVATEAPP, 0, 0 );
//     // First try ShellExecute()
//     HINSTANCE result = ShellExecuteA(NULL, "open", url, NULL,NULL, showcmd);
// 
//     // If it failed, get the .htm regkey and lookup the program
//     if ((UINT)result <= HINSTANCE_ERROR)
// 	{
// 		if (GetRegKey(HKEY_CLASSES_ROOT, L".htm", key) == ERROR_SUCCESS)
// 		{
// 			lstrcatW(key, L"\\shell\\open\\command");
// 
//             if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS)
// 			{
// 				wchar_t *pos;
// 				pos = wcsstr(key, L"\"%1\"");
// 				if (pos == NULL)					// No quotes found
// 				{
// 					pos = wcsstr(key, L"%1");			// Check for %1, without quotes
//                     if (pos == NULL)				// No parameter at all...
//                         pos = key+lstrlen(key)-1;
//                     else *pos = '\0';				// Remove the parameter
//                 }
// 				else    *pos = '\0';				// Remove the parameter
//                 lstrcatW(pos, L" ");
//                 lstrcatW(pos, url);
//                 result = (HINSTANCE) WinExec(key,showcmd);
// 			}
// 		}
// 	}
}

// num : 1 - F2, 2 - F3
void CGame::UseShortCut( int num )
{int index;
	if( num < 3 ) index = num+1;
	else index = num+7;
	if(m_cGameMode != GAMEMODE_ONMAINGAME) return;
	if (m_bCtrlPressed == true)
	{	if (m_sRecentShortCut == -1)
		{	AddEventList(MSG_SHORTCUT1, 10);
			wsprintfA( G_cTxt, MSG_SHORTCUT2, index );// [F%d]
			AddEventList(G_cTxt, 10);
			wsprintfA( G_cTxt, MSG_SHORTCUT3, index );// [Control]-[F%d]
			AddEventList(G_cTxt, 10);
		}else
		{	m_sShortCut[num] = m_sRecentShortCut;
			if (m_sShortCut[num] < 100)
			{	if (m_pItemList[m_sShortCut[num]] == 0)
				{	m_sShortCut[num] = -1;
					m_sRecentShortCut = -1;
					return;
				}
				char cStr1[64], cStr2[64], cStr3[64];
				ZeroMemory(cStr1, sizeof(cStr1));
				ZeroMemory(cStr2, sizeof(cStr2));
				ZeroMemory(cStr3, sizeof(cStr3));

				GetItemName(m_pItemList[m_sShortCut[num]], cStr1, cStr2, cStr3);
				wsprintfA(G_cTxt, MSG_SHORTCUT4, cStr1, cStr2, cStr3, index);// (%s %s %s) [F%d]
				AddEventList(G_cTxt, 10);
			}else if (m_sShortCut[num] >= 100)
			{	if (m_pMagicCfgList[m_sShortCut[num]-100] == 0)
				{	m_sShortCut[num] = -1;
					m_sRecentShortCut = -1;
					return;
				}
				wsprintfA(G_cTxt, MSG_SHORTCUT5, m_pMagicCfgList[m_sShortCut[num]-100]->m_cName, index);// %s) [F%d]
				AddEventList(G_cTxt, 10);
		}	}
	}else
	{	if (m_sShortCut[num] == -1)
		{	AddEventList(MSG_SHORTCUT1, 10);
			wsprintfA( G_cTxt, MSG_SHORTCUT2, index );// [F%d]
			AddEventList(G_cTxt, 10);
			wsprintfA( G_cTxt, MSG_SHORTCUT3, index );// [Control]-[F%d]
			AddEventList(G_cTxt, 10);
		}else if (m_sShortCut[num] < 100)
		{	ItemEquipHandler((char)m_sShortCut[num]);
		}
		else if (m_sShortCut[num] >= 100) UseMagic(m_sShortCut[num]-100);
	}
}

int CGame::iGetManaCost(int iMagicNo)
{	int i, iManaSave, iManaCost;
	iManaSave = 0;
	if( iMagicNo < 0 || iMagicNo >= 100 ) return 1;
	for( i=0; i<MAXITEMS ; i++ )
	{	if( m_pItemList[i] == 0 ) continue;
		if( m_bIsItemEquipped[i] == true )
		{	if(      strcmp( m_pItemList[i]->m_cName, "MagicWand(MS10)" ) == 0)		iManaSave += 10;
			else if( strcmp( m_pItemList[i]->m_cName, "MagicWand(MS20)" ) == 0)		iManaSave += 20;
			else if( strcmp( m_pItemList[i]->m_cName, "MagicWand(MS30-LLF)" ) == 0) iManaSave += 30;
			else if( strcmp( m_pItemList[i]->m_cName, "WizMagicWand(MS10)" ) == 0)	iManaSave += 10;
			else if( strcmp( m_pItemList[i]->m_cName, "WizMagicWand(MS20)" ) == 0)	iManaSave += 20;
			else if( strcmp( m_pItemList[i]->m_cName, "MagicNecklace(MS15)" ) == 0) iManaSave += 15;
			else if( strcmp( m_pItemList[i]->m_cName, "MagicNecklace(MS18)" ) == 0) iManaSave += 18;
			else if( strcmp( m_pItemList[i]->m_cName, "MagicNecklace(MS20)" ) == 0) iManaSave += 20;
			else if( strcmp( m_pItemList[i]->m_cName, "MagicNecklace(MS22)" ) == 0) iManaSave += 22;
			else if( strcmp( m_pItemList[i]->m_cName, "MagicNecklace(MS25)" ) == 0) iManaSave += 25;
			else if( strcmp( m_pItemList[i]->m_cName, "MagicNecklace(MS30)" ) == 0) iManaSave += 30;
			else if( strcmp( m_pItemList[i]->m_cName, "DMMagicStaff" ) == 0)		iManaSave += 22;
			else if( strcmp( m_pItemList[i]->m_cName, "DMMagicWand" ) == 0)			iManaSave += 25;
			else if( strcmp( m_pItemList[i]->m_cName, "BlackMageTemple" ) == 0)		iManaSave += 28;
			else if( strcmp( m_pItemList[i]->m_cName, "NecklaceOfLiche") ==0)   	iManaSave += 35;
			else if( strcmp( m_pItemList[i]->m_cName, "BerserkWand(MS.20)" ) == 0)	iManaSave += 20;
			else if( strcmp( m_pItemList[i]->m_cName, "BerserkWand(MS.10)" ) == 0)	iManaSave += 10;
			else if( strcmp( m_pItemList[i]->m_cName, "KlonessWand(MS.20)" ) == 0)	iManaSave += 20;
			else if( strcmp( m_pItemList[i]->m_cName, "KlonessWand(MS.10)" ) == 0)	iManaSave += 10;
			else if( strcmp( m_pItemList[i]->m_cName, "DameWand(MS10)" ) == 0)		iManaSave += 10; // Magic Save Ratio xRisenx
			else if( strcmp( m_pItemList[i]->m_cName, "MasterWand(MS20)" ) == 0)	iManaSave += 20; // Magic Save Ratio xRisenx
			else if( strcmp( m_pItemList[i]->m_cName, "LegendWand(MS25)" ) == 0)	iManaSave += 25; // Magic Save Ratio xRisenx
			else if( strcmp( m_pItemList[i]->m_cName, "WandOfWitch" ) == 0)			iManaSave += 15; // Magic Save Ratio xRisenx
			else if( strcmp( m_pItemList[i]->m_cName, "WandOfGWitch" ) == 0)		iManaSave += 10; // Magic Save Ratio xRisenx
			else if( strcmp( m_pItemList[i]->m_cName, "WandOfAncWitch" ) == 0)		iManaSave += 5; // Magic Save Ratio xRisenx

	}	}
	iManaCost = m_pMagicCfgList[iMagicNo]->m_sValue1;
	if (m_bIsSafeAttackMode) iManaCost *= 1.1;
	if (iManaSave > 0)
	{	double dV1 = (double)iManaSave;
		double dV2 = (double)(dV1 / 100.0f);
		double dV3 = (double)iManaCost;
		dV1 = dV2 * dV3;
		dV2 = dV3 - dV1;
		iManaCost = (int)dV2;
	}
	if( iManaCost < 1 ) iManaCost = 1;
	return iManaCost;
}

void CGame::UseMagic(int iMagicNo)
{
	char cTemp[100];

	if( iMagicNo < 0 || iMagicNo >= MAXMAGICTYPE ) return;
	if ((m_cMagicMastery[iMagicNo] == 0) || (m_pMagicCfgList[iMagicNo] == 0)) return;

	// Casting
	if (m_iHP <= 0) return;
	if (m_bIsGetPointingMode == true) return;
	if (iGetManaCost(iMagicNo) > m_iMP) return;
	if (_bIsItemOnHand() == true)
	{	AddEventList(DLGBOX_CLICK_MAGIC1, 10);
		return;
	}
	if (m_bSkillUsingStatus == true)
	{	AddEventList(DLGBOX_CLICK_MAGIC2, 10);
		return;
	}
	if (m_magicDisabled[iMagicNo]){
		wsprintfA(cTemp, NOTIFYMSG_SPELLDISABLED, m_pMagicCfgList[iMagicNo]->m_cName);
		AddEventList(cTemp, 10);
		return;
	}
	if ((m_sPlayerAppr2 & 0xF000) == 0) bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);
	m_cCommand = OBJECTMAGIC;
	m_iCastingMagicType = iMagicNo;
	m_sMagicShortCut    = iMagicNo;
	m_sRecentShortCut   = iMagicNo + 100;
	m_iPointCommandType = iMagicNo + 100;
	//m_bIsGetPointingMode = TRUE;
	DisableDialogBox(3);
}


void CGame::ReleaseEquipHandler(char cEquipPos)
{	char cStr1[64], cStr2[64], cStr3[64];
	if( m_sItemEquipmentStatus[cEquipPos] < 0 ) return;
	// Remove Angelic Stats
	if (   (cEquipPos >= 11)
		&& (m_pItemList[m_sItemEquipmentStatus[cEquipPos]]->m_cItemType == 1))
	{	char cItemID = m_sItemEquipmentStatus[cEquipPos];
		if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(STR)", 19) == 0)
		{	m_angelStat[STAT_STR] = 0;
		}else if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(DEX)", 19) == 0)
		{	m_angelStat[STAT_DEX] = 0;
		}else if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(INT)", 19) == 0)
		{	m_angelStat[STAT_INT] = 0;
		}else if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(MAG)", 19) == 0)
		{	m_angelStat[STAT_MAG] = 0;
	}	}

	GetItemName(m_pItemList[m_sItemEquipmentStatus[cEquipPos]], cStr1, cStr2, cStr3);
	wsprintfA(G_cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
	AddEventList(G_cTxt, 10);
	m_bIsItemEquipped[m_sItemEquipmentStatus[cEquipPos]] = false;
	m_sItemEquipmentStatus[cEquipPos] = -1;
}

void CGame::ItemEquipHandler(char cItemID)
{	if (bCheckItemOperationEnabled(cItemID) == false) return;
	if (m_bIsItemEquipped[cItemID] == true) return;
	if (m_pItemList[cItemID]->m_cEquipPos == EQUIPPOS_NONE)
	{	AddEventList(BITEMDROP_CHARACTER3, 10);//"The item is not available."
		return;
	}
	if ( m_pItemList[cItemID]->m_wCurLifeSpan == 0 )
	{	AddEventList(BITEMDROP_CHARACTER1, 10); //"The item is exhausted. Fix it to use it."
		return;
	}
	if (m_pItemList[cItemID]->m_wWeight / 100 > m_stat[STAT_STR])
	{	AddEventList(BITEMDROP_CHARACTER2, 10);
		return;
	}
	if (((m_pItemList[cItemID]->m_dwAttribute & 0x00000001) == 0) && (m_pItemList[cItemID]->m_sLevelLimit > m_iLevel)   )
	{	AddEventList(BITEMDROP_CHARACTER4, 10);
		return;
	}
	if (m_bSkillUsingStatus == true)
	{	AddEventList(BITEMDROP_CHARACTER5, 10);
		return;
	}
	if (m_pItemList[cItemID]->m_cGenderLimit != 0)
	{	switch (m_sPlayerType) {
		case 1:
		case 2:
		case 3:
			if (m_pItemList[cItemID]->m_cGenderLimit != 1)
			{	AddEventList(BITEMDROP_CHARACTER6, 10);
				return;
			}
			break;
		case 4:
		case 5:
		case 6:
			if (m_pItemList[cItemID]->m_cGenderLimit != 2)
			{	AddEventList(BITEMDROP_CHARACTER7, 10);
				return;
			}
			break;
	}	}
	if (m_armorDisabled &&
		(m_pItemList[cItemID]->m_cEquipPos == EQUIPPOS_HEAD ||
		m_pItemList[cItemID]->m_cEquipPos == EQUIPPOS_ARMS ||
		m_pItemList[cItemID]->m_cEquipPos == EQUIPPOS_BODY ||
		m_pItemList[cItemID]->m_cEquipPos == EQUIPPOS_PANTS))
	{
		AddEventList(NOTIFYMSG_ARMORDISABLED, 10);//"Use of armors have been disabled."
		return;
	}
	if (m_shieldDisabled && m_pItemList[cItemID]->m_cEquipPos == EQUIPPOS_LHAND)
	{
		AddEventList(NOTIFYMSG_SHIELDDISABLED, 10);//"Shields have been disabled."
		return;
	}

	bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_EQUIPITEM, 0, cItemID, 0, 0, 0);
	m_sRecentShortCut = cItemID;
	ReleaseEquipHandler(m_pItemList[cItemID]->m_cEquipPos);
	switch( m_pItemList[cItemID]->m_cEquipPos )	{
	case EQUIPPOS_HEAD:
	case EQUIPPOS_BODY:
	case EQUIPPOS_ARMS:
	case EQUIPPOS_PANTS:
	case EQUIPPOS_BOOTS:
	case EQUIPPOS_BACK:
		ReleaseEquipHandler(EQUIPPOS_FULLBODY);
		break;
	case EQUIPPOS_FULLBODY:
		ReleaseEquipHandler(EQUIPPOS_HEAD);
		ReleaseEquipHandler(EQUIPPOS_BODY);
		ReleaseEquipHandler(EQUIPPOS_ARMS);
		ReleaseEquipHandler(EQUIPPOS_PANTS);
		ReleaseEquipHandler(EQUIPPOS_BOOTS);
		ReleaseEquipHandler(EQUIPPOS_BACK);
		break;
	case EQUIPPOS_LHAND:
	case EQUIPPOS_RHAND:
		ReleaseEquipHandler(EQUIPPOS_TWOHAND);
		break;
	case EQUIPPOS_TWOHAND:
		ReleaseEquipHandler(EQUIPPOS_RHAND);
		ReleaseEquipHandler(EQUIPPOS_LHAND);
		break;
	}

	m_sItemEquipmentStatus[ m_pItemList[cItemID]->m_cEquipPos ] = cItemID;
	m_bIsItemEquipped[cItemID] = true;

	// Add Angelic Stats
	if (   (m_pItemList[cItemID]->m_cItemType == 1)
		&& (m_pItemList[cItemID]->m_cEquipPos >= 11))
	{	int iAngelValue = 0;
		if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(STR)", 19) == 0)
		{	iAngelValue = (m_pItemList[cItemID]->m_dwAttribute & 0xF0000000) >> 28;
			//m_angelStat[STAT_STR] = 1 + iAngelValue; // Angel Stats xRisenx
			m_angelStat[STAT_STR] = 12 * iAngelValue; // Angel Stats xRisenx
		}else if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(DEX)", 19) == 0)
		{	iAngelValue = (m_pItemList[cItemID]->m_dwAttribute & 0xF0000000) >> 28;
			//m_angelStat[STAT_DEX] = 1 + iAngelValue; // Angel Stats xRisenx
			m_angelStat[STAT_DEX] = 12 * iAngelValue; // Angel Stats xRisenx
		}else if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(INT)", 19) == 0)
		{	iAngelValue = (m_pItemList[cItemID]->m_dwAttribute & 0xF0000000) >> 28;
			//m_angelStat[STAT_INT] = 1 + iAngelValue; // Angel Stats xRisenx
			m_angelStat[STAT_INT] = 12 * iAngelValue; // Angel Stats xRisenx
		}else if(memcmp(m_pItemList[cItemID]->m_cName, "AngelicPendant(MAG)", 19) == 0)
		{	iAngelValue = (m_pItemList[cItemID]->m_dwAttribute & 0xF0000000) >> 28;
			//m_angelStat[STAT_MAG] = 1 + iAngelValue; // Angel Stats xRisenx
			m_angelStat[STAT_MAG] = 12 * iAngelValue; // Angel Stats xRisenx
	}	}

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[cItemID], cStr1, cStr2, cStr3);
	wsprintfA(G_cTxt, BITEMDROP_CHARACTER9, cStr1);
	AddEventList(G_cTxt, 10);
	PlaySound('E', 28, 0);
}

/*********************************************************************************************************************
**  void CheckActiveAura(short sX, short sY, uint32_t dwTime, short sOwnerType)( initially Cleroth fixed by Snoopy )	**
**  description			: Generates special auras around players													**
**						: v351 implements this in each drawn function,beter to regroup in single function.			**
**********************************************************************************************************************/
void CGame::CheckActiveAura(short sX, short sY, uint32_t dwTime, short sOwnerType)
{	// Used at the beginning of character drawing
	// DefenseShield
	if ((_tmp_iStatus & STATUS_DEFENSESHIELD) != 0)
		//m_pEffectSpr[80]->PutTransSprite(sX+75, sY+107, _tmp_iEffectFrame%17, dwTime);
		m_pEffectSpr[80]->PutTransSprite50(sX+75, sY+107, _tmp_iEffectFrame%17, dwTime);

	// Protection From Magic
	if ((_tmp_iStatus & STATUS_PFM) != 0)
		//m_pEffectSpr[79]->PutTransSprite(sX+101, sY+135, _tmp_iEffectFrame%15, dwTime);
		m_pEffectSpr[79]->PutTransSprite70(sX+101, sY+135, _tmp_iEffectFrame%15, dwTime);

	// Protection From Arrow
	if ((_tmp_iStatus & STATUS_PFA) != 0)
		//m_pEffectSpr[72]->PutTransSprite(sX, sY+35, _tmp_iEffectFrame%30, dwTime);
		m_pEffectSpr[72]->PutTransSprite70(sX, sY+35, _tmp_iEffectFrame%30, dwTime);

	// Illusion
	if ((_tmp_iStatus & STATUS_ILLUSION) != 0)
		//m_pEffectSpr[73]->PutTransSprite(sX+125, sY+95, _tmp_iEffectFrame%24, dwTime);
		m_pEffectSpr[73]->PutTransSprite70(sX+125, sY+130 - _iAttackerHeight[sOwnerType], _tmp_iEffectFrame%24, dwTime);

	// Illusion movement
	if ((_tmp_iStatus & STATUS_ILLUSIONMOVEMENT) != 0)
		//m_pEffectSpr[151]->PutTransSprite(sX+90, sY+55, _tmp_iEffectFrame%24, dwTime);
		m_pEffectSpr[151]->PutTransSprite70(sX+90, sY+90 - _iAttackerHeight[sOwnerType], _tmp_iEffectFrame%24, dwTime);

	// Slate red  (HP)  Flame au sol
	if ((_tmp_iStatus & STATUS_REDSLATE) != 0)
		//m_pEffectSpr[149]->PutTransSprite(sX+90, sY+120, _tmp_iEffectFrame%15, dwTime);
		m_pEffectSpr[149]->PutTransSprite70(sX+90, sY+120, _tmp_iEffectFrame%15, dwTime);

	// Slate Blue (Mana) Bleu au sol
	if ((_tmp_iStatus & STATUS_BLUESLATE) != 0)
		//m_pEffectSpr[150]->PutTransSprite(sX+1, sY+26, _tmp_iEffectFrame%15, dwTime);
		m_pEffectSpr[150]->PutTransSprite70(sX+1, sY+26, _tmp_iEffectFrame%15, dwTime);

	// Slate Green (XP) Mauve au sol
	if ((_tmp_iStatus & STATUS_GREENSLATE) != 0)
		//m_pEffectSpr[148]->PutTransSprite(sX, sY+32, _tmp_iEffectFrame%23, dwTime);
		m_pEffectSpr[148]->PutTransSprite70(sX, sY+32, _tmp_iEffectFrame%23, dwTime);

	// the ZZZ's
	if ((_tmp_iStatus & STATUS_AFK) != 0)
		m_pEffectSpr[85]->PutTransSprite70(sX+50, sY+30, _tmp_iEffectFrame%29, dwTime);

	if (_tmp_iStatus & STATUS_RELICHOLDER)
	{
		int framesNum = m_pEffectSpr[26]->m_iTotalFrame -4;
		m_pEffectSpr[26]->PutTransSprite(sX, sY,
			4 + _tmp_iEffectFrame % framesNum
			, dwTime);
		m_pEffectSpr[26]->PutTransSprite70(sX, sY,
			4 + (_tmp_iEffectFrame + framesNum/2) % framesNum
			, dwTime);
// 		m_pEffectSpr[150]->PutTransSprite50(sX+1, sY+26, (_tmp_iEffectFrame+7)%15, dwTime);
// 		m_pEffectSpr[148]->PutTransSprite50(sX, sY+32, (_tmp_iEffectFrame+15)%23, dwTime);
	}
}

/*********************************************************************************************************************
**  void CheckActiveAura2(short sX, short sY, uint32_t dwTime,  _tmp_sOwnerType) ( initially Cleroth fixed by Snoopy )	**
**  description			: Generates poison aura around players. This one should be use later...						**
**						: v351 implements this in each drawn function,beter to regroup in single function.			**
**********************************************************************************************************************/
void CGame::CheckActiveAura2(short sX, short sY, uint32_t dwTime, short sOwnerType)
{	// Poison
	if ((_tmp_iStatus & STATUS_POISON) != 0)
		//m_pEffectSpr[81]->PutTransSprite(sX+115, sY+85, _tmp_iEffectFrame%21, dwTime);
		m_pEffectSpr[81]->PutTransSprite70(sX+115, sY+120 - _iAttackerHeight[sOwnerType], _tmp_iEffectFrame%21, dwTime);
//	_iAttackerHeight[]

	if ((m_bIsHeldenianMode) && (m_bIsHeldenianMap))
	{
		if ((_tmp_iStatus & 0x00400000) != 0)			// BloodyMantle
			m_pEffectSpr[107]->PutTransSprite70(sX-5, sY, _tmp_iEffectFrame%15, dwTime);
		else if ((_tmp_iStatus & 0x00800000) != 0)		// Magic Mantle
			m_pEffectSpr[114]->PutTransSprite70(sX-5, sY, _tmp_iEffectFrame%15, dwTime);
		else if ((_tmp_iStatus & 0x00010000) != 0)		// ArrowMantle
			m_pEffectSpr[115]->PutTransSprite70(sX-5, sY, _tmp_iEffectFrame%15, dwTime);
		else if ((_tmp_iStatus & 0x00020000) != 0)
			m_pEffectSpr[87]->PutTransSprite70(sX+53, sY+54, _tmp_iEffectFrame%29, dwTime);
	}
}

void CGame::DrawAngel(int iSprite, short sX, short sY, char cFrame, uint32_t dwTime)
{	if  ((_tmp_iStatus & 0x1000) != 0) m_pSprite[SPRID_TUTELARYANGELS_PIVOTPOINT+iSprite]->PutSpriteFast(sX, sY, cFrame, dwTime);  //AngelicPendant(STR)
	else if ((_tmp_iStatus & 0x2000) != 0) m_pSprite[SPRID_TUTELARYANGELS_PIVOTPOINT+(50*1)+iSprite]->PutSpriteFast(sX, sY, cFrame, dwTime); //AngelicPendant(DEX)
	else if ((_tmp_iStatus & 0x4000) != 0) m_pSprite[SPRID_TUTELARYANGELS_PIVOTPOINT+(50*2)+iSprite]->PutSpriteFast(sX, sY-20, cFrame, dwTime);//AngelicPendant(INT)
	else if ((_tmp_iStatus & 0x8000) != 0) m_pSprite[SPRID_TUTELARYANGELS_PIVOTPOINT+(50*3)+iSprite]->PutSpriteFast(sX, sY-20, cFrame, dwTime);//AngelicPendant(MAG)

}
/*********************************************************************************************************************
**  int CGame::bHasHeroSet( short m_sAppr3, short m_sAppr3, char OwnerType)		( Snoopy )							**
**  description			:: check weather the object (is character) is using a hero set (1:war, 2:mage)				**
**********************************************************************************************************************/
//int CGame::bHasHeroSet(short Appr3, short Appr4, char OwnerType)
int CGame::bHasHeroSet(short HeadApprValue, short BodyApprValue, short ArmApprValue, short LegApprValue, char OwnerType)
{	char cArmor, cLeg, cBerk, cHat;
	cArmor = BodyApprValue;
	cLeg   = LegApprValue;
	cHat   = HeadApprValue;
	cBerk  = ArmApprValue;
	//cArmor = (Appr3 & 0xF000) >> 12;
	//cLeg   = (Appr3 & 0x0F00) >> 8;
	//cHat   = (Appr3 & 0x00F0) >> 4;
	//cBerk  =  Appr3 & 0x000F;
	switch (OwnerType){
	case 1:
	case 2:
	case 3:
		if ((cArmor == 8) &&(cLeg == 5)&&(cHat == 9) &&(cBerk == 3)) return (1); // Warr elv M
		if ((cArmor == 9) &&(cLeg == 6)&&(cHat == 10)&&(cBerk == 4)) return (1); // Warr ares M
		if ((cArmor == 12)&&(cLeg == 7)&&(cHat == 13)&&(cBerk == 5)) return (1); // Ancient War M
		if ((cArmor == 10)&&(cLeg == 5)&&(cHat == 11)&&(cBerk == 3)) return (2); // Mage elv M
		if ((cArmor == 11)&&(cLeg == 6)&&(cHat == 12)&&(cBerk == 4)) return (2); // Mage ares M
		if ((cArmor == 13)&&(cLeg == 7)&&(cHat == 14)&&(cBerk == 5)) return (2); // Ancient Mage M
		break;
	case 4:
	case 5:
	case 6: // fixed
		if ((cArmor == 9) &&(cLeg == 6)&&(cHat == 9) &&(cBerk == 4)) return (1); //warr elv W
		if ((cArmor == 10)&&(cLeg == 7)&&(cHat == 10)&&(cBerk == 5)) return (1); //warr ares W
		if ((cArmor == 13)&&(cLeg == 8)&&(cHat == 13)&&(cBerk == 6)) return (1); // Ancient War W
		if ((cArmor == 11)&&(cLeg == 6)&&(cHat == 11)&&(cBerk == 4)) return (2); //mage elv W
		if ((cArmor == 12)&&(cLeg == 7)&&(cHat == 12)&&(cBerk == 5)) return (2); //mage ares W
		if ((cArmor == 14)&&(cLeg == 8)&&(cHat == 14)&&(cBerk == 6)) return (2); // Ancient Mage W
		break;
	}
	return 0;
}
/*********************************************************************************************************************
**  void ShowHeldenianVictory( short sSide)				( Snoopy )													**
**  description			: Shows the Heldenian's End window															**
**********************************************************************************************************************/
void CGame::ShowHeldenianVictory(short sSide)
{	int i;
	DisableDialogBox(18);
	for (i = 0; i < TEXTDLGMAXLINES; i++)
	{	if (m_pMsgTextList[i] != 0)
			delete m_pMsgTextList[i];
		m_pMsgTextList[i] = 0;
	}

	switch (m_side) {
	case 0:
		PlaySound('E', 25, 0, 0);
		m_pMsgTextList[0] = new class CMsg(0, "Heldenian holy war has been closed!", 0);
		m_pMsgTextList[1] = new class CMsg(0, " ", 0);
		m_pMsgTextList[2] = new class CMsg(0, "Heldenian Holy war ended", 0);
		m_pMsgTextList[3] = new class CMsg(0, "in a tie.", 0);
		break;
	case 1:
		PlaySound('E', 25, 0, 0);
		m_pMsgTextList[0] = new class CMsg(0, "Heldenian holy war has been closed!", 0);
		m_pMsgTextList[1] = new class CMsg(0, " ", 0);
		m_pMsgTextList[2] = new class CMsg(0, "Heldenian Holy war ended", 0);
		m_pMsgTextList[3] = new class CMsg(0, "in favor of Aresden.", 0);
		break;
	case 2:
		PlaySound('E', 25, 0, 0);
		m_pMsgTextList[0] = new class CMsg(0, "Heldenian holy war has been closed!", 0);
		m_pMsgTextList[1] = new class CMsg(0, " ", 0);
		m_pMsgTextList[2] = new class CMsg(0, "Heldenian Holy war ended", 0);
		m_pMsgTextList[3] = new class CMsg(0, "in favor of Elvine.", 0);
		break;
	}
	m_pMsgTextList[4] = new class CMsg(0, " ", 0);

	if (   ((m_side != 1) && (m_side != 2))   // Player not a normal citizen
		|| (sSide == 0))								// or no winner
	{	PlaySound('E', 25, 0, 0);
		m_pMsgTextList[5] = new class CMsg(0, " ", 0);
		m_pMsgTextList[6] = new class CMsg(0, " ", 0);
		m_pMsgTextList[7] = new class CMsg(0, " ", 0);
		m_pMsgTextList[8] = new class CMsg(0, " ", 0);
	}else
	{ 	if (sSide == m_side)
		{	PlaySound('E', 23, 0, 0);
			PlaySound('C', 21, 0, 0);
			PlaySound('C', 22, 0, 0);
			m_pMsgTextList[5] = new class CMsg(0, "Congratulations.", 0);
			m_pMsgTextList[6] = new class CMsg(0, "As a victorious citizen,", 0);
			m_pMsgTextList[7] = new class CMsg(0, "You will receive a reward.", 0);
			m_pMsgTextList[8] = new class CMsg(0, "      ", 0);
		}else
		{	PlaySound('E', 24, 0, 0);
			PlaySound('C', 12, 0, 0);
			PlaySound('C', 13, 0, 0);
			m_pMsgTextList[5] = new class CMsg(0, "To our regret", 0);
			m_pMsgTextList[6] = new class CMsg(0, "As a defeated citizen,", 0);
			m_pMsgTextList[7] = new class CMsg(0, "You won't receive any reward.", 0);
			m_pMsgTextList[8] = new class CMsg(0, "     ", 0);
	}	}
	for (i = 9; i < 18; i++)
		m_pMsgTextList[i] = new class CMsg(0, " ", 0);
	EnableDialogBox(18, 0, 0, 0);
	DisableDialogBox(36);
	DisableDialogBox(37);
	DisableDialogBox(38);
}

/*********************************************************************************************************************
**  void 	ResponseHeldenianTeleportList(char *pData)									(  Snoopy )					**
**  description			: Gail's TP																					**
**********************************************************************************************************************/
void CGame::ResponseHeldenianTeleportList(char *pData)
{	char *cp;
	int  *ip, i;
#ifdef _DEBUG
	AddEventList("Teleport ???", 10);
#endif
	cp = pData + 6;
	ip = (int*) cp;
	m_iTeleportMapCount = *ip;
	cp += 4;
	for ( i = 0 ; i < m_iTeleportMapCount ; i++)
	{	ip = (int*)cp;
		m_stTeleportList[i].iIndex = *ip;
		cp += 4;
		ZeroMemory(m_stTeleportList[i].mapname, sizeof(m_stTeleportList[i].mapname) );
		memcpy(m_stTeleportList[i].mapname, cp, 10);
		cp += 10;
		ip = (int*)cp;
		m_stTeleportList[i].iX = *ip;
		cp += 4;
		ip = (int*)cp;
		m_stTeleportList[i].iY = *ip;
		cp += 4;
		ip = (int*)cp;
		m_stTeleportList[i].iCost = *ip;
		cp += 4;
	}
}

/*********************************************************************************************************************
**  bool DKGlare(int iWeaponIndex, int iWeaponIndex, int *iWeaponGlare)	( Snoopy )									**
**  description			: test glowing condition for DK set															**
**********************************************************************************************************************/
void CGame::DKGlare(int iWeaponColor, int iWeaponIndex, int *iWeaponGlare)
{	if (iWeaponColor != 9) return;
	if (   ((iWeaponIndex >= SPRID_WEAPON_M + 64*14)&&(iWeaponIndex < SPRID_WEAPON_M + 64*14 + 56)) //msw3
		|| ((iWeaponIndex >= SPRID_WEAPON_W + 64*14)&&(iWeaponIndex < SPRID_WEAPON_W + 64*14 + 56))) //wsw3
	{	*iWeaponGlare = 3;
	}else if (  ((iWeaponIndex >= SPRID_WEAPON_M + 64*37)&&(iWeaponIndex < SPRID_WEAPON_M + 64*37 + 56)) //MStaff3
		|| ((iWeaponIndex >= SPRID_WEAPON_W + 64*37)&&(iWeaponIndex < SPRID_WEAPON_W + 64*37 + 56)))//WStaff3
	{	*iWeaponGlare = 2;
	}
}
/*********************************************************************************************************************
**  void DrawDruncncity()	( Snoopy )																				**
**  description			: 	Placeholder for Druncncity effects (nota: bubbles already coded)						**
**********************************************************************************************************************/
void CGame::DrawDruncncity()
{	//DebugLog("druncncity");
}

/*********************************************************************************************************************
**  void CGame::Abaddon_corpse(int sX, int sY);		( Snoopy )														**
**  description			: Placeholder for abaddon's death lightnings												**
**********************************************************************************************************************/
void CGame::Abaddon_corpse(int sX, int sY)
{	int ir = (rand()%20)-10;
	_DrawThunderEffect(sX+30 , 0, sX+30 , sY-10 , ir, ir, 1);
	_DrawThunderEffect(sX+30 , 0, sX+30 , sY-10 , ir+2, ir, 2);
	_DrawThunderEffect(sX+30 , 0, sX+30 , sY-10 , ir-2, ir, 2);
	ir = (rand()%20)-10;
	_DrawThunderEffect(sX-20 , 0, sX-20 , sY-35 , ir, ir, 1);
	_DrawThunderEffect(sX-20 , 0, sX-20 , sY-35 , ir+2, ir, 2);
	_DrawThunderEffect(sX-20 , 0, sX-20 , sY-35 , ir-2, ir, 2);
	ir = (rand()%20)-10;
	_DrawThunderEffect(sX-10 , 0, sX-10 , sY+30	, ir, ir, 1);
	_DrawThunderEffect(sX-10 , 0, sX-10 , sY+30	, ir+2, ir+2, 2);
	_DrawThunderEffect(sX-10 , 0, sX-10 , sY+30	, ir-2, ir+2, 2);
	ir = (rand()%20)-10;
	_DrawThunderEffect(sX+50 , 0, sX+50 , sY+35	, ir, ir, 1);
	_DrawThunderEffect(sX+50 , 0, sX+50 , sY+35	, ir+2, ir+2, 2);
	_DrawThunderEffect(sX+50 , 0, sX+50 , sY+35	, ir-2, ir+2, 2);
	ir = (rand()%20)-10;
	_DrawThunderEffect(sX+65 , 0, sX+65 , sY-5	, ir, ir, 1);
	_DrawThunderEffect(sX+65 , 0, sX+65 , sY-5	, ir+2, ir+2, 2);
	_DrawThunderEffect(sX+65 , 0, sX+65 , sY-5	, ir-2, ir+2, 2);
	ir = (rand()%20)-10;
	_DrawThunderEffect(sX+45 , 0, sX+45 , sY-50	, ir, ir, 1);
	_DrawThunderEffect(sX+45 , 0, sX+45 , sY-50	, ir+2, ir+2, 2);
	_DrawThunderEffect(sX+45 , 0, sX+45 , sY-50	, ir-2, ir+2, 2);

	for (int x=sX-50; x<=sX+100; x+= rand()%35)
	for (int y=sY-30; y<=sY+50; y+= rand()%45)
	{	ir = (rand()%20)-10;
		_DrawThunderEffect(x , 0, x , y, ir, ir, 2);
	}
}

/*********************************************************************************************************************
**  void DebugLog(char * cStr)			( Snoopy )																	**
**  description			: writes data into "Debug.txt"																**
**********************************************************************************************************************/
void CGame::DebugLog(char * cStr)
{FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	pFile = fopen("Debug.txt", "at");
	if (pFile == 0) return;
	ZeroMemory(cBuffer, sizeof(cBuffer));
	GetLocalTime(&SysTime);
	wsprintfA(cBuffer, "(%4d:%2d:%2d_%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute,SysTime.wSecond);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");
	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void CGame::DrawQuestHelper()
{
	char cTemp[21], cTxt[120];
	if (m_stQuest.sQuestType != 0) {
		PutString2(530+160, 130, QUESTHELPER, 255,200,0); // Fixed Quest Details xRisenx

		if (m_stQuest.bIsQuestCompleted) {
			PutString2(530+160, 154, QUESTHELPERCOMPLETE, 255, 0, 0); // Fixed Quest Details xRisenx
		} else {
			if (m_stQuest.sX != 0) {
				ZeroMemory(cTxt, sizeof(cTxt));
				wsprintfA(cTxt, "%d,%d Range: %d", m_stQuest.sX, m_stQuest.sY, m_stQuest.sRange);
				PutString2(520+160, 142, cTxt,  55,255,255); // Fixed Quest Details xRisenx
			} else {
				ZeroMemory(cTemp, sizeof(cTemp));
				GetNpcName(m_stQuest.sTargetType, cTemp);
				ZeroMemory(cTxt, sizeof(cTxt));
				wsprintfA(cTxt, "%s: %d/%d", cTemp, ( m_stQuest.sTargetCount - m_stQuest.sCurrentCount), m_stQuest.sTargetCount);
				PutString2(530+160, 142, cTxt,  55,255,255); // Fixed Quest Details xRisenx
			}
		}
	}
}

void CGame::SaveFriendList()
{
	char cFn[256]/*, cTemp[255]*/;
	ZeroMemory(cFn,sizeof(cFn));
	if(strlen(m_cPlayerName) > 0) wsprintfA(cFn,"data\\shops\\friends\\%s.txt", m_cPlayerName);
	FILE* f=fopen(cFn,"w");
	int i;
	char wr[12];
	_mkdir("data\\shops\\friends");
	//_mkdir(cFn);
	for (i=0;i<m_iTotalFriends;i++) {
		ZeroMemory(wr,sizeof(wr));
		wsprintfA(wr,"%s\n",friendsList[i].friendName);
		fputs(wr,f);
	}
	fclose(f);
}
void CGame::LoadFriendList()
{
	char buf[1024];
	int konieclinii;
	unsigned long linie=0;
	//new load by char name
	char cFn[256]/*, cTemp[255]*/;

	ZeroMemory(cFn,sizeof(cFn));
	if(strlen(m_cPlayerName) > 0) wsprintfA(cFn,"data\\shops\\friends\\%s.txt", m_cPlayerName);
	FILE* f=fopen(cFn,"rt");
	m_iTotalFriends=0;
	if(f == 0) {
		_mkdir("data\\shops\\friends");
		return;
	}
	while(fgets(buf,1024,f)){
		konieclinii=0;
		int i=strlen(buf);
		if(i>0 && buf[--i]=='\n'){
			buf[i]=0;
			konieclinii=1;
			linie++;
		}
		if (linie-1<13) {
			strcpy(friendsList[linie-1].friendName,buf);
			m_iTotalFriends++;
		}
	}
	fclose(f);
}

void CGame::UpdateFriendsStatus()
{
	if((G_dwGlobalTime - lastFriendUpdate) < 4000) return;
	lastFriendUpdate = G_dwGlobalTime;
	char namesBuf[255];
	ZeroMemory(namesBuf, sizeof(namesBuf));
	for(int i = 0; i < m_iTotalFriends; i++){
			friendsList[i].updated = false;
			strcat(namesBuf, friendsList[i].friendName);
			strcat(namesBuf, " ");
	}
	bSendCommand(MSGID_REQUEST_UPDATE_FRIENDS,0,0,0,0,0,namesBuf);
}


void CGame::LoadMuteList()
{
	char buffer[64];
	char fileLocation[64];

	m_MuteList.clear();
	ZeroMemory(fileLocation,sizeof(fileLocation));
	if(strlen(m_cPlayerName) > 0) wsprintfA(fileLocation,"data\\shops\\mutes\\%s.txt", m_cPlayerName);
	FILE* f = fopen(fileLocation,"rt");
	if(f == 0) {
		_mkdir("data\\shops\\mutes");
		return;
	}
	while(fgets(buffer,64,f)){
		int i = strlen(buffer);
		if(i > 0 && buffer[--i]=='\n') {
			buffer[i] = 0;
			m_MuteList.insert(string(buffer));
		}
	}
	fclose(f);
}

void CGame::SaveMuteList()
{
	char fileLocation[64];

	ZeroMemory(fileLocation,sizeof(fileLocation));
	if(strlen(m_cPlayerName) > 0) wsprintfA(fileLocation,"data\\shops\\mutes\\%s.txt", m_cPlayerName);
	FILE* f = fopen(fileLocation,"w");
	_mkdir("data\\shops\\mutes");

	for(std::set<string>::iterator it = m_MuteList.begin(); it != m_MuteList.end(); ++it){
		fputs((*it).c_str(),f);
		fputs("\n",f);
	}
	fclose(f);
}

void CGame::bItemDrop_GuildBank()
{
	m_dialogBoxes[39].sV1 = m_stMCursor.sSelectedObjectID;

	if (m_cCommand < 0 || !m_pItemList[m_dialogBoxes[39].sV1] || m_bIsItemDisabled[m_dialogBoxes[39].sV1]) 
		return;

	if(m_bIsDialogEnabled[DIALOG_QUERYDROPITEMAMOUNT] || m_bIsDialogEnabled[DIALOG_SELLORREPAIRITEM] || 
		m_bIsDialogEnabled[DIALOG_ITEMDROP] ||
		 (m_bIsDialogEnabled[DIALOG_NPCACTIONQUERY] && 
		 (m_dialogBoxes[DIALOG_NPCACTIONQUERY].GetMode() == 1 || m_dialogBoxes[DIALOG_NPCACTIONQUERY].GetMode() == 2))
		)
	{
		AddEventList(BITEMDROP_SKILLDIALOG1, 10);
		return;
	}

	if((m_pItemList[m_dialogBoxes[39].sV1]->m_cItemType == ITEMTYPE_CONSUME || 
		m_pItemList[m_dialogBoxes[39].sV1]->m_cItemType == ITEMTYPE_ARROW) && 
		m_pItemList[m_dialogBoxes[39].sV1]->m_dwCount > 1)
	{
		m_dialogBoxes[17].m_X  = G_pGame->m_stMCursor.sX - 140;
		m_dialogBoxes[17].m_Y  = G_pGame->m_stMCursor.sY - 70;
		if (m_dialogBoxes[17].m_Y < 0) m_dialogBoxes[17].m_Y = 0;

		m_dialogBoxes[17].sV1 = m_sPlayerX+1;
		m_dialogBoxes[17].sV2 = m_sPlayerY+1;
		m_dialogBoxes[17].sV3 = 1003;// NPC
		m_dialogBoxes[17].sV4 = m_dialogBoxes[39].sV1;

		ZeroMemory(m_dialogBoxes[17].cStr, sizeof(m_dialogBoxes[17].cStr));
		EnableDialogBox(17, m_dialogBoxes[39].sV1, m_pItemList[m_dialogBoxes[39].sV1]->m_dwCount, 0);
	}else
	{
		if (m_guildBankMap.size() < maxGWHItems[ m_guild.m_upgrades[GU_WAREHOUSE] ])
			bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, m_dialogBoxes[39].sV1, 1, m_dialogBoxes[39].sV5, m_dialogBoxes[39].sV6, m_pItemList[m_dialogBoxes[39].sV1]->m_cName, m_dialogBoxes[39].sV4); //v1.4
		else 
			AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);
	}
}

void CGame::NotifyMsg_ItemToGuildBank(char *pData)
{
	char * cp, cIndex;
	uint32_t * dwp, dwCount, dwAttribute;
	char  cName[21], cItemType, cEquipPos, cGenderLimit, cItemColor;
	bool  bIsEquipped;
	short * sp, sSprite, sSpriteFrame, sLevelLimit, sItemEffectValue2, sItemSpecEffectValue2;
	uint16_t  * wp, wWeight, wCurLifeSpan;
	char  cTxt[120];

	cp = (pData + INDEX2_MSGTYPE + 2);

	CItem * item = new CItem;

	Pop(cp, item->m_serverPtr);
	Pop(cp, m_guildBankVer);

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	dwp = (uint32_t *)cp;
	dwCount = *dwp;
	cp += 4;

	cItemType = *cp;
	cp++;

	cEquipPos = *cp;
	cp++;

	bIsEquipped = (bool)*cp;
	cp++;

	sp = (short *)cp;
	sLevelLimit = *sp;
	cp += 2;

	cGenderLimit = *cp;
	cp++;

	wp = (uint16_t *)cp;
	wCurLifeSpan = *wp;
	cp += 2;

	wp = (uint16_t *)cp;
	wWeight = *wp;
	cp += 2;

	sp = (short *)cp;
	sSprite = *sp;
	cp += 2;

	sp = (short *)cp;
	sSpriteFrame = *sp;
	cp += 2;

	cItemColor = *cp;
	cp++;


	sp = (short *)cp;
	sItemEffectValue2 = *sp;
	cp += 2;

	dwp = (uint32_t *)cp;
	dwAttribute = *dwp;
	cp += 4;
	sItemSpecEffectValue2 = (short) *cp ;
	cp++;

	for(int i = 0; i < MAXITEMSOCKETS; i++)
	{
		Pop(cp, item->m_sockets[i]);
	}

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(cName, dwAttribute, cStr1, cStr2, cStr3);



	memcpy(item->m_cName, cName, 20);
	item->m_dwCount = dwCount;

	item->m_cItemType = cItemType;
	item->m_cEquipPos = cEquipPos;

	item->m_sLevelLimit  = sLevelLimit;
	item->m_cGenderLimit = cGenderLimit;
	item->m_wCurLifeSpan = wCurLifeSpan;
	item->m_wWeight      = wWeight;
	item->m_sSprite      = sSprite;
	item->m_sSpriteFrame = sSpriteFrame;
	item->m_ItemColor   = cItemColor;
	item->m_sItemEffectValue2  = sItemEffectValue2;
	item->m_dwAttribute        = dwAttribute;
	item->m_sItemSpecEffectValue2 = sItemSpecEffectValue2;

	ItemMapIter dstIt = m_guildBankMap.insert( std::pair<CItem*, uint32_t> (item, item->m_serverPtr) );
	ItemMapIter it = m_guildBankMap.begin();

	uint32_t pos = 0;
	while(it != dstIt)
	{
		pos++;
		++it;
	}

	if(*(*dstIt).first < (*it).first)
		m_dialogBoxes[56].sView++;

	int32_t itPos = 0;
	while(it != dstIt)
	{
		if(*(*dstIt).first < (*it).first)
		{
			itPos--;
			--it;
		}else{
			itPos++;
			++it;
		}
	}

	ZeroMemory(cTxt, sizeof(cTxt));
	if( dwCount == 1 ) wsprintfA(cTxt, NOTIFYMSG_ITEMTOBANK3, cStr1);
	else wsprintfA(cTxt, NOTIFYMSG_ITEMTOBANK2, dwCount, cStr1);

	if(m_bIsDialogEnabled[56])
	{
		if(m_guildBankMap.size() > m_dialogBoxes[56].sV1)
		{
			if(pos < m_dialogBoxes[56].sView)
			{
				
				while(pos < m_dialogBoxes[56].sView)
				{
					m_dialogBoxes[56].sView--;
					--m_guildBankIt;
				}
			}else{
				while(pos > m_dialogBoxes[56].sView + m_dialogBoxes[56].sV1-1)
				{
					m_dialogBoxes[56].sView++;
					++m_guildBankIt;
				}
			}
		}else
			m_guildBankIt = m_guildBankMap.begin();
	}

	AddEventList(cTxt, 10);
}

void CGame::ReceiveGuildInfo(char * data)
{
	uint8_t tmpUpgrades[GU_MAX];

	for(int i=0; i < GU_MAX; i++)
	{
		Pop(data, tmpUpgrades[i]);

		if(m_guild.m_upgrades[i] != -1 && tmpUpgrades[i] > m_guild.m_upgrades[i])
		{
			sprintf(G_cTxt, DRAW_DIALOGBOX_GUILD42, gldUps[i].name.c_str(), tmpUpgrades[i]);
			AddEventList(G_cTxt, CHAT_GM);
		}

		m_guild.m_upgrades[i] = tmpUpgrades[i];
	}

	Pop(data, m_guild.m_gold);
	Pop(data, m_guild.m_maj);
	Pop(data, m_guild.m_cont);

	Pop(data, m_guild.m_captains);
	Pop(data, m_guild.m_raidmasters);
	Pop(data, m_guild.m_huntmasters);

	uint32_t memberCnt;
	Pop(data, memberCnt);

	while(memberCnt > 0)
	{
		char name[11];
		ZeroMemory(name, sizeof(name));
		GuildMember member;
		member.summonTime = 0;
		Pop(data, name, 10);
		Pop(data, member.rank);
		Pop(data, member.map);
		m_guild.AddMember(member, name);
		memberCnt--;
	}
}

void CGame::ReceiveGuildsmanStatus(char * data)
{
	char name[11];
	ZeroMemory(name, sizeof(name));

	Pop(data, name, 10);

	bool online;
	Pop(data, (uint8_t&)online);

	MemberList::iterator it = m_guild.m_onlineList.find( name );

	char txt[100];
	if(online)
	{
		if( strcmp(name, m_cPlayerName) == 0 )
		{
			GuildMember member;
			Pop(data, member.rank);

			wsprintfA(txt, GUILD_MEMBERRANKCHANGE,
				GetGuildRankName( m_iGuildRank ), GetGuildRankName( member.rank ) );
			AddEventList(txt, CHAT_GUILD);

			m_iGuildRank = (GuildRank)member.rank;
		}
		else if(it == m_guild.m_onlineList.end())
		{
			GuildMember member;
			Pop(data, member.rank);
			Pop(data, member.map);
			member.summonTime = 0;
			m_guild.AddMember(member, name);
			if(member.rank < GUILDRANK_MAX)
			{
				wsprintfA(txt, GUILD_MEMBERONLINE, GetGuildRankName( member.rank ), name);
				AddEventList(txt, CHAT_GUILD);
			}
		}
		else
		{
			uint8_t newrank;
			Pop(data, (uint8_t&)newrank);
			if((*it).second.rank != newrank)
			{
				wsprintfA(txt, GUILD_MEMBERRANKCHANGE2, name, GetGuildRankName( (*it).second.rank ), GetGuildRankName( newrank ));
				AddEventList(txt, CHAT_GUILD);
			}
			(*it).second.rank = newrank;

			Pop(data, (*it).second.map);
		}
	}
	else
	{
		if(it != m_guild.m_onlineList.end())
		{
			wsprintfA(txt, GUILD_MEMBEROFFLINE, GetGuildRankName((*it).second.rank), name);
			AddEventList(txt, CHAT_GUILD);
			m_guild.RemoveMember(name);
		}
	}
}

void CGame::InitItemList_GuildBank(char * data)
{
	uint16_t type;
	Pop(data, type);

	if(type == MSGTYPE_REJECT)
	{
		AddEventList(NOTIFY_MSG_HANDLER75);
		AddEventList(NOTIFY_MSG_HANDLER76);
		return;
	}

#ifdef _DEBUG
	AddEventList("Received Guild Bank List");
#endif

	m_guildBankMap.clear();

	uint16_t itemCnt;
	Pop(data, itemCnt);
	Pop(data, m_guildBankVer);

	for (int i = 0; i < itemCnt; i++)
	{
		CItem * item = new CItem;

		memcpy(item->m_cName, data, 20);
		data += 20;

		Pop(data, item->m_dwCount);
		Pop(data, (uint8_t&)item->m_cItemType);
		Pop(data, (uint8_t&)item->m_cEquipPos);
		Pop(data, (uint16_t&)item->m_sLevelLimit);
		Pop(data, (uint8_t&)item->m_cGenderLimit);
		Pop(data, item->m_wCurLifeSpan);
		Pop(data, item->m_wWeight);
		Pop(data, (uint16_t&)item->m_sSprite);
		Pop(data, (uint16_t&)item->m_sSpriteFrame);
		Pop(data, (uint8_t&)item->m_ItemColor);
		Pop(data, (uint16_t&)item->m_sItemSpecEffectValue2);
		Pop(data, item->m_dwAttribute);
		for(int i = 0; i < MAXITEMSOCKETS; i++)
			Pop(data, item->m_sockets[i]);
		Pop(data, item->m_serverPtr);

		item->m_sX = 40;
		item->m_sY = 30;

		m_guildBankMap.insert( std::pair<CItem*, uint32_t> (item, item->m_serverPtr) );
	}

	m_guildBankIt = m_guildBankMap.begin();
}

void CGame::RetrieveItemHandler_GuildBank(char * data)
{
	uint16_t type;
	Pop(data, type);

	if(type == MSGTYPE_REJECT) return;

	uint32_t serverPtr;
	Pop(data, serverPtr);

	uint8_t itemInd;
	Pop(data, itemInd);

	m_guildBankVer++;

	uint32_t m_tmpBankVer;
	Pop(data, m_tmpBankVer);
	if(m_guildBankVer != m_tmpBankVer)
		bSendCommand(MSGID_REQUEST_GUILDBANK);

	m_dialogBoxes[56].SetMode(0);


	ItemMapIter it = m_guildBankMap.begin();

	while(it != m_guildBankMap.end())
	{
		if((*it).second == serverPtr)
			break;
		++it;
	}

	if(it == m_guildBankMap.end()) return;


	CItem * item = (*it).first;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(item, cStr1, cStr2, cStr3);

	char cTxt[120];
	memset(cTxt, 0, sizeof(cTxt));
	wsprintfA(cTxt, RETIEVE_ITEM_HANDLER4, cStr1);//""You took out %s."
	AddEventList(cTxt, 10);

	if(it == m_guildBankIt)
	{
		//if(m_stDialogBoxInfo[56].sView > 0)
		//if(m_guildBankIt == m_guildBankMap.begin())
		++m_guildBankIt;
	}

	if(m_pItemList[itemInd] && (item->m_cItemType == ITEMTYPE_CONSUME || item->m_cItemType == ITEMTYPE_ARROW))
	{
		m_guildBankMap.erase(it);
		delete item;
	}
	else
	{
		short nX = 40, nY = 30;

		for(int j = 0; j < MAXITEMS; j++)
		{
			if(m_pItemList[j] && memcmp(m_pItemList[j]->m_cName, cStr1, 20) == 0)
			{
				nX = m_pItemList[j]->m_sX+1;
				nY = m_pItemList[j]->m_sY+1;
				break;
			}
		}

		m_pItemList[itemInd] = item;
		m_pItemList[itemInd]->m_sX = nX;
		m_pItemList[itemInd]->m_sY = nY;

		bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, itemInd, nX, nY, 0, 0);

		for(int j = 0; j < MAXITEMS; j++)
		{
			if (m_cItemOrder[j] == -1)
			{
				m_cItemOrder[j] = itemInd;
				break;
			}
		}

		m_bIsItemEquipped[itemInd] = false;
		m_bIsItemDisabled[itemInd] = false;

		m_guildBankMap.erase(it);
	}
}

void CGame::NotifyMsg_RepairAllPrice(char * pData)
{
	uint32_t price;
	char * cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	Pop(cp, price);
	if (price == 0) {
		AddEventList(NOTIFYMSG_NOTHING_TO_REPAIR, 10); //"You don't have any items that require repair."
	}
	else {
		EnableDialogBox(20, 11, 0, price);
	}
}

void CGame::ClearPartyMembers()
{
	for (std::vector<partyMember *>::iterator it = m_stPartyMember.begin(); it < m_stPartyMember.end();)
	{
		delete *it;
		it = m_stPartyMember.erase(it);
	}
}

void CGame::ResponseQuestList(char *pData)
{	
	char *cp;
	int  *ip, i;
#ifdef _DEBUG
	AddEventList("Quest List?", 10);
#endif
	cp = pData + 6;

	ip = (int*) cp;
	m_iQuestCount = *ip;
	cp += 4;

	for ( i = 0 ; i < m_iQuestCount ; i++)
	{	
		ip = (int*)cp;
		m_stQuestList[i].iIndex = *ip;
		cp += 4;

		ip = (int*)cp;
		m_stQuestList[i].Type = *ip;
		cp += 4;

		ip = (int*)cp;
		m_stQuestList[i].TargetType = *ip;
		cp += 4;

		ip = (int*)cp;
		m_stQuestList[i].MaxCount = *ip;
		cp += 4;

		ip = (int*)cp;
		m_stQuestList[i].RewardAmount = *ip;
		cp += 8;

		ip = (int*)cp;
		m_stQuestList[i].Contribution = *ip;
		cp += 4;

		ZeroMemory(m_stQuestList[i].TargetName, sizeof(m_stQuestList[i].TargetName) );
		memcpy(m_stQuestList[i].TargetName, cp, 20);
		cp += 20;

		ip = (int*)cp;
		m_stQuestList[i].sX = *ip;
		cp += 4;

		ip = (int*)cp;
		m_stQuestList[i].sY = *ip;
		cp += 4;

		ip = (int*)cp;
		m_stQuestList[i].Range = *ip;
		cp += 4;
	}
}

void CGame::ShowSoccerVictory(short sSide)
{	
	int i;
	DisableDialogBox(18);
	for (i = 0; i < TEXTDLGMAXLINES; i++)
	{	if (m_pMsgTextList[i] != 0)
	delete m_pMsgTextList[i];
	m_pMsgTextList[i] = 0;
	}

	switch (m_side) 
	{
	case 0:
		PlaySound('E', 25, 0, 0);
		m_pMsgTextList[0] = new class CMsg(0, "Soccer event Has Finished!", 0);
		m_pMsgTextList[1] = new class CMsg(0, " ", 0);
		m_pMsgTextList[2] = new class CMsg(0, "Soccer event has finished in a tie", 0);
		break;
	case 1:
		PlaySound('E', 25, 0, 0);
		m_pMsgTextList[0] = new class CMsg(0, "Soccer event Has Finished!", 0);
		m_pMsgTextList[1] = new class CMsg(0, " ", 0);
		m_pMsgTextList[2] = new class CMsg(0, "Soccer event has finished. Aresden Wins!", 0);
		break;
	case 2:
		PlaySound('E', 25, 0, 0);
		m_pMsgTextList[0] = new class CMsg(0, "Soccer event Has Finished!", 0);
		m_pMsgTextList[1] = new class CMsg(0, " ", 0);
		m_pMsgTextList[2] = new class CMsg(0, "Soccer event has finished. Elvine Wins!", 0);
		break;
	}
	m_pMsgTextList[3] = new class CMsg(0, " ", 0);

	if (sSide == m_side)
	{	PlaySound('E', 23, 0, 0);
	PlaySound('C', 21, 0, 0);
	PlaySound('C', 22, 0, 0);
	m_pMsgTextList[4] = new class CMsg(0, "Congratulations.", 0);
	m_pMsgTextList[5] = new class CMsg(0, "As a victorious citizen,", 0);
	m_pMsgTextList[6] = new class CMsg(0, "You'r town will receive a reward.", 0);
	m_pMsgTextList[7] = new class CMsg(0, "      ", 0);
	}else
	{	PlaySound('E', 24, 0, 0);
	PlaySound('C', 12, 0, 0);
	PlaySound('C', 13, 0, 0);
	m_pMsgTextList[4] = new class CMsg(0, "To our regret", 0);
	m_pMsgTextList[5] = new class CMsg(0, "As a defeated citizen,", 0);
	m_pMsgTextList[6] = new class CMsg(0, "You'r town won't receive any reward.", 0);
	m_pMsgTextList[7] = new class CMsg(0, "     ", 0);
	}	
	for (i = 9; i < 18; i++)
		m_pMsgTextList[i] = new class CMsg(0, " ", 0);
	EnableDialogBox(18, 0, 0, 0);
}

void CGame::NotifyMsg_Soccer(char * pData)
{
	uint16_t *wp;
	wp = (uint16_t *)(pData + INDEX2_MSGTYPE + 2);
	m_SoccerAresdenGoals = *wp;
	wp++;
	m_SoccerElvineGoals = *wp;
	wp++;
}

void CGame::DrawSoccerInfo()
{
	ZeroMemory(G_cTxt, sizeof(G_cTxt));
	wsprintfA( G_cTxt, "%10s Goals %2d Out Of 20", sideName[ARESDEN], m_SoccerAresdenGoals);
	PutString_SprFont3(10, 155, G_cTxt, m_wR[14]*4, m_wG[14]*4, m_wB[14]*4, true, 2);
	ZeroMemory(G_cTxt, sizeof(G_cTxt));
	wsprintfA( G_cTxt, "%10s Goals %2d Out Of 20", sideName[ELVINE], m_SoccerElvineGoals);
	PutString_SprFont3(13, 170, G_cTxt, m_wR[14]*4, m_wG[14]*4, m_wB[14]*4, true, 2);
}

#ifdef TitleClient
bool CGame::FindTitleName(char* pName, int* ipIndex)
{
	int i, iRet = 0;
	//uint32_t dwTmpTime;// commented, not used
	for( i=0 ; i < MAXGUILDNAMES ; i++ )
	{
		if( memcmp(m_stTitles[i].cCharName, pName, 10) == 0 )
		{
			// if char exists in list of structs, then its been requested before

			// we do not say false here because when someone changes their title,
			// then the are de-referenced and have to be requested again because
			// their char name does not exist!

			m_stTitles[i].dwRefTime = m_dwCurTime;
			*ipIndex = i;
			return true;
		}
	}

	// if the char name doesnt exist.... create a new entry, set it as "NONE" for title.
	// after we return false, it will request the title anyway!

	// the ClearPlayerTitle method is performed if somebody changed their title. This clears
	// thier CharName from the struct which in turns makes this method return false again.

	//dwTmpTime = m_stTitles[0].dwRefTime;//besk commented, not used
	//BESK: we arent creating a new entry here, we are looking for a pre-existing entry with cCharName "NONE"
	//		after we find one, we set its cCharName as pName and cSubject as "NONE", and we return its index
	for( i=0 ; i < MAXGUILDNAMES ; i++ )
	{
		if(strcmp(m_stTitles[i].cCharName, "NONE") == 0)
		{
			iRet = i;
			break;
		}
	}
	//BESK: what happens if we get here and we never found an empty index? we are then overwriting the first m_stTitles[] entry
	ZeroMemory(m_stTitles[iRet].cCharName, sizeof(m_stTitles[iRet].cCharName) );
	memcpy(m_stTitles[iRet].cCharName, pName, 10 );//BESK: cCharName is 12 bytes, why? if names are only 10...

	ZeroMemory(m_stTitles[iRet].cSubject, sizeof(m_stTitles[iRet].cSubject) );
	memcpy(m_stTitles[iRet].cSubject, "NONE", 4 );

	m_stTitles[iRet].dwRefTime = m_dwCurTime;
	m_stTitles[iRet].iCurrentNo = -1;

	*ipIndex = iRet;
	return false;
}

/*
void CGame::DrawDialogBox_Skill()
{
 short sX, sY;
 int  iTotalLines, iPointerLoc;
 char cTemp[255], cTemp2[255];
 double d1,d2,d3;
 Point pointerLoc;

	char onButton = m_dialogBoxes[15].OnButton();
	sX = m_dialogBoxes[15].m_X;
	sY = m_dialogBoxes[15].m_Y;

	//DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 0); // Normal Dialog
	//DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, sX, sY, 1); // Skill Dialog Title Bar

	switch (m_dialogBoxes[15].GetMode()) {
	case 0:
		for (int line=0, skillIndex=0; line < 17;skillIndex++)
		if (line < MAXSKILLTYPE && m_pSkillCfgList[skillIndex+m_dialogBoxes[15].sView])
		{
			ZeroMemory(cTemp, sizeof(cTemp));
			wsprintfA(cTemp, "%s", m_pSkillCfgList[skillIndex + m_dialogBoxes[15].sView]->m_cName);
			m_Misc.ReplaceString(cTemp, '-', ' ');
			ZeroMemory(cTemp2, sizeof(cTemp2));
			wsprintfA(cTemp2, "%3d%%", m_pSkillCfgList[skillIndex + m_dialogBoxes[15].sView]->m_iLevel);

			COLORREF colour =video::SColor(255,5,5,5);
			if(m_pSkillCfgList[ skillIndex+m_dialogBoxes[15].sView ]->m_bIsUseable
				&& m_pSkillCfgList[ skillIndex+m_dialogBoxes[15].sView ]->m_iLevel != 0)
				{
				if (onButton == line + 1)
				{
					colour =video::SColor(255,255,255,255);
				}
			else
				{
					colour =video::SColor(255,34,30,120);
				}
			}

			PutString(sX + 30, sY + 45 + line*15, cTemp, colour);
			PutString(sX + 183, sY + 45 + line*15, cTemp2, colour);

			if (m_iDownSkillIndex == (skillIndex + m_dialogBoxes[15].sView))
				 m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSpriteRGB(sX + 215, sY + 47 + line*15, 21, 50,50,50, m_dwTime);
			else m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutSpriteFast(sX + 215, sY + 47 + line*15, 20, m_dwTime);
			line++;
		}

		iTotalLines = 0;
		for (int i = 0; i < MAXSKILLTYPE; i++)
		if (m_pSkillCfgList[i] != NULL) iTotalLines++;

		pointerLoc = m_dialogBoxes[15].HandleScroll(17, iTotalLines);

		if (iTotalLines > 17)
		{	
			DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, sX, sY, 1);
			DrawNewDialogBox(SPRID_INTERFACE_ND_GAME2, pointerLoc.x, pointerLoc.y, 7);
		}

	   	break;
	}
}
*/

void CGame::NotifyMsg_AddTitle(char * pData)
{

	char  * cp, cSubject[21];
	int   i, j;
	int * ip;
	uint32_t * dwp;
	uint32_t   iCurrentNo;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	ZeroMemory(cSubject, sizeof(cSubject));
	memcpy(cSubject, cp, 21);
	cp += 21;

	dwp = (uint32_t *)cp;
	iCurrentNo = *dwp;
	cp += 4;

	ip = (int *)cp;
	int iNextRankPoints = *ip;
	cp += 4;

	ip = (int *)cp;
	int iRank = *ip;
	cp += 4;

	ip = (int *)cp;
	int iServerTitleIndex = *ip;
	cp += 4;

	char cActiveTitle = *cp;//this is 0 if this isnt our active title, 1 if it is our active title
	cp++;

	// check for existing title
	for(i = 0; i < MAXMENUITEMS; i++)
	{
		if(m_pTitles[i] != 0)
		{
			if(memcmp(m_pTitles[i]->m_cSubject, cSubject, 21) == 0)
			{
				m_pTitles[i]->m_iCurrentNo = iCurrentNo;
				m_pTitles[i]->m_iNextRankPoints = iNextRankPoints;
				m_pTitles[i]->m_iRank = iRank;
				m_pTitles[i]->m_iServerTitleIndex = iServerTitleIndex;
				if (cActiveTitle == 1)
					m_iTitleIndex = i;
				return;//BESK added
			}
		}
	}

	int k;
	for(k = 0; k < MAXMENUITEMS; k++)
	{
		if(m_pTitles[k] == 0)
		{
			m_pTitles[k] = new class Title;
			break;
		}
	}
	
	memcpy(m_pTitles[k]->m_cSubject, cSubject, sizeof(cSubject));
	m_pTitles[k]->m_iCurrentNo = iCurrentNo;
	m_pTitles[k]->m_iNextRankPoints = iNextRankPoints;
	m_pTitles[k]->m_iRank = iRank;
	m_pTitles[k]->m_iServerTitleIndex = iServerTitleIndex;
	if (cActiveTitle == 1)
		m_iTitleIndex = k;
}

void CGame::NotifyMsg_TitleLevelUp(char * pData)//BESK TITLES
{

	char  * cp, cSubject[21], cMsg[120], cTxt[120];
	int   i, j;
	int * ip;
	uint32_t * dwp;
	uint32_t   iCurrentNo;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	ZeroMemory(cSubject, sizeof(cSubject));
	memcpy(cSubject, cp, 21);
	cp += 21;

	dwp = (uint32_t *)cp;
	iCurrentNo = *dwp;
	cp += 4;

	ip = (int *)cp;
	int iNextRankPoints = *ip;
	cp += 4;

	ip = (int *)cp;
	int iRank = *ip;
	cp += 4;

	ip = (int *)cp;
	int iServerTitleIndex = *ip;
	cp += 4;

	// check for existing title, and update it
	//bool bUsingLeveledUpTitle = false;//is the newly leveled up title the one we are wearing?
	for(i = 0; i < MAXMENUITEMS; i++)
	{
		if(m_pTitles[i] != 0)
		{
			if(memcmp(m_pTitles[i]->m_cSubject, cSubject, 21) == 0)
			{// FUCK IDK? D:
				m_pTitles[i]->m_iCurrentNo = iCurrentNo;
				m_pTitles[i]->m_iNextRankPoints = iNextRankPoints;
				m_pTitles[i]->m_iRank = iRank;
				m_pTitles[i]->m_iServerTitleIndex = iServerTitleIndex;
				//if (i == m_iTitleIndex) bUsingLeveledUpTitle = true;
				break;
			}
		}
	}

	ZeroMemory(cMsg, sizeof(cMsg));
	ZeroMemory(cTxt, sizeof(cTxt));
	ZeroMemory(cTitleLevelUpSubject, sizeof(cTitleLevelUpSubject));

	GetTitleName(cSubject, iRank, cTxt);

	//if(memcmp(cSubject, "Events", 6) == 0)
	//{
	//	wsprintfA(cMsg, "You have earned a new title by winning %i events!", iCurrentNo);
	//}
	//else if (memcmp(cSubject, "Donation", 8) == 0)
	//{
	//	wsprintfA(cMsg, "You have earned a new title by donating %i pounds!", iCurrentNo);
	//}
	//else if (memcmp(cSubject, "Gladiator", 9) == 0)
	//{
	//	wsprintfA(cMsg, "You have earned a new title by earning 25 consecutive kills %i times in the arena!", iCurrentNo);
	//}
	//else if (memcmp(cSubject, "Constructor", 11) == 0)
	//{
	//	wsprintfA(cMsg, "You have earned a new title by completing %i constructions!", iCurrentNo);
	//}
	//else if (memcmp(cSubject, "Treasure", 8) == 0)
	//{
	//	wsprintfA(cMsg, "You have earned a new title by finded %i rare items!", iCurrentNo);
	//}
	//else if (memcmp(cSubject, "Suicide", 7) == 0)
	//{
	//	wsprintfA(cMsg, "You have earned a new title by committing suicide %i times!", iCurrentNo);
	//}
	//else if (memcmp(cSubject, "Crafting", 8) == 0)
	//{
	//	wsprintfA(cMsg, "You have earned a new title by crafting %i items!", iCurrentNo);
	//}
	//else if (memcmp(cSubject, "Hero", 4) == 0)
	//{
	//	wsprintfA(cMsg, "You have earned a new title by killing %i players from the enemy town!", iCurrentNo);
	//}
	//else if (memcmp(cSubject, "Drunkard", 8) == 0)
	//{
	//	wsprintfA(cMsg, "You have earned a new title by being drunk for %i minutes!", iCurrentNo);
	//}
	//else if (memcmp(cSubject, "SweetTooth", 10) == 0)
	//{
	//	wsprintfA(cMsg, "You have earned a new title by eating %i sweets!", iCurrentNo);
	//}
	if (false) {}
	else if (memcmp(cSubject, "Alchemy", 7) == 0)
	{
		wsprintfA(cMsg, "You have earned a new title by creating %i potions!", iCurrentNo);
	}
	else if (memcmp(cSubject, "Mining", 6) == 0)
	{
		wsprintfA(cMsg, "You have earned a new title by mining %i ores!", iCurrentNo);
	}
	else if (memcmp(cSubject, "Blacksmithing", 13) == 0)
	{
		wsprintfA(cMsg, "You have earned a new title by smithing %i items!", iCurrentNo);
	}
	else if (memcmp(cSubject, "Farming", 7) == 0)
	{
		wsprintfA(cMsg, "You have earned a new title by farming %i crops!", iCurrentNo);
	}
	else if (memcmp(cSubject, "Fishing", 7) == 0)
	{
		wsprintfA(cMsg, "You have earned a new title by catching %i fish!", iCurrentNo);
	}
	else if (memcmp(cSubject, "Gemcutting", 10) == 0)
	{
		wsprintfA(cMsg, "You have earned a new title by creating %i pieces of jewelery!", iCurrentNo);
	}
	else if (memcmp(cSubject, "Gladiator", 9) == 0)
	{
		wsprintfA(cMsg, "You have earned a new title by earning 25 consecutive kills %i times in the arena!", iCurrentNo);
	}else if (memcmp(cSubject, "Hero", 4) == 0)
	{
		wsprintfA(cMsg, "You have earned a new title by earning 25 consecutive kills %i times in the arena!", iCurrentNo);
	}
	else
	{
		wsprintfA(cMsg, "You have earned a new title by killing %i %s!", iCurrentNo, cSubject);
	}
	AddEventList(cMsg, 10);

	switch (m_sPlayerType) {
	case 1:
	case 2:
	case 3:
	case 7:
		PlaySound('C', 21, 0);
		break;

	case 4:
	case 5:
	case 6:
	case 8:
		PlaySound('C', 22, 0);
		break;
	}

	dwTitleLevelUpTime = m_dwCurTime;

	wsprintfA(cTitleLevelUpSubject, cTxt);

	//for (i = 0; i < DEF_MAXMENUITEMS; i++) {
	//	if (m_pTitles[i] != NULL) {
	//		delete m_pTitles[i];
	//		m_pTitles[i] = NULL;
	//	}
	//}

	//m_iTitleIndex = -1;//BESK TODO: in my code, dont do this! why reset the player's active title on any title rankup???
	//if (bUsingLeveledUpTitle)//send SETTITLE to the server to force us and everyone else to refresh our title, since we ranked up
	//	bSendCommand(MSGID_SETTITLE, NULL, NULL, m_pTitles[m_iTitleIndex]->m_iServerTitleIndex, NULL, NULL, NULL);
		
	bSendCommand(MSGID_REQUEST_TITLES, 0, 0, 0, 0, 0, 0);
	m_cCommandCount--;
	m_dwReqTitlesTime = m_dwCurTime;
}

void CGame::GetTitleName(char * cSubject, int iRank, char *pStr1)
{

	char cTxt[256];

	ZeroMemory(cTxt, sizeof(cTxt));

	if (memcmp(cSubject, "Events", 6) == 0)
	{
		//if(iNumber >   0 && iNumber < 10)	   strcpy(cTxt, "Events Lv.0");
		//if(iNumber >= 10 && iNumber < 25)      strcpy(cTxt, "Events Lv.1");
		//if(iNumber >= 25 && iNumber < 50)      strcpy(cTxt, "Events Lv.2");
		//if(iNumber >= 50 && iNumber < 100)     strcpy(cTxt, "Events Lv.3");
		//if(iNumber >= 100 && iNumber < 250)    strcpy(cTxt, "Events Lv.4");
		//if(iNumber >= 250 && iNumber < 500)    strcpy(cTxt, "Events Lv.5");
		//if(iNumber >= 500 && iNumber < 1000)   strcpy(cTxt, "Events Lv.6");
		//if(iNumber >= 1000 && iNumber < 2500)  strcpy(cTxt, "Events Lv.7");
		//if(iNumber >= 2500 && iNumber < 5000)  strcpy(cTxt, "Events Lv.8");
		//if(iNumber >= 5000)					   strcpy(cTxt, "Events Lv.9");

		wsprintfA(cTxt, "Events Lv.%d", iRank);

		//if (iRank == 0) {		 strcpy(cTxt, "Events Lv.0");
		//} else if (iRank == 1) { strcpy(cTxt, "Events Lv.1");
		//} else if (iRank == 2) { strcpy(cTxt, "Events Lv.2");
		//} else if (iRank == 3) { strcpy(cTxt, "Events Lv.3");
		//} else if (iRank == 4) { strcpy(cTxt, "Events Lv.4");
		//} else if (iRank == 5) { strcpy(cTxt, "Events Lv.5");
		//} else if (iRank == 6) { strcpy(cTxt, "Events Lv.6");
		//} else if (iRank == 7) { strcpy(cTxt, "Events Lv.7");
		//} else if (iRank == 8) { strcpy(cTxt, "Events Lv.8");
		//} else if (iRank == 9) { strcpy(cTxt, "Events Lv.9");
		//} else if (iRank == 10) { strcpy(cTxt, "Events Lv.10");
		//}
	}
	else if (memcmp(cSubject, "Donation", 8) == 0)
	{
	/*	if(iNumber >= 1 && iNumber < 5)			strcpy(cTxt, "Penny Pincher");
		if(iNumber >= 5 && iNumber < 10)		strcpy(cTxt, "Spare Change");
		if(iNumber >= 10 && iNumber < 20)		strcpy(cTxt, "Bargain Hunter");
		if(iNumber >= 20 && iNumber < 50)		strcpy(cTxt, "Rich Kid");
		if(iNumber >= 50 && iNumber < 100)		strcpy(cTxt, "Money Bags");
		if(iNumber >= 100 && iNumber < 175)		strcpy(cTxt, "Big Spender");
		if(iNumber >= 175 && iNumber < 250)		strcpy(cTxt, "Filthy Rich");
		if(iNumber >= 250 && iNumber < 500)		strcpy(cTxt, "Bling Bling");
		if(iNumber >= 500)					    strcpy(cTxt, "Bling Bling");*/
		//if (iRank == 0) {		 strcpy(cTxt, "Penny Pincher Lv.%d");
		//} else if (iRank == 1) { strcpy(cTxt, "Spare Change Lv.%d");
		//} else if (iRank == 2) { strcpy(cTxt, "Bargain Hunter Lv.%d");
		//} else if (iRank == 3) { strcpy(cTxt, "Rich Kid");
		//} else if (iRank == 4) { strcpy(cTxt, "Money Bags");
		//} else if (iRank == 5) { strcpy(cTxt, "Big Spender");
		//} else if (iRank == 6) { strcpy(cTxt, "Filthy Rich");
		//} else if (iRank == 7) { strcpy(cTxt, "Bling Bling");
		//} else if (iRank == 8) { strcpy(cTxt, "Events Lv.8");
		//} else if (iRank == 9) { strcpy(cTxt, "Events Lv.9");
		//} else if (iRank == 10) { strcpy(cTxt, "Philanthropist Lv.%d");
		//}
		if ((iRank >= 0)&&(iRank < 4)) wsprintfA(cTxt, "Tipper Lv.%d", iRank);
		else if ((iRank >= 4)&&(iRank < 6)) wsprintfA(cTxt, "Patron Lv.%d", iRank);
		else if ((iRank >= 6)&&(iRank < 8)) wsprintfA(cTxt, "Sponsor Lv.%d", iRank);
		else if ((iRank >= 8)&&(iRank < 9)) wsprintfA(cTxt, "Benefactor Lv.%d", iRank);
		else if ((iRank >= 9)) wsprintfA(cTxt, "Philanthropist Lv.%d", iRank);



	}
    else if (memcmp(cSubject, "Gladiator", 9) == 0)
	{
		//if(iNumber >  0 && iNumber < 3)			strcpy(cTxt, "Novice Gladiator");
		//if(iNumber >= 3 && iNumber < 5)			strcpy(cTxt, "Fierce Gladiator");
		//if(iNumber >= 5 && iNumber < 10)		strcpy(cTxt, "Mighty Gladiator");
		//if(iNumber >= 10 && iNumber < 25)		strcpy(cTxt, "Deadly Gladiator");
		//if(iNumber >= 25 && iNumber < 50)		strcpy(cTxt, "Terrifying Gladiator");
		//if(iNumber >= 50 && iNumber < 100)		strcpy(cTxt, "Conquering Gladiator");
		//if(iNumber >= 100 && iNumber < 250)		strcpy(cTxt, "King's Gladiator");
		//if(iNumber >= 250 && iNumber < 500)		strcpy(cTxt, "Emperor's Gladiator");
		//if(iNumber >= 500 && iNumber < 1000)	strcpy(cTxt, "Adept Gladiator");
		//if(iNumber >= 1000 && iNumber < 2500)	strcpy(cTxt, "Vanquishing Gladiator");
		//if(iNumber >= 2500 && iNumber < 5000)	strcpy(cTxt, "Legendary Gladiator");
		//if(iNumber >= 5000)						strcpy(cTxt, "Legendary Gladiator");

		if (iRank == 0) {		 wsprintfA(cTxt, "Gladiator Lv.%d", iRank);
		} else if (iRank == 1) { wsprintfA(cTxt, "Gladiator Lv.%d", iRank);
		} else if (iRank == 2) { wsprintfA(cTxt, "Gladiator Lv.%d", iRank);
		} else if (iRank == 3) { wsprintfA(cTxt, "Gladiator Lv.%d", iRank);
		} else if (iRank == 4) { wsprintfA(cTxt, "Gladiator Lv.%d", iRank);
		} else if (iRank == 5) { wsprintfA(cTxt, "Gladiator Lv.%d", iRank);
		} else if (iRank == 6) { wsprintfA(cTxt, "Gladiator Lv.%d", iRank);
		} else if (iRank == 7) { wsprintfA(cTxt, "Gladiator Lv.%d", iRank);
		} else if (iRank == 8) { wsprintfA(cTxt, "Gladiator Lv.%d", iRank);
		} else if (iRank == 9) { wsprintfA(cTxt, "Gladiator Lv.%d", iRank);
		} else if (iRank >= 10) { wsprintfA(cTxt, "Gladiator Lv.%d", iRank);
		}
	}
	else if (memcmp(cSubject, "Constructor", 11) == 0)
	{
		//if(iNumber >  0 && iNumber < 3)			strcpy(cTxt, "Beginner Constructor");
		//if(iNumber >= 3 && iNumber < 5)			strcpy(cTxt, "Novice Constructor");
		//if(iNumber >= 5 && iNumber < 10)		strcpy(cTxt, "Expert Constructor");
		//if(iNumber >= 10 && iNumber < 25)		strcpy(cTxt, "Adept Constructor");
		//if(iNumber >= 25 && iNumber < 50)		strcpy(cTxt, "Master Constructor");
		//if(iNumber >= 50 && iNumber < 100)		strcpy(cTxt, "Elite Constructor");
		//if(iNumber >= 100 && iNumber < 250)		strcpy(cTxt, "Grandmaster Constructor");
		//if(iNumber >= 250 && iNumber < 500)		strcpy(cTxt, "Supreme Constructor");
		//if(iNumber >= 500 && iNumber < 1000)	strcpy(cTxt, "Divine Constructor");
		//if(iNumber >= 1000 && iNumber < 2500)	strcpy(cTxt, "Godly Constructor");
		//if(iNumber >= 2500 && iNumber < 5000)	strcpy(cTxt, "Legendary Constructor");
		//if(iNumber >= 5000)						strcpy(cTxt, "Legendary Constructor");

		wsprintfA(cTxt, "Constructor Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "Hero", 4) == 0)
	{
		//if(iNumber > 0  && iNumber < 25)		strcpy(cTxt, "Novice Hero");
		//if(iNumber >= 25 && iNumber < 50)		strcpy(cTxt, "Fierce Hero");
		//if(iNumber >= 50 && iNumber < 150)		strcpy(cTxt, "Mighty Hero");
		//if(iNumber >= 150 && iNumber < 300)		strcpy(cTxt, "Deadly Hero");
		//if(iNumber >= 300 && iNumber < 500)		strcpy(cTxt, "Terrifying Hero");
		//if(iNumber >= 500 && iNumber < 1500)	strcpy(cTxt, "Conquering Hero");
		//if(iNumber >= 1500 && iNumber < 3000)	strcpy(cTxt, "King's Hero");
		//if(iNumber >= 3000 && iNumber < 5000)	strcpy(cTxt, "Emperor's Hero");
		//if(iNumber >= 5000 && iNumber < 7500)	strcpy(cTxt, "Adept Hero");
		//if(iNumber >= 7500 && iNumber < 10000)	strcpy(cTxt, "Vanquishing Hero");
		//if(iNumber >= 10000 && iNumber < 20000)	strcpy(cTxt, "Divine Hero");
		//if(iNumber >= 20000 && iNumber < 50000)	strcpy(cTxt, "Godly Hero");
		//if(iNumber >= 50000)					strcpy(cTxt, "Legendary Hero");
		wsprintfA(cTxt, "Hero Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "Treasure", 8) == 0)
	{
		//if(iNumber >  0 && iNumber < 10)		strcpy(cTxt, "Novice Treasure Hunter");
		//if(iNumber >= 10 && iNumber < 25)		strcpy(cTxt, "Expert Treasure Hunter");
		//if(iNumber >= 25 && iNumber < 50)		strcpy(cTxt, "Adept Treasure Hunter");
		//if(iNumber >= 50 && iNumber < 100)		strcpy(cTxt, "Master Treasure Hunter");
		//if(iNumber >= 100 && iNumber < 250)		strcpy(cTxt, "Elite Treasure Hunter");
		//if(iNumber >= 250 && iNumber < 500)		strcpy(cTxt, "Grandmaster Treasure Hunter");
		//if(iNumber >= 500 && iNumber < 1000)	strcpy(cTxt, "Supreme Treasure Hunter");
		//if(iNumber >= 1000 && iNumber < 2500)	strcpy(cTxt, "Divine Treasure Hunter");
		//if(iNumber >= 2500 && iNumber < 5000)	strcpy(cTxt, "Godly Treasure Hunter");
		//if(iNumber >= 5000 && iNumber < 10000)	strcpy(cTxt, "Legendary Treasure Hunter");
		//if(iNumber >= 10000)					strcpy(cTxt, "Legendary Treasure Hunter");
		wsprintfA(cTxt, "Treasure Hunter Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "Drunkard", 8) == 0)
	{
		//if(iNumber >= 0 && iNumber < 5)			 strcpy(cTxt, "T Total Drunkard");
		//if(iNumber >= 5 && iNumber <  10)		 strcpy(cTxt, "Sober Drunkard");
		//if(iNumber >= 10 && iNumber <   20)		 strcpy(cTxt, "Tipsy Drunkard");
		//if(iNumber >= 20 && iNumber <  40)		 strcpy(cTxt, "Buzzed Drunkard");
		//if(iNumber >= 40 && iNumber <  80)	     strcpy(cTxt, "Drunk Drunkard");
		//if(iNumber >= 80 && iNumber <  160)		 strcpy(cTxt, "Wasted Drunkard");
		//if(iNumber >= 160 && iNumber < 320)		 strcpy(cTxt, "Smashed Drunkard");
		//if(iNumber >= 320 && iNumber < 640)		 strcpy(cTxt, "Binge Drunkard");
		//if(iNumber >= 640 && iNumber < 1280)	 strcpy(cTxt, "Hammered Drunkard");
		//if(iNumber >= 1280 && iNumber < 2560)	 strcpy(cTxt, "Plastered Drunkard");
		//if(iNumber >= 2560 && iNumber < 5120)	 strcpy(cTxt, "Sloshed Drunkard");
		//if(iNumber >= 5120 && iNumber < 10240)	 strcpy(cTxt, "Legendary Drunkard");
		//if(iNumber >= 10240)					 strcpy(cTxt, "Legendary Drunkard");
		wsprintfA(cTxt, "Drunkard Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "SweetTooth", 10) == 0)
	{
		//if(iNumber >= 0 && iNumber < 5)			 strcpy(cTxt, "Sweet Tooth");
		//if(iNumber >= 5 && iNumber <  10)		 strcpy(cTxt, "Beginner Sweet Tooth");
		//if(iNumber >= 10 && iNumber <   20)		 strcpy(cTxt, "Novice Sweet Tooth");
		//if(iNumber >= 20 && iNumber <  40)		 strcpy(cTxt, "Expert Sweet Tooth");
		//if(iNumber >= 40 && iNumber <  80)	     strcpy(cTxt, "Adept Sweet Tooth");
		//if(iNumber >= 80 && iNumber <  160)		 strcpy(cTxt, "Master Sweet Tooth");
		//if(iNumber >= 160 && iNumber < 320)		 strcpy(cTxt, "Elite Sweet Tooth");
		//if(iNumber >= 320 && iNumber < 640)		 strcpy(cTxt, "Grandmaster Sweet Tooth");
		//if(iNumber >= 640 && iNumber < 1280)	 strcpy(cTxt, "Supreme Sweet Tooth");
		//if(iNumber >= 1280 && iNumber < 2560)	 strcpy(cTxt, "Divine Sweet Tooth");
		//if(iNumber >= 2560 && iNumber < 5120)	 strcpy(cTxt, "Godly Sweet Tooth");
		//if(iNumber >= 5120 && iNumber < 10240)	 strcpy(cTxt, "Legendary Sweet Tooth");
		//if(iNumber >= 10240)					 strcpy(cTxt, "Legendary Sweet Tooth");
		wsprintfA(cTxt, "SweetTooth Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "Suicide", 7) == 0)
	{
		//if(iNumber >= 10 && iNumber < 25)		strcpy(cTxt, "Novice Lemming");
		//if(iNumber >= 25 && iNumber < 50)		strcpy(cTxt, "Expert Lemming");
		//if(iNumber >= 50 && iNumber < 100)		strcpy(cTxt, "Adept Lemming");
		//if(iNumber >= 100 && iNumber < 250)		strcpy(cTxt, "Master Lemming");
		//if(iNumber >= 250 && iNumber < 500)		strcpy(cTxt, "Elite Lemming");
		//if(iNumber >= 500 && iNumber < 1000)	strcpy(cTxt, "Grandmaster Lemming");
		//if(iNumber >= 1000 && iNumber < 1750)	strcpy(cTxt, "Supreme Lemming");
		//if(iNumber >= 1750 && iNumber < 2500)	strcpy(cTxt, "Divine Lemming");
		//if(iNumber >= 2500 && iNumber < 5000)	strcpy(cTxt, "Godly Lemming");
		//if(iNumber >= 5000 && iNumber < 10000)	strcpy(cTxt, "Legendary Lemming");
		//if(iNumber >= 10000)					strcpy(cTxt, "Legendary Lemming");
		wsprintfA(cTxt, "Suicide Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "Alchemy", 7) == 0)
	{
		/*if(iRank >= 1 && iRank <=   1)			 strcat(cTxt, "Trainee Herbalist (Lvl.1)");
		if(iRank >= 2 && iRank <=   2)			 strcat(cTxt, "Initiate Herbalist (Lvl.2)");
		if(iRank >= 3 && iRank <    5)			 strcat(cTxt, "Beginner Herbalist (Lvl.3)");
		if(iRank >= 5 && iRank <    8)           strcat(cTxt, "Apprentice Herbalist (Lvl.4)");
		if(iRank >= 8 && iRank <   13)           strcat(cTxt, "Novice Herbalist (Lvl.5)");
		if(iRank >= 13 && iRank <  21)			 strcat(cTxt, "Novice Herbalist (Lvl.6)");
		if(iRank >= 21 && iRank <   34)          strcat(cTxt, "Novice Herbalist (Lvl.7)");
		if(iRank >= 34 && iRank <   55)			 strcat(cTxt, "Novice Herbalist (Lvl.8)");
		if(iRank >= 55 && iRank <   89)			 strcat(cTxt, "Novice Herbalist (Lvl.9)");
		if(iRank >= 89 && iRank <   144)		 strcat(cTxt, "Experienced Herbalist (Lvl.10)");
		if(iRank >= 144 && iRank <   233)		 strcat(cTxt, "Experienced Herbalist (Lvl.11)");
		if(iRank >= 233 && iRank <   377)		 strcat(cTxt, "Experienced Herbalist (Lvl.12)");
		if(iRank >= 377 && iRank <   610)		 strcat(cTxt, "Experienced Herbalist (Lvl.13)");
		if(iRank >= 610 && iRank <   987)		 strcat(cTxt, "Experienced Herbalist (Lvl.14)");
		if(iRank >= 987 && iRank <  1597)		 strcat(cTxt, "Master Herbalist (Lvl.15)");
		if(iRank >= 1597 && iRank <  2584)		 strcat(cTxt, "Master Herbalist (Lvl.16)");
		if(iRank >= 2584 && iRank <  4181)		 strcat(cTxt, "Master Herbalist (Lvl.17)");
		if(iRank >= 4181 && iRank <  6765)		 strcat(cTxt, "Master Herbalist (Lvl.18)");
		if(iRank >= 6765 && iRank < 10946)		 strcat(cTxt, "Master Herbalist (Lvl.19)");
		if(iRank >= 10946 && iRank < 17711)		 strcat(cTxt, "Grandmaster Herbalist (Lvl.20)");
		if(iRank >= 17711 && iRank < 28657)		 strcat(cTxt, "Supreme Herbalist (Lvl.21)");
		if(iRank >= 28657 && iRank < 46368)		 strcat(cTxt, "Supreme Herbalist (Lvl.22)");
		if(iRank >= 46368 && iRank < 75025)		 strcat(cTxt, "Supreme Herbalist (Lvl.23)");
		if(iRank >= 75025 && iRank < 121393)	 strcat(cTxt, "Supreme Herbalist (Lvl.24)");
		if(iRank >= 121393)						 strcat(cTxt, "Legendary Herbalist (Lvl.25)");*/
		wsprintfA(cTxt, "Herbalist Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "Gemcutting", 10) == 0)
	{
		/*if(iRank >= 1 && iRank <=   1)			 strcat(cTxt, "Trainee Gemcutter (Lvl.1)");
		if(iRank >= 2 && iRank <=   2)			 strcat(cTxt, "Initiate Gemcutter (Lvl.2)");
		if(iRank >= 3 && iRank <    5)			 strcat(cTxt, "Beginner Gemcutter (Lvl.3)");
		if(iRank >= 5 && iRank <    8)           strcat(cTxt, "Apprentice Gemcutter (Lvl.4)");
		if(iRank >= 8 && iRank <   13)           strcat(cTxt, "Novice Gemcutter (Lvl.5)");
		if(iRank >= 13 && iRank <  21)			 strcat(cTxt, "Novice Gemcutter (Lvl.6)");
		if(iRank >= 21 && iRank <   34)          strcat(cTxt, "Novice Gemcutter (Lvl.7)");
		if(iRank >= 34 && iRank <   55)			 strcat(cTxt, "Novice Gemcutter (Lvl.8)");
		if(iRank >= 55 && iRank <   89)			 strcat(cTxt, "Novice Gemcutter (Lvl.9)");
		if(iRank >= 89 && iRank <   144)		 strcat(cTxt, "Experienced Gemcutter (Lvl.10)");
		if(iRank >= 144 && iRank <   233)		 strcat(cTxt, "Experienced Gemcutter (Lvl.11)");
		if(iRank >= 233 && iRank <   377)		 strcat(cTxt, "Experienced Gemcutter (Lvl.12)");
		if(iRank >= 377 && iRank <   610)		 strcat(cTxt, "Experienced Gemcutter (Lvl.13)");
		if(iRank >= 610 && iRank <   987)		 strcat(cTxt, "Experienced Gemcutter (Lvl.14)");
		if(iRank >= 987 && iRank <  1597)		 strcat(cTxt, "Master Gemcutter (Lvl.15)");
		if(iRank >= 1597 && iRank <  2584)		 strcat(cTxt, "Master Gemcutter (Lvl.16)");
		if(iRank >= 2584 && iRank <  4181)		 strcat(cTxt, "Master Gemcutter (Lvl.17)");
		if(iRank >= 4181 && iRank <  6765)		 strcat(cTxt, "Master Gemcutter (Lvl.18)");
		if(iRank >= 6765 && iRank < 10946)		 strcat(cTxt, "Master Gemcutter (Lvl.19)");
		if(iRank >= 10946 && iRank < 17711)		 strcat(cTxt, "Grandmaster Gemcutter (Lvl.20)");
		if(iRank >= 17711 && iRank < 28657)		 strcat(cTxt, "Supreme Gemcutter (Lvl.21)");
		if(iRank >= 28657 && iRank < 46368)		 strcat(cTxt, "Supreme Gemcutter (Lvl.22)");
		if(iRank >= 46368 && iRank < 75025)		 strcat(cTxt, "Supreme Gemcutter (Lvl.23)");
		if(iRank >= 75025 && iRank < 121393)	 strcat(cTxt, "Supreme Gemcutter (Lvl.24)");
		if(iRank >= 121393)						 strcat(cTxt, "Legendary Gemcutter (Lvl.25)");*/
		wsprintfA(cTxt, "Gemcutter Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "Mining", 6) == 0)
	{
		/*if(iRank >= 1 && iRank <=   1)			 strcat(cTxt, "Trainee Miner (Lvl.1)");
		if(iRank >= 2 && iRank <=   2)			 strcat(cTxt, "Initiate Miner (Lvl.2)");
		if(iRank >= 3 && iRank <    5)			 strcat(cTxt, "Beginner Miner (Lvl.3)");
		if(iRank >= 5 && iRank <    8)           strcat(cTxt, "Apprentice Miner (Lvl.4)");
		if(iRank >= 8 && iRank <   13)           strcat(cTxt, "Novice Miner (Lvl.5)");
		if(iRank >= 13 && iRank <  21)			 strcat(cTxt, "Novice Miner (Lvl.6)");
		if(iRank >= 21 && iRank <   34)          strcat(cTxt, "Novice Miner (Lvl.7)");
		if(iRank >= 34 && iRank <   55)			 strcat(cTxt, "Novice Miner (Lvl.8)");
		if(iRank >= 55 && iRank <   89)			 strcat(cTxt, "Novice Miner (Lvl.9)");
		if(iRank >= 89 && iRank <   144)		 strcat(cTxt, "Experienced Miner (Lvl.10)");
		if(iRank >= 144 && iRank <   233)		 strcat(cTxt, "Experienced Miner (Lvl.11)");
		if(iRank >= 233 && iRank <   377)		 strcat(cTxt, "Experienced Miner (Lvl.12)");
		if(iRank >= 377 && iRank <   610)		 strcat(cTxt, "Experienced Miner (Lvl.13)");
		if(iRank >= 610 && iRank <   987)		 strcat(cTxt, "Experienced Miner (Lvl.14)");
		if(iRank >= 987 && iRank <  1597)		 strcat(cTxt, "Master Miner (Lvl.15)");
		if(iRank >= 1597 && iRank <  2584)		 strcat(cTxt, "Master Miner (Lvl.16)");
		if(iRank >= 2584 && iRank <  4181)		 strcat(cTxt, "Master Miner (Lvl.17)");
		if(iRank >= 4181 && iRank <  6765)		 strcat(cTxt, "Master Miner (Lvl.18)");
		if(iRank >= 6765 && iRank < 10946)		 strcat(cTxt, "Master Miner (Lvl.19)");
		if(iRank >= 10946 && iRank < 17711)		 strcat(cTxt, "Grandmaster Miner (Lvl.20)");
		if(iRank >= 17711 && iRank < 28657)		 strcat(cTxt, "Supreme Miner (Lvl.21)");
		if(iRank >= 28657 && iRank < 46368)		 strcat(cTxt, "Supreme Miner (Lvl.22)");
		if(iRank >= 46368 && iRank < 75025)		 strcat(cTxt, "Supreme Miner (Lvl.23)");
		if(iRank >= 75025 && iRank < 121393)	 strcat(cTxt, "Supreme Miner (Lvl.24)");
		if(iRank >= 121393)						 strcat(cTxt, "Legendary Miner (Lvl.25)");*/
		wsprintfA(cTxt, "Miner Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "Blacksmithing", 13) == 0)
	{
		/*if(iRank >= 1 && iRank <=   1)			 strcat(cTxt, "Trainee Blacksmith (Lvl.1)");
		if(iRank >= 2 && iRank <=   2)			 strcat(cTxt, "Initiate Blacksmith (Lvl.2)");
		if(iRank >= 3 && iRank <    5)			 strcat(cTxt, "Beginner Blacksmith (Lvl.3)");
		if(iRank >= 5 && iRank <    8)           strcat(cTxt, "Apprentice Blacksmith (Lvl.4)");
		if(iRank >= 8 && iRank <   13)           strcat(cTxt, "Novice Blacksmith (Lvl.5)");
		if(iRank >= 13 && iRank <  21)			 strcat(cTxt, "Novice Blacksmith (Lvl.6)");
		if(iRank >= 21 && iRank <   34)          strcat(cTxt, "Novice Blacksmith (Lvl.7)");
		if(iRank >= 34 && iRank <   55)			 strcat(cTxt, "Novice Blacksmith (Lvl.8)");
		if(iRank >= 55 && iRank <   89)			 strcat(cTxt, "Novice Blacksmith (Lvl.9)");
		if(iRank >= 89 && iRank <   144)		 strcat(cTxt, "Experienced Blacksmith (Lvl.10)");
		if(iRank >= 144 && iRank <   233)		 strcat(cTxt, "Experienced Blacksmith (Lvl.11)");
		if(iRank >= 233 && iRank <   377)		 strcat(cTxt, "Experienced Blacksmith (Lvl.12)");
		if(iRank >= 377 && iRank <   610)		 strcat(cTxt, "Experienced Blacksmith (Lvl.13)");
		if(iRank >= 610 && iRank <   987)		 strcat(cTxt, "Experienced Blacksmith (Lvl.14)");
		if(iRank >= 987 && iRank <  1597)		 strcat(cTxt, "Master Blacksmith (Lvl.15)");
		if(iRank >= 1597 && iRank <  2584)		 strcat(cTxt, "Master Blacksmith (Lvl.16)");
		if(iRank >= 2584 && iRank <  4181)		 strcat(cTxt, "Master Blacksmith (Lvl.17)");
		if(iRank >= 4181 && iRank <  6765)		 strcat(cTxt, "Master Blacksmith (Lvl.18)");
		if(iRank >= 6765 && iRank < 10946)		 strcat(cTxt, "Master Blacksmith (Lvl.19)");
		if(iRank >= 10946 && iRank < 17711)		 strcat(cTxt, "Grandmaster Blacksmith (Lvl.20)");
		if(iRank >= 17711 && iRank < 28657)		 strcat(cTxt, "Supreme Blacksmith (Lvl.21)");
		if(iRank >= 28657 && iRank < 46368)		 strcat(cTxt, "Supreme Blacksmith (Lvl.22)");
		if(iRank >= 46368 && iRank < 75025)		 strcat(cTxt, "Supreme Blacksmith (Lvl.23)");
		if(iRank >= 75025 && iRank < 121393)	 strcat(cTxt, "Supreme Blacksmith (Lvl.24)");
		if(iRank >= 121393)						 strcat(cTxt, "Legendary Blacksmith (Lvl.25)");*/
		wsprintfA(cTxt, "Blacksmith Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "Fishing", 7) == 0)
	{
		/*if(iRank >= 1 && iRank <=   1)			 strcat(cTxt, "Trainee Fisherman (Lvl.1)");
		if(iRank >= 2 && iRank <=   2)			 strcat(cTxt, "Initiate Fisherman (Lvl.2)");
		if(iRank >= 3 && iRank <    5)			 strcat(cTxt, "Beginner Fisherman (Lvl.3)");
		if(iRank >= 5 && iRank <    8)           strcat(cTxt, "Apprentice Fisherman (Lvl.4)");
		if(iRank >= 8 && iRank <   13)           strcat(cTxt, "Novice Fisherman (Lvl.5)");
		if(iRank >= 13 && iRank <  21)			 strcat(cTxt, "Novice Fisherman (Lvl.6)");
		if(iRank >= 21 && iRank <   34)          strcat(cTxt, "Novice Fisherman (Lvl.7)");
		if(iRank >= 34 && iRank <   55)			 strcat(cTxt, "Novice Fisherman (Lvl.8)");
		if(iRank >= 55 && iRank <   89)			 strcat(cTxt, "Novice Fisherman (Lvl.9)");
		if(iRank >= 89 && iRank <   144)		 strcat(cTxt, "Experienced Fisherman (Lvl.10)");
		if(iRank >= 144 && iRank <   233)		 strcat(cTxt, "Experienced Fisherman (Lvl.11)");
		if(iRank >= 233 && iRank <   377)		 strcat(cTxt, "Experienced Fisherman (Lvl.12)");
		if(iRank >= 377 && iRank <   610)		 strcat(cTxt, "Experienced Fisherman (Lvl.13)");
		if(iRank >= 610 && iRank <   987)		 strcat(cTxt, "Experienced Fisherman (Lvl.14)");
		if(iRank >= 987 && iRank <  1597)		 strcat(cTxt, "Master Fisherman (Lvl.15)");
		if(iRank >= 1597 && iRank <  2584)		 strcat(cTxt, "Master Fisherman (Lvl.16)");
		if(iRank >= 2584 && iRank <  4181)		 strcat(cTxt, "Master Fisherman (Lvl.17)");
		if(iRank >= 4181 && iRank <  6765)		 strcat(cTxt, "Master Fisherman (Lvl.18)");
		if(iRank >= 6765 && iRank < 10946)		 strcat(cTxt, "Master Fisherman (Lvl.19)");
		if(iRank >= 10946 && iRank < 17711)		 strcat(cTxt, "Grandmaster Fisherman (Lvl.20)");
		if(iRank >= 17711 && iRank < 28657)		 strcat(cTxt, "Supreme Fisherman (Lvl.21)");
		if(iRank >= 28657 && iRank < 46368)		 strcat(cTxt, "Supreme Fisherman (Lvl.22)");
		if(iRank >= 46368 && iRank < 75025)		 strcat(cTxt, "Supreme Fisherman (Lvl.23)");
		if(iRank >= 75025 && iRank < 121393)	 strcat(cTxt, "Supreme Fisherman (Lvl.24)");
		if(iRank >= 121393)						 strcat(cTxt, "Legendary Fisherman (Lvl.25)");*/
		wsprintfA(cTxt, "Fisherman Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "Farming", 7) == 0)
	{
		/*if(iRank >= 1 && iRank <=   1)			 strcat(cTxt, "Trainee Farmer (Lvl.1)");
		if(iRank >= 2 && iRank <=   2)			 strcat(cTxt, "Initiate Farmer (Lvl.2)");
		if(iRank >= 3 && iRank <    5)			 strcat(cTxt, "Beginner Farmer (Lvl.3)");
		if(iRank >= 5 && iRank <    8)           strcat(cTxt, "Apprentice Farmer (Lvl.4)");
		if(iRank >= 8 && iRank <   13)           strcat(cTxt, "Novice Farmer (Lvl.5)");
		if(iRank >= 13 && iRank <  21)			 strcat(cTxt, "Novice Farmer (Lvl.6)");
		if(iRank >= 21 && iRank <   34)          strcat(cTxt, "Novice Farmer (Lvl.7)");
		if(iRank >= 34 && iRank <   55)			 strcat(cTxt, "Novice Farmer (Lvl.8)");
		if(iRank >= 55 && iRank <   89)			 strcat(cTxt, "Novice Farmer (Lvl.9)");
		if(iRank >= 89 && iRank <   144)		 strcat(cTxt, "Experienced Farmer (Lvl.10)");
		if(iRank >= 144 && iRank <   233)		 strcat(cTxt, "Experienced Farmer (Lvl.11)");
		if(iRank >= 233 && iRank <   377)		 strcat(cTxt, "Experienced Farmer (Lvl.12)");
		if(iRank >= 377 && iRank <   610)		 strcat(cTxt, "Experienced Farmer (Lvl.13)");
		if(iRank >= 610 && iRank <   987)		 strcat(cTxt, "Experienced Farmer (Lvl.14)");
		if(iRank >= 987 && iRank <  1597)		 strcat(cTxt, "Master Farmer (Lvl.15)");
		if(iRank >= 1597 && iRank <  2584)		 strcat(cTxt, "Master Farmer (Lvl.16)");
		if(iRank >= 2584 && iRank <  4181)		 strcat(cTxt, "Master Farmer (Lvl.17)");
		if(iRank >= 4181 && iRank <  6765)		 strcat(cTxt, "Master Farmer (Lvl.18)");
		if(iRank >= 6765 && iRank < 10946)		 strcat(cTxt, "Master Farmer (Lvl.19)");
		if(iRank >= 10946 && iRank < 17711)		 strcat(cTxt, "Grandmaster Farmer (Lvl.20)");
		if(iRank >= 17711 && iRank < 28657)		 strcat(cTxt, "Supreme Farmer (Lvl.21)");
		if(iRank >= 28657 && iRank < 46368)		 strcat(cTxt, "Supreme Farmer (Lvl.22)");
		if(iRank >= 46368 && iRank < 75025)		 strcat(cTxt, "Supreme Farmer (Lvl.23)");
		if(iRank >= 75025 && iRank < 121393)	 strcat(cTxt, "Supreme Farmer (Lvl.24)");
		if(iRank >= 121393)						 strcat(cTxt, "Legendary Farmer (Lvl.25)");*/
		wsprintfA(cTxt, "Farmer Lv.%d", iRank);
	}
	else if (memcmp(cSubject, "Abaddon", 7) == 0 || memcmp(cSubject, "Wyvern", 6) == 0 
		|| memcmp(cSubject, "FireWyvern", 10) == 0)
	{
		strcpy(cTxt, cSubject);
		//if(iNumber >= 20 && iNumber <=   50)		 strcat(cTxt, " Lvl.1");
		//if(iNumber > 50 && iNumber <=   75)			 strcat(cTxt, " Lvl.2");
		//if(iNumber >  75 && iNumber <=  100)		 strcat(cTxt, " Lvl.3");
		//if(iNumber > 100 && iNumber <=  125)         strcat(cTxt, " Lvl.4");
		//if(iNumber > 125 && iNumber <=  175)         strcat(cTxt, " Lvl.5");
		//if(iNumber > 175 && iNumber <= 250)			 strcat(cTxt, " Lvl.6");
		//if(iNumber > 250 && iNumber <= 500)          strcat(cTxt, " Lvl.7");
		//if(iNumber > 500 && iNumber <= 1000)		 strcat(cTxt, " Lvl.8");
		//if(iNumber > 1000)							 strcat(cTxt, " Lvl.9");
		wsprintfA(cTxt, "%s Slayer Lv.%d", cSubject, iRank);
	}
	else
	{
		strcpy(cTxt, cSubject);
		/*if(iRank >= 1000 && iRank < 5000)      wsprintfA(cTxt, "Initiate %s Hunter", cSubject);
		if(iRank >= 5000 && iRank < 10000)     wsprintfA(cTxt, "Initiate %s Hunter", cSubject);
		if(iRank >= 10000 && iRank < 20000)    wsprintfA(cTxt, "Beginner %s Hunter", cSubject);
		if(iRank >= 20000 && iRank < 50000)    wsprintfA(cTxt, "Novice %s Hunter", cSubject);
		if(iRank >= 50000 && iRank < 100000)   wsprintfA(cTxt, "Experienced %s Hunter", cSubject);
		if(iRank >= 100000 && iRank < 250000)  wsprintfA(cTxt, "Master %s Hunter", cSubject);
		if(iRank >= 250000 && iRank < 500000)  wsprintfA(cTxt, "Grand Master %s Slayer", cSubject);
		if(iRank >= 500000 && iRank < 750000)  wsprintfA(cTxt, "Supreme %s Slayer", cSubject);
		if(iRank >= 750000)					   wsprintfA(cTxt, "Legendary %s Slayer", cSubject);*/
		wsprintfA(cTxt, "%s Slayer Lv.%d", cSubject, iRank);
	}

	

	strcpy(pStr1, cTxt);
}

int CGame::iGetNextTitle(char * cSubject, uint32_t iNumber)
{

 char cTxt[256];

	ZeroMemory(cTxt, sizeof(cTxt));

	if (memcmp(cSubject, "Events", 6) == 0)
	{
		if(iNumber >   0  && iNumber < 10)     return 10;
		if(iNumber >= 10 && iNumber < 25)      return 25;
		if(iNumber >= 25 && iNumber < 50)      return 50;
		if(iNumber >= 50 && iNumber < 100)     return 100;
		if(iNumber >= 100 && iNumber < 250)    return 250;
		if(iNumber >= 250 && iNumber < 500)    return 500;
		if(iNumber >= 500 && iNumber < 1000)   return 1000;
		if(iNumber >= 1000 && iNumber < 2500)  return 2500;
		if(iNumber >= 2500 && iNumber < 5000)  return 5000;
		if(iNumber >= 5000)					   return 9999;
	}
	else if (memcmp(cSubject, "Donation", 8) == 0)
	{
		if(iNumber >  0 && iNumber < 5)			return 5;
		if(iNumber >= 5 && iNumber < 10)		return 10;
		if(iNumber >= 10 && iNumber < 20)		return 20;
		if(iNumber >= 20 && iNumber < 50)		return 50;
		if(iNumber >= 50 && iNumber < 100)		return 100;
		if(iNumber >= 100 && iNumber < 175)		return 175;
		if(iNumber >= 175 && iNumber < 250)		return 250;
		if(iNumber >= 250 && iNumber < 500)		return 500;
		if(iNumber >= 500)					    return 999;
	}
	else if (memcmp(cSubject, "Hero", 4) == 0)
	{
		if(iNumber >= 0  && iNumber < 5)		return 5;
		if(iNumber >= 5  && iNumber < 25)		return 25;
		if(iNumber >= 25 && iNumber < 50)		return 50;
		if(iNumber >= 50 && iNumber < 150)		return 150;
		if(iNumber >= 150 && iNumber < 300)		return 300;
		if(iNumber >= 300 && iNumber < 500)		return 500;
		if(iNumber >= 500 && iNumber < 1500)	return 1500;
		if(iNumber >= 1500 && iNumber < 3000)	return 3000;
		if(iNumber >= 3000 && iNumber < 5000)	return 5000;
		if(iNumber >= 5000 && iNumber < 7500)	return 7500;
		if(iNumber >= 7500 && iNumber < 10000)	return 10000;
		if(iNumber >= 10000 && iNumber < 20000)	return 20000;
		if(iNumber >= 20000 && iNumber < 50000)	return 50000;
		if(iNumber >= 50000)					return 99999;
	}
    else if (memcmp(cSubject, "Gladiator", 9) == 0 || memcmp(cSubject, "Constructor", 11) == 0)
	{
		if(iNumber >= 0 && iNumber <    2)		 return 3;
		if(iNumber >= 2 && iNumber <    5)		 return 5;
		if(iNumber >= 5 && iNumber <    10)		 return 10;
		if(iNumber >= 10 && iNumber <   25)		 return 25;
		if(iNumber >= 25 && iNumber <   50)		 return 50;
		if(iNumber >= 50 && iNumber <   100)	 return 100;
		if(iNumber >= 100 && iNumber <  250)	 return 250;
		if(iNumber >= 250 && iNumber <  500)	 return 500;
		if(iNumber >= 500 && iNumber <  1000)	 return 1000;
		if(iNumber >= 1000 && iNumber < 2500)	 return 2500;
		if(iNumber >= 2500 && iNumber < 5000)	 return 5000;
		if(iNumber >= 5000)					     return 9999;
	}
	else if (memcmp(cSubject, "Treasure", 8) == 0)
	{
		if(iNumber >   0 && iNumber <    10)	 return 10;
		if(iNumber >= 10 && iNumber <   25)		 return 25;
		if(iNumber >= 25 && iNumber <   50)		 return 50;
		if(iNumber >= 50 && iNumber <   100)	 return 100;
		if(iNumber >= 100 && iNumber <  250)	 return 250;
		if(iNumber >= 250 && iNumber <  500)	 return 500;
		if(iNumber >= 500 && iNumber <  1000)	 return 1000;
		if(iNumber >= 1000 && iNumber < 2500)	 return 2500;
		if(iNumber >= 2500 && iNumber < 5000)	 return 5000;
		if(iNumber >= 5000)					     return 9999;
	}
	else if (memcmp(cSubject, "Drunkard", 8) == 0 || memcmp(cSubject, "SweetTooth", 10) == 0)
	{
		if(iNumber >= 0 && iNumber < 5)			 return 5;
		if(iNumber >= 5 && iNumber <  10)		 return 10;
		if(iNumber >= 10 && iNumber <   20)		 return 20;
		if(iNumber >= 20 && iNumber <  40)		 return 40;
		if(iNumber >= 40 && iNumber <  80)	     return 80;
		if(iNumber >= 80 && iNumber <  160)		 return 160;
		if(iNumber >= 160 && iNumber < 320)		 return 320;
		if(iNumber >= 320 && iNumber < 640)		 return 640;
		if(iNumber >= 640 && iNumber < 1280)	 return 1280;
		if(iNumber >= 1280 && iNumber < 2560)	 return 2560;
		if(iNumber >= 2560 && iNumber < 5120)	 return 5120;
		if(iNumber >= 5120 && iNumber < 10240)	 return 10240;
		if(iNumber >= 10240)					 return 10240;
	}
	else if (memcmp(cSubject, "Suicide", 7) == 0)
	{
		if(iNumber >= 0 && iNumber <    10)	     return 10;
		if(iNumber >= 10 && iNumber <   25)		 return 25;
		if(iNumber >= 25 && iNumber <   50)	     return 50;
		if(iNumber >= 50 && iNumber <   100)	 return 100;
		if(iNumber >= 100 && iNumber <  250)	 return 250;
		if(iNumber >= 250 && iNumber <  500)	 return 500;
		if(iNumber >= 500 && iNumber <  1000)	 return 1000;
		if(iNumber >= 1000 && iNumber < 1750)	 return 1750;
		if(iNumber >= 1750 && iNumber < 2500)	 return 2500;
		if(iNumber >= 2500 && iNumber < 5000)	 return 5000;
		if(iNumber >= 5000 && iNumber < 10000)	 return 10000;
		if(iNumber >= 10000)					 return 99999;
	}
	else if (memcmp(cSubject, "Herbalist", 9) == 0 || memcmp(cSubject, "Miner", 5) == 0 || memcmp(cSubject, "Crafting", 8) == 0 || memcmp(cSubject, "Smither", 7) == 0
				 || memcmp(cSubject, "Fisher", 6) == 0 || memcmp(cSubject, "Farmer", 6) == 0)
	{
		if(iNumber == 0)							 return 1;
		if(iNumber >= 1 && iNumber <=   1)			 return 2;
		if(iNumber >= 2 && iNumber <=   2)			 return 3;
		if(iNumber >= 3 && iNumber <    5)           return 5;
		if(iNumber >= 5 && iNumber <    8)           return 8;
		if(iNumber >= 8 && iNumber <  13)			 return 13;//besk rank 5?
		if(iNumber >= 13 && iNumber <   21)          return 21;
		if(iNumber >= 21 && iNumber <   34)			 return 34;
		if(iNumber >= 34 && iNumber <   55)			 return 55;
		if(iNumber >= 55 && iNumber <   89)			 return 89;
		if(iNumber >= 89 && iNumber <   144)		 return 144;//besk rank 10
		if(iNumber >= 144 && iNumber <   233)		 return 233;
		if(iNumber >= 233 && iNumber <   377)		 return 377;
		if(iNumber >= 377 && iNumber <   610)		 return 610;
		if(iNumber >= 610 && iNumber <   987)		 return 987;
		if(iNumber >= 987 && iNumber <  1597)		 return 1597;//besk rank 15
		if(iNumber >= 1597 && iNumber <  2584)		 return 2584;
		if(iNumber >= 2584 && iNumber <  4181)		 return 4181;
		if(iNumber >= 4181 && iNumber <  6765)		 return 6765;
		if(iNumber >= 6765 && iNumber < 10946)		 return 10946;
		if(iNumber >= 10946 && iNumber < 17711)		 return 17711;//besk rank 20
		if(iNumber >= 17711 && iNumber < 28657)		 return 28657;
		if(iNumber >= 28657 && iNumber < 46368)		 return 46368;
		if(iNumber >= 46368 && iNumber < 75025)		 return 75025;
		if(iNumber >= 75025 && iNumber < 121393)	 return 121393;
		if(iNumber >= 121393)						 return 999999;//besk rank 25
	}
	else if (memcmp(cSubject, "Abaddon", 7) == 0 || memcmp(cSubject, "Wyvern", 6) == 0 
		|| memcmp(cSubject, "FireWyvern", 10) == 0)
	{
		if(iNumber >= 0	&& iNumber <=  20)		     return 20;
		if(iNumber > 20 && iNumber <=   50)			 return 50;
		if(iNumber > 50 && iNumber <=   85)			 return 85;
		if(iNumber > 85 && iNumber <=  130)			 return 130;
		if(iNumber > 130 && iNumber <=  170)         return 170;
		if(iNumber > 170 && iNumber <=  235)         return 235;
		if(iNumber > 235 && iNumber <= 385)			 return 385;
		if(iNumber > 250 && iNumber <= 550)          return 550;
		if(iNumber > 550 && iNumber <= 1200)		 return 1200;
		if(iNumber > 1200)							 return 2500;
	}
	else
	{
		if(iNumber >= 1000 && iNumber < 5000)      return 5000;
		if(iNumber >= 5000 && iNumber < 10000)     return 10000;
		if(iNumber >= 10000 && iNumber < 20000)    return 20000;
		if(iNumber >= 20000 && iNumber < 50000)    return 50000;
		if(iNumber >= 50000 && iNumber < 100000)   return 100000;
		if(iNumber >= 100000 && iNumber < 175000)  return 175000;
		if(iNumber >= 175000 && iNumber < 275000)  return 275000;
		if(iNumber >= 275000 && iNumber < 550000) return 550000;
		if(iNumber >= 550000)					   return 750000;
	}

	

	return 999999;
}

void CGame::ClearPlayerTitle(char * cName)
{
	int i;

	for( i=0 ; i < MAXGUILDNAMES ; i++ )
	{
		
		if(memcmp(cName, m_stTitles[i].cCharName, 10) == 0 )
		{
			strcpy(m_stTitles[i].cSubject, "NONE");
			ZeroMemory(m_stTitles[i].cCharName, sizeof(m_stTitles[i].cCharName));
			strcpy(m_stTitles[i].cCharName, "NONE");
		}
	}
}

int CGame::getTitleIndexFromSubject(char * cSubject)
{
	if (strcmp(cSubject, "NONE") == 0) {
		return -1;
	} else if (strcmp(cSubject, "Slime") == 0) {
		return 0;
	} else if (strcmp(cSubject, "GiantAnt") == 0) {
		return 1;
	} else if (strcmp(cSubject, "Amphis") == 0) {
		return 2;
	} else if (strcmp(cSubject, "Orc") == 0) {
		return 3;
	} else if (strcmp(cSubject, "OrcMage") == 0) {
		return 4;
	} else if (strcmp(cSubject, "Zombie") == 0) {
		return 5;
	} else if (strcmp(cSubject, "Skeleton") == 0) {
		return 6;
	} else if (strcmp(cSubject, "Scorpion") == 0) {
		return 7;
	} else if (strcmp(cSubject, "ClayGolem") == 0) {
		return 8;
	} else if (strcmp(cSubject, "StoneGolem") == 0) {
		return 9;
	} else if (strcmp(cSubject, "Helhound") == 0) {
		return 10;
	} else if (strcmp(cSubject, "GiantFrog") == 0) {
		return 11;
	} else if (strcmp(cSubject, "Rudolf") == 0) {
		return 12;
	} else if (strcmp(cSubject, "Troll") == 0) {
		return 13;
	} else if (strcmp(cSubject, "Cyclops") == 0) {
		return 14;
	} else if (strcmp(cSubject, "IceGolem") == 0) {
		return 15;
	} else if (strcmp(cSubject, "Beholder") == 0) {
		return 16;
	} else if (strcmp(cSubject, "CannibalPlant") == 0) {
		return 17;
	} else if (strcmp(cSubject, "Ogre") == 0) {
		return 18;
	} else if (strcmp(cSubject, "DireBoar") == 0) {
		return 19;
	} else if (strcmp(cSubject, "Tentocle") == 0) {
		return 20;
	} else if (strcmp(cSubject, "GiantCrawfish") == 0) {
		return 21;
	} else if (strcmp(cSubject, "MountainGiant") == 0) {
		return 22;
	} else if (strcmp(cSubject, "GiantPlant") == 0) {
		return 23;
	} else if (strcmp(cSubject, "Liche") == 0) {
		return 24;
	} else if (strcmp(cSubject, "Stalker") == 0) {
		return 25;
	} else if (strcmp(cSubject, "Werewolf") == 0) {
		return 26;
	} else if (strcmp(cSubject, "DarkElf") == 0) {
		return 27;
	} else if (strcmp(cSubject, "Frost") == 0) {
		return 28;
	} else if (strcmp(cSubject, "ClawTurtle") == 0) {
		return 29;
	} else if (strcmp(cSubject, "Ettin") == 0) {
		return 30;
	} else if (strcmp(cSubject, "Demon") == 0) {
		return 31;
	} else if (strcmp(cSubject, "Unicorn") == 0) {
		return 32;
	} else if (strcmp(cSubject, "Gargoyle") == 0) {
		return 33;
	} else if (strcmp(cSubject, "GiantLizard") == 0) {
		return 34;
	} else if (strcmp(cSubject, "Lizie") == 0) {
		return 35;
	} else if (strcmp(cSubject, "Hellclaw") == 0) {
		return 36;
	} else if (strcmp(cSubject, "Tigerworm") == 0) {
		return 37;
	} else if (strcmp(cSubject, "Wyvern") == 0) {
		return 38;
	} else if (strcmp(cSubject, "FireWyvern") == 0) {
		return 39;
	} else if (strcmp(cSubject, "Abbadon") == 0) {
		return 40;
	} else if (strcmp(cSubject, "Centaurus") == 0) {
		return 41;
	} else if (strcmp(cSubject, "MasterMageOrc") == 0) {
		return 42;
	} else if (strcmp(cSubject, "Minotaur") == 0) {
		return 43;
	} else if (strcmp(cSubject, "Mining") == 0) {
		return 44;
	} else if (strcmp(cSubject, "Fishing") == 0) {
		return 45;
	} else if (strcmp(cSubject, "Farming") == 0) {
		return 46;
	} else if (strcmp(cSubject, "Alchemy") == 0) {
		return 47;
	} else if (strcmp(cSubject, "Blacksmithing") == 0) {
		return 48;
	} else if (strcmp(cSubject, "Gemcutting") == 0) {
		return 49;
	} /*else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	} else if (strcmp(cSubject, "xxxxxxxxx") == 0) {
		return 0000000;
	}	
	
	
	*/
	return -1;


}
#endif

void CGame::_LoadItemContents()
{
 char cFileName[255], cTemp[255];
 HANDLE hFile;
 FILE * pFile;
 uint32_t  dwFileSize;
 char * pBuffer;

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));
	strcat(cFileName, "data\\shops\\ItemConfigs\\ItemConfig1.txt");

	hFile = CreateFileA(cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	dwFileSize = GetFileSize(hFile, 0);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFileName, "rt");
	if (pFile == 0) return;
	else {
		pBuffer = new char[dwFileSize+1];
		ZeroMemory(pBuffer, dwFileSize+1);
		fread(pBuffer, dwFileSize, 1, pFile);

		__bDecodeContentsAndBuildItemList(pBuffer);
		isItemLoaded = true;
		delete[] pBuffer;
	}
	fclose(pFile);
}

bool CGame::__bDecodeContentsAndBuildItemList(char * pBuffer)
{
 char * pContents, * token;
 char seps[] = "= ,\t\n";
 char cReadModeA = 0;
 char cReadModeB = 0;
 int  iItemListIndex=0;
 class CStrTok * pStrTok;

	pContents = pBuffer;

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while( token != 0 ) {
		if (cReadModeA != 0) {
			//
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					m_pBuildItemListStats[iItemListIndex]->ItemUniqueID = atoi(token);
					cReadModeB = 2;
					break;
				case 2:
					ZeroMemory(m_pBuildItemListStats[iItemListIndex]->m_cName, sizeof(m_pBuildItemListStats[iItemListIndex]->m_cName));
					memcpy(m_pBuildItemListStats[iItemListIndex]->m_cName, token, strlen(token));
					cReadModeB = 3;
					break;
				case 3:	// m_cItemType
					m_pBuildItemListStats[iItemListIndex]->m_cItemType = atoi(token);
					cReadModeB = 4;
					break;
				case 4: // m_cEquipPos
					m_pBuildItemListStats[iItemListIndex]->m_cEquipPos = atoi(token);
					cReadModeB = 5;
					break;
				case 5: // m_sItemEffectType
					//m_pBuildItemListStats[iItemListIndex]->m_sItemEffectType = atoi(token);
					cReadModeB = 6;
					break;
				case 6:	// m_sItemEffectValue1
					m_pBuildItemListStats[iItemListIndex]->m_sItemEffectValue1 = atoi(token);
					cReadModeB = 7;
					break;
				case 7: // m_sItemEffectValue2
					m_pBuildItemListStats[iItemListIndex]->m_sItemEffectValue2 = atoi(token);
					cReadModeB = 8;
					break;
				case 8: // m_sItemEffectValue3
					m_pBuildItemListStats[iItemListIndex]->m_sItemEffectValue3 = atoi(token);
					cReadModeB = 9;
					break;
				case 9: // m_sItemEffectValue4
					m_pBuildItemListStats[iItemListIndex]->m_sItemEffectValue4 = atoi(token);
					cReadModeB = 10;
					break;
				case 10: // m_sItemEffectValue5
					m_pBuildItemListStats[iItemListIndex]->m_sItemEffectValue5 = atoi(token);
					cReadModeB = 11;
					break;
				case 11: // m_sItemEffectValue6
					m_pBuildItemListStats[iItemListIndex]->m_sItemEffectValue6 = atoi(token);
					cReadModeB = 12;
					break;
				case 12: // m_wMaxLifeSpan
					m_pBuildItemListStats[iItemListIndex]->m_wMaxLifeSpan = (uint16_t)atoi(token);
					cReadModeB = 13;
					break;
				case 13: // m_sMaxFixCount
					//m_pBuildItemListStats[iItemListIndex]->m_sMaxFixCount = atoi(token);
					cReadModeB = 14;
					break;
				case 14: // m_sSprite
					m_pBuildItemListStats[iItemListIndex]->m_sSprite = atoi(token);
					cReadModeB = 15;
					break;
				case 15: // m_sSpriteFrame
					m_pBuildItemListStats[iItemListIndex]->m_sSpriteFrame = atoi(token);
					cReadModeB = 16;
					break;
				case 16: // m_wPrice
					m_pBuildItemListStats[iItemListIndex]->m_wPrice = atoi(token);
					cReadModeB = 17;
					break;
				case 17: // m_wWeight
					m_pBuildItemListStats[iItemListIndex]->m_wWeight = atoi(token);
					cReadModeB = 18;
					break;
				case 18: // Appr Value
					//m_pBuildItemListStats[iItemListIndex]->m_cApprValue = atoi(token);
					cReadModeB = 19;
					break;
				case 19: // m_cSpeed
					m_pBuildItemListStats[iItemListIndex]->m_cSpeed = atoi(token);
					cReadModeB = 20;
					break;
				case 20: // Level Limit
					m_pBuildItemListStats[iItemListIndex]->m_sLevelLimit = atoi(token);
					m_pBuildItemListStats[iItemListIndex]->m_dwCount = 1;
					cReadModeA = 0;
					cReadModeB = 0;
					iItemListIndex++;
					break;
				}
				break;

			default:
				break;
			}
		}else
		{
			if (memcmp(token, "ItemForSale", 4) == 0)
			{
				if (iItemListIndex >= MAXITEMSTATS)
				{
					delete pStrTok;
					return false;
				}
				cReadModeA = 1;
				cReadModeB = 1;
				m_pBuildItemListStats[iItemListIndex] = new class CItem;
		}	}
		token = pStrTok->pGet();
	}
	delete pStrTok;
	if ((cReadModeA != 0) || (cReadModeB != 0)) return false;
	return true;
}

void CGame::isValue(uint32_t value)
{
	if(value < 30) {
		isCommon = true;
		m_itemColor =video::SColor(255,255,255,255);
		isUncommon = isRare = isEpic = isLegendary = false;
	} else if((value >= 30) && (value < 60)) {
		isUncommon = true;
		m_itemColor =video::SColor(255,0,200,0);
		isCommon = isRare = isEpic = isLegendary = false;
	} else if((value >= 60) && (value < 80)) {
		isRare = true;
		m_itemColor =video::SColor(255,0,0,200);
		isCommon = isUncommon = isEpic = isLegendary = false;
	} else if((value >= 80) && (value < 100)) {
		isEpic = true;
		m_itemColor =video::SColor(255,225,191,0);
		isCommon = isUncommon = isRare = isLegendary = false;
	} else if((value >= 100)) {
		isLegendary = true;
		m_itemColor =video::SColor(255,225,132,0);
		isCommon = isUncommon = isRare = isEpic = false;
	}
}

void CGame::HandleItemDescription(CItem * item)
{
	if (strcmp(item->m_cName,"RingofOgrepower") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MELEEDAMAGE, 40);
	}else if (strcmp(item->m_cName,"RingofDemonpower") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MELEEDAMAGE, 50);
	}else if (strcmp(item->m_cName,"RingofDragonpower") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MELEEDAMAGE, 60);
		m_itemColor =video::SColor(255,0,200,0);
	}else if (strcmp(item->m_cName,"RingoftheXelima") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MELEEDAMAGE, 70);
		m_itemColor =video::SColor(255,0,0,200);
	}else if (strcmp(item->m_cName,"RingoftheAbaddon") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MELEEDAMAGE, 100);
		m_itemColor =video::SColor(255,225,191,0);
	}else if (strcmp(item->m_cName,"RingofWizard") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MAGICDAMAGE, 5);
	}else if (strcmp(item->m_cName,"RingofMage") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MAGICDAMAGE, 10);
		m_itemColor =video::SColor(255,0,200,0);
	}else if (strcmp(item->m_cName,"RingofGrandMage") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MAGICDAMAGE, 15);
		m_itemColor =video::SColor(255,0,0,200);
	}else if (strcmp(item->m_cName,"RingofArchmage") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MAGICDAMAGE, 20);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "NecklaceOfMerien") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_DEFENSE, 500);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "MerienShield") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MERIENSHIELD, 60);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "MerienPlateMailM") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MERIENARMOR, 60);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "MerienPlateMailW") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MERIENARMOR, 60);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "MagicWand(M.Shield)") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MSHIELDWAND, 14);
	}else if(strcmp(item->m_cName, "NecklaceOfKloness") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_REPDAMAGE, 1, 100);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "KlonessWand(MS.20)") == 0 || strcmp(item->m_cName, "KlonessWand(MS.10)") == 0 || strcmp(item->m_cName, "KlonessEsterk") == 0
			|| strcmp(item->m_cName, "KlonessBlade") == 0 || strcmp(item->m_cName, "KlonessAxe") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_REPDAMAGE, 1, 100);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "NecklaceOfXelima") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_XELIMANECK, 160);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "XelimaBlade") == 0 || strcmp(item->m_cName, "XelimaAxe") == 0 || strcmp(item->m_cName, "XelimaRapier") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_XELIMAWEAPON, 50);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "Excaliber") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_EXCALIBER);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "DarkElfBow") == 0){
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "NecklaceOfLightPro") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_AIRABSORBNECK, 25);
		m_itemColor =video::SColor(255,0,200,0);
	}else if(strcmp(item->m_cName, "NecklaceOfAirEle") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_AIRABSORBNECK, 50);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "NecklaceOfFirePro") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_FIREABSORBNECK, 25);
		m_itemColor =video::SColor(255,0,200,0);
	}else if(strcmp(item->m_cName, "NecklaceOfEfreet") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_FIREABSORBNECK, 50);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "NecklaceOfIcePro") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ICEABSORBNECK1, 25, 50);
		m_itemColor =video::SColor(255,0,200,0);
	}else if(strcmp(item->m_cName, "NecklaceOfIceEle") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ICEABSORBNECK2, 50);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "NecklaceOfPoisonPro") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_POISONABSORBNECK1, 50);
		m_itemColor =video::SColor(255,0,200,0);
	}else if(strcmp(item->m_cName, "NecklaceOfSufferent") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_POISONABSORBNECK2, 100);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "NecklaceOfBeholder") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_NECKOFBEHOLDER);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "NecklaceOfLiche") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MAGICSAVE, 35);
		m_itemColor =video::SColor(255,190,119,119);
	}else if(strcmp(item->m_cName, "NecklaceOfStoneGol") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_DEFENSE, 400);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "RingoftheXelima") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MELEEDAMAGE, 75);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "SwordofMedusa") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MEDUSASWORD, 60);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "SwordofIceElemental") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ICEELESWORD);
		m_itemColor =video::SColor(255,0,200,0);
	}else if(strcmp(item->m_cName, "DemonSlayer") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_DEMONSLAYER);
		m_itemColor =video::SColor(255,150,0,0);
	}else if(strcmp(item->m_cName, "StormBringer") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_STORMBRINGER);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "DarkExecutor") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_DARKEXECUTOR);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "LightingBlade") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_LIGHTINGBLADE);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "BerserkWand(MS.20)") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ZERKWAND, 30);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "BerserkWand(MS.10)") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ZERKWAND, 30);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "ResurWand(MS.20)") == 0 || strcmp(item->m_cName, "ResurWand(MS.10)") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_REZZWAND, 100, 100);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "LegendWand(MS25)") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_LEGENDWAND, 10);
	}else if(strcmp(item->m_cName, "CritPot") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_CRITPOT, 200);
	}else if(strcmp(item->m_cName, "RedPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_HPPOTION,15,30);
	}else if(strcmp(item->m_cName, "BigRedPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_HPPOTION,40,60);
	}else if(strcmp(item->m_cName, "BluePotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MPPOTION,15,30);
	}else if(strcmp(item->m_cName, "BigBluePotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MPPOTION,40,60);
	}else if(strcmp(item->m_cName, "GreenPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SPPOTION,15,30);
	}else if(strcmp(item->m_cName, "BigGreenPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SPPOTION,40,50);
	}else if(strcmp(item->m_cName, "DilutionPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_DELUTIONPOTION,20);
	}else if(strcmp(item->m_cName, "Baguette") == 0 || strcmp(item->m_cName, "Meat") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_FOOD,20,30,5);
	}else if(strcmp(item->m_cName, "ArcheryManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ARCHERYMANUAL, 20);
	}else if(strcmp(item->m_cName, "ShieldManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SHIELDMANUAL, 20);
	}else if(strcmp(item->m_cName, "LongSwordManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_LONGSWORDMANUAL, 20);
	}else if(strcmp(item->m_cName, "FencingManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_FENCINGMANUAL, 20);
	}else if(strcmp(item->m_cName, "FishingManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_FISHINGMANUAL, 20);
	}else if(strcmp(item->m_cName, "AxeManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_AXEMANUAL, 20);
	}else if(strcmp(item->m_cName, "MagicResistManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MAGICRESISTMANUAL, 20);
	}else if(strcmp(item->m_cName, "RecallScroll") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_RECALLSCROLL);
	}else if(strcmp(item->m_cName, "InvisibilityScroll") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_INVISSCROLL);
	}else if(strcmp(item->m_cName, "DetectInviScroll") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_DETECTINVISSCROLL);
	}else if(strcmp(item->m_cName, "GuildAdmissionTicket") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_GUILDADMISSION);
	}else if(strcmp(item->m_cName, "GuildSucessionTicket") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_GUILDWITHDRAWAL);
	}else if(strcmp(item->m_cName, "FlowerBasket") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_BASKETFLOWERS);
	}else if(strcmp(item->m_cName, "Flowerpot") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_POTFLOWERS);
	}else if(strcmp(item->m_cName, "Map") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MAP);
	}else if(strcmp(item->m_cName, "FishingRod") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_FISHINGROD);
	}else if(strcmp(item->m_cName, "PretendCorpseManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_PRETENDCORPSEMANUAL, 20);
	}else if(strncmp(item->m_cName, "ArenaTicket", 11) == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ARENATICKETS);
	}else if(strcmp(item->m_cName, "AlchemyBowl") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ALCHEMYBOWL);
	}else if(strcmp(item->m_cName, "PickAxe") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_PICKAXE);
	}else if(strcmp(item->m_cName, "Hoe") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_HOE);
	}else if(strcmp(item->m_cName, "ManufacturingHammer") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SMITHSANVIL);
	}else if(strcmp(item->m_cName, "HammerAttackManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_HAMMERATTACKMANUAL, 20);
	}else if(strcmp(item->m_cName, "AresdenFlag(Master)") == 0 || strcmp(item->m_cName, "ElvineFlag(Master)") == 0 || strcmp(item->m_cName, "AresdenFlag") == 0
			|| strcmp(item->m_cName, "ElvineFlag") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_EVENTITEM);
	}else if(strcmp(item->m_cName, "StaffAttackManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_STAFFATTACKMANUAL, 20);
	}else if(strcmp(item->m_cName, "HairColorPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_HAIRCOLORPOTION);
	}else if(strcmp(item->m_cName, "HairStylePotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_HAIRSTYLEPOTION);
	}else if(strcmp(item->m_cName, "SkinColorPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SKINCOLORPOTION);
	}else if(strcmp(item->m_cName, "InvisibilityPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_INVISPOTION);
	}else if(strcmp(item->m_cName, "SexChangePotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SEXCHANGEPOTION);
	}else if(strcmp(item->m_cName, "OgrePotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_OGREPOTION);
	}else if(strcmp(item->m_cName, "UnderWearPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_UNDERWEARPOTION);
	}else if(strncmp(item->m_cName, "Dye", 3) == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_CLOTHINGDYE);
	}else if(strcmp(item->m_cName, "DecolorationPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_DECOLORATIONPOTION);
	}else if(strcmp(item->m_cName, "HandAttackManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_HANDATTACKMANUAL, 20);
	}else if(strcmp(item->m_cName, "ShortSwordManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SHORTSWORDMANUAL, 20);
	}else if(strcmp(item->m_cName, "PowerGreenPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_POWERGREENPOTION, 15);
	}else if(strcmp(item->m_cName, "SuperGreenPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_POWERGREENPOTION, 30);
	}else if(strcmp(item->m_cName, "BloodSword") == 0 || strcmp(item->m_cName, "BloodAxe") == 0 || strcmp(item->m_cName, "BloodRapier") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_BLOODWEAPON, 20);
		m_itemColor =video::SColor(255,125,0,0);
	}else if(strcmp(item->m_cName, "StoneOfXelima") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_XELIMASTONE);
	}else if(strcmp(item->m_cName, "StoneOfMerien") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MERIENSTONE);
	}else if(strcmp(item->m_cName, "UnfreezePotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_UNFREEZEPOTION);
	}else if(strncmp(item->m_cName, "SeedBag", 7) == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SEEDBAG);
	}else if(strcmp(item->m_cName, "SuperRedPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_HPPOTION,503,524);
	}else if(strcmp(item->m_cName, "SuperBluePotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MPPOTION,503,524);
	}else if(strcmp(item->m_cName, "SuperGreenPotion") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SPPOTION,504,532);
	}else if(strstr(item->m_cName, "ResetScroll") != 0 && strcmp(item->m_cName, "ResetScroll(ALL)") != 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_RESETSCROLL);
	}else if(strcmp(item->m_cName, "ResetScroll(ALL)") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_RESETSCROLLALL);
	}else if(strcmp(item->m_cName, "AlchemyManual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ALCHEMYMANUAL, 20);
	}else if(strcmp(item->m_cName, "MsRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MSRUNE);
	}else if(strcmp(item->m_cName, "DmgRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_DMGRUNE);
	}else if(strcmp(item->m_cName, "DefRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_DEFRUNE);
	}else if(strcmp(item->m_cName, "MrRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MRRUNE);
	}else if(strcmp(item->m_cName, "DiaRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_DIARUNE);
	}else if(strcmp(item->m_cName, "RubyRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_RUBYRUNE);
	}else if(strcmp(item->m_cName, "EmeRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_EMERUNE);
	}else if(strcmp(item->m_cName, "SapRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SAPRUNE);
	}else if(strcmp(item->m_cName, "MasterDefRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MASTERDEFRUNE);
	}else if(strcmp(item->m_cName, "MasterMrRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MASTERMRRUNE);
	}else if(strcmp(item->m_cName, "MasterDmgRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MASTERDMGRUNE);
	}else if(strcmp(item->m_cName, "MasterMsRune") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MASTERMSRUNE);
	}

	if(strcmp(item->m_cName, "RingOfWis") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_WISDOM, 50);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "NeckOfWis") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_WISDOM, 50);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "RingOfGWis") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_GREATERWISDOM, 100);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "NeckOfGWis") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_GREATERWISDOM, 100);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "RingOfAncWis") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ANCIENTWISDOM, 150);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "NeckOfAncWis") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ANCIENTWISDOM, 150);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "RingOfReg") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_REGENRATION, 20);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "NeckOfReg") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_REGENRATION, 25);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "RingOfGReg") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_GREATERREGENRATION, 45);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "NeckOfGReg") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_GREATERREGENRATION, 55);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "RingOfAncReg") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ANCIENTREGENRATION, 105);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "NeckOfAncReg") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ANCIENTREGENRATION, 125);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "WandOfWitch") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_WANDOFWITCH, 12);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "WandOfGWitch") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_WANDOFGWITCH, 14);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "WandOfAncWitch") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_WANDOFANCWITCH, 18);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "HamOfWrathran") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_HAMOFWRATHRAN);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "HamOfGWrathran") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_HAMOFWRATHRAN);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "HamOfAncWrathran") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_HAMOFWRATHRAN);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "G.Zerk.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_GZERKMANUAL);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "A.Zerk.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_AZERKMANUAL);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "Scan.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SCANMANUAL);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "I.S.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ISMANUAL);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "B.S.W.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_BSWMANUAL);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "M.H.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MHMANUAL);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "M.S.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MSMANUAL);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "C.O.T.G.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_COTGMANUAL);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "S.O.T.G.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_SOTGMANUAL);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "M.B.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MBMANUAL);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "F.S.W.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_FSWMANUAL);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "L.C.Manual") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_LCMANUAL);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "Greater(M.Shield)") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_GREATERMSHIELD, 22);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "MagicMissleBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ENERGYSTRIKEBS, 22);
		m_itemColor =video::SColor(255,255,255,255);
	}else if(strcmp(item->m_cName, "EnergyBoltBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ENERGYBOLTBS, 22);
		m_itemColor =video::SColor(255,255,255,255);
	}else if(strcmp(item->m_cName, "FireBallBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_FIREBALLBS, 22);
		m_itemColor =video::SColor(255,255,255,255);
	}else if(strcmp(item->m_cName, "GreatHealBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_GREATHEALBS, 22);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "FireStrikeBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_FIRESTRIKEBS, 22);
		m_itemColor =video::SColor(255,255,255,255);
	}else if(strcmp(item->m_cName, "TriEnergyBoltBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_TRIPLEENERGYBOLTBS, 22);
		m_itemColor =video::SColor(255,0,200,0);
	}else if(strcmp(item->m_cName, "ChillWindBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_CHILLWINDBS, 22);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "BerserkBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_BERSERKBS, 22);
		m_itemColor =video::SColor(255,0,0,200);
	}else if(strcmp(item->m_cName, "IceStrikeBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ICESTRIKEBS, 22);
		m_itemColor =video::SColor(255,0,200,0);
	}else if(strcmp(item->m_cName, "EnergyStrikeBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ENERGYSTRIKEBS, 22);
		m_itemColor =video::SColor(255,0,200,0);
	}else if(strcmp(item->m_cName, "M.ChillWindBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MASSCHILLWINDBS, 22);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "ArmorBreakBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ARMORBREAKBS, 22);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "G.BerserkBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_GREATBERSERKBS, 22);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "M.IceStrikeBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MASSICESTRIKEBS, 22);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "A.BerserkBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_ANCIENTBERSERKBS, 22);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "LightStrikeBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_LIGHTNINGSTRIKEBS, 22);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "MeteorStrikeBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_METEORSTRIKEBS, 22);
		m_itemColor =video::SColor(255,225,191,0);
	}else if(strcmp(item->m_cName, "MassHealBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_MASSHEALBS, 22);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "BlizzardBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_BLIZZARDBS, 22);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "E.S.WBS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_EARTHSHOCKWAVEBS, 22);
		m_itemColor =video::SColor(255,225,132,0);
	}/*else if(strcmp(item->m_cName, "BS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_BS, 22);
		m_itemColor =video::SColor(255,225,132,0);
	}else if(strcmp(item->m_cName, "BS") == 0){
		wsprintfA(G_cTxt, ITEM_DESCRIPTION_BS, 22);
		m_itemColor =video::SColor(255,225,132,0);
	}*/

	// String search classification
	if(strstr(item->m_cName, "Manual") != 0)
	{
		m_itemColor =video::SColor(255,150,241,255);
	} else if(strstr(item->m_cName, "Potion") != 0) {
		m_itemColor =video::SColor(255,0,186,255);
	} else if(strstr(item->m_cName, "Rune") != 0) {
		if(strstr(item->m_cName, "Master") != 0)
		{
			m_itemColor =video::SColor(255,225,132,0);
		} else {
			m_itemColor =video::SColor(255,225,191,0);
		}
	}

	int iValue = 0;
	char STAT[5];
	ZeroMemory(STAT, sizeof(STAT));
	if((memcmp(item->m_cName, "AngelicPendant(STR)", 19) == 0) || (memcmp(item->m_cName, "AngelicPendant(DEX)", 19) == 0)
		|| (memcmp(item->m_cName, "AngelicPendant(INT)", 19) == 0) || (memcmp(item->m_cName, "AngelicPendant(MAG)", 19) == 0)) {
		iValue = (item->m_dwAttribute & 0xF0000000) >> 28;
		if(memcmp(item->m_cName, "AngelicPendant(STR)", 19) == 0) {
			strcpy(STAT, "STR");
		} else if(memcmp(item->m_cName, "AngelicPendant(DEX)", 19) == 0) {
			strcpy(STAT, "DEX");
		} else if(memcmp(item->m_cName, "AngelicPendant(INT)", 19) == 0) {
			strcpy(STAT, "INT");
		} else if(memcmp(item->m_cName, "AngelicPendant(MAG)", 19) == 0) {
			strcpy(STAT, "MAG");
		}

		switch (iValue)
		{
		case 0:	wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 1:	wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 2:	wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 3:	wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 4:	wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 5:	wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 6:	wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 7:	wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 8:	wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 9:	wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 10: wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 11: wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 12: wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 13: wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 14: wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		case 15: wsprintfA(G_cTxt, "+%i %s", 12*iValue, STAT); break;
		}
	}
}

// Beholder Necklace Fix xRisenx
bool CGame::bCheckItemEquiped(char itemName[])
{
	for (int i = 0; i < MAXITEMS; i++)
	{
		if (m_pItemList[i] != 0)
		{
			if (strcmp(m_pItemList[i]->m_cName, itemName) == 0)
			{
			for (int x = 0; x < MAXITEMEQUIPPOS; x++)
	{
	if (m_sItemEquipmentStatus[x] == i)
	return true;
	}
			}
		}
	}
	return false;
}
	// Monster kill event xRisenx
void CGame::NotifyMsg_NpcHuntingON()
{
	SetTopMsg("Npc Hunting Game Activated!", 10);
}

void CGame::NotifyMsg_NpcHuntingOFF()
{
	SetTopMsg("Npc Hunting Game Deacticated!", 10);
}

void CGame::NotifyMsg_NpcHuntingWinner(char * pData)
{
	char * cp;
	int * ip;
	uint32_t * dwp;

	cp = (char *)(pData    + INDEX2_MSGTYPE + 2);
	ip = (int *)cp;
	m_iContribution = *ip;
	cp += 4;
	ip = (int *)cp;
	m_iRating = *ip;
	cp += 4;
}
	// Monster kill event xRisenx

