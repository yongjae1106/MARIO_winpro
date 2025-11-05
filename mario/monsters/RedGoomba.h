#pragma once

#include "Goomba.h"

class RedGoomba : public Goomba {
public:
    RedGoomba(int x, int y);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;
};
