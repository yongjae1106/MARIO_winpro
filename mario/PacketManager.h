#pragma once
#include "PacketInfo.h"
#include "NetworkManager/ThreadSafeQueue.h"
#include <vector>

struct PacketData
{
    unsigned int type;
    std::vector<char> data;
};

class PacketManager
{
public:
    static PacketManager* GetInstance();

    void ProcessReceivedData(std::vector<char>& buffer);
    bool TryGetPacket(PacketData& outPacket);

    unsigned int Serialize_KEY_EVENT(char* buffer, unsigned int keyCode, PacketType type);

private:
    PacketManager();
    ~PacketManager() = default;

    PacketManager(const PacketManager&) = delete;
    PacketManager& operator=(const PacketManager&) = delete;

    ThreadSafeQueue<PacketData> m_receivedPackets;

    int TryParse(const std::vector<char>& buffer);
};