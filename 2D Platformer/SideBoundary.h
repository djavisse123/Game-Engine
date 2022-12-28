/**
 * @file SideBoundary.h
 * @author Daniel Avisse (djavisse)
 * This is the SideBoundary header file. This file
 * contains all the functions and fields needed to create a SideBoundary.
 */
#ifndef SIDEBOUNDARY_H
#define SIDEBOUNDARY_H

#include "GameObject.h"

class SideBoundary : public GameObject 
{
protected:
    sf::View *view;
    sf::Vector2i startingPos;
    CollisionHandler * handleCollison;
public:
    SideBoundary(int posX, int posY, float width, float length, sf::View *view);
    sf::View * getView();
    void setStartingPos(int posX, int posY);
    sf::Vector2i getStartingPos();
    CollisionHandler * getCollisionHandler();
};


#endif
