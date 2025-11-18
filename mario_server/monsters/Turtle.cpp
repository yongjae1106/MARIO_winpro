#include "Turtle.h"
#include "../GamePhysics.h"

Turtle::Turtle(MonsterType type, int x, int y, int width, int height)
    : Monster(type, x, y, width, height), m_state(TurtleState::NORMAL), m_shellTimer(0) {
}

void Turtle::monster_logic(GameWorld& world) {
    if (!isAlive() || isFalling()) return;

    if (m_state == TurtleState::NORMAL) 
    {
        // Check for ledges
        int nextX = getX() + getVx();
        int footX = (getVx() < 0) ? nextX : nextX + getWidth();
        int footTileX = footX / TILE_SIZE;
        int footTileY = static_cast<int>((getY() + getHeight()) / TILE_SIZE);

        const int(*currentMap)[MAP_WIDTH] = world.getCurrentMap();
        bool willFall = true;
        if (footTileX >= 0 && footTileX < MAP_WIDTH && footTileY >= 0 && footTileY < MAP_HEIGHT)
        {
            int tile = currentMap[footTileY][footTileX];
            if (tile != 0 && tile != 2)
                willFall = false;
        }

        if (willFall) 
        {
            setVx(-getVx());
        }

        setX(getX() + getVx());
    }
    else if (m_state == TurtleState::SHELL) 
    {
        if (GetTickCount() - m_shellTimer > 5000) {
            setState(TurtleState::NORMAL);
            height = 60;
            setVx(-1);
            setVy(-10);
        }
    }
    else if (m_state == TurtleState::SPINNING) 
    {
        setX(getX() + getVx());
    }
}

void Turtle::takeDamage(GameWorld& world, int damage) {
    if (m_state == TurtleState::NORMAL) 
    {
        m_state = TurtleState::SHELL;
        setVx(0);
        height = 40;
        m_shellTimer = GetTickCount();
    } 
    else if (m_state == TurtleState::SHELL) 
    {
        m_state = TurtleState::SPINNING;
        // Determine direction based on player's collision
        int playerX = world.getPlayer().getX();
        int monsterScreenX = getX() - world.getCameraX();
        int spinSpeed = 5; // Define a spin speed

        if (playerX < monsterScreenX) 
        { // Player collided from left, spin right
            setVx(spinSpeed);
        } else { // Player collided from right, spin left
            setVx(-spinSpeed);
        }
    } 
    else if (m_state == TurtleState::SPINNING) 
    {
        m_state = TurtleState::SHELL;
        setVx(0);
    }
}

Turtle::TurtleState Turtle::getState() const {
    return m_state;
}

void Turtle::setState(TurtleState state) {
    m_state = state;
}

