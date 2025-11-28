#pragma once

#include "Particles/Particle.h"

class PlayerFireball : public Particle {
public:
    PlayerFireball(int x, int y, int vx);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;

    bool isFading() const { return fade; }
    void setFade(bool isFading) { fade = isFading; }
    int getMotion() const { return motion; }

private:
    int motion;
    bool fade;
    int fade_timer;
};
