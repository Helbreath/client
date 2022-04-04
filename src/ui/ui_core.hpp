
#pragma once

#include <queue>
#include <fstream>
#include <utility>
#include <chrono>
#include <thread>
#include <map>
#include <shared_mutex>
#include <iostream>
#include <string>
#include <regex>
#include <filesystem>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "ui_input.hpp"
#include "ui_panel.hpp"

class CGame;

namespace ui
{

class dev_tools_client : public CefClient
{
private:
    IMPLEMENT_REFCOUNTING(dev_tools_client);
};

using json = nlohmann::json;

class ui_core;
class ui_panel;
class ui_input;

class ui_game
{
public:
    ui_game(CGame * _game, sf::WindowHandle _handle);
    ~ui_game() = default;

    ui_panel * create_panel(std::string name, std::string url, int x, int y, int width, int height);

    ui_core * core;
    CGame * game;
    sf::WindowHandle handle;
    bool is_initialized = false;
    //std::map<std::string, ui_panel *> panels; // this for multiple panels
    ui_panel * panel = nullptr; // single panel for SPA ui design

    void run_cef_thread();
    void do_work();

    bool is_running = true;
    bool begin_shutdown = false;
};

class ui_core : public CefApp,
    public CefClient,
    public CefBrowserProcessHandler,
    //     public CefRenderProcessHandler,
    public CefLifeSpanHandler,
    public CefLoadHandler,
    public CefRequestHandler,
    public CefDisplayHandler,
    public CefRenderHandler,
    public CefKeyboardHandler,
    public CefContextMenuHandler
{
public:
    ui_core(sf::WindowHandle _handle, CGame * _game) : handle(_handle), view(nullptr), game(_game) {}
    ~ui_core() = default;

    // on exit signal
    std::condition_variable cv;

    // CefClient
    CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
    CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }
    CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
    CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }
    CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override { return this; }


    // CefLoadHandler
    void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) override;
    void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override;
    void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString & errorText, const CefString & failedUrl) override;

    // CefRenderHandler
    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect & rect) override;
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList & dirtyRects, const void * buffer, int width, int height) override;
    bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int & screenX, int & screenY) override;

    // CefKeyboardHandler
    bool OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent & e, CefEventHandle os_event) override;

    // CefLifeSpanHandler
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    // CefRenderProcessHandler
//     virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info) override;
//     virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;
//     virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
//     virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
//     virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace) override;

    // CefBrowserProcessHandler
    void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) override;
    void OnContextInitialized() override;
    CefRefPtr<CefClient> GetDefaultClient() override { return this; };

    // CefClient
    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;
    void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) override;
    bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags) override;


    void add_browser_to_interface(ui_view * view);

    sf::WindowHandle handle;
    ui_view * view;
    bool create_browser = false;
    CGame * game;
    ui_panel * panel = nullptr; // single panel for SPA ui design

    ui_view * create_view(int width, int height, const std::string & url, bool transparent, ui_game * ui);

    void create_dev_tools()
    {
        CefWindowInfo windowInfo;
        windowInfo.SetAsPopup(nullptr, "Developer Tools");
        windowInfo.style = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
        windowInfo.x = 0;
        windowInfo.y = 0;
        windowInfo.width = 640;
        windowInfo.height = 480;
        panel->view->browser->GetHost()->ShowDevTools(windowInfo, new dev_tools_client(), CefBrowserSettings(), { 0, 0 });
    }

    IMPLEMENT_REFCOUNTING(ui_core);
};

}

