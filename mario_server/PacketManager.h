#pragma once
#include "PacketInfo.h"
#include <vector>

// 추가: GameWorld 전방 선언 (헤더 순환 참조 방지)
class GameWorld;

class PacketManager
{
public:
    // 수정: 인자에 GameWorld* world 추가
    int TryParse(const std::vector<char>& buffer, unsigned int socketID, GameWorld* world);
    void HandlePacket(unsigned int type, const char* data, unsigned int length, unsigned int socketID, GameWorld* world);

    // 서버 → 클라 직렬화
    unsigned int Serialize_MOVE(char* buffer, const Packet_MOVE_S2C& move);
    unsigned int Serialize_HIT(char* buffer, const Packet_HIT_S2C& hit);
    unsigned int Serialize_BLOCK(char* buffer, const Packet_BLOCK_S2C& blk);
};