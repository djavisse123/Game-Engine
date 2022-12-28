/**
 * @file Timeline.cpp
 * @author Daniel Avisse (djavisse)
 * This is the Timeline class. This class acts sorta like an abstract class
 * where some functions can be overridden by a child class. This class
 * allows for the creation of a represenation of a timeline by basing it of 
 * of real time using chrono.
 */
#include "Timeline.h"

/**
 * Constructer for Timeline class
 * @param anchor Reference to another timeline object
 * @param tic The step size used to determine the speed of the timeline
 */
Timeline::Timeline(Timeline *anchor, float tic)
{
    this->anchor = anchor;
    this->tic = tic;
    if (anchor == NULL)
    {
        this->start_time = std::chrono::duration<float>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }
    else
    {
        this->start_time = anchor->getTime();
    }
    this->paused = false;
    this->last_paused_time = 0;
    this->elapsed_pause_time = 0;
}

/**
 * This function returns the start time of the Timeline
 * @return float 
 */
float Timeline::getStartTime()
{
    return start_time;
}

/**
 * This function returns the the pause status of the Timeline
 * @return true if the Timeline is paused
 * @return false if the Timeline isn't paused
 */
bool Timeline::getPauseStatus()
{
    return paused;
}

/** 
 * This sets the pause status of the Timeline to either true for paused
 * or false for not paused
 */
void Timeline::setPauseStatus()
{
    if (!paused)
    {
        last_paused_time = getTime();
        paused = true;    
    }
    else
    {
        paused = false;
        //elapsed_pause_time = 0;
    }
}

/**
 * This function returns the elapsed pause time for the Timeline
 * @return float The amount time the Timeline has been paused
 */
float Timeline::getElapsedPauseTime() {
    return elapsed_pause_time;
}

/**
 * This function returns the time the pause function was used last on the Timeline
 * @return float The time the pause was last used
 */
float Timeline::getLastPausedTime() {
    return last_paused_time;
}

/**
 * This function sets the total amount of elasped pause time
 */
void Timeline::setElaspedPauseTime() {
    elapsed_pause_time += getTime() - last_paused_time;
}

/**
 * This function returns the current tic size of the Timeline
 * @return float The tic size of the Timeline
 */
float Timeline::getTicSize()
{
    return tic;
}

/**
 * This function returns the Timeline object that the current Timeline is anchored to.
 * @return Timeline* The Timeline object the current Timeline is anchored to
 */
Timeline * Timeline:: getAnchor() {
    return anchor;
} 

/**
 * This function allows for the current tic size of the Timeline to be changed
 * @param newTic The new tic size to replace the old tic size.
 */
void Timeline::changeTic(float newTic) {
    this->tic = newTic;
}
