// <windows.h> 위에 "NetworkManager/NetworkManager.h" 를 include 하도록 순서를 바꾸었습니다.(오류 발생)
#define WIN32_LEAN_AND_MEAN // Winsock 1 헤더 자동 포함 방지 (소켓 재정의 에러 해결)

#include "NetworkManager/NetworkManager.h"
#include <windows.h>
#include <ole2.h> // GDI+ 사용을 위해 OLE 헤더 수동 포함 (IStream 에러 해결)
#include "GameWorld.h"
#include "GameRender.h"
#include "Resource_WINAPI/resource1.h"

// Global variables
GameWorld gameWorld;
GameRender gameRender;

NetworkManager networkManager;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
            SetTimer(hWnd, 1, 15, NULL);
            SetTimer(hWnd, 2, 15, NULL);
            SetTimer(hWnd, 3, 1000, NULL);
            break;
        }
        case WM_TIMER: {
            switch(wParam) {
                case(1):
                    gameWorld.update();
                    InvalidateRect(hWnd, NULL, FALSE);
                    break;
                case(2):
                    gameWorld.updateAnimations();
                    break;
                case(3):
                    gameWorld.getPlayer().updateCooldown();
                    break;
            }
            break;
        }
        case WM_KEYDOWN: {
            gameWorld.handleKeyDown(wParam);
            break;
        }
        case WM_KEYUP: {
            gameWorld.handleKeyUp(wParam);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            gameRender.render(hdc, gameWorld);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY: {
            // 종료 시 연결 해제
            networkManager.Disconnect();
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MarioClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

    RegisterClass(&wc);

    HWND hWnd = CreateWindow(L"MarioClass", L"SUPERMARIO BROS TINO", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 640,
        NULL, NULL, hInstance, NULL);

    gameWorld.sound_init(hWnd);

    gameWorld.init();
    gameRender.init();

    if (networkManager.Connect("127.0.0.1", 9000)) {
        OutputDebugString(L"[Client] 서버 연결 성공!\n");
    }
    else {
        OutputDebugString(L"[Client] 서버 연결 실패...\n");
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
