//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
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
#include "global_def.h"
#include <condition_variable>
#include <filesystem>

#include <intrin.h>
#include <fmt/format.h>

char G_cSpriteAlphaDegree;
helbreath * game;

bool Initialize(char * pCmdLine);

bool isrunning = true;
bool is_ui_running = true;
bool restart_ui = false;

std::condition_variable cv;
std::condition_variable cv2;

#include <windows.h>
#include <conio.h>
#include <tchar.h>
#include <WinBase.h>

bool isvm()
{
    unsigned int cpuInfo[4] = {};
    __cpuid((int *)cpuInfo, 1);
    return ((cpuInfo[2] >> 31) & 1) == 1;
}

constexpr uint64_t BUFSIZE = 2048;

int main(int argc, char * argv[])
{
#ifndef _DEBUG
    HANDLE hMutex;
    if (OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "__DDrawWriteMode__") != NULL)
    {
        if (OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "DirectInput.{D48C3787-B82E-4601-BFC3-8E03380CCF96}") != NULL)
        {
            MessageBoxA(NULL, "Only two Helbreath Xtreme instances allowed!", "ERROR!", MB_OK);
            return 0;
        }
        else
            hMutex = CreateMutexA(NULL, FALSE, "DirectInput.{D48C3787-B82E-4601-BFC3-8E03380CCF96}");
    }
    else
        hMutex = CreateMutexA(NULL, FALSE, "__DDrawWriteMode__");
#endif

#if !defined(_DEBUG) && !defined(UNLIMITED_CLIENTS)
    HANDLE hPipe;
    char chBuf[BUFSIZE];
    BOOL fSuccess = FALSE;
    DWORD cbRead, cbToWrite, cbWritten, dwMode;
    char lpszPipename[] = R"(\\.\pipe\hbxpipe)";

    // Try to open a named pipe; wait for it, if necessary.

    while (1)
    {
        hPipe = CreateFileA(
            lpszPipename,  // pipe name
            GENERIC_READ | // read and write access
            GENERIC_WRITE,
            0,             // no sharing
            NULL,          // default security attributes
            OPEN_EXISTING, // opens existing pipe
            0,             // default attributes
            NULL);         // no template file

        // Break if the pipe handle is valid.

        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        // Exit if an error other than ERROR_PIPE_BUSY occurs.

        if (GetLastError() != ERROR_PIPE_BUSY) 
        {
            //printf("Could not open pipe. GLE=%d\n", GetLastError());
            MessageBoxA(NULL, "You must launch Helbreath Xtreme from the launcher!", "ERROR!", MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }

        // All pipe instances are busy, so wait for 20 seconds.

        if (!WaitNamedPipeA(lpszPipename, 20000))
        {
            MessageBoxA(NULL, "Unable to start!", "ERROR!", MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
    }

    // The pipe connected; change to message-read mode.

    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(
        hPipe,   // pipe handle
        &dwMode, // new pipe mode
        NULL,    // don't set maximum bytes
        NULL);   // don't set maximum time
    if (!fSuccess)
    {
        MessageBoxA(NULL, "Unable to start!", "ERROR2!", MB_ICONEXCLAMATION | MB_OK);
        return -1;
    }

    do
    {
        // Read from the pipe.

        fSuccess = ReadFile(
            hPipe,   // pipe handle
            chBuf,   // buffer to receive reply
            BUFSIZE, // size of buffer
            &cbRead, // number of bytes read
            NULL);   // not overlapped

        if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
            break;

        //std::cout << "\"%s\"" << chBuf << "\n";
    } while (!fSuccess); // repeat loop if ERROR_MORE_DATA

    if (!fSuccess)
    {
        MessageBoxA(NULL, "Unable to start!", fmt::format("ERROR-{}!", GetLastError()).c_str(), MB_ICONEXCLAMATION | MB_OK);
        //std::cout << "ReadFile from pipe failed. GLE=%d" << GetLastError() << "\n";
        return -1;
    }

    CloseHandle(hPipe);

    std::string received_login = chBuf;
    auto pos = received_login.find_first_of("\n");
    if (pos == std::string::npos)
    {
        MessageBoxA(NULL, "Unable to start!", "ERROR4!", MB_ICONEXCLAMATION | MB_OK);
        return -1;
    }
#endif

    /*
    {
        int core_count = std::thread::hardware_concurrency();
        HANDLE process = GetCurrentProcess();

        BOOL success = 0;

        switch (core_count)
        {
            case 1:
                success = SetProcessAffinityMask(process, 0b00000001);
                break;
            case 2:
                success = SetProcessAffinityMask(process, 0b00000010);
                break;
            case 3:
                success = SetProcessAffinityMask(process, 0b00000100);
                break;
            case 4:
                success = SetProcessAffinityMask(process, 0b00001100);
                break;
            case 5:
                success = SetProcessAffinityMask(process, 0b00011000);
                break;
            case 6:
                success = SetProcessAffinityMask(process, 0b00110000);
                break;
            default:
                success = SetProcessAffinityMask(process, 0b00110000);
                break;
        }
    }*/

    srand((unsigned)time(0));

    std::filesystem::current_path("../");

    game = new class helbreath;

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
    //game->window.setMouseCursorGrabbed(true);
    game->window.setMouseCursorVisible(false);

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
    delete game;

#if !defined(_DEBUG) && !defined(UNLIMITED_CLIENTS)
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
#endif

    return 0;
}
