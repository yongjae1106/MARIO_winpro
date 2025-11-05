#include "Monster.h"
#include <Gdiplus.h>
using namespace Gdiplus;

Monster::Monster(MonsterType type, int x, int y, int width, int height)
    : type(type), width(width), height(height) {
    setX(x);
    setY(y);
    setVx(0);
    setVy(0);
    setAlive(true);
    setFalling(false);
}

void Monster::setAlive(bool isAlive) { alive = isAlive; }

void Monster::setFalling(bool isFalling) { falling = isFalling; }

bool Monster::isFalling() const { return falling; }

int Monster::getVx() const { return vx; }
void Monster::setVx(int newVx) { vx = newVx; }
int Monster::getVy() const { return vy; }
void Monster::setVy(int newVy) { vy = newVy; }
void Monster::setX(int newX) { x = newX; }
void Monster::setY(int newY) { y = newY; }
void Monster::takeDamage(GameWorld& world, int damage) { /* Implement damage logic later */ }

Monster::MonsterType Monster::getType() const { return type; }
int Monster::getX() const { return x; }
int Monster::getY() const { return y; }
int Monster::getWidth() const { return width; }
int Monster::getHeight() const { return height; }
bool Monster::isAlive() const { return alive; }
bool Monster::isDead() const { return !alive; }
