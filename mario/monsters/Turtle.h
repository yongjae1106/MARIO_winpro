#pragma once

#include "../Monster.h"

class Turtle : public Monster 
{
public:
    enum class TurtleState { ANGEL, NORMAL, SHELL, SPINNING };

    Turtle(MonsterType type, int x, int y, int width, int height);

    virtual void monster_logic(GameWorld& world) override;
    void takeDamage(GameWorld& world, int damage) override;

    TurtleState getState() const;
    void setState(TurtleState state);

protected:
    TurtleState m_state;
    DWORD m_shellTimer;
};