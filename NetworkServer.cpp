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

    // Initialize list of Client Iteration trackers
    std::vector<ClientIteration> clientList = std::vector<ClientIteration>();

    std::cout << "Running server.\n";
    // initialize the zmq context with a single IO thread
    zmq::context_t context{ 2 };

    // construct a PUB (publisher) socket and broadcast information about entity movements to all clients
    zmq::socket_t serverToClientPublisher{ context, zmq::socket_type::pub };
    serverToClientPublisher.bind("tcp://*:5555");

    // construct a REP (reply) socket to send client identifier information back to client
    zmq::socket_t replyToClient{ context, zmq::socket_type::rep };
    replyToClient.bind("tcp://*:5556");

    // construct a SUB (subscriber) socket that gets player information from client
    zmq::socket_t clientToServerSubscriber{ context, zmq::socket_type::sub };
    clientToServerSubscriber.bind("tcp://*:5557");
    clientToServerSubscriber.set(zmq::sockopt::subscribe, "Server");

    /// Controller for all entities and their physics
    EntityHandler* entityHandler;
    entityHandler = new EntityHandler();

    // Instantiate entities
	// Create ball object (Temp)
	Entities::Entity *ball = new Entities::Entity
	(
        1.0, 1.0,
		550.0, 250.0,
		20.0, 20.0,
		10.0,
		"./Assets/Textures/BallTexture.png",
		true,
		false
	);
	// Create box object that moves (Temp)
	Entities::MovingEntity* movingBox = new Entities::MovingEntity
	(
        1.0, 1.0,
		550.0, 550.0,
		10.0,
		50.0f, 50.0f,
		"./Assets/Textures/devTexture0.png",
		false,
		false,
		true,
		false,
		10,
		500,
		800.0,
		800.0
	);
	// Create ground object (Temp)
	Entities::Entity* ground = new Entities::Entity(
        1.0, 1.0,
		250.0, 550.0,
		128.0, 64.0,
		10.0,
		"./Assets/Textures/devTexture1.png",
		true,
		false
	);

	// Create second ground object (Temp)
	Entities::Entity* platform = new Entities::Entity(
        1.0, 1.0,
		350.0, 525.0,
		64.0, 64.0,
		10.0,
		"./Assets/Textures/devTexture3.png",
		true,
		false
	);

    entityHandler->insertMovingEntity(*movingBox);

    // Send out multipart messages forever
    while (true) {

        // REQ model ready to receive requests from the client.
        zmq::message_t clientIdRequest;
        replyToClient.recv(clientIdRequest, zmq::recv_flags::dontwait);

        if (!clientIdRequest.empty()) {
            // Print request information from client
            std::cout << "Publisher: [" << clientIdRequest.to_string() << "]\n";
        }

        // If the client sends a request, start handling sending the reply
        if (!clientIdRequest.empty()) {
            clientIdentifierCounter++;
            std::cout << "Client [" << clientIdentifierCounter << "] connected.\n";

            // Cast clientIdentifierCounter to a char array to send to client
            char clientIdentifier[9] = "Client_";
            clientIdentifier[7] = static_cast<char>(clientIdentifierCounter + 48);
            clientIdentifier[8] = '\0';

            // Create a player Entity (Temp: Make more malleable in the future)
            Entities::Player player = Entities::Player(
                1.0, 1.0,
                250.0, 250.0,
                15.0, 25.0,
                50.0,
                "./Assets/Textures/DefaultPlayerTexture1.png",
                false,
                true,
                0.0f, -150.0f,
                6.0
            );

            // Add player to entity list
            int playerUUID = entityHandler->insertPlayer(player);
            player.setUUID(playerUUID);
   
            // Add new client to list of clients
            ClientIteration newClient = { clientIdentifierCounter, 0, {} };
            strncpy_s(newClient.m_identifier, clientIdentifier, sizeof(newClient.m_identifier));
            clientList.push_back(newClient);
            
            std::string clientString = std::to_string(clientIdentifierCounter);

            //std::cout << "Client Identifier Initialized: " << clientIdentifier << "\n";

            // Send the identifier, as well as Player object back to the client
            zmq::message_t msg("Client_" + std::to_string(clientIdentifierCounter) + "\n" + player.toString());
            replyToClient.send(msg, zmq::send_flags::none);

        }

        // Send all entity information to every client.
        zmq::message_t infoStr("Client\n" + entityHandler->toString());
        serverToClientPublisher.send(infoStr, zmq::send_flags::dontwait);

        for (ClientIteration& client : clientList) {
            client.m_iteration += 1;
        }

        // Receive client info
        zmq::message_t clientInfo;
        clientToServerSubscriber.recv(clientInfo, zmq::recv_flags::dontwait);
        if (!clientInfo.empty()) {
            //std::cout << "Received client identifier: " << clientInfo.to_string() << /*"," << clientInfo.to_string().length() <<*/ "\n";

            Entities::Player updatedPlayer = *Entities::Player::fromString(clientInfo.to_string());
            entityHandler->insertPlayer(updatedPlayer);
            std::cout << "updatedPlayer: " << updatedPlayer.toString() << /*"," << clientInfo.to_string().length() <<*/ "\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
