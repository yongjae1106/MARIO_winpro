#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread> // For std::thread
#include <atomic> // For std::atomic<bool>
#include "ThreadSafeQueue.h" // For ThreadSafeQueue

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
    bool TryGetReceivedData(std::string& outData);

    bool IsConnected() const;

private:
    SOCKET clientSocket;
    bool isConnected;

    std::thread networkThread;
    std::atomic<bool> isRunning;

    ThreadSafeQueue<std::string> sendQueue;
    ThreadSafeQueue<std::string> receiveQueue;

    // 네트워크 스레드가 실행할 메인 루프
    void NetworkLoop();

    // 소켓을 논블로킹 모드로 설정
    bool SetSocketNonBlocking(SOCKET sock);
};
