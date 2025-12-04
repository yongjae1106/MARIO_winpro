#include "PacketManager.h"
//#include "GameWorld.h"
#include <cstring>
#include <iostream>

int PacketManager::TryParse(const std::vector<char>& buffer, unsigned int socketID, GameWorld* world)
{
    if (buffer.size() < sizeof(PacketHeader))
        return 0;

    PacketHeader header;
    memcpy(&header, buffer.data(), sizeof(PacketHeader));

    if (header.totalLength < sizeof(PacketHeader) || buffer.size() < header.totalLength)
        return 0;

    HandlePacket(header.type, buffer.data() + sizeof(PacketHeader), header.totalLength, socketID, world);

    return header.totalLength;
}

void PacketManager::HandlePacket(unsigned int type, const char* data, unsigned int length, unsigned int socketID, GameWorld* world)
{
    switch (type)
    {
    case PKT_KEY_DOWN:
    {
        Packet_KEY_EVENT_C2S pkt;
        memcpy(&pkt, data, sizeof(pkt));
        //world->UpdatePeerState(socketID, pkt.keyCode, true);
        break;
    }

    case PKT_KEY_UP:
    {
        Packet_KEY_EVENT_C2S pkt;
        memcpy(&pkt, data, sizeof(pkt));
        //world->UpdatePeerState(socketID, pkt.keyCode, false);
        break;
    }
    }
}

// -------- SERVER ¡æ CLIENT SERIALIZE --------

unsigned int PacketManager::Serialize_PlayerState(char* buffer, const Packet_PLAYER_STATE_S2C& state)
{
    PacketHeader header;
    header.type = PKT_PLAYER_STATE;
    header.totalLength = sizeof(PacketHeader) + sizeof(state);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &state, sizeof(state));
    return header.totalLength;
}

unsigned int PacketManager::Serialize_Hit(char* buffer, const Packet_HIT_S2C& hit)
{
    PacketHeader header;
    header.type = PKT_HIT;
    header.totalLength = sizeof(PacketHeader) + sizeof(hit);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &hit, sizeof(hit));
    return header.totalLength;
}

unsigned int PacketManager::Serialize_Block(char* buffer, const Packet_BLOCK_S2C& blk)
{
    PacketHeader header;
    header.type = PKT_BLOCK_ATTACK;
    header.totalLength = sizeof(PacketHeader) + sizeof(blk);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &blk, sizeof(blk));
    return header.totalLength;
}