#pragma once

#include "Player.h"
#include "monsters/Monster.h"
#include "items/Item.h"
#include "Particles/Particle.h"
#include "PacketInfo.h" // GameEvent, BGM_Type 등

#include <vector>
#include <memory>
#include <map>
#include <windows.h>
#include <algorithm> // for std::remove_if

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640
#define MAP_WIDTH 200
#define MAP_HEIGHT 15
#define TILE_SIZE 40

// 전역 충돌 함수 선언
bool isColliding(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

enum class GameState
{
    GAME_TITLE,
    GAME_START,
    GAME_RUNNING,
    GAME_VICTORY,
    GAME_CLEAR,
    GAME_OVER
};

struct MonsterSpawnInfo {
    Monster::MonsterType type;
    int x;
    int y;
};

class GameWorld {
public:
    // 싱글톤 인스턴스 반환
    static GameWorld& getInstance() {
        static GameWorld instance;
        return instance;
    }

    // 복사 방지
    GameWorld(const GameWorld&) = delete;
    void operator=(const GameWorld&) = delete;

    bool isSolidTile(int tileValue) const;

    void init();

    // 메인 업데이트 루프
    void update();

    // 입력 처리 (PlayerID 포함)
    void handleKeyDown(int playerID, WPARAM wParam);
    void handleKeyUp(int playerID, WPARAM wParam);

    void loadStage(int stage);
    void resetForDeath(int playerID);

    // 플레이어 관리 (네트워크 연동용)
    void addPlayer(int playerID);
    void removePlayer(int playerID);
    Player* getPlayer(int playerID);
    std::map<int, Player>& getPlayers(); // 전체 플레이어 목록 반환 (Broadcast용)
    // Peer 상태 업데이트 (클라이언트로부터 받은 정보 동기화가 필요할 때 사용)
    void UpdatePeerState(int peerID, int x, int y, int vx, int vy, int state);

    // Getters
    const int (*getCurrentMap() const)[MAP_WIDTH];
    int getStage() const;
    double getCameraX() const; // 서버에서는 0번 플레이어 기준 혹은 별도 로직

    const std::vector<std::unique_ptr<Monster>>& getMonsters() const;
    const std::vector<std::unique_ptr<Item>>& getItems() const;
    const std::vector<std::unique_ptr<Particle>>& getParticles() const;

    // 게임 상태 및 기타 정보
    GameState getGameState() const;
    void setGameState(GameState state);

    int getStageTime() const { return stage_time; }
    void setStage_time(int time);

    // 이벤트 및 BGM (서버는 로직만 처리하지만, 이벤트 큐는 필요할 수 있음)
    void pushEvent(GameEvent event);
    void clearEventQueue();
    const std::vector<GameEvent>& getEventQueue() const;

    // 스폰 함수들
    void spawnItem(Item::ItemType type, int x, int y);
    void spawnMonster(std::unique_ptr<Monster> monster);
    void spawnParticle(std::unique_ptr<Particle> particle);
    void spawnPlayerFireball(int x, int y, int vx);
    void spawnTinoFireball(int x, int y, int vx, int direction);
    void spawnTinoFireballEffect(int x, int y, int vx, int direction);

private:
    GameWorld();
    ~GameWorld();

    // 내부 로직 함수들
    void updatePlayers();
    void updateMonsters();
    void updateItems();
    void updateParticles();

    void checkCollisions();

    // 개별 충돌 체크 로직
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

    // 맵 및 스폰 초기화
    void initMaps();
    void initMonsterSpawns();
    void initMap1();
    void initMap2();
    void initMap3();
    void spawnMonsters();

    void monster_reset();
    void item_reset();

    // 멤버 변수
    std::map<int, Player> m_players; // 접속한 모든 플레이어 관리

    std::vector<std::unique_ptr<Monster>> monsters;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Particle>> newParticles;

    std::vector<MonsterSpawnInfo> stage1Monsters;
    std::vector<MonsterSpawnInfo> stage2Monsters;
    std::vector<MonsterSpawnInfo> stage3Monsters;

    GameState gameState;

    // 타이머 관련
    DWORD victoryStart;
    DWORD clearStart;
    DWORD godstart;
    DWORD deadStartTime;

    bool gameover_TitleDead;
    bool gameClearText;

    double cameraX;
    int map1[MAP_HEIGHT][MAP_WIDTH];
    int map2[MAP_HEIGHT][MAP_WIDTH];
    int map3[MAP_HEIGHT][MAP_WIDTH];

    int (*currentMap)[MAP_WIDTH];
    int stage;
    int stage_time;

    BGM_Type m_currentBGM;
    std::vector<GameEvent> m_eventQueue;

    int m_global_animation_frame_counter;
};