#include "Player.h"
#include "GamePhysics.h"
#include "monsters/Bowser.h"
#include "monsters/Monster.h"
#include <Gdiplus.h>
#include <tchar.h>
#include <format>
#include <string>

using namespace Gdiplus;

Player::Player(int id) : playerID(id)
{
    setState(PlayerState::Small);
    _isStarGodModeActive = false;
    _starGodModeEndTime = 0;
    _isSuperGodModeActive = false;
    _superGodModeEndTime = 0;
    m_tinofire_cooldown = 0;
    tino_attack_motion = false; // Initialize new member variable
    m_state_trans = GameState_Trans::GAME_TRANS_NONE; // New
    m_transformStartTime = 0; // New
    memset(m_keyState, 0, sizeof(m_keyState)); // Initialize m_keyState
    reset();
}

int Player::getPlayerID() const {
    return playerID;
}

void Player::reset() 
{
    setX(100);
    setY(300);
    setVx(0);
    setVy(0);
    width = 40;
    height = 40;
    direction = 1;
    tino_cooldown_space = 0;
    m_fire_cooldown = 0;
    m_tinofire_cooldown = 0;
    m_god_timer = 0;
    setJumping(false);
    setFlying(false);
    setWalking(false);
    setDead(false);
    setGameOver(false);
    setState(PlayerState::Small);
    setGameState_trans(GameState_Trans::GAME_TRANS_NONE); // New
    setTransformStartTime(0); // New
    _isStarGodModeActive = false;
    _starGodModeEndTime = 0;
    _isSuperGodModeActive = false;
    _superGodModeEndTime = 0;
}

void Player::update(GameWorld& world)
{
    if (getGameState_trans() != GameState_Trans::GAME_TRANS_NONE) // Modified
    {
        setVx(0);
        setVy(0);
        setWalking(false);
        setSuperGodMode(true); //    Ȱȭ
        return; // Skip all other updates during transformation
    }

    if (world.getGameState() != GameState::GAME_VICTORY && world.getGameState() != GameState::GAME_CLEAR) 
    {
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

    if (_isSuperGodModeActive) {
        if (GetTickCount() >= _superGodModeEndTime) {
            _isSuperGodModeActive = false; // Super god mode ends
        }
    }
}

void Player::updateCooldown()
{
    if (m_fire_cooldown > 0) {
        m_fire_cooldown--;
    }

    if (m_tinofire_cooldown > 0) {
        m_tinofire_cooldown--;
    }


    if (tino_cooldown_space > 0) {
        tino_cooldown_space--;
    }


}

void Player::move(GameWorld& world)
{
    if (this->isDead()) return;
    // const bool* keyState = world.getKeyState(); // REMOVED
    // : move Լ   Ű    vx 
    TCHAR debugMessage[256];
    _stprintf_s(debugMessage, _T("Player::move - Before logic: keyState[VK_LEFT]: %d, keyState[VK_RIGHT]: %d, CurrentVx: %d\n"), getKeyState(VK_LEFT), getKeyState(VK_RIGHT), getVx()); // Modified
    OutputDebugString(debugMessage);
    if (getKeyState(VK_LEFT) && !getKeyState(VK_RIGHT)) // Modified
    {
        setVx(-5);
        direction = 0;
        setWalking(true);
    }
    else if (getKeyState(VK_RIGHT) && !getKeyState(VK_LEFT)) // Modified
    {
        setVx(5);
        direction = 1;
        setWalking(true);
    }
    else if (getKeyState(VK_LEFT) && getKeyState(VK_RIGHT)) // Modified
    {
        //       
        setVx((getDirection() == 0 ? -5 : 5));
        setWalking(true);
    }
    else
    {
        setWalking(false);
        setVx(0); // This is the deceleration
    }

    if (getKeyState(VK_UP) && !isJumping()) // Modified
    {
        switch (currentState)
        {
        case (PlayerState::Small):
        {
            world.pushEvent(GameEvent::PLAYER_SMALL_JUMP);
            break;
        }
        default:
            world.pushEvent(GameEvent::PLAYER_BIG_JUMP);
        }
        setVy(-20);
        setJumping(true);
    }

    if (getKeyState('Z') && isFlower() && m_fire_cooldown == 0) { // Modified
        world.spawnPlayerFireball(getX(), getY(), (direction == 0 ? -10 : 10));
        m_fire_cooldown = 2; // Cooldown for 20 frames
        fire_motion = true;
        m_fire_motion_timer = 10; // Play fire motion for 10 frames
        world.pushEvent(GameEvent::PLAYER_FIRE);
    }

    if (getKeyState('Z') && isTino() && m_tinofire_cooldown == 0) { // 'Z' for Tino Fireball // Modified
        world.spawnTinoFireball(getX(), getY(), (direction == 0 ? -7 : 7), direction);
        m_tinofire_cooldown = 5; // Cooldown for 30 frames
        tino_fire_motion = true; // Set Tino Fireball motion flag
        world.pushEvent(GameEvent::PLAYER_TINOFIRE);
    }

    if (getKeyState(VK_SPACE) && isTino() && tino_cooldown_space == 0) { // 'SPACE' for Tino Attack // Modified
        tinoAttack(world);
        tino_cooldown_space = 15; // Cooldown for 30 frames
        tino_attack_motion = true; // Set Tino Attack motion flag
        setSuperGodMode(true); // Activate invincibility
        _superGodModeEndTime = GetTickCount() + 3000; // Invincibility duration based on motion timer
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

PlayerState Player::getState() const {
    return currentState;
}

void Player::setState(PlayerState newState) {
    currentState = newState;
}

void Player::dead(GameWorld& world) 
{
    world.pushEvent(GameEvent::PLAYER_DIE);
    setVx(0);
    setVy(0);
    setDead(true);
    setdeadStartTime(GetTickCount());
    world.addLife(-1);
}

void Player::setdeadStartTime(int time)
{
    deadStartTime = time;
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

        int monsterScreenX = monster->getX();

        if (isColliding(attackRangeX, attackRangeY, attackWidth, attackHeight,
            monsterScreenX, monster->getY(), monster->getWidth(), monster->getHeight())) {
            world.pushEvent(GameEvent::KICK);

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

void Player::setKeyState(WPARAM key, bool isPressed) {
    if (key < 256) {
        m_keyState[key] = isPressed;
    }
}

bool Player::getKeyState(WPARAM key) const {
    if (key < 256) {
        return m_keyState[key];
    }
    return false;
}


GameState_Trans Player::getGameState_trans() const {
    return m_state_trans;
}

void Player::setGameState_trans(GameState_Trans trans_state) {
    m_state_trans = trans_state;
}

DWORD Player::getTransformStartTime() const {
    return m_transformStartTime;
}

void Player::setTransformStartTime(DWORD time) {
    m_transformStartTime = time;
}