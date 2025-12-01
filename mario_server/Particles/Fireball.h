#pragma once

#include "../items/Item.h"

class Fireball : public Item {
public:
    Fireball(int x, int y, int vx);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;
};
