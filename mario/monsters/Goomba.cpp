#include "Goomba.h"
#include "../GameWorld.h"

Goomba::Goomba(MonsterType type, int x, int y, int width, int height)
    : Monster(type, x, y, width, height) {
}

void Goomba::takeDamage(GameWorld& world, int damage) {
    setAlive(false);
}