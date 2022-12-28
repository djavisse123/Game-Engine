/**
 * @file CharacterMove.cpp
 * @author Daniel Avisse (djavisse)
 * This is the CharacterMove component. This component is
 * specifically used for the Character object to allow for
 * keyboard inputs to control the movements of the Character.
 */
#include "Character.h"
#include "CharacterMove.h"

/**
 * Constructs a CharacterMove componet using a super to the Move component.
 * @param parentObject The object that is using this component
 */
CharacterMove::CharacterMove(void *parentObject) : Move(parentObject) {
    
}

/**
 * This function takes the keyboard inputs of the user and allows for the Character 
 * to move on screen.
 * @param deltaTime The amount of time between frames
 */
void CharacterMove::movement(float deltaTime) {
    Character *sprite = static_cast<Character *>(parentObject);
    float spriteSpeed = sprite->getCharacterMovement();
    sf::Vector2f velocity = sprite->getCharacterVelocity();
    velocity.x = 0;
    //Check if a key has been pressed before raising the event
    bool eventFlag = false;
    //Create an event in prepartion of a keyboard event
    Event e;
    e.type = Event::KEYBOARD_MOVE_INPUT;
    e.sprite.characterNumber = sprite->getCharacterNumber();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        eventFlag = true;
        velocity.x -= spriteSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        eventFlag = true;
        velocity.x += spriteSpeed;
    }
    /* if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        sprite->move(sf::Vector2f(0, -2));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        sprite->move(sf::Vector2f(0, 2));
    } */
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && sprite->getCanJump()) {
        eventFlag = true;
        velocity.y = JUMPVELOCITY;
        sprite->setOffGround();
    }
    //Chord that when used it allows for cheats to be active which can allow the player to use the scripts
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) 
    && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        Event cheat;
        cheat.type = Event::CHEATS_ACTIVE;
        cheat.sprite.characterNumber = sprite->getCharacterNumber();
        EventManager * manager = EventManager::getInstance();
        manager->raiseEvents(cheat);
    }
    //Chord that when used it disables the cheats and the player can no longer use scripts
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && 
    sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sprite->getCheatStatus()) {
        Event cheat;
        cheat.type = Event::CHEAT_DISABLE;
        cheat.sprite.characterNumber = sprite->getCharacterNumber();
        EventManager * manager = EventManager::getInstance();
        manager->raiseEvents(cheat);
    }
    if (!sprite->getOnGround()) {
        velocity.y += GRAVITY;
    }
    sprite->setCharacterVelocity(velocity);
    velocity *= deltaTime;
    e.sprite.velocityX = velocity.x;
    e.sprite.velocityY = velocity.y;
    //If no key has been pressed then don't raise the event and apply any movement or gravity needed.
    if(!eventFlag) {
        sprite->move(velocity.x, velocity.y);
    }
    //If a key has been pressed then get an instance of the manager and raise the event
    else {
        EventManager * manager = EventManager::getInstance();
        manager->raiseEvents(e);
    }
}