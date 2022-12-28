/**
 * @file Character.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the character class. This file contains all the fields
 * and functions needed for the character class.
 */
#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameObject.h"
#include "Health.h"
#include "SpawnPoint.h"
#include "CharacterMove.h"
#include "ScriptHandler.h"
#include <v8.h>


#include <utility>

#define DEFAULT_NUMBER_OF_LIVES 3
#define MAX_HP 100

class Character : public GameObject
{
private: 
    

    ///////////V8 STUFF//////////////
    v8::Isolate* isolate;
    v8::Global<v8::Context>* context;
    static void setCharacterMovementV8(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
    static void getCharacterMovementV8(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type
    ////////////////////////////////

    std::string characterName = "";
    int characterMovement = 150;
    int characterNumber;
    Health *healthComponet;
    bool canJump = false;
    bool onGround = false;
    SpawnPoint *lastSpawnPoint;
    sf::Vector2f characterVelocity;
    CharacterHandler  * handleEvents;
    ScriptHandler * scriptHandler;
    bool cheatActive = false;

public:
    Character(float width, float length, int charNumber, std::string charName);
    int getCharacterMovement();
    void setCharacterName(std::string name);
    std::string getCharacterName();
    void setCharacterNumber(int num);
    int getCharacterNumber();
    void setOnGround();
    void setOffGround();
    bool getOnGround();
    bool getCanJump();
    Health * getHealth();
    void setSpawnPoint(SpawnPoint *spawn);
    SpawnPoint * getSpawnPoint();
    void setCharacterVelocity(sf::Vector2f velocity);
    sf::Vector2f getCharacterVelocity();
    CharacterHandler * getEventHandler();
    void setCheatStatus(bool cheat);
    bool getCheatStatus();
    /////////V8 Stuff////////
    v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");
    ScriptHandler * getScriptHandler();
    /////////////////////////
}; 

#endif