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
#include <tchar.h>
#include <iostream>

// 전역 충돌 함수 구현
bool isColliding(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

GameWorld::GameWorld() {
    initMaps();
    initMonsterSpawns();
    gameState = GameState::GAME_TITLE; // 초기 상태
    cameraX = 0;
    stage = 1;
    stage_time = 400;
    m_global_animation_frame_counter = 0;
    currentMap = map1;
    victoryStart = 0;
    clearStart = 0;
    godstart = 0;
    deadStartTime = 0;
    gameover_TitleDead = false;
    gameClearText = false;
    m_currentBGM = BGM_Type::NONE;
}

GameWorld::~GameWorld() {
    // 필요한 경우 자원 해제
}

void GameWorld::init() {
    // 서버 초기화 시 필요한 로직
}

bool GameWorld::isSolidTile(int tileValue) const {
    // 0: hole, 2: coin, 7: flag, 8: flag top 등은 통과 가능
    return !(tileValue == 0 || tileValue == 2 || tileValue == 7 || tileValue == 8);
}

// ---------------------------------------------------------
// 메인 업데이트 로직
// ---------------------------------------------------------
void GameWorld::update()
{
    clearEventQueue(); // 이전 프레임 이벤트 초기화

    switch (gameState)
    {
    case GameState::GAME_TITLE:
        // 타이틀 화면 로직 (서버에서는 클라 연결 대기 등)
        break;

    case GameState::GAME_START:
        loadStage(1);
        gameState = GameState::GAME_RUNNING;
        break;

    case GameState::GAME_RUNNING:
        updatePlayers();
        updateMonsters();
        updateItems();
        updateParticles();
        checkCollisions(); // 모든 객체 간 충돌 처리
        break;

    case GameState::GAME_VICTORY:
        if (GetTickCount() - victoryStart >= 5000)
        {
            stage++;
            gameClearText = false;
            if (stage > 3) stage = 1; // 임시 루프
            loadStage(stage);
            gameState = GameState::GAME_RUNNING;
        }
        break;

    case GameState::GAME_CLEAR:
        if (GetTickCount() - clearStart >= 10000)
        {
            stage = 1;
            gameClearText = false;
            gameState = GameState::GAME_RUNNING;
        }
        break;

    case GameState::GAME_OVER:
        OutputDebugString(_T("GameWorld::update() - Case GAME_OVER\n"));
        DWORD now = GetTickCount();

        // 게임오버 타이틀 표시 후 4초 뒤 리셋 로직
        if (now - deadStartTime >= 4000)
        {
            // [수정] 모든 플레이어를 순회하며 생명력 체크 (임시 로직)
            // 실제 게임에서는 죽은 플레이어만 개별 리셋하거나 방을 폭파해야 합니다.
            bool allDead = true;
            for (auto& pair : m_players) {
                if (pair.second.getLife() > 0) {
                    allDead = false;
                    break;
                }
            }

            if (allDead) {
                // 모든 플레이어 사망 시 초기화
                // resetForDeath(-1); // -1은 전체 리셋을 의미하도록 구현 필요
                stage = 1;
                gameState = GameState::GAME_TITLE; // 혹은 GAME_RUNNING
                return;
            }

            // 개별 부활 로직은 복잡하므로, 일단 게임 상태를 실행 중으로 복구
            gameover_TitleDead = false;
            // 죽은 플레이어들만 상태 변경
            for (auto& pair : m_players) {
                if (pair.second.isDead()) {
                    pair.second.setDead(false);
                    resetForDeath(pair.first); // 해당 플레이어 리셋
                }
            }
        }
        break; // [중요] break 추가
    }

    // 글로벌 애니메이션 카운터 (필요 시)
    m_global_animation_frame_counter++;
}

// ---------------------------------------------------------
// 플레이어 및 입력 관리 (멀티플레이어 지원)
// ---------------------------------------------------------
void GameWorld::addPlayer(int playerID) {
    // 맵에 플레이어 추가 (기본 생성자 호출)
    m_players.emplace(playerID, Player(playerID));
    printf("[GameWorld] Player %d Added.\n", playerID);
}

void GameWorld::removePlayer(int playerID) {
    m_players.erase(playerID);
    printf("[GameWorld] Player %d Removed.\n", playerID);
}

Player* GameWorld::getPlayer(int playerID) {
    auto it = m_players.find(playerID);
    if (it != m_players.end()) {
        return &it->second;
    }
    return nullptr;
}

std::map<int, Player>& GameWorld::getPlayers() {
    return m_players;
}

void GameWorld::UpdatePeerState(int peerID, int x, int y, int vx, int vy, int state) {
    auto it = m_players.find(peerID);
    if (it == m_players.end()) {
        addPlayer(peerID); // 없으면 추가
        it = m_players.find(peerID);
    }
    if (it != m_players.end()) {
        Player& p = it->second;
        p.setX(x); p.setY(y);
        p.setVx(vx); p.setVy(vy);
        // p.setState((PlayerState)state); // 필요 시 상태 동기화
    }
}

void GameWorld::handleKeyDown(int playerID, WPARAM wParam) {
    Player* p = getPlayer(playerID);
    if (p && wParam < 256) {
        p->setKeyState(wParam, true);
    }
}

void GameWorld::handleKeyUp(int playerID, WPARAM wParam) {
    Player* p = getPlayer(playerID);
    if (p && wParam < 256) {
        p->setKeyState(wParam, false);
    }
}

// ---------------------------------------------------------
// 업데이트 세부 로직
// ---------------------------------------------------------
void GameWorld::updatePlayers() {
    // 모든 플레이어에 대해 업데이트 수행
    for (auto& pair : m_players) {
        pair.second.update(*this);
    }
}

void GameWorld::updateMonsters() {
    monsters.erase(std::remove_if(monsters.begin(), monsters.end(), [](const std::unique_ptr<Monster>& monster) {
        return !monster->isAlive();
        }), monsters.end());

    for (auto& monster : monsters) {
        monster->update(*this);
    }
}

void GameWorld::updateItems() {
    for (auto& item : items) {
        item->update(*this);
    }
    items.erase(std::remove_if(items.begin(), items.end(), [](const std::unique_ptr<Item>& item) {
        return !item->isActive();
        }), items.end());
}

void GameWorld::updateParticles() {
    // 새로 생성된 파티클 추가
    if (!newParticles.empty()) {
        for (auto& p : newParticles)
            particles.push_back(std::move(p));
        newParticles.clear();
    }
    // 파티클 업데이트
    for (auto& particle : particles) {
        if (particle) particle->update(*this);
    }
    // 비활성 파티클 제거
    particles.erase(std::remove_if(particles.begin(), particles.end(), [](const std::unique_ptr<Particle>& p) {
        return !p || !p->isActive();
        }), particles.end());
}

// ---------------------------------------------------------
// 충돌 처리 (모든 플레이어 대상)
// ---------------------------------------------------------
void GameWorld::checkCollisions() {
    for (auto& pair : m_players) {
        Player& player = pair.second;
        // 각 플레이어에 대해 충돌 체크 수행
        checkPlayerMapCollision(player);
        checkPlayerMonsterCollision(player);
        checkPlayerItemCollision(player);
        checkPlayerCoinCollision(player);
        checkFlagCollision(player);
        checkClearCollision(player);
    }
    // 객체 간 충돌
    checkParticleMonsterCollision();
    checkMonsterMonsterCollision();
    checkItemMapCollision();
}

void GameWorld::checkPlayerMapCollision(Player& player) {
    float playerX = player.getX();
    float playerY = player.getY();
    float playerWidth = player.getWidth();
    float playerHeight = player.getHeight();

    int leftTile = static_cast<int>(playerX / TILE_SIZE);
    int rightTile = static_cast<int>((playerX + playerWidth - 1) / TILE_SIZE);
    int topTile = static_cast<int>(playerY / TILE_SIZE);
    int bottomTile = static_cast<int>((playerY + playerHeight - 1) / TILE_SIZE);
    int middleTile = static_cast<int>((playerY + playerHeight / 2 - 1) / TILE_SIZE);

    // X축 충돌
    if (player.getVx() < 0 && (isSolidTile(currentMap[topTile][leftTile]) || isSolidTile(currentMap[middleTile][leftTile]))) {
        player.setX(leftTile * TILE_SIZE + TILE_SIZE);
        player.setVx(0);
    }
    else if (player.getVx() > 0 && (isSolidTile(currentMap[topTile][rightTile]) || isSolidTile(currentMap[middleTile][rightTile]))) {
        player.setX(rightTile * TILE_SIZE - playerWidth);
        player.setVx(0);
    }

    // 재계산
    leftTile = static_cast<int>(player.getX() / TILE_SIZE);
    rightTile = static_cast<int>((player.getX() + player.getWidth() - 1) / TILE_SIZE);

    // Y축 충돌 (바닥)
    if (player.getVy() > 0 && bottomTile < MAP_HEIGHT && (isSolidTile(currentMap[bottomTile][leftTile]) || isSolidTile(currentMap[bottomTile][rightTile]))) {
        // 용암/함정 체크
        if ((currentMap[bottomTile][leftTile] == 17 || currentMap[bottomTile][leftTile] == 18) ||
            (currentMap[bottomTile][rightTile] == 17 || currentMap[bottomTile][rightTile] == 18)) {
            player.setDead(true);
        }
        player.setY(bottomTile * TILE_SIZE - playerHeight);
        player.setVy(0);
        player.setJumping(false);
        player.setFlying(false);
    }
    // Y축 충돌 (천장)
    else if (player.getVy() < 0 && topTile >= 0 && (isSolidTile(currentMap[topTile][leftTile]) || isSolidTile(currentMap[topTile][rightTile]))) {
        int hitBlockX = static_cast<int>((player.getX() + playerWidth / 2) / TILE_SIZE);
        int hitBlockY = topTile;

        // 블럭 상호작용
        int tile = currentMap[hitBlockY][hitBlockX];
        if (tile == 6) { spawnItem(Item::ItemType::Mushroom, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE); currentMap[hitBlockY][hitBlockX] = 16; }
        else if (tile == 60 || tile == 65) { spawnItem(Item::ItemType::Star, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE); currentMap[hitBlockY][hitBlockX] = 16; }
        else if (tile == 61) { spawnItem(Item::ItemType::Flower, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE); currentMap[hitBlockY][hitBlockX] = 16; }
        else if (tile == 62) { spawnItem(Item::ItemType::Tino, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE); currentMap[hitBlockY][hitBlockX] = 16; }
        else if (tile == 63) { spawnItem(Item::ItemType::UpMushroom, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE); currentMap[hitBlockY][hitBlockX] = 16; }
        else if (tile == 64) { currentMap[hitBlockY][hitBlockX] = 16; player.setCoin(player.getCoin() + 1); pushEvent(GameEvent::GET_COIN); }

        player.setY((topTile + 1) * TILE_SIZE);
        player.setVy(0);
    }

    if (player.getX() < 0) player.setX(0);
    if (player.getY() > 800) player.setDead(true);
}

void GameWorld::checkPlayerMonsterCollision(Player& player) {
    if (player.isSuperGodMode() || player.isDead()) return;

    for (auto& monster : monsters) {
        if (!monster->isAlive() || monster->isFalling()) continue;

        if (isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(),
            monster->getX(), monster->getY(), monster->getWidth(), monster->getHeight() - TILE_SIZE / 4)) {

            if (player.isStarGodMode()) {
                monster->setVy(-15);
                monster->setFalling(true);
                pushEvent(GameEvent::KICK);
            }
            else if (player.getVy() > 0 && player.getY() + player.getHeight() - player.getVy() <= monster->getY()) {
                monster->takeDamage(*this, 1);
                player.setVy(-10);
                pushEvent(GameEvent::STOMP_ENEMY);
            }
            else {
                // 등껍질 체크
                auto* turtle = dynamic_cast<Turtle*>(monster.get());
                if (turtle && turtle->getState() == Turtle::TurtleState::SHELL) {
                    turtle->takeDamage(*this, 1);
                    pushEvent(GameEvent::KICK);
                }
                else {
                    applyplayertakedamage(player);
                }
            }
        }
    }
}

void GameWorld::checkPlayerItemCollision(Player& player) {
    if (player.isDead()) return;

    for (auto it = items.begin(); it != items.end(); ) {
        Item* item = it->get();
        // 자기 자신의 파이어볼과는 충돌하지 않음
        if (item->getType() == Item::ItemType::PlayerFireball) { ++it; continue; }

        if (isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(),
            item->getX(), item->getY(), item->getWidth(), item->getHeight())) {

            switch (item->getType()) {
            case Item::ItemType::Mushroom:
                player.setTransformStartTime(GetTickCount());
                if (!player.isBig()) player.setGameState_trans(GameState_Trans::GAME_BIG_TRANS);
                pushEvent(GameEvent::POWERUP);
                break;
            case Item::ItemType::Star:
                player.setStarGodMode(true);
                pushEvent(GameEvent::POWERUP);
                break;
            case Item::ItemType::Flower:
                player.setTransformStartTime(GetTickCount());
                player.setGameState_trans(GameState_Trans::GAME_FLOWER_TRANS);
                pushEvent(GameEvent::POWERUP);
                break;
            case Item::ItemType::Tino:
                player.setTransformStartTime(GetTickCount());
                player.setGameState_trans(GameState_Trans::GAME_TINO_TRANS);
                break;
            case Item::ItemType::UpMushroom:
                player.setLife(player.getLife() + 1);
                pushEvent(GameEvent::ONE_UP);
                break;
            }
            it = items.erase(it);
        }
        else {
            ++it;
        }
    }
}

void GameWorld::checkPlayerCoinCollision(Player& player) {
    int startX = player.getX() / TILE_SIZE;
    int startY = player.getY() / TILE_SIZE;
    int endX = (player.getX() + player.getWidth()) / TILE_SIZE;
    int endY = (player.getY() + player.getHeight()) / TILE_SIZE;

    // 플레이어 주변 타일만 검사
    for (int i = startY; i <= endY && i < MAP_HEIGHT; i++) {
        for (int j = startX; j <= endX && j < MAP_WIDTH; j++) {
            if (i >= 0 && j >= 0 && currentMap[i][j] == 2) {
                if (isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(), j * TILE_SIZE, i * TILE_SIZE, 30, 30)) {
                    currentMap[i][j] = 0;
                    player.setCoin(player.getCoin() + 1);
                    pushEvent(GameEvent::GET_COIN);
                }
            }
        }
    }
}

void GameWorld::checkFlagCollision(Player& player) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if ((currentMap[i][j] == 7 || currentMap[i][j] == 8) &&
                isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(), j * TILE_SIZE, i * TILE_SIZE, 10, 30)) {

                gameState = GameState::GAME_VICTORY;
                victoryStart = GetTickCount();
                return;
            }
        }
    }
}

void GameWorld::checkClearCollision(Player& player) {
    if (stage != 3) return;
    // 특정 좌표(성)와 충돌 체크
    if (isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(), 139 * 40, 0, 40, 15 * 40)) {
        gameState = GameState::GAME_CLEAR;
        clearStart = GetTickCount();
    }
}

void GameWorld::applyplayertakedamage(Player& player) {
    if (player.isSuperGodMode() || player.isDead()) return;

    DamageResult result = player.calculateDamageResult(1);
    player.setSuperGodMode(true); // 피격 후 무적

    switch (result) {
    case DamageResult::Shrunk:
        player.setTransformStartTime(GetTickCount());
        if (player.isFlower()) player.setGameState_trans(GameState_Trans::GAME_FLOWER_TRANS);
        else if (player.isTino()) player.setGameState_trans(GameState_Trans::GAME_TINO_TRANS);
        else player.setGameState_trans(GameState_Trans::GAME_BIG_TRANS);
        break;
    case DamageResult::Died:
        player.dead(*this);
        player.setDead(true);
        break;
    case DamageResult::NoDamage:
        break;
    }
}

void GameWorld::checkParticleMonsterCollision() {
    for (auto& particle : particles) {
        if (!particle || !particle->isActive() || particle->getType() == Particle::ParticleType::TinoFireballEffect) continue;

        for (auto& monster : monsters) {
            if (!monster->isAlive() || monster->isFalling()) continue;

            if (isColliding(particle->getX(), particle->getY(), particle->getWidth(), particle->getHeight(),
                monster->getX(), monster->getY(), monster->getWidth(), monster->getHeight())) {

                if (particle->getType() == Particle::ParticleType::PlayerFireball) {
                    monster->takeDamage(*this, 1);
                    pushEvent(GameEvent::KICK);
                    // 파이어볼 소멸 처리
                    if (auto* fireball = dynamic_cast<PlayerFireball*>(particle.get())) {
                        fireball->setFade(true);
                        fireball->setVx(0); fireball->setVy(0);
                    }
                    else {
                        particle->setActive(false);
                    }
                    break;
                }
                else if (particle->getType() == Particle::ParticleType::TinoFireball) {
                    if (!monster->isImmuneToTino()) {
                        monster->takeDamage(*this, 1);
                        monster->setHitByTino();
                        spawnTinoFireballEffect(monster->getX(), monster->getY(), 0, 0);
                        pushEvent(GameEvent::KICK);
                    }
                }
            }
        }
    }
}

void GameWorld::checkMonsterMonsterCollision() {
    for (size_t i = 0; i < monsters.size(); ++i) {
        for (size_t j = i + 1; j < monsters.size(); ++j) {
            Monster* m1 = monsters[i].get();
            Monster* m2 = monsters[j].get();

            if (!m1->isAlive() || !m2->isAlive() || m1->isFalling() || m2->isFalling()) continue;

            if (isColliding(m1->getX(), m1->getY(), m1->getWidth(), m1->getHeight(),
                m2->getX(), m2->getY(), m2->getWidth(), m2->getHeight())) {

                // 거북이 등껍질 로직
                Turtle* t1 = dynamic_cast<Turtle*>(m1);
                Turtle* t2 = dynamic_cast<Turtle*>(m2);

                if (t1 && t1->getState() == Turtle::TurtleState::SPINNING) {
                    m2->setVy(-15); m2->setFalling(true);
                    pushEvent(GameEvent::KICK);
                }
                else if (t2 && t2->getState() == Turtle::TurtleState::SPINNING) {
                    m1->setVy(-15); m1->setFalling(true);
                    pushEvent(GameEvent::KICK);
                }
            }
        }
    }
}

void GameWorld::checkItemMapCollision() {
    for (auto& item : items) {
        if (!item->isActive()) continue;
        // 정적 아이템 제외
        if (item->getType() == Item::ItemType::Flower || item->getType() == Item::ItemType::Tino || item->getType() == Item::ItemType::UpMushroom) continue;
        // 파이어볼은 자체 물리
        if (item->getType() == Item::ItemType::PlayerFireball) continue;

        // 중력
        item->setVy(item->getVy() + 1);
        if (item->getVy() > 10) item->setVy(10);
        item->setY(item->getY() + item->getVy());
        item->setX(item->getX() + item->getVx());

        int left = item->getX() / TILE_SIZE;
        int right = (item->getX() + item->getWidth() - 1) / TILE_SIZE;
        int bottom = (item->getY() + item->getHeight() - 1) / TILE_SIZE;
        int middle = (item->getY() + item->getHeight() / 2 - 1) / TILE_SIZE;

        // 벽
        if (item->getVx() < 0 && left >= 0 && isSolidTile(currentMap[middle][left])) {
            item->setX((left + 1) * TILE_SIZE);
            item->setVx(-item->getVx());
        }
        else if (item->getVx() > 0 && right < MAP_WIDTH && isSolidTile(currentMap[middle][right])) {
            item->setX(right * TILE_SIZE - item->getWidth());
            item->setVx(-item->getVx());
        }

        // 바닥
        if (item->getVy() > 0 && bottom < MAP_HEIGHT && (isSolidTile(currentMap[bottom][left]) || isSolidTile(currentMap[bottom][right]))) {
            item->setY(bottom * TILE_SIZE - item->getHeight());
            if (item->getType() == Item::ItemType::Star) item->setVy(-10); // 스타는 통통 튐
            else item->setVy(0);
        }
    }
}

// ---------------------------------------------------------
// 스테이지 및 리셋 로직
// ---------------------------------------------------------
void GameWorld::loadStage(int newStage) {
    stage = newStage;
    monster_reset();
    item_reset();
    setStage_time(400);

    if (stage == 1) currentMap = map1;
    else if (stage == 2) currentMap = map2;
    else if (stage == 3) currentMap = map3;

    spawnMonsters();

    // 플레이어들 위치 초기화
    for (auto& pair : m_players) {
        pair.second.reset();
    }
}

void GameWorld::resetForDeath(int playerID) {
    Player* p = getPlayer(playerID);
    if (p) {
        p->reset();
        p->setCoin(0);
    }
    // 게임 전체 리셋은 멀티플레이어에서 신중해야 함. 여기서는 해당 플레이어만 리셋하거나,
    // 게임 오버 시 전체 리셋 로직을 따름.
    // 일단 전체 리셋을 가정한다면:
    gameState = GameState::GAME_RUNNING;
}

void GameWorld::monster_reset() { monsters.clear(); particles.clear(); }
void GameWorld::item_reset() { items.clear(); }

// ---------------------------------------------------------
// 스폰 함수들
// ---------------------------------------------------------
void GameWorld::spawnItem(Item::ItemType type, int x, int y) {
    switch (type) {
    case Item::ItemType::Mushroom: items.push_back(std::make_unique<Mushroom>(x, y)); break;
    case Item::ItemType::Star: items.push_back(std::make_unique<Star>(x, y)); break;
    case Item::ItemType::Flower: items.push_back(std::make_unique<Flower>(x, y)); break;
    case Item::ItemType::Tino: items.push_back(std::make_unique<Tino>(x, y)); break;
    case Item::ItemType::UpMushroom: items.push_back(std::make_unique<UpMushroom>(x, y)); break;
    }
    pushEvent(GameEvent::POWERUP_APPEARS);
}

void GameWorld::spawnMonster(std::unique_ptr<Monster> monster) { monsters.push_back(std::move(monster)); }
void GameWorld::spawnParticle(std::unique_ptr<Particle> particle) { particles.push_back(std::move(particle)); }
void GameWorld::spawnPlayerFireball(int x, int y, int vx) { spawnParticle(std::make_unique<PlayerFireball>(x, y, vx)); }
void GameWorld::spawnTinoFireball(int x, int y, int vx, int direction) { spawnParticle(std::make_unique<TinoFireball>(x, y, vx, direction)); }
void GameWorld::spawnTinoFireballEffect(int x, int y, int vx, int direction) { newParticles.push_back(std::make_unique<TinoFireballEffect>(x, y, vx, direction)); }

// ---------------------------------------------------------
// 데이터 초기화 (맵, 몬스터 정보)
// ---------------------------------------------------------
void GameWorld::initMaps() { initMap1(); initMap2(); initMap3(); }

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

// 이벤트 큐
void GameWorld::pushEvent(GameEvent event) { m_eventQueue.push_back(event); }
void GameWorld::clearEventQueue() { m_eventQueue.clear(); }
const std::vector<GameEvent>& GameWorld::getEventQueue() const { return m_eventQueue; }

// Getter
GameState GameWorld::getGameState() const { return gameState; }
void GameWorld::setGameState(GameState state) { gameState = state; }
void GameWorld::setStage_time(int time) { stage_time = time; }
const int (*GameWorld::getCurrentMap() const)[MAP_WIDTH] { return currentMap; }
int GameWorld::getStage() const { return stage; }
double GameWorld::getCameraX() const { return cameraX; }
const std::vector<std::unique_ptr<Monster>>& GameWorld::getMonsters() const { return monsters; }
const std::vector<std::unique_ptr<Item>>& GameWorld::getItems() const { return items; }
const std::vector<std::unique_ptr<Particle>>& GameWorld::getParticles() const { return particles; }