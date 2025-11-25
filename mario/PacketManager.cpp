#include "PacketManager.h"
#include <cstring>
#include <cstdio>

int PacketManager::TryParse(const std::vector<char>& buffer)
{
    if (buffer.size() < sizeof(PacketHeader))
        return 0;

    const PacketHeader* header = (const PacketHeader*)buffer.data();

    if (buffer.size() < header->totalLength)
        return 0;

    HandlePacket(header->type, buffer.data() + sizeof(PacketHeader), header->totalLength);

    return header->totalLength;
}

void PacketManager::HandlePacket(unsigned int type, const char* data, unsigned int length)
{
    switch (type)
    {
    case PKT_MOVE:
    {
        Packet_MOVE_S2C pkt;
        memcpy(&pkt, data, sizeof(pkt));
        OnMove(pkt);
        break;
    }
    case PKT_HIT:
    {
        Packet_HIT_S2C pkt;
        memcpy(&pkt, data, sizeof(pkt));
        OnHit(pkt);
        break;
    }
    case PKT_BLOCK_ATTACK:
    {
        Packet_BLOCK_S2C pkt;
        memcpy(&pkt, data, sizeof(pkt));
        OnBlock(pkt);
        break;
    }
    }
}

// =============================
// 직렬화 (클라 → 서버)
// =============================
unsigned int PacketManager::Serialize_MOVE(char* buffer, const Packet_MOVE_C2S& move)
{
    PacketHeader header;
    header.type = PKT_MOVE;
    header.totalLength = sizeof(PacketHeader) + sizeof(move);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &move, sizeof(move));

    return header.totalLength;
}

unsigned int PacketManager::Serialize_ATTACK(char* buffer, const Packet_ATTACK_C2S& atk)
{
    PacketHeader header;
    header.type = PKT_ATTACK;
    header.totalLength = sizeof(PacketHeader) + sizeof(atk);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &atk, sizeof(atk));

    return header.totalLength;
}

// =============================
// 수신 패킷 처리
// =============================
void PacketManager::OnMove(const Packet_MOVE_S2C& pkt)
{
    printf("[Client] Player %u → MOVE (%u,%u)\n", pkt.playerID, pkt.x, pkt.y);
}

void PacketManager::OnHit(const Packet_HIT_S2C& pkt)
{
    printf("[Client] HIT damage = %u\n", pkt.damage);
}

void PacketManager::OnBlock(const Packet_BLOCK_S2C& pkt)
{
    printf("[Client] BLOCK %u at (%u,%u)\n", pkt.blockID, pkt.block_x, pkt.block_y);
}