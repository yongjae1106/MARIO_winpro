#pragma once

#include "Particle.h"

class Fireball : public Particle {
public:
    Fireball(int x, int y, int vx);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;
};
