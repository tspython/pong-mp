#ifndef CLIENT_H
#define CLIENT_H

class Client {
private:
    int clientSocket;
    int serverSocket; 
public:
    Client();
    ~Client();
    bool connect(const char* ip, int port);
    void disconnect();
    void setServerSocket(int socket);     
	int getSocket() const;
};

#endif

