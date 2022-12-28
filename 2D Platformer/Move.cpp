/**
 * @file Move.cpp
 * @author Daniel Avisse (djavisse)
 * This is the Move component. This class acts somewhat like an abstract 
 * class or interface where other move components like CharacterMove
 * can extend it and implement their own functinality.
 */
#include "Move.h"

/**
 * Constructs a Move component
 * @param parentObject A reference to the object using the Move component
 */
Move::Move(void *parentObject) {
    this->parentObject = parentObject;
}

void Move::movement(float deltaTime) {
    //Nothing
}