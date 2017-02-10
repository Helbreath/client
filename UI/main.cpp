// -------------------------------------------------------------- 
//                      Helbreath Client 						  
//
//                      1998.10 by Soph
//
// --------------------------------------------------------------


#include "main.h"
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

void * G_hWnd = 0;
void * G_hInstance = 0;
char   G_cSpriteAlphaDegree;
class CGame * G_pGame;

bool Initialize(char * pCmdLine);

int main(int argc, char * argv[])
{
	srand((unsigned)time(0));

    G_pGame = new class CGame;
  
    
    G_pGame->_renderer = "OpenGL";

#ifdef WIN32
    SetCurrentDirectoryA("../");
#else
#endif

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
//             else if (!memcmp(argv[i], "-renderer=", 10))
//             {
//                 char * ctx;
//                 char * c = strtok_s(argv[i], "=:", &ctx);
//                 string renderer = strtok_s(0, "=:", &ctx);
//                 if (renderer == "opengl")
//                 {
//                     G_pGame->_renderer = "OpenG";
//                     driverType = video::EDT_OPENGL;
//                 }
//                 else if (renderer == "software")
//                 {
//                     G_pGame->_renderer = "Software";
//                     driverType = video::EDT_BURNINGSVIDEO;// EDT_SOFTWARE;
//                 }
//                 else
//                 {
//                     G_pGame->_renderer = "DirectX3D9";
//                     driverType = video::EDT_DIRECT3D9;
//                 }
//             }
        }
    }


	if (!G_pGame->CreateRenderer())
	{
		return 0;
	}


	Initialize("");

    G_pGame->window.setMouseCursorGrabbed(true);
    G_pGame->window.setMouseCursorVisible(false);

// 	G_pGame->clipmousegame = true;
// 	G_pGame->clipmousewindow = true;

	int grace = 0;

    uint64_t frametime;
    uint64_t fps;
	uint64_t & time1 = G_pGame->time1;
	uint64_t & time2 = G_pGame->time2;

	time1 = time2 = unixtime();

    sf::Event event;
    sf::RenderWindow & window = G_pGame->window;
    while (window.isOpen())
	{
        //timers first
        G_pGame->OnTimer();

        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == Keyboard::Key::Escape)
                    {
                        G_pGame->clipmousegame = ~G_pGame->clipmousegame;
                        G_pGame->window.setMouseCursorGrabbed(G_pGame->clipmousegame);
                    }
                {
                    WebKeyboardEvent keyEvent = WebKeyboardEvent();
                    keyEvent.type = WebKeyboardEvent::kTypeKeyDown;
                    keyEvent.virtual_key_code = event.key.code;
                    keyEvent.native_key_code = event.key.code;

                    char buf[20], * cp;
                    cp = buf;
                    GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &cp);
                    strcpy(keyEvent.key_identifier, buf);

                    if (event.key.control)
                    {
                        keyEvent.modifiers |= WebKeyboardEvent::kModControlKey;
                    }
                    else if (event.key.shift)
                    {
                        keyEvent.modifiers |= WebKeyboardEvent::kModShiftKey;
                    }

                    G_pGame->htmlUI->view->InjectKeyboardEvent(keyEvent);


                    if (event.key.code && event.key.code != VK_TAB)
                    {
                        WebKeyboardEvent keyEvent2 = WebKeyboardEvent();
                        keyEvent2.type = WebKeyboardEvent::kTypeChar;
                        keyEvent2.text[0] = event.key.code;

                        if (event.key.control)
                        {
                            keyEvent2.modifiers |= WebKeyboardEvent::kModControlKey;
                        }
                        else if (event.key.shift)
                        {
                            keyEvent2.modifiers |= WebKeyboardEvent::kModShiftKey;
                        }

                        G_pGame->htmlUI->view->InjectKeyboardEvent(keyEvent2);
                    }
                }

                    G_pGame->OnKeyDown(event.key.code);
                    break;
                case sf::Event::KeyReleased:
                    G_pGame->OnKeyUp(event.key.code);
                    break;
                case sf::Event::Resized:
                    break;
                case sf::Event::LostFocus:
                    G_pGame->window.setFramerateLimit(45);//set to var
                    break;
                case sf::Event::GainedFocus:
                    G_pGame->window.setFramerateLimit(0);
                    break;
                case sf::Event::MouseWheelScrolled:
                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                    {
                        if (event.mouseWheelScroll.delta > 0)
                            G_pGame->m_stMCursor.sZ = 1;
                        else
                            G_pGame->m_stMCursor.sZ = -1;
                    }
                    else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
                    {

                    }
                    else
                    {

                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        G_pGame->m_stMCursor.LB = true;
                        G_pGame->htmlUI->view->InjectMouseDown(Awesomium::MouseButton::kMouseButton_Right);
                    }
                    else if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        G_pGame->m_stMCursor.RB = true;
                        G_pGame->htmlUI->view->InjectMouseDown(Awesomium::MouseButton::kMouseButton_Left);
                    }
                    else if (event.mouseButton.button == sf::Mouse::Middle)
                    {
                        G_pGame->m_stMCursor.MB = true;
                        G_pGame->htmlUI->view->InjectMouseDown(Awesomium::MouseButton::kMouseButton_Middle);
                    }

                    if (G_pGame->wasinactive)
                    {
                        G_pGame->wasinactive = false;
                    }

                    break;
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        G_pGame->m_stMCursor.LB = false;
                        G_pGame->htmlUI->view->InjectMouseUp(Awesomium::MouseButton::kMouseButton_Right);
                    }
                    else if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        G_pGame->m_stMCursor.RB = false;
                        G_pGame->htmlUI->view->InjectMouseUp(Awesomium::MouseButton::kMouseButton_Left);
                    }
                    else if (event.mouseButton.button == sf::Mouse::Middle)
                    {
                        G_pGame->m_stMCursor.MB = false;
                        G_pGame->htmlUI->view->InjectMouseUp(Awesomium::MouseButton::kMouseButton_Middle);
                    }
                    break;
                case sf::Event::MouseMoved:
                    float diffx = static_cast<float>(G_pGame->screenwidth_v) / G_pGame->screenwidth;
                    float diffy = static_cast<float>(G_pGame->screenheight_v) / G_pGame->screenheight;
                    uint16_t x = event.mouseMove.x * diffx;
                    uint16_t y = event.mouseMove.y * diffy;

                    G_pGame->m_stMCursor.sX = x;
                    G_pGame->m_stMCursor.sY = y;

                    G_pGame->htmlUI->view->InjectMouseMove(x, y);
                    break;
            }
        }

        window.clear(sf::Color::Black);


        G_pGame->UpdateScreen();
        //draw here

        window.display();
	}

	G_pGame->signals_.cancel();
	G_pGame->Quit();
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