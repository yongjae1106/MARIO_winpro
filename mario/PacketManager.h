#pragma once
#include "PacketInfo.h"
#include <queue>

class PacketManager
{
public:
	std::queue<PacketInfo_ServerToClient> m_packetQueue;

	int SerializePacket(char* buffer, const PacketInfo_ClientToServer& pkt);
	bool ParsePacket(const char* buffer, int size);
	void HandlePacket();
};