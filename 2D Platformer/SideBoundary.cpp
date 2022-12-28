/**
 * @file SideBoundary.cpp
 * @author Daniel Avisse (djavisse)
 * This is the SideBoundary class and it is also a GameObject that is
 * a part of our GameObject Model. This class creates a GameObject that is
 * able to be collided with by a character to then produce a screen transtion effect.
 */
#include "SideBoundary.h"

/**
 * This constructs a SideBoundary
 * @param posX The x coordinate of the position for the SideBoundary
 * @param posY The Y coordinate of the position for the SideBoundary
 * @param width The width of the SideBoundary
 * @param length The length of the SideBoundary
 * @param view A pointer to the view 
 */
SideBoundary::SideBoundary(int posX, int posY, float width, float length, sf::View *view) {
    this->setPosition(posX, posY);
    this->setFillColor(sf::Color(0, 0, 0, 100));
    this->setSize(sf::Vector2f(width, length));
    this->startingPos = sf::Vector2i(posX, posY);
    this->view = view;
    this->collideComponent = new CollideSideBoundary(this);
    this->handleCollison = new CollisionHandler(this);
}

/**
 * Gets the current view of the game.
 * @return sf::View* The pointer of the view
 */
sf::View * SideBoundary::getView() {
    return this->view;
}

/**
 * Sets the starting position of the SideBoundary
 * @param posX The x coordinate of the position for the SideBoundary
 * @param posY The Y coordinate of the position for the SideBoundary
 */
void SideBoundary::setStartingPos(int posX, int posY) {
    this->startingPos = sf::Vector2i(posX, posY);
}

/** 
 * Gets the starting positon of the SideBoundary
 * @return sf::Vector2i The vector of the position
 */
sf::Vector2i SideBoundary::getStartingPos() {
    return this->startingPos;
}

/**
 * Gets the EventHandler for the SideBoundary that handles events specific for the SideBoundary
 * @return Collision* The EventHandler for the SideBoundary
 */
CollisionHandler * SideBoundary::getCollisionHandler() {
    return this->handleCollison;
}