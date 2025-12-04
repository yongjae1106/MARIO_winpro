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

    // ---- Serialize (클라 → 서버) ----
    unsigned int Serialize_KeyDown(char* buffer, unsigned int keyCode);
    unsigned int Serialize_KeyUp(char* buffer, unsigned int keyCode);

private:
    PacketManager();
    ThreadSafeQueue<PacketData> m_receivedPackets;
};