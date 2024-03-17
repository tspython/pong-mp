// client.h

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

class Client {
private:
    int clientSocket;
    int serverSocket; // Add member variable to store server socket

public:
    Client();
    ~Client();
    bool connect(const char* ip, int port);
    void disconnect();
    int getSocket() const;
    int getServerSocket() const;
};

