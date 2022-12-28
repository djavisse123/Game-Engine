/**
 * @file BallCollision.h
 * @author Daniel Avisse (djavisse)
 * This is the BallCollision header file. This holds
 * all the functions need for this component to function.
 */
#ifndef BALLCOLLISION_H
#define BALLCOLLISION_H

#include "Collide.h"


class BallCollision : public Collide 
{
public:
    BallCollision(void *parentobject);
    bool checkCollision(void *object);
}; 



#endif