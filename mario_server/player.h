#pragma once

#include <Windows.h>

// Forward declaration to avoid circular dependency
class GameWorld;

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

enum class GameState_Trans
{
    GAME_TRANS_NONE,
    GAME_BIG_TRANS,
    GAME_FLOWER_TRANS,
    GAME_TINO_TRANS
};

class Player {
public:
    Player(int id = -1); // Constructor with optional player ID
    void reset();
    void update(GameWorld& world);
    void updateCooldown();
    void move(GameWorld& world);
    void setStop();
    void dead(GameWorld& world);
    void tinoAttack(GameWorld& world);

    // Getters
    int getPlayerID() const;
    int getX() const;
    int getY() const;
    int getVx() const;
    int getVy() const;
    int getWidth() const;
    int getHeight() const;
    int getDirection() const;
    int getWalkMotion() const;
    int getTinoCooldownTinoFireball() const;
    int getTinoCooldownSpace() const;
    int getFireMotionTimer() const;
    PlayerState getState() const;
    GameState_Trans getGameState_trans() const;
    DWORD getTransformStartTime() const;
    bool isDead() const;
    bool isFlying() const;
    bool isGameOver() const;
    bool isJumping() const;
    bool isWalking() const;
    bool isBig() const;
    bool isFlower() const;
    bool isTino() const;
    bool isStarGodMode() const;
    bool isSuperGodMode() const;
    bool isFiring() const;

    // Setters
    void setX(int newX);
    void setY(int newY);
    void setVx(int newVx);
    void setVy(int newVy);
    void setWidth(int newWidth);
    void setHeight(int newHeight);
    void setState(PlayerState newState);
    void setGameState_trans(GameState_Trans trans_state);
    void setTransformStartTime(DWORD time);
    void setDead(bool isDead);
    void setFlying(bool flying);
    void setGameOver(bool isGameOver);
    void setJumping(bool jumping);
    void setWalking(bool walking);
    void setStarGodMode(bool godMode);
    void setSuperGodMode(bool godMode);

    DamageResult calculateDamageResult(int damage) const;

private:
    int playerID;
    int x, y, vx, vy;
    int width, height;
    int direction;
    int walk_motion;
    int m_fire_cooldown;
    int m_tinofire_cooldown;
    int tino_cooldown_space;
    int m_god_timer;
    int m_fire_motion_timer;

    bool m_isJumping;
    bool m_isFlying;
    bool m_isWalking;
    bool m_dead;
    bool m_gameOver;
    bool fire_motion;
    bool tino_fire_motion;
    bool tino_attack_motion;

    PlayerState currentState;
    GameState_Trans m_state_trans;
    DWORD m_transformStartTime;

    bool _isStarGodModeActive;
    DWORD _starGodModeEndTime;
    bool _isSuperGodModeActive;
    DWORD _superGodModeEndTime;
};