#ifndef SERVER_H
#define SERVER_H

struct Server {
	int serverSocket;
	bool start(int port);
	void stop();
};


#endif // SERVER_H
