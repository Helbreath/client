// --------------------------------------------------------------
//                      Helbreath Client
//
//                      1998.10 by Soph
//
// --------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <process.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <sys/timeb.h>
#include "Game.h"
#include "GlobalDef.h"
#include <condition_variable>

#include <intrin.h>
#include <sodium.h>
#include <fmt/format.h>

#include "ui/ui_core.hpp"

void * G_hWnd = nullptr;
void * G_hInstance = nullptr;
char G_cSpriteAlphaDegree;
class CGame * G_pGame;

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
    unsigned int cpuInfo[4];
    __cpuid((int *)cpuInfo, 1);
    return ((cpuInfo[2] >> 31) & 1) == 1;
}

#define BUFSIZE 2048

int main(int argc, char * argv[])
{
    //////////////////////////////////////////////////////////////////////////
    // debug named pipes

#ifdef _DEBUG
    std::thread pipethead([&] {
        std::vector<std::thread> pipethreadlist;
        for (;;)
        {
            HANDLE hPipe = CreateNamedPipeA(
                "\\\\.\\pipe\\stupidcefdebuglog", // pipe name
                PIPE_ACCESS_DUPLEX,               // read/write access
                PIPE_TYPE_MESSAGE |               // message type pipe
                PIPE_READMODE_MESSAGE |       // message-read mode
                PIPE_WAIT,                    // blocking mode
                PIPE_UNLIMITED_INSTANCES,         // max. instances
                BUFSIZE,                          // output buffer size
                BUFSIZE,                          // input buffer size
                0,                                // client time-out
                NULL);                            // default security attribute
            BOOL fConnected = FALSE;
            fConnected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
            if (fConnected)
            {
                printf("Client connected, creating a processing thread.\n");

                // Create a thread for this client.
                pipethreadlist.push_back(std::thread([&, hPipe] {
                    HANDLE hHeap = GetProcessHeap();
                    char * pchRequest = (char *)HeapAlloc(hHeap, 0, BUFSIZE);
                    char * pchReply = (char *)HeapAlloc(hHeap, 0, BUFSIZE);

                    DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
                    BOOL fSuccess = FALSE;

                    if (hPipe == NULL)
                    {
                        printf("\nERROR - Pipe Server Failure:\n");
                        printf("   InstanceThread got an unexpected NULL value in lpvParam.\n");
                        printf("   InstanceThread exitting.\n");
                        if (pchReply != NULL)
                            HeapFree(hHeap, 0, pchReply);
                        if (pchRequest != NULL)
                            HeapFree(hHeap, 0, pchRequest);
                        return;
                    }

                    if (pchRequest == NULL)
                    {
                        printf("\nERROR - Pipe Server Failure:\n");
                        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
                        printf("   InstanceThread exitting.\n");
                        if (pchReply != NULL)
                            HeapFree(hHeap, 0, pchReply);
                        return;
                    }

                    if (pchReply == NULL)
                    {
                        printf("\nERROR - Pipe Server Failure:\n");
                        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
                        printf("   InstanceThread exitting.\n");
                        if (pchRequest != NULL)
                            HeapFree(hHeap, 0, pchRequest);
                        return;
                    }

                    // Print verbose messages. In production code, this should be for debugging only.
                    printf("InstanceThread created, receiving and processing messages.\n");

                    // The thread's parameter is a handle to a pipe object instance.

                    // Loop until done reading
                    while (1)
                    {
                        // Read client requests from the pipe. This simplistic code only allows messages
                        // up to BUFSIZE characters in length.
                        fSuccess = ReadFile(
                            hPipe,        // handle to pipe
                            pchRequest,   // buffer to receive data
                            BUFSIZE,      // size of buffer
                            &cbBytesRead, // number of bytes read
                            NULL);        // not overlapped I/O

                        if (!fSuccess)
                            break;

                        std::cout << "!!CEF!!: " << pchRequest << "\n";
                    }

                    // Flush the pipe to allow the client to read the pipe's contents
                    // before disconnecting. Then disconnect the pipe, and close the
                    // handle to this pipe instance.

                    FlushFileBuffers(hPipe);
                    DisconnectNamedPipe(hPipe);
                    CloseHandle(hPipe);

                    HeapFree(hHeap, 0, pchRequest);
                    HeapFree(hHeap, 0, pchReply);

                    printf("InstanceThread exiting.\n");
                    return;
                }));
            }
            else
                // The client could not connect, so close the pipe.
                CloseHandle(hPipe);
        }
    });
#endif
    //////////////////////////////////////////////////////////////////////////

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

#ifndef _DEBUG
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

    sodium_init();

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

#ifdef WIN32
    SetCurrentDirectoryA("../");
#else
#endif

    G_pGame = new class CGame;

#ifndef _DEBUG
    G_pGame->m_cAccountName = received_login.substr(0, pos);
    G_pGame->m_cAccountPassword = received_login.substr(pos + 1);
    G_pGame->autologin = true;
#endif

    G_pGame->_renderer = "OpenGL";

    G_pGame->inside_vm = isvm();

    if (argc >= 2)
    {
        for (int i = 0; i < argc; ++i)
        {
#ifdef _DEBUG

            if (!memcmp(argv[i], "-data=", 6))
            {
                char * ctx;
                char * c = strtok_s(argv[i], "=:", &ctx);
                G_pGame->m_cAccountName = strtok_s(0, "=:", &ctx);
                G_pGame->m_cAccountPassword = strtok_s(0, "=:", &ctx);
                G_pGame->autologin = true;
            }

            //debug only stuff
            if (!memcmp(argv[i], "-res=", 5))
            {
                char * ctx;
                char * c = strtok_s(argv[i], "=x", &ctx);
                G_pGame->screenwidth = atoi(strtok_s(0, "=x", &ctx));
                G_pGame->screenheight = atoi(strtok_s(0, "=x", &ctx));
                G_pGame->autoresolution = true;
            }
            else if (!memcmp(argv[i], "-vres=", 6))
            {
                char * ctx;
                char * c = strtok_s(argv[i], "=x", &ctx);
                G_pGame->screenwidth_v = atoi(strtok_s(0, "=x", &ctx));
                G_pGame->screenheight_v = atoi(strtok_s(0, "=x", &ctx));
                G_pGame->autovresolution = true;
            }
#endif
        }
    }

    if (!G_pGame->CreateRenderer())
    {
        return 0;
    }

    Initialize("");

    G_pGame->window.setMouseCursorGrabbed(G_pGame->clipmousegame);
    //G_pGame->window.setMouseCursorGrabbed(true);
    G_pGame->window.setMouseCursorVisible(false);

    G_pGame->m_pSprite[SPRID_MOUSECURSOR] = CSprite::CreateSprite("interface", 0, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_SPRFONTS] = CSprite::CreateSprite("interface", 1, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_ND_LOGIN] = CSprite::CreateSprite("LoginDialog", 0, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_ND_MAINMENU] = CSprite::CreateSprite("New-Dialog", 1, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_ND_QUIT] = CSprite::CreateSprite("New-Dialog", 2, false);

    sf::Event event;
    sf::RenderWindow & window = G_pGame->window;

    std::thread cef_thread(std::bind(&ui::ui_game::run_cef_thread, G_pGame->cef_ui));

    while (window.isOpen() && isrunning)
    {
        //timers first
        G_pGame->OnTimer();
        G_pGame->fps.update();

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }

            G_pGame->OnEvent(event);
        }

        {
            std::unique_lock<std::mutex> l(G_pGame->screenupdate);
            window.clear(sf::Color::Black);
            G_pGame->visible.clear(sf::Color::Black);

            G_pGame->UpdateScreen();

            window.display();
        }

    }
    isrunning = false;

    G_pGame->cef_ui->is_running = false;
    cef_thread.join();

    G_pGame->signals_.cancel();
    G_pGame->Quit();
    G_pGame->socketthread->join();
    if (window.isOpen())
        window.close();
    G_pGame->io_service_.stop();
    delete G_pGame;

#ifndef _DEBUG
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
#endif

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
