/**
 * @file CollideDeathZone.h
 * @author Daniel Avisse (djavisse)
 * This is the CollideDeathZone header file. This holds
 * all the functions need for this component to function.
 */
#ifndef COLLIDEDEATHZONE_H
#define COLLIDEDEATHZONE_H

#include "Collide.h"


class CollideDeathZone : public Collide 
{
public:
    CollideDeathZone(void *parentobject);
    bool checkCollision(void *object);
}; 



#endif