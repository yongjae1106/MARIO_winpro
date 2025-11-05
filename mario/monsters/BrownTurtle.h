#pragma once

#include "Turtle.h"

class BrownTurtle : public Turtle {
public:
    BrownTurtle(int x, int y);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;
};
