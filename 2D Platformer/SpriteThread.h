/**
 * @file SpriteThread.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the SpriteThread class. This file contains
 * all the fields and functions needed for the SpriteThread.
 */
#ifndef SPRITETHREAD_H
#define SPRITETHREAD_H

#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "Character.h"

class SpriteThread
{
    int threadID; // an identifier
    SpriteThread* other; // a reference to the "other" thread in this example
    std::mutex* _mutex; // the object for mutual exclusion of execution
    std::condition_variable* _condition_variable; // for messaging between threads
    Character* sprite;
    float delta_time;

public:
    SpriteThread(int threadID, SpriteThread* other, std::mutex* _mutex, std::condition_variable* _condition_variable, Character* sprite, float delta_time);
    void handleMovement();
};

#endif

