/**
 * @file EventManager.cpp
 * @author Daniel Avisse (djavisse)
 * This is the EventManager class. This class is responsible for hold all events created
 * by other classes and then handling them at a certain time. This class uses 
 * a singleton so an instance of the manager can be used anywhere. Allowing for any class
 * to raise events and then let the manager handle them. To handle events, handlers 
 * can be registered to handle a certain event type they care about.
 */
#include "EventManager.h"

/**
 * This is the constructor for the EventManager. This creates
 * a new Manager to be used to hold and manage events.
 */
EventManager::EventManager() {
    //Construct Manager
}

/**
 * This function uses the EventManager's singleton design and returns an instance of the
 * EventManager. If an instance of the Manager doesn't exist then a new one will be created
 * and returned.
 * @return EventManager* An instance of the EventManager
 */
EventManager * EventManager::getInstance() {
    //If the eventManagerInstance doesn't exist then create a new one
    if(eventManagerInstance == 0) {
        printf("A new instance of the Event Manager has been created\n");
        eventManagerInstance = new EventManager();
    }
    //Return an the current instance of the eventManager
    return eventManagerInstance;
}

/**
 * This function allows for event handlers to be registered with the type of events they respond to.
 * @param listOfTypes The different types of Events the handler responds to.
 * @param handler The handler responding to different events
 */
void EventManager::registerHandlers(std::vector<Event::EventType> listOfTypes, EventHandler * handler) {
    //For each of the types assign the handler to it
    for(int i = 0; i < listOfTypes.size(); i++) {
        //First check to see if the event type is already in the map
        auto it = eventManagerInstance->registeredHandlers.find(listOfTypes[i]);
        if(it != eventManagerInstance->registeredHandlers.end()) {
            //If it's in the map then add the handler to the list of handlers
            it->second.push_back(handler);
        }
        else {
            //If not then create a new list of handlers and add it to the map
            std::vector<EventHandler *> listOfHandlers;
            listOfHandlers.push_back(handler);
            eventManagerInstance->registeredHandlers.insert(std::pair<Event::EventType, 
            std::vector<EventHandler *>>(listOfTypes[i], listOfHandlers));
        }
    }
}

/**
 * Sets the timeline for the EventManger
 * @param timeline A pointer to the timeline
 */
void EventManager::setGlobalTime(GlobalTime * timeline) {
    this->timeline = timeline;
}

/**
 * Adds events to the eventQueue and gives them a timestamp
 * @param e The event being added to the eventQueue
 */
void EventManager::raiseEvents(Event e) {
    //Assign the event a timestamp
    e.timeStamp = eventManagerInstance->timeline->getTime();
    //Add the event to the eventQueue
    eventManagerInstance->eventQueue.push(e);
}

/**
 * This function looks through the eventQueue and will handle every event
 * with a timestamp less than or equal to the current time.
 */
void EventManager::handleEvents() {
    //Use the timeline to get the currentTime
    float currentTime = eventManagerInstance->timeline->getTime();
    while(!eventManagerInstance->eventQueue.empty()) {
        Event e = eventManagerInstance->eventQueue.top();
        //Only handle events with a time equal to or less than the current time.
        if(e.timeStamp <= currentTime) {
            //Find the correct handlers using the event type
            auto it = eventManagerInstance->registeredHandlers.find(e.type);
            for(int i = 0; i < it->second.size(); i++) {
                //Handle the event
                it->second[i]->onEvent(e);
            }
            //Remove the event from the queue
            eventManagerInstance->eventQueue.pop();
        }
        else {
            break;
        }
    }
}

EventManager * EventManager::eventManagerInstance;