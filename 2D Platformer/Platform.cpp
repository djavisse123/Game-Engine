/**
 * @file Platform.cpp
 * @author Daniel Avisse (djavisse)
 * This is the Platform class and it is also a GameObject that is
 * a part of our GameObject Model. This class creates a GameObject that
 * represents a static platform a character can stand on.
 */
#include "Platform.h"

/**
 * This constructs a Platform object
 * @param posX The x coordinate of the position for the Platform
 * @param posY The Y coordinate of the position for the Platform
 * @param width The width of the Platform
 * @param length The length of the Platform
 * @param color The color of the Platform
 */
Platform::Platform(float posX, float posY, float width, float length, sf::Color color) {
    this->setSize(sf::Vector2f(width, length));
    this->setPosition(posX, posY);
    this->setFillColor(sf::Color(color));

}