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


#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_base.h>
#include <include/cef_render_handler.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "json.hpp"

#include "include/base/cef_bind.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

using json = nlohmann::json;

#define WIDTH   800
#define HEIGHT  600

class CGame;
class HTMLUI;
class HTMLUIPanel;
class HTMLUICore;
class HTMLUIApp;

struct JSBinding
{
public:
	typedef bool(*JSCallback) (CefRefPtr<CefListValue> arguments);
	std::string mFunctionName;
	JSCallback mfpJSCallback;

	JSBinding(std::string name, JSCallback callback)
		:mFunctionName(name),
		mfpJSCallback(callback)
	{}
};

class WebV8Handler : public CefV8Handler
{
	friend class HTMLUICore;

public:
	WebV8Handler() {}

	virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override;

private:
	IMPLEMENT_REFCOUNTING(WebV8Handler);
};

CefRefPtr<CefV8Value> JSONtoCef(json o);

class HTMLUICore : public CefApp,
	public CefBrowserProcessHandler,
	public CefRenderProcessHandler,
	public CefClient,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefRequestHandler,
	public CefDisplayHandler,
	public CefRenderHandler,
	public CefKeyboardHandler,
	public CefContextMenuHandler
{

public:
	friend class HTMLUIView;
	friend class WebV8Handler;
public:
    virtual void OnContextInitialized() override {
    }
	static int Main();
	static void SetSubprocess(const std::string path) { sSubprocess = path; }
	static void SetSingleProcess(bool single) { sSingleProcess = single; }
	static void StartWeb();
	static void EndWeb();
	static void WaitForWebEnd();
	static void RegisterScheme(std::string name, std::string domain, CefRefPtr<CefSchemeHandlerFactory> factory);
	static HTMLUIView* CreateView(int width, int height, const std::string& url, bool transparent, HTMLUI *ui, void(*cb)(HTMLUIView*));
	static void UpdateInterfaceTextures();
	static CefRefPtr<HTMLUICore> GetInstance();
    virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override { return this; }
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) override;
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override;
	virtual void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
	virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override;
	virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& e, CefEventHandle os_event) override;
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
    //virtual void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) override;
    //virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) override;
	virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info) override;
	virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
	virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
	virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace) override;
	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;

	virtual void OnBeforeCommandLineProcessing(const CefString & process_type, CefRefPtr<CefCommandLine> command_line) override
	{
		command_line->AppendSwitchWithValue("log-level", "0");
	}

	void CefV8Array2ListValue(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target);
	void CefListValue2V8Array(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target);
	void CefV8JsonObject2DictionaryValue(CefRefPtr<CefV8Value>source, CefRefPtr<CefDictionaryValue> target);
	void CefDictionaryValue2V8JsonObject(CefRefPtr<CefDictionaryValue> source, CefRefPtr<CefV8Value> target);

    void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) override;
    bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags) override;

    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }
//private:

	struct RegScheme
	{
	public:
		RegScheme(std::string name, std::string domain, CefRefPtr<CefSchemeHandlerFactory> factory)
			:mName(name)
			, mDomain(domain)
			, mFactory(factory)
		{}
		std::string mName;
		std::string mDomain;
		CefRefPtr<CefSchemeHandlerFactory> mFactory;
	};
	static std::string sSubprocess;
	static bool sSingleProcess;
	static CefMainArgs sArgs;
	static CefRefPtr<HTMLUICore> sInstance;
	static void WebThread();
	static sf::Thread* spThread;
	static bool sEndThread;
	static std::queue<RegScheme> sRegSchemeQueue;
	static std::queue<HTMLUIView*> sViewQueue;
	static std::map<int, HTMLUIView*> sViews;
	typedef std::map<std::pair<std::string, int>, JSBinding::JSCallback> BindingMap;
	static BindingMap sBindings;
	void AddBrowserToInterface(HTMLUIView* pWeb);

	HTMLUICore() {}
	~HTMLUICore() {}

private:
	IMPLEMENT_REFCOUNTING(HTMLUICore);
};

class HTMLUIApp : public CefApp
{
public:
	HTMLUIApp() {}
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return HTMLUICore::GetInstance().get(); }
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return HTMLUICore::GetInstance().get(); }
	IMPLEMENT_REFCOUNTING(HTMLUIApp);
};

class HTMLUIView : public CefBaseRefCounted
{
	friend class HTMLUICore;
public:

	void SendFocusEvent(bool setFocus);
	void SendMouseClickEvent(int x, int y, sf::Mouse::Button button, bool mouseUp, int clickCount);
	void SendMouseMoveEvent(int x, int y, bool mouseLeave = false);
	void SendMouseWheelEvent(int x, int y, int deltaX, int deltaY);
	void SendKeyEvent(WPARAM key, bool keyUp, bool isSystem = false, int modifiers = -1);
	void SendKeyEvent(char key, int modifiers = -1);
	void AddJSBinding(const std::string name, JSBinding::JSCallback callback);
	void AddJSBindings(const std::vector<JSBinding> bindings);
	void ExecuteJS(const CefString& code);
	void ExecuteJS(const CefString& code, CefRefPtr<CefFrame> frame);
	void ExecuteJS(const CefString& code, CefRefPtr<CefFrame> frame, int startLine);
	bool JSCallback(const CefString& name,
		CefRefPtr<CefListValue> arguments
	);
	HTMLUIView(int width, int height, const std::string& url, bool transparent, HTMLUI *ui);
	HTMLUI *htmlUI;
	virtual ~HTMLUIView();
	void Close(bool force = true) { sf::Lock lock(mMutex); mBrowser->GetHost()->CloseBrowser(force); }
	CefRefPtr<CefBrowser> GetBrowser() { return mBrowser; }
	int GetWidth() { return mTextureWidth; }
	int GetHeight() { return mTextureHeight; }

	std::vector<void(*)(HTMLUIView*)> onReadyCallbacks;

	std::shared_mutex emitMtx;
	std::map<std::string, json> pendingEmit;
	void Emit(std::string method, json object);
	void SendEmitters();

	void UpdateTexture();
	sf::Texture* GetTexture() { return mpTexture; }
	std::string GetCurrentURL() { return mCurrentURL; }
	bool IsTransparent() { return mTransparent; }
	void SetSize(int width, int height);
	void SetContext(CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Value> object, CefRefPtr<CefV8Handler> handler);
	bool hasContext = false;
	CefRefPtr<CefV8Context> jsContext;
	CefRefPtr<CefV8Value> jsObject;
	CefRefPtr<CefV8Handler> jsHandler;
	CefRefPtr<CefBrowser> mBrowser;
    sf::Mutex mMutex;
private:
	static int GetMouseModifiers();
	static int GetKeyboardModifiers();

	struct UpdateRect
	{
	public:
		char* buffer;
		CefRect rect;
	};
	void ClearBrowser() { mBrowser = NULL; }
	sf::WindowHandle mHandle;
	sf::Texture* mpTexture;
	int mTextureWidth;
	int mTextureHeight;
	std::queue<UpdateRect> mUpdateRects;
	std::string mCurrentURL;
	bool mTransparent;

public:
	IMPLEMENT_REFCOUNTING(HTMLUIView);
};

class HTMLUI
{
public:
	HTMLUICore *core;
	std::map<std::string, HTMLUIPanel*> panels;
	sf::WindowHandle window;

	CGame * game;

	HTMLUI(class CGame *pGame, sf::WindowHandle handle);
	~HTMLUI();

	bool isInitialized = false;
	HTMLUIPanel* createPanel(std::string name, std::string url, int x, int y, int width, int height, void(*cb)(HTMLUIView*));
	void Work();
};

class HTMLUIMethodHandler : public CefV8Handler {
public:
	HTMLUI *mHTMLUI;
	HTMLUIMethodHandler(HTMLUI * htmlUI);
	virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override;

	IMPLEMENT_REFCOUNTING(HTMLUIMethodHandler);
};

class HTMLUIPanel
{
public:
	HTMLUIView *mView;
	sf::Sprite mSprite;
	std::string mUrl;
	int mWidth;
	int mHeight;
	int mX;
	int mY;
	bool hasFocus;
	bool mouseDown;

	HTMLUIPanel::HTMLUIPanel(HTMLUIView *view, std::string url, int x, int y, int width, int height);
	void paint();
};


class HTMLUISpriteHandler : public CefResourceHandler
{
public:
	HTMLUISpriteHandler();

	bool ReadFromFile(std::string filePath);

	virtual bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) override;

	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) override;

	virtual void Cancel() override;

	virtual bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) override;

	~HTMLUISpriteHandler();

	virtual bool HasAtLeastOneRef() const override
	{
		return 1;
	}

/*
	virtual bool CanGetCookie(const CefCookie& cookie) override;


	virtual bool CanSetCookie(const CefCookie& cookie) override;*/


	virtual void AddRef() const override;


	virtual bool Release() const override;


	virtual bool HasOneRef() const override;

private:
	std::string mMimeType;
	size_t mOffset;
	int fileSize;
	char *fileBuffer;
};


class HTMLUISpriteHandlerFactory : public CefSchemeHandlerFactory
{
public:
	virtual CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name, CefRefPtr<CefRequest> request) override
	{
		return new HTMLUISpriteHandler();
	}

	IMPLEMENT_REFCOUNTING(HTMLUISpriteHandlerFactory);
};
