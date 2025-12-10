#include "Common.h"     // WinSock, printf, err_display 등을 위한 공통 헤더
#include "NetworkManager.h" // NetworkManager 클래스 정의
#include "ThreadManager.h"  // ThreadManager 클래스 정의
#include "GameWorld.h"

int main()
{
    printf("main 시작\n");

    // 1. NetworkManager 생성 및 초기화 (InitServer 호출)
    NetworkManager network_manager;
    // [수정] GameWorld는 싱글턴이므로 직접 생성하지 않고 초기화 함수만 호출합니다.
        // (생성자는 getInstance 호출 시 자동으로 실행됩니다)
    GameWorld::getInstance().init();
    if (!network_manager.InitServer()) {
        // InitServer 실패 시, 내부적으로 err_quit() 호출 또는 메시지 출력 후 종료되었다고 가정.
        printf("[ERROR] NetworkManager 초기화 실패. 서버를 종료합니다.\n");
        return 1;
    }
    printf("네트워크 초기화 및 서버 리스닝 소켓 설정 완료.\n");

    // 2. ThreadManager 생성 및 NetworkManager 바인딩
    // ThreadManager는 서버의 핵심 스레드 및 로직 처리를 담당.
    // [수정] &gameWorld 변수 대신 &GameWorld::getInstance()로 싱글턴 주소를 넘깁니다.
    ThreadManager thread_manager(&network_manager, &GameWorld::getInstance());
    printf("ThreadManager 객체 생성 완료.\n");

    // 3. GameLoop 실행 (서버의 메인 스레드 시작)
    // GameLoop은 AcceptLoop 스레드를 생성하고 메인 게임 루프를 시작.
    thread_manager.GameLoop();

    // GameLoop은 무한 루프이므로, 여기까지 코드가 도달하지 않음.
    // 도달한다면 비정상 종료를 의미함.
    printf("마리오 서버 종료 (비정상 종료 예상)\n");
    return 0;
}