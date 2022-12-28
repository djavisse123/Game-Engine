/**
 * @file MovingPlatform.cpp
 * @author Daniel Avisse (djavisse)
 * This is the MovingPlatform class and it is also a GameObject that is
 * a part of our GameObject Model. This class creates a GameObject that
 * represents a platform that a character can stand on and these platforms
 * also have the ability to move around.
 */
#include "MovingPlatform.h"

/**
 * This constructs a MovingPlatform object
 * @param posX The x coordinate of the position for the Platform
 * @param posY The Y coordinate of the position for the Platform
 * @param width The width of the Platform
 * @param length The length of the Platform
 * @param color The color of the Platform
 * @param patternNumber The number representing which pattern the platform will use
 * @param reverse The direction the platform will move.
 */
MovingPlatform::MovingPlatform(float posX, float posY, float width, float length, sf::Color color, int patternNumber, bool reverse) : 
Platform(posX, posY, width, length, color) {
    this->startingPosition = sf::Vector2f(posX, posY);
    this->moveComponet = new MovePattern(this, DEFAULT_WAITTIME, DEFAULT_MOVEDISTANCE);
    this->patternNumber = patternNumber;
    this->reverse = reverse;
}

/**
 * Sets the movement speed of the platform
 * @param platformSpeed The speed of the platform
 */
void MovingPlatform::setPlatformSpeed(int platformSpeed) {
    this->platformSpeed = platformSpeed;
}

/**
 * Gets the movement speed of the platform
 * @return int The speed of the platform
 */
int MovingPlatform::getPlatformSpeed()
{
    return platformSpeed;
}

/**
 * Sets the velocity of the platform
 * @param velocity The velocity of the platform
 */
void MovingPlatform::setPlatformVelocity(sf::Vector2f velocity) {
    this->platformVelocity = velocity;
}

/**
 * Gets the velocity of the platform
 * @return sf::Vector2f The velocity of the platform
 */
sf::Vector2f MovingPlatform::getPlatformVelocity() {
    return this->platformVelocity;
}

/**
 * Sets the starting position of the platform
 * @param position The position of where the platform is first created
 */
void MovingPlatform::setStartingPosition(sf::Vector2f position) {
    this->startingPosition = position;
}

/**
 * Gets the starting position of the platform
 * @return sf::Vector2f The position of where the platform is first created
 */
sf::Vector2f MovingPlatform::getStartingPosition() {
    return this->startingPosition;
}

/**
 * Changes the movement pattern number of the platform to give it different movement
 * @param patternNumber The number representing the movement pattern of the platform
 */
void MovingPlatform::changePatternNumber(int patternNumber) {
    this->patternNumber = patternNumber;
}

/**
 * Gets the movement patten number of the platform
 * @return int The number representing the movement pattern of the platform
 */
int MovingPlatform::getPatternNumber() {
    return this->patternNumber;
}

/**
 * Changes the direction of the platform to go in the opposite.
 * @param reverse The boolean representing the direction the platform moves.
 */
void MovingPlatform::changeReverse(bool reverse) {
    this->reverse = reverse;
}

/**
 * Gets the direction of the platform
 * @return true The platform is moving in reverse
 * @return false The platform is moving normal
 */
bool MovingPlatform::getReverse() {
    return this->reverse;
}
