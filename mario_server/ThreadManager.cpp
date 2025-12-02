#include "ThreadManager.h"
#include "NetworkManager.h" // NetworkManager Ŭ   
#include "Common.h"
#include "GameWorld.h"
#include "Player.h"


// 
ThreadManager::ThreadManager(NetworkManager* netMgr, GameWorld* world)
    : m_network_manager(netMgr), m_gameWorld(world)
{
    printf("ThreadManager ü  Ϸ.\n");
}

//--------------------------------------------------------------------------------------------------
// 1. ThreadManager::GameLoop() (Main Thread)
// AcceptLoop std::thread Ͽ   
//--------------------------------------------------------------------------------------------------
void ThreadManager::GameLoop()
{
    printf("[Main Thread] GameLoop \n");

    // 1. AcceptLoop  std::thread 
    // std::thread::join() ȣ  ü 
    std::thread hAcceptThread([this] { AcceptLoop(); });

    printf("[Main Thread] AcceptLoop  Ҵ Ϸ.\n");

    while (true)
    {
        // 1. ProcessInputQueue() (Ŷ ó)
        // [ؾ  : Ŷ ť   m_mtx lock/unlock ʿ]
        // ť ִ  Ŭ̾Ʈ Է Ŷ óմϴ.
        ProcessInputQueue();

        // 2. CalculateGamePhysics() ( ) 
        m_gameWorld->update();

        // 3. BroadcastState() ( ) 
        BroadcastState();

        //  ƽ  (Common.h  Sleep  )
        Sleep(16); // 60 fps ƽ 
    }

    //   
    if (hAcceptThread.joinable()) {
        hAcceptThread.join();
    }
}

//--------------------------------------------------------------------------------------------------
// 2. ThreadManager::AcceptLoop() (Accept Thread ) 
//--------------------------------------------------------------------------------------------------
void ThreadManager::AcceptLoop()
{
    printf("[Accept Thread] AcceptLoop : Ŭ̾Ʈ  .\n");

    while (true)
    {
        SOCKET new_client_sock = m_network_manager->AcceptClient();

        if (new_client_sock != INVALID_SOCKET)
        {
            // AddNewClient ȣ
            int player_id = AddNewClient(new_client_sock);

            if (player_id == -1) {
                printf("[Accept Thread] Ŭ̾Ʈ   : ִ ÷̾ (%d) ʰ.\n", MAX_PLAYERS);
                closesocket(new_client_sock);
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
// 3. ThreadManager::AddNewClient()
// ClientLoop    
//--------------------------------------------------------------------------------------------------
int ThreadManager::AddNewClient(SOCKET clientSock)
{
    std::lock_guard<std::mutex> lock(m_mtx); //  ڿ(m_clients)  ȣ

    // 1.  playerID () ã
    int new_id = -1;
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        if (!m_clients[i].is_active) {
            new_id = i;
            break;
        }
    }

    if (new_id == -1) {
        return -1; // ִ ÷̾  ʰ
    }

    // 2. ClientInfo ü 
    m_clients[new_id].playerID = new_id;
    m_clients[new_id].clientSock = clientSock;
    m_clients[new_id].is_active = true;

    m_gameWorld->addPlayer(new_id); // Add player to gameworld

    // 3. ClientLoop    ü  (std::thread )
    //  ĸó [this, new_id, clientSock]    Լ ClientLoop ȣ
    try {
        m_clients[new_id].client_thread = std::thread(
            &ThreadManager::ClientLoop, this, new_id, clientSock);
        m_clients[new_id].client_thread.detach(); // 带 иϿ ThreadManager   
    }
    catch (const std::system_error& e) {
        printf("[Thread Manager] ClientLoop   : %s\n", e.what());
        m_clients[new_id].is_active = false;
        return -1;
    }

    printf("[Thread Manager]  ÷̾ (%d)  Ϸ. ClientLoop  .\n", new_id);
    return new_id;
}

//--------------------------------------------------------------------------------------------------
// 4. ThreadManager::ClientLoop() (Client Thread ) 
//--------------------------------------------------------------------------------------------------
void ThreadManager::ClientLoop(int playerID, SOCKET clientSock)
{
    printf("[Client Thread %d] . Ʈũ I/O .\n", playerID);

    char buf[512];
    int retval;

    //    (   ڿ̹Ƿ while  ܺο lock  )
    std::vector<char>& client_buffer = m_clients[playerID].recv_buffer;

    while (true)
    {
        // 1. recv()  Ŷ  õ (ŷ )
        retval = recv(clientSock, buf, sizeof(buf), 0);

        if (retval == SOCKET_ERROR) {
            //  ڵ 
            int err_code = WSAGetLastError();

            // WSAEWOULDBLOCK " Ͱ  "̹Ƿ  ƴ -> ϰ 
            if (err_code == WSAEWOULDBLOCK) {
                // CPU    ¦  (ʿ  Sleep(0) Ǵ Sleep(1))
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }

            // ¥  쿡 
            err_display(err_code);
            break;
        }
        if (retval == 0) {
            // Ŭ̾Ʈ  
            printf("[Client Thread %d] Ŭ̾Ʈ   .\n", playerID);
            break;
        }

        // 2. Ŷ  ó
        // ŵ ͸ Ŭ̾Ʈ  ۿ ߰
        if (retval > 0)
        {
            client_buffer.insert(client_buffer.end(), buf, buf + retval);
        }

        // 3. Ŷ ť 
        // [ؾ  : Ŷ ť   m_mtx lock/unlock ʿ]
        // ۿ  Ŷ   ť  (Ŷ )
        while (client_buffer.size() >= sizeof(PacketHeader))
        {
            // Ŷ   Ȯ (Ʈ    ʿ)
            const PacketHeader* header = (const PacketHeader*)client_buffer.data();
            unsigned int packet_length = header->totalLength;
            unsigned int packet_type = header->type;

            // ҿ Ŷ:  recv ٸ
            if (client_buffer.size() < packet_length)
                break;

            //  Ŷ ߰ -> QueuedPacket   Payload 
            QueuedPacket q_pkt;
            q_pkt.socketID = playerID;
            q_pkt.type = packet_type;

            // (PacketHeader)   Payload 
            q_pkt.data.assign(
                client_buffer.begin() + sizeof(PacketHeader),
                client_buffer.begin() + packet_length
            );

            // ť  (  ϴ ڿ̹Ƿ lock)
            {
                std::lock_guard<std::mutex> lock(m_mtx);
                m_input_queue.push(std::move(q_pkt));
            }

            //printf("[Client Thread %d] Packet Type %u, Size %u   ť  Ϸ.\n",playerID, packet_type, packet_length);
			// ̳  ܼâ ʹ   ּ ó.

            // ۿ ó Ŷ 
            client_buffer.erase(client_buffer.begin(), client_buffer.begin() + packet_length);
        }
    }

    // **4.     ۾**
    m_mtx.lock(); //  ڿ(m_clients)  ȣ ( lock/unlock)
    if (playerID >= 0 && playerID < MAX_PLAYERS && m_clients[playerID].is_active) {
        printf("[Thread Manager] PlayerID %d   ó .\n", playerID);

        m_gameWorld->removePlayer(playerID); // Remove player from gameworld

        //  ݱ
        closesocket(m_clients[playerID].clientSock);

        //  ʱȭ
        m_clients[playerID].playerID = -1;
        m_clients[playerID].clientSock = INVALID_SOCKET;
        m_clients[playerID].is_active = false;
        // std::thread ü detach ǾǷ  CloseHandle ʿ 

        printf("[Thread Manager] PlayerID %d   Ϸ  .\n", playerID);
    }
    m_mtx.unlock();

    printf("[Client Thread %d] .\n", playerID);
}

//--------------------------------------------------------------------------------------------------
// 5. ThreadManager::BroadcastState()
//--------------------------------------------------------------------------------------------------
void ThreadManager::BroadcastState()
{
    std::lock_guard<std::mutex> lock(m_mtx);

    const auto& players = m_gameWorld->getPlayers(); 

    for (const auto& pair : players) {
        int pID = pair.first;
        const Player& p = pair.second;

        Packet_PLAYER_STATE_S2C pkt;
        pkt.playerID = pID;
        pkt.x = p.getX();
        pkt.y = p.getY();
        pkt.vx = p.getVx();
        pkt.vy = p.getVy();
        pkt.life = m_gameWorld->getLife();
        pkt.coin = m_gameWorld->getCoin();
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
        pkt.tino_attack_motion_timer = 0; // Not available in player class

        char sendBuffer[2048]; // Increased buffer size
        unsigned int len = m_packet_manager.Serialize_PLAYER_STATE(sendBuffer, pkt); 
        
        for (int i = 0; i < MAX_PLAYERS; ++i) {
            if (m_clients[i].is_active) {
                send(m_clients[i].clientSock, sendBuffer, len, 0);
            }
        }
    }
}


// [ ߰ Լ] ThreadManager::ProcessInputQueue() 
void ThreadManager::ProcessInputQueue()
{
    std::lock_guard<std::mutex> lock(m_mtx);

    while (!m_input_queue.empty()) {
        QueuedPacket pkt = m_input_queue.front();
        m_input_queue.pop();

        // : HandlePacket ȣ  m_gameWorld 
        // Main Thread ǹǷ GameWorld  
        m_packet_manager.HandlePacket(
            pkt.type,
            pkt.data.data(),
            (unsigned int)pkt.data.size(),
            pkt.socketID,
            m_gameWorld // <--- ߰
        );
    }
}
