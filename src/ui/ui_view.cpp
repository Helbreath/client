
#include "ui_view.hpp"
#include <iostream>

namespace ui
{

CefRefPtr<CefV8Value> json_to_cef(json o)
{
    if (o.is_number_float()) {
        return CefV8Value::CreateDouble(o);
    }
    else if (o.is_number()) {
        return CefV8Value::CreateInt(o);
    }
    else if (o.is_boolean()) {
        return CefV8Value::CreateBool(o);
    }
    else if (o.is_array()) {
        CefRefPtr<CefV8Value> arr = CefV8Value::CreateArray(0);
        int i = 0;
        for (json entry : o) {
            arr->SetValue(i, json_to_cef(entry));
        }
        return arr;
    }
    else if (o.is_object()) {
        CefRefPtr<CefV8Value> obj = CefV8Value::CreateObject(NULL, NULL);
        for (json::iterator oi = o.begin(); oi != o.end(); ++oi) {
            obj->SetValue(oi.key(), json_to_cef(oi.value()), V8_PROPERTY_ATTRIBUTE_NONE);
        }
        return obj;
    }
    else if (o.is_string()) {
        std::string s = o;
        return CefV8Value::CreateString(s);
    }

    return CefV8Value::CreateNull();
}

ui_view::ui_view(int _width, int _height, const std::string & _url, bool _transparent, ui_game * _ui)
    : texture_width(_width)
    , texture_height(_height)
    , current_url(_url)
    , transparent(_transparent)
    , js_context(NULL)
    , js_object(NULL)
    , browser(NULL)
    , texture(NULL)
{
    texture = new sf::Texture();
    texture->create(texture_width, texture_height);
}

ui_view::~ui_view()
{
    if (texture)
        delete texture;
}

void ui_view::send_focus_event(bool set_focus)
{
    if (!browser)
        return;

    browser->GetHost()->SendFocusEvent(set_focus);
}

void ui_view::send_mouse_click_event(int x, int y, sf::Mouse::Button button, bool mouse_up, int click_count)
{
    if (!browser)
        return;

    CefBrowserHost::MouseButtonType type = button == sf::Mouse::Left ? MBT_LEFT : button == sf::Mouse::Right ? MBT_RIGHT : MBT_MIDDLE;

    CefMouseEvent e;
    e.x = x;
    e.y = y;
    e.modifiers = get_mouse_modifiers();
    browser->GetHost()->SendMouseMoveEvent(e, false);
    browser->GetHost()->SendMouseClickEvent(e, type, mouse_up, click_count);
}

void ui_view::send_mouse_move_event(int x, int y, bool mouse_leave /*= false*/)
{
    if (!browser)
        return;

    CefMouseEvent e;
    e.x = x;
    e.y = y;
    e.modifiers = get_mouse_modifiers();
    browser->GetHost()->SendMouseMoveEvent(e, mouse_leave);
}

void ui_view::send_mouse_wheel_event(int x, int y, int deltax, int deltay)
{
    if (!browser)
        return;

    CefMouseEvent e;
    e.x = x;
    e.y = y;
    e.modifiers = get_mouse_modifiers();
    browser->GetHost()->SendMouseWheelEvent(e, 0, 30 * deltay);
}

void ui_view::send_key_event(WPARAM key, bool key_up, bool is_system /*= false*/, int modifiers /*= -1*/)
{
    if (!browser)
        return;

    CefKeyEvent e;
    e.windows_key_code = key;
    e.modifiers = modifiers == -1 ? get_keyboard_modifiers() : modifiers;
    e.type = key_up ? KEYEVENT_KEYUP : KEYEVENT_KEYDOWN;
    e.is_system_key = is_system;
    e.character = key;
    e.unmodified_character = key; //e.native_key_code = 0;
    browser->GetHost()->SendKeyEvent(e);
}

void ui_view::send_key_event(char key, int modifiers /*= -1*/)
{
    if (!browser)
        return;

    CefKeyEvent e;
    e.windows_key_code = key;
    e.modifiers = modifiers == -1 ? get_keyboard_modifiers() : modifiers;
    e.type = KEYEVENT_CHAR;
    e.character = key;
    e.unmodified_character = key;
    browser->GetHost()->SendKeyEvent(e);
}

void ui_view::add_js_binding(const std::string name, js_binding::js_callback callback)
{
    //     HTMLUICore::sBindings.insert(std::make_pair(std::make_pair(name, mBrowser->GetIdentifier()), callback));
    // 
    //     CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("jsbinding_create");
    //     message->GetArgumentList()->SetString(0, name.c_str());
    //     message->GetArgumentList()->SetInt(1, mBrowser->GetIdentifier());
    // 
    //     mBrowser->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);
    // 
    //     //We have to reload the page otherwise the bindings wont be added to the context.
    //     mBrowser->Reload();
}

void ui_view::add_js_bindings(const std::vector<js_binding> bindings)
{
    //     for (unsigned int i = 0; i < bindings.size(); i++)
    //     {
    //         HTMLUICore::sBindings.insert(std::make_pair(std::make_pair(bindings[i].mFunctionName, mBrowser->GetIdentifier()), bindings[i].mfpJSCallback));
    // 
    //         CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("jsbinding_create");
    //         message->GetArgumentList()->SetString(0, bindings[i].mFunctionName);
    //         message->GetArgumentList()->SetInt(1, mBrowser->GetIdentifier());
    // 
    //         mBrowser->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);
    //     }
    // 
    //     //We have to reload the page otherwise the bindings wont be added to the context.
    //     mBrowser->Reload();
}

void ui_view::execute_js(const CefString & code)
{
    if (!browser)
        return;

    execute_js(code, browser->GetMainFrame());
}

void ui_view::execute_js(const CefString & code, CefRefPtr<CefFrame> frame)
{
    if (!browser)
        return;

    //Should probably check to make sure the frame is from our browser here.

    execute_js(code, frame, 0);
}

void ui_view::execute_js(const CefString & code, CefRefPtr<CefFrame> frame, int start_line)
{
    std::cout << "execute_js\n";
    if (!browser)
        return;

    //Should probably check to make sure the frame is from our browser here.

    frame->ExecuteJavaScript(code, frame->GetURL(), start_line);
}

bool ui_view::jscallback(const CefString & name, CefRefPtr<CefListValue> arguments)
{
    //     bool result = false;
    //     //Check if this is one of our bindings.
    // 
    //     if (HTMLUICore::sBindings.count(std::make_pair(name, mBrowser->GetIdentifier())))
    //     {
    //         result = HTMLUICore::sBindings[std::make_pair(name, mBrowser->GetIdentifier())](arguments);
    //     }
    // 
    //     //Otherwise fallthrough and return false.
    //     return result;

    return false;
}

void ui_view::emit(std::string method, json object)
{
    std::unique_lock<std::mutex> l(m);
    pending_emit.emplace(method, std::move(object));
}

void ui_view::send_emitters()
{
    std::unique_lock<std::mutex> l(m);

    if (pending_emit.empty())
        return;

    if (!browser)
        return;

    //CefRefPtr<CefV8Value> ui = js_object->GetValue("UI");
    while (!pending_emit.empty())
    {
        auto & e = pending_emit.front();
        //         CefV8ValueList args;
        //         args.push_back(CefV8Value::CreateString(e.first));
        //         args.push_back(json_to_cef(e.second));


        CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(e.first);
        message->GetArgumentList()->SetString(0, e.second.dump());
        browser->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);


        //         if (ui->HasValue("emit")) {
        //             auto fnc = ui->GetValue("emit");
        //             if (fnc->IsFunction()) {
        //                 if (!fnc->ExecuteFunctionWithContext(js_context, NULL, args)) {
        //                     printf("Unable to execute function: %s", e.first.c_str());
        //                 }
        //             }
        //         }
        pending_emit.pop();
    }
}

int ui_view::get_mouse_modifiers()
{
    int mod = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
        mod |= EVENTFLAG_CONTROL_DOWN;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
        mod |= EVENTFLAG_SHIFT_DOWN;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt))
        mod |= EVENTFLAG_ALT_DOWN;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        mod |= EVENTFLAG_LEFT_MOUSE_BUTTON;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
        mod |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        mod |= EVENTFLAG_RIGHT_MOUSE_BUTTON;

    // Low bit set from GetKeyState indicates "toggled".
    //if (::GetKeyState(VK_NUMLOCK) & 1)
    //	mod |= EVENTFLAG_NUM_LOCK_ON;
    //if (::GetKeyState(VK_CAPITAL) & 1)
    //	mod |= EVENTFLAG_CAPS_LOCK_ON;

    return mod;
}


int ui_view::get_keyboard_modifiers()
{
    int mod = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        mod |= EVENTFLAG_CONTROL_DOWN;// mod |= EVENTFLAG_IS_LEFT;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
        mod |= EVENTFLAG_CONTROL_DOWN;// mod |= EVENTFLAG_IS_RIGHT;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        mod |= EVENTFLAG_SHIFT_DOWN;// mod |= EVENTFLAG_IS_LEFT;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
        mod |= EVENTFLAG_SHIFT_DOWN;// mod |= EVENTFLAG_IS_RIGHT;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        mod |= EVENTFLAG_ALT_DOWN;// mod |= EVENTFLAG_IS_LEFT;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt))
        mod |= EVENTFLAG_ALT_DOWN;// mod |= EVENTFLAG_IS_RIGHT;

    return mod;
}

void ui_view::set_size(int width, int height)
{
    texture_width = width;
    texture_height = height;

    sf::Lock lock(sfmutex);

    sf::Texture * pOldTexture = texture;
    texture = new sf::Texture();
    texture->create(texture_width, texture_height);
    texture->setSmooth(true);
    if (pOldTexture)
        delete pOldTexture;

    //Clear the update rects
    while (update_rects.size() > 0)
    {
        delete[] update_rects.front().buffer;
        update_rects.pop();
    }

    if (browser)
    {
        browser->GetHost()->Invalidate(PET_VIEW);
        browser->GetHost()->WasResized();
    }
}

void ui_view::update_texture()
{
    sf::Lock lock(sfmutex);
    while (update_rects.size() > 0)
    {
        int size = update_rects.size();
        const CefRect & rect = update_rects.front().rect;
        texture->update((sf::Uint8 *)update_rects.front().buffer, rect.width, rect.height, rect.x, rect.y);
        delete[] update_rects.front().buffer;
        update_rects.pop();
    }
}

}
