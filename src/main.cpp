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
#include "Game.h"
#include "GlobalDef.h"
//#include "listeners.h"
#include <condition_variable>

#include <intrin.h>
#include <sodium.h>
#include <fmt/format.h>

#include "HTMLUI.h"

#include "ui/ui_core.hpp"

//#define DEBUG_INSPECTOR

void * G_hWnd = 0;
void * G_hInstance = 0;
char   G_cSpriteAlphaDegree;
class CGame * G_pGame;

bool Initialize(char * pCmdLine);

bool isrunning = true;
bool is_ui_running = true;
bool restart_ui = false;

std::condition_variable cv;
std::condition_variable cv2;

#include <windows.h> 
#include <stdio.h> 
#include <conio.h>
#include <tchar.h>
#include <WinBase.h>

bool isvm()
{
    unsigned int cpuInfo[4];
    __cpuid((int *)cpuInfo, 1);
    return ((cpuInfo[2] >> 31) & 1) == 1;
}

#define BUFSIZE 512

int main(int argc, char * argv[])
{
    //////////////////////////////////////////////////////////////////////////
    // debug named pipes

    std::thread pipethead([&] {
        std::vector<std::thread> pipethreadlist;
        for (;;)
        {
            HANDLE hPipe = CreateNamedPipeA(
                "\\\\.\\pipe\\stupidcefdebuglog",             // pipe name 
                PIPE_ACCESS_DUPLEX,       // read/write access 
                PIPE_TYPE_MESSAGE |       // message type pipe 
                PIPE_READMODE_MESSAGE |   // message-read mode 
                PIPE_WAIT,                // blocking mode 
                PIPE_UNLIMITED_INSTANCES, // max. instances  
                BUFSIZE,                  // output buffer size 
                BUFSIZE,                  // input buffer size 
                0,                        // client time-out 
                NULL);                    // default security attribute 
            BOOL   fConnected = FALSE;
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
                        if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
                        if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
                        return;
                    }

                    if (pchRequest == NULL)
                    {
                        printf("\nERROR - Pipe Server Failure:\n");
                        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
                        printf("   InstanceThread exitting.\n");
                        if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
                        return;
                    }

                    if (pchReply == NULL)
                    {
                        printf("\nERROR - Pipe Server Failure:\n");
                        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
                        printf("   InstanceThread exitting.\n");
                        if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
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
                            pchRequest,    // buffer to receive data 
                            BUFSIZE, // size of buffer 
                            &cbBytesRead, // number of bytes read 
                            NULL);        // not overlapped I/O 

                        if (!fSuccess) break;

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

    //////////////////////////////////////////////////////////////////////////


    //std::this_thread::sleep_for(10s);
    //if (HTMLUICore::Main() >= 0)
    //    return -1;
/*
    CefSettings settings;
    settings.multi_threaded_message_loop = true;
    settings.windowless_rendering_enabled = true;
    settings.remote_debugging_port = 9222;

    HTMLUICore::GetInstance();

    HTMLUICore::sApp = new HTMLUIApp();

    HTMLUICore::sArgs = CefMainArgs(GetModuleHandle(NULL));

    const auto exit_code = CefExecuteProcess(HTMLUICore::sArgs, HTMLUICore::sApp.get(), nullptr);
    if (exit_code >= 0)
        return exit_code;*/

#ifndef _DEBUG
    HANDLE hMutex;
    if (OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "__DDrawWriteMode__") != NULL) {
        if (OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "DirectInput.{D48C3787-B82E-4601-BFC3-8E03380CCF96}") != NULL) {
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
    BOOL   fSuccess = FALSE;
    DWORD  cbRead, cbToWrite, cbWritten, dwMode;
    char lpszPipename[] = R"(\\.\pipe\hbxpipe)";

    // Try to open a named pipe; wait for it, if necessary. 

    while (1)
    {
        hPipe = CreateFileA(
            lpszPipename,   // pipe name 
            GENERIC_READ |  // read and write access 
            GENERIC_WRITE,
            0,              // no sharing 
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe 
            0,              // default attributes 
            NULL);          // no template file 

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
        hPipe,    // pipe handle 
        &dwMode,  // new pipe mode 
        NULL,     // don't set maximum bytes 
        NULL);    // don't set maximum time 
    if (!fSuccess)
    {
        MessageBoxA(NULL, "Unable to start!", "ERROR2!", MB_ICONEXCLAMATION | MB_OK);
        return -1;
    }

    // Send a message to the pipe server. 

/*
    cbToWrite = (lstrlen(lpvMessage) + 1) * sizeof(TCHAR);
    _tprintf(TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage);

    fSuccess = WriteFile(
        hPipe,                  // pipe handle 
        lpvMessage,             // message 
        cbToWrite,              // message length 
        &cbWritten,             // bytes written 
        NULL);                  // not overlapped 

    if (!fSuccess)
    {
        _tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
        return -1;
    }*/

    //printf("\nMessage sent to server, receiving reply as follows:\n");

    do
    {
        // Read from the pipe. 

        fSuccess = ReadFile(
            hPipe,    // pipe handle 
            chBuf,    // buffer to receive reply 
            BUFSIZE,  // size of buffer 
            &cbRead,  // number of bytes read 
            NULL);    // not overlapped 

        if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
            break;

        //std::cout << "\"%s\"" << chBuf << "\n";
    } while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

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
                G_pGame->m_cAccountPassword = strtok_s(0, "=:", &ctx);//TODO: encode this when sent from launcher
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
    //view->LoadURL("https://forum.helbreathx.net/");

    /*Ref<App> app = App::Create();

    Ref<UIWindow> uiwindow = UIWindow::Create(app->main_monitor(), 900, 600, false, kWindowFlags_Titled);

    uiwindow->SetTitle("Ultralight Sample 2 - Basic App");
    app->set_window(uiwindow);
    Ref<Overlay> overlay = Overlay::Create(uiwindow, uiwindow->width(), uiwindow->height(), 0, 0);
    overlay->view()->LoadHTML(htmlString());

    app->Run();*/



/*
    CefSettings settings;
    settings.multi_threaded_message_loop = false;
    settings.windowless_rendering_enabled = true;
    settings.remote_debugging_port = 9222;*/
/*
    cef_string_t * resources_path;
    char rpath[] = R"(C:\Code\x-client\bin\bin)";

    resources_path = cef_string_userfree_utf16_alloc();
    cef_string_ascii_to_utf16(rpath, sizeof(rpath), resources_path);*/

/*
    std::string s = u8"Hello, World!";

    // #include <codecvt>
    std::wstring_convert<std::codecvt<char16_t, char, std::mbstate_t>, char16_t> convert;

    std::u16string u16 = convert.from_bytes(s);*/

    //settings.resources_dir_path = *resources_path;


    //HTMLUICore::StartWeb();

    //CefInitialize(HTMLUICore::sArgs, settings, HTMLUICore::sApp.get(), NULL);

	if (!G_pGame->CreateRenderer())
	{
		return 0;
	}


	Initialize("");

    //G_pGame->window.setMouseCursorGrabbed(true);

    G_pGame->window.setMouseCursorGrabbed(false);
    G_pGame->window.setMouseCursorVisible(false);

    G_pGame->m_pSprite[SPRID_MOUSECURSOR] = CSprite::CreateSprite("interface", 0, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_SPRFONTS] = CSprite::CreateSprite("interface", 1, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_ND_LOGIN] = CSprite::CreateSprite("LoginDialog", 0, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_ND_MAINMENU] = CSprite::CreateSprite("New-Dialog", 1, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_ND_QUIT] = CSprite::CreateSprite("New-Dialog", 2, false);

    sf::Event event;
    sf::RenderWindow & window = G_pGame->window;

    std::thread cef_thread(std::bind(&ui::ui_game::run_cef_thread, G_pGame->cef_ui));

    //G_pGame->CreateUI();
    //HTMLUICore::RegisterScheme("game", "sprite", new HTMLUISpriteHandlerFactory());

    while (window.isOpen() && isrunning)
	{
//         while (sRegSchemeQueue.size() > 0)
//         {
//             CefRegisterSchemeHandlerFactory(sRegSchemeQueue.front().mName, sRegSchemeQueue.front().mDomain, sRegSchemeQueue.front().mFactory);
//             std::cout << fmt::format("Registering Scheme Handler {} - {}\n", sRegSchemeQueue.front().mName, sRegSchemeQueue.front().mDomain);
//             sRegSchemeQueue.pop();
//         }

/*
        while (_ui->view_queue.size() > 0)
        {
            GetInstance()->AddBrowserToInterface(_ui->view_queue.front());
            sViewQueue.pop();
        }*/


/*
        while (HTMLUICore::sRegSchemeQueue.size() > 0)
        {
            CefRegisterSchemeHandlerFactory(HTMLUICore::sRegSchemeQueue.front().mName, HTMLUICore::sRegSchemeQueue.front().mDomain, HTMLUICore::sRegSchemeQueue.front().mFactory);
            HTMLUICore::sRegSchemeQueue.pop();
        }*/

/*
        while (HTMLUICore::sViewQueue.size() > 0)
        {
            HTMLUICore::GetInstance()->AddBrowserToInterface(HTMLUICore::sViewQueue.front());
            HTMLUICore::sViewQueue.pop();
        }*/


        //timers first
        G_pGame->OnTimer();
        G_pGame->fps.update();

        //CefDoMessageLoopWork();
        while (window.pollEvent(event))
        {
            

            // "close requested" event: we close the window
			if (event.type == sf::Event::Closed) {
				window.close();
				break;
			}

			// Give the UI a chance to claim the event
/*
			std::vector<sf::Event::EventType> captureEvents{ sf::Event::KeyPressed, sf::Event::KeyReleased, sf::Event::MouseButtonPressed, sf::Event::MouseButtonReleased, sf::Event::TextEntered };
			if (std::find(captureEvents.begin(), captureEvents.end(), event.type) != captureEvents.end() && G_pGame->htmlUI->CaptureEvent(event)) {
				break;
			}
*/

            G_pGame->OnEvent(event);

        }

/*
        for (std::pair<std::string, HTMLUIPanel *> entry : G_pGame->htmlUI->panels) {
            auto * panel = entry.second;
            if (panel != nullptr && !panel->mView->GetBrowser()->IsLoading()) {
                panel->paint();
            }
        }*/

        {
            std::unique_lock<std::mutex> l(G_pGame->screenupdate);
            window.clear(sf::Color::Black);
            G_pGame->visible.clear(sf::Color::Black);

            G_pGame->UpdateScreen();
            //draw here

            window.display();
        }

        if (restart_ui)
        {
            //ui_session = renderer->CreateSession(false, fmt::format("{}", std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()).c_str());
/*
            restart_ui = false;
            is_ui_running = false;
            htmlrenderer.join();
            htmlrenderer = std::thread(start_ui_thread);
            cv.wait(l);*/
        }
	}
    isrunning = false;

    G_pGame->cef_ui->is_running = false;
    cef_thread.join();

    //HTMLUICore::EndWeb();
    //HTMLUICore::WaitForWebEnd();

    G_pGame->signals_.cancel();
    G_pGame->Quit();
    G_pGame->socketthread->join();
    if (window.isOpen()) window.close();
    G_pGame->io_service_.stop();
    delete G_pGame;

    //CefShutdown();

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