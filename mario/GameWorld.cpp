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
    gameState_trans = GameState_Trans::GAME_NONE; // Restored
    cameraX = 0;
    stage = 1;
    stage_time = 0; // Restored
    memset(keyState, 0, sizeof(keyState));
    m_global_animation_frame_counter = 0;
    currentMap = map1;
    transformStartTime = 0; // Restored
    deadStartTime = 0; // Restored
    victoryStart = 0; // Restored
    clearStart = 0; // Restored
    godstart = 0; // Restored
    gameover_TitleDead = false; // Restored
    gameClearText = false; // Restored
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

void GameWorld::cameraUpdate() 
{
    // Camera update logic remains client-side for smooth rendering
    // It should follow the player's position as received from the server
    // For now, keep existing logic, but understand player.getX() and player.getVx()
    // will be based on server-provided data.
    if (gameState_trans == GameState_Trans::GAME_NONE) // Keep this for client-side visual pause during transformation
    {
        int playerX = player.getX();
        if (playerX > SCREEN_WIDTH / 2) {
            cameraX += player.getVx();
            player.setX(SCREEN_WIDTH / 2);
        } else if (playerX < SCREEN_WIDTH / 2 && cameraX > 0) {
            cameraX += player.getVx();
            if (cameraX < 0) cameraX = 0;
            player.setX(SCREEN_WIDTH / 2);
        }
        if (cameraX < 0) cameraX = 0;
        if (cameraX > MAP_WIDTH * TILE_SIZE - SCREEN_WIDTH)
            cameraX = MAP_WIDTH * TILE_SIZE - SCREEN_WIDTH;
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

const Player& GameWorld::getPlayer() const { return player; }
Player& GameWorld::getPlayer() { return player; }
const int(*GameWorld::getCurrentMap() const)[MAP_WIDTH] { return currentMap; }
int GameWorld::getStage() const { return stage; }
double GameWorld::getCameraX() const { return cameraX; }
const std::vector<std::unique_ptr<Monster>>& GameWorld::getMonsters() const { return monsters; }
const std::vector<std::unique_ptr<Item>>& GameWorld::getItems() const { return items; }
const std::vector<std::unique_ptr<Particle>>& GameWorld::getParticles() const { return particles; }
const std::vector<std::unique_ptr<Particle>>& GameWorld::getNewParticles() const { return newParticles; }
int GameWorld::getLife() const { return player.getLife(); }
int GameWorld::getCoin() const { return player.getCoin(); }
int GameWorld::getTinoCooldownSpace() const { return player.getTinoCooldownSpace(); }
int GameWorld::getGlobalAnimationFrameCounter() const { return m_global_animation_frame_counter; }
const bool* GameWorld::getKeyState() const { return keyState; }
GameState GameWorld::getGameState() const { return gameState; }
GameState_Trans GameWorld::getGameState_trans() const { return gameState_trans; } // Restored
int GameWorld::getStageTime() const { return stage_time; } // Restored
bool GameWorld::getGameClearText() const { return gameClearText; } // Restored
bool GameWorld::getGameoverTitleDead() const { return gameover_TitleDead; } // Restored

void GameWorld::setGameState(GameState state) { gameState = state; }
void GameWorld::setGameState_trans(GameState_Trans state_trans) { gameState_trans = state_trans; } // Restored
void GameWorld::setStage_time(int time) { stage_time = time; } // Restored
void GameWorld::setGameOverTitleDead(bool gameover) { gameover_TitleDead = gameover; } // Restored
void GameWorld::setdeadStartTime(int time) { deadStartTime = time; } // Restored

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
