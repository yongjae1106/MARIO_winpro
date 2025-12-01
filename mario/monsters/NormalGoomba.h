#pragma once

#include "Goomba.h"

class NormalGoomba : public Goomba {
public:
    NormalGoomba(int x, int y);

    // Client-side NormalGoomba has no specific logic, it inherits from Goomba.
};