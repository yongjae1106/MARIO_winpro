#include "PacketManager.h"
#include <cstring>
#include <cstdio>

// ==============================================================
// 클라 → 서버 직렬화 (입력만 보냄)
// ==============================================================
int PacketManager::SerializePacket(char* buffer, const PacketInfo_ClientToServer& pkt)
{
    int offset = 0;

    memcpy(buffer + offset, &pkt.playerID, sizeof(int)); offset += sizeof(int);
    memcpy(buffer + offset, &pkt.type, sizeof(char)); offset += sizeof(char);

    if (pkt.type == PKT_MOVE)
    {
        memcpy(buffer + offset, &pkt.x, sizeof(int)); offset += sizeof(int);
        memcpy(buffer + offset, &pkt.y, sizeof(int)); offset += sizeof(int);
        memcpy(buffer + offset, &pkt.vx, sizeof(int)); offset += sizeof(int);
        memcpy(buffer + offset, &pkt.vy, sizeof(int)); offset += sizeof(int);
        memcpy(buffer + offset, &pkt.state, sizeof(char)); offset += sizeof(char);
    }
    else if (pkt.type == PKT_ATTACK)
    {
        memcpy(buffer + offset, &pkt.targetID, sizeof(int)); offset += sizeof(int);
        memcpy(buffer + offset, &pkt.damage, sizeof(int)); offset += sizeof(int);
    }

    return offset;
}

// ==============================================================
// 서버 → 클라 파싱 (충돌·HIT·블록 생성 등)
// ==============================================================
bool PacketManager::ParsePacket(const char* buffer, int size)
{
    PacketInfo_ServerToClient pkt;
    int offset = 0;

    memcpy(&pkt.playerID, buffer + offset, sizeof(int)); offset += sizeof(int);
    memcpy(&pkt.type, buffer + offset, sizeof(char)); offset += sizeof(char);

    switch (pkt.type)
    {
    case PKT_MOVE:
        memcpy(&pkt.x, buffer + offset, sizeof(int)); offset += sizeof(int);
        memcpy(&pkt.y, buffer + offset, sizeof(int)); offset += sizeof(int);
        memcpy(&pkt.vx, buffer + offset, sizeof(int)); offset += sizeof(int);
        memcpy(&pkt.vy, buffer + offset, sizeof(int)); offset += sizeof(int);
        memcpy(&pkt.state, buffer + offset, sizeof(char)); offset += sizeof(char);
        break;

    case PKT_HIT:
        memcpy(&pkt.damage, buffer + offset, sizeof(int)); offset += sizeof(int);
        break;

    case PKT_BLOCK_ATTACK:
        memcpy(&pkt.blockID, buffer + offset, sizeof(int)); offset += sizeof(int);
        memcpy(&pkt.block_x, buffer + offset, sizeof(int)); offset += sizeof(int);
        memcpy(&pkt.block_y, buffer + offset, sizeof(int)); offset += sizeof(int);
        break;
    }

    int eventCount = 0;
    memcpy(&eventCount, buffer + offset, sizeof(int)); offset += sizeof(int);

    pkt.events.resize(eventCount);
    for (int i = 0; i < eventCount; i++)
    {
        int eventVal = 0;
        memcpy(&eventVal, buffer + offset, sizeof(int)); offset += sizeof(int);
        pkt.events[i] = static_cast<GameEvent>(eventVal);
    }

    m_packetQueue.push(pkt);
    return true;
}

// ==============================================================
// HandlePacket ? 클라는 받은 결과만 처리
// ==============================================================
void PacketManager::HandlePacket()
{
    while (!m_packetQueue.empty())
    {
        PacketInfo_ServerToClient pkt = m_packetQueue.front();
        m_packetQueue.pop();

        switch (pkt.type)
        {
        case PKT_MOVE:
            printf("[CLIENT] Player %d 위치 (%d, %d)\n", pkt.playerID, pkt.x, pkt.y);
            break;

        case PKT_HIT:
            printf("[CLIENT] 피해량: %d\n", pkt.damage);
            break;

        case PKT_BLOCK_ATTACK:
            printf("[CLIENT] 블록 %d (%d,%d)\n", pkt.blockID, pkt.block_x, pkt.block_y);
            break;
        }

        for (auto& e : pkt.events)
        {
            printf("[CLIENT EVENT] %d\n", static_cast<int>(e));
        }
    }
}