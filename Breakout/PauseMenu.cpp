/**
 * @file PauseMenu.cpp
 * @author Daniel Avisse (djavisse)
 * This is the pauseMenu class. This class displays a menu for when the game is paused.
 */
#include "PauseMenu.h"

/**
 * This constructs a PauseMenu object and initializes all fields to create a
 * displayable pause menu.
 * @param target A pointer to the window to get the current state of it.
 * @param time A pointer to the timer to know when the game is paused or how long the game is running.
 * @param view A pointer to the view to know where to display elements
 */
PauseMenu::PauseMenu(sf::RenderWindow *target, GlobalTime *time, sf::View *view) {
    this->view = view;
    this->time = time;
    this->target = target;
    this->pauseBorder.setFillColor(sf::Color(166, 166, 166, 100));
    this->font.loadFromFile("kirby-classic.ttf");
    this->pauseText.setFont(this->font);
    this->pauseText.setString("Paused!");
    this->pauseText.setCharacterSize(125);
    this->pauseText.setFillColor(sf::Color(255, 255, 255));
}

/**
 * This function draws the pause menu. This function uses the renderTarget to draw to the window and
 * the timeline to display the elapsed paused time. 
 */
void PauseMenu::drawMenu()
{
    pauseBorder.setSize(sf::Vector2f(this->target->getSize().x, this->target->getSize().y));
    pauseBorder.setPosition(this->view->getCenter().x - (this->target->getSize().x / 2), 
    this->view->getCenter().y - (this->target->getSize().y / 2));
    sf::FloatRect pauseBounds = this->pauseText.getLocalBounds();
    pauseText.setOrigin(pauseBounds.top + pauseBounds.width / 2, pauseBounds.left + pauseBounds.height / 2);
    pauseText.setPosition(this->view->getCenter().x, this->view->getCenter().y);
    //Draw everything to the window
    target->draw(pauseBorder);
    target->draw(pauseText);
}