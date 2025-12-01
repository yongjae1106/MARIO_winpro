#include "GreenTurtle.h"
#include "../GameWorld.h"

GreenTurtle::GreenTurtle(int x, int y)
    : Turtle(MonsterType::GreenTurtle, x, y, 40, 60) {
    // Initial state is set by the server.
}