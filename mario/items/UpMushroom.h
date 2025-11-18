#pragma once

#include "Item.h"

class UpMushroom : public Item {
public:
    UpMushroom(int x, int y);
    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;


};
