#include "HTMLUI.h"

HTMLUIPanel::HTMLUIPanel(HTMLUIView *view, std::string url, int x, int y, int width, int height)
{
	mView = view;
	mX = x;
	mY = y;
	mWidth = width;
	mHeight = height;
	mUrl = url;

	// view->SendFocusEvent(true);
	// view->UpdateTexture();

	mSprite.setPosition(x, y);
	mSprite.setTexture(*view->GetTexture());

	mouseDown = false;
}

void HTMLUIPanel::paint()
{
	mView->UpdateTexture();
}
