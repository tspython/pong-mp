#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno> 
#include "client.h"

bool Client::connect(const char* ip, int port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error: Failed to create socket: " << strerror(errno) << "\n";
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Error: Invalid address or address not supported: " << strerror(errno) << "\n";
        close(clientSocket);
        return false;
    }

    if (::connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Failed to connect to server: " << strerror(errno) << "\n";
        close(clientSocket);
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

