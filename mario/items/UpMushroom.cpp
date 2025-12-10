#include "UpMushroom.h"
#include "../GameWorld.h"

UpMushroom::UpMushroom(int x, int y)
    : Item(ItemType::UpMushroom, x, y) {
    setVx(1);
}

//void UpMushroom::render(HDC hdc, int cameraX) {
//    // Rendering will be handled by GameRender
//}