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
    std::cout << "[Client] PacketManager initialized\n";
}

void PacketManager::ProcessReceivedData(std::vector<char>& buffer)
{
    while (buffer.size() >= sizeof(PacketHeader))
    {
        PacketHeader header;
        memcpy(&header, buffer.data(), sizeof(PacketHeader));

        if (buffer.size() < header.totalLength)
            break;

        PacketData newPacket;
        newPacket.type = header.type;

        newPacket.data.resize(header.totalLength - sizeof(PacketHeader));
        memcpy(newPacket.data.data(), buffer.data() + sizeof(PacketHeader), newPacket.data.size());

        m_receivedPackets.push(newPacket);

        buffer.erase(buffer.begin(), buffer.begin() + header.totalLength);
    }
}

bool PacketManager::TryGetPacket(PacketData& outPacket)
{
    return m_receivedPackets.try_pop(outPacket);
}

// ---- Serialize for key event ----
unsigned int PacketManager::Serialize_KEY_EVENT(char* buffer, unsigned int keyCode, PacketType type)
{
    Packet_KEY_EVENT_C2S pkt;
    pkt.keyCode = keyCode;

    PacketHeader header;
    header.type = (unsigned int)type;
    header.totalLength = sizeof(PacketHeader) + sizeof(Packet_KEY_EVENT_C2S);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &pkt, sizeof(pkt));

    return header.totalLength;
}

int PacketManager::TryParse(const std::vector<char>& buffer)
{
    return 0; // no direct use
}