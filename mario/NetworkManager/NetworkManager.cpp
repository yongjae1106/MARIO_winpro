#include "NetworkManager.h"
#include <vector> // For std::vector in NetworkLoop

NetworkManager::NetworkManager() : clientSocket(INVALID_SOCKET), isConnected(false), isRunning(false) {
}

NetworkManager::~NetworkManager() 
{
    Stop();
    if (isConnected) {
        Disconnect();
    }
}

bool NetworkManager::Init() 
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return false;
    }
    std::cout << "NetworkManager Initialized (WSAStartup successful)." << std::endl;
    return true;
}

// 네트워크 스레드 시작
void NetworkManager::Start() {
    if (!isRunning) 
    {
        isRunning = true;
        networkThread = std::thread(&NetworkManager::NetworkLoop, this);
        std::cout << "Network thread started." << std::endl;
    }
}

// 네트워크 스레드 중지
void NetworkManager::Stop() {
    if (isRunning) 
    {
        isRunning = false;
        // 네트워크 스레드가 조인 가능하다면
        if (networkThread.joinable()) 
        {
            // NetworkLoop()가 완전히 종료될 때까지 현재 스레드 대기
            networkThread.join();
        }
        std::cout << "Network thread stopped." << std::endl;
    }
}

// 소켓을 논블로킹 모드로 설정
bool NetworkManager::SetSocketNonBlocking(SOCKET sock) {
    u_long mode = 1; // 1 to enable non-blocking mode
    if (ioctlsocket(sock, FIONBIO, &mode) != 0) {
        std::cerr << "ioctlsocket failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool NetworkManager::Connect(const std::string& ipAddress, int port) 
{
    if (!Init()) 
    {
        return false;
    }

    if (isConnected) 
    {
        Disconnect();
    }

    // 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) 
    {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
    std::cout << "Client socket created successfully." << std::endl;

    // 소켓 논블로킹 모드 설정
    if (!SetSocketNonBlocking(clientSocket)) 
    {
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);

    // 5초간 연결시도를 기다리는 비동기 connecting
    int result = connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR)
    {
        // connect()가 바로 성공하지 않고, WSAEWOULDBLOCK은 "진행 중"을 의미
        if (WSAGetLastError() == WSAEWOULDBLOCK)
        {
            // 연결이 완료될 때까지 기다리기 위해 select() 사용
            fd_set writeSet;                        // 쓰기 가능 상태를 확인할 fd_set
            FD_ZERO(&writeSet);                     // fd_set 초기화
            FD_SET(clientSocket, &writeSet);        // 클라이언트 소켓 등록

            timeval timeout;
            timeout.tv_sec = 5;                     // 최대 5초 동안 대기
            timeout.tv_usec = 0;

            // select() 호출: 소켓이 쓰기 가능해질 때까지 대기
            result = select(0, NULL, &writeSet, NULL, &timeout);
            if (result == 0)
            {
                // 5초간 연결 x, 타임아웃
                std::cerr << "Connection timed out." << std::endl;
                closesocket(clientSocket);
                WSACleanup();
                return false;
            }
            else if (result == SOCKET_ERROR)
            {
                std::cerr << "Select error during connect: " << WSAGetLastError() << std::endl;
                closesocket(clientSocket);
                WSACleanup();
                return false;
            }
            else
            {
                // select()가 성공적으로 반환된 후 연결 성공 여부를 다시 확인
                int optval;
                int optlen = sizeof(optval);
                if (getsockopt(clientSocket, SOL_SOCKET, SO_ERROR, (char*)&optval, &optlen) == SOCKET_ERROR || optval != 0)
                {
                    std::cerr << "Connection failed after select: " << optval << std::endl;
                    closesocket(clientSocket);
                    WSACleanup();
                    return false;
                }
            }
        }
        else
        {
            // WSAEWOULDBLOCK 이외의 다른 오류 발생
            std::cerr << "Connection to server failed: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return false;
        }
    }
    std::cout << "Successfully connected to server at " << ipAddress << ":" << port << std::endl;
    isConnected = true;
    Start();
    return true;
}

void NetworkManager::Disconnect() 
{
    Stop(); // Stop the network thread first
    if (clientSocket != INVALID_SOCKET) 
    {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }
    WSACleanup();
    isConnected = false;
    std::cout << "Disconnected from server and cleaned up Winsock." << std::endl;
}

void NetworkManager::Send(const std::string& data) {
    if (isConnected) 
    {
        sendQueue.push(data);
    } 
    else 
    {
        std::cerr << "Send failed: Not connected to server." << std::endl;
    }
}

bool NetworkManager::TryGetReceivedData(std::string& outData) {
    return receiveQueue.try_pop(outData);
}

bool NetworkManager::IsConnected() const {
    return isConnected;
}

void NetworkManager::NetworkLoop() {
    std::cout << "NetworkLoop started." << std::endl;
    char recvBuffer[4096];

    while (isRunning && isConnected) {
        fd_set readSet, writeSet;
        FD_ZERO(&readSet);
        FD_ZERO(&writeSet);

        // Add clientSocket to readSet if connected
        if (clientSocket != INVALID_SOCKET) {
            FD_SET(clientSocket, &readSet);
        }

        // Add clientSocket to writeSet if there's data to send
        if (!sendQueue.empty() && clientSocket != INVALID_SOCKET) {
            FD_SET(clientSocket, &writeSet);
        }

        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000; // 10ms timeout for select

        int activity = select(0, &readSet, &writeSet, NULL, &timeout);

        if (activity == SOCKET_ERROR) {
            std::cerr << "Select error in NetworkLoop: " << WSAGetLastError() << std::endl;
            // Consider setting isConnected to false and breaking the loop
            break;
        }

        // Handle incoming data
        if (FD_ISSET(clientSocket, &readSet)) {
            int bytesReceived = recv(clientSocket, recvBuffer, sizeof(recvBuffer) - 1, 0);
            if (bytesReceived > 0) {
                recvBuffer[bytesReceived] = '\0';
                receiveQueue.push(std::string(recvBuffer, bytesReceived));
                // std::cout << "NetworkLoop: Received " << bytesReceived << " bytes." << std::endl;
            } else if (bytesReceived == 0) {
                std::cout << "NetworkLoop: Server disconnected." << std::endl;
                isConnected = false;
                break;
            } else {
                int error = WSAGetLastError();
                if (error != WSAEWOULDBLOCK) { // WSAEWOULDBLOCK is expected for non-blocking sockets
                    std::cerr << "NetworkLoop: Recv error: " << error << std::endl;
                    isConnected = false;
                    break;
                }
            }
        }

        // Handle outgoing data
        if (FD_ISSET(clientSocket, &writeSet)) {
            std::string dataToSend;
            while (sendQueue.try_pop(dataToSend)) {
                int bytesSent = send(clientSocket, dataToSend.c_str(), dataToSend.length(), 0);
                if (bytesSent == SOCKET_ERROR) {
                    int error = WSAGetLastError();
                    if (error != WSAEWOULDBLOCK) {
                        std::cerr << "NetworkLoop: Send error: " << error << std::endl;
                        isConnected = false;
                        break; // Break from inner while and outer while
                    } else {
                        // Socket is temporarily not writable, push data back to queue and try again later
                        sendQueue.push(dataToSend);
                        break; // Break from inner while, try again next select loop
                    }
                } else {
                    // std::cout << "NetworkLoop: Sent " << bytesSent << " bytes." << std::endl;
                }
            }
        }
        // Small sleep to prevent busy-waiting if select returns immediately without activity
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "NetworkLoop stopped." << std::endl;
    // If loop breaks due to disconnection, ensure Disconnect is called to clean up
    /*if (!isConnected) {
        Disconnect();
    }*/
    // 스레드 내부에서 Disconnect()를 호출하면 자기 자신을 join()하게 되어 abort 에러가 발생합니다.
    if (clientSocket != INVALID_SOCKET) {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }
    isConnected = false;
}