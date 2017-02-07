#pragma once

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>
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

    void Emit(string event, bool result, string message);
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
    CGame * game;
    JSValue window;

	HTMLUI(class CGame *pGame);
	~HTMLUI();
	void Init();
	bool isDirty();
	void Update(int sleep_ms);
    void SetCharacters();
};
