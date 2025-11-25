#include "AngelTurtle.h"
#include "../GameWorld.h"
// #include "GreenTurtle.h" // Might not be needed if GreenTurtle isn't directly used here.

AngelTurtle::AngelTurtle(int x, int y)
    : Turtle(MonsterType::AngelTurtle, x, y, 40, 50) {
    // Constructor for client-side AngelTurtle.
    // Its state, including movement pattern, is driven by the server via updateStateFromServer.
}

// Client-side AngelTurtle inherits updateStateFromServer from the base Turtle class.
// If AngelTurtle has specific client-side state not covered by Turtle's updateStateFromServer,
// this function should be overridden here. For now, it doesn't seem to have unique client-side
// fields that need direct packet parsing beyond what MonsterDataPacket already provides or
// what Turtle handles.
