/**
 * @file Event.h
 * @author Daniel Avisse (djavisse)
 * This is the Event header file. This file holds a represenation of
 * an event. Events can come with a certain type and can hold a certain
 * amount of data so a handler can use this information to handle the event. 
 * There a lot of events that can be represented, a few include spirte collision, 
 * user input, death collision, etc.
 */
#ifndef EVENT_H
#define EVENT_H

#include <utility>
#include <v8.h>

class Event
{
private:
    static int current_guid;
    /////////V8 Stuff////////
    v8::Isolate* isolate;
    v8::Global<v8::Context>* context;
    static void setEventType(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
    static void getEventType(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setEventGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
    static void getEventGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    /////////////////////////
public:
    Event();
    ~Event();
    float timeStamp;
    std::string guid;
    //Struct that holds information relating to the player
    struct PlayerInformation {
        float posX;
        float posY;
        float velocityX;
        float velocityY;
        int characterNumber;
    };
    //Struct that holds information relating to the side boundaries
    struct SideBoundaryCollsion{
        float boundaryPosX;
        float boundaryPosY;
        float viewCenterX;
        float viewCenterY;
    };
    //Struct that holds information relating to the server
    struct ServerInfo {
        int clientID;
        int clientCount;
        int disconnectedClients;
    };
    //Enum representing all possible event types
    enum EventType
    {
        TOP_COLLISION, BOTTOM_COLLISION, LEFT_COLLISION, RIGHT_COLLISION, SPRITE_COLLISION, DEATH_COLLISION, SPAWN_EVENT,
        KEYBOARD_MOVE_INPUT, SERVER_CONNECTION, SERVER_DISCONNECTION, REST_BOUNDARY, CHEATS_ACTIVE, CHEAT_DISABLE, 
        DECREASE_SPEED_SCRIPT, INCREASE_SPEED_SCRIPT, RESET_SPEED_SCRIPT, INCREASE_LIVES_SCRIPT, RESET_LIVES_SCRIPT, RESET_ALL, LAUNCH_BALL
    };
    //Union to allocate space for only one struct needed.
    union
    {
        PlayerInformation player;
        SideBoundaryCollsion sideBoundary;
        ServerInfo server;
    };
    //Override compare function so events can be sorted based on which ones occured the earliest.
    struct timeStampCompare {
        bool operator()(const Event& e1, const Event& e2) {
            return e1.timeStamp > e2.timeStamp;
        }
    };
    EventType type;

    /////////V8 Stuff////////
    v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context);
    static void scriptedEventFactory(const v8::FunctionCallbackInfo<v8::Value>& args);
    /////////////////////////
    static std::vector<Event *> listOfEvents;
};

#endif