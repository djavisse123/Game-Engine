/**
 * @file CollideDeathZone.cpp
 * @author Daniel Avisse (djavisse)
 * This is the CollideDeathZone component. This component is specifically used 
 * for the DeathZone object to check when a Character collides with it
 * to cause the Character to die and then respawn at SpawnPoint.
 */
#include "CollideDeathZone.h"
#include "GameObject.h"
#include "Character.h"


/**
 * Constructs a CollideDeathZone component using a super to the Collide component
 * @param parentObject A reference to the object using the CollideDeathZone component
 */
CollideDeathZone::CollideDeathZone(void *parentObject) : Collide(parentObject) {
    
}

/**
 * This function is an overridden version of the Collide component's checkCollision function. 
 * This function checks to see if a Charcter object has collided with it. If so
 * then the function will uses the Character's health component to cause it die and the
 * Character when then return to it's spawn.
 * @param object The Character object colliding into the DeathZone
 * @return true If the Character collides with the DeathZone and is dead
 * @return false If the Character does not collide with the DeathZone
 */
bool CollideDeathZone::checkCollision(void *object) {
    Character *charObject = static_cast<Character *>(object);
    sf::FloatRect objectBoundingBox = charObject->getGlobalBounds();
    GameObject *parObj = static_cast<GameObject *>(parentObject);
    sf::FloatRect parentObjectBoundingBox = parObj->getGlobalBounds();
    if (parentObjectBoundingBox.intersects(objectBoundingBox)) {
        //Create an event to represent the character has collided with the deathzone
        Event e;
        e.type = Event::DEATH_COLLISION;
        e.player.characterNumber = charObject->getCharacterNumber();
        //Get an instance of the EventManager to raise the event
        EventManager * manager = EventManager::getInstance();
        manager->raiseEvents(e);
        return true;
    }
    return false;
}