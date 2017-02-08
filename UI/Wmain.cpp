// -------------------------------------------------------------- 
//                      Helbreath Client 						  
//
//                      1998.10 by Soph
//
// --------------------------------------------------------------


#include "winmain.h"
#include <stdio.h>
#include <stdlib.h> 
#include <process.h>
#include <iostream>
#include <string>
#include <stdint.h>
#include <sys/timeb.h>
#include "..\Game.h"
#include "..\res\resource.h"
#include "..\GlobalDef.h"

extern "C" __declspec( dllimport) int __FindHackingDll__(char *);

// --------------------------------------------------------------

#define WM_USER_TIMERSIGNAL		WM_USER + 500
#define WM_USER_CALCSOCKETEVENT WM_USER + 600

int				G_iAddTable31[64][510], G_iAddTable63[64][510]; 
int				G_iAddTransTable31[510][64], G_iAddTransTable63[510][64]; 

char G_cCmdLine[256], G_cCmdLineTokenA[120], G_cCmdLineTokenA_Lowercase[120], G_cCmdLineTokenB[120], G_cCmdLineTokenC[120], G_cCmdLineTokenD[120], G_cCmdLineTokenE[120];

long    G_lTransG100[64][64], G_lTransRB100[64][64];
long    G_lTransG70[64][64], G_lTransRB70[64][64];
long    G_lTransG50[64][64], G_lTransRB50[64][64];
long    G_lTransG25[64][64], G_lTransRB25[64][64];
long    G_lTransG2[64][64], G_lTransRB2[64][64];

void * G_hWnd = 0;
void * G_hInstance = 0;
char   G_cSpriteAlphaDegree;
class CGame * G_pGame;

video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
//video::E_DRIVER_TYPE driverType = video::EDT_SOFTWARE;
//video::E_DRIVER_TYPE driverType = video::EDT_DIRECT3D9;

using namespace irr;
using namespace irr::video;

// --------------------------------------------------------------

int main(int argc, char * argv[])
{
	srand((unsigned)time(0));

    G_pGame = new class CGame;
  
    
    G_pGame->_renderer = L"DirectX3D9";
    driverType = video::EDT_DIRECT3D9;

    if (argc >= 2)
    {
        for (int i = 0; i < argc; ++i)
        {
            if (!memcmp(argv[i], "-data=", 6))
            {
                char * ctx;
                char * c = strtok_s(argv[i], "=:", &ctx);
                G_pGame->m_cAccountName = strtok_s(0, "=:", &ctx);
                G_pGame->m_cAccountPassword = strtok_s(0, "=:", &ctx);//TODO: encode this when sent from launcher
                G_pGame->autologin = true;
            }
            else if (!memcmp(argv[i], "-renderer=", 10))
            {
                char * ctx;
                char * c = strtok_s(argv[i], "=:", &ctx);
                string renderer = strtok_s(0, "=:", &ctx);
                if (renderer == "opengl")
                {
                    G_pGame->_renderer = L"OpenGL";
                    driverType = video::EDT_OPENGL;
                }
                else if (renderer == "software")
                {
                    G_pGame->_renderer = L"Software";
                    driverType = video::EDT_SOFTWARE;
                }
                else
                {
                    G_pGame->_renderer = L"DirectX3D9";
                    driverType = video::EDT_DIRECT3D9;
                }
            }
        }
    }


	if (!G_pGame->CreateRenderer())
	{
		return 0;
	}


	G_pGame->device->getFileSystem()->changeWorkingDirectoryTo(L"../");


	gui::IGUISkin * guiskin = G_pGame->env->getSkin();
	G_pGame->font[FONT_BUILTIN]  = G_pGame->env->getBuiltInFont();
	G_pGame->font[FONT_TREBMS6PX]  = G_pGame->env->getFont(L"data/fonts/treb6px.xml");
	G_pGame->font[FONT_TREBMS8PX]  = G_pGame->env->getFont(L"data/fonts/treb8px.xml");
	G_pGame->font[FONT_TREBMS10PX]  = G_pGame->env->getFont(L"data/fonts/treb10px.xml");
	G_pGame->font[FONT_TREBMS12PX]  = G_pGame->env->getFont(L"data/fonts/treb12px.xml");
	G_pGame->font[FONT_TREBMS14PX]  = G_pGame->env->getFont(L"data/fonts/treb14px.xml");
	G_pGame->font[FONT_TREBMS16PX]  = G_pGame->env->getFont(L"data/fonts/treb16px.xml");
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

	Initialize((char *)"");

	irr::gui::IGUIEnvironment* env = G_pGame->device->getGUIEnvironment();
	irr::gui::IGUISkin* skin = env->getSkin();
	irr::gui::IGUIFont* font = G_pGame->font[FONT_TREBMS6PX];
	if (font)
		skin->setFont(font);

	skin->setFont(G_pGame->font[FONT_BUILTIN]);

	gui::ICursorControl* cursor = G_pGame->device->getCursorControl();
	cursor->setVisible(false);
	cursor->setPosition(G_pGame->GetWidth() / 2, G_pGame->GetHeight() / 2);

	RECT trect;
	RECT WindowRect;
	GetWindowRect(*(HWND*)&G_hWnd, &WindowRect);
	// Restrict the cursor
	ClipCursor(&WindowRect);


	G_pGame->clipmousegame = true;
	G_pGame->clipmousewindow = true;

	int grace = 0;

	uint32_t frametime;
	uint32_t fps;
	uint64_t & time1 = G_pGame->time1;
	uint64_t & time2 = G_pGame->time2;

	time1 = time2 = unixtime();

	MSG msg;
	while (G_pGame->device->run() && G_pGame->driver)
	{
		G_pGame->OnTimer();
		//process packets

		if (G_pGame->fullscreenswap)
		{
			grace++;
            guiskin = G_pGame->env->getSkin();
            G_pGame->font[FONT_BUILTIN] = G_pGame->env->getBuiltInFont();
            G_pGame->font[FONT_TREBMS6PX] = G_pGame->env->getFont(L"data/fonts/treb6px.xml");
            G_pGame->font[FONT_TREBMS8PX] = G_pGame->env->getFont(L"data/fonts/treb8px.xml");
            G_pGame->font[FONT_TREBMS10PX] = G_pGame->env->getFont(L"data/fonts/treb10px.xml");
            G_pGame->font[FONT_TREBMS12PX] = G_pGame->env->getFont(L"data/fonts/treb12px.xml");
            G_pGame->font[FONT_TREBMS14PX] = G_pGame->env->getFont(L"data/fonts/treb14px.xml");
            G_pGame->font[FONT_TREBMS16PX] = G_pGame->env->getFont(L"data/fonts/treb16px.xml");

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
			cursor->setPosition(G_pGame->GetWidth() / 2, G_pGame->GetHeight() / 2);
			//cursor->changeIcon()

			GetWindowRect(*(HWND*)&G_hWnd, &WindowRect);
			// Restrict the cursor
			ClipCursor(&WindowRect);


			G_pGame->clipmousegame = true;
			G_pGame->clipmousewindow = true;

			G_pGame->fullscreenswap = false;
		}

		// increase virtual timer time
		G_pGame->device->getTimer()->tick();


		uint32_t time = G_pGame->device->getTimer()->getTime();
		//Cursor clipping stuff
		{
			if (!G_pGame->device->isWindowActive() && G_pGame->isactive)
			{
				ClipCursor(0);
				G_pGame->isactive = false;
			}
			//core::position2d<s32> m = cursor->getPosition();
			core::position2d<s32> m = cursor->getPosition();
			G_pGame->m_stMCursor.sX = m.X;
			G_pGame->m_stMCursor.sY = m.Y;

			//stick in a window move event
			GetWindowRect(*(HWND*)&G_hWnd, &trect);
			if (((WindowRect.top != trect.top) || (WindowRect.bottom != trect.bottom) || (WindowRect.left != trect.left) || (WindowRect.right != trect.right)) || (G_pGame->device->isWindowActive() && !G_pGame->isactive))
			{
 				if (G_pGame->clipmousewindow)
					ClipCursor(&trect);
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
			G_pGame->wasinactive = true;
			G_pGame->device->yield();
		}
 		else if (!G_pGame->device->isWindowActive())
 		{
			GetClipCursor(&trect);
			if (trect.top != 0)
				ClipCursor(0);
			G_pGame->wasinactive = true;
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
			G_pGame->wasinactive = false;
// 			if (G_pGame->foregroundfpstarget)
// 			{
// 				if ((time2 - time1) < G_pGame->foregroundframetime)
// 				{
// 					G_pGame->device->yield();
// 					time2 = unixtime();
// 					if ((time2 - time1) > (1000 / G_pGame->foregroundfpstarget))
// 					{
// 						G_pGame->foregroundframetime--;
// 					}
// 				}
// 				else
// 				{
// 					if ((time2 - time1) < (1000 / G_pGame->foregroundfpstarget))
// 					{
// 						G_pGame->foregroundframetime++;
// 					}
// 					time1 = unixtime();
// 					G_pGame->UpdateScreen();
// 					time2 = unixtime();
// 				}
// 			}
// 			else
				G_pGame->UpdateScreen();
		}
	}

	G_pGame->signals_.cancel();
	G_pGame->Quit();
	G_pGame->device->closeDevice();
	G_pGame->socketthread->join();
	delete G_pGame;

	return 0;
}

bool Initialize(char * pCmdLine)
{
	if (G_pGame->bInit(G_hWnd, G_hInstance, pCmdLine) == false)
	{
		return false;
	}
	return true;
}