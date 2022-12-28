/**
 * @file PlatformHandler.h
 * @author Daniel Avisse (djavisse)
 * This is the PlatformHandler header file. This holds
 * all the functions need for this component to function.
 */
#ifndef PLATFORMHANDLER_H
#define PLATFORMHANDLER_H

#include "EventHandler.h"

class PlatformHandler : public EventHandler
{
public:
    PlatformHandler(void * parentObject);
    void onEvent(Event e);
};

#endif