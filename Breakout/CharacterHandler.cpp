/**
 * @file CharacterHandler.cpp
 * @author Daniel Avisse (djavisse)
 * This is the CharacterHandler class. This class handles any events
 * relating to the character.
 */
#include "CharacterHandler.h"
#include "Character.h"

/**
 * Constructs a CharacterHandler component using a super to the EventHandler
 * @param parentObject A reference to the object using the CharacterHandler
 */
CharacterHandler::CharacterHandler(void * parentObject) : EventHandler(parentObject) {

}

void CharacterHandler::onEvent(Event e) {
    switch(e.type){
        case Event::SPRITE_COLLISION:
        {
            Character *spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.player.characterNumber) {
                //Move the position of the character so it no longer intersects with a platform
                spriteObj->setPosition(e.player.posX, e.player.posY);
            }
            break;
        }
        case Event::DEATH_COLLISION:
        {
            Character *spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.player.characterNumber) {
                //Kill the character and make them respawn
                spriteObj->getHealth()->instantKO();
            }
            break;
        }
        case Event::SPAWN_EVENT:
        {   
            Character *spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.player.characterNumber) {
                //Use the character's spawnPoint and place them at the location of it.
                spriteObj->setPosition(spriteObj->getSpawnPoint()->getSpawn());
                //Reset the velocity to the default
                spriteObj->setCharacterVelocity(sf::Vector2f(-1.f, -1.f));
                //Set the character on the ground
                spriteObj->setOnGround();
            }
            break;
        }
        case Event::KEYBOARD_MOVE_INPUT:
        {
            Character * spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.player.characterNumber) {
                //Move the character
                spriteObj->move(e.player.velocityX, e.player.velocityY);
            }
            break;
        }
        case Event::LAUNCH_BALL:
        {
            Character * spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.player.characterNumber && spriteObj->getOnGround()) {
                //Move the ball in a certain direction based on where the player is
                if(e.player.posX <= 400) {
                    spriteObj->setCharacterVelocity(sf::Vector2f(1, -1));
                }
                else {
                    spriteObj->setCharacterVelocity(sf::Vector2f(-1, -1));
                }
                //Set the charcter off the ground
                spriteObj->setOffGround();
            }
            break;
        }
        case Event::CHEATS_ACTIVE:
        {
            Character * spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.player.characterNumber) {
                //Set the cheat status to true
                spriteObj->setCheatStatus(true);
            }
            break;
        }
        case Event::CHEAT_DISABLE:
        {
            Character * spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.player.characterNumber) {
                //Create an event to that will call the scripts to reset all values
                Event resetAll;
                resetAll.type = Event::RESET_ALL;
                EventManager * manager = EventManager::getInstance();
                manager->raiseEvents(resetAll);
                //Set cheat status to false
                spriteObj->setCheatStatus(false);
            }
            break;
        }
    }
}