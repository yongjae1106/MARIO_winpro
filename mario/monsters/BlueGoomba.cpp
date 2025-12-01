#include "BlueGoomba.h"
#include "../GameWorld.h"

BlueGoomba::BlueGoomba(int x, int y)
    : Goomba(MonsterType::BlueGoomba, x, y, 40, 40) {
    // The initial state, including velocity and jump timers, is set by the server.
}

// The monster_logic function is server-side and has been removed from the client.