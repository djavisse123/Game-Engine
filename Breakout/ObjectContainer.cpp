/**
 * @file ObjectContainer.cpp
 * @author Daniel Avisse (djavisse)
 * This is ObjectContainer class. This class uses a singleton so any class
 * can access it. This class uses a map where the key is the name of a context
 * and the value is the name of the object inside that context.
 */
#include "ObjectContainer.h"

ObjectContainer::ObjectContainer() {
    //Construct the ObjectContainer
}

ObjectContainer* ObjectContainer::getInstance() {
    //If the objectContainerInstance doesn't exist then create a new one
    if(objectContainerInstance == 0) {
        objectContainerInstance = new ObjectContainer();
    }
    //Return an the current instance of the objectContainer
    return objectContainerInstance;
}

ObjectContainer * ObjectContainer::objectContainerInstance;
