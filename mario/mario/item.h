#pragma once
#include "data.h"
#define MAX_ITEMS 30
#define MAX_SHOT 10

struct Item
{
    int x, y;
    int vx, vy;
    int width, height;
    int spawn_motion;
    bool active;
    bool motion;

    Item(int _x = 0, int _y = 0, int _vx = 0, int _vy = 0, int _width = 30, int _height = 30, int _spawn_motion = 0, bool _active = false, bool _motion = false)
        : x(_x), y(_y), vx(_vx), vy(_vy), width(_width), height(_height), active(_active), spawn_motion(_spawn_motion), motion(_motion) {
    };
};
extern Item mushroom[MAX_ITEMS];
extern Item up_mushroom[MAX_ITEMS];
extern Item star[MAX_ITEMS];
extern Item flower[MAX_ITEMS];
extern Item tino[MAX_ITEMS];
extern int mushroom_count;

struct Shot
{
    int x, y, vx, vy;
    int width, height;
    int motion;
    int motion_fade;
    int duration;
    int direction;
    bool active;
    bool fade;
};
struct Shot_Effect
{
    int x, y;
    int timer;
    bool active;
};
extern Shot fireball[MAX_SHOT];
extern Shot tinofire[MAX_SHOT];
extern Shot_Effect tinofire_effect[MAX_SHOT];

void UpdateItems();
void UpdateItems_up_mushroom();
void UpdateItems_star();
void UpdateItems_flower();
void UpdateItems_tino();
void UpdateShot_fireball();
void UpdateShot_tinofire();
void UpdateShot_tinofire_effect();
void OnMonsterHit(int x, int y);

void transform_bigmario();
void transform_smallmario();
void transform_to_flower();
void transform_to_tino();

void SpawnItem(int x, int y);
void SpawnItem_up_mushroom(int x, int y);
void SpawnItem_star(int x, int y);
void SpawnItem_flower(int x, int y);
void SpawnItem_tino(int x, int y);
void fireball_spawn(int x, int y);
void tinofire_spawn(int x, int y);

void CheckCollision_mushroom();
void CheckCollision_up_mushroom();
void CheckCollision_star();
void CheckCollision_flower();
void CheckCollision_tino();
void CheckCollision_fireball();
void CheckCollision_tinofire();

void tino_attack();

void god_mario(DWORD _godstart);
void star_mario(DWORD _starstart);