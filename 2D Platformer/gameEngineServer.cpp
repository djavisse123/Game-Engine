/**
 * @file gameEngineServer.cpp
 * @author Daniel Avisse (djavisse)
 * This is the gameEngineServer program. This program allows for the SFML Game Engine
 * to support a multiplayer envirnomnet. This programs allows for multiple clients (At least 4)
 * to connect and play in a multiplayer environment where thte different clients can all see
 * each other iteract. This program maintains a gameState by using a json and will regularly update it
 * when new players join or when players request to be updated. Once players disconnect from the server 
 * all other clients should recieve information about that player's disconnection and should remove that player
 * from displaying in the game.
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

/**
 * This function allows for the PlatformThread to run. The PlatformThread handles
 * the movement pattern of the moving platform in background so the main thread in the
 * server can regularly handle replying to requests from different clients.
 * @param platformthread The platformthread object used to run the movingPlatform
 */
void run_Movement(PlatformThread *platformthread)
{
    //Continously run so the plaform can continuing moving in the background
    while (true) {
        platformthread->handleMovement();
    }
}

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

    //Create some MovingPlatforms that will move in the background for clients to display
    sf::Color green(0, 161, 11);
    MovingPlatform platform(340, 800, 225.0, 50.0, green, MOVE_HORIZONTAL, false);
    sf::Color blue(69, 212, 255);
    MovingPlatform platform2(1500, 800, 200.0, 75.0, blue, MOVE_VERTICAL, false); 

    std::vector<MovingPlatform *> listOfMovingPlats;
    listOfMovingPlats.push_back(&platform);
    listOfMovingPlats.push_back(&platform2);

    //Create a list of threadedObjects to run the MovingPlatform pattern in the background
    std::mutex m;
    std::condition_variable cv;
    std::vector<PlatformThread> listOfThreadObjs;
    int ID = 0;
    for(MovingPlatform* plat : listOfMovingPlats) {
        PlatformThread platThread(ID, NULL, &m, &cv, plat);
        listOfThreadObjs.push_back(platThread);
        ID++;
    }
    //Use the threaded objects and create acutal threads to run in the background
    std::thread listOfPlatThreads[listOfMovingPlats.size()];
    for(int i = 0; i < listOfMovingPlats.size(); i++) {
        listOfPlatThreads[i]= std::thread(run_Movement, &listOfThreadObjs[i]);
    }

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

    //MovingPlatform Information
    gameState["MovingPlatform X Size"] = {};
    gameState["MovingPlatform Y Size"] = {};
    gameState["MovingPlatform X Position"] = {};
    gameState["MovingPlatform Y Position"] = {};
    gameState["MovingPlatform Color"] = {};
    gameState["Number Of MovingPlatforms"] = listOfMovingPlats.size();    

    //Initialize the platform positions
    for(int i = 0; i < listOfMovingPlats.size(); i++) {
        gameState["MovingPlatform X Position"].push_back(listOfMovingPlats[i]->getPosition().x);
        gameState["MovingPlatform Y Position"].push_back(listOfMovingPlats[i]->getPosition().y);
        gameState["MovingPlatform X Size"].push_back(listOfMovingPlats[i]->getSize().x);
        gameState["MovingPlatform Y Size"].push_back(listOfMovingPlats[i]->getSize().y);
        gameState["MovingPlatform Color"].push_back(listOfMovingPlats[i]->getFillColor().toInteger());
    }

    //Static Platforms Information
    gameState["Static Platform X Size"] = {300, 500, 800};
    gameState["Static Platform Y Size"] = {100, 200, 25};
    gameState["Static Platform X Position"] = {0, 900, 550};
    gameState["Static Platform Y Position"] = {900, 800, 340};
    gameState["Static Platform Color"] = {0xff0000ff, 0xf7650aff, 0xff63d3ff};
    gameState["Number Of Static Platform"] = 3;

    //DeathZone Information
    gameState["DeathZone X Size"] = 1920;
    gameState["DeathZone Y Size"] = 75;
    gameState["DeathZone X Position"] = 0;
    gameState["DeathZone Y Position"] = 1180;

    //SideBoundaries Information
    //Left Boundary Index = 0
    //Right Boundary Index = 1
    //Top Boundary Index = 2
    //Bottom Boundary Index = 3
    gameState["SideBoundary X Size"] = {100, 100, 1920, 1920};
    gameState["SideBoundary Y Size"] = {1200, 1200, 100, 100};
    gameState["SideBoundary X Position"] = {-100, 500, 0, 0};
    gameState["SideBoundary Y Position"] = {0, 0, 700, 1080};
    gameState["Number Of SideBoundaries"] = 4;

    //SpawnPoint Information
    gameState["SpawnPoint X Position"] = 50;
    gameState["SpawnPoint Y Position"] = 800;
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
        
        //Regularly update the position of the moving platforms
        for(int i = 0; i < listOfMovingPlats.size(); i++) {
            gameState["MovingPlatform X Position"][i] = listOfMovingPlats[i]->getPosition().x;
            gameState["MovingPlatform Y Position"][i] = listOfMovingPlats[i]->getPosition().y;
        }

        //When a new client joins initialize them with the current state of the game
        // if (request.to_string() == newClient) {
        //     std::cout << "Received " << request.to_string() << std::endl;
        //     //Increment the client count when the client connects with the server
        //     clientCount++;
        //     gameState["Player Number"] = clientCount - 1;
        //     gameState["Player Count"] = clientCount;
        //     std::string playerNameString = "Player: " + std::to_string(clientCount);
        //     gameState["Player Name"].push_back(playerNameString);
        //     gameState["Player X Position"].push_back(10);
        //     gameState["Player Y Position"].push_back(50); 

        //     //Set the disconnection status for this client to be false.
        //     gameState["Disconnected Client"].push_back(false);    
        //     //Dump the contents of the json send it to the client
        //     std::string json_str = gameState.dump();
        //     zmq::message_t reply(json_str.size());
        //     memcpy(reply.data(), json_str.c_str(), json_str.size());
        //     communicationSocket.send(reply, zmq::send_flags::none);        
        // }
        // //Check for disconnections
        // else if(request.size() > 0) {
        //     //Parse the json sent from the client
        //     auto disconnect_str = std::string(static_cast<char*>(request.data()), request.size());
        //     auto disconnect_json = json::parse(disconnect_str);
        //     //Get the ID of the client
        //     int clientID = disconnect_json["Client Number"];
        //     //Increment the number of disconnected clients
        //     disconnectedClients++;
        //     //Set that a client with this ID has disconnected from the server.
        //     gameState["Disconnected Client"][clientID] = true;
        //     gameState["Disconnection Count"] = disconnectedClients;
        //     gameState["Last Disconnect"] = clientID;
        //     //Send a message back to the other client that they have disconnected
        //     communicationSocket.send(zmq::buffer(disconnection), zmq::send_flags::none);
        // }
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