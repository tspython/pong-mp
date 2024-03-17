// client.cpp

#include "client.h"

Client::Client() : clientSocket(-1), serverSocket(-1) {}

Client::~Client() {
    disconnect();
}

bool Client::connect(const char* ip, int port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        // Error handling
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0) {
        // Error handling
        return -1;
    }

    int serverDescriptor = ::connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (serverDescriptor == -1) {
        // Error handling
        return -1;
    }

    // Store the server socket descriptor
    serverSocket = clientSocket;
    return true;
}


void Client::disconnect() {
    close(clientSocket);
    std::cout << "Disconnected from server\n";
}

int Client::getSocket() const {
    return clientSocket;
}

int Client::getServerSocket() const { 
    return serverSocket; // Return the stored server socket
}

