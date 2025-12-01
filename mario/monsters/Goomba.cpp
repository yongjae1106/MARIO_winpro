#include "Goomba.h"
#include "../GameWorld.h"

Goomba::Goomba(MonsterType type, int x, int y, int width, int height)
    : Monster(type, x, y, width, height) {
    // Constructor for Goomba. The client-side object is simple,
    // as most logic is handled by the server.
}

// Server-side logic like monster_logic and takeDamage is removed from the client.
// The client's Goomba will be updated via updateStateFromServer from the Monster base class.
