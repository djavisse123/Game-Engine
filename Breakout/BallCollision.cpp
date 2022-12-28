/**
 * @file BallCollision.cpp
 * @author Daniel Avisse (djavisse)
 * This is the BallCollision class. This component is used specifically for
 * for when the character object is a ball so that when it collides with certain objects
 * it will bounce in a certain direction.
 */
#include "Collide.h"
#include "GameObject.h"
#include "Character.h"
#include "BallCollision.h"

/**
 * Constructs a BallCollision component using a super to the Collide component
 * @param parentObject A reference to the object using the BallCollision component
 */
BallCollision::BallCollision(void *parentObject) : Collide(parentObject) {
    
}

/**
 * This is the checkCollision function. This function is used to check if
 * two objects are colliding and will act accordingly to prevent the objects
 * from intersecting.
 * @param object The other object coming in contact 
 */
bool BallCollision::checkCollision(void *object) {
    GameObject *otherObject = static_cast<GameObject *>(object);
    sf::FloatRect objectBoundingBox = otherObject->getGlobalBounds();
    Character *parObj = static_cast<Character *>(parentObject);
    sf::FloatRect parentObjectBoundingBox = parObj->getGlobalBounds();
    if (parentObjectBoundingBox.intersects(objectBoundingBox)) {
        //Left side of the parentObject
        if (objectBoundingBox.left < parentObjectBoundingBox.left && objectBoundingBox.left + objectBoundingBox.width < parentObjectBoundingBox.left + parentObjectBoundingBox.width) {
            parObj->setCharacterVelocity(sf::Vector2f(1, parObj->getCharacterVelocity().y));
        }
        // Right side of the parentObject
        else if (objectBoundingBox.left + objectBoundingBox.width > parentObjectBoundingBox.left + parentObjectBoundingBox.width && objectBoundingBox.left > parentObjectBoundingBox.left) {
            parObj->setCharacterVelocity(sf::Vector2f(-1, parObj->getCharacterVelocity().y));
        }
        // Top of the parentObject
        if (objectBoundingBox.top < parentObjectBoundingBox.top && objectBoundingBox.top + objectBoundingBox.height < parentObjectBoundingBox.height + parentObjectBoundingBox.top) {
            parObj->setCharacterVelocity(sf::Vector2f(parObj->getCharacterVelocity().x, 1));
        }
        // Bottom of the parentObject
        else if (objectBoundingBox.top + objectBoundingBox.height > parentObjectBoundingBox.top + parentObjectBoundingBox.height && objectBoundingBox.top > parentObjectBoundingBox.top)  {
            parObj->setCharacterVelocity(sf::Vector2f(parObj->getCharacterVelocity().x, -1));
        }
        return true;
    }
    return false;
}