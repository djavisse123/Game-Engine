/**
 * @file PlatformMove.h
 * @author Daniel Avisse (djavisse)
 * This is the PlatformMove header file. This file
 * contains all the functions needed for this component.
 */
#ifndef PLATFORMMOVE_H
#define PLATFORMMOVE_H

#include "Move.h"

class PlatformMove : public Move
{
public: 
    PlatformMove(void *parentObject);
    void movement(float deltaTime);
};


#endif