#pragma once

#include "Goomba.h"

class NormalGoomba : public Goomba {
public:
    NormalGoomba(int x, int y);

    void monster_logic(GameWorld& world) override;
};
