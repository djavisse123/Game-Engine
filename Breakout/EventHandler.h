/**
 * @file EventHandler.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the EventHandler class. This file proivdes
 * the fields and functions need for that class. This class acts like an abstract
 * class where specific handlers will extend it and override the onEvent function.
 */
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "Event.h"

class EventHandler
{
protected:
    void * parentObject;
public:
    EventHandler(void * parentObject);
    virtual void onEvent(Event e) = 0;
};

#endif