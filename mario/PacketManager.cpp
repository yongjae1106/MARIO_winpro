#include "PacketManager.h"
#include <cstring>
#include <iostream>

PacketManager* PacketManager::GetInstance() {
    static PacketManager instance;
    return &instance;
}

PacketManager::PacketManager() {
    std::cout << "PacketManager Initialized (Client)\n";
}

void PacketManager::ProcessReceivedData(std::vector<char>& buffer)
{
    while (true)
    {
        if (buffer.size() < sizeof(PacketHeader))
            break;

        PacketHeader header;
        memcpy(&header, buffer.data(), sizeof(PacketHeader));

        if (header.totalLength < sizeof(PacketHeader) || buffer.size() < header.totalLength)
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

// ---------- Serialize (Client ¡æ Server) ----------

unsigned int PacketManager::Serialize_KeyDown(char* buffer, unsigned int keyCode)
{
    PacketHeader header;
    header.type = PKT_KEY_DOWN;

    Packet_KEY_EVENT_C2S pkt;
    pkt.keyCode = keyCode;

    header.totalLength = sizeof(PacketHeader) + sizeof(pkt);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &pkt, sizeof(pkt));

    return header.totalLength;
}

unsigned int PacketManager::Serialize_KeyUp(char* buffer, unsigned int keyCode)
{
    PacketHeader header;
    header.type = PKT_KEY_UP;

    Packet_KEY_EVENT_C2S pkt;
    pkt.keyCode = keyCode;

    header.totalLength = sizeof(PacketHeader) + sizeof(pkt);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &pkt, sizeof(pkt));

    return header.totalLength;
}