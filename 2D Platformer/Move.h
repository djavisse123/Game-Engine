/**
 * @file Move.h
 * @author Daniel Avisse (djavisse)
 * This the Move header file for the Move component.
 * This file contains all the functions that a 
 * Move component can use.
 */
#ifndef MOVE_H
#define MOVE_H

class Move
{
protected:
    void *parentObject;
public: 
    Move(void *parentObject);
    void virtual movement(float deltaTime);
};


#endif