#pragma once
#include <winsock2.h>
#include "PacketManager.h"

enum class PlayerState {
    Small,
    Big,
    Flower,
    Tino
};

enum class DamageResult {
    NoDamage,
    Shrunk,
    Died
};

// Forward declaration for GameWorld is still needed if Player has GameWorld members or methods that take GameWorld as argument
// However, since Player will only receive state from server, direct GameWorld dependency might be reduced.
// For now, keep it as a forward declaration.
class GameWorld; 

class Player {
public:
    Player();

    // New method to update player state from server data
    void updateStateFromServer(const Packet_PLAYER_STATE_S2C& packet);

    void update();
    void updateAnimation();
    void setStop();

    int getX() const;
    void setX(int x);
    int getVx() const;
    void setVx(int vx);
    int getY() const;
    void setY(int y);
    int getVy() const;
    void setVy(int vy);

    int getLife() const;
    void setLife(int life);

    int getCoin() const;
    void setCoin(int coin);
    // addCoin is game logic, will be removed from Player.cpp

    bool isBig() const;
    bool isFlower() const;
    bool isTino() const;
    
    PlayerState getState() const;
    void setState(PlayerState state);

    bool isDead() const;
    void setDead(bool isDead);
    bool isFlying() const;
    void setFlying(bool flying);

    bool isGameOver() const;
    void setGameOver(bool isGameOver);

    bool isJumping() const;
    void setJumping(bool jumping);

    bool isWalking() const;
    void setWalking(bool walking);

    bool isFiring() const; // For rendering fire animation

    int getWalkMotion() const;
    int getDirection() const;
    int getWidth() const;
    void setWidth(int width);
    int getHeight() const;
    void setHeight(int height);
    int getTinoCooldownSpace() const; // For UI display
    int getFireMotionTimer() const; // For rendering fire animation
    int getTinoAttackMotionTimer() const; // For rendering Tino attack animation
    bool isTinoFireMotion() const; // For rendering Tino fire animation
    bool isTinoAttackMotion() const; // For rendering Tino attack animation

    // Client-side flags for visual effects, set by server data
    bool isStarGodMode() const { return _isStarGodModeActive; }
    bool isSuperGodMode() const { return _isSuperGodModeActive; }
    GameState_Trans getGameState_trans() const;

private:
    int x, y;
    int vx, vy;
    int life;
    int coin;
    int width, height;
    int direction;      // 0: left, 1: right
    int walk_motion;
    
    bool m_isJumping;
    bool m_isFlying;
    bool m_isWalking;
    bool m_dead;
    bool m_gameOver;
    bool fire_motion; // For rendering fire animation
    bool tino_fire_motion; // For rendering Tino fire animation
    bool tino_attack_motion; // For rendering Tino attack animation

    // Client-side timers for visual effects, updated by server data
    int m_fire_motion_timer;
    int m_tino_attack_motion_timer;
    int tino_cooldown_space; // For UI display

    bool _isStarGodModeActive; // Set by server data
    bool _isSuperGodModeActive; // Set by server data

    PlayerState currentState;
    GameState_Trans m_state_trans; // New
    DWORD m_transformStartTime;    // New
};