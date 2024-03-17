#include <iostream>
#include <string>
#include "game.h"
#include "server.h"
#include "client.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <mode>\n";
        return -1;
    }

    std::string mode = argv[1];

    if (mode == "server") {
        Server server;
        if (!server.start(8080)) {
            std::cerr << "Failed to start server\n";
            return -1;
        }
        init();         
	server.stop();
    } 
    else if (mode == "client") {
        Client client;
        if (!client.connect("127.0.0.1", 8080)) {
            std::cerr << "Failed to connect to server\n";
            return -1;
        }
        init();         
	client.disconnect();
    } 
    else {
        std::cerr << "Invalid mode. Please specify either 'server' or 'client'\n";
        return -1;
    }

    return 0;
}


