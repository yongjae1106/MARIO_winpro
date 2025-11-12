#include "Monster.h"
#include "GameWorld.h"
#include <Gdiplus.h>
using namespace Gdiplus;

Monster::Monster(MonsterType type, int x, int y, int width, int height)
    : type(type), x(x), y(y), vx(0), vy(0),
    width(width), height(height),
    alive(true), falling(false) {}

void Monster::checkFallDeath()
{
    if (y > 800)
    {
        setAlive(false);
    }
}

void Monster::update(GameWorld& world)
{
    monster_logic(world);
    gravity();
    checkFallDeath();
    checkMonsterMapCollision(world);
}

void Monster::checkMonsterMapCollision(GameWorld& world)
{
    if (!isAlive() || isFalling()) return; // Skip dead monsters

    // Apply gravity
    setVy(getVy() + 1);
    if (getVy() > 10) setVy(10);
    float nextY = getY() + getVy(); // Calculate next Y position

    // Calculate tile coordinates for monster's bounding box (based on current X, and nextY for vertical)
    int leftTile = static_cast<int>(getX() / TILE_SIZE);
    int rightTile = static_cast<int>((getX() + getWidth() - 1) / TILE_SIZE);

    // Calculate tile row of the monster's feet at nextY
    int feetTileY = static_cast<int>((nextY + getHeight()) / TILE_SIZE);

    const int(*currentMap)[MAP_WIDTH] = world.getCurrentMap();

    // Check for vertical collision (ground)
    if (getVy() > 0 && feetTileY < MAP_HEIGHT && feetTileY >= 0 &&
        (world.isSolidTile(currentMap[feetTileY][leftTile]) || world.isSolidTile(currentMap[feetTileY][rightTile]))) {
        // Collision detected. Snap monster to the top of the solid tile.
        setY(feetTileY * TILE_SIZE - getHeight());
        setVy(0);
    }
    else {
        // No collision, update Y
        setY(nextY);
    }

    // Horizontal movement (after vertical resolution)
    setX(getX() + getVx());

    // Recalculate tile coordinates for horizontal collision based on potentially new Y and new X
    leftTile = static_cast<int>(getX() / TILE_SIZE);
    int topTile = static_cast<int>(getY() / TILE_SIZE); // Use resolved Y
    rightTile = static_cast<int>((getX() + getWidth() - 1) / TILE_SIZE);
    int middleTile = static_cast<int>((getY() + getHeight() / 2 - 1) / TILE_SIZE); // Use resolved Y

    // Check for horizontal collision with walls
    if (getVx() < 0) { // Moving left
        if (leftTile >= 0 && leftTile < MAP_WIDTH &&
            (world.isSolidTile(currentMap[topTile][leftTile]) || world.isSolidTile(currentMap[middleTile][leftTile]))) {
            setX((leftTile + 1) * TILE_SIZE);
            setVx(-getVx());
        }
    }
    else if (getVx() > 0) { // Moving right
        if (rightTile < MAP_WIDTH && rightTile >= 0 &&
            (world.isSolidTile(currentMap[topTile][rightTile]) || world.isSolidTile(currentMap[middleTile][rightTile]))) {
            setX(rightTile * TILE_SIZE - getWidth());
            setVx(-getVx());
        }
    }
}

void Monster::gravity()
{
    setVy(getVy() + 1);
    if (getVy() > 10) setVy(10);
    setY(getY() + getVy());
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
