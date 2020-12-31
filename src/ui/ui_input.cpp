
#include "ui_input.hpp"
#include "ui_panel.hpp"
#include "ui_core.hpp"

#include <map>
#include <WinUser.h>

namespace ui
{

ui_input::ui_input(ui_game * _ui)
    : _ui(_ui)
{

}

uint64_t ui_input::sfkeyToWparam(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::LControl:		return VK_CONTROL;
        case sf::Keyboard::RControl:		return VK_CONTROL;
        case sf::Keyboard::LSystem:		return VK_LWIN;
        case sf::Keyboard::RSystem:		return VK_RWIN;
        case sf::Keyboard::Menu:		return VK_APPS;
        case sf::Keyboard::SemiColon:		return VK_OEM_1;
        case sf::Keyboard::Slash:		return VK_OEM_2;
        case sf::Keyboard::Equal:		return 	VK_OEM_PLUS;
        case sf::Keyboard::Dash:		return 	VK_OEM_MINUS;
        case sf::Keyboard::LBracket:		return 	VK_OEM_4;
        case sf::Keyboard::RBracket:		return 	VK_OEM_6;
        case sf::Keyboard::Comma:		return 	VK_OEM_COMMA;
        case sf::Keyboard::Period:		return 	VK_OEM_PERIOD;
        case sf::Keyboard::Quote:		return 	VK_OEM_7;
        case sf::Keyboard::BackSlash:		return 	VK_OEM_5;
        case sf::Keyboard::Tilde:		return 	VK_OEM_3;
        case sf::Keyboard::Escape:		return 	VK_ESCAPE;
        case sf::Keyboard::Space:		return 	VK_SPACE;
        case sf::Keyboard::Return:		return 	VK_RETURN;
        case sf::Keyboard::BackSpace:       return 	VK_BACK;
        case sf::Keyboard::Tab:       return 	VK_TAB;
        case sf::Keyboard::PageUp:		return 	VK_PRIOR;
        case sf::Keyboard::PageDown:       return 	VK_NEXT;
        case sf::Keyboard::End:       return 	VK_END;
        case sf::Keyboard::Home:       return 	VK_HOME;
        case sf::Keyboard::Insert:		return 	VK_INSERT;
        case sf::Keyboard::Delete:		return 	VK_DELETE;
        case sf::Keyboard::Add:       return 	VK_ADD;
        case sf::Keyboard::Subtract:		return 	VK_SUBTRACT;
        case sf::Keyboard::Multiply:		return 	VK_MULTIPLY;
        case sf::Keyboard::Divide:		return 	VK_DIVIDE;
        case sf::Keyboard::Pause:		return 	VK_PAUSE;
        case sf::Keyboard::F1:       return 	VK_F1;
        case sf::Keyboard::F2:       return 	VK_F2;
        case sf::Keyboard::F3:       return 	VK_F3;
        case sf::Keyboard::F4:       return 	VK_F4;
        case sf::Keyboard::F5:       return 	VK_F5;
        case sf::Keyboard::F6:       return 	VK_F6;
        case sf::Keyboard::F7:       return 	VK_F7;
        case sf::Keyboard::F8:       return 	VK_F8;
        case sf::Keyboard::F9:       return 	VK_F9;
        case sf::Keyboard::F10:       return 	VK_F10;
        case sf::Keyboard::F11:       return 	VK_F11;
        case sf::Keyboard::F12:       return 	VK_F12;
        case sf::Keyboard::F13:       return 	VK_F13;
        case sf::Keyboard::F14:       return 	VK_F14;
        case sf::Keyboard::F15:       return 	VK_F15;
        case sf::Keyboard::Left:       return 	VK_LEFT;
        case sf::Keyboard::Right:		return 	VK_RIGHT;
        case sf::Keyboard::Up:       return 	VK_UP;
        case sf::Keyboard::Down:       return 	VK_DOWN;
        case sf::Keyboard::Numpad0:		return 	VK_NUMPAD0;
        case sf::Keyboard::Numpad1:		return 	VK_NUMPAD1;
        case sf::Keyboard::Numpad2:		return 	VK_NUMPAD2;
        case sf::Keyboard::Numpad3:		return 	VK_NUMPAD3;
        case sf::Keyboard::Numpad4:		return 	VK_NUMPAD4;
        case sf::Keyboard::Numpad5:		return 	VK_NUMPAD5;
        case sf::Keyboard::Numpad6:		return 	VK_NUMPAD6;
        case sf::Keyboard::Numpad7:		return 	VK_NUMPAD7;
        case sf::Keyboard::Numpad8:		return 	VK_NUMPAD8;
        case sf::Keyboard::Numpad9:		return 	VK_NUMPAD9;
        case sf::Keyboard::A:						return	'A';
        case sf::Keyboard::B:						return	'B';
        case sf::Keyboard::C:						return	'C';
        case sf::Keyboard::D:						return	'D';
        case sf::Keyboard::E:						return	'E';
        case sf::Keyboard::F:						return	'F';
        case sf::Keyboard::G:						return	'G';
        case sf::Keyboard::H:						return	'H';
        case sf::Keyboard::I:						return	'I';
        case sf::Keyboard::J:						return	'J';
        case sf::Keyboard::K:						return	'K';
        case sf::Keyboard::L:						return	'L';
        case sf::Keyboard::M:						return	'M';
        case sf::Keyboard::N:						return	'N';
        case sf::Keyboard::O:						return	'O';
        case sf::Keyboard::P:						return	'P';
        case sf::Keyboard::Q:						return	'Q';
        case sf::Keyboard::R:						return	'R';
        case sf::Keyboard::S:						return	'S';
        case sf::Keyboard::T:						return	'T';
        case sf::Keyboard::U:						return	'U';
        case sf::Keyboard::V:						return	'V';
        case sf::Keyboard::W:						return	'W';
        case sf::Keyboard::X:						return	'X';
        case sf::Keyboard::Y:						return	'Y';
        case sf::Keyboard::Z:						return	'Z';
        case sf::Keyboard::Num0:					return	'0';
        case sf::Keyboard::Num1:					return	'1';
        case sf::Keyboard::Num2:					return	'2';
        case sf::Keyboard::Num3:					return	'3';
        case sf::Keyboard::Num4:					return	'4';
        case sf::Keyboard::Num5:					return	'5';
        case sf::Keyboard::Num6:					return	'6';
        case sf::Keyboard::Num7:					return	'7';
        case sf::Keyboard::Num8:					return	'8';
        case sf::Keyboard::Num9:					return	'9';
    }

    return VK_NONAME;
}

bool ui_input::capture_event(sf::Event event)
{
    bool eventCaptured = false;
    switch (event.type) {

        case sf::Event::MouseMoved:
        case sf::Event::MouseWheelMoved:
            eventCaptured = ui_capture_mouse_move(event);
            break;

        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased:
            eventCaptured = ui_capture_mouse_button(event);
            break;

        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased:
        case sf::Event::TextEntered:
            eventCaptured = ui_capture_key_event(event);
            break;

        default:
            break;
    }
    return eventCaptured;
}

bool ui_input::ui_capture_mouse_move(sf::Event event)
{

    if (event.type == sf::Event::MouseMoved)
    {
        if (
            event.mouseMove.x >= _ui->panel->x && event.mouseMove.x <= _ui->panel->x + _ui->panel->width &&
            event.mouseMove.y >= _ui->panel->y && event.mouseMove.y <= _ui->panel->y + _ui->panel->height
            )
        {
            _ui->panel->has_focus = true;
            _ui->panel->view->send_focus_event(true);
            _ui->panel->view->send_mouse_move_event(event.mouseMove.x - _ui->panel->x, event.mouseMove.y - _ui->panel->y, false);
            _ui->panel->view->update_texture();
            return false;
        }
    }

    if (event.type == sf::Event::MouseWheelMoved)
    {
        if (_ui->panel->has_focus)
        {
            _ui->panel->view->send_mouse_wheel_event(event.mouseWheel.x - _ui->panel->x, event.mouseWheel.y - _ui->panel->y, event.mouseWheel.delta, event.mouseWheel.delta);
            _ui->panel->view->update_texture();
            return true;
        }

    }

    //     for (std::pair<std::string, ui_panel *> entry : this->ui->panels) {
    //         ui_panel * panel = entry.second;
    //         if (
    //             event.mouseMove.x >= panel->mX && event.mouseMove.x <= panel->mX + panel->mWidth &&
    //             event.mouseMove.y >= panel->mY && event.mouseMove.y <= panel->mY + panel->mHeight
    //             ) {
    //             panel->hasFocus = true;
    //             panel->mView->SendFocusEvent(true);
    //             panel->mView->SendMouseMoveEvent(event.mouseMove.x - panel->mX, event.mouseMove.y - panel->mY, false);
    //             panel->mView->UpdateTexture();
    //         }
    //     }
    return false;
}

bool ui_input::ui_capture_mouse_button(sf::Event event)
{

    bool inPanel = (
        event.mouseButton.x >= this->_ui->panel->x && event.mouseButton.x <= this->_ui->panel->x + this->_ui->panel->width &&
        event.mouseButton.y >= this->_ui->panel->y && event.mouseButton.y <= this->_ui->panel->y + this->_ui->panel->height
        );
    if ((event.type == sf::Event::MouseButtonReleased && this->_ui->panel->mouse_down) || inPanel) {
        this->_ui->panel->view->send_mouse_click_event(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button, (event.type == sf::Event::MouseButtonReleased), 1);
        if (event.type == sf::Event::MouseButtonPressed) {
            this->_ui->panel->mouse_down = true;
            this->_ui->panel->has_focus = true;
            this->_ui->panel->view->send_focus_event(true);
            this->_ui->panel->view->update_texture();
            return true;
        }
        else {
            this->_ui->panel->mouse_down = false;
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed && !inPanel) {
        this->_ui->panel->has_focus = false;
        this->_ui->panel->view->send_focus_event(false);
    }




    //     for (std::pair<std::string, ui_panel *> entry : this->ui->panels) {
    //         ui_panel * panel = entry.second;
    //         bool inPanel = (
    //             event.mouseButton.x >= panel->mX && event.mouseButton.x <= panel->mX + panel->mWidth &&
    //             event.mouseButton.y >= panel->mY && event.mouseButton.y <= panel->mY + panel->mHeight
    //             );
    //         if ((event.type == sf::Event::MouseButtonReleased && panel->mouseDown) || inPanel) {
    //             panel->mView->SendMouseClickEvent(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button, (event.type == sf::Event::MouseButtonReleased), 1);
    //             if (event.type == sf::Event::MouseButtonPressed) {
    //                 panel->mouseDown = true;
    //                 panel->hasFocus = true;
    //                 panel->mView->SendFocusEvent(true);
    //                 panel->mView->UpdateTexture();
    //             }
    //             else {
    //                 panel->mouseDown = false;
    //             }
    //         }
    //         else if (event.type == sf::Event::MouseButtonPressed && !inPanel) {
    //             panel->hasFocus = false;
    //             panel->mView->SendFocusEvent(false);
    //         }
    //     }
    return false;
}

bool ui_input::ui_capture_key_event(sf::Event event)
{
    if (this->_ui->panel->has_focus) {
        if (event.type == sf::Event::TextEntered) {
            this->_ui->panel->view->send_key_event((char)event.text.unicode);
        }
        else {
            this->_ui->panel->view->send_key_event(sfkeyToWparam(event.key.code), (event.type == sf::Event::KeyReleased), event.key.control);
        }
        return true;
    }


    //     for (std::pair<std::string, ui_panel *> entry : this->ui->panels) {
    //         ui_panel * panel = entry.second;
    // 
    //         if (panel->hasFocus) {
    //             if (event.type == sf::Event::TextEntered) {
    //                 panel->mView->SendKeyEvent((char)event.text.unicode);
    //             }
    //             else {
    //                 panel->mView->SendKeyEvent(sfkeyToWparam(event.key.code), (event.type == sf::Event::KeyReleased), event.key.control);
    //             }
    //         }
    //     }
    return false;
}

ui_input::~ui_input()
{
}

}
