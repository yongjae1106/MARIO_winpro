#pragma once
#include "PacketInfo.h"
#include <vector>

class PacketManager
{
public:
    int ProcessPacket(const std::vector<char>& buffer);

private:
    void HandlePacket(unsigned int type, const char* data, unsigned int length);

    // 서버는 직렬화가 선택 사항
    unsigned int Serialize_MOVE(char* buffer, const Packet_MOVE_S2C& move);
    unsigned int Serialize_HIT(char* buffer, const Packet_HIT_S2C& hit);
};