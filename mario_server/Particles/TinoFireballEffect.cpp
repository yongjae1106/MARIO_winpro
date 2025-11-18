#include "TinoFireballEffect.h"
#include "../GamePhysics.h"

TinoFireballEffect::TinoFireballEffect(int x, int y, int vx, int direction)
    : Particle(ParticleType::TinoFireballEffect, x, y)
{
    this->vx = vx;
    this->vy = 0;
    this->width = 32;
    this->height = 32;
    this->timer = 0;
    this->active = true;
}

void TinoFireballEffect::update(GameWorld& world) {
    if (!isActive()) return;

    timer++;
    if (timer >= 15) { // Lasts for 15 frames
        setActive(false);
    }
}

void TinoFireballEffect::render(HDC hdc, int cameraX) {
    // Rendering is handled by GameRender
}