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
#include "projectiles/Fireball.h"
#include "projectiles/PlayerFireball.h"
#include <memory>
#include <tchar.h>

bool isColliding(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

bool isSolidTile(int tileID) {
    return tileID != 0 && tileID != 2 && tileID != 7 && tileID != 8;
}

GameWorld::GameWorld() {
    initMaps();
    initMonsterSpawns();
    gameState = GameState::GAME_TITLE;
    gameState_trans = GameState_Trans::GAME_NONE;
    cameraX = 0;
    stage = 1;
    stage_time = 0;
    gameclear_text = false;
    memset(keyState, 0, sizeof(keyState));
    m_global_animation_frame_counter = 0; // Initialize new counter
    currentMap = map1;
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

void GameWorld::update() 
{
    switch (gameState) 
    {
    case GameState::GAME_START:
    {
        loadStage(1);
        player.reset(); // Reset player state for new game
        gameState = GameState::GAME_RUNNING;
        break;
    }
    case GameState::GAME_RUNNING:
    {
        switch (gameState_trans)
        {
            case GameState_Trans::GAME_NONE:
            {
                updatePlayer();
                break; // No special action when not transforming
            }
            // mushroom 변신 모션
            case GameState_Trans::GAME_BIG_TRANS:
            {
                DWORD now = GetTickCount();
                if (now - transformStartTime >= 1500)
                {
                    if (!player.isBig())
                    {
                        player.grow();
                        gameState_trans = GameState_Trans::GAME_NONE;
                    }
                }
                break;
            }
            // flower 변신 모션
            case GameState_Trans::GAME_FLOWER_TRANS:
            {
                DWORD now = GetTickCount();
                if (now - transformStartTime >= 1500)
                {
                    if (!player.isFlower())
                    {
                        player.gainFlower();
                        gameState_trans = GameState_Trans::GAME_NONE;
                    }
                }
                break;
            }
            // tino 변신 모션
            case GameState_Trans::GAME_TINO_TRANS:
            {
                DWORD now = GetTickCount();
                if (now - transformStartTime >= 1500)
                {
                    if (!player.isTino())
                    {
                        player.gainTino();
                        gameState_trans = GameState_Trans::GAME_NONE;
                    }
                }
                break;
            }
        }

        if (player.getCoin() > 99)
        {
            playSound("1-up");
            player.setLife(player.getLife() + 1);
            player.setCoin(0);
        }

        // 변신중일때 정지
        if(gameState_trans != GameState_Trans::GAME_NONE) 
        {
            player.setStop();
        }

        updateMonsters(); // Always update monsters
        updateItems();    // Always update items
        checkCollisions(); // Always check collisions

        // Camera update only if not transforming
        if (gameState_trans == GameState_Trans::GAME_NONE) 
        {
            // 카메라 업데이트
            // 플레이어의 화면 X 위치 계산
            int playerX = player.getX();

            // 플레이어가 화면 중앙을 넘어 오른쪽으로 이동하는 경우
            if (playerX > SCREEN_WIDTH / 2)
            {
                cameraX += player.getVx();
                player.setX(SCREEN_WIDTH / 2);
            }
            // 플레이어가 화면 중앙을 넘어 왼쪽으로 이동하는 경우 (cameraX가 0보다 큰 경우에만)
            else if (playerX < SCREEN_WIDTH / 2 && cameraX > 0)
            {
                cameraX += player.getVx();
                if (cameraX < 0) cameraX = 0; // 카메라가 왼쪽 경계를 넘어가지 않도록 보장
                player.setX(SCREEN_WIDTH / 2);
            }

            // cameraX를 맵 경계 내로 제한
            if (cameraX < 0) cameraX = 0;
            if (cameraX > MAP_WIDTH * TILE_SIZE - SCREEN_WIDTH)
                cameraX = MAP_WIDTH * TILE_SIZE - SCREEN_WIDTH;
        }        default:
            break;
    }
    // 승리 모션
    case GameState::GAME_VICTORY:
    {
        DWORD now = GetTickCount();

        if (now - victoryStart >= 5000)
        {
            stage++;
            loadStage(stage);
            gameState = GameState::GAME_RUNNING;
        }
        break;
    }
    // 최종 승리 모션
    case GameState::GAME_CLEAR:
    {

        if (GetTickCount() - clearStart >= 10000)
        {
            stage = 1;
            gameclear_text = false;
            gameStarted = false;

            gameState = GameState::GAME_RUNNING;
            return;
        }
        break;
    }
    // 시망모션
    case GameState::GAME_OVER:
    {
        DWORD now = GetTickCount();
        if (player.isDead())
        {
            static bool motion1;

            if (now - deadStartTime >= 2000)
            {
                if (player.getLife() <= 0)
                {
                    stage = 1;
                    gameclear_text = false;
                    gameStarted = false;

                    gameState = GameState::GAME_RUNNING;
                    return;
                }
                player.setDead(false);
                motion1 = false;
            }
            return;
        }


        return;
    }

    }
}

void GameWorld::render(HDC hdc) {
    m_gameRender.render(hdc, *this);
}

// player.cpp의 move함수에서 역할을 수행함

void GameWorld::handleKeyDown(WPARAM wParam) 
{
    if (wParam < 256) 
    {
        keyState[wParam] = true;
        // 디버그: 키 다운 이벤트 및 현재 키 상태 출력
        TCHAR debugMessage[256];
        _stprintf_s(debugMessage, _T("KeyDown: %d (keyState[VK_LEFT]: %d, keyState[VK_RIGHT]: %d)\n"), wParam, keyState[VK_LEFT], keyState[VK_RIGHT]);
        OutputDebugString(debugMessage);
    }

    if (gameState == GameState::GAME_TITLE)
    {
        switch (wParam)
        {
        case VK_UP:
        {
            if (title_select == 1)
            {
                title_select = 0;
            }
            break;
        }
        case VK_DOWN:
        {
            if (title_select == 0)
            {
                title_select = 1;
            }
            break;
        }
        case VK_RETURN:
        {
            if (title_select == 0)
            {
                gameState = GameState::GAME_START;
            }
            else
            {
                exit(1);
            }
        }
        }
    }

}

void GameWorld::handleKeyUp(WPARAM wParam) 
{
    if (wParam < 256) {
        // 디버그: 키 업 이벤트 및 keyState[wParam] 변경 전 상태 출력
        TCHAR debugMessageBefore[256];
        _stprintf_s(debugMessageBefore, _T("KeyUp: %d (Before: keyState[%d]=%d, keyState[VK_LEFT]: %d, keyState[VK_RIGHT]: %d)\n"), wParam, wParam, keyState[wParam], keyState[VK_LEFT], keyState[VK_RIGHT]);
        OutputDebugString(debugMessageBefore);

        keyState[wParam] = false; // This line should set keyState[VK_RIGHT] to false

        // 디버그: 키 업 이벤트 및 keyState[wParam] 변경 후 상태 출력
        TCHAR debugMessageAfter[256];
        _stprintf_s(debugMessageAfter, _T("KeyUp: %d (After: keyState[%d]=%d, keyState[VK_LEFT]: %d, keyState[VK_RIGHT]: %d)\n"), wParam, wParam, keyState[wParam], keyState[VK_LEFT], keyState[VK_RIGHT]);
        OutputDebugString(debugMessageAfter);
    }
}

void GameWorld::loadStage(int newStage) {
    stage = newStage;
    monsters.clear();
    setStage_time(400);
    setStageBGM();
    if (stage == 1) {
        currentMap = map1;
    } else if (stage == 2) {
        currentMap = map2;
    } else if (stage == 3) {
        currentMap = map3;
    }
    spawnMonsters();
}

void GameWorld::spawnMonsters()
{
    const std::vector<MonsterSpawnInfo>* currentMonsterSpawns = nullptr;
    if (stage == 1) {
        currentMonsterSpawns = &stage1Monsters;
    } else if (stage == 2) {
        currentMonsterSpawns = &stage2Monsters;
    } else if (stage == 3) {
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

const Player& GameWorld::getPlayer() const {
    return player;
}

Player& GameWorld::getPlayer() {
    return player;
}

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

const bool* GameWorld::getKeyState() const {
    return keyState;
}

GameState GameWorld::getGameState() const {
    return gameState;
}

GameState_Trans GameWorld::getGameState_trans() const {
    return gameState_trans;
}

void GameWorld::setGameState(GameState state) {
    gameState = state;
}
void GameWorld::setGameState_trans(GameState_Trans state_trans) {
    gameState_trans = state_trans;
}
void GameWorld::setStage_time(int time) {
    stage_time = time;
}
void GameWorld::setdeadStartTime(int time)
{
    deadStartTime = time;
}
void GameWorld::updatePlayer() {
    player.update(*this);
}

void GameWorld::updateAnimations() {
    player.updateAnimation();
    m_global_animation_frame_counter++; // Increment global animation frame counter
}

void GameWorld::updateMonsters() {
    // Remove dead monsters
    monsters.erase(std::remove_if(monsters.begin(), monsters.end(), [](const std::unique_ptr<Monster>& monster) {
        return !monster->isAlive();
    }),
    monsters.end());

    // Update remaining monsters
    for (auto& monster : monsters) {
        monster->update(*this);
    }
}

void GameWorld::updateItems() {
    for (auto& item : items) {
        item->update(*this);
    }
}

void GameWorld::checkCollisions() {
    checkMonsterMapCollision();
    checkPlayerMapCollision();
    checkPlayerMonsterCollision();
    checkPlayerItemCollision();
    checkItemMapCollision();
    checkFlagCollision();
    checkClearCollision();
    checkPlayerCoinCollision();
}


// ... other collision methods ...

void GameWorld::dead() {
    stopAllSounds();
    player.setDead(true);
    gameState = GameState::GAME_OVER;
    deadStartTime = GetTickCount();
}

void GameWorld::resurrection() {
    player.setX(100);
    player.setY(300);
    player.setVx(0);
    player.setVy(0);
    player.setDead(false);
    player.setGameOver(false);
    player.setLife(player.getLife() - 1);
    player.setState(PlayerState::Small);
    player.setSuperGodMode(true);
    player.setStarGodMode(false);
    gameState = GameState::GAME_RUNNING;
}

void GameWorld::monster_reset() {
    monsters.clear();
}

void GameWorld::item_reset() {
    items.clear();
}

void GameWorld::spawnItem(Item::ItemType type, int x, int y) {
    switch (type) {
        case Item::ItemType::Mushroom:
            items.push_back(std::make_unique<Mushroom>(x, y));
            break;
        case Item::ItemType::Star:
            items.push_back(std::make_unique<Star>(x, y));
            break;
        case Item::ItemType::Flower:
            items.push_back(std::make_unique<Flower>(x, y));
            break;
        case Item::ItemType::Tino:
            items.push_back(std::make_unique<Tino>(x, y));
            break;
        case Item::ItemType::UpMushroom:
            items.push_back(std::make_unique<UpMushroom>(x, y));
            break;
        default:
            // Handle unknown item type or log an error
            break;
    }
}

void GameWorld::spawnMonster(std::unique_ptr<Monster> monster) {
    monsters.push_back(std::move(monster));
}

void GameWorld::spawnFireball(int x, int y, int vx) {
    items.push_back(std::make_unique<Fireball>(x, y, vx));
}

void GameWorld::spawnPlayerFireball(int x, int y, int vx) {
    items.push_back(std::make_unique<PlayerFireball>(x, y, vx));
}

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ충돌ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

void GameWorld::checkMonsterMapCollision() 
{
    for (auto& monster : monsters) {
        if (!monster->isAlive() || monster->isFalling()) continue; // Skip dead monsters

        // Apply gravity
        monster->setVy(monster->getVy() + 1);
        if (monster->getVy() > 10) monster->setVy(10);
        monster->setY(monster->getY() + monster->getVy());

        // Horizontal movement
        monster->setX(monster->getX() + monster->getVx());

        // Calculate tile coordinates for monster's bounding box
        int leftTile = static_cast<int>(monster->getX() / TILE_SIZE);
        int rightTile = static_cast<int>((monster->getX() + monster->getWidth() - 1) / TILE_SIZE);
        int topTile = static_cast<int>(monster->getY() / TILE_SIZE);
        int bottomTile = static_cast<int>((monster->getY() + monster->getHeight() - 1) / TILE_SIZE);
        int middleTile = static_cast<int>((monster->getY() + monster->getHeight() / 2 - 1) / TILE_SIZE);

        // Check for horizontal collision with walls
        if (monster->getVx() < 0) { // Moving left
            if (leftTile >= 0 && leftTile < MAP_WIDTH &&
                (isSolidTile(currentMap[topTile][leftTile]) || isSolidTile(currentMap[middleTile][leftTile]))) {
                monster->setX((leftTile + 1) * TILE_SIZE);
                monster->setVx(-monster->getVx());
            }
        } else if (monster->getVx() > 0) { // Moving right
            if (rightTile < MAP_WIDTH && rightTile >= 0 &&
                (isSolidTile(currentMap[topTile][rightTile]) || isSolidTile(currentMap[middleTile][rightTile]))) {
                monster->setX(rightTile * TILE_SIZE - monster->getWidth());
                monster->setVx(-monster->getVx());
            }
        }

        // Check for vertical collision (ground)
        if (monster->getVy() > 0 && bottomTile < MAP_HEIGHT && bottomTile >= 0 &&
            (isSolidTile(currentMap[bottomTile][leftTile]) || isSolidTile(currentMap[bottomTile][rightTile]))) {
            monster->setY(bottomTile * TILE_SIZE - monster->getHeight());
            monster->setVy(0);
        }
    }
}

void GameWorld::checkPlayerMonsterCollision() 
{
    if (player.isSuperGodMode()) return; // Player is invincible

    for (auto& monster : monsters) 
    {
        if (!monster->isAlive() || monster->isFalling()) continue; // Skip dead monsters

        if (isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(),
                        monster->getX() - cameraX, monster->getY(), monster->getWidth(), monster->getHeight())) 
        {
            if (player.isStarGodMode()) 
            { // Player is in Star mode
                playSound("kick");
                monster->setVy(-15); // Make monster fly upwards
                monster->setFalling(true);
            }
            // 밟았을 때
            else if (player.getVy() > 0 && player.getY() + player.getHeight() - player.getVy() <= monster->getY()) 
            {
                monster->takeDamage(*this, 1);
                player.setVy(-10); // Player bounces up
                playSound("stomp");
            } 
            else 
            { // Player collides with monster from side or bottom
                DamageResult result = player.calculateDamageResult(1);
                player.applyDamageResult(result);
                if (result == DamageResult::Died) {
                    dead(); // Call GameWorld's dead()
                }
            }
        }
    }
}

void GameWorld::checkPlayerMapCollision() {
    // Player position and dimensions
    float playerX = player.getX();
    float playerY = player.getY();
    float playerWidth = player.getWidth();
    float playerHeight = player.getHeight();

    // Calculate tile coordinates for player's bounding box
    int leftTile = static_cast<int>((playerX + cameraX) / TILE_SIZE);
    int rightTile = static_cast<int>((playerX + playerWidth - 1 + cameraX) / TILE_SIZE);
    int topTile = static_cast<int>(playerY / TILE_SIZE);
    int bottomTile = static_cast<int>((playerY + playerHeight - 1) / TILE_SIZE);
    int middleTile = static_cast<int>((playerY + playerHeight / 2 - 1) / TILE_SIZE);

    // Horizontal collision (left wall)
    if (!(player.getY() < 0) && player.getVx() < 0 &&
        (isSolidTile(currentMap[topTile][leftTile]) || isSolidTile(currentMap[middleTile][leftTile]))) {
        player.setX(leftTile * TILE_SIZE - cameraX + TILE_SIZE);
        player.setVx(0); // 벽과 충돌 시 수평 속도를 0으로 설정
    }
    // Horizontal collision (right wall)
    else if (!(player.getY() < 0) && player.getVx() > 0 && 
        (isSolidTile(currentMap[topTile][rightTile]) || isSolidTile(currentMap[middleTile][rightTile]))) {
        player.setX(rightTile * TILE_SIZE - cameraX - playerWidth);
        player.setVx(0); // 벽과 충돌 시 수평 속도를 0으로 설정
    }

    // 재정의
    leftTile = static_cast<int>((playerX + cameraX) / TILE_SIZE);
    rightTile = static_cast<int>((playerX + playerWidth - 1 + cameraX) / TILE_SIZE);
    topTile = static_cast<int>(playerY / TILE_SIZE);
    bottomTile = static_cast<int>((playerY + playerHeight - 1) / TILE_SIZE);
    middleTile = static_cast<int>((playerY + playerHeight / 2 - 1) / TILE_SIZE);

    // Vertical collision (bottom - ground)
    if (!(player.getY() < 0) && player.getVy() > 0 && bottomTile < MAP_HEIGHT && bottomTile >= 0 &&
        (isSolidTile(currentMap[bottomTile][leftTile]) || isSolidTile(currentMap[bottomTile][rightTile]))) {
        if ((currentMap[bottomTile][leftTile] == 17 || currentMap[bottomTile][rightTile] == 17) || 
            (currentMap[bottomTile][leftTile] == 18 || currentMap[bottomTile][rightTile] == 18)) {
            player.setDead(true); // Assuming player has a dead() method
        }

        player.setY(bottomTile * TILE_SIZE - playerHeight);
        player.setVy(0);
        player.setJumping(false);
        player.setFlying(false);
    }
    // Vertical collision (top - hitting block from below)
    else if (!(player.getY() < 0) && player.getVy() < 0 && topTile >= 0 && topTile < MAP_HEIGHT &&
             (isSolidTile(currentMap[topTile][leftTile]) || isSolidTile(currentMap[topTile][rightTile]))) {
        
        int hitBlockX = static_cast<int>((playerX + playerWidth / 2 + cameraX) / TILE_SIZE);
        int hitBlockY = topTile;

        if (currentMap[hitBlockY][hitBlockX] == 6) { // mushroom Box
            playSound("powerup_appears");
            spawnItem(Item::ItemType::Mushroom, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE);
            currentMap[hitBlockY][hitBlockX] = 16;
        }
        // Add other item block types (60-65) here as needed
        else if (currentMap[hitBlockY][hitBlockX] == 65) { // Star Box
            playSound("powerup_appears");
            spawnItem(Item::ItemType::Star, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE);
            currentMap[hitBlockY][hitBlockX] = 16;
        }
        else if (currentMap[hitBlockY][hitBlockX] == 60) { // Star Box
            playSound("powerup_appears");
            spawnItem(Item::ItemType::Star, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE);
            currentMap[hitBlockY][hitBlockX] = 16;
        }
        else if (currentMap[hitBlockY][hitBlockX] == 61) { // Flower Box
            playSound("powerup_appears");
            spawnItem(Item::ItemType::Flower, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE);
            currentMap[hitBlockY][hitBlockX] = 16;
        }
        else if (currentMap[hitBlockY][hitBlockX] == 62) { // Tino Box
            playSound("powerup_appears");
            spawnItem(Item::ItemType::Tino, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE);
            currentMap[hitBlockY][hitBlockX] = 16;
        }
        else if (currentMap[hitBlockY][hitBlockX] == 63) { // Up Mushroom Box
            playSound("powerup_appears");
            spawnItem(Item::ItemType::UpMushroom, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE);
            currentMap[hitBlockY][hitBlockX] = 16;
        }
        else if (currentMap[hitBlockY][hitBlockX] == 64) { // Coin Box
            playSound("coin");
            currentMap[hitBlockY][hitBlockX] = 16;
            player.addCoin(1);
        }
        else if (currentMap[hitBlockY][hitBlockX] == 65) { // Invisible Star Block
            spawnItem(Item::ItemType::Star, hitBlockX * TILE_SIZE, hitBlockY * TILE_SIZE);
            currentMap[hitBlockY][hitBlockX] = 16;
        }

        player.setY((topTile + 1) * TILE_SIZE);
        player.setVy(0);
    }

    // Boundary checks (left/right of screen)
    if (playerX < 0) {
        player.setX(0);
    }
    // Player falling off the bottom of the screen
    if (playerY > 800) { // Assuming 800 is roughly the bottom of the screen
        player.setDead(true); // Assuming player has a dead() method
    }
}

void GameWorld::checkPlayerItemCollision() 
{
    for (auto it = items.begin(); it != items.end(); )
    {
        Item* item = it->get();
        if (isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(),
                        item->getX() - cameraX, item->getY(), item->getWidth(), item->getHeight())) 
        {
            // Collision detected
            switch (item->getType()) 
            {
                case Item::ItemType::Mushroom:
                    playSound("powerup");
                    transformStartTime = GetTickCount();
                    setGameState_trans(GameState_Trans::GAME_BIG_TRANS);
                    break;
                case Item::ItemType::Star:
                    player.gainStar(*this); // Assuming player has a gainStar method
                    break;
                case Item::ItemType::Flower:
                    playSound("powerup");
                    transformStartTime = GetTickCount();
                    setGameState_trans(GameState_Trans::GAME_FLOWER_TRANS);
                    break;
                case Item::ItemType::Tino:
                    playSound("tino_attack");
                    transformStartTime = GetTickCount();
                    setGameState_trans(GameState_Trans::GAME_TINO_TRANS);
                    break;
                case Item::ItemType::UpMushroom:
                    playSound("1-up");
                    player.addLife(1); // Assuming player has an addLife method
                    break;
            }
            it = items.erase(it); // Remove item after collision
        } 
        else 
        {
            ++it;
        }
    }
}

void GameWorld::checkFlagCollision() {
    int left = (player.getX() + cameraX) / 40;
    int right = (player.getX() + player.getWidth() - 1 + cameraX) / 40;
    int top = player.getY() / 40;
    int bottom = (player.getY() + player.getHeight() - 1) / 40;
    int middle = (player.getY() + player.getHeight() / 2 - 1) / 40;

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int screenX = j * 40 - cameraX;
            int screenY = i * 40;
            if ((currentMap[i][j] == 7 || currentMap[i][j] == 8) && isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(), screenX, screenY, 10, 30))
            {
                stopAllSounds();
                playSound("stage_clear");
                gameState = GameState::GAME_VICTORY;
                victoryStart = GetTickCount();
            }
        }
    }
}

void GameWorld::checkClearCollision() {
    if (stage != 3) return;

    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            int screenX = j * 40 - cameraX;
            int screenY = i * 40;
            if (j == 139 && isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(), screenX, screenY, 40, 40))
            {
                stopAllSounds();
                playSound("world_clear");
                gameState = GameState::GAME_CLEAR;
                clearStart = GetTickCount();
            }
        }
    }
}

void GameWorld::checkItemMapCollision() {
    for (auto& item : items) {
        if (!item->isActive()) continue;

        // Apply gravity
        item->setVy(item->getVy() + 1);
        if (item->getVy() > 10) item->setVy(10);
        item->setY(item->getY() + item->getVy());

        // Horizontal movement
        item->setX(item->getX() + item->getVx());

        // Calculate tile coordinates for item's bounding box
        int leftTile = static_cast<int>(item->getX() / TILE_SIZE);
        int rightTile = static_cast<int>((item->getX() + item->getWidth() - 1) / TILE_SIZE);
        int topTile = static_cast<int>(item->getY() / TILE_SIZE);
        int bottomTile = static_cast<int>((item->getY() + item->getHeight() - 1) / TILE_SIZE);
        int middleTile = static_cast<int>((item->getY() + item->getHeight() / 2 - 1) / TILE_SIZE);

        // Check for horizontal collision with walls
        if (item->getVx() < 0) { // Moving left
            if (leftTile >= 0 && leftTile < MAP_WIDTH && isSolidTile(currentMap[middleTile][leftTile])) {
                item->setX((leftTile + 1) * TILE_SIZE);
                item->setVx(-item->getVx());
            }
        } else if (item->getVx() > 0) { // Moving right
            if (rightTile < MAP_WIDTH && rightTile >= 0 && isSolidTile(currentMap[middleTile][rightTile])) {
                item->setX(rightTile * TILE_SIZE - item->getWidth());
                item->setVx(-item->getVx());
            }
        }

        // Check for vertical collision (ground)
        if (item->getVy() > 0 && bottomTile < MAP_HEIGHT && bottomTile >= 0 &&
            (isSolidTile(currentMap[bottomTile][leftTile]) || isSolidTile(currentMap[bottomTile][rightTile]))) {
            item->setY(bottomTile * TILE_SIZE - item->getHeight());
            if (item->getType() == Item::ItemType::Star) {
                item->setVy(-10);
            } else {
                item->setVy(0);
            }
        }
    }
}

void GameWorld::checkPlayerCoinCollision()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int screenX = j * TILE_SIZE - cameraX;
            int screenY = i * TILE_SIZE;
            if (currentMap[i][j] == 2 && isColliding(player.getX(), player.getY(), player.getWidth(), player.getHeight(), screenX, screenY, 30, 30))
            {
                playSound("coin");
                currentMap[i][j] = 0;
                player.addCoin(1);
            }
        }
    }
}


// load 관련

void GameWorld::setStageBGM()
{
    if (stage == 1 || stage == 2)
    {
        playSound("GroundTheme", true);
    }
    else if (stage == 3)
    {
        playSound("CastleTheme", true);
    }
}

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
        { Monster::MonsterType::GreenTurtle, 55, 10 },
        { Monster::MonsterType::GreenTurtle, 65, 10 },
        { Monster::MonsterType::GreenTurtle, 85, 10 },
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

void GameWorld::initMap1() {
    // Map 1 initialization logic from func.cpp
    for (int j = 0; j < MAP_WIDTH; j++)
    {
        map1[MAP_HEIGHT - 2][j] = 1;
        map1[MAP_HEIGHT - 1][j] = 1;
    }
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