#pragma once

#include "Goomba.h"

class BlueGoomba : public Goomba {
public:
    BlueGoomba(int x, int y);

    void monster_logic(GameWorld& world) override;
private:
    int m_jumpTimer;
};
