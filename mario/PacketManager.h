#pragma once
#include <vector>
#include "PacketInfo.h"
#include "NetworkManager/ThreadSafeQueue.h"

struct PacketData
{
    unsigned int type;
    std::vector<char> data;
};

class PacketManager
{
public:
    static PacketManager* GetInstance();

    // ---- Parsing ----
    void ProcessReceivedData(std::vector<char>& buffer);
    bool TryGetPacket(PacketData& outPacket);

    // ---- Serialize ----
    unsigned int Serialize_INPUT(char* buffer, const Packet_INPUT_C2S& input);

private:
    PacketManager();
    ThreadSafeQueue<PacketData> m_receivedPackets;
};