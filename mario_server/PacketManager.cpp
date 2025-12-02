#include "PacketManager.h"
#include <cstring>
#include <iostream>
#include "GameWorld.h" // 角力 霸烙 包府 肺流

int PacketManager::TryParse(std::vector<char>& buffer, unsigned int socketID, GameWorld* world)
{
    while (true)
    {
        if (buffer.size() < sizeof(PacketHeader))
            return 0;

        PacketHeader header;
        memcpy(&header, buffer.data(), sizeof(PacketHeader));

        if (buffer.size() < header.totalLength)
            return 0;

        HandlePacket(header.type, buffer.data() + sizeof(PacketHeader), header.totalLength, socketID, world);

        buffer.erase(buffer.begin(), buffer.begin() + header.totalLength);
    }

    return 0;
}

void PacketManager::HandlePacket(unsigned int type, const char* data, unsigned int length, unsigned int socketID, GameWorld* world)
{
    switch (type)
    {
    case PKT_INPUT:
    {
        Packet_INPUT_C2S pkt{};
        memcpy(&pkt, data, sizeof(pkt));

        switch (pkt.inputType)
        {
        case InputType::MOVE:
            std::cout << "[Server] Player(" << socketID << ") MOVE => (" << pkt.x << "," << pkt.y << ")\n";
            //world->OnPlayerMove(socketID, pkt.x, pkt.y, pkt.vx, pkt.vy, pkt.state);
            break;

        case InputType::ATTACK:
            std::cout << "[Server] Player(" << socketID << ") ATTACK Target(" << pkt.targetID << ") Damage(" << pkt.damage << ")\n";
            //world->OnPlayerAttack(socketID, pkt.targetID, pkt.damage);
            break;

        default:
            std::cout << "[Server] Unknown InputType\n";
            break;
        }
        break;
    }

    default:
        std::cout << "[Server] Unknown Packet Type: " << type << "\n";
        break;
    }
}