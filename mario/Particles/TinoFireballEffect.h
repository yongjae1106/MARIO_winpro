#pragma once

#include "Particle.h"

class TinoFireballEffect : public Particle 
{
public:
    TinoFireballEffect(int x, int y, int vx, int direction);

    void update(GameWorld& world) override;
    //void render(HDC hdc, int cameraX) override;

    int getTimer() const { return timer; }

private:
    int timer;
};
