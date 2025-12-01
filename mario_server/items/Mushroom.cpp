#include "Mushroom.h"
#include "../GameWorld.h"

Mushroom::Mushroom(int x, int y)
    : Item(ItemType::Mushroom, x, y) {
    setVx(1);
}

void Mushroom::update(GameWorld& world) {
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

    // Collision with tiles is handled by GameWorld
    // Gravity is also handled by GameWorld
}

void Mushroom::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}