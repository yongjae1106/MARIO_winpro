#include "GameRender.h"
#include "GameWorld.h"
#include "Player.h"
#include "items/Star.h"
#include "items/Flower.h"
#include "items/Tino.h"
#include "monsters/Turtle.h"
#include <tchar.h>
#include <gdiplus.h>
#include <mmsystem.h>  // PlaySound 함수 포함

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "winmm.lib")

using namespace Gdiplus;

GameRender::GameRender() : gdiplusToken(0) {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    sky_brush = CreateSolidBrush(RGB(148, 148, 255));
    black_brush = CreateSolidBrush(RGB(0, 0, 0));
}

GameRender::~GameRender() {
    m_font.reset();
    // ... delete images

    DeleteObject(sky_brush);
    DeleteObject(black_brush);

    Gdiplus::GdiplusShutdown(gdiplusToken);
}

void GameRender::init() {
    loadImages();
    loadSounds();

    // Load the font
    m_fontCollection.AddFontFile(L"resource/SuperMarioBrosNES.ttf");
    
    // Get the font family
    int numFound = 0;
    Gdiplus::FontFamily fontFamily;
    m_fontCollection.GetFamilies(1, &fontFamily, &numFound);

    if (numFound > 0) {
        m_font = std::make_unique<Gdiplus::Font>(&fontFamily, 25, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    } else {
        // Fallback to a default font if the custom font fails to load
        m_font = std::make_unique<Gdiplus::Font>(L"Arial", 25, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    }
}

void GameRender::loadSounds() {
    m_sounds[L"1-up"] = L"resource/sound/smb_1-up.wav";
    m_sounds[L"bowserdead"] = L"resource/sound/smb_bowserdead.wav";
    m_sounds[L"bowserfalls"] = L"resource/sound/smb_bowserfalls.wav";
    m_sounds[L"bowserfire"] = L"resource/sound/smb_bowserfire.wav";
    m_sounds[L"bump"] = L"resource/sound/smb_bump.wav";
    m_sounds[L"coin"] = L"resource/sound/smb_coin.wav";
    m_sounds[L"fireball"] = L"resource/sound/smb_fireball.wav";
    m_sounds[L"gameover"] = L"resource/sound/smb_gameover.wav";
    m_sounds[L"jump-small"] = L"resource/sound/smb_jump-small.wav";
    m_sounds[L"jump-super"] = L"resource/sound/smb_jump-super.wav";
    m_sounds[L"kick"] = L"resource/sound/smb_kick.wav";
    m_sounds[L"mariodie"] = L"resource/sound/smb_mariodie.wav";
    m_sounds[L"pipe"] = L"resource/sound/smb_pipe.wav";
    m_sounds[L"powerup_appears"] = L"resource/sound/smb_powerup_appears.wav";
    m_sounds[L"powerup"] = L"resource/sound/smb_powerup.wav";
    m_sounds[L"stage_clear"] = L"resource/sound/smb_stage_clear.wav";
    m_sounds[L"stomp"] = L"resource/sound/smb_stomp.wav";
    m_sounds[L"tino_attack"] = L"resource/sound/smb_tino_attack.wav";
    m_sounds[L"world_clear"] = L"resource/sound/smb_world_clear.wav";
}

void GameRender::playSound(const WCHAR* sound) {
    auto it = m_sounds.find(sound);
    if (it != m_sounds.end()) {
        PlaySound(it->second.c_str(), NULL, SND_FILENAME | SND_ASYNC);
    }
}

void GameRender::render(HDC hdc, const GameWorld& world) {
    HDC memDC = CreateCompatibleDC(hdc);
    RECT clientRect = { 0, 0, 800, 640 };
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

    Gdiplus::Graphics graphics(memDC);
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);

    if (world.getGameState() == GameState::GAME_TITLE) {
        drawTitleScreen(graphics, world);
    } else {
        drawBackground(graphics, world);
        drawMap(graphics, world);
        drawItems(graphics, world);
        drawMonsters(graphics, world);
        drawPlayer(graphics, world, world.getPlayer());
        drawUI(graphics, world);
    }

    BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
}

Gdiplus::Image* GameRender::getMushroomImage() { return item_mushroom; }
Gdiplus::Image* GameRender::getStarImage() { return item_star_1; }

Gdiplus::Image* GameRender::getCoinImage1() { return coin_1; }
Gdiplus::Image* GameRender::getCoinImage2() { return coin_2; }
Gdiplus::Image* GameRender::getCoinImage3() { return coin_3; }

Gdiplus::Image* GameRender::getFireballImage() { return shot_fireball_1; }

Gdiplus::Image* GameRender::getStarImage2() { return item_star_2; }
Gdiplus::Image* GameRender::getStarImage3() { return item_star_3; }
Gdiplus::Image* GameRender::getStarImage4() { return item_star_4; }
Gdiplus::Image* GameRender::getFlowerImage1() { return item_flower_1; }
Gdiplus::Image* GameRender::getFlowerImage2() { return item_flower_2; }
Gdiplus::Image* GameRender::getFlowerImage3() { return item_flower_3; }
Gdiplus::Image* GameRender::getFlowerImage4() { return item_flower_4; }
Gdiplus::Image* GameRender::getTinoImage() { return item_tino; }

Gdiplus::Image* GameRender::getGoombaImage() { return monster1_motion1; }
Gdiplus::Image* GameRender::getTurtleImage() { return turtle_1; }

Gdiplus::Image* GameRender::getGoombaImage2() { return monster1_motion2; }
Gdiplus::Image* GameRender::getTurtleShellImage() { return turtle_hide; }

Gdiplus::Image* GameRender::getTurtleImage2() { return turtle_2; }
Gdiplus::Image* GameRender::getBowserImage1() { return bowser_walk_1; }
Gdiplus::Image* GameRender::getBowserImage2() { return bowser_walk_2; }

void GameRender::drawItems(Gdiplus::Graphics& graphics, const GameWorld& world) {
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);

    for (const auto& item : world.getItems()) {
        if (!item->isActive()) continue;

        int screenX = item->getX() - world.getCameraX();
        int screenY = item->getY();

        Gdiplus::Image* imageToDraw = nullptr;

        switch (item->getType()) {
            case Item::ItemType::Mushroom:
                imageToDraw = getMushroomImage();
                break;
            case Item::ItemType::Star:
            {
                int globalFrame = world.getGlobalAnimationFrameCounter();
                // Star has 4 frames (0, 1, 2, 3). Let's say it animates every 5 global frames.
                int frame = (globalFrame / 5) % 4;
                if (frame == 0) imageToDraw = item_star_1; // Use direct image pointers
                else if (frame == 1) imageToDraw = item_star_2;
                else if (frame == 2) imageToDraw = item_star_3;
                else imageToDraw = item_star_4;
                break;
            }

            case Item::ItemType::Flower:
            {
                int globalFrame = world.getGlobalAnimationFrameCounter();
                // Flower has 4 frames (0, 1, 2, 3). Let's say it animates every 5 global frames.
                int frame = (globalFrame / 5) % 4;
                if (frame == 0) imageToDraw = item_flower_1; // Use direct image pointers
                else if (frame == 1) imageToDraw = item_flower_2;
                else if (frame == 2) imageToDraw = item_flower_3;
                else imageToDraw = item_flower_4;
                break;
            }
            case Item::ItemType::Tino:
            {
                imageToDraw = item_tino;
                break;
            }
            case Item::ItemType::Fireball:
                imageToDraw = getFireballImage();
                break;
            case Item::ItemType::PlayerFireball:
                imageToDraw = getPlayerFireballImage();
                break;
            // ... other item types
        }

        if (imageToDraw) {
            graphics.DrawImage(imageToDraw, (REAL)screenX, (REAL)screenY, (REAL)item->getWidth(), (REAL)item->getHeight());
        }
    }
}

void GameRender::drawMonsters(Gdiplus::Graphics& graphics, const GameWorld& world) {
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);

    for (const auto& monster : world.getMonsters()) {
        if (!monster->isAlive()) continue;

        int screenX = monster->getX() - world.getCameraX();
        int screenY = monster->getY();

        Gdiplus::Image* imageToDraw = nullptr;

        switch (monster->getType()) {
            case Monster::MonsterType::Goomba:
            {
                if ((monster->getX() / 20) % 2 == 0) {
                    imageToDraw = getGoombaImage();
                } else {
                    imageToDraw = getGoombaImage2();
                }
                break;
            }
            case Monster::MonsterType::Turtle:
            {
                const Turtle* turtle = static_cast<const Turtle*>(monster.get());
                if (turtle->getState() == Turtle::TurtleState::NORMAL) {
                    if ((monster->getX() / 20) % 2 == 0) {
                        imageToDraw = getTurtleImage();
                    } else {
                        imageToDraw = getTurtleImage2();
                    }
                } else {
                    imageToDraw = getTurtleShellImage();
                }
                break;
            }
            case Monster::MonsterType::Bowser:
            {
                if ((monster->getX() / 40) % 2 == 0) {
                    imageToDraw = getBowserImage1();
                } else {
                    imageToDraw = getBowserImage2();
                }
                break;
            }
            // ... other monster types
        }

        if (imageToDraw) {
            if (monster->getVx() > 0) {
                imageToDraw->RotateFlip(Gdiplus::RotateNoneFlipX);
            }
            graphics.DrawImage(imageToDraw, (REAL)screenX, (REAL)screenY, (REAL)monster->getWidth(), (REAL)monster->getHeight());
            if (monster->getVx() > 0) { // Flip back
                imageToDraw->RotateFlip(Gdiplus::RotateNoneFlipX);
            }
        }
    }
}
Gdiplus::Image* GameRender::loadImage(const WCHAR* path) {
    Gdiplus::Image* image = new Gdiplus::Image(path);
    if (image->GetLastStatus() != Gdiplus::Ok) {
        MessageBox(NULL, L"Image loading failed!", L"Error", MB_OK);
        exit(1);
    }
    return image;
}

void GameRender::loadImages() {
    peach = loadImage(L"resource/peach/peach.png");
    stage_1_dirt = loadImage(L"resource/tile/stage_1_dirt.png");
    stage_1_brick = loadImage(L"resource/tile/stage_1_brick.png");
    stage_1_background = loadImage(L"resource/background/stage_1_background.png");
    stage_2_background = loadImage(L"resource/background/stage_2_background.png");
    mushroom_head_1 = loadImage(L"resource/tile/mushroom_head_1.png");
    mushroom_head_2 = loadImage(L"resource/tile/mushroom_head_2.png");
    mushroom_head_3 = loadImage(L"resource/tile/mushroom_head_3.png");
    mushroom_trunk_1 = loadImage(L"resource/tile/mushroom_trunk_1.png");
    mushroom_trunk_2 = loadImage(L"resource/tile/mushroom_trunk_2.png");
    cloud_block = loadImage(L"resource/tile/cloud_block.png");
    stone_tile = loadImage(L"resource/tile/stone_tile.png");
    koopa_block = loadImage(L"resource/tile/koopa_block.png");
    fire_head = loadImage(L"resource/tile/fire_head.png");
    fire_body = loadImage(L"resource/tile/fire_body.png");
    fire_switch_tile = loadImage(L"resource/tile/fire_switch_tile.png");
    firetrap = loadImage(L"resource/tile/fire.png");
    pipe_1 = loadImage(L"resource/pipe/pipe_1.png");
    pipe_2 = loadImage(L"resource/pipe/pipe_2.png");
    pipe_3 = loadImage(L"resource/pipe/pipe_3.png");
    pipe_4 = loadImage(L"resource/pipe/pipe_4.png");
    item_block_1 = loadImage(L"resource/tile/item_block_1.png");
    item_block_2 = loadImage(L"resource/tile/item_block_2.png");
    item_block_3 = loadImage(L"resource/tile/item_block_3.png");
    item_block_used = loadImage(L"resource/tile/item_block_used.png");
    unbreakable_block = loadImage(L"resource/tile/unbreakable_block.png");
    castle_1 = loadImage(L"resource/tile/castle_1.png");
    castle_blank = loadImage(L"resource/tile/castle_blank.png");
    coin_1 = loadImage(L"resource/tile/coin_1.png");
    coin_2 = loadImage(L"resource/tile/coin_2.png");
    coin_3 = loadImage(L"resource/tile/coin_3.png");
    mario_stop = loadImage(L"resource/mario/mario_stop.png");
    mario_walk_motion_1 = loadImage(L"resource/mario/mario_walk_motion_1.png");
    mario_walk_motion_2 = loadImage(L"resource/mario/mario_walk_motion_2.png");
    mario_walk_motion_3 = loadImage(L"resource/mario/mario_walk_motion_3.png");
    mario_jump = loadImage(L"resource/mario/mario_jump.png");
    mario_dead = loadImage(L"resource/mario/mario_dead.png");
    big_mario_stop = loadImage(L"resource/mario/big_mario_stop.png");
    big_mario_walk_motion_1 = loadImage(L"resource/mario/big_mario_walk_motion_1.png");
    big_mario_walk_motion_2 = loadImage(L"resource/mario/big_mario_walk_motion_2.png");
    big_mario_walk_motion_3 = loadImage(L"resource/mario/big_mario_walk_motion_3.png");
    big_mario_jump = loadImage(L"resource/mario/big_mario_jump.png");
    big_mario_change = loadImage(L"resource/mario/big_mario_change.png");
    tino_mario_stop = loadImage(L"resource/mario/tino/Tino_mario_stop.png");
    tino_mario_jump = loadImage(L"resource/mario/tino/Tino_mario_jump.png");
    tino_mario_walk_motion_1 = loadImage(L"resource/mario/tino/Tino_mario_walk_motion_1.png");
    tino_mario_walk_motion_2 = loadImage(L"resource/mario/tino/Tino_mario_walk_motion_2.png");
    tino_mario_walk_motion_3 = loadImage(L"resource/mario/tino/Tino_mario_walk_motion_3.png");
    tino_mario_attack_1 = loadImage(L"resource/mario/tino/Tino_mario_attack_1.png");
    tino_mario_attack_2 = loadImage(L"resource/mario/tino/Tino_mario_attack_2.png");
    tino_mario_attack_3 = loadImage(L"resource/mario/tino/Tino_mario_attack_3.png");
    tino_mario_attack_4 = loadImage(L"resource/mario/tino/Tino_mario_attack_4.png");
    tino_mario_attack_5 = loadImage(L"resource/mario/tino/Tino_mario_attack_5.png");
    tino_mario_attack_6 = loadImage(L"resource/mario/tino/Tino_mario_attack_6.png");
    tino_mario_fire_1 = loadImage(L"resource/mario/tino/Tino_mario_fire_1.png");
    tino_mario_fire_2 = loadImage(L"resource/mario/tino/Tino_mario_fire_2.png");
    tino_mario_fire_R_1 = loadImage(L"resource/mario/tino/Tino_mario_fire_R_1.png");
    tino_mario_fire_R_2 = loadImage(L"resource/mario/tino/Tino_mario_fire_R_2.png");
    tino_mario_fire_fade_1 = loadImage(L"resource/mario/tino/Tino_mario_fire_fade_1.png");
    tino_mario_fire_fade_2 = loadImage(L"resource/mario/tino/Tino_mario_fire_fade_2.png");
    tino_mario_fire_fade_3 = loadImage(L"resource/mario/tino/Tino_mario_fire_fade_3.png");
    flower_mario_stop = loadImage(L"resource/mario/flower/flower_mario_stop.png");
    flower_mario_jump = loadImage(L"resource/mario/flower/flower_mario_jump.png");
    flower_mario_walk_motion_1 = loadImage(L"resource/mario/flower/flower_mario_walk_motion_1.png");
    flower_mario_walk_motion_2 = loadImage(L"resource/mario/flower/flower_mario_walk_motion_2.png");
    flower_mario_walk_motion_3 = loadImage(L"resource/mario/flower/flower_mario_walk_motion_3.png");
    flower_mario_change = loadImage(L"resource/mario/flower/flower_mario_change.png");
    flower_mario_fire = loadImage(L"resource/mario/flower/flower_mario_fire.png");
    star_mario_stop_1 = loadImage(L"resource/mario/star/star_mario_stop_1.png");
    star_mario_stop_2 = loadImage(L"resource/mario/star/star_mario_stop_2.png");
    star_mario_stop_3 = loadImage(L"resource/mario/star/star_mario_stop_3.png");
    star_mario_walk_motion_1_1 = loadImage(L"resource/mario/star/star_mario_walk_motion_1_1.png");
    star_mario_walk_motion_1_2 = loadImage(L"resource/mario/star/star_mario_walk_motion_1_2.png");
    star_mario_walk_motion_1_3 = loadImage(L"resource/mario/star/star_mario_walk_motion_1_3.png");
    star_mario_walk_motion_2_1 = loadImage(L"resource/mario/star/star_mario_walk_motion_2_1.png");
    star_mario_walk_motion_2_2 = loadImage(L"resource/mario/star/star_mario_walk_motion_2_2.png");
    star_mario_walk_motion_2_3 = loadImage(L"resource/mario/star/star_mario_walk_motion_2_3.png");
    star_mario_walk_motion_3_1 = loadImage(L"resource/mario/star/star_mario_walk_motion_3_1.png");
    star_mario_walk_motion_3_2 = loadImage(L"resource/mario/star/star_mario_walk_motion_3_2.png");
    star_mario_walk_motion_3_3 = loadImage(L"resource/mario/star/star_mario_walk_motion_3_3.png");
    star_mario_jump_1 = loadImage(L"resource/mario/star/star_mario_jump_1.png");
    star_mario_jump_2 = loadImage(L"resource/mario/star/star_mario_jump_2.png");
    star_mario_jump_3 = loadImage(L"resource/mario/star/star_mario_jump_3.png");
    star_big_mario_stop_1 = loadImage(L"resource/mario/star/star_big_mario_stop_1.png");
    star_big_mario_stop_2 = loadImage(L"resource/mario/star/star_big_mario_stop_2.png");
    star_big_mario_stop_3 = loadImage(L"resource/mario/star/star_big_mario_stop_3.png");
    star_big_mario_walk_motion_1_1 = loadImage(L"resource/mario/star/star_big_mario_walk_motion_1_1.png");
    star_big_mario_walk_motion_1_2 = loadImage(L"resource/mario/star/star_big_mario_walk_motion_1_2.png");
    star_big_mario_walk_motion_1_3 = loadImage(L"resource/mario/star/star_big_mario_walk_motion_1_3.png");
    star_big_mario_walk_motion_2_1 = loadImage(L"resource/mario/star/star_big_mario_walk_motion_2_1.png");
    star_big_mario_walk_motion_2_2 = loadImage(L"resource/mario/star/star_big_mario_walk_motion_2_2.png");
    star_big_mario_walk_motion_2_3 = loadImage(L"resource/mario/star/star_big_mario_walk_motion_2_3.png");
    star_big_mario_walk_motion_3_1 = loadImage(L"resource/mario/star/star_big_mario_walk_motion_3_1.png");
    star_big_mario_walk_motion_3_2 = loadImage(L"resource/mario/star/star_big_mario_walk_motion_3_2.png");
    star_big_mario_walk_motion_3_3 = loadImage(L"resource/mario/star/star_big_mario_walk_motion_3_3.png");
    star_big_mario_jump_1 = loadImage(L"resource/mario/star/star_big_mario_jump_1.png");
    star_big_mario_jump_2 = loadImage(L"resource/mario/star/star_big_mario_jump_2.png");
    star_big_mario_jump_3 = loadImage(L"resource/mario/star/star_big_mario_jump_3.png");
    item_mushroom = loadImage(L"resource/items/mushroom.png");
    item_up_mushroom = loadImage(L"resource/items/up_mushroom.png");
    item_star_1 = loadImage(L"resource/items/star_1.png");
    item_star_2 = loadImage(L"resource/items/star_2.png");
    item_star_3 = loadImage(L"resource/items/star_3.png");
    item_star_4 = loadImage(L"resource/items/star_4.png");
    item_flower_1 = loadImage(L"resource/items/flower_1.png");
    item_flower_2 = loadImage(L"resource/items/flower_2.png");
    item_flower_3 = loadImage(L"resource/items/flower_3.png");
    item_flower_4 = loadImage(L"resource/items/flower_4.png");
    item_tino = loadImage(L"resource/items/tino.png");
    shot_fireball_1 = loadImage(L"resource/items/shot/fireball_1.png");
    shot_fireball_2 = loadImage(L"resource/items/shot/fireball_2.png");
    shot_fireball_3 = loadImage(L"resource/items/shot/fireball_3.png");
    shot_fireball_4 = loadImage(L"resource/items/shot/fireball_4.png");
    shot_fireball_fadeout_1 = loadImage(L"resource/items/shot/fireball_fadeout_1.png");
    shot_fireball_fadeout_2 = loadImage(L"resource/items/shot/fireball_fadeout_2.png");
    shot_fireball_fadeout_3 = loadImage(L"resource/items/shot/fireball_fadeout_3.png");
    monster1_motion1 = loadImage(L"resource/monster/monster1_motion1.png");
    monster1_motion2 = loadImage(L"resource/monster/monster1_motion2.png");
    monster1_dead = loadImage(L"resource/monster/monster1_dead.png");
    monster2_motion1 = loadImage(L"resource/monster/monster2_motion1.png");
    monster2_motion2 = loadImage(L"resource/monster/monster2_motion2.png");
    monster2_dead = loadImage(L"resource/monster/monster2_dead.png");
    monster3_motion1 = loadImage(L"resource/monster/monster3_motion1.png");
    monster3_motion2 = loadImage(L"resource/monster/monster3_motion2.png");
    monster3_dead = loadImage(L"resource/monster/monster3_dead.png");
    turtle_1 = loadImage(L"resource/monster/turtle_1.png");
    turtle_2 = loadImage(L"resource/monster/turtle_2.png");
    turtle_R_1 = loadImage(L"resource/monster/turtle_R_1.png");
    turtle_R_2 = loadImage(L"resource/monster/turtle_R_2.png");
    turtle_hide = loadImage(L"resource/monster/turtle_hide.png");
    brown_turtle_1 = loadImage(L"resource/monster/brown_turtle_1.png");
    brown_turtle_2 = loadImage(L"resource/monster/brown_turtle_2.png");
    brown_turtle_R_1 = loadImage(L"resource/monster/brown_turtle_R_1.png");
    brown_turtle_R_2 = loadImage(L"resource/monster/brown_turtle_R_2.png");
    brown_turtle_hide = loadImage(L"resource/monster/brown_turtle_hide.png");
    angel_turtle_1 = loadImage(L"resource/monster/angel_turtle_1.png");
    angel_turtle_2 = loadImage(L"resource/monster/angel_turtle_2.png");
    bowser_walk_1 = loadImage(L"resource/monster/bowser_walk_1.png");
    bowser_walk_2 = loadImage(L"resource/monster/bowser_walk_2.png");
    bowser_fire_walk_1 = loadImage(L"resource/monster/bowser_fire_walk_1.png");
    bowser_fire_walk_2 = loadImage(L"resource/monster/bowser_fire_walk_2.png");
    bowser_fireball_1 = loadImage(L"resource/monster/bowser_fireball_1.png");
    bowser_fireball_2 = loadImage(L"resource/monster/bowser_fireball_2.png");
    screen_coin_1 = loadImage(L"resource/screen/screen_coin_1.png");
    screen_coin_2 = loadImage(L"resource/screen/screen_coin_2.png");
    screen_coin_3 = loadImage(L"resource/screen/screen_coin_3.png");
    screen_coin_x = loadImage(L"resource/screen/screen_coin_x.png");
    title_screen = loadImage(L"resource/title/title_screen.png");
    title_cursor = loadImage(L"resource/title/title_cursor.png");
    title_dead = loadImage(L"resource/title/title_dead.png");
    flag_stick = loadImage(L"resource/tile/flag_stick.png");
    flag_marble = loadImage(L"resource/tile/flag_marble.png");
    flag = loadImage(L"resource/tile/flag.png");
}

void GameRender::drawBackground(Gdiplus::Graphics& graphics, const GameWorld& world) {
    Gdiplus::SolidBrush skyBrush(Gdiplus::Color(255, 148, 148, 255));
    Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));

    if (world.getStage() == 1 || world.getStage() == 2) 
    {
        graphics.FillRectangle(&skyBrush, 0, 0, 800, 640);
        graphics.DrawImage(stage_1_background, (REAL)-world.getCameraX(), -36.0f, 4600.0f, 640.0f);
    } 
    else if (world.getStage() == 2) 
    {
        graphics.FillRectangle(&skyBrush, 0, 0, 800, 640);
        graphics.DrawImage(stage_2_background, (REAL)-world.getCameraX(), -36.0f, 4600.0f, 640.0f);
    }
    else if (world.getStage() == 3)
    {
        graphics.FillRectangle(&blackBrush, 0, 0, 800, 640);
    }
}

void GameRender::drawMap(Gdiplus::Graphics& graphics, const GameWorld& world) {
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);

    const int(*currentMap)[MAP_WIDTH] = world.getCurrentMap();
    double cameraX = world.getCameraX();
    int frame_motion = (world.getGlobalAnimationFrameCounter() / 8) % 7;

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int screenX = j * TILE_SIZE - cameraX;
            int screenY = i * TILE_SIZE;
            if (screenX + TILE_SIZE < 0 || screenX > SCREEN_WIDTH) continue;

            switch (currentMap[i][j]) {
                case 1: // dirt
                    graphics.DrawImage(stage_1_dirt, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 10: // brick
                    graphics.DrawImage(stage_1_brick, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 2: // coin
                    if (frame_motion == 0 || (frame_motion >= 4 && frame_motion < 7))
                    {
                        graphics.DrawImage(coin_1, (REAL)screenX, (REAL)screenY, 41.0f, 40.0f);
                    }
                    else if (frame_motion == 1 || frame_motion == 3)
                    {
                        graphics.DrawImage(coin_2, (REAL)screenX, (REAL)screenY, 41.0f, 40.0f);
                    }
                    else if (frame_motion == 2)
                    {
                        graphics.DrawImage(coin_3, (REAL)screenX, (REAL)screenY, 41.0f, 40.0f);
                    }
                    break;
                case 5: // unbreakable block
                    graphics.DrawImage(unbreakable_block, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 6: case 60: case 61: case 62: case 63: case 64: // item blocks
                    if (frame_motion == 0 || (frame_motion >= 4 && frame_motion < 7))
                    {
                        graphics.DrawImage(item_block_1, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    }
                    else if (frame_motion == 1 || frame_motion == 3)
                    {
                        graphics.DrawImage(item_block_2, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    }
                    else if (frame_motion == 2)
                    {
                        graphics.DrawImage(item_block_3, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    }
                    break;
                case 7: // flag stick
                    graphics.DrawImage(flag_stick, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 8: // flag marble
                    graphics.DrawImage(flag_marble, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 9: // castle
                    graphics.DrawImage(castle_1, (REAL)screenX - 40 * 3, (REAL)screenY - 40 * 4, (41.0f) * 5, (41.0f) * 5);
                    break;
                case 90: // mushroom_head_1
                    graphics.DrawImage(mushroom_head_1, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 91: // mushroom_head_2
                    graphics.DrawImage(mushroom_head_2, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 92: // mushroom_head_3
                    graphics.DrawImage(mushroom_head_3, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 11: // mushroom_trunk_1
                    graphics.DrawImage(mushroom_trunk_1, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 12: // mushroom_trunk_2
                    graphics.DrawImage(mushroom_trunk_2, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 13: // cloud_block
                    graphics.DrawImage(cloud_block, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 14: // fire_switch_tile
                    graphics.DrawImage(fire_switch_tile, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 15: // stone_tile
                    graphics.DrawImage(stone_tile, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 16: // item_block_used
                    graphics.DrawImage(item_block_used, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 17: // fire_head
                    graphics.DrawImage(fire_head, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 18: // fire_body
                    graphics.DrawImage(fire_body, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 19: // koopa_block
                    graphics.DrawImage(koopa_block, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 40: // pipe_1
                    graphics.DrawImage(pipe_1, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 41: // pipe_2
                    graphics.DrawImage(pipe_2, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 42: // pipe_3
                    graphics.DrawImage(pipe_3, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 43: // pipe_4
                    graphics.DrawImage(pipe_4, (REAL)screenX, (REAL)screenY, 41.0f, 41.0f);
                    break;
                case 999: // peach
                    graphics.DrawImage(peach, (REAL)screenX, (REAL)screenY, 41.0f, 82.0f);
                    break;
            }
        }
    }
}

void GameRender::drawPlayer(Gdiplus::Graphics& graphics, const GameWorld& world, const Player& player) 
{
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);

    int drawX = player.getX();
    int drawY = player.getY();
    Gdiplus::Image* imageToDraw = mario_stop;

    if (player.isDead()) {
        graphics.DrawImage(mario_dead, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), (REAL)player.getHeight());
        return;
    }
    
    // Transformation animations

    if (world.getGameState_trans() == GameState_Trans::GAME_BIG_TRANS)
    {
        if (player.isBig()) // Big Mario shrinking to Small Mario
        { 
            if ((world.getGlobalAnimationFrameCounter() / 5) % 2 == 0) 
            {
                graphics.DrawImage(big_mario_change, (REAL)drawX, (REAL)drawY - TILE_SIZE, (REAL)player.getWidth(), (REAL)player.getHeight());
            } 
            else 
            {
                graphics.DrawImage(mario_stop, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), (REAL)player.getHeight());
            }
        } 
        else // Small Mario growing to Big Mario 
        { 
            if ((world.getGlobalAnimationFrameCounter() / 5) % 2 == 0) 
            {
                graphics.DrawImage(big_mario_change, (REAL)drawX, (REAL)drawY - TILE_SIZE, (REAL)player.getWidth(), 40.0f * 2);
            } 
            else 
            {
                graphics.DrawImage(mario_stop, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), (REAL)player.getHeight());
            }
        }
    }
    else if (world.getGameState_trans() == GameState_Trans::GAME_FLOWER_TRANS) { // Any state to Flower Mario
        if ((world.getGlobalAnimationFrameCounter() / 5) % 2 == 0) {
            if (player.getState() == PlayerState::Small) { // Small to Flower
                graphics.DrawImage(flower_mario_change, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), 40.0f * 2);
            } else { // Big/Tino to Flower
                graphics.DrawImage(flower_mario_stop, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), (REAL)player.getHeight());
            }
        } else {
            if (player.getState() == PlayerState::Small) { // Small to Flower
                graphics.DrawImage(mario_stop, (REAL)drawX, (REAL)drawY + 40, (REAL)player.getWidth(), (REAL)player.getHeight());
            } else if (player.getState() == PlayerState::Big) { // Big to Flower
                graphics.DrawImage(big_mario_stop, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), (REAL)player.getHeight());
            } else if (player.getState() == PlayerState::Tino) { // Tino to Flower
                graphics.DrawImage(tino_mario_stop, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), (REAL)player.getHeight());
            }
        }
    }
    else if (world.getGameState_trans() == GameState_Trans::GAME_TINO_TRANS) { // Any state to Tino Mario
        if ((world.getGlobalAnimationFrameCounter() / 5) % 2 == 0) {
            if (player.getState() == PlayerState::Small) { // Small to Tino
                graphics.DrawImage(tino_mario_stop, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), 40.0f * 2);
            } else { // Big/Flower to Tino
                graphics.DrawImage(tino_mario_stop, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), (REAL)player.getHeight());
            }
        } else {
            if (player.getState() == PlayerState::Small) { // Small to Tino
                graphics.DrawImage(mario_stop, (REAL)drawX, (REAL)drawY + 40, (REAL)player.getWidth(), (REAL)player.getHeight());
            } else if (player.getState() == PlayerState::Big) { // Big to Tino
                graphics.DrawImage(big_mario_stop, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), (REAL)player.getHeight());
            } else if (player.getState() == PlayerState::Flower) { // Flower to Tino
                graphics.DrawImage(flower_mario_stop, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), (REAL)player.getHeight());
            }
        }
    }
    else
    {
        switch (player.getState())
        {
        case PlayerState::Small:
            if (player.isJumping()) imageToDraw = mario_jump;
            else if (player.isWalking())
            {
                if (player.getWalkMotion() == 0) imageToDraw = mario_walk_motion_1;
                else if (player.getWalkMotion() == 1) imageToDraw = mario_walk_motion_2;
                else imageToDraw = mario_walk_motion_3;
            }
            else
            {
                imageToDraw = mario_stop;
            }
            break;
        case PlayerState::Big:
            if (player.isJumping()) imageToDraw = big_mario_jump;
            else if (player.isWalking())
            {
                if (player.getWalkMotion() == 0) imageToDraw = big_mario_walk_motion_1;
                else if (player.getWalkMotion() == 1) imageToDraw = big_mario_walk_motion_2;
                else imageToDraw = big_mario_walk_motion_3;
            }
            else
            {
                imageToDraw = big_mario_stop;
            }

            break;
        case PlayerState::Flower:
            if (player.isJumping()) imageToDraw = flower_mario_jump;
            else if (player.isWalking())
            {
                if (player.getWalkMotion() == 0) imageToDraw = flower_mario_walk_motion_1;
                else if (player.getWalkMotion() == 1) imageToDraw = flower_mario_walk_motion_2;
                else imageToDraw = flower_mario_walk_motion_3;
            }
            else
            {
                imageToDraw = flower_mario_stop;
            }

            break;
        case PlayerState::Tino:
            if (player.isJumping()) imageToDraw = tino_mario_jump;
            else if (player.isWalking())
            {
                if (player.getWalkMotion() == 0) imageToDraw = tino_mario_walk_motion_1;
                else if (player.getWalkMotion() == 1) imageToDraw = tino_mario_walk_motion_2;
                else imageToDraw = tino_mario_walk_motion_3;
            }
            else
            {
                imageToDraw = tino_mario_stop;
            }
            break;
        case PlayerState::Star:
            // Star state will be handled by a color matrix or other effect
            // For now, just use big mario images
            if (player.isJumping()) imageToDraw = star_big_mario_jump_1;
            else if (player.isWalking())
            {
                if (player.getWalkMotion() == 0) imageToDraw = star_big_mario_walk_motion_1_1;
                else if (player.getWalkMotion() == 1) imageToDraw = star_big_mario_walk_motion_2_1;
                else imageToDraw = star_big_mario_walk_motion_3_1;
            }
            else
            {
                imageToDraw = star_big_mario_stop_1;
            }
            break;
        }
        if (player.getDirection() == 0) { // Facing left
            imageToDraw->RotateFlip(Gdiplus::RotateNoneFlipX);
        }

        graphics.DrawImage(imageToDraw, (REAL)drawX, (REAL)drawY, (REAL)player.getWidth(), (REAL)player.getHeight());

        if (player.getDirection() == 0) { // Flip back
            imageToDraw->RotateFlip(Gdiplus::RotateNoneFlipX);
        }
    }
}

void GameRender::drawUI(Gdiplus::Graphics& graphics, const GameWorld& world) {
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);

    SolidBrush brush(Color(255, 255, 255, 255));
    StringFormat stringFormat;
    stringFormat.SetAlignment(StringAlignmentNear);

    TCHAR life_print[32], life_print2[32], time_print[32], time_print2[32], coin_print[32], stage_print[32], stage_print2[32];
    TCHAR clear_text_1[32];
    TCHAR clear_text_2[32];

    int frame_motion = (world.getGlobalAnimationFrameCounter() / 8) % 7;

    //코인
    if (frame_motion < 4 || frame_motion > 5)
    {
        graphics.DrawImage(screen_coin_1, 260, 40, 25, 25);
    }
    else if (frame_motion == 4 || frame_motion == 6)
    {
        graphics.DrawImage(screen_coin_2, 260, 40, 25, 25);
    }
    else if (frame_motion == 5)
    {
        graphics.DrawImage(screen_coin_3, 260, 40, 25, 25);
    }

    _stprintf_s(life_print, _T("MARIO"));
    _stprintf_s(life_print2, _T("%05d"), world.getPlayer().getLife());
    _stprintf_s(time_print, _T("TIME"));
    _stprintf_s(time_print2, _T("%03d"), world.getStageTime());
    _stprintf_s(coin_print, _T("%02d"), world.getPlayer().getCoin());
    _stprintf_s(stage_print, _T("WORLD"));
    _stprintf_s(stage_print2, _T("%d"), world.getStage());
    _stprintf_s(clear_text_1, _T("THANK YOU MARIO!"));
    _stprintf_s(clear_text_2, _T("YOUR QUEST IS OVER.!"));

    graphics.DrawString(life_print, -1, m_font.get(), PointF(80, 20), &stringFormat, &brush);
    graphics.DrawString(life_print2, -1, m_font.get(), PointF(80, 40), &stringFormat, &brush);
    graphics.DrawString(time_print, -1, m_font.get(), PointF(620, 20), &stringFormat, &brush);
    graphics.DrawString(time_print2, -1, m_font.get(), PointF(640, 40), &stringFormat, &brush);

    graphics.DrawImage(screen_coin_x, 290, 40, 25, 25);

    graphics.DrawString(coin_print, -1, m_font.get(), PointF(320, 40), &stringFormat, &brush);
    graphics.DrawString(stage_print, -1, m_font.get(), PointF(440, 20), &stringFormat, &brush);
    graphics.DrawString(stage_print2, -1, m_font.get(), PointF(480, 40), &stringFormat, &brush);

    if (world.getGameClearText())
    {
        graphics.DrawString(clear_text_1, -1, m_font.get(), PointF(240, 160), &stringFormat, &brush);
        graphics.DrawString(clear_text_2, -1, m_font.get(), PointF(200, 240), &stringFormat, &brush);
    }
}

void GameRender::drawTitleScreen(Gdiplus::Graphics& graphics, const GameWorld& world) {
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);

    // Draw title screen background
    graphics.DrawImage(title_screen, 0, 0, 800, 640);

    // Draw title cursor (assuming it's for selecting options)
    // Position it based on some logic, for now, a fixed position
    // 커서
    if (world.title_select == 0)
    {
        graphics.DrawImage(title_cursor, 220, 380, 25, 25);
    }
    else if (world.title_select == 1)
    {
        graphics.DrawImage(title_cursor, 220, 420, 25, 25);
    }
    drawUI(graphics, world);
}

Gdiplus::Image* GameRender::getPlayerFireballImage() { return shot_fireball_1; }



