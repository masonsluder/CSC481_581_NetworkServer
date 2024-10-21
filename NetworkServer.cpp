#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <mutex>

#include "networkthread.h"
#include "entityHandler.h"
#include "networkEntity.h"
#include "networkMovingEntity.h"
#include "networkPlayer.h"
#include "networkTimeline.h"
#include "n_gameObjectManager.h"

#include "n_MovingObject.h"
#include "n_playerGO.h"

#include "json.hpp"
using json = nlohmann::json;

// Struct that stores simple client identification information
typedef struct {
    int m_id;
    int m_iteration;
    char m_identifier[9];
} ClientIteration;


/**
* The main method that runs the server-client connection for the GameEngine. Keeps 
* track of important movements and corrects abnormalities between client and server.
* This code was originally referencing the link below, but was changed drastically in 
* order to fulfill the requirements of the Homework. It was used primarily as a tutorial
* for the use of the PUB-SUB interaction between server and client: 
* https://zeromq.org/socket-api/?language=cpp&library=cppzmq#publish-subscribe-pattern
*/
int main(int argc, char* argv[]) {
    int clientIteration = 0;
	int clientIdentifierCounter = 0;

    int networkConfigurationSetting = 1;

    // Initialize list of Client Iteration trackers
    std::vector<ClientIteration> clientList = std::vector<ClientIteration>();

    std::cout << "Running server.\n";
    // initialize the zmq context with a single IO thread
    zmq::context_t context{ 2 };

    // construct a PUB (publisher) socket and broadcast information about entity movements to all clients
    zmq::socket_t serverToClientPublisher{ context, zmq::socket_type::pub };

    // Set conflate to only take most recent message
    int conflate = 1;
    zmq_setsockopt(serverToClientPublisher, ZMQ_CONFLATE, &conflate, sizeof(conflate));
    serverToClientPublisher.bind("tcp://*:5555");

    // construct a REP (reply) socket to send client identifier information back to client
    zmq::socket_t replyToClient{ context, zmq::socket_type::rep };
    replyToClient.bind("tcp://*:5556");

    // construct a SUB (subscriber) socket that gets player information from client
    zmq::socket_t clientToServerSubscriber{ context, zmq::socket_type::sub };
    clientToServerSubscriber.bind("tcp://*:5557");
    clientToServerSubscriber.set(zmq::sockopt::subscribe, "Server");
   
    Timeline* timeline = new Timeline();

    /// Controller for all entities and their physics
    EntityHandler* entityHandler;
    entityHandler = new EntityHandler();

    N_GameObjectManager* gameObjectManager;
    gameObjectManager = new N_GameObjectManager(timeline);

    N_MovingObject* movingBox = new N_MovingObject(
        1.0f, 1.0f,
        550.0f, 550.0f,
        32.0f, 32.0f,
        10.0f,
		"./Assets/Textures/devTexture0.png",
		Utils::Vector2D(250.0f, 350.0f),
		Utils::Vector2D(600.0f, 350.0f),
        40.0f,
        10
    );

    N_MovingObject* movingBox2 = new N_MovingObject(
        1.0f, 1.0f,
        550.0f, 550.0f,
        100.0f, 16.0f,
        10.0f,
        "./Assets/Textures/devLongMoveTexture.png",
        Utils::Vector2D(150.0f, 250.0f),
        Utils::Vector2D(150.0f, 800.0f),
        90.0f,
        10
    );

    N_MovingObject* movingBox3 = new N_MovingObject(
        1.0f, 1.0f,
        550.0f, 550.0f,
        100.0f, 16.0f,
        10.0f,
        "./Assets/Textures/devLongMoveTexture.png",
        Utils::Vector2D(300.0f, 500.0f),
        Utils::Vector2D(600.0f, 800.0f),
        90.0f,
        10
    );

    gameObjectManager->insert(movingBox);
    gameObjectManager->insert(movingBox2);
    gameObjectManager->insert(movingBox3);

 //   // Instantiate entities
	//// Create box object that moves (Temp)
	//Entities::MovingEntity* movingBox = new Entities::MovingEntity
	//(
 //       1.0, 1.0,
	//	550.0, 550.0,
	//	10.0,
	//	50.0f, 50.0f,
	//	"./Assets/Textures/devTexture0.png",
	//	false,
	//	false,
	//	true,
	//	false,
	//	10,
	//	50,
	//	800.0,
	//	800.0
	//);

    //entityHandler->insertMovingEntity(*movingBox);

    // Send out multipart messages forever
    while (true) {
        // Update timeline
        timeline->updateTime();

        // Update entity movements
        entityHandler->updateEntities();

        // Update GameObjects
        gameObjectManager->update();

        // REQ model ready to receive requests from the client.
        zmq::message_t clientIdRequest;
        replyToClient.recv(clientIdRequest, zmq::recv_flags::dontwait);
        // 1 = Client to server, 2 = Peer to Peer
       
        if (!clientIdRequest.empty()) {
            networkConfigurationSetting = (int)clientIdRequest.str()[0] - 48;
            // Print request information from client
            std::cout << "Publisher: [" << clientIdRequest.to_string() << "]\n";
        }

        // If the client sends a request, start handling sending the reply
        if (!clientIdRequest.empty()) {
            clientIdentifierCounter++;
            //std::cout << "Client [" << clientIdentifierCounter << "] connected.\n";

            // Cast clientIdentifierCounter to a char array to send to client
            char clientIdentifier[9] = "Client_";
            clientIdentifier[7] = static_cast<char>(clientIdentifierCounter + 48);
            clientIdentifier[8] = '\0';

            // Create a player Entity (Temp: Make more malleable in the future)
            //Entities::Player player = Entities::Player(
            //    1.0, 1.0,
            //    250.0, 250.0,
            //    15.0, 25.0,
            //    50.0,
            //    "./Assets/Textures/DefaultPlayerTexture1.png",
            //    false,
            //    true,
            //    0.0f, -7000.0f,
            //    100.0f
            //);

            
            // Create player game object
            N_PlayerGO *playerGO = new N_PlayerGO(
                1.0f, 1.0f,
                250.0f, 250.0f,
                15.0f, 25.0f,
                50.0f,
                "./Assets/Textures/DefaultPlayerTexture1.png",
                false,
                0.0f, -2000.0f,
                100.0f
            );

            // Add player to entity list
            //int playerUUID = entityHandler->insertPlayer(player);
            //player.setUUID(playerUUID);

            // Add player to GameObjectManager
            gameObjectManager->insert(playerGO);
            gameObjectManager->insertPlayer(playerGO);
   
            // Add new client to list of clients
            ClientIteration newClient = { clientIdentifierCounter, 0, {} };
            strncpy_s(newClient.m_identifier, clientIdentifier, sizeof(newClient.m_identifier));
            clientList.push_back(newClient);
            
            std::string clientString = std::to_string(clientIdentifierCounter);

            //std::cout << "Client Identifier Initialized: " << clientIdentifier << "\n";

            // Send the identifier, as well as Player object back to the client

            // Generate json_string for the player
            json j;
            playerGO->to_json(j);

            //zmq::message_t msg("Client_" + std::to_string(clientIdentifierCounter) + "\n" + player.toString());
            zmq::message_t msg("Client_" + std::to_string(clientIdentifierCounter) + "\n" + j.dump());
            replyToClient.send(msg, zmq::send_flags::none);
        }
        // Send all entity information to every client.
        std::stringstream ss;
        ss.str("");
        ss << std::clock() << "\n";
        std::string gameObjectString;
        gameObjectManager->serialize(gameObjectString, true);
        zmq::message_t infoStr(ss.str() + gameObjectString/*entityHandler->toString()*/);
        zmq_connect(serverToClientPublisher, "tcp://:5555");
        serverToClientPublisher.send(infoStr, zmq::send_flags::dontwait);
        zmq_disconnect(serverToClientPublisher, "tcp://:5555");

        //// Receive client info
        //zmq::message_t clientInfo;
        //clientToServerSubscriber.recv(clientInfo, zmq::recv_flags::dontwait);
        //if (!clientInfo.empty()) {
        //    // Create player from string and update their information
        //    Entities::Player updatedPlayer = *Entities::Player::fromString(clientInfo.to_string());
        //    // Insert and update/create a new player into the Entity maps
        //    entityHandler->insertPlayer(updatedPlayer);
        //}

		// Make so that server only sends every 1/40th of a second or so
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    return 0;
}
