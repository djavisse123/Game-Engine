/**
 * @file main.cpp
 * @author Daniel Avisse (djavisse)
 * This is the main.cpp file for HW5's game enigne environment. This version
 * of the engine is being used to create another game. The game being made in
 * this engine is a version of breakout. In this version of breakout the player 
 * connects to the server to get information about the ball, moving platforms, and
 * levels for the game. When the player breaks all blocks they will make contact with
 * the server to move onto the next level. There are four levels in total and once the
 * player beats them all they can reach an endless mode where they can see how high of 
 * a score they can get. Scripts can also be used in this game as cheats. The cheats can be
 * used to either decrease or increase the speed of the ball or to increase number of lives.
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
#include <stdlib.h>
#include <time.h> 

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
#include "PlatformThread.h"

/**
 * This fucntion is used to make the spriteThread run and handle movment for the sprite
 * The spriteThread will check for keyboard inputs for the cheats or will just move
 * the sprite automatically.
 * @param spritethread The spritethread object used to handle sprite movement
 */
void run_MovementSprite(SpriteThread *spriteThread)
{
    spriteThread->handleMovement();
}

/**
 * This fucntion is used to make the platformThread run and handle movment for the platform
 * The platformThread will check for keyboard inputs from the user and then move the platform
 * accordingly to that input.
 * @param spritethread The spritethread object used to handle sprite movement
 */
void run_MovementPlatform(PlatformThread *platThread)
{
    platThread->handleMovement();
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
    sf::RenderWindow window(sf::VideoMode(800, 900), "My window", sf::Style::Default);
    //Sets the position for where the window opens
    window.setPosition(sf::Vector2i(10, 50));
    //Sets the Frame Rate to 60fps
    window.setFramerateLimit(60);
    window.setTitle("Kirby's Breakout");
    //Set the view for the window
    sf::View view;
    view = window.getDefaultView();
    window.setView(view);
    
    //Create a list to hold some the platforms that will be created once the client joins the server
    std::vector<Platform *> listOfPlatforms;

    //Load in textures for the backgrounds
    sf::Texture spaceBackgroundTexture;
    sf::Texture forestBackgroundTexture;
    sf::Texture beachBackgroundTexture;
    sf::Texture pyramidBackgroundTexture;
    sf::Texture iceCaveBackgroundTexture;
    spaceBackgroundTexture.loadFromFile("textures/space_background.png");
    forestBackgroundTexture.loadFromFile("textures/forest_background.png");
    beachBackgroundTexture.loadFromFile("textures/beach_background.png");
    pyramidBackgroundTexture.loadFromFile("textures/pyramid_background.png");
    iceCaveBackgroundTexture.loadFromFile("textures/ice_cave_background.png");
    beachBackgroundTexture;
    sf::Sprite spaceBackground;
    sf::Sprite forestBackground;
    sf::Sprite beachBackground;
    sf::Sprite pyramidBackground;
    sf::Sprite iceCaveBackground;
    spaceBackground.setTexture(spaceBackgroundTexture);
    forestBackground.setTexture(forestBackgroundTexture);
    beachBackground.setTexture(beachBackgroundTexture);
    pyramidBackground.setTexture(pyramidBackgroundTexture);
    iceCaveBackground.setTexture(iceCaveBackgroundTexture);

    //Load in textures for the platforms
    sf::Texture kirbyBallTexture;
    sf::Texture bandandDeeTexture;
    sf::Texture buffDededeTexture;
    sf::Texture candyTexture;
    sf::Texture chefKirbyTexture;
    sf::Texture chubbyKirbyTexture;
    sf::Texture dededeTambourineTexture;
    sf::Texture kirby30Texture;
    sf::Texture kirbyCakeTexture;
    sf::Texture kirbyConeTexture;
    sf::Texture kirbyDripTexture;
    sf::Texture kirbyHeartTexture;
    sf::Texture kirbyTexture;
    sf::Texture maskedDededeTexture;
    sf::Texture metaKnightTexture;
    sf::Texture pixelKirbyTexture;
    sf::Texture pixelStarKirbyTexture;
    sf::Texture sleepyKirbyTexture;
    sf::Texture ultraSwordTexture;
    sf::Texture yarnKirbyTexture;
    sf::Texture yellowAndPinkTexture;
    sf::Texture cloudTexture;
    kirbyBallTexture.loadFromFile("textures/Kirby_Ball.png");
    bandandDeeTexture.loadFromFile("textures/Bandana_Dee.png");
    buffDededeTexture.loadFromFile("textures/Buff_Dedede.png");
    candyTexture.loadFromFile("textures/Candy.png");
    chefKirbyTexture.loadFromFile("textures/Chubby_Kirby.png");
    dededeTambourineTexture.loadFromFile("textures/Dedede_Tambourine.png");
    kirby30Texture.loadFromFile("textures/Kirby_30.png");
    kirbyCakeTexture.loadFromFile("textures/Kirby_Cake.png");
    kirbyConeTexture.loadFromFile("textures/Kirby_Cone.png");
    kirbyDripTexture.loadFromFile("textures/Kirby_Drip.png");
    kirbyHeartTexture.loadFromFile("textures/Kirby_Heart.png");
    kirbyTexture.loadFromFile("textures/Kirby.png");
    maskedDededeTexture.loadFromFile("textures/Masked_Dedede.png");
    metaKnightTexture.loadFromFile("textures/MetaKnight.png");
    pixelKirbyTexture.loadFromFile("textures/Pixel_Kirby.png");
    pixelStarKirbyTexture.loadFromFile("textures/Pixel_Star_Kirby.png");
    sleepyKirbyTexture.loadFromFile("textures/Sleepy_Kirby.png");
    ultraSwordTexture.loadFromFile("textures/Ultra_Sword.png");
    yarnKirbyTexture.loadFromFile("textures/Yarn_Kirby.png");
    yellowAndPinkTexture.loadFromFile("textures/Yellow_&_Pink_Kirby.png");
    cloudTexture.loadFromFile("textures/Cloud.png");
    cloudTexture.setSmooth(true);
    sf::Texture blockTextures[] = {bandandDeeTexture, buffDededeTexture, candyTexture, chefKirbyTexture, dededeTambourineTexture, kirby30Texture,
    kirbyCakeTexture, kirbyConeTexture, kirbyDripTexture, kirbyHeartTexture, kirbyTexture, maskedDededeTexture, metaKnightTexture, pixelKirbyTexture,
    pixelStarKirbyTexture, sleepyKirbyTexture, ultraSwordTexture, yarnKirbyTexture, yellowAndPinkTexture};


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

    //Use the information from the server to create the ball
    Character ball(50.0, 50.0, recv_json["Player Number"], "");
    ball.setCharacterName(recv_json["Player Name"][0]);
    ball.setTexture(&kirbyBallTexture);
    ball.setOnGround();
    ball.setPosition(playerSpawn.getSpawn());
    ball.setCharacterVelocity(sf::Vector2f(-1.f, -1.f));
    ball.setSpawnPoint(&playerSpawn);

    //Use the information from the server to create the movingPlatform
    int posX = recv_json["MovingPlatform X Position"];
    int posY = recv_json["MovingPlatform Y Position"];
    int sizeX = recv_json["MovingPlatform X Size"];
    int sizeY = recv_json["MovingPlatform Y Size"];
    unsigned int color = recv_json["MovingPlatform Color"];
    MovingPlatform mPlat(posX, posY, sizeX, sizeY, sf::Color(color), MOVE_HORIZONTAL, false);
    mPlat.setPlatformNumber(recv_json["Player Number"]);
    mPlat.setTexture(&cloudTexture);

    //Start the game at the initial level
    int currentLevel = recv_json["Current Level"][ball.getCharacterNumber()];
    ball.setCurrentLevel(currentLevel);

    //Get all the information about the static platforms and create them
    srand(time(NULL));
    for(int i = 0; i < recv_json["Level"][currentLevel]["Number Of Rows"]; i++) {
        for(int j = 0; j < recv_json["Level"][currentLevel]["Number Of Static Platform"]; j++) {
            int posX = recv_json["Level"][currentLevel]["Static Platform X Position"][j];
            int posY = recv_json["Level"][currentLevel]["Static Platform Y Position"][j];
            int sizeX = recv_json["Level"][currentLevel]["Static Platform X Size"][j];
            int sizeY = recv_json["Level"][currentLevel]["Static Platform Y Size"][j];
            posY += (i) * sizeY;
            unsigned int color = recv_json["Static Platform Color"];
            Platform *plat = new Platform(posX, posY, sizeX, sizeY, sf::Color(color));
            int textureLocation = rand() % 19;
            plat->setTexture(&blockTextures[textureLocation]);
            listOfPlatforms.push_back(plat);
        }
    }

    //std::cout << "This is the number of players: " << recv_json["Player Count"] << " This is the player's number: " << sprite.getCharacterNumber() << std::endl;
    //std::cout << "This is the name of the character: " << sprite.getCharacterName() << std::endl;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Create a HUD to display player information
    HUD playerUI(&view, &ball, &globalTime, &window);

    //First create an instance of the manager and then register all handlers
    EventManager * manager = EventManager::getInstance();
    manager->setGlobalTime(&globalTime);
    //Register Handler for the movingPlatform
    std::vector<Event::EventType> platformHandler;
    platformHandler.push_back(Event::KEYBOARD_MOVE_INPUT);
    manager->registerHandlers(platformHandler, mPlat.getEventHandler());
    //Register Handler for the sprite
    std::vector<Event::EventType> spriteHandler;
    spriteHandler.push_back(Event::LAUNCH_BALL);
    spriteHandler.push_back(Event::DEATH_COLLISION);
    spriteHandler.push_back(Event::SPAWN_EVENT);
    spriteHandler.push_back(Event::CHEATS_ACTIVE);
    spriteHandler.push_back(Event::CHEAT_DISABLE);
    manager->registerHandlers(spriteHandler, ball.getEventHandler());

    //Initalize the V8 Plaform and create the isolate
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.release());
    v8::V8::InitializeICU();
    v8::V8::Initialize();
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate *isolate = v8::Isolate::New(create_params);

    //Add the isolate to the sprite's script handler
    ball.getScriptHandler()->setIsolate(isolate);
    //Add all events that the handler
    std::vector<Event::EventType> scriptHandler;
    scriptHandler.push_back(Event::INCREASE_SPEED_SCRIPT);
    scriptHandler.push_back(Event::RESET_SPEED_SCRIPT);
    scriptHandler.push_back(Event::DECREASE_SPEED_SCRIPT);
    scriptHandler.push_back(Event::INCREASE_LIVES_SCRIPT);
    scriptHandler.push_back(Event::RESET_LIVES_SCRIPT);
    scriptHandler.push_back(Event::RESET_ALL);
    //Register the handler to the Event Manager
    manager->registerHandlers(scriptHandler, ball.getScriptHandler());

    //Game loop that keeps everything running until the program is closed
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
                disconnect["Client Number"] = ball.getCharacterNumber();
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
        if(ball.getCheatStatus()) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                Event decreaseSpeed;
                decreaseSpeed.type = Event::DECREASE_SPEED_SCRIPT;
                manager->raiseEvents(decreaseSpeed);
            }
            //Increase the speed of the character
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
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
                    e->player.characterNumber = ball.getCharacterNumber();
                    manager->raiseEvents(*e);
                    manager->handleEvents();
                    delete sm;
                }
            }
        }

        //////////////////////////////////////CLIENT STUFF/////////////////////////////////////////

        //Check if the player has completed the level
        bool completeLevel = false;
        if(listOfPlatforms.size() == 0) {
            completeLevel = true;
        }
        //Create a json to send updates to the server
        json clientUpdate;
        clientUpdate["Client Number"] = ball.getCharacterNumber();
        clientUpdate["Client X Position"] = ball.getPosition().x;
        clientUpdate["Client Y Position"] = ball.getPosition().y;
        clientUpdate["Complete Level"] = completeLevel;

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

        //If the player has completed the level then proceed to the next level and update the game state.
        if(completeLevel) {
            currentLevel = update_json["Current Level"][ball.getCharacterNumber()];
            ball.setCurrentLevel(currentLevel);
            srand(time(NULL));
            //If the current level is greater than 4 then the player has reached endless mode.
            if(currentLevel > 4) {
                int randomLevel = rand() % 4 + 1;
                for(int i = 0; i < update_json["Level"][randomLevel]["Number Of Rows"]; i++) {
                    for(int j = 0; j < update_json["Level"][randomLevel]["Number Of Static Platform"]; j++) {
                        int posX = update_json["Level"][randomLevel]["Static Platform X Position"][j];
                        int posY = update_json["Level"][randomLevel]["Static Platform Y Position"][j];
                        int sizeX = update_json["Level"][randomLevel]["Static Platform X Size"][j];
                        int sizeY = update_json["Level"][randomLevel]["Static Platform Y Size"][j];
                        posY += (i) * sizeY;
                        unsigned int color = update_json["Static Platform Color"];
                        Platform *plat = new Platform(posX, posY, sizeX, sizeY, sf::Color(color));
                        int textureLocation = rand() % 19;
                        plat->setTexture(&blockTextures[textureLocation]);
                        listOfPlatforms.push_back(plat);
                    }
                }
            }
            //Proceed to the next level if the current level is less than 4.
            else {
                for(int i = 0; i < update_json["Level"][currentLevel]["Number Of Rows"]; i++) {
                    for(int j = 0; j < update_json["Level"][currentLevel]["Number Of Static Platform"]; j++) {
                        int posX = update_json["Level"][currentLevel]["Static Platform X Position"][j];
                        int posY = update_json["Level"][currentLevel]["Static Platform Y Position"][j];
                        int sizeX = update_json["Level"][currentLevel]["Static Platform X Size"][j];
                        int sizeY = update_json["Level"][currentLevel]["Static Platform Y Size"][j];
                        posY += (i) * sizeY;
                        unsigned int color = update_json["Static Platform Color"];
                        Platform *plat = new Platform(posX, posY, sizeX, sizeY, sf::Color(color));
                        int textureLocation = rand() % 19;
                        plat->setTexture(&blockTextures[textureLocation]);
                        listOfPlatforms.push_back(plat);
                    }
                }
            }
            //Return the ball to spawn when starting the new level
            Event spawn;
            spawn.type = Event::SPAWN_EVENT;
            spawn.player.characterNumber = ball.getCharacterNumber();
            manager->raiseEvents(spawn);
            manager->handleEvents();
        }
        ////////////////////////////////////////////////////////////////////////////////////////

        if (window.hasFocus()) {
            window.setActive(true);
            //Allow for platform movement only if the character has not lost all lives
            if(!ball.getHealth()->getGameOver()) {
                //Create a thread to handle character inputs in the background
                std::mutex m;
                std::condition_variable cv;
                PlatformThread platThreadObj(0, NULL, &m, &cv, &mPlat, delta_local_time);
                std::thread platThread(run_MovementPlatform, &platThreadObj);
                platThread.join();
            }
        }
        else {
            window.setActive(false);
        }
        //If the ball is on the ground (on the platform) then follow the position of the platform
        if(ball.getOnGround()) {
            ball.setPosition(mPlat.getPosition().x + 65, ball.getPosition().y);
        }
        //If the ball is off the ground then create a thread to handle the balls movements
        else {
            std::mutex m;
            std::condition_variable cv;
            SpriteThread spriteThreadObj(0, NULL, &m, &cv, &ball, delta_local_time);
            std::thread spriteThread(run_MovementSprite, &spriteThreadObj);
            spriteThread.join();
        }

        //Colors the entire window black
        window.clear(sf::Color::Black);
        //Draw the background
        if(currentLevel == 1) {
            window.draw(forestBackground);
        }
        else if(currentLevel == 2) {
            window.draw(pyramidBackground);
        }
        else if(currentLevel == 3) {
            window.draw(beachBackground);
        }
        else if(currentLevel == 4) {
            window.draw(iceCaveBackground);
        }
        else {
            window.draw(spaceBackground);
        }
        
        //Draw the ball
        window.draw(ball);
        //Draw the movingPlatform
        window.draw(mPlat);

        //Draw all the platforms
        for(Platform* plat : listOfPlatforms) {
            window.draw(*plat);
        }

        //window.draw(playerSpawn);
        //window.draw(deathzone);

        sf::FloatRect spriteBoundingBox = ball.getGlobalBounds();
        sf::FloatRect movingPlatformBox = mPlat.getGlobalBounds();
        //Window Collision for the movingPlatform
        // Left edge of screen
        if (movingPlatformBox.left < 0) {
            mPlat.setPosition(0, mPlat.getPosition().y);
        }
        // Right edge of screen
        if (movingPlatformBox.left + movingPlatformBox.width > 800 ) {
            mPlat.setPosition(800 - movingPlatformBox.width, mPlat.getPosition().y);
        }

        //Collision Detection for the platforms
        ball.getCollide()->checkCollision(&mPlat);
        for(auto it = listOfPlatforms.begin(); it != listOfPlatforms.end();) {
            bool hitPlatform = ball.getCollide()->checkCollision(*it);
            //If the ball hits the platform then remove and add to the score
            if(hitPlatform) {
                it = listOfPlatforms.erase(it);
                int scoreAmount = 1 * currentLevel;
                ball.increasePlayerScore(scoreAmount);
            } 
            else {
                ++it;
            }
        } 

        //Check if the player has collided with the deathzone and has died
        bool checkDeath = false;
        checkDeath = deathzone.getCollide()->checkCollision(&ball);

        //If the sprite has lost all their lives then display GameOver screen
        if(ball.getHealth()->getGameOver()) {
            playerUI.displayGameOver();
        }
        //Display the playerUI
        else {
            playerUI.displayHUD();
        }

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