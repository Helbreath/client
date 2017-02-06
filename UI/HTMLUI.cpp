#include "../Game.h"


#include "HTMLUI.h"


HTMLUI::HTMLUI(class CGame * pGame)
{
	core = WebCore::Initialize(WebConfig());
	view = this->core->CreateWebView(pGame->GetWidth(), pGame->GetHeight());
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
		HTMLUI::Update(25);
	}
	HTMLUI::Update(250);

	surface = (BitmapSurface*)view->surface();

	JSValue result = view->CreateGlobalJavascriptObject(WSLit("client"));
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