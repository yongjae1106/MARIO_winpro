#pragma once
#include "PacketInfo.h"
#include <vector>

// 전방 선언(순환 참조 방지)
class GameWorld;

class PacketManager
{
public:
    // 클라이언트별 버퍼 기반 패킷 처리 (ThreadManager → ClientLoop 이용)
    int TryParse(std::vector<char>& buffer, unsigned int socketID, GameWorld* world);

private:
    void HandlePacket(unsigned int type, const char* data, unsigned int length, unsigned int socketID, GameWorld* world);

public:
    // ---- Server → Client 직렬화 ----
    unsigned int Serialize_PLAYER_STATE(char* buffer, const Packet_MOVE_S2C& move);
    unsigned int Serialize_HIT(char* buffer, const Packet_HIT_S2C& hit);
    unsigned int Serialize_BLOCK(char* buffer, const Packet_BLOCK_S2C& blk);
};