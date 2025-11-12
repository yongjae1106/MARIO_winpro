#include "BrownTurtle.h"
#include "../GameWorld.h"

BrownTurtle::BrownTurtle(int x, int y)
    : Turtle(MonsterType::BrownTurtle, x, y, 40, 60) {
    setVx(-1);
}
