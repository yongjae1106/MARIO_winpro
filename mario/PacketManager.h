#pragma once
#include "PacketInfo.h"
#include "NetworkManager/ThreadSafeQueue.h" // ThreadSafeQueue를 사용하기 위해 추가
#include <vector>
#include <queue> // std::queue 사용을 위해 추가 (PacketData struct가 std::vector를 사용하므로 굳이 queue 헤더가 직접 필요 없을 수 있으나, 명시적으로 포함)
#include <memory> // std::shared_ptr 사용을 위해 추가 (싱글턴 관리에 사용될 수 있으나, 여기서는 단순히 포인터로 관리)
#include <mutex> // for singleton

// PacketManager가 처리할 패킷들을 저장할 큐의 요소.
// 나중에 GameWorld가 이 큐에서 패킷을 꺼내가서 처리합니다.
struct PacketData {
    unsigned int type;
    std::vector<char> data; // 패킷 데이터를 저장
};

class PacketManager
{
public:
    // 싱글턴 인스턴스에 접근하기 위한 정적 메서드
    static PacketManager* GetInstance();

    // NetworkManager로부터 원시 데이터를 받아 패킷으로 분리하고 큐에 저장
    void ProcessReceivedData(std::vector<char>& buffer);

    // GameWorld에서 처리할 패킷을 가져가는 메서드
    bool TryGetPacket(PacketData& outPacket);

    // Serialize 함수들은 그대로 유지
    unsigned int Serialize_MOVE(char* buffer, const Packet_MOVE_C2S& move);
    unsigned int Serialize_ATTACK(char* buffer, const Packet_ATTACK_C2S& atk);

private:
    // 싱글턴을 위해 생성자를 private으로
    PacketManager(); 
    // 소멸자는 public 또는 default
    ~PacketManager() = default; 

    // 복사 생성자 및 대입 연산자 금지 (싱글턴)
    PacketManager(const PacketManager&) = delete;
    PacketManager& operator=(const PacketManager&) = delete;

    // 수신된 완전한 패킷들을 저장할 큐 (GameWorld에서 가져갈 용도)
    ThreadSafeQueue<PacketData> m_receivedPackets;

    // 패킷 처리 함수들 (이들은 ProcessReceivedData 내부에서 호출될 수 있습니다)
    void OnMove(const Packet_MOVE_S2C& pkt);
    void OnHit(const Packet_HIT_S2C& pkt);
    void OnBlock(const Packet_BLOCK_S2C& pkt);

    // 기존 TryParse는 ProcessReceivedData 내부에서 사용될 수 있음
    // TryParse의 원래 목적이 불분명하여 일단 private으로 유지하고 ProcessReceivedData에서 어떻게 활용할지 결정
    int TryParse(const std::vector<char>& buffer); 
};