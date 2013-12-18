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
					G_pGame->m_cLogOutCount = 11;
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
		if( wParam == 0 ) 
		{	G_pGame->m_bIsProgramActive = FALSE;
			G_pGame->m_DInput.SetAcquire(FALSE);
		}else 
		{	G_pGame->m_bIsProgramActive = TRUE;
			G_pGame->m_DInput.SetAcquire(TRUE);
			G_pGame->m_bCtrlPressed = FALSE;
			
			if (G_pGame->bCheckImportantFile() == FALSE) 
			{	MessageBox(G_pGame->m_hWnd, "File checksum error! Get Update again please!", "ERROR1", MB_ICONEXCLAMATION | MB_OK);
				PostQuitMessage(0);
				return 0;
			}			
			if (__FindHackingDll__("CRCCHECK") != 1) 
			{	G_pGame->ChangeGameMode(GAMEMODE_ONQUIT);
				return NULL;
		}	}
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_SETCURSOR:
		SetCursor(NULL);
		return TRUE;

	case WM_DESTROY:
		OnDestroy();
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
		
	case WM_USER_GAMESOCKETEVENT:
		G_pGame->OnGameSocketEvent(wParam, lParam);
		break;

	case WM_USER_LOGSOCKETEVENT:
		G_pGame->OnLogSocketEvent(wParam, lParam);
		break;

	case WM_USER_VOTESOCKETEVENT:
		G_pGame->OnVoteSocketEvent(wParam, lParam);
		break;
		
	default: 
		return (DefWindowProc(hWnd, message, wParam, lParam));
	}	
	return NULL;
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
               LPSTR lpCmdLine, int nCmdShow )
{
	HINSTANCE hDll;
	char cSearchDll[] = "rd`qbg-ckk";
	char cRealName[12];
	g_keyboardHook = NULL;

	srand((unsigned)time(NULL));
	char *pJammer = new char[(rand() % 100) +1];
	G_pGame = new class CGame;
	ZeroMemory(cRealName, sizeof(cRealName));
	strcpy(cRealName, cSearchDll);
	for (WORD i = 0; i < strlen(cRealName); i++)
	if (cRealName[i] != NULL) cRealName[i]++;

	hDll = LoadLibrary(cRealName);
	if( hDll == NULL ) 
	{	MessageBox(NULL, "don't find search.dll", "ERROR!", MB_OK);
		return 0;
	}

#ifdef USING_WIN_IME
	HINSTANCE hRichDll = LoadLibrary( "Riched20.dll" );
#endif

	typedef int (MYPROC)(char *) ;
	MYPROC *pFindHook; 
	pFindHook = (MYPROC *) GetProcAddress(hDll, "__FindHackingDll__") ;

	if (pFindHook== NULL) 
	{	MessageBox(NULL, "can't find search.dll", "ERROR!", MB_OK);
		return 0 ;
	}else if ((*pFindHook)("CRCCHECK") != 1) 
	{	return 0 ;
	}
	FreeLibrary(hDll);

#ifndef _DEBUG
	if (OpenMutex(MUTEX_ALL_ACCESS, FALSE, "0643kjg3j30%") != NULL) {
		MessageBox(NULL, "Only one Helbreath client program allowed!", "ERROR!", MB_OK);
		return 0;
	}
	HANDLE hMutex = CreateMutex(NULL, FALSE, "0643kjg3j30%");
#endif

	sprintf( szAppClass, "Client-I%d", hInstance);
	if (!InitApplication( hInstance))		return (FALSE);
   if (!InitInstance(hInstance, nCmdShow)) return (FALSE);

	Initialize((char *)lpCmdLine);

	EventLoop();

#ifndef _DEBUG
	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
#endif

	delete[] pJammer;
	delete G_pGame;

#ifdef USING_WIN_IME
	FreeLibrary(hRichDll);
#endif

	return 0;
}

BOOL InitApplication( HINSTANCE hInstance)
{
	WNDCLASS  wc;
	wc.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS);
	wc.lpfnWndProc   = (WNDPROC)WndProc;             
	wc.cbClsExtra    = 0;                            
	wc.cbWndExtra    = sizeof (int);
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadCursor(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = szAppClass;        
	return (RegisterClass(&wc));
}

BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{	int cx = GetSystemMetrics(SM_CXFULLSCREEN)/2;
	int cy = GetSystemMetrics(SM_CYFULLSCREEN)/2;
	if(cy>280) cy -= 40;

	char winName[256];
	sprintf(winName, "Helbreath Legion %u.%u", UPPER_VERSION, LOWER_VERSION);
#ifdef _DEBUG
	strcat(winName, " debug");
#endif
	G_hWnd = CreateWindowEx(NULL, szAppClass, winName, WS_POPUP, cx-320, cy-240, 
							640, 480, NULL, NULL, hInstance, NULL);  
	if (!G_hWnd) return FALSE;
	G_hInstance	= hInstance;

	ShowWindow(G_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(G_hWnd);
	return TRUE;
}


void EventLoop()
{
	MSG msg;
	while( 1 ) 
	{	if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
		{	if( !GetMessage( &msg, NULL, 0, 0 ) ) return;// msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
		else if (G_pGame->m_bIsProgramActive) G_pGame->UpdateScreen();
		else if (G_pGame->m_cGameMode == GAMEMODE_ONLOADING) G_pGame->UpdateScreen_OnLoading( FALSE );
		else WaitMessage();
	}
}

void OnDestroy()
{	G_pGame->m_bIsProgramActive = FALSE;		
	_StopTimer(G_mmTimer);
	G_pGame->Quit();
	WSACleanup();
	SetKeyboardHook(false);
	PostQuitMessage(0);
}

void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2)
{	PostMessage(G_hWnd, WM_USER_TIMERSIGNAL, wID, NULL);
}


MMRESULT _StartTimer(DWORD dwTime)
{TIMECAPS caps;
	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);
	return timeSetEvent(dwTime,0,_TimerFunc,0, (UINT)TIME_PERIODIC);
}


void _StopTimer(MMRESULT timerid)
{TIMECAPS caps;
	if (timerid != 0) 
	{	timeKillEvent(timerid);
		timerid = 0;
		timeGetDevCaps(&caps, sizeof(caps));
		timeEndPeriod(caps.wPeriodMin);
	}
}


void Initialize(char * pCmdLine)
{int iX, iY, iSum;
 int     iErrCode;
 WORD	 wVersionRequested;
 WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 2 ); 
	iErrCode = WSAStartup( wVersionRequested, &wsaData );
	if ( iErrCode ) 
	{	MessageBox(G_hWnd, "Winsock-V1.1 not found! Cannot execute program.","ERROR",MB_ICONEXCLAMATION | MB_OK);
		PostQuitMessage(0);
		return;
	}
	if (G_pGame->bInit(G_hWnd, G_hInstance, pCmdLine) == FALSE) 
	{	PostQuitMessage(0);
		return;
	}	
	G_mmTimer = _StartTimer(1000);
	for (iX = 0; iX < 64; iX++)
	for (iY = 0; iY < 510; iY++) 
	{	iSum = iX + (iY - 255);
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
{   HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);
    if (retval == ERROR_SUCCESS) 
	{	long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }
    return retval;
}

void GoHomepage() 
{	
	LPCTSTR	url = MSG_HOMEPAGE;

	int		showcmd = SW_SHOW;
	char	key[MAX_PATH + MAX_PATH];	
    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, "open", url, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) 
	{  if (GetRegKey(HKEY_CLASSES_ROOT, ".htm", key) == ERROR_SUCCESS) 
		{  lstrcat(key, "\\shell\\open\\command");
            if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS)
			{   char *pos;
                pos = strstr(key, "\"%1\"");
                if (pos == NULL) {                     // No quotes found
                    pos = strstr(key, "%1");           // Check for %1, without quotes 
                    if (pos == NULL)                   // No parameter at all...
                          pos = key+lstrlen(key)-1;
                    else *pos = '\0';                   // Remove the parameter
                }else    *pos = '\0';                   // Remove the parameter
                lstrcat(pos, " ");
                lstrcat(pos, url);
                result = (HINSTANCE) WinExec(key,showcmd);
    }	}	}
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
  HANDLE hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetCurrentProcessId() ); 
  if( hModuleSnap == INVALID_HANDLE_VALUE ) 
  { 
	  return( FALSE ); 
  } 

  MODULEENTRY32 me32; 
  me32.dwSize = sizeof( MODULEENTRY32 ); 

  if( !Module32First( hModuleSnap, &me32 ) ) 
  { 
	  CloseHandle( hModuleSnap );
	  return( FALSE ); 
  } 

  bool exit = false;
  do 
  {
	  if(strcmp(me32.szModule, "WpeSpy.dll") == 0)
	  {
		  FreeLibrary(me32.hModule);
	  }

  } while( Module32Next( hModuleSnap, &me32 ) ); 

  CloseHandle( hModuleSnap ); 

  return exit;
}