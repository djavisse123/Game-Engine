/**
 * @file MovingPlatform.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the movingPlatform class. This file contains
 * all the fields and functions needed for the movingPlatform.
 */
#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

#include "Platform.h"
//#include "MovePattern.h"

#define DEFAULT_WAITTIME 3.0
#define DEFAULT_MOVEDISTANCE 460
#define MOVE_HORIZONTAL 0
#define MOVE_VERTICAL 1

class MovingPlatform : public Platform
{
private:
    int platformSpeed = 300;
    sf::Vector2f platformVelocity;
    sf::Vector2f startingPosition;
    int patternNumber = 0;
    bool reverse = false;
    MovePattern *movePatternComponent;
    PlatformHandler * handleEvents;
    int platformNumber;
public:
    MovingPlatform(float posX, float posY, float width, float length, sf::Color color, int patternNumber, bool reverse);
    void setPlatformSpeed(int platformSpeed);
    int getPlatformSpeed();
    void setPlatformVelocity(sf::Vector2f velocity);
    sf::Vector2f getPlatformVelocity();
    void setStartingPosition(sf::Vector2f Position);
    sf::Vector2f getStartingPosition();
    void changePatternNumber(int patternNumber);
    int getPatternNumber();
    void changeReverse(bool reverse);
    bool getReverse();
    void setPlatformNumber(int number);
    int getPlatformNumber();
    PlatformHandler * getEventHandler();
};

#endif
