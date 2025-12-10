#pragma once

// [중요] NetworkManager를 가장 먼저 포함해야 Winsock 충돌을 방지할 수 있습니다.
#include "NetworkManager/NetworkManager.h"

#include "Player.h"
#include "monsters/Monster.h"
#include "items/Item.h"
#include "Particles/Particle.h"
#include "Sound.h"
#include "GameRender.h"
#include "PacketInfo.h" // GameEvent, BGM_Type 등

#include <vector>
#include <memory>
#include <map>
#include <windows.h>
#include <algorithm>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640
#define MAP_WIDTH 200
#define MAP_HEIGHT 15
#define TILE_SIZE 40

// 전역 충돌 함수 선언
bool isColliding(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

enum class GameState_Trans
{
    GAME_NONE,
    GAME_BIG_TRANS,
    GAME_FLOWER_TRANS,
    GAME_TINO_TRANS
};

enum class GameState
{
    GAME_TITLE,
    GAME_START,
    GAME_RUNNING,
    GAME_VICTORY,
    GAME_CLEAR,
    GAME_OVER
};

// 맵 초기화 로직을 위해 필요한 구조체
struct MonsterSpawnInfo {
    Monster::MonsterType type;
    int x;
    int y;
};

class GameWorld {
public:
    static GameWorld& getInstance();

    // 복사 방지
    GameWorld(const GameWorld&) = delete;
    void operator=(const GameWorld&) = delete;

    bool isSolidTile(int tileValue) const;

    void init();
    void sound_init(HWND hwnd);

    void updateAnimations();
    void update();
    void render(HDC hdc);

    void handleKeyDown(WPARAM wParam);
    void handleKeyUp(WPARAM wParam);

    void loadStage(int stage);

    // 렌더러 접근자
    GameRender& getGameRender() { return m_gameRender; }

    void cameraUpdate();

    // --- Player Management ---
    Player* getLocalPlayer();
    const Player* getLocalPlayer() const;
    Player* getPlayerById(int id);
    const std::map<int, Player>& getPlayers() const;
    void setLocalPlayerId(int id);

    // [GameRender 호환성용] 로컬 플레이어 반환
    const Player& getPlayer() const {
        const Player* p = getLocalPlayer();
        if (!p) {
            static Player dummy;
            return dummy;
        }
        return *p;
    }
    // --- End Player Management ---

    // Getters
    const int(*getCurrentMap() const)[MAP_WIDTH];
    int getStage() const;
    double getCameraX() const;

    const std::map<int, std::unique_ptr<Monster>>& getMonsters() const;
    const std::vector<std::unique_ptr<Item>>& getItems() const;
    const std::vector<std::unique_ptr<Particle>>& getParticles() const; // const 추가
    const std::vector<std::unique_ptr<Particle>>& getNewParticles() const;

    void newParticles_insertTo_Particles();

    // UI 표시용 Getter (로컬 플레이어 기준)
    int getLife() const;
    int getCoin() const;
    int getTinoCooldownSpace() const;

    int getStageTime() const { return stage_time; }
    bool getGameClearText() const { return gameClearText; }
    bool getGameoverTitleDead() const { return gameover_TitleDead; }
    GameState_Trans getGameState_trans() const;

    int getGlobalAnimationFrameCounter() const { return m_global_animation_frame_counter; }

    const bool* getKeyState() const;
    GameState getGameState() const;

    // Setters
    void setGameState(GameState state);
    void setGameState_trans(GameState_Trans state_trans);
    void setStage_time(int time);
    void setGameOverTitleDead(bool gameover);
    void setdeadStartTime(int time);

    void setStageBGM();
    void playSound(const std::string& name, bool loop = false);
    void stopAllSounds();

    // 이벤트 큐 (서버 로직 호환용)
    void pushEvent(GameEvent event);
    void clearEventQueue();
    const std::vector<GameEvent>& getEventQueue() const;

    // 스폰 함수 (서버 로직 호환용)
    void spawnItem(Item::ItemType type, int x, int y);
    void spawnMonster(std::unique_ptr<Monster> monster);
    void spawnParticle(std::unique_ptr<Particle> particle);
    void spawnPlayerFireball(int x, int y, int vx);
    void spawnTinoFireball(int x, int y, int vx, int direction);
    void spawnTinoFireballEffect(int x, int y, int vx, int direction);

    int title_select;

private:
    GameWorld();
    ~GameWorld();

    // 시스템 객체들
    GameRender m_gameRender;
    NetworkManager m_networkManager;
    Sound m_sound;

    // 플레이어 관리
    std::map<int, Player> m_players;
    int m_localPlayerId;

    // 게임 오브젝트
    std::map<int, std::unique_ptr<Monster>> m_monsters; // ID 기반 관리 (서버 동기화용)
    // 참고: 서버 코드를 그대로 붙여넣으셨다면 vector<unique_ptr<Monster>> monsters; 로 선언되어 있을 수 있습니다.
    // 하지만 클라이언트는 ID로 몬스터를 찾아 업데이트해야 하므로 map이 더 유리합니다.
    // 기존에 붙여넣으신 cpp 코드와 호환성을 위해 아래와 같이 임시로 vector도 선언해 둡니다.
    // (cpp에서 m_monsters를 쓰는지 monsters를 쓰는지에 따라 수정 필요)
    std::vector<std::unique_ptr<Monster>> monsters;

    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Particle>> newParticles;

    // 스폰 정보 (서버 로직 복사본)
    std::vector<MonsterSpawnInfo> stage1Monsters;
    std::vector<MonsterSpawnInfo> stage2Monsters;
    std::vector<MonsterSpawnInfo> stage3Monsters;

    GameState gameState;

    DWORD deadStartTime;
    DWORD victoryStart;
    DWORD clearStart;
    DWORD godstart;

    bool gameover_TitleDead;
    bool gameClearText;
    int stage_time;

    double cameraX;
    int map1[MAP_HEIGHT][MAP_WIDTH];
    int map2[MAP_HEIGHT][MAP_WIDTH];
    int map3[MAP_HEIGHT][MAP_WIDTH];

    int (*currentMap)[MAP_WIDTH];
    int stage;

    bool keyState[256];
    int m_global_animation_frame_counter;

    BGM_Type m_currentBGM;
    std::vector<GameEvent> m_eventQueue;

    // 내부 로직 함수들 (서버 코드 호환용)
    void updatePlayers();
    void updateMonsters();
    void updateItems();
    void updateParticles();

    void checkCollisions();
    void checkPlayerMapCollision(Player& player);
    void checkPlayerMonsterCollision(Player& player);
    void checkPlayerItemCollision(Player& player);
    void checkPlayerCoinCollision(Player& player);
    void checkFlagCollision(Player& player);
    void checkClearCollision(Player& player);
    void checkParticleMonsterCollision();
    void checkMonsterMonsterCollision();
    void checkItemMapCollision();

    void applyplayertakedamage(Player& player);

    // 맵 및 스폰 초기화 함수
    void initMaps();
    void initMonsterSpawns();
    void initMap1();
    void initMap2();
    void initMap3();
    void spawnMonsters();

    void monster_reset();
    void item_reset();
    void resetForDeath(int playerID);
};