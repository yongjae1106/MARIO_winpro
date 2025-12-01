#include "Turtle.h"
#include "../GameWorld.h"

Turtle::Turtle(MonsterType type, int x, int y, int width, int height)
    : Monster(type, x, y, width, height), m_state(TurtleState::NORMAL) {
    // Initial state is set by the server.
}

void Turtle::updateStateFromServer(const MonsterDataPacket& packet) {
    // Call base class implementation first to update common fields
    Monster::updateStateFromServer(packet);

    // Update Turtle-specific state
    m_state = static_cast<TurtleState>(packet.turtleState);
    // Other specific state if any
}

Turtle::TurtleState Turtle::getState() const {
    return m_state;
}

void Turtle::setState(TurtleState state) {
    m_state = state;
}
