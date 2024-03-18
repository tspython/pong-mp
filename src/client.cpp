// client.cpp

#include "client.h"

Client::Client() : clientSocket(-1), serverSocket(-1) {}

Client::~Client() {
    disconnect();
}

bool Client::connect(const char* ip, int port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0) {
        return -1;
    }

    int serverDescriptor = ::connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (serverDescriptor == -1) {
        return -1;
    }

    if (!receiveServerSocketFd()) {
        std::cerr << "Error receiving server socket fd from server\n";
        return false;
    }
    int tmp = clientSocket;
    clientSocket = serverSocket;
    serverSocket = tmp;
    return true;
}

bool Client::receiveServerSocketFd() {
    int fd; 
    if (recv(clientSocket, &fd, sizeof(fd), 0) == -1) {
        std::cerr << "Error receiving server socket fd from server\n";
        return false;
    }
    serverSocket = fd;
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
    return serverSocket;
}

