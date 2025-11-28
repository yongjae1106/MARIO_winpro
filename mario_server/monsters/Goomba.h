#pragma once

#include "../Monster.h"

class Goomba : public Monster {
public:
    Goomba(MonsterType type, int x, int y, int width, int height);

    void monster_logic(GameWorld& world) override;
    void takeDamage(GameWorld& world, int damage) override;
};