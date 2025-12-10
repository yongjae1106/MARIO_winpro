#pragma once
// [중요] 이 매크로를 최상단에 정의하여 windows.h가 구형 winsock.h를 포함하지 않게 막습니다.
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h> // 반드시 winsock2.h 뒤에 와야 함

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "ThreadSafeQueue.h"

#pragma comment(lib, "ws2_32.lib")

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    // Winsock 초기화

    bool Init();

    // 네트워크 스레드 시작/중지
    void Start();
    void Stop();

    // 서버에 연결
    bool Connect(const std::string& ipAddress, int port);

    // 연결 종료
    void Disconnect();

    // 데이터 전송 (큐에 푸시)
    void Send(const std::string& data);

    // 데이터 수신 (큐에서 트라이 팝)
    // bool TryGetReceivedData(std::string& outData); // receiveQueue 제거로 불필요
    bool IsConnected() const;

private:
    SOCKET clientSocket;
    bool isConnected;

    std::thread networkThread;
    std::atomic<bool> isRunning;

    ThreadSafeQueue<std::string> sendQueue;
    // ThreadSafeQueue<std::string> receiveQueue; // PacketManager가 관리하도록 변경


    // 네트워크 스레드가 실행할 메인 루프
    void NetworkLoop();

    // 소켓을 논블로킹 모드로 설정
    bool SetSocketNonBlocking(SOCKET sock);

    // 패킷 재조합을 위한 내부 버퍼
    std::vector<char> m_internalRecvBuffer;
    size_t m_internalRecvBufferSize;
};
