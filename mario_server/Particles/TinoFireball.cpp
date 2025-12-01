#include "TinoFireball.h"
#include "../GameWorld.h"

TinoFireball::TinoFireball(int x, int y, int vx, int direction)
    : Particle(ParticleType::TinoFireball, x, y), motion(0), duration(0), direction(direction) {
    setVx(vx);
    setWidth(TILE_SIZE * 2); // TinoFireball is TILE_SIZE * 2
    setHeight(TILE_SIZE * 2);
    setActive(true); // Make it active on creation
}

void TinoFireball::update(GameWorld& world) {
    if (!isActive()) return;

    // Animation motion (7 frames: 0-6)
    motion++;
    if (motion > 6) {
        motion = 0;
    }

    // Duration-based lifespan
    duration++;
    if (duration >= 100) { // Lasts for 100 frames
        setActive(false);
        return;
    }

    // Horizontal movement (no gravity or vertical movement)
    setX(getX() + getVx());

    // No tile collision for TinoFireball (it passes through)
    // Check if it went off-screen
    if (getX() < world.getCameraX() - getWidth() || getX() > world.getCameraX() + SCREEN_WIDTH) {
        setActive(false);
    }
}

void TinoFireball::render(HDC hdc, int cameraX) {
    // Rendering is handled by GameRender
}