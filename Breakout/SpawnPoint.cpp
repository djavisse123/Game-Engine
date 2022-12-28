/**
 * @file SpawnPoint.cpp
 * @author Daniel Avisse (djavisse)
 * This is the SpawnPoint class and it is also a GameObject that is
 * a part of our GameObject Model. This class creates a GameObject that 
 * represents the location that a player can Spawn from. All players would spawn
 * from the inital SpawnPoint and other SpawnPoints could be used to act as checkpoints.
 * When a player dies, the player should spawn via the SpawnPoint.
 */
#include "SpawnPoint.h"

/**
 * This constructs a SpawnPoint object
 * @param posX The x coordinate of the position for the SpawnPoint
 * @param posY The Y coordinate of the position for the SpawnPoint
 * @param spawnOrder The order in which the SpawnPoint appear in
 * @param viewCenter The center of the view 
 */
SpawnPoint::SpawnPoint(int posX, int posY, int spawnOrder, sf::Vector2f viewCenter) {
    this->setSize(sf::Vector2f(10.f, 10.f));
    //this->setFillColor(sf::Color(0, 0, 0));
    this->setPosition(posX, posY);
    this->spawnOrder = spawnOrder;
    this->viewCenter = viewCenter;
}

/**
 * Gets the position of the SpawnPoint for players to spawn
 * @return sf::Vector2f The postion for players to spawn
 */
sf::Vector2f SpawnPoint::getSpawn() {
    return this->getPosition();
}

/**
 * Sets the position of the SpawnPoint for players to spawn
 * @param posX The x coordiante of the spawn
 * @param posY The y cooridnate of the spawn
 */
void SpawnPoint::setSpawn(int posX, int posY) {
    this->setPosition(posX, posY);
}

/**
 * Sets the order for which the SpawnPoint should appear
 * @param spawnOrder The order the SpawnPoint would appear
 */
void SpawnPoint::setSpawnOrder(int spawnOrder) {
    this->spawnOrder = spawnOrder;
}

/**
 * Gets the order for which the SpawnPoint appears in 
 * @return int 
 */
int SpawnPoint::getSpawnOrder() {
    return this->spawnOrder;
}

/**
 * Sets the center of the view for the SpawnPoint
 * @param viewCenter The center of the view
 */
void SpawnPoint::setViewCenter(sf::Vector2f viewCenter) {
    this->viewCenter = viewCenter;
}

/**
 * Gets the center of the view for the SpawnPoint
 * @return sf::Vector2f The center of the view
 */
sf::Vector2f SpawnPoint::getViewCenter() {
    return this->viewCenter;
}