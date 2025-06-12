#pragma once
#include <windows.h>

void InitMap();
void InitMap2();
void InitMap3();

void Draw();
void Draw_spawn_item();
void Draw_map();
void Draw_castle_blank();
void Draw_mario();
void Draw_item();
void Draw_information();
void Draw_background();
void Draw_fireball();
void Draw_Monsters();
void Draw_Turtles();
void Draw_Angel_Turtles();
void Draw_Bowser();
void Draw_Fireballs();
void DrawFireTraps();

void movePlayer();

void UpdateGame();
void UpdateMario_motion();
void UpdatePlayer();
void UpdateDeadMotion();

void timegoes();

void checkcollision_flag();
void checkcollision_clear();
void checkcollision_coin();
bool IsColliding(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh);
bool IsColliding_item(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh);

void dead();
void resurrection();
void monster_reset();
void item_reset();

void stage_load();