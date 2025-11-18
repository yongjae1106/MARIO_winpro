#pragma once

#include "Item.h"

class Flower : public Item {
public:
    Flower(int x, int y);
    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;
    // getAnimationFrame() and animation members removed


};
