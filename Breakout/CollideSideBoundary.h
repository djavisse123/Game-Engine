/**
 * @file CollideSideBoundary.h
 * @author Daniel Avisse (djavisse)
 * This is the CollideSideBoundary header file. This holds
 * all the functions need for this component to function.
 */
#ifndef COLLIDESIDEBOUNDARY_H
#define COLLIDESIDEBOUNDARY_H

#include "Collide.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class CollideSideBoundary : public Collide 
{
public:
    CollideSideBoundary(void *parentobject);
    bool checkCollision(void *object);
}; 



#endif