#pragma once
#include "PacketInfo.h"
#include <vector>

class GameWorld;

class PacketManager
{
public:
    int HandlePacket(const std::vector<char>& buffer, unsigned int socketID, GameWorld* world);

    // Serialize functions (server ¡æ client)
    unsigned int Serialize_PLAYER_STATE(char* buffer, const Packet_PLAYER_STATE_S2C& state);
};