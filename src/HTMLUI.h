#pragma once

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/WebViewListener.h>
#include <Awesomium/ResourceInterceptor.h>
#include <map>
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

class HTMLUIResourceInterceptor : public ResourceInterceptor {
public:
	HTMLUIResourceInterceptor(class HTMLUI *htmlUI);
	ResourceResponse* OnRequest(ResourceRequest *request);
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
	HTMLUIResourceInterceptor *iResource;
    CGame * game;
    JSObject uiJS;

	std::map<sf::Keyboard::Key, int> keyMap{
		// A-Z
		{ sf::Keyboard::A, Awesomium::KeyCodes::AK_A },
		{ sf::Keyboard::B, Awesomium::KeyCodes::AK_B },
		{ sf::Keyboard::C, Awesomium::KeyCodes::AK_C },
		{ sf::Keyboard::D, Awesomium::KeyCodes::AK_D },
		{ sf::Keyboard::E, Awesomium::KeyCodes::AK_E },
		{ sf::Keyboard::F, Awesomium::KeyCodes::AK_F },
		{ sf::Keyboard::G, Awesomium::KeyCodes::AK_G },
		{ sf::Keyboard::H, Awesomium::KeyCodes::AK_H },
		{ sf::Keyboard::I, Awesomium::KeyCodes::AK_I },
		{ sf::Keyboard::J, Awesomium::KeyCodes::AK_J },
		{ sf::Keyboard::K, Awesomium::KeyCodes::AK_K },
		{ sf::Keyboard::L, Awesomium::KeyCodes::AK_L },
		{ sf::Keyboard::M, Awesomium::KeyCodes::AK_M },
		{ sf::Keyboard::N, Awesomium::KeyCodes::AK_N },
		{ sf::Keyboard::O, Awesomium::KeyCodes::AK_O },
		{ sf::Keyboard::P, Awesomium::KeyCodes::AK_P },
		{ sf::Keyboard::Q, Awesomium::KeyCodes::AK_Q },
		{ sf::Keyboard::R, Awesomium::KeyCodes::AK_R },
		{ sf::Keyboard::S, Awesomium::KeyCodes::AK_S },
		{ sf::Keyboard::T, Awesomium::KeyCodes::AK_T },
		{ sf::Keyboard::U, Awesomium::KeyCodes::AK_U },
		{ sf::Keyboard::V, Awesomium::KeyCodes::AK_V },
		{ sf::Keyboard::W, Awesomium::KeyCodes::AK_W },
		{ sf::Keyboard::X, Awesomium::KeyCodes::AK_X },
		{ sf::Keyboard::Y, Awesomium::KeyCodes::AK_Y },
		{ sf::Keyboard::Z, Awesomium::KeyCodes::AK_Z },
		// 0-9
		{ sf::Keyboard::Num0, Awesomium::KeyCodes::AK_0 },
		{ sf::Keyboard::Num1, Awesomium::KeyCodes::AK_1 },
		{ sf::Keyboard::Num2, Awesomium::KeyCodes::AK_2 },
		{ sf::Keyboard::Num3, Awesomium::KeyCodes::AK_3 },
		{ sf::Keyboard::Num4, Awesomium::KeyCodes::AK_4 },
		{ sf::Keyboard::Num5, Awesomium::KeyCodes::AK_5 },
		{ sf::Keyboard::Num6, Awesomium::KeyCodes::AK_6 },
		{ sf::Keyboard::Num7, Awesomium::KeyCodes::AK_7 },
		{ sf::Keyboard::Num8, Awesomium::KeyCodes::AK_8 },
		{ sf::Keyboard::Num9, Awesomium::KeyCodes::AK_9 },
		// Function keys
		{ sf::Keyboard::BackSpace, Awesomium::KeyCodes::AK_BACK },
		{ sf::Keyboard::Return, Awesomium::KeyCodes::AK_RETURN },
		{ sf::Keyboard::Tab, Awesomium::KeyCodes::AK_TAB },
		{ sf::Keyboard::Left, Awesomium::KeyCodes::AK_LEFT },
		{ sf::Keyboard::Right, Awesomium::KeyCodes::AK_RIGHT },
		{ sf::Keyboard::Up, Awesomium::KeyCodes::AK_UP },
		{ sf::Keyboard::Down, Awesomium::KeyCodes::AK_DOWN },
		{ sf::Keyboard::Home, Awesomium::KeyCodes::AK_HOME },
		{ sf::Keyboard::End, Awesomium::KeyCodes::AK_END },
		{ sf::Keyboard::Delete, Awesomium::KeyCodes::AK_DELETE },
		{ sf::Keyboard::Insert, Awesomium::KeyCodes::AK_INSERT },
		{ sf::Keyboard::LControl, Awesomium::KeyCodes::AK_LCONTROL },
		{ sf::Keyboard::RControl, Awesomium::KeyCodes::AK_RCONTROL }
	};

	HTMLUI(class CGame *pGame);
	~HTMLUI();
	void Init();
	bool isDirty();
	void Update(int sleep_ms);
    void SetCharacters();
    JSValue GetUI();
    void Emit(string event, bool result, string message);
    void EmitObject(string event, JSValue obj, bool async=false);
	void MouseMove(int x, int y);
	bool CaptureEvent(sf::Event event);
};
