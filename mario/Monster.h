#pragma once

#include <windows.h>

class GameWorld;

class Monster {
public:
    enum class MonsterType { Goomba, Turtle, AngelTurtle, Bowser };

    Monster(MonsterType type, int x, int y, int width, int height);
    virtual ~Monster() = default;

    virtual void update(GameWorld& world) = 0;
    virtual void render(HDC hdc, int cameraX) = 0;

    // Common getters and setters
    MonsterType getType() const;
    int getX() const;
    void setX(int x);
    int getY() const;
    void setY(int y);
    int getWidth() const;
    int getHeight() const;
    int getVx() const;
    void setVx(int vx);
    int getVy() const;
    void setVy(int vy);
    bool isAlive() const;
    void setAlive(bool isAlive);
    bool isDead() const;
    bool isFalling() const;
    void setFalling(bool isFalling);
    virtual void takeDamage(GameWorld& world, int damage);

protected:
    MonsterType type;
    int x, y;
    int vx, vy;
    int width, height;
    bool alive;
    bool falling;
};
