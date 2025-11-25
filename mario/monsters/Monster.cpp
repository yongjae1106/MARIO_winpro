#include "Monster.h"
#include "../GameWorld.h"
#include <Gdiplus.h>

using namespace Gdiplus;

Monster::Monster(MonsterType type, int x, int y, int width, int height)
    : type(type), x(x), y(y), 
      width(width), height(height),
      alive(true), walk_motion(0), direction(0) {}

void Monster::update()
{
    // Client-side update for animations
    updateAnimation();
}

void Monster::updateAnimation()
{
    // Simple animation logic, relies on server to set walk_motion.
    // If walk_motion is not sent by server, a client-side timer can be used.
    if (GameWorld::getInstance().getGlobalAnimationFrameCounter() % 15 == 0) {
        // This is a purely cosmetic animation effect if server doesn't drive walk_motion
    }
}

void Monster::updateStateFromServer(const MonsterDataPacket& packet)
{
    this->x = packet.x;
    this->y = packet.y;
    this->alive = packet.isAlive;
    this->walk_motion = packet.walk_motion;
    this->direction = packet.direction;
    // this->type = static_cast<MonsterType>(packet.type); // Be careful with this, type should not change
}

// --- Getters ---
Monster::MonsterType Monster::getType() const { return type; }
int Monster::getX() const { return x; }
int Monster::getY() const { return y; }
int Monster::getWidth() const { return width; }
int Monster::getHeight() const { return height; }
bool Monster::isAlive() const { return alive; }
int Monster::getWalkMotion() const { return walk_motion; }
int Monster::getDirection() const { return direction; }
