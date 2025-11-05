#pragma once
#include "data.h"
#include "monster.h"
#include "func.h"
#include "image.h"
#define MAX_HAZARDS 100
struct Hazard
{
    int x, y;
    int width, height;
    int tileCount;
    bool active;
};
struct FireTrapPos
{
    int tileX;
    int tileY;
    int tileCount; // 붙일 이미지 수
};
extern FireTrapPos fireTrapPositions[];
extern int fireTrapPositionsCount;
extern Hazard fireTraps[MAX_HAZARDS];
extern int fireTrapCount;
void InitFireTraps();
void UpdateFireTraps();
void CheckMarioHazardCollision();
void trap_reset();
