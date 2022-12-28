/**
 * @file Collide.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the Collide class. This file proivdes
 * the fields and functions need for that class.
 */
#ifndef COLLIDE_H
#define COLLIDE_H


class Collide 
{
protected:
    //Reference to the object that is using the collide component
    void *parentObject;
public:
    Collide(void *parentobject);
    virtual bool checkCollision(void *object);
};


#endif