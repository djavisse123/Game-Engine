/**
 * @file Timeline.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the Timeline class. This file contains
 * all the fields and methods needed for the Timeline. Some functions
 * are virtual methods where they can be override by another class 
 * extending Timeline.
 */
#ifndef TIMELINE_H
#define TIMELINE_H

#include <cstddef>
#include <chrono>

class Timeline
{
private:
    float start_time;
    float tic;
    bool paused;
    float last_paused_time;
    float elapsed_pause_time;
    Timeline *anchor;

public:
    Timeline(Timeline *anchor, float tic);
    float virtual getTime() = 0;
    float getStartTime();
    bool getPauseStatus();
    void setPauseStatus();
    void virtual pause() = 0;
    float getElapsedPauseTime();
    void setElaspedPauseTime();
    float getLastPausedTime();
    Timeline * getAnchor();
    void virtual unPause() = 0;
    float getTicSize();
    void virtual changeTic(float newTic);
};

#endif