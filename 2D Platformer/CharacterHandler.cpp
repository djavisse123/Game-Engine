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
            if(spriteObj->getCharacterNumber() == e.sprite.characterNumber) {
                //Move the position of the character so it no longer intersects with a platform
                spriteObj->setPosition(e.sprite.posX, e.sprite.posY);
            }
            break;
        }
        case Event::DEATH_COLLISION:
        {
            Character *spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.sprite.characterNumber) {
                //Kill the character and make them respawn
                spriteObj->getHealth()->instantKO();
            }
            break;
        }
        case Event::SPAWN_EVENT:
        {   
            Character *spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.sprite.characterNumber) {
                //Use the character's spawnPoint and place them at the location of it.
                spriteObj->setPosition(spriteObj->getSpawnPoint()->getSpawn());
            }
            //Create an event to reset the boundaries
            Event resetEvent;
            resetEvent.type = Event::REST_BOUNDARY;
            EventManager * manager = EventManager::getInstance();
            manager->raiseEvents(resetEvent);
            break;
        }
        case Event::KEYBOARD_MOVE_INPUT:
        {
            Character * spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.sprite.characterNumber) {
                //Move the character
                spriteObj->move(e.sprite.velocityX, e.sprite.velocityY);
            }
            break;
        }
        case Event::CHEATS_ACTIVE:
        {
            Character * spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.sprite.characterNumber) {
                //Set the cheat status to true
                spriteObj->setCheatStatus(true);
            }
            break;
        }
        case Event::CHEAT_DISABLE:
        {
            Character * spriteObj = static_cast<Character *>(parentObject);
            //Check that this event belongs to this character
            if(spriteObj->getCharacterNumber() == e.sprite.characterNumber) {
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