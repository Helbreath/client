
#pragma once

#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_base.h>
#include <include/cef_render_handler.h>
#include <include/base/cef_bind.h>
#include <include/cef_parser.h>
#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>
#include <include/wrapper/cef_closure_task.h>
#include <include/wrapper/cef_helpers.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <queue>
#include <mutex>
#include "../json.hpp"
#include <functional>

namespace ui
{

using namespace nlohmann;

class ui_game;

struct update_rect
{
    char * buffer;
    CefRect rect;
};

struct js_binding
{
    using js_callback = std::function<bool(CefRefPtr<CefListValue> arguments)>;
    std::string function_name;
    std::function<bool(CefRefPtr<CefListValue> arguments)> callback;

    js_binding(const std::string & _function_name, js_callback _callback)
        :function_name(_function_name),
        callback(_callback)
    {}
};

class ui_view : public CefBaseRefCounted
{
public:

    ui_view(int width, int height, const std::string & url, bool transparent, ui_game * ui);
    virtual ~ui_view();

    void send_focus_event(bool set_focus);
    void send_mouse_click_event(int x, int y, sf::Mouse::Button button, bool mouse_up, int click_count);
    void send_mouse_move_event(int x, int y, bool mouse_leave = false);
    void send_mouse_wheel_event(int x, int y, int deltax, int deltay);
    void send_key_event(WPARAM key, bool key_up, bool is_system = false, int modifiers = -1);
    void send_key_event(char key, int modifiers = -1);
    void add_js_binding(const std::string name, js_binding::js_callback callback);
    void add_js_bindings(const std::vector<js_binding> bindings);
    void execute_js(const CefString & code);
    void execute_js(const CefString & code, CefRefPtr<CefFrame> frame);
    void execute_js(const CefString & code, CefRefPtr<CefFrame> frame, int start_line);
    bool jscallback(const CefString & name, CefRefPtr<CefListValue> arguments);

    int get_width() { return texture_width; }
    int get_height() { return texture_height; }

    void emit(std::string method, json object);
    void send_emitters();
    std::queue<std::pair<std::string, json>> pending_emit;

    int get_mouse_modifiers();
    int get_keyboard_modifiers();

    sf::Texture * get_texture() { return texture; }
    std::string get_current_url() { return current_url; }
    bool is_transparent() { return transparent; }
    void set_size(int width, int height);
    void update_texture();

    void clear_browser() { browser = NULL; }

    CefRefPtr<CefV8Context> js_context;
    CefRefPtr<CefV8Value> js_object;
    CefRefPtr<CefV8Handler> js_handler;
    CefRefPtr<CefBrowser> browser;

    std::mutex m;
    sf::Mutex sfmutex;
    sf::Texture * texture;
    sf::WindowHandle handle;

    int texture_width;
    int texture_height;

    std::queue<update_rect> update_rects;
    std::string current_url;
    bool transparent;

    IMPLEMENT_REFCOUNTING(ui_view);
};

}
