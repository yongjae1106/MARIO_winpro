#include "PlayerFireball.h"
#include "../GameWorld.h"

PlayerFireball::PlayerFireball(int x, int y, int vx)
    : Item(ItemType::Fireball, x, y) {
    setVx(vx);
    setWidth(20);
    setHeight(20);
}

void PlayerFireball::update(GameWorld& world) {
    x += vx;
    y += vy;

    // Fireball disappears after a certain distance or collision
    // Collision with tiles and monsters will be handled by GameWorld
}

void PlayerFireball::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}
