/**
 * @file Collide.cpp
 * @author Daniel Avisse (djavisse)
 * This is the Collide class. This class acts as a component for the
 * GameObject class. GameObjects can use this component to do collision
 * dection.
 */
#include "Collide.h"
#include "GameObject.h"
#include "Character.h"

/**
 * This is the constructor for the Collide component. This creates
 * a new Collide componets and creates a reference to the object
 * that is using it.
 * @param parentObject A reference to the object using the Collide component
 */
Collide::Collide(void *parentObject) {
    this->parentObject = parentObject;
}

/**
 * This is the checkCollision function. This function is used to check if
 * two objects are colliding and will act accordingly to prevent the objects
 * from intersecting.
 * @param object The other object coming in contact 
 */
bool Collide::checkCollision(void *object) {
    GameObject *otherObject = static_cast<GameObject *>(object);
    sf::FloatRect objectBoundingBox = otherObject->getGlobalBounds();
    Character *parObj = static_cast<Character *>(parentObject);
    sf::FloatRect parentObjectBoundingBox = parObj->getGlobalBounds();
    if (parentObjectBoundingBox.intersects(objectBoundingBox)) {
        //Left side of the parentObject
        if (objectBoundingBox.left < parentObjectBoundingBox.left && objectBoundingBox.left + objectBoundingBox.width < parentObjectBoundingBox.left + parentObjectBoundingBox.width) {
            //Create an event representing a collision
            Event e;
            e.type = Event::SPRITE_COLLISION;
            e.sprite.posX = objectBoundingBox.width + objectBoundingBox.left;
            e.sprite.posY = parObj->getPosition().y;
            e.sprite.characterNumber = parObj->getCharacterNumber();
            //Get an instance of the EventManager to raise the event
            EventManager * manager = EventManager::getInstance();
            manager->raiseEvents(e);
        }
        // Right side of the parentObject
        else if (objectBoundingBox.left + objectBoundingBox.width > parentObjectBoundingBox.left + parentObjectBoundingBox.width && objectBoundingBox.left > parentObjectBoundingBox.left) {
            //Create an event representing a collision
            Event e;
            e.type = Event::SPRITE_COLLISION;
            e.sprite.posX = objectBoundingBox.left - parentObjectBoundingBox.width;
            e.sprite.posY = parObj->getPosition().y;
            e.sprite.characterNumber = parObj->getCharacterNumber();
            //Get an instance of the EventManager to raise the event
            EventManager * manager = EventManager::getInstance();
            manager->raiseEvents(e);
        }
        // Top of the parentObject
        if (objectBoundingBox.top < parentObjectBoundingBox.top && objectBoundingBox.top + objectBoundingBox.height < parentObjectBoundingBox.height + parentObjectBoundingBox.top) {
            //Create an event representing a collision
            Event e;
            e.type = Event::SPRITE_COLLISION;
            e.sprite.posX = parObj->getPosition().x;
            e.sprite.posY = objectBoundingBox.top + objectBoundingBox.height;
            //Get an instance of the EventManager to raise the event
            e.sprite.characterNumber = parObj->getCharacterNumber();
            EventManager * manager = EventManager::getInstance();
            manager->raiseEvents(e);
        }
        // Bottom of the parentObject
        else if (objectBoundingBox.top + objectBoundingBox.height > parentObjectBoundingBox.top + parentObjectBoundingBox.height && objectBoundingBox.top > parentObjectBoundingBox.top)  {
            //Create an event representing a collision
            Event e;
            e.type = Event::SPRITE_COLLISION;
            e.sprite.posX = parObj->getPosition().x;
            e.sprite.posY = objectBoundingBox.top - parentObjectBoundingBox.height;
            e.sprite.characterNumber = parObj->getCharacterNumber();
            //Get an instance of the EventManager to raise the event
            EventManager * manager = EventManager::getInstance();
            manager->raiseEvents(e);
        }
        return true;
    }
    else if((objectBoundingBox.top - (parentObjectBoundingBox.height + parentObjectBoundingBox.top)) <= 1
    && (objectBoundingBox.top - (parentObjectBoundingBox.height + parentObjectBoundingBox.top)) >= 0
    && objectBoundingBox.left + objectBoundingBox.width > parentObjectBoundingBox.left
    && objectBoundingBox.left < parentObjectBoundingBox.left + parentObjectBoundingBox.height) {
        return true;     
    }
    return false;
}