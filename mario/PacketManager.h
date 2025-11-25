#pragma once
#include "PacketInfo.h"
#include <vector>

class PacketManager
{
public:
    int TryParse(const std::vector<char>& buffer);
    void HandlePacket(unsigned int type, const char* data, unsigned int length);

    // 직렬화 (클라 → 서버)
    unsigned int Serialize_MOVE(char* buffer, const Packet_MOVE_C2S& move);
    unsigned int Serialize_ATTACK(char* buffer, const Packet_ATTACK_C2S& atk);

private:
    // 패킷 처리 함수 (게임 로직과 연동 예정)
    void OnMove(const Packet_MOVE_S2C& pkt);
    void OnHit(const Packet_HIT_S2C& pkt);
    void OnBlock(const Packet_BLOCK_S2C& pkt);
};