#ifndef SERVER_H
#define SERVER_H

struct Server {
	int serverSocket;
	bool start(int port);
	void stop();
	int waitForClient();
	int getSocket() { return serverSocket; }
};


#endif // SERVER_H
