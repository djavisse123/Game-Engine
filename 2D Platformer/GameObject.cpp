/**
 * @file GameObject.cpp
 * @author Daniel Avisse (djavisse)
 * This is the GameObject class. This class functions as the main representation
 * for our Game Object Model. Objects like the Character and MovingPlatform will inherit
 * from this class and have access to the components this class uses by default. Each 
 * of the actual objects can also have access to their own specific components as well.
 */
#include "GameObject.h"

/**
 * This is the constructor for the GameObject. This creates the GameObject
 * ane sets all the components
 */
GameObject::GameObject() {
    this->collideComponent = NULL;
    this->moveComponet = NULL;
}

/**
 * This function returns the collide component used by the GameObject.
 * @return Collide* Pointer to the collide component used by the GameObject.
 */
Collide * GameObject::getCollide() {
    return collideComponent;
}

/**
 * This function returns the move component used by the GameObject
 * @return Move* Pointer to the move component used by the GameObject.
 */
Move * GameObject::getMove() {
    return moveComponet;
}