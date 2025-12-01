#include "RedGoomba.h"
#include "../GameWorld.h"

RedGoomba::RedGoomba(int x, int y)
    : Goomba(MonsterType::RedGoomba, x, y, 40, 40) {
    // The initial state, including velocity, is set by the server.
}

// The monster_logic function is server-side and has been removed from the client.