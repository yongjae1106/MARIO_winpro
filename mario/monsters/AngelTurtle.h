#pragma once

#include "Turtle.h"

class AngelTurtle : public Turtle {
public:
    AngelTurtle(int x, int y);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;
    void takeDamage(GameWorld& world, int damage) override;

private:
    int m_topY;
    int m_bottomY;
    bool m_goingUp;
};