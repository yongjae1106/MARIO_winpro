#include "RedGoomba.h"
#include "../GameWorld.h"

RedGoomba::RedGoomba(int x, int y)
    : Goomba(MonsterType::RedGoomba, x, y, 40, 40) {
    setVx(-2); // Faster than NormalGoomba
}

void RedGoomba::monster_logic(GameWorld& world) {
    if (!isAlive() || isFalling()) return;

    // Check for ledges
    int nextX = getX() + getVx();
    int footX = (getVx() < 0) ? nextX : nextX + getWidth();
    int footTileX = footX / TILE_SIZE;
    int footTileY = getY() / TILE_SIZE + 1;

    const int(*currentMap)[MAP_WIDTH] = world.getCurrentMap();
    bool willFall = true;
    if (footTileX >= 0 && footTileX < MAP_WIDTH && footTileY >= 0 && footTileY < MAP_HEIGHT)
    {
        int tile = currentMap[footTileY][footTileX];
        if (tile != 0 && tile != 2)
            willFall = false;
    }

    if (willFall) {
        setVx(-getVx());
    }

    setX(getX() + getVx());
}
