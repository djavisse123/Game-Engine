/**
 * @file CharacterMove.cpp
 * @author Daniel Avisse (djavisse)
 * This is the CharacterMove component. This component is
 * specifically used for the Character object to allow for
 * keyboard inputs to control the movements of the Character.
 */
#include "Character.h"
#include "CharacterMove.h"
#include <stdlib.h> 
#include <time.h> 

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

    //Window Collison to change the velocity of the sprite when hitting a wall
    //Left edge of screen
    sf::FloatRect spriteBoundingBox = sprite->getGlobalBounds();
    srand(time(NULL));
    if (spriteBoundingBox.left <= 0) {
        float direction = rand() % 3 + 1;
        velocity.x = (float) 1 / direction;
        if(velocity.y < 0) {
            velocity.y = -1;
        }
        else {
            velocity.y = 1;
        }
    }
    // Right edge of screen
    else if (spriteBoundingBox.left + spriteBoundingBox.width >= 800 ) {
        float direction = rand() % 3 + 1;
        velocity.x  = (float) -1 / direction;
        if(velocity.y < 0) {
            velocity.y = -1;
        }
        else {
            velocity.y = 1;
        }
    }
    // Top edge of screen
    if (spriteBoundingBox.top <= 0) {
        float direction = rand() % 2 + 1;
        velocity.y = (float) 1 / direction;
        if(velocity.x < 0) {
            velocity.x = -1;
        }
        else {
            velocity.x = 1;
        }
    }
    //Chord that when used it allows for cheats to be active which can allow the player to use the scripts
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) 
    && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        Event cheat;
        cheat.type = Event::CHEATS_ACTIVE;
        cheat.player.characterNumber = sprite->getCharacterNumber();
        EventManager * manager = EventManager::getInstance();
        manager->raiseEvents(cheat);
    }
    //Chord that when used it disables the cheats and the player can no longer use scripts
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && 
    sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sprite->getCheatStatus()) {
        Event cheat;
        cheat.type = Event::CHEAT_DISABLE;
        cheat.player.characterNumber = sprite->getCharacterNumber();
        EventManager * manager = EventManager::getInstance();
        manager->raiseEvents(cheat);
    }
    sprite->setCharacterVelocity(velocity);
    velocity.x *= (spriteSpeed *deltaTime);
    velocity.y *= (spriteSpeed *deltaTime);
    sprite->move(velocity);
}