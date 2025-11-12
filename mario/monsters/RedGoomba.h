#pragma once

#include "Goomba.h"

class RedGoomba : public Goomba {
public:
    RedGoomba(int x, int y);

    void monster_logic(GameWorld& world) override;
};
