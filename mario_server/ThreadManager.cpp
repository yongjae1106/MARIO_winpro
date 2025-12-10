#include "ThreadManager.h"
#include "NetworkManager.h" // NetworkManager 클래스 사용을 위해 포함
#include "Common.h"
#include "GameWorld.h"
#include "Player.h"


// 생성자
ThreadManager::ThreadManager(NetworkManager* netMgr, GameWorld* world)
    : m_network_manager(netMgr), m_gameWorld(world)
{
    printf("ThreadManager 객체 생성 완료.\n");
}

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
        // 큐에 있는 모든 클라이언트 입력 패킷을 처리합니다.
        ProcessInputQueue();

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

    char buf[512];
    int retval;

    // 수신 버퍼 참조 (이 스레드 전용 자원이므로 while 루프 외부에서 lock 없이 사용)
    std::vector<char>& client_buffer = m_clients[playerID].recv_buffer;

    while (true)
    {
        // 1. recv()를 통해 패킷 수신 시도 (블로킹 모드)
        retval = recv(clientSock, buf, sizeof(buf), 0);

        if (retval == SOCKET_ERROR) {
            // 에러 코드 수정
            int err_code = WSAGetLastError();

            // WSAEWOULDBLOCK은 "아직 데이터가 도착하지 않음"이므로 에러가 아님 -> 무시하고 계속
            if (err_code == WSAEWOULDBLOCK) {
                // CPU 폭주 방지를 위해 살짝 대기 (필요 시 Sleep(0) 또는 Sleep(1))
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }

            // 진짜 에러인 경우에만 종료
            err_display(err_code);
            break;
        }
        if (retval == 0) {
            // 클라이언트 정상 종료
            printf("[Client Thread %d] 클라이언트 연결 종료 감지.\n", playerID);
            break;
        }

        // 2. 패킷 수신 처리
        // 수신된 데이터를 클라이언트의 수신 버퍼에 추가
        if (retval > 0)
        {
            client_buffer.insert(client_buffer.end(), buf, buf + retval);
        }

        // 3. 패킷 큐에 삽입
        // [해야 할 것: 패킷 큐 삽입 시 m_mtx lock/unlock 필요]
        // 버퍼에서 완전한 패킷 추출 및 큐에 삽입 (패킷 조립)
        while (client_buffer.size() >= sizeof(PacketHeader))
        {
            // 패킷 헤더 정보 확인 (바이트 오더링 문제 주의 필요)
            const PacketHeader* header = (const PacketHeader*)client_buffer.data();
            unsigned int packet_length = header->totalLength;
            unsigned int packet_type = header->type;

            // 불완전한 패킷: 다음 recv를 기다림
            if (client_buffer.size() < packet_length)
                break;

            // 완전한 패킷 발견 -> QueuedPacket 생성 및 Payload 추출
            QueuedPacket q_pkt;
            q_pkt.socketID = playerID;
            q_pkt.type = packet_type;

            // 헤더(PacketHeader)를 제외한 순수 Payload만 복사
            q_pkt.data.assign(
                client_buffer.begin() + sizeof(PacketHeader),
                client_buffer.begin() + packet_length
            );

            // 큐에 삽입 (메인 스레드와 공유하는 자원이므로 lock)
            {
                std::lock_guard<std::mutex> lock(m_mtx);
                m_input_queue.push(std::move(q_pkt));
            }

            //printf("[Client Thread %d] Packet Type %u, Size %u 수신 및 큐 삽입 완료.\\n",playerID, packet_type, packet_length);
			// 이놈이 서버 콘솔창에 너무 많이 찍혀서 주석 처리함.

            // 버퍼에서 처리된 패킷 제거
            client_buffer.erase(client_buffer.begin(), client_buffer.begin() + packet_length);
        }
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
    std::lock_guard<std::mutex> lock(m_mtx);

    // 1. [수정] 함수 이름 변경: GetPeerPlayers() -> getPlayers()
    const auto& players = m_gameWorld->getPlayers();

    // 2. 각 플레이어의 정보를 패킷으로 만들어 모든 클라이언트에게 전송
    for (const auto& pair : players) {
        int pID = pair.first;          // 플레이어 ID (Socket ID)
        const Player& p = pair.second; // 플레이어 객체

        // [수정] 패킷 구조체 변경: Packet_MOVE_S2C -> Packet_PLAYER_STATE_S2C
        Packet_PLAYER_STATE_S2C pkt;
        memset(&pkt, 0, sizeof(pkt)); // 안전하게 0으로 초기화

        pkt.playerID = pID;
        pkt.x = p.getX();
        pkt.y = p.getY();
        pkt.vx = p.getVx();
        pkt.vy = p.getVy();
        pkt.life = p.getLife();
        pkt.coin = p.getCoin();
        pkt.width = p.getWidth();
        pkt.height = p.getHeight();
        pkt.direction = p.getDirection();
        pkt.walk_motion = p.getWalkMotion();
        pkt.m_isJumping = p.isJumping();
        pkt.m_isFlying = p.isFlying();
        pkt.m_isWalking = p.isWalking();
        pkt.m_dead = p.isDead();
        pkt.m_gameOver = p.isGameOver();
        pkt.fire_motion = p.isFiring();
        pkt.tino_fire_motion = p.isTinoFireMotion();
        pkt.tino_attack_motion = p.isTinoAttackMotion();
        pkt.currentState = p.getState();
        pkt.state_trans = p.getGameState_trans();
        pkt.transformStartTime = p.getTransformStartTime();
        pkt._isStarGodModeActive = p.isStarGodMode();
        pkt._isSuperGodModeActive = p.isSuperGodMode();
        pkt.tino_cooldown_space = p.getTinoCooldownSpace();
        pkt.fire_motion_timer = p.getFireMotionTimer();
        // pkt.tino_attack_motion_timer = ...; // Player에 getter가 있다면 추가

        // 직렬화
        char sendBuffer[1024];
        // [수정] 함수 이름 변경: Serialize_MOVE -> Serialize_PLAYER_STATE
        unsigned int len = m_packet_manager.Serialize_PLAYER_STATE(sendBuffer, pkt);

        // 3. 모든 활성 클라이언트에게 전송
        for (int i = 0; i < MAX_PLAYERS; ++i) {
            if (m_clients[i].is_active) {
                // 패킷의 주인(pID)에게도 보내야 내 캐릭터가 움직임 (클라 구조상)
                send(m_clients[i].clientSock, sendBuffer, len, 0);
            }
        }
    }
}


// [새로 추가된 함수] ThreadManager::ProcessInputQueue()
void ThreadManager::ProcessInputQueue()
{
    std::lock_guard<std::mutex> lock(m_mtx);

    while (!m_input_queue.empty())
    {
        QueuedPacket pkt = m_input_queue.front();
        m_input_queue.pop();

        // 수정: HandlePacket 호출 시 m_gameWorld 전달
        // Main Thread에서 실행되므로 GameWorld 접근 안전함
        m_packet_manager.HandlePacket(
            pkt.type,
            pkt.data.data(),
            (unsigned int)pkt.data.size(),
            pkt.socketID,
            m_gameWorld // <--- 추가됨
        );
    }
}