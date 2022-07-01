//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include <cstdio>
#include <cstdlib>
#include <process.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <sys/timeb.h>
#include "helbreath.h"
#include "GlobalDef.h"
#include <condition_variable>
#include <filesystem>

#include <intrin.h>
#include <fmt/format.h>
#include <ixwebsocket/IXWebSocket.h>

char G_cSpriteAlphaDegree;
helbreath * game;

bool isrunning = true;
bool is_ui_running = true;
bool restart_ui = false;

std::condition_variable cv;
std::condition_variable cv2;

#if defined(WIN32)
#include <windows.h>
#include <conio.h>
#include <tchar.h>
#include <WinBase.h>
#endif

bool isvm()
{
    unsigned int cpuInfo[4] = {};
    __cpuid((int *)cpuInfo, 1);
    return ((cpuInfo[2] >> 31) & 1) == 1;
}

constexpr uint64_t BUFSIZE = 2048;
#define UNLIMITED_CLIENTS

int main(int argc, char * argv[])
{
    srand((unsigned)time(nullptr));

    std::filesystem::current_path("../");

    game = new helbreath();

#if !defined(_DEBUG) && !defined(UNLIMITED_CLIENTS)
    game->m_cAccountName = received_login.substr(0, pos);
    game->m_cAccountPassword = received_login.substr(pos + 1);
    game->autologin = true;
#endif

    game->_renderer = "OpenGL";

    game->inside_vm = isvm();


    if (argc >= 2)
    {
        for (int i = 0; i < argc; ++i)
        {
#ifdef _DEBUG

            if (memcmp(argv[i], "-data=", 6) == 0)
            {
                char * ctx;
                char * c = strtok_s(argv[i], "=:", &ctx);
                game->m_cAccountName = strtok_s(nullptr, "=:", &ctx);
                game->m_cAccountPassword = strtok_s(nullptr, "=:", &ctx);
                game->autologin = true;
            }

            //debug only stuff
            if (memcmp(argv[i], "-res=", 5) == 0)
            {
                char * ctx;
                char * c = strtok_s(argv[i], "=x", &ctx);
                game->screenwidth = atoi(strtok_s(nullptr, "=x", &ctx));
                game->screenheight = atoi(strtok_s(nullptr, "=x", &ctx));
                game->autoresolution = true;
            }
            else if (memcmp(argv[i], "-vres=", 6) == 0)
            {
                char * ctx;
                char * c = strtok_s(argv[i], "=x", &ctx);
                game->screenwidth_v = atoi(strtok_s(nullptr, "=x", &ctx));
                game->screenheight_v = atoi(strtok_s(nullptr, "=x", &ctx));
                game->autovresolution = true;
            }
#endif
        }
    }

    if (!game->CreateRenderer())
    {
        return 0;
    }

    if (game->bInit() == false)
    {
        return false;
    }

    game->window.setMouseCursorGrabbed(game->clipmousegame);
    game->window.setMouseCursorVisible(false);

    // Load interface sprites first to show sprites during loading
    game->m_pSprite[SPRID_MOUSECURSOR] = sprite::CreateSprite("interface", 0, false);
    game->m_pSprite[SPRID_INTERFACE_SPRFONTS] = sprite::CreateSprite("interface", 1, false);
    game->m_pSprite[SPRID_INTERFACE_ND_LOGIN] = sprite::CreateSprite("LoginDialog", 0, false);
    game->m_pSprite[SPRID_INTERFACE_ND_MAINMENU] = sprite::CreateSprite("New-Dialog", 1, false);
    game->m_pSprite[SPRID_INTERFACE_ND_QUIT] = sprite::CreateSprite("New-Dialog", 2, false);

    sf::Event event;
    sf::RenderWindow & window = game->window;

    while (window.isOpen() && isrunning)
    {
        //timers first
        game->OnTimer();
        game->fps.update();

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
            {
                isrunning = false;
                window.close();
                break;
            }

            game->OnEvent(event);
        }

        {
            std::unique_lock<std::mutex> l(game->screenupdate);
            window.clear(sf::Color::Black);
            game->visible.clear(sf::Color::Black);

            game->UpdateScreen();

            window.display();
        }
    }
    isrunning = false;

    game->Quit();
    if (window.isOpen())
        window.close();

    game->ws->stop();
    delete game;

#if !defined(_DEBUG) && !defined(UNLIMITED_CLIENTS)
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
#endif

    return 0;
}
