#ifndef NETWORKING_H
#define NETWORKING_H

#include <cstdint>
#include <queue>
#include <unordered_map>
#include <vector>

struct PacketHeader {
	uint32_t packetType;
	uint32_t packetSize;
};

struct PaddleMovePacket {
	float deltaY;
};

struct BallPositionPacket {
	float x;
	float y;
};

struct PacketData {
	uint32_t sequenceNum;
	std::vector<char> data;
};

class PacketQueue {
	private:
		std::queue<PacketData> packetQueue;
		std::unordered_map<uint32_t, PacketData> receivedPackets;
		uint32_t nextExpectedSequenceNum = 0;
	public:
		void enqueuePacket(uint32_t sequenceNum, const std::vector<char>& data);
		bool hasPacketsAvailable() const;
		PacketData getNextPacket();
};

bool sendPacket(int socket, const void* packet, uint32_t packetSize);
bool receivePacket(int socket, void* buffer, uint32_t bufferSize, uint32_t& bytesRecv);
void sendPaddleMovePacket(int socket,  PaddleMovePacket& packet);
bool recvPaddleMovePacket(int socket, PaddleMovePacket& packet);
void sendBallPositionPacket(int socket, BallPositionPacket& packet);
bool recvBallPositionPacket(int socket, BallPositionPacket& packet);

void serialize(const PaddleMovePacket& packet, char* buffer);
void deserialize(const char* buffer, PaddleMovePacket& packet);
void serialize(const BallPositionPacket& packet, char* buffer);
void deserialize(const char* buffer, BallPositionPacket& packet);

#endif // NETWORKING_H
