/**
 * @file CollideSideBoundary.cpp
 * @author Daniel Avisse (djavisse)
 * This is the CollideSideBounary component. This component is specifically used 
 * for the SideBoundary object to check when a Character collides with it
 * to then produce a screen transition effect.
 */
#include "CollideSideBoundary.h"
#include "SideBoundary.h"
#include "Character.h"

/**
 * Constructs a CollideSideBoundary component using a super to the Collide component
 * @param parentObject A reference to the object using the CollideSideBoundary component
 */
CollideSideBoundary::CollideSideBoundary(void *parentObject) : Collide(parentObject) {
    
}

/**
 * This function is an overridden version of the Collide component's checkCollision function. 
 * This function checks to see if a character object has collide with the
 * SideBoundary. If the Character does collide with the boundary then
 * the view will adjust causing a screen transtion effect. The SideBoundary will
 * also move as well so the Character can continue to collide with them and produce
 * a smooth screen moving effect when moving.
 * @param object The Character object colliding into the boundary
 * @return true If the Character collides with the boundary
 * @return false If the Character doesn't collide with the boundary
 */
bool CollideSideBoundary::checkCollision(void *object) {
    //Parse the void pointers into the objects we need
    Character *charObject = static_cast<Character *>(object);
    sf::FloatRect objectBoundingBox = charObject->getGlobalBounds();
    SideBoundary *parObj = static_cast<SideBoundary *>(parentObject);
    sf::FloatRect parentObjectBoundingBox = parObj->getGlobalBounds();
    //Check if the sprite is intersecting with the boundary
    if (parentObjectBoundingBox.intersects(objectBoundingBox)) {
        // Left side of the SideBoundary
        if (objectBoundingBox.left < parentObjectBoundingBox.left && objectBoundingBox.left + objectBoundingBox.width < parentObjectBoundingBox.left + parentObjectBoundingBox.width) {
            //Create an event representing a SideBoundary collision
            Event e;
            e.type = Event::LEFT_COLLISION;
            if(charObject->getPosition().x <= 1520) {
                e.sideBoundary.viewCenterX = charObject->getPosition().x;
            }
            else {
                e.sideBoundary.viewCenterX = 1520;
            } 
            e.sideBoundary.boundaryPosX = objectBoundingBox.left + objectBoundingBox.width;
            e.sideBoundary.boundaryPosY = parObj->getPosition().y;
            //Get an instance of the EventManager to raise the event
            EventManager * manager = EventManager::getInstance();
            manager->raiseEvents(e);
        }
        //Right side of the SiBeboundary
        if (objectBoundingBox.left + objectBoundingBox.width > parentObjectBoundingBox.left + parentObjectBoundingBox.width && objectBoundingBox.left > parentObjectBoundingBox.left) {
            //Create an event representing a SideBoundary collision
            Event e;
            e.type = Event::RIGHT_COLLISION;
            if(charObject->getPosition().x >= 400) {
                e.sideBoundary.viewCenterX = charObject->getPosition().x;
            }
            else {
                e.sideBoundary.viewCenterX = 400;
            } 
            e.sideBoundary.boundaryPosX = objectBoundingBox.left - parentObjectBoundingBox.width;
            e.sideBoundary.boundaryPosY = parObj->getPosition().y;
            //Get an instance of the EventManager to raise the event
            EventManager * manager = EventManager::getInstance(); 
            manager->raiseEvents(e);
        }
        // Top of the SideBoundary
        if (objectBoundingBox.top < parentObjectBoundingBox.top && objectBoundingBox.top + objectBoundingBox.height < parentObjectBoundingBox.height + parentObjectBoundingBox.top) {
            //Create an event representing a SideBoundary collision
            Event e;
            e.type = Event::TOP_COLLISION;
            if(charObject->getPosition().y <= 780) {
                e.sideBoundary.viewCenterY = charObject->getPosition().y;
            }
            else {
                e.sideBoundary.viewCenterY = 780;
            }
            e.sideBoundary.boundaryPosX = parObj->getPosition().x;
            e.sideBoundary.boundaryPosY = objectBoundingBox.top + parentObjectBoundingBox.height;
            //Get an instance of the EventManager to raise the event
            EventManager * manager = EventManager::getInstance();
            manager->raiseEvents(e);
        }
        // Bottom of the SideBoundary
        if (objectBoundingBox.top + objectBoundingBox.height > parentObjectBoundingBox.top + parentObjectBoundingBox.height && objectBoundingBox.top > parentObjectBoundingBox.top)  {
            //Create an event representing a SideBoundary collision
            Event e;
            e.type = Event::BOTTOM_COLLISION;
            if(charObject->getPosition().y >= 300) {
                e.sideBoundary.viewCenterY = charObject->getPosition().y;
            }
            else {
                e.sideBoundary.viewCenterY = 300;
            }
            e.sideBoundary.boundaryPosX = parObj->getPosition().x;
            e.sideBoundary.boundaryPosY = objectBoundingBox.top - objectBoundingBox.height;
            //Get an instance of the EventManager to raise the event
            EventManager * manager = EventManager::getInstance();
            manager->raiseEvents(e);
        }
        return true;
    }
    return false;
}