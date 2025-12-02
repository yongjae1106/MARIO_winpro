#include "Mushroom.h"
#include "../GameWorld.h"

Mushroom::Mushroom(int x, int y)
    : Item(ItemType::Mushroom, x, y) {
    setVx(1);
}

void Mushroom::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}