#include "Common.h"
#include <mutex> // 동기화 객체
#include <thread> // std::thread 사용 (Wrapper 함수 대체)
#include "PacketInfo.h"
#include "PacketManager.h"
#include <vector>
#include <queue>

// NetworkManager 클래스 선언 (순환 참조 방지)
class NetworkManager;

// 마찬가지로 GameWorld 클래스도 선언 (순환 참조 방지)
class GameWorld;

// 상수 정의
#define MAX_PLAYERS 3

// (추가된 구조체)클라이언트 스레드에서 메인 스레드로 전달할 패킷 정보
struct QueuedPacket {
    unsigned int socketID;
    unsigned int type;
    std::vector<char> data; // PacketHeader를 제외한 순수 페이로드
};

// 클라이언트 연결 정보를 담는 구조체
struct ClientInfo {
    int playerID = -1;
    SOCKET clientSock = INVALID_SOCKET;
    bool is_active = false;
    // std::thread 사용 시 HANDLE 대신 std::thread::id 또는 std::thread 객체 사용 가능
    std::thread client_thread; // 클라이언트 루프 스레드 객체

    std::vector<char> recv_buffer;
};

class ThreadManager {
private:
    // **동기화 자원**
    ClientInfo m_clients[MAX_PLAYERS];
    std::mutex m_mtx;

    // 메인 스레드가 처리할 패킷 대기열 (공유 자원)
    std::queue<QueuedPacket> m_input_queue;

    // 패킷 파싱 및 직렬화 기능을 위한 인스턴스
    PacketManager m_packet_manager;

    // 외부 클래스 참조
    NetworkManager* m_network_manager;

    GameWorld* m_gameWorld;

public:
    // 생성자
    ThreadManager(NetworkManager* netMgr, GameWorld* world);

    // **요청된 5가지 Public 멤버 함수**

    // 1. 서버의 메인 게임 루프 (Main Thread)
    void GameLoop();

    // 2. 새로운 클라이언트 접속 요청 처리 (Accept Thread의 실제 로직)
    void AcceptLoop();

    // 3. 클라이언트 소켓 및 playerID 할당 후 ClientLoop 스레드 생성 (std::thread 사용)
    int AddNewClient(SOCKET clientSock);

    // 4. 할당된 단일 클라이언트와의 네트워크 I/O 전담 (Client Thread의 실제 로직)
    void ClientLoop(int playerID, SOCKET clientSock);

    // 5. 상태 동기화 함수
    void BroadcastState();

    // 6. 메인 스레드의 입력 처리 함수
    void ProcessInputQueue();
};