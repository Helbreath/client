#include "HTMLUI.h"
#include "Game.h"

extern CGame * G_pGame;

// Static variables
std::string HTMLUICore::sSubprocess = "hbx_ui.exe";
bool HTMLUICore::sSingleProcess = true;
CefMainArgs HTMLUICore::sArgs;
CefRefPtr<HTMLUIApp> HTMLUICore::sApp = NULL;
CefRefPtr<HTMLUICore> HTMLUICore::sInstance = NULL;
sf::Thread* HTMLUICore::spThread = NULL;
bool HTMLUICore::sEndThread = false;
std::queue<HTMLUICore::RegScheme> HTMLUICore::sRegSchemeQueue;
std::queue<HTMLUIView*> HTMLUICore::sViewQueue;
std::map<int, HTMLUIView*> HTMLUICore::sViews;
HTMLUICore::BindingMap HTMLUICore::sBindings;

void SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target);
void SetList(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target);

// Transfer a V8 value to a List index.
void SetListValue(CefRefPtr<CefListValue> list, int index,
	CefRefPtr<CefV8Value> value) {
	if (value->IsArray()) {
		CefRefPtr<CefListValue> new_list = CefListValue::Create();
		SetList(value, new_list);
		list->SetList(index, new_list);
	}
	else if (value->IsString()) {
		list->SetString(index, value->GetStringValue());
	}
	else if (value->IsBool()) {
		list->SetBool(index, value->GetBoolValue());
	}
	else if (value->IsInt()) {
		list->SetInt(index, value->GetIntValue());
	}
	else if (value->IsDouble()) {
		list->SetDouble(index, value->GetDoubleValue());
	}
}

// Transfer a V8 array to a List.
void SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target) {
	int arg_length = source->GetArrayLength();
	if (arg_length == 0)
		return;

	target->SetSize(arg_length);

	for (int i = 0; i < arg_length; ++i)
		SetListValue(target, i, source->GetValue(i));
}

void SetListValue(CefRefPtr<CefV8Value> list, int index,
	CefRefPtr<CefListValue> value) {
	CefRefPtr<CefV8Value> new_value;

	CefValueType type = value->GetType(index);
	switch (type) {
	case VTYPE_LIST: {
		CefRefPtr<CefListValue> list = value->GetList(index);
		new_value = CefV8Value::CreateArray(static_cast<int>(list->GetSize()));
		SetList(list, new_value);
	} break;
	case VTYPE_BOOL:
		new_value = CefV8Value::CreateBool(value->GetBool(index));
		break;
	case VTYPE_DOUBLE:
		new_value = CefV8Value::CreateDouble(value->GetDouble(index));
		break;
	case VTYPE_INT:
		new_value = CefV8Value::CreateInt(value->GetInt(index));
		break;
	case VTYPE_STRING:
		new_value = CefV8Value::CreateString(value->GetString(index));
		break;
	default:
		break;
	}

	if (new_value.get()) {
		list->SetValue(index, new_value);
	}
	else {
		list->SetValue(index, CefV8Value::CreateNull());
	}
}

// Transfer a List to a V8 array.
void SetList(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target) {
	int arg_length = static_cast<int>(source->GetSize());
	if (arg_length == 0)
		return;

	for (int i = 0; i < arg_length; ++i)
		SetListValue(target, i, source);
}


CefRefPtr<HTMLUICore> HTMLUICore::GetInstance()
{
	if (!sInstance)
	{
		sInstance = new HTMLUICore();
	}

	return sInstance;
}

extern std::condition_variable cv;
extern std::condition_variable cv2;

void HTMLUICore::WebThread()
{
	if (sApp == nullptr)
	{
		Main();
	}

	CefSettings settings;
	settings.multi_threaded_message_loop = false;
	// settings.single_process = sSingleProcess;
	settings.windowless_rendering_enabled = true;
	settings.external_message_pump = true;
	settings.remote_debugging_port = 9222;

	/*if (sSubprocess != "")
	{
	std::ifstream ifile(sSubprocess.c_str());
	if (ifile)
	CefString(&settings.browser_subprocess_path).FromASCII(sSubprocess.c_str());
	}*/

	CefInitialize(sArgs, settings, sApp.get(), NULL);

	while (!sEndThread)
	{
		CefDoMessageLoopWork();

		while (sRegSchemeQueue.size() > 0)
		{
			CefRegisterSchemeHandlerFactory(sRegSchemeQueue.front().mName, sRegSchemeQueue.front().mDomain, sRegSchemeQueue.front().mFactory);
			sRegSchemeQueue.pop();
		}

		while (sViewQueue.size() > 0)
		{
			GetInstance()->AddBrowserToInterface(sViewQueue.front());
			sViewQueue.pop();
		}

/*
        for (std::pair<std::string, HTMLUIPanel *> entry : G_pGame->htmlUI->panels) {
            auto * panel = entry.second;
            if (panel != nullptr && !panel->mView->GetBrowser()->IsLoading()) {
                panel->paint();
            }
        }*/
	}
/*
	auto m = sViews;
	for (auto view : m)
	{
		//view.second->Close();
		//view.second->GetBrowser()->Release();
		//view.second->mBrowser
		delete view.second;
	}*/


	for (auto panel : G_pGame->htmlUI->panels)
	{
        //delete panel.second->mView;
		//delete panel.second;
		//G_pGame->htmlUI->core->DoClose(panel.second->mView->mBrowser);
		G_pGame->htmlUI->core->DoClose(panel.second->mView->GetBrowser());
/*
		panel.second->mView->ClearBrowser();
        delete panel.second->mView;
        panel.second->mView = nullptr;
		delete panel.second;*/
    }

	//G_pGame->htmlUI->panels.clear();
	//G_pGame->htmlUI->core->
	//G_pGame->htmlUI->panels.clear();
	//delete G_pGame->htmlUI->core;
	//delete G_pGame->htmlUI;

	//G_pGame->htmlUI->core->DoClose();
	//G_pGame->htmlUI->core->sViews[0]->

	CefShutdown();
}

/*
auto const hud_file = locate_media("resource/hud.html");
if (hud_file) {
    // (we need to convert to file:/// url for CEF
    auto const hud_url = to_file_url(*hud_file);*/

void HTMLUICore::AddBrowserToInterface(HTMLUIView* view)
{
	CefWindowInfo info;

	info.SetAsWindowless(view->mHandle);
	CefBrowserSettings browserSettings;
	browserSettings.javascript_access_clipboard = STATE_ENABLED;


	CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(info, this, view->GetCurrentURL(), browserSettings, NULL, NULL);

	view->mBrowser = browser;

	sViews[browser->GetIdentifier()] = view;
}


int HTMLUICore::Main()
{
#if(PLATFORM == WINDOWS)
	sArgs = CefMainArgs(GetModuleHandle(NULL));
#else
	sArgs = CefMainArgs();
#endif
	sApp = new HTMLUIApp();

	int exit_code = CefExecuteProcess(sArgs, sApp.get(), NULL);

	if (exit_code >= 0)
	{
		return exit_code;
	}

	return -1;
}


void HTMLUICore::StartWeb()
{
	if (!spThread)
	{
		spThread = new sf::Thread(&WebThread);

		sEndThread = false;
		spThread->launch();
	}
}

void HTMLUICore::EndWeb()
{
	sEndThread = true;
}

void HTMLUICore::WaitForWebEnd()
{
	if (spThread)
	{
		spThread->wait();

		delete spThread;
		spThread = NULL;
	}
}

void HTMLUICore::RegisterScheme(std::string name, std::string domain, CefRefPtr<CefSchemeHandlerFactory> factory)
{
	sRegSchemeQueue.push(RegScheme(name, domain, factory));
}

HTMLUIView* HTMLUICore::CreateView(int width, int height, const std::string& url, bool transparent, HTMLUI *ui, void(*cb)(HTMLUIView*))
{
    HTMLUIView* view = new HTMLUIView(width, height, url, transparent, ui);

	view->onReadyCallbacks.push_back(cb);
	//swapped
    //HTMLUICore::GetInstance()->AddBrowserToInterface(view);
	sViewQueue.push(view);

    return view;
}


void HTMLUICore::UpdateInterfaceTextures()
{
	std::map<int, HTMLUIView*>::iterator i;
	for (i = sViews.begin(); i != sViews.end(); i++)
	{
		i->second->UpdateTexture();
	}
}


void HTMLUICore::OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info)
{
	BindingMap::iterator i = sBindings.begin();
	for (; i != sBindings.end(); i++)
	{
		if (i->first.second == browser->GetIdentifier())
		{
			CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("jsbinding_create");
			message->GetArgumentList()->SetString(0, i->first.first);
			message->GetArgumentList()->SetInt(1, i->first.second);

			browser->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);
		}
	}
}


void HTMLUICore::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{
	BindingMap::iterator i = sBindings.begin();
	for (; i != sBindings.end();)
	{
		if (i->first.second == browser->GetIdentifier())
			sBindings.erase(i++);
		else
			++i;
	}
}


void HTMLUICore::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	// Ask the interface if it wants bindings
	if (!sViews.count(browser->GetIdentifier()))
	{
		return;
	}

	HTMLUIView* view = sViews[browser->GetIdentifier()];

	// Create a new callback handler.
	CefRefPtr<CefV8Handler> handler = new HTMLUIMethodHandler(view->htmlUI);

	// Retrieve the context's window object.
	CefRefPtr<CefV8Value> object = context->GetGlobal();

	HTMLUICore::BindingMap::iterator i = sBindings.begin();
	for (; i != sBindings.end(); i++)
	{
		if (i->first.second == browser->GetIdentifier())
		{
			CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(i->first.first, handler);

			object->SetValue(i->first.first, func, V8_PROPERTY_ATTRIBUTE_NONE);
		}
	}

	view->SetContext(context, object, handler);
}


void HTMLUICore::OnContextReleased(CefRefPtr< CefBrowser > browser, CefRefPtr< CefFrame > frame, CefRefPtr< CefV8Context > context)
{
	printf("Context released\n");
}


void HTMLUICore::OnUncaughtException(CefRefPtr< CefBrowser > browser, CefRefPtr< CefFrame > frame, CefRefPtr< CefV8Context > context, CefRefPtr< CefV8Exception > exception, CefRefPtr< CefV8StackTrace > stackTrace)
{

}


/*
void HTMLUICore::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
{

}*/


bool HTMLUICore::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	CefString message_name = message->GetName();

	if (CefCurrentlyOn(TID_RENDERER))
	{
		if (message_name == "jsbinding_create")
		{
			std::string name = message->GetArgumentList()->GetString(0);

			sBindings.insert(std::make_pair(
				std::make_pair(name, message->GetArgumentList()->GetInt(1)),
				(JSBinding::JSCallback)NULL));

			browser->Reload();

			return true;
		}
	}
	else
	{
		if (message_name == "jsbinding_call")
		{
			if (sViews.count(browser->GetIdentifier()))
			{
				HTMLUIView* view = sViews[browser->GetIdentifier()];

				std::string name;
				CefRefPtr<CefListValue> arguments = CefListValue::Create();
				CefRefPtr<CefV8Value> retval;
				CefString exception;

				CefRefPtr<CefListValue> margs = message->GetArgumentList();
				name = margs->GetString(0);
				int argCount = margs->GetInt(1);
				std::string test = margs->GetString(2);
				for (int i = 0; i < argCount; i++)
				{
					CefValueType type = margs->GetType(i + 2);
					switch (type) {
					case VTYPE_LIST: {
						CefRefPtr<CefListValue> list = margs->GetList(i + 2);
						arguments->SetList(i, list);
					} break;
					case VTYPE_BOOL: {
						bool b = margs->GetBool(i + 2);
						arguments->SetBool(i, b);
					} break;
					case VTYPE_DOUBLE: {
						double d = margs->GetDouble(i + 2);
						arguments->SetDouble(i, d);
					} break;
					case VTYPE_INT: {
						int integer = margs->GetInt(i + 2);
						arguments->SetInt(i, integer);
					} break;
					case VTYPE_STRING: {
						std::string s = margs->GetString(i + 2);
						arguments->SetString(i, s);
					} break;
					default:
						break;
					}
				}

				view->JSCallback(name, arguments);

				return true;
			}
		}
	}

	return false;
}


void HTMLUICore::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{

}


void HTMLUICore::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{

}


void HTMLUICore::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	std::cout << "OnAfterCreated\n";
}


void HTMLUICore::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	//Free the browser pointer so that the browser can be destroyed.
	if (sViews.count(browser->GetIdentifier()))
		sViews[browser->GetIdentifier()]->ClearBrowser();
}


void HTMLUICore::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	if (!sViews.count(browser->GetIdentifier()))
	{
		rect = CefRect(0, 0, 1024, 768);
		return;
	}
	HTMLUIView* view = sViews[browser->GetIdentifier()];
	if (!view)
	{
		rect = CefRect(0, 0, 1024, 768);
		return;
	}

	rect = CefRect(0, 0, view->GetWidth(), view->GetHeight());
	return;
}


void HTMLUICore::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
{
	////Get the web interface we will be working with. 
	if (!sViews.count(browser->GetIdentifier())) return;
	HTMLUIView* view = sViews[browser->GetIdentifier()];
	if (!view)
		return;

	if (!buffer)
		return;

	if (type == PET_VIEW)
	{
		int old_width = view->mTextureWidth;
		int old_height = view->mTextureHeight;

		//Retrieve current size of browser view.
		view->mTextureWidth = width;
		view->mTextureHeight = height;

		//Check if we need to resize the texture before drawing to it.
		if (old_width != view->mTextureWidth || old_height != view->mTextureHeight)
		{
			// Does nothing
		}
		else
		{
			//We want to work on the buffer byte by byte so get a pointer with a new type.
			char* bitmap = (char*)(buffer);

			sf::Lock lock(view->mMutex);
			//Update the dirty rectangles.
			CefRenderHandler::RectList::const_iterator i = dirtyRects.begin();
			for (; i != dirtyRects.end(); ++i)
			{
				const CefRect& rect = *i;
				//Create a rect sized buffer for the new rectangle data.
				char* rectBuffer = new char[rect.width * (rect.height + 1) * 4];

				for (int jj = 0; jj < rect.height; jj++)
				{
					//Copy the new rectangle data out of the full size buffer into our rect sized one.  
					memcpy(
						rectBuffer + jj * rect.width * 4,
						bitmap + ((rect.x + ((rect.y + jj) * view->mTextureWidth)) * 4),
						rect.width * 4
					);
				}

				//Convert BGRA to RGBA
				unsigned int* pTmpBuf = (unsigned int*)rectBuffer;
				const int numPixels = rect.width * rect.height;
				for (int i = 0; i < numPixels; i++)
				{
					pTmpBuf[i] = (pTmpBuf[i] & 0xFF00FF00) | ((pTmpBuf[i] & 0x00FF0000) >> 16) | ((pTmpBuf[i] & 0x000000FF) << 16);
				}

				if (!rectBuffer)
					continue;
				//Update the texture with the new data.  
				//This can be interrupted if the main thread calls a draw on a sprite which uses this texture
				// as the texture is bound by openGL calls.  
				//To rectify this we have the redundancy updating system.  
				view->mpTexture->update((sf::Uint8*)rectBuffer, rect.width, rect.height, rect.x, rect.y);

				//Here we need to add the data required for the update to the queue for redundancy updates.  
				view->mUpdateRects.push(HTMLUIView::UpdateRect());
				view->mUpdateRects.back().buffer = rectBuffer;
				view->mUpdateRects.back().rect = rect;
			}
		}
	}
}


bool HTMLUICore::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& e, CefEventHandle os_event)
{
	if (e.is_system_key && e.modifiers == 4)
	{
		if (e.character == 'V')
			browser->GetFocusedFrame()->Paste();
		else if (e.character == 'X')
			browser->GetFocusedFrame()->Cut();
		else if (e.character == 'C')
			browser->GetFocusedFrame()->Copy();
		else if (e.character == 'Z')
			browser->GetFocusedFrame()->Undo();
		else if (e.character == 'Y')
			browser->GetFocusedFrame()->Redo();
		else if (e.character == 'A')
			browser->GetFocusedFrame()->SelectAll();
	}

	return true;
}

void HTMLUICore::CefV8Array2ListValue(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target)
{
	assert(source->IsArray());

	int arg_length = source->GetArrayLength();
	if (arg_length == 0)
		return;

	// Start with null types in all spaces.
	target->SetSize(arg_length);

	for (int i = 0; i < arg_length; ++i)
	{
		CefRefPtr<CefV8Value> value = source->GetValue(i);
		if (value->IsBool())
		{
			target->SetBool(i, value->GetBoolValue());
		}
		else if (value->IsInt() || value->IsUInt())
		{
			target->SetInt(i, value->GetIntValue());
		}
		else if (value->IsDouble())
		{
			target->SetDouble(i, value->GetDoubleValue());
		}
		else if (value->IsNull())
		{
			target->SetNull(i);
		}
		else if (value->IsString() || value->IsDate())
		{
			target->SetString(i, value->GetStringValue());
		}
		else if (value->IsArray())
		{
			CefRefPtr<CefListValue> new_list = CefListValue::Create();
			CefV8Array2ListValue(value, new_list);
			target->SetList(i, new_list);
		}
		else if (value->IsObject())
		{
			CefRefPtr<CefDictionaryValue> new_dictionary = CefDictionaryValue::Create();
			CefV8JsonObject2DictionaryValue(value, new_dictionary);
			target->SetDictionary(i, new_dictionary);
		}
	}
}

void HTMLUICore::CefListValue2V8Array(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target) {
	assert(target->IsArray());

	int arg_length = static_cast<int>(source->GetSize());
	if (arg_length == 0)
		return;

	for (int i = 0; i < arg_length; ++i)
	{
		CefRefPtr<CefV8Value> new_value;

		CefValueType type = source->GetType(i);
		switch (type)
		{
		case VTYPE_BOOL:
			new_value = CefV8Value::CreateBool(source->GetBool(i));
			break;
		case VTYPE_DOUBLE:
			new_value = CefV8Value::CreateDouble(source->GetDouble(i));
			break;
		case VTYPE_INT:
			new_value = CefV8Value::CreateInt(source->GetInt(i));
			break;
		case VTYPE_STRING:
			new_value = CefV8Value::CreateString(source->GetString(i));
			break;
		case VTYPE_NULL:
			new_value = CefV8Value::CreateNull();
			break;
		case VTYPE_LIST:
		{
			CefRefPtr<CefListValue> list = source->GetList(i);
			new_value = CefV8Value::CreateArray(static_cast<int>(list->GetSize()));
			CefListValue2V8Array(list, new_value);
		}
		break;
		case VTYPE_DICTIONARY:
		{
			CefRefPtr<CefDictionaryValue> dictionary = source->GetDictionary(i);
			new_value = CefV8Value::CreateObject(NULL, NULL);
			CefDictionaryValue2V8JsonObject(dictionary, new_value);
		}
		break;
		default:
			break;
		}

		if (new_value.get())
		{
			target->SetValue(i, new_value);
		}
		else
		{
			target->SetValue(i, CefV8Value::CreateNull());
		}
	}
}

void HTMLUICore::CefV8JsonObject2DictionaryValue(CefRefPtr<CefV8Value>source, CefRefPtr<CefDictionaryValue> target)
{
	assert(source->IsObject());

	std::vector<CefString> keys;
	source->GetKeys(keys);
	std::vector<CefString>::const_iterator beg = keys.begin();
	std::vector<CefString>::const_iterator end = keys.end();
	for (std::vector<CefString>::const_iterator it = beg; it != end; ++it)
	{
		const CefString key = *it;
		CefRefPtr<CefV8Value> value = source->GetValue(key);

		if (value->IsBool())
		{
			target->SetBool(key, value->GetBoolValue());
		}
		else if (value->IsDouble())
		{
			target->SetDouble(key, value->GetDoubleValue());
		}
		else if (value->IsInt() || value->IsUInt())
		{
			target->SetInt(key, value->GetIntValue());
		}
		else if (value->IsNull())
		{
			target->SetNull(key);
		}
		else if (value->IsString() || value->IsDate())
		{
			target->SetString(key, value->GetStringValue());
		}
		else if (value->IsArray())
		{
			CefRefPtr<CefListValue> listValue = CefListValue::Create();
			CefV8Array2ListValue(value, listValue);
			target->SetList(key, listValue);
		}
		else if (value->IsObject())
		{
			CefRefPtr<CefDictionaryValue> dictionaryValue = CefDictionaryValue::Create();
			CefV8JsonObject2DictionaryValue(value, dictionaryValue);
			target->SetDictionary(key, dictionaryValue);
		}
	}
}

void HTMLUICore::CefDictionaryValue2V8JsonObject(CefRefPtr<CefDictionaryValue> source, CefRefPtr<CefV8Value> target)
{
	assert(target->IsObject());

	CefDictionaryValue::KeyList keys;
	source->GetKeys(keys);
	CefDictionaryValue::KeyList::const_iterator beg = keys.begin();
	CefDictionaryValue::KeyList::const_iterator end = keys.end();

	for (CefDictionaryValue::KeyList::const_iterator it = beg; it != end; ++it)
	{
		CefRefPtr<CefV8Value> new_value;
		CefString key = *it;
		CefValueType type = source->GetType(key);

		switch (type)
		{
		case VTYPE_BOOL:
			new_value = CefV8Value::CreateBool(source->GetBool(key));
			break;
		case VTYPE_DOUBLE:
			new_value = CefV8Value::CreateDouble(source->GetDouble(key));
			break;
		case VTYPE_INT:
			new_value = CefV8Value::CreateInt(source->GetInt(key));
			break;
		case VTYPE_STRING:
			new_value = CefV8Value::CreateString(source->GetString(key));
			break;
		case VTYPE_NULL:
			new_value = CefV8Value::CreateNull();
			break;
		case VTYPE_LIST:
		{
			CefRefPtr<CefListValue> list = source->GetList(key);
			new_value = CefV8Value::CreateArray(static_cast<int>(list->GetSize()));
			CefListValue2V8Array(list, new_value);
		}
		break;
		case VTYPE_DICTIONARY:
		{
			CefRefPtr<CefDictionaryValue> dictionary = source->GetDictionary(key);
			new_value = CefV8Value::CreateObject(NULL, NULL);
			CefDictionaryValue2V8JsonObject(dictionary, new_value);
		}
		break;
		default:
			break;
		}
		if (new_value.get())
		{
			target->SetValue(key, new_value, V8_PROPERTY_ATTRIBUTE_NONE);
		}
		else
		{
			target->SetValue(key, CefV8Value::CreateNull(), V8_PROPERTY_ATTRIBUTE_NONE);
		}
	}
}

HTMLUIView::HTMLUIView(int width, int height, const std::string& url, bool transparent, HTMLUI *mHTMLui)
	: mTextureWidth(width)
	, mTextureHeight(height)
	, mCurrentURL(url)
	, mTransparent(transparent)
	, jsContext(NULL)
	, jsObject(NULL)
	, mBrowser(NULL)
	, mpTexture(NULL)
{
	htmlUI = mHTMLui;
	mpTexture = new sf::Texture();
	mpTexture->create(mTextureWidth, mTextureHeight);

}

void HTMLUIView::SetContext(CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Value> object, CefRefPtr<CefV8Handler> handler)
{
	jsContext = context;
	jsObject = object;
	jsHandler = handler;

	// Process onReady callbacks
	for (int i = 0; i < onReadyCallbacks.size(); i++) {
		auto cb = onReadyCallbacks.at(i);
		cb(this);
	}

	this->hasContext = true;
}


HTMLUIView::~HTMLUIView()
{

	delete mpTexture;

	while (mUpdateRects.size() > 0)
	{
		delete[] mUpdateRects.front().buffer;
		mUpdateRects.pop();
	}

	std::map<int, HTMLUIView*>::iterator i;
	for (i = HTMLUICore::sViews.begin(); i != HTMLUICore::sViews.end(); i++)
	{
		if (i->second == this)
		{
			HTMLUICore::sViews.erase(i);
			break;
		}
	}

	if (mBrowser)
	{
		mBrowser->GetHost()->CloseBrowser(true);
		mBrowser = NULL;
	}
}


void HTMLUIView::Emit(std::string method, json object)
{
	// emitMtx.lock();
	pendingEmit[method] = object;
	// emitMtx.unlock();
}

void HTMLUIView::SendEmitters()
{
	// emitMtx.lock_shared();
	CefRefPtr<CefV8Value> ui = jsObject->GetValue("UI");
	for(auto i = pendingEmit.cbegin(); i != pendingEmit.cend();)
	{
		CefV8ValueList args;
		args.push_back(CefV8Value::CreateString(i->first));
		args.push_back(JSONtoCef(i->second));

		if (ui->HasValue("emit")) {
			auto fnc = ui->GetValue("emit");
			if (fnc->IsFunction()) {
				if (!fnc->ExecuteFunctionWithContext(jsContext, NULL, args)) {
					printf("Unable to execute function: %s", i->first.c_str());
				}
				// Remove the element
				// emitMtx.lock();
				i = pendingEmit.erase(i);
				// emitMtx.unlock();
			}
			else {
				++i;
			}
		}
		else {
			++i;
		}
	}
	// emitMtx.unlock_shared();
}


void HTMLUIView::UpdateTexture()
{
	sf::Lock lock(mMutex);
	while (mUpdateRects.size() > 0)
	{
		int size = mUpdateRects.size();
		const CefRect& rect = mUpdateRects.front().rect;
		mpTexture->update((sf::Uint8*)mUpdateRects.front().buffer, rect.width, rect.height, rect.x, rect.y);
		delete[] mUpdateRects.front().buffer;
		mUpdateRects.pop();
	}
}


void HTMLUIView::SendFocusEvent(bool setFocus)
{
	mBrowser->GetHost()->SendFocusEvent(setFocus);
}


int HTMLUIView::GetMouseModifiers()
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


int HTMLUIView::GetKeyboardModifiers()
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


void HTMLUIView::SendMouseClickEvent(int x, int y, sf::Mouse::Button button, bool mouseUp, int clickCount)
{
	if (!mBrowser)
		return;

	CefBrowserHost::MouseButtonType type = button == sf::Mouse::Left ? MBT_LEFT :
		button == sf::Mouse::Right ? MBT_RIGHT : MBT_MIDDLE;

	CefMouseEvent e; e.x = x; e.y = y; e.modifiers = GetMouseModifiers();
	mBrowser->GetHost()->SendMouseMoveEvent(e, false);
	mBrowser->GetHost()->SendMouseClickEvent(e, type, mouseUp, clickCount);
}


void HTMLUIView::SendMouseMoveEvent(int x, int y, bool mouseLeave)
{
	if (!mBrowser)
		return;

	CefMouseEvent e; e.x = x; e.y = y; e.modifiers = GetMouseModifiers();
	mBrowser->GetHost()->SendMouseMoveEvent(e, mouseLeave);
}


void HTMLUIView::SendMouseWheelEvent(int x, int y, int deltaX, int deltaY)
{
	if (!mBrowser)
		return;

	CefMouseEvent e; e.x = x; e.y = y; e.modifiers = GetMouseModifiers();
	mBrowser->GetHost()->SendMouseWheelEvent(e, deltaX, deltaY);
}


void HTMLUIView::SendKeyEvent(WPARAM key, bool keyUp, bool isSystem, int modifiers)
{
	if (!mBrowser)
		return;
	CefKeyEvent e; e.windows_key_code = key; e.modifiers = modifiers == -1 ? GetKeyboardModifiers() : modifiers;
	e.type = keyUp ? KEYEVENT_KEYUP : KEYEVENT_KEYDOWN;
	e.is_system_key = isSystem; e.character = key; e.unmodified_character = key; //e.native_key_code = 0;
	mBrowser->GetHost()->SendKeyEvent(e);
}


void HTMLUIView::SendKeyEvent(char key, int modifiers)
{
	if (!mBrowser)
		return;
	CefKeyEvent e; e.windows_key_code = key; e.modifiers = modifiers == -1 ? GetKeyboardModifiers() : modifiers;
	e.type = KEYEVENT_CHAR; e.character = key; e.unmodified_character = key;
	mBrowser->GetHost()->SendKeyEvent(e);
}


void HTMLUIView::AddJSBinding(const std::string name, JSBinding::JSCallback callback)
{
	HTMLUICore::sBindings.insert(std::make_pair(std::make_pair(name, mBrowser->GetIdentifier()), callback));

	CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("jsbinding_create");
	message->GetArgumentList()->SetString(0, name.c_str());
	message->GetArgumentList()->SetInt(1, mBrowser->GetIdentifier());

	mBrowser->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);

	//We have to reload the page otherwise the bindings wont be added to the context.
	mBrowser->Reload();
}


void HTMLUIView::AddJSBindings(const std::vector<JSBinding> bindings)
{
	for (unsigned int i = 0; i < bindings.size(); i++)
	{
		HTMLUICore::sBindings.insert(std::make_pair(std::make_pair(bindings[i].mFunctionName, mBrowser->GetIdentifier()), bindings[i].mfpJSCallback));

		CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("jsbinding_create");
		message->GetArgumentList()->SetString(0, bindings[i].mFunctionName);
		message->GetArgumentList()->SetInt(1, mBrowser->GetIdentifier());

		mBrowser->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);
	}

	//We have to reload the page otherwise the bindings wont be added to the context.
	mBrowser->Reload();
}


void HTMLUIView::ExecuteJS(const CefString& code)
{
	if (!mBrowser)
		return;

	ExecuteJS(code, mBrowser->GetMainFrame());
}


void HTMLUIView::ExecuteJS(const CefString& code, CefRefPtr<CefFrame> frame)
{
	if (!mBrowser)
		return;

	//Should probably check to make sure the frame is from our browser here.

	ExecuteJS(code, frame, 0);
}


void HTMLUIView::ExecuteJS(const CefString& code, CefRefPtr<CefFrame> frame, int startLine)
{
	if (!mBrowser)
		return;

	//Should probably check to make sure the frame is from our browser here.

	frame->ExecuteJavaScript(code, frame->GetURL(), startLine);
}


bool HTMLUIView::JSCallback(const CefString& name,
	CefRefPtr<CefListValue> arguments
)
{
	bool result = false;
	//Check if this is one of our bindings.

	if (HTMLUICore::sBindings.count(std::make_pair(name, mBrowser->GetIdentifier())))
	{
		result = HTMLUICore::sBindings[std::make_pair(name, mBrowser->GetIdentifier())](arguments);
	}

	//Otherwise fallthrough and return false.
	return result;
}


void HTMLUIView::SetSize(int width, int height)
{
	mTextureWidth = width;
	mTextureHeight = height;

	if (mBrowser)
		mBrowser->GetHost()->WasResized();

	sf::Texture* pOldTexture = mpTexture;
	mpTexture = new sf::Texture();
	mpTexture->create(mTextureWidth, mTextureHeight);
	mpTexture->setSmooth(true);
	if (pOldTexture)
		delete pOldTexture;

	sf::Lock lock(mMutex);

	//Clear the update rects
	while (mUpdateRects.size() > 0)
	{
		delete[] mUpdateRects.front().buffer;
		mUpdateRects.pop();
	}

	if (mBrowser)
		mBrowser->GetHost()->Invalidate(PET_VIEW);
}

bool WebV8Handler::Execute(const CefString& name,
	CefRefPtr<CefV8Value> object,
	const CefV8ValueList& arguments,
	CefRefPtr<CefV8Value>& retval,
	CefString& exception)
{
	//Send message to browser process to call function
	CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
	CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("jsbinding_call");
	message->GetArgumentList()->SetString(0, name);
	message->GetArgumentList()->SetInt(1, arguments.size());
	for (unsigned int i = 0; i < arguments.size(); i++)
	{
		SetListValue(message->GetArgumentList(), i + 2, arguments[i]);
	}

	browser->GetMainFrame()->SendProcessMessage(PID_BROWSER, message);

	return false;
}
