/**
 * @file Character.cpp
 * @author Daniel Avisse (djavisse)
 * This is the Character class. This creates a simple rectangle
 * that can be used with a texture to create a character.
 */
#include "Character.h"
#include "v8helpers.h"
#include "ScriptManager.h"


#include <filesystem>
#include <cstring>
#include <iostream>

/**
 * Constructs a Character object
 * @param width The width of the Character
 * @param length The length of the Character
 * @param charNumber The number for the Character
 * @param charName The name of the Character
 */
Character::Character(float width, float length, int charNumber, std::string charName) {
    this->setSize(sf::Vector2f(width, length));
    this->characterNumber = charNumber;
    this->characterName = characterName;
    this->healthComponet = new Health(DEFAULT_NUMBER_OF_LIVES, MAX_HP, this);
    this->lastSpawnPoint = NULL;
    this->moveComponet = new CharacterMove(this);
    this->collideComponent = new BallCollision(this);
    this->handleEvents = new CharacterHandler(this);
    this->scriptHandler = new ScriptHandler(this);
    this->characterVelocity = sf::Vector2f(0, 0);
}

/** 
 * This function just returns the character's movement values. This value
 * determines how fast the character can move.
 * @return int The value of the character's movement
 */
int Character::getCharacterMovement()
{
    return characterMovement;
}

/**
 * This function sets the name of the character. The name can be used to
 * identify the character.
 * @param name The name of the character
 */
void Character::setCharacterName(std::string name) {
    this->characterName = name;
}

/**
 * This function returns the name of the character. The name can be used to
 * identify the character.
 * @return std::string The name of the character
 */
std::string Character::getCharacterName() {
    return characterName;
}

/**
 * This function sets the number of the character. The number can be used
 * to identify the character.
 * @param num 
 */
void Character::setCharacterNumber(int num) {
    this->characterNumber = num;
}

/**
 * This function returns the character's number. The number can be used
 * to indentify the character.
 * @return int 
 */
int Character::getCharacterNumber() {
    return characterNumber;
}

/**
 * Gets the Health component of the Character to return information
 * relating to the Character like number of lives
 * @return Health* The Health component of the Character
 */
Health * Character::getHealth() {
    return this->healthComponet;
}

/**
 * Sets the status of the Character being on the ground
 */
void Character::setOnGround() {
    onGround = true;
    canJump = true;
    //this->setCharacterVelocity(sf::Vector2f(characterVelocity.x, 0));
}

/** 
 * Sets the status of the Character being off the ground
 */
void Character::setOffGround() {
    onGround = false;
    canJump = false;
}

/**
 * Gets the the status of the Character being on the ground
 * @return true The Character is on the ground
 * @return false The Character is not on the ground
 */
bool Character::getOnGround() {
    return onGround;
}

/**
 * Gets the status of the Character being able to jump
 * @return true The Character can jump
 * @return false The Character can't jump
 */
bool Character::getCanJump() {
    return canJump;
}

/**
 * Sets the SpawnPoint of the Character
 * @param spawn The Character's SpawnPoint
 */
void Character::setSpawnPoint(SpawnPoint * spawn) {
    this->lastSpawnPoint = spawn;
}

/** 
 * Gets the SpawnPoint of the Character
 * @return SpawnPoint* The Character's SpawnPoint
 */
SpawnPoint  * Character::getSpawnPoint() {
    return this->lastSpawnPoint;
}

/** 
 * Sets the Character's velocity
 * @param velocity The velocity of the Character
 */
void Character::setCharacterVelocity(sf::Vector2f velocity) {
    this->characterVelocity = velocity;
}

/**
 * Gets the Character's velocity
 * @return sf::Vector2f The velocity of the Character
 */
sf::Vector2f Character::getCharacterVelocity() {
    return this->characterVelocity;
}

/**
 * Gets the EventHandler for the Character that handles events specific for the Character
 * @return CharacterHandler* The EventHandler for the Character
 */
CharacterHandler * Character::getEventHandler() {
    return this->handleEvents;
}

/**
 * Gets the EventHandler for the Character that handles events relating to scripts that modify the character
 * @return ScriptHandler* The EventHandler for the Character that handles scripts
 */
ScriptHandler * Character::getScriptHandler() {
    return this->scriptHandler;
}

/**
 * Sets the cheat status
 * @param cheat The cheat status of the character
 */
void Character::setCheatStatus(bool cheat) {
    this->cheatActive = cheat;
}

/**
 * Gets the cheat status
 * @return true The character has cheats active
 * @return false The character doesn't have cheats active
 */
bool Character::getCheatStatus() {
    return this->cheatActive;
}

/**
 * Increase the Character's/player's score based on the amount given
 * @param scoreAmount The amount of points that will increase the total score
 */
void Character::increasePlayerScore(int scoreAmount) {
    this->playerScore += scoreAmount;
}

/**
 * Gets the total score for the Character/player
 * @return int The score for the Character/player
 */
int Character::getPlayerScore() {
    return this->playerScore;
}

/**
 * Sets the level the Character/player is on
 * @param level The level the Character/player will be set to
 */
void Character::setCurrentLevel(int level) {
    this->currentLevel = level;
}

/**
 * Gets the current level the Character/player is on
 * @return int The current level the Character/Player is on
 */
int Character::getCurrentLevel() {
    return this->currentLevel;
}

/**
 * This function is used to expose the Character to v8.
 * Certain fields for the object are also exposed so they can be
 * accessed and modfied.
 * @param isolate The isolate used to enter the V8 vitrual machine
 * @param context The context the object will be in
 * @param context_name The name of the context
 * @return v8::Local<v8::Object> The v8 local object
 */
v8::Local<v8::Object> Character::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
    std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("characterMovement", getCharacterMovementV8, setCharacterMovementV8));
    return v8helpers::exposeToV8(this->characterName, this, v, isolate, context, context_name);
}

/**
 * Allows for a script in V8 to retrieve the character's speed
 */
void Character::getCharacterMovementV8(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    int CharMovement = static_cast<Character*>(ptr)->characterMovement;
    info.GetReturnValue().Set(CharMovement);
}

/**
 * Allows for a script in V8 to modify the character's speed
 */
void Character::setCharacterMovementV8(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<Character*>(ptr)->characterMovement = value->Int32Value();
}

