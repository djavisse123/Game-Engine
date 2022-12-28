/**
 * @file ScriptHandler.cpp
 * @author Daniel Avisse (djavisse)
 * This is the ScriptHandler class. This event handler handles any events relating to scripts.
 * An event raised with any of the types this handler uses will cause a script in javascript to
 * run using the v8 platform.
 */
#include "ScriptHandler.h"
#include "ScriptManager.h"
#include "v8helpers.h"
#include "Character.h"

/**
 * Constructs a ScriptHandler component using a super to the EventHandler
 * @param parentObject A reference to the object using the ScriptHandler
 */
ScriptHandler::ScriptHandler(void * parentObject) : EventHandler(parentObject) {

}

void ScriptHandler::onEvent(Event e) {
    switch(e.type) {
        case Event::INCREASE_SPEED_SCRIPT:
        {
            Character *spriteObj = (Character*) parentObject; 
                {
                    v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
                    v8::HandleScope handle_scope(isolate);
                    //Create the global and add functions that can be used in V8
                    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
                    global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
                    global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
                    global->Set(isolate, "getObjectName", v8::FunctionTemplate::New(isolate, v8helpers::getObjectV8));
                    //Create a context for the manager
                    v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
                    //Create a script manager to add the contexts and the scripts
                    ScriptManager *sm = new ScriptManager(isolate, default_context);
                    bool reload = false;
                    //Add a context for the scripts to modify the sprite
                    v8::Local<v8::Context> sprite_context = v8::Context::New(isolate, NULL, global);
                    sm->addContext(isolate, sprite_context, "sprite_context");
                    //Add the change movement speed script to the manager
                    sm->addScript("change_movement_speed", "scripts/change_movement_speed.js", "sprite_context");
                    //Expose the sprite to V8 so it can be modified
                    spriteObj->exposeToV8(isolate, sprite_context, "sprite_context");
                    //Run the change movement speed script to modify the sprite's speed
                    sm->runOne("change_movement_speed", reload, "sprite_context");
                    //Delete the manager to clear the scripts and contexts
                    delete sm;
                }
            break;
        }
        case Event::RESET_SPEED_SCRIPT:
        {
            Character *spriteObj = (Character*) parentObject; 
            {
                v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
                v8::HandleScope handle_scope(isolate);
                //Create the global and add functions that can be used in V8
                v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
                global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
                global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
                global->Set(isolate, "getObjectName", v8::FunctionTemplate::New(isolate, v8helpers::getObjectV8));
                //Create a context for the manager
                v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
                //Create a script manager to add the contexts and the scripts
                ScriptManager *sm = new ScriptManager(isolate, default_context);
                bool reload = false;
                //Add a context for the scripts to modify the sprite
                v8::Local<v8::Context> sprite_context = v8::Context::New(isolate, NULL, global);
                sm->addContext(isolate, sprite_context, "sprite_context");
                //Add the reset movement speed script to the manager
                sm->addScript("reset_movement_speed", "scripts/reset_movement_speed.js", "sprite_context");
                //Expose the sprite to V8 so it can be modified
                spriteObj->exposeToV8(isolate, sprite_context, "sprite_context");
                //Run the reset movement speed script to reset the sprite's movement speed to it's default value
                sm->runOne("reset_movement_speed", reload, "sprite_context");
                //Delete the manager to clear the scripts and contexts
                delete sm;
            }
            break;
        }
        case Event::INCREASE_LIVES_SCRIPT:
        {
            Character *spriteObj = (Character*) parentObject;
            {
                v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
                v8::HandleScope handle_scope(isolate);
                //Create the global and add functions that can be used in V8
                v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
                global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
                global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
                global->Set(isolate, "getObjectName", v8::FunctionTemplate::New(isolate, v8helpers::getObjectV8));
                //Create a context for the manager
                v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
                //Create a script manager to add the contexts and the scripts
                ScriptManager *sm = new ScriptManager(isolate, default_context);
                bool reload = false;
                //Add a context for the health componet to modify the health componet
                v8::Local<v8::Context> health_context = v8::Context::New(isolate, NULL, global);
                sm->addContext(isolate, health_context, "health_context");
                //Add the increse lives script to the manager
                sm->addScript("increase_lives", "scripts/increase_lives.js", "health_context");
                //Expose the sprite's health componet to V8 so it can be modified
                spriteObj->getHealth()->exposeToV8(isolate, health_context, "health_context");
                //Run the increase lives script to increase the number of lives for the sprite
                sm->runOne("increase_lives", reload, "health_context");
                //Delete the manager to clear the scripts and contexts
                delete sm;
            }
            break;
        }
        case Event::RESET_LIVES_SCRIPT:
        {
            Character *spriteObj = (Character*) parentObject;
            {
                v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
                v8::HandleScope handle_scope(isolate);
                v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
                //Create the global and add functions that can be used in V8
                global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
                global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
                global->Set(isolate, "getObjectName", v8::FunctionTemplate::New(isolate, v8helpers::getObjectV8));
                //Create a context for the manager
                v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
                //Create a script manager to add the contexts and the scripts
                ScriptManager *sm = new ScriptManager(isolate, default_context);
                bool reload = false;
                //Add a context for the health componet to modify the health componet
                v8::Local<v8::Context> health_context = v8::Context::New(isolate, NULL, global);
                sm->addContext(isolate, health_context, "health_context");
                //Add the reset lives script to the manager
                sm->addScript("reset_lives", "scripts/reset_lives.js", "health_context");
                //Expose the sprite's health componet to V8 so it can be modified
                spriteObj->getHealth()->exposeToV8(isolate, health_context, "health_context");
                //Run the reset lives script to reset the sprite's lives to it's default value
                sm->runOne("reset_lives", reload, "health_context");
                //Delete the manager to clear the scripts and contexts
                delete sm;
            }
            break;
        }
        case Event::RESET_ALL:
        {
            Character *spriteObj = (Character*) parentObject;
            {
                v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
                v8::HandleScope handle_scope(isolate);
                //Create the global and add functions that can be used in V8
                v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
                global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
                global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
                global->Set(isolate, "getObjectName", v8::FunctionTemplate::New(isolate, v8helpers::getObjectV8));
                //Create a context for the manager
                v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
                //Create a script manager to add the contexts and the scripts
                ScriptManager *sm = new ScriptManager(isolate, default_context);
                bool reload = false;
                //Add the contexts to modify the objects
                v8::Local<v8::Context> health_context = v8::Context::New(isolate, NULL, global);
                v8::Local<v8::Context> sprite_context = v8::Context::New(isolate, NULL, global);
                sm->addContext(isolate, health_context, "health_context");
                sm->addContext(isolate, sprite_context, "sprite_context");
                //Add all the scritps that reset values
                sm->addScript("reset_lives", "scripts/reset_lives.js", "health_context");
                sm->addScript("reset_movement_speed", "scripts/reset_movement_speed.js", "sprite_context");
                //Expose all objects
                spriteObj->getHealth()->exposeToV8(isolate, health_context, "health_context");
                spriteObj->exposeToV8(isolate, sprite_context, "sprite_context");
                //Run the scripts to reset all values
                sm->runOne("reset_lives", reload, "health_context");
                sm->runOne("reset_movement_speed", reload, "sprite_context");
                //Delete the manager to clear the scripts and contexts
                delete sm;
            }
            break;
        }
    }
}

/**
 * Sets the isolate for this handler
 * @param isolate The isolate that will be used to enter the V8 vitural machine
 */
void ScriptHandler::setIsolate(v8::Isolate *isolate) {
    this->isolate = isolate;
}

