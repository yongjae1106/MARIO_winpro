#include "BlueGoomba.h"
#include "../GameWorld.h"

BlueGoomba::BlueGoomba(int x, int y)
    : Goomba(MonsterType::BlueGoomba, x, y, 40, 40), m_jumpTimer(0) {
    setVx(-1);
}

void BlueGoomba::monster_logic(GameWorld& world) {
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

    m_jumpTimer++;
    if (m_jumpTimer > 120) { // Jump every 2 seconds
        setVy(-10);
        m_jumpTimer = 0;
    }
}
