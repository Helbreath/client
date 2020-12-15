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

void * G_hWnd = 0;
void * G_hInstance = 0;
char   G_cSpriteAlphaDegree;
class CGame * G_pGame;

bool Initialize(char * pCmdLine);

using namespace ultralight;
using UIWindow = ultralight::Window;

RefPtr<ultralight::View> view;

void SelfUpdate(const JSObject& thisObject, const JSArgs& args)
{
    std::cout << "Updated html.\n";
    view->LoadURL("file:///ui/main.html");
    //return JSValue("Hello from C++!<br/>Ultralight rocks!");
}

class MyListener: public ultralight::LoadListener {
    virtual void OnBeginLoading(ultralight::View* caller)
    {
        std::cout << "Loading\n";
    }
    virtual void OnFinishLoading(ultralight::View* caller)
    {
        std::cout << "Finish Loading\n";
    }
    virtual void OnUpdateHistory(ultralight::View* caller)
    {
        std::cout << "Update History\n";
    }
    virtual void OnDOMReady(ultralight::View* caller)
    {
        std::cout << "Dom Ready\n";
    }
};

class MyViewListener : public ultralight::ViewListener {
    virtual void OnChangeTitle(ultralight::View* caller, const ultralight::String& title)
    {
        std::cout << "On Change Title\n";
    }
    virtual void OnChangeURL(ultralight::View* caller, const ultralight::String& url)
    {
        /// All javascript function/variable bindings must be changed here every single reload

        std::cout << "On Change URL\n";
        std::unique_lock<std::mutex> l(G_pGame->_html_eventm);
        G_pGame->_html_eventqueue.emplace([]()
        {
            Ref<JSContext> context = view->LockJSContext();
            SetJSContext(context.get());
            JSObject global = JSGlobalObject();
            global["SelfUpdate"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(SelfUpdate);
            //global["UpdateLoading"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(std::bind(&CGame::thing, G_pGame));
        });
    }
    virtual void OnChangeTooltip(ultralight::View* caller, const ultralight::String& tooltip)
    {
        //std::cout << "On Change Tooltip\n";
    }
    virtual void OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor)
    {
        //std::cout << "On Change Cursor\n";
    }
    virtual void OnAddConsoleMessage(ultralight::View* caller, MessageSource source, MessageLevel level, const ultralight::String& message, uint32_t line_number, uint32_t column_number, const ultralight::String& source_id)
    {
        std::cout << "Console Message\n" << message.utf8().data() << "\n";
        //view->Stop();
        //view->LoadURL("https://google.com");
        //view->Reload();
    }
};

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

//debug only stuff
            else if (!memcmp(argv[i], "-res=", 5))
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

    bool isrunning = true;

    std::condition_variable cv;

    std::mutex m;

    std::unique_lock<std::mutex> l(m);

    std::thread htmlrenderer([&]() {
        Config config;
        config.resource_path = "./resources/";
        config.use_gpu_renderer = false;
        config.device_scale = 1.0;
        config.enable_javascript = true;
        config.enable_images = true;
        Platform::instance().set_config(config);

        Platform::instance().set_font_loader(GetPlatformFontLoader());
        Platform::instance().set_file_system(GetPlatformFileSystem("."));
        //Platform::instance().set_logger(GetDefaultLogger("ultralight.log"));
        Platform::instance().set_logger(new MyLogger());
        //Platform::instance().set_gpu_driver();

        RefPtr<Renderer> renderer = Renderer::Create();

        view = renderer->CreateView(G_pGame->screenwidth_v, G_pGame->screenheight_v, true, nullptr);
        MyListener lstnr;
        MyViewListener viewlstnr;
        view->set_load_listener(&lstnr);
        view->set_view_listener(&viewlstnr);

        view->Focus();

        view->LoadURL("file:///ui/main.html");


/*
        {
            Ref<JSContext> context = view->LockJSContext();
            SetJSContext(context.get());
            JSObject global = JSGlobalObject();
            global["SelfUpdate"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(SelfUpdate);
        }*/

        cv.notify_all();

        while (isrunning)
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(10ms);
            {
                std::unique_lock<std::mutex> l(G_pGame->_html_m);

                {
                    std::unique_lock<std::mutex> l(G_pGame->_html_eventm);
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

                G_pGame->dirty_html = !view->surface()->dirty_bounds().IsEmpty();
                view->surface()->ClearDirtyBounds();
            }
        }
    });

    cv.wait(l);
 
    while (window.isOpen())
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

        window.clear(sf::Color::Black);
        //G_pGame->visible.clear(sf::Color::Black);

        G_pGame->UpdateScreen();
        //draw here

        window.display();
	}

    isrunning = false;
	G_pGame->signals_.cancel();
	G_pGame->Quit();
	G_pGame->socketthread->join();
    view->Stop();
    htmlrenderer.join();
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