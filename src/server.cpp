#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include "server.h"

bool Server::start(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error: Failed to create socket\n";
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Failed to bind to port " << port << "\n";
        return false;
    }

    if (listen(serverSocket, 5) == -1) {        
        std::cerr << "Error: Failed to listen on socket\n";
        return false;
    }

    std::cout << "Server listening on port " << port << std::endl;
    return true;
}

void Server::stop() {
    close(serverSocket);
}

int Server::waitForClient() {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(serverSocket, &readfds);

    // Timeout for select in seconds
    struct timeval timeout;
    timeout.tv_sec = 10;  // Adjust this value as needed
    timeout.tv_usec = 0;

    // Wait for activity on the server socket
    int activity = select(serverSocket + 1, &readfds, NULL, NULL, &timeout);
    if (activity == -1) {
        std::cerr << "Error: select() failed\n";
        return -1;
    } else if (activity == 0) {
        std::cerr << "Error: select() timed out\n";
        return -1;
    }

    // Check if there is incoming client connection
    if (FD_ISSET(serverSocket, &readfds)) {
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Error: Failed to accept client connection\n";
            return -1;
        }

        std::cout << "Client connected\n";
        return clientSocket;
    }

    return -1; // Should not reach here under normal circumstances
}
