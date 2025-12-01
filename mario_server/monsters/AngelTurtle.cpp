#include "AngelTurtle.h"
#include "../GameWorld.h"
#include "GreenTurtle.h"

AngelTurtle::AngelTurtle(int x, int y)
    : Turtle(MonsterType::AngelTurtle, x, y, 40, 50), m_topY(y - 30), m_bottomY(y + 30), m_goingUp(false) {
    setVy(1);
}

void AngelTurtle::monster_logic(GameWorld& world)
{
    if (!isAlive() || isFalling()) return;

    if (m_state == TurtleState::ANGEL) 
    {
        if (m_goingUp)
            setY(getY() - getVy());
        else
            setY(getY() + getVy());

        if (getY() <= m_topY)
            m_goingUp = false;
        else if (getY() >= m_bottomY)
            m_goingUp = true;
    } 
    else 
    {
        // After being stomped, it behaves like a normal turtle shell
        Turtle::update(world);
    }
}

void AngelTurtle::takeDamage(GameWorld& world, int damage) {
    if (m_state == TurtleState::NORMAL) {
        // Transform into a GreenTurtle
        // This is a simplified approach. A better approach would be to have GameWorld handle the transformation.
        setAlive(false);
        world.spawnMonster(std::make_unique<GreenTurtle>(getX(), getY()));
    } else {
        Turtle::takeDamage(world, damage);
    }
}