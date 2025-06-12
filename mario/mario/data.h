#pragma once
#include <stdio.h>
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "Msimg32.lib")

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640
#define TILE_SIZE 40
#define MAP_WIDTH 200
#define MAP_HEIGHT 15

struct Player
{
    int x, y;
    int vx, vy;
    int life;
    int coin;
    int width, height;
    int direction;      // 0: left  1: right
    int walk_motion;    // 0: 1�����  1: 2�����  2: 3�����
    int motion_timer;  // 0~5�� ���
    int tino_cooldown_z;
    int tino_cooldown_space;
    bool isJumping;
    bool isflying;
    bool isWalking;
    bool isDead;
    bool gameover;
    bool isBig;
    bool god;
    bool star;
    bool flower;
    bool fire_motion;
    bool tino;
    bool tino_motion;
    bool tino_fire_motion;
    bool supergod;
};
extern Player mario;

// ���� ����
enum GameState
{
    GAME_RUNNING,
    GAME_TRANSFORMING,
    GAME_FLOWER_TRANS,
    GAME_TINO_TRANS,
    GAME_VICTORY,
    GAME_CLEAR,
    GAME_OVER
};
extern GameState gameState;
extern DWORD transformStartTime;
extern DWORD starStartTime;
extern DWORD deadStartTime;
extern DWORD godstart;
extern DWORD victoryStart;
extern DWORD clearStart;

extern bool gamestart;
extern int title_select;


extern double cameraX;
// ��: 0�� ����, 1�� ��
extern int map1[MAP_HEIGHT][MAP_WIDTH];
extern int map2[MAP_HEIGHT][MAP_WIDTH];
extern int map3[MAP_HEIGHT][MAP_WIDTH];

extern int worldMarioX;

extern int (*currentMap)[MAP_WIDTH];
extern int stage;
extern int stage_time;

extern int frame_motion;
extern int frame_motion_star;

extern bool keyState[256];
extern bool gameclear_text;

extern HDC g_memDC;				// ���� �޸� DC
extern HBITMAP g_memBitmap;		// ���� ��Ʈ��
extern HBITMAP g_oldBitmap;		// SelectObject ���� ��Ʈ��

// �귯��
extern HBRUSH sky_brush;
extern HBRUSH black_brush;

extern TCHAR life_print[32], life_print2[32], life_print3[32];
extern TCHAR time_print[32], time_print2[32];
extern TCHAR coin_print[32];
extern TCHAR stage_print[32], stage_print2[32];
