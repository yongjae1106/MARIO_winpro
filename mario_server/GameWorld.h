#pragma once

#include "Player.h"
#include "monsters/Monster.h"
#include "items/Item.h"
#include "Particles/Particle.h"
//#include "GameRender.h"
//#include "Sound.h"
#include <vector>
#include <memory>
#include <map> // map 헤더 필요
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

    // Getter for the renderer to allow safe access from other classes
    //GameRender& getGameRender() { return m_gameRender; }

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

    int getLife() const { return player.getLife(); }
    int getCoin() const { return player.getCoin(); }
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
    void setStageBGM();
    void setGameOverTitleDead(bool gameover);
    void setdeadStartTime(int time);

    void spawnItem(Item::ItemType type, int x, int y);
    void spawnMonster(std::unique_ptr<Monster> monster);
    void spawnParticle(std::unique_ptr<Particle> particle);
    void spawnPlayerFireball(int x, int y, int vx);
    void spawnTinoFireball(int x, int y, int vx, int direction);
    void spawnTinoFireballEffect(int x, int y, int vx, int direction);

    //void playSound(const std::string& name, bool loop = false);
    void stopAllSounds();

    int title_select;

    // 추가: 접속한 클라이언트(Peer)의 상태를 업데이트하는 함수
    void UpdatePeerState(int peerID, int x, int y, int vx, int vy, int state)
    {
        // 맵에 없으면 새로 생성, 있으면 업데이트
        // Player 클래스의 세터 함수들을 이용해 동기화
        m_peerPlayers[peerID].setX(x);
        m_peerPlayers[peerID].setY(y);
        m_peerPlayers[peerID].setVx(vx);
        m_peerPlayers[peerID].setVy(vy);
        // m_peerPlayers[peerID].setState((PlayerState)state); // 필요 시 state 변환 로직 추가
    }

    // 추가: 특정 플레이어 제거 (연결 끊김 시)
    void RemovePeer(int peerID)
    {
        m_peerPlayers.erase(peerID);
    }

    // 추가: 모든 접속자 정보 반환 (Broadcast용)
    const std::map<int, Player>& GetPeerPlayers() const
    {
        return m_peerPlayers;
    }

private:
    //GameRender m_gameRender;
    //Sound m_sound;

private:
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

    bool keyState[256];
    int m_global_animation_frame_counter;

    // 추가: 접속된 플레이어들을 관리하는 컨테이너 (Key: SocketID, Value: Player 객체)
    std::map<int, Player> m_peerPlayers;
};

