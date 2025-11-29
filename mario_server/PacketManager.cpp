#include "PacketManager.h"
#include "GameWorld.h" // GameWorld 멤버 함수 호출을 위해 include
#include <cstring>
#include <cstdio>

int PacketManager::TryParse(const std::vector<char>& buffer, unsigned int socketID, GameWorld* world)
{
    if (buffer.size() < sizeof(PacketHeader)) return 0;

    const PacketHeader* header = (const PacketHeader*)buffer.data();

    if (buffer.size() < header->totalLength) return 0;

    // HandlePacket에 world 전달
    HandlePacket(header->type, buffer.data() + sizeof(PacketHeader), header->totalLength, socketID, world);

    return header->totalLength;
}

void PacketManager::HandlePacket(unsigned int type, const char* data, unsigned int length, unsigned int socketID, GameWorld* world) 
{
    switch (type)
    {
    case PKT_MOVE:
    {
        Packet_MOVE_C2S pkt;
        memcpy(&pkt, data, sizeof(pkt));
        //printf("[Server] Socket %u → MOVE (%u,%u)\n", socketID, pkt.x, pkt.y);
        if (world) { // 추가: GameWorld에 접속자 상태 업데이트
            world->UpdatePeerState(socketID, pkt.x, pkt.y, pkt.vx, pkt.vy, pkt.state);
        }
        break;
    }
    case PKT_ATTACK:
    {
        Packet_ATTACK_C2S pkt;
        memcpy(&pkt, data, sizeof(pkt));
        //printf("[Server] Socket %u → ATTACK target=%u dmg=%u\n",socketID, pkt.targetID, pkt.damage);
        break;
    }
    }
}

// =============================
// 서버 → 클라 직렬화
// =============================
unsigned int PacketManager::Serialize_MOVE(char* buffer, const Packet_MOVE_S2C& move)
{
    PacketHeader header;
    header.type = PKT_MOVE;
    header.totalLength = sizeof(PacketHeader) + sizeof(move);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &move, sizeof(move));

    return header.totalLength;
}

unsigned int PacketManager::Serialize_HIT(char* buffer, const Packet_HIT_S2C& hit)
{
    PacketHeader header;
    header.type = PKT_HIT;
    header.totalLength = sizeof(PacketHeader) + sizeof(hit);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &hit, sizeof(hit));

    return header.totalLength;
}

unsigned int PacketManager::Serialize_BLOCK(char* buffer, const Packet_BLOCK_S2C& blk)
{
    PacketHeader header;
    header.type = PKT_BLOCK_ATTACK;
    header.totalLength = sizeof(PacketHeader) + sizeof(blk);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &blk, sizeof(blk));

    return header.totalLength;
}