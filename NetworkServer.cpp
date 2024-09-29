#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>

//#include "zhelpers.hpp"

int clientIteration = 0;
int clientIdentifierCounter = 0;

typedef struct {
    int m_id;
    int m_iteration;
} ClientIteration;

int main(int argc, char* argv[]) {

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
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        if (keyboardState[SDL_SCANCODE_ESCAPE]) {
            SDL_Quit();  // Clean up SDL
            zmq_ctx_destroy((void*)context);
            exit(0);
        }

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

            // Add new client to list of clients
            clientList.push_back(ClientIteration() = { clientIdentifierCounter, 0 });

            std::string clientString = std::to_string(clientIdentifierCounter);

            // Send back to the client
            zmq::message_t msg(clientString);
            reply.send(msg, zmq::send_flags::none);

            // TODO Create new thread for newly connected client

            // Cast clientIdentifierCounter to a char array to send to client
            char clientIdentifier[2];
            clientIdentifier[0] = static_cast<char>(clientIdentifierCounter + 48);
            clientIdentifier[1] = '\0';

            std::this_thread::sleep_for(std::chrono::seconds(1));
            // Handle publishing to clients (in their branch using each of their identifiers)
            //publisher.send(zmq::str_buffer(clientIdentifier), zmq::send_flags::sndmore);
            //publisher.send(zmq::str_buffer("Message in status"));
        }

        // Iterate throught the list of clients and broadcast each of their iteration numbers to each of the clients
        //std::cout << "Iterating through ClientIterations\n";
        for (ClientIteration client : clientList) {
            //std::cout << "Looping: " << client.m_id << "\n";
            // Creates stringstream for string building
            std::stringstream ss;
            // Prints out Client X: Iteration Y, then increments Y
            ss << "Client " << client.m_id << ": " << "Iteration " << client.m_iteration << "\n";
            client.m_iteration++;
            // Broadcast message to clients
            zmq::message_t iterationStr(ss.str());
            publisher.send(iterationStr, zmq::send_flags::dontwait);
        }

        /*if (client_joins()) {
            publisher.send(clientIdentifierCounter);
            clientIdentifierCounter++;
        }*/
    }

    return 0;
}
