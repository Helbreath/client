#include "../Game.h"


#include "HTMLUI.h"

extern CGame * G_pGame;

HTMLUI::HTMLUI(class CGame * pGame)
{
	core = WebCore::Initialize(WebConfig());
	view = this->core->CreateWebView(pGame->GetWidth(), pGame->GetHeight());
    game = pGame;

	jsNamespace = view->CreateGlobalJavascriptObject(WSLit("client"));
	jsData = jsNamespace.ToObject();
    jsData.SetCustomMethod(WSLit("log"), false);
    jsData.SetCustomMethod(WSLit("login"), false);
    jsData.SetCustomMethod(WSLit("selectcharacter"), false);
    jsData.SetCustomMethod(WSLit("entergame"), false);

	mHandler = new HTMLUIMethodHandler(this);
	view->set_js_method_handler(mHandler);
	view->Focus();
    mHandler->htmlUI = this;
}


HTMLUI::~HTMLUI()
{
}

void HTMLUI::Init()
{
	WebURL url(WSLit("http://hbx.decouple.io/index.html"));
	view->LoadURL(url);
	view->SetTransparent(true);

	while (view->IsLoading()) {
		HTMLUI::Update(10);
	}
	HTMLUI::Update(100);

	uiValue = view->ExecuteJavascriptWithResult(WSLit("UI"), WSLit(""));
	if (!uiValue.IsObject())
	{
		//ERROR
		__asm int 3;
	}

	uiJS = uiValue.ToObject();
	JSArray args;
	args.Push(WSLit("load"));
	uiJS.Invoke(WSLit("emit"), args);

    window = view->ExecuteJavascriptWithResult(WSLit("window"), WSLit(""));
    if (!window.IsObject())
    {
        //ERROR
        __asm int 3;
    }

	surface = (BitmapSurface*)view->surface();
}

bool HTMLUI::isDirty()
{
	return surface && surface->is_dirty();
}

void HTMLUI::Update(int sleep_ms)
{
	// Sleep a specified amount
#if defined(__WIN32__) || defined(_WIN32)
	Sleep(sleep_ms);
#elif defined(__APPLE__)
	usleep(sleep_ms * 1000);
#endif

	// You must call WebCore::update periodically
	// during the lifetime of your application.
	WebCore::instance()->Update();
}

void HTMLUI::SetCharacters()
{
    if (!window.IsObject())
    {
        //ERROR
        __asm int 3;
    }

    if (game->m_pCharList.size() > 0)
    {
        JSArray args;
        JSObject command;
        command.SetProperty(WSLit("command"), JSValue(WSLit("setcharacters")));
        args.Push(command);
        for (shared_ptr<CCharInfo> character : game->m_pCharList)
        {
            JSObject properties;
            properties.SetProperty(WSLit("sex"), JSValue(character->m_sSex));
            properties.SetProperty(WSLit("level"), JSValue(character->m_sLevel));
            properties.SetProperty(WSLit("exp"), JSValue(static_cast<double>(character->m_iExp)));
            properties.SetProperty(WSLit("str"), JSValue(character->m_sStr));
            properties.SetProperty(WSLit("vit"), JSValue(character->m_sVit));
            properties.SetProperty(WSLit("dex"), JSValue(character->m_sDex));
            properties.SetProperty(WSLit("int"), JSValue(character->m_sInt));
            properties.SetProperty(WSLit("mag"), JSValue(character->m_sMag));
            properties.SetProperty(WSLit("chr"), JSValue(character->m_sChr));
            properties.SetProperty(WSLit("mapname"), JSValue(ToWebString(character->m_cMapName)));
            args.Push(properties);
        }
        JSObject obj = window.ToObject();
        obj.Invoke(WSLit("UI.emit"), args);
    }
}

HTMLUIMethodHandler::HTMLUIMethodHandler(HTMLUI * htmlUI)
{

}

void HTMLUIMethodHandler::OnMethodCall(WebView *caller, unsigned int remote_object_id, const WebString& method_name, const JSArray& args)
{
	if (method_name == WSLit("log")) {
		std::string buffer = "";
		int i = 0;
		while (i < args.size()) {
			JSValue entry = args.At(i);
			buffer = buffer + Awesomium::ToString(entry.ToString()) + " ";
			i++;
		}
		printf("[JS] > %s\n", buffer.c_str());
        return;
	}
    else if (method_name == WSLit("login"))
    {
        if (args.size() < 2)
        {
            Emit("login", false, "Invalid login information");
            return;
        }
        else
        {
            WebString username = args.At(0).ToString();
            WebString password = args.At(1).ToString();

            if (username.IsEmpty() || password.IsEmpty())
            {
                Emit("login", false, "Username and password cannot be empty");
                return;
            }

            G_pGame->m_cAccountName = ToString(username);
            G_pGame->m_cAccountPassword = ToString(password);
            G_pGame->StartLogin();
            G_pGame->ChangeGameMode(GAMEMODE_ONCONNECTING);
            Emit("login", true, "");
            return;
        }
    }
    else if (method_name == WSLit("selectcharacter"))
    {
        if (args.size() == 0)
        {
            Emit("selectcharacter", false, "Invalid character");
            return;
        }
        else
        {
            int16_t selectedchar = static_cast<int16_t>(args.At(0).ToInteger());

            if (G_pGame->m_pCharList.size() > selectedchar)
            {
                G_pGame->m_cCurFocus = selectedchar;
                Emit("selectcharacter", true, "");
            }
            return;
        }
    }
    else if (method_name == WSLit("entergame"))
    {
        if (G_pGame->m_pCharList[G_pGame->m_cCurFocus] != 0)
        {
            if (G_pGame->m_pCharList[G_pGame->m_cCurFocus]->m_sSex != 0)
            {
                ZeroMemory(G_pGame->m_cPlayerName, sizeof(G_pGame->m_cPlayerName));
                strcpy(G_pGame->m_cPlayerName, G_pGame->m_pCharList[G_pGame->m_cCurFocus]->m_cName.c_str());
                G_pGame->m_iLevel = (int)G_pGame->m_pCharList[G_pGame->m_cCurFocus]->m_sLevel;
                if (G_pGame->m_Misc.bCheckValidString(G_pGame->m_cPlayerName) == true)
                {
                    G_pGame->m_pSprite[SPRID_INTERFACE_ND_LOGIN]->_iCloseSprite();
                    G_pGame->m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->_iCloseSprite();
                    G_pGame->ChangeGameMode(GAMEMODE_ONCONNECTING);
                    G_pGame->m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                    G_pGame->m_wEnterGameType = ENTERGAMEMSGTYPE_NEW;
                    ZeroMemory(G_pGame->m_cMsg, sizeof(G_pGame->m_cMsg));
                    strcpy(G_pGame->m_cMsg, "33");
                    ZeroMemory(G_pGame->m_cMapName, sizeof(G_pGame->m_cMapName));
                    memcpy(G_pGame->m_cMapName, G_pGame->m_pCharList[G_pGame->m_cCurFocus]->m_cMapName.c_str(), 10);
                    Emit("entergame", true, "");
                    return;
                }
                Emit("entergame", false, "Invalid character name");
                return;
            }
        }
    }
}

JSValue HTMLUIMethodHandler::OnMethodCallWithReturnValue(WebView *caller, unsigned int remote_object_id, const WebString& method_name, const JSArray& args)
{
	return JSValue::Undefined();
}

void HTMLUIMethodHandler::Emit(string event, bool result, string message)
{
    JSObject properties;
    properties.SetProperty(WSLit("event"), JSValue(ToWebString(event)));
    properties.SetProperty(WSLit("success"), JSValue(false));
    properties.SetProperty(WSLit("message"), JSValue(ToWebString(message)));

    JSArray args;
    args.Push(properties);
    JSObject obj = htmlUI->window.ToObject();
    if (obj.HasProperty(WSLit("UI.emit")))
        obj.Invoke(WSLit("UI.emit"), args);
    else
        __asm int 3;
}

