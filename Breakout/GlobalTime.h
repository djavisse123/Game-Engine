/**
 * @file GlobalTime.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the GlobalTime class. This file contains
 * all the functions needed for the GlobalTime class.
 */
#ifndef GLOBALTIME_H
#define GLOBALTIME_H

#include "Timeline.h"

class GlobalTime : public Timeline
{
public:
    GlobalTime(Timeline *anchor, float tic);
    float getTime();
    void pause();
    void unPause();
};

#endif
