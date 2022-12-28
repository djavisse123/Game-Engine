/**
 * @file DeathZone.cpp
 * @author Daniel Avisse (djavisse)
 * This is the DeathZone class and it is also a GameObject that is
 * a part of our GameObject Model. This class creates a GameObject that is
 * able to be collided with by a character and when the character collides with
 * the DeathZone they will die. Once a character dies they lose a life and will respawn back
 * at their spawn point.
 */
#include "DeathZone.h"

/**
 * This constructs a DeathZone
 * @param posX The x coordinate of the position for the DeathZone
 * @param posY The Y coordinate of the position for the DeathZone
 * @param width The width of the DeathZone
 * @param length The length of the DeathZone
 */
DeathZone::DeathZone(int posX, int posY, float width, float length) {
    this->setFillColor(sf::Color(0, 0, 0));
    this->setSize(sf::Vector2f(width, length));
    this->setPosition(posX, posY);
    this->collideComponent = new CollideDeathZone(this);
}