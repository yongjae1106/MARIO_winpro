#include "UpMushroom.h"
#include "../GameWorld.h"

UpMushroom::UpMushroom(int x, int y)
    : Item(ItemType::UpMushroom, x, y) {
    setVx(1);
}

void UpMushroom::update(GameWorld& world) {
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

void UpMushroom::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}