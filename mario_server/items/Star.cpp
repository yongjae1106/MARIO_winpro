#include "Star.h"
#include "../GameWorld.h"

Star::Star(int x, int y)
    : Item(ItemType::Star, x, y) {
    setVx(2);
    setVy(-10);
}

void Star::update(GameWorld& world) {
    if (motion) {
        y -= 3;
        spawn_motion++;
        if (spawn_motion == 18) {
            motion = false;
            active = true;
            spawn_motion = 0;
        }
    }

    if (!active) return;

    x += vx;
    y += vy;

    // Animation logic now handled globally
}

void Star::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}