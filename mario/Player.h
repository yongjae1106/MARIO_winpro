#pragma once
#include <windows.h>

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

class GameWorld; // Forward declaration

class Player {
public:
    Player();

    void update(GameWorld& world);
    void reset();

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
    void addCoin(int count);

    void grow();
    void shrink();
    void gainStar(GameWorld& world);
    void gainFlower();
    void gainTino();
    void addLife(int count);
    void tinoAttack(GameWorld& world);
    DamageResult calculateDamageResult(int damage) const;

    bool isBig() const;
    bool isFlower() const;
    bool isTino() const;
    void setStarGodMode(bool godMode);
    void updateAnimation();
    bool isStarGodMode() const;

    void setSuperGodMode(bool godMode);
    bool isSuperGodMode() const;

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

    bool isFiring() const;

    int getWalkMotion() const;
    int getDirection() const;
    int getWidth() const;
    void setWidth(int width);
    int getHeight() const;
    void setHeight(int height);
    int getTinoCooldownTinoFireball() const;
    int getTinoCooldownSpace() const;
    int getFireMotionTimer() const; // New getter
    int getTinoAttackMotionTimer() const; // New getter
    bool isTinoFireMotion() const; // New getter
    bool isTinoAttackMotion() const; // New getter

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
    int tino_cooldown_space;
    int m_fire_motion_timer;

    bool m_isJumping;
    bool m_isFlying;
    bool m_isWalking;
    bool m_dead;
    bool m_gameOver;
    bool fire_motion;
    bool tino_motion;
    bool tino_fire_motion;
    bool tino_attack_motion; // New member variable for Tino attack motion
    int m_fire_cooldown;
    int m_tinofire_cooldown;
    DWORD m_god_timer;
    int m_tino_attack_motion_timer; // New member variable for Tino attack motion timer

    bool _isStarGodModeActive;
    DWORD _starGodModeEndTime;

    bool _isSuperGodModeActive;
    DWORD _superGodModeEndTime;

    PlayerState currentState;

};