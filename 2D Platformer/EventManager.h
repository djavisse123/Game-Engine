/**
 * @file EventManager.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the EventManager class. This file proivdes
 * the fields and functions need for that class.
 */
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <map>
#include <list>
#include <queue>
#include "Event.h"
#include "EventHandler.h"
#include "GlobalTime.h"

class EventManager 
{
private:
    std::map <Event::EventType, std::vector<EventHandler *>> registeredHandlers;
    static EventManager * eventManagerInstance;
    std::priority_queue<Event, std::vector<Event>, Event::timeStampCompare> eventQueue;
    EventManager();
    GlobalTime * timeline;
public:
    static EventManager * getInstance();
    static void registerHandlers(std::vector<Event::EventType> listofTypes, EventHandler * handler);
    void setGlobalTime(GlobalTime * timeline);
    static void raiseEvents(Event e);
    static void handleEvents();
};

#endif