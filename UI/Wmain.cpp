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
#include <CEGUI/RendererModules/Irrlicht/Renderer.h>

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
using namespace CEGUI;

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
                memcpy(G_pGame->m_cAccountName, strtok_s(0, "=:", &ctx), 10);
                memcpy(G_pGame->m_cAccountPassword, strtok_s(0, "=:", &ctx), 10);//TODO: encode this when sent from launcher
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

// 	irr::gui::IGUISpriteBank* test = skin->getSpriteBank();
// 	//skin->
// 
// 	env->addButton(irr::core::rect<s32>(10, 240, 110, 240 + 32), 0, GUI_ID_QUIT_BUTTON,
// 		L"Quit", L"Exits Program");
// 	env->addButton(irr::core::rect<s32>(10, 280, 110, 280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON,
// 		L"New Window", L"Launches a new Window");
// 	env->addButton(irr::core::rect<s32>(10, 320, 110, 320 + 32), 0, GUI_ID_FILE_OPEN_BUTTON,
// 		L"File Open", L"Opens a file");
// 
// 
// 	env->addStaticText(L"Transparent Control:", irr::core::rect<s32>(150, 20, 350, 40), true);
// 	irr::gui::IGUIScrollBar* scrollbar = env->addScrollBar(true,
// 		irr::core::rect<s32>(150, 45, 350, 60), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
// 	scrollbar->setMax(255);
// 	scrollbar->setPos(255);
// 	CGame::setSkinTransparency(scrollbar->getPos(), env->getSkin());
// 
// 	// set scrollbar position to alpha value of an arbitrary element
// 	scrollbar->setPos(env->getSkin()->getColor(irr::gui::EGDC_WINDOW).getAlpha());
// 
// 	env->addStaticText(L"Logging ListBox:", irr::core::rect<s32>(50, 110, 250, 130), true);
// 	irr::gui::IGUIListBox * listbox = env->addListBox(irr::core::rect<s32>(50, 140, 250, 210));
// 	env->addEditBox(L"Editable Text", irr::core::rect<s32>(350, 80, 550, 100));
// 
// 	// Store the appropriate data in a context structure.
// 	G_pGame->context.device = G_pGame->device;
// 	G_pGame->context.counter = 0;
// 	G_pGame->context.listbox = listbox;
// 
// 	env->addMessageBox(L"TEST NIGGA", L"TEXT NIGGA", false, irr::gui::EMBF_OK | irr::gui::EMBF_CANCEL);



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


	//IrrlichtRenderer& myRenderer = IrrlichtRenderer::bootstrapSystem(*G_pGame->device);

	
	CEGUI::IrrlichtRenderer& myRenderer = CEGUI::IrrlichtRenderer::create(*G_pGame->device);
	CEGUI::System::create(myRenderer);

	DefaultResourceProvider * rp = static_cast<DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());

	rp->setResourceGroupDirectory("schemes", "./data/schemes/");
	rp->setResourceGroupDirectory("imagesets", "./data/imagesets/");
	rp->setResourceGroupDirectory("fonts", "./data/fonts/");
	rp->setResourceGroupDirectory("layouts", "./data/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "./data/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "./data/lua_scripts/");
	rp->setResourceGroupDirectory("schemas", "./data/xml_schemas/");

	// set the default resource groups to be used
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
	// setup default group for validation schemas
	CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
	if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
		parser->setProperty("SchemaDefaultResourceGroup", "schemas");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("GameMenu.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaCommonDialogs.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
	CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
	

	WindowManager & wmgr = WindowManager::getSingleton();
	Window* myRoot = wmgr.createWindow("DefaultWindow", "root");
	System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);
	myRoot->setMousePassThroughEnabled(true);

// 	FrameWindow* fWnd = static_cast<FrameWindow*>(wmgr.createWindow("TaharezLook/FrameWindow", "testWindow"));
// 	myRoot->addChild(fWnd);
// 	// position a quarter of the way in from the top-left of parent.
// 	fWnd->setPosition(UVector2(UDim(0.25f, 0.0f), UDim(0.25f, 0.0f)));
// 	// set size to be half the size of the parent
// 	fWnd->setSize(USize(UDim(0.5f, 0.0f), UDim(0.5f, 0.0f)));
// 	fWnd->setText("Hello World!");


	G_pGame->ceguistarted = true;

	uint64_t ceguitime = 0.0f;
	uint64_t currenttime = 0;

	MSG msg;
	while (G_pGame->device->run() && G_pGame->driver)
	{
		currenttime = unixtime();
		G_pGame->OnTimer();
		//process packets

		{
			std::lock_guard<std::mutex> lock(G_pGame->socketmut);
			while (G_pGame->socketpipe.size() > 0)
			{
				shared_ptr<CGame::MsgQueueEntry> entry = G_pGame->GetMsgQueue(G_pGame->socketpipe);
				G_pGame->GameRecvMsgHandler(entry->size, entry->data);
			}
		}

// 		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
// 		{
// 			if (msg.message == WM_QUIT)
// 			{
// 				if (grace > 0)
// 					grace--;
// 				else
// 					break;
// 			}
// 			TranslateMessage(&msg);
// 			DispatchMessage(&msg);
// 
// 			WndProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
// 		}

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
			CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(((float)(currenttime - ceguitime))/1000);
			ceguitime = currenttime;
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