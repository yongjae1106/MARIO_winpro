#include "ThreadManager.h"
#include "NetworkManager.h" // NetworkManager Ŭ   
#include "Common.h"
#include "GamePhysics.h" // GameWorld 싱글턴 사용을 위해 추가
#include "PacketInfo.h" // ServerPacket 사용을 위해 추가

//--------------------------------------------------------------------------------------------------
// 1. ThreadManager::GameLoop() (Main Thread)
// AcceptLoop std::thread  Ͽ   
//--------------------------------------------------------------------------------------------------
void ThreadManager::GameLoop()
{
    printf("[Main Thread] GameLoop \n");

    // 1. AcceptLoop std::thread  
    // std::thread::join() ȣ  ü 
    std::thread hAcceptThread([this] { AcceptLoop(); });

    printf("[Main Thread] AcceptLoop  Ҵ Ϸ.\n");

    while (true)
    {
        // 1. ProcessInputQueue() (Ŷ ó)
        // [ؾ  : Ŷ ť   m_mtx lock/unlock ʿ]

        // 2. CalculateGamePhysics() ( )
        GameWorld::getInstance().update();

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

    while (true)
    {
        // 1. recv()  Ŷ  õ (ŷ )
        retval = recv(clientSock, buf, sizeof(buf), 0);

        if (retval == SOCKET_ERROR) {
            err_display("recv ");
            break;
        }
        if (retval == 0) {
            // Ŭ̾Ʈ  
            printf("[Client Thread %d] Ŭ̾Ʈ   .\n", playerID);
            break;
        }

        // 2. Ŷ  ó
        // [ؾ  ]

        // 3. Ŷ ť 
        // [ؾ  : Ŷ ť   m_mtx lock/unlock ʿ]

        printf("[Client Thread %d] %d Ʈ   Ϸ.\n", playerID, retval);
    }

    // **4.     ۾**
    m_mtx.lock(); //  ڿ(m_clients)  ȣ ( lock/unlock)
    if (playerID >= 0 && playerID < MAX_PLAYERS && m_clients[playerID].is_active) {
        printf("[Thread Manager] PlayerID %d   ó .\n", playerID);

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
    std::lock_guard<std::mutex> lock(m_mtx); //  ڿ(m_clients)  ȣ

    ServerPacket packet;
    packet.packet_type = SERVER_STATE_UPDATE;
    packet.currentBGM = GameWorld::getInstance().getCurrentBGM();
    packet.events = GameWorld::getInstance().getEventQueue();
    // Other packet data (player positions, monster states, etc.) would go here

    //  Ȱ Ŭ̾Ʈ  
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        if (m_clients[i].is_active) {
            send(m_clients[i].clientSock, (char*)&packet, sizeof(ServerPacket), 0);
        }
    }
}
