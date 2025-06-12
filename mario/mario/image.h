#pragma once
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;

extern HDC mario_DC;

extern GdiplusStartupInput gdiplusStartupInput;
extern ULONG_PTR gdiplusToken;

extern Image* peach;

extern Image* stage_1_dirt;
extern Image* stage_1_brick;
extern Image* stage_1_background;
extern Image* stage_2_background;

extern Image* mushroom_head_1;
extern Image* mushroom_head_2;
extern Image* mushroom_head_3;
extern Image* mushroom_trunk_1;
extern Image* mushroom_trunk_2;

extern Image* cloud_block;

extern Image* stone_tile;
extern Image* koopa_block;

extern Image* fire_head;
extern Image* fire_body;

extern Image* fire_switch_tile;

extern Image* firetrap;

extern Image* pipe_1;	// ¿ìÇÏ
extern Image* pipe_2;	// ÁÂÇÏ
extern Image* pipe_3;	// ¿ì»ó
extern Image* pipe_4;	// ÁÂ»ó

extern Image* item_block_1;
extern Image* item_block_2;
extern Image* item_block_3;
extern Image* item_block_used;
extern Image* unbreakable_block;

extern Image* castle_1;
extern Image* castle_blank;

extern Image* coin_1;
extern Image* coin_2;
extern Image* coin_3;

extern Image* mario_stop;
extern Image* mario_walk_motion_1;
extern Image* mario_walk_motion_2;
extern Image* mario_walk_motion_3;
extern Image* mario_jump;
extern Image* mario_dead;
extern Image* big_mario_stop;
extern Image* big_mario_walk_motion_1;
extern Image* big_mario_walk_motion_2;
extern Image* big_mario_walk_motion_3;
extern Image* big_mario_jump;
extern Image* big_mario_change;

extern Image* tino_mario_stop;
extern Image* tino_mario_walk_motion_1;
extern Image* tino_mario_walk_motion_2;
extern Image* tino_mario_walk_motion_3;
extern Image* tino_mario_jump;
extern Image* tino_mario_attack_1;
extern Image* tino_mario_attack_2;
extern Image* tino_mario_attack_3;
extern Image* tino_mario_attack_4;
extern Image* tino_mario_attack_5;
extern Image* tino_mario_attack_6;
extern Image* tino_mario_fire_1;
extern Image* tino_mario_fire_2;
extern Image* tino_mario_fire_R_1;
extern Image* tino_mario_fire_R_2;
extern Image* tino_mario_fire_fade_1;
extern Image* tino_mario_fire_fade_2;
extern Image* tino_mario_fire_fade_3;


extern Image* flower_mario_stop;
extern Image* flower_mario_walk_motion_1;
extern Image* flower_mario_walk_motion_2;
extern Image* flower_mario_walk_motion_3;
extern Image* flower_mario_jump;
extern Image* flower_mario_change;
extern Image* flower_mario_fire;

extern Image* star_mario_stop_1;
extern Image* star_mario_walk_motion_1_1;
extern Image* star_mario_walk_motion_2_1;
extern Image* star_mario_walk_motion_3_1;
extern Image* star_mario_jump_1;
extern Image* star_big_mario_stop_1;
extern Image* star_big_mario_walk_motion_1_1;
extern Image* star_big_mario_walk_motion_2_1;
extern Image* star_big_mario_walk_motion_3_1;
extern Image* star_big_mario_jump_1;

extern Image* star_mario_stop_2;
extern Image* star_mario_walk_motion_1_2;
extern Image* star_mario_walk_motion_2_2;
extern Image* star_mario_walk_motion_3_2;
extern Image* star_mario_jump_2;
extern Image* star_big_mario_stop_2;
extern Image* star_big_mario_walk_motion_1_2;
extern Image* star_big_mario_walk_motion_2_2;
extern Image* star_big_mario_walk_motion_3_2;
extern Image* star_big_mario_jump_2;

extern Image* star_mario_stop_3;
extern Image* star_mario_walk_motion_1_3;
extern Image* star_mario_walk_motion_2_3;
extern Image* star_mario_walk_motion_3_3;
extern Image* star_mario_jump_3;
extern Image* star_big_mario_stop_3;
extern Image* star_big_mario_walk_motion_1_3;
extern Image* star_big_mario_walk_motion_2_3;
extern Image* star_big_mario_walk_motion_3_3;
extern Image* star_big_mario_jump_3;


extern Image* item_mushroom;
extern Image* item_up_mushroom;
extern Image* item_star_1;
extern Image* item_star_2;
extern Image* item_star_3;
extern Image* item_star_4;
extern Image* item_flower_1;
extern Image* item_flower_2;
extern Image* item_flower_3;
extern Image* item_flower_4;
extern Image* item_tino;

extern Image* shot_fireball_1;
extern Image* shot_fireball_2;
extern Image* shot_fireball_3;
extern Image* shot_fireball_4;
extern Image* shot_fireball_fadeout_1;
extern Image* shot_fireball_fadeout_2;
extern Image* shot_fireball_fadeout_3;

extern Image* monster1_motion1;
extern Image* monster1_motion2;
extern Image* monster1_dead;

extern Image* monster2_motion1;
extern Image* monster2_motion2;
extern Image* monster2_dead;

extern Image* monster3_motion1;
extern Image* monster3_motion2;
extern Image* monster3_dead;

extern Image* turtle_1;
extern Image* turtle_2;
extern Image* turtle_R_1;
extern Image* turtle_R_2;
extern Image* turtle_hide;

extern Image* brown_turtle_1;
extern Image* brown_turtle_2;
extern Image* brown_turtle_R_1;
extern Image* brown_turtle_R_2;
extern Image* brown_turtle_hide;

extern Image* angel_turtle_1;
extern Image* angel_turtle_2;

extern Image* bowser_walk_1;
extern Image* bowser_walk_2;
extern Image* bowser_fire_walk_1;
extern Image* bowser_fire_walk_2;

extern Image* bowser_fireball_1;
extern Image* bowser_fireball_2;

extern Image* screen_coin_1;
extern Image* screen_coin_2;
extern Image* screen_coin_3;
extern Image* screen_coin_x;

extern Image* title_screen;
extern Image* title_cursor;
extern Image* title_dead;

extern Image* flag_stick;
extern Image* flag_marble;
extern Image* flag;

extern int mario_stop_Width;
extern int mario_stop_Height;

extern void image_load();