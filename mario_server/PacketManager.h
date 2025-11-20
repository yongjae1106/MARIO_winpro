#pragma once
#include "PacketInfo.h"
#include <queue>

class PacketManager
{
public:
	std::queue<PacketInfo_ClientToServer> m_packetQueue;

	bool ParsePacket(const char* buffer, int size);
	int SerializePacket(char* buffer, const PacketInfo_ServerToClient& pkt);
	void HandlePacket();
};