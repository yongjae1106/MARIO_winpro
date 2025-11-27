#pragma once
#include "PacketInfo.h"
#include <vector>

class PacketManager
{
public:
    int TryParse(const std::vector<char>& buffer, unsigned int socketID);
    void HandlePacket(unsigned int type, const char* data, unsigned int length, unsigned int socketID);

    // 서버 → 클라 직렬화
    unsigned int Serialize_MOVE(char* buffer, const Packet_MOVE_S2C& move);
    unsigned int Serialize_HIT(char* buffer, const Packet_HIT_S2C& hit);
    unsigned int Serialize_BLOCK(char* buffer, const Packet_BLOCK_S2C& blk);
};