/**
 * @file ScriptHandler.h
 * @author Daniel Avisse (djavisse)
 * This is the ScriptHandler header file. This holds
 * all the functions need for this handler to function.
 */
#ifndef SCRIPTHANDLER_H
#define SCRIPTHANDLER_H

#include "EventHandler.h"
#include <v8.h>

class ScriptHandler : public EventHandler
{
private:
    v8::Isolate * isolate;
public:
    ScriptHandler(void * parentObject);
    void onEvent(Event e);
    void setIsolate(v8::Isolate * isolate);
};

#endif