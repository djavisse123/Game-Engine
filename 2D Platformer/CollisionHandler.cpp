/**
 * @file CollisionHandler.cpp
 * @author Daniel Avisse (djavisse)
 * This the CollisionHandler class. This class handles events
 * relating to collision of the SideBoundary.
 */
#include "CollisionHandler.h"
#include "Character.h"
#include "SideBoundary.h"

/**
 * Constructs a CharacterHandler component using a super to the EventHandler
 * @param parentObject A reference to the object using the CharacterHandler
 */
CollisionHandler::CollisionHandler(void * parentObject) : EventHandler(parentObject) {

}

void CollisionHandler::onEvent(Event e) {
    switch(e.type){
        case Event::LEFT_COLLISION:
        {
            SideBoundary *sideBoundaryObj = static_cast<SideBoundary *>(parentObject);
            //Center the view to the sprite's X position and move the boundary to the right since the sprite is moving towards the right
            sideBoundaryObj->getView()->setCenter(e.sideBoundary.viewCenterX, sideBoundaryObj->getView()->getCenter().y);
            sideBoundaryObj->setPosition(e.sideBoundary.boundaryPosX, e.sideBoundary.boundaryPosY);
            break;
        }
        case Event::RIGHT_COLLISION:
        {
            SideBoundary *sideBoundaryObj = static_cast<SideBoundary *>(parentObject);
            //Center the view to the sprite's X position and move the boundary to the left since the sprite is moving towards the left
            sideBoundaryObj->getView()->setCenter(e.sideBoundary.viewCenterX, sideBoundaryObj->getView()->getCenter().y);
            sideBoundaryObj->setPosition(e.sideBoundary.boundaryPosX, e.sideBoundary.boundaryPosY);
            break;
        }
        case Event::TOP_COLLISION:
        {
            SideBoundary *sideBoundaryObj = static_cast<SideBoundary *>(parentObject);
            //Center the view to the sprite's Y position and move the boundary down since the sprite is moving downwards
            sideBoundaryObj->getView()->setCenter(sideBoundaryObj->getView()->getCenter().x, e.sideBoundary.viewCenterY);
            sideBoundaryObj->setPosition(e.sideBoundary.boundaryPosX, e.sideBoundary.boundaryPosY);
            break;
        }
        case Event::BOTTOM_COLLISION:
        {
            SideBoundary *sideBoundaryObj = static_cast<SideBoundary *>(parentObject);
            //Center the view to the sprite's Y position and move the boundary up since the sprite is move upwards
            sideBoundaryObj->getView()->setCenter(sideBoundaryObj->getView()->getCenter().x, e.sideBoundary.viewCenterY);
            sideBoundaryObj->setPosition(e.sideBoundary.boundaryPosX, e.sideBoundary.boundaryPosY);
            break;
        }
        //Not really a collision event but need to reset boundaries when either the sprite collides
        //with a death zone or spawns
        case Event::REST_BOUNDARY:
        {
            SideBoundary *sideBoundaryObj = static_cast<SideBoundary *>(parentObject);
            //Set the side boundary to it's starting position
            sideBoundaryObj->setPosition(sideBoundaryObj->getStartingPos().x, sideBoundaryObj->getStartingPos().y);
            break;
        }
    }
}