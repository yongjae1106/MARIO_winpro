#include "Tino.h"
#include "../GameWorld.h"

Tino::Tino(int x, int y)
    : Item(ItemType::Tino, x, y) {
    setVx(0);
    setVy(0);
}

void Tino::update(GameWorld& world) {
    if (motion) {
        y -= 2;
        spawn_motion++;
        if (spawn_motion == 18) {
            motion = false;
            active = true;
            spawn_motion = 0;
        }
    }

    if (!active) return;

    // Animation logic now handled globally
}

void Tino::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}