#include "map.h"

//3스테이지 불꽃 스위치 불덩이 생성
FireTrapPos fireTrapPositions[] = 
{
    {10, 6, 4},
    {15, 5, 4}, //x축 y축 불덩이갯수
    {20, 6, 4},
    {28, 6, 4}, //위에서 생성
    {24, 5, 4}, //아래에서 생성
    {50, 3, 6},
    {52, 4, 6},
    {55, 3, 6},
    {60, 1,8},
    {70, 1, 8},
    {78, 4, 6},
    {79, 4, 6},
    {76,3,6},
    {81,3,6},
    {105,1,4},
    {107,1,4}
};
Hazard fireTraps[MAX_HAZARDS];
int fireTrapCount = 3;
int fireTrapPositionsCount = sizeof(fireTrapPositions) / sizeof(FireTrapPos);  // 총 개수

//불 스위치
int fireIntervals[MAP_WIDTH] = { 0 };
int fireTimers[MAP_WIDTH] = { 0 };
int fireIntervalsOn[MAP_WIDTH] = { 0 };
int fireIntervalsOff[MAP_WIDTH] = { 0 };
bool fireState[MAP_WIDTH] = { true };

void InitFireTraps() 
{

    fireTrapCount = 0;
    int n = fireTrapPositionsCount;  // 배열 크기 활용

    for (int i = 0; i < n && fireTrapCount < MAX_HAZARDS; i++) {
        fireTraps[fireTrapCount++] = {
            fireTrapPositions[i].tileX * TILE_SIZE + 20,                // x
            fireTrapPositions[i].tileY * TILE_SIZE,                // y
            10,                                             // width
            TILE_SIZE * fireTrapPositions[i].tileCount,            // height
            fireTrapPositions[i].tileCount,                        // tileCount
            true                                                   // active
        };

    }
}

void UpdateFireTraps() 
{
    if (stage != 3) return; // 3스테이지가 아닐 땐 작동 안 하게

    for (int x = 0; x < MAP_WIDTH; x++)
    {
        fireTimers[x]++;
        if (fireState[x]) {
            if (fireTimers[x] > fireIntervalsOn[x]) 
            {
                fireState[x] = false;
                fireTimers[x] = 0;
                fireIntervalsOff[x] = 60 + rand() % 121;
            }
        }
        else {
            if (fireTimers[x] > fireIntervalsOff[x]) 
            {
                fireState[x] = true;
                fireTimers[x] = 0;
                fireIntervalsOn[x] = 30 + rand() % 61;
            }
        }
    }

    for (int i = 0; i < fireTrapCount; i++)
    {
        int tileX = fireTraps[i].x / TILE_SIZE;
        fireTraps[i].active = fireState[tileX];
    }
}

void CheckMarioHazardCollision()
{
    if (mario.star || mario.isDead || mario.god || mario.supergod) return;

    RECT rMario = 
    {
        mario.x, mario.y,
        mario.x + mario.width,
        mario.y + mario.height
    };

    for (int i = 0; i < fireTrapCount; i++) 
    {
        if (!fireTraps[i].active) continue;

        RECT rTrap = {
            fireTraps[i].x - cameraX, fireTraps[i].y,
            fireTraps[i].x - cameraX + fireTraps[i].width,
            fireTraps[i].y + fireTraps[i].height
        };

        RECT intersect;
        if (IntersectRect(&intersect, &rMario, &rTrap))
        {
            // 디버그용 출력
            printf("마리오가 불덩이와 충돌! [%d] at (%d, %d)\n", i, fireTraps[i].x, fireTraps[i].y);

            dead();
            return;
        }
    }
}

void trap_reset()
{
    for (int i = 0; i < MAX_HAZARDS; i++)
    {
        fireTraps[i].active = false;
    }
}