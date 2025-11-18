#include "GreenTurtle.h"
#include "../GamePhysics.h"

GreenTurtle::GreenTurtle(int x, int y)
    : Turtle(MonsterType::GreenTurtle, x, y, 40, 60) {
    setVx(-1);
}
