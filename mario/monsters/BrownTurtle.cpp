#include "BrownTurtle.h"
#include "../GameWorld.h"

BrownTurtle::BrownTurtle(int x, int y)
    : Turtle(MonsterType::Turtle, x, y, 40, 60) {
    setVx(-1);
}

void BrownTurtle::update(GameWorld& world) {
    if (!isAlive()) return;

    if (m_state == TurtleState::NORMAL) {
        // Check for ledges
        int nextX = getX() + getVx();
        int footX = (getVx() < 0) ? nextX : nextX + getWidth();
        int footTileX = footX / 40;
        int footTileY = getY() / 40 + 1;

        const int(*currentMap)[200] = world.getCurrentMap();
        bool willFall = true;
        if (footTileX >= 0 && footTileX < 200 && footTileY >= 0 && footTileY < 15)
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

void BrownTurtle::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}
