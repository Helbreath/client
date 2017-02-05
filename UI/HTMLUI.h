#pragma once

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>
using namespace Awesomium;

#define WIDTH   800
#define HEIGHT  600


class HTMLUI
{
public:
	WebView *view;
	WebCore *core;
	BitmapSurface *surface;

	HTMLUI(class CGame *pGame);
	~HTMLUI();
	void Init();
	bool isDirty();
	void Update(int sleep_ms);
};