#pragma once

#include "../Item.h"

class PlayerFireball : public Item {
public:
    PlayerFireball(int x, int y, int vx);

    void update(GameWorld& world) override;
    void render(HDC hdc, int cameraX) override;
};
