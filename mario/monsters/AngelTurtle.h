#pragma once

#include "Turtle.h"

class AngelTurtle : public Turtle {
public:
    AngelTurtle(int x, int y);

    // Client-side AngelTurtle has no specific logic, it inherits from Turtle.
    // Its unique behavior (like how it's drawn) will be handled by the renderer based on its type.
    // updateStateFromServer is handled by the base Turtle class for common fields and m_state.
};
