#pragma once

#include "NetworkManager/NetworkManager.h"
#include "Player.h"
#include "monsters/Monster.h"
#include "items/Item.h"
#include "Particles/Particle.h"
#include "Sound.h"
#include <vector>
#include <memory>
#include "GameRender.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640
#define MAP_WIDTH 200
#define MAP_HEIGHT 15
#define TILE_SIZE 40

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


class GameWorld {
public:
    static GameWorld& getInstance();

    // Delete copy constructor and assignment operator to prevent copying
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

    // Getter for the renderer to allow safe access from other classes
    GameRender& getGameRender() { return m_gameRender; }

    void cameraUpdate();

    // --- Player Management ---
    Player* getLocalPlayer();
    const Player* getLocalPlayer() const;
    Player* getPlayerById(int id);
    const std::map<int, Player>& getPlayers() const;
    void setLocalPlayerId(int id);
    // --- End Player Management ---

    const int (*getCurrentMap() const)[MAP_WIDTH];
    int getStage() const;
    double getCameraX() const;

    const std::map<int, std::unique_ptr<Monster>>& getMonsters() const;
    const std::vector<std::unique_ptr<Item>>& getItems() const;
    const std::vector<std::unique_ptr<Particle>>& getParticles() const;
    const std::vector<std::unique_ptr<Particle>>& getNewParticles() const;

    void newParticles_insertTo_Particles();

    // Getters that now refer to the local player
    int getLife() const;
    int getCoin() const;
    int getTinoCooldownSpace() const;

    int getStageTime() const { return stage_time; }
    bool getGameClearText() const { return gameClearText; }
    bool getGameoverTitleDead () const { return gameover_TitleDead; }
    GameState_Trans getGameState_trans() const;

    int getGlobalAnimationFrameCounter() const { return m_global_animation_frame_counter; }

    const bool* getKeyState() const;
    GameState getGameState() const;

    void setGameState(GameState state);
    void setGameState_trans(GameState_Trans state_trans);
    void setStage_time(int time);
    void setGameOverTitleDead(bool gameover);
    void setdeadStartTime(int time);

    void setStageBGM();

    void playSound(const std::string& name, bool loop = false);
    void stopAllSounds();

    int title_select;

private:
    GameWorld(); // Private constructor for Singleton pattern
    ~GameWorld(); // Private destructor for Singleton pattern

    GameRender m_gameRender;
    NetworkManager m_networkManager;
    Sound m_sound;

    // --- New Player Structure ---
    std::map<int, Player> m_players;
    int m_localPlayerId;
    // --- End New Player Structure ---

    std::map<int, std::unique_ptr<Monster>> m_monsters;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<Particle>> newParticles;

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
};