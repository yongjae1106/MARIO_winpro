#include "data.h"

Player mario = { 100, 300, 0, 0, 5, 0, 40, 40, 1, 0, 0, 0, 0, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
// x, y, vx, xy, life, coin, width, height, direction, walk_motion, motion_timer, cooldown_z, cooldown_space, isJumping, isflying, isWalking, isDead, gameover, 
// isBig, god, star, flower, flower_motion, tino, tino_motion, tino_fire_motion, supergod

GameState gameState = GAME_RUNNING;
DWORD transformStartTime = 0;
DWORD starStartTime = 0;
DWORD deadStartTime = 0;
DWORD godstart = 0;
DWORD victoryStart = 0;
DWORD clearStart = 0;

bool gamestart = false;
int title_select = 0;

double cameraX = 0;
int map[MAP_HEIGHT][MAP_WIDTH] = { 0 };

bool keyState[256] = { false };
bool gameclear_text = false;

int stage = 1;
int stage_time = 400;
int frame_motion;
int frame_motion_star;
int map1[MAP_HEIGHT][MAP_WIDTH] = { 0 };
int map2[MAP_HEIGHT][MAP_WIDTH] = { 0 };
int map3[MAP_HEIGHT][MAP_WIDTH] = { 0 };
int (*currentMap)[MAP_WIDTH] = map1;

int worldMarioX = 0;

HDC g_memDC;				// 전역 메모리 DC
HBITMAP g_memBitmap;		// 전역 비트맵
HBITMAP g_oldBitmap;		// SelectObject 이전 비트맵

// 브러쉬
HBRUSH sky_brush = CreateSolidBrush(RGB(148, 148, 255));
HBRUSH black_brush = CreateSolidBrush(RGB(0, 0, 0));

// 정보 텍스트
TCHAR life_print[32], life_print2[32], life_print3[32];
TCHAR time_print[32], time_print2[32];
TCHAR coin_print[32];
TCHAR stage_print[32], stage_print2[32];