#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
