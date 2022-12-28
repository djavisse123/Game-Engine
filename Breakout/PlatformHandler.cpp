/**
 * @file PlatformHandler.cpp
 * @author Daniel Avisse (djavisse)
 * This is the PlatformHandler class. This class handles any events
 * relating to the platform.
 */
#include "PlatformHandler.h"
#include "MovingPlatform.h"

/**
 * Constructs a PlatformHandler component using a super to the EventHandler
 * @param parentObject A reference to the object using the PlatformHandler
 */
PlatformHandler::PlatformHandler(void * parentObject) : EventHandler(parentObject) {

}

void PlatformHandler::onEvent(Event e) {
    switch(e.type){
        case Event::KEYBOARD_MOVE_INPUT:
        {
            MovingPlatform * platObj = static_cast<MovingPlatform *>(parentObject);
            platObj->move(e.player.velocityX, e.player.velocityY);
            break;
        }
    }
}