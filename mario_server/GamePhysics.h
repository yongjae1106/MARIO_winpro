#pragma once

#include "Player.h"
#include "monsters/Monster.h"
#include "items/Item.h"
#include "Particles/Particle.h"
#include "PacketInfo.h" // BGM_Type, GameEvent enum 사용을 위해 추가
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

enum class GameState
{
    GAME_START,
    GAME_OVER,
    GAME_RUNNING,
    GAME_VICTORY,
    GAME_CLEAR
};

struct MonsterSpawnInfo 
{
    Monster::MonsterType type;
    int x;
    int y;
};

class GameWorld 
{
public:
    // 1. 유일한 인스턴스를 얻기 위한 public static 함수
    static GameWorld& getInstance() {
        static GameWorld instance;
        return instance;
    }

    // 2. 복사 및 대입을 방지합니다. 싱글턴 객체는 복사되면 안 됩니다.
    GameWorld(const GameWorld&) = delete;
    void operator=(const GameWorld&) = delete;

    bool isSolidTile(int tileValue) const;

    void init();
    void sound_init(HWND hwnd);

    void update();
    void updateAnimations();
    void render(HDC hdc);

    void handleKeyDown(WPARAM wParam);
    void handleKeyUp(WPARAM wParam);

    void loadStage(int stage);
    void resetForDeath();

    void transUpdate();
    void cameraUpdate();

    const Player& getPlayer() const;
    Player& getPlayer();
    const int (*getCurrentMap() const)[MAP_WIDTH];
    int getStage() const;
    double getCameraX() const;

    const std::vector<std::unique_ptr<Monster>>& getMonsters() const;
    const std::vector<std::unique_ptr<Item>>& getItems() const;
    const std::vector<std::unique_ptr<Particle>>& getParticles() const;
    const std::vector<std::unique_ptr<Particle>>& getNewParticles() const;

    void newParticles_insertTo_Particles();

    int getLife() const { return m_life; }
    int getCoin() const { return m_coin; }
    void setLife(int life) { m_life = life; }
    void addLife(int amount) { m_life += amount; }
    void setCoin(int coin) { m_coin = coin; }
    void addCoin(int amount) { m_coin += amount; }

    int getStageTime() const { return stage_time; }
    int getTinoCooldownSpace() const { return player.getTinoCooldownSpace(); }
    bool getGameClearText() const { return gameClearText; }
    bool getGameoverTitleDead() const { return gameover_TitleDead; }
    int getGlobalAnimationFrameCounter() const { return m_global_animation_frame_counter; }

    const bool* getKeyState() const;
    GameState getGameState() const;
    GameState_Trans getGameState_trans() const;
    void setGameState(GameState state);
    void setGameState_trans(GameState_Trans state_trans);
    void setStage_time(int time);
    void setGameOverTitleDead(bool gameover);
    void setdeadStartTime(int time);

    // BGM & Event System
    BGM_Type getCurrentBGM() const;
    const std::vector<GameEvent>& getEventQueue() const;
    void pushEvent(GameEvent event);
    void clearEventQueue();

    void spawnItem(Item::ItemType type, int x, int y);
    void spawnMonster(std::unique_ptr<Monster> monster);
    void spawnParticle(std::unique_ptr<Particle> particle);
    void spawnPlayerFireball(int x, int y, int vx);
    void spawnTinoFireball(int x, int y, int vx, int direction);
    void spawnTinoFireballEffect(int x, int y, int vx, int direction);

private:
    // 3. 생성자를 private으로 변경하여 외부에서 new GameWorld() 호출을 막습니다.
    GameWorld();
    ~GameWorld();

    void updatePlayer();
    void updateMonsters();
    void updateItems();
    void updateParticles();
    void checkCollisions();
    void checkMonsterMapCollision();
    void checkPlayerCoinCollision();
    void checkPlayerMapCollision();
    void checkPlayerMonsterCollision();
    void checkPlayerItemCollision();
    void checkParticleMonsterCollision();
    void checkMonsterMonsterCollision();
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
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Particle>> newParticles;

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
    int m_life;
    int m_coin;

    BGM_Type m_currentBGM;
    std::vector<GameEvent> m_eventQueue;

    bool keyState[256];
    int m_global_animation_frame_counter;
};

