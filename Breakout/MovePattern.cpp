/**
 * @file MovePattern.cpp
 * @author Daniel Avisse (djavisse)
 * This is the MovePattern component. This component
 * is specifically used for the MovingPlatforms to control 
 * it's movement.
 */
#include "MovePattern.h"
#include "MovingPlatform.h"

/**
 * Constructs a MovePattern componet.
 * @param parentObject The object that is using this component
 * @param waitTime The amount of time the object should stop in place before moving again
 * @param moveDistance The amount of distance the object should move.
 */
MovePattern::MovePattern(void *parentObject, float waitTime, int moveDistance) : Move(parentObject) {
    //this->parentObject = parentObject;
    this->moveLeft = false;
    this->moveRight = true;
    this->moveUp = true;
    this->moveDown = false;
    this->waitTime = waitTime;
    this->moveDistance = moveDistance;
}

/**
 * This function allows for platforms to move horizontally
 * @param reverse The direction the platform will move.
 */
void MovePattern::movePatternHorizontal(bool reverse) {
    MovingPlatform *platform = static_cast<MovingPlatform *>(parentObject);
    //Timer used to pause a platform in place for a couple of seconds
    GlobalTime timer(NULL, 1);
    sf::FloatRect movPlatBoundingBox = platform->getGlobalBounds();
    //Moves the platform to the right if the moveRight flag is true
    if (moveRight) {
        //Checks if the platform has moved to the furtherest distance it's allowed to go before it has to switch directions
        if (movPlatBoundingBox.left + movPlatBoundingBox.width >= platform->getStartingPosition().x + moveDistance && !reverse) {
            platform->setPlatformVelocity(sf::Vector2f(0.f, 0.f));
            float startTime = timer.getTime();
            float lastTime = timer.getTime();
            //Checks if the amount of time that has passed is equal to the wait time.
            while(lastTime - startTime < waitTime) {
                lastTime = timer.getTime();
            }
            moveLeft = true;
            moveRight = false;  
        }
        //Checks if the platform has reached it's starting position (This time it's going in reverse)
        else if(movPlatBoundingBox.left >= platform->getStartingPosition().x && reverse) {
            float startTime = timer.getTime();
            float lastTime = timer.getTime();
            while(lastTime - startTime < waitTime) {
                lastTime = timer.getTime();
            }
            moveLeft = true;
            moveRight = false;
        }
        // Move the platform to the right
        else {     
            platform->move(sf::Vector2f(platform->getPlatformSpeed(), 0));
        }
    }
    //Moves the platform to the left if the moveLeft flag is true
    else if (moveLeft) {
        //Checks if the platform has reached it's starting position
        if (movPlatBoundingBox.left <= platform->getStartingPosition().x && !reverse) {
            platform->setPlatformVelocity(sf::Vector2f(0.f, 0.f));
            float startTime = timer.getTime();
            float lastTime = timer.getTime();
            //Checks if the amount of time that has passed is equal to the wait time.
            while(lastTime - startTime < waitTime) {
                lastTime = timer.getTime();
            }
            moveLeft = false;
            moveRight = true;    
        }
        //Checks if the platform has moved to the furtherest distance it's allowed to go before it has to switch directions
        //(This time it's going in reverse)
        else if (movPlatBoundingBox.left + movPlatBoundingBox.width <= platform->getStartingPosition().x - moveDistance && reverse) {
            platform->setPlatformVelocity(sf::Vector2f(0.f, 0.f));
            float startTime = timer.getTime();
            float lastTime = timer.getTime();
            while(lastTime - startTime < waitTime) {
                lastTime = timer.getTime();
            }
            moveLeft = false;
            moveRight = true;   
        }
        //Move the platform to the left
        else {     
            platform->move(sf::Vector2f(-platform->getPlatformSpeed(), 0));
        }
    }
}

/**
 * This function allows for platforms to move vertically.
 * @param reverse The direction the platform will move.
 */
void MovePattern::movePatternVertical(bool reverse) {
    MovingPlatform *platform = static_cast<MovingPlatform *>(parentObject);
    //Timer used to pause a platform in place for a couple of seconds
    GlobalTime timer(NULL, 1);
    sf::FloatRect movPlatBoundingBox = platform->getGlobalBounds();
    //Moves the platform to the right if the moveRight flag is true
    if (moveUp) {
        //Checks if the platform has moved to the furtherest distance it's allowed to go before it has to switch directions
        if (movPlatBoundingBox.top + movPlatBoundingBox.height <= platform->getStartingPosition().y - moveDistance && !reverse) {
            float startTime = timer.getTime();
            float lastTime = timer.getTime();
            //Checks if the amount of time that has passed is equal to the wait time.
            while(lastTime - startTime < waitTime) {
                lastTime = timer.getTime();
            }
            moveDown = true;
            moveUp = false;  
        }
        //Checks if the platform has reached it's starting position (This time it's going in reverse)
        else if(movPlatBoundingBox.top <= platform->getStartingPosition().y && reverse) {
            float startTime = timer.getTime();
            float lastTime = timer.getTime();
            while(lastTime - startTime < waitTime) {
                lastTime = timer.getTime();
            }
            moveDown = true;
            moveUp = false;
        }
        // Move the platform to the right
        else {     
            platform->move(sf::Vector2f(0, -platform->getPlatformSpeed()));
        }
    }
    //Moves the platform to the left if the moveLeft flag is true
    else if (moveDown) {
        //Checks if the platform has reached it's starting position
        if (movPlatBoundingBox.top >= platform->getStartingPosition().y && !reverse) {
            platform->setPlatformVelocity(sf::Vector2f(0.f, 0.f));
            float startTime = timer.getTime();
            float lastTime = timer.getTime();
            //Checks if the amount of time that has passed is equal to the wait time.
            while(lastTime - startTime < waitTime) {
                lastTime = timer.getTime();
            }
            moveDown = false;
            moveUp = true;    
        }
        //Checks if the platform has moved to the furtherest distance it's allowed to go before it has to switch directions
        //(This time it's going in reverse)
        else if (movPlatBoundingBox.top + movPlatBoundingBox.height >= platform->getStartingPosition().y + moveDistance && reverse) {
            platform->setPlatformVelocity(sf::Vector2f(0.f, 0.f));
            float startTime = timer.getTime();
            float lastTime = timer.getTime();
            while(lastTime - startTime < waitTime) {
                lastTime = timer.getTime();
            }
            moveDown = false;
            moveUp = true;   
        }
        //Move the platform to the left
        else {     
            platform->move(sf::Vector2f(0, platform->getPlatformSpeed()));
        }
    }
}