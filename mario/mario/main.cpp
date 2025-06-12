// 마리오 스타일 WinAPI 엔진 (카메라 포함, Y 고정)
#define _CRT_SECURE_NO_WARNINGS
#include <tchar.h>
#include "func.h"
#include "data.h"
#include "image.h"
#include "item.h"
#include "monster.h"
#include "sound.h"
#include "resource1.h"

using namespace Gdiplus;

// 윈도우 핸들
HWND hWnd;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    if (!gamestart)
    {
        switch (message)
        {
            case WM_CREATE:
            {
                SetTimer(hWnd, 1, 15, NULL);            // 1번타이머 기본(카메라)타이머
                SetTimer(hWnd, 2, 60, NULL);            // 2번타이머 모션 타이머
                SetTimer(hWnd, 3, 120, NULL);           // 3번타이머 블럭 모션 타이머
                SetTimer(hWnd, 4, 500, NULL);           // 4번타이머 시간초 타이머
                SetTimer(hWnd, 5, 1000, NULL);          // 5번타이머 쿨타임 타이머
                break;
            }
            case WM_KEYDOWN:
            {
                switch (wParam)
                {
                    case VK_UP:
                    {
                        if (title_select == 1)
                        {
                            title_select = 0;
                        }
                        break;
                    }
                    case VK_DOWN:
                    {
                        if (title_select == 0)
                        {
                            title_select = 1;
                        }
                        break;
                    }
                    case VK_RETURN:
                    {
                        if (title_select == 0)
                        {
                            mario_DC = CreateCompatibleDC(g_memDC);

                            stage_load();

                            gamestart = true;
                        }
                        else if (title_select == 1)
                        {
                            exit(1);
                        }
                    }
                }
                InvalidateRect(hWnd, NULL, FALSE);
                break;
            }
            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);

                // 이미지 로드
                Graphics graphics(g_memDC);
                graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

                RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
                FillRect(g_memDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));  // 배경 클리어

                graphics.DrawImage(title_screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);    // 타이틀

                // 커서
                if (title_select == 0)
                {
                    graphics.DrawImage(title_cursor, 220, 380, 25, 25);
                }
                else if (title_select == 1)
                {
                    graphics.DrawImage(title_cursor, 220, 420, 25, 25);
                }
                
                Draw_information();


                BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, g_memDC, 0, 0, SRCCOPY);

                EndPaint(hWnd, &ps);
                break;
            }

            case WM_DESTROY:
            {
                RemoveFontResourceEx(L"SuperMarioBrosNES.ttf", FR_PRIVATE, 0);
                GdiplusShutdown(gdiplusToken);
                PostQuitMessage(0);
                break;
            }

        }
    }
    else
    {
        switch (message)
        {
        case WM_CREATE:
        {
            break;
        }

        case WM_TIMER:
        {
            switch (wParam)
            {
            case(1):
            {
                UpdateGame();
                break;
            }
            case(2):
            {
                if (mario.isWalking)
                {
                    if (mario.walk_motion < 2)
                    {
                        mario.walk_motion++;
                    }
                    else
                    {
                        mario.walk_motion = 0;
                    }
                }
                break;
            }
            case(3):
            {
                if (frame_motion < 6)
                {
                    frame_motion++;
                }
                else
                {
                    frame_motion = 0;
                }
                if (frame_motion_star < 2)
                {
                    frame_motion_star++;
                }
                else
                {
                    frame_motion_star = 0;
                }
                break;
            }
            case(4):
            {
                if (gameState == GAME_OVER || gameState == GAME_VICTORY || gameState == GAME_CLEAR) break;
                timegoes();
                break;
            }
            case(5):
            {
                if (mario.tino_cooldown_z > 0)
                {
                    mario.tino_cooldown_z--;
                }
                if (mario.tino_cooldown_space > 0)
                {
                    mario.tino_cooldown_space--;
                }
            }
            }
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }
        case WM_CHAR:
        {
            switch (wParam)
            {
            case ('z'):
            {
                if (mario.tino)
                {
                    if (mario.tino_cooldown_z > 0) break;
                    bowser.ignore_tinofire = false;
                    PlaySoundBuffer(bowserfire_Sound);
                    mario.tino_fire_motion = true;
                    mario.motion_timer = 10;
                    tinofire_spawn(mario.x + cameraX, mario.y);
                    mario.tino_cooldown_z = 5;
                    break;
                }
            }
            }
            break;
        }
        case WM_KEYDOWN:
        {
            if (gameState == GAME_OVER || gameState == GAME_VICTORY || gameState == GAME_CLEAR) break;
            keyState[wParam] = true;
            mario.isWalking = true;
            switch (wParam)
            {
            case VK_UP:
            {
                if (!mario.isJumping && !mario.isflying)
                {
                    if (mario.isBig || mario.flower)
                    {
                        PlaySoundBuffer(jump_big_Sound);
                    }
                    else if (!mario.isBig)
                    {
                        PlaySoundBuffer(jump_small_Sound);
                    }
                    mario.vy = -19;         // 점프 속도
                    mario.isJumping = true; // 공중 상태로 설정
                }
                break;
            }
            case VK_SPACE:
            {
                if (mario.tino)
                {
                    if (mario.tino_cooldown_space > 0) break;
                    bowser.ignore_tinobite = false;
                    mario.tino_motion = true;
                    mario.motion_timer = 30;
                    tino_attack();
                    mario.god = true;
                    if(!mario.supergod) godstart = GetTickCount();
                    mario.tino_cooldown_space = 15;
                }
            }
            }
            break;
        }
        case WM_KEYUP:
        {
            keyState[wParam] = false;
            switch (wParam)
            {
            case VK_SPACE:
            {
                if (mario.flower)
                {
                    static bool flower_delay_bool = false;
                    static DWORD flower_delay;
                    if (!flower_delay_bool)
                    {
                        flower_delay = GetTickCount();
                        flower_delay_bool = true;
                    }
                    if (GetTickCount() - flower_delay >= 50)
                    {
                        PlaySoundBuffer(fireball_Sound);
                        fireball_spawn(mario.x + cameraX, mario.y);
                        mario.fire_motion = true;
                        mario.motion_timer = 5;
                        flower_delay_bool = false;
                    }

                }
            }
            }
            break;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case(ID_TRANS_TINO):
            {
                if (!mario.tino && gameState == GAME_RUNNING)
                {
                    PlaySoundBuffer(powerup_Sound);
                    if (!mario.flower && !mario.isBig)
                    {
                        mario.y -= TILE_SIZE + 1;       // 위치 맞추기
                    }
                    gameState = GAME_TINO_TRANS;
                    transformStartTime = GetTickCount();
                }
                break;
            }
            case(ID_TRANS_FLOWER):
            {
                if (!mario.flower && gameState == GAME_RUNNING)
                {
                    PlaySoundBuffer(powerup_Sound);
                    if (!mario.tino && !mario.isBig)
                    {
                        mario.y -= TILE_SIZE + 1;       // 위치 맞추기
                    }
                    gameState = GAME_FLOWER_TRANS;
                    transformStartTime = GetTickCount();
                }
                break;
            }
            case(ID_TRANS_BIG):
            {
                if (!mario.isBig && !mario.flower && !mario.tino && gameState == GAME_RUNNING)
                {
                    PlaySoundBuffer(powerup_Sound);
                    mario.y -= TILE_SIZE + 1;       // 위치 맞추기
                    gameState = GAME_TRANSFORMING;
                    transformStartTime = GetTickCount();
                }
                break;
            }
            case(ID_TRANS_STAR):
            {
                PlayBGM("resource\\sound\\bgm\\InvincibilityTheam.wav");
                mario.star = true;
                starStartTime = GetTickCount();
                break;
            }
            }
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // 이미지 로드
            Graphics graphics(g_memDC);
            graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

            // 게임오버
            if (mario.gameover)
            {
                TCHAR gameover[30] = L"GAME OVER";
                TextOut(g_memDC, 360, 300, gameover, lstrlen(gameover));    // 게임오버 문구

                EndPaint(hWnd, &ps);
                break;
            }
            RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            FillRect(g_memDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));  // 배경 클리어

            Draw();
               
            Draw_information();
                
            BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, g_memDC, 0, 0, SRCCOPY);

            EndPaint(hWnd, &ps);
            break;
        }

        case WM_DESTROY:
        {
            RemoveFontResourceEx(L"SuperMarioBrosNES.ttf", FR_PRIVATE, 0);
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 2);
            KillTimer(hWnd, 3);
            KillTimer(hWnd, 4);
            GdiplusShutdown(gdiplusToken);
            PostQuitMessage(0);
            break;
        }
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MarioClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);					// 메뉴이름

    RegisterClass(&wc);
    hWnd = CreateWindow(L"MarioClass", L"SUPERMARIO BROS TINO", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT,
        NULL, NULL, hInstance, NULL);

    // GDI+ 기본설정
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    InitDirectSound(hWnd);
    sound_load();
    image_load();

    // 핸들 설정
    HDC hdc = GetDC(hWnd);
    g_memDC = CreateCompatibleDC(hdc);
    g_memBitmap = CreateCompatibleBitmap(hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
    g_oldBitmap = (HBITMAP)SelectObject(g_memDC, g_memBitmap);
    ReleaseDC(hWnd, hdc);

    // 글꼴
    int loadedFonts = AddFontResourceEx(L"resource\\SuperMarioBrosNES.ttf", FR_PRIVATE, 0);
    if (loadedFonts > 0)
    {
        // 성공적으로 폰트 로드됨!
    }
    else
    {
        MessageBox(NULL, L"SuperMarioBrosNES.ttf 폰트 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    // 폰트 선택
    HFONT hFont = CreateFont(25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Super Mario Bros. NES");
    HFONT hOldFont = (HFONT)SelectObject(g_memDC, hFont);
    SetTextColor(g_memDC, RGB(255, 255, 255));    // 흰색 글꼴
    SetBkMode(g_memDC, TRANSPARENT);    // 투명배경 글꼴

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
