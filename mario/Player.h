#pragma once
#include <windows.h>

enum class PlayerState {
    Small,
    Big,
    Flower,
    Tino,
    Star
};

enum class DamageResult {
    NoDamage,
    Shrunk,
    Died
};

class GameWorld; // Forward declaration

class Player {
public:
    Player();

    void update(GameWorld& world);
    void reset();

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
    void addCoin(int count);

    void grow();
    void shrink();
    void gainStar();
    void gainFlower();
    void gainTino();
    void addLife(int count);
    void tinoAttack(GameWorld& world);
    DamageResult takeDamage(int damage);

    bool isBig() const;
    bool isFlower() const;
    bool isTino() const;
    bool hasStar() const;
    void setGodMode(bool godMode);
    void updateAnimation();
    bool isGodMode() const;

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

    int getWalkMotion() const;
    int getDirection() const;
    int getWidth() const;
    void setWidth(int width);
    int getHeight() const;
    void setHeight(int height);
    int getTinoCooldownZ() const;
    int getTinoCooldownSpace() const;

private:
    void move(GameWorld& world);
    int x, y;
    int vx, vy;
    int life;
    int coin;
    int width, height;
    int direction;      // 0: left, 1: right
    int walk_motion;
    int motion_timer;
    int m_walk_motion_timer;
    int tino_cooldown_z;
    int tino_cooldown_space;

    bool m_isJumping;
    bool m_isFlying;
    bool m_isWalking;
    bool m_dead;
    bool m_gameOver;
    bool fire_motion;
    bool tino_motion;
    bool tino_fire_motion;
    bool supergod;
    int m_fire_cooldown;
    DWORD m_god_timer;

    bool _isGodModeActive; // New member for god mode status
    DWORD _godModeEndTime; // New member for god mode end time

    PlayerState currentState;

};