#pragma once

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/WebViewListener.h>
using namespace Awesomium;

#define WIDTH   800
#define HEIGHT  600

class CGame;
class HTMLUI;

class HTMLUIMethodHandler : public JSMethodHandler
{
public:
	HTMLUIMethodHandler(class HTMLUI *htmlUI);
	void OnMethodCall(WebView *caller, unsigned int remote_object_id, const WebString& method_name, const JSArray& args);
	JSValue OnMethodCallWithReturnValue(WebView *caller, unsigned int remote_object_id, const WebString& method_name, const JSArray& args);

	HTMLUI *htmlUI;
};

class HTMLUIViewListener : public WebViewListener::View {
public:
	HTMLUIViewListener(class HTMLUI *htmlUI);
	void OnAddConsoleMessage(Awesomium::WebView* caller, const Awesomium::WebString& message, int line_number, const Awesomium::WebString& source);
	virtual void OnChangeTitle(Awesomium::WebView* caller, const Awesomium::WebString& title) override;
	virtual void OnChangeAddressBar(Awesomium::WebView* caller, const Awesomium::WebURL& url) override;
	virtual void OnChangeTooltip(Awesomium::WebView* caller, const Awesomium::WebString& tooltip) override;
	virtual void OnChangeTargetURL(Awesomium::WebView* caller, const Awesomium::WebURL& url) override;
	virtual void OnChangeCursor(Awesomium::WebView* caller, Awesomium::Cursor cursor) override;
	virtual void OnChangeFocus(Awesomium::WebView* caller, Awesomium::FocusedElementType focused_type) override;
	virtual void OnShowCreatedWebView(Awesomium::WebView* caller, Awesomium::WebView* new_view, const Awesomium::WebURL& opener_url, const Awesomium::WebURL& target_url, const Awesomium::Rect& initial_pos, bool is_popup) override;
};

class HTMLUI
{
public:
	WebView *view;
	WebCore *core;
	BitmapSurface *surface;
	JSValue jsNamespace;
	JSObject jsData;
	HTMLUIMethodHandler *mHandler;
	HTMLUIViewListener *lView;
    CGame * game;
    JSObject uiJS;

	HTMLUI(class CGame *pGame);
	~HTMLUI();
	void Init();
	bool isDirty();
	void Update(int sleep_ms);
    void SetCharacters();
    JSValue GetUI();
    void Emit(string event, bool result, string message);
    void EmitObject(string event, bool result, JSValue obj);
};
