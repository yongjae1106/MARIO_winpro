#pragma once

#include "Monster.h"

class Goomba : public Monster {
public:
    Goomba(MonsterType type, int x, int y, int width, int height);

    // Client-side Goomba has no specific logic, it just uses the base Monster class methods.
    // Its unique behavior (like how it's drawn) will be handled by the renderer based on its type.
};
