#include "Game.h"
#include "HTMLUI.h"

extern CGame * G_pGame;

void HTMLUICore::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
    CEF_REQUIRE_UI_THREAD();

    model->Clear();
}

bool HTMLUICore::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags)
{
    CEF_REQUIRE_UI_THREAD();

    MessageBox(browser->GetHost()->GetWindowHandle(), L"The requested action is not supported", L"Unsupported Action", MB_OK | MB_ICONINFORMATION);
    return false;
}
#include <filesystem>
std::string getcwd_string(void) {
	char buff[1024];
	
	getcwd(buff, 1024);
	std::string cwd(buff);
	return cwd;
}

CefRefPtr<CefV8Value> JSONtoCef(json o)
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
			arr->SetValue(i, JSONtoCef(entry));
		}
		return arr;
	}
	else if (o.is_object()) {
		CefRefPtr<CefV8Value> obj = CefV8Value::CreateObject(NULL, NULL);
		for (json::iterator oi = o.begin(); oi != o.end(); ++oi) {
			obj->SetValue(oi.key(), JSONtoCef(oi.value()), V8_PROPERTY_ATTRIBUTE_NONE);
		}
		return obj;
	}
	else if (o.is_string()) {
		std::string s = o;
		return CefV8Value::CreateString(s);
	}

	return CefV8Value::CreateNull();
}

HTMLUI::HTMLUI(class CGame * pGame, sf::WindowHandle handle)
{
	core = HTMLUICore::GetInstance();

	std::map<std::string, HTMLUIPanel *> nPanels;
	panels = nPanels;

	game = pGame;

	window = handle;
}

HTMLUIPanel* HTMLUI::createPanel(std::string name, std::string url, int x, int y, int width, int height, void(*cb)(HTMLUIView*))
{
	if (this->panels.find(name) != this->panels.end()) {
		return this->panels.at(name);
	}

	HTMLUIView *view = HTMLUICore::CreateView(width, height, url, true, this, cb);
	HTMLUIPanel *container = new HTMLUIPanel(view, url, x, y, width, height);

	this->panels[name] = container;

	return container;
}

HTMLUI::~HTMLUI()
{
}


HTMLUIMethodHandler::HTMLUIMethodHandler(HTMLUI * htmlUI)
{
	mHTMLUI = htmlUI;
}

bool HTMLUIMethodHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList &arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	CefRefPtr<CefV8Value> obj = CefV8Value::CreateObject(NULL, NULL);
	CefRefPtr<CefV8Value> arr = CefV8Value::CreateArray(0);
	CefV8ValueList::const_iterator it;
	for (int i = 0; i < arguments.size(); i++) {
		arr->SetValue(i, arguments[i]);
	}

	obj->SetValue("method_name", CefV8Value::CreateString(name), V8_PROPERTY_ATTRIBUTE_NONE);
	obj->SetValue("args", arr, V8_PROPERTY_ATTRIBUTE_NONE);

	mHTMLUI->game->ProcessUI(name, obj);

	// Function does not exist.
	return true;
}


HTMLUISpriteHandler::HTMLUISpriteHandler()
	:mOffset(0)
{
}

bool HTMLUISpriteHandler::ReadFromFile(std::string filePath)
{
	ifstream file(filePath.c_str(), std::ifstream::binary);
	if (file) {
		file.seekg(0, file.end);
		fileSize = file.tellg();
		file.seekg(0, file.beg);

		fileBuffer = new char[fileSize];
		file.read(fileBuffer, fileSize);

		file.close();
		return true;
	}

	return false;
}

bool HTMLUISpriteHandler::ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback)
{
	bool handled = false;

	std::string url = request->GetURL();

	std::cmatch matches;
	std::regex pattern ("game://([a-zA-Z0-9]+)/([0-9]+)");

	std::string basePath = std::filesystem::current_path().string();
	mMimeType = "image/png";

	if (std::regex_match(url.c_str(), matches, pattern) && matches.size() == 3) {
		std::csub_match typeMatch = matches[1];
		std::csub_match idMatch = matches[2];
		std::string thingType = typeMatch.str();
		int thingID = std::stoi(idMatch.str(), nullptr, 0);

		std::string filePath = basePath + "/cache/" + thingType + "/" + idMatch.str() + ".png";
		if (ReadFromFile(filePath)) {
			handled = true;
		} else if (thingType == "sprite" && G_pGame->m_pSprite[thingID] != 0) {
			G_pGame->m_pSprite[thingID]->_pMakeSpriteSurface();

			sf::IntRect rect;
			rect.left = G_pGame->m_pSprite[thingID]->m_stBrush->sx;
			rect.top = G_pGame->m_pSprite[thingID]->m_stBrush->sy;
			rect.width = G_pGame->m_pSprite[thingID]->m_stBrush->szx;
			rect.height = G_pGame->m_pSprite[thingID]->m_stBrush->szy;
			
			sf::Texture tmp;
			tmp.loadFromImage(G_pGame->m_pSprite[thingID]->_localimage.copyToImage(), rect);

			sf::Image img = tmp.copyToImage();
			img.saveToFile(filePath);

			handled = ReadFromFile(filePath);
		}
	}

	if (handled)
	{
		//Indicate the headers are available.
		callback->Continue();
		return true;
	}

	return false;
}

void HTMLUISpriteHandler::GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl)
{
	response->SetMimeType(mMimeType);
	response->SetStatus(200);

	response_length = fileSize;
}

void HTMLUISpriteHandler::Cancel()
{

}

bool HTMLUISpriteHandler::ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback)
{
	bool has_data = false;
	bytes_read = 0;

	if (mOffset < fileSize)
	{
		//Copy the next block of data into the buffer.
		int transfer_size = std::min(bytes_to_read, static_cast<int>(fileSize - mOffset));
		memcpy(data_out, fileBuffer + mOffset, transfer_size);
		mOffset += transfer_size;

		bytes_read = transfer_size;
		has_data = true;
	}

	return has_data;
}


HTMLUISpriteHandler::~HTMLUISpriteHandler()
{
	delete[] fileBuffer;
}


/*
bool HTMLUISpriteHandler::CanGetCookie(const CefCookie& cookie)
{
	return true;
}

bool HTMLUISpriteHandler::CanSetCookie(const CefCookie& cookie)
{
	return true;
}*/

void HTMLUISpriteHandler::AddRef() const
{

}

bool HTMLUISpriteHandler::Release() const
{
	return true;
}

bool HTMLUISpriteHandler::HasOneRef() const
{
	return true;
}