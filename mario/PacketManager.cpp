#include "PacketManager.h"
#include <cstring>
#include <cstdio>
#include <iostream> // For error logging

// 싱글턴 인스턴스 초기화
PacketManager* PacketManager::GetInstance() {
    static PacketManager instance; // C++11 이후 스레드-세이프 보장
    return &instance;
}

// private 생성자 구현
PacketManager::PacketManager() {
    // 필요한 초기화 작업 수행
    std::cout << "PacketManager initialized." << std::endl;
}

// NetworkManager로부터 원시 데이터를 받아 패킷으로 분리하고 큐에 저장
void PacketManager::ProcessReceivedData(std::vector<char>& buffer) {
    // 버퍼에서 패킷을 분리하고 큐에 추가하는 로직
    while (true) {
        // 1. 헤더 사이즈만큼 데이터가 있는지 확인
        if (buffer.size() < sizeof(PacketHeader)) {
            // std::cout << "Not enough data for PacketHeader. Buffer size: " << buffer.size() << std::endl;
            break; // 파싱할 데이터가 부족함
        }

        PacketHeader header;
        // 버퍼의 시작 부분에서 헤더를 복사하여 읽음
        memcpy(&header, buffer.data(), sizeof(PacketHeader));

        // 유효한 패킷 길이인지 확인 (0이거나 너무 크지 않은지)
        if (header.totalLength <= 0 || header.totalLength > 65535) { // 적절한 최대 패킷 크기 지정
            std::cerr << "Invalid packet length received: " << header.totalLength << ". Clearing buffer." << std::endl;
            buffer.clear(); // 잘못된 데이터는 버퍼를 비워 오류 확산을 방지
            break;
        }

        // 2. 완전한 패킷 하나만큼 데이터가 있는지 확인
        if (buffer.size() < header.totalLength) {
            // std::cout << "Not enough data for full packet. Expected: " << header.totalLength << ", Got: " << buffer.size() << std::endl;
            break; // 아직 패킷 전체가 도착하지 않음
        }

        // 3. 패킷 데이터 복사 및 큐에 추가
        PacketData newPacket;
        newPacket.type = header.type;
        // 헤더를 제외한 실제 데이터만 복사
        newPacket.data.resize(header.totalLength - sizeof(PacketHeader));
        memcpy(newPacket.data.data(), buffer.data() + sizeof(PacketHeader), newPacket.data.size());
        
        m_receivedPackets.push(newPacket);
        // std::cout << "Packet " << newPacket.type << " (size: " << header.totalLength << ") received and enqueued." << std::endl;

        // 4. 처리된 패킷만큼 버퍼에서 제거
        buffer.erase(buffer.begin(), buffer.begin() + header.totalLength);
    }
}

// GameWorld에서 처리할 패킷을 가져가는 메서드
bool PacketManager::TryGetPacket(PacketData& outPacket) {
    return m_receivedPackets.try_pop(outPacket);
}


// =============================
// Serialize 함수들 (클라이언트 -> 서버)
// =============================
unsigned int PacketManager::Serialize_MOVE(char* buffer, const Packet_MOVE_C2S& move)
{
    PacketHeader header;
    header.type = PKT_MOVE;
    header.totalLength = sizeof(PacketHeader) + sizeof(move);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &move, sizeof(move));

    return header.totalLength;
}

unsigned int PacketManager::Serialize_ATTACK(char* buffer, const Packet_ATTACK_C2S& atk)
{
    PacketHeader header;
    header.type = PKT_ATTACK;
    header.totalLength = sizeof(PacketHeader) + sizeof(atk);

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &atk, sizeof(atk));

    return header.totalLength;
}

// =============================
// 수신 패킷 처리 (PacketManager 내부 또는 GameWorld에서 활용)
// OnMove, OnHit, OnBlock은 이제 ProcessReceivedData에서 직접 호출하는 대신,
// GameWorld가 TryGetPacket으로 PacketData를 받아 자체적으로 처리할 때 사용될 수 있습니다.
// 여기서는 예시로 남겨두지만, 실제 구현에서는 GameWorld의 Update 함수 내에서 처리 로직을 구현하게 됩니다.
// =============================
void PacketManager::OnMove(const Packet_MOVE_S2C& pkt)
{
    printf("[Client] Player %u moved to (%u,%u)\n", pkt.playerID, pkt.x, pkt.y);
}

void PacketManager::OnHit(const Packet_HIT_S2C& pkt)
{
    printf("[Client] HIT damage = %u\n", pkt.damage);
}

void PacketManager::OnBlock(const Packet_BLOCK_S2C& pkt)
{
    printf("[Client] BLOCK %u at (%u,%u)\n", pkt.blockID, pkt.block_x, pkt.block_y);
}

// TryParse는 더 이상 직접적으로 사용되지 않으므로, 정의는 유지하지만 호출되지 않을 수 있음
// 기존 로직은 ProcessReceivedData로 이동함
int PacketManager::TryParse(const std::vector<char>& buffer)
{
    // 이 함수는 더 이상 외부에서 직접 호출되지 않으며, ProcessReceivedData 내부 로직에 흡수됩니다.
    // 기존 TryParse의 역할이 불분명하여 일단 내용은 비워두거나,
    // 필요하다면 ProcessReceivedData의 일부 기능을 헬퍼 함수로 분리할 때 사용될 수 있습니다.
    return 0; 
}
