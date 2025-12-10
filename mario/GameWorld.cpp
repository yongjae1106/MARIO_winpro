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

GameWorld& GameWorld::getInstance() {
    static GameWorld instance; // CRT 초기화 후 안전하게 생성
    return instance;
}

// Private constructor for Singleton pattern
GameWorld::GameWorld()
{
    m_localPlayerId = -1; // Initialize local player ID
    gameState = GameState::GAME_TITLE;
    cameraX = 0;
    stage = 1;
    stage_time = 0;
    memset(keyState, 0, sizeof(keyState));
    m_global_animation_frame_counter = 0;
    currentMap = map1;
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
    // Loop through all players and update their animations
    for (auto& pair : m_players) {
        pair.second.updateAnimation();
    }
    m_global_animation_frame_counter++; // Increment global animation frame counter
}

void GameWorld::update() {
    // This is the main update function for GameWorld
    // It should orchestrate all game logic updates
    cameraUpdate();

    // 1. Process incoming network packets from PacketManager
    PacketData packet;
    while (PacketManager::GetInstance()->TryGetPacket(packet)) {
        switch (packet.type) {
            case PKT_PLAYER_STATE: { 
                PlayerDataPacket player_data;
                if (packet.data.size() == sizeof(PlayerDataPacket)) {
                    memcpy(&player_data, packet.data.data(), sizeof(PlayerDataPacket));
                    auto& playerToUpdate = m_players[player_data.playerID]; 
                    playerToUpdate.updateStateFromServer(player_data);
                }
                break;
            }
            case PKT_MONSTER_STATE: {
                MonsterDataPacket monster_data;
                if (packet.data.size() == sizeof(MonsterDataPacket)) {
                    memcpy(&monster_data, packet.data.data(), sizeof(MonsterDataPacket));
                    
                    auto it = m_monsters.find(monster_data.monsterID);
                    if (it != m_monsters.end()) {
                        // Existing monster, update its state
                        it->second->updateStateFromServer(monster_data);
                    } else {
                        // New monster. We need a way to create the correct monster type.
                        // This requires a "factory" pattern.
                        // TODO: Implement a MonsterFactory to create different monster types based on 'monster_data.type'.
                        // For now, we will just log that a new monster needs to be created.
                        // m_monsters[monster_data.monsterID] = createMonsterFromPacket(monster_data);
                    }
                }
                break;
            }
            // TODO: Add cases for other packet types (item state, etc.)
            default:
                // printf("[Client] Unknown packet type received: %u\n", packet.type);
                break;
        }
    }

    // 2. Handle local player input and send to server
    Player* localPlayer = getLocalPlayer();
    if (localPlayer) {
        // TODO: Check keyState, decide on action (move, jump, attack).
        // If an action occurs, create a C2S packet and send it.
        // e.g., if (keyState[VK_LEFT]) { ... create and send CS_MOVE_PACKET ... }
    }

    // 3. Other update logic (e.g., local animations for all objects)
    for (auto& pair : m_players) {
        pair.second.update(); // This calls updateAnimation() for each player
    }

    // Update animations for Monsters, Items, and Particles
    for (auto& pair : m_monsters) {
        pair.second->update();
    }
    for (auto& item : items) {
        item->update();
    }
    // TODO: Add an update() method to the Particle base class and uncomment the following lines.
    // for (auto& particle : particles) {
    //     particle->update();
    // }
    newParticles_insertTo_Particles();
}

void GameWorld::cameraUpdate()
{
    // cameraX is a GameWorld member, no need to re-declare
    // maxCameraX is only used in this function, declare it here
    double maxCameraX;

    if (getLocalPlayer() && getLocalPlayer()->getGameState_trans() == GameState_Trans::GAME_NONE)
    {
        const Player* localPlayer = getLocalPlayer();
        if (!localPlayer) return; // Do nothing if there is no local player

        double playerWorldX = localPlayer->getX();

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
        
        // Send key down event to server
        Packet_KEY_EVENT_C2S keyEvent;
        keyEvent.keyCode = wParam;
        char buffer[sizeof(PacketHeader) + sizeof(Packet_KEY_EVENT_C2S)];
        unsigned int packetSize = PacketManager::GetInstance()->Serialize_KEY_EVENT(buffer, keyEvent, PKT_KEY_DOWN);
        m_networkManager.Send(std::string(buffer, packetSize));
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
        
        // Send key up event to server
        Packet_KEY_EVENT_C2S keyEvent;
        keyEvent.keyCode = wParam;
        char buffer[sizeof(PacketHeader) + sizeof(Packet_KEY_EVENT_C2S)];
        unsigned int packetSize = PacketManager::GetInstance()->Serialize_KEY_EVENT(buffer, keyEvent, PKT_KEY_UP);
        m_networkManager.Send(std::string(buffer, packetSize));
    }
}

void GameWorld::loadStage(int newStage) {
    stage = newStage;
    m_monsters.clear();
    items.clear();
    particles.clear();
    // setStage_time(400); // Server will manage time
    setStageBGM(); // Client still plays BGM
    if (stage == 1) currentMap = map1;
    else if (stage == 2) currentMap = map2;
    else if (stage == 3) currentMap = map3;
    // spawnMonsters(); // Removed, server-side
}

// --- Player Management ---
Player* GameWorld::getLocalPlayer()
{
    if (m_localPlayerId == -1) return nullptr;
    auto it = m_players.find(m_localPlayerId);
    if (it == m_players.end()) return nullptr;
    return &it->second;
}

const Player* GameWorld::getLocalPlayer() const
{
    if (m_localPlayerId == -1) return nullptr;
    auto it = m_players.find(m_localPlayerId);
    if (it == m_players.end()) return nullptr;
    return &it->second;
}

Player* GameWorld::getPlayerById(int id)
{
    auto it = m_players.find(id);
    if (it == m_players.end()) return nullptr;
    return &it->second;
}

const std::map<int, Player>& GameWorld::getPlayers() const
{
    return m_players;
}

void GameWorld::setLocalPlayerId(int id)
{
    m_localPlayerId = id;
}
// --- End Player Management ---


// These getters are declared in GameWorld.h and defined here.
const int(*GameWorld::getCurrentMap() const)[MAP_WIDTH] { return currentMap; }
int GameWorld::getStage() const { return stage; }
double GameWorld::getCameraX() const { return cameraX; }
const std::map<int, std::unique_ptr<Monster>>& GameWorld::getMonsters() const { return m_monsters; }
const std::vector<std::unique_ptr<Item>>& GameWorld::getItems() const { return items; }
const std::vector<std::unique_ptr<Particle>>& GameWorld::getParticles() const { return particles; }
const std::vector<std::unique_ptr<Particle>>& GameWorld::getNewParticles() const { return newParticles; }

int GameWorld::getLife() const
{
    const Player* p = getLocalPlayer();
    return p ? p->getLife() : 0;
}

int GameWorld::getCoin() const
{
    const Player* p = getLocalPlayer();
    return p ? p->getCoin() : 0;
}

int GameWorld::getTinoCooldownSpace() const
{
    const Player* p = getLocalPlayer();
    return p ? p->getTinoCooldownSpace() : 0;
}

GameState_Trans GameWorld::getGameState_trans() const
{
    const Player* p = getLocalPlayer();
    return p ? p->getGameState_trans() : GameState_Trans::GAME_NONE;
}

// The following setters are declared in GameWorld.h and defined here.
void GameWorld::setGameState(GameState state) { gameState = state; }
void GameWorld::setStage_time(int time) { stage_time = time; }
void GameWorld::setGameOverTitleDead(bool gameover) { gameover_TitleDead = gameover; }

void GameWorld::newParticles_insertTo_Particles() {
    if (!newParticles.empty()) {
        for (auto& p : newParticles) particles.push_back(std::move(p));
        newParticles.clear();
    }
}



void GameWorld::setStageBGM() {
    if (stage == 1 || stage == 2) playSound("GroundTheme", true);
    else if (stage == 3) playSound("CastleTheme", true);
}