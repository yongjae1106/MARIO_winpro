#include "Bowser.h"
#include "../GameWorld.h" // For GameWorld::getInstance() if needed, but not directly for Bowser logic

using namespace Gdiplus;

Bowser::Bowser(int x, int y)
    : Monster(MonsterType::Bowser, x, y, 120, 120), m_isFiring(false) {
    // Constructor for client-side Bowser.
    // Initial state, including 'isFiring', is set by the server via updateStateFromServer.
}

void Bowser::updateStateFromServer(const MonsterDataPacket& packet) {
    // Call base class implementation first to update common fields
    Monster::updateStateFromServer(packet);

    // Update Bowser-specific state
    m_isFiring = packet.isFiring;
    // No other client-side specific state for Bowser currently.
}

// Monster base class handles getters for x, y, width, height, alive, walk_motion, direction
// The m_isFiring getter is in Bowser.h