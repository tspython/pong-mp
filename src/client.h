#ifndef CLIENT_H
#define CLIENT_H

struct Client {
	int clientSocket;
	bool connect(const char* ip, int port);
	void disconnect();
};

#endif // CLIENT_H

