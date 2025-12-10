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
#include <iostream>

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

    // [중요] 맵 데이터 초기화 호출
    initMaps();
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
        // 서버 포트 9000으로 연결
        if (m_networkManager.Connect("127.0.0.1", 9000)) {
            std::cout << "[Client] Connected to server." << std::endl;
        }
        else {
            std::cout << "[Client] Connection failed." << std::endl;
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

                if (m_localPlayerId == -1) {
                    m_localPlayerId = player_data.playerID;
                    std::cout << "[Client] Assigned Local Player ID: " << m_localPlayerId << std::endl;
                }

                // 맵에 없으면 추가, 있으면 업데이트
                m_players[player_data.playerID].updateStateFromServer(player_data);
            }
            break;
        }
        case PKT_MONSTER_STATE: {
            if (packet.data.size() >= sizeof(MonsterDataPacket)) {
                MonsterDataPacket* mData = (MonsterDataPacket*)packet.data.data();
                int count = packet.data.size() / sizeof(MonsterDataPacket); // 현재는 1개씩 온다고 가정

                // 단일 패킷으로 가정
                MonsterDataPacket& data = *mData;

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
                        m_monsters[data.monsterID] = std::move(newMonster);
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

    // 몬스터 업데이트 (물리는 서버가 하므로, 클라는 상태만 반영)
    // for (auto& pair : m_monsters) { ... } 

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

// [추가] 맵 데이터 초기화 함수들 (서버 코드 복사본)
void GameWorld::initMaps() {
    initMap1();
    initMap2();
    initMap3();
}

void GameWorld::initMonsterSpawns() {
    stage1Monsters = {
        { Monster::MonsterType::NormalGoomba, 20, 12 },
        { Monster::MonsterType::NormalGoomba, 25, 12 },
        { Monster::MonsterType::NormalGoomba, 30, 12 },
        { Monster::MonsterType::NormalGoomba, 40, 12 },
        { Monster::MonsterType::NormalGoomba, 48, 12 },
        { Monster::MonsterType::NormalGoomba, 50, 12 },
        { Monster::MonsterType::NormalGoomba, 58, 12 },
        { Monster::MonsterType::NormalGoomba, 60, 12 },
        { Monster::MonsterType::NormalGoomba, 62, 12 },
        { Monster::MonsterType::NormalGoomba, 70, 12 },
        { Monster::MonsterType::NormalGoomba, 78, 12 },
        { Monster::MonsterType::NormalGoomba, 80, 12 },
        { Monster::MonsterType::NormalGoomba, 88, 12 },
        { Monster::MonsterType::NormalGoomba, 90, 12 },
        { Monster::MonsterType::NormalGoomba, 92, 12 },
        { Monster::MonsterType::GreenTurtle, 35, 10 },
        { Monster::MonsterType::GreenTurtle, 53, 10 },
        { Monster::MonsterType::GreenTurtle, 65, 10 },
        { Monster::MonsterType::GreenTurtle, 82, 10 },
        { Monster::MonsterType::GreenTurtle, 95, 10 },
    };

    stage2Monsters = {
        { Monster::MonsterType::RedGoomba, 20, 12 },
        { Monster::MonsterType::RedGoomba, 25, 12 },
        { Monster::MonsterType::RedGoomba, 30, 12 },
        { Monster::MonsterType::RedGoomba, 40, 12 },
        { Monster::MonsterType::RedGoomba, 48, 12 },
        { Monster::MonsterType::RedGoomba, 50, 12 },
        { Monster::MonsterType::RedGoomba, 58, 12 },
        { Monster::MonsterType::RedGoomba, 60, 12 },
        { Monster::MonsterType::RedGoomba, 62, 12 },
        { Monster::MonsterType::RedGoomba, 70, 12 },
        { Monster::MonsterType::AngelTurtle, 15, 5 },
        { Monster::MonsterType::AngelTurtle, 45, 5 },
        { Monster::MonsterType::AngelTurtle, 65, 5 },
        { Monster::MonsterType::AngelTurtle, 85, 5 },
        { Monster::MonsterType::AngelTurtle, 105, 5 },
    };

    stage3Monsters = {
        { Monster::MonsterType::BlueGoomba, 20, 8 },
        { Monster::MonsterType::BlueGoomba, 25, 8 },
        { Monster::MonsterType::BlueGoomba, 30, 8 },
        { Monster::MonsterType::BlueGoomba, 40, 8 },
        { Monster::MonsterType::BlueGoomba, 48, 8 },
        { Monster::MonsterType::BrownTurtle, 35, 8 },
        { Monster::MonsterType::BrownTurtle, 55, 8 },
        { Monster::MonsterType::BrownTurtle, 65, 8 },
        { Monster::MonsterType::BrownTurtle, 85, 8 },
        { Monster::MonsterType::BrownTurtle, 95, 8 },
        { Monster::MonsterType::Bowser, 130, 6 },
    };
}

void GameWorld::spawnMonsters()
{
    const std::vector<MonsterSpawnInfo>* currentMonsterSpawns = nullptr;
    if (stage == 1) {
        currentMonsterSpawns = &stage1Monsters;
    }
    else if (stage == 2) {
        currentMonsterSpawns = &stage2Monsters;
    }
    else if (stage == 3) {
        currentMonsterSpawns = &stage3Monsters;
    }

    if (currentMonsterSpawns) {
        for (const auto& spawnInfo : *currentMonsterSpawns) {
            float x = spawnInfo.x * TILE_SIZE;
            float y = spawnInfo.y * TILE_SIZE;
            switch (spawnInfo.type) {
            case Monster::MonsterType::NormalGoomba:
                monsters.push_back(std::make_unique<NormalGoomba>(x, y));
                break;
            case Monster::MonsterType::RedGoomba:
                monsters.push_back(std::make_unique<RedGoomba>(x, y));
                break;
            case Monster::MonsterType::BlueGoomba:
                monsters.push_back(std::make_unique<BlueGoomba>(x, y));
                break;
            case Monster::MonsterType::GreenTurtle:
                monsters.push_back(std::make_unique<GreenTurtle>(x, y));
                break;
            case Monster::MonsterType::BrownTurtle:
                monsters.push_back(std::make_unique<BrownTurtle>(x, y));
                break;
            case Monster::MonsterType::AngelTurtle:
                monsters.push_back(std::make_unique<AngelTurtle>(x, y));
                break;
            case Monster::MonsterType::Bowser:
                monsters.push_back(std::make_unique<Bowser>(x, y));
                break;
            }
        }
    }
}

void GameWorld::initMap1() {
    // Map 1 initialization logic from func.cpp
    for (int j = 0; j < MAP_WIDTH; j++)
    {
        map1[MAP_HEIGHT - 2][j] = 1;
        map1[MAP_HEIGHT - 1][j] = 1;
    }
    map1[7][0] = 61;
    map1[7][4] = 62;
    map1[9][1] = 65;    // 보여주기식 스타박스
    //벽돌
    map1[6][16] = 10;
    map1[6][18] = 10;
    map1[10][15] = 10;
    map1[10][16] = 10;
    map1[10][18] = 10;
    map1[10][19] = 10;
    map1[10][30] = 10;
    map1[10][32] = 10;
    map1[10][75] = 10;
    map1[10][76] = 10;
    map1[10][77] = 10;
    map1[7][77] = 10;
    map1[7][45] = 10;
    map1[7][44] = 10;
    map1[7][46] = 10;
    map1[7][43] = 10;
    map1[7][47] = 10;
    map1[9][49] = 10;
    map1[9][50] = 10;

    // 0. 구멍
    map1[14][21] = 0;
    map1[13][21] = 0;
    map1[14][22] = 0;
    map1[13][22] = 0;
    map1[14][23] = 0;
    map1[13][23] = 0;
    map1[14][45] = 0;
    map1[13][45] = 0;
    map1[14][68] = 0;
    map1[13][68] = 0;
    map1[14][69] = 0;
    map1[13][69] = 0;
    map1[14][70] = 0;
    map1[13][70] = 0;
    map1[14][85] = 0;
    map1[13][85] = 0;
    map1[14][86] = 0;
    map1[13][86] = 0;
    map1[14][87] = 0;
    map1[13][87] = 0;

    // 2. 코인
    map1[12][14] = 2;
    map1[5][17] = 2;
    map1[9][15] = 2;
    map1[9][18] = 2;
    map1[9][76] = 2;
    map1[9][31] = 2;
    map1[10][60] = 2;
    map1[6][43] = 2;
    map1[6][44] = 2;
    map1[6][45] = 2;
    map1[6][78] = 2;

    //4. 파이프
    map1[10][55] = 43; // 좌상단
    map1[10][56] = 42; // 우상단
    map1[11][55] = 41; // 좌하단
    map1[11][56] = 40; // 우하단
    map1[12][55] = 41;
    map1[12][56] = 40;


    map1[11][38] = 43; // 좌하단
    map1[11][39] = 42; // 우하단
    map1[12][38] = 41;
    map1[12][39] = 40;
    map1[13][38] = 41;
    map1[13][39] = 40;
    map1[14][38] = 41;
    map1[14][39] = 40;



    // 5. 계단 
    map1[14][80] = 5;
    map1[13][81] = 5; map1[14][81] = 5;
    map1[12][82] = 5; map1[13][82] = 5; map1[14][82] = 5;
    map1[11][83] = 5; map1[12][83] = 5; map1[13][83] = 5; map1[14][83] = 5;
    map1[10][84] = 5; map1[11][84] = 5; map1[12][84] = 5; map1[13][84] = 5; map1[14][84] = 5;

    map1[14][88] = 5;
    map1[13][88] = 5;
    map1[12][88] = 5;
    map1[11][88] = 5;
    map1[10][88] = 5;
    map1[14][89] = 5;
    map1[13][89] = 5;
    map1[12][89] = 5;
    map1[11][89] = 5;
    map1[14][90] = 5;
    map1[13][90] = 5;
    map1[12][90] = 5;
    map1[14][91] = 5;
    map1[13][91] = 5;
    map1[14][92] = 5;

    // 6. 미스테리 박스
    map1[9][10] = 6;
    map1[6][17] = 64;
    map1[10][17] = 64;
    map1[10][31] = 64;
    map1[10][60] = 64;
    map1[20][76] = 64;
    map1[10][62] = 64;
    map1[10][64] = 64;
    map1[6][62] = 62;
    map1[7][78] = 64;
    map1[7][73] = 64;

    // 깃발
    map1[MAP_HEIGHT - 3][108] = 5;
    // 7. 깃발
    for (int y = 3; y <= 11; y++)
    {
        map1[y][108] = 7;
    }
    // 8. 깃발 꼭짓점
    map1[2][108] = 8;

    // 9. 성
    map1[12][115] = 9;
}

void GameWorld::initMap2() {
    // Map 2 initialization logic from func.cpp
    // 0: 구멍 1: 땅 2: 코인 3:굼바 4:파이프 5:계단 6:미스테리박스 7:깃발 8: 깃발꼭짓점 9:성 10: 벽돌 
    // 90. 버섯머리1 91. 버섯머리2 92. 버섯머리3 11. 버섯줄기 12. 버섯줄기2 13.구름 14: 불기둥스위치 15: 회색벽돌
    // 16: 사용된블럭 17:용암head 18:용암body 60:스타박스 61:꽃박스 62: 티노박스 63: 생명버섯박스 64: 코인박스 999: 피치공주

    memset(map2, 0, sizeof(map2));  //맵 초기화

    // 1. 시작 벽돌(=땅)
    for (int y = 13; y <= 14; y++)
    {
        for (int x = 0; x <= 3; x++)
        {
            map2[y][x] = 1;
        }
    }

    for (int y = 13; y <= 14; y++)
    {
        for (int x = 111; x < 140; x++)
        {
            map2[y][x] = 1;
        }
    }

    // 6. 미스테리 박스
    map2[2][69] = 64;
    map2[2][71] = 6;
    map2[6][75] = 64;
    map2[8][102] = 61;

    //13. 구름
    map2[8][30] = 13; map2[8][31] = 13; map2[8][32] = 13; map2[8][33] = 13;
    map2[6][34] = 13; map2[6][35] = 13; map2[6][36] = 13;
    map2[6][44] = 13; map2[6][45] = 13; map2[6][46] = 13;
    map2[7][50] = 13; map2[7][51] = 13; map2[7][52] = 13; map2[7][53] = 13;
    map2[6][55] = 13; map2[6][56] = 13;
    map2[13][48] = 13; map2[13][49] = 13; map2[13][50] = 13; map2[13][51] = 13; map2[13][52] = 13; map2[13][53] = 13; map2[13][54] = 13; map2[13][55] = 13; map2[13][56] = 13; map2[13][57] = 13;
    map2[8][95] = 13; map2[8][96] = 13; map2[8][97] = 13;
    map2[12][97] = 13; map2[12][98] = 13; map2[12][101] = 13; map2[12][101] = 13; map2[12][102] = 13; map2[12][103] = 13;
    map2[12][104] = 13;
    map2[7][63] = 13; map2[7][64] = 13; map2[7][65] = 13;
    map2[5][68] = 13; map2[5][69] = 13; map2[5][70] = 13; map2[5][71] = 13; map2[5][72] = 13;
    map2[3][77] = 13; map2[3][78] = 13; map2[3][79] = 13;
    map2[12][66] = 13; map2[12][67] = 13; map2[12][68] = 13; map2[12][69] = 13; map2[12][70] = 13; map2[12][71] = 13; map2[12][72] = 13; map2[12][73] = 13; map2[12][74] = 13; map2[12][75] = 13; map2[12][76] = 13;
    map2[5][101] = 13; map2[5][100] = 13; map2[5][99] = 13;
    map2[5][102] = 13;
    map2[5][105] = 13; map2[5][106] = 13; map2[5][107] = 13;
    map2[4][22] = 13; map2[4][23] = 13; map2[4][24] = 13;

    // 9. 버섯 머리
    map2[11][6] = 90; map2[11][7] = 91; map2[11][8] = 92;
    map2[10][8] = 90; map2[10][9] = 91; map2[10][10] = 92;
    map2[6][13] = 90; map2[6][14] = 91; map2[6][15] = 91; map2[6][16] = 91; map2[6][17] = 92;
    map2[11][17] = 90; map2[11][18] = 91; map2[11][19] = 91; map2[11][20] = 92;
    map2[8][24] = 90; map2[8][25] = 91; map2[8][26] = 92;
    map2[4][38] = 90; map2[4][39] = 91; map2[4][40] = 91; map2[4][41] = 91; map2[4][42] = 92;
    map2[10][42] = 90; map2[10][43] = 91; map2[10][44] = 92;
    map2[10][42] = 90; map2[10][43] = 91; map2[10][44] = 92;
    map2[3][58] = 90; map2[3][59] = 91; map2[3][60] = 91; map2[3][61] = 91;  map2[3][62] = 92;
    map2[6][84] = 90; map2[6][85] = 91; map2[6][86] = 92;
    map2[4][90] = 90; map2[4][91] = 91; map2[4][92] = 92;
    map2[9][79] = 90; map2[9][80] = 91; map2[9][81] = 91; map2[9][82] = 91; map2[9][83] = 92;
    map2[10][106] = 90; map2[10][107] = 91; map2[10][108] = 92;

    // 10. 버섯 줄기
    for (int y = 12; y <= 14; y++) map2[y][7] = 11;
    for (int y = 11; y <= 14; y++) map2[y][9] = 11;
    for (int y = 7; y <= 14; y++) map2[y][15] = 11;
    for (int y = 12; y <= 14; y++) map2[y][18] = 11;
    for (int y = 9; y <= 14; y++) map2[y][25] = 11;
    for (int y = 5; y <= 14; y++) map2[y][40] = 11;
    for (int y = 11; y <= 14; y++) map2[y][43] = 11;
    for (int y = 4; y <= 14; y++) map2[y][60] = 11;
    for (int y = 7; y <= 14; y++) map2[y][85] = 11;
    for (int y = 5; y <= 14; y++) map2[y][91] = 11;
    for (int y = 10; y <= 14; y++) map2[y][81] = 11;
    for (int y = 11; y <= 14; y++) map2[y][107] = 11;

    // 2. 코인
    map2[5][13] = 2;
    map2[5][14] = 2;
    map2[5][15] = 2;
    map2[5][16] = 2;
    map2[5][17] = 2;
    map2[6][51] = 2;
    map2[11][68] = 2;
    map2[11][69] = 2;
    map2[11][70] = 2;
    map2[12][48] = 2;
    map2[12][49] = 2;
    map2[12][52] = 2;
    map2[12][53] = 2;
    map2[12][54] = 2;
    map2[11][101] = 2;
    map2[11][102] = 2;
    map2[11][103] = 2;

    map2[9][53] = 63;

    // 깃발
    map2[MAP_HEIGHT - 3][115] = 5;
    // 7. 깃발
    for (int y = 3; y <= 11; y++)
    {
        map2[y][115] = 7;
    }
    // 8. 깃발 꼭짓점
    map2[2][115] = 8;

    // 9. 성
    map2[12][121] = 9;
}

void GameWorld::initMap3()
{
    // ... (existing code) ...
}

// [추가] getGameState 구현
GameState GameWorld::getGameState() const {
    return gameState;
}