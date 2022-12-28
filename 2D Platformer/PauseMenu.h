/**
 * @file PauseMenu.h
 * @author Daniel Avisse (djavisse)
 * This the header file for the PauseMenu. This file contains
 * the function needed to display the PauseMenu.
 */
#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "GlobalTime.h"

class PauseMenu
{
private:
    sf::View *view;
    GlobalTime *time;
    sf::RenderWindow *target;
    sf::RectangleShape pauseBorder;
    sf::Font font;
    sf::Text pauseText;
public:
    PauseMenu(sf::RenderWindow *target, GlobalTime *time, sf::View *view);
    void drawMenu();
};

#endif