#include "Star.h"
#include "../GameWorld.h"

Star::Star(int x, int y)
    : Item(ItemType::Star, x, y) {
    setVx(2);
    setVy(-10);
}

void Star::render(HDC hdc, int cameraX) {
    // Rendering will be handled by GameRender
}