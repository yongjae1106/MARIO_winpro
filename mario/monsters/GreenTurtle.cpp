#include "GreenTurtle.h"
#include "../GameWorld.h"

GreenTurtle::GreenTurtle(int x, int y)
    : Turtle(MonsterType::Turtle, x, y, 40, 60) {
    setVx(-1);
}

void GreenTurtle::update(GameWorld& world) {
    if (!isAlive()) return;

    if (m_state == TurtleState::NORMAL) {
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
    } else if (m_state == TurtleState::SHELL) {
        if (GetTickCount() - m_shellTimer > 5000) {
            setState(TurtleState::NORMAL);
            setVx(-1);
        }
    } else if (m_state == TurtleState::SPINNING) {
        setX(getX() + getVx());
    }
}

void GreenTurtle::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}
