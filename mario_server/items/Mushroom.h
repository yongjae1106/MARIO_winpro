#pragma once

#include "Item.h"

class Mushroom : public Item {
public:
    Mushroom(int x, int y);
    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;


};
