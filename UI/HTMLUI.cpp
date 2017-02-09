#include "../Game.h"


#include "HTMLUI.h"

extern CGame * G_pGame;

HTMLUI::HTMLUI(class CGame * pGame)
{
	WebConfig webConfig;
	// webConfig.remote_debugging_host = WSLit("127.0.0.1");
	// webConfig.remote_debugging_port = 9222;

	core = WebCore::Initialize(webConfig);
	view = this->core->CreateWebView(pGame->GetWidth(), pGame->GetHeight());
    game = pGame;

	jsNamespace = view->CreateGlobalJavascriptObject(WSLit("client"));
	jsData = jsNamespace.ToObject();
    jsData.SetCustomMethod(WSLit("log"), false);
    jsData.SetCustomMethod(WSLit("login"), false);
    jsData.SetCustomMethod(WSLit("selectcharacter"), false);
    jsData.SetCustomMethod(WSLit("entergame"), false);

	mHandler = new HTMLUIMethodHandler(this);
	lView = new HTMLUIViewListener(this);
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
	// WebURL url(WSLit("file:///g:/projects/hbx-ui/public/index.html"));
	view->LoadURL(url);
	view->SetTransparent(true);

	while (view->IsLoading()) {
		HTMLUI::Update(100);
	}
	HTMLUI::Update(1000);

	JSValue uiValue = view->ExecuteJavascriptWithResult(WSLit("UI"), WSLit(""));
	if (!uiValue.IsObject())
	{
		//ERROR
		__asm int 3;
	}

	uiJS = uiValue.ToObject();
	JSArray args;
	args.Push(WSLit("load"));
	uiJS.Invoke(WSLit("emit"), args);

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

JSValue HTMLUI::GetUI()
{
    JSValue ui = view->ExecuteJavascriptWithResult(WSLit("UI"), WSLit(""));
    if (!ui.IsObject())
    {
        //ERROR
        __asm int 3;
    }
    return ui;
}

void HTMLUI::SetCharacters()
{
    JSValue ui = GetUI();

    if (game->m_pCharList.size() > 0)
    {
        JSArray args;
        int i = 0;
        for (shared_ptr<CCharInfo> character : game->m_pCharList)
        {
            JSObject properties;
            properties.SetProperty(WSLit("id"), JSValue(i++));
            properties.SetProperty(WSLit("name"), JSValue(ToWebString(character->m_cName)));
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
        EmitObject("setcharacters", true, JSValue(args));
    }
}

HTMLUIViewListener::HTMLUIViewListener(HTMLUI * htmlUI)
{

}

void HTMLUIViewListener::OnAddConsoleMessage(Awesomium::WebView* caller, const Awesomium::WebString& message, int line_number, const Awesomium::WebString& source)
{
	printf("[JS] > %s [%s:%d]\n", ToString(message), line_number, ToString(source));
}

void HTMLUIViewListener::OnChangeTitle(Awesomium::WebView* caller, const Awesomium::WebString& title)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void HTMLUIViewListener::OnChangeAddressBar(Awesomium::WebView* caller, const Awesomium::WebURL& url)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void HTMLUIViewListener::OnChangeTooltip(Awesomium::WebView* caller, const Awesomium::WebString& tooltip)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void HTMLUIViewListener::OnChangeTargetURL(Awesomium::WebView* caller, const Awesomium::WebURL& url)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void HTMLUIViewListener::OnChangeCursor(Awesomium::WebView* caller, Awesomium::Cursor cursor)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void HTMLUIViewListener::OnChangeFocus(Awesomium::WebView* caller, Awesomium::FocusedElementType focused_type)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void HTMLUIViewListener::OnShowCreatedWebView(Awesomium::WebView* caller, Awesomium::WebView* new_view, const Awesomium::WebURL& opener_url, const Awesomium::WebURL& target_url, const Awesomium::Rect& initial_pos, bool is_popup)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HTMLUIMethodHandler::HTMLUIMethodHandler(HTMLUI * htmlUI)
{

}

void HTMLUIMethodHandler::OnMethodCall(WebView *caller, unsigned int remote_object_id, const WebString& method_name, const JSArray& args)
{
    JSObject obj;
    obj.SetProperty(WSLit("method_name"), method_name);
    obj.SetProperty(WSLit("args"), args);
    shared_ptr<CGame::UIMsgQueueEntry> entry = make_shared<CGame::UIMsgQueueEntry>();
    entry->obj = obj;
    htmlUI->game->PutUIMsgQueue(entry);
    return;
}

JSValue HTMLUIMethodHandler::OnMethodCallWithReturnValue(WebView *caller, unsigned int remote_object_id, const WebString& method_name, const JSArray& args)
{
	return JSValue::Undefined();
}

void HTMLUI::Emit(string event, bool result, string message)
{
    JSValue ui = GetUI();

    JSObject properties;
    properties.SetProperty(WSLit("success"), JSValue(result));
    properties.SetProperty(WSLit("message"), JSValue(ToWebString(message)));

    JSArray args;
	args.Push(ToWebString(event));
    args.Push(properties);
    if (ui.ToObject().HasMethod(WSLit("emit")))
        ui.ToObject().Invoke(WSLit("emit"), args);
    else
        __asm int 3;//TODO: error handling
}

void HTMLUI::EmitObject(string event, bool result, JSValue obj)
{
    JSValue ui = GetUI();

    JSArray args;
    args.Push(ToWebString(event));
    args.Push(obj);
    if (ui.ToObject().HasMethod(WSLit("emit")))
        ui.ToObject().Invoke(WSLit("emit"), args);
    else
        __asm int 3;//TODO: error handling
}
