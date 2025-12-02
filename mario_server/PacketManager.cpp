#include "PacketManager.h"
#include <cstring>
#include <iostream>

int PacketManager::ProcessPacket(const std::vector<char>& buffer)
{
    if (buffer.size() < sizeof(PacketHeader))
        return 0;

    PacketHeader header;
    memcpy(&header, buffer.data(), sizeof(PacketHeader));

    if (buffer.size() < header.totalLength)
        return 0;

    HandlePacket(header.type, buffer.data() + sizeof(PacketHeader), header.totalLength);

    return header.totalLength;
}

void PacketManager::HandlePacket(unsigned int type, const char* data, unsigned int length)
{
    switch (type)
    {
    case PKT_MOVE:
    {
        Packet_MOVE_C2S pkt;
        memcpy(&pkt, data, sizeof(pkt));
        std::cout << "[Server] Player " << pkt.playerID << " moved to (" << pkt.x << ", " << pkt.y << ")\n";
        break;
    }
    case PKT_ATTACK:
    {
        Packet_ATTACK_C2S pkt;
        memcpy(&pkt, data, sizeof(pkt));
        std::cout << "[Server] Player " << pkt.playerID << " attacked target " << pkt.targetID
            << " with damage " << pkt.damage << "\n";
        break;
    }
    }
}