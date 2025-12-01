#include "NormalGoomba.h"
#include "../GameWorld.h"

NormalGoomba::NormalGoomba(int x, int y)
    : Goomba(MonsterType::NormalGoomba, x, y, 40, 40) {
    // The initial state, including velocity, is set by the server.
}

// The monster_logic function is server-side and has been removed from the client.