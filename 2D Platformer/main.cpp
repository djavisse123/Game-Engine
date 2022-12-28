/**
 * @file main.cpp
 * @author Daniel Avisse (djavisse)
 * This is the main.cpp file for HW4's game enigne environment. This is an 
 * extension of HW4's game engine environment. This version of the game engine
 * features the ablity to add scripts. Scripts are written in JavaScript and can
 * be used to modify game objects or create events.
 */
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <math.h>
#include <zmq.hpp>
#include <v8.h>
#include <libplatform/libplatform.h>
#include "nlohmann/json.hpp"

#include "Character.h"
#include "GlobalTime.h"
#include "MovingPlatform.h"
#include "PauseMenu.h"
#include "Platform.h"
#include "Timeline.h"
#include "SpriteThread.h"
#include "DeathZone.h"
#include "SpawnPoint.h"
#include "SideBoundary.h"
#include "HUD.h"
#include "v8helpers.h"
#include "ScriptManager.h"
#include "ScriptHandler.h"
#include "ObjectContainer.h"

/**
 * This fucntion is used to make the spriteThread run and handle movment for the sprite
 * The spriteThread will check for keyboard inputs from the user and then move the sprite
 * accordingly to that input.
 * @param spritethread The spritethread object used to handle sprite movement
 */
void run_Movement(SpriteThread *spritethread)
{
    spritethread->handleMovement();
}

/**
 * This is the main function the runs the entire game engine. This function
 * features a game loop that will keep the window open until the user decides to
 * close it. While the window is open, shapes are drawn onto the screen and the user
 * can move the character and that will be updated accordingly.
 * @return int Value returned when successful execution
 */
int main()
{
    //Creates a RenderWindow which allows for different objects and shapes to be drawn
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window", sf::Style::Default);
    //Sets the position for where the window opens
    window.setPosition(sf::Vector2i(10, 50));
    //Sets the Frame Rate to 60fps
    window.setFramerateLimit(60);
    window.setTitle("Kirby's Adventure");
    //Set the view for the window
    sf::View view;
    view = window.getDefaultView();
    view.setCenter(view.getCenter().x, 780.f);
    window.setView(view);

    ///ObjectContainer *obj = ObjectContainer::getInstance();
    
    //Create lists to hold some Game Objects that will be created once the client joins the server
    std::vector<Platform *> listOfPlatforms;
    std::vector<MovingPlatform *> listOfMovingPlatforms;
    std::vector<SideBoundary *> listOfSideBoundaries;
    //Load in textures for the background and multiplayer
    sf::Texture spriteTexture;
    sf::Texture blueSpriteTexture;
    sf::Texture yellowSpriteTexture;
    sf::Texture redSpriteTexture;
    sf::Texture backgroundTexture;
    spriteTexture.loadFromFile("Kirby.png");
    blueSpriteTexture.loadFromFile("Blue_Kirby.png");
    yellowSpriteTexture.loadFromFile("Yellow_Kirby.png");
    redSpriteTexture.loadFromFile("Red_Kirby.png");
    backgroundTexture.loadFromFile("space_background.png");

    //Create an array of all the textures to be used for multiplayer
    sf::Texture textures[] = { spriteTexture, yellowSpriteTexture, blueSpriteTexture, redSpriteTexture };

    //Load in background texture and setSize
    sf::Sprite background;
    background.setTexture(backgroundTexture);

    //Create a global timeline to last throughout the game
    GlobalTime globalTime(NULL, 1);
    float last_time = globalTime.getTime();
    GlobalTime localTime(&globalTime, 1);
    //Create a local timeline used for character velocity
    float last_local_time = localTime.getTime();
    //Time we want to target when running the game
    float targetTime = 1.0 / 60.0;

    ///////////////////////////////////////////////CLIENT STUFF//////////////////////////////////////////////////////////

    //Create a zmq context to be used to create the socket
    using json = nlohmann::json;
    zmq::context_t context{1};

    //Create a req socket to be used to send a message to the server
    zmq::socket_t communicationSocket{context, zmq::socket_type::req};
    communicationSocket.connect("tcp://localhost:5555");
    
    //Create another req socket to be used to send updates to the server
    zmq::socket_t updateSocket{ context, zmq::socket_type::req };
    updateSocket.connect("tcp://localhost:5556");

    //Send to the server that you are a new client connecting to the server
    json connect;
    connect["Event Type"] = Event::SERVER_CONNECTION;
    std::string connection_str = connect.dump();
    zmq::message_t sendNewConnection(connection_str.size());
    memcpy(sendNewConnection.data(), connection_str.c_str(), connection_str.size());
    //The auto res for all sends and recieves serve no function other than to remove the c++17 standard
    //warning that appears when you ignore a function that has a return value.
    auto res = communicationSocket.send(sendNewConnection, zmq::send_flags::none);

    // const std::string newClient{ "New Client" };
    // communicationSocket.send(zmq::buffer(newClient), zmq::send_flags::none);

    //Wait for reply from the server
    zmq::message_t reply;
    auto res2 = communicationSocket.recv(reply, zmq::recv_flags::none);

    //Get the information from the server and parse it into a json
    auto json_str = std::string(static_cast<char*>(reply.data()), reply.size());
    auto recv_json = json::parse(json_str);

    //Create a SpawnPoint for the player
    SpawnPoint playerSpawn(recv_json["SpawnPoint X Position"], recv_json["SpawnPoint Y Position"], recv_json["SpawnPoint SpawnOrder"], 
    sf::Vector2f(recv_json["SpawnPoint ViewCenter X"], recv_json["SpawnPoint ViewCenter Y"]));

    //Create a DeathZone where the player can collide with and die
    DeathZone deathzone(recv_json["DeathZone X Position"], recv_json["DeathZone Y Position"], 
    recv_json["DeathZone X Size"], recv_json["DeathZone Y Size"]);

    int numberOfPlayers = 1;
    int numberOfDisconnections = 0;

    //Create a vector used to contain all sprites that will appear on every clients scrren
    std::vector<Character> characterCollection;

    //Start initializing the client with information from the client
    numberOfPlayers = recv_json["Player Count"];
    numberOfDisconnections = recv_json["Disconnection Count"];
    Character sprite(100.0, 100.0, recv_json["Player Number"], "");

    //Use for loop to assign more information
    for (int i = 0; i < numberOfPlayers; i++) {
        //Assign the correct player name and color
        if (i == sprite.getCharacterNumber()) {
            sprite.setCharacterName(recv_json["Player Name"][i]);
            sprite.setTexture(&textures[i % 4]);
            sprite.setSpawnPoint(&playerSpawn);
            //sprite.setPosition(playerSpawn.getSpawn());
        }
        //Create other sprites representing the other players and add them to the character collection
        if (i != sprite.getCharacterNumber() && recv_json["Disconnected Client"][i] == false) {
            Character newSprite(100.0, 100.0, i, recv_json["Player Name"][i]);
            int spriteXPosition = (int)recv_json["Player X Position"][i];
            int spriteYPostion = (int)recv_json["Player Y Position"][i];
            newSprite.setPosition(spriteXPosition, spriteYPostion);
            newSprite.setTexture(&textures[i % 4]);
            characterCollection.push_back(newSprite);
        }
    }
    //Get all the information about the MovingPlatforms and create them
    for(int i = 0; i < recv_json["Number Of MovingPlatforms"]; i++) {
        int posX = recv_json["MovingPlatform X Position"][i];
        int posY = recv_json["MovingPlatform Y Position"][i];
        int sizeX = recv_json["MovingPlatform X Size"][i];
        int sizeY = recv_json["MovingPlatform Y Size"][i];
        unsigned int color = recv_json["MovingPlatform Color"][i];
        MovingPlatform *plat = new MovingPlatform(posX, posY, sizeX, sizeY, sf::Color(color), MOVE_HORIZONTAL, false);
        listOfMovingPlatforms.push_back(plat);
        listOfPlatforms.push_back(plat);
    }
    //Get all the information about the static platforms and create them
    for(int i = 0; i < recv_json["Number Of Static Platform"]; i++) {
        int posX = recv_json["Static Platform X Position"][i];
        int posY = recv_json["Static Platform Y Position"][i];
        int sizeX = recv_json["Static Platform X Size"][i];
        int sizeY = recv_json["Static Platform Y Size"][i];
        unsigned int color = recv_json["Static Platform Color"][i];
        Platform *plat = new Platform(posX, posY, sizeX, sizeY, sf::Color(color));
        listOfPlatforms.push_back(plat);
    }
    //Get all the information about the SideBoundaries and create them
    for(int i = 0; i < recv_json["Number Of SideBoundaries"]; i++) {
        int posX = recv_json["SideBoundary X Position"][i];
        int posY = recv_json["SideBoundary Y Position"][i];
        int sizeX = recv_json["SideBoundary X Size"][i];
        int sizeY = recv_json["SideBoundary Y Size"][i];
        SideBoundary *boundary = new SideBoundary(posX, posY, sizeX, sizeY, &view);
        listOfSideBoundaries.push_back(boundary);
    }
    //std::cout << "This is the number of players: " << recv_json["Player Count"] << " This is the player's number: " << sprite.getCharacterNumber() << std::endl;
    //std::cout << "This is the name of the character: " << sprite.getCharacterName() << std::endl;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Create a HUD to display player information
    HUD playerUI(&view, &sprite, &globalTime, &window);
    //Gather the lastPosition of the sprite to be used for some calculations
    sf::Vector2f spriteLastPos = sprite.getPosition();

    //First create an instance of the manager and then register all handlers
    EventManager * manager = EventManager::getInstance();
    manager->setGlobalTime(&globalTime);
    //Register Handler for the sprite
    std::vector<Event::EventType> spriteHandler;
    spriteHandler.push_back(Event::DEATH_COLLISION);
    spriteHandler.push_back(Event::SPRITE_COLLISION);
    spriteHandler.push_back(Event::KEYBOARD_MOVE_INPUT);
    spriteHandler.push_back(Event::SPAWN_EVENT);
    spriteHandler.push_back(Event::CHEATS_ACTIVE);
    spriteHandler.push_back(Event::CHEAT_DISABLE);
    manager->registerHandlers(spriteHandler, sprite.getEventHandler());
    //Register Handlers for each SideBoundary
    std::vector<Event::EventType> leftSideBoundary;
    leftSideBoundary.push_back(Event::RIGHT_COLLISION);
    leftSideBoundary.push_back(Event::REST_BOUNDARY);
    manager->registerHandlers(leftSideBoundary, listOfSideBoundaries[0]->getCollisionHandler());
    std::vector<Event::EventType> rightSideBoundary;
    rightSideBoundary.push_back(Event::LEFT_COLLISION);
    rightSideBoundary.push_back(Event::REST_BOUNDARY);
    manager->registerHandlers(rightSideBoundary, listOfSideBoundaries[1]->getCollisionHandler());
    std::vector<Event::EventType> topSideBoundary;
    topSideBoundary.push_back(Event::BOTTOM_COLLISION);
    topSideBoundary.push_back(Event::REST_BOUNDARY);
    manager->registerHandlers(topSideBoundary, listOfSideBoundaries[2]->getCollisionHandler());
    std::vector<Event::EventType> bottomSideBoundary;
    bottomSideBoundary.push_back(Event::TOP_COLLISION);
    bottomSideBoundary.push_back(Event::REST_BOUNDARY);
    manager->registerHandlers(bottomSideBoundary, listOfSideBoundaries[3]->getCollisionHandler());

    //First handle the spawn event so the player starts at their inital spawn.
    // Event playerSpawnEvent;
    // playerSpawnEvent.type = Event::SPAWN_EVENT;
    // playerSpawnEvent.sprite.characterNumber = sprite.getCharacterNumber();
    // manager->raiseEvents(playerSpawnEvent);
    // manager->handleEvents();

    //Initalize the V8 Plaform and create the isolate
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.release());
    v8::V8::InitializeICU();
    v8::V8::Initialize();
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate *isolate = v8::Isolate::New(create_params);
    {
        v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
        v8::HandleScope handle_scope(isolate);
        //Create the global and add functions that can be used in V8
        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
        global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
        global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
        global->Set(isolate, "getObjectName", v8::FunctionTemplate::New(isolate, v8helpers::getObjectV8));
        global->Set(isolate, "eventFactory", v8::FunctionTemplate::New(isolate, Event::scriptedEventFactory));
        //Create a context for the manager
        v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
        //Create a script manager to add the contexts and the scripts
        ScriptManager *sm = new ScriptManager(isolate, default_context);
        //Add a context for the scripts to modify the sprite
        v8::Local<v8::Context> sprite_context = v8::Context::New(isolate, NULL, global);
		sm->addContext(isolate, sprite_context, "sprite_context");
        bool reload = false;
        //Add the spawn event script to the manager
        sm->addScript("spawn_event_script", "scripts/spawn_event_script.js", "sprite_context");
        //Run the spawn event script to create an event inside V8 that has the type SPAWN_EVENT
        sm->runOne("spawn_event_script", reload, "sprite_context");
        //Retrieve the event from the list
        Event *e = Event::listOfEvents[0];
        //Add the character number to the event
        e->sprite.characterNumber = sprite.getCharacterNumber();
        //Raise the event
        manager->raiseEvents(*e);
        //Handle the event so the character can be at spawn
        manager->handleEvents();
        //Delete the manager to clear the scripts and contexts
        delete sm;
    }

    //Add the isolate to the sprite's script handler
    sprite.getScriptHandler()->setIsolate(isolate);
    //Add all events that the handler
    std::vector<Event::EventType> scriptHandler;
    scriptHandler.push_back(Event::INCREASE_SPEED_SCRIPT);
    scriptHandler.push_back(Event::RESET_SPEED_SCRIPT);
    scriptHandler.push_back(Event::INCREASE_LIVES_SCRIPT);
    scriptHandler.push_back(Event::RESET_LIVES_SCRIPT);
    scriptHandler.push_back(Event::RESET_ALL);
    //Register the handler to the Event Manager
    manager->registerHandlers(scriptHandler, sprite.getScriptHandler());

    // Game loop that keeps everything running until the program is closed
    while (window.isOpen()) {
        
        sf::Event event;


        //Get the current time from both timelines and set the delta time for both
        float current_time = globalTime.getTime();
        float current_local_time = localTime.getTime();
        float delta_time = current_time - last_time;
        float delta_local_time = current_local_time - last_local_time;

        //Get the diff_time and use it to guard the frame rate
        float diff_time = targetTime - fmod(delta_time, targetTime);
        //printf("This is the diff time: %lf\n", diff_time);
        if (diff_time > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float>(diff_time));
        }

        //Update the last time
        last_time = current_time;
        last_local_time = current_local_time;

        //printf("This is the time in seconds???: %lf\n", delta_time);
        //printf("This is the local delta time in seconds: %lf\n", delta_local_time);
        //printf("This is the current time:%lf\n", current_time);
        //printf("This is the time time in seconds for the localTime: %lf\n", localTime.getTime());

        // Checks for different events
        while (window.pollEvent(event)) {
            // If the event is Closed then the program will close
            if (event.type == sf::Event::Closed) {
                //////////////////////DISCONNECT BEFORE CLOSING THE WINDOW/////////////////////////
                //Create a json to send to the server containing the client's ID
                json disconnect;
                disconnect["Event Type"] = Event::SERVER_DISCONNECTION;
                disconnect["Client Number"] = sprite.getCharacterNumber();
                //Dump the json and send it to the server
                std::string disconnect_str = disconnect.dump();
                zmq::message_t sendDisconnection(disconnect_str.size());
                memcpy(sendDisconnection.data(), disconnect_str.c_str(), disconnect_str.size());
                auto res3 = communicationSocket.send(sendDisconnection, zmq::send_flags::none);
                //Wait for the server to reply then print out that you have disconnected
                auto res4 = communicationSocket.recv(reply, zmq::recv_flags::none);
                std::cout << "You have successfully " << reply.to_string() << "!" << std::endl;
                ///////////////////////////////////////////////////////////////////////////////
                window.close();
            }
            //Check for certain keyboard inputs
            if (event.type == sf::Event::KeyPressed) {
                //Pausing
                if (event.key.code == sf::Keyboard::P) {
                    if (globalTime.getPauseStatus()) {
                        globalTime.unPause();
                    }
                    else {
                        globalTime.pause();
                    }
                }
                //Change the tic size of the local timeline to speed up the game
                else if (event.key.code == sf::Keyboard::Period) {
                    if (localTime.getTicSize() == 1) {
                        localTime.changeTic(0.5);
                        last_local_time = localTime.getTime();
                    }
                    else if (localTime.getTicSize() == 2) {
                        localTime.changeTic(1);
                        last_local_time = localTime.getTime();
                    }
                }
                //Change the tic size of the local timeline to slow down the game
                else if (event.key.code == sf::Keyboard::Comma) {
                    if (localTime.getTicSize() == 1) {
                        localTime.changeTic(2);
                        last_local_time = localTime.getTime();
                    }
                    else if (localTime.getTicSize() == 0.5) {
                        localTime.changeTic(1);
                        last_local_time = localTime.getTime();
                    }
                }
            }
        }

        //If the player has cheats active then allow them to use the scripts
        if(sprite.getCheatStatus()) {
            //Increase the speed of the character
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                Event increaseSpeed;
                increaseSpeed.type = Event::INCREASE_SPEED_SCRIPT;
                manager->raiseEvents(increaseSpeed);
            }
            //Reset the speed of the character
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
                Event resetSpeed;
                resetSpeed.type = Event::RESET_SPEED_SCRIPT;
                manager->raiseEvents(resetSpeed);
            }
            //Increase the number of lives
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                Event increaseLives;
                increaseLives.type = Event::INCREASE_LIVES_SCRIPT;
                manager->raiseEvents(increaseLives);
            }
            //Reset the number of lives
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
                Event resetLives;
                resetLives.type = Event::RESET_LIVES_SCRIPT;
                manager->raiseEvents(resetLives);
            }
            //Reset all modified values
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                Event resetAll;
                resetAll.type = Event::RESET_ALL;
                manager->raiseEvents(resetAll);
            }
            //Teleport to spawn point
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                {
                    v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
                    v8::HandleScope handle_scope(isolate);
                    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
                    global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
                    global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
                    global->Set(isolate, "getObjectName", v8::FunctionTemplate::New(isolate, v8helpers::getObjectV8));
                    global->Set(isolate, "eventFactory", v8::FunctionTemplate::New(isolate, Event::scriptedEventFactory));
                    v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
                    ScriptManager *sm = new ScriptManager(isolate, default_context);
                    bool reload = false;
                    v8::Local<v8::Context> sprite_context = v8::Context::New(isolate, NULL, global);
                    sm->addContext(isolate, sprite_context, "sprite_context");
                    sm->addScript("spawn_event_script", "scripts/spawn_event_script.js", "sprite_context");
                    sm->runOne("spawn_event_script", reload, "sprite_context");
                    Event *e = Event::listOfEvents[Event::listOfEvents.size() - 1];
                    e->sprite.characterNumber = sprite.getCharacterNumber();
                    manager->raiseEvents(*e);
                    view.setCenter(playerSpawn.getViewCenter());
                    manager->handleEvents();
                    delete sm;
                }
            }
        }

        //////////////////////////////////////CLIENT STUFF/////////////////////////////////////////

        //Create a json to send updates to the server
        json clientUpdate;
        clientUpdate["Client Number"] = sprite.getCharacterNumber();
        clientUpdate["Client X Position"] = sprite.getPosition().x;
        clientUpdate["Client Y Position"] = sprite.getPosition().y;
        //Dump the contents of the json and send it to the server
        std::string update_str = clientUpdate.dump();
        zmq::message_t updateInformation(update_str.size());
        memcpy(updateInformation.data(), update_str.c_str(), update_str.size());
        auto res5 = updateSocket.send(updateInformation, zmq::send_flags::none);

        //Wait to receive an update from the server
        zmq::message_t receiveUpdate;
        auto res6 = updateSocket.recv(receiveUpdate, zmq::recv_flags::none);
        update_str = std::string(static_cast<char*>(receiveUpdate.data()), receiveUpdate.size());
        auto update_json = json::parse(update_str);

        //Update the positions of the MovingPlatforms
        for(int i = 0; i < listOfMovingPlatforms.size(); i++) {
            float platformXPosition = update_json["MovingPlatform X Position"][i];
            float platformYPosition = update_json["MovingPlatform Y Position"][i];
            listOfMovingPlatforms[i]->setPosition(platformXPosition, platformYPosition);
        }

        //If the number of players is less than the server's player count, then add new player to the character collection
        if (numberOfPlayers < update_json["Player Count"]) {
            numberOfPlayers = update_json["Player Count"];
            int index = update_json["Player Number"];
            Character newSprite(100.0, 100.0, index, update_json["Player Name"][index]);
            int spriteXPosition = (int)update_json["Player X Position"][index];
            int spriteYPostion = (int)update_json["Player Y Position"][index];
            newSprite.setPosition(spriteXPosition, spriteYPostion);
            newSprite.setTexture(&textures[index % 4]);
            characterCollection.push_back(newSprite);
        }
        //If the number of disconnections is less than the server's number then a player disconnected and needs to
        //be removed on the client side.
        if (numberOfDisconnections < update_json["Disconnection Count"]) {
            numberOfDisconnections = update_json["Disconnection Count"];
            for(int i = 0; i < characterCollection.size(); i++) {
                if (characterCollection[i].getCharacterNumber() == update_json["Last Disconnect"]) {
                    characterCollection.erase(characterCollection.begin() + i);
                    break;
                }
            }
        }
        //Update the positon of all the other players
        for (int i = 0; i < characterCollection.size(); i++) {
            int index = characterCollection[i].getCharacterNumber();
            int spriteXPosition = (int)update_json["Player X Position"][index];
            int spriteYPostion = (int)update_json["Player Y Position"][index];
            characterCollection[i].setPosition(spriteXPosition, spriteYPostion);
        }
        ////////////////////////////////////////////////////////////////////////////////////////

        if (window.hasFocus()) {
            window.setActive(true);
            //Allow for characterMovement only if the character has not lost all lives
            if(!sprite.getHealth()->getGameOver()) {
                //Create a thread to handle character inputs in the background
                std::mutex m;
                std::condition_variable cv;
                SpriteThread spriteThreadObj(0, NULL, &m, &cv, &sprite, delta_local_time);
                std::thread spriteThread(run_Movement, &spriteThreadObj);
                //std::thread thread(run_Isolate, isolate);
                //thread.join();
                spriteThread.join();
            }
        }
        else {
            window.setActive(false);
            //If the other window doesn't have focus and the character is above ground
            //then apply gravity to make sure they fall and that they appear falling on other clients
            if (!sprite.getOnGround()) {
                sf::Vector2f velocity = sprite.getCharacterVelocity();
                velocity.y += GRAVITY;
                sprite.setCharacterVelocity(velocity);
                velocity *= delta_local_time;
                sprite.move(velocity.x, velocity.y);
            }
        }

        //Colors the entire window black
        window.clear(sf::Color::Black);
        //Draw the background
        window.draw(background);
        //Draw the client's character and each character in the character collection
        window.draw(sprite);
        for (int i = 0; i < characterCollection.size(); i++) {
            window.draw(characterCollection[i]);
        }
        //Draw all the platforms
        for(Platform* plat : listOfPlatforms) {
            window.draw(*plat);
        }
        //window.draw(playerSpawn);
        //window.draw(deathzone);
        /* for(SideBoundary* boundary : listOfSideBoundaries) {
            window.draw(*boundary);
        }  */

        sf::FloatRect spriteBoundingBox = sprite.getGlobalBounds();
        // Left edge of screen
        if (sprite.getPosition().x < 0) {
            sprite.setPosition(0, sprite.getPosition().y);
        }
        // Right edge of screen
        if (spriteBoundingBox.left > 1920 - sprite.getGlobalBounds().width) {
            sprite.setPosition(1920 - sprite.getGlobalBounds().width, sprite.getPosition().y);
        }
        // Top edge of screen
        if (sprite.getPosition().y < 0) {
            sprite.setPosition(sprite.getPosition().x, 0);
        }
        // Bottom edge of screen
        /* if (sprite.getPosition().y > 1080 - sprite.getGlobalBounds().height) {
            sprite.setPosition(sprite.getPosition().x, 1080 - sprite.getGlobalBounds().height);
        } */
        
        //Collision Detection for the platforms
        bool checkOnGround = false;
        for(Platform* plat : listOfPlatforms) {
            bool check = false;
            check = sprite.getCollide()->checkCollision(plat);
            //If check is true then the character is colliding with a platform and should be on ground.
            if(check) {
                checkOnGround = check;
            }
        }
        if(checkOnGround) {
            sprite.setOnGround();
        }
        else {
            sprite.setOffGround();
        }

        //Check if the player has collided with the deathzone and has died
        bool checkDeath = false;
        checkDeath = deathzone.getCollide()->checkCollision(&sprite);

        //If the sprite has lost all their lives then display GameOver screen
        if(sprite.getHealth()->getGameOver()) {
            playerUI.displayGameOver();
        }
        //Display the playerUI
        else {
            playerUI.displayHUD();
        }
        
        //Collision Detection for the SideBoundaries
        if(listOfSideBoundaries[0]->getPosition().x > 300) {
            listOfSideBoundaries[0]->getCollide()->checkCollision(&sprite);
        }
        if(listOfSideBoundaries[1]->getPosition().x < 1620) {
            listOfSideBoundaries[1]->getCollide()->checkCollision(&sprite);
        }
        if(listOfSideBoundaries[2]->getPosition().y > 200) {
            listOfSideBoundaries[2]->getCollide()->checkCollision(&sprite);
        }
        if(listOfSideBoundaries[3]->getPosition().y < 880) {
            listOfSideBoundaries[3]->getCollide()->checkCollision(&sprite);
        }

        //Movement algorithm for the SideBoundaries to follow the character
        if(sprite.getPosition().x > spriteLastPos.x && sprite.getPosition().x - listOfSideBoundaries[0]->getPosition().x >= 100
        && listOfSideBoundaries[0]->getPosition().x < 1415) {
            listOfSideBoundaries[0]->setPosition(sprite.getPosition().x - 100, listOfSideBoundaries[0]->getPosition().y);
        }
        if(sprite.getPosition().x < spriteLastPos.x && listOfSideBoundaries[1]->getPosition().x - sprite.getPosition().x >= 100
        && listOfSideBoundaries[1]->getPosition().x > 500) {
            listOfSideBoundaries[1]->setPosition(sprite.getPosition().x + 100, listOfSideBoundaries[1]->getPosition().y);
        }
        if(sprite.getPosition().y < spriteLastPos.y && listOfSideBoundaries[3]->getPosition().y - sprite.getPosition().y >= 100
        && listOfSideBoundaries[3]->getPosition().y > 400) {
            listOfSideBoundaries[3]->setPosition(listOfSideBoundaries[3]->getPosition().x, sprite.getPosition().y + 100);
        }
         if(sprite.getPosition().y > spriteLastPos.y  && sprite.getPosition().y - listOfSideBoundaries[2]->getPosition().y >= 100
         && listOfSideBoundaries[2]->getPosition().y < 680) {
            listOfSideBoundaries[2]->setPosition(listOfSideBoundaries[2]->getPosition().x, sprite.getPosition().y - 100);
        }
        //Conditon to reset the left and right boundaries when the sprite somehow passes them.
        if(listOfSideBoundaries[1]->getPosition().x < sprite.getPosition().x && listOfSideBoundaries[1]->getPosition().x < 1620) {
            listOfSideBoundaries[1]->setPosition(sprite.getPosition().x + 100, 0);
        } 
        if(listOfSideBoundaries[0]->getPosition().x > sprite.getPosition().x && listOfSideBoundaries[0]->getPosition().x > 300) {
            listOfSideBoundaries[0]->setPosition(sprite.getPosition().x - 100, 0);
        } 
        //Reset the boundaries and the view if the player dies
        if(checkDeath) {
            Event resetEvent;
            resetEvent.type = Event::REST_BOUNDARY;
            manager->raiseEvents(resetEvent);
            view.setCenter(playerSpawn.getViewCenter());
        }
        //Update the sprite's last position
        spriteLastPos = sprite.getPosition();
        //Handle all events
        manager->handleEvents();
        //Set the view of the window
        window.setView(view);
        //Displays all changes drawn onto the 
        window.display();    
    }
    //Dispose the isolate and shutdown the V8 platform
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();

    return 0;
}