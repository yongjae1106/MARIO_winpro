#pragma once
#include "PacketInfo.h"
#include <vector>

class GameWorld;

class PacketManager
{
public:
    int TryParse(const std::vector<char>& buffer, unsigned int socketID, GameWorld* world);
    void HandlePacket(unsigned int type, const char* data, unsigned int length, unsigned int socketID, GameWorld* world);

    // Á÷·ÄÈ­ (Server ¡æ Client)
    unsigned int Serialize_PlayerState(char* buffer, const Packet_PLAYER_STATE_S2C& state);
    unsigned int Serialize_Hit(char* buffer, const Packet_HIT_S2C& hit);
    unsigned int Serialize_Block(char* buffer, const Packet_BLOCK_S2C& blk);
};