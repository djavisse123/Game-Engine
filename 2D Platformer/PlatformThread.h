/**
 * @file PlatformThread.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the PlatformThread class. This file contains
 * all the fields and functions needed for the PlatformThread.
 */
#ifndef PLATFORMTHREAD_H
#define PLATFORMTHREAD_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include "MovingPlatform.h"

class PlatformThread 
{
    int threadID;
    PlatformThread *other;
    std::mutex *_mutex;
    std::condition_variable *_condition_variable;
    MovingPlatform *platform;
    float targetTime = 1.0 / 60.0;
public:
    PlatformThread(int threadID, PlatformThread *other, std::mutex *_mutex, std::condition_variable *_condition_variable, MovingPlatform *platform);
    void handleMovement();
};

#endif