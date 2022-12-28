/**
 * @file GameObject.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the GameObject class. This file
 * contains the constructor and components needed for a GameObject.
 */
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Collide.h"
#include "CollideDeathZone.h"
#include "CollideSideBoundary.h"
#include "Move.h"
#include "MovePattern.h"
#include "EventHandler.h"
#include "CollisionHandler.h"
#include "CharacterHandler.h"
#include "EventManager.h"

class GameObject : public sf::RectangleShape 
{
protected:
    Collide *collideComponent;
    Move *moveComponet;
public:
    GameObject();
    Collide * getCollide();
    Move * getMove();
};


#endif