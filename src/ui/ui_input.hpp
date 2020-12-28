#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

namespace ui
{

class ui_game;

class ui_input
{
public:
    ui_input(ui_game * _ui);
    ~ui_input();

    uint64_t sfkeyToWparam(sf::Keyboard::Key key);
    bool ui_capture_mouse_move(sf::Event event);
    bool ui_capture_mouse_button(sf::Event event);
    bool ui_capture_key_event(sf::Event event);
    bool capture_event(sf::Event event);
    ui_game * _ui;
};

}
