
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

using json = nlohmann::json;

class ui_core;
class ui_panel;
class ui_input;

class ui_game
{
public:
    ui_game(CGame * _game, sf::WindowHandle _handle);
    ~ui_game() {}

    ui_panel * create_panel(std::string name, std::string url, int x, int y, int width, int height);

    ui_core * core;
    CGame * game;
    sf::WindowHandle handle;
    bool is_initialized = false;
    //std::map<std::string, ui_panel *> panels; // this for multiple panels
    ui_panel * panel = nullptr; // single panel for SPA ui design

    void run_cef_thread();

    bool is_running = true;
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
    ui_core(sf::WindowHandle _handle, CGame * _game): handle(_handle), view(nullptr), game(_game) {}
    ~ui_core() {}

    // CefClient
    virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
    virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }
    virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override { return this; }


    // CefLoadHandler
    virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) override;
    virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override;
    virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString & errorText, const CefString & failedUrl) override;

    // CefRenderHandler
    virtual void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect & rect) override;
    virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList & dirtyRects, const void * buffer, int width, int height) override;
    virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int & screenX, int & screenY) override;

    // CefKeyboardHandler
    virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent & e, CefEventHandle os_event) override;

    // CefLifeSpanHandler
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    // CefRenderProcessHandler
//     virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info) override;
//     virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;
//     virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
//     virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
//     virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace) override;

    // CefBrowserProcessHandler
    virtual void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) override;
    virtual void OnContextInitialized() override;
    virtual CefRefPtr<CefClient> GetDefaultClient() override { return this; };

    // CefClient
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;
    virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) override;
    virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags) override;


    void add_browser_to_interface(ui_view * view);

    sf::WindowHandle handle;
    ui_view * view;
    bool create_browser = false;
    CGame * game;
    ui_panel * panel = nullptr; // single panel for SPA ui design

    ui_view * create_view(int width, int height, const std::string & url, bool transparent, ui_game * ui);

private:
    IMPLEMENT_REFCOUNTING(ui_core);
};

}

