/**
 * @file Health.cpp
 * @author Daniel Avisse (djavisse)
 * This is the Health component. This component is specifically used 
 * for the Character object. This component is used to maintain all
 * information relating to the Health of the Character and is responsible
 * for maintaing the Character's health when they get hit and die and should
 * also send the Character back to their spawn when they die.
 */
#include "Health.h"
#include "Character.h"
#include <iostream>
#include "v8helpers.h"
#include <filesystem>
#include <cstring>
#include "ScriptManager.h"

int Health::current_guid = 0;
/**
 * Constructs a Health component
 * @param lives The number of lives the Character has
 * @param hp The total amount of health points the Character has
 * @param parentCharacterObject A reference to the object using the Health component
 */
Health::Health(int lives, int hp, void *parentCharacterObject) {
    this->lives = lives;
    this->healthPoints = hp;
    this->defaultHP = hp;
    this->gameOver = false;
    this->invincibility = false;
    this->parentCharacterObject = parentCharacterObject;
    this->guid = "Health" + std::to_string(current_guid);
    current_guid++;
}

/**
 * Sets the number of lives the Character has
 * @param lives Number of lives for the Character
 */
void Health::setLives(int lives) {
    this->lives = lives;
}

/**
 * Gets the current number of lives the Character has
 * @return int Number of lives
 */
int Health::getLives() {
    return this->lives;
}

/**
 * Sets the status of when the Character has lost all their lives
 * and the game is over
 */
void Health::setGameOver() {
    if (this->gameOver) {
        this->gameOver = false;
    }
    else {
        this->gameOver = true;
    }
}

/**
 * Gets the status of the game being over
 * @return true The Character has lost all their lives and the game is over
 * @return false The Character still has some lives and the game is continuing
 */
bool Health::getGameOver() {
    return this->gameOver;
}

/**
 * Resets the Character's HP to their max once they have died.
 */
void Health::resetHealth() {
    this->healthPoints = defaultHP;
}

/**
 * Sets the number of health points the Character has
 * @param hp The number of health points for the Character
 */
void Health::setHealthPoints(int hp) {
    this->healthPoints = hp;
}

/**
 * Gets the current number of health points the Character has
 * @return int The number health points
 */
int Health::getHealthPoints() {
    return this->healthPoints;
}

/**
 * This function reduces the Character's health points by the number
 * of damage they have taken. If a Character is invincible then they won't take
 * any damage. If a Character's HP drops to 0 or is less than 0 then they have died
 * and will lose a life an respawn.
 * @param damage 
 */
void Health::takeDamage(int damage) {
    if(!this->invincibility) {
        this->healthPoints -= damage;
        if(this->healthPoints <= 0) {
            this->lives--;  
            Character *parObj = static_cast<Character *>(this->parentCharacterObject);
            parObj->setPosition(parObj->getSpawnPoint()->getSpawn());
            if(this->lives < 0) {
                setGameOver();
            }
            resetHealth(); 
        }
    }
}

/**
 * This function instantly kills the Character and makes the lose all HP and
 * lose a life. The Character will still die even if they are invincible. This function
 * is only used for special events that require the Character to die instantly like
 * the DeathZone.
 */
void Health::instantKO() {
    this->healthPoints = 0;
    this->lives--;
    Character *parObj = static_cast<Character *>(this->parentCharacterObject);
    parObj->setPosition(parObj->getSpawnPoint()->getSpawn());
    if(this->lives < 0) {
        setGameOver();
    }
    resetHealth();
}
 
/**
 * Sets the invincibility status of the Character
 */
void Health::setInvincibility() {
    if(this->invincibility) {
        this->invincibility = false;
    }
    else {
        this->invincibility = true;
    }
}

/**
 * Gets the invincibility status of the Character
 * @return true The Character is invincible
 * @return false The Character is not invincible
 */
bool Health::getInvincibility() {
    return this->invincibility;
}

/**
 * This function is used to expose the Health component to v8.
 * Certain fields for the object are also exposed so they can be
 * accessed and modfied.
 * @param isolate The isolate used to enter the V8 vitrual machine
 * @param context The context the object will be in
 * @param context_name The name of the context
 * @return v8::Local<v8::Object> The v8 local object
 */
v8::Local<v8::Object> Health::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
	std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
	v.push_back(v8helpers::ParamContainer("lives", getLivesV8, setLivesV8));
    v.push_back(v8helpers::ParamContainer("guid", getHealthGUID, setHealthGUID));
	return v8helpers::exposeToV8(guid, this, v, isolate, context, context_name);
}

/**
 * Allows for a script in V8 to modify the number of lives the character has
 */
void Health::setLivesV8(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<Health*>(ptr)->lives = value->Int32Value();
}

/**
 * Allows for a script in V8 to retrieve the number of lives the character has
 */
void Health::getLivesV8(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    int num_lives = static_cast<Health*>(ptr)->lives;
    info.GetReturnValue().Set(num_lives);
}

/**
 * Allows for a script in V8 to modify the health component's guid
 */
void Health::setHealthGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
	static_cast<Health*>(ptr)->guid = *utf8_str;
}

/**
 * Allows for a script in V8 to retrieve the health componet's guid
 */
void Health::getHealthGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	std::string guid = static_cast<Health*>(ptr)->guid;
	v8::Local<v8::String> v8_guid = v8::String::NewFromUtf8(info.GetIsolate(), guid.c_str(), v8::String::kNormalString);
	info.GetReturnValue().Set(v8_guid);
}
