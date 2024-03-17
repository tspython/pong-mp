#ifndef NETWORKING_H
#define NETWORKING_H

#include <cstdint>

struct PaddleMovePacket {
	float deltaY;
};

struct BallPositionPacket {
	float x;
	float y;
};	

bool sendPacket(int socket, const void* packet, uint32_t packetSize);
bool receivePacket(int socket, void* buffer, uint32_t bufferSize, uint32_t& bytesRecv);
void sendPaddleMovePacket(int socket,  const PaddleMovePacket& packet);
bool recvPaddleMovePacket(int socket, PaddleMovePacket& packet);
void sendBallPositionPacket(int socket, const BallPositionPacket& packet);
bool recvBallPositionPacket(int socket, BallPositionPacket& packet);

void serialize(const PaddleMovePacket& packet, char* buffer);
void deserialize(const char* buffer, PaddleMovePacket& packet);
void serialize(const BallPositionPacket& packet, char* buffer);
void deserialize(const char* buffer, BallPositionPacket& packet);

#endif // NETWORKING_H
