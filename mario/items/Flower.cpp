#include "Flower.h"
#include "../GameWorld.h"

Flower::Flower(int x, int y)
    : Item(ItemType::Flower, x, y) {
    setVx(0);
    setVy(0);
}

void Flower::update(GameWorld& world) {
    if (motion) 
    {
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

void Flower::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}