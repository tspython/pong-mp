#include <iostream>
#include <string>
#include "game.h"
#include "networking.h"
#include "server.h"
#include "client.h"

std::pair<int, int> initialize(const std::string& mode) {
    int serverSocket = -1;
    int clientSocket = -1;

    if (mode == "server") {
        Server *server = new Server();
        if (!server -> start(8080)) { 
            std::cerr << "Failed to start server\n";
            exit(-1);
        }
        serverSocket = server -> getSocket();
        clientSocket = server -> waitForClient();
    } 
    else if (mode == "client") {
        Client *client = new Client();
        if (!client->connect("127.0.0.1", 8080)) {
            std::cerr << "Failed to connect to server\n";
            delete client;             exit(-1);
        }
        clientSocket = client->getSocket();
        serverSocket = client->getServerSocket();

    } 
    else {
        std::cerr << "Invalid mode. Please specify either 'server' or 'client'\n";
        exit(-1);
    }

    return std::make_pair(serverSocket, clientSocket);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <mode>\n";
        return -1;
    }

    std::string mode = argv[1];

    auto sockets = initialize(mode);
    int serverSocket = sockets.first;
    int clientSocket = sockets.second;

    if (serverSocket == -1 || clientSocket == -1) {
        std::cerr << "Failed to initialize sockets\n";
        return -1;
    }
    // TODO: Pass in packet queue's to game loop
    if (mode == "server") {
        PacketQueue *serverPQ = new PacketQueue();
        init(true, serverSocket, clientSocket);
    } 
    else if (mode == "client") {
        PacketQueue *clientPQ = new PacketQueue();
        init(false, serverSocket, clientSocket);
    }

    return 0;
}

