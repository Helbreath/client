// -------------------------------------------------------------- 
//                      Helbreath Client 						  
//
//                      1998.10 by Soph
//
// --------------------------------------------------------------


#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h> 
#include <winbase.h>
#include <mmsystem.h>
#include <process.h>
#include <tlhelp32.h> 
#include "..\res\resource.h"
#include "..\net\XSocket.h"
#include "winmain.h"
#include "..\Game.h"
#include "..\GlobalDef.h"
#include <iostream>
#include <string>
#include <stdint.h>
#include <irrlicht.h>
#include <sys/timeb.h>

extern "C" __declspec( dllimport) int __FindHackingDll__(char *);

// --------------------------------------------------------------

#define WM_USER_TIMERSIGNAL		WM_USER + 500
#define WM_USER_CALCSOCKETEVENT WM_USER + 600

int				G_iAddTable31[64][510], G_iAddTable63[64][510]; 
int				G_iAddTransTable31[510][64], G_iAddTransTable63[510][64]; 

long    G_lTransG100[64][64], G_lTransRB100[64][64];
long    G_lTransG70[64][64], G_lTransRB70[64][64];
long    G_lTransG50[64][64], G_lTransRB50[64][64];
long    G_lTransG25[64][64], G_lTransRB25[64][64];
long    G_lTransG2[64][64], G_lTransRB2[64][64];

HHOOK		g_keyboardHook;
char			szAppClass[32];
HWND			G_hWnd = NULL;
HWND			G_hEditWnd = NULL;
HINSTANCE       G_hInstance = NULL;
MMRESULT		G_mmTimer;
char   G_cSpriteAlphaDegree;
class CGame * G_pGame;
class XSocket * G_pCalcSocket = NULL;
BOOL  G_bIsCalcSocketConnected = TRUE;
DWORD G_dwCalcSocketTime = NULL, G_dwCalcSocketSendTime = NULL;

char G_cCmdLine[256], G_cCmdLineTokenA[120], G_cCmdLineTokenA_Lowercase[120], G_cCmdLineTokenB[120], G_cCmdLineTokenC[120], G_cCmdLineTokenD[120], G_cCmdLineTokenE[120];

//video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
//video::E_DRIVER_TYPE driverType = video::EDT_SOFTWARE;
video::E_DRIVER_TYPE driverType = video::EDT_DIRECT3D9;

// --------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam, LPARAM lParam)
{ 
	if(G_pGame->GetText( hWnd, message, wParam, lParam)) return 0;
	//disables alt-f4 if ((message == 0x104) && (((int)lParam) == 0x203e0001)) return 0;

	switch (message) {
	case WM_USER_CALCSOCKETEVENT:
		G_pGame->_CalcSocketClosed();
		break;

	case WM_CLOSE:
		if ( (G_pGame->m_cGameMode == GAMEMODE_ONMAINGAME) && ( G_pGame->m_bForceDisconn == FALSE ) )
		{
			if (!G_pGame->m_bSkillUsingStatus)
			{
				if( G_pGame->m_cLogOutCount == -1 )

#ifdef _DEBUG
					G_pGame->m_cLogOutCount = 1;
#else
					G_pGame->m_cLogOutCount = 11; // Instant Log out xRisenx 1 / 11 Logout
#endif
				else {
					G_pGame->m_cLogOutCount = -1;
				}
				G_pGame->PlaySound('E', 14, 5);
			} else {
				G_pGame->AddEventList("Logout was canceled because your using a skill!", 10);
			}
		}
		else if (G_pGame->m_cGameMode == GAMEMODE_ONLOADING) return (DefWindowProc(hWnd, message, wParam, lParam));
		else if (G_pGame->m_cGameMode == GAMEMODE_ONMAINMENU) G_pGame->ChangeGameMode(GAMEMODE_ONQUIT);
		break;
	
	case WM_SYSCOMMAND:
		if((wParam&0xFFF0)==SC_SCREENSAVE || (wParam&0xFFF0)==SC_MONITORPOWER) 
			return 0; 
		return DefWindowProc(hWnd, message, wParam, lParam);
			
	case WM_USER_TIMERSIGNAL:
		G_pGame->OnTimer();
		break;

	case WM_KEYDOWN:
		G_pGame->OnKeyDown(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		
	case WM_KEYUP:
		G_pGame->OnKeyUp(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));

	case WM_SYSKEYDOWN:
		G_pGame->OnSysKeyDown(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		break;

	case WM_SYSKEYUP:
		G_pGame->OnSysKeyUp(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		break;

	case WM_ACTIVATEAPP:
// 		if( wParam == 0 ) 
// 		{	G_pGame->m_bIsProgramActive = FALSE;
// 			G_pGame->m_DInput.SetAcquire(FALSE);
// 		}else 
// 		{	G_pGame->m_bIsProgramActive = TRUE;
// 			G_pGame->m_DInput.SetAcquire(TRUE);
// 			G_pGame->m_bCtrlPressed = FALSE;
// 			
// 			if (G_pGame->bCheckImportantFile() == FALSE) 
// 			{	MessageBox(G_pGame->m_hWnd, "File checksum error! Get Update again please!", "ERROR1", MB_ICONEXCLAMATION | MB_OK);
// 				PostQuitMessage(0);
// 				return 0;
// 			}			
// 			if (__FindHackingDll__("CRCCHECK") != 1) 
// 			{	G_pGame->ChangeGameMode(GAMEMODE_ONQUIT);
// 				return NULL;
// 		}	}
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_SETCURSOR:
		//SetCursor(NULL);
		return TRUE;

	case WM_DESTROY:
		//OnDestroy();
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
		
	case WM_USER_GAMESOCKETEVENT:
		break;

	case WM_USER_LOGSOCKETEVENT:
		if (G_pGame->gamemode == 1)
			G_pGame->OnGameSocketEvent(wParam, lParam);
		else
			G_pGame->OnLogSocketEvent(wParam, lParam);
		break;

	case WM_USER_VOTESOCKETEVENT:
		G_pGame->OnVoteSocketEvent(wParam, lParam);
		break;
		
	//default: 
		//return (DefWindowProc(hWnd, message, wParam, lParam));
	}	
	return NULL;
}

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

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
               LPSTR lpCmdLine, int nCmdShow )
{
	HINSTANCE hDll;
	char cSearchDll[] = "rd`qbg-ckk";
	char cRealName[12];
	g_keyboardHook = NULL;

	srand((unsigned)time(NULL));
	//char *pJammer = new char[(rand() % 100) +1];
	G_pGame = new class CGame;
// 	ZeroMemory(cRealName, sizeof(cRealName));
// 	strcpy(cRealName, cSearchDll);
// 	for (WORD i = 0; i < strlen(cRealName); i++)
// 	if (cRealName[i] != NULL) cRealName[i]++;
// 
// 	hDll = LoadLibrary(cRealName);
// 	if( hDll == NULL ) 
// 	{	MessageBoxA(NULL, "don't find search.dll", "ERROR!", MB_OK);
// 		return 0;
// 	}

#ifdef USING_WIN_IME
	HINSTANCE hRichDll = LoadLibrary( "Riched20.dll" );
#endif

// 	typedef int (MYPROC)(char *) ;
// 	MYPROC *pFindHook; 
// 	pFindHook = (MYPROC *) GetProcAddress(hDll, "__FindHackingDll__") ;
// 
// 	if (pFindHook== NULL) 
// 	{	MessageBoxA(NULL, "can't find search.dll", "ERROR!", MB_OK);
// 		return 0 ;
// 	}else if ((*pFindHook)("CRCCHECK") != 1) 
// 	{	return 0 ;
// 	}
// 	FreeLibrary(hDll);

// #ifndef _DEBUG
// 	if (OpenMutex(MUTEX_ALL_ACCESS, FALSE, "0643kjg3j30%") != NULL) {
// 		MessageBox(NULL, "Only one Helbreath client program allowed!", "ERROR!", MB_OK);
// 		return 0;
// 	}
// 	HANDLE hMutex = CreateMutex(NULL, FALSE, "0643kjg3j30%");
// #endif



	if (!G_pGame->CreateRenderer())
	{
		return 0;
	}


	G_pGame->device->getFileSystem()->changeWorkingDirectoryTo(L"../");


	irr::gui::IGUISkin * guiskin = G_pGame->env->getSkin();
	G_pGame->font[FONT_BUILTIN]  = G_pGame->env->getBuiltInFont();
	G_pGame->font[FONT_TREBMS6PX]  = G_pGame->env->getFont(L"fonts/treb6px.xml");
	G_pGame->font[FONT_TREBMS8PX]  = G_pGame->env->getFont(L"fonts/treb8px.xml");
	G_pGame->font[FONT_TREBMS10PX]  = G_pGame->env->getFont(L"fonts/treb10px.xml");
	G_pGame->font[FONT_TREBMS12PX]  = G_pGame->env->getFont(L"fonts/treb12px.xml");
	G_pGame->font[FONT_TREBMS14PX]  = G_pGame->env->getFont(L"fonts/treb14px.xml");
	G_pGame->font[FONT_TREBMS16PX]  = G_pGame->env->getFont(L"fonts/treb16px.xml");
	//font[TESTFONT3] = device->getGUIEnvironment()->getFont(L"fonts/myfont.xml");
	//font[DEFAULT] = device->getGUIEnvironment()->getFont(L"fonts/dfont.xml");
	//font[TESTFONT2] = device->getGUIEnvironment()->getFont(L"fonts/fonthaettenschweiler.bmp");

	//set skin defaults
#define setskincolor(x) guiskin->setColor(x, guiskin->getColor(x) + SColor(255,0,0,0))
	setskincolor(irr::gui::EGDC_3D_FACE);
	setskincolor(irr::gui::EGDC_3D_SHADOW);
	setskincolor(irr::gui::EGDC_ACTIVE_CAPTION);
	setskincolor(irr::gui::EGDC_ACTIVE_BORDER);
	setskincolor(irr::gui::EGDC_3D_DARK_SHADOW);
	setskincolor(irr::gui::EGDC_3D_HIGH_LIGHT);
	setskincolor(irr::gui::EGDC_BUTTON_TEXT);
	setskincolor(irr::gui::EGDC_HIGH_LIGHT_TEXT);
	setskincolor(irr::gui::EGDC_HIGH_LIGHT);
	setskincolor(irr::gui::EGDC_WINDOW);
	setskincolor(irr::gui::EGDC_WINDOW_SYMBOL);
	setskincolor(irr::gui::EGDC_SCROLLBAR);

	Initialize((char *)lpCmdLine);


	gui::ICursorControl* cursor = G_pGame->device->getCursorControl();
	cursor->setVisible(false);
	cursor->setPosition(G_pGame->GetWidth()/2, G_pGame->GetHeight()/2);

	RECT trect;
	RECT WindowRect;
	GetWindowRect( G_hWnd, &WindowRect );
	// Restrict the cursor
	ClipCursor( &WindowRect );


	G_pGame->clipmousegame = true;
	G_pGame->clipmousewindow = true;

	int grace = 0;

	uint32_t frametime;
	uint32_t fps;
	uint64_t & time1 = G_pGame->time1;
	uint64_t & time2 = G_pGame->time2;

	time1 = time2 = unixtime();

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				if (grace > 0)
					grace--;
				else
					break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			WndProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
		}

		if (G_pGame->fullscreenswap)
		{
			grace++;
			guiskin = G_pGame->env->getSkin();
			G_pGame->font[FONT_BUILTIN]  = G_pGame->env->getBuiltInFont();
			G_pGame->font[FONT_TREBMS6PX]  = G_pGame->env->getFont(L"fonts/treb6px.xml");
			G_pGame->font[FONT_TREBMS8PX]  = G_pGame->env->getFont(L"fonts/treb8px.xml");
			G_pGame->font[FONT_TREBMS10PX]  = G_pGame->env->getFont(L"fonts/treb10px.xml");
			G_pGame->font[FONT_TREBMS12PX]  = G_pGame->env->getFont(L"fonts/treb12px.xml");
			G_pGame->font[FONT_TREBMS14PX]  = G_pGame->env->getFont(L"fonts/treb14px.xml");
			G_pGame->font[FONT_TREBMS16PX]  = G_pGame->env->getFont(L"fonts/treb16px.xml");

			setskincolor(irr::gui::EGDC_3D_FACE);
			setskincolor(irr::gui::EGDC_3D_SHADOW);
			setskincolor(irr::gui::EGDC_ACTIVE_CAPTION);
			setskincolor(irr::gui::EGDC_ACTIVE_BORDER);
			setskincolor(irr::gui::EGDC_3D_DARK_SHADOW);
			setskincolor(irr::gui::EGDC_3D_HIGH_LIGHT);
			setskincolor(irr::gui::EGDC_BUTTON_TEXT);
			setskincolor(irr::gui::EGDC_HIGH_LIGHT_TEXT);
			setskincolor(irr::gui::EGDC_HIGH_LIGHT);
			setskincolor(irr::gui::EGDC_WINDOW);
			setskincolor(irr::gui::EGDC_WINDOW_SYMBOL);
			setskincolor(irr::gui::EGDC_SCROLLBAR);

			cursor = G_pGame->device->getCursorControl();
			cursor->setVisible(false);
			cursor->setPosition(G_pGame->GetWidth()/2, G_pGame->GetHeight()/2);

			GetWindowRect( G_hWnd, &WindowRect );
			// Restrict the cursor
			ClipCursor( &WindowRect );


			G_pGame->clipmousegame = true;
			G_pGame->clipmousewindow = true;

			G_pGame->fullscreenswap = false;
		}

		// increase virtual timer time
		G_pGame->device->getTimer()->tick();


		u32 time = G_pGame->device->getTimer()->getTime();
		//Cursor clipping stuff
		{
			if (!G_pGame->device->isWindowActive() && G_pGame->isactive)
			{
				ClipCursor( NULL );
				G_pGame->isactive = false;
			}
			//core::position2d<s32> m = cursor->getPosition();
			core::position2d<s32> m = cursor->getPosition();
			G_pGame->m_stMCursor.sX = m.X;
			G_pGame->m_stMCursor.sY = m.Y;

			//stick in a window move event
			GetWindowRect(G_hWnd, &trect);
			if (((WindowRect.top != trect.top) || (WindowRect.bottom != trect.bottom) || (WindowRect.left != trect.left) || (WindowRect.right != trect.right)) || (G_pGame->device->isWindowActive() && !G_pGame->isactive))
			{
 				if (G_pGame->clipmousewindow)
 					ClipCursor( &trect );
				WindowRect = trect;
			}
 			if (G_pGame->device->isWindowActive())
 			{
// 				game->m.X -= game->GetWidth()/2 - m.X;
// 				game->m.Y -= game->GetHeight()/2 - m.Y;
// 				cursor->setPosition(game->GetWidth()/2, game->GetHeight()/2);
 				if (G_pGame->clipmousegame && G_pGame->clipmousewindow)
				{
					if (G_pGame->m_stMCursor.sX < 1) G_pGame->m_stMCursor.sX = 0;
					if (G_pGame->m_stMCursor.sY < 1) G_pGame->m_stMCursor.sY = 0;
					if (G_pGame->m_stMCursor.sX > G_pGame->GetWidth()) G_pGame->m_stMCursor.sX = G_pGame->GetWidth();
					if (G_pGame->m_stMCursor.sY > G_pGame->GetHeight()) G_pGame->m_stMCursor.sY = G_pGame->GetHeight();
				}
				cursor->setPosition(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
			}
		}

		//check incoming packets first
		//game->ProcessPacket();
		//Draw game mode scene

		if (G_pGame->device->isWindowMinimized())
		{
			G_pGame->device->yield();
		}
 		else if (!G_pGame->device->isWindowActive())
 		{
			if (G_pGame->backgroundfpstarget)
			{
				if ((time2 - time1) < G_pGame->backgroundframetime)
				{
					G_pGame->device->yield();
					if ((time2 - time1) > (1000 / G_pGame->backgroundfpstarget))
					{
						G_pGame->backgroundframetime--;
					}
					time2 = unixtime();
				}
				else
				{
					if ((time2 - time1) < (1000 / G_pGame->backgroundfpstarget))
					{
						G_pGame->backgroundframetime++;
					}
					time1 = unixtime();
					G_pGame->UpdateScreen();
					time2 = unixtime();
				}
			}
			else
				G_pGame->UpdateScreen();
 		}
		else
		{
			if (G_pGame->foregroundfpstarget)
			{
				if ((time2 - time1) < G_pGame->foregroundframetime)
				{
					G_pGame->device->yield();
					time2 = unixtime();
					if ((time2 - time1) > (1000 / G_pGame->foregroundfpstarget))
					{
						G_pGame->foregroundframetime--;
					}
				}
				else
				{
					if ((time2 - time1) < (1000 / G_pGame->foregroundfpstarget))
					{
						G_pGame->foregroundframetime++;
					}
					time1 = unixtime();
					G_pGame->UpdateScreen();
					time2 = unixtime();
				}
			}
			else
				G_pGame->UpdateScreen();
		}
	}



// 	while(G_pGame->device->run() && G_pGame->driver)
// 	{
// 		//if (device->isWindowActive())
// 		{
// 			u32 time = G_pGame->device->getTimer()->getTime();
// 			//Cursor clipping stuff
// 			{
// 				if (!G_pGame->device->isWindowActive() && G_pGame->isactive)
// 				{
// 					ClipCursor( NULL );
// 					G_pGame->isactive = false;
// 				}
// 				//core::position2d<s32> m = cursor->getPosition();
// 				core::position2d<s32> m = cursor->getPosition();
// 				G_pGame->m_stMCursor.sX = m.X;
// 				G_pGame->m_stMCursor.sY = m.Y;
// 
// 				//stick in a window move event
// 				GetWindowRect(G_hWnd, &trect);
// 				if (((WindowRect.top != trect.top) || (WindowRect.bottom != trect.bottom) || (WindowRect.left != trect.left) || (WindowRect.right != trect.right)) || (G_pGame->device->isWindowActive() && !G_pGame->isactive))
// 				{
//  					if (G_pGame->clipmousewindow)
//  						ClipCursor( &trect );
// 					WindowRect = trect;
// 				}
//  				if (G_pGame->device->isWindowActive())
//  				{
// // 					game->m.X -= game->GetWidth()/2 - m.X;
// // 					game->m.Y -= game->GetHeight()/2 - m.Y;
// // 					cursor->setPosition(game->GetWidth()/2, game->GetHeight()/2);
//  					if (G_pGame->clipmousegame && G_pGame->clipmousewindow)
// 					{
// 						if (G_pGame->m_stMCursor.sX < 1) G_pGame->m_stMCursor.sX = 1;
// 						if (G_pGame->m_stMCursor.sY < 1) G_pGame->m_stMCursor.sY = 1;
// 						if (G_pGame->m_stMCursor.sX > G_pGame->GetWidth()-1) G_pGame->m_stMCursor.sX = G_pGame->GetWidth()-1;
// 						if (G_pGame->m_stMCursor.sY > G_pGame->GetHeight()-1) G_pGame->m_stMCursor.sY = G_pGame->GetHeight()-1;
// 					}
// 					cursor->setPosition(G_pGame->m_stMCursor.sX, G_pGame->m_stMCursor.sY);
// 				}
// 			}
// 
// 			//check incoming packets first
// 			//game->ProcessPacket();
// 			//Draw game mode scene
// 			if (G_pGame->device->isWindowMinimized())
// 			{
// 			}
//  			else if (!G_pGame->device->isWindowActive())
//  			{
// 				if (rand()%10 == 2)
// 				{
// 					/*if (G_pGame->m_cGameMode == GAMEMODE_ONLOADING) G_pGame->UpdateScreen_OnLoading( FALSE );
// 					else*/ G_pGame->UpdateScreen();
// 				}
//  			}
// 			else
// 			{
// 				/*if (G_pGame->m_cGameMode == GAMEMODE_ONLOADING) G_pGame->UpdateScreen_OnLoading( FALSE );
// 				else*/ G_pGame->UpdateScreen();
// 				//G_pGame->DrawScene(time);
// 			}
// 		}
// 	}
// 	if (G_pGame->gamemode != ONQUIT)
// 		G_pGame->ChangeMode(ONQUIT);
// 	while (G_pGame->sockets->threadcount > 0)
// 	{
// 		Sleep(10);
// 	}
	G_pGame->Quit();
	WSACleanup();
	G_pGame->device->closeDevice();
	delete G_pGame;

	return 0;

// 	sprintf( szAppClass, "Client-I%d", hInstance);
// 	if (!InitApplication( hInstance))		return (FALSE);
//    if (!InitInstance(hInstance, nCmdShow)) return (FALSE);
// 
// 	//Initialize((char *)lpCmdLine);
// 
// 	EventLoop();
// 
// #ifndef _DEBUG
// 	ReleaseMutex(hMutex);
// 	CloseHandle(hMutex);
// #endif
// 
// 	delete[] pJammer;
// 	delete G_pGame;
// 
// #ifdef USING_WIN_IME
// 	FreeLibrary(hRichDll);
// #endif
// 
// 	return 0;
}

// BOOL InitApplication( HINSTANCE hInstance)
// {
// 	WNDCLASS  wc;
// 	wc.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS);
// 	wc.lpfnWndProc   = (WNDPROC)WndProc;             
// 	wc.cbClsExtra    = 0;                            
// 	wc.cbWndExtra    = sizeof (int);
// 	wc.hInstance     = hInstance;
// 	wc.hIcon         = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
// 	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
// 	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
// 	wc.lpszMenuName  = NULL;
// 	wc.lpszClassName = szAppClass;        
// 	return (RegisterClass(&wc));
// }
// 
// BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
// {	int cx = GetSystemMetrics(SM_CXFULLSCREEN)/2;
// 	int cy = GetSystemMetrics(SM_CYFULLSCREEN)/2;
// 	if(cy>280) cy -= 40;
// 
// 	char winName[256];
// 	sprintf(winName, "Helbreath Fantasy %u.%u.%u", HBF_MAJOR, HBF_MINOR, HBF_LOWER);
// #ifdef _DEBUG
// 	strcat(winName, " debug");
// #endif
// 	//G_hWnd = CreateWindowEx(NULL, szAppClass, winName, WS_POPUP, cx-320, cy-240,
// 	//G_hWnd = CreateWindowEx(NULL, szAppClass, "Helbreath Fantasy", WS_POPUP, cx-400, cy-300,  // 800x600 Resolution xRisenx
// 	G_hWnd = CreateWindowEx(NULL, szAppClass, winName, WS_POPUP, cx-400, cy-300,  // 800x600 Resolution xRisenx
// 							//640, 480, NULL, NULL, hInstance, NULL);
// 							800, 600, NULL, NULL, hInstance, NULL); // 800x600 Resolution xRisenx
// 	if (!G_hWnd) return FALSE;
// 	G_hInstance	= hInstance;
// 
// 	ShowWindow(G_hWnd, SW_SHOWDEFAULT);
// 	UpdateWindow(G_hWnd);
// 	return TRUE;
// }


// void EventLoop()
// {
// 	MSG msg;
// 	while( 1 ) 
// 	{
// 		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
// 		{
// 			if( !GetMessage( &msg, NULL, 0, 0 ) ) return;// msg.wParam;
//             TranslateMessage(&msg);
//             DispatchMessage(&msg);
// 		}
// 		else if (G_pGame->m_bIsProgramActive) G_pGame->UpdateScreen();
// 		else if (G_pGame->m_cGameMode == GAMEMODE_ONLOADING) G_pGame->UpdateScreen_OnLoading( FALSE );
// 		else WaitMessage();
// 	}
// }
// 
// void OnDestroy()
// {
// 	G_pGame->m_bIsProgramActive = FALSE;		
// 	_StopTimer(G_mmTimer);
// 	G_pGame->Quit();
// 	WSACleanup();
// 	SetKeyboardHook(false);
// 	PostQuitMessage(0);
// }

void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2)
{
	PostMessage(G_hWnd, WM_USER_TIMERSIGNAL, wID, NULL);
}


MMRESULT _StartTimer(DWORD dwTime)
{
	TIMECAPS caps;
	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);
	return timeSetEvent(dwTime,0,_TimerFunc,0, (UINT)TIME_PERIODIC);
}


void _StopTimer(MMRESULT timerid)
{
	TIMECAPS caps;
	if (timerid != 0) 
	{
		timeKillEvent(timerid);
		timerid = 0;
		timeGetDevCaps(&caps, sizeof(caps));
		timeEndPeriod(caps.wPeriodMin);
	}
}


void Initialize(char * pCmdLine)
{
	int iX, iY, iSum;
 int     iErrCode;
 WORD	 wVersionRequested;
 WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 2 ); 
	iErrCode = WSAStartup( wVersionRequested, &wsaData );
	if ( iErrCode ) 
	{
		MessageBoxA(G_hWnd, "Winsock-V1.1 not found! Cannot execute program.","ERROR",MB_ICONEXCLAMATION | MB_OK);
		PostQuitMessage(0);
		return;
	}
	if (G_pGame->bInit(G_hWnd, G_hInstance, pCmdLine) == FALSE) 
	{
		PostQuitMessage(0);
		return;
	}	

// 	// Limit the process to only 1 thread so we don't chew up system resources
// 	HANDLE ProcessHandle = GetCurrentProcess();
// 	DWORD ProcessAffinityMask;
// 	DWORD SystemAffinityMask;
// 	if(GetProcessAffinityMask(ProcessHandle,&ProcessAffinityMask,&SystemAffinityMask)
// 		&& SystemAffinityMask != 0)
// 	{
// 		// Limit to 1 thread by masking all but 1 bit of the system affinity mask
// 		DWORD NewProcessAffinityMask = ((SystemAffinityMask-1) ^ SystemAffinityMask) & SystemAffinityMask;
// 		SetProcessAffinityMask(ProcessHandle,NewProcessAffinityMask);
// 	}

	G_mmTimer = _StartTimer(1000);
	for (iX = 0; iX < 64; iX++)
	for (iY = 0; iY < 510; iY++) 
	{
		iSum = iX + (iY - 255);
		if (iSum <= 0)  iSum = 1;
		if (iSum >= 31) iSum = 31;
		G_iAddTable31[iX][iY] = iSum; 
		iSum = iX + (iY - 255);
		if (iSum <= 0)  iSum = 1;
		if (iSum >= 63) iSum = 63;
		G_iAddTable63[iX][iY] = iSum; 
		if ((iY - 255) < iX) G_iAddTransTable31[iY][iX] = iX;
		else if ((iY - 255) > 31) G_iAddTransTable31[iY][iX] = 31;
		else G_iAddTransTable31[iY][iX] = iY-255;
		if ((iY - 255) < iX) G_iAddTransTable63[iY][iX] = iX;
		else if ((iY - 255) > 63) G_iAddTransTable63[iY][iX] = 63;
		else G_iAddTransTable63[iY][iX] = iY-255;
	}

	if(!G_pGame->m_windowsKey) SetKeyboardHook(true);
}

LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);
    if (retval == ERROR_SUCCESS) 
	{
		long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }
    return retval;
}

void GoHomepage() 
{	
// 	LPCTSTR	url = MSG_HOMEPAGE;
// 
// 	int		showcmd = SW_SHOW;
// 	wchar_t	key[MAX_PATH + MAX_PATH];	
//     // First try ShellExecute()
//     HINSTANCE result = ShellExecute(NULL, L"open", url, NULL,NULL, showcmd);
// 
// 	// If it failed, get the .htm regkey and lookup the program
// 	if ((UINT)result <= HINSTANCE_ERROR) 
// 	{
// 		if (GetRegKey(HKEY_CLASSES_ROOT, L".htm", key) == ERROR_SUCCESS) 
// 		{ 
// 			lstrcatW(key, L"\\shell\\open\\command");
// 			if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS)
// 			{
// 				wchar_t *pos;
// 				pos = wcsstr(key, L"\"%1\"");
// 				if (pos == NULL) {                     // No quotes found
// 					pos = wcsstr(key, L"%1");           // Check for %1, without quotes 
// 					if (pos == NULL)                   // No parameter at all...
// 						pos = key+lstrlen(key)-1;
// 					else *pos = '\0';                   // Remove the parameter
// 				}
// 				else    *pos = '\0';                   // Remove the parameter
// 				lstrcat(pos, L" ");
// 				lstrcat(pos, url);
// 				result = (HINSTANCE) WinExec(key,showcmd);
// 			}	
// 		}
// 	}
}

void SetKeyboardHook(bool enable)
{
	if(enable)
		g_keyboardHook = SetWindowsHookEx( WH_KEYBOARD_LL, KeyboardProc, G_hInstance, 0);
	else if(g_keyboardHook != NULL)
		UnhookWindowsHookEx(g_keyboardHook);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || nCode != HC_ACTION)
		return CallNextHookEx( g_keyboardHook, nCode, wParam, lParam);

	bool bPreventKeypress = false;
	KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
	switch(wParam)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			bPreventKeypress = (G_pGame->m_bIsProgramActive && ((p->vkCode == VK_LWIN) || (p->vkCode == VK_RWIN)));
			break;
		}
	}

	if (bPreventKeypress)
		return 1;		// we are handling the key (aka gobbling it up!)
	else
		return CallNextHookEx( g_keyboardHook, nCode, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////

bool CheckCheating()
{
	return false;
//   HANDLE hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetCurrentProcessId() ); 
//   if( hModuleSnap == INVALID_HANDLE_VALUE ) 
//   { 
// 	  return( FALSE ); 
//   } 
// 
//   MODULEENTRY32 me32; 
//   me32.dwSize = sizeof( MODULEENTRY32 ); 
// 
//   if( !Module32First( hModuleSnap, &me32 ) ) 
//   { 
// 	  CloseHandle( hModuleSnap );
// 	  return( FALSE ); 
//   } 
// 
//   bool exit = false;
//   do 
//   {
// 	  if(strcmp(me32.szModule, "WpeSpy.dll") == 0 //||
// 		  /*strcmp(me32.szModule, "JIntellitype.dll") == 0 ||
// 		  strcmp(me32.szModule, "speedhack-i386.dll") == 0 ||
// 		  strcmp(me32.szModule, "lua5.1-32.dll") == 0 ||
// 		  strcmp(me32.szModule, "lua5.1-64.dll") == 0 ||
// 		  strcmp(me32.szModule, "speedhack-x86_64.dll") == 0 ||
// 		  strcmp(me32.szModule, "allochook-i386.dll") == 0 ||
// 		  strcmp(me32.szModule, "d3d10hook.dll") == 0 ||
// 		  strcmp(me32.szModule, "d3d11hook.dll") == 0 ||
// 		  strcmp(me32.szModule, "speedhack-x86_64.dll") == 0 ||
// 		  strcmp(me32.szModule, "ced3d9hook.dll") == 0 ||
// 		  strcmp(me32.szModule, "TSpeech.dll") == 0 ||
// 		  strcmp(me32.szModule, "vehdebug-i386.dll") == 0 ||
//  	 	  strcmp(me32.szModule, "vehdebug-x86_64.dll") == 0 ||
// 		  strcmp(me32.szModule, "allochook-x86_64.dll") == 0 ||
// 		  strcmp(me32.szModule, "d3dhook64.dll") == 0 ||
//  		  strcmp(me32.szModule, "luaclient-i386.dll") == 0 ||
// 		  strcmp(me32.szModule, "luaclient-x86_64.dll") == 0 ||
// 		  strcmp(me32.szModule, "d3d11hook.dll") == 0 ||
// 		  strcmp(me32.szModule, "ced3d11hook64.dll") == 0 ||
// 		  strcmp(me32.szModule, "ced3d10hook64.dll") == 0 ||
// 		  strcmp(me32.szModule, "ced3d10hook.dll") == 0 ||
// 		  strcmp(me32.szModule, "ced3d11hook.dll") == 0 ||
// 		  strstr(me32.szModule, "hook") != NULL ||
// 		  strstr(me32.szModule, "HOOK") != NULL ||
// 		  strstr(me32.szModule, "Hook") != NULL*/)
// 	  {
// 		exit = true;
// 		FreeLibrary(me32.hModule);
// 	  }
//   } while( Module32Next( hModuleSnap, &me32 ) ); 
// 
//   CloseHandle( hModuleSnap ); 
// 
//   return exit;
}

bool CheckHackProgram()
{
// 	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL,0);
// 	if(hSnapShot == INVALID_HANDLE_VALUE)	return( FALSE );
// 	PROCESSENTRY32 uProcess;
//     uProcess.dwSize = sizeof(PROCESSENTRY32);
// 
// 	char *Ufoundprocess;
// 
// 	if(!Process32First(hSnapShot, &uProcess))
// 	{
// 		CloseHandle(hSnapShot);
// 		return ( FALSE );
// 	}

	bool bFound = false;
//     do
//     {
// 		Ufoundprocess = _strupr( uProcess.szExeFile );
// 		if ( strcmp(Ufoundprocess, "cheatengine-x86_64.exe") == 0 //||
// 			/*strcmp(Ufoundprocess, "cheatengine-i386.exe") == 0 ||
// 			strcmp(Ufoundprocess, "Start.exe") == 0 ||
// 			strcmp(Ufoundprocess, "cheat engine.exe") == 0 ||
// 			strcmp(Ufoundprocess, "351HAX.EXE") == 0 ||
// 			strcmp(Ufoundprocess, "msched.EXE") == 0 ||
// 			strcmp(Ufoundprocess, "010Editor.exe") == 0 ||
// 			strstr(Ufoundprocess, "HACK") != NULL ||
// 			strstr(Ufoundprocess, "CHEAT") != NULL*/ )
// 		{
// 			bFound = true;
// 		}
//     } while (Process32Next(hSnapShot, &uProcess));
// 
//     CloseHandle(hSnapShot);

    return bFound;
}