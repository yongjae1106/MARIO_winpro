#include "image.h"

HDC mario_DC;

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken = 0;

Image* peach;

Image* stage_1_dirt = nullptr;
Image* stage_1_brick = nullptr;
Image* stage_1_background = nullptr;
Image* stage_2_background = nullptr;

Image* mushroom_head_1 = nullptr;
Image* mushroom_head_2 = nullptr;
Image* mushroom_head_3 = nullptr;
Image* mushroom_trunk_1 = nullptr;
Image* mushroom_trunk_2 = nullptr;

Image* cloud_block = nullptr;

Image* stone_tile;
Image* koopa_block;

Image* fire_head;
Image* fire_body;

Image* fire_switch_tile;

Image* firetrap;

Image* pipe_1 = nullptr;	// 우하
Image* pipe_2 = nullptr;	// 좌하
Image* pipe_3 = nullptr;	// 우상
Image* pipe_4 = nullptr;	// 좌상

Image* item_block_1 = nullptr;
Image* item_block_2 = nullptr;
Image* item_block_3 = nullptr;
Image* item_block_used = nullptr;
Image* unbreakable_block = nullptr;

Image* castle_1 = nullptr;
Image* castle_blank = nullptr;

Image* coin_1 = nullptr;
Image* coin_2 = nullptr;
Image* coin_3 = nullptr;

Image* mario_stop = nullptr;
Image* mario_walk_motion_1 = nullptr;
Image* mario_walk_motion_2 = nullptr;
Image* mario_walk_motion_3 = nullptr;
Image* mario_jump = nullptr;
Image* mario_dead = nullptr;

Image* big_mario_stop = nullptr;
Image* big_mario_walk_motion_1 = nullptr;
Image* big_mario_walk_motion_2 = nullptr;
Image* big_mario_walk_motion_3 = nullptr;
Image* big_mario_jump = nullptr;
Image* big_mario_change = nullptr;

Image* tino_mario_stop;
Image* tino_mario_walk_motion_1;
Image* tino_mario_walk_motion_2;
Image* tino_mario_walk_motion_3;
Image* tino_mario_jump;
Image* tino_mario_attack_1;
Image* tino_mario_attack_2;
Image* tino_mario_attack_3;
Image* tino_mario_attack_4;
Image* tino_mario_attack_5;
Image* tino_mario_attack_6;
Image* tino_mario_fire_1;
Image* tino_mario_fire_2;
Image* tino_mario_fire_R_1;
Image* tino_mario_fire_R_2;
Image* tino_mario_fire_fade_1;
Image* tino_mario_fire_fade_2;
Image* tino_mario_fire_fade_3;

Image* flower_mario_stop = nullptr;
Image* flower_mario_walk_motion_1 = nullptr;
Image* flower_mario_walk_motion_2 = nullptr;
Image* flower_mario_walk_motion_3 = nullptr;
Image* flower_mario_jump = nullptr;
Image* flower_mario_change = nullptr;
Image* flower_mario_fire = nullptr;

Image* star_mario_stop_1 = nullptr;
Image* star_mario_walk_motion_1_1 = nullptr;
Image* star_mario_walk_motion_2_1 = nullptr;
Image* star_mario_walk_motion_3_1 = nullptr;
Image* star_mario_jump_1 = nullptr;

Image* star_big_mario_stop_1 = nullptr;
Image* star_big_mario_walk_motion_1_1 = nullptr;
Image* star_big_mario_walk_motion_2_1 = nullptr;
Image* star_big_mario_walk_motion_3_1 = nullptr;
Image* star_big_mario_jump_1 = nullptr;

Image* star_mario_stop_2 = nullptr;
Image* star_mario_walk_motion_1_2 = nullptr;
Image* star_mario_walk_motion_2_2 = nullptr;
Image* star_mario_walk_motion_3_2 = nullptr;
Image* star_mario_jump_2 = nullptr;

Image* star_big_mario_stop_2 = nullptr;
Image* star_big_mario_walk_motion_1_2 = nullptr;
Image* star_big_mario_walk_motion_2_2 = nullptr;
Image* star_big_mario_walk_motion_3_2 = nullptr;
Image* star_big_mario_jump_2 = nullptr;

Image* star_mario_stop_3 = nullptr;
Image* star_mario_walk_motion_1_3 = nullptr;
Image* star_mario_walk_motion_2_3 = nullptr;
Image* star_mario_walk_motion_3_3 = nullptr;
Image* star_mario_jump_3 = nullptr;

Image* star_big_mario_stop_3 = nullptr;
Image* star_big_mario_walk_motion_1_3 = nullptr;
Image* star_big_mario_walk_motion_2_3 = nullptr;
Image* star_big_mario_walk_motion_3_3 = nullptr;
Image* star_big_mario_jump_3 = nullptr;


Image* item_mushroom = nullptr;
Image* item_up_mushroom = nullptr;
Image* item_star_1 = nullptr;
Image* item_star_2 = nullptr;
Image* item_star_3 = nullptr;
Image* item_star_4 = nullptr;
Image* item_flower_1 = nullptr;
Image* item_flower_2 = nullptr;
Image* item_flower_3 = nullptr;
Image* item_flower_4 = nullptr;
Image* item_tino = nullptr;

Image* shot_fireball_1 = nullptr;
Image* shot_fireball_2 = nullptr;
Image* shot_fireball_3 = nullptr;
Image* shot_fireball_4 = nullptr;
Image* shot_fireball_fadeout_1 = nullptr;
Image* shot_fireball_fadeout_2 = nullptr;
Image* shot_fireball_fadeout_3 = nullptr;

Image* monster1_motion1 = nullptr;
Image* monster1_motion2 = nullptr;
Image* monster1_dead = nullptr;

Image* monster2_motion1 = nullptr;
Image* monster2_motion2 = nullptr;
Image* monster2_dead = nullptr;

Image* monster3_motion1 = nullptr;
Image* monster3_motion2 = nullptr;
Image* monster3_dead = nullptr;

Image* turtle_1 = nullptr;
Image* turtle_2 = nullptr;
Image* turtle_R_1;
Image* turtle_R_2;
Image* turtle_hide = nullptr;

Image* brown_turtle_1;
Image* brown_turtle_2;
Image* brown_turtle_R_1;
Image* brown_turtle_R_2;
Image* brown_turtle_hide;

Image* angel_turtle_1 = nullptr;
Image* angel_turtle_2 = nullptr;

Image* bowser_walk_1 = nullptr;
Image* bowser_walk_2 = nullptr;
Image* bowser_fire_walk_1 = nullptr;
Image* bowser_fire_walk_2 = nullptr;

Image* bowser_fireball_1 = nullptr;
Image* bowser_fireball_2 = nullptr;

Image* screen_coin_1 = nullptr;
Image* screen_coin_2 = nullptr;
Image* screen_coin_3 = nullptr;
Image* screen_coin_x = nullptr;

Image* title_screen = nullptr;
Image* title_cursor = nullptr;
Image* title_dead = nullptr;

Image* flag_stick = nullptr;
Image* flag_marble = nullptr;
Image* flag = nullptr;

int mario_stop_Width;
int mario_stop_Height;

// 이미지 로드
void image_load()
{
    peach = new Image(L"resource\\peach\\peach.png");
    // stage_1
    stage_1_dirt = new Image(L"resource\\tile\\stage_1_dirt.png");
    if (stage_1_dirt->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"stage_1_dirt 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    stage_1_brick = new Image(L"resource\\tile\\stage_1_brick.png");
    if (stage_1_brick->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"stage_1_brick 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    stage_1_background = new Image(L"resource\\background\\stage_1_background.png");
    stage_2_background = new Image(L"resource\\background\\stage_2_background.png");
    if (stage_1_background->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"stage_1_background 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    // stage_2
    mushroom_head_1 = new Image(L"resource\\tile\\mushroom_head_1.png");
    mushroom_head_2 = new Image(L"resource\\tile\\mushroom_head_2.png");
    mushroom_head_3 = new Image(L"resource\\tile\\mushroom_head_3.png");
    mushroom_trunk_1 = new Image(L"resource\\tile\\mushroom_trunk_1.png");
    mushroom_trunk_2 = new Image(L"resource\\tile\\mushroom_trunk_2.png");

    cloud_block = new Image(L"resource\\tile\\cloud_block.png");

    //stage_3 tile
    koopa_block = new Image(L"resource\\tile\\koopa_block.png");
    stone_tile = new Image(L"resource\\tile\\stone_tile.png");
    if (stone_tile->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"stone_tile 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    fire_switch_tile = new Image(L"resource\\tile\\fire_switch_tile.png");
    if (fire_switch_tile->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"fire_switch_tile 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    firetrap = new Image(L"resource\\tile\\fire.png");
    if (firetrap->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"firetrap 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }

    //stage 3 fire
    fire_head = new Image(L"resource\\tile\\fire_head.png");
    if (fire_head->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"fire_head 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    fire_body = new Image(L"resource\\tile\\fire_body.png");
    if (fire_body->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"fire_body 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }

    // pipe
    pipe_1 = new Image(L"resource\\pipe\\pipe_1.png");
    if (pipe_1->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"pipe_1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    pipe_2 = new Image(L"resource\\pipe\\pipe_2.png");
    if (pipe_2->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"pipe_2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    pipe_3 = new Image(L"resource\\pipe\\pipe_3.png");
    if (pipe_3->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"pipe_3 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    pipe_4 = new Image(L"resource\\pipe\\pipe_4.png");
    if (pipe_4->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"pipe_4 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }

    // default block
    item_block_1 = new Image(L"resource\\tile\\item_block_1.png");
    if (item_block_1->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"item_block_1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    item_block_2 = new Image(L"resource\\tile\\item_block_2.png");
    if (item_block_2->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"item_block_2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    item_block_3 = new Image(L"resource\\tile\\item_block_3.png");
    if (item_block_3->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"item_block_3 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    item_block_used = new Image(L"resource\\tile\\item_block_used.png");
    if (item_block_used->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"item_block_used 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    unbreakable_block = new Image(L"resource\\tile\\unbreakable_block.png");
    if (unbreakable_block->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"unbreakable_block 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }

    castle_1 = new Image(L"resource\\tile\\castle_1.png");
    castle_blank = new Image(L"resource\\tile\\castle_blank.png");


    // coin
    coin_1 = new Image(L"resource\\tile\\coin_1.png");
    if (coin_1->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"coin_1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    coin_2 = new Image(L"resource\\tile\\coin_2.png");
    if (coin_2->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"coin_2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    coin_3 = new Image(L"resource\\tile\\coin_3.png");
    if (coin_3->GetLastStatus() != Ok)
    {
        MessageBox(NULL, L"coin_3 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }



    // mario
    mario_stop = new Image(L"resource\\mario\\mario_stop.png");
    if (mario_stop->GetLastStatus() != Ok) {
        MessageBox(NULL, L"mario_stop 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    mario_walk_motion_1 = new Image(L"resource\\mario\\mario_walk_motion_1.png");
    if (mario_walk_motion_1->GetLastStatus() != Ok) {
        MessageBox(NULL, L"mario_walk_motion_1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    mario_walk_motion_2 = new Image(L"resource\\mario\\mario_walk_motion_2.png");
    if (mario_walk_motion_2->GetLastStatus() != Ok) {
        MessageBox(NULL, L"mario_walk_motion_2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    mario_walk_motion_3 = new Image(L"resource\\mario\\mario_walk_motion_3.png");
    if (mario_walk_motion_3->GetLastStatus() != Ok) {
        MessageBox(NULL, L"mario_walk_motion_3 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    mario_jump = new Image(L"resource\\mario\\mario_jump.png");
    if (mario_jump->GetLastStatus() != Ok) {
        MessageBox(NULL, L"mario_jump 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    mario_dead = new Image(L"resource\\mario\\mario_dead.png");
    if (mario_dead->GetLastStatus() != Ok) {
        MessageBox(NULL, L"mario_dead 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    // bigmario
    big_mario_stop = new Image(L"resource\\mario\\big_mario_stop.png");
    if (big_mario_stop->GetLastStatus() != Ok) {
        MessageBox(NULL, L"big_mario_stop 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    big_mario_walk_motion_1 = new Image(L"resource\\mario\\big_mario_stop.png");
    if (big_mario_walk_motion_1->GetLastStatus() != Ok) {
        MessageBox(NULL, L"big_mario_walk_motion_1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    big_mario_walk_motion_2 = new Image(L"resource\\mario\\big_mario_walk_motion_2.png");
    if (big_mario_walk_motion_2->GetLastStatus() != Ok) {
        MessageBox(NULL, L"big_mario_walk_motion_2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    big_mario_walk_motion_3 = new Image(L"resource\\mario\\big_mario_walk_motion_3.png");
    if (big_mario_walk_motion_3->GetLastStatus() != Ok) {
        MessageBox(NULL, L"big_mario_walk_motion_3 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    big_mario_jump = new Image(L"resource\\mario\\big_mario_jump.png");
    if (big_mario_jump->GetLastStatus() != Ok) {
        MessageBox(NULL, L"big_mario_jump 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    big_mario_change = new Image(L"resource\\mario\\big_mario_change.png");
    if (big_mario_change->GetLastStatus() != Ok) {
        MessageBox(NULL, L"big_mario_change 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }

    // tino mario
    tino_mario_stop = new Image(L"resource\\mario\\tino\\Tino_mario_stop.png");
    tino_mario_jump = new Image(L"resource\\mario\\tino\\Tino_mario_jump.png");
    tino_mario_walk_motion_1 = new Image(L"resource\\mario\\tino\\Tino_mario_walk_motion_1.png");
    tino_mario_walk_motion_2 = new Image(L"resource\\mario\\tino\\Tino_mario_walk_motion_2.png");
    tino_mario_walk_motion_3 = new Image(L"resource\\mario\\tino\\Tino_mario_walk_motion_3.png");
    tino_mario_attack_1 = new Image(L"resource\\mario\\tino\\Tino_mario_attack_1.png");
    tino_mario_attack_2 = new Image(L"resource\\mario\\tino\\Tino_mario_attack_2.png");
    tino_mario_attack_3 = new Image(L"resource\\mario\\tino\\Tino_mario_attack_3.png");
    tino_mario_attack_4 = new Image(L"resource\\mario\\tino\\Tino_mario_attack_4.png");
    tino_mario_attack_5 = new Image(L"resource\\mario\\tino\\Tino_mario_attack_5.png");
    tino_mario_attack_6 = new Image(L"resource\\mario\\tino\\Tino_mario_attack_6.png");
    tino_mario_fire_1 = new Image(L"resource\\mario\\tino\\Tino_mario_fire_1.png");
    tino_mario_fire_2 = new Image(L"resource\\mario\\tino\\Tino_mario_fire_2.png");
    tino_mario_fire_R_1 = new Image(L"resource\\mario\\tino\\Tino_mario_fire_R_1.png");
    tino_mario_fire_R_2 = new Image(L"resource\\mario\\tino\\Tino_mario_fire_R_2.png");
    tino_mario_fire_fade_1 = new Image(L"resource\\mario\\tino\\Tino_mario_fire_fade_1.png");
    tino_mario_fire_fade_2 = new Image(L"resource\\mario\\tino\\Tino_mario_fire_fade_2.png");
    tino_mario_fire_fade_3 = new Image(L"resource\\mario\\tino\\Tino_mario_fire_fade_3.png");

    // flower mario
    flower_mario_stop = new Image(L"resource\\mario\\flower\\flower_mario_stop.png");
    flower_mario_jump = new Image(L"resource\\mario\\flower\\flower_mario_jump.png");
    flower_mario_walk_motion_1 = new Image(L"resource\\mario\\flower\\flower_mario_walk_motion_1.png");
    flower_mario_walk_motion_2 = new Image(L"resource\\mario\\flower\\flower_mario_walk_motion_2.png");
    flower_mario_walk_motion_3 = new Image(L"resource\\mario\\flower\\flower_mario_walk_motion_3.png");
    flower_mario_change = new Image(L"resource\\mario\\flower\\flower_mario_change.png");
    flower_mario_fire = new Image(L"resource\\mario\\flower\\flower_mario_fire.png");

    // star mario
    star_mario_stop_1 = new Image(L"resource\\mario\\star\\star_mario_stop_1.png");
    star_mario_stop_2 = new Image(L"resource\\mario\\star\\star_mario_stop_2.png");
    star_mario_stop_3 = new Image(L"resource\\mario\\star\\star_mario_stop_3.png");
    star_mario_walk_motion_1_1 = new Image(L"resource\\mario\\star\\star_mario_walk_motion_1_1.png");
    star_mario_walk_motion_1_2 = new Image(L"resource\\mario\\star\\star_mario_walk_motion_1_2.png");
    star_mario_walk_motion_1_3 = new Image(L"resource\\mario\\star\\star_mario_walk_motion_1_3.png");
    star_mario_walk_motion_2_1 = new Image(L"resource\\mario\\star\\star_mario_walk_motion_2_1.png");
    star_mario_walk_motion_2_2 = new Image(L"resource\\mario\\star\\star_mario_walk_motion_2_2.png");
    star_mario_walk_motion_2_3 = new Image(L"resource\\mario\\star\\star_mario_walk_motion_2_3.png");
    star_mario_walk_motion_3_1 = new Image(L"resource\\mario\\star\\star_mario_walk_motion_3_1.png");
    star_mario_walk_motion_3_2 = new Image(L"resource\\mario\\star\\star_mario_walk_motion_3_2.png");
    star_mario_walk_motion_3_3 = new Image(L"resource\\mario\\star\\star_mario_walk_motion_3_3.png");
    star_mario_jump_1 = new Image(L"resource\\mario\\star\\star_mario_jump_1.png");
    star_mario_jump_2 = new Image(L"resource\\mario\\star\\star_mario_jump_2.png");
    star_mario_jump_3 = new Image(L"resource\\mario\\star\\star_mario_jump_3.png");

    star_big_mario_stop_1 = new Image(L"resource\\mario\\star\\star_big_mario_stop_1.png");
    star_big_mario_stop_2 = new Image(L"resource\\mario\\star\\star_big_mario_stop_2.png");
    star_big_mario_stop_3 = new Image(L"resource\\mario\\star\\star_big_mario_stop_3.png");
    star_big_mario_walk_motion_1_1 = new Image(L"resource\\mario\\star\\star_big_mario_walk_motion_1_1.png");
    star_big_mario_walk_motion_1_2 = new Image(L"resource\\mario\\star\\star_big_mario_walk_motion_1_2.png");
    star_big_mario_walk_motion_1_3 = new Image(L"resource\\mario\\star\\star_big_mario_walk_motion_1_3.png");
    star_big_mario_walk_motion_2_1 = new Image(L"resource\\mario\\star\\star_big_mario_walk_motion_2_1.png");
    star_big_mario_walk_motion_2_2 = new Image(L"resource\\mario\\star\\star_big_mario_walk_motion_2_2.png");
    star_big_mario_walk_motion_2_3 = new Image(L"resource\\mario\\star\\star_big_mario_walk_motion_2_3.png");
    star_big_mario_walk_motion_3_1 = new Image(L"resource\\mario\\star\\star_big_mario_walk_motion_3_1.png");
    star_big_mario_walk_motion_3_2 = new Image(L"resource\\mario\\star\\star_big_mario_walk_motion_3_2.png");
    star_big_mario_walk_motion_3_3 = new Image(L"resource\\mario\\star\\star_big_mario_walk_motion_3_3.png");
    star_big_mario_jump_1 = new Image(L"resource\\mario\\star\\star_big_mario_jump_1.png");
    star_big_mario_jump_2 = new Image(L"resource\\mario\\star\\star_big_mario_jump_2.png");
    star_big_mario_jump_3 = new Image(L"resource\\mario\\star\\star_big_mario_jump_3.png");

    // items
    item_mushroom = new Image(L"resource\\items\\mushroom.png");
    item_up_mushroom = new Image(L"resource\\items\\up_mushroom.png");
    if (item_mushroom->GetLastStatus() != Ok) {
        MessageBox(NULL, L"item_mushroom 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    item_star_1 = new Image(L"resource\\items\\star_1.png");
    if (item_star_1->GetLastStatus() != Ok) {
        MessageBox(NULL, L"item_star_1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    item_star_2 = new Image(L"resource\\items\\star_2.png");
    if (item_star_2->GetLastStatus() != Ok) {
        MessageBox(NULL, L"item_star_2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    item_star_3 = new Image(L"resource\\items\\star_3.png");
    if (item_star_3->GetLastStatus() != Ok) {
        MessageBox(NULL, L"item_star_3 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    item_star_4 = new Image(L"resource\\items\\star_4.png");
    if (item_star_4->GetLastStatus() != Ok) {
        MessageBox(NULL, L"item_star_4 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    item_flower_1 = new Image(L"resource\\items\\flower_1.png");
    item_flower_2 = new Image(L"resource\\items\\flower_2.png");
    item_flower_3 = new Image(L"resource\\items\\flower_3.png");
    item_flower_4 = new Image(L"resource\\items\\flower_4.png");
    item_tino = new Image(L"resource\\items\\tino.png");

    shot_fireball_1 = new Image(L"resource\\items\\shot\\fireball_1.png");
    shot_fireball_2 = new Image(L"resource\\items\\shot\\fireball_2.png");
    shot_fireball_3 = new Image(L"resource\\items\\shot\\fireball_3.png");
    shot_fireball_4 = new Image(L"resource\\items\\shot\\fireball_4.png");
    shot_fireball_fadeout_1 = new Image(L"resource\\items\\shot\\fireball_fadeout_1.png");
    shot_fireball_fadeout_2 = new Image(L"resource\\items\\shot\\fireball_fadeout_2.png");
    shot_fireball_fadeout_3 = new Image(L"resource\\items\\shot\\fireball_fadeout_3.png");

    //monster 1
    monster1_motion1 = new Image(L"resource\\monster\\monster1_motion1.png");
    if (monster1_motion1->GetLastStatus() != Ok) {
        MessageBox(NULL, L"monster1_motion1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    monster1_motion2 = new Image(L"resource\\monster\\monster1_motion2.png");
    if (monster1_motion2->GetLastStatus() != Ok) {
        MessageBox(NULL, L"monster1_motion2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    monster1_dead = new Image(L"resource\\monster\\monster1_dead.png");
    if (monster1_dead->GetLastStatus() != Ok) {
        MessageBox(NULL, L"monster1_dead 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    //monster 2
    monster2_motion1 = new Image(L"resource\\monster\\monster2_motion1.png");
    if (monster2_motion1->GetLastStatus() != Ok) {
        MessageBox(NULL, L"monster1_motion1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    monster2_motion2 = new Image(L"resource\\monster\\monster2_motion2.png");
    if (monster2_motion2->GetLastStatus() != Ok) {
        MessageBox(NULL, L"monster2_motion2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    monster2_dead = new Image(L"resource\\monster\\monster2_dead.png");
    if (monster2_dead->GetLastStatus() != Ok) {
        MessageBox(NULL, L"monster2_dead 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    //monster 3
    monster3_motion1 = new Image(L"resource\\monster\\monster3_motion1.png");
    if (monster3_motion1->GetLastStatus() != Ok) {
        MessageBox(NULL, L"monster3_motion1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    monster3_motion2 = new Image(L"resource\\monster\\monster3_motion2.png");
    if (monster3_motion2->GetLastStatus() != Ok) {
        MessageBox(NULL, L"monster3_motion2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    monster3_dead = new Image(L"resource\\monster\\monster3_dead.png");
    if (monster3_dead->GetLastStatus() != Ok) {
        MessageBox(NULL, L"monster3_dead 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }

    //turtle
    turtle_1 = new Image(L"resource\\monster\\turtle_1.png");
    turtle_2 = new Image(L"resource\\monster\\turtle_2.png");
    turtle_R_1 = new Image(L"resource\\monster\\turtle_R_1.png");
    turtle_R_2 = new Image(L"resource\\monster\\turtle_R_2.png");
    turtle_hide = new Image(L"resource\\monster\\turtle_hide.png");

    brown_turtle_1 = new Image(L"resource\\monster\\brown_turtle_1.png");
    brown_turtle_2 = new Image(L"resource\\monster\\brown_turtle_2.png");
    brown_turtle_R_1 = new Image(L"resource\\monster\\brown_turtle_R_1.png");
    brown_turtle_R_2 = new Image(L"resource\\monster\\brown_turtle_R_2.png");
    brown_turtle_hide = new Image(L"resource\\monster\\brown_turtle_hide.png");

    //angel turtle
    angel_turtle_1 = new Image(L"resource\\monster\\angel_turtle_1.png");
    if (angel_turtle_1->GetLastStatus() != Ok) {
        MessageBox(NULL, L"angel_turtle_1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    angel_turtle_2 = new Image(L"resource\\monster\\angel_turtle_2.png");
    if (angel_turtle_2->GetLastStatus() != Ok) {
        MessageBox(NULL, L"angel_turtle_2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }

    //bowser
    bowser_walk_1 = new Image(L"resource\\monster\\bowser_walk_1.png");
    if (bowser_walk_1->GetLastStatus() != Ok) {
        MessageBox(NULL, L"bowser_walk_1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    bowser_walk_2 = new Image(L"resource\\monster\\bowser_walk_2.png");
    if (bowser_walk_2->GetLastStatus() != Ok) {
        MessageBox(NULL, L"bowser_walk_2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    bowser_fire_walk_1 = new Image(L"resource\\monster\\bowser_fire_walk_1.png");
    if (bowser_fire_walk_1->GetLastStatus() != Ok) {
        MessageBox(NULL, L"bowser_fire_walk_1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    bowser_fire_walk_2 = new Image(L"resource\\monster\\bowser_fire_walk_2.png");
    if (bowser_fire_walk_2->GetLastStatus() != Ok) {
        MessageBox(NULL, L"bowser_fire_walk_2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }

    //fireball
    bowser_fireball_1 = new Image(L"resource\\monster\\bowser_fireball_1.png");
    bowser_fireball_2 = new Image(L"resource\\monster\\bowser_fireball_1.png");

    // screen
    screen_coin_1 = new Image(L"resource\\screen\\screen_coin_1.png");
    if (screen_coin_1->GetLastStatus() != Ok) {
        MessageBox(NULL, L"screen_coin_1 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    screen_coin_2 = new Image(L"resource\\screen\\screen_coin_2.png");
    if (screen_coin_2->GetLastStatus() != Ok) {
        MessageBox(NULL, L"screen_coin_2 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    screen_coin_3 = new Image(L"resource\\screen\\screen_coin_3.png");
    if (screen_coin_3->GetLastStatus() != Ok) {
        MessageBox(NULL, L"screen_coin_3 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    screen_coin_x = new Image(L"resource\\screen\\screen_coin_x.png");
    if (screen_coin_x->GetLastStatus() != Ok) {
        MessageBox(NULL, L"screen_coin_x 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }

    // title
    title_screen = new Image(L"resource\\title\\title_screen.png");
    if (title_screen->GetLastStatus() != Ok) {
        MessageBox(NULL, L"title_screen 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    title_cursor = new Image(L"resource\\title\\title_cursor.png");
    if (title_cursor->GetLastStatus() != Ok) {
        MessageBox(NULL, L"title_cursor 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    title_dead = new Image(L"resource\\title\\title_dead.png");
    if (title_dead->GetLastStatus() != Ok) {
        MessageBox(NULL, L"title_dead 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }

    // flag
    flag_stick = new Image(L"resource\\tile\\flag_stick.png");
    if (flag_stick->GetLastStatus() != Ok) {
        MessageBox(NULL, L"flag_stick 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    flag_marble = new Image(L"resource\\tile\\flag_marble.png");
    if (flag_marble->GetLastStatus() != Ok) {
        MessageBox(NULL, L"flag_marble 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
    flag = new Image(L"resource\\tile\\flag.png");
    if (flag->GetLastStatus() != Ok) {
        MessageBox(NULL, L"flag 이미지 로드 실패!", L"Error", MB_OK);
        exit(1);
    }
}
