/**
 * @file CharacterHandler.h
 * @author Daniel Avisse (djavisse)
 * This is the CharacterHandler header file. This holds
 * all the functions need for this component to function.
 */
#ifndef CHARACTERHANDLER_H
#define CHARACTERHANDLER_H

#include "EventHandler.h"

class CharacterHandler : public EventHandler
{
public:
    CharacterHandler(void * parentObject);
    void onEvent(Event e);
};

#endif