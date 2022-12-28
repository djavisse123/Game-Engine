/**
 * @file gameEngineServer.cpp
 * @author Daniel Avisse (djavisse)
 * This is the gameEngineServer program. The server for this version of the game engine
 * is used to mainly just initalize new clients and update them when they reach a new level.
 * This server doesn't support multiplayer like in the other version of the game enginer.
 */

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <math.h>
#include <zmq.hpp>
#include "nlohmann/json.hpp"

#include "Character.h"
#include "GlobalTime.h"
#include "MovingPlatform.h"
#include "PauseMenu.h"
#include "Platform.h"
#include "Timeline.h"
#include "PlatformThread.h"
#include "ServerHandler.h"

int main() 
{
    //Use shortcut for json
    using json = nlohmann::json;

    //Create a zmq context to be used to create the socket
    zmq::context_t context{1};

    //Create a rep socket to be used to check for new clients
    zmq::socket_t communicationSocket{context, zmq::socket_type::rep};
    communicationSocket.bind("tcp://*:5555");
    
    //Create another rep socket to be used to gather client update information
    zmq::socket_t updateSocket{context, zmq::socket_type::rep};
    updateSocket.bind("tcp://*:5556");

    //Prepare some static data for responses
    const std::string receieve{"Received"};
    const std::string newClient{"New Client"};
    const std::string disconnection{"disconnected"};

    //Number of clients connected to the server
    int clientCount = 0;
    //Number of clients that have disconnected from the server
    int disconnectedClients = 0;

    //Create and initialize a json to create a representation of the game's state
    json gameState;

    //Player Information
    gameState["Player Count"] = 0;
    gameState["Player Name"] = {};
    gameState["Player Number"] = 0;
    gameState["Player X Position"] = {};
    gameState["Player Y Position"] = {};
    gameState["Current Level"] = {};

    //MovingPlatform Information
    gameState["MovingPlatform X Size"] = 175.0;
    gameState["MovingPlatform Y Size"] = 50.0;
    gameState["MovingPlatform X Position"] = 300;
    gameState["MovingPlatform Y Position"] = 800;
    gameState["MovingPlatform Color"] = 0xFFFFFFFFF;

    //Plaform Information for Level 1
    gameState["Level"][1]["Number Of Rows"] = 6;
    gameState["Level"][1]["Static Platform X Size"] = {80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
    gameState["Level"][1]["Static Platform Y Size"] = {80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
    gameState["Level"][1]["Static Platform X Position"] = {0, 80, 160, 240, 320, 400, 480, 560, 640, 720};
    gameState["Level"][1]["Static Platform Y Position"] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    gameState["Level"][1]["Number Of Static Platform"] = 10;
    gameState["Static Platform Color"] = 0xFFFFFFFFF;
    
    //Platform Information for Level 2
    gameState["Level"][2]["Number Of Rows"] = 10;
    gameState["Level"][2]["Static Platform X Size"] = {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40};
    gameState["Level"][2]["Static Platform Y Size"] = {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40};
    gameState["Level"][2]["Static Platform X Position"] = {0, 40, 80, 120, 160, 200, 240, 280, 320, 360, 400, 440, 480, 520, 560, 600, 640, 680, 720, 760};
    gameState["Level"][2]["Static Platform Y Position"] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    gameState["Level"][2]["Number Of Static Platform"] = 20;

    //Platform Information for Level 3
    gameState["Level"][3]["Number Of Rows"] = 6;
    gameState["Level"][3]["Static Platform X Size"] = {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 
    80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
    gameState["Level"][3]["Static Platform Y Size"] = {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 
    80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
    gameState["Level"][3]["Static Platform X Position"] = {0, 40, 80, 120, 160, 200, 240, 280, 320, 360, 400, 440, 480, 520, 560, 600, 640, 680, 720, 760,
    0, 80, 160, 240, 320, 400, 480, 560, 640, 720};
    gameState["Level"][3]["Static Platform Y Position"] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
    180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};
    gameState["Level"][3]["Number Of Static Platform"] = 30;

    //Platform Information for Level 4
    gameState["Level"][4]["Number Of Rows"] = 6;
    gameState["Level"][4]["Static Platform X Size"] = {40, 40, 40, 40, 80, 80, 80, 80, 80, 80, 40, 40, 40, 40};
    gameState["Level"][4]["Static Platform Y Size"] = {40, 40, 40, 40, 80, 80, 80, 80, 80, 80, 40, 40, 40, 40};
    gameState["Level"][4]["Static Platform X Position"] = {0, 40, 80, 120, 160, 240, 320, 400, 480, 560, 640, 680, 720, 760};
    gameState["Level"][4]["Static Platform Y Position"] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    gameState["Level"][4]["Number Of Static Platform"] = 14;
    gameState["Static Platform Color"] = 0xFFFFFFFFF;

    //DeathZone Information
    gameState["DeathZone X Size"] = 1920;
    gameState["DeathZone Y Size"] = 75;
    gameState["DeathZone X Position"] = 0;
    gameState["DeathZone Y Position"] = 1180;

    //SpawnPoint Information
    gameState["SpawnPoint X Position"] = 365;
    gameState["SpawnPoint Y Position"] = 750;
    gameState["SpawnPoint SpawnOrder"] = 0;
    gameState["SpawnPoint ViewCenter X"] = 400;
    gameState["SpawnPoint ViewCenter Y"] = 780;
  
    //Handle Disconnections
    gameState["Disconnected Client"] = {};
    gameState["Disconnection Count"] = 0;
    gameState["Last Disconnect"] = 0;

    //Create a timeline for the server to be used by the EventManager
    GlobalTime serverTime(NULL, 1);
    //Create a handler for the server so it handle events
    ServerHandler serverHandler(&gameState);
    //Create an EventManager to handle server events
    EventManager * manager = EventManager::getInstance();
    manager->setGlobalTime(&serverTime);
    //List out all the event types the server handler responds to and register it.
    std::vector<Event::EventType> serverHandlerTypes;
    serverHandlerTypes.push_back(Event::SERVER_CONNECTION);
    serverHandlerTypes.push_back(Event::SERVER_DISCONNECTION);
    manager->registerHandlers(serverHandlerTypes, &serverHandler);

    //Keep looping to receive new clients and update gameState
    while(true) {
        
        //Create message that will be used to receive requests from clients
        zmq::message_t request;

        //Check for messages from client but don't wait for them so the server can keep running
        auto res = communicationSocket.recv(request, zmq::recv_flags::dontwait);
        if(request.size() > 0) {
            auto request_str = std::string(static_cast<char*>(request.data()), request.size());
            auto request_json = json::parse(request_str);  
            //Create an event for either connection or disconnection 
            Event e;
            e.type = request_json["Event Type"];
            //If the event sent from the client is a connection then initialize the client
            if(e.type == Event::SERVER_CONNECTION) {
                std::cout << "Received New Client" << std::endl;
                //Increment the client count when the client connects with the server
                clientCount++;
                e.server.clientCount = clientCount;
                manager->raiseEvents(e);
                //Handle the event now so the client can get their information.
                manager->handleEvents();
                //Dump the contents of the json send it to the client
                std::string json_str = gameState.dump();
                zmq::message_t reply(json_str.size());
                memcpy(reply.data(), json_str.c_str(), json_str.size());
                auto res2 = communicationSocket.send(reply, zmq::send_flags::none);   
            }
            //If the event sent from the client is a disconnection then handle the disconnection
            else if (e.type == Event::SERVER_DISCONNECTION) {
                //Get the ID of the client
                e.server.clientID = request_json["Client Number"];
                //Increment the number of disconnected clients
                disconnectedClients++;
                e.server.disconnectedClients = disconnectedClients;
                manager->raiseEvents(e);
                //Handle the event now so the client can disconnect.
                manager->handleEvents();
                //Send a message back to the other client that they have disconnected
                auto res3 =communicationSocket.send(zmq::buffer(disconnection), zmq::send_flags::none);
            }
        }
        //Check for update requests from clients but don't wait for them so the server can keep running
        zmq::message_t updateRequest;
        auto res4 = updateSocket.recv(updateRequest, zmq::recv_flags::dontwait);
        if (updateRequest.size() > 0) {
            //Parse the json sent from the client
            auto update_str = std::string(static_cast<char*>(updateRequest.data()), updateRequest.size());
            auto update_json = json::parse(update_str);
            //Gather the client's ID so it can be used to update information accordingly
            int clientID = update_json["Client Number"];
            //If the client has completed a level then update them to the next level
            if(update_json["Complete Level"]) {
                int currentLevel = gameState["Current Level"][clientID];
                currentLevel++;
                gameState["Current Level"][clientID]  = currentLevel;
            }
            //Update the specific position of the client that requested the update
            gameState["Player X Position"][clientID] = update_json["Client X Position"];
            gameState["Player Y Position"][clientID] = update_json["Client Y Position"];
            //Update the player count just in case the client requesting needs to be aware of new players
            gameState["Player Count"] = clientCount;
            //Dump the contents of the gameState json and send it back to the requester for them to update
            std::string json_str2 = gameState.dump();
            //std::cout << "This is the contents of the json: " << json_str2.c_str() << std::endl;
            zmq::message_t updateReply(json_str2.size());
            memcpy(updateReply.data(), json_str2.c_str(), json_str2.size());
            auto res5 = updateSocket.send(updateReply, zmq::send_flags::none);
        }
    }
}