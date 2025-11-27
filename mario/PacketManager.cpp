#include "PacketManager.h"
#include <cstring>
#include <iostream>

PacketManager* PacketManager::GetInstance()
{
    static PacketManager instance;
    return &instance;
}

PacketManager::PacketManager()
{
    std::cout << "PacketManager initialized." << std::endl;
}

// =============================
// 수신 패킷(바이트 스트림) -> 패킷 분리 후 큐에 저장
// =============================
void PacketManager::ProcessReceivedData(std::vector<char>& buffer)
{
    while (true)
    {
        if (buffer.size() < sizeof(PacketHeader))
            break;

        PacketHeader header;
        memcpy(&header, buffer.data(), sizeof(PacketHeader));

        if (header.totalLength <= 0 || buffer.size() < header.totalLength)
            break;

        PacketData pkt;
        pkt.type = header.type;
        pkt.data.resize(header.totalLength - sizeof(PacketHeader));
        memcpy(pkt.data.data(), buffer.data() + sizeof(PacketHeader), pkt.data.size());

        m_receivedPackets.push(pkt);

        buffer.erase(buffer.begin(), buffer.begin() + header.totalLength);
    }
}

// =============================
// GameWorld에서 꺼내가기
// =============================
bool PacketManager::TryGetPacket(PacketData& outPacket)
{
    return m_receivedPackets.try_pop(outPacket);
}

// =============================
// Serialize (클라 → 서버)
// =============================
unsigned int PacketManager::Serialize_MOVE(char* buffer, const Packet_MOVE_C2S& move)
{
    PacketHeader header{ sizeof(PacketHeader) + sizeof(move), PKT_MOVE };
    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &move, sizeof(move));
    return header.totalLength;
}

unsigned int PacketManager::Serialize_ATTACK(char* buffer, const Packet_ATTACK_C2S& atk)
{
    PacketHeader header{ sizeof(PacketHeader) + sizeof(atk), PKT_ATTACK };
    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &atk, sizeof(atk));
    return header.totalLength;
}