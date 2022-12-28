/**
 * @file SpriteThread.cpp
 * @author Daniel Avisse (djavisse)
 * This is the SpriteThread class. This class creates a threaded object used
 * to handle sprite movements.
 */
#include "SpriteThread.h"

/**
 * Constructer for the SpriteThread class.
 * @param threadID Identifier for the thread
 * @param other A reference to another PlatformThread
 * @param _mutex A mutex lock used to allow for mutual exectuion
 * @param _condition_variable A condition variable used to message other threads
 * @param sprite The movingPlatform being used by the PlatformThread
 * @param delta_time The amount of time between frames
 */
SpriteThread::SpriteThread(int threadID, SpriteThread* other, std::mutex* _mutex, std::condition_variable* _condition_variable, Character* sprite, float delta_time)
{
    this->threadID = threadID; // set the id of this thread
    this->other = other;
    this->_mutex = _mutex;
    this->_condition_variable = _condition_variable;
    this->sprite = sprite;
    this->delta_time = delta_time;
}

/**
 * This function is used to handle the movement for the SpriteThread
 */
void SpriteThread::handleMovement()
{
    sprite->getMove()->movement(delta_time);
}