#pragma once

#include "Goomba.h"

class NormalGoomba : public Goomba {
public:
    NormalGoomba(int x, int y);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;
};
