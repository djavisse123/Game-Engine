/**
 * @file PlatformThread.cpp
 * @author Daniel Avisse (djavisse)
 * This is the PlatformThread class. This class creates a threaded object used
 * to handle the movement pattern for the movingPlatform.
 */
#include "PlatformThread.h"

/**
 * Constructer for the PlatformThread class.
 * @param threadID Identifier for the thread
 * @param other A reference to another PlatformThread
 * @param _mutex A mutex lock used to allow for mutual exectuion
 * @param _condition_variable A condition variable used to message other threads
 * @param platform The movingPlatform being used by the PlatformThread
 */
PlatformThread::PlatformThread(int threadID, PlatformThread *other, std::mutex *_mutex, std::condition_variable *_condition_variable, MovingPlatform *platform) {
    this->threadID = threadID;
    this->other = other;
    this->_mutex = _mutex;
    this->_condition_variable = _condition_variable;
    this->platform = platform;
}

/**
 * This function is used to handle the movement pattern of the movingPlatform.
 */
void PlatformThread::handleMovement() {
    //Sleep for the length of the targetTime so then the platform moves at 60fps.
    std::this_thread::sleep_for(std::chrono::duration<float>(targetTime));
    if(platform->getPatternNumber() == MOVE_HORIZONTAL) {
        MovePattern *pattern = static_cast<MovePattern *>(platform->getMove());
        pattern->movePatternHorizontal(platform->getReverse());
    }
    else if(platform->getPatternNumber() == MOVE_VERTICAL) {
        MovePattern *pattern = static_cast<MovePattern *>(platform->getMove());
       pattern->movePatternVertical(platform->getReverse());
    }
}