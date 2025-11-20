#include <windows.h>
#include "GameWorld.h"
#include "GameRender.h"
#include "Resource_WINAPI/resource1.h"



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
                case 1:
                    GameWorld::getInstance().update();
                    InvalidateRect(hWnd, NULL, FALSE);
                    break;
                case 2:
                    GameWorld::getInstance().updateAnimations();
                    break;
                case 3:
                    GameWorld::getInstance().getPlayer().updateCooldown();
                    break;
            }
            break;
        }
        case WM_KEYDOWN: {
            GameWorld::getInstance().handleKeyDown(wParam);
            break;
        }
        case WM_KEYUP: {
            GameWorld::getInstance().handleKeyUp(wParam);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            GameWorld::getInstance().getGameRender().render(hdc, GameWorld::getInstance());
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY: {
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

    GameWorld::getInstance().sound_init(hWnd);

    GameWorld::getInstance().init();
    GameWorld::getInstance().getGameRender().init();

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
