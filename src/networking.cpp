#include "networking.h"
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream> // For error logging

void serialize(const PaddleMovePacket& packet, char* buffer) {
    std::memcpy(buffer, &packet, sizeof(PaddleMovePacket));
}

void deserialize(const char* buffer, PaddleMovePacket& packet) {
    std::memcpy(&packet, buffer, sizeof(PaddleMovePacket));
}

void serialize(const BallPositionPacket& packet, char* buffer) {
    std::memcpy(buffer, &packet, sizeof(BallPositionPacket));
}

void deserialize(const char* buffer, BallPositionPacket& packet) {
    std::memcpy(&packet, buffer, sizeof(BallPositionPacket));
}

bool sendPacket(int socket, const void* packet, uint32_t packetSize) {
    uint32_t bytesSent = 0;
    while (bytesSent < packetSize) {
        int result = send(socket, reinterpret_cast<const char*>(packet) + bytesSent, packetSize - bytesSent, 0);
        if (result == -1) {
            std::cerr << "Error sending packet: " << strerror(errno) << std::endl;
            return false;
        }
        bytesSent += static_cast<uint32_t>(result);
    }
    return true;
}

bool receivePacket(int socket, void* buffer, uint32_t bufferSize, uint32_t& bytesRecv) {
    bytesRecv = 0;
    while (bytesRecv < bufferSize) {
        int result = recv(socket, reinterpret_cast<char*>(buffer) + bytesRecv, bufferSize - bytesRecv, 0);
        if (result == -1) {
            std::cerr << "Error receiving packet: " << strerror(errno) << std::endl;
            return false;
        } else if (result == 0) {
            return true; // Connection closed
        }

        bytesRecv += static_cast<uint32_t>(result);
    }
    return true;
}

void sendPaddleMovePacket(int socket, PaddleMovePacket& packet) {
    char buffer[sizeof(PaddleMovePacket)];
    serialize(packet, buffer);
    sendPacket(socket, buffer, sizeof(PaddleMovePacket));
}

bool recvPaddleMovePacket(int socket, PaddleMovePacket& packet) {
    char buffer[sizeof(PaddleMovePacket)];
    uint32_t bytesRecv;
    if (receivePacket(socket, buffer, sizeof(PaddleMovePacket), bytesRecv)) {
        if (bytesRecv == sizeof(PaddleMovePacket)) {
            deserialize(buffer, packet);
            return true;
        }
    }
    return false;
}

void sendBallPositionPacket(int socket, BallPositionPacket& packet) {
    char buffer[sizeof(BallPositionPacket)];
    serialize(packet, buffer);
    sendPacket(socket, buffer, sizeof(BallPositionPacket));
}

bool recvBallPositionPacket(int socket, BallPositionPacket& packet) {
    char buffer[sizeof(BallPositionPacket)];
    uint32_t bytesRecv;
    if (receivePacket(socket, buffer, sizeof(BallPositionPacket), bytesRecv)) {
        if (bytesRecv == sizeof(BallPositionPacket)) {
            deserialize(buffer, packet);
            return true;
        }
    }
    return false;
}

// Packet Queue Implentation
void PacketQueue::enqueuePacket(uint32_t sequenceNum, const std::vector<char>& data) {
    PacketData packet = { 
        sequenceNum, 
        data
    };
    
    receivedPackets[sequenceNum] = packet;

    while(!receivedPackets.empty() && receivedPackets.count(nextExpectedSequenceNum)) {
        packetQueue.push(receivedPackets[nextExpectedSequenceNum]);
        receivedPackets.erase(nextExpectedSequenceNum);
        nextExpectedSequenceNum++;
    }
}

bool PacketQueue::hasPacketsAvailable() const {
    return !packetQueue.empty();
}

PacketData PacketQueue::getNextPacket() {
    PacketData packet = packetQueue.front();
    packetQueue.pop();
    return packet;
}
