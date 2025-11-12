#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <map>
#include <string>
#include <memory>

class GameWorld;
class Player;

class GameRender {
public:
    GameRender();
    ~GameRender();

    void init();
    void render(HDC hdc, const GameWorld& world);


    Gdiplus::Image* getGoombaImage();
    Gdiplus::Image* getGoombaImage2();
    Gdiplus::Image* getTurtleImage();
    Gdiplus::Image* getTurtleImage2();
    Gdiplus::Image* getTurtleShellImage();
    Gdiplus::Image* getBowserImage1();
    Gdiplus::Image* getBowserImage2();

    Gdiplus::Image* getMushroomImage();
    Gdiplus::Image* getStarImage();
    Gdiplus::Image* getStarImage2();
    Gdiplus::Image* getStarImage3();
    Gdiplus::Image* getStarImage4();
    Gdiplus::Image* getFlowerImage1();
    Gdiplus::Image* getFlowerImage2();
    Gdiplus::Image* getFlowerImage3();
    Gdiplus::Image* getFlowerImage4();

    Gdiplus::Image* getFireballImage();
    Gdiplus::Image* getTinoImage();

    Gdiplus::Image* getCoinImage1();
    Gdiplus::Image* getCoinImage2();
    Gdiplus::Image* getCoinImage3();

    Gdiplus::Image* getPlayerFireballImage();

private:

    void loadImages();
    Gdiplus::Image* loadImage(const WCHAR* path);

    // Private rendering functions
    void drawBackground(Gdiplus::Graphics& graphics, const GameWorld& world);
    void drawMap(Gdiplus::Graphics& graphics, const GameWorld& world);

    void drawPlayer(Gdiplus::Graphics& graphics, const GameWorld& world, const Player& player);
    void drawMonsters(Gdiplus::Graphics& graphics, const GameWorld& world);
    void drawItems(Gdiplus::Graphics& graphics, const GameWorld& world);
    void drawUI(Gdiplus::Graphics& graphics, const GameWorld& world);
    void drawTitleScreen(Gdiplus::Graphics& graphics, const GameWorld& world);
    void drawTitleDead(Gdiplus::Graphics& graphics, const GameWorld& world);

    // GDI+ startup token
    ULONG_PTR gdiplusToken;

    // Brushes
    HBRUSH sky_brush;
    HBRUSH black_brush;

    // Images
    Gdiplus::Image* peach;
    Gdiplus::Image* stage_1_dirt;
    Gdiplus::Image* stage_1_brick;
    Gdiplus::Image* stage_1_background;
    Gdiplus::Image* stage_2_background;
    Gdiplus::Image* mushroom_head_1;
    Gdiplus::Image* mushroom_head_2;
    Gdiplus::Image* mushroom_head_3;
    Gdiplus::Image* mushroom_trunk_1;
    Gdiplus::Image* mushroom_trunk_2;
    Gdiplus::Image* cloud_block;
    Gdiplus::Image* stone_tile;
    Gdiplus::Image* koopa_block;
    Gdiplus::Image* fire_head;
    Gdiplus::Image* fire_body;
    Gdiplus::Image* fire_switch_tile;
    Gdiplus::Image* firetrap;
    Gdiplus::Image* pipe_1;
    Gdiplus::Image* pipe_2;
    Gdiplus::Image* pipe_3;
    Gdiplus::Image* pipe_4;
    Gdiplus::Image* item_block_1;
    Gdiplus::Image* item_block_2;
    Gdiplus::Image* item_block_3;
    Gdiplus::Image* item_block_used;
    Gdiplus::Image* unbreakable_block;
    Gdiplus::Image* castle_1;
    Gdiplus::Image* castle_blank;
    Gdiplus::Image* coin_1;
    Gdiplus::Image* coin_2;
    Gdiplus::Image* coin_3;
    Gdiplus::Image* mario_stop;
    Gdiplus::Image* mario_walk_motion_1;
    Gdiplus::Image* mario_walk_motion_2;
    Gdiplus::Image* mario_walk_motion_3;
    Gdiplus::Image* mario_jump;
    Gdiplus::Image* mario_dead;
    Gdiplus::Image* big_mario_stop;
    Gdiplus::Image* big_mario_walk_motion_1;
    Gdiplus::Image* big_mario_walk_motion_2;
    Gdiplus::Image* big_mario_walk_motion_3;
    Gdiplus::Image* big_mario_jump;
    Gdiplus::Image* big_mario_change;
    Gdiplus::Image* tino_mario_stop;
    Gdiplus::Image* tino_mario_walk_motion_1;
    Gdiplus::Image* tino_mario_walk_motion_2;
    Gdiplus::Image* tino_mario_walk_motion_3;
    Gdiplus::Image* tino_mario_jump;
    Gdiplus::Image* tino_mario_attack_1;
    Gdiplus::Image* tino_mario_attack_2;
    Gdiplus::Image* tino_mario_attack_3;
    Gdiplus::Image* tino_mario_attack_4;
    Gdiplus::Image* tino_mario_attack_5;
    Gdiplus::Image* tino_mario_attack_6;
    Gdiplus::Image* tino_mario_fire_1;
    Gdiplus::Image* tino_mario_fire_2;
    Gdiplus::Image* tino_mario_fire_R_1;
    Gdiplus::Image* tino_mario_fire_R_2;
    Gdiplus::Image* tino_mario_fire_fade_1;
    Gdiplus::Image* tino_mario_fire_fade_2;
    Gdiplus::Image* tino_mario_fire_fade_3;
    Gdiplus::Image* flower_mario_stop;
    Gdiplus::Image* flower_mario_walk_motion_1;
    Gdiplus::Image* flower_mario_walk_motion_2;
    Gdiplus::Image* flower_mario_walk_motion_3;
    Gdiplus::Image* flower_mario_jump;
    Gdiplus::Image* flower_mario_change;
    Gdiplus::Image* flower_mario_fire;
    Gdiplus::Image* star_mario_stop_1;
    Gdiplus::Image* star_mario_walk_motion_1_1;
    Gdiplus::Image* star_mario_walk_motion_2_1;
    Gdiplus::Image* star_mario_walk_motion_3_1;
    Gdiplus::Image* star_mario_jump_1;
    Gdiplus::Image* star_big_mario_stop_1;
    Gdiplus::Image* star_big_mario_walk_motion_1_1;
    Gdiplus::Image* star_big_mario_walk_motion_2_1;
    Gdiplus::Image* star_big_mario_walk_motion_3_1;
    Gdiplus::Image* star_big_mario_jump_1;
    Gdiplus::Image* star_mario_stop_2;
    Gdiplus::Image* star_mario_walk_motion_1_2;
    Gdiplus::Image* star_mario_walk_motion_2_2;
    Gdiplus::Image* star_mario_walk_motion_3_2;
    Gdiplus::Image* star_mario_jump_2;
    Gdiplus::Image* star_big_mario_stop_2;
    Gdiplus::Image* star_big_mario_walk_motion_1_2;
    Gdiplus::Image* star_big_mario_walk_motion_2_2;
    Gdiplus::Image* star_big_mario_walk_motion_3_2;
    Gdiplus::Image* star_big_mario_jump_2;
    Gdiplus::Image* star_mario_stop_3;
    Gdiplus::Image* star_mario_walk_motion_1_3;
    Gdiplus::Image* star_mario_walk_motion_2_3;
    Gdiplus::Image* star_mario_walk_motion_3_3;
    Gdiplus::Image* star_mario_jump_3;
    Gdiplus::Image* star_big_mario_stop_3;
    Gdiplus::Image* star_big_mario_walk_motion_1_3;
    Gdiplus::Image* star_big_mario_walk_motion_2_3;
    Gdiplus::Image* star_big_mario_walk_motion_3_3;
    Gdiplus::Image* star_big_mario_jump_3;
    Gdiplus::Image* item_mushroom;
    Gdiplus::Image* item_up_mushroom;
    Gdiplus::Image* item_star_1;
    Gdiplus::Image* item_star_2;
    Gdiplus::Image* item_star_3;
    Gdiplus::Image* item_star_4;
    Gdiplus::Image* item_flower_1;
    Gdiplus::Image* item_flower_2;
    Gdiplus::Image* item_flower_3;
    Gdiplus::Image* item_flower_4;
    Gdiplus::Image* item_tino;
    Gdiplus::Image* shot_fireball_1;
    Gdiplus::Image* shot_fireball_2;
    Gdiplus::Image* shot_fireball_3;
    Gdiplus::Image* shot_fireball_4;
    Gdiplus::Image* shot_fireball_fadeout_1;
    Gdiplus::Image* shot_fireball_fadeout_2;
    Gdiplus::Image* shot_fireball_fadeout_3;
    Gdiplus::Image* monster1_motion1;
    Gdiplus::Image* monster1_motion2;
    Gdiplus::Image* monster1_dead;
    Gdiplus::Image* monster2_motion1;
    Gdiplus::Image* monster2_motion2;
    Gdiplus::Image* monster2_dead;
    Gdiplus::Image* monster3_motion1;
    Gdiplus::Image* monster3_motion2;
    Gdiplus::Image* monster3_dead;
    Gdiplus::Image* turtle_1;
    Gdiplus::Image* turtle_2;
    Gdiplus::Image* turtle_R_1;
    Gdiplus::Image* turtle_R_2;
    Gdiplus::Image* turtle_hide;
    Gdiplus::Image* brown_turtle_1;
    Gdiplus::Image* brown_turtle_2;
    Gdiplus::Image* brown_turtle_R_1;
    Gdiplus::Image* brown_turtle_R_2;
    Gdiplus::Image* brown_turtle_hide;
    Gdiplus::Image* angel_turtle_1;
    Gdiplus::Image* angel_turtle_2;
    Gdiplus::Image* bowser_walk_1;
    Gdiplus::Image* bowser_walk_2;
    Gdiplus::Image* bowser_fire_walk_1;
    Gdiplus::Image* bowser_fire_walk_2;
    Gdiplus::Image* bowser_fireball_1;
    Gdiplus::Image* bowser_fireball_2;
    Gdiplus::Image* screen_coin_1;
    Gdiplus::Image* screen_coin_2;
    Gdiplus::Image* screen_coin_3;
    Gdiplus::Image* screen_coin_x;
    Gdiplus::Image* title_screen;
    Gdiplus::Image* title_cursor;
    Gdiplus::Image* title_dead;
    Gdiplus::Image* flag_stick;
    Gdiplus::Image* flag_marble;
    Gdiplus::Image* flag;
    Gdiplus::PrivateFontCollection m_fontCollection;
    std::unique_ptr<Gdiplus::Font> m_font;
};
