/**
 * @file ServerHandler.h
 * @author Daniel Avisse (djavisse)
 * This is the ServerHandler header file. This holds
 * all the functions need for this component to function.
 */
#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include "EventHandler.h"

class ServerHandler : public EventHandler
{
public:
    ServerHandler(void * parentObject);
    void onEvent(Event e);
};

#endif