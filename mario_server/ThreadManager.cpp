#include "ThreadManager.h"
#include "NetworkManager.h" // NetworkManager 클래스 사용을 위해 포함
#include "Common.h"

//--------------------------------------------------------------------------------------------------
// 1. ThreadManager::GameLoop() (Main Thread)
// AcceptLoop를 std::thread를 사용하여 별도의 스레드로 실행
//--------------------------------------------------------------------------------------------------
void ThreadManager::GameLoop()
{
    printf("[Main Thread] GameLoop 시작\n");

    // 1. AcceptLoop를 별도의 std::thread로 생성
    // std::thread::join() 호출을 위해 객체를 저장
    std::thread hAcceptThread([this] { AcceptLoop(); });

    printf("[Main Thread] AcceptLoop 스레드 할당 완료.\n");

    while (true)
    {
        // 1. ProcessInputQueue() (패킷 처리)
        // [해야 할 것: 패킷 큐 접근 시 m_mtx lock/unlock 필요]

        // 2. CalculateGamePhysics() (로직 계산)

        // 3. BroadcastState() (상태 전파)
        BroadcastState();

        // 게임 틱 제어 (Common.h에 의해 Sleep 사용 가능)
        Sleep(16); // 60 fps 틱 제한
    }

    // 서버 종료 시
    if (hAcceptThread.joinable()) {
        hAcceptThread.join();
    }
}

//--------------------------------------------------------------------------------------------------
// 2. ThreadManager::AcceptLoop() (Accept Thread 로직)
//--------------------------------------------------------------------------------------------------
void ThreadManager::AcceptLoop()
{
    printf("[Accept Thread] AcceptLoop 시작: 클라이언트 연결 대기.\n");

    while (true)
    {
        SOCKET new_client_sock = m_network_manager->AcceptClient();

        if (new_client_sock != INVALID_SOCKET)
        {
            // AddNewClient 호출
            int player_id = AddNewClient(new_client_sock);

            if (player_id == -1) {
                printf("[Accept Thread] 클라이언트 접속 수락 실패: 최대 플레이어 수(%d) 초과.\n", MAX_PLAYERS);
                closesocket(new_client_sock);
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
// 3. ThreadManager::AddNewClient()
// ClientLoop 스레드 생성 로직을 포함
//--------------------------------------------------------------------------------------------------
int ThreadManager::AddNewClient(SOCKET clientSock)
{
    std::lock_guard<std::mutex> lock(m_mtx); // 공유 자원(m_clients) 접근 보호

    // 1. 빈 playerID (슬롯) 찾기
    int new_id = -1;
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        if (!m_clients[i].is_active) {
            new_id = i;
            break;
        }
    }

    if (new_id == -1) {
        return -1; // 최대 플레이어 수 초과
    }

    // 2. ClientInfo 구조체 설정
    m_clients[new_id].playerID = new_id;
    m_clients[new_id].clientSock = clientSock;
    m_clients[new_id].is_active = true;

    // 3. ClientLoop 스레드 생성 및 객체 저장 (std::thread 사용)
    // 람다 캡처 [this, new_id, clientSock]를 통해 비정적 멤버 함수 ClientLoop 호출
    try {
        m_clients[new_id].client_thread = std::thread(
            &ThreadManager::ClientLoop, this, new_id, clientSock);
        m_clients[new_id].client_thread.detach(); // 스레드를 분리하여 ThreadManager가 명시적으로 관리하지 않음
    }
    catch (const std::system_error& e) {
        printf("[Thread Manager] ClientLoop 스레드 생성 실패: %s\n", e.what());
        m_clients[new_id].is_active = false;
        return -1;
    }

    printf("[Thread Manager] 새 플레이어 (%d) 등록 완료. ClientLoop 스레드 생성됨.\n", new_id);
    return new_id;
}

//--------------------------------------------------------------------------------------------------
// 4. ThreadManager::ClientLoop() (Client Thread 로직)
//--------------------------------------------------------------------------------------------------
void ThreadManager::ClientLoop(int playerID, SOCKET clientSock)
{
    printf("[Client Thread %d] 시작. 네트워크 I/O 전담.\n", playerID);

    // ===================================================================
    // [추가] 클라이언트 소켓을 블로킹 모드로 명시적 재설정
    // - Accept 소켓이 리스닝 소켓의 논블로킹 속성을 상속받는 문제를 해결
    // - 이로써 recv()가 데이터가 올 때까지 스레드를 안전하게 대기시킵니다.
    u_long blocking = 0; // 0은 블로킹 모드를 의미
    if (ioctlsocket(clientSock, FIONBIO, &blocking) == SOCKET_ERROR) {
        // 치명적인 오류는 아니지만, 오류 발생 시 로깅
        err_display("ioctlsocket(FIONBIO, blocking) 실패");
        // 오류가 발생해도 스레드는 계속 진행하여 recv()를 호출합니다.
    }
    // ===================================================================

    char buf[512];
    int retval;

    while (true)
    {
        // 1. recv()를 통해 패킷 수신 시도 (블로킹 모드)
        retval = recv(clientSock, buf, sizeof(buf), 0);

        if (retval == SOCKET_ERROR) {
            err_display("recv 실패");
            break;
        }
        if (retval == 0) {
            // 클라이언트 정상 종료
            printf("[Client Thread %d] 클라이언트 연결 종료 감지.\n", playerID);
            break;
        }

        // 2. 패킷 수신 처리
        // [해야 할 것]

        // 3. 패킷 큐에 삽입
        // [해야 할 것: 패킷 큐 삽입 시 m_mtx lock/unlock 필요]

        printf("[Client Thread %d] %d 바이트 데이터 수신 완료.\n", playerID, retval);
    }

    // **4. 스레드 종료 전 정리 작업**
    m_mtx.lock(); // 공유 자원(m_clients) 접근 보호 (수동 lock/unlock)
    if (playerID >= 0 && playerID < MAX_PLAYERS && m_clients[playerID].is_active) {
        printf("[Thread Manager] PlayerID %d 연결 해제 처리 시작.\n", playerID);

        // 소켓 닫기
        closesocket(m_clients[playerID].clientSock);

        // 상태 초기화
        m_clients[playerID].playerID = -1;
        m_clients[playerID].clientSock = INVALID_SOCKET;
        m_clients[playerID].is_active = false;
        // std::thread 객체는 detach 되었으므로 명시적인 CloseHandle이 필요 없음

        printf("[Thread Manager] PlayerID %d 슬롯 해제 완료 및 정리.\n", playerID);
    }
    m_mtx.unlock();

    printf("[Client Thread %d] 종료.\n", playerID);
}

//--------------------------------------------------------------------------------------------------
// 5. ThreadManager::BroadcastState()
//--------------------------------------------------------------------------------------------------
void ThreadManager::BroadcastState()
{
    std::lock_guard<std::mutex> lock(m_mtx); // 공유 자원(m_clients) 접근 보호

    // [해야 할 것: 브로드캐스트할 상태 데이터 생성 로직]
    char broadcast_data[1024] = "SERVER_STATE_UPDATE";
    int data_len = (int)strlen(broadcast_data) + 1;

    // 모든 활성 클라이언트에게 데이터 전송
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        if (m_clients[i].is_active) {
            send(m_clients[i].clientSock, broadcast_data, data_len, 0);
        }
    }
}