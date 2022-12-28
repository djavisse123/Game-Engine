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
PlatformThread::PlatformThread(int threadID, PlatformThread *other, std::mutex *_mutex, std::condition_variable *_condition_variable, MovingPlatform *platform, float delta_time) {
    this->threadID = threadID;
    this->other = other;
    this->_mutex = _mutex;
    this->_condition_variable = _condition_variable;
    this->platform = platform;
    this->delta_time = delta_time;
}

/**
 * This function is used to handle the movement pattern of the movingPlatform.
 */
void PlatformThread::handleMovement() {
    platform->getMove()->movement(delta_time);
}