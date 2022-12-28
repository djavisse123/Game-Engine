/**
 * @file MovePattern.h
 * @author Daniel Avisse (djavisse)
 * This is the MovePattern header file for the MovePattern.
 * This file contains all the functions and fields need for this
 * component.
 */
#ifndef MOVEPATTERN_H
#define MOVEPATTERN_H

#include "GlobalTime.h"
#include "Move.h"

class MovePattern : public Move
{
protected:
    //void *parentObject;
    bool moveLeft;
    bool moveRight;
    bool moveUp;
    bool moveDown;
    float waitTime;
    int moveDistance;
public: 
    MovePattern(void *parentObject, float waitTime, int moveDistance);
    void changeMoveDistance(int moveDistance);
    int getMoveDistance();
    void changeWaitTime(int waitTime);
    int getWaitTime();
    void movePatternVertical(bool reverse);
    void movePatternHorizontal(bool reverse);
};


#endif