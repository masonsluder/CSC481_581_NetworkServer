#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>
#include "networkthread.h"
#include <mutex>


typedef struct {
    int m_id;
    int m_iteration;
    char m_identifier[9];
    std::thread m_thread;
} ClientIteration;

int main(int argc, char* argv[]) {

	int clientIteration = 0;
	int clientIdentifierCounter = 0;

    // Initialize list of Client Iteration trackers
    std::vector<ClientIteration> clientList = std::vector<ClientIteration>();

    std::cout << "Running server.\n";
    // initialize the zmq context with a single IO thread
    zmq::context_t context{ 2 };

    // construct a PUB (publisher) socket and connect to interface
    zmq::socket_t publisher{ context, zmq::socket_type::pub };
    publisher.bind("tcp://*:5555");

    // construct a REP (reply) socket
    zmq::socket_t reply{ context, zmq::socket_type::rep };
    reply.bind("tcp://*:5556");
    //reply.set(zmq::sockopt::subscribe, "Subscribe");

    // Send out multipart messages forever
    while (true) {

        // REQ model ready to receive requests from the client.
        zmq::message_t clientIdRequest;
        reply.recv(clientIdRequest, zmq::recv_flags::dontwait);

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

            // TODO Create new thread for newly connected client?
            // Mutex to handle locking, condition variable to handle notifications between threads
            std::mutex m;
            std::condition_variable cv;

            // Create thread objects
            NetworkThread clientThread(0, NULL, &m, &cv);

            // Start thread
            std::thread first(&NetworkThread::run, &clientThread);

            // Add new client to list of clients
            ClientIteration newClient = { clientIdentifierCounter, 0, {}, std::move(std::thread(&NetworkThread::run, clientThread)) };
            strncpy_s(newClient.m_identifier, clientIdentifier, sizeof(newClient.m_identifier));
            clientList.push_back(newClient);
            
            std::string clientString = std::to_string(clientIdentifierCounter);

            std::cout << "Client Identifier Initialized: " << clientIdentifier << "\n";

            //assert("Client_" + std::to_string(clientIdentifierCounter) == "Client_1");

            // Send the identifier back to the client
            zmq::message_t msg("Client_" + std::to_string(clientIdentifierCounter));
            reply.send(msg, zmq::send_flags::none);
        }

        // Iterate throught the list of clients and broadcast each of their iteration numbers to each of the clients
        //std::cout << "Iterating through ClientIterations\n";
        for (ClientIteration& client : clientList) {
            // Handle publishing to clients (in their branch using each of their identifiers)
            //std::cout << "Looping: " << client.m_id << "\n";
			client.m_iteration += 1;
            // Creates stringstream for string building
            std::stringstream ss;
            // Prints out Client X: Iteration Y, then increments Y
            ss << " " << client.m_id << " Iteration " << client.m_iteration;

            //std::cout << client.m_identifier << ": " << "Iteration " << client.m_iteration << "\n";
            // Broadcast message to clients
            zmq::message_t iterationStr(ss.str());
            
            // Send to one client (FOR SECTION 5)
            publisher.send(zmq::str_buffer(client.m_identifier), zmq::send_flags::sndmore);

            // Send to all clients
            //publisher.send(zmq::str_buffer("Client"), zmq::send_flags::sndmore);
            publisher.send(iterationStr, zmq::send_flags::dontwait);
        }

        /*if (client_joins()) {
            publisher.send(clientIdentifierCounter);
            clientIdentifierCounter++;
        }*/

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
