/**
 * @file ServerHandler.cpp
 * @author Daniel Avisse (djavisse)
 * This is the ServerHandler class. This class handlers any events generated in
 * the server.
 */
#include "ServerHandler.h"
#include "nlohmann/json.hpp"

/**
 * Constructs a ServerHandler component using a super to the EventHandler
 * @param parentObject A reference to the object using the ServerHandler
 */
ServerHandler::ServerHandler(void * parentObject) : EventHandler(parentObject) {

}

void ServerHandler::onEvent(Event e) {
    using json = nlohmann::json;
    json *gameStatePointer = static_cast<json *>(parentObject);
    json gameStateJSON = *gameStatePointer;
    switch(e.type) {
        case Event::SERVER_CONNECTION:
        {
            //Prepare some information to initialize the client
            gameStateJSON["Player Number"] = e.server.clientCount - 1;
            gameStateJSON["Player Count"] = e.server.clientCount;
            std::string playerNameString = "Player: " + std::to_string(e.server.clientCount);
            gameStateJSON["Player Name"].push_back(playerNameString);
            gameStateJSON["Player X Position"].push_back(365);
            gameStateJSON["Player Y Position"].push_back(750); 
            gameStateJSON["Current Level"].push_back(1);
            //Set the disconnection status for this client to be false.
            gameStateJSON["Disconnected Client"].push_back(false);
            *gameStatePointer = gameStateJSON;
            break;
        }
        case Event::SERVER_DISCONNECTION:
        {
            //Set that a client with this ID has disconnected from the server.
            gameStateJSON["Disconnected Client"][e.server.clientID] = true;
            gameStateJSON["Disconnection Count"] = e.server.disconnectedClients;
            gameStateJSON["Last Disconnect"] = e.server.clientID;
            *gameStatePointer = gameStateJSON;
            break;
        }
    }
}