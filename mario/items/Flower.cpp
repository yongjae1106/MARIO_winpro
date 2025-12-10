#include "Flower.h"
#include "../GameWorld.h"

Flower::Flower(int x, int y)
    : Item(ItemType::Flower, x, y) {
    setVx(0);
    setVy(0);
}

void Flower::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}