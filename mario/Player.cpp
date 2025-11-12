#include "Player.h"
#include "GameWorld.h"
#include "monsters/Bowser.h"
#include "Monster.h"
#include <Gdiplus.h>
#include <tchar.h>
#include <format>
#include <string>

using namespace Gdiplus;

Player::Player() {
    setState(PlayerState::Small);
    _isStarGodModeActive = false;
    _starGodModeEndTime = 0;
    _isSuperGodModeActive = false;
    _superGodModeEndTime = 0;
    m_tinofire_cooldown = 0;
    tino_attack_motion = false; // Initialize new member variable
    m_tino_attack_motion_timer = 0; // Initialize new member variable
}

void Player::reset() {
    setX(100);
    setY(300);
    setVx(0);
    setVy(0);
    setLife(5);
    setCoin(0);
    width = 40;
    height = 40;
    direction = 1;
    walk_motion = 0;
    motion_timer = 0;
    m_walk_motion_timer = 0;
    tino_cooldown_space = 0;
    m_fire_cooldown = 0;
    m_tinofire_cooldown = 0;
    m_fire_motion_timer = 0;
    m_god_timer = 0;
    setJumping(false);
    setFlying(false);
    setWalking(false);
    setDead(false);
    setGameOver(false);
    fire_motion = false;
    tino_motion = false;
    tino_fire_motion = false;
    tino_attack_motion = false; // Initialize new member variable
    setState(PlayerState::Small);
    _isStarGodModeActive = false;
    _starGodModeEndTime = 0;
    _isSuperGodModeActive = false;
    _superGodModeEndTime = 0;
}

void Player::update(GameWorld& world) 
{
    if (world.getGameState_trans() == GameState_Trans::GAME_BIG_TRANS ||
        world.getGameState_trans() == GameState_Trans::GAME_FLOWER_TRANS ||
        world.getGameState_trans() == GameState_Trans::GAME_TINO_TRANS)
    {
        setVx(0);
        setVy(0);
        setWalking(false);
        setSuperGodMode(true); // 변신 중 무적 활성화
        return; // Skip all other updates during transformation
    }

    if (world.getGameState() != GameState::GAME_VICTORY && world.getGameState() != GameState::GAME_CLEAR) {
        move(world);
    }
    // Horizontal movement
    setX(getX() + getVx());
    // Apply gravity
    setVy(getVy() + 1);
    if (getVy() > 15) setVy(15);
    setY(getY() + getVy());

    // Mario left wall collision
    if (getX() < 0) {
        setX(0);
    }

    if (getY() > 800) {
        setDead(true);
    }

    if (m_fire_cooldown > 0) {
        m_fire_cooldown--;
    }

    if (m_tinofire_cooldown > 0) {
        m_tinofire_cooldown--;
    }
    else
    {
        tino_fire_motion = false;
    }

    if (tino_cooldown_space > 0) {
        tino_cooldown_space--;
    }

    if (m_fire_motion_timer > 0) 
    {
        m_fire_motion_timer--;
    } 
    else 
    {
        fire_motion = false;
    }

    if (m_tino_attack_motion_timer > 0) 
    {
        m_tino_attack_motion_timer--;
    } 
    else 
    {
        tino_attack_motion = false;
    }

    if (_isStarGodModeActive) {
        if (GetTickCount() >= _starGodModeEndTime) {
            _isStarGodModeActive = false; // God mode ends
            world.stopAllSounds();
            world.setStageBGM();
        }
    }

    if (_isSuperGodModeActive) {
        if (GetTickCount() >= _superGodModeEndTime) {
            _isSuperGodModeActive = false; // Super god mode ends
        }
    }
}

void Player::move(GameWorld& world) 
{
    if (world.getPlayer().isDead()) return;
    const bool* keyState = world.getKeyState();
    // 디버그: move 함수 시작 시 키 상태 및 현재 vx 출력
    TCHAR debugMessage[256];
    _stprintf_s(debugMessage, _T("Player::move - Before logic: keyState[VK_LEFT]: %d, keyState[VK_RIGHT]: %d, CurrentVx: %d\n"), keyState[VK_LEFT], keyState[VK_RIGHT], getVx());
    OutputDebugString(debugMessage);
    if (keyState[VK_LEFT] && !keyState[VK_RIGHT])
    {
        setVx(-5);
        direction = 0;
        setWalking(true);
    }
    else if (keyState[VK_RIGHT] && !keyState[VK_LEFT]) 
    {
        setVx(5);
        direction = 1;
        setWalking(true);
    }
    else if (keyState[VK_LEFT] && keyState[VK_RIGHT])
    {
        // 둘 다 눌린 경우 마지막 방향 유지
        setVx((getDirection() == 0 ? -5 : 5));
        setWalking(true);
    }
    else 
    {
        setWalking(false);
        setVx(0); // This is the deceleration
        walk_motion = 0;
        // 디버그: ELSE 블록 실행됨, Vx를 0으로 설정
        OutputDebugString(L"Player::move - ELSE block executed. Setting Vx to 0.\n");
    }

    if (keyState[VK_UP] && !isJumping())
    {
        switch (currentState)
        {
            case (PlayerState::Small):
            {
                world.playSound("jump-small");
                break;
            }
            default:
                world.playSound("jump-super");
        }
        setVy(-20);
        setJumping(true);
    }

    if (keyState['Z'] && isFlower() && m_fire_cooldown == 0) {
        world.spawnPlayerFireball(getX() + world.getCameraX(), getY(), (direction == 0 ? -10 : 10));
        m_fire_cooldown = 20; // Cooldown for 20 frames
        fire_motion = true;
        m_fire_motion_timer = 10; // Play fire motion for 10 frames
    }

    if (keyState['Z'] && isTino() && m_tinofire_cooldown == 0) { // 'Z' for Tino Fireball
        world.spawnTinoFireball(getX() + world.getCameraX(), getY(), (direction == 0 ? -7 : 7), direction);
        m_tinofire_cooldown = 30; // Cooldown for 30 frames
        world.playSound("tino_fire_shoot");
        tino_fire_motion = true; // Set Tino Fireball motion flag
        m_fire_motion_timer = 10; // Set motion timer
    }

    if (keyState[VK_SPACE] && isTino() && tino_cooldown_space == 0) { // 'SPACE' for Tino Attack
        tinoAttack(world);
        tino_cooldown_space = 100; // Cooldown for 30 frames
        tino_attack_motion = true; // Set Tino Attack motion flag
        m_tino_attack_motion_timer = 30; // Set motion timer
        setSuperGodMode(true); // Activate invincibility
        _superGodModeEndTime = GetTickCount() + (m_tino_attack_motion_timer * (3000 / 60)); // Invincibility duration based on motion timer
    }

    // 디버그: 입력 처리 후 Vx 값 출력
    _stprintf_s(debugMessage, _T("Player::move - After logic: Vx: %d\n"), getVx());
    OutputDebugString(debugMessage);
}

void Player::updateAnimation() {
    if (tino_fire_motion || tino_attack_motion) {
        // Specific animation logic for Tino fire or attack motion
        // For now, we just ensure walking animation is paused/overridden
        walk_motion = 0; // Stop walking animation
        return;
    }

    if (isWalking()) {
        m_walk_motion_timer++;
        if (m_walk_motion_timer > 5) {
            walk_motion = (walk_motion + 1) % 3;
            m_walk_motion_timer = 0;
        }
    }
}

void Player::setStop()
{
    setVx(0);
    setVy(0);
    setWalking(false);
}

int Player::getX() const {
    return x;
}

void Player::setX(int newX) {
    x = newX;
}

int Player::getVx() const {
    return vx;
}

void Player::setVx(int newVx) {
    vx = newVx;
}

int Player::getY() const {
    return y;
}

void Player::setY(int newY) {
    y = newY;
}

int Player::getVy() const {
    return vy;
}

void Player::setVy(int newVy) {
    vy = newVy;
}

int Player::getLife() const {
    return life;
}

void Player::setLife(int newLife) {
    life = newLife;
}

int Player::getCoin() const {
    return coin;
}

void Player::setCoin(int newCoin) {
    coin = newCoin;
}

PlayerState Player::getState() const {
    return currentState;
}

void Player::setState(PlayerState newState) {
    currentState = newState;
}

bool Player::isDead() const {
    return m_dead;
}

void Player::setDead(bool isDead) {
    m_dead = isDead;
}

bool Player::isFlying() const {
    return m_isFlying;
}

void Player::setFlying(bool flying) {
    m_isFlying = flying;
}

bool Player::isGameOver() const {
    return m_gameOver;
}

void Player::setGameOver(bool isGameOver) {
    m_gameOver = isGameOver;
}

bool Player::isJumping() const {
    return m_isJumping;
}

void Player::setJumping(bool jumping) {
    m_isJumping = jumping;
}

bool Player::isWalking() const {
    return m_isWalking;
}

void Player::setWalking(bool walking) {
    m_isWalking = walking;
}

int Player::getWalkMotion() const {
    return walk_motion;
}

int Player::getDirection() const {
    return direction;
}

int Player::getWidth() const {
    return width;
}

void Player::setWidth(int newWidth) {
    width = newWidth;
}

int Player::getHeight() const {
    return height;
}

void Player::setHeight(int newHeight) {
    height = newHeight;
}

int Player::getTinoCooldownTinoFireball() const {
    return m_tinofire_cooldown;
}

int Player::getTinoCooldownSpace() const {
    return tino_cooldown_space;
}

int Player::getFireMotionTimer() const {
    return m_fire_motion_timer;
}

int Player::getTinoAttackMotionTimer() const {
    return m_tino_attack_motion_timer;
}

bool Player::isTinoFireMotion() const {
    return tino_fire_motion;
}

bool Player::isTinoAttackMotion() const {
    return tino_attack_motion;
}

void Player::addCoin(int count) {
    coin += count;
}

void Player::grow() {
    if (currentState == PlayerState::Small) {
        setY(getY() - TILE_SIZE);
        currentState = PlayerState::Big;
        // height = 80; // Assuming big Mario is taller
        setHeight(80);
    }
}

void Player::shrink() {
    if (isBig()) {
        setY(getY() + TILE_SIZE);
        currentState = PlayerState::Small;
        setHeight(40);
    }
}

void Player::gainStar(GameWorld& world) {
    setStarGodMode(true);
    world.stopAllSounds();
    world.playSound("InvincibilityTheme", true);
    // Implement star power-up effects (e.g., temporary invincibility)
}

void Player::gainFlower() {
    if (currentState == PlayerState::Small) {
        setY(getY() - TILE_SIZE);
        currentState = PlayerState::Big;
        // height = 80;
        setHeight(80);
    }
    currentState = PlayerState::Flower;
    // Implement flower power-up effects (e.g., fireball ability)
}

void Player::gainTino() {
    if (currentState == PlayerState::Small) {
        currentState = PlayerState::Big;
        // height = 80;
        setHeight(80);
    }
    currentState = PlayerState::Tino;
    // Implement Tino power-up effects
}

void Player::addLife(int count) {
    life += count;
}



DamageResult Player::calculateDamageResult(int damage) const {
    if (isStarGodMode() || isSuperGodMode()) return DamageResult::NoDamage; // Invincible

    if (isBig()) 
    {
        return DamageResult::Shrunk;
    }
    else if (isFlower() || isTino())
    {
        return DamageResult::Shrunk;
    }
    else 
    {
        return DamageResult::Died;
    }
}

bool Player::isBig() const 
{
    return currentState == PlayerState::Big || currentState == PlayerState::Flower || currentState == PlayerState::Tino;
}

bool Player::isFlower() const 
{
    return currentState == PlayerState::Flower;
}

bool Player::isTino() const {
    return currentState == PlayerState::Tino;
}

void Player::setStarGodMode(bool godMode) 
{
    if (godMode) 
    {
        _isStarGodModeActive = true;
        _starGodModeEndTime = GetTickCount() + 10000; // 10 seconds of god mode
    } 
    else 
    {
        _isStarGodModeActive = false;
        _starGodModeEndTime = 0;
    }
}

bool Player::isStarGodMode() const 
{
    return _isStarGodModeActive && (GetTickCount() < _starGodModeEndTime);
}

void Player::setSuperGodMode(bool godMode) 
{
    if (godMode) 
    {
        _isSuperGodModeActive = true;
        _superGodModeEndTime = GetTickCount() + 3000; // 3 seconds of god mode by default
    } 
    else 
    {
        _isSuperGodModeActive = false;
        _superGodModeEndTime = 0;
    }
}

bool Player::isSuperGodMode() const {
    return _isSuperGodModeActive;
}

bool Player::isFiring() const {
    return fire_motion;
}

void Player::tinoAttack(GameWorld& world) {
    // Tino attack logic (collision with monsters)
    // Attack box definition
    int attackRangeX = (direction == 0) ? getX() - 50 : getX() + 50;
    int attackRangeY = getY() - 15; // Centered vertically, 50px height
    int attackWidth = 50;
    int attackHeight = 100; // Smaller, more focused attack box

    for (auto& monster : world.getMonsters()) {
        if (!monster->isAlive()) continue;

        int monsterScreenX = monster->getX() - world.getCameraX();

        if (isColliding(attackRangeX, attackRangeY, attackWidth, attackHeight,
                        monsterScreenX, monster->getY(), monster->getWidth(), monster->getHeight())) {
            world.playSound("kick");

            // Handle Bowser specific logic
            Bowser* bowserMonster = dynamic_cast<Bowser*>(monster.get());
            if (bowserMonster)
            {
                monster->takeDamage(world, 10); // Deal more damage to Bowser
            } 
            else 
            {
                // Knockback for non-Bowser monsters
                monster->setVy(-15); // Make monster fly upwards
                monster->setFalling(true);
            }
            
            // Add visual feedback
            world.spawnTinoFireballEffect(monster->getX(), monster->getY(), 0, direction);
        }
    }
}



