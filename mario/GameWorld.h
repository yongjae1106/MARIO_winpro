#pragma once

#include "Player.h"
#include "Monster.h"
#include "Item.h"
#include "GameRender.h"
#include "Sound.h"
#include <vector>
#include <memory>

#include <windows.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640
#define MAP_WIDTH 200
#define MAP_HEIGHT 15
#define TILE_SIZE 40

// Declaration for the global collision function to make it accessible across files
bool isColliding(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
bool isSolidTile(int tileID); // Existing declaration

enum class GameState
{
    GAME_TITLE,
    GAME_START,
    GAME_RUNNING,
    GAME_VICTORY,
    GAME_CLEAR,
    GAME_OVER
};
enum class GameState_Trans
{
    GAME_NONE,
    GAME_BIG_TRANS,
    GAME_FLOWER_TRANS,
    GAME_TINO_TRANS,
};

struct MonsterSpawnInfo {
    Monster::MonsterType type;
    int x;
    int y;
};

class GameWorld {
public:
    GameWorld();

    void init();
    void sound_init(HWND hwnd);

    void update();
    void updateAnimations();
    void render(HDC hdc);

    void handleKeyDown(WPARAM wParam);
    void handleKeyUp(WPARAM wParam);

    void loadStage(int stage);
    void resetForDeath();

    // Getter for the renderer to allow safe access from other classes
    GameRender& getGameRender() { return m_gameRender; }

    void transUpdate();
    void cameraUpdate();

    const Player& getPlayer() const;
    Player& getPlayer();
    const int (*getCurrentMap() const)[MAP_WIDTH];
    int getStage() const;
    double getCameraX() const;
    const std::vector<std::unique_ptr<Monster>>& getMonsters() const;
    const std::vector<std::unique_ptr<Item>>& getItems() const;
    int getLife() const { return player.getLife(); }
    int getCoin() const { return player.getCoin(); }
    int getStageTime() const { return stage_time; }
    int getTinoCooldownZ() const { return player.getTinoCooldownZ(); }
    int getTinoCooldownSpace() const { return player.getTinoCooldownSpace(); }
    bool getGameClearText() const { return gameClearText; }
    bool getGameoverTitleDead () const { return gameover_TitleDead; }
    int getGlobalAnimationFrameCounter() const { return m_global_animation_frame_counter; }

    const bool* getKeyState() const;
    GameState getGameState() const;
    GameState_Trans getGameState_trans() const;
    void setGameState(GameState state);
    void setGameState_trans(GameState_Trans state_trans);
    void setStage_time(int time);
    void setStageBGM();
    void setGameOverTitleDead(bool gameover);
    void setdeadStartTime(int time);

    void spawnItem(Item::ItemType type, int x, int y);
    void spawnMonster(std::unique_ptr<Monster> monster);
    void spawnFireball(int x, int y, int vx);
    void spawnPlayerFireball(int x, int y, int vx);

    void playSound(const std::string& name, bool loop = false);
    void stopAllSounds();

    int title_select;

private:
    GameRender m_gameRender;
    Sound m_sound;

private:
    void updatePlayer();
    void updateMonsters();
    void updateItems();
    void checkCollisions();
    void checkMonsterMapCollision();
    void checkPlayerCoinCollision();
    void checkPlayerMapCollision();
    void checkPlayerMonsterCollision();
    void checkPlayerItemCollision();
    void checkItemMapCollision();
    void checkFlagCollision();
    void checkClearCollision();
    void spawnMonsters();
    
    void applyplayertakedamage();

    void initMaps();
    void initMonsterSpawns();
    void initMap1();
    void initMap2();
    void initMap3();

    void dead();
    void resurrection();
    void monster_reset();
    void item_reset();


    Player player;
    std::vector<std::unique_ptr<Monster>> monsters;
    std::vector<std::unique_ptr<Item>> items;

    std::vector<MonsterSpawnInfo> stage1Monsters;
    std::vector<MonsterSpawnInfo> stage2Monsters;
    std::vector<MonsterSpawnInfo> stage3Monsters;

    GameState gameState;
    GameState_Trans gameState_trans;
    DWORD transformStartTime;
    DWORD deadStartTime;
    DWORD victoryStart;
    DWORD clearStart;
    DWORD godstart; // Missing member variable added

    bool gameover_TitleDead;
    bool gameClearText;

    double cameraX;
    int map1[MAP_HEIGHT][MAP_WIDTH];
    int map2[MAP_HEIGHT][MAP_WIDTH];
    int map3[MAP_HEIGHT][MAP_WIDTH];

    int (*currentMap)[MAP_WIDTH];
    int stage;
    int stage_time;

    bool keyState[256];
    int m_global_animation_frame_counter;
};

