/**
 * @file Health.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the health componet. This
 * contains all the functions and fields need to create
 * a health component for the character.
 */
#ifndef HEALTH_H
#define HEALTH_H

#include <utility>
#include <string>
#include <v8.h>

class Health 
{
private:
    int lives;
    int healthPoints;
    int defaultHP;
    bool invincibility;
    bool gameOver;
    void *parentCharacterObject;
    static int current_guid;
    /////////V8 Stuff////////
    v8::Isolate* isolate;
    v8::Global<v8::Context>* context;
    static void setLivesV8(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
    static void getLivesV8(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setHealthGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
    static void getHealthGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    /////////////////////////
public:
    Health(int lives, int hp, void *parentCharacterObject);
    void setLives(int lives);
    int getLives();
    void setGameOver();
    bool getGameOver();
    void resetHealth();
    void setHealthPoints(int hp);
    int getHealthPoints();
    void takeDamage(int damage);
    void instantKO();
    void setInvincibility();
    bool getInvincibility();
    std::string guid;
    /////////V8 Stuff////////
    v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");
};


#endif