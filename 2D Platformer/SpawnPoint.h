/**
 * @file SpawnPoint.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the SpawnPoint. This file
 * contains all the fields and functions need for the SpawnPoint.
 */
#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include "GameObject.h"

class SpawnPoint : public GameObject
{
private:
    int spawnOrder;
    sf::Vector2f viewCenter;
public:
    SpawnPoint(int posX, int posY, int spawnOrder, sf::Vector2f viewCenter);
    sf::Vector2f getSpawn();
    void setSpawn(int posX, int posY);
    void setSpawnOrder(int spawnOrdr);
    int getSpawnOrder();
    void setViewCenter(sf::Vector2f viewCenter);
    sf::Vector2f getViewCenter();
};


#endif