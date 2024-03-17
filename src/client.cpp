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
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0) {
        // Error handling
        return false;
    }

    if (::connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        // Error handling
        return false;
    }

    std::cout << "Connected to server\n";
    return true; 
}

void Client::disconnect() {
    if (clientSocket != -1) {
        close(clientSocket);
        std::cout << "Disconnected from server\n";
    }
}

void Client::setServerSocket(int socket) {
    serverSocket = socket;
}

int Client::getSocket() const {
    return clientSocket;
}

