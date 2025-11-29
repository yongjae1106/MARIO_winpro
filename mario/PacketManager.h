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

    // ---- Serialize (클라→서버) ----
    unsigned int Serialize_MOVE(char* buffer, const Packet_MOVE_C2S& move);
    unsigned int Serialize_ATTACK(char* buffer, const Packet_ATTACK_C2S& atk);

private:
    PacketManager();
    ThreadSafeQueue<PacketData> m_receivedPackets;
};