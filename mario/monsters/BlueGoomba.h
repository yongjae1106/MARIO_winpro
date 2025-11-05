#pragma once

#include "Goomba.h"

class BlueGoomba : public Goomba {
public:
    BlueGoomba(int x, int y);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;

private:
    int m_jumpTimer;
};
