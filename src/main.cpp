// -------------------------------------------------------------- 
//                      Helbreath Client 						  
//
//                      1998.10 by Soph
//
// --------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN

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

#include <intrin.h>
#include <sodium.h>
#include <fmt/format.h>

#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>

//#define DEBUG_INSPECTOR

void * G_hWnd = 0;
void * G_hInstance = 0;
char   G_cSpriteAlphaDegree;
class CGame * G_pGame;

MyListener lstnr;
MyViewListener viewlstnr;

bool Initialize(char * pCmdLine);

using namespace ultralight;
using UIWindow = ultralight::Window;

RefPtr<ultralight::View> view;
RefPtr<ultralight::View> inspector_view;
RefPtr<ultralight::Renderer> renderer;
RefPtr<ultralight::Session> ui_session;

bool isrunning = true;
bool is_ui_running = true;
bool restart_ui = false;
std::thread htmlrenderer;
std::condition_variable cv;

#include <windows.h> 
#include <stdio.h> 
#include <conio.h>
#include <tchar.h>

class MyLogger : public Logger {
public:
    MyLogger() {}
    virtual ~MyLogger() {}

    ///
    /// Called when the library wants to print a message to the log.
    ///
    virtual void LogMessage(ultralight::LogLevel log_level, const String16& message) override {
        printf("%s\n", ultralight::String(message).utf8().data());
    }
};

MyLogger uilogger;

void start_ui_thread()
{
    Config config;
    config.resource_path = "./resources";
    config.use_gpu_renderer = false;
    config.device_scale = 1.0;
    config.enable_javascript = true;
    config.enable_images = true;
    Platform::instance().set_config(config);

    Platform::instance().set_font_loader(GetPlatformFontLoader());
    Platform::instance().set_file_system(GetPlatformFileSystem("."));
    //Platform::instance().set_logger(GetDefaultLogger("ultralight.log"));
    Platform::instance().set_logger(&uilogger);
    //Platform::instance().set_gpu_driver();

    renderer = Renderer::Create();

    view = renderer->CreateView(G_pGame->screenwidth_v, G_pGame->screenheight_v, true, nullptr);

    view->set_load_listener(&lstnr);
    view->set_view_listener(&viewlstnr);

    view->Focus();

    //view->LoadURL("file:///ui/index.html");
    view->LoadURL("http://localhost:8080/");
    //view->LoadURL("https://helbreath.io/ui/main.html");

#ifdef DEBUG_INSPECTOR
    inspector_view = view->inspector();
    inspector_view->Resize(1024, G_pGame->inspector_size);

    G_pGame->inspectoractive = true;

    inspector_view->Focus();
#endif


    /*
            {
                Ref<JSContext> context = view->LockJSContext();
                SetJSContext(context.get());
                JSObject global = JSGlobalObject();
                global["SelfUpdate"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(SelfUpdate);
            }*/

    cv.notify_all();

    while (is_ui_running && isrunning)
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10ms);
        {
            std::unique_lock<std::recursive_mutex> l(G_pGame->_html_m);

            {
                std::unique_lock<std::recursive_mutex> l(G_pGame->_html_eventm);
                int i = 10;
                while (--i)
                {
                    if (!G_pGame->_html_eventqueue.empty())
                    {
                        auto evt = G_pGame->_html_eventqueue.front();
                        evt();
                        G_pGame->_html_eventqueue.pop();
                    }
                }
            }

            renderer->Update();
            renderer->Render();

            G_pGame->dirty_html = !view->surface()->dirty_bounds().IsEmpty() || (inspector_view ? !inspector_view->surface()->dirty_bounds().IsEmpty() : false);
            //view->surface()->ClearDirtyBounds();
            if (inspector_view) inspector_view->surface()->ClearDirtyBounds();
        }
    }
}

bool isvm()
{
    unsigned int cpuInfo[4];
    __cpuid((int *)cpuInfo, 1);
    return ((cpuInfo[2] >> 31) & 1) == 1;
}

#define BUFSIZE 512

int main(int argc, char * argv[])
{
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

    G_pGame = new class CGame;

#ifndef _DEBUG
    G_pGame->m_cAccountName = received_login.substr(0, pos);
    G_pGame->m_cAccountPassword = received_login.substr(pos + 1);
    G_pGame->autologin = true;
#endif

    G_pGame->_renderer = "OpenGL";

    G_pGame->inside_vm = isvm();

#ifdef WIN32
    SetCurrentDirectoryA("../");
#else
#endif

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

	if (!G_pGame->CreateRenderer())
	{
		return 0;
	}


	Initialize("");

    G_pGame->window.setMouseCursorGrabbed(true);
    G_pGame->window.setMouseCursorVisible(false);

    G_pGame->m_pSprite[SPRID_MOUSECURSOR] = CSprite::CreateSprite("interface", 0, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_SPRFONTS] = CSprite::CreateSprite("interface", 1, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_ND_LOGIN] = CSprite::CreateSprite("LoginDialog", 0, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_ND_MAINMENU] = CSprite::CreateSprite("New-Dialog", 1, false);
    G_pGame->m_pSprite[SPRID_INTERFACE_ND_QUIT] = CSprite::CreateSprite("New-Dialog", 2, false);

// 	G_pGame->clipmousegame = true;
// 	G_pGame->clipmousewindow = true;

	int grace = 0;

    sf::Event event;
    sf::RenderWindow & window = G_pGame->window;


    std::mutex m;

    std::unique_lock<std::mutex> l(m);

    htmlrenderer = std::thread(start_ui_thread);

    cv.wait(l);
 
    while (window.isOpen() && isrunning)
	{
        //timers first
        G_pGame->OnTimer();
        G_pGame->fps.update();

        
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
	G_pGame->signals_.cancel();
	G_pGame->Quit();
	G_pGame->socketthread->join();
    view->Stop();
    htmlrenderer.join();
    if (window.isOpen()) window.close();
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