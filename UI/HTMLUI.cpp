#include "HTMLUI.h"

HTMLUI::HTMLUI(class CGame * pGame)
{
	core = WebCore::Initialize(WebConfig());
	view = this->core->CreateWebView(WIDTH, HEIGHT);

	jsNamespace = view->CreateGlobalJavascriptObject(WSLit("client"));
	jsData = jsNamespace.ToObject();
	jsData.SetCustomMethod(WSLit("log"), false);

	mHandler = new HTMLUIMethodHandler(this);
	view->set_js_method_handler(mHandler);
	view->Focus();
}


HTMLUI::~HTMLUI()
{
}

void HTMLUI::Init()
{
	// WebURL url(WSLit("http://hbx.decouple.io/index.html"));
	WebURL url(WSLit("file:///G:/projects/hbx-ui/public/index.html"));
	view->LoadURL(url);
	view->SetTransparent(true);
	while (view->IsLoading()) {
		HTMLUI::Update(25);
	}
	HTMLUI::Update(250);

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
	}
}

JSValue HTMLUIMethodHandler::OnMethodCallWithReturnValue(WebView *caller, unsigned int remote_object_id, const WebString& method_name, const JSArray& args)
{
	return JSValue::Undefined();
}