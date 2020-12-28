
#pragma once

#include "ui_view.hpp"

namespace ui
{

class ui_panel
{
public:
    ui_panel(ui_view * _view, const std::string & _url, int _x, int _y, int _width, int _height);
    ~ui_panel();

    ui_view * view;
    sf::Sprite sprite;
    std::string url;
    int width;
    int height;
    int x;
    int y;
    bool has_focus;
    bool mouse_down;

    void paint();
};

}
