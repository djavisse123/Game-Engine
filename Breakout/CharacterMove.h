/**
 * @file CharacterMove.h
 * @author Daniel Avisse (djavisse)
 * This is the CharacterMove header file. This file
 * contains all the functions needed for this component.
 */
#ifndef CHARACTERMOVE_H
#define CHARACTERMOVE_H

#include "Move.h"
#define GRAVITY 25
#define JUMPVELOCITY -600

class CharacterMove : public Move
{
public: 
    CharacterMove(void *parentObject);
    void movement(float deltaTime);
};


#endif