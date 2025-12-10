#include "Tino.h"
#include "../GameWorld.h"

Tino::Tino(int x, int y)
    : Item(ItemType::Tino, x, y) {
    setVx(0);
    setVy(0);
}

//void Tino::render(HDC hdc, int cameraX) {
//    // Rendering will be handled by GameRender
//}