
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
#include <tchar.h>


GameWorld& GameWorld::getInstance() {
    static GameWorld instance;
    return instance;
}

GameWorld::GameWorld()
{
    m_localPlayerId = -1;
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

GameWorld::~GameWorld() {
}

bool GameWorld::isSolidTile(int tileValue) const {
    return !(tileValue == 0 || tileValue == 2 || tileValue == 7 || tileValue == 8);
}

void GameWorld::sound_init(HWND hwnd) {
    m_sound.init(hwnd);
}

void GameWorld::init() {
    m_gameRender.init();

    // Winsock 및 네트워크 초기화
    if (m_networkManager.Init()) {
        // [수정] 서버 포트 9000으로 연결
        if (m_networkManager.Connect("127.0.0.1", 9000)) {
            // 연결 성공 시 로그 출력 등을 할 수 있음
        }
    }
    m_networkManager.Start(); // 수신 스레드 시작
    m_sound.loadAllSounds();
}

void GameWorld::playSound(const std::string& name, bool loop) {
    m_sound.play(name, loop);
}

void GameWorld::stopAllSounds() {
    m_sound.stopAllSounds();
}

void GameWorld::updateAnimations() {
    for (auto& pair : m_players) {
        pair.second.updateAnimation();
    }
    m_global_animation_frame_counter++;
}

void GameWorld::update() {
    cameraUpdate();

    // 1. 서버로부터 수신된 패킷 처리
    PacketData packet;
    while (PacketManager::GetInstance()->TryGetPacket(packet)) {
        switch (packet.type) {
        case PKT_PLAYER_STATE: {
            PlayerDataPacket player_data;
            if (packet.data.size() == sizeof(PlayerDataPacket)) {
                memcpy(&player_data, packet.data.data(), sizeof(PlayerDataPacket));

                // 내 플레이어 ID가 설정되지 않았다면 최초 패킷의 ID를 내 ID로 설정 (임시)
                // 실제로는 로그인 성공 패킷(PKT_LOGIN_OK) 등을 통해 ID를 받아야 정확함
                if (m_localPlayerId == -1) {
                    m_localPlayerId = player_data.playerID;
                }

                // 맵에 없으면 추가, 있으면 업데이트
                m_players[player_data.playerID].updateStateFromServer(player_data);
            }
            break;
        }
        case PKT_MONSTER_STATE: {
            // [수정] 몬스터 동기화 로직 구현
            // 서버에서 보낸 구조체와 일치해야 함 (PacketInfo.h 참고)
            // 현재 PacketInfo.h에 struct MonsterDataPacket 정의 필요 (위 1단계에서 추가함)

            // 패킷 데이터 크기 확인 (헤더 제외한 데이터)
            // 주의: 서버가 vector<MonsterDataPacket>을 보내는지, 단일 struct를 보내는지 확인 필요.
            // 여기서는 단일 MonsterDataPacket을 가정합니다.
            if (packet.data.size() >= sizeof(MonsterDataPacket)) {
                MonsterDataPacket* mData = (MonsterDataPacket*)packet.data.data();
                int count = packet.data.size() / sizeof(MonsterDataPacket);

                for (int i = 0; i < count; ++i) {
                    MonsterDataPacket& data = mData[i];

                    auto it = m_monsters.find(data.monsterID);
                    if (it != m_monsters.end()) {
                        // 기존 몬스터 업데이트
                        it->second->setX(data.x);
                        it->second->setY(data.y);
                        it->second->setVx(data.vx);
                        it->second->setVy(data.vy);
                        it->second->setAlive(data.isAlive);
                    }
                    else {
                        // 새 몬스터 생성
                        std::unique_ptr<Monster> newMonster = nullptr;
                        switch ((Monster::MonsterType)data.type) {
                        case Monster::MonsterType::NormalGoomba: newMonster = std::make_unique<NormalGoomba>(data.x, data.y); break;
                        case Monster::MonsterType::RedGoomba:    newMonster = std::make_unique<RedGoomba>(data.x, data.y); break;
                        case Monster::MonsterType::BlueGoomba:   newMonster = std::make_unique<BlueGoomba>(data.x, data.y); break;
                        case Monster::MonsterType::GreenTurtle:  newMonster = std::make_unique<GreenTurtle>(data.x, data.y); break;
                        case Monster::MonsterType::BrownTurtle:  newMonster = std::make_unique<BrownTurtle>(data.x, data.y); break;
                        case Monster::MonsterType::AngelTurtle:  newMonster = std::make_unique<AngelTurtle>(data.x, data.y); break;
                        case Monster::MonsterType::Bowser:       newMonster = std::make_unique<Bowser>(data.x, data.y); break;
                        }
                        if (newMonster) {
                            // newMonster->setID(data.monsterID); // Monster 클래스에 ID 멤버가 있다면 설정
                            m_monsters[data.monsterID] = std::move(newMonster);
                        }
                    }
                }
            }
            break;
        }
        default:
            break;
        }
    }

    // 2. 로컬 애니메이션 업데이트
    for (auto& pair : m_players) {
        pair.second.update();
    }

    // 몬스터, 아이템, 파티클의 로컬 업데이트 (애니메이션 등)
    // 위치는 서버에서 받지만, 애니메이션 프레임 등은 클라에서 돌려야 함
    for (auto& pair : m_monsters) {
        // pair.second->update(); // 클라 update는 물리 연산을 포함하므로, 
                                  // 서버 동기화 시에는 animationUpdate만 하거나 
                                  // 물리 연산을 끄고 렌더링 상태만 갱신해야 함.
                                  // 여기서는 일단 주석 처리 혹은 별도 함수(updateAnimation) 필요
    }

    newParticles_insertTo_Particles();
}

void GameWorld::cameraUpdate()
{
    double maxCameraX;
    const Player* localPlayer = getLocalPlayer();

    if (localPlayer && localPlayer->getGameState_trans() == GameState_Trans::GAME_NONE)
    {
        double playerWorldX = localPlayer->getX();

        if (playerWorldX < SCREEN_WIDTH / 2.0) {
            cameraX = 0;
        }
        else {
            cameraX = playerWorldX - (SCREEN_WIDTH / 2.0);
        }

        maxCameraX = (double)MAP_WIDTH * TILE_SIZE - SCREEN_WIDTH;
        if (maxCameraX < 0) {
            maxCameraX = 0;
            cameraX = 0;
        }

        if (cameraX > maxCameraX) {
            cameraX = maxCameraX;
        }
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

        Packet_KEY_EVENT_C2S keyEvent;
        keyEvent.keyCode = wParam;
        char buffer[sizeof(PacketHeader) + sizeof(Packet_KEY_EVENT_C2S)];
        unsigned int packetSize = PacketManager::GetInstance()->Serialize_KEY_EVENT(buffer, keyEvent, PKT_KEY_DOWN);
        m_networkManager.Send(std::string(buffer, packetSize));
    }

    // 타이틀 화면 입력 처리
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
    setStageBGM();
    if (stage == 1) currentMap = map1;
    else if (stage == 2) currentMap = map2;
    else if (stage == 3) currentMap = map3;
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

// 클라_GameWorld.cpp 맨 아래에 추가
GameState GameWorld::getGameState() const {
    return gameState;
}