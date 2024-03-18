#ifndef SERVER_H
#define SERVER_H

struct Server {
	int serverSocket;
	int clientSocket;
	bool start(int port);
	void stop();
	int waitForClient();
	int getSocket() { return serverSocket; }
	int getClientSocket() { return clientSocket; }
	bool sendClientSocketFd(int clientSocket);
};


#endif // SERVER_H

