#pragma once

#include "Player.h"
#include "monsters/Monster.h"
#include "items/Item.h"
#include "Particles/Particle.h"
//#include "GameRender.h"
//#include "Sound.h"
#include <vector>
#include <memory>
#include <map>   // 멀티 플레이어 관리를 위해 추가
#include <mutex> // 다중 스레드에서 GameWorld 상태 접근 시 보호하기 위해 추가(현재는 메인 스레드만 접근하지만, 혹시 몰라 추가)
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

// 클라이언트로부터 수신된 입력 데이터를 처리하기 위한 구조체 추가
// PacketInfo.h에 정의된 구조체를 재활용하거나 새로 정의할 수 있음
// 키 입력 형태로 가정해놓고 후에 다시 수정할 예정
struct ClientInputData
{
    bool isKeyDown; // 눌림: true, 떼어짐: false
    WPARAM key;     // 어떤 키인지 (VK_LEFT, VK_SPACE 등)
};

class GameWorld {
public:
    GameWorld();

    bool isSolidTile(int tileValue) const;

    void init();
    //void sound_init(HWND hwnd);

    void update();
    //void updateAnimations();
    //void render(HDC hdc);

    //void handleKeyDown(WPARAM wParam);
    //void handleKeyUp(WPARAM wParam);
    void handlePlayerInput(int playerID, WPARAM key, bool isKeyDown); // 네트워크 패킷을 통해 받은 플레이어 입력 처리 추가

    // 서버용 플레이어 관리 함수 추가
    void addPlayer(int playerID);
    void removePlayer(int playerID);
    Player* getPlayer(int playerID); // 포인터 반환 가정

    const std::map<int, Player>& getPlayers() const { return m_players; } // 서버용 전체 플레이어 맵 반환 (BroadcastState 등에서 사용) 추가

    void loadStage(int stage);
	//void resetForDeath(); // 개별 플레이어 사망 처리는 별도로 구현 필요하므로 지금은 일단 주석 처리

    // Getter for the renderer to allow safe access from other classes
    //GameRender& getGameRender() { return m_gameRender; }

    //void transUpdate();
    //void cameraUpdate();

    //const Player& getPlayer() const;
    //Player& getPlayer();
    const int (*getCurrentMap() const)[MAP_WIDTH];
    int getStage() const;
    //double getCameraX() const;


    const std::vector<std::unique_ptr<Monster>>& getMonsters() const;
    const std::vector<std::unique_ptr<Item>>& getItems() const;
    const std::vector<std::unique_ptr<Particle>>& getParticles() const;
    const std::vector<std::unique_ptr<Particle>>& getNewParticles() const;

    void newParticles_insertTo_Particles();

    //int getLife() const { return player.getLife(); } // 개별 플레이어 정보 접근은 getPlayer(id)를 통해 수행해야 함
    //int getCoin() const { return player.getCoin(); }

    int getStageTime() const { return stage_time; }

    //int getTinoCooldownSpace() const { return player.getTinoCooldownSpace(); }

    bool getGameClearText() const { return gameClearText; }
    bool getGameoverTitleDead () const { return gameover_TitleDead; }
    int getGlobalAnimationFrameCounter() const { return m_global_animation_frame_counter; }

    //const bool* getKeyState() const; // 개별 플레이어의 입력 상태로 관리됨
    GameState getGameState() const;
    GameState_Trans getGameState_trans() const;
    void setGameState(GameState state);
    void setGameState_trans(GameState_Trans state_trans);
    void setStage_time(int time);
    //void setStageBGM();
    void setGameOverTitleDead(bool gameover);
    void setdeadStartTime(int time);

    void spawnItem(Item::ItemType type, int x, int y);
    void spawnMonster(std::unique_ptr<Monster> monster);
    void spawnParticle(std::unique_ptr<Particle> particle);

    // 파이어볼 스폰 시 주체(플레이어) 정보가 필요할 수 있음
    void spawnPlayerFireball(int x, int y, int vx);
    void spawnTinoFireball(int x, int y, int vx, int direction);
    void spawnTinoFireballEffect(int x, int y, int vx, int direction);

    //void playSound(const std::string& name, bool loop = false);
    //void stopAllSounds();

    int title_select;

//private:
//    GameRender m_gameRender;
//    Sound m_sound;

private:
    void updatePlayers(); // updatePlayer -> updatePlayers 개별 플레이어 업데이트 로직으로 변경
    void updateMonsters();
    void updateItems();
    void updateParticles();

    void checkCollisions();

    // 충돌 처리 함수들 (인자로 Player 객체를 받거나 루프 내부에서 처리)
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
    
    //void applyplayertakedamage();

    void initMaps();
    void initMonsterSpawns();
    void initMap1();
    void initMap2();
    void initMap3();

    //void dead();
    //void resurrection();
    void monster_reset();
    void item_reset();


    //Player player;
    std::map<int, Player> m_players; // PlayerID를 키로 하는 맵 추가
    std::mutex m_playerMtx;          // 플레이어 맵 접근 보호용 뮤텍스 추가

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

    //double cameraX;
    int map1[MAP_HEIGHT][MAP_WIDTH];
    int map2[MAP_HEIGHT][MAP_WIDTH];
    int map3[MAP_HEIGHT][MAP_WIDTH];

    int (*currentMap)[MAP_WIDTH];
    int stage;
    int stage_time;

    //bool keyState[256];
    int m_global_animation_frame_counter;
};

