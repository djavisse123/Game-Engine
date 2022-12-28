/**
 * @file CollisionHandler.h
 * @author Daniel Avisse (djavisse)
 * This is the CollisionHandler header file. This holds
 * all the functions need for this component to function.
 */
#ifndef COLLSIONHANDLER_H
#define COLLSIONHANDLER_H

#include "EventHandler.h"

class CollisionHandler : public EventHandler
{
public:
    CollisionHandler(void * parentObject);
    void onEvent(Event e);
};

#endif


