#pragma once
#include "PacketInfo.h"
#include <vector>

// : GameWorld   (  )
class GameWorld;

class PacketManager
{
public:
    // :  GameWorld* world 
    int TryParse(const std::vector<char>& buffer, unsigned int socketID, GameWorld* world);
    void HandlePacket(unsigned int type, const char* data, unsigned int length, unsigned int socketID, GameWorld* world);

    //   Ŭ ȭ
    unsigned int Serialize_PLAYER_STATE(char* buffer, const Packet_PLAYER_STATE_S2C& state);
    unsigned int Serialize_HIT(char* buffer, const Packet_HIT_S2C& hit);
    unsigned int Serialize_BLOCK(char* buffer, const Packet_BLOCK_S2C& blk);
};
