/**
 * @file GlobalTime.cpp
 * @author Daniel Avisse (djavisse)
 * This class is the GlobalTime class. This class extends from the Timeline class
 * and overides some functions from it. This class allows for the creaton of a global Timeline
 * which can use chrono to create a timeline represenation.
 */
#include "GlobalTime.h"


/**
 * Constructer for GlobalTime that calls the Timeline constructor
 * @param anchor Reference to another timeline object
 * @param tic The step size used to determine the speed of the timeline
 */
GlobalTime::GlobalTime(Timeline *anchor, float tic) : Timeline(anchor, tic) {
    
}

/**
 * This is the getTime function. This function is overridden from the Timeline class.
 * This function uses chrono to return the time if the GlobalTime is not anchored to any
 * other Timeline. If the GlobalTime is anchored to another Timeline then it just uses the
 * anchor to get the time.
 * @return float The current time on the Timeline
 */
float GlobalTime::getTime()
{
    //If not anchored then use chrono to return time
    if (getAnchor() == NULL) {
        if (!getPauseStatus()) {
            float currentRealTime = std::chrono::duration<float>(std::chrono::steady_clock::now().time_since_epoch()).count();
            float timeLineTime = currentRealTime - getStartTime();
            return (timeLineTime - getElapsedPauseTime()) / getTicSize();
        }
        return getLastPausedTime();   
    }
    //If anchored then use the time from the anchor
    float currentArchorTime = getAnchor()->getTime();
    float localTime = currentArchorTime - getStartTime();
    return localTime / getTicSize();
}

/**
 * This is the pause function. This function will use the setPauseStatus() function
 * to pause the Timeline.
 */
void GlobalTime::pause()
{
    setPauseStatus();
}

/**
 * This is the unPause() function. This function will use the setPauseStatus() function
 * to pause the Timeline and then set the elasped pause time.
 */
void GlobalTime::unPause() {
    setPauseStatus();
    setElaspedPauseTime();
}