#pragma once

#include "../Monster.h"

class Goomba : public Monster {
public:
    Goomba(MonsterType type, int x, int y, int width, int height);

    void update(GameWorld& world) override = 0;
    void render(HDC hdc, int cameraX) override = 0;
    void takeDamage(GameWorld& world, int damage) override;
};