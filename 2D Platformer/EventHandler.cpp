#include "EventHandler.h"

/**
 * This is the constructor for the EventHandler component. This creates
 * a new EventHandler and creates a reference to the object
 * that is using it.
 * @param parentObject A reference to the object using the EventHandler
 */
EventHandler::EventHandler(void * parentObject) {
    this->parentObject = parentObject;
}