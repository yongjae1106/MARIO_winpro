#pragma once

#define _WINSOCKAPI_ // Prevent windows.h from including winsock.h
#include <windows.h>
#include "Particle.h"

class TinoFireball : public Particle {
public:
    TinoFireball(int x, int y, int vx, int direction);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;

    int getMotion() const { return motion; }
    int getDirection() const { return direction; }

private:
    int motion;
    int duration;
    int direction;
};
