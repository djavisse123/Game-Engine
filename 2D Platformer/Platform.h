/**
 * @file Platform.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the platform class. It holds a function
 * needed for the class to create a platform.
 */
#ifndef PLATFORM_H
#define PLATFORM_H

#include "GameObject.h"

class Platform : public GameObject
{
public:
    Platform(float posX, float posY, float width, float length, sf::Color color);
};

#endif