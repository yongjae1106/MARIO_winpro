#include "Turtle.h"
#include "../GameWorld.h"

Turtle::Turtle(MonsterType type, int x, int y, int width, int height)
    : Monster(type, x, y, width, height), m_state(TurtleState::NORMAL), m_shellTimer(0) {
}

void Turtle::update(GameWorld& world) {
    // Base class update logic
}

void Turtle::render(HDC hdc, int cameraX) {
    // Base class render logic
}

void Turtle::takeDamage(GameWorld& world, int damage) {
    if (m_state == TurtleState::NORMAL) {
        m_state = TurtleState::SHELL;
        setVx(0);
        m_shellTimer = GetTickCount();
    } else if (m_state == TurtleState::SHELL) {
        m_state = TurtleState::SPINNING;
        // Set vx based on player's position, which should be handled in GameWorld
    } else if (m_state == TurtleState::SPINNING) {
        m_state = TurtleState::SHELL;
        setVx(0);
    }
}

Turtle::TurtleState Turtle::getState() const {
    return m_state;
}

void Turtle::setState(TurtleState state) {
    m_state = state;
}