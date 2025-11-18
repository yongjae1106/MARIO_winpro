#include "Common.h"
#include <mutex> // ȭ ü
#include <thread> // std::thread  (Wrapper Լ ü)
#include "GamePhysics.h" // GameWorld 싱글턴 사용을 위해 추가

// NetworkManager Ŭ  (ȯ  )
class NetworkManager;

//  
#define MAX_PLAYERS 3

// Ŭ̾Ʈ    ü
struct ClientInfo {
    int playerID = -1;
    SOCKET clientSock = INVALID_SOCKET;
    bool is_active = false;
    // std::thread   HANDLE  std::thread::id Ǵ std::thread ü  
    std::thread client_thread; // Ŭ̾Ʈ   ü
};

class ThreadManager {
private:
    // **ȭ ڿ**
    ClientInfo m_clients[MAX_PLAYERS];
    std::mutex m_mtx;

    // ܺ Ŭ 
    NetworkManager* m_network_manager;

public:
    // 
    ThreadManager(NetworkManager* nm) : m_network_manager(nm) {}

    // **û 5 Public  Լ**

    // 1.     (Main Thread)
    void GameLoop();

    // 2. ο Ŭ̾Ʈ  û ó (Accept Thread  )
    void AcceptLoop();

    // 3. Ŭ̾Ʈ   playerID Ҵ  ClientLoop   (std::thread )
    int AddNewClient(SOCKET clientSock);

    // 4. Ҵ  Ŭ̾Ʈ Ʈũ I/O  (Client Thread  )
    void ClientLoop(int playerID, SOCKET clientSock);

    // 5.  ȭ Լ
    void BroadcastState();
};