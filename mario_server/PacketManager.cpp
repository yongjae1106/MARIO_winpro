#include "PacketInfo.h"
#include <queue>
#include <cstring>
#include <cstdio>

class PacketManager_Server
{
public:
    std::queue<PacketInfo_ClientToServer> m_packetQueue;

    // ==============================================================
    // 클라 → 서버 파싱 (입력만 받음)
    // ==============================================================
    bool ParsePacket(const char* buffer, int size)
    {
        PacketInfo_ClientToServer pkt;
        int offset = 0;

        memcpy(&pkt.playerID, buffer + offset, sizeof(int)); offset += sizeof(int);
        memcpy(&pkt.type, buffer + offset, sizeof(char)); offset += sizeof(char);

        if (pkt.type == PKT_MOVE)
        {
            memcpy(&pkt.x, buffer + offset, sizeof(int)); offset += sizeof(int);
            memcpy(&pkt.y, buffer + offset, sizeof(int)); offset += sizeof(int);
            memcpy(&pkt.vx, buffer + offset, sizeof(int)); offset += sizeof(int);
            memcpy(&pkt.vy, buffer + offset, sizeof(int)); offset += sizeof(int);
            memcpy(&pkt.state, buffer + offset, sizeof(char)); offset += sizeof(char);
        }
        else if (pkt.type == PKT_ATTACK)
        {
            memcpy(&pkt.targetID, buffer + offset, sizeof(int)); offset += sizeof(int);
            memcpy(&pkt.damage, buffer + offset, sizeof(int)); offset += sizeof(int);
        }

        m_packetQueue.push(pkt);
        return true;
    }

    // ==============================================================
    // 서버 → 클라 Serialize (브로드캐스트용)
    // ==============================================================
    int SerializePacket(char* buffer, const PacketInfo_ServerToClient& pkt)
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
        else if (pkt.type == PKT_HIT)
        {
            memcpy(buffer + offset, &pkt.damage, sizeof(int)); offset += sizeof(int);
        }
        else if (pkt.type == PKT_BLOCK_ATTACK)
        {
            memcpy(buffer + offset, &pkt.blockID, sizeof(int)); offset += sizeof(int);
            memcpy(buffer + offset, &pkt.block_x, sizeof(int)); offset += sizeof(int);
            memcpy(buffer + offset, &pkt.block_y, sizeof(int)); offset += sizeof(int);
        }

        return offset;
    }

    // ==============================================================
    // 서버 HandlePacket ? 게임 로직 처리
    // ==============================================================
    void HandlePacket()
    {
        while (!m_packetQueue.empty())
        {
            PacketInfo_ClientToServer pkt = m_packetQueue.front();
            m_packetQueue.pop();

            switch (pkt.type)
            {
            case PKT_MOVE:
                printf("[SERVER] Player %d 이동 처리\n", pkt.playerID);
                // TODO: GameWorld에서 좌표 갱신
                break;

            case PKT_ATTACK:
                printf("[SERVER] Player %d이 target %d에게 공격\n",
                    pkt.playerID, pkt.targetID);
                // TODO: 충돌 판정 및 데미지 계산
                break;
            }
        }
    }
};
