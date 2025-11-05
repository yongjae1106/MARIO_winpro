#pragma once

#include "../Monster.h"

class Turtle : public Monster {
public:
    enum class TurtleState { NORMAL, SHELL, SPINNING };

    Turtle(MonsterType type, int x, int y, int width, int height);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;
    void takeDamage(GameWorld& world, int damage) override;

    TurtleState getState() const;
    void setState(TurtleState state);

protected:
    TurtleState m_state;
    DWORD m_shellTimer;
};