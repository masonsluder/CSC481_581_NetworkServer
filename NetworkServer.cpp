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
    zmq::context_t context{ 3 };

    // Thread safe map to manage client threads
    std::unordered_map<int, std::thread> clientThreads;
    std::mutex clientThreadsMutex;

    // construct a REP (reply) socket to send client identifier information back to client
    zmq::socket_t replyToClient{ context, zmq::socket_type::rep };
    replyToClient.bind("tcp://*:5556");

    Timeline* timeline = new Timeline();

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

    struct ClientState {
        std::atomic<bool> active{ true };
        std::chrono::time_point<std::chrono::steady_clock> lastHeartbeat;
    };

    std::unordered_map<int, ClientState*> clientStates;
    std::mutex clientStateMutex;

    auto handleClient = [&](int clientIdentifierCounter, N_PlayerGO* playerGO) {
        int portNum = 6658 + clientIdentifierCounter;
        int portNum2 = 6558 + clientIdentifierCounter;

        //std::stringstream ss;
        //ss << "tcp://*:" << portNum;
        //std::stringstream ss2;
        //ss2 << "tcp://*:" << portNum2;

        const std::string ss = "tcp://*:" + std::to_string(portNum);
        const std::string ss2 = "tcp://*:" + std::to_string(portNum2);

        // Sockets to send and receive messages from client
        // (Receive only for Client-Server network setting)
        zmq::socket_t serverToClientPublisher{ context, zmq::socket_type::pub };
        zmq::socket_t clientToServerSubscriber{ context, zmq::socket_type::sub };

        // Subscriber for a sub socket that receives disconnect commands from the clients
        zmq::socket_t disconnectSocket{ context, zmq::socket_type::rep };
        

        // Set conflate to only take most recent message
        int conflate = 1;
        zmq_setsockopt(serverToClientPublisher, ZMQ_CONFLATE, &conflate, sizeof(conflate));
        
        // Bind publisher to client's port
        serverToClientPublisher.bind(ss);

        // Bind subscriber to client's port
        if (networkConfigurationSetting == 1) {
            clientToServerSubscriber.bind(ss2);
        }
        disconnectSocket.bind("tcp://*:" + std::to_string(4558 + playerGO->getUUID()));
        
        // Subscribe to messages related to client
        clientToServerSubscriber.set(zmq::sockopt::subscribe, "");
        
        //disconnectSocket.set(zmq::sockopt::reply, std::to_string(playerGO->getUUID()));

        std::cout << "Server bound to ports: " << portNum << " and " << portNum2 << "\n";

        auto& clientState = clientStates[clientIdentifierCounter];
        clientState->lastHeartbeat = std::chrono::steady_clock::now();

        // Main loop to send and receive messages for client
        while (clientState->active) {
            // TODO: Add receiving client messages for Client-Server network setting
            if (networkConfigurationSetting == 1) {
                zmq::message_t clientInfo;
                clientToServerSubscriber.recv(clientInfo, zmq::recv_flags::dontwait);
                if (!clientInfo.empty()) {
                    //std::cout << "SUB FROM CLIENT: " << clientInfo.to_string() << "\n";
                    clientState->lastHeartbeat = std::chrono::steady_clock::now();
                    gameObjectManager->deserializeClient(clientInfo.to_string(), 1);
                }
            }
            else if (networkConfigurationSetting == 2) {
                // If the disconnect message was retrieved, then set active to false
                zmq::message_t disconnectMessage;
                disconnectSocket.recv(disconnectMessage, zmq::recv_flags::dontwait);

                if (!disconnectMessage.empty()) {
                    std::cout << "Client_" << clientIdentifierCounter << " disconnected due to timeout.\n";
                    clientState->active = false;
                    break;
                }
            }

            // Send all entity information to every client.
            std::string gameObjectString;
            gameObjectManager->serialize(gameObjectString, true);
            zmq::message_t msg("Client_" + std::to_string(clientIdentifierCounter) + "\n" + gameObjectString);

            zmq::send_result_t result = serverToClientPublisher.send(msg, zmq::send_flags::dontwait);
            if (result) {
                //std::cout << "Result: " << result.value() << "\n";
                //clientState->lastHeartbeat = std::chrono::steady_clock::now();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(15));

            // Check for client timeout (5 seconds)
            if (networkConfigurationSetting == 1 && std::chrono::steady_clock::now() - clientState->lastHeartbeat > std::chrono::seconds(5)) {
                std::cout << "Client_" << clientIdentifierCounter << " disconnected due to timeout.\n";
                clientState->active = false;
            }
        }

        // Clean up client
        {
            std::lock_guard<std::mutex> lock(clientThreadsMutex);
            //clientThreads[clientIdentifierCounter].;
            clientThreads.erase(clientIdentifierCounter);
        }

        // Remove client game objects
        gameObjectManager->terminateClient(playerGO->getUUID());
        zmq::message_t disconnectMsg("Successfully deleted client " + std::to_string(clientIdentifierCounter));
        disconnectSocket.send(disconnectMsg, zmq::send_flags::none);
    };

    // Send out multipart messages forever
    while (true) {
        // Update timeline
        timeline->updateTime();

        // Update GameObjects
        gameObjectManager->update();

        // REQ model ready to receive requests from the client.
        zmq::message_t clientIdRequest;
        replyToClient.recv(clientIdRequest, zmq::recv_flags::dontwait);
        // 1 = Client to server, 2 = Peer to Peer

        if (!clientIdRequest.empty()) {
            networkConfigurationSetting = std::stoi(clientIdRequest.to_string());
            // Print request information from client
            std::cout << "Publisher: [" << clientIdRequest.to_string() << "]\n";
            std::cout << "NetworkSetting: [" << networkConfigurationSetting << "]\n";
        }

        // If the client sends a request, start handling sending the reply
        if (!clientIdRequest.empty()) {
            clientIdentifierCounter++;

            char clientIdentifier[9] = "Client_";
            clientIdentifier[7] = static_cast<char>(clientIdentifierCounter + 48);
            clientIdentifier[8] = '\0';

            // Create player game object
            N_PlayerGO* playerGO = new N_PlayerGO(
                1.0f, 1.0f,
                250.0f, 250.0f,
                15.0f, 25.0f,
                100.0f,
                "./Assets/Textures/DefaultPlayerTexture1.png",
                false,
                0.0f, -3000.0f,
                100.0f
            );

            // Add player to GameObjectManager
            gameObjectManager->insert(playerGO);
            gameObjectManager->insertPlayer(playerGO);

            // Add new client to list of clients
            ClientIteration newClient = { clientIdentifierCounter, 0, {} };
            strncpy_s(newClient.m_identifier, clientIdentifier, sizeof(newClient.m_identifier));
            clientList.push_back(newClient);

            // Generate json_string for the player
            json j;
            playerGO->to_json(j);
            zmq::message_t msg("Client_" + std::to_string(clientIdentifierCounter) + "\n" + j.dump());
            replyToClient.send(msg, zmq::send_flags::none);

            // Start new thread to handle client
            {
                std::lock_guard<std::mutex> lock(clientThreadsMutex);
                clientThreads[clientIdentifierCounter] = std::thread(handleClient, clientIdentifierCounter, playerGO);
                clientThreads[clientIdentifierCounter].detach(); // Run thread independently
            }
            // Handle client disconnect
            {
                std::lock_guard<std::mutex> lock(clientStateMutex);
                clientStates[clientIdentifierCounter] = new ClientState();
            }
        }

        // Remove disconnected client
        {
            std::lock_guard<std::mutex> lock(clientStateMutex);
            for (auto it = clientStates.begin(); it != clientStates.end();) {
                if (!it->second->active) {
                    std::cout << "Cleaning up client: " << it->first << "\n";
                    it = clientStates.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        // Make so that server only sends every 1/40th of a second or so
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    return 0;
}
