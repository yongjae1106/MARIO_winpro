#include "GamePhysics.h"
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

bool isColliding(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

GameWorld::GameWorld() {
    initMaps();
    initMonsterSpawns();
    stage = 1;
    stage_time = 0;
    m_life = 5;
    m_coin = 0;
    m_currentBGM = BGM_Type::NONE;
    memset(keyState, 0, sizeof(keyState));
    m_global_animation_frame_counter = 0; // Initialize new counter
    currentMap = map1;

    deadStartTime = 0;
    victoryStart = 0;
    clearStart = 0;
    godstart = 0;
}

GameWorld::~GameWorld() {}

bool GameWorld::isSolidTile(int tileValue) const {
    return !(tileValue == 0 || tileValue == 2 || tileValue == 7 || tileValue == 8);
}

void GameWorld::update()
{
    clearEventQueue();

    switch (gameState)
    {
    case GameState::GAME_START:
        loadStage(1);
        // Players are added via network, so no reset here.
        gameState = GameState::GAME_RUNNING;
        break;
    case GameState::GAME_RUNNING:
        if (getCoin() > 99)
        {
            addLife(1);
            setCoin(0);
        }
        updatePlayers();
        updateMonsters();
        updateItems();
        updateParticles();
        checkCollisions();
        break;
    case GameState::GAME_VICTORY:
        if (GetTickCount() - victoryStart >= 5000)
        {
            stage++;
            gameClearText = false;
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
            return;
        }
        break;
    }
}

void GameWorld::handleKeyDown(WPARAM wParam)
{
    if (wParam < 256)
    {
        keyState[wParam] = true;
    }
}

void GameWorld::handleKeyUp(WPARAM wParam)
{
    if (wParam < 256) 
    {
        keyState[wParam] = false;
    }
}

void GameWorld::loadStage(int newStage) 
{
    stage = newStage;
    monsters.clear();
    items.clear();
    particles.clear();
    setStage_time(400);
    
    if (stage == 1 || stage == 2)
    {
        m_currentBGM = BGM_Type::GROUND_THEME;
    }
    else if (stage == 3)
    {
        m_currentBGM = BGM_Type::CASTLE_THEME;
    }

    if (stage == 1) currentMap = map1;
    else if (stage == 2) currentMap = map2;
    else if (stage == 3) currentMap = map3;
    
    spawnMonsters();

    for (auto& pair : m_players) {
        pair.second.reset();
    }
}

void GameWorld::spawnMonsters()
{
    const std::vector<MonsterSpawnInfo>* currentMonsterSpawns = nullptr;
    if (stage == 1) currentMonsterSpawns = &stage1Monsters;
    else if (stage == 2) currentMonsterSpawns = &stage2Monsters;
    else if (stage == 3) currentMonsterSpawns = &stage3Monsters;

    if (currentMonsterSpawns) {
        for (const auto& spawnInfo : *currentMonsterSpawns) {
            float x = spawnInfo.x * TILE_SIZE;
            float y = spawnInfo.y * TILE_SIZE;
            switch (spawnInfo.type) {
            case Monster::MonsterType::NormalGoomba: monsters.push_back(std::make_unique<NormalGoomba>(x, y)); break;
            case Monster::MonsterType::RedGoomba: monsters.push_back(std::make_unique<RedGoomba>(x, y)); break;
            case Monster::MonsterType::BlueGoomba: monsters.push_back(std::make_unique<BlueGoomba>(x, y)); break;
            case Monster::MonsterType::GreenTurtle: monsters.push_back(std::make_unique<GreenTurtle>(x, y)); break;
            case Monster::MonsterType::BrownTurtle: monsters.push_back(std::make_unique<BrownTurtle>(x, y)); break;
            case Monster::MonsterType::AngelTurtle: monsters.push_back(std::make_unique<AngelTurtle>(x, y)); break;
            case Monster::MonsterType::Bowser: monsters.push_back(std::make_unique<Bowser>(x, y)); break;
            }
        }
    }
}

// --- Player Management ---
void GameWorld::addPlayer(int playerID) {
    m_players.emplace(playerID, Player(playerID));
}

void GameWorld::removePlayer(int playerID) {
    m_players.erase(playerID);
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
// -------------------------

const int(*GameWorld::getCurrentMap() const)[MAP_WIDTH] {
    return currentMap;
}

int GameWorld::getStage() const {
    return stage;
}

double GameWorld::getCameraX() const {
    return cameraX;
}

const std::vector<std::unique_ptr<Monster>>& GameWorld::getMonsters() const {
    return monsters;
}

const std::vector<std::unique_ptr<Item>>& GameWorld::getItems() const {
    return items;
}

const std::vector<std::unique_ptr<Particle>>& GameWorld::getParticles() const {
    return particles;
}

const std::vector<std::unique_ptr<Particle>>& GameWorld::getNewParticles() const {
    return newParticles;
}

const bool* GameWorld::getKeyState() const {
    return keyState;
}

GameState GameWorld::getGameState() const {
    return gameState;
}

void GameWorld::setGameState(GameState state) {
    gameState = state;
}

void GameWorld::setStage_time(int time) {
    stage_time = time;
}

// --- Update Functions ---
void GameWorld::updatePlayers() {
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
    if (!newParticles.empty()) {
        for (auto& p : newParticles)
            particles.push_back(std::move(p));
        newParticles.clear();
    }
    for (auto& particle : particles) {
        if (particle) particle->update(*this);
    }
    particles.erase(std::remove_if(particles.begin(), particles.end(), [](const std::unique_ptr<Particle>& p) {
        return !p || !p->isActive();
    }), particles.end());
}

// --- Collision Detection ---
void GameWorld::checkCollisions() {
    for (auto& pair : m_players) {
        Player& player = pair.second;
        checkPlayerMapCollision(player);
        checkPlayerMonsterCollision(player);
        checkPlayerItemCollision(player);
        checkPlayerCoinCollision(player);
        checkFlagCollision(player);
        checkClearCollision(player);
    }
    checkParticleMonsterCollision();
    checkMonsterMonsterCollision();
    checkItemMapCollision();
}

void GameWorld::resurrection(int playerID) {
    Player* player = getPlayer(playerID);
    if (!player) return;

    player->setX(140);
    player->setY(300);
    player->setVx(0);
    player->setVy(0);
    player->setDead(false);
    player->setGameOver(false);
    player->setState(PlayerState::Small);
    player->setSuperGodMode(false);
    player->setStarGodMode(false);
    gameState = GameState::GAME_RUNNING;
}

void GameWorld::monster_reset() {
    monsters.clear();
    particles.clear();
}

void GameWorld::item_reset() {
    items.clear();
}

void GameWorld::resetForDeath(int playerID) {
    Player* player = getPlayer(playerID);
    if (!player) return;
    
    player->reset();
    setCoin(0); // This might need to be player-specific later
    gameClearText = false;
    gameState = GameState::GAME_RUNNING;
    // setStageBGM(); // This is handled in loadStage
}

void GameWorld::spawnItem(Item::ItemType type, int x, int y) {
    switch (type) {
    case Item::ItemType::Mushroom: items.push_back(std::make_unique<Mushroom>(x, y)); break;
    case Item::ItemType::Star: items.push_back(std::make_unique<Star>(x, y)); break;
    case Item::ItemType::Flower: items.push_back(std::make_unique<Flower>(x, y)); break;
    case Item::ItemType::Tino: items.push_back(std::make_unique<Tino>(x, y)); break;
    case Item::ItemType::UpMushroom: items.push_back(std::make_unique<UpMushroom>(x, y)); break;
    default: pushEvent(GameEvent::POWERUP_APPEARS); break;
    }
}

void GameWorld::spawnMonster(std::unique_ptr<Monster> monster) {
    monsters.push_back(std::move(monster));
}

void GameWorld::spawnParticle(std::unique_ptr<Particle> particle) {
    particles.push_back(std::move(particle));
}

void GameWorld::spawnPlayerFireball(int x, int y, int vx) {
    spawnParticle(std::make_unique<PlayerFireball>(x, y, vx));
}

void GameWorld::spawnTinoFireball(int x, int y, int vx, int direction) {
    spawnParticle(std::make_unique<TinoFireball>(x, y, vx, direction));
}

void GameWorld::spawnTinoFireballEffect(int x, int y, int vx, int direction) {
    newParticles.push_back(std::move(std::make_unique<TinoFireballEffect>(x, y, vx, direction)));
}

void GameWorld::checkParticleMonsterCollision() {
    for (auto& particle : particles) {
        if (!particle || !particle->isActive() || particle->getType() == Particle::ParticleType::TinoFireballEffect) continue;
        for (auto& monster : monsters) {
            if (!monster->isAlive() || monster->isFalling()) continue;
            if (isColliding(particle->getX(), particle->getY(), particle->getWidth(), particle->getHeight(), monster->getX(), monster->getY(), monster->getWidth(), monster->getHeight())) {
                if (particle->getType() == Particle::ParticleType::PlayerFireball) {
                    pushEvent(GameEvent::KICK);
                    monster->takeDamage(*this, 1);
                    if (auto* fireball = dynamic_cast<PlayerFireball*>(particle.get())) {
                        fireball->setFade(true);
                        fireball->setVx(0);
                        fireball->setVy(0);
                    } else {
                        particle->setActive(false);
                    }
                    break;
                } else if (particle->getType() == Particle::ParticleType::TinoFireball) {
                    if (!monster->isImmuneToTino()) {
                        pushEvent(GameEvent::KICK);
                        spawnTinoFireballEffect(monster->getX(), monster->getY(), 0, 0);
                        monster->takeDamage(*this, 1);
                        monster->setHitByTino();
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
            if (isColliding(m1->getX(), m1->getY(), m1->getWidth(), m1->getHeight(), m2->getX(), m2->getY(), m2->getWidth(), m2->getHeight())) {
                Turtle* t1 = dynamic_cast<Turtle*>(m1);
                Turtle* t2 = dynamic_cast<Turtle*>(m2);
                if (t1 && t1->getState() == Turtle::TurtleState::SPINNING) {
                    pushEvent(GameEvent::KICK);
                    m2->setVy(-15);
                    m2->setFalling(true);
                } else if (t2 && t2->getState() == Turtle::TurtleState::SPINNING) {
                    pushEvent(GameEvent::KICK);
                    m1->setVy(-15);
                    m1->setFalling(true);
                }
            }
        }
    }
}

void GameWorld::checkPlayerMonsterCollision(Player& player) {
    if (player.isSuperGodMode() || player.isDead()) return;
    for (auto& monster : monsters) {
        if (!monster->isAlive() || monster->isFalling()) continue;
        if (isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(), monster->getX(), monster->getY(), monster->getWidth(), monster->getHeight() - TILE_SIZE / 4)) {
            if (player.isStarGodMode()) {
                pushEvent(GameEvent::KICK);
                monster->setVy(-15);
                monster->setFalling(true);
            } else if (player.getVy() > 0 && player.getY() + player.getHeight() - player.getVy() <= monster->getY()) {
                monster->takeDamage(*this, 1);
                player.setVy(-10);
                pushEvent(GameEvent::STOMP_ENEMY);
            } else {
                if (auto* turtle = dynamic_cast<Turtle*>(monster.get())) {
                    if (turtle->getState() == Turtle::TurtleState::SHELL) {
                        turtle->takeDamage(*this, 1);
                        pushEvent(GameEvent::KICK);
                        return;
                    }
                }
                applyplayertakedamage(player);
            }
        }
    }
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

    if (!(player.getY() < 0) && player.getVx() < 0 && (isSolidTile(currentMap[topTile][leftTile]) || isSolidTile(currentMap[middleTile][leftTile]))) {
        player.setX(leftTile * TILE_SIZE + TILE_SIZE);
        player.setVx(0);
    } else if (!(player.getY() < 0) && player.getVx() > 0 && (isSolidTile(currentMap[topTile][rightTile]) || isSolidTile(currentMap[middleTile][rightTile]))) {
        player.setX(rightTile * TILE_SIZE - playerWidth);
        player.setVx(0);
    }

    leftTile = static_cast<int>(player.getX() / TILE_SIZE);
    rightTile = static_cast<int>((player.getX() + player.getWidth() - 1) / TILE_SIZE);
    topTile = static_cast<int>(player.getY() / TILE_SIZE);
    bottomTile = static_cast<int>((player.getY() + player.getHeight() - 1) / TILE_SIZE);

    if (!(player.getY() < 0) && player.getVy() > 0 && bottomTile < MAP_HEIGHT && bottomTile >= 0 && (isSolidTile(currentMap[bottomTile][leftTile]) || isSolidTile(currentMap[bottomTile][rightTile]))) {
        if ((currentMap[bottomTile][leftTile] == 17 || currentMap[bottomTile][rightTile] == 17) || (currentMap[bottomTile][leftTile] == 18 || currentMap[bottomTile][rightTile] == 18)) {
            player.setDead(true);
        }
        player.setY(bottomTile * TILE_SIZE - playerHeight);
        player.setVy(0);
        player.setJumping(false);
        player.setFlying(false);
    } else if (!(player.getY() < 0) && player.getVy() < 0 && topTile >= 0 && topTile < MAP_HEIGHT && (isSolidTile(currentMap[topTile][leftTile]) || isSolidTile(currentMap[topTile][rightTile]))) {
        int hitBlockX = static_cast<int>((player.getX() + playerWidth / 2) / TILE_SIZE);
        int hitBlockY = topTile;
        if (currentMap[hitBlockY][hitBlockX] == 6) { pushEvent(GameEvent::POWERUP_APPEARS); spawnItem(Item::ItemType::Mushroom, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE); currentMap[hitBlockY][hitBlockX] = 16; }
        else if (currentMap[hitBlockY][hitBlockX] == 65) { pushEvent(GameEvent::POWERUP_APPEARS); spawnItem(Item::ItemType::Star, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE); currentMap[hitBlockY][hitBlockX] = 16; }
        else if (currentMap[hitBlockY][hitBlockX] == 60) { pushEvent(GameEvent::POWERUP_APPEARS); spawnItem(Item::ItemType::Star, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE); currentMap[hitBlockY][hitBlockX] = 16; }
        else if (currentMap[hitBlockY][hitBlockX] == 61) { pushEvent(GameEvent::POWERUP_APPEARS); spawnItem(Item::ItemType::Flower, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE); currentMap[hitBlockY][hitBlockX] = 16; }
        else if (currentMap[hitBlockY][hitBlockX] == 62) { pushEvent(GameEvent::POWERUP_APPEARS); spawnItem(Item::ItemType::Tino, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE); currentMap[hitBlockY][hitBlockX] = 16; }
        else if (currentMap[hitBlockY][hitBlockX] == 63) { pushEvent(GameEvent::POWERUP_APPEARS); spawnItem(Item::ItemType::UpMushroom, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE); currentMap[hitBlockY][hitBlockX] = 16; }
        else if (currentMap[hitBlockY][hitBlockX] == 64) { pushEvent(GameEvent::GET_COIN); currentMap[hitBlockY][hitBlockX] = 16; addCoin(1); }
        player.setY((topTile + 1) * TILE_SIZE);
        player.setVy(0);
    }

    if (player.getX() < 0) player.setX(0);
    if (player.getY() > 800) player.setDead(true);
}

void GameWorld::checkPlayerItemCollision(Player& player) {
    if (player.isDead()) return;
    for (auto it = items.begin(); it != items.end(); ) {
        Item* item = it->get();
        if (item->getType() == Item::ItemType::PlayerFireball) { ++it; continue; }
        if (isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(), item->getX(), item->getY(), item->getWidth(), item->getHeight())) {
            switch (item->getType()) 
            {
            case Item::ItemType::Mushroom: 
                pushEvent(GameEvent::POWERUP); 
                player.setTransformStartTime(GetTickCount()); 
                if (!player.isBig()) 
                    player.setGameState_trans(GameState_Trans::GAME_BIG_TRANS); 
                break;
            case Item::ItemType::Star: 
                pushEvent(GameEvent::POWERUP); 
                player.setStarGodMode(true); 
                break;
            case Item::ItemType::Flower: 
                pushEvent(GameEvent::POWERUP); 
                player.setTransformStartTime(GetTickCount()); 
                player.setGameState_trans(GameState_Trans::GAME_FLOWER_TRANS); 
                break;
            case Item::ItemType::Tino: 
                player.setTransformStartTime(GetTickCount()); 
                player.setGameState_trans(GameState_Trans::GAME_TINO_TRANS); 
                break;
            case Item::ItemType::UpMushroom: 
                pushEvent(GameEvent::ONE_UP); 
                addLife(1); 
                break;
            }
            it = items.erase(it);
        } 
        else 
        {
            ++it;
        }
    }
}

void GameWorld::checkFlagCollision(Player& player) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if ((currentMap[i][j] == 7 || currentMap[i][j] == 8) && isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(), j * 40, i * 40, 10, 30)) {
                m_currentBGM = BGM_Type::VICTORY;
                gameState = GameState::GAME_VICTORY;
                victoryStart = GetTickCount();
                return;
            }
        }
    }
}

void GameWorld::checkClearCollision(Player& player) {
    if (stage != 3) return;
    if (isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(), 139 * 40, 0, 40, 15 * 40)) {
        m_currentBGM = BGM_Type::WORLD_CLEAR;
        gameState = GameState::GAME_CLEAR;
        clearStart = GetTickCount();
    }
}

void GameWorld::checkItemMapCollision() {
    for (auto& item : items) {
        if (!item->isActive() || item->getType() == Item::ItemType::PlayerFireball || item->getType() == Item::ItemType::Flower || item->getType() == Item::ItemType::Tino || item->getType() == Item::ItemType::UpMushroom) continue;
        item->setVy(item->getVy() + 1);
        if (item->getVy() > 10) item->setVy(10);
        item->setY(item->getY() + item->getVy());
        item->setX(item->getX() + item->getVx());

        int leftTile = static_cast<int>(item->getX() / TILE_SIZE);
        int rightTile = static_cast<int>((item->getX() + item->getWidth() - 1) / TILE_SIZE);
        int middleTile = static_cast<int>((item->getY() + item->getHeight() / 2 - 1) / TILE_SIZE);
        int bottomTile = static_cast<int>((item->getY() + item->getHeight() - 1) / TILE_SIZE);

        if (item->getVx() < 0 && leftTile >= 0 && isSolidTile(currentMap[middleTile][leftTile])) {
            item->setX((leftTile + 1) * TILE_SIZE);
            item->setVx(-item->getVx());
        } else if (item->getVx() > 0 && rightTile < MAP_WIDTH && isSolidTile(currentMap[middleTile][rightTile])) {
            item->setX(rightTile * TILE_SIZE - item->getWidth());
            item->setVx(-item->getVx());
        }

        if (item->getVy() > 0 && bottomTile < MAP_HEIGHT && (isSolidTile(currentMap[bottomTile][leftTile]) || isSolidTile(currentMap[bottomTile][rightTile]))) {
            item->setY(bottomTile * TILE_SIZE - item->getHeight());
            item->setVy(item->getType() == Item::ItemType::Star ? -10 : 0);
        }
    }
}

void GameWorld::checkPlayerCoinCollision(Player& player) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (currentMap[i][j] == 2 && isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(), j * TILE_SIZE, i * TILE_SIZE, 30, 30)) {
                pushEvent(GameEvent::GET_COIN);
                currentMap[i][j] = 0;
                addCoin(1);
            }
        }
    }
}

void GameWorld::applyplayertakedamage(Player& player) {
    // Implement damage logic
}

// --- Map and Monster Initialization ---
void GameWorld::initMaps() { initMap1(); initMap2(); initMap3(); }
void GameWorld::initMonsterSpawns() { /* ... monster spawn data ... */ }
void GameWorld::initMap1() { /* ... map data ... */ }
void GameWorld::initMap2() { /* ... map data ... */ }
void GameWorld::initMap3() { /* ... map data ... */ }

// BGM & Event System Implementations
BGM_Type GameWorld::getCurrentBGM() const { return m_currentBGM; }
const std::vector<GameEvent>& GameWorld::getEventQueue() const { return m_eventQueue; }
void GameWorld::pushEvent(GameEvent event) { m_eventQueue.push_back(event); }
void GameWorld::clearEventQueue() { m_eventQueue.clear(); }