/*

#include "listeners.h"
#include "Game.h"
#include <fstream>
#include <chrono>

using namespace ultralight;
using namespace std::chrono_literals;

extern RefPtr<ultralight::View> view;
extern RefPtr<ultralight::Renderer> renderer;
extern CGame* G_pGame;
extern MyListener lstnr;
extern MyViewListener viewlstnr;
extern bool restart_ui;
extern RefPtr<ultralight::Session> ui_session;

void SelfUpdate(const JSObject& thisObject, const JSArgs& args)
{
    std::cout << "Updated html.\n";
    //view->LoadURL("file:///ui/main.html");


    G_pGame->_html_eventqueue.emplace([]() mutable {
        std::unique_lock<std::recursive_mutex> l(G_pGame->_html_m);
        //view->Stop();
        //view->Release();
      
        view = renderer->CreateView(G_pGame->screenwidth_v, G_pGame->screenheight_v, true, nullptr);

        view->set_load_listener(&lstnr);
        view->set_view_listener(&viewlstnr);

        view->Focus();

        //restart_ui = true;

        ui_session = renderer->CreateSession(false, fmt::format("{}", std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()).c_str());


        //view->LoadURL("file:///ui/index.html");
        view->LoadURL("http://localhost:8080/");

/ *
        std::ifstream file("ui/main.html", std::ios::in);

        if (!file.is_open())
            return;

        file.seekg(0, std::ios::end);
        int64_t count = file.tellg();
        file.seekg(0, std::ios::beg);
        char * data = new char[count + 1];
        memset(data, 0, count + 1);
        file.read(data, count);

        std::string html = data;
        delete[] data;

        std::string now = fmt::format("{}", std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());

        std::stringstream result;
        std::string replacement = "HBX_TIMESTAMP_TEMPLATE";
        for (int j = 0; j < html.size(); j++)
        {
            string key = html.substr(j, replacement.length());
            if (key == replacement)
            {
                result << now;
                j += replacement.length();
            }
            result << html[j];
        }
        html = result.str();
/ *
        std::vector<std::string> res = replace_timestamp_template(html, "HBX_TIMESTAMP_TEMPLATE");

        std::stringstream result;

        for (int i = 0; i < res.size(); ++i)
        {
            if (i == res.size())
                result << res[i];
            else
                result << res[i] << now;
        }

        html = result.str();* /

        view->LoadHTML(html.c_str());* /
    });



    //return JSValue("Hello from C++!<br/>Ultralight rocks!");
}

void MyListener::OnBeginLoading(ultralight::View* caller)
{
    std::cout << "Loading\n";
}
void MyListener::OnFinishLoading(ultralight::View* caller)
{
    std::cout << "Finish Loading\n";

    ultralight::Ref<ultralight::JSContext> context = view->LockJSContext();
    ultralight::SetJSContext(context.get());
    ultralight::JSObject global = ultralight::JSGlobalObject();
    ultralight::JSFunction OnLoad = global["OnLoad"];
    if (OnLoad.IsValid())
    {
        OnLoad({});
    }
}
void MyListener::OnUpdateHistory(ultralight::View* caller)
{
    std::cout << "Update History\n";
}
void MyListener::OnDOMReady(ultralight::View* caller)
{
    std::cout << "Dom Ready\n";
}

void MyViewListener::OnChangeTitle(ultralight::View* caller, const ultralight::String& title)
{
    std::cout << "On Change Title\n";
}
void MyViewListener::OnChangeURL(ultralight::View* caller, const ultralight::String& url)
{
    /// All javascript function/variable bindings must be changed here every single reload

    std::cout << "On Change URL\n";
    //         std::unique_lock<std::mutex> l(G_pGame->_html_eventm);
    //         G_pGame->_html_eventqueue.emplace([]()
    //         {
    Ref<JSContext> context = view->LockJSContext();
    SetJSContext(context.get());
    JSObject global = JSGlobalObject();
    global["SelfUpdate"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(SelfUpdate);
    global["StartLoading"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(std::bind(&CGame::start_loading, G_pGame));
    global["GetGameMode"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(std::bind(&CGame::update_ui_game_mode, G_pGame));
    //global["GetCharacterData"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(std::bind(&CGame::get_character_data, G_pGame));
    //global["GetGuildData"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(std::bind(&CGame::get_guild_data, G_pGame));
    //global["GetMapData"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(std::bind(&CGame::get_map_data, G_pGame));
    global["SendMessage"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(std::bind(&CGame::receive_message_from_ui, G_pGame, std::placeholders::_1, std::placeholders::_2));
    //global["UpdateLoading"] = std::function<void(const JSObject& thisObject, const JSArgs& args)>(std::bind(&CGame::thing, G_pGame));
//        });
}
void MyViewListener::OnChangeTooltip(ultralight::View* caller, const ultralight::String& tooltip)
{
    //std::cout << "On Change Tooltip\n";
}
void MyViewListener::OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor)
{
    //std::cout << "On Change Cursor\n";
}
void MyViewListener::OnAddConsoleMessage(ultralight::View* caller, MessageSource source, MessageLevel level, const ultralight::String& message, uint32_t line_number, uint32_t column_number, const ultralight::String& source_id)
{
    std::cout << fmt::format("Console Message : {} - {}:{}\n{}\n", source_id.utf8().data(), line_number, column_number, message.utf8().data());
    //view->Stop();
    //view->LoadURL("https://google.com");
    //view->Reload();
}
*/
