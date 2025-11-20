#include "GameWorld.h"
#include "monsters/NormalGoomba.h"
#include "monsters/RedGoomba.h"
#include "monsters/BlueGoomba.h"
#include "monsters/GreenTurtle.h"
#include "monsters/BrownTurtle.h"
#include "monsters/AngelTurtle.h"
#include "monsters/Bowser.h"
#include "items/Mushroom.h"
#include "items/Star.h"
#include "items/Flower.h"
#include "items/Tino.h"
#include "items/UpMushroom.h"
#include "Particles/Fireball.h"
#include "Particles/PlayerFireball.h"
#include "Particles/TinoFireball.h"
#include "Particles/TinoFireballEffect.h"
#include "Particles/Particle.h"
#include "monsters/Turtle.h"
#include <memory>
#include <tchar.h> // Added for _stprintf_s and OutputDebugString

// Private constructor for Singleton pattern
GameWorld::GameWorld()
{
    gameState = GameState::GAME_TITLE;
    gameState_trans = GameState_Trans::GAME_NONE;
    cameraX = 0;
    stage = 1;
    stage_time = 0;
    memset(keyState, 0, sizeof(keyState));
    m_global_animation_frame_counter = 0;
    currentMap = map1;
    transformStartTime = 0;
    deadStartTime = 0;
    victoryStart = 0;
    clearStart = 0;
    godstart = 0;
    gameover_TitleDead = false;
    gameClearText = false;
}

// Private destructor for Singleton pattern
GameWorld::~GameWorld() {
    // Clean up resources if necessary
}

bool GameWorld::isSolidTile(int tileValue) const {
    // Based on mario_old/data.h comments and fireball collision logic
    // 0: hole, 2: coin, 7: flag, 8: flag top are NOT solid for fireballs
    return !(tileValue == 0 || tileValue == 2 || tileValue == 7 || tileValue == 8);
}

void GameWorld::sound_init(HWND hwnd) {
    m_sound.init(hwnd);
}

void GameWorld::init() {
    m_gameRender.init();
    m_networkManager.Init(); // NetworkManager 초기화 (WSAStartup 등)
    m_networkManager.Connect("127.0.0.1", 12345); // 임시 IP 및 포트
    m_networkManager.Start(); // 네트워크 수신/송신 스레드 시작
    m_sound.loadAllSounds();
}

void GameWorld::playSound(const std::string& name, bool loop) {
    m_sound.play(name, loop);
}

void GameWorld::stopAllSounds() {
    m_sound.stopAllSounds();
}

void GameWorld::updateAnimations() {
    player.updateAnimation();
    m_global_animation_frame_counter++; // Increment global animation frame counter
}

void GameWorld::update() {
    // This is the main update function for GameWorld
    // It should orchestrate all game logic updates
    // For now, let's just ensure camera updates here
    cameraUpdate();

    // Process incoming network packets
    std::string receivedPacket;
    while (m_networkManager.TryGetReceivedData(receivedPacket)) {
        processServerUpdate(receivedPacket);
    }

    // Other update logic can be added here later (e.g., monster updates, item updates)
}

void GameWorld::cameraUpdate()
{
    // cameraX is a GameWorld member, no need to re-declare
    // maxCameraX is only used in this function, declare it here
    double maxCameraX;

    if (gameState_trans == GameState_Trans::GAME_NONE)
    {
        double playerWorldX = player.getX();

        // Camera does not scroll until player passes half screen
        if (playerWorldX < SCREEN_WIDTH / 2.0) {
            cameraX = 0;
        } else {
            // Once player passes center, camera tries to center on player.
            cameraX = playerWorldX - (SCREEN_WIDTH / 2.0);
        }

        // Clamp camera to the rightmost boundary of the map.
        maxCameraX = (double)MAP_WIDTH * TILE_SIZE - SCREEN_WIDTH;
        if (maxCameraX < 0) { // Handle cases where map is smaller than screen
            maxCameraX = 0;
            cameraX = 0; // If map is smaller, camera should probably just stay at 0
        }

        if (cameraX > maxCameraX) {
            cameraX = maxCameraX;
        }

        // Clamp camera to prevent negative values (defensive)
        if (cameraX < 0) {
            cameraX = 0;
        }
    }
}

void GameWorld::render(HDC hdc) {
    m_gameRender.render(hdc, *this);
}

void GameWorld::handleKeyDown(WPARAM wParam) {
    if (wParam < 256) {
        keyState[wParam] = true;
        // TODO: Send key down event to server
    }
    // Title screen navigation can remain client-side
    if (gameState == GameState::GAME_TITLE) {
        switch (wParam) {
            case VK_UP: if (title_select == 1) title_select = 0; break;
            case VK_DOWN: if (title_select == 0) title_select = 1; break;
            case VK_RETURN:
                if (title_select == 0) gameState = GameState::GAME_START;
                else exit(1);
                break;
        }
    }
}

void GameWorld::handleKeyUp(WPARAM wParam) {
    if (wParam < 256) {
        keyState[wParam] = false;
        // TODO: Send key up event to server
    }
}

void GameWorld::loadStage(int newStage) {
    stage = newStage;
    monsters.clear();
    items.clear();
    particles.clear();
    // setStage_time(400); // Server will manage time
    setStageBGM(); // Client still plays BGM
    if (stage == 1) currentMap = map1;
    else if (stage == 2) currentMap = map2;
    else if (stage == 3) currentMap = map3;
    // spawnMonsters(); // Removed, server-side
}

// These getters are declared in GameWorld.h and defined here.
const Player& GameWorld::getPlayer() const { return player; }
Player& GameWorld::getPlayer() { return player; }
const int(*GameWorld::getCurrentMap() const)[MAP_WIDTH] { return currentMap; }
int GameWorld::getStage() const { return stage; }
double GameWorld::getCameraX() const { return cameraX; }
const std::vector<std::unique_ptr<Monster>>& GameWorld::getMonsters() const { return monsters; }
const std::vector<std::unique_ptr<Item>>& GameWorld::getItems() const { return items; }
const std::vector<std::unique_ptr<Particle>>& GameWorld::getParticles() const { return particles; }
const std::vector<std::unique_ptr<Particle>>& GameWorld::getNewParticles() const { return newParticles; }

// The following setters are declared in GameWorld.h and defined here.
void GameWorld::setGameState(GameState state) { gameState = state; }
void GameWorld::setGameState_trans(GameState_Trans state_trans) { gameState_trans = state_trans; }
void GameWorld::setStage_time(int time) { stage_time = time; }
void GameWorld::setGameOverTitleDead(bool gameover) { gameover_TitleDead = gameover; }
void GameWorld::setdeadStartTime(int time) { deadStartTime = time; }

void GameWorld::newParticles_insertTo_Particles() {
    if (!newParticles.empty()) {
        for (auto& p : newParticles) particles.push_back(std::move(p));
        newParticles.clear();
    }
}

void GameWorld::processServerUpdate(const std::string& serializedData) {
    // TODO: Implement deserialization of server data and update local player, monsters, items, particles, etc.
    // This will involve parsing 'serializedData' and updating the member variables.
    // Example:
    // ServerStatePacket packet = deserialize(serializedData);
    // player.updateStateFromServer(packet.playerData);
    // updateMonstersFromServer(packet.monsterData);
    // updateItemsFromServer(packet.itemData);
    // ...
}

void GameWorld::setStageBGM() {
    if (stage == 1 || stage == 2) playSound("GroundTheme", true);
    else if (stage == 3) playSound("CastleTheme", true);
}
