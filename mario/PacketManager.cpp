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

void PacketManager::ProcessReceivedData(std::vector<char>& buffer)
{
    while (true)
    {
        if (buffer.size() < sizeof(PacketHeader))
            break;

        PacketHeader header;
        memcpy(&header, buffer.data(), sizeof(PacketHeader));

        if (header.totalLength <= 0 || header.totalLength > 65535)
        {
            std::cerr << "Invalid packet size: " << header.totalLength << "\n";
            buffer.clear();
            break;
        }

        if (buffer.size() < header.totalLength)
            break;

        PacketData pkt;
        pkt.type = header.type;
        pkt.data.resize(header.totalLength - sizeof(PacketHeader));

        memcpy(pkt.data.data(), buffer.data() + sizeof(PacketHeader), pkt.data.size());

        m_receivedPackets.push(pkt);

        buffer.erase(buffer.begin(), buffer.begin() + header.totalLength);
    }
}

bool PacketManager::TryGetPacket(PacketData& outPacket)
{
    return m_receivedPackets.try_pop(outPacket);
}

unsigned int PacketManager::Serialize_INPUT(char* buffer, const Packet_INPUT_C2S& input)
{
    PacketHeader header;
    header.type = PKT_INPUT;
    header.totalLength = sizeof(PacketHeader) + sizeof(input);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &input, sizeof(input));

    return header.totalLength;
}