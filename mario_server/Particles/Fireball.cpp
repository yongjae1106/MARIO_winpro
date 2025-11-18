#include "Fireball.h"
#include "../GameWorld.h"

Fireball::Fireball(int x, int y, int vx)
    : Particle(ParticleType::EnemyFireball, x, y) {
    setVx(vx);
}

void Fireball::update(GameWorld& world) {
    x += vx;
    y += vy;
}

void Fireball::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}
