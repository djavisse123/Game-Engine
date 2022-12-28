/**
 * @file HUD.cpp
 * @author Daniel Avisse (djavisse)
 * This is the HUD class. This class allows for the display of onscreen information
 * like number of lives, a gameOver screen, and the pauseMenu. This class uses
 * the view and window from the client to present information constantly no matter 
 * where the player is on screen.
 */
#include "HUD.h"

/**
 * This constructor a new HUD object and initializes all the necessary elements
 * needed to display the HUD.
 * @param view A pointer to the view to know where to display elements
 * @param sprite A pointer to the Character being used to display certain information about it like lives and health
 * @param time A pointer to the timer to know when the game is paused or how long the game is running.
 * @param target A pointer to the window to get the current state of it.
 */
HUD::HUD(sf::View *view, Character *sprite, GlobalTime *time, sf::RenderWindow *target) {
    this->view = view;
    this->sprite = sprite;
    this->time = time;
    this->target = target;
    this->font.loadFromFile("kirby-classic.ttf");
    this->livesIconTexture.loadFromFile("KirbyLifeIcon.png");
    this->livesIcon.setSize(sf::Vector2f(40.f, 40.f));
    this->livesCounter.setFont(this->font);
    this->livesIcon.setTexture(&this->livesIconTexture);
    this->livesCounter.setCharacterSize(40);
    this->livesCounter.setOutlineThickness(3);
    this->livesCounter.setOutlineColor(sf::Color(255,148,197));
    this->livesCounter.setFillColor(sf::Color(254,220,237));
    this->pause = new PauseMenu(this->target, this->time, this->view);
    this->gameOver.setFont(this->font);
    this->gameOver.setCharacterSize(125);
    this->gameOver.setString("GAME OVER!");
    this->gameOver.setFillColor(sf::Color(255,255,255));
    this->gameOverBorder.setFillColor(sf::Color(0, 0, 0));
    this->cheats.setFont(this->font);
    this->cheats.setCharacterSize(30);
    this->cheats.setString("Cheats Active");
    this->cheats.setFillColor(sf::Color(255,255,255));
    this->target = target;
}

/**
 * This function is used to display the general information of the HUD. So either
 * this will display the number of lives the player has or will display the pauseMenu 
 * when the game is paused.
 */
void HUD::displayHUD() {
    if(this->time->getPauseStatus()) {
        this->pause->drawMenu();
    }
    else {
        std::string numberOfLives = "x" + std::to_string((int) this->sprite->getHealth()->getLives());
        this->livesCounter.setString(numberOfLives);
        this->livesCounter.setPosition(this->view->getCenter().x - 355, this->view->getCenter().y - 265);
        this->livesIcon.setPosition(this->view->getCenter().x - 400, this->view->getCenter().y - 260);
        this->target->draw(livesCounter);
        this->target->draw(livesIcon);
        if(this->sprite->getCheatStatus()) {
            this->cheats.setPosition(this->view->getCenter().x - 400, this->view->getCenter().y - 200);
            this->target->draw(cheats);
        }
    }
}

/**
 * This function displays the GameOver screen when a player loses all their lives.
 */
void HUD::displayGameOver() {
    this->gameOverBorder.setSize(sf::Vector2f(this->target->getSize().x, this->target->getSize().y));
    this->gameOverBorder.setPosition(this->view->getCenter().x - (this->target->getSize().x / 2),
    this->view->getCenter().y - (this->target->getSize().y / 2));
    sf::FloatRect gameOverBounds = this->gameOver.getLocalBounds();
    this->gameOver.setOrigin(gameOverBounds.top + gameOverBounds.width / 2, gameOverBounds.left + gameOverBounds.height / 2);
    this->gameOver.setPosition(this->view->getCenter().x, this->view->getCenter().y);
    this->target->draw(gameOverBorder);
    this->target->draw(gameOver);
}