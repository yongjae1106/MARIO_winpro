#include "Player.h"
#include "GameWorld.h"
#include <Gdiplus.h>
#include <tchar.h>
#include <format>
#include <string>

using namespace Gdiplus;

Player::Player() {
    setState(PlayerState::Small);
    // Initialize all member variables to a default state
    x = 0; y = 0;
    vx = 0; vy = 0;
    life = 0; coin = 0;
    width = 0; height = 0;
    direction = 1;
    walk_motion = 0;
    m_isJumping = false;
    m_isFlying = false;
    m_isWalking = false;
    m_dead = false;
    m_gameOver = false;
    fire_motion = false;
    tino_fire_motion = false;
    tino_attack_motion = false;
    m_fire_motion_timer = 0;
    m_tino_attack_motion_timer = 0;
    tino_cooldown_space = 0;
    _isStarGodModeActive = false;
    _isSuperGodModeActive = false;
    m_state_trans = GameState_Trans::GAME_NONE; // New
    m_transformStartTime = 0;                 // New
}

void Player::updateStateFromServer(const Packet_PLAYER_STATE_S2C& packet) {
    // Update all local member variables based on the received packet
    x = packet.x;
    y = packet.y;
    vx = packet.vx;
    vy = packet.vy;
    life = packet.life;
    coin = packet.coin;
    width = packet.width;
    height = packet.height;
    direction = packet.direction;
    walk_motion = packet.walk_motion;
    m_isJumping = packet.m_isJumping;
    m_isFlying = packet.m_isFlying;
    m_isWalking = packet.m_isWalking;
    m_dead = packet.m_dead;
    m_gameOver = packet.m_gameOver;
    fire_motion = packet.fire_motion;
    tino_fire_motion = packet.tino_fire_motion;
    tino_attack_motion = packet.tino_attack_motion;
    currentState = packet.currentState;
    m_state_trans = packet.state_trans; // New
    m_transformStartTime = packet.transformStartTime; // New
    _isStarGodModeActive = packet._isStarGodModeActive;
    _isSuperGodModeActive = packet._isSuperGodModeActive;
    tino_cooldown_space = packet.tino_cooldown_space;
    m_fire_motion_timer = packet.fire_motion_timer;
    m_tino_attack_motion_timer = packet.tino_attack_motion_timer;
}

void Player::updateAnimation() 
{
    // Client-side animation logic based on current state
    if (tino_fire_motion || tino_attack_motion) 
    {
        walk_motion = 0; // Stop walking animation if attacking
        // Specific animation logic for Tino fire or attack motion can go here
    } 
    else if (m_isWalking) 
    {
        walk_motion = (walk_motion + 1) % 3; // Example: Cycle through 3 walk frames
    } 
    else 
    {
        walk_motion = 0; // Standing still
    }
}

void Player::setStop() {
    // This might be used to visually stop the player on client side,
    // but actual movement stop is server-driven.
    setVx(0);
    setVy(0);
    setWalking(false);
}

// All getters and setters remain as they are for accessing local state
int Player::getX() const { return x; }
void Player::setX(int newX) { x = newX; }
int Player::getVx() const { return vx; }
void Player::setVx(int newVx) { vx = newVx; }
int Player::getY() const { return y; }
void Player::setY(int newY) { y = newY; }
int Player::getVy() const { return vy; }
void Player::setVy(int newVy) { vy = newVy; }
int Player::getLife() const { return life; }
void Player::setLife(int newLife) { life = newLife; }
int Player::getCoin() const { return coin; }
void Player::setCoin(int newCoin) { coin = newCoin; }
PlayerState Player::getState() const { return currentState; }
void Player::setState(PlayerState newState) { currentState = newState; }
bool Player::isDead() const { return m_dead; }
void Player::setDead(bool isDead) { m_dead = isDead; }
bool Player::isFlying() const { return m_isFlying; }
void Player::setFlying(bool flying) { m_isFlying = flying; }
bool Player::isGameOver() const { return m_gameOver; }
void Player::setGameOver(bool isGameOver) { m_gameOver = isGameOver; }
bool Player::isJumping() const { return m_isJumping; }
void Player::setJumping(bool jumping) { m_isJumping = jumping; }
bool Player::isWalking() const { return m_isWalking; }
void Player::setWalking(bool walking) { m_isWalking = walking; }
bool Player::isFiring() const { return fire_motion; }
int Player::getWalkMotion() const { return walk_motion; }
int Player::getDirection() const { return direction; }
int Player::getWidth() const { return width; }
void Player::setWidth(int newWidth) { width = newWidth; }
int Player::getHeight() const { return height; }
void Player::setHeight(int newHeight) { height = newHeight; }
int Player::getTinoCooldownSpace() const { return tino_cooldown_space; }
int Player::getFireMotionTimer() const { return m_fire_motion_timer; }
int Player::getTinoAttackMotionTimer() const { return m_tino_attack_motion_timer; }
bool Player::isTinoFireMotion() const { return tino_fire_motion; }
bool Player::isTinoAttackMotion() const { return tino_attack_motion; }
bool Player::isBig() const { return currentState == PlayerState::Big || currentState == PlayerState::Flower || currentState == PlayerState::Tino; }
bool Player::isFlower() const { return currentState == PlayerState::Flower; }
bool Player::isTino() const { return currentState == PlayerState::Tino; }

GameState_Trans Player::getGameState_trans() const {
    return m_state_trans;
}

// Removed game logic methods:
// reset(), update(), updateCooldown(), grow(), shrink(), gainStar(), gainFlower(), gainTino(), addLife(),
// tinoAttack(), calculateDamageResult(), setStarGodMode(), setSuperGodMode(), move()
// Also removed GameState_Trans related getters/setters and member variable.