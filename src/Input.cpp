#include "Input.h"

Input::Input(HTMLUI *ui)
{
	this->ui = ui;

}

bool Input::CaptureEvent(sf::Event event)
{

	bool eventCaptured = false;
	switch (event.type) {

	case sf::Event::MouseMoved:
		eventCaptured = UICaptureMouseMove(event);
		break;

	case sf::Event::MouseButtonPressed:
	case sf::Event::MouseButtonReleased:
		eventCaptured = UICaptureMouseButton(event);
		break;

	case sf::Event::KeyPressed:
	case sf::Event::KeyReleased:
	case sf::Event::TextEntered:
		eventCaptured = UICaptureKeyEvent(event);
		break;

	default:
		break;
	}
	return eventCaptured;
}

bool Input::UICaptureMouseMove(sf::Event event)
{
	for (std::pair<std::string, HTMLUIPanel*> entry : this->ui->panels) {
		HTMLUIPanel *panel = entry.second;
		if (
			event.mouseMove.x >= panel->mX && event.mouseMove.x <= panel->mX + panel->mWidth &&
			event.mouseMove.y >= panel->mY && event.mouseMove.y <= panel->mY + panel->mHeight
			) {
			panel->hasFocus = true;
			panel->mView->SendFocusEvent(true);
			panel->mView->SendMouseMoveEvent(event.mouseMove.x - panel->mX, event.mouseMove.y - panel->mY, false);
			panel->mView->UpdateTexture();
		}
	}
	return false;
}

bool Input::UICaptureMouseButton(sf::Event event)
{
	for (std::pair<std::string, HTMLUIPanel*> entry : this->ui->panels) {
		HTMLUIPanel *panel = entry.second;
		bool inPanel = (
			event.mouseButton.x >= panel->mX && event.mouseButton.x <= panel->mX + panel->mWidth &&
			event.mouseButton.y >= panel->mY && event.mouseButton.y <= panel->mY + panel->mHeight
		);
		if ((event.type == sf::Event::MouseButtonReleased && panel->mouseDown) || inPanel) {
			panel->mView->SendMouseClickEvent(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button, (event.type == sf::Event::MouseButtonReleased), 1);
			if (event.type == sf::Event::MouseButtonPressed) {
				panel->mouseDown = true;
				panel->hasFocus = true;
				panel->mView->SendFocusEvent(true);
				panel->mView->UpdateTexture();
			}
			else {
				panel->mouseDown = false;
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed && !inPanel) {
			panel->hasFocus = false;
			panel->mView->SendFocusEvent(false);
		}
	}
	return false;
}

bool Input::UICaptureKeyEvent(sf::Event event)
{
	for (std::pair<std::string, HTMLUIPanel*> entry : this->ui->panels) {
		HTMLUIPanel *panel = entry.second;

		if (panel->hasFocus) {
			if (event.type == sf::Event::TextEntered) {
				panel->mView->SendKeyEvent((char)event.text.unicode);
			}
			else {
				panel->mView->SendKeyEvent(sfkeyToWparam(event.key.code), (event.type == sf::Event::KeyReleased), event.key.control);
			}
		}
	}
	return false;
}

Input::~Input()
{
}
