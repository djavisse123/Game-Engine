/**
 * @file DeathZone.h
 * @author Daniel Avisse (djavisse)
 * This is the DeathZone header file. This file
 * contains the constructor needed to create a DeathZone.
 */
#ifndef DEATHZONE_H
#define DEATHZONE_H

#include "GameObject.h"

class DeathZone : public GameObject
{
public:
    DeathZone(int posX, int posY, float width, float length);
};

#endif