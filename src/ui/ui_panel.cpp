
#include "ui_panel.hpp"

namespace ui
{

ui_panel::ui_panel(ui_view * _view, const std::string & _url, int _x, int _y, int _width, int _height)
    : view(_view)
    , url(_url)
    , x(_x)
    , y(_y)
    , width(_width)
    , height(_height)
    , mouse_down(false)
{
    // view->SendFocusEvent(true);
    // view->UpdateTexture();

    sprite.setPosition(x, y);
    sprite.setTexture(*view->get_texture());

}

ui_panel::~ui_panel()
{

}

void ui_panel::paint()
{
    view->update_texture();
}

}
