#include "NormalGoomba.h"
#include "../GamePhysics.h"

NormalGoomba::NormalGoomba(int x, int y)
    : Goomba(MonsterType::NormalGoomba, x, y, 40, 40) {
    setVx(-1);
}

void NormalGoomba::monster_logic(GameWorld& world) {
    if (!isAlive() || isFalling()) return;

    // Check for ledges
    int nextX = getX() + getVx();
    int footX = (getVx() < 0) ? nextX : nextX + getWidth();
    int footTileX = footX / TILE_SIZE;
    int footTileY = getY() / TILE_SIZE + 1;

    const int(*currentMap)[MAP_WIDTH] = world.getCurrentMap();
    if (footTileX >= 0 && footTileX < MAP_WIDTH && footTileY >= 0 && footTileY < MAP_HEIGHT)
    {
        int tile = currentMap[footTileY][footTileX];
    }

    setX(getX() + getVx());
}
