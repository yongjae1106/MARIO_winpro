#include "PacketManager.h"
//#include "GameWorld.h"
#include <cstring>
#include <iostream>

int PacketManager::HandlePacket(const std::vector<char>& buffer, unsigned int socketID, GameWorld* world)
{
    if (buffer.size() < sizeof(PacketHeader))
        return 0;

    PacketHeader header;
    memcpy(&header, buffer.data(), sizeof(header));

    if (buffer.size() < header.totalLength)
        return 0;

    const char* data = buffer.data() + sizeof(PacketHeader);

    switch (header.type)
    {
    case PKT_KEY_DOWN:
    case PKT_KEY_UP:
    {
        Packet_KEY_EVENT_C2S pkt;
        memcpy(&pkt, data, sizeof(pkt));

        //world->UpdatePeerState(socketID, pkt.keyCode, header.type == PKT_KEY_DOWN);
        break;
    }
    default:
        std::cout << "[Server] Unknown Packet: " << header.type << "\n";
        break;
    }

    return header.totalLength;
}


// ---- Serialize for sending world/player state to clients ----

unsigned int PacketManager::Serialize_PLAYER_STATE(char* buffer, const Packet_PLAYER_STATE_S2C& state)
{
    PacketHeader header;
    header.type = PKT_PLAYER_STATE;
    header.totalLength = sizeof(PacketHeader) + sizeof(state);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &state, sizeof(state));

    return header.totalLength;
}