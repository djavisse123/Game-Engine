/**
 * @file HUD.h
 * @author Daniel Avisse (djavisse)
 * This is the HUD header file. This file contains the fields and functions
 * needed for the HUD class to function.
 */
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Character.h"
#include "GlobalTime.h"
#include "PauseMenu.h"

class HUD {
private:
    sf::View *view;
    Character *sprite;
    GlobalTime *time;
    sf::RenderWindow *target;
    sf::RectangleShape livesIcon;
    sf::Font font;
    sf::Text livesCounter;
    sf::Texture livesIconTexture;
    sf::RectangleShape gameOverBorder;
    sf::Text gameOver;
    sf::Text cheats;
    sf::Text score;
    sf::Text currentLevel;
    PauseMenu * pause;
public:
    HUD(sf::View *view, Character *sprite, GlobalTime *time, sf::RenderWindow *target);
    void displayHUD();
    void displayGameOver();
};