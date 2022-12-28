/**
 * @file PlatformMove.cpp
 * @author Daniel Avisse (djavisse)
 * This is the PlatformMove component. This component is
 * specifically used for the MovingPlatform object to allow for
 * keyboard inputs to control the movements of the MovingPlatform.
 */
#include "MovingPlatform.h"
#include "PlatformMove.h"

/**
 * Constructs a PlatformMove componet using a super to the Move component.
 * @param parentObject The object that is using this component
 */
PlatformMove::PlatformMove(void *parentObject) : Move(parentObject) {
    
}

/**
 * This function takes the keyboard inputs of the user and allows for the MovingPlatform 
 * to move on screen.
 * @param deltaTime The amount of time between frames
 */
void PlatformMove::movement(float deltaTime) {
    MovingPlatform *mPlat = static_cast<MovingPlatform *>(parentObject);
    float spriteSpeed = mPlat->getPlatformSpeed();
    sf::Vector2f velocity = mPlat->getPlatformVelocity();
    velocity.x = 0;
    velocity.y = 0;
    //Check if a key has been pressed before raising the event
    bool eventFlag = false;
    //Create an event in prepartion of a keyboard event
    Event e;
    e.type = Event::KEYBOARD_MOVE_INPUT;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        eventFlag = true;
        velocity.x -= spriteSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        eventFlag = true;
        velocity.x += spriteSpeed;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        Event launchBall;
        launchBall.type = Event::LAUNCH_BALL;
        launchBall.player.characterNumber = mPlat->getPlatformNumber();
        launchBall.player.posX = mPlat->getPosition().x;
        EventManager * manager = EventManager::getInstance();
        manager->raiseEvents(launchBall);
    }
    mPlat->setPlatformVelocity(velocity);
    velocity *= deltaTime;
    e.player.velocityX = velocity.x;
    e.player.velocityY = velocity.y;
    //If a key has been pressed then raise the event
    if(eventFlag) {
        EventManager * manager = EventManager::getInstance();
        manager->raiseEvents(e);
    }
}