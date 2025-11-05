#pragma once

#include "Turtle.h"

class GreenTurtle : public Turtle {
public:
    GreenTurtle(int x, int y);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;
};
