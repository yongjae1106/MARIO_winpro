#pragma once

#include "Item.h"

class Tino : public Item {
public:
    Tino(int x, int y);
    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;
    // getAnimationFrame() and animation members removed


};
